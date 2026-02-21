/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Dec 04 18:42:56 2019
 */

#ifndef SERIAL_PL011_H
#define SERIAL_PL011_H

#include "serial.h"
#include "../tty_wrap.h"

bool pl011_rx_fifo_empty(struct serial_device *serdev);
int pl011_startup(struct serial_device *serdev);
char pl011_serial_getc(struct serial_device *serdev);
void pl011_handle_error(struct serial_device *serdev);
enum uart_interrupt_type pl011_interrupt_state(const struct serial_device *serdev);

int pl011_serial_putc(void *backend, char ch);
int pl011_serial_flush(void *backend);
int pl011_set_speed(void *backend, unsigned int speed);
int pl011_get_speed(void *backend, unsigned int *speed);
void pl011_serial_stop_tx(void *backend);
void pl011_serial_start_tx(void *backend);
void pl011_serial_stop_rx(void *backend);
void pl011_serial_start_rx(void *backend);
int pl011_serial_get_icount(void *backend, struct serial_icounter_struct *icount);
int pl011_serial_break_ctl(void *backend, int state);

#endif
