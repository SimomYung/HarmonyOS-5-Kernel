/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Mar 11 13:24:18 2019
 */

#include "serial_pl011.h"
#include "serial_core.h"
#include "../tty_wrap.h"

#define UART_DR		0x00UL	/* Data register */
#define UART_RSR	0x04UL	/* Receive status register */
#define UART_FR		0x18UL	/* Flag register */
#define UART_IBRD	0x24UL	/* Integer baud rate divisor register */
#define UART_FBRD	0x28UL	/* Fractional baud rate divisor register */
#define UART_LCR_H	0x2cUL	/* Line control register */
#define UART_CR		0x30UL	/* Control register */
#define UART_IMSC	0x38UL	/* Interrupt mask set/clear register */
#define UART_RIS	0x3CUL	/* Raw interrupt status register */
#define UART_MIS	0x40UL	/* Modern interrupt status register */
#define UART_ICR	0x44UL	/* Interrupt clear register */

#define UART_FR_BUSY	(1U<<3)	/* UART busy*/
#define UART_FR_RXFE	(1U<<4)	/* Receive FIFO empty */
#define UART_FR_TXFF	(1U<<5)	/* Transmit FIFO full */
#define UART_FR_RXFF	(1U<<6)	/* Receive FIFO full */
#define UART_FR_TXFE	(1U<<7)	/* Transmit FIFO empty */
#define UART_LCR_H_BRK	1U	/* Send Break */
#define UART_LCR_H_FEN	(1U<<4)	/* Enable FIFOs */
#define UART_IMSC_RXIM	(1U<<4)	/* Receive interrupt mask */
#define UART_IMSC_TXIM	(1U<<5)	/* Transmit interrupt mask */
#define UART_IMSC_RTIM	(1U<<6)	/* Receive Timeout mask */
#define UART_RIS_RXRIS	(1U<<4)	/* Receive interrupt status */
#define UART_RIS_TXRIS	(1U<<5)	/* Transmit interrupt status */
#define UART_RIS_RTRIS	(1U<<6)	/* Receive Timeout interrupt status */
#define UART_RIS_FERIS	(1U<<7)	/* Framing Error interrupt status */
#define UART_RIS_PERIS	(1U<<8)	/* Parity Error interrupt status */
#define UART_ICR_RXIC	(1U<<4)	/* Receive interrupt clear */
#define UART_ICR_RTIC	(1U<<6)	/* Receive timeout interrupt clear */
#define UART_ICR_FEIC	(1U<<7)	/* Framing error interrupt clear */
#define UART_ICR_PEIC	(1U<<8)	/* Parity error interrupt clear */
#define UART_ICR_ALL	(0x7FF)	/* Clear all interrupt status */

#define PL011_BAUD_BIT_OFFSET		4
#define PL011_BASE_BIT_OFFSET		6
#define PL011_BAUD_RATE_FACTOR		(1 << PL011_BAUD_BIT_OFFSET)
#define PL011_BRDF_FACTOR		(1 << PL011_BASE_BIT_OFFSET)
#define PL011_BRDF_ROUNDING_ERROR	500
#define PL011_BRDF_PRECISION		1000
#define TX_FIFO_DEPTH_DEFAULT		32
#define PARITY_ERROR_REPLACE_CHAR	'\0'

static int pl011_get_baud_divisor(unsigned int *speed, unsigned int urtclk,
				  unsigned int reg_high, unsigned int reg_low)
{
	unsigned int ibrd = reg_high;
	unsigned int fbrd = reg_low;
	unsigned long tmp;
	unsigned int brd = urtclk >> PL011_BAUD_BIT_OFFSET;

	/* cannot get available data in regs */
	if (reg_high + reg_low == 0) {
		return -ENODATA;
	}

	if (fbrd > 0) {
		fbrd *= PL011_BRDF_PRECISION;
		fbrd -= PL011_BRDF_ROUNDING_ERROR;
		fbrd >>= PL011_BASE_BIT_OFFSET;
	}

	tmp = (unsigned long)brd * PL011_BRDF_PRECISION;
	tmp /= (unsigned long)ibrd * PL011_BRDF_PRECISION + fbrd;

	*speed = (unsigned int)tmp;
	return 0;
}

