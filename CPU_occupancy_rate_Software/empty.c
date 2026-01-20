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

/* For sleep/usleep() */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/apps/LED.h>
/* Power驱动头文件 */
#include <ti/drivers/power/PowerCC26XX.h>

/* Driver configuration */
#include "ti_drivers_config.h"

#define SYSCLK_FREQ_MHZ 48 /* 系统时钟频率为48MHz */

static uint32_t timer_period = 1000; /* 模拟定时器触发周期us，与硬件版本一致 */
LED_Handle led0Handle = NULL;
LED_Handle led1Handle = NULL;

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
