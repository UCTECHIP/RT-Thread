/*
 * File      : interrupt.h
 *
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-12-19	  ViKing       first implementation
 */

#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include <rthw.h>

#define RTC_FREQ 48000
#define TICK_COUNT  (2 * RTC_FREQ / RT_TICK_PER_SECOND)

#define MTIME       (*((volatile uint64_t *)(CLINT_CTRL_ADDR + CLINT_MTIME)))
#define MTIMECMP    (*((volatile uint64_t *)(CLINT_CTRL_ADDR + CLINT_MTIMECMP)))


void rt_hw_interrupt_mask(int irq);
void rt_hw_interrupt_unmask(int irq);
rt_isr_handler_t rt_hw_interrupt_handle(rt_uint32_t vector, void *param);
void rt_hw_interrupt_init(void);
rt_uint32_t rt_hw_interrupt_get_active(rt_uint32_t fiq_irq);
void rt_hw_interrupt_ack(rt_uint32_t fiq_irq, rt_uint32_t id);
rt_isr_handler_t rt_hw_interrupt_install(int vector, rt_isr_handler_t handler,
        void *param, const char *name);
        
#endif /* _INTERRUPT_H */
