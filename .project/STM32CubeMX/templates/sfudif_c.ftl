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
 * @Description  : This file is the sfud interface file.
 * @Author       : Jason Chen
 * @Date         : 2024-12-27 14:05:19
 * @FilePath     : sfud_if.c
 * @Version      : ${valVersion}
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
#include "sfud.h"
/* USER CODE BEGIN Include */

/* USER CODE END Include */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */


/**
 * SPI write data then read data
 */
static sfud_err spi_write_read(const sfud_spi *spi, const uint8_t *write_buf, size_t write_size, uint8_t *read_buf,
        size_t read_size) {
    sfud_err result = SFUD_SUCCESS;
    uint8_t send_data, read_data;
    /* USER CODE BEGIN 0 */

    /**
     * add your spi write and read code
     */

    /* USER CODE END 0 */
    return result;
}

#ifdef SFUD_USING_QSPI
/**
 * read flash data by QSPI
 */
static sfud_err qspi_read(const struct __sfud_spi *spi, uint32_t addr, sfud_qspi_read_cmd_format *qspi_read_cmd_format,
        uint8_t *read_buf, size_t read_size) {
    sfud_err result = SFUD_SUCCESS;
    /* USER CODE BEGIN 0 */

    /**
     * add your qspi read flash data code
     */

    /* USER CODE END 0 */
    return result;
}
#endif /* SFUD_USING_QSPI */

sfud_err sfud_spi_port_init(sfud_flash *flash)
{
    sfud_err result = SFUD_SUCCESS;

    /* USER CODE BEGIN 0 */

    /**
     * add your port spi bus and device object initialize code like this:
     * 1. rcc initialize
     * 2. gpio initialize
     * 3. spi device initialize
     * 4. flash->spi and flash->retry item initialize
     *    flash->spi.wr = spi_write_read; //Required
     *    flash->spi.qspi_read = qspi_read; //Required when QSPI mode enable
     *    flash->spi.lock = spi_lock;
     *    flash->spi.unlock = spi_unlock;
     *    flash->spi.user_data = &spix;
     *    flash->retry.delay = null;
     *    flash->retry.times = 10000; //Required
     */

    /* USER CODE END 0 */
    return result;
}


void sfud_log_debug(const char *file, const long line, const char *format, ...)
{
    /* USER CODE BEGIN 0 */

    /* USER CODE END 0 */
}


void sfud_log_info(const char *format, ...)
{
    /* USER CODE BEGIN 0 */

    /* USER CODE END 0 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
