/**********************************
 * @Description  : AT24Cxx驱动文件
 * @Author       : Jason Chen
 * @Date         : 2023-06-18 21:59:34
 * @FilePath     : at24cxx.h
 * @Copyright 2023 Jason Chen, All Rights Reserved.
 **********************************/
#ifndef __AT24CXX_H
#define __AT24CXX_H
#include "common_lib.h"

typedef enum at24c_err_e {
    AT24C_OK        = 0,
    AT24C_ERROR
} at24c_err_t;

typedef enum at24cxx_type_e {
    AT24C_01        = 0,
    AT24C_02,
    AT24C_04,
    AT24C_08,
    AT24C_16,
    AT24C_32,
    AT24C_64,
    AT24C_128,
    AT24C_256,
    AT24C_512
} at24cxx_type_t;

typedef enum at24cxx_status_e {
    AT24C_INIT_FAILED,
    AT24C_NORMAL
} at24cxx_status_t;

typedef struct at24cxx_dev_s {
    /* Status Flag 用户无需配置 */
    at24cxx_status_t    status;
    /* Max Page 用户无需配置 */
    uint16_t            max_page;
    /* Bytes In Page 用户无需配置 */
    uint16_t            bytes_in_page;
    /* Offset 用户无需配置 */
    uint8_t             offset;
    /* Type 用户需实现 */
    at24cxx_type_t      type;
    /* Delay ms 用户需实现 */
    void                (*delay_ms)(uint32_t delay);
    /* AT24CXX I2C Write 使用AT24C04及更大容量的EEPROM 寻址为16bit!!! */
    at24c_err_t         (*iic_write)(uint16_t regAddr, uint8_t *data, uint16_t len);
    /* AT24CXX I2C Read 使用AT24C04及更大容量的EEPROM 寻址为16bit!!! */
    at24c_err_t         (*iic_read)(uint16_t regAddr, uint8_t *data, uint16_t len);
} at24cxx_dev_t;

at24c_err_t at24cxx_init(at24cxx_dev_t *dev);
at24cxx_status_t at24cxx_get_status(at24cxx_dev_t *dev);
at24c_err_t at24cxx_write_one_byte(at24cxx_dev_t *dev, uint16_t start_page, uint8_t offset, uint8_t data);
at24c_err_t at24cxx_read_one_byte(at24cxx_dev_t *dev, uint16_t start_page, uint8_t offset, uint8_t *data);
at24c_err_t at24cxx_write_muti_byte(at24cxx_dev_t *dev, uint16_t start_page, uint8_t offset, uint8_t *data, uint16_t len);
at24c_err_t at24cxx_read_muti_byte(at24cxx_dev_t *dev, uint16_t start_page, uint8_t offset, uint8_t *data, uint16_t len);

#endif