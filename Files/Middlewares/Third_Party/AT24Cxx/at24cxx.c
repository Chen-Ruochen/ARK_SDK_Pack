/**********************************
 * @Description  : AT24Cxx驱动文件
 * @Author       : Jason Chen
 * @Date         : 2023-06-18 21:59:34
 * @FilePath     : at24cxx.c
 * @Copyright 2023 Jason Chen, All Rights Reserved.
 **********************************/
#include "at24cxx.h"

static uint16_t at24cxx_cap_address(at24cxx_dev_t *dev, uint16_t page, uint16_t word);
at24c_err_t at24cxx_write_one_byte(at24cxx_dev_t *dev, uint16_t start_page, uint8_t offset, uint8_t data);
at24c_err_t at24cxx_read_one_byte(at24cxx_dev_t *dev, uint16_t start_page, uint8_t offset, uint8_t *data);
at24c_err_t at24cxx_write_muti_byte(at24cxx_dev_t *dev, uint16_t start_page, uint8_t offset, uint8_t *data, uint16_t len);
at24c_err_t at24cxx_read_muti_byte(at24cxx_dev_t *dev, uint16_t start_page, uint8_t offset, uint8_t *data, uint16_t len);

/**********************************
 * @brief AT24Cxx初始化
 * @param *dev 句柄
 * @param type 存储器类型
 * @return 成功 或 失败
 * @note
 **********************************/
at24c_err_t at24cxx_init(at24cxx_dev_t *dev)
{
    switch (dev->type) {
        case AT24C_01:
            dev->max_page = 16;
            dev->bytes_in_page = 8;
            dev->offset = 3;
            break;
        case AT24C_02:
            dev->max_page = 32;
            dev->bytes_in_page = 8;
            dev->offset = 3;
            break;
        case AT24C_04:
            dev->max_page = 32;
            dev->bytes_in_page = 16;
            dev->offset = 4;
            break;
        case AT24C_08:
            dev->max_page = 64;
            dev->bytes_in_page = 16;
            dev->offset = 4;
            break;
        case AT24C_16:
            dev->max_page = 128;
            dev->bytes_in_page = 16;
            dev->offset = 4;
            break;
        case AT24C_32:
            dev->max_page = 128;
            dev->bytes_in_page = 32;
            dev->offset = 5;
            break;
        case AT24C_64:
            dev->max_page = 256;
            dev->bytes_in_page = 32;
            dev->offset = 5;
            break;
        case AT24C_128:
            dev->max_page = 256;
            dev->bytes_in_page = 64;
            dev->offset = 6;
            break;
        case AT24C_256:
            dev->max_page = 512;
            dev->bytes_in_page = 64;
            dev->offset = 6;
            break;
        case AT24C_512:
            dev->max_page = 512;
            dev->bytes_in_page = 128;
            dev->offset = 7;
            break;
        default:
            dev->max_page = 32;
            dev->bytes_in_page = 16;
            dev->offset = 4;
            break;
    }
    if (dev->iic_read == NULL || dev->iic_write == NULL || dev->delay_ms == NULL) {
        dev->status = AT24C_INIT_FAILED;
        return AT24C_ERROR;
    }
    uint8_t init_byte = 0;
    if (dev->iic_read(0, &init_byte, sizeof(init_byte)) != AT24C_OK) {
        dev->status = AT24C_INIT_FAILED;
        return AT24C_ERROR;
    }
    dev->status = AT24C_NORMAL;
    return AT24C_OK;
}
/**********************************
 * @brief 获取AT24Cxx当前状态
 * @param *dev 句柄
 * @return AT24Cxx当前状态
 * @note
 **********************************/
at24cxx_status_t at24cxx_get_status(at24cxx_dev_t *dev)
{
    return dev->status;
}
/**********************************
 * @brief 指定地址偏移写一个字节
 * @param *dev 句柄
 * @param start_page 起始页
 * @param offset 偏移
 * @param data 数据
 * @return 成功或失败
 * @note 偏移量须小于单页最大字节数
 **********************************/
