/**********************************
 * @Description  : CRC16校验
 * @Author       : Jason Chen
 * @Date         : 2024-12-10 16:46:01
 * @FilePath     : crc16_xmodem.c
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
#include "crc16_xmodem.h"


/**********************************
 * @brief CRC16校验
 * @param *data 数据
 * @param len 数据长度
 * @return CRC16校验值
 * @note
 **********************************/
uint16_t crc16_xmodem(uint8_t *data, uint16_t len)
{
    uint16_t crc = 0;

    for (uint32_t i = 0; i < len; i++) {
        uint16_t code = crc >> 8;
        code ^= data[i];
        code ^= code >> 4;
        crc = crc << 8;
        crc ^= code;
        code = code << 5;
        crc ^= code;
        code = code << 7;
        crc ^= code;
    }

    return crc;
}