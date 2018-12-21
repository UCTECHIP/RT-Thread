/*
 * File      : drv_usart.c
 *
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-12-20	  ViKing       first implementation
 */

#include <rtdevice.h>

#include <encoding.h>
#include <platform.h>
#include <interrupt.h>
#include "uart.h"

static void usart_handler(int vector, void *param)
{
    rt_hw_serial_isr((struct rt_serial_device *)param, RT_SERIAL_EVENT_RX_IND);
}

static rt_err_t usart_configure(struct rt_serial_device *serial,
                                struct serial_configure *cfg)
{
    RT_ASSERT(serial != RT_NULL);
    RT_ASSERT(cfg != RT_NULL);
    uart_init(115200);
    return RT_EOK;
}

static rt_err_t usart_control(struct rt_serial_device *serial,
                              int cmd, void *arg)
{
    RT_ASSERT(serial != RT_NULL);

    switch (cmd)
    {
    case RT_DEVICE_CTRL_CLR_INT:
        break;
    case RT_DEVICE_CTRL_SET_INT:
        break;
    }

    return RT_EOK;
}

static int usart_putc(struct rt_serial_device *serial, char c)
{
    rt_uint32_t level;
    level = rt_hw_interrupt_disable();
    uart_putchar(c);
    rt_hw_interrupt_enable(level);
    return 0;
}

static int  usart_getc(struct rt_serial_device *serial)
{
    int ch;
    ch = -1;
    if(UART0_REG(UART_IS) & UART_IS_FIFO_NE)
	ch = uart_getchar();
    return ch;
}

static struct rt_uart_ops ops =
{
    usart_configure,
    usart_control,
    usart_putc,
    usart_getc,
};
 
static struct rt_serial_device serial =
{
    .ops = &ops,
    .config.baud_rate = BAUD_RATE_115200,
    .config.bit_order = BIT_ORDER_LSB,
    .config.data_bits = DATA_BITS_8,
    .config.parity    = PARITY_NONE,
    .config.stop_bits = STOP_BITS_1,
    .config.invert    = NRZ_NORMAL,
    .config.bufsz     = RT_SERIAL_RB_BUFSZ,
};

int rt_hw_uart_init(void)
{

    rt_hw_serial_register(
        &serial,
        "dusart",
        RT_DEVICE_FLAG_STREAM
        | RT_DEVICE_FLAG_RDWR
        | RT_DEVICE_FLAG_INT_RX, RT_NULL);

   rt_hw_interrupt_install(
       INT_UART0_BASE,
       usart_handler,
       (void *) &(serial.parent),
       "uart interrupt");

    rt_hw_interrupt_unmask(INT_UART0_BASE);

    return 0;
}
INIT_BOARD_EXPORT(rt_hw_uart_init);
