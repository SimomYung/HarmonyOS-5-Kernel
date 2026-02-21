/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Define dw8250 serial driver
 * Author: Huawei OS Kernel Lab
 * Create: Mon Mar 11 13:24:18 2019
 */

#include "serial_8250.h"
#include "../tty_wrap.h"

/* Following registers depend on LCR[7] (DLAB=0) */
#define UART_RBR		0x0	/* R:  Receive Buffer Register */
#define UART_THR		0x0	/* W:  Transmit Holding Register */
#define UART_IER		0x4	/* RW: Interrupt Enable Register */
#define UART_IER_ERBFI		0x1	/* Enable Received Data Available Interrupt */
#define UART_IER_ETBEI		0x2	/* Enable Transmit Holding Register Empty Interrupt */
#define UART_IER_ELSI		0x4	/* Enable Receiver Line Status Interrupt */
/* Following registers depend on LCR[7] (DLAB=1) */
#define UART_DLL		0x0	/* RW: Divisor Latch Low */
#define UART_DLH		0x4	/* RW: Divisor Latch High */

#define UART_IIR		0x8	/* R:  Interrupt Identification Register, Reset: 0x01 */
#define UART_IIR_IID_NO_INTERRUPT  1	/* IIR status: no interrupt */
#define UART_IIR_IID_THR_EMPTY     2	/* IIR status: transmit holding register empty */
#define UART_IIR_IID_RECEIVED_DATA 4	/* IIR status: received data available */
#define UART_IIR_IID_RECEIVER_LINE 6	/* IIR status: line break error */
#define UART_IIR_IID_TIMEOUT       12	/* IIR status: timeout for fifo */
#define UART_FCR		0x8	/* W:  FIFO Control Register,   Reset: 0x0 */
#define UART_FCR_FIFOE		0x1	/* W:  Enable FIFO */
#define UART_FCR_RX_TRIG_HALF	0x80	/* W:  Select FIFO trigger level, 10 = FIFO 1/2 full */
#define UART_LCR		0xC	/* RW: Line Control Register,   Reset: 0x0 */
#define UART_LCR_DLS_5BITS	0x0	/* 5bits data length */
#define UART_LCR_DLS_6BITS	0x1	/* 6bits data length */
#define UART_LCR_DLS_7BITS	0x2	/* 7bits data length */
#define UART_LCR_DLS_8BITS	0x3	/* 8bits data length */
#define UART_LCR_DLS_MASK	0x03
#define UART_LCR_PEN		0x08	/* RW: Enable Parity */
#define UART_LCR_EPS		0x10	/* RW: Select Even Parity, 0 = Odd, 1 = Even */
#define UART_LCR_STOP		0x04	/* RW: Number of Stop Bits */
#define UART_LCR_BC		0x40	/* RW: Break Control Bit */
#define UART_LSR		0x14	/* R:  Line Status Register,    Reset: 0x60 */
#define UART_LSR_TEMT		0x40	/* R:  Transmitter EMpTy */
#define UART_LSR_THRE		0x20	/* R:  Transmit-Hold-Register Empty */
#define UART_LSR_BOTH_EMPTY	(UART_LSR_TEMT | UART_LSR_THRE)
#define UART_LSR_PE		0x04	/* R:  Parity Error bit */
#define UART_LSR_FE		0x08	/* R:  Framing Error bit */
#define UART_LSR_DR		0x1	/* R:  Data Ready bit */
/* MSR, SCR, LPDLL, LPDLH, SRBR, STHR are not implemented */
#define UART_USR		0x7C	/* R:  UART Status Register,    Reset: 0x6 */
#define UART_USR_BUSY		0x1	/* UART Busy Bit */
/* UCV, CTR are not implemented */
#define UART_LCR_DLAB		(1 << 7)
#define UART_BIT_FACTOR		(1 << 4)
#define UART_DLH_OFFSET		(1 << 3)
#define UART_DLL_MASK		0x00ff
#define UART_DLH_MASK		0xff00

#define TX_FIFO_DEPTH_DEFAULT	16U
#define PARITY_ERROR_REPLACE_CHAR '\0'

