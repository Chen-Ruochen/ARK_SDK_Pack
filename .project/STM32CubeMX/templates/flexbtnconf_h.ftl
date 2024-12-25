[#ftl]
/**********************************
 * @Description  : Flexible button configuration
 * @Author       : Jason Chen
 * @Date         : 2024-12-24 17:20:02
 * @FilePath     : flexbtn_conf.h
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
#ifndef __FLEXBTN_CONF_H
#define __FLEXBTN_CONF_H

#include "common_lib.h"

[#list SWIPdatas as SWIP]
[#if SWIP.defines??]
[#list SWIP.defines as definition]
#define ${definition.name} #t#t ${definition.value}
[/#list]

#ifdef __cplusplus
extern "C" {
#endif

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

#ifdef __cplusplus
}
#endif
#endif /* __FLEXBTN_CONF_H */
