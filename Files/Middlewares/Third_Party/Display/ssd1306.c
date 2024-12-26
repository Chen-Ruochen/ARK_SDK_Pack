/**********************************
 * @Description  : ssd1306 driver source file
 * @Author       : Jason Chen
 * @Date         : 2024-12-17 09:01:01
 * @FilePath     : ssd1306.c
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
#include "ssd1306.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Solomon Systech SSD1306"        /**< chip name */
#define MANUFACTURER_NAME         "Solomon Systech"                /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        1.65f                            /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        3.3f                             /**< chip max supply voltage */
#define MAX_CURRENT               0.78f                            /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                           /**< chip min operating temperature */
#define TEMPERATURE_MAX           85.0f                            /**< chip max operating temperature */
#define DRIVER_VERSION            2000                             /**< driver version */

/**
 * @brief chip command data definition
 */
#define SSD1306_CMD          0        /**< command */
#define SSD1306_DATA         1        /**< data */

/**
 * @brief chip command definition
 */
#define SSD1306_CMD_LOWER_COLUMN_START_ADDRESS              0x00        /**< command lower column start address */
#define SSD1306_CMD_HIGHER_COLUMN_START_ADDRESS             0x10        /**< command higher column start address */
#define SSD1306_CMD_MEMORY_ADDRESSING_MODE                  0x20        /**< command memory addressing mode */
#define SSD1306_CMD_SET_COLUMN_ADDRESS                      0x21        /**< command set column address */
#define SSD1306_CMD_SET_PAGE_ADDRESS                        0x22        /**< command set page address */
#define SSD1306_CMD_SET_FADE_OUT_AND_BLINKING               0x23        /**< command set fade out and blinking */
#define SSD1306_CMD_RIGHT_HORIZONTAL_SCROLL                 0x26        /**< command right horizontal scroll */
#define SSD1306_CMD_LEFT_HORIZONTAL_SCROLL                  0x27        /**< command left horizontal scroll */
#define SSD1306_CMD_VERTICAL_RIGHT_HORIZONTAL_SCROLL        0x29        /**< command vertical right horizontal scroll */
#define SSD1306_CMD_VERTICAL_LEFT_HORIZONTAL_SCROLL         0x2A        /**< command vertical left horizontal scroll */
#define SSD1306_CMD_DEACTIVATE_SCROLL                       0x2E        /**< command deactivate scroll */
#define SSD1306_CMD_ACTIVATE_SCROLL                         0x2F        /**< command activate scroll */
#define SSD1306_CMD_DISPLAY_START_LINE                      0x40        /**< command display start line */
#define SSD1306_CMD_CONTRAST_CONTROL                        0x81        /**< command contrast control */
#define SSD1306_CMD_CHARGE_PUMP_SETTING                     0x8D        /**< command charge pump setting */
#define SSD1306_CMD_COLUMN_0_MAPPED_TO_SEG0                 0xA0        /**< command column 0 mapped to seg 0 */
#define SSD1306_CMD_COLUMN_127_MAPPED_TO_SEG0               0xA1        /**< command column 127 mapped to seg 0 */
#define SSD1306_CMD_VERTICAL_SCROLL_AREA                    0xA3        /**< command vertical scroll area */
#define SSD1306_CMD_ENTIRE_DISPLAY_OFF                      0xA4        /**< command entire display off */
#define SSD1306_CMD_ENTIRE_DISPLAY_ON                       0xA5        /**< command entire display on */
#define SSD1306_CMD_NORMAL_DISPLAY                          0xA6        /**< command normal display */
#define SSD1306_CMD_INVERSE_DISPLAY                         0xA7        /**< command inverse display */
#define SSD1306_CMD_MULTIPLEX_RATIO                         0xA8        /**< command multiplex ratio */
#define SSD1306_CMD_DISPLAY_OFF                             0xAE        /**< command display off */
#define SSD1306_CMD_DISPLAY_ON                              0xAF        /**< command display on */
#define SSD1306_CMD_PAGE_ADDR                               0xB0        /**< command page address */
#define SSD1306_CMD_SCAN_DIRECTION_COM0_START               0xC0        /**< command scan direction com 0 start */
#define SSD1306_CMD_SCAN_DIRECTION_COMN_1_START             0xC8        /**< command scan direction com n-1 start */
#define SSD1306_CMD_DISPLAY_OFFSET                          0xD3        /**< command display offset */
#define SSD1306_CMD_DISPLAY_CLOCK_DIVIDE                    0xD5        /**< command display clock divide */
#define SSD1306_CMD_SET_ZOOM_IN                             0xD6        /**< command set zoom in */
#define SSD1306_CMD_PRE_CHARGE_PERIOD                       0xD9        /**< command pre charge period */
#define SSD1306_CMD_COM_PINS_CONF                           0xDA        /**< command com pins conf */
#define SSD1306_CMD_COMH_DESLECT_LEVEL                      0xDB        /**< command comh deslect level */
#define SSD1306_CMD_NOP                                     0xE3        /**< command nop */