static int pl011_set_baud_divisor(unsigned int baudrate, unsigned int urtclk,
				  unsigned int *reg_high, unsigned int *reg_low)
{
	unsigned int ibrd;
	unsigned int fbrd;
	unsigned long tmp;

	if (baudrate == 0) {
		tty_warn("tty: input baudrate error:%u\n", baudrate);
		return -EINVAL;
	}

	/*
	 * the relationship of those params is in pl0ll manual:
	 * UARTIBRD: baud_divint
	 * UARTFBRD: integer(baud_divfac * 2^6 + 0.5)
	 * baud_divint is the integer of urtclk/(brd * 2^4)
	 * baud_divfac is the fac of urtclk/(brd * 2^4)
	 */
	ibrd = urtclk / (PL011_BAUD_RATE_FACTOR * baudrate);

	tmp = (unsigned long)urtclk * PL011_BRDF_PRECISION /
	      (PL011_BAUD_RATE_FACTOR * baudrate);
	tmp -= (unsigned long)ibrd * PL011_BRDF_PRECISION;
	tmp *= PL011_BRDF_FACTOR;
	tmp += PL011_BRDF_ROUNDING_ERROR;
	fbrd = (unsigned int)(tmp / PL011_BRDF_PRECISION);

	*reg_low = fbrd;
	*reg_high = ibrd;
	return 0;
}

bool pl011_rx_fifo_empty(struct serial_device *serdev)
{
	void *mem_base = NULL;

	BUG_ON(serdev == NULL);
	mem_base = (void *)serdev->mem_base;
	return (bool)(tty_io_read32(ptr_add(mem_base, UART_FR)) & UART_FR_RXFE);
}

enum uart_interrupt_type pl011_interrupt_state(const struct serial_device *serdev)
{
	u32 ris;
	void *mem_base = NULL;

	BUG_ON(serdev == NULL);
	mem_base = (void *)serdev->mem_base;

	/* read interrupt type from register, if support OE or BE in the future
	 * add case then return RE interrupt, which stands for all Receive Err-
	 * or interrupt types
	 */
	ris = tty_io_read32(ptr_add(mem_base, UART_RIS));
	if ((ris & UART_RIS_RXRIS) == UART_RIS_RXRIS) {
		return UART_INTR_RX;
	}
	if ((ris & UART_RIS_RTRIS) == UART_RIS_RTRIS) {
		return UART_INTR_RT;
	}
	if ((ris & UART_RIS_TXRIS) == UART_RIS_TXRIS) {
		return UART_INTR_TX;
	}
	if ((ris & UART_RIS_PERIS) == UART_RIS_PERIS ||
	    (ris & UART_RIS_FERIS) == UART_RIS_FERIS) {
		return UART_INTR_RE;
	}
	tty_err("tty: receive unknown interrupt 0x%x from uart\n", ris);

	return UART_INTR_INVALID;
}

