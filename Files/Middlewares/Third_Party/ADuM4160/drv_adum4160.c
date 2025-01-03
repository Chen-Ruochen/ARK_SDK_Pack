/**********************************
 * @Description  : ADUM4160驱动源文件
 * @Author       : Jason Chen
 * @Date         : 2025-01-03 10:02:56
 * @FilePath     : drv_adum4160.c
 * @Version      : V1.0
 * @Copyright 2025 Jason Chen, All Rights Reserved.
 **********************************/
#include "drv_adum4160.h"


void adum4160_enable(adum4160_t *adum, bool state)
{
    if (adum->enpin) {
        adum->enpin(state);
    }
}


void adum4160_reset(adum4160_t *adum)
{
    if (adum->enpin && adum->delay) {
        adum->enpin(false);
        adum->delay(1000);
        adum->enpin(true);
    }
}

void adum4160_speedset(adum4160_t *adum, adum4160_speed_t speed)
{
    if (adum->spd && adum->spu) {
        if (speed == ADUM4160_LOW_SPEED) {
            adum->spd(false);
            adum->spu(false);
        } else {
            adum->spd(true);
            adum->spu(true);
        }
    }
}
