/**********************************
 * @Description  : RCP协议源文件
 * @Author       : Jason Chen
 * @Date         : 2024-12-10 10:07:36
 * @FilePath     : protocol_rcp.c
 * @Version      : V1.2
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
#include "protocol_rcp.h"
#include "crc16_XMODEM.h"


#define RCP_IS_TIMEOUT ((me->cur_time - me->old_time > RCP_TIMEOUT) ? true : false)
#define RCP_VLEN1_SIZE (sizeof(rcp_hdr_t) + sizeof(uint32_t) + sizeof(uint16_t))
#define RCP_VLEN0_SIZE (sizeof(rcp_hdr_t) + sizeof(uint16_t))


typedef enum rcp_state_e {
    RCP_FIFO_PROCESS = Q_USER_SIG,
    RCP_FIFO_WAIT
} rcp_state_t;

static QState rcp_state_init(rcp_protocol_t *me, QEvent const *e);
static QState rcp_state_get_stx(rcp_protocol_t *me, QEvent const *e);
static QState rcp_state_get_len_flag_cmd(rcp_protocol_t *me, QEvent const *e);
static QState rcp_state_get_payload_len(rcp_protocol_t* me, QEvent const* e);
static QState rcp_state_get_data_crc(rcp_protocol_t *me, QEvent const *e);
static QState rcp_state_complete(rcp_protocol_t *me, QEvent const *e);
static QState rcp_state_error(rcp_protocol_t *me, QEvent const *e);
static QState rcp_state_sleep(rcp_protocol_t *me, QEvent const *e);
static rcp_code_t rcp_call_list_cb(rcp_protocol_t *me);
static rcp_code_t rcp_flag_error_check(rcp_protocol_t *me);
static void rcp_clear_frame_data(rcp_protocol_t *me);


/**********************************
 * @brief RCP协议初始化
 * @param *protocol RCP协议句柄
 * @param *public_list 公共命令列表
 * @param pub_num 公共命令列表大小
 * @param *private_list 私有命令列表
 * @param pri_num 私有命令列表大小
 * @return RCP状态码
 * @note
 **********************************/
rcp_err_t rcp_api_init( rcp_protocol_t *protocol,
                        rcp_list_t *public_list,
                        uint16_t pub_num,
                        rcp_list_t *private_list,
                        uint16_t pri_num,
                        rcp_role_t role)
{
    if (protocol == NULL) {
        return RCP_ERROR;
    }
    if (public_list == NULL && private_list == NULL) {
        return RCP_ERROR;
    }

    //FIFO初始化
    if (!ring_fifo_init(&protocol->tx_fifo, protocol->tx_buf, RCP_TX_FIFO_SIZE)) {
        return RCP_ERROR;
    }

    if (!ring_fifo_init(&protocol->rx_fifo, protocol->rx_buf, RCP_RX_FIFO_SIZE)) {
        return RCP_ERROR;
    }

    //状态机初始化
    QFsm_ctor(&protocol->fsm, (QStateHandler)&rcp_state_init);

    protocol->public_list = public_list;
    protocol->pub_num = pub_num;
    protocol->private_list = private_list;
    protocol->pri_num = pri_num;
    protocol->role = role;
    protocol->is_init = true;

    return RCP_OK;
}

/**********************************
 * @brief RCP协议反初始化
 * @param *protocol RCP协议句柄
 * @return RCP状态码
 * @note
 **********************************/
rcp_err_t rcp_api_deinit(rcp_protocol_t *protocol)
{
    if (protocol == NULL) {
        return RCP_ERROR;
    }

    //FIFO复位
    ring_fifo_reset(&protocol->tx_fifo);
    ring_fifo_reset(&protocol->rx_fifo);

    protocol->public_list = NULL;
    protocol->private_list = NULL;
    protocol->is_init = false;

    return RCP_OK;
}

