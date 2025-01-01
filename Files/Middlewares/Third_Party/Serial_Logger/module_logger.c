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
    queue_init qinit;
    queue_push qpush;
    queue_pop qpop;
    serial_transmit tx;
    uint8_t logbuf[LOGGER_BUF_SIZE];
} logger_t;

static logger_t s_logger;

ark_err_t Logger_Init(void *ptr, uint16_t depth, uint16_t length, queue_init init, queue_push push, queue_pop pop, serial_transmit tx)
{
    s_logger.qinit = init;
    s_logger.qpush = push;
    s_logger.qpop = pop;
    s_logger.tx = tx;
    if (s_logger.qinit(ptr, depth, length) != ARK_OK) {
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

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);

    // 拼凑前部分[DBG][TAG] 然后调用队列push函数
    char formatted_msg[128];
    vsnprintf(formatted_msg, sizeof(formatted_msg), fmt, args);
    snprintf((char *)s_logger.logbuf, sizeof(s_logger.logbuf), "[DBG][%s]:%s", tag, formatted_msg);
    s_logger.qpush(s_logger.logbuf);

    va_end(args);
#endif
}

void Logger_Info(const char *tag, const char *fmt, ...)
{
#if LOGGER_LEVEL >= 3
    if (!s_logger.log_init) {
        return;
    }

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);

    // 拼凑前部分[INFO][TAG] 然后调用队列push函数
    char formatted_msg[128];
    vsnprintf(formatted_msg, sizeof(formatted_msg), fmt, args);
    snprintf((char *)s_logger.logbuf, sizeof(s_logger.logbuf), "\033[32m[INF]\033[0m[%s]:%s", tag, formatted_msg);
    s_logger.qpush(s_logger.logbuf);

    va_end(args);
#endif
}

void Logger_Warn(const char *tag, const char *fmt, ...)
{
#if LOGGER_LEVEL >= 2
    if (!s_logger.log_init) {
        return;
    }

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);

    // 拼凑前部分[WARN][TAG] 然后调用队列push函数
    char formatted_msg[128];
    vsnprintf(formatted_msg, sizeof(formatted_msg), fmt, args);
    snprintf((char *)s_logger.logbuf, sizeof(s_logger.logbuf), "\033[33m[WRN]\033[0m[%s]:%s", tag, formatted_msg);
    s_logger.qpush(s_logger.logbuf);

    va_end(args);
#endif
}

void Logger_Error(const char *tag, const char *fmt, ...)
{
#if LOGGER_LEVEL >= 1
    if (!s_logger.log_init) {
        return;
    }

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);

    // 拼凑前部分[ERR][TAG] 然后调用队列push函数
    char formatted_msg[128];
    vsnprintf(formatted_msg, sizeof(formatted_msg), fmt, args);
    snprintf((char *)s_logger.logbuf, sizeof(s_logger.logbuf), "\033[31m[ERR]\033[0m[%s]:%s", tag, formatted_msg);
    s_logger.qpush(s_logger.logbuf);

    va_end(args);
#endif
}

void Logger_State_Machine(void)
{
    if (!s_logger.log_init) {
        return;
    }

    uint8_t logbuf[LOGGER_BUF_SIZE];
    //调用队列pop函数 然后调用串口发送函数
    if (s_logger.qpop(&logbuf) == ARK_OK) {
        s_logger.tx(&logbuf, strlen((char *)s_logger.logbuf));
    }
}
