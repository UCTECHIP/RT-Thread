/*
 * File      : interrupt.c
 *
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-12-19	  ViKing       first implementation
 */

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include <rtthread.h>
#include <platform.h>
#include <encoding.h>
#include <interrupt.h>
#include <string.h>
#include <plic.h>

/****************************** PLIC ******************************/

typedef struct __plic_instance_t
{
  uintptr_t base_addr;

  uint32_t num_sources;
  uint32_t num_priorities;

} plic_instance_t;

typedef uint32_t plic_source;
typedef uint32_t plic_priority;
typedef uint32_t plic_threshold;

void volatile_memzero(uint8_t * base, unsigned int size)
{
  volatile uint8_t * ptr;
  for (ptr = base; ptr < (base + size); ptr++){
    *ptr = 0;
  }
}

void PLIC_init (
                plic_instance_t * this_plic,
                uintptr_t         base_addr,
                uint32_t num_sources,
                uint32_t num_priorities
                )
{

  this_plic->base_addr = base_addr;
  this_plic->num_sources = num_sources;
  this_plic->num_priorities = num_priorities;

  // Disable all interrupts (don't assume that these registers are reset).
  unsigned long hart_id = read_csr(mhartid);
  volatile_memzero((uint8_t*) (this_plic->base_addr +
                               PLIC_ENABLE_OFFSET +
                               (hart_id << PLIC_ENABLE_SHIFT_PER_TARGET)),
                   (num_sources + 8) / 8);

  // Set all priorities to 0 (equal priority -- don't assume that these are reset).
  volatile_memzero ((uint8_t *)(this_plic->base_addr +
                                PLIC_PRIORITY_OFFSET),
                    (num_sources + 1) << PLIC_PRIORITY_SHIFT_PER_SOURCE);

  // Set the threshold to 0.
  volatile plic_threshold* threshold = (plic_threshold*)
    (this_plic->base_addr +
     PLIC_THRESHOLD_OFFSET +
     (hart_id << PLIC_THRESHOLD_SHIFT_PER_TARGET));

  *threshold = 0;

}

void PLIC_enable_interrupt (plic_instance_t * this_plic, plic_source source){

  unsigned long hart_id = read_csr(mhartid);
  volatile uint8_t * current_ptr = (volatile uint8_t *)(this_plic->base_addr +
                                                        PLIC_ENABLE_OFFSET +
                                                        (hart_id << PLIC_ENABLE_SHIFT_PER_TARGET) +
                                                        (source >> 3));
  uint8_t current = *current_ptr;
  current = current | ( 1 << (source & 0x7));
  *current_ptr = current;

}

void PLIC_disable_interrupt (plic_instance_t * this_plic, plic_source source){

  unsigned long hart_id = read_csr(mhartid);
  volatile uint8_t * current_ptr = (volatile uint8_t *) (this_plic->base_addr +
                                                         PLIC_ENABLE_OFFSET +
                                                         (hart_id << PLIC_ENABLE_SHIFT_PER_TARGET) +
                                                         (source >> 3));
  uint8_t current = *current_ptr;
  current = current & ~(( 1 << (source & 0x7)));
  *current_ptr = current;

}

void PLIC_set_priority (plic_instance_t * this_plic, plic_source source, plic_priority priority){

  if (this_plic->num_priorities > 0) {
    volatile plic_priority * priority_ptr = (volatile plic_priority *)
      (this_plic->base_addr +
       PLIC_PRIORITY_OFFSET +
       (source << PLIC_PRIORITY_SHIFT_PER_SOURCE));
    *priority_ptr = priority;
  }
}

plic_source PLIC_claim_interrupt(plic_instance_t * this_plic){

  unsigned long hart_id = read_csr(mhartid);

  volatile plic_source * claim_addr = (volatile plic_source * )
    (this_plic->base_addr +
     PLIC_CLAIM_OFFSET +
     (hart_id << PLIC_CLAIM_SHIFT_PER_TARGET));

  return  *claim_addr;

}

void PLIC_complete_interrupt(plic_instance_t * this_plic, plic_source source){

  unsigned long hart_id = read_csr(mhartid);
  volatile plic_source * claim_addr = (volatile plic_source *) (this_plic->base_addr +
                                                                PLIC_CLAIM_OFFSET +
                                                                (hart_id << PLIC_CLAIM_SHIFT_PER_TARGET));
  *claim_addr = source;

}

/****************************** INTERRUPT ******************************/

#define MAX_HANDLERS    PLIC_NUM_INTERRUPTS

#define TICK_COUNT  (2 * RTC_FREQ / RT_TICK_PER_SECOND)
#define MTIME       (*((volatile uint64_t *)(CLINT_CTRL_ADDR + CLINT_MTIME)))
#define MTIMECMP    (*((volatile uint64_t *)(CLINT_CTRL_ADDR + CLINT_MTIMECMP)))

