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
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

/* Driver Header files */
#include <math.h>
#include <stdio.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/Timer.h>

/* Driver configuration */
#include "ti_drivers_config.h"

static uint32_t timer_total_step = 40;     /* 一次完整呼吸的总步数 */
static uint32_t timer_period = 50;          /* 定时器中断周期us */
volatile static uint32_t pwm_duty = 0;      /* pwm占空比 */
static uint32_t pwm_total_steps = 1000;     /* pwm总步数 */
static uint32_t breathperiod = 2000;          /* pwm周期ms */
volatile static uint32_t pwm_steps_idx = 0; /* 当前步数参数 */
static uint32_t pwm_timer_idx = 0;          /* 单次改变占空比所需要进入timer的次数*/


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
static Timer_Handle my_timer_init(uint_least8_t index, Timer_Mode timerMode,
                                  Timer_PeriodUnits periodUnits,
                                  Timer_CallBackFxn timerCallback,
                                  uint32_t period)
{
    Timer_Handle timerhandle;
    Timer_Params timerparams;

    Timer_init();
    Timer_Params_init(&timerparams);

    timerparams.timerMode = timerMode;
    timerparams.periodUnits = periodUnits;
    timerparams.timerCallback = timerCallback;
    timerparams.period = period;

    timerhandle = Timer_open(index, &timerparams);

    return timerhandle;
}

/**
 * @brief       计算单次占空比变化所需的定时器中断次数
 * @param[in]   pwm_period      呼吸灯总周期ms
 * @param[in]   pwm_total_steps 呼吸灯总步数
 * @param[in]   timer_period    定时器周期us
 * @return      中断次数（向上取整，避免为0）
 */
static uint32_t PWM_period_once(uint32_t breathperiod, uint32_t pwm_total_steps,
                                uint32_t timer_period)
{
    uint32_t us_per_step  = breathperiod * 1000 / pwm_total_steps / timer_period;
    /* 进行零值保护 */
    if(us_per_step == 0)
    {
        us_per_step = 1;
    }

    return us_per_step;
}

/**
 * @brief      Timer 回调函数
 * @param[in]  handle    Timer 句柄
 * @param[in]  status    Timer 状态
 */
void timercallback(Timer_Handle handle, int_fast16_t status)
{
    /* 单周期占空比增长参数*/
    volatile static uint32_t duty_idx = 0;
    volatile static uint32_t step_idx = 0;
    /* 单个亮灭周期和整轮周期参数自增 */
    duty_idx++;
    step_idx++;
    /* 进行GPIO输出高低电平判别 */
    if(duty_idx >= timer_total_step)
    {
        duty_idx = 0;
        GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);
    }
    else if(duty_idx >= pwm_duty)
    {
        GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);
    }
    /* 进行占空比周期变化计算 */
    if(step_idx >= pwm_timer_idx)
    {
        step_idx = 0;
        /* 0 ~ 2π 的相位 */
        float phase = 2.0f * M_PI * pwm_steps_idx / pwm_total_steps;

        /* sin(phase)：-1 ~ +1 */
        float s = sinf(phase);

        /* 映射到 0 ~ timer_total_step */
        pwm_duty = (uint32_t)((s + 1.0f) * 0.5f * timer_total_step);

        pwm_steps_idx++;
        if(pwm_steps_idx >= pwm_total_steps)
        {
            pwm_steps_idx = 0;
        }
    }
}

/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    uint32_t time = 1;

    /* GPIO初始化 */
    GPIO_init();
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* 计算占空比参数每次改变需要进入定时器的次数*/
    pwm_timer_idx = PWM_period_once(breathperiod, pwm_total_steps, timer_period);
    /* 定时器初始化 */
    Timer_Handle timer0 =
        my_timer_init(CONFIG_TIMER_0, Timer_CONTINUOUS_CALLBACK, Timer_PERIOD_US,
                      timercallback, timer_period);
    if(timer0 == NULL)
    {
        printf("Timer init failed\n");
        while(1)
        {
        }
    }

    /* 开启定时器 */
    if(Timer_start(timer0) == Timer_STATUS_ERROR)
    {
        printf("Timer start failed\n");
        while(1)
        {
        }
    }

    /* 主循环 */
    while(1)
    {
        sleep(time);
    }
}