int pl011_startup(struct serial_device *serdev)
{
	/*
	 * for interrupt driven input, disable FIFO so interrupt raises
	 * for each inputs
	 */
	u32 lcr_h;
	u32 imsc;
	void *base_addr = NULL;

	BUG_ON(serdev == NULL);
	/*
	 * If the user-mode serial driver does not use interrupt input and
	 * asynchronous output, we do not initialize the serial here to avoid
	 * conflicts with the initialization of the kernel serial driver.
	 */
	if (serdev->input_mode != UART_INPUT_MODE_IRQ &&
	    serdev->output_mode != UART_OUTPUT_MODE_ASYNC) {
		tty_info("tty: does not need to init hardware in input_mode: %u"
			 " and output_mode: %u\n", serdev->input_mode,
			 serdev->output_mode);
		return 0;
	}

	(void)tty_mutex_lock(&serdev->uart_mutex);
	base_addr = (void *)serdev->mem_base;

	/* close all interrupts and clear all interrupt states */
	tty_io_write32(0U, ptr_add(base_addr, UART_IMSC));
	tty_io_write32(UART_ICR_ALL, ptr_add(base_addr, UART_ICR));

	lcr_h = tty_io_read32(ptr_add(base_addr, UART_LCR_H));
	imsc = tty_io_read32(ptr_add(base_addr, UART_IMSC));

	/* step 1: configure interrupt */
	if (serdev->input_mode == UART_INPUT_MODE_IRQ) {
		/* if not enable kvic in dts, enable RXIM */
		imsc |= UART_IMSC_RXIM;
	}
	if (serdev->output_mode == UART_OUTPUT_MODE_ASYNC) {
		imsc |= UART_IMSC_TXIM;
	}

	/*
	 * step 2: configure fifo
	 * The user mode serial port configuration should be consistent
	 * with the kernel mode serial port.
	 */
	if (serdev->fifo_enabled) {
		lcr_h |= UART_LCR_H_FEN; /* enable fifo */
		imsc |= UART_IMSC_RTIM; /* receive timeout interrupt mask */
	} else {
		lcr_h &= ~UART_LCR_H_FEN; /* disable FIFO */
	}
	tty_io_write32(lcr_h, ptr_add(base_addr, UART_LCR_H));
	tty_io_write32(imsc, ptr_add(base_addr, UART_IMSC));
	(void)tty_mutex_unlock(&serdev->uart_mutex);

	return 0;
}

/* error notifier */
void pl011_handle_error(struct serial_device *serdev)
{
	u32 ris;
	u32 icr;
	void *mem_base = NULL;
	char c = PARITY_ERROR_REPLACE_CHAR;

	BUG_ON(serdev == NULL);
	mem_base = (void *)serdev->mem_base;
	ris = tty_io_read32(ptr_add(mem_base, UART_RIS));
	icr = tty_io_read32(ptr_add(mem_base, UART_ICR));

	if ((ris & UART_RIS_PERIS) == UART_RIS_PERIS) {
		icr |= UART_ICR_PEIC;
		serdev->icount.parity++;
	} else if ((ris & UART_RIS_FERIS) == UART_RIS_FERIS) {
		icr |= UART_ICR_FEIC;
		serdev->icount.frame++;
	} else {
		tty_warn("receive char with unknown error, RIS = 0x%x\n", ris);
		return;
	}

	/* char with parity error or framing error will be sent as '\0' */
	(void)tty_io_read32(ptr_add(mem_base, UART_DR));
	(void)tty_line_buf_write(serdev->input_buf, &c, sizeof(c));

	/* clear interrupt */
	tty_io_write32(icr, ptr_add(mem_base, UART_ICR));
}

char pl011_serial_getc(struct serial_device *serdev)
{
	void *mem_base = NULL;

	BUG_ON(serdev == NULL);
	mem_base = (void *)serdev->mem_base;
	return (char)((u32)tty_io_read32(ptr_add(mem_base, UART_DR)) & 0xff);
}

static int __pl011_serial_put_chars(struct serial_device *pl011_dev, const char *tx_buf, int tx_size)
{
	int i;
	int ret;
	void *mem_base = NULL;

	BUG_ON(pl011_dev == NULL);
	mem_base = (void *)pl011_dev->mem_base;
	/* wait until fifo is not full */
	ret = wait_until_reg_not_equal(ptr_add(mem_base, UART_FR), UART_FR_TXFF);
	if (ret < 0) {
		tty_warn("tty: serial is still busy after the longest wait\n");
	}

	for (i = 0; i < tx_size; i++) {
		tty_io_write32((u32)(unsigned char)tx_buf[i], ptr_add(mem_base, UART_DR));
		pl011_dev->icount.tx++;
	}

	/* Transmit buffer isn't empty means data is transfering */
	ret = wait_until_reg_equal(ptr_add(mem_base, UART_FR), UART_FR_TXFE);
	if (ret < 0) {
		tty_warn("tty: serial is still busy after the longest wait\n");
	}

	return 0;
}

int pl011_serial_putc(void *backend, char ch)
{
	int ret;
	struct serial_device *pl011_dev = (struct serial_device *)backend;

	BUG_ON(pl011_dev == NULL);
	(void)tty_mutex_lock(&pl011_dev->uart_mutex);
	ret = __pl011_serial_put_chars(pl011_dev, &ch, sizeof(ch));
	(void)tty_mutex_unlock(&pl011_dev->uart_mutex);
	return ret;
}

