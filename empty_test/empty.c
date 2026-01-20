/*********************************************************************
* Copyright (c)  Hehe,Ltd. All rights reserved.
* Description:引脚输出正弦波          
*
* History:修改历史记录列表，包括修改日期、修改者及修改内容简述           
* Date               Author                Modification:                     
* 2026-1-20          张也                   create v1.0
*********************************************************************/


/*
 *  ======== empty.c ========
 */

/* For sleep() */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/apps/Button.h>
// 仅保留必要的Power头文件（新版SDK中Power_init()已由SysConfig自动调用）
#include <ti/drivers/Power.h>

/* Driver configuration */
#include "ti_drivers_config.h"



/*
 *  ======== mainThread ========
 */
void *mainThread ( void *arg0 )
{
    while ( 1 ) {}
}
