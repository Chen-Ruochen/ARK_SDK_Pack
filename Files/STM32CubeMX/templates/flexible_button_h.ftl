[#ftl]
/**********************************
 * @Description  : Flexible button driver
 * @Author       : Jason Chen
 * @Date         : 2024-12-19 23:25:22
 * @FilePath     : flexible_button.h
 * @Version      : V1.0
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
#ifndef FLEXIBLE_BUTTON_H__
#define FLEXIBLE_BUTTON_H__

#include "common_lib.h"

[#list SWIPdatas as SWIP]

[#if SWIP.defines??]
    [#list SWIP.defines as definition]
#define ${definition.name} #t#t ${definition.value}
    [/#list]
[/#if]
#define FLEX_MS_TO_SCAN_CNT(ms) (ms / (1000 / FLEX_BTN_SCAN_FREQ_HZ))

/* Multiple clicks interval, default 300ms */
#define MAX_MULTIPLE_CLICKS_INTERVAL (FLEX_MS_TO_SCAN_CNT(300))

typedef void (*flex_button_response_callback)(void*);

typedef enum
{
    FLEX_BTN_PRESS_DOWN = 0,
    FLEX_BTN_PRESS_CLICK,
    FLEX_BTN_PRESS_DOUBLE_CLICK,
    FLEX_BTN_PRESS_REPEAT_CLICK,
    FLEX_BTN_PRESS_SHORT_START,
    FLEX_BTN_PRESS_SHORT_UP,
    FLEX_BTN_PRESS_LONG_START,
    FLEX_BTN_PRESS_LONG_UP,
    FLEX_BTN_PRESS_LONG_HOLD,
    FLEX_BTN_PRESS_LONG_HOLD_UP,
    FLEX_BTN_PRESS_MAX,
    FLEX_BTN_PRESS_NONE,
} flex_button_event_t;

typedef struct flex_button
{
    struct flex_button* next;

    uint8_t  (*usr_button_read)(void *);
    flex_button_response_callback  cb;

    uint16_t scan_cnt;
    uint16_t click_cnt;
    uint16_t max_multiple_clicks_interval;

    uint16_t debounce_tick;
    uint16_t short_press_start_tick;
    uint16_t long_press_start_tick;
    uint16_t long_hold_start_tick;

    uint8_t id;
    uint8_t pressed_logic_level : 1;
    uint8_t event               : 4;
    uint8_t status              : 3;
} flex_button_t;

#ifdef __cplusplus
extern "C" {
#endif

int32_t flex_button_register(flex_button_t *button);
flex_button_event_t flex_button_event_read(flex_button_t* button);
uint8_t flex_button_scan(void);

#ifdef __cplusplus
}
#endif
#endif /* __FLEXIBLE_BUTTON_H__ */

[/#list]
