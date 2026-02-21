/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Serial driver functions
 * Author: Huawei OS Kernel Lab
 * Create: Sat Mar 02 21:15:22 2019
 */

#include "serial_core.h"
#include "../tty_api.h"
#include "../tty_wrap.h"
#include "../tty_line_buf.h"
#include "../tty_keyboard.h"

#define KVIC_INPUT_TYPE 0x0 /* KVIC_IRQ_TYPE_INPUT is 0 */
#define KVIC_INPUT_MAX 1024 /* same as KVIC_UART_DATA_MAX in devhost_kvic */

static struct serial_kvic_input {
	char input_buffer[KVIC_INPUT_MAX];
	unsigned int input_len;
	unsigned int read_idx;
} g_kvic_input;

/*
 * alloc one tty driver and init it,
 * then serial_device bind with it
 */
int uart_register_tty(struct serial_device *serdev, unsigned int options)
{
	int err = 0;
	if (serdev == NULL) {
		return -EIO;
	}

	tty_info("tty: serial try to register as tty\n");
	serdev->tty = tty_struct_register((void *)serdev, serdev->tty_bops,
					  options, NULL);
	if (serdev->tty == NULL) {
		tty_err("tty: uart_register_tty failed, ret NULL\n");
		return -EIO;
	}
	/* serial's input buf points to tty's input buf */
	serdev->input_buf = serdev->tty->input_buf;
	serdev->output_buf = serdev->tty->output_buf;

	return err;
}

/* free tty and unbind with serial_device */
void uart_unregister_tty(struct serial_device *serdev)
{
	if (serdev == NULL) {
		tty_warn("tty: unregister tty but serdev is NULL\n");
		return;
	}
	tty_struct_unregister(serdev->tty);
	serdev->tty = NULL;
}

struct serial_device *serial_device_create(const char *dev_name)
{
	struct serial_device *serdev = NULL;
	if (dev_name == NULL) {
		return NULL;
	}

	serdev = tty_mem_zalloc(sizeof(struct serial_device));
	if (serdev == NULL) {
		return serdev;
	}

	serdev->dev_name = dev_name;
	tty_mutex_init(&serdev->uart_mutex);
	return serdev;
}

void serial_device_remove(struct serial_device *serdev)
{
	if (serdev == NULL) {
		return;
	}

	tty_mem_free(serdev);
}

#define MAX_IRQ_INPUT_LEN 1024

static int uart_irq_read(struct serial_device *serdev)
{
	const struct uart_ops *uops = NULL;
	struct tty_struct *tty = NULL;
	char echo_inputs[MAX_IRQ_INPUT_LEN];
	int echo_len = 0;
	int input_len = 0;
	int err;

	if (serdev == NULL || serdev->tty == NULL || serdev->uops == NULL) {
		return -EIO;
	}

	uops = serdev->uops;
	tty = serdev->tty;
	mem_zero_a(echo_inputs);
	/* keep get char until empty or overflow */
	while (!uops->rx_empty(serdev) && echo_len < MAX_IRQ_INPUT_LEN) {
		char c = uops->getc(serdev);

		if ((tty->tio.c_cflag & CREAD) == 0) {
			/* ignore all characters if CREAD is not set */
			continue;
		}
		serdev->icount.rx++;

		err = tty_handle_shortcuts_key(c, tty);
		if (SHORTCUT_NEED_ECHO((unsigned int)err) != 0U) {
			/* Store all the inputs in stack for echo display */
			echo_inputs[echo_len] = c;
			echo_len++;
		}
		if (SHORTCUT_NEED_INPUT((unsigned int)err) != 0U) {
			(void)tty_line_buf_write(serdev->input_buf, &c, sizeof(c));
			input_len++;
		}
	}

	/* echo display input from serial */
	err = tty_echo(tty, echo_inputs, echo_len);
	if (err < 0) {
		tty_warn("tty: tty_echo failed, err: %s\n", strerror(-err));
		return err;
	}
	if (input_len > 0) {
		tty_poll_notify(&tty->wq, POLLIN);
		tty_line_buf_wake_up_reader(serdev->input_buf);
	}

	return 0;
}

static tty_irqreturn_t uart_irq_handler(int irq, void *dev_id)
{
	int err;
	const struct uart_ops *uops = NULL;
	enum uart_interrupt_type uart_interrupt_state;
	struct serial_device *serdev = (struct serial_device *)dev_id;

	UNUSED(irq);
	if (serdev == NULL || serdev->uops == NULL) {
		return TTY_IRQ_NONE;
	}

	uops = serdev->uops;
	if (uops->interrupt_state == NULL || uops->handle_error == NULL) {
		return TTY_IRQ_NONE;
	}

	uart_interrupt_state = uops->interrupt_state(serdev);
	switch (uart_interrupt_state) {
	case UART_INTR_RX:
		err = uart_irq_read(serdev);
		if (err < 0) {
			tty_err("tty: uart_irq_read failed, err: %s\n", strerror(-err));
			return TTY_IRQ_NONE;
		}
		break;
	case UART_INTR_RE:
		uops->handle_error(serdev);
		break;
	case UART_INTR_RT:
		/*
		 * rt interrupt happens when received data depth doesn't reach FIFO trigger
		 * level, rt_interrupt will be cleared when receiver buffer is empty again
		 */
		err = uart_irq_read(serdev);
		if (err < 0) {
			tty_err("tty: uart_irq_read failed, err: %s\n", strerror(-err));
			return TTY_IRQ_NONE;
		}
		break;
	case UART_INTR_TX:
		if (uops->tx_async != NULL) {
			(void)uops->tx_async(serdev);
		}
		break;
	case UART_INTR_IGNORE:
		break;
	case UART_INTR_INVALID:
	default:
		tty_err("tty: receive unknown interrupt type %u from uart\n",
			(unsigned int)uart_interrupt_state);
		return TTY_IRQ_NONE;
	}

	return TTY_IRQ_HANDLED;
}

static bool kvic_rx_empty(struct serial_device *serdev)
{
	UNUSED(serdev);
	return g_kvic_input.input_len == (unsigned int)g_kvic_input.read_idx;
}

/*
 * kvic do not read serial driver registers directly, it is for transmitting
 * data. Uart interrupts have been processed in kernel serial, so kvic will
 * only receive rx interrupts
 */
static enum uart_interrupt_type
kvic_interrupt_state(const struct serial_device *serdev)
{
	UNUSED(serdev);

	return UART_INTR_RX;
}

/*
 * kvic do not read serial driver registers directly,
 * user input is acquired from g_kvic_input.
 */
static void kvic_handle_error(struct serial_device *serdev)
{
	/* input parity error has been handled in kernel */
	UNUSED(serdev);
}

static char kvic_serial_getc(struct serial_device *serdev)
{
	char ret;

	UNUSED(serdev);
	ret = g_kvic_input.input_buffer[g_kvic_input.read_idx];
	g_kvic_input.read_idx++;
	if (g_kvic_input.input_len == (unsigned int)g_kvic_input.read_idx) {
		/* all characters are read, reset buffer */
		g_kvic_input.input_len = 0;
		g_kvic_input.read_idx = 0;
	}

	return ret;
}

static int uart_kvic_init(void)
{
	int err;
	unsigned int virq;

	err = tty_kvic_prepare_irq(KVIC_INPUT_TYPE, &g_kvic_input,
				   KVIC_INPUT_MAX);
	if (err < 0) {
		tty_err("tty serial: kvic prepare irq failed: %s\n", strerror(-err));
		return err;
	}

	virq = tty_kvic_create_mapping(KVIC_INPUT_TYPE);
	if (virq == 0) {
		tty_err("tty serial: kvic map irq failed\n");
		return -EFAULT;
	}

	return (int)virq;
}

static const struct uart_ops g_kvic_uops = {
	.rx_empty = kvic_rx_empty,
	.getc = kvic_serial_getc,
	.handle_error = kvic_handle_error,
	.interrupt_state = kvic_interrupt_state,
};

int uart_register_irq(struct serial_device *serdev)
{
	int ret;
	if (serdev == NULL) {
		tty_err("tty: Null serdev\n");
		return -EIO;
	}
	if (serdev->input_mode == UART_INPUT_MODE_NONE) {
		tty_info("tty: Since serial set input mode to none,"
			 "do not request_irq for this serial\n");
		return 0;
	}

	tty_info("tty: try to request_irq, irq=%d, name=%s\n", serdev->irq, serdev->dev_name);
	/* The forth argument of tty_request_irq can be passed to irq handler */
	ret = tty_request_irq((unsigned int)serdev->irq, uart_irq_handler, 0,
			      serdev->dev_name, serdev);
	if (ret != 0) {
		tty_err("tty: fail to request_irq, irq=%d, err: %s\n", serdev->irq, strerror(-ret));
		return ret;
	}

	tty_info("tty: successfully enable irq, irq=%d\n", serdev->irq);
	return 0;
}

void serial_device_setup(struct serial_device *serdev,
			 const struct tty_backend_operations *bops,
			 const struct uart_ops *uops,
			 int irq,
			 unsigned int input_mode)
{
	if (serdev == NULL) {
		return;
	}

	serdev->tty_bops = bops;
	serdev->input_mode = input_mode;

	switch(serdev->input_mode) {
	case UART_INPUT_MODE_NONE:
		serdev->uops = NULL;
		serdev->irq = 0;
		break;
	case UART_INPUT_MODE_KVIC:
		serdev->uops = &g_kvic_uops;
		serdev->irq = uart_kvic_init();
		break;
	case UART_INPUT_MODE_IRQ:
		serdev->uops = uops;
		serdev->irq = irq;
		break;
	default:
		tty_warn("tty: Unsupport input mode: %u\n", serdev->input_mode);
		break;
	}
}