static void do_serial_stop_tx(struct serial_device *serdev);
static void do_serial_stop_rx(struct serial_device *serdev);

static int dw8250_serial_calc_speed(unsigned int *speed, unsigned int urtclk,
				    unsigned int reg_dlh, unsigned int reg_dll)
{
	unsigned int divisor;

	/* cannot get available data in regs */
	if (reg_dll + reg_dlh == 0) {
		*speed = 0;
		return 0;
	}

	divisor = (reg_dlh << UART_DLH_OFFSET) + reg_dll;
	*speed = urtclk / (UART_BIT_FACTOR * divisor);
	return 0;
}

static int dw8250_serial_calc_divisor(unsigned int baudrate, unsigned int urtclk,
				      unsigned int *reg_dlh, unsigned int *reg_dll)
{
	unsigned int divisor;

	if (baudrate == 0) {
		tty_warn("tty: input baudrate error, baudrate=%u\n", baudrate);
		return -EINVAL;
	}

	divisor = urtclk / (baudrate * UART_BIT_FACTOR);
	*reg_dlh = (divisor & UART_DLH_MASK) >> UART_DLH_OFFSET;
	*reg_dll = divisor & UART_DLL_MASK;
	return 0;
}

int dw8250_serial_set_speed(void *backend, unsigned int speed)
{
	int ret;
	unsigned int urtclk;
	unsigned int reg_dlh;
	unsigned int reg_dll;
	unsigned int dw8250_lcr;
	void *base_addr = NULL;
	struct serial_device *serdev = NULL;

	serdev = (struct serial_device *)backend;
	BUG_ON(serdev == NULL);
	if (serdev->uart_clk == 0) {
		return -ENOSYS;
	}

	base_addr = serdev->mem_base;
	urtclk = serdev->uart_clk;
	ret = dw8250_serial_calc_divisor(speed, urtclk, &reg_dlh, &reg_dll);
	if (ret != 0) {
		return ret;
	}

	(void)tty_mutex_lock(&serdev->uart_mutex);
	/*
	 * 1. DLAB(LCR[7]):Divisor Latch Access Bit.
	 *    Writeable only when UART is not busy (USR[0] is zero), always readable.
	 * 2. DLH and DLL register may only be accessed when the
	 *    DLAB bit (LCR[7]) is set and the UART is not busy (USR[0] is zero).
	 * */
	dw8250_lcr = tty_io_read32(ptr_add(base_addr, UART_LCR));
	/* wait until UART_USR is not busy */
	ret = wait_until_reg_not_equal(ptr_add(base_addr, UART_USR), UART_USR_BUSY);
	if (ret < 0) {
		tty_warn("tty: serial is still busy after the longest wait\n");
	}

	dw8250_lcr |= UART_LCR_DLAB;
	tty_io_write32(dw8250_lcr, ptr_add(base_addr, UART_LCR)); /* set DLAB(LCR[7]) to 1 */

	tty_io_write32(reg_dlh, ptr_add(base_addr, UART_DLH));
	tty_io_write32(reg_dll, ptr_add(base_addr, UART_DLL));

	dw8250_lcr &= ~UART_LCR_DLAB;
	tty_io_write32(dw8250_lcr, ptr_add(base_addr, UART_LCR));
	(void)tty_mutex_unlock(&serdev->uart_mutex);
	return 0;
}

