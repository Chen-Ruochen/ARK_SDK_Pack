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

//4:debug, 3:info, 2:warn, 1:error
#ifndef LOGGER_LEVEL
#define LOGGER_LEVEL            (4)
#endif

#ifndef LOGGER_LENGTH
#define LOGGER_LENGTH           (256)
#endif

typedef ark_err_t   (*mutex_init)(void);
typedef ark_err_t   (*mutex_set)(bool lock);
typedef ark_err_t   (*mutex_get)(void);
typedef void        (*serial_transmit)(void *data, uint16_t len);

ark_err_t Logger_Init(mutex_init init, mutex_set set, mutex_get get, serial_transmit tx);
void Logger_Debug(const char *tag, const char *fmt, ...);
void Logger_Info(const char *tag, const char *fmt, ...);
void Logger_Warn(const char *tag, const char *fmt, ...);
void Logger_Error(const char *tag, const char *fmt, ...);
void Logger_Raw(const char *fmt, ...);


#ifdef __cplusplus
}
#endif
#endif // __MODULE_LOGGER_H