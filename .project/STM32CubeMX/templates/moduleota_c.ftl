[#ftl]
[#assign valVersion = "1.0.0"]
[#-- SWIPdatas is a list of SWIPconfigModel --]
[#list SWIPdatas as SWIP]
[#if SWIP.defines??]
[#list SWIP.defines as definition]
[#compress]
[#if definition.name == "OTA_VERSION"] [#assign valVersion = definition.value] [/#if]
[/#compress]
[/#list]
[/#if]
[/#list]
/**********************************
 * @Description  : OTA模块源文件
 * @Author       : Jason Chen
 * @Date         : 2025-01-04 09:52:10
 * @FilePath     : module_ota.c
 * @Version      : ${valVersion}
 * @Copyright 2025 Jason Chen, All Rights Reserved.
 **********************************/
#include "module_ota.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

void I_CUBE_OTA_JumpToAddr(uint32_t targetAddr)
{
    /* USER CODE BEGIN I_CUBE_OTA_JumpToAddr */

    /* USER CODE END I_CUBE_OTA_JumpToAddr */
}

void I_CUBE_OTA_Start(uint8_t otaBlock)
{
    /* USER CODE BEGIN I_CUBE_OTA_Start */

    /* USER CODE END I_CUBE_OTA_Start */
}

void I_CUBE_OTA_Ruin(uint32_t startAddr, uint32_t size)
{
    /* USER CODE BEGIN I_CUBE_OTA_Ruin */

    /* USER CODE END I_CUBE_OTA_Ruin */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
