/*********************************************************************
* Copyright (c)  Hehe,Ltd. All rights reserved.
* Description:进行sleep休眠测试功耗的代码
*
* History:修改历史记录列表，包括修改日期、修改者及修改内容简述
* Date               Author                Modification:
* 2026-1-20          张也                   create v1.0
*********************************************************************/


/*
 *  ======== empty.c ========
 */

/* For usleep() */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/apps/Button.h>

/* Driver configuration */
#include "ti_drivers_config.h"

/* 休眠模式和正常轮循模式 */
#define MODE_SLEEP 0
#define MODE_NORMAL 1
/* 初始化为休眠模式 */
volatile uint8_t mode = MODE_SLEEP;

/**
 * @brief      按键回调函数
 * @param[in]  buttonHandle    按键序列号
 * @param[in]  buttonEvents    中断触发类型
 * @note       进行模式的切换
 */
static void buttonCallback ( Button_Handle buttonHandle, Button_EventMask buttonEvents )
{
    if ( mode == MODE_SLEEP )
    {
        mode = MODE_NORMAL;
    }
    else
    {
        mode = MODE_SLEEP;
    }
}
/*
 *  ======== mainThread ========
 */
void *mainThread ( void *arg0 )
{
    uint32_t time = 4;
    /* 按键初始化 */
    Button_Params buttonParams;
    Button_Params_init ( &buttonParams );
    buttonParams.buttonCallback  = buttonCallback;
    buttonParams.buttonEventMask = Button_EV_PRESSED;
    Button_open ( CONFIG_BUTTON_0, &buttonParams );
    /* 主循环 */
    while ( 1 )
    {
        if ( mode == MODE_SLEEP )
        {
            sleep ( time );
        }
        else
        {

        }
    }
}
