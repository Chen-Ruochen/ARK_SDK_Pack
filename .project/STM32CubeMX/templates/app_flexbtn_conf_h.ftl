[#ftl]
/**********************************
 * @Description  : Flexible button configuration
 * @Author       : Jason Chen
 * @Date         : 2024-12-24 17:20:02
 * @FilePath     : app_flexbtn_conf.h
 * @Version      : V1.0
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
#ifndef APP_FLEXBTN_CONF_H__
#define APP_FLEXBTN_CONF_H__

#include "common_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

[#list SWIPdatas as SWIP]

[#if SWIP.defines??]
    [#list SWIP.defines as definition]
#define ${definition.name} #t#t (${definition.value})
    [/#list]
[/#if]

#define FLEX_MS_TO_SCAN_CNT(ms) (ms / (1000 / FLEX_BTN_CONF_SCAN_FREQ_HZ))
#define MAX_MULTIPLE_CLICKS_INTERVAL (FLEX_MS_TO_SCAN_CNT(FLEX_BTN_CONF_MCLICK_INTER))

#ifdef __cplusplus
}
#endif
#endif /* APP_FLEXBTN_CONF_H__ */

[/#list]
