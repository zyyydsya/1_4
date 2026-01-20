/*********************************************************************
* Copyright (c)  Hehe,Ltd. All rights reserved.
* Description:进行关机模式测试功耗的代码
*
* History:修改历史记录列表，包括修改日期、修改者及修改内容简述
* Date               Author                Modification:
* 2026-1-20          张也                   create v1.0
*********************************************************************/


/*
 *  ======== empty.c ========
 */

#include <stdbool.h>

/* 驱动头文件 */
#include <ti/drivers/dpl/SemaphoreP.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26X2.h>
#include <ti/drivers/apps/Button.h>
#include <ti/drivers/apps/LED.h>

/* Config配置 */
#include "ti_drivers_config.h"

/* 信号量配置和信号量序列号 */
SemaphoreP_Struct semStruct;
SemaphoreP_Handle semHandle;

static void buttonCallback ( Button_Handle buttonHandle, Button_EventMask buttonEvents )
{
    /* 释放信号量 */
    SemaphoreP_post ( semHandle );
}

/*
 *  ======== mainThread ========
 */
void *mainThread ( void *arg0 )
{
    LED_Handle led0Handle = LED_open ( CONFIG_LED_0, NULL );
    LED_Handle led1Handle = LED_open ( CONFIG_LED_1, NULL );

    /* 获得复位原因：关机模式唤醒复位或掉电复位 */
    PowerCC26X2_ResetReason resetReason = PowerCC26X2_getResetReason();

    /* 若是从关机模式中醒来，执行以下操作 */
    if ( resetReason == PowerCC26X2_RESET_SHUTDOWN_IO )
    {
        /* 禁用IO锁存器，使IO可正常变化 */
        PowerCC26X2_releaseLatches();

        /* 闪烁LED */
        LED_startBlinking ( led1Handle, 500, 3 );
    }

    /* 创建信号量 */
    semHandle = SemaphoreP_constructBinary ( &semStruct, 0 );

    /* 配置按键CONFIG_BUTTON_SHUTDOWN */
    Button_Params buttonParams;
    Button_Params_init ( &buttonParams );
    buttonParams.buttonCallback  = buttonCallback;
    buttonParams.buttonEventMask = Button_EV_PRESSED;
    Button_open ( CONFIG_BUTTON_SHUTDOWN, &buttonParams );

    /* 点亮LED */
    LED_setOn ( led0Handle, 100 );

    /* 挂起信号量，等待按下按键CONFIG_BUTTON_SHUTDOWN继续执行下面的操作 */
    SemaphoreP_pend ( semHandle, SemaphoreP_WAIT_FOREVER );

    /* 关灯 */
    LED_setOff ( led0Handle );

    /* 配置按键CONFIG_BUTTON_WAKEUP按下触发唤醒 */
    GPIO_setConfig ( CONFIG_BUTTON_WAKEUP, GPIO_CFG_IN_PU | GPIO_CFG_SHUTDOWN_WAKE_LOW );

    /* 关机 */
    Power_shutdown ( 0, 0 );

    while ( 1 ) {}
}
