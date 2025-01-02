[#ftl]
[#assign valVersion = "1.0.0"]
[#-- SWIPdatas is a list of SWIPconfigModel --]
[#list SWIPdatas as SWIP]
[#if SWIP.defines??]
[#list SWIP.defines as definition]
[#compress]
[#if definition.name == "LOGGER_VERSION"] [#assign valVersion = definition.value] [/#if]
[/#compress]
[/#list]
[/#if]
[/#list]
/**********************************
 * @Description  : Logger模块应用文件
 * @Author       : Jason Chen
 * @Date         : 2024-12-30 11:50:38
 * @FilePath     : app_logger.c
 * @Version      : ${valVersion}
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
#include "app_logger.h"
#include "module_logger.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

static ark_err_t log_mutex_init()
{
    /* USER CODE BEGIN log_mutex_init 0 */
    return ARK_OK;
    /* USER CODE END log_mutex_init 0 */
}

static ark_err_t log_mutex_set(bool lock)
{
    /* USER CODE BEGIN log_mutex_push 0 */
    return ARK_OK;
    /* USER CODE END log_mutex_push 0 */
}

static ark_err_t log_mutex_get(void)
{
    /* USER CODE BEGIN log_mutex_pop 0 */
    return ARK_OK;
    /* USER CODE END log_mutex_pop 0 */
}

static void log_tx(void *data, uint16_t len)
{
    /* USER CODE BEGIN log_tx 0 */

    /* USER CODE END log_tx 0 */
}

void I_CUBE_Logger_Init(void)
{
    /* USER CODE BEGIN I_CUBE_Logger_Init 0 */

    /* USER CODE END I_CUBE_Logger_Init 0 */
    if (Logger_Init(&log_mutex_init, &log_mutex_set, &log_mutex_get, &log_tx) != ARK_OK) {
        /* USER CODE BEGIN I_CUBE_Logger_Init 1 */

        /* USER CODE END I_CUBE_Logger_Init 1 */
    }
    /* USER CODE BEGIN I_CUBE_Logger_Init 2 */

    /* USER CODE END I_CUBE_Logger_Init 2 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */