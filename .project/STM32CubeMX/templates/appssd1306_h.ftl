[#ftl]
[#assign valVersion = "1.0.0"]
[#assign valInterface = "SSD1306_INTERFACE_IIC"]
[#assign valAddr = "SSD1306_ADDR_SA0_1"]
[#-- SWIPdatas is a list of SWIPconfigModel --]
[#list SWIPdatas as SWIP]
[#if SWIP.defines??]
[#list SWIP.defines as definition]
[#compress]
[#if definition.name == "SSD1306_VERSION"] [#assign valVersion = definition.value] [/#if]
[#if definition.name == "SSD1306_INTERFACE"] [#assign valInterface = definition.value] [/#if]
[#if definition.name == "SSD1306_ADDR"] [#assign valAddr = definition.value] [/#if]
[/#compress]
[/#list]
[/#if]
[/#list]
/**********************************
 * @Description  : SSD1306驱动扩展源文件
 * @Author       : Jason Chen
 * @Date         : 2024-12-26 15:28:17
 * @FilePath     : appssd1306_h.h
 * @Version      : ${valVersion}
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
#ifndef __APP_SSD1306_H
#define __APP_SSD1306_H

#include "common_lib.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

#ifdef __cplusplus
extern "C"{
#endif

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

#define SSD1306_INTERFACE    (${valInterface})
[#if valInterface?contains("IIC")]
#define SSD1306_ADDR         (${valAddr})
[/#if]

void I_CUBE_SSD1306_Init(void);

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

#ifdef __cplusplus
}
#endif

#endif