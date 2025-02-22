[#ftl]
[#assign valVersion = "1.0.0"]
[#-- SWIPdatas is a list of SWIPconfigModel --]
[#list SWIPdatas as SWIP]
[#if SWIP.defines??]
[#list SWIP.defines as definition]
[#compress]
[#if definition.name == "AES_VERSION"] [#assign valVersion = definition.value] [/#if]
[/#compress]
[/#list]
[/#if]
[/#list]
/**********************************
 * @Description  : AES模块源文件
 * @Author       : Jason Chen
 * @Date         : 2025-01-23 14:39:19
 * @FilePath     : aes.c
 * @Version      : ${valVersion}
 * @Copyright 2025 Jason Chen, All Rights Reserved.
 **********************************/
#include "aes.h"

#define Nk (AES_KEY_LENGTH / 32) // 以“字”（4字节）为单位的密钥长度
#define Nb 4                     // 以“字”（4字节）为单位的加解密数据块大小，固定为4

// Nr：加密的轮数
#if AES_KEY_LENGTH == 128
    #define Nr 10
#elif AES_KEY_LENGTH == 192
    #define Nr 12
#elif AES_KEY_LENGTH == 256
    #define Nr 14
#else
    #error AES_KEY_LENGTH must be 128, 192 or 256 BOOLs!
#endif

// GF(28) 多项式
#define BPOLY 0x1B // Lower 8 BOOLs of (x^8 + x^4 + x^3 + x + 1), ie. (x^4 + x^3 + x + 1).

/*****************************************************************************
*  Typedef  Enum                        枚举定义
*****************************************************************************/

/*****************************************************************************
*  Struct                               数据结构定义
******************************************************************************/

/*****************************************************************************
*  Local variable                       局部变量
*****************************************************************************/

// AES子密钥表，当密钥长度为128位时，占用176字节空间
static uint8_t g_roundKeyTable[4 * Nb * (Nr + 1)];

// 加密用的SBox
static const uint8_t sBox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

// 解密用的SBox
static const uint8_t invSBox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

///
//	函数名：	rotation_word
//	描述：		对一个“字”数据进行循环右移。
//	输入参数：	pWord -- 要右移的4字节数据。
//	输出参数：	pWord -- 右移后的4字节数据。
//	返回值：	无。
///
static void rotation_word(uint8_t *pWord)
{
    uint8_t temp = pWord[0];
    pWord[0] = pWord[1];
    pWord[1] = pWord[2];
    pWord[2] = pWord[3];
    pWord[3] = temp;
}

///
//	函数名：	xor_bytes
//	描述：		批量异或两组数据。
//	输入参数：	pData1 -- 要异或的第一组数据。
//				pData1 -- 要异或的第二组数据。
//				nCount -- 要异或的数据长度。
//	输出参数：	pData1 -- 异或后的结果。
//	返回值：	无。
///
static void xor_bytes(uint8_t *pData1, const uint8_t *pData2, uint8_t nCount)
{
    uint8_t i;

    for (i = 0; i < nCount; i++) {
        pData1[i] ^= pData2[i];
    }
}

///
//	函数名：	AddRoundKey
//	描述：		把 中间状态数据 加上（异或）子密钥，数据长度为16字节。
//	输入参数：	pState	  -- 状态数据。
//				pRoundKey -- 子密钥数据。
//	输出参数：	pState	  -- 加上子密钥后的状态数据。
//	返回值：	无。
///
// static void AddRoundKey(uint8_t *pState, const uint8_t *pRoundKey)
// {
// 	xor_bytes(pState, pRoundKey, 4*Nb);
// }

// AddRoundKey的宏形式，比函数形式可以节省4字节的data数据
#define AddRoundKey(pState, pRoundKey) \
    xor_bytes((pState), (pRoundKey), 4 * Nb)

///
//	函数名：	sub_bytes
//	描述：		通过S盒子置换状态数据。
//	输入参数：	pState	-- 状态数据。
//				nCount  -- 状态数据长度。
//				bInvert	-- 是否使用反向S盒子（解密时使用）。
//	输出参数：	pState	-- 置换后的状态数据。
//	返回值：	无。
///
static void sub_bytes(uint8_t *pState, uint8_t nCount, bool bInvert)
{
    uint8_t i;
    const uint8_t *pSBox = bInvert ? invSBox : sBox;

    for (i = 0; i < nCount; i++) {
        pState[i] = pSBox[pState[i]];
    }
}

///
//	函数名：	shift_rows
//	描述：		把状态数据移行。
//	输入参数：	pState	-- 状态数据。
//				bInvert	-- 是否反向移行（解密时使用）。
//	输出参数：	pState	-- 移行后的状态数据。
//	返回值：	无。
///
static void shift_rows(uint8_t *pState, bool bInvert)
{
    // 注意：状态数据以列形式存放！

    uint8_t r; // row，   行
    uint8_t c; // column，列
    uint8_t temp;
    uint8_t rowData[4];

    for (r = 1; r < 4; r++) {
        // 备份一行数据
        for (c = 0; c < 4; c++) {
            rowData[c] = pState[r + 4 * c];
        }

        temp = bInvert ? (4 - r) : r;
        for (c = 0; c < 4; c++) {
            pState[r + 4 * c] = rowData[(c + temp) % 4];
        }
    }
}

///
//	函数名：	gf_mult_by02
//	描述：		在GF(28)域的 乘2 运算。
//	输入参数：	num	-- 乘数。
//	输出参数：	无。
//	返回值：	num乘以2的结果。
///
static uint8_t gf_mult_by02(uint8_t num)
{
    if ((num & 0x80) == 0) {
        num = num << 1;
    } else {
        num = (num << 1) ^ BPOLY;
    }

    return num;
}

///
//	函数名：	mix_columns
//	描述：		混合状态各列数据。
//	输入参数：	pState	-- 状态数据。
//				bInvert	-- 是否反向混合（解密时使用）。
//	输出参数：	pState	-- 混合列后的状态数据。
//	返回值：	无。
///
static void mix_columns(uint8_t *pState, bool bInvert)
{
    uint8_t i;
    uint8_t temp;
    uint8_t a0Pa2_M4; // 4(a0 + a2)
    uint8_t a1Pa3_M4; // 4(a1 + a3)
    uint8_t result[4];

    for (i = 0; i < 4; i++, pState += 4) {
        // b0 = 2a0 + 3a1 + a2 + a3
        //    = (a0 + a1 + a2 + a3) + 2(a0 + a1) + a0

        temp = pState[0] ^ pState[1] ^ pState[2] ^ pState[3];
        result[0] = temp ^ pState[0] ^ gf_mult_by02((uint8_t)(pState[0] ^ pState[1]));
        result[1] = temp ^ pState[1] ^ gf_mult_by02((uint8_t)(pState[1] ^ pState[2]));
        result[2] = temp ^ pState[2] ^ gf_mult_by02((uint8_t)(pState[2] ^ pState[3]));
        result[3] = temp ^ pState[3] ^ gf_mult_by02((uint8_t)(pState[3] ^ pState[0]));

        if (bInvert) {
            // b0' = 14a0 + 11a1 + 13a2 + 9a3
            //     = (a0 + a1 + a2 + a3) + 2(a0 + a1) + a0	（这部分为b0）
            //       + 2(4(a0 + a2) + 4(a1 + a3))
            //       +   4(a0 + a2)

            a0Pa2_M4 = gf_mult_by02(gf_mult_by02((uint8_t)(pState[0] ^ pState[2])));
            a1Pa3_M4 = gf_mult_by02(gf_mult_by02((uint8_t)(pState[1] ^ pState[3])));
            temp = gf_mult_by02((uint8_t)(a0Pa2_M4 ^ a1Pa3_M4));
            result[0] ^= temp ^ a0Pa2_M4;
            result[1] ^= temp ^ a1Pa3_M4;
            result[2] ^= temp ^ a0Pa2_M4;
            result[3] ^= temp ^ a1Pa3_M4;
        }

        memcpy(pState, result, 4);
    }
}

///
//	函数名：	block_encrypt
//	描述：		对单块数据加密。
//	输入参数：	pState -- 状态数据。
//	输出参数：	pState -- 加密后的状态数据。
//	返回值：	无。
///
static void block_encrypt(uint8_t *pState)
{
    uint8_t i;

    AddRoundKey(pState, g_roundKeyTable);

    for (i = 1; i <= Nr; i++) { // i = [1, Nr]
        sub_bytes(pState, 4 * Nb, 0);
        shift_rows(pState, 0);

        if (i != Nr) {
            mix_columns(pState, 0);
        }

        AddRoundKey(pState, &g_roundKeyTable[4 * Nb * i]);
    }
}

///
//	函数名：	block_decrypt
//	描述：		对单块数据解密。
//	输入参数：	pState -- 状态数据。
//	输出参数：	pState -- 解密后的状态数据。
//	返回值：	无。
///
static void block_decrypt(uint8_t *pState)
{
    uint8_t i;

    AddRoundKey(pState, &g_roundKeyTable[4 * Nb * Nr]);

    for (i = Nr; i > 0; i--) { // i = [Nr, 1]
        shift_rows(pState, 1);
        sub_bytes(pState, 4 * Nb, 1);
        AddRoundKey(pState, &g_roundKeyTable[4 * Nb * (i - 1)]);

        if (i != 1) {
            mix_columns(pState, 1);
        }
    }
}

/*****************************************************************************
*  Global Functions                     全局函数
******************************************************************************/

///
//	函数名：	aes_init
//	描述：		初始化，在此执行扩展密钥操作。
//	输入参数：	pKey -- 原始密钥，其长度必须为 AES_KEY_LENGTH/8 字节。
//	输出参数：	无。
//	返回值：	无。
///
void aes_init(const void *pKey)
{
    // 扩展密钥
    uint8_t i;
    uint8_t *pRoundKey;
    uint8_t Rcon[4] = {0x01, 0x00, 0x00, 0x00};

    memcpy(g_roundKeyTable, pKey, 4 * Nk);

    pRoundKey = &g_roundKeyTable[4 * Nk];

    for (i = Nk; i < Nb * (Nr + 1); pRoundKey += 4, i++) {
        memcpy(pRoundKey, pRoundKey - 4, 4);

        if (i % Nk == 0) {
            rotation_word(pRoundKey);
            sub_bytes(pRoundKey, 4, 0);
            xor_bytes(pRoundKey, Rcon, 4);

            Rcon[0] = gf_mult_by02(Rcon[0]);
        } else if (Nk > 6 && i % Nk == Nb) {
            sub_bytes(pRoundKey, 4, 0);
        }

        xor_bytes(pRoundKey, pRoundKey - 4 * Nk, 4);
    }
}

///
//	函数名：	aes_encrypt
//	描述：		加密数据
//	输入参数：	pPlainText	-- 明文，即需加密的数据，其长度为nDataLen字节。
//				nDataLen	-- 数据长度，以字节为单位，必须为AES_KEY_LENGTH/8的整倍数。
//				pIV			-- 初始化向量，如果使用ECB模式，可设为NULL。
//	输出参数：	pCipherText	-- 密文，即由明文加密后的数据，可以与pPlainText相同。
//	返回值：	无。
///
void aes_encrypt(const uint8_t *pPlainText, uint8_t *pCipherText, uint32_t nDataLen, const uint8_t *pIV)
{
    uint32_t i;

    if (pPlainText != pCipherText) {
        memcpy(pCipherText, pPlainText, nDataLen);
    }

    for (i = nDataLen / (4 * Nb); i > 0; i--, pCipherText += 4 * Nb) {
        #if AES_MODE == AES_MODE_CBC
            xor_bytes(pCipherText, pIV, 4 * Nb);
        #endif

        block_encrypt(pCipherText);
        pIV = pCipherText;
    }
}

///
//	函数名：	aes_decrypt
//	描述：		解密数据
//	输入参数：	pCipherText -- 密文，即需解密的数据，其长度为nDataLen字节。
//				nDataLen	-- 数据长度，以字节为单位，必须为AES_KEY_LENGTH/8的整倍数。
//				pIV			-- 初始化向量，如果使用ECB模式，可设为NULL。
//	输出参数：	pPlainText  -- 明文，即由密文解密后的数据，可以与pCipherText相同。
//	返回值：	无。
///
void aes_decrypt(const uint8_t *pCipherText, uint8_t *pPlainText, uint32_t nDataLen, const uint8_t *pIV)
{
    uint32_t i;

    if (pPlainText != pCipherText) {
        memcpy(pPlainText, pCipherText, nDataLen);
    }

    // 从最后一块数据开始解密，这样不用开辟空间来保存IV
    pPlainText += nDataLen - 4 * Nb;
    for (i = nDataLen / (4 * Nb); i > 0; i--, pPlainText -= 4 * Nb) {
        block_decrypt(pPlainText);

        #if AES_MODE == AES_MODE_CBC
            if (i == 1) {
                // 最后一块数据
                xor_bytes(pPlainText, pIV, 4 * Nb);
            } else {
                xor_bytes(pPlainText, pPlainText - 4 * Nb, 4 * Nb);
            }
        #endif
    }
}
