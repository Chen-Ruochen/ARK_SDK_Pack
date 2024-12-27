[#ftl]
[#assign valVersion = "1.0.0"]
[#assign valBtnLPL = 0]
[#assign valBtnSPT = 1500]
[#assign valBtnLPT = 3000]
[#assign valBtnLHT = 4500]
[#assign valBtnNum = 1]
[#assign valBtnSFreq = 50]
[#assign valBtnMClickInter = 300]
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
[#if definition.name == "FLEXBTN_CONF_LPL"] [#assign valBtnLPL = definition.value] [/#if]
[#if definition.name == "FLEXBTN_CONF_SPT"] [#assign valBtnSPT = definition.value] [/#if]
[#if definition.name == "FLEXBTN_CONF_LPT"] [#assign valBtnLPT = definition.value] [/#if]
[#if definition.name == "FLEXBTN_CONF_LHT"] [#assign valBtnLHT = definition.value] [/#if]
[#if definition.name == "FLEXBTN_CONF_SCAN_FREQ"] [#assign valBtnSFreq = definition.value] [/#if]
[#if definition.name == "FLEXBTN_CONF_CLICKINTERVAL"] [#assign valBtnMClickInter = definition.value] [/#if]
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
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

#ifdef __cplusplus
extern "C" {
#endif

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

#define FLEXBTN_CONF_SPT            (${valBtnSPT})
#define FLEXBTN_CONF_LPT            (${valBtnLPT})
#define FLEXBTN_CONF_LHT            (${valBtnLHT})
#define FLEXBTN_CONF_LPL            (${valBtnLPL})
#define FLEXBTN_CONF_SCAN_FREQ      (${valBtnSFreq})
#define FLEXBTN_CONF_CLICKINTERVAL  (${valBtnMClickInter})

#define FLEX_MS_TO_SCAN_CNT(ms) (ms / (1000 / FLEXBTN_CONF_SCAN_FREQ))

/* Multiple clicks interval, default 300ms */
#define MAX_MULTIPLE_CLICKS_INTERVAL (FLEX_MS_TO_SCAN_CNT(FLEXBTN_CONF_CLICKINTERVAL))

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

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

#ifdef __cplusplus
}
#endif
#endif /* __APP_FLEXBTN_H */