/**********************************
 * @brief RCP协议接收FIFO数据入队
 * @param *protocol RCP协议句柄
 * @param *data 数据指针
 * @param len 数据长度
 * @return RCP状态码
 * @note
 **********************************/
rcp_err_t rcp_api_rx_fifo_push(rcp_protocol_t *protocol, uint8_t *data, uint16_t len)
{
    if (!protocol->is_init) {
        return RCP_NOINIT;
    }

    //数据入FIFO
    //判断FIFO是否已满 以及 FIFO剩余空间是否足够
    if (ring_fifo_available(&protocol->rx_fifo) >= len) {
        ring_fifo_put(&protocol->rx_fifo, data, len);
    } else {
        return RCP_FIFO_FULL;
    }

    return RCP_OK;
}

/**********************************
 * @brief RCP协议发送FIFO数据入队
 * @param *protocol RCP协议句柄
 * @param cmd 命令字
 * @param rwx 读写标志
 * @param *data 数据指针
 * @param *len 数据长度
 * @return RCP状态码
 * @note
 **********************************/
rcp_err_t rcp_api_tx_fifo_push(rcp_protocol_t *protocol, uint8_t cmd, rcp_rwx_t rwx, uint8_t *data, uint16_t len)
{
    if (!protocol->is_init) {
        return RCP_NOINIT;
    }
    uint8_t tx_buf[RCP_TX_MAX_SIZE];
    uint32_t payload_len = 0;
    uint32_t total_len = 0;

    rcp_hdr_t *hdr = (rcp_hdr_t *)tx_buf;
    memset(tx_buf, 0, sizeof(tx_buf));

    while (len >= 0) {
        /* 1.帧头 */
        hdr->stx = RCP_FRAME_STX;
        /* 2.读写标志位 */
        switch (rwx) {
            case RCP_WRITE:
                hdr->flag.rw = 1;
                hdr->flag.rwx = 0;
                break;
            case RCP_READ:
                hdr->flag.rw = 0;
                hdr->flag.rwx = 0;
                break;
            case RCP_RW_NONE:
                hdr->flag.rw = 0;
                hdr->flag.rwx = 1;
                break;
            default:
                break;
        }
        /* 方向标志位及协议版本 */
        if (protocol->role == RCP_ROLE_MASTER) {
            //上位机
            hdr->flag.dir = 1;
            hdr->flag.ver = 0;
        } else {
            //下位机
            hdr->flag.dir = 0;
            hdr->flag.ver = RCP_VERSION;
        }
        /* 命令 */
        hdr->cmd = cmd;
        total_len += sizeof(rcp_hdr_t);
        /* 长度处理 数据拷贝 */
        payload_len = (len > (RCP_TX_MAX_SIZE - RCP_VLEN0_SIZE)) ? (RCP_TX_MAX_SIZE - RCP_VLEN0_SIZE) : (len);
        if (payload_len > 0xFF) {
            payload_len -= (payload_len > (RCP_TX_MAX_SIZE - RCP_VLEN1_SIZE)) ? (sizeof(payload_len)) : (0);
            hdr->flag.vlen = 1;
            hdr->len = 0;
            memcpy(tx_buf + sizeof(rcp_hdr_t), &payload_len, sizeof(payload_len));
            memcpy(tx_buf + sizeof(rcp_hdr_t) + sizeof(payload_len), data, payload_len);
            total_len += sizeof(payload_len) + payload_len;
        } else {
            hdr->flag.vlen = 0;
            hdr->len = payload_len;
            memcpy(tx_buf + sizeof(rcp_hdr_t), data, payload_len);
            total_len += payload_len;
        }

        /* EOF FID 结束帧处理 */
        if (payload_len == len) {
            hdr->flag.fid = 0;
            hdr->flag.eof = 1;
        } else {
            hdr->flag.fid ^= 1;
            hdr->flag.eof = 0;
        }

        /* CRC处理 */
        uint16_t crc_cal = crc16_xmodem(tx_buf, total_len);
        memcpy(tx_buf + total_len, &crc_cal, sizeof(crc_cal));
        total_len += sizeof(crc_cal);

        /* 数据入FIFO */
        if (ring_fifo_available(&protocol->tx_fifo) >= total_len) {
            ring_fifo_put(&protocol->tx_fifo, tx_buf, total_len);
        } else {
            return RCP_FIFO_FULL;
        }
        /* 剩余有效长度偏移 */
        len -= payload_len;
        data += payload_len;
        total_len = 0;
        if (len == 0) {
            break;
        }
    }

    return RCP_OK;
}

