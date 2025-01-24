/**********************************
 * @Description  : MD5加密算法头文件
 * @Author       : Jason Chen
 * @Date         : 2025-01-23 14:17:34
 * @FilePath     : md5.h
 * @Version      : V1.0
 * @Copyright 2025 Jason Chen, All Rights Reserved.
 **********************************/
#ifndef __MD5_H
#define __MD5_H
#include "common_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

int32_t cal_md5(uint8_t *result, uint8_t *data, uint32_t length);

#ifdef __cplusplus
}
#endif

#endif