int dw8250_serial_get_speed(void *backend, unsigned int *speed)
{
	int ret;
	unsigned int reg_dlh;
	unsigned int reg_dll;
	unsigned int dw8250_lcr;
	unsigned int urtclk;
	void *base_addr = NULL;
	struct serial_device *serdev = NULL;

	serdev = (struct serial_device *)backend;
	BUG_ON(serdev == NULL);
	if (serdev->uart_clk == 0) {
		return -ENOSYS;
	}

	base_addr = serdev->mem_base;
	urtclk = serdev->uart_clk;
	/* the relationship of those params is in dw8250 manual:
	 * DLL = Divisor Latch Low 8 bits of 16 bits
	 * DLH = Divisor Latch high 8 bits of 16 bits
	 * baud rate = (serial clock freq) / (16 * divisor).
	 */
	(void)tty_mutex_lock(&serdev->uart_mutex);
	dw8250_lcr = tty_io_read32(ptr_add(base_addr, UART_LCR));
	/* wait until UART_USR is not busy */
	ret = wait_until_reg_not_equal(ptr_add(base_addr, UART_USR), UART_USR_BUSY);
	if (ret < 0) {
		tty_warn("tty: serial is still busy after the longest wait\n");
	}

	dw8250_lcr |= UART_LCR_DLAB;
	tty_io_write32(dw8250_lcr, ptr_add(base_addr, UART_LCR)); /* set DLAB(LCR[7]) to 1 */

	reg_dlh = tty_io_read32(ptr_add(base_addr, UART_DLH));
	reg_dll = tty_io_read32(ptr_add(base_addr, UART_DLL));

	dw8250_lcr &= ~UART_LCR_DLAB;
	tty_io_write32(dw8250_lcr, ptr_add(base_addr, UART_LCR));
	(void)tty_mutex_unlock(&serdev->uart_mutex);

	return dw8250_serial_calc_speed(speed, urtclk, reg_dlh, reg_dll);
}

/*
 * actually in 8250 there is no register to reprenent rx empty,
 * here we use data ready bit to reprenent rx not empty
 */
bool dw8250_rx_empty(struct serial_device *serdev)
{
	u32 data_ready;
	void *mem_base = NULL;
	BUG_ON(serdev == NULL);
	mem_base = (void *)serdev->mem_base;
	data_ready = tty_io_read32(ptr_add(mem_base, UART_LSR)) & UART_LSR_DR;
	return (data_ready == 0);
}

#define UART_INTERRUPT_MASK 0xF
enum uart_interrupt_type dw8250_interrupt_state(const struct serial_device *serdev)
{
	u32 iid;
	void *mem_base = NULL;

	BUG_ON(serdev == NULL);
	mem_base = (void *)serdev->mem_base;

	/* read interrupt type from register */
	iid = tty_io_read32(ptr_add(mem_base, UART_IIR));
	iid &= UART_INTERRUPT_MASK;
	switch (iid) {
	case UART_IIR_IID_NO_INTERRUPT:
		return UART_INTR_IGNORE;
	case UART_IIR_IID_RECEIVED_DATA:
		return UART_INTR_RX;
	case UART_IIR_IID_RECEIVER_LINE:
		return UART_INTR_RE;
	case UART_IIR_IID_TIMEOUT:
		return UART_INTR_RT;
	case UART_IIR_IID_THR_EMPTY:
		return UART_INTR_TX;
	default:
		tty_err("tty: receive unknown interrupt ID 0x%x from uart\n", iid);
		return UART_INTR_INVALID;
	}
}

int dw8250_startup(struct serial_device *serdev, unsigned int init_io_state)
{
	/*
	 * for interrupt driven input, disable FIFO so interrupt raises
	 * for each inputs
	 */
	int ret;
	u32 ier;
	void *base_addr = NULL;

	BUG_ON(serdev == NULL);
	base_addr = (void *)serdev->mem_base;

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
	/* step 1: configure interrupt */
	ier = tty_io_read32(ptr_add(base_addr, UART_IER));
	if (serdev->input_mode == UART_INPUT_MODE_IRQ) {
		/* if not enable kvic in dts, enable ELSI and ERBFI */
		ier |= (UART_IER_ELSI | UART_IER_ERBFI);
	}
	if (serdev->output_mode == UART_OUTPUT_MODE_ASYNC) {
		ier |= UART_IER_ETBEI;
	}
	tty_io_write32(ier, ptr_add(base_addr, UART_IER));

	/* step 2: configure transmit data len when USR is not busy */
	ret = wait_until_reg_not_equal(ptr_add(base_addr, UART_USR), UART_USR_BUSY);
	if (ret < 0) {
		tty_warn("tty: serial is still busy after the longest wait\n");
	}
	tty_io_write32(UART_LCR_DLS_8BITS, ptr_add(base_addr, UART_LCR));

	/* step 3: configure fifo if enabled in dts */
	if ((serdev->fifo_enabled) != 0U) {
		tty_io_write32(UART_FCR_FIFOE | UART_FCR_RX_TRIG_HALF, ptr_add(base_addr, UART_FCR));
	} else {
		tty_io_write32(0, ptr_add(base_addr, UART_FCR));
	}

	/* step 4: configure init io state according to dts */
	if ((init_io_state & UART_OUTPUT_STATE_ON) != UART_OUTPUT_STATE_ON) {
		do_serial_stop_tx(serdev);
	}
	if ((init_io_state & UART_INPUT_STATE_ON) != UART_INPUT_STATE_ON) {
		do_serial_stop_rx(serdev);
	}
	(void)tty_mutex_unlock(&serdev->uart_mutex);

	return 0;
}

