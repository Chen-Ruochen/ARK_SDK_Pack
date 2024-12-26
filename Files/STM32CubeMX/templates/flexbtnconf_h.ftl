[#ftl]
[#assign valVersion = "1.0.0"]
[#assign valBtnLPL = 0]
[#assign valBtnSPT = 1500]
[#assign valBtnLPT = 3000]
[#assign valBtnLHT = 4500]
[#assign valBtnNum = 1]
[#assign valBtnSFreq = 50]
[#assign valBtnMClickInter = 300]
[#-- SWIPdatas is a list of SWIPconfigModel --]
[#list SWIPdatas as SWIP]
[#if SWIP.defines??]
[#list SWIP.defines as definition]
[#if definition.name == "FLEXBTN_VERSION"]  [#assign valVersion = definition.value] [/#if]
[#if definition.name == "FLEXBTN_CONF_LPL"] [#assign valBtnLPL = definition.value] [/#if]
[#if definition.name == "FLEXBTN_CONF_SPT"] [#assign valBtnSPT = definition.value] [/#if]
[#if definition.name == "FLEXBTN_CONF_LPT"] [#assign valBtnLPT = definition.value] [/#if]
[#if definition.name == "FLEXBTN_CONF_LHT"] [#assign valBtnLHT = definition.value] [/#if]
[#if definition.name == "FLEXBTN_CONF_NUM"] [#assign valBtnNum = definition.value] [/#if]
[#if definition.name == "FLEXBTN_CONF_SCAN_FREQ"] [#assign valBtnSFreq = definition.value] [/#if]
[#if definition.name == "FLEXBTN_CONF_CLICKINTERVAL"] [#assign valBtnMClickInter = definition.value] [/#if]
[/#list]
[/#if]
[/#list]
/**********************************
 * @Description  : Flexible button configuration
 * @Author       : Jason Chen
 * @Date         : 2024-12-24 17:20:02
 * @FilePath     : flexbtn_conf.h
 * @Version      : ${valVersion}
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
#ifndef __FLEXBTN_CONF_H
#define __FLEXBTN_CONF_H

#include "common_lib.h"

#define FLEXBTN_CONF_NUM            (${valBtnNum})
#define FLEXBTN_CONF_SPT            (${valBtnSPT})
#define FLEXBTN_CONF_LPT            (${valBtnLPT})
#define FLEXBTN_CONF_LHT            (${valBtnLHT})
#define FLEXBTN_CONF_LPL            (${valBtnLPL})
#define FLEXBTN_CONF_SCAN_FREQ      (${valBtnSFreq})
#define FLEXBTN_CONF_CLICKINTERVAL  (${valBtnMClickInter})

#define FLEX_MS_TO_SCAN_CNT(ms) (ms / (1000 / FLEXBTN_CONF_SCAN_FREQ))

/* Multiple clicks interval, default 300ms */
#define MAX_MULTIPLE_CLICKS_INTERVAL (FLEX_MS_TO_SCAN_CNT(FLEXBTN_CONF_CLICKINTERVAL))

#ifdef __cplusplus
extern "C" {
#endif

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

#ifdef __cplusplus
}
#endif
#endif /* __FLEXBTN_CONF_H */
