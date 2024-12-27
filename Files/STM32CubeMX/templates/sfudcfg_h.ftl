[#ftl]
[#assign valVersion = "1.0.0"]
[#assign valDebugMode = 0]
[#assign valFastRead = 0]
[#assign valCustomTable = 0]
[#assign valFlashName = "W25Q64FV"]
[#assign valSpiName = "SPI1"]
[#assign valCustomName = "W25Q64FV"]
[#-- SWIPdatas is a list of SWIPconfigModel --]
[#list SWIPdatas as SWIP]
[#if SWIP.defines??]
[#list SWIP.defines as definition]
[#compress]
[#if definition.name == "SFUD_VERSION"] [#assign valVersion = definition.value] [/#if]
[#if definition.name == "SFUD_DEBUG_MODE"] [#assign valDebugMode = definition.value] [/#if]
[#if definition.name == "SFUD_FASTREAD"] [#assign valFastRead = definition.value] [/#if]
[#if definition.name == "SFUD_CUSTOMTABLE"] [#assign valCustomTable = definition.value] [/#if]
[#if definition.name == "SFUD_FLASHNAME"] [#assign valFlashName = definition.value] [/#if]
[#if definition.name == "SFUD_SPINAME"] [#assign valSpiName = definition.value] [/#if]
[#if definition.name == "SFUD_CUSTOMNAME"] [#assign valCustomName = definition.value] [/#if]
[/#compress]
[/#list]
[/#if]
[/#list]
/**********************************
 * @Description  : sfud配置文件
 * @Author       : Jason Chen
 * @Date         : 2024-12-27 14:05:19
 * @FilePath     : sfud_cfg.h
 * @Version      : ${valVersion}
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
#ifndef __SFUD_CFG_H
#define __SFUD_CFG_H
/* USER CODE BEGIN Include */

/* USER CODE END Include */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

[#if valDebugMode?number == 1]
#define SFUD_DEBUG_MODE
[/#if]
[#if valFastRead?number == 1]
#define SFUD_USING_QSPI
#define SFUD_USING_FAST_READ
[/#if]
[#if valCustomTable?number == 1]
[#assign valFlashName = valCustomName]
#define SFUD_USING_SFDP
[#else]
#define SFUD_USING_FLASH_INFO_TABLE
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
[/#if]

enum {
    SFUD_${valFlashName}_DEVICE_INDEX = 0,
};

#define SFUD_FLASH_DEVICE_TABLE                                                \
{                                                                              \
    [SFUD_${valFlashName}_DEVICE_INDEX] = {.name = "${valFlashName}", .spi.name = "${valSpiName}"},           \
}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

#endif /* __SFUD_CFG_H */
