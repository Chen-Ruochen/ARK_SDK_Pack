/**********************************
 * @Description  : RCP协议头文件
 * @Author       : Jason Chen
 * @Date         : 2024-12-18 11:38:47
 * @FilePath     : protocol_rcp.h
 * @Version      : V1.2
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
#ifndef __PROTOCOL_RCP_H
#define __PROTOCOL_RCP_H

#include "common_lib.h"
#include "ring_fifo.h"
#include "QFsm.h"

#ifdef __cplusplus
extern "C" {
#endif

/* RCP版本号 */
#ifndef RCP_VERSION
#define RCP_VERSION             (0)
#endif
/* RCP帧头 默认0x5A */
#ifndef RCP_FRAME_STX
#define RCP_FRAME_STX           (0x5A)
#endif
/* RCP发送缓冲大小 */
#ifndef RCP_TX_FIFO_SIZE
#define RCP_TX_FIFO_SIZE        (2048)
#endif
/* RCP接收缓冲大小 */
#ifndef RCP_RX_FIFO_SIZE
#define RCP_RX_FIFO_SIZE        (2048)
#endif
/* RCP单次发送大小 */
#ifndef RCP_TX_MAX_SIZE
#define RCP_TX_MAX_SIZE         (300)
#endif
/* RCP超时时间ms */
#ifndef RCP_TIMEOUT
#define RCP_TIMEOUT             (3000)
#endif


typedef enum rcp_err_e {
    RCP_OK = 0,
    RCP_ERROR,
    RCP_NOINIT,
    RCP_FIFO_EMPTY,
    RCP_FIFO_FULL,
} rcp_err_t;

typedef enum rcp_code_e {
    RCP_CODE_NONE             = 0x00,
    RCP_CODE_CMD_INVALID      = 0xE0,
    RCP_CODE_PARAM_INVALID    = 0xE1,
    RCP_CODE_CRC_ERROR        = 0xE2,
    RCP_CODE_RW_ERROR         = 0xE3,
    RCP_CODE_UNKNOWN_ERROR    = 0xE4,
    RCP_CODE_DIR_ERROR        = 0xE5,
    RCP_CODE_TIMEOUT          = 0xFF,
} rcp_code_t;

typedef enum rcp_role_e {
    RCP_ROLE_MASTER = 0,
    RCP_ROLE_SLAVE,
} rcp_role_t;

typedef enum rcp_rwx_e {
    RCP_WRITE = 0,
    RCP_READ,
    RCP_RW_NONE
} rcp_rwx_t;

/* RCP标志位 */
typedef struct rcp_flag_s {
    uint8_t rw      : 1;    /* 读写标志位 */
    uint8_t dir     : 1;    /* 方向标志位 */
    uint8_t eof     : 1;    /* 结束标志位 */
    uint8_t fid     : 1;    /* 帧ID标志位 */
    uint8_t vlen    : 1;    /* 变长标志位 */
    uint8_t rwx     : 1;    /* 读写使能标志位 */
    uint8_t ver     : 2;    /* 版本标志位 */
} rcp_flag_t;

/* RCP帧结构 */
typedef struct rcp_hdr_s {
    uint8_t     stx;        /* 帧头 */
    uint8_t     len;        /* 数据长度 */
    rcp_flag_t  flag;       /* 标志位 */
    uint8_t     cmd;        /* 命令字 */
} rcp_hdr_t;

/* 命令列表 */
typedef struct rcp_list_s {
    uint8_t     cmd;        /* 命令字 */
    void        (*callback)(bool eof, uint8_t *data, uint16_t len); /* 命令处理函数 */
} rcp_list_t;

/* 协议结构体 */
typedef struct rcp_protocol_s {
    QFsm        fsm;            /* 状态机 必须放在结构体首位 */
    rcp_role_t  role;           /* 上位机/下位机 */
    rcp_hdr_t   hdr;            /* 帧结构体 */
    uint32_t    total_len;      /* 帧长度 */
    uint32_t    payload_len;    /* 数据长度 */
    rcp_list_t  *public_list;   /* 公共命令列表 */
    uint16_t    pub_num;        /* 公共命令列表大小 */
    rcp_list_t  *private_list;  /* 私有命令列表 */
    uint16_t    pri_num;        /* 私有命令列表大小 */
    bool        is_init;        /* 协议初始化标志 */
    ring_fifo_t tx_fifo;        /* 发送缓冲区 */
    ring_fifo_t rx_fifo;        /* 接收缓冲区 */
    uint8_t     tx_buf[RCP_TX_FIFO_SIZE]; /* 发送缓冲区数据 */
    uint8_t     rx_buf[RCP_RX_FIFO_SIZE]; /* 接收缓冲区数据 */
    uint8_t     temp_buf[RCP_RX_FIFO_SIZE]; /* 临时缓冲区数据 */
    uint32_t    cur_time;       /* 当前时间 */
    uint32_t    old_time;       /* 上次时间 */
    rcp_code_t  err_code;       /* 错误代码 */
    uint8_t     running;        /* 运行标志 */
} rcp_protocol_t;

rcp_err_t rcp_api_init( rcp_protocol_t *protocol,
                        rcp_list_t *public_list,
                        uint16_t pub_num,
                        rcp_list_t *private_list,
                        uint16_t pri_num,
                        rcp_role_t role);
rcp_err_t rcp_api_deinit(rcp_protocol_t *protocol);
rcp_err_t rcp_api_rx_fifo_push(rcp_protocol_t *protocol, uint8_t *data, uint16_t len);
rcp_err_t rcp_api_tx_fifo_push(rcp_protocol_t *protocol, uint8_t cmd, rcp_rwx_t rwx, uint8_t *data, uint16_t len);
rcp_err_t rcp_api_tx_fifo_pop(rcp_protocol_t *protocol, uint8_t *pkt, uint16_t *len);
void rcp_api_state_machine(rcp_protocol_t *protocol, uint32_t tick);
uint8_t rcp_api_get_state(rcp_protocol_t *protocol);

#ifdef __cplusplus
}
#endif

#endif