int pl011_serial_flush(void *backend)
{
	int ret;
	unsigned int tx_size;
	int i = 0;
	char tx_buf[TX_FIFO_DEPTH_DEFAULT];
	struct serial_device *pl011_dev = (struct serial_device *)backend;

	BUG_ON(pl011_dev == NULL);
	(void)tty_mutex_lock(&pl011_dev->uart_mutex);
	do {
		if (pl011_dev->fifo_enabled) {
			tx_size = TX_FIFO_DEPTH_DEFAULT;
		} else {
			tx_size = sizeof(char);
		}

		mem_zero_a(tx_buf);
		ret = tty_line_buf_read(pl011_dev->output_buf, tx_buf, tx_size);
		if (ret <= 0) {
			break;
		}
		(void)__pl011_serial_put_chars(pl011_dev, tx_buf, ret);
		i += ret;
	} while (ret > 0);
	(void)tty_mutex_unlock(&pl011_dev->uart_mutex);
	if (ret < 0) {
		return ret;
	}
	if (i > 0) {
		tty_line_buf_wake_up_writer(pl011_dev->output_buf);
	}
	return i;
}

int pl011_set_speed(void *backend, unsigned int speed)
{
	struct serial_device *pl011_dev = (struct serial_device *)backend;
	unsigned int urtclk;
	unsigned int ibrd = 0;
	unsigned int fbrd = 0;
	unsigned int lcr_h;
	void *mem_base = NULL;
	int ret;

	BUG_ON(pl011_dev == NULL);
	urtclk = pl011_dev->uart_clk;
	mem_base = pl011_dev->mem_base;
	if (pl011_dev->uart_clk == 0) {
		return -ENOSYS;
	}

	ret = pl011_set_baud_divisor(speed, urtclk, &ibrd, &fbrd);
	if (ret) {
		return ret;
	}
	(void)tty_mutex_lock(&pl011_dev->uart_mutex);

	tty_io_write32(ibrd, ptr_add(mem_base, UART_IBRD));
	tty_io_write32(fbrd, ptr_add(mem_base, UART_FBRD));

	lcr_h = tty_io_read32(ptr_add(mem_base, UART_LCR_H));
	tty_io_write32(lcr_h, ptr_add(mem_base, UART_LCR_H));

	(void)tty_mutex_unlock(&pl011_dev->uart_mutex);
	return 0;
}

/* caller has guarenteed that `speed` is non-NULL */
int pl011_get_speed(void *backend, unsigned int *speed)
{
	unsigned int ibrd;
	unsigned int fbrd;
	unsigned int urtclk;
	void *mem_base = NULL;
	struct serial_device *pl011_dev = (struct serial_device *)backend;

	BUG_ON(pl011_dev == NULL);
	mem_base = pl011_dev->mem_base;

	if (pl011_dev->uart_clk == 0) {
		return -ENOSYS;
	}

	/* the relationship of those params is in pl0ll manual:
	 * UARTIBRD equals to baud_divint
	 * UARTFBRD equals to integer(baud_divfac * 2^6 + 0.5)
	 * baud_divint is the integer of urtclk/(brd * 2^4)
	 * baud_divfac is the fac of urtclk/(brd * 2^4)
	 */
	urtclk = pl011_dev->uart_clk;

	(void)tty_mutex_lock(&pl011_dev->uart_mutex);
	ibrd = tty_io_read32(ptr_add(mem_base, UART_IBRD));
	fbrd = tty_io_read32(ptr_add(mem_base, UART_FBRD));
	(void)tty_mutex_unlock(&pl011_dev->uart_mutex);

	return pl011_get_baud_divisor(speed, urtclk, ibrd, fbrd);
}

void pl011_serial_stop_tx(void *backend)
{
	/*
	 * Since the pl011 does not send characters through interrupts,
	 * there is nothing to do here right now.
	 */
	UNUSED(backend);
}

void pl011_serial_start_tx(void *backend)
{
	/*
	 * Since the pl011 does not send characters through interrupts,
	 * there is nothing to do here right now.
	 */
	UNUSED(backend);
}

