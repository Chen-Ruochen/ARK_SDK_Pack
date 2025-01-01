/**********************************
 * @Description  : Logger模块头文件
 * @Author       : Jason Chen
 * @Date         : 2024-12-30 11:50:38
 * @FilePath     : module_logger.h
 * @Version      : V1.0
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
#ifndef __MODULE_LOGGER_H
#define __MODULE_LOGGER_H

#include "common_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LOGGERLEVEL_DEBUG       4
#define LOGGERLEVEL_INFO        3
#define LOGGERLEVEL_WARN        2
#define LOGGERLEVEL_ERROR       1

#ifndef LOGGER_LEVEL
#define LOGGER_LEVEL            LOGGERLEVEL_DEBUG
#endif
#define LOGGER_BUF_SIZE         1024


typedef ark_err_t   (*queue_init)(void *ptr, uint16_t depth, uint16_t len);
typedef ark_err_t   (*queue_push)(void *data);
typedef ark_err_t   (*queue_pop)(void *data);
typedef void        (*serial_transmit)(void *data, uint16_t len);

ark_err_t Logger_Init(void *ptr, uint16_t depth, uint16_t length, queue_init init, queue_push push, queue_pop pop, serial_transmit tx);
void Logger_Debug(const char *fmt, ...);
void Logger_Info(const char *fmt, ...);
void Logger_Warn(const char *fmt, ...);
void Logger_Error(const char *fmt, ...);
void Logger_State_Machine(void);


#ifdef __cplusplus
}
#endif
#endif // __MODULE_LOGGER_H