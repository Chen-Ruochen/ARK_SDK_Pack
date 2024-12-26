[#ftl]
[#assign valVersion = "1.0.0"]
[#assign valBtnNum = 1]
[#assign valBtnName1 = "BUTTON1"]
[#assign valBtnName2 = "BUTTON2"]
[#assign valBtnName3 = "BUTTON3"]
[#assign valBtnName4 = "BUTTON4"]
[#assign valBtnName5 = "BUTTON5"]
[#-- SWIPdatas is a list of SWIPconfigModel --]
[#list SWIPdatas as SWIP]
[#if SWIP.defines??]
[#list SWIP.defines as definition]
[#compress]
[#if definition.name == "FLEXBTN_VERSION"] [#assign valVersion = definition.value] [/#if]
[#if definition.name == "FLEXBTN_NUM"] [#assign valBtnNum = definition.value] [/#if]
[#if definition.name == "FLEXBTN_NAME1"] [#assign valBtnName1 = definition.value] [/#if]
[#if definition.name == "FLEXBTN_NAME2"] [#assign valBtnName2 = definition.value] [/#if]
[#if definition.name == "FLEXBTN_NAME3"] [#assign valBtnName3 = definition.value] [/#if]
[#if definition.name == "FLEXBTN_NAME4"] [#assign valBtnName4 = definition.value] [/#if]
[#if definition.name == "FLEXBTN_NAME5"] [#assign valBtnName5 = definition.value] [/#if]
[/#compress]
[/#list]
[/#if]
[/#list]
/**********************************
 * @Description  : Flexible button application
 * @Author       : Jason Chen
 * @Date         : 2024-12-24 20:44:43
 * @FilePath     : app_flexbtn.h
 * @Version      : ${valVersion}
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
#ifndef __APP_FLEXBTN_H
#define __APP_FLEXBTN_H

#include "common_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum user_btn_e
{
    ${valBtnName1} = 0,
[#if valBtnNum?number > 1]
    ${valBtnName2},
[/#if]
[#if valBtnNum?number > 2]
    ${valBtnName3},
[/#if]
[#if valBtnNum?number > 3]
    ${valBtnName4},
[/#if]
[#if valBtnNum?number > 4]
    ${valBtnName5},
[/#if]
    USER_BUTTON_MAX
} user_btn_t;

void I_CUBE_FlexBtn_Init(void);
void I_CUBE_FlexBtn_Scan(void *arg);

#ifdef __cplusplus
}
#endif
#endif /* __APP_FLEXBTN_H */
