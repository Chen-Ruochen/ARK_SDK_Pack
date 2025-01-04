[#ftl]
[#assign valVersion = "1.0.0"]
[#assign valBootAddr = "0x08000000"]
[#assign valBootSize = "0x00020000"]
[#assign valAppAddr = "0x08020000"]
[#assign valAppSize = "0x00080000"]
[#assign valSectorSize = "0x00000800"]
[#-- SWIPdatas is a list of SWIPconfigModel --]
[#list SWIPdatas as SWIP]
[#if SWIP.defines??]
[#list SWIP.defines as definition]
[#compress]
[#if definition.name == "OTA_VERSION"] [#assign valVersion = definition.value] [/#if]
[#if definition.name == "OTA_BOOT_STARTADDR"] [#assign valBootAddr = definition.value] [/#if]
[#if definition.name == "OTA_BOOT_SIZE"] [#assign valBootSize = definition.value] [/#if]
[#if definition.name == "OTA_APP_STARTADDR"] [#assign valAppAddr = definition.value] [/#if]
[#if definition.name == "OTA_APP_SIZE"] [#assign valAppSize = definition.value] [/#if]
[#if definition.name == "OTA_SECTOR_SIZE"] [#assign valSectorSize = definition.value] [/#if]
[/#compress]
[/#list]
[/#if]
[/#list]
/**********************************
 * @Description  : OTA模块头文件
 * @Author       : Jason Chen
 * @Date         : 2025-01-04 09:52:16
 * @FilePath     : module_ota.h
 * @Version      : ${valVersion}
 * @Copyright 2025 Jason Chen, All Rights Reserved.
 **********************************/
#ifndef __MODULE_OTA_H__
#define __MODULE_OTA_H__

#include "common_lib.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

#ifdef __cplusplus
extern "C" {
#endif

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

#define OTA_BOOT_STARTADDR      (${valBootAddr})
#define OTA_BOOT_SIZE           (${valBootSize})
#define OTA_APP_STARTADDR       (${valAppAddr})
#define OTA_APP_SIZE            (${valAppSize})
#define OTA_SECTOR_SIZE         (${valSectorSize})

void I_CUBE_OTA_JumpToAddr(uint32_t targetAddr);
void I_CUBE_OTA_Start(uint8_t otaBlock);
void I_CUBE_OTA_Ruin(uint32_t startAddr, uint32_t size);

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

#ifdef __cplusplus
}
#endif

#endif // __MODULE_OTA_H__
