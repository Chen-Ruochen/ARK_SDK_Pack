[#ftl]
[#assign valVersion = "1.0.0"]
[#-- SWIPdatas is a list of SWIPconfigModel --]
[#list SWIPdatas as SWIP]
[#if SWIP.defines??]
[#list SWIP.defines as definition]
[#compress]
[#if definition.name == "FLEXBTN_VERSION"] [#assign valVersion = definition.value] [/#if]
[/#compress]
[/#list]
[/#if]
[/#list]
/**********************************
 * @Description  : Flexible button application
 * @Author       : Jason Chen
 * @Date         : 2024-12-24 20:44:35
 * @FilePath     : app_flexbtn.c
 * @Version      : ${valVersion}
 * @Copyright 2024 Jason Chen, All Rights Reserved.
 **********************************/
#include "app_flexbtn.h"
#include "flexbtn_conf.h"
#include "flexible_button.h"
/* USER CODE BEGIN Include */

/* USER CODE END Include */

static flex_button_t user_button[USER_BUTTON_MAX];

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

static uint8_t I_CUBE_FlexBtn_Read(void *arg)
{
    flex_button_t *btn = (flex_button_t *)arg;
    /* USER CODE BEGIN 0 */
#warning "Please implement I_CUBE_FlexBtn_Read() for your application"
    switch (btn->id) {
        default:
            break;
    }
    return 0;
    /* USER CODE END 0 */
}


static void I_CUBE_FlexBtn_Evt(void *arg)
{
    flex_button_t *btn = (flex_button_t *)arg;
    /* USER CODE BEGIN 0 */
#warning "Please implement I_CUBE_FlexBtn_Evt() for your application"
    /* USER CODE END 0 */
}

void I_CUBE_FlexBtn_Init(void)
{
    for (uint16_t i = 0; i < USER_BUTTON_MAX; i ++) {
        user_button[i].id = i;
        user_button[i].usr_button_read = I_CUBE_FlexBtn_Read;
        user_button[i].cb = I_CUBE_FlexBtn_Evt;
        user_button[i].pressed_logic_level = FLEXBTN_CONF_LPL;
        user_button[i].short_press_start_tick = FLEX_MS_TO_SCAN_CNT(FLEXBTN_CONF_SPT);
        user_button[i].long_press_start_tick = FLEX_MS_TO_SCAN_CNT(FLEXBTN_CONF_LPT);
        user_button[i].long_hold_start_tick = FLEX_MS_TO_SCAN_CNT(FLEXBTN_CONF_LHT);
        user_button[i].max_multiple_clicks_interval = MAX_MULTIPLE_CLICKS_INTERVAL;

        flex_button_register(&user_button[i]);
    }
}

void I_CUBE_FlexBtn_Scan(void *arg)
{
    flex_button_scan();
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
