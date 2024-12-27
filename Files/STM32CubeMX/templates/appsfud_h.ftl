[#ftl]
[#assign valVersion = "1.0.0"]
[#-- SWIPdatas is a list of SWIPconfigModel --]
[#list SWIPdatas as SWIP]
[#if SWIP.defines??]
[#list SWIP.defines as definition]
[#compress]
[#if definition.name == "SFUD_VERSION"] [#assign valVersion = definition.value] [/#if]
[/#compress]
[/#list]
[/#if]
[/#list]
/**********************************
 * @Description  : sfud应用文件
 * @Author       : Jason Chen
 * @Date         : 2024-12-27 14:05:19
 * @FilePath     : app_sfud.h
 * @Version      : ${valVersion}
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
/* USER CODE BEGIN Include */

/* USER CODE END Include */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

void I_CUBE_SFUD_Init(void);

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