/**
 * @brief     write one byte
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] data is the write data
 * @param[in] cmd is the command or data type
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_ssd1306_write_byte(ssd1306_handle_t *handle, uint8_t data, uint8_t cmd)
{
    uint8_t res;

    if (handle->iic_spi == SSD1306_INTERFACE_IIC) {
        if (cmd != 0) {
            if (handle->iic_write(handle->iic_addr, 0x40, &data, 1) != 0) {
                return 1;
            } else {
                return 0;
            }
        } else {
            if (handle->iic_write(handle->iic_addr, 0x00, &data, 1) != 0) {
                return 1;
            } else {
                return 0;
            }
        }
    } else if (handle->iic_spi == SSD1306_INTERFACE_SPI) {
        res = handle->spi_cmd_data_gpio_write(cmd);
        if (res != 0) {
            return 1;
        }

        if (handle->spi_write_cmd(&data, 1) != 0) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 1;
    }
}

/**
 * @brief     write multiple bytes
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] *data points to a data buffer
 * @param[in] len is the data length
 * @param[in] cmd is the command or data type
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_ssd1306_multiple_write_byte(ssd1306_handle_t *handle, uint8_t *data, uint8_t len, uint8_t cmd)
{
    uint8_t res;

    if (handle->iic_spi == SSD1306_INTERFACE_IIC) {
        if (cmd != 0) {
            if (handle->iic_write(handle->iic_addr, 0x40, data, len) != 0) {
                return 1;
            } else {
                return 0;
            }
        } else {
            if (handle->iic_write(handle->iic_addr, 0x00, data, len) != 0) {
                return 1;
            } else {
                return 0;
            }
        }
    } else if (handle->iic_spi == SSD1306_INTERFACE_SPI) {
        res = handle->spi_cmd_data_gpio_write(cmd);
        if (res != 0) {
            return 1;
        }

        if (handle->spi_write_cmd(data, len) != 0) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 1;
    }
}

/**
 * @brief     draw a point in gram
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] x is the coordinate x
 * @param[in] y is the coordinate y
 * @param[in] data is the write data
 * @return    status code
 *            - 0 success
 *            - 1 gram draw point failed
 * @note      none
 */
static uint8_t a_ssd1306_gram_draw_point(ssd1306_handle_t *handle, uint8_t x, uint8_t y, uint8_t data)
{
    uint8_t pos;
    uint8_t bx;
    uint8_t temp = 0;

    pos = y / 8;
    bx = y % 8;
    temp = 1 << bx;
    if (data != 0) {
        handle->gram[x][pos] |= temp;
    } else {
        handle->gram[x][pos] &= ~temp;
    }

    return 0;
}

/**
 * @brief     clear the screen
 * @param[in] *handle points to an ssd1306 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 clear failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1306_clear(ssd1306_handle_t *handle)
{
    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }

    memset(handle->gram, 0, sizeof(handle->gram));
    for (uint16_t i = 0; i < 8; i++) {
        if (a_ssd1306_write_byte(handle, SSD1306_CMD_PAGE_ADDR + i, SSD1306_CMD) != 0) {
            return 1;
        }
        if (a_ssd1306_write_byte(handle, SSD1306_CMD_LOWER_COLUMN_START_ADDRESS, SSD1306_CMD) != 0) {
            return 1;
        }
        if (a_ssd1306_write_byte(handle, SSD1306_CMD_HIGHER_COLUMN_START_ADDRESS, SSD1306_CMD) != 0) {
            return 1;
        }
        if (a_ssd1306_multiple_write_byte(handle, &handle->gram[0][i], 128, SSD1306_DATA) != 0) {
            return 1;
        }
    }

    return 0;
}

/**
 * @brief     update the gram data
 * @param[in] *handle points to an ssd1306 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 gram update failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1306_gram_update(ssd1306_handle_t *handle)
{
    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }

    for (uint16_t i = 0; i < 8; i++) {
        if (a_ssd1306_write_byte(handle, SSD1306_CMD_PAGE_ADDR + i, SSD1306_CMD) != 0) {
            return 1;
        }
        if (a_ssd1306_write_byte(handle, SSD1306_CMD_LOWER_COLUMN_START_ADDRESS, SSD1306_CMD) != 0) {
            return 1;
        }
        if (a_ssd1306_write_byte(handle, SSD1306_CMD_HIGHER_COLUMN_START_ADDRESS, SSD1306_CMD) != 0) {
            return 1;
        }
        if (a_ssd1306_multiple_write_byte(handle, &handle->gram[0][i], 128 , SSD1306_DATA) != 0) {
            return 1;
        }
    }

    return 0;
}

/**
 * @brief     write a point
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] x is the coordinate x
 * @param[in] y is the coordinate y
 * @param[in] data is the write data
 * @return    status code
 *            - 0 success
 *            - 1 write point failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 x or y is invalid
 * @note      none
 */
uint8_t ssd1306_write_point(ssd1306_handle_t *handle, uint8_t x, uint8_t y, uint8_t data)
{
    uint8_t pos;
    uint8_t bx;
    uint8_t temp = 0;

    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }
    if ((x > 127) || (y > 63)) {
        return 4;
    }

    pos = y / 8;
    bx = y % 8;
    temp = 1 << bx;
    if (data != 0) {
        handle->gram[x][pos] |= temp;
    } else {
        handle->gram[x][pos] &= ~temp;
    }
    if (a_ssd1306_write_byte(handle, SSD1306_CMD_PAGE_ADDR + pos, SSD1306_CMD) != 0) {
        return 1;
    }
    if (a_ssd1306_write_byte(handle, SSD1306_CMD_LOWER_COLUMN_START_ADDRESS|(x&0x0F), SSD1306_CMD) != 0) {
        return 1;
    }
    if (a_ssd1306_write_byte(handle, SSD1306_CMD_HIGHER_COLUMN_START_ADDRESS|((x>4)&0x0F), SSD1306_CMD) != 0) {
        return 1;
    }
    if (a_ssd1306_write_byte(handle, handle->gram[x][pos], SSD1306_DATA) != 0) {
        return 1;
    } else {
       return 0;
    }
}

