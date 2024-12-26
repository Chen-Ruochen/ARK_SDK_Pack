[#ftl]
[#assign valVersion = "1.0.0"]
[#assign valInterface = "SSD1306_INTERFACE_IIC"]
[#-- SWIPdatas is a list of SWIPconfigModel --]
[#list SWIPdatas as SWIP]
[#if SWIP.defines??]
[#list SWIP.defines as definition]
[#compress]
[#if definition.name == "SSD1306_VERSION"] [#assign valVersion = definition.value] [/#if]
[#if definition.name == "SSD1306_INTERFACE"] [#assign valInterface = definition.value] [/#if]
[/#compress]
[/#list]
[/#if]
[/#list]
/**********************************
 * @Description  : ssd1306 interface driver function
 * @Author       : Jason Chen
 * @Date         : 2024-12-17 15:44:07
 * @FilePath     : ssd1306_if_template.c
 * @Version      : ${valVersion}
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
#include "ssd1306_if.h"
/* USER CODE BEGIN Include */

/* USER CODE END Include */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

[#if valInterface?contains("IIC")]
/**
 * @brief  interface iic bus init
 * @return status code
 *         - 0 success
 *         - 1 iic init failed
 * @note   none
 */
uint8_t ssd1306_interface_iic_init(void)
{
    /* USER CODE BEGIN 0 */
#warning "Please implement ssd1306_interface_iic_init() for your application"
    return 0;
    /* USER CODE END 0 */
}

/**
 * @brief  interface iic bus deinit
 * @return status code
 *         - 0 success
 *         - 1 iic deinit failed
 * @note   none
 */
uint8_t ssd1306_interface_iic_deinit(void)
{
    /* USER CODE BEGIN 0 */
#warning "Please implement ssd1306_interface_iic_deinit() for your application"
    return 0;
    /* USER CODE END 0 */
}

/**
 * @brief     interface iic bus write
 * @param[in] addr is the iic device write address
 * @param[in] reg is the iic register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t ssd1306_interface_iic_write(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)
{
    /* USER CODE BEGIN 0 */
#warning "Please implement ssd1306_interface_iic_write() for your application"
    return 0;
    /* USER CODE END 0 */
}
[#else]

/**
 * @brief  interface spi bus init
 * @return status code
 *         - 0 success
 *         - 1 spi init failed
 * @note   none
 */
uint8_t ssd1306_interface_spi_init(void)
{
    /* USER CODE BEGIN 0 */
#warning "Please implement ssd1306_interface_spi_init() for your application"
    return 0;
    /* USER CODE END 0 */
}

/**
 * @brief  interface spi bus deinit
 * @return status code
 *         - 0 success
 *         - 1 spi deinit failed
 * @note   none
 */
uint8_t ssd1306_interface_spi_deinit(void)
{
    /* USER CODE BEGIN 0 */
#warning "Please implement ssd1306_interface_spi_deinit() for your application"
    return 0;
    /* USER CODE END 0 */
}

/**
 * @brief     interface spi bus write
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t ssd1306_interface_spi_write_cmd(uint8_t *buf, uint16_t len)
{
    /* USER CODE BEGIN 0 */
#warning "Please implement ssd1306_interface_spi_write_cmd() for your application"
    return 0;
    /* USER CODE END 0 */
}

/**
 * @brief  interface command && data gpio init
 * @return status code
 *         - 0 success
 *         - 1 gpio init failed
 * @note   none
 */
uint8_t ssd1306_interface_spi_cmd_data_gpio_init(void)
{
    /* USER CODE BEGIN 0 */
#warning "Please implement ssd1306_interface_spi_cmd_data_gpio_init() for your application"
    return 0;
    /* USER CODE END 0 */
}

/**
 * @brief  interface command && data gpio deinit
 * @return status code
 *         - 0 success
 *         - 1 gpio deinit failed
 * @note   none
 */
uint8_t ssd1306_interface_spi_cmd_data_gpio_deinit(void)
{
    /* USER CODE BEGIN 0 */
#warning "Please implement ssd1306_interface_spi_cmd_data_gpio_deinit() for your application"
    return 0;
    /* USER CODE END 0 */
}

/**
 * @brief     interface command && data gpio write
 * @param[in] value is the written value
 * @return    status code
 *            - 0 success
 *            - 1 gpio write failed
 * @note      none
 */
uint8_t ssd1306_interface_spi_cmd_data_gpio_write(uint8_t value)
{
    /* USER CODE BEGIN 0 */
#warning "Please implement ssd1306_interface_spi_cmd_data_gpio_write() for your application"
    return 0;
    /* USER CODE END 0 */
}
[/#if]

/**
 * @brief     interface delay ms
 * @param[in] ms
 * @note      none
 */
void ssd1306_interface_delay_ms(uint32_t ms)
{
    /* USER CODE BEGIN 0 */
#warning "Please implement ssd1306_interface_delay_ms() for your application"
    /* USER CODE END 0 */
}

/**
 * @brief  interface reset gpio init
 * @return status code
 *         - 0 success
 *         - 1 gpio init failed
 * @note   none
 */
uint8_t ssd1306_interface_reset_gpio_init(void)
{
    /* USER CODE BEGIN 0 */
#warning "Please implement ssd1306_interface_reset_gpio_init() for your application"
    return 0;
    /* USER CODE END 0 */
}

/**
 * @brief  interface reset gpio deinit
 * @return status code
 *         - 0 success
 *         - 1 gpio deinit failed
 * @note   none
 */
uint8_t ssd1306_interface_reset_gpio_deinit(void)
{
    /* USER CODE BEGIN 0 */
#warning "Please implement ssd1306_interface_reset_gpio_deinit() for your application"
    return 0;
    /* USER CODE END 0 */
}

/**
 * @brief     interface reset gpio write
 * @param[in] value is the written value
 * @return    status code
 *            - 0 success
 *            - 1 gpio write failed
 * @note      none
 */
uint8_t ssd1306_interface_reset_gpio_write(uint8_t value)
{
    /* USER CODE BEGIN 0 */
#warning "Please implement ssd1306_interface_reset_gpio_write() for your application"
    return 0;
    /* USER CODE END 0 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