void pl011_serial_stop_rx(void *backend)
{
	unsigned int imsc;
	void *mem_base = NULL;
	struct serial_device *pl011_dev = (struct serial_device *)backend;

	BUG_ON(pl011_dev == NULL);
	mem_base = pl011_dev->mem_base;

	(void)tty_mutex_lock(&pl011_dev->uart_mutex);
	imsc = tty_io_read32(ptr_add(mem_base, UART_IMSC));
	if (pl011_dev->fifo_enabled) {
		unsigned int lcr_h;
		imsc &= ~(UART_IMSC_RXIM | UART_IMSC_RTIM);
		/* disable fifo */
		lcr_h = tty_io_read32(ptr_add(mem_base, UART_LCR_H));
		lcr_h &= ~UART_LCR_H_FEN;
		tty_io_write32(lcr_h, ptr_add(mem_base, UART_LCR_H));
	} else {
		imsc &= ~UART_IMSC_RXIM;
	}
	tty_io_write32(imsc, ptr_add(mem_base, UART_IMSC)); /* disable interrupt */
	(void)tty_mutex_unlock(&pl011_dev->uart_mutex);
}

void pl011_serial_start_rx(void *backend)
{
	unsigned int imsc;
	unsigned int icr;
	void *mem_base = NULL;
	struct serial_device *pl011_dev = (struct serial_device *)backend;

	BUG_ON(pl011_dev == NULL);
	mem_base = pl011_dev->mem_base;

	(void)tty_mutex_lock(&pl011_dev->uart_mutex);
	imsc = tty_io_read32(ptr_add(mem_base, UART_IMSC));
	if (pl011_dev->fifo_enabled) {
		unsigned int lcr_h;
		imsc |= (UART_IMSC_RXIM | UART_IMSC_RTIM);
		icr = (UART_ICR_RXIC | UART_ICR_RTIC);
		/* enable fifo */
		lcr_h = tty_io_read32(ptr_add(mem_base, UART_LCR_H));
		lcr_h |= UART_LCR_H_FEN;
		tty_io_write32(lcr_h, ptr_add(mem_base, UART_LCR_H));
	} else {
		imsc |= UART_IMSC_RXIM;
		icr = UART_ICR_RXIC;
	}
	tty_io_write32(icr, ptr_add(mem_base, UART_ICR)); /* clear interrupt */
	tty_io_write32(imsc, ptr_add(mem_base, UART_IMSC)); /* enable interrupt */
	(void)tty_mutex_unlock(&pl011_dev->uart_mutex);
}

int pl011_serial_get_icount(void *backend,
			    struct serial_icounter_struct *icount)
{
	struct serial_device *serdev = (struct serial_device *)backend;
	int ret;

	if (backend == NULL || icount == NULL) {
		return -EIO;
	}
	/* Read data from serdev->icount, we do not lock here */
	ret = memcpy_s(icount, sizeof(struct serial_icounter_struct),
		       &serdev->icount, sizeof(struct serial_icounter_struct));
	if (ret != 0) {
		return -EIO;
	}
	return 0;
}

/*
 * Follow the linux implementation. When need to output a break signal, set the
 * BRK bit of the register UART_LCR_H to 1. Writing 0 will stop the output of the
 * break signal.
 */
int pl011_serial_break_ctl(void *backend, int state)
{
	unsigned int lcr_h;
	void *mem_base = NULL;
	struct serial_device *pl011_dev = (struct serial_device *)backend;

	BUG_ON(pl011_dev == NULL);
	mem_base = pl011_dev->mem_base;

	(void)tty_mutex_lock(&pl011_dev->uart_mutex);
	lcr_h = tty_io_read32(ptr_add(mem_base, UART_LCR_H));
	if (state == TTY_BREAK_CTL_ENABLE) {
		lcr_h |= UART_LCR_H_BRK;
	} else {
		lcr_h &= ~UART_LCR_H_BRK;
	}
	tty_io_write32(lcr_h, ptr_add(mem_base, UART_LCR_H));
	(void)tty_mutex_unlock(&pl011_dev->uart_mutex);

	return 0;
}
