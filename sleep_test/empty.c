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
