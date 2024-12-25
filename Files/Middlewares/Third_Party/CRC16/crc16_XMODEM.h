/**********************************
 * @Description  : CRC16头文件
 * @Author       : Jason Chen
 * @Date         : 2024-12-10 16:46:45
 * @FilePath     : crc16_XMODEM.h
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
#ifndef __CRC16_XMODEM_H
#define __CRC16_XMODEM_H

#include "common_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

uint16_t crc16_xmodem(uint8_t *data, uint16_t len);

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

#ifdef __cplusplus
}
#endif

#endif // __CRC16_XMODEM_H