/**
 * @brief      read a point
 * @param[in]  *handle points to an ssd1306 handle structure
 * @param[in]  x is the coordinate x
 * @param[in]  y is the coordinate y
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read point failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 x or y is invalid
 * @note       none
 */
uint8_t ssd1306_read_point(ssd1306_handle_t *handle, uint8_t x, uint8_t y, uint8_t *data)
{
    uint8_t pos;
    uint8_t bx;
    uint8_t temp = 0;

    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }
    if ((x > 127) || (y > 63)) {
        return 4;
    }

    pos = y / 8;
    bx = y % 8;
    temp = 1 << bx;
    if ((handle->gram[x][pos] & temp) != 0) {
        *data = 1;
    } else {
        *data = 0;
    }

    return 0;
}

/**
 * @brief     write a point in the gram
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] x is the coordinate x
 * @param[in] y is the coordinate y
 * @param[in] data is the write data
 * @return    status code
 *            - 0 success
 *            - 1 gram write point failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 x or y is invalid
 * @note      none
 */
uint8_t ssd1306_gram_write_point(ssd1306_handle_t *handle, uint8_t x, uint8_t y, uint8_t data)
{
    uint8_t pos;
    uint8_t bx;
    uint8_t temp = 0;

    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }
    if ((x > 127) || (y > 63)) {
        return 4;
    }

    pos = y / 8;
    bx = y % 8;
    temp = 1 << bx;
    if (data != 0) {
        handle->gram[x][pos] |= temp;
    } else {
        handle->gram[x][pos] &= ~temp;
    }

    return 0;
}

/**
 * @brief      read a point from the gram
 * @param[in]  *handle points to an ssd1306 handle structure
 * @param[in]  x is the coordinate x
 * @param[in]  y is the coordinate y
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 gram read point failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 x or y is invalid
 * @note       none
 */
uint8_t ssd1306_gram_read_point(ssd1306_handle_t *handle, uint8_t x, uint8_t y, uint8_t *data)
{
    uint8_t pos;
    uint8_t bx;
    uint8_t temp = 0;

    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }
    if ((x > 127) || (y > 63)) {
        return 4;
    }

    pos = y / 8;
    bx = y % 8;
    temp = 1 << bx;
    if ((handle->gram[x][pos] & temp) != 0) {
        *data = 1;
    } else {
        *data = 0;
    }

    return 0;
}

/**
 * @brief     fill a rectangle in the gram
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] left is the left coordinate x
 * @param[in] top is the top coordinate y
 * @param[in] right is the right coordinate x
 * @param[in] bottom is the bottom coordinate y
 * @param[in] color is the display color
 * @return    status code
 *            - 0 success
 *            - 1 gram fill rect failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 left or top is invalid
 *            - 5 right or bottom is invalid
 *            - 6 left > right or top > bottom
 * @note      none
 */
uint8_t ssd1306_gram_fill_rect(ssd1306_handle_t *handle, uint8_t left, uint8_t top, uint8_t right, uint8_t bottom, uint8_t color)
{
    uint8_t x, y;

    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }
    if ((left > 127) || (top > 63)) {
        return 4;
    }
    if ((right > 127) || (bottom > 63)) {
        return 5;
    }
    if ((left > right) || (top > bottom)) {
        return 6;
    }

    for (x = left; x <= right; x++) {
        for (y = top; y <= bottom; y++) {
            if (a_ssd1306_gram_draw_point(handle, x, y, color) != 0) {
                return 1;
            }
        }
    }

    return 0;
}

/**
 * @brief     draw a picture in the gram
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] left is the left coordinate x
 * @param[in] top is the top coordinate y
 * @param[in] right is the right coordinate x
 * @param[in] bottom is the bottom coordinate y
 * @param[in] *img points to an image buffer
 * @return    status code
 *            - 0 success
 *            - 1 gram draw picture failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 left or top is invalid
 *            - 5 right or bottom is invalid
 *            - 6 left > right or top > bottom
 * @note      none
 */
uint8_t ssd1306_gram_draw_picture(ssd1306_handle_t *handle, uint8_t left, uint8_t top, uint8_t right, uint8_t bottom, uint8_t *img)
{
    uint8_t x, y;

    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }
    if ((left > 127) || (top > 63)) {
        return 4;
    }
    if ((right > 127) || (bottom > 63)) {
        return 5;
    }
    if ((left > right) || (top > bottom)) {
        return 6;
    }

    for (x = left; x <= right; x++) {
        for (y = top; y <= bottom; y++) {
            if (a_ssd1306_gram_draw_point(handle, x, y, *img) != 0) {
                return 1;
            }
            img++;
        }
    }

    return 0;
}

/**
 * @brief     initialize the chip
 * @param[in] *handle points to an ssd1306 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic or spi initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 reset failed
 *            - 5 command && data init failed
 *            - 6 interface param is invalid
 * @note      none
 */
