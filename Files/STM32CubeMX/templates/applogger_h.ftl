[#ftl]
[#assign valVersion = "1.0.0"]
[#assign valLevel = "LOGGERLEVEL_DEBUG"]
[#-- SWIPdatas is a list of SWIPconfigModel --]
[#list SWIPdatas as SWIP]
[#if SWIP.defines??]
[#list SWIP.defines as definition]
[#compress]
[#if definition.name == "LOGGER_VERSION"] [#assign valVersion = definition.value] [/#if]
[#if definition.name == "LOGGER_LEVEL"] [#assign valLevel = definition.value] [/#if]
[/#compress]
[/#list]
[/#if]
[/#list]
/**********************************
 * @Description  : Logger模块应用文件
 * @Author       : Jason Chen
 * @Date         : 2024-12-30 11:50:38
 * @FilePath     : app_logger.h
 * @Version      : ${valVersion}
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
#ifndef APP_LOGGER_H
#define APP_LOGGER_H
#include "common_lib.h"
#include "module_logger.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

#ifdef __cplusplus
extern "C" {
#endif

#define LOGGER_LEVEL ${valLevel}

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

void I_CUBE_Logger_Init(void *ptr);
void I_CUBE_Logger_Process(void);

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

#ifdef __cplusplus
}
#endif

#endif // APP_LOGGER_H
