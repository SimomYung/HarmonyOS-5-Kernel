/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Uart framework interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 14 19:15:55 2022
 */

#ifndef __UDK_UART_H__
#define __UDK_UART_H__

#include <stdbool.h>
#include <termios.h>

#define UDK_UART_DRV_INTR_RX		0 /* receive data interrupt */
#define UDK_UART_DRV_INTR_RE		1 /* receive error interrupt, inc. PE,OE,FE,BE */
#define UDK_UART_DRV_INTR_RT		2 /* receive timeout interrupt */
#define UDK_UART_DRV_INTR_INVALID	3

struct udk_uart;

struct udk_uart_ops {
	int (*putc)(struct udk_uart *uart, char ch);
	int (*flush)(struct udk_uart *uart);
	int (*set_speed)(struct udk_uart *uart, unsigned int speed);
	int (*get_speed)(struct udk_uart *uart, unsigned int *speed);
	void (*stop_tx)(struct udk_uart *uart);
	void (*start_tx)(struct udk_uart *uart);
	void (*stop_rx)(struct udk_uart *uart);
	void (*start_rx)(struct udk_uart *uart);
	int (*set_termios)(struct udk_uart *uart, const struct termios *tio);
	int (*get_termios)(struct udk_uart *uart, struct termios *tio);
	int (*break_ctl)(struct udk_uart *uart, int state);
	bool (*rx_empty)(struct udk_uart *uart);
	char (*getc)(struct udk_uart *uart);
	void (*handle_error)(struct udk_uart *uart);
	int (*interrupt_state)(struct udk_uart *uart);
	void (*interrupt_clean)(struct udk_uart *uart, int state);
	int (*irq_handler)(struct udk_uart *uart);
};

struct udk_uart_desc {
	unsigned int irq;
	void *private;
	const char *name;
	struct udk_uart_ops *uart_ops;
};

struct udk_uart *udk_uart_register(struct udk_uart_desc *desc);
void udk_uart_unregister(struct udk_uart *uart);
void *udk_uart_get_description(const struct udk_uart *uart);
int udk_uart_rx_write(const struct udk_uart *uart, const char *buf, unsigned int size);
int udk_uart_tx_read(const struct udk_uart *uart, char *buf, unsigned int size);
int udk_uart_tx_wake_up(const struct udk_uart *uart);
void udk_uart_tx_inc(struct udk_uart *uart);
void udk_uart_parity_inc(struct udk_uart *uart);
void udk_uart_frame_inc(struct udk_uart *uart);

#endif