uint8_t ssd1306_init(ssd1306_handle_t *handle)
{
    if (handle == NULL) {
        return 2;
    }
    if (handle->iic_init == NULL) {
        return 3;
    }
    if (handle->iic_deinit == NULL) {
        return 3;
    }
    if (handle->iic_write == NULL) {
        return 3;
    }
    if (handle->spi_init == NULL) {
        return 3;
    }
    if (handle->spi_deinit == NULL) {
        return 3;
    }
    if (handle->spi_write_cmd == NULL) {
        return 3;
    }
    if (handle->delay_ms == NULL) {
        return 3;
    }
    if (handle->spi_cmd_data_gpio_init == NULL) {
        return 3;
    }
    if (handle->spi_cmd_data_gpio_deinit == NULL) {
        return 3;
    }
    if (handle->spi_cmd_data_gpio_write == NULL) {
        return 3;
    }
    if (handle->reset_gpio_init == NULL) {
        return 3;
    }
    if (handle->reset_gpio_deinit == NULL) {
        return 3;
    }
    if(handle->reset_gpio_write == NULL) {
        return 3;
    }

    if (handle->spi_cmd_data_gpio_init() != 0) {
        return 5;
    }
    if (handle->reset_gpio_init() != 0) {
        (void)handle->spi_cmd_data_gpio_deinit();

        return 4;
    }
    if (handle->reset_gpio_write(0) != 0) {
        (void)handle->spi_cmd_data_gpio_deinit();
        (void)handle->reset_gpio_deinit();

        return 4;
    }
    handle->delay_ms(100);
    if (handle->reset_gpio_write(1) != 0) {
        (void)handle->spi_cmd_data_gpio_deinit();
        (void)handle->reset_gpio_deinit();

        return 4;
    }
    if (handle->iic_spi == SSD1306_INTERFACE_IIC) {
        if (handle->iic_init() != 0) {
            (void)handle->spi_cmd_data_gpio_deinit();
            (void)handle->reset_gpio_deinit();

            return 1;
        }
    } else if (handle->iic_spi == SSD1306_INTERFACE_SPI) {
        if (handle->spi_init() != 0) {
            (void)handle->spi_cmd_data_gpio_deinit();
            (void)handle->reset_gpio_deinit();

            return 1;
        }
    } else {
        (void)handle->spi_cmd_data_gpio_deinit();
        (void)handle->reset_gpio_deinit();

        return 6;
    }
    handle->inited = 1;

    return 0;
}

/**
 * @brief     close the chip
 * @param[in] *handle points to an ssd1306 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 iic or spi deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 power down failed
 *            - 5 reset gpio deinit failed
 *            - 6 command && data deinit failed
 *            - 7 interface param is invalid
 * @note      none
 */
uint8_t ssd1306_deinit(ssd1306_handle_t *handle)
{
    uint8_t buf[2];

    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }

    buf[0] = SSD1306_CMD_CHARGE_PUMP_SETTING;
    buf[1] = 0x10 | (0 << 2);
    if (a_ssd1306_multiple_write_byte(handle, (uint8_t *)buf, 2, SSD1306_CMD) != 0) {
        return 4;
    }
    if (a_ssd1306_write_byte(handle, SSD1306_CMD_DISPLAY_OFF, SSD1306_CMD) != 0) {
        return 4;
    }
    if (handle->reset_gpio_deinit() != 0) {
        return 5;
    }
    if (handle->spi_cmd_data_gpio_deinit() != 0) {
        return 6;
    }
    if (handle->iic_spi == SSD1306_INTERFACE_IIC) {
        if (handle->iic_deinit() != 0) {
            return 1;
        }
    } else if (handle->iic_spi == SSD1306_INTERFACE_SPI) {
        if (handle->spi_deinit() != 0) {
            return 1;
        }
    } else {
        return 7;
    }
    handle->inited = 0;

    return 0;
}

/**
 * @brief     set the chip interface
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] interface is the chip interface
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t ssd1306_set_interface(ssd1306_handle_t *handle, ssd1306_interface_t interface)
{
    if (handle == NULL) {
        return 2;
    }

    handle->iic_spi = (uint8_t)interface;

    return 0;
}

/**
 * @brief      get the chip interface
 * @param[in]  *handle points to an ssd1306 handle structure
 * @param[out] *interface points to a chip interface buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ssd1306_get_interface(ssd1306_handle_t *handle, ssd1306_interface_t *interface)
{
    if (handle == NULL) {
        return 2;
    }

    *interface = (ssd1306_interface_t)(handle->iic_spi);

    return 0;
}

/**
 * @brief     set the chip iic address
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] addr_pin is the iic address
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 * @note      none
 */
uint8_t ssd1306_set_addr_pin(ssd1306_handle_t *handle, ssd1306_address_t addr_pin)
{
    if (handle == NULL) {
        return 2;
    }

    handle->iic_addr = (uint8_t)addr_pin;

    return 0;
}

/**
 * @brief      get the chip iic address
 * @param[in]  *handle points to an ssd1306 handle structure
 * @param[out] *addr_pin points to an iic address buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ssd1306_get_addr_pin(ssd1306_handle_t *handle, ssd1306_address_t *addr_pin)
{
    if (handle == NULL) {
        return 2;
    }

    *addr_pin = (ssd1306_address_t)(handle->iic_addr);

    return 0;
}

/**
 * @brief     set the low column start address
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] addr is the low column start address
 * @return    status code
 *            - 0 success
 *            - 1 set low column start address failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 addr is invalid
 * @note      addr <= 0xF
 */
uint8_t ssd1306_set_low_column_start_address(ssd1306_handle_t *handle, uint8_t addr)
{
    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }
    if (addr > 0x0F) {
        return 4;
    }

    return a_ssd1306_write_byte(handle, SSD1306_CMD_LOWER_COLUMN_START_ADDRESS|(addr&0x0F), SSD1306_CMD);
}

/**
 * @brief     set the high column start address
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] addr is the high column start address
 * @return    status code
 *            - 0 success
 *            - 1 set high column start address failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 addr is invalid
 * @note      addr <= 0xF
 */