/**********************************
 * @brief RCP协议发送FIFO数据出队
 * @param *protocol RCP协议句柄
 * @param *pkt 数据指针
 * @param *len 出队的数据长度
 * @return RCP状态码
 * @note
 **********************************/
rcp_err_t rcp_api_tx_fifo_pop(rcp_protocol_t *protocol, uint8_t *pkt, uint16_t *len)
{
    if (!protocol->is_init) {
        return RCP_NOINIT;
    }

    rcp_flag_t flag;
    uint32_t payload_len = 0;
    (*len) = 0;
    /* 判断FIFO是否为空 获取一帧完整数据 */
    if (!ring_fifo_is_empty(&protocol->tx_fifo)) {
        /* 获取帧头 */
        ring_fifo_get(&protocol->tx_fifo, pkt, sizeof(protocol->hdr.stx));
        (*len)++;
        /* 获取数据长度len */
        ring_fifo_get(&protocol->tx_fifo, pkt + (*len), sizeof(protocol->hdr.len));
        uint8_t temp_len = *(pkt + 1);
        (*len)++;
        /* 获取标志位 */
        ring_fifo_get(&protocol->tx_fifo, pkt + (*len), sizeof(protocol->hdr.flag));
        flag = *(rcp_flag_t *)(pkt + (*len));
        (*len)++;
        /* 获取cmd */
        ring_fifo_get(&protocol->tx_fifo, pkt + (*len), sizeof(protocol->hdr.cmd));
        (*len)++;
        /* 判断payload长度 */
        if (flag.vlen) {
            ring_fifo_get(&protocol->tx_fifo, pkt + (*len), sizeof(payload_len));
            payload_len = *(uint32_t *)(pkt + (*len));
            (*len) += sizeof(payload_len);
        } else {
            payload_len = temp_len;
        }
        /* 获取剩余数据+crc16 */
        ring_fifo_get(&protocol->tx_fifo, pkt + (*len), payload_len + sizeof(uint16_t));
        (*len) += payload_len + sizeof(uint16_t);
    } else {
        return RCP_FIFO_EMPTY;
    }

    return RCP_OK;
}

/**********************************
 * @brief RCP协议状态机
 * @param *protocol RCP协议句柄
 * @param tick 时间递增间隔
 * @return 无
 * @note 每隔(n)ms调用一次
 **********************************/
void rcp_api_state_machine(rcp_protocol_t *protocol, uint32_t tick)
{
    if (!protocol->is_init) {
        return;
    }
    //时间递增
    protocol->cur_time += tick;

    if (!ring_fifo_is_empty(&protocol->rx_fifo)) {
        //FIFO不为空 通知状态机进行处理
        QEvent e = {RCP_FIFO_PROCESS, 0};
        QFsm_dispatch(&protocol->fsm, &e);
    } else {
        //FIFO为空 通知状态机等待
        QEvent e = {RCP_FIFO_WAIT, 0};
        QFsm_dispatch(&protocol->fsm, &e);
    }
}

/**********************************
 * @brief RCP协议获取当前状态
 * @param *protocol RCP协议句柄
 * @return 当前状态 0:休眠 1:运行
 * @note
 **********************************/
uint8_t rcp_api_get_state(rcp_protocol_t *protocol)
{
    return protocol->running;
}

static QState rcp_state_init(rcp_protocol_t *me, QEvent const *e)
{
    (void)e;
    rcp_clear_frame_data(me);
    //初始化结束 进入休眠
    return Q_TRAN(&rcp_state_sleep);
}

