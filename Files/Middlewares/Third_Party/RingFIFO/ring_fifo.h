/**********************************
 * @Description  : 环形缓冲区头文件
 * @Author       : Jason Chen
 * @Date         : 2024-12-10 14:38:46
 * @FilePath     : ring_fifo.h
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
#ifndef __RING_FIFO_H
#define __RING_FIFO_H
#include "common_lib.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct ring_fifo_s {
    uint8_t *buffer;
    uint32_t size;
    uint32_t in;
    uint32_t out;
} ring_fifo_t;


uint8_t ring_fifo_init(ring_fifo_t *fifo, uint8_t *buffer, uint32_t size);
uint32_t ring_fifo_put(ring_fifo_t *fifo, uint8_t *buffer, uint32_t len);
uint32_t ring_fifo_get(ring_fifo_t *fifo, uint8_t *buffer, uint32_t len);
uint32_t ring_fifo_len(ring_fifo_t *fifo);
uint8_t ring_fifo_is_empty(ring_fifo_t *fifo);
uint8_t ring_fifo_is_full(ring_fifo_t *fifo);
uint16_t ring_fifo_stored(ring_fifo_t *fifo);
uint16_t ring_fifo_available(ring_fifo_t *fifo);
void ring_fifo_reset(ring_fifo_t *fifo);


#ifdef __cplusplus
}
#endif

#endif /* __RING_FIFO_H */