uint8_t ssd1306_set_high_column_start_address(ssd1306_handle_t *handle, uint8_t addr)
{
    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }
    if (addr > 0x0F) {
        return 4;
    }

    return a_ssd1306_write_byte(handle, SSD1306_CMD_HIGHER_COLUMN_START_ADDRESS|(addr&0x0F), SSD1306_CMD);
}

/**
 * @brief     set the memory addressing mode
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] mode is the memory addressing mode
 * @return    status code
 *            - 0 success
 *            - 1 set memory addressing mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1306_set_memory_addressing_mode(ssd1306_handle_t *handle, ssd1306_memory_addressing_mode_t mode)
{
    uint8_t buf[2];

    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }

    buf[0] = SSD1306_CMD_MEMORY_ADDRESSING_MODE;
    buf[1] = mode;

    return a_ssd1306_multiple_write_byte(handle, (uint8_t *)buf, 2, SSD1306_CMD);
}

/**
 * @brief     set the column address range
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] start_addr is the column start address
 * @param[in] end_addr is the column end address
 * @return    status code
 *            - 0 success
 *            - 1 set column address range failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 start addr is invalid
 *            - 5 end addr is invalid
 * @note      start addr and end addr can't be over 0x7F
 */
uint8_t ssd1306_set_column_address_range(ssd1306_handle_t *handle, uint8_t start_addr, uint8_t end_addr)
{
    uint8_t buf[3];

    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }
    if (start_addr > 0x7F) {
        return 4;
    }
    if (end_addr > 0x7F) {
        return 5;
    }

    buf[0] = SSD1306_CMD_SET_COLUMN_ADDRESS;
    buf[1] = start_addr & 0x7F;
    buf[2] = end_addr & 0x7F;

    return a_ssd1306_multiple_write_byte(handle, (uint8_t *)buf, 3, SSD1306_CMD);
}

/**
 * @brief     set the page address range
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] start_addr is the page start address
 * @param[in] end_addr is the page end address
 * @return    status code
 *            - 0 success
 *            - 1 set page address range failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 start addr is invalid
 *            - 5 end addr is invalid
 * @note      start addr and end addr can't be over 0x07
 */
uint8_t ssd1306_set_page_address_range(ssd1306_handle_t *handle, uint8_t start_addr, uint8_t end_addr)
{
    uint8_t buf[3];

    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }
    if (start_addr > 0x07) {
        return 4;
    }
    if (end_addr > 0x07) {
        return 5;
    }

    buf[0] = SSD1306_CMD_SET_PAGE_ADDRESS;
    buf[1] = start_addr & 0x07;
    buf[2] = end_addr & 0x07;

    return a_ssd1306_multiple_write_byte(handle, (uint8_t *)buf, 3, SSD1306_CMD);
}

/**
 * @brief     set the fade blinking mode
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] mode is the fade blinking mode
 * @param[in] frames is the fade or blinking frames
 * @return    status code
 *            - 0 success
 *            - 1 set fade blinking mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 frames is invalid
 * @note      frames max is 0x0F and div is (frames + 1) * 8
 */
uint8_t ssd1306_set_fade_blinking_mode(ssd1306_handle_t *handle, ssd1306_fade_blinking_mode_t mode, uint8_t frames)
{
    uint8_t buf[2];

    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }
    if (frames> 0x0F) {
        return 4;
    }

    buf[0] = SSD1306_CMD_SET_FADE_OUT_AND_BLINKING;
    buf[1] = (uint8_t)((mode << 4) | (frames & 0x0F));

    return a_ssd1306_multiple_write_byte(handle, (uint8_t *)buf, 2, SSD1306_CMD);
}

/**
 * @brief     set the right horizontal scroll
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] start_page_addr is the start page address
 * @param[in] end_page_addr is the end page address
 * @param[in] frames is the scroll frames
 * @return    status code
 *            - 0 success
 *            - 1 set right horizontal scroll failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 start page addr is invalid
 *            - 5 end page addr is invalid
 * @note       start_page_addr <= 0x07, end_page_addr <= 0x07
 */
uint8_t ssd1306_set_right_horizontal_scroll(ssd1306_handle_t *handle, uint8_t start_page_addr, uint8_t end_page_addr,
                                            ssd1306_scroll_frame_t frames)
{
    uint8_t buf[7];

    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }
    if (start_page_addr > 0x07) {
        return 4;
    }
    if (end_page_addr > 0x07) {
        return 5;
    }

    buf[0] = SSD1306_CMD_RIGHT_HORIZONTAL_SCROLL;
    buf[1] = 0x00;
    buf[2] = start_page_addr & 0x07;
    buf[3] = frames & 0x07;
    buf[4] = end_page_addr & 0x07;
    buf[5] = 0x00;
    buf[6] = 0xFF;

    return a_ssd1306_multiple_write_byte(handle, (uint8_t *)buf, 7, SSD1306_CMD);
}

/**
 * @brief     set the left horizontal scroll
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] start_page_addr is the start page address
 * @param[in] end_page_addr is the end page address
 * @param[in] frames is the scroll frames
 * @return    status code
 *            - 0 success
 *            - 1 set left horizontal scroll failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 start_page_addr is invalid
 *            - 5 end_page_addr is invalid
 * @note      start_page_addr <= 0x07, end_page_addr <= 0x07
 */
