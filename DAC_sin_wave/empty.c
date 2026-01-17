/*
 * Copyright (c) 2015-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== empty.c ========
 */

/* For usleep() */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/DAC.h>
#include <ti/drivers/Timer.h>

/* Driver configuration */
#include "ti_drivers_config.h"

/* 打印和数学计算 */
#include <stdio.h>
#include <math.h>
/* 用于输出正弦波形的点 */
#include "empty.h"

static uint8_t sineTable[BUFFER_SIZE];/* 正弦表数据存放的数组 */
static DAC_Handle dachandle; /* DAC序列号 */

/**
 * @brief       初始化正弦表
 * @note        保证BUFFER_SIZE和sineTable已定义，BUFFER_SIZE<65536
 */
void sineTable_init ( void )
{
    for ( uint16_t i = 0; i < BUFFER_SIZE; i++ )
    {
        double x = 2 * M_PI * i / BUFFER_SIZE;
        double y = sin ( x );
        uint8_t dat = ( uint8_t ) ( ( ( y + 1.0 ) * 0.5 ) * 255 );
        sineTable[i] = dat;
    }

}

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
 * @brief      Timer 回调函数，用于控制呼吸灯
 * @param[in]  handle    Timer 句柄
 * @param[in]  status    Timer 状态
 */
void timercallback ( Timer_Handle handle, int_fast16_t status )
{
    volatile static uint16_t sin_dat = 0;
    DAC_setCode ( dachandle, sineTable[sin_dat++] );
    if ( sin_dat >= BUFFER_SIZE )
    {
        sin_dat = 0;
    }

}

/*
 *  ======== mainThread ========
 */
void *mainThread ( void *arg0 )
{
    /* 1 second delay */
    uint32_t time = 1;

    /* 初始化正弦表*/
    sineTable_init();

    /* DAC初始化 */
    DAC_init();
    DAC_Params dacparams;
    DAC_Params_init ( &dacparams );
    dachandle = DAC_open ( CONFIG_DAC_0, &dacparams );
    /* 开启DAC */
    DAC_enable ( dachandle );

    /* 定时器参数初始化 */
    uint32_t timer_period_us = 100;/* 定时器中断间隔us */
    Timer_Handle timer0 =
        my_timer_init ( CONFIG_TIMER_0, Timer_CONTINUOUS_CALLBACK, Timer_PERIOD_US,
                        timercallback, timer_period_us );
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
    /* 主循环 */
    while ( 1 )
    {
        sleep ( time );
    }
}
