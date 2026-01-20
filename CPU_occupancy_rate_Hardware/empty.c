/*********************************************************************
* Copyright (c)  Hehe,Ltd. All rights reserved.
* Description:测试硬件延时的CPU功耗
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
#include <stdio.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Timer.h>


/* Driver configuration */
#include "ti_drivers_config.h"
#include <ti/drivers/apps/LED.h>

static uint32_t timer_period = 1000; /* 定时器出发周期us */
LED_Handle led0Handle = NULL;
LED_Handle led1Handle = NULL;


/**
 * @brief       初始化定时器
 * @note        Timer 支持连续或单次回调模式
 * @param[in]   index           计时器索引到的逻辑外设编号
 * @param[in]   timerMode       计时器驱动程序要使用的模式
 * @param[in]   periodUnits     用于指定周期的单位
 * @param[in]   timerCallback
 * 当timerMode为Timer_ONESHOT_CALLBACK或Timer_CONTINUOUS_CALLBACK时调用的回调函数
 * @param[in]   period          周期，单位为periodUnits
 * @return      返回打开的 Timer 句柄，失败返回 NULL
 * @warning     调用者需要保证 Timer 实例号合法
 */
static Timer_Handle my_timer_init ( uint_least8_t index, Timer_Mode timerMode,
                                    Timer_PeriodUnits periodUnits,
                                    Timer_CallBackFxn timerCallback,
                                    uint32_t period )
{
    Timer_Handle timerhandle;
    Timer_Params timerparams;

    Timer_init();
    Timer_Params_init ( &timerparams );

    timerparams.timerMode = timerMode;
    timerparams.periodUnits = periodUnits;
    timerparams.timerCallback = timerCallback;
    timerparams.period = period;

    timerhandle = Timer_open ( index, &timerparams );

    return timerhandle;
}


/**
 * @brief      Timer 回调函数
 * @param[in]  handle    Timer 句柄
 * @param[in]  status    Timer 状态
 */
void timercallback ( Timer_Handle handle, int_fast16_t status )
{
    LED_toggle ( led0Handle );
    LED_toggle ( led1Handle );
}

/*
 *  ======== mainThread ========
 */
void *mainThread ( void *arg0 )
{
    uint32_t time = 5;

    /* 配置led */
    led0Handle = LED_open ( CONFIG_LED_0, NULL );
    led1Handle = LED_open ( CONFIG_LED_1, NULL );

    if ( led0Handle == NULL || led1Handle == NULL )
    {
        printf ( "LED init failed\n" );
        while ( 1 )
        {
        }
    }
    /* 打开led1 */
    LED_setOn ( led1Handle, 100 );

    /* 定时器参数配置 */
    Timer_Handle timer0 =
        my_timer_init ( CONFIG_TIMER_0, Timer_CONTINUOUS_CALLBACK, Timer_PERIOD_US,
                        timercallback, timer_period );
    if ( timer0 == NULL )
    {
        printf ( "Timer init failed\n" );
        while ( 1 )
        {
        }
    }

    /* 开启定时器 */
    if ( Timer_start ( timer0 ) == Timer_STATUS_ERROR )
    {
        printf ( "Timer start failed\n" );
        while ( 1 )
        {
        }
    }


    while ( 1 )
    {
        sleep ( time );
    }
}