/* exception and interrupt handler table */
static struct rt_irq_desc irq_desc[MAX_HANDLERS];

static plic_instance_t g_plic;

/**
 * This function will mask a interrupt.
 * @param vector the interrupt number
 */
void rt_hw_interrupt_mask(int irq)
{
    PLIC_disable_interrupt(&g_plic, irq);
}

/**
 * This function will un-mask a interrupt.
 * @param vector the interrupt number
 */
void rt_hw_interrupt_unmask(int irq)
{
    PLIC_enable_interrupt(&g_plic, irq);
    PLIC_set_priority(&g_plic, irq, 1);
}

rt_isr_handler_t rt_hw_interrupt_handle(rt_uint32_t vector, void *param)
{
    rt_kprintf("UN-handled interrupt %d occurred!!!\n", vector);
    return RT_NULL;
}

void rt_hw_interrupt_init(void)
{
    int idx;
    
    /*  config interrupt vector*/
    asm volatile(
        "la t0, trap_entry\n"
        "csrw mtvec, t0"
    );
    
    /*  enable global interrupt*/
    PLIC_init(&g_plic,
            PLIC_CTRL_ADDR,
            PLIC_NUM_INTERRUPTS,
            PLIC_NUM_PRIORITIES);

    /* init exceptions table */
    for (idx = 0; idx < MAX_HANDLERS; idx++)
    {
        rt_hw_interrupt_mask(idx);
        irq_desc[idx].handler = (rt_isr_handler_t)rt_hw_interrupt_handle;
        irq_desc[idx].param = RT_NULL;
#ifdef RT_USING_INTERRUPT_INFO
        rt_snprintf(irq_desc[idx].name, RT_NAME_MAX - 1, "default");
        irq_desc[idx].counter = 0;
#endif
    }
    
    // enable machine external interrupt 
    set_csr(mie, MIP_MEIP);
}

rt_uint32_t rt_hw_interrupt_get_active(rt_uint32_t fiq_irq)
{
    return (rt_uint32_t)PLIC_claim_interrupt(&g_plic);;
}

void rt_hw_interrupt_ack(rt_uint32_t fiq_irq, rt_uint32_t id)
{
    PLIC_complete_interrupt(&g_plic, id);
}

/**
 * This function will install a interrupt service routine to a interrupt.
 * @param vector the interrupt number
 * @param handler the interrupt service routine to be installed
 * @param param the interrupt service function parameter
 * @param name the interrupt name
 * @return old handler
 */
rt_isr_handler_t rt_hw_interrupt_install(int vector, rt_isr_handler_t handler,
        void *param, const char *name)
{
    rt_isr_handler_t old_handler = RT_NULL;

    if(vector < MAX_HANDLERS)
    {
        old_handler = irq_desc[vector].handler;
        if (handler != RT_NULL)
        {
            irq_desc[vector].handler = (rt_isr_handler_t)handler;
            irq_desc[vector].param = param;
#ifdef RT_USING_INTERRUPT_INFO
            rt_snprintf(irq_desc[vector].name, RT_NAME_MAX - 1, "%s", name);
            irq_desc[vector].counter = 0;
#endif
        }
    }

    return old_handler;
}


/**
 * This function will be call when external machine-level 
 * interrupt from PLIC occurred.
 */
void handle_m_ext_interrupt(void)
{
    rt_isr_handler_t isr_func;
    rt_uint32_t irq;
    void *param;

    /* get irq number */
    irq = rt_hw_interrupt_get_active(0);

    /* get interrupt service routine */
    isr_func = irq_desc[irq].handler;
    param = irq_desc[irq].param;

    /* turn to interrupt service routine */
    isr_func(irq, param);
    rt_hw_interrupt_ack(0, irq);

#ifdef RT_USING_INTERRUPT_INFO
    irq_desc[irq].counter ++;
#endif
}

/* system tick interrupt */
void handle_m_time_interrupt()
{
    MTIMECMP = MTIME + TICK_COUNT;
    rt_tick_increase();
}

/* interrupt entry */
uintptr_t handle_trap(uintptr_t mcause, uintptr_t epc)
{
  if (0){
#ifdef USE_PLIC
    // External Machine-Level interrupt from PLIC
  } else if ((mcause & MCAUSE_INT) && ((mcause & MCAUSE_CAUSE) == IRQ_M_EXT)) {
    handle_m_ext_interrupt();
#endif
#ifdef USE_M_TIME
    // External Machine-Level interrupt from PLIC
  } else if ((mcause & MCAUSE_INT) && ((mcause & MCAUSE_CAUSE) == IRQ_M_TIMER)){
    handle_m_time_interrupt();
#endif
  }
  else {
    write(1, "trap\n", 5);
    _exit(mcause);
  }
  return epc;
}
