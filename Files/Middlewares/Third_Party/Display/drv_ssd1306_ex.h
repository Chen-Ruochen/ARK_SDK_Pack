/**********************************
 * @Description  : SSD1306驱动扩展头文件
 * @Author       : Jason Chen
 * @Date         : 2024-12-17 13:55:46
 * @FilePath     : drv_ssd1306_ex.h
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
#ifndef __DRV_SSD1306_ADVANCE_H
#define __DRV_SSD1306_ADVANCE_H

#include "common_lib.h"
#include "drv_ssd1306.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @addtogroup ssd1306_example_driver
 * @{
 */

/**
 * @brief ssd1306 advance example default definition
 */
#define SSD1306_ADVANCE_DEFAULT_DESELECT_LEVEL                   SSD1306_DESELECT_LEVEL_0P77                /**< set deselect level 0.77 */
#define SSD1306_ADVANCE_DEFAULT_LEFT_RIGHT_REMAP                 SSD1306_LEFT_RIGHT_REMAP_DISABLE           /**< disable remap */
#define SSD1306_ADVANCE_DEFAULT_PIN_CONF                         SSD1306_PIN_CONF_ALTERNATIVE               /**< set alternative */
#define SSD1306_ADVANCE_DEFAULT_PHASE1_PERIOD                    0x01                                       /**< set phase 1 */
#define SSD1306_ADVANCE_DEFAULT_PHASE2_PERIOD                    0x0F                                       /**< set phase F */
#define SSD1306_ADVANCE_DEFAULT_OSCILLATOR_FREQUENCY             0x08                                       /**< set 8 */
#define SSD1306_ADVANCE_DEFAULT_CLOCK_DIVIDE                     0x00                                       /**< set clock div 0 */
#define SSD1306_ADVANCE_DEFAULT_DISPLAY_OFFSET                   0x00                                       /**< set display offset */
#define SSD1306_ADVANCE_DEFAULT_MULTIPLEX_RATIO                  0x3F                                       /**< set ratio */
#define SSD1306_ADVANCE_DEFAULT_DISPLAY_MODE                     SSD1306_DISPLAY_MODE_NORMAL                /**< set normal mode */
#define SSD1306_ADVANCE_DEFAULT_SCAN_DIRECTION                   SSD1306_SCAN_DIRECTION_COMN_1_START        /**< set scan 1 */
#define SSD1306_ADVANCE_DEFAULT_SEGMENT                          SSD1306_SEGMENT_COLUMN_ADDRESS_127         /**< set column 127 */
#define SSD1306_ADVANCE_DEFAULT_CONTRAST                         0xCF                                       /**< set contrast CF */
#define SSD1306_ADVANCE_DEFAULT_ZOOM_IN                          SSD1306_ZOOM_IN_DISABLE                    /**< disable zoom in */
#define SSD1306_ADVANCE_DEFAULT_FADE_BLINKING_MODE               SSD1306_FADE_BLINKING_MODE_DISABLE         /**< disable fade */
#define SSD1306_ADVANCE_DEFAULT_FADE_FRAMES                      0x00                                       /**< set frame 0 */
#define SSD1306_ADVANCE_DEFAULT_DISPLAY_START_LINE               0x00                                       /**< set start line 0 */
#define SSD1306_ADVANCE_DEFAULT_HIGH_COLUMN_START_ADDRESS        0x00                                       /**< set high start 0 */
#define SSD1306_ADVANCE_DEFAULT_LOW_COLUMN_START_ADDRESS         0x00                                       /**< set low start 0 */
#define SSD1306_ADVANCE_DEFAULT_PAGE_ADDRESS_RANGE_START         0x00                                       /**< set page range start */
#define SSD1306_ADVANCE_DEFAULT_PAGE_ADDRESS_RANGE_END           0x07                                       /**< set page range end */
#define SSD1306_ADVANCE_DEFAULT_COLUMN_ADDRESS_RANGE_START       0x00                                       /**< set range start */
#define SSD1306_ADVANCE_DEFAULT_COLUMN_ADDRESS_RANGE_END         0x7F                                       /**< set range end */

/**
 * @brief     advance example init
 * @param[in] interface is the interface type
 * @param[in] addr is the iic device address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t ssd1306_advance_init(ssd1306_interface_t interface, ssd1306_address_t addr);

/**
 * @brief  advance example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ssd1306_advance_deinit(void);

/**
 * @brief  advance example display on
 * @return status code
 *         - 0 success
 *         - 1 display on failed
 * @note   none
 */