uint8_t ssd1306_set_left_horizontal_scroll(ssd1306_handle_t *handle, uint8_t start_page_addr, uint8_t end_page_addr,
                                           ssd1306_scroll_frame_t frames)
{
    uint8_t buf[7];

    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }
    if (start_page_addr > 0x07) {
        return 4;
    }
    if (end_page_addr > 0x07) {
        return 5;
    }

    buf[0] = SSD1306_CMD_LEFT_HORIZONTAL_SCROLL;
    buf[1] = 0x00;
    buf[2] = start_page_addr & 0x07;
    buf[3] = frames & 0x07;
    buf[4] = end_page_addr & 0x07;
    buf[5] = 0x00;
    buf[6] = 0xFF;

    return a_ssd1306_multiple_write_byte(handle, (uint8_t *)buf, 7, SSD1306_CMD);
}

/**
 * @brief     set the vertical right horizontal scroll
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] start_page_addr is the start page address
 * @param[in] end_page_addr is the end page address
 * @param[in] rows is the row address
 * @param[in] frames is the scroll frames
 * @return    status code
 *            - 0 success
 *            - 1 set vertical right horizontal scroll failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 start_page_addr is invalid
 *            - 5 end_page_addr is invalid
 *            - 6 rows is invalid
 * @note      start_page_addr <= 0x07, end_page_addr <= 0x07, rows <= 0x3F
 */
uint8_t ssd1306_set_vertical_right_horizontal_scroll(ssd1306_handle_t *handle, uint8_t start_page_addr, uint8_t end_page_addr,
                                                     uint8_t rows, ssd1306_scroll_frame_t frames)
{
    uint8_t buf[6];

    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }
    if (start_page_addr > 0x07) {
        return 4;
    }
    if (end_page_addr > 0x07) {
        return 5;
    }
    if (rows > 0x3F) {
        return 6;
    }

    buf[0] = SSD1306_CMD_VERTICAL_RIGHT_HORIZONTAL_SCROLL;
    buf[1] = 0x00;
    buf[2] = start_page_addr & 0x07;
    buf[3] = frames & 0x07;
    buf[4] = end_page_addr & 0x07;
    buf[5] = rows & 0x3F;

    return a_ssd1306_multiple_write_byte(handle, (uint8_t *)buf, 6, SSD1306_CMD);
}

/**
 * @brief     set the vertical left horizontal scroll
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] start_page_addr is the start page address
 * @param[in] end_page_addr is the end page address
 * @param[in] rows is the row address
 * @param[in] frames is the scroll frames
 * @return    status code
 *            - 0 success
 *            - 1 set vertical left horizontal scroll failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 start_page_addr is invalid
 *            - 5 end_page_addr is invalid
 *            - 6 rows is invalid
 * @note      start_page_addr <= 0x07, end_page_addr <= 0x07, rows <= 0x3F
 */
uint8_t ssd1306_set_vertical_left_horizontal_scroll(ssd1306_handle_t *handle, uint8_t start_page_addr, uint8_t end_page_addr,
                                                    uint8_t rows, ssd1306_scroll_frame_t frames)
{
    uint8_t buf[6];

    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }
    if (start_page_addr > 0x07) {
        return 4;
    }
    if (end_page_addr > 0x07) {
        return 5;
    }
    if (rows > 0x3F) {
        return 6;
    }

    buf[0] = SSD1306_CMD_VERTICAL_LEFT_HORIZONTAL_SCROLL;
    buf[1] = 0x00;
    buf[2] = start_page_addr & 0x07;
    buf[3] = frames & 0x07;
    buf[4] = end_page_addr & 0x07;
    buf[5] = rows & 0x3F;

    return a_ssd1306_multiple_write_byte(handle, (uint8_t *)buf, 6, SSD1306_CMD);
}

/**
 * @brief     deactivate the scroll
 * @param[in] *handle points to an ssd1306 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 deactivate scroll failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1306_deactivate_scroll(ssd1306_handle_t *handle)
{
    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }

    return a_ssd1306_write_byte(handle, SSD1306_CMD_DEACTIVATE_SCROLL, SSD1306_CMD);
}

/**
 * @brief     activate the scroll
 * @param[in] *handle points to an ssd1306 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 activate scroll failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1306_activate_scroll(ssd1306_handle_t *handle)
{
    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }

    return a_ssd1306_write_byte(handle, SSD1306_CMD_ACTIVATE_SCROLL, SSD1306_CMD);
}

/**
 * @brief     set the display start line
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] l is the start line
 * @return    status code
 *            - 0 success
 *            - 1 set display start line failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 line is invalid
 * @note      line <= 0x3F
 */
uint8_t ssd1306_set_display_start_line(ssd1306_handle_t *handle, uint8_t l)
{
    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }
    if (l > 0x3F) {
        return 4;
    }

    return a_ssd1306_write_byte(handle, SSD1306_CMD_DISPLAY_START_LINE|(l&0x3F), SSD1306_CMD);
}

