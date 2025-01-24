[#ftl]
[#assign valVersion = "1.0.0"]
[#assign valKeyLength = 128]
[#assign valMode = "AES_MODE_CBC"]
[#-- SWIPdatas is a list of SWIPconfigModel --]
[#list SWIPdatas as SWIP]
[#if SWIP.defines??]
[#list SWIP.defines as definition]
[#compress]
[#if definition.name == "AES_VERSION"] [#assign valVersion = definition.value] [/#if]
[#if definition.name == "AES_KEY_LENGTH"] [#assign valKeyLength = definition.value] [/#if]
[#if definition.name == "AES_MODE"] [#assign valMode = definition.value] [/#if]
[/#compress]
[/#list]
[/#if]
[/#list]
/**********************************
 * @Description  : AES模块头文件
 * @Author       : Jason Chen
 * @Date         : 2025-01-23 14:39:23
 * @FilePath     : aes.h
 * @Version      : ${valVersion}
 * @Copyright 2025 Jason Chen, All Rights Reserved.
 **********************************/
#ifndef __AES_H
#define __AES_H

#include "common_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AES_KEY_LENGTH      (${valKeyLength})

// 加解密模式
#define AES_MODE_ECB        (0)             // 电子密码本模式（一般模式）
#define AES_MODE_CBC        (1)             // 密码分组链接模式
#define AES_MODE            ${valMode}

void aes_init(const void *pKey);
void aes_encrypt(const uint8_t *pPlainText, uint8_t *pCipherText, uint32_t nDataLen, const uint8_t *pIV);
void aes_decrypt(const uint8_t *pCipherText, uint8_t *pPlainText, uint32_t nDataLen, const uint8_t *pIV);

#ifdef __cplusplus
}
#endif

#endif
