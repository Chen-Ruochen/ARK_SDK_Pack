/**********************************
 * @Description  : ADUM4160驱动头文件
 * @Author       : Jason Chen
 * @Date         : 2025-01-03 10:03:02
 * @FilePath     : drv_adum4160.h
 * @Version      : V1.0
 * @Copyright 2025 Jason Chen, All Rights Reserved.
 **********************************/
#ifndef __DRV_ADUM4160_H
#define __DRV_ADUM4160_H
#include "common_lib.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum adum4160_speed_e {
    ADUM4160_LOW_SPEED = 0,
    ADUM4160_FULL_SPEED
} adum4160_speed_t;

typedef void (*spu_set)(bool state);
typedef void (*spd_set)(bool state);
typedef void (*enpin_set)(bool state);
typedef void (*delay_ms)(uint32_t ms);

typedef struct adum4160_s {
    spu_set     spu;
    spd_set     spd;
    enpin_set   enpin;
    delay_ms    delay;
} adum4160_t;

void adum4160_enable(adum4160_t *adum, bool state);
void adum4160_reset(adum4160_t *adum);
void adum4160_speedset(adum4160_t *adum, adum4160_speed_t speed);

#ifdef __cplusplus
}
#endif

#endif
