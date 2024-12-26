/**********************************
 * @Description  : 环形缓冲区源文件
 * @Author       : Jason Chen
 * @Date         : 2024-12-10 14:38:40
 * @FilePath     : module_ring_fifo.c
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
#include "module_ring_fifo.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

/**********************************
 * @brief 判断一个数是否是2的幂
 * @param num 待判断的数
 * @return 1 是2的幂 0 不是2的幂
 * @note
 **********************************/
static uint8_t is_power_of_two(uint32_t num)
{
    return (num != 0) && ((num & (num - 1)) == 0);
}

/**********************************
 * @brief 初始化环形缓冲区
 * @param *fifo 环形缓冲区结构体指针
 * @param *buffer 缓冲区指针
 * @param size 缓冲区大小
 * @return 1 初始化成功 0 初始化失败
 * @note
 **********************************/
uint8_t ring_fifo_init(ring_fifo_t *fifo, uint8_t *buffer, uint32_t size)
{
    if (!is_power_of_two(size)) {
        return 0;
    }

    fifo->buffer = buffer;
    fifo->size = size;
    fifo->in = 0;
    fifo->out = 0;

    return 1;
}

/**********************************
 * @brief 向环形缓冲区写入数据
 * @param *fifo 环形缓冲区结构体指针
 * @param *buffer 待写入数据指针
 * @param len 待写入数据长度
 * @return 实际写入数据长度
 * @note
 **********************************/
uint32_t ring_fifo_put(ring_fifo_t *fifo, uint8_t *buffer, uint32_t len)
{
    uint32_t l;

    len = MIN(len, fifo->size - fifo->in + fifo->out);
    l = MIN(len, fifo->size - (fifo->in & (fifo->size - 1)));

    memcpy(fifo->buffer + (fifo->in & (fifo->size - 1)), buffer, l);
    memcpy(fifo->buffer, buffer + l, len - l);

    fifo->in += len;

    return len;
}

/**********************************
 * @brief 从环形缓冲区读取数据
 * @param *fifo 环形缓冲区结构体指针
 * @param *buffer 读取数据存放指针
 * @param len 读取数据长度
 * @return 实际读取数据长度
 * @note
 **********************************/
uint32_t ring_fifo_get(ring_fifo_t *fifo, uint8_t *buffer, uint32_t len)
{
    uint32_t l;

    len = MIN(len, fifo->in - fifo->out);
    l = MIN(len, fifo->size - (fifo->out & (fifo->size - 1)));

    memcpy(buffer, fifo->buffer + (fifo->out & (fifo->size - 1)), l);
    memcpy(buffer + l, fifo->buffer, len - l);

    fifo->out += len;

    return len;
}

/**********************************
 * @brief 获取环形缓冲区长度
 * @param *fifo 环形缓冲区结构体指针
 * @return 环形缓冲区长度
 * @note
 **********************************/
uint32_t ring_fifo_len(ring_fifo_t *fifo)
{
    return (fifo->in - fifo->out);
}

/**********************************
 * @brief 判断环形缓冲区是否为空
 * @param *fifo 环形缓冲区结构体指针
 * @return 1 空 0 非空
 * @note
 **********************************/
uint8_t ring_fifo_is_empty(ring_fifo_t *fifo)
{
    return (fifo->in == fifo->out);
}

/**********************************
 * @brief 判断环形缓冲区是否已满
 * @param *fifo 环形缓冲区结构体指针
 * @return 1 满 0 非满
 * @note
 **********************************/
uint8_t ring_fifo_is_full(ring_fifo_t *fifo)
{
    return (fifo->in - fifo->out) == fifo->size;
}

/**********************************
 * @brief 获取环形缓冲区已存储数据大小
 * @param *fifo 环形缓冲区结构体指针
 * @return 已存储数据大小
 * @note
 **********************************/
uint16_t ring_fifo_stored(ring_fifo_t *fifo)
{
    return (fifo->in - fifo->out);
}

/**********************************
 * @brief 获取环形缓冲区可用空间
 * @param *fifo 环形缓冲区结构体指针
 * @return 可用空间大小
 * @note
 **********************************/
uint16_t ring_fifo_available(ring_fifo_t *fifo)
{
    return (fifo->size - (fifo->in - fifo->out));
}

/**********************************
 * @brief 重置环形缓冲区
 * @param *fifo 环形缓冲区结构体指针
 * @return 1 重置成功 0 重置失败
 * @note
 **********************************/
void ring_fifo_reset(ring_fifo_t *fifo)
{
    memset(fifo->buffer, 0, fifo->size);
    fifo->in = 0;
    fifo->out = 0;
}