/**
 * @brief     set the display contrast
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] contrast is the display contrast
 * @return    status code
 *            - 0 success
 *            - 1 set contrast failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1306_set_contrast(ssd1306_handle_t *handle, uint8_t contrast)
{
    uint8_t buf[2];

    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }

    buf[0] = SSD1306_CMD_CONTRAST_CONTROL;
    buf[1] = contrast;

    return a_ssd1306_multiple_write_byte(handle, (uint8_t *)buf, 2, SSD1306_CMD);
}

/**
 * @brief     enable or disable the charge pump
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set charge pump failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1306_set_charge_pump(ssd1306_handle_t *handle, ssd1306_charge_pump_t enable)
{
    uint8_t buf[2];

    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }

    buf[0] = SSD1306_CMD_CHARGE_PUMP_SETTING;
    buf[1] = (uint8_t)(0x10 | (enable << 2));

    return a_ssd1306_multiple_write_byte(handle, (uint8_t *)buf, 2, SSD1306_CMD);
}

/**
 * @brief     set the segment remap
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] remap is the segment remap param
 * @return    status code
 *            - 0 success
 *            - 1 set segment remap failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1306_set_segment_remap(ssd1306_handle_t *handle, ssd1306_segment_column_remap_t remap)
{
    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }

    if (remap != 0) {
        return a_ssd1306_write_byte(handle, SSD1306_CMD_COLUMN_127_MAPPED_TO_SEG0, SSD1306_CMD);
    } else {
        return a_ssd1306_write_byte(handle, SSD1306_CMD_COLUMN_0_MAPPED_TO_SEG0, SSD1306_CMD);
    }
}

/**
 * @brief     set the vertical scroll area
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] start_row is the start row
 * @param[in] end_row is the end row
 * @return    status code
 *            - 0 success
 *            - 1 set vertical scroll area failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 start_row is invalid
 *            - 5 end_row is invalid
 *            - 6 end_row > start_row
 * @note      start_row <= 0x3F, end_row <= 0x7F, start_row >= end_row
 */
uint8_t ssd1306_set_vertical_scroll_area(ssd1306_handle_t *handle, uint8_t start_row, uint8_t end_row)
{
    uint8_t buf[3];

    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }
    if (start_row > 0x3F) {
        return 4;
    }
    if (end_row > 0x7F) {
        return 5;
    }
    if (end_row > start_row) {
        return 6;
    }

    buf[0] = SSD1306_CMD_VERTICAL_SCROLL_AREA;
    buf[1] = start_row;
    buf[2] = end_row;

    return a_ssd1306_multiple_write_byte(handle, (uint8_t *)buf, 3, SSD1306_CMD);
}

/**
 * @brief     enable or disable the entire display
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set entire display failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1306_set_entire_display(ssd1306_handle_t *handle, ssd1306_entire_display_t enable)
{
    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }

    if (enable != 0) {
        return a_ssd1306_write_byte(handle, SSD1306_CMD_ENTIRE_DISPLAY_ON, SSD1306_CMD);
    } else {
        return a_ssd1306_write_byte(handle, SSD1306_CMD_ENTIRE_DISPLAY_OFF, SSD1306_CMD);
    }
}

/**
 * @brief     set the display mode
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] mode is the display mode
 * @return    status code
 *            - 0 success
 *            - 1 set display mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1306_set_display_mode(ssd1306_handle_t *handle, ssd1306_display_mode_t mode)
{
    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }

    if (mode != 0) {
        return a_ssd1306_write_byte(handle, SSD1306_CMD_INVERSE_DISPLAY, SSD1306_CMD);
    } else {
        return a_ssd1306_write_byte(handle, SSD1306_CMD_NORMAL_DISPLAY, SSD1306_CMD);
    }
}

/**
 * @brief     set the multiplex ratio
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] multiplex is the multiplex ratio
 * @return    status code
 *            - 0 success
 *            - 1 set multiplex ratio failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 multiplex is too small
 *            - 5 multiplex is too large
 * @note      multiplex must be over 0x0E and less than 0x40
 */
uint8_t ssd1306_set_multiplex_ratio(ssd1306_handle_t *handle, uint8_t multiplex)
{
    uint8_t buf[2];

    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }
    if (multiplex < 0x0F) {
        return 4;
    }
    if (multiplex > 0x3F) {
        return 5;
    }

    buf[0] = SSD1306_CMD_MULTIPLEX_RATIO ;
    buf[1] = multiplex;

    return a_ssd1306_multiple_write_byte(handle, (uint8_t *)buf, 2, SSD1306_CMD);
}

/**
 * @brief     enable or disable the display
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] on_off is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set display failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1306_set_display(ssd1306_handle_t *handle, ssd1306_display_t on_off)
{
    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }

    if (on_off != 0) {
        return a_ssd1306_write_byte(handle, SSD1306_CMD_DISPLAY_ON, SSD1306_CMD);
    } else {
        return a_ssd1306_write_byte(handle, SSD1306_CMD_DISPLAY_OFF, SSD1306_CMD);
    }
}

/**
 * @brief     set the page address
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] addr is the page address
 * @return    status code
 *            - 0 success
 *            - 1 set page address failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 addr is invalid
 * @note      addr <= 0x07
 */
uint8_t ssd1306_set_page_address(ssd1306_handle_t *handle, uint8_t addr)
{
    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }
    if (addr > 0x07) {
        return 4;
    }

    return a_ssd1306_write_byte(handle, SSD1306_CMD_PAGE_ADDR|(addr&0x07), SSD1306_CMD);
}

/**
 * @brief     set the scan direction
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] dir is the scan direction
 * @return    status code
 *            - 0 success
 *            - 1 set scan direction failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1306_set_scan_direction(ssd1306_handle_t *handle, ssd1306_scan_direction_t dir)
{
    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }

    if (dir != 0) {
        return a_ssd1306_write_byte(handle, SSD1306_CMD_SCAN_DIRECTION_COMN_1_START, SSD1306_CMD);
    } else {
        return a_ssd1306_write_byte(handle, SSD1306_CMD_SCAN_DIRECTION_COM0_START, SSD1306_CMD);
    }
}

/**
 * @brief     set the display offset
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] offset is the display offset
 * @return    status code
 *            - 0 success
 *            - 1 set display offset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 offset is invalid
 * @note      offset <= 0x3F
 */
