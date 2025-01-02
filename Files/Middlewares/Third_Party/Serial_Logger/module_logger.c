/**********************************
 * @Description  : Logger模块源文件
 * @Author       : Jason Chen
 * @Date         : 2024-12-30 11:50:32
 * @FilePath     : module_logger.c
 * @Version      : V1.0
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
#include "module_logger.h"

typedef struct logger_s {
    bool log_init;
    mutex_init minit;
    mutex_set mset;
    mutex_get mget;
    serial_transmit tx;
} logger_t;

static logger_t s_logger;

ark_err_t Logger_Init(mutex_init init, mutex_set set, mutex_get get, serial_transmit tx)
{
    s_logger.minit = init;
    s_logger.mset = set;
    s_logger.mget = get;
    s_logger.tx = tx;
    if (s_logger.minit() != ARK_OK) {
        s_logger.log_init = false;
        return ARK_ERR;
    }
    s_logger.log_init = true;
    return ARK_OK;
}

void Logger_Debug(const char *tag, const char *fmt, ...)
{
#if LOGGER_LEVEL >= 4
    if (!s_logger.log_init) {
        return;
    }

    if (s_logger.mget() != ARK_OK) {
        return;
    }

    s_logger.mset(true);

    uint8_t buf[LOGGER_LENGTH] = {0};
    memcpy(buf, "[DBG]", sizeof("[DBG]") - 1);
    memcpy(buf + sizeof("[DBG]") - 1, tag, strlen(tag));

    va_list args;
    va_start(args, fmt);
    vsnprintf((char *)buf + sizeof("[DBG]") - 1 + strlen(tag), sizeof(buf), fmt, args);
    va_end(args);

    s_logger.tx(buf, strlen((char *)buf));

    s_logger.mset(false);
#endif
}

void Logger_Info(const char *tag, const char *fmt, ...)
{
#if LOGGER_LEVEL >= 3
    if (!s_logger.log_init) {
        return;
    }

    if (s_logger.mget() != ARK_OK) {
        return;
    }

    s_logger.mset(true);

    uint8_t buf[LOGGER_LENGTH] = {0};
    memcpy(buf, "\033[32m[INF]\033[0m", sizeof("\033[32m[INF]\033[0m") - 1);
    memcpy(buf + sizeof("\033[32m[INF]\033[0m") - 1, tag, strlen(tag));

    va_list args;
    va_start(args, fmt);
    vsnprintf((char *)buf + sizeof("\033[32m[INF]\033[0m") - 1 + strlen(tag), sizeof(buf), fmt, args);
    va_end(args);

    s_logger.tx(buf, strlen((char *)buf));

    s_logger.mset(false);
#endif
}

void Logger_Warn(const char *tag, const char *fmt, ...)
{
#if LOGGER_LEVEL >= 2
    if (!s_logger.log_init) {
        return;
    }

    if (s_logger.mget() != ARK_OK) {
        return;
    }

    s_logger.mset(true);

    uint8_t buf[LOGGER_LENGTH] = {0};
    memcpy(buf, "\033[33m[WRN]\033[0m", sizeof("\033[33m[WRN]\033[0m") - 1);
    memcpy(buf + sizeof("\033[33m[WRN]\033[0m") - 1, tag, strlen(tag));

    va_list args;
    va_start(args, fmt);
    vsnprintf((char *)buf + sizeof("\033[33m[WRN]\033[0m") - 1 + strlen(tag), sizeof(buf), fmt, args);
    va_end(args);

    s_logger.tx(buf, strlen((char *)buf));

    s_logger.mset(false);
#endif
}

void Logger_Error(const char *tag, const char *fmt, ...)
{
#if LOGGER_LEVEL >= 1
    if (!s_logger.log_init) {
        return;
    }

    if (s_logger.mget() != ARK_OK) {
        return;
    }

    s_logger.mset(true);

    uint8_t buf[LOGGER_LENGTH] = {0};
    memcpy(buf, "\033[31m[ERR]\033[0m", sizeof("\033[31m[ERR]\033[0m") - 1);
    memcpy(buf + sizeof("\033[31m[ERR]\033[0m") - 1, tag, strlen(tag));

    va_list args;
    va_start(args, fmt);
    vsnprintf((char *)buf + sizeof("\033[31m[ERR]\033[0m") - 1 + strlen(tag), sizeof(buf), fmt, args);
    va_end(args);

    s_logger.tx(buf, strlen((char *)buf));

    s_logger.mset(false);
#endif
}

void Logger_Raw(const char *fmt, ...)
{
#if LOGGER_LEVEL >= 4
    if (!s_logger.log_init) {
        return;
    }

    if (s_logger.mget() != ARK_OK) {
        return;
    }

    s_logger.mset(true);

    uint8_t buf[LOGGER_LENGTH] = {0};

    va_list args;
    va_start(args, fmt);
    vsnprintf((char *)buf, sizeof(buf), fmt, args);
    va_end(args);

    s_logger.tx(buf, strlen((char *)buf));

    s_logger.mset(false);
#endif
}