uint8_t ssd1306_advance_display_on(void);

/**
 * @brief  advance example display off
 * @return status code
 *         - 0 success
 *         - 1 display off failed
 * @note   none
 */
uint8_t ssd1306_advance_display_off(void);

/**
 * @brief  advance example clear
 * @return status code
 *         - 0 success
 *         - 1 clear failed
 * @note   none
 */
uint8_t ssd1306_advance_clear(void);

/**
 * @brief     advance example write a point
 * @param[in] x is the coordinate x
 * @param[in] y is the coordinate y
 * @param[in] data is the written data
 * @return    status code
 *            - 0 success
 *            - 1 write point failed
 * @note      none
 */
uint8_t ssd1306_advance_write_point(uint8_t x, uint8_t y, uint8_t data);

/**
 * @brief      advance example read a point
 * @param[in]  x is the coordinate x
 * @param[in]  y is the coordinate y
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read point failed
 * @note       none
 */
uint8_t ssd1306_advance_read_point(uint8_t x, uint8_t y, uint8_t *data);

/**
 * @brief     advance example fill a rectangle
 * @param[in] left is the left coordinate x
 * @param[in] top is the top coordinate y
 * @param[in] right is the right coordinate x
 * @param[in] bottom is the bottom coordinate y
 * @param[in] color is the display color
 * @return    status code
 *            - 0 success
 *            - 1 fill rect failed
 * @note      none
 */
uint8_t ssd1306_advance_rect(uint8_t left, uint8_t top, uint8_t right, uint8_t bottom, uint8_t color);

/**
 * @brief     advance example draw a picture
 * @param[in] left is the left coordinate x
 * @param[in] top is the top coordinate y
 * @param[in] right is the right coordinate x
 * @param[in] bottom is the bottom coordinate y
 * @param[in] *img points to a image buffer
 * @return    status code
 *            - 0 success
 *            - 1 draw picture failed
 * @note      none
 */
uint8_t ssd1306_advance_picture(uint8_t left, uint8_t top, uint8_t right, uint8_t bottom, uint8_t *img);

/**
 * @brief  advance example enable the zoom in
 * @return status code
 *         - 0 success
 *         - 1 enable zoom in failed
 * @note   none
 */
uint8_t ssd1306_advance_enable_zoom_in(void);

/**
 * @brief  advance example disable the zoom in
 * @return status code
 *         - 0 success
 *         - 1 disable zoom in failed
 * @note   none
 */
uint8_t ssd1306_advance_disable_zoom_in(void);

/**
 * @brief     advance example set the fade blinking mode
 * @param[in] mode is the fade blinking mode
 * @param[in] frames is the fade or blinking frames
 * @return    status code
 *            - 0 success
 *            - 1 set fade blinking mode failed
 * @note      frames max is 0x0F and div is (frames + 1) * 8
 */
uint8_t ssd1306_advance_fade_blinking(ssd1306_fade_blinking_mode_t mode, uint8_t frames);

/**
 * @brief  advance example deactivate the scroll
 * @return status code
 *         - 0 success
 *         - 1 deactivate scroll failed
 * @note   none
 */
uint8_t ssd1306_advance_deactivate_scroll(void);

/**
 * @brief     advance example enable the left horizontal scroll
 * @param[in] start_page_addr is the start page address
 * @param[in] end_page_addr is the end page address
 * @param[in] rows is the row address
 * @param[in] frames is the scroll frames
 * @return    status code
 *            - 0 success
 *            - 1 enable left horizontal scroll failed
 * @note      start_page_addr <= 0x07, end_page_addr <= 0x07, rows <= 0x3F
 */
uint8_t ssd1306_advance_vertical_left_horizontal_scroll(uint8_t start_page_addr, uint8_t end_page_addr, uint8_t rows,
                                                        ssd1306_scroll_frame_t frames);

/**
 * @brief     advance example enable the right horizontal scroll
 * @param[in] start_page_addr is the start page address
 * @param[in] end_page_addr is the end page address
 * @param[in] rows is the row address
 * @param[in] frames is the scroll frames
 * @return    status code
 *            - 0 success
 *            - 1 enable right horizontal scroll failed
 * @note      start_page_addr <= 0x07, end_page_addr <= 0x07, rows <= 0x3F
 */
uint8_t ssd1306_advance_vertical_right_horizontal_scroll(uint8_t start_page_addr, uint8_t end_page_addr, uint8_t rows,
                                                         ssd1306_scroll_frame_t frames);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