uint8_t ssd1306_set_display_offset(ssd1306_handle_t *handle, uint8_t offset)
{
    uint8_t buf[2];

    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }
    if (offset > 0x3F) {
        return 4;
    }

    buf[0] = SSD1306_CMD_DISPLAY_OFFSET ;
    buf[1] = offset;

    return a_ssd1306_multiple_write_byte(handle, (uint8_t *)buf, 2, SSD1306_CMD);
}

/**
 * @brief     set the display clock
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] oscillator_frequency is the oscillator frequency
 * @param[in] clock_divide is the clock divide
 * @return    status code
 *            - 0 success
 *            - 1 set display clock failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 oscillator frequency is invalid
 *            - 5 clock divide is invalid
 * @note      oscillator_frequency <= 0x0F, clock_divide <= 0x0F
 */
uint8_t ssd1306_set_display_clock(ssd1306_handle_t *handle, uint8_t oscillator_frequency, uint8_t clock_divide)
{
    uint8_t buf[2];

    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }
    if (oscillator_frequency> 0x0F) {
        return 4;
    }
    if (clock_divide> 0x0F) {
        return 5;
    }

    buf[0] = SSD1306_CMD_DISPLAY_CLOCK_DIVIDE ;
    buf[1] = (oscillator_frequency<<4) | clock_divide;

    return a_ssd1306_multiple_write_byte(handle, (uint8_t *)buf, 2, SSD1306_CMD);
}

/**
 * @brief     set the display zoom in
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] zoom is the display zoom in
 * @return    status code
 *            - 0 success
 *            - 1 set zoom in failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1306_set_zoom_in(ssd1306_handle_t *handle, ssd1306_zoom_in_t zoom)
{
    uint8_t buf[2];

    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }

    buf[0] = SSD1306_CMD_SET_ZOOM_IN ;
    buf[1] = zoom;

    return a_ssd1306_multiple_write_byte(handle, (uint8_t *)buf, 2, SSD1306_CMD);
}

/**
 * @brief     set the pre charge period
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] phase1_period is the phase1 period
 * @param[in] phase2_period is the phase2 period
 * @return    status code
 *            - 0 success
 *            - 1 set pre charge period failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 phase1 period is invalid
 *            - 5 phase2 period is invalid
 * @note      phase1_period <= 0x0F, phase2_period <= 0x0F
 */
uint8_t ssd1306_set_precharge_period(ssd1306_handle_t *handle, uint8_t phase1_period, uint8_t phase2_period)
{
    uint8_t buf[2];

    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }
    if (phase1_period> 0x0F) {
        return 4;
    }
    if (phase2_period> 0x0F) {
        return 5;
    }

    buf[0] = SSD1306_CMD_PRE_CHARGE_PERIOD;
    buf[1] = (phase2_period << 4) | phase1_period;

    return a_ssd1306_multiple_write_byte(handle, (uint8_t *)buf, 2, SSD1306_CMD);
}

/**
 * @brief     set the hardware com pins
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] conf is the pin conf
 * @param[in] remap is the left right remap
 * @return    status code
 *            - 0 success
 *            - 1 set com pins hardware conf failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1306_set_com_pins_hardware_conf(ssd1306_handle_t *handle, ssd1306_pin_conf_t conf, ssd1306_left_right_remap_t remap)
{
    uint8_t buf[2];

    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }

    buf[0] = SSD1306_CMD_COM_PINS_CONF;
    buf[1] = (uint8_t)((conf<<4) | (remap<<5) |0x02);

    return a_ssd1306_multiple_write_byte(handle, (uint8_t *)buf, 2, SSD1306_CMD);
}

/**
 * @brief     set the deselect level
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] level is the deselect level
 * @return    status code
 *            - 0 success
 *            - 1 set deselect level failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1306_set_deselect_level(ssd1306_handle_t *handle, ssd1306_deselect_level_t level)
{
    uint8_t buf[2];

    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }

    buf[0] = SSD1306_CMD_COMH_DESLECT_LEVEL;
    buf[1] = (uint8_t)(level << 4);

    return a_ssd1306_multiple_write_byte(handle, (uint8_t *)buf, 2, SSD1306_CMD);
}

/**
 * @brief     write the register command
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1306_write_cmd(ssd1306_handle_t *handle, uint8_t *buf, uint8_t len)
{
    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }

    return a_ssd1306_multiple_write_byte(handle, (uint8_t *)buf, len, SSD1306_CMD);
}

/**
 * @brief     write the register data
 * @param[in] *handle points to an ssd1306 handle structure
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ssd1306_write_data(ssd1306_handle_t *handle, uint8_t *buf, uint8_t len)
{
    if (handle == NULL) {
        return 2;
    }
    if (handle->inited != 1) {
        return 3;
    }

    return a_ssd1306_multiple_write_byte(handle, (uint8_t *)buf, len, SSD1306_DATA);
}

/**
 * @brief      get chip's information
 * @param[out] *info points to an ssd1306 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ssd1306_info(ssd1306_info_t *info)
{
    if (info == NULL) {
        return 2;
    }

    memset(info, 0, sizeof(ssd1306_info_t));
    strncpy(info->chip_name, CHIP_NAME, 32);
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);
    strncpy(info->interface, "IIC SPI", 8);
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;
    info->max_current_ma = MAX_CURRENT;
    info->temperature_max = TEMPERATURE_MAX;
    info->temperature_min = TEMPERATURE_MIN;
    info->driver_version = DRIVER_VERSION;

    return 0;
}
