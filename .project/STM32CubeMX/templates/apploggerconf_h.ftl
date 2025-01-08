[#ftl]
[#assign valVersion = "1.0.0"]
[#assign valLevel = 4]
[#assign valLoggerLength = 256]
[#-- SWIPdatas is a list of SWIPconfigModel --]
[#list SWIPdatas as SWIP]
[#if SWIP.defines??]
[#list SWIP.defines as definition]
[#compress]
[#if definition.name == "LOGGER_VERSION"] [#assign valVersion = definition.value] [/#if]
[#if definition.name == "LOGGER_LEVEL"] [#assign valLevel = definition.value] [/#if]
[#if definition.name == "LOGGER_LENGTH"] [#assign valLoggerLength = definition.value] [/#if]
[/#compress]
[/#list]
[/#if]
[/#list]
/**********************************
 * @Description  : Logger模块配置文件
 * @Author       : Jason Chen
 * @Date         : 2024-12-30 11:50:38
 * @FilePath     : app_logger_conf.h
 * @Version      : ${valVersion}
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
#ifndef __APP_LOGGER_CONF_H
#define __APP_LOGGER_CONF_H

#define LOGGER_LEVEL        (${valLevel})
#define LOGGER_LENGTH       (${valLoggerLength})

#endif // __APP_LOGGER_CONF_H