static QState rcp_state_get_stx(rcp_protocol_t *me, QEvent const *e)
{
    switch (e->sig) {
        case Q_ENTRY_SIG:
            //开始计时
            me->running = 1;
            me->old_time = me->cur_time;
            return Q_HANDLED();
        case RCP_FIFO_PROCESS:
            ring_fifo_get(&me->rx_fifo, &me->hdr.stx, sizeof(me->hdr.stx));
            //判断首字节是否为STX
            if (me->hdr.stx == RCP_FRAME_STX) {
                me->total_len += sizeof(me->hdr.stx);
                //正确 进入解析长度
                return Q_TRAN(&rcp_state_get_len_flag_cmd);
            } else {
                //错误 保持当前
                return Q_HANDLED();
            }
        case RCP_FIFO_WAIT:
            //当FIFO无数据且超时 状态机应进入休眠
            if (RCP_IS_TIMEOUT) {
                return Q_TRAN(&rcp_state_sleep);
            }
        default:
            return Q_IGNORED();
    }
    return Q_IGNORED();
}

static QState rcp_state_get_len_flag_cmd(rcp_protocol_t *me, QEvent const *e)
{
    switch (e->sig) {
        case Q_ENTRY_SIG:
            //开始计时
            me->old_time = me->cur_time;
            return Q_HANDLED();
        case RCP_FIFO_PROCESS:
            if (ring_fifo_stored(&me->rx_fifo) >= (sizeof(me->hdr) - 1)) {
                ring_fifo_get(&me->rx_fifo, &me->hdr.len, sizeof(me->hdr.len));
                ring_fifo_get(&me->rx_fifo, (uint8_t *)&me->hdr.flag, sizeof(me->hdr.flag));
                ring_fifo_get(&me->rx_fifo, &me->hdr.cmd, sizeof(me->hdr.cmd));
                //将帧头部分拷贝到临时缓冲区
                memcpy(me->temp_buf, &me->hdr, sizeof(me->hdr));
                //进入解析payload
                me->total_len += sizeof(me->hdr) - 1;
                return Q_TRAN(&rcp_state_get_payload_len);
            } else {
                //超时处理
                if (RCP_IS_TIMEOUT) {
                    me->err_code = RCP_CODE_TIMEOUT;
                    return Q_TRAN(&rcp_state_error);
                }
            }
        case RCP_FIFO_WAIT:
            //超时处理
            if (RCP_IS_TIMEOUT) {
                me->err_code = RCP_CODE_TIMEOUT;
                return Q_TRAN(&rcp_state_error);
            }
        default:
            return Q_IGNORED();
    }
    return Q_IGNORED();
}

static QState rcp_state_get_payload_len(rcp_protocol_t* me, QEvent const* e)
{
    switch (e->sig) {
        case Q_ENTRY_SIG:
            //开始计时
            me->old_time = me->cur_time;
            return Q_HANDLED();
        case RCP_FIFO_PROCESS:
            if (me->hdr.flag.vlen) {
                //变长长度
                //判断数据长度是否足够
                if (ring_fifo_stored(&me->rx_fifo) > sizeof(uint32_t)) {
                    ring_fifo_get(&me->rx_fifo, (uint8_t*)&me->payload_len, sizeof(me->payload_len));
                    memcpy(me->temp_buf + sizeof(me->hdr), &me->payload_len, sizeof(me->payload_len));
                    me->total_len += sizeof(me->payload_len);
                    return Q_TRAN(&rcp_state_get_data_crc);
                } else {
                    //超时处理
                    if (RCP_IS_TIMEOUT) {
                        me->err_code = RCP_CODE_TIMEOUT;
                        return Q_TRAN(&rcp_state_error);
                    }
                }
            } else {
                //固定长度
                me->payload_len = me->hdr.len;
                return Q_TRAN(&rcp_state_get_data_crc);
            }
        case RCP_FIFO_WAIT:
            //超时处理
            if (RCP_IS_TIMEOUT) {
                me->err_code = RCP_CODE_TIMEOUT;
                return Q_TRAN(&rcp_state_error);
            }
        default:
            return Q_IGNORED();
    }
    return Q_IGNORED();
}