/* error notifier */
void dw8250_handle_error(struct serial_device *serdev)
{
	u32 lsr;
	void *mem_base = NULL;
	char c = PARITY_ERROR_REPLACE_CHAR;

	BUG_ON(serdev == NULL);
	mem_base = (void *)serdev->mem_base;

	/*
	 * line status register(LSR) will be cleared after read
	 * we handle framing error in the same way as parity error
	 */
	lsr = tty_io_read32(ptr_add(mem_base, UART_LSR));
	if ((lsr & UART_LSR_PE) == UART_LSR_PE) {
		serdev->icount.parity++;
	} else if ((lsr & UART_LSR_FE) ==  UART_LSR_FE) {
		serdev->icount.frame++;
	} else {
		tty_warn("tty: get char with unknown error, LSR = 0x%x\n", lsr);
		return;
	}

	/* clear RBR, sent '\0' */
	(void)tty_io_read32(ptr_add(mem_base, UART_RBR));
	(void)tty_line_buf_write(serdev->input_buf, &c, sizeof(c));
}

char dw8250_serial_getc(struct serial_device *serdev)
{
	u32 lsr;
	u32 rbr;
	void *mem_base = NULL;

	BUG_ON(serdev == NULL);
	mem_base = (void *)serdev->mem_base;

	lsr = tty_io_read32(ptr_add(mem_base, UART_LSR));
	if ((lsr & UART_LSR_DR) == 0U) {
		tty_warn("tty: interrupt happen but data is not ready, LSR=0x%x\n", lsr);
		rbr = tty_io_read32(ptr_add(mem_base, UART_RBR));
		tty_warn("tty: so get char from old rbr, rbr=0x%x\n", rbr);
		return (char)rbr;
	}

	rbr = tty_io_read32(ptr_add(mem_base, UART_RBR));
	return (char)rbr;
}

static int __dw8250_serial_put_chars(struct serial_device *serdev,
				     const char *tx_buf, unsigned int tx_size,
				     bool need_wait)
{
	int ret;
	unsigned int i;
	void *mem_base = NULL;

	BUG_ON(serdev == NULL);
	mem_base = (void *)serdev->mem_base;

	if (serdev->output_disabled) {
		return 0;
	}
	if (need_wait) {
		/* write to THR when LSR is both empty */
		ret = wait_until_reg_equal(ptr_add(mem_base, UART_LSR),
						   UART_LSR_BOTH_EMPTY);
		if (ret < 0) {
			tty_warn("tty: serial is still not empty after the longest wait\n");
		}
	}

	for (i = 0; i < tx_size; i++) {
		tty_io_write32((u32)(unsigned char)tx_buf[i], ptr_add(mem_base, UART_THR));
		serdev->icount.tx++;
	}

	return 0;
}

static void enable_tx_intr(struct serial_device *serdev)
{
	u32 ier;
	void *mem_base = serdev->mem_base;

	ier = tty_io_read32(ptr_add(mem_base, UART_IER));
	ier |= UART_IER_ETBEI;
	tty_io_write32(ier, ptr_add(mem_base, UART_IER));
}

static void disable_tx_intr(struct serial_device *serdev)
{
	u32 ier;
	void *mem_base = serdev->mem_base;

	ier = tty_io_read32(ptr_add(mem_base, UART_IER));
	ier &= ~(UART_IER_ETBEI);
	tty_io_write32(ier, ptr_add(mem_base, UART_IER));
}

