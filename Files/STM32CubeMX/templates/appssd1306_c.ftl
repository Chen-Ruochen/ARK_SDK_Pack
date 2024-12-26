[#ftl]
[#assign valVersion = "1.0.0"]
[#-- SWIPdatas is a list of SWIPconfigModel --]
[#list SWIPdatas as SWIP]
[#if SWIP.defines??]
[#list SWIP.defines as definition]
[#compress]
[#if definition.name == "SSD1306_VERSION"] [#assign valVersion = definition.value] [/#if]
[/#compress]
[/#list]
[/#if]
[/#list]
/**********************************
 * @Description  : SSD1306驱动扩展源文件
 * @Author       : Jason Chen
 * @Date         : 2024-12-26 15:28:15
 * @FilePath     : appssd1306_c.c
 * @Version      : ${valVersion}
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
#include "app_ssd1306.h"
#include "drv_ssd1306_ex.h"

#ifndef SSD1306_ADDR
#define SSD1306_ADDR         SSD1306_ADDR_SA0_1
#endif

/* USER CODE BEGIN Include */

/* USER CODE END Include */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

void I_CUBE_SSD1306_Init(void)
{
    ssd1306_advance_init(SSD1306_INTERFACE, SSD1306_ADDR);
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