at24c_err_t at24cxx_write_one_byte(at24cxx_dev_t *dev, uint16_t start_page, uint8_t offset, uint8_t data)
{
    if (dev->status != AT24C_NORMAL) {
        return AT24C_ERROR;
    } else {
        if (start_page >= dev->max_page || offset >= dev->bytes_in_page) {
            return AT24C_ERROR;
        }
        uint16_t addr = at24cxx_cap_address(dev, start_page, offset);
        if (dev->iic_write(addr, &data, sizeof(data)) != AT24C_OK) {
            return AT24C_ERROR;
        }
    }

    return AT24C_OK;
}
/**********************************
 * @brief 指定地址偏移读一个字节
 * @param *dev 句柄
 * @param start_page 起始页
 * @param offset 偏移
 * @param *data 数据
 * @return 成功或失败
 * @note 偏移量须小于单页最大字节数
 **********************************/
at24c_err_t at24cxx_read_one_byte(at24cxx_dev_t *dev, uint16_t start_page, uint8_t offset, uint8_t *data)
{
    if (dev->status != AT24C_NORMAL) {
        return AT24C_ERROR;
    } else {
        if (start_page >= dev->max_page || offset >= dev->bytes_in_page) {
            return AT24C_ERROR;
        }
        uint16_t addr = at24cxx_cap_address(dev, start_page, offset);
        uint8_t buf = 0;
        if (dev->iic_read(addr, &buf, sizeof(buf)) != AT24C_OK) {
            return AT24C_ERROR;
        } else {
            *data = buf;
        }
    }

    return AT24C_OK;
}
/**********************************
 * @brief 指定地址偏移写多个字节
 * @param *dev 句柄
 * @param start_page 起始页
 * @param offset 偏移
 * @param *data 数据
 * @param len 数据长度
 * @return 成功或失败
 * @note 偏移量+数据长度须小于单页最大字节数
 **********************************/
at24c_err_t at24cxx_write_muti_byte(at24cxx_dev_t *dev, uint16_t start_page, uint8_t offset, uint8_t *data, uint16_t len)
{
    if (dev->status != AT24C_NORMAL) {
        return AT24C_ERROR;
    } else {
        while (len > 0) {
            if (start_page >= dev->max_page || offset >= dev->bytes_in_page) {
                return AT24C_ERROR;
            }
            uint8_t bytes_2_write = ARK_MIN(dev->bytes_in_page - (offset % dev->bytes_in_page), len);
            uint16_t addr = at24cxx_cap_address(dev, start_page, offset);
            if (dev->iic_write(addr, data, bytes_2_write) != AT24C_OK) {
                return AT24C_ERROR;
            }
            len -= bytes_2_write;
            data += bytes_2_write;
            if (offset + bytes_2_write >= dev->bytes_in_page) {
                start_page++;
                offset = 0;
            }
            dev->delay_ms(10);
        }
    }

    return AT24C_OK;
}
/**********************************
 * @brief 指定地址偏移读多个字节
 * @param *dev 句柄
 * @param start_page 起始页
 * @param offset 偏移
 * @param *data 数据
 * @param len 数据长度
 * @return 成功或失败
 * @note 偏移量+数据长度须小于单页最大字节数
 **********************************/
at24c_err_t at24cxx_read_muti_byte(at24cxx_dev_t *dev, uint16_t start_page, uint8_t offset, uint8_t *data, uint16_t len)
{
    if (dev->status != AT24C_NORMAL) {
        return AT24C_ERROR;
    } else {
        while (len > 0) {
            if (start_page >= dev->max_page || offset >= dev->bytes_in_page) {
                return AT24C_ERROR;
            }
            uint8_t bytes_2_read = ARK_MIN(dev->bytes_in_page - (offset % dev->bytes_in_page), len);
            uint16_t addr = at24cxx_cap_address(dev, start_page, offset);
            if (dev->iic_read(addr, data, bytes_2_read) != AT24C_OK) {
                return AT24C_ERROR;
            }
            len -= bytes_2_read;
            data += bytes_2_read;
            offset = 0;
            start_page++;
        }
    }

    return AT24C_OK;
}
/**********************************
 * @brief 计算存储地址
 * @param *dev 句柄
 * @param page 页
 * @param word 字
 * @return 地址
 * @note
 **********************************/
static uint16_t at24cxx_cap_address(at24cxx_dev_t *dev, uint16_t page, uint16_t word)
{
    return (page << dev->offset | word);
}