static int do_tx_async(struct serial_device *serdev)
{
	int ret;
	unsigned int tx_size = 1U;
	char tx_buf[TX_FIFO_DEPTH_DEFAULT];

	if ((serdev->fifo_enabled) != 0U) {
		tx_size = TX_FIFO_DEPTH_DEFAULT;
	}
	ret = tty_line_buf_read(serdev->output_buf, tx_buf, tx_size);
	if (ret <= 0) {
		disable_tx_intr(serdev);
		return ret;
	}
	(void)__dw8250_serial_put_chars(serdev, tx_buf, (unsigned int)ret,
					false);
	tty_line_buf_wake_up_writer(serdev->output_buf);

	return 0;
}

int dw8250_tx_async(struct serial_device *serdev)
{
	int ret;

	BUG_ON(serdev == NULL);
	(void)tty_mutex_lock(&serdev->uart_mutex);
	ret = do_tx_async(serdev);
	(void)tty_mutex_unlock(&serdev->uart_mutex);

	return ret;
}

int dw8250_serial_putc(void *backend, char ch)
{
	int ret;
	char swap_ch;
	struct serial_device *serdev = (struct serial_device *)backend;

	BUG_ON(serdev == NULL);
	(void)tty_mutex_lock(&serdev->uart_mutex);
	if (serdev->output_mode == UART_OUTPUT_MODE_SYNC) {
		ret = __dw8250_serial_put_chars(serdev, &ch, sizeof(ch), true);
		(void)tty_mutex_unlock(&serdev->uart_mutex);
		return ret;
	}

	ret = tty_line_buf_write_maybe_swapout(serdev->output_buf,
					       &ch, sizeof(char),
					       &swap_ch, sizeof(char));
	if (ret > 0) {
		ret = __dw8250_serial_put_chars(serdev, &swap_ch, sizeof(char),
						true);
	}
	enable_tx_intr(serdev);
	(void)tty_mutex_unlock(&serdev->uart_mutex);

	return ret;
}

int dw8250_serial_flush(void *backend)
{
	int ret;
	unsigned int tx_size;
	int i = 0;
	char tx_buf[TX_FIFO_DEPTH_DEFAULT];
	struct serial_device *serdev = (struct serial_device *)backend;

	BUG_ON(serdev == NULL);
	(void)tty_mutex_lock(&serdev->uart_mutex);
	if (serdev->output_mode == UART_OUTPUT_MODE_ASYNC) {
		enable_tx_intr(serdev);
		(void)tty_mutex_unlock(&serdev->uart_mutex);
		return 0;
	}

	do {
		if ((serdev->fifo_enabled) != 0U) {
			tx_size = TX_FIFO_DEPTH_DEFAULT;
		} else {
			tx_size = sizeof(char);
		}

		mem_zero_a(tx_buf);
		ret = tty_line_buf_read(serdev->output_buf, tx_buf, tx_size);
		if (ret <= 0) {
			break;
		} else if ((unsigned int)ret > TX_FIFO_DEPTH_DEFAULT) {
			(void)tty_mutex_unlock(&serdev->uart_mutex);
			return -EIO;
		}
		(void)__dw8250_serial_put_chars(serdev, tx_buf,
						(unsigned int)ret, true);
		i += ret;
	} while (ret > 0);
	(void)tty_mutex_unlock(&serdev->uart_mutex);
	if (ret < 0) {
		return ret;
	}
	if (i > 0) {
		tty_line_buf_wake_up_writer(serdev->output_buf);
	}
	return i;
}

static void do_serial_stop_tx(struct serial_device *serdev)
{
	serdev->output_disabled = true;
	if (serdev->output_mode == UART_OUTPUT_MODE_ASYNC) {
		disable_tx_intr(serdev);
	}
}

void dw8250_serial_stop_tx(void *backend)
{
	struct serial_device *serdev = (struct serial_device *)backend;

	BUG_ON(serdev == NULL);
	(void)tty_mutex_lock(&serdev->uart_mutex);
	do_serial_stop_tx(serdev);
	(void)tty_mutex_unlock(&serdev->uart_mutex);
}

