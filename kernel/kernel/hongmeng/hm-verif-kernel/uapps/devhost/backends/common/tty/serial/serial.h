/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Structures of uart serial device
 * Author: Huawei OS Kernel Lab
 * Create: Sat Mar 02 20:52:48 2019
 */

#ifndef SERIAL_H
#define SERIAL_H

#include "../tty.h"
#include "../tty_wrap.h"

struct serial_device;

enum uart_interrupt_type {
	UART_INTR_RX = 0,	/* receive data interrupt */
	UART_INTR_RE,		/* receive error interrupt, inc. PE,OE,FE,BE */
	UART_INTR_RT,		/* receive timeout interrupt */
	UART_INTR_TX,		/* transmit interrupt */
	UART_INTR_IGNORE,	/* interrupt to ignore */
	UART_INTR_INVALID
};

struct uart_ops {
	bool	(*rx_empty)(struct serial_device *serdev);
	char	(*getc)(struct serial_device *serdev);
	void	(*handle_error)(struct serial_device *serdev);
	enum uart_interrupt_type (*interrupt_state)(const struct serial_device *serdev);
	int	(*tx_async)(struct serial_device *serdev);
};

struct serial_device {
	const struct uart_ops	*uops;
	int			irq; /* irq number */
	const char		*dev_name;
	struct tty_struct	*tty;
	const struct tty_backend_operations      *tty_bops;
	struct tty_mutex_t	uart_mutex; /* Used to lock the write operations on hardware */
	void			*mem_base; /* memory base of uart device */
	size_t			mem_size; /* memory size to ioremap */
	unsigned int		uart_clk; /* current uart clock frequency in dts */
	int			fifo_enabled;
	struct serial_icounter_struct icount;
	struct tty_line_buf	*input_buf;
	struct tty_line_buf	*output_buf;
	unsigned int		input_mode;
	unsigned int		output_mode;
	/*
	 * Since the serial does not use interrupt output at present, it cannot
	 * be turned off in the hardware, this flag bit is added to control the
	 * output of the serial port in putc function.
	 */
	bool			output_disabled;
};

#define UART_FIFO_DISABLED	0
#define UART_FIFO_ENABLED	1
/* serial I/O control flag, 0 is off and 1 is on */
#define UART_OUTPUT_STATE_ON	0x1U /* output control bit */
#define UART_INPUT_STATE_ON	(0x1U << 1) /* input control bit */
#define MAX_STATUS_CHECK_COUNT	(0x1U << 20)

#define UART_INPUT_MODE_NONE 0
#define UART_INPUT_MODE_KVIC 1
#define UART_INPUT_MODE_IRQ  2

#define UART_OUTPUT_MODE_SYNC  1
#define UART_OUTPUT_MODE_ASYNC 2

static inline int wait_until_reg_equal(const void *reg_addr, u32 status)
{
	u32 i = 0;

	while ((((u32)tty_io_read32(reg_addr) & status) != status) &&
	       (i < MAX_STATUS_CHECK_COUNT)) {
		cpu_relax();
		i++;
	}
	return (i < MAX_STATUS_CHECK_COUNT) ? 0 : -EBUSY;
}

static inline int wait_until_reg_not_equal(const void *reg_addr, u32 status)
{
	u32 i = 0;

	while ((((u32)tty_io_read32(reg_addr) & status) == status) &&
	       (i < MAX_STATUS_CHECK_COUNT)) {
		cpu_relax();
		i++;
	}
	return (i < MAX_STATUS_CHECK_COUNT) ? 0 : -EBUSY;
}

#endif
