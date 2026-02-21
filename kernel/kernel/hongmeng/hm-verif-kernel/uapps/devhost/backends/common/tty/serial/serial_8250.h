/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Prototypes of dw8250 serial driver
 * Author: Huawei OS Kernel Lab
 * Create: Wed Dec 04 18:42:56 2019
 */

#ifndef SERIAL_8250_H
#define SERIAL_8250_H

#include "serial.h"
#include "../tty_wrap.h"

bool dw8250_rx_empty(struct serial_device *serdev);
int dw8250_tx_async(struct serial_device *serdev);
int dw8250_startup(struct serial_device *serdev, unsigned int init_io_state);
char dw8250_serial_getc(struct serial_device *serdev);
void dw8250_handle_error(struct serial_device *serdev);
enum uart_interrupt_type dw8250_interrupt_state(const struct serial_device *serdev);

int dw8250_serial_putc(void *backend, char ch);
int dw8250_serial_flush(void *backend);
void dw8250_serial_stop_tx(void *backend);
void dw8250_serial_start_tx(void *backend);
void dw8250_serial_stop_rx(void *backend);
void dw8250_serial_start_rx(void *backend);

int dw8250_serial_get_speed(void *backend, unsigned int *speed);
int dw8250_serial_set_speed(void *backend, unsigned int speed);
int dw8250_serial_set_termios(void *backend, const struct termios *tio);
int dw8250_serial_get_termios(void *backend, struct termios *tio);
int dw8250_serial_get_icount(void *backend,
			     struct serial_icounter_struct *icount);
int dw8250_serial_break_ctl(void *backend, int state);

#endif
