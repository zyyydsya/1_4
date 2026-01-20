/*********************************************************************
* Copyright (c)  Hehe,Ltd. All rights reserved.
* Description:硬件配置PWM呼吸灯         
*
* History:修改历史记录列表，包括修改日期、修改者及修改内容简述           
* Date               Author                Modification:                     
* 2026-1-20          张也                   create v1.0
*********************************************************************/


/*
 *  ======== empty.c ========
 */

/* For usleep() */
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

/* Driver Header files */
#include <math.h>
#include <stdio.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/PWM.h>
#include <ti/drivers/Timer.h>

/* Driver configuration */
#include "ti_drivers_config.h"


static uint32_t time = 1;                   /* 每次循环睡眠时间，单位：s */
static PWM_Handle pwm_hardware = NULL;      /* pwm实例号 */
static uint32_t pwm_total_steps = 1000;     /* pwm总步数 */
volatile static uint32_t pwm_steps_idx = 0; /* 当前步数参数 */
static uint32_t pwmperiod = 2000;           /* pwm周期us */
static uint32_t breathperiod = 2000;           /* 一个完整呼吸灯周期ms */
volatile static uint32_t pwm_duty = 0;          /* pwm占空比 */

/**
 * @brief       PWM参数配置
 * @note        PWM 使用微秒为单位设置周期和占空比
 * @param[in]   index         PWM索引到的逻辑外设编号
 * @param[in]   periodValue   PWM周期
 * @return      返回打开的 Timer 句柄，失败返回 NULL
 * @warning     调用者需要保证 PWM 实例号合法
 */
static PWM_Handle my_pwm_init ( int index, uint32_t periodValue )
{
    PWM_Handle pwmhandle;
    PWM_Params pwmparams;

    PWM_init();
    PWM_Params_init ( &pwmparams );

    pwmparams.dutyUnits = PWM_DUTY_US;
    pwmparams.dutyValue = 0;
    pwmparams.periodUnits = PWM_PERIOD_US;
    pwmparams.periodValue = periodValue;

    pwmhandle = PWM_open ( index, &pwmparams );
    return pwmhandle;
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
 * \brief  根据呼吸灯周期和占空比步长计算 Timer 周期
 * \note   duty_step 变化步长
 * \param  cycleTimeMs 呼吸灯从灭到亮再回到灭的总时间（ms）
 * \retval Timer周期（微秒）
 * \warning None
 */
uint32_t get_timerperiodus ( uint32_t cycletimems )
{
    uint32_t timerPeriodus =
        ( cycletimems * 1000 ) / pwm_total_steps; /* 每步时间 us */
    return timerPeriodus;
}

/**
 * @brief      Timer 回调函数，用于控制呼吸灯
 * @param[in]  handle    Timer 句柄
 * @param[in]  status    Timer 状态
 */
void timer0callback ( Timer_Handle handle, int_fast16_t status )
{
    /* 0 ~ 2π 的相位 */
    float phase = 2.0f * M_PI * pwm_steps_idx / pwm_total_steps;

    /* sin(phase)：-1 ~ +1 */
    float s = sinf ( phase );

    /* 映射到 0 ~ pwmperiod */
    pwm_duty = ( uint32_t ) ( ( s + 1.0f ) * 0.5f * pwmperiod );

    PWM_setDuty ( pwm_hardware, pwm_duty );

    pwm_steps_idx++;
    if ( pwm_steps_idx >= pwm_total_steps )
    {
        pwm_steps_idx = 0;
    }
}

/*
 *  ======== mainThread ========
 */
void *mainThread ( void *arg0 )
{
    /* PWM参数初始化 */
    pwm_hardware = my_pwm_init ( CONFIG_PWM_0_CONST, pwmperiod );
    if ( pwm_hardware == NULL )
    {
        printf ( "PWM open failed\n" );
        while ( 1 )
        {
        }
    }
    /* 定时器参数初始化 */
    uint32_t timer_period_us = get_timerperiodus ( breathperiod );
    Timer_Handle timer0 =
        my_timer_init ( CONFIG_TIMER_0, Timer_CONTINUOUS_CALLBACK, Timer_PERIOD_US,
                        timer0callback, timer_period_us );
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
    /* 开启PWM */
    PWM_start ( pwm_hardware );
    /* 主循环 */
    while ( 1 )
    {
        sleep ( time );
    }
}
