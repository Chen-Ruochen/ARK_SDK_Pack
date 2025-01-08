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
 * @FilePath     : module_system.c
 * @Version      : ${valVersion}
 * @Copyright 2025 Jason Chen, All Rights Reserved.
 **********************************/
#include "module_system.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

void I_CUBE_System_Init(void)
{
    /* USER CODE BEGIN I_CUBE_System_Init 0 */

    /* USER CODE END I_CUBE_System_Init 0 */
}

void I_CUBE_System_Reset(void)
{
    /* USER CODE BEGIN I_CUBE_System_Reset 0 */

    /* USER CODE END I_CUBE_System_Reset 0 */
}

void I_CUBE_System_Sleep(void)
{
    /* USER CODE BEGIN I_CUBE_System_Sleep 0 */

    /* USER CODE END I_CUBE_System_Sleep 0 */
}

void I_CUBE_System_Get_BootReason(uint8_t *reason)
{
    /* USER CODE BEGIN I_CUBE_System_Get_BootReason 0 */

    /* USER CODE END I_CUBE_System_Get_BootReason 0 */
}

void I_CUBE_System_Get_SerialNumber(uint8_t *sn)
{
    /* USER CODE BEGIN I_CUBE_System_Get_SerialNumber 0 */

    /* USER CODE END I_CUBE_System_Get_SerialNumber 0 */
}

void I_CUBE_System_Get_License(uint8_t *license)
{
    /* USER CODE BEGIN I_CUBE_System_Get_License 0 */

    /* USER CODE END I_CUBE_System_Get_License 0 */
}

void I_CUBE_System_Get_Version(uint8_t *version)
{
    /* USER CODE BEGIN I_CUBE_System_Get_Version 0 */

    /* USER CODE END I_CUBE_System_Get_Version 0 */
}

void I_CUBE_System_Get_BuildTime(uint8_t *buildTime)
{
    /* USER CODE BEGIN I_CUBE_System_Get_BuildTime 0 */

    /* USER CODE END I_CUBE_System_Get_BuildTime 0 */
}

void I_CUBE_System_Error_Handler(const char *file, const long line)
{
    /* USER CODE BEGIN I_CUBE_System_Error_Handler 0 */

    /* USER CODE END I_CUBE_System_Error_Handler 0 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