static QState rcp_state_get_data_crc(rcp_protocol_t *me, QEvent const *e)
{
    uint16_t crc_recv = 0;
    switch (e->sig) {
        case Q_ENTRY_SIG:
            //开始计时
            me->old_time = me->cur_time;
            return Q_HANDLED();
        case RCP_FIFO_PROCESS:
            if (ring_fifo_stored(&me->rx_fifo) >= me->payload_len + sizeof(uint16_t)) {
                //将帧数据拷贝到临时缓冲区
                if (me->hdr.flag.vlen) {
                    //变长长度
                    ring_fifo_get(&me->rx_fifo, me->temp_buf + sizeof(me->hdr) + sizeof(me->payload_len), me->payload_len);
                } else {
                    //固定长度
                    ring_fifo_get(&me->rx_fifo, me->temp_buf + sizeof(me->hdr), me->payload_len);
                }
                me->total_len += me->payload_len;
                //CRC校验
                ring_fifo_get(&me->rx_fifo, (uint8_t *)&crc_recv, sizeof(crc_recv));
                //将CRC校验码拷贝到临时缓冲区
                memcpy(me->temp_buf + me->total_len, &crc_recv, sizeof(crc_recv));
                me->total_len += sizeof(crc_recv);
                uint16_t crc_cal = crc16_xmodem(me->temp_buf, me->total_len - sizeof(crc_recv));
                if (crc_cal == crc_recv) {
                    //CRC正确 进入flag错误判断
					me->err_code = rcp_flag_error_check(me);
                    if (me->err_code != RCP_CODE_NONE) {
                        return Q_TRAN(&rcp_state_error);
                    } else {
                        //flag无错误 进入命令列表回调
                        return Q_TRAN(&rcp_state_complete);
                    }
                } else {
                    //CRC错误 进入错误处理
                    me->err_code = RCP_CODE_CRC_ERROR;
                    return Q_TRAN(&rcp_state_error);
                }
            } else {
                //超时处理
                if (RCP_IS_TIMEOUT) {
                    me->err_code = RCP_CODE_TIMEOUT;
                    return Q_TRAN(&rcp_state_error);
                }
            }
        case RCP_FIFO_WAIT:
            //超时处理
            if (RCP_IS_TIMEOUT) {
                me->err_code = RCP_CODE_TIMEOUT;
                return Q_TRAN(&rcp_state_error);
            }
        default:
            return Q_IGNORED();
    }
    return Q_IGNORED();
}

static QState rcp_state_complete(rcp_protocol_t *me, QEvent const *e)
{
    switch (e->sig) {
        case Q_ENTRY_SIG:
            //命令列表回调
			me->err_code = rcp_call_list_cb(me);
            return Q_IGNORED();
        default:
            if (me->err_code != RCP_CODE_NONE) {
                return Q_TRAN(&rcp_state_error);
            } else {
                //清空上一帧数据
                rcp_clear_frame_data(me);
                //进入解析帧头
                return Q_TRAN(&rcp_state_get_stx);
            }
    }
    return Q_IGNORED();
}