static void do_serial_start_tx(struct serial_device *serdev)
{
	serdev->output_disabled = false;
	if (serdev->output_mode == UART_OUTPUT_MODE_ASYNC) {
		enable_tx_intr(serdev);
		(void)do_tx_async(serdev);
	}
}

void dw8250_serial_start_tx(void *backend)
{
	struct serial_device *serdev = (struct serial_device *)backend;

	BUG_ON(serdev == NULL);
	(void)tty_mutex_lock(&serdev->uart_mutex);
	do_serial_start_tx(serdev);
	(void)tty_mutex_unlock(&serdev->uart_mutex);
}

static void do_serial_stop_rx(struct serial_device *serdev)
{
	u32 ier;
	void *mem_base = NULL;

	mem_base = (void *)serdev->mem_base;
	ier = tty_io_read32(ptr_add(mem_base, UART_IER));
	ier &= ~(UART_IER_ERBFI | UART_IER_ELSI);
	tty_io_write32(ier, ptr_add(mem_base, UART_IER));
}

void dw8250_serial_stop_rx(void *backend)
{
	struct serial_device *serdev = (struct serial_device *)backend;

	BUG_ON(serdev == NULL);
	(void)tty_mutex_lock(&serdev->uart_mutex);
	do_serial_stop_rx(serdev);
	(void)tty_mutex_unlock(&serdev->uart_mutex);
}

static void do_serial_start_rx(struct serial_device *serdev)
{
	u32 ier;
	void *mem_base = NULL;

	mem_base = (void *)serdev->mem_base;
	ier = tty_io_read32(ptr_add(mem_base, UART_IER));
	ier |= (UART_IER_ERBFI | UART_IER_ELSI);
	/*
	 * In non-FIFO mode, before re-enabling the rx interrupt, clear the
	 * receive interrupt by reading the receive buffer register.
	 * In FIFO mode, chars left will be cleared by reading RBR
	 */
	while (!dw8250_rx_empty(serdev)) {
		(void)tty_io_read32(ptr_add(mem_base, UART_RBR));
	}

	tty_io_write32(ier, ptr_add(mem_base, UART_IER));
}

void dw8250_serial_start_rx(void *backend)
{
	struct serial_device *serdev = (struct serial_device *)backend;

	BUG_ON(serdev == NULL);
	(void)tty_mutex_lock(&serdev->uart_mutex);
	do_serial_start_rx(serdev);
	(void)tty_mutex_unlock(&serdev->uart_mutex);
}

static u32 __set_csize(const struct termios *tio, u32 lcr)
{
	/* clear lcr last 2 bits to set CSIZE */
	lcr &= (~UART_LCR_DLS_MASK);
	switch (tio->c_cflag & CSIZE) {
	case CS5:
		lcr |= UART_LCR_DLS_5BITS;
		break;
	case CS6:
		lcr |= UART_LCR_DLS_6BITS;
		break;
	case CS7:
		lcr |= UART_LCR_DLS_7BITS;
		break;
	case CS8:
		lcr |= UART_LCR_DLS_8BITS;
		break;
	default:
		/* defaultly set as CS8 */
		lcr |= UART_LCR_DLS_8BITS;
	}
	return lcr;
}

static u32 __set_parity(const struct termios *tio, u32 lcr)
{
	/* set parity with user-defined termios, write to register */
	if ((tio->c_cflag & PARENB) != 0U) { /* enable parity */
		lcr |= UART_LCR_PEN;
		if ((tio->c_cflag & PARODD) != 0U) {
			lcr &= (~UART_LCR_EPS); /* set odd parity */
		} else {
			lcr |= UART_LCR_EPS; /* set even parity */
		}
	} else { /* disable parity */
		lcr &= (~UART_LCR_PEN);
	}
	return lcr;
}

static u32 __set_stop_bit(const struct termios *tio, u32 lcr)
{
	/* if set CSTOPB, dw8250 will send 1.5 stop bits with CSIZE 5 bits,
	 * otherweise 2 stop bits are transmitted, handled by hardware
	 */
	if ((tio->c_cflag & CSTOPB) != 0U) {
		lcr |= UART_LCR_STOP;
	} else {
		lcr &= (~UART_LCR_STOP);
	}
	return lcr;
}

