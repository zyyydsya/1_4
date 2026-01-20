/*********************************************************************
* Copyright (c)  Hehe,Ltd. All rights reserved.
* Description:测试软件延时的CPU功耗
*
* History:修改历史记录列表，包括修改日期、修改者及修改内容简述
* Date               Author                Modification:
* 2026-1-20          张也                   create v1.0
*********************************************************************/

/*
 *  ======== empty.c ========
 */

/* For sleep/usleep() */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/apps/LED.h>
/* 引入正确的Power驱动头文件（SDK 8.x版本） */
#include <ti/drivers/power/PowerCC26XX.h>

/* Driver configuration */
#include "ti_drivers_config.h"

static uint32_t timer_period = 1000; /* 模拟定时器触发周期us，与硬件版本一致 */
LED_Handle led0Handle = NULL;
LED_Handle led1Handle = NULL;
/* 直接定义CC13xx/CC26xx默认主频（48MHz），避免Power接口兼容问题 */
#define SYSCLK_FREQ_MHZ 48

/**
 * @brief       软件微秒级延时函数
 * @note        基于固定主频（SYSCLK_FREQ_MHZ）
 * @param[in]   us  要延时的微秒数
 */
static void software_delay_us ( uint32_t us )
{
    /* 48MHz主频下，1us = 48个时钟周期 */
    uint32_t cycles = us * SYSCLK_FREQ_MHZ;
    /* volatile + 空循环，彻底防止编译器优化 */
    volatile uint32_t i;
    for ( i = 0; i < cycles; i++ )
    {
        /* 空操作，仅占用时钟周期 */
    }
}

/*
 *  ======== mainThread ========
 */
void *mainThread ( void *arg0 )
{
    /* 1. 配置LED（与硬件版本完全一致） */
    led0Handle = LED_open ( CONFIG_LED_0, NULL );
    led1Handle = LED_open ( CONFIG_LED_1, NULL );

    if ( led0Handle == NULL || led1Handle == NULL )
    {
        printf ( "LED init failed\n" );
        while ( 1 )
        {
        }
    }
    /* 打开led1（与硬件版本完全一致） */
    LED_setOn ( led1Handle, 100 );

    /* 2. 软件定时器主循环：模拟硬件定时器逻辑 */
    while ( 1 )
    {
        /* 执行LED翻转（与硬件定时器回调逻辑一致） */
        LED_toggle ( led0Handle );
        LED_toggle ( led1Handle );

        /* 软件延时1000us，模拟硬件定时器间隔 */
        software_delay_us ( timer_period );
    }
}
