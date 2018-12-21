/*
 * File      : board.c
 *
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-12-20	  ViKing       first implementation
 */

#include <interrupt.h>
#include <rthw.h>
#include "board.h"
#include <platform.h>
#include <encoding.h>


static void rt_hw_timer_init(void)
{
    MTIMECMP = MTIME + TICK_COUNT;

    /*  enable timer interrupt*/
    set_csr(mie, MIP_MTIP);
}

void rt_hw_board_init(void)
{
    /* initialize the system clock */
//    rt_hw_clock_init();

    /* initialize hardware interrupt */
    rt_hw_interrupt_init();

    /* initialize timer0 */
    rt_hw_timer_init();
    
    /* initialize uart */
   // rt_hw_uart_init();
    
#ifdef RT_USING_HEAP
    rt_system_heap_init((void *)HEAP_BEGIN, (void *)HEAP_END);
#endif

#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#ifdef RT_USING_CONSOLE
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif

    return;
}