int dw8250_serial_set_termios(void *backend, const struct termios *tio)
{
	int ret;
	u32 lcr;
	void *mem_base = NULL;
	struct serial_device *serdev = (struct serial_device *)backend;

	if (backend == NULL || tio == NULL) {
		return -EIO;
	}
	mem_base = (void *)serdev->mem_base;

	(void)tty_mutex_lock(&serdev->uart_mutex);
	lcr = tty_io_read32(ptr_add(mem_base, UART_LCR));

	lcr = __set_csize(tio, lcr);
	lcr = __set_parity(tio, lcr);
	lcr = __set_stop_bit(tio, lcr);

	/* wait until UART_USR is not busy */
	ret = wait_until_reg_not_equal(ptr_add(mem_base, UART_USR), UART_USR_BUSY);
	if (ret < 0) {
		tty_warn("tty: serial is still busy after the longest wait\n");
	}
	tty_io_write32(lcr, ptr_add(mem_base, UART_LCR));
	(void)tty_mutex_unlock(&serdev->uart_mutex);
	return 0;
}

int dw8250_serial_get_termios(void *backend, struct termios *tio)
{
	u32 lcr;
	void *mem_base = NULL;
	struct serial_device *serdev = (struct serial_device *)backend;

	if (backend == NULL || tio == NULL) {
		return -EIO;
	}
	mem_base = (void *)serdev->mem_base;

	(void)tty_mutex_lock(&serdev->uart_mutex);
	lcr = tty_io_read32(ptr_add(mem_base, UART_LCR));
	switch (lcr & UART_LCR_DLS_MASK) {
	case UART_LCR_DLS_5BITS:
		tio->c_cflag |= CS5;
		break;
	case UART_LCR_DLS_6BITS:
		tio->c_cflag |= CS6;
		break;
	case UART_LCR_DLS_7BITS:
		tio->c_cflag |= CS7;
		break;
	case UART_LCR_DLS_8BITS:
		/* fall-through */
	default:
		/* defaultly set as CS8 */
		tio->c_cflag |= CS8;
	}
	/* read lcr register, get parity setting information */
	if ((lcr & UART_LCR_PEN) != 0U) {
		tio->c_cflag |= PARENB;
		/* even parity was set in register */
		if ((lcr & UART_LCR_EPS) != 0U) {
			tio->c_cflag &= (~PARODD);
		}
	}
	/* get stop bits information */
	if ((lcr & UART_LCR_STOP) != 0U) {
		tio->c_cflag |= CSTOPB;
	}
	(void)tty_mutex_unlock(&serdev->uart_mutex);
	return 0;
}

int dw8250_serial_get_icount(void *backend,
			     struct serial_icounter_struct *icount)
{
	struct serial_device *serdev = (struct serial_device *)backend;
	int ret;

	if (icount == NULL || backend == NULL) {
		return -EIO;
	}
	/* Read data from serdev->icount, we do not lock here */
	ret = memcpy_s(icount, sizeof(struct serial_icounter_struct),
		       &serdev->icount, sizeof(struct serial_icounter_struct));
	if (ret != EOK) {
		return -EIO;
	}
	return 0;
}

/*
 * Follow the linux implementation. When need to output a break signal, set the
 * BC bit of the register UART_LCR to 1. Writing 0 will stop the output of the
 * break signal.
 */
int dw8250_serial_break_ctl(void *backend, int state)
{
	u32 lcr;
	void *mem_base = NULL;
	struct serial_device *serdev = (struct serial_device *)backend;

	if (backend == NULL) {
		return -EIO;
	}
	mem_base = (void *)serdev->mem_base;

	(void)tty_mutex_lock(&serdev->uart_mutex);
	lcr = tty_io_read32(ptr_add(mem_base, UART_LCR));
	if (state == TTY_BREAK_CTL_ENABLE) {
		lcr |= UART_LCR_BC;
	} else {
		lcr &= ~UART_LCR_BC;
	}
	tty_io_write32(lcr, ptr_add(mem_base, UART_LCR));
	(void)tty_mutex_unlock(&serdev->uart_mutex);

	return 0;
}
