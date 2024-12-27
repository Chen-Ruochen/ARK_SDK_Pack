[#ftl]
[#assign valVersion = "1.0.0"]
[#assign valFastRead = 0]
[#assign valCustomTable = 0]
[#assign valFlashName = "W25Q64FV"]
[#assign valSpiName = "SPI1"]
[#assign valMfId = "SFUD_MF_ID_WINBOND"]
[#assign valTypeId = "0x40"]
[#assign valCapId = "0x17"]
[#assign valCapacity = 8388608]
[#assign valWriteMode = "SFUD_WM_PAGE_256B"]
[#assign valEraseGran = 4096]
[#assign valEraseCmd = "0x20"]
[#assign valDataLineWidth = 4]
[#assign valCustomName = "W25Q64FV"]
[#-- SWIPdatas is a list of SWIPconfigModel --]
[#list SWIPdatas as SWIP]
[#if SWIP.defines??]
[#list SWIP.defines as definition]
[#compress]
[#if definition.name == "SFUD_VERSION"] [#assign valVersion = definition.value] [/#if]
[#if definition.name == "SFUD_FASTREAD"] [#assign valFastRead = definition.value] [/#if]
[#if definition.name == "SFUD_CUSTOMTABLE"] [#assign valCustomTable = definition.value] [/#if]
[#if definition.name == "SFUD_FLASHNAME"] [#assign valFlashName = definition.value] [/#if]
[#if definition.name == "SFUD_SPINAME"] [#assign valSpiName = definition.value] [/#if]
[#if definition.name == "SFUD_MFID"] [#assign valMfId = definition.value] [/#if]
[#if definition.name == "SFUD_TYPEID"] [#assign valTypeId = definition.value] [/#if]
[#if definition.name == "SFUD_CAPID"] [#assign valCapId = definition.value] [/#if]
[#if definition.name == "SFUD_CAPACITY"] [#assign valCapacity = definition.value] [/#if]
[#if definition.name == "SFUD_WRITEMODE"] [#assign valWriteMode = definition.value] [/#if]
[#if definition.name == "SFUD_ERASEGRAN"] [#assign valEraseGran = definition.value] [/#if]
[#if definition.name == "SFUD_ERASECMD"] [#assign valEraseCmd = definition.value] [/#if]
[#if definition.name == "SFUD_DATALINEWIDTH"] [#assign valDataLineWidth = definition.value] [/#if]
[#if definition.name == "SFUD_CUSTOMNAME"] [#assign valCustomName = definition.value] [/#if]
[/#compress]
[/#list]
[/#if]
[/#list]
/**********************************
 * @Description  : sfud应用文件
 * @Author       : Jason Chen
 * @Date         : 2024-12-27 14:05:19
 * @FilePath     : app_sfud.c
 * @Version      : ${valVersion}
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
#include "app_sfud.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

[#if valCustomTable?number == 1]
[#assign valFlashName = valCustomName]
sfud_flash sfud_${valFlashName} = {
        .name = "${valFlashName}",
        .spi.name = "${valSpiName}",
        .chip = {"${valFlashName}", ${valMfId}, ${valTypeId}, ${valCapId}, ${valCapacity}, ${valWriteMode}, ${valEraseGran}, ${valEraseCmd}}};
[/#if]

void I_CUBE_SFUD_Init(void)
{
    /* USER CODE BEGIN I_CUBE_SFUD_Init 0 */

    /* USER CODE END I_CUBE_SFUD_Init 0 */
[#if valCustomTable?number == 1]
    if (sfud_device_init(&sfud_${valFlashName}) == SFUD_SUCCESS) {
        [#if valFastRead?number == 1]
        /* enable qspi fast read mode, set four data lines width */
        sfud_qspi_fast_read_enable(sfud_get_device(SFUD_${valFlashName}_DEVICE_INDEX), ${valDataLineWidth});
        [/#if]
        /* USER CODE BEGIN I_CUBE_SFUD_Init 1 */

        /* USER CODE END I_CUBE_SFUD_Init 1 */
    } else {
        /* USER CODE BEGIN I_CUBE_SFUD_Init 2 */

        /* USER CODE END I_CUBE_SFUD_Init 2 */
    }
[#else]
    if (sfud_init() == SFUD_SUCCESS) {
        [#if valFastRead?number == 1]
        /* enable qspi fast read mode, set four data lines width */
        sfud_qspi_fast_read_enable(sfud_get_device(SFUD_${valFlashName}_DEVICE_INDEX), ${valDataLineWidth});
        [/#if]
        /* USER CODE BEGIN I_CUBE_SFUD_Init 1 */

        /* USER CODE END I_CUBE_SFUD_Init 1 */
    } else {
        /* USER CODE BEGIN I_CUBE_SFUD_Init 2 */

        /* USER CODE END I_CUBE_SFUD_Init 2 */
    }
[/#if]
    /* USER CODE BEGIN I_CUBE_SFUD_Init 3 */

    /* USER CODE END I_CUBE_SFUD_Init 3 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
