[#ftl]
[#assign valVersion = "1.0.0"]
[#-- SWIPdatas is a list of SWIPconfigModel --]
[#list SWIPdatas as SWIP]
[#if SWIP.defines??]
[#list SWIP.defines as definition]
[#compress]
[#if definition.name == "SYSTEM_VERSION"] [#assign valVersion = definition.value] [/#if]
[/#compress]
[/#list]
[/#if]
[/#list]
/**********************************
 * @Description  : System模块应用文件
 * @Author       : Jason Chen
 * @Date         : 2025-01-03 09:50:00
 * @FilePath     : module_system.h
 * @Version      : ${valVersion}
 * @Copyright 2025 Jason Chen, All Rights Reserved.
 **********************************/
#ifndef __MODULE_SYSTEM_H
#define __MODULE_SYSTEM_H
#include "common_lib.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

#ifdef __cplusplus
extern "C" {
#endif

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

void I_CUBE_System_Reset(void);
void I_CUBE_System_Sleep(void);
void I_CUBE_System_Get_BootReason(uint8_t *reason);
void I_CUBE_System_Get_SerialNumber(uint8_t *sn);
void I_CUBE_System_Get_License(uint8_t *license);
void I_CUBE_System_Get_Version(uint8_t *version);
void I_CUBE_System_Get_BuildTime(uint8_t *buildTime);
void I_CUBE_System_Error_Handler(const char *file, const long line);

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

#ifdef __cplusplus
}
#endif

#endif
