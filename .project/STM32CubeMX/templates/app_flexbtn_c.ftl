[#ftl]
/**********************************
 * @Description  : Flexible button application
 * @Author       : Jason Chen
 * @Date         : 2024-12-24 20:44:35
 * @FilePath     : app_flexbtn.c
 * @Version      : V1.0
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
#include "app_flexbtn.h"
#include "app_flexbtn_conf.h"
#include "flexible_button.h"

static flex_button_t user_button[USER_BUTTON_MAX];

__weak uint8_t I_CUBE_FlexBtn_Read(void *arg)
{
    /* USER CODE BEGIN 0 */
#warning "Please implement I_CUBE_FlexBtn_Read() for your application"
    return 0;
    /* USER CODE BEGIN 0 */
}


__weak void I_CUBE_FlexBtn_Evt(void *arg)
{
    /* USER CODE BEGIN 0 */
#warning "Please implement I_CUBE_FlexBtn_Evt() for your application"
    /* USER CODE BEGIN 0 */
}

void I_CUBE_FlexBtn_Init(void)
{
    for (uint16_t i = 0; i < USER_BUTTON_MAX; i ++) {
        user_button[i].id = i;
        user_button[i].usr_button_read = I_CUBE_FlexBtn_Read;
        user_button[i].cb = I_CUBE_FlexBtn_Evt;
        user_button[i].pressed_logic_level = FLEX_BTN_CONF_LOGIC_LEVEL;
        user_button[i].short_press_start_tick = FLEX_MS_TO_SCAN_CNT(FLEX_BTN_CONF_SPT);
        user_button[i].long_press_start_tick = FLEX_MS_TO_SCAN_CNT(FLEX_BTN_CONF_LPT);
        user_button[i].long_hold_start_tick = FLEX_MS_TO_SCAN_CNT(FLEX_BTN_CONF_LHT);
        user_button[i].max_multiple_clicks_interval = MAX_MULTIPLE_CLICKS_INTERVAL;

        flex_button_register(&user_button[i]);
    }
}

void I_CUBE_FlexBtn_Scan(void *arg)
{
    flex_button_scan();
}

[/#list]