static QState rcp_state_error(rcp_protocol_t *me, QEvent const *e)
{
    switch (e->sig) {
        case RCP_FIFO_PROCESS:
        case RCP_FIFO_WAIT:
            if (me->err_code == RCP_CODE_CRC_ERROR || me->err_code == RCP_CODE_TIMEOUT) {
                //CRC错误数据处理
                //踢掉buf的第一个字节
                memmove(me->temp_buf, me->temp_buf + 1, sizeof(me->temp_buf) - 1);
                //将fifo内的剩余数据copy到buf尾部
                uint32_t get_len = ring_fifo_get(&me->rx_fifo, me->temp_buf + me->total_len - 1, RCP_RX_FIFO_SIZE);
                //重新装载fifo进行解析
                ring_fifo_put(&me->rx_fifo, me->temp_buf, me->total_len - 1 + get_len);
            } else {
                if (me->role == RCP_ROLE_SLAVE) {
                    //错误不是CRC则自动装载一帧错误上报帧
                    uint8_t err_buf[2] = {0};
                    err_buf[0] = me->hdr.cmd;
                    err_buf[1] = me->err_code;
                    rcp_api_tx_fifo_push(me, 0x86, RCP_RW_NONE, err_buf, sizeof(err_buf));
                }
            }
            //清空上一帧数据
            rcp_clear_frame_data(me);
            //进入解析帧头
            return Q_TRAN(&rcp_state_get_stx);
        default:
            return Q_IGNORED();
    }
    return Q_IGNORED();
}

static QState rcp_state_sleep(rcp_protocol_t *me, QEvent const *e)
{
    switch (e->sig) {
        case Q_ENTRY_SIG:
            me->running = 0;
            return Q_HANDLED();
        case RCP_FIFO_PROCESS:
            //当FIFO有数据时 进入解析帧头
            return Q_TRAN(&rcp_state_get_stx);
        default:
            return Q_IGNORED();
    }
    return Q_IGNORED();
}

static rcp_code_t rcp_call_list_cb(rcp_protocol_t *me)
{
    rcp_code_t ret = RCP_CODE_NONE;
    //需判断是否找到对应命令字
    bool find_valid_cmd = false;
    if (me->public_list != NULL) {
        for (uint16_t i = 0; i < me->pub_num; i++) {
            if (me->hdr.cmd == me->public_list[i].cmd && me->public_list[i].callback != NULL) {
                find_valid_cmd = true;
                me->public_list[i].callback(me->hdr.flag.eof, me->temp_buf + sizeof(me->hdr) + (me->hdr.flag.vlen ? sizeof(me->payload_len) : 0), me->payload_len);
            }
        }
    }
    if (me->private_list != NULL) {
        for (uint16_t i = 0; i < me->pri_num; i++) {
            if (me->hdr.cmd == me->private_list[i].cmd && me->private_list[i].callback != NULL) {
                find_valid_cmd = true;
                me->private_list[i].callback(me->hdr.flag.eof, me->temp_buf + sizeof(me->hdr) + (me->hdr.flag.vlen ? sizeof(me->payload_len) : 0), me->payload_len);
            }
        }
    }

    if (!find_valid_cmd) {
        //未找到对应命令字
        ret = RCP_CODE_CMD_INVALID;
    }
    return ret;
}

static rcp_code_t rcp_flag_error_check(rcp_protocol_t *me)
{
    rcp_code_t ret = RCP_CODE_NONE;

    if (me->role == RCP_ROLE_MASTER) {
        //上位机
        //读写判断
        if (!me->hdr.flag.rwx) {
            ret = RCP_CODE_RW_ERROR;
        }
        //方向判断
        if (me->hdr.flag.dir) {
            ret = RCP_CODE_DIR_ERROR;
        }
    } else {
        //下位机
        //读写判断
        if (me->hdr.flag.rwx) {
            ret = RCP_CODE_RW_ERROR;
        }
        //方向判断
        if (!me->hdr.flag.dir) {
            ret = RCP_CODE_DIR_ERROR;
        }
    }

    return ret;
}

static void rcp_clear_frame_data(rcp_protocol_t *me)
{
    memset(&me->hdr, 0, sizeof(me->hdr));
    memset(&me->temp_buf, 0, sizeof(me->temp_buf));
    memset(&me->total_len, 0, sizeof(me->total_len));
    memset(&me->payload_len, 0, sizeof(me->payload_len));
    me->err_code = RCP_CODE_NONE;
}
