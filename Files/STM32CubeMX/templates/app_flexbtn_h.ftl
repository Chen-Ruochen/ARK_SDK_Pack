[#ftl]
/**********************************
 * @Description  : Flexible button application
 * @Author       : Jason Chen
 * @Date         : 2024-12-24 20:44:43
 * @FilePath     : app_flexbtn.h
 * @Version      : V1.0
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
#ifndef APP_FLEXBTN_H__
#define APP_FLEXBTN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "common_lib.h"

typedef enum user_btn_e
{
    USER_BUTTON_1 = 0,
    USER_BUTTON_MAX
} user_btn_t;

__weak uint8_t I_CUBE_FlexBtn_Read(void *arg);
__weak void I_CUBE_FlexBtn_Evt(void *arg);
void I_CUBE_FlexBtn_Init(void);
void I_CUBE_FlexBtn_Scan(void *arg);

#ifdef __cplusplus
}
#endif

#endif

[/#list]