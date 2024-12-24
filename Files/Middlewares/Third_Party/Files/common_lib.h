/**********************************
 * @Description  : 公共库头文件
 * @Author       : Jason Chen
 * @Date         : 2024-12-10 08:48:10
 * @FilePath     : common_lib.h
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
#ifndef __COMMON_LIB_H__
#define __COMMON_LIB_H__

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "stdint.h"

typedef enum ark_err_e {
    ARK_OK              = 0,
    ARK_ERR             = 1,
    ARK_ERR_PARAM       = 2,
    ARK_ERR_MEM         = 3,
    ARK_ERR_TIMEOUT     = 4,
    ARK_ERR_BUSY        = 5,
    ARK_ERR_FULL        = 6,
    ARK_ERR_EMPTY       = 7,
    ARK_ERR_NOT_FOUND   = 8,
    ARK_ERR_EXIST       = 9,
    ARK_ERR_OVERFLOW    = 10,
    ARK_ERR_NOT_INIT    = 12,
    ARK_ERR_NOT_SUPPORT = 13,
    ARK_ERR_NOT_PERM    = 14,
    ARK_ERR_NOT_READY   = 15,
} ark_err_t;

#endif