/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Uart framework
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 14 14:55:25 2022
 */
#include <udk/uart.h>
#include <udk/errno.h>
#include <libstrict/strict.h>

#ifdef CONFIG_UDK_UART

#include <udk/log.h>
#include <udk/mutex.h>
#include <udk/iolib.h>
#include <udk/securec.h>
#include <udk/tty/tty_wrap.h>
#include <internal/init.h>
#include <internal/tty/tty_api.h>
#include <internal/tty/tty_keyboard.h>
#include <hongmeng/macro.h>

#define UART_TTY_OPTION			TTY_OPTION_DEFAULT
#define MAX_IRQ_INPUT_LEN		1024

struct udk_uart {
	struct udk_uart_desc *desc;
	struct tty_struct *tty;
	struct serial_icounter_struct icount;
	struct dlist_node node;
};

struct udk_uart_manager {
	struct dlist_node uart_list;
	struct udk_mutex mutex;
};

#define UART_ARG_NR_N(t1, a1, t2, a2, t3, a3, t4, a4, t5, a5, n, x, ...) n
#define UART_ARG_NR(...)							\
	UART_ARG_NR_N(__VA_ARGS__, 5, x, 4, x, 3, x, 2, x, 1, x, 0, x)

#define FIRST_ARG(...) __SC_MAP(1, __SC_USE, __VA_ARGS__)
#define READ_N_ARGS(n, ...) __SC_MAP(n, __SC_USE, __VA_ARGS__)
#define READ_N_TYP_ARGS(n, ...) __SC_MAP(n, __SC_DECL, __VA_ARGS__)
#define ALL_ARGS(...) READ_N_ARGS(UART_ARG_NR(__VA_ARGS__), __VA_ARGS__)
#define ALL_TYP_ARGS(...) READ_N_TYP_ARGS(UART_ARG_NR(__VA_ARGS__), __VA_ARGS__)

#define DEFINE_FUNC_UART_TTY_OPS_RETINT(ops_func, ...)				\
static int uart_##ops_func(ALL_TYP_ARGS(__VA_ARGS__))				\
{										\
	struct udk_uart *uart = NULL;						\
	if (FIRST_ARG(__VA_ARGS__) == NULL) {					\
		return -EINVAL;							\
	}									\
	uart = (struct udk_uart *)FIRST_ARG(__VA_ARGS__);			\
	if (uart->desc->uart_ops == NULL ||					\
	    uart->desc->uart_ops->ops_func == NULL) {				\
		return -EINVAL;							\
	}									\
	return uart->desc->uart_ops->ops_func(ALL_ARGS(__VA_ARGS__));		\
}

#define DEFINE_FUNC_UART_TTY_OPS_RETVOID(ops_func, ...)				\
static void uart_##ops_func(ALL_TYP_ARGS(__VA_ARGS__))				\
{										\
	struct udk_uart *uart = NULL;						\
	if (FIRST_ARG(__VA_ARGS__) == NULL) {					\
		return;								\
	}									\
	uart = (struct udk_uart *)FIRST_ARG(__VA_ARGS__);			\
	if (uart->desc->uart_ops == NULL ||					\
	    uart->desc->uart_ops->ops_func == NULL) {				\
		return;								\
	}									\
	uart->desc->uart_ops->ops_func(ALL_ARGS(__VA_ARGS__));			\
}

#define DEFINE_FUNC_UART_ICOUNT(property)					\
void udk_uart_##property##_inc(struct udk_uart *uart)				\
{										\
	if (uart == NULL) {							\
		udk_warn("icount increase failed, invalid uart\n");		\
		return;								\
	}									\
	uart->icount.property++;						\
}

DEFINE_FUNC_UART_ICOUNT(tx);
DEFINE_FUNC_UART_ICOUNT(parity);
DEFINE_FUNC_UART_ICOUNT(frame);

DEFINE_FUNC_UART_TTY_OPS_RETINT(putc, void *, backend, char, ch);
DEFINE_FUNC_UART_TTY_OPS_RETINT(flush, void *, backend);
DEFINE_FUNC_UART_TTY_OPS_RETINT(set_speed, void *, backend, unsigned int, speed);
DEFINE_FUNC_UART_TTY_OPS_RETINT(get_speed, void *, backend, unsigned int *, speed);
DEFINE_FUNC_UART_TTY_OPS_RETVOID(stop_tx, void *, backend);
DEFINE_FUNC_UART_TTY_OPS_RETVOID(start_tx, void *, backend);
DEFINE_FUNC_UART_TTY_OPS_RETVOID(stop_rx, void *, backend);
DEFINE_FUNC_UART_TTY_OPS_RETVOID(start_rx, void *, backend);
DEFINE_FUNC_UART_TTY_OPS_RETINT(set_termios, void *, backend, const struct termios *, tio);
DEFINE_FUNC_UART_TTY_OPS_RETINT(get_termios, void *, backend, struct termios *, tio);
DEFINE_FUNC_UART_TTY_OPS_RETINT(break_ctl, void *, backend, int, state);
static int uart_get_icount(void *backend, struct serial_icounter_struct *icount);

static struct udk_uart_manager g_uart_manager;
static struct tty_backend_operations g_uart_tty_ops __read_mostly = {
	.putc = uart_putc,
	.flush = uart_flush,
	.set_speed = uart_set_speed,
	.get_speed = uart_get_speed,
	.stop_tx = uart_stop_tx,
	.start_tx = uart_start_tx,
	.stop_rx = uart_stop_rx,
	.start_rx = uart_start_rx,
	.set_termios = uart_set_termios,
	.get_termios = uart_get_termios,
	.ioctl = NULL,
	.poll_notify = NULL,
	.get_icount = uart_get_icount,
	.break_ctl = uart_break_ctl,
};

static inline void uart_manager_lock(void)
{
	udk_mutex_lock(&g_uart_manager.mutex);
}

static inline void uart_manager_unlock(void)
{
	udk_mutex_unlock(&g_uart_manager.mutex);
}

static void uart_list_add(struct udk_uart *uart)
{
	dlist_init(&uart->node);
	uart_manager_lock();
	dlist_insert_tail(&g_uart_manager.uart_list, &uart->node);
	uart_manager_unlock();
}

static void uart_list_remove(const struct udk_uart *uart)
{
	struct udk_uart *pos = NULL;
	struct udk_uart *next = NULL;

	uart_manager_lock();
	dlist_for_each_entry_safe(pos, next, &g_uart_manager.uart_list,
				  struct udk_uart, node) {
		if (pos == uart) {
			dlist_delete(&pos->node);
			break;
		}
	}
	uart_manager_unlock();
}

static struct udk_uart *uart_list_lookup(const char *name)
{
	struct udk_uart *pos = NULL;
	int ret;

	uart_manager_lock();
	dlist_for_each_entry(pos, &g_uart_manager.uart_list,
			     struct udk_uart, node) {
		ret = strcmp(name, pos->desc->name);
		if (ret == 0) {
			uart_manager_unlock();
			return pos;
		}
	}
	uart_manager_unlock();

	return NULL;
}

int udk_uart_rx_write(const struct udk_uart *uart, const char *buf, unsigned int size)
{
	if (uart == NULL || buf == NULL || uart->tty == NULL ||
	    uart->tty->input_buf == NULL) {
		return -EINVAL;
	}

	return tty_line_buf_write(uart->tty->input_buf, buf, size);
}

int udk_uart_tx_read(const struct udk_uart *uart, char *buf, unsigned int size)
{
	if (uart == NULL || buf == NULL || uart->tty == NULL ||
	    uart->tty->output_buf == NULL) {
		return -EINVAL;
	}

	return tty_line_buf_read(uart->tty->output_buf, buf, size);
}

int udk_uart_tx_wake_up(const struct udk_uart *uart)
{
	if (uart == NULL || uart->tty == NULL ||
	    uart->tty->output_buf == NULL) {
		return -EINVAL;
	}
	tty_line_buf_wake_up_writer(uart->tty->output_buf);

	return 0;
}

static int uart_irq_read(struct udk_uart *uart)
{
	int err;
	int input_len = 0;
	char c;
	char inputs[MAX_IRQ_INPUT_LEN];
	struct tty_struct *tty = NULL;
	const struct udk_uart_ops *uart_ops = uart->desc->uart_ops;

	if (uart->tty == NULL || uart_ops->rx_empty == NULL ||
	    uart_ops->getc == NULL) {
		return -EINVAL;
	}
	tty = uart->tty;
	mem_zero_a(inputs);
	/* keep get char until empty or overflow */
	while (!uart_ops->rx_empty(uart) && input_len < MAX_IRQ_INPUT_LEN) {
		c = uart_ops->getc(uart);
		if ((tty->tio.c_cflag & CREAD) == 0) {
			/* ignore all characters if CREAD is not set */
			continue;
		}
		uart->icount.rx++;
		err = tty_handle_shortcuts_key(c, tty);
		if (SHORTCUT_NEED_ECHO(err)) {
			/* Store all the inputs in stack for echo display */
			inputs[input_len] = c;
			input_len++;
		}
		if (SHORTCUT_NEED_INPUT(err)) {
			(void)tty_line_buf_write(uart->tty->input_buf, &c, sizeof(c));
		}
	}
	/* echo display input from serial */
	err = tty_echo(tty, inputs, input_len);
	if (err < 0) {
		udk_warn("tty echo failed, err: %s\n", strerror(-err));
		return err;
	}
	if (input_len > 0) {
		tty_poll_notify(&tty->wq, POLLIN);
		tty_line_buf_wake_up_reader(uart->tty->input_buf);
	}

	return 0;
}

static void uart_interrupt_clean(struct udk_uart *uart, int state)
{
	if (uart->desc->uart_ops->interrupt_clean != NULL) {
		uart->desc->uart_ops->interrupt_clean(uart, state);
	}
}

static int uart_default_irq_handler(struct udk_uart *uart)
{
	int err = 0;
	int state;
	const struct udk_uart_ops *uart_ops = uart->desc->uart_ops;

	if (uart_ops->interrupt_state == NULL) {
		udk_error("invalid interrupt state function\n");
		return -EINVAL;
	}
	uart_ops = uart->desc->uart_ops;
	state = uart_ops->interrupt_state(uart);
	switch (state) {
	case UDK_UART_DRV_INTR_RX :
	case UDK_UART_DRV_INTR_RT :
		err = uart_irq_read(uart);
		if (err < 0) {
			udk_error("uart irq read failed, err: %s\n",
				  strerror(-err));
		}
		break;
	case UDK_UART_DRV_INTR_RE :
		if (uart_ops->handle_error == NULL) {
			err = -EINVAL;
			break;
		}
		uart_ops->handle_error(uart);
		break;
	case UDK_UART_DRV_INTR_INVALID :
		break;
	default:
		udk_error("unknown interrupt type %d from uart\n", state);
		err = -ENOENT;
	}
	uart_interrupt_clean(uart, state);

	return err;
}

static int uart_irq_handler(int irq, void *dev_id)
{
	int ret;
	struct udk_uart *uart = NULL;

	UNUSED(irq);
	if (dev_id == NULL) {
		return -EINVAL;
	}
	uart = (struct udk_uart *)dev_id;
	if (uart->desc == NULL || uart->desc->uart_ops == NULL) {
		return -EINVAL;
	}
	if (uart->desc->uart_ops->irq_handler != NULL) {
		ret = uart->desc->uart_ops->irq_handler(uart);
	} else {
		ret = uart_default_irq_handler(uart);
	}
	if (ret < 0) {
		udk_error("uart irq handler error:%s\n", strerror(-ret));
		return ret;
	}

	return 0;
}

static int uart_request_irq(struct udk_uart *uart)
{
	int ret;

	ret = udk_request_irq(uart->desc->irq, uart_irq_handler,
			      0, uart->desc->name, uart);
	if (ret < 0) {
		udk_error("uart fail to request irq, irq=%d, err: %s\n",
			  uart->desc->irq, strerror(-ret));
		return ret;
	}

	return 0;
}

static void uart_free_irq(struct udk_uart *uart)
{
	udk_free_irq(uart->desc->irq);
}

static struct udk_uart *uart_create(struct udk_uart_desc *desc)
{
	struct udk_uart *uart = NULL;
	struct tty_struct *tty = NULL;

	uart = (struct udk_uart *)udk_malloc(sizeof(*uart));
	if (uart == NULL) {
		udk_error("malloc udk uart failed\n");
		return NULL;
	}
	mem_zero_s(*uart);
	uart->desc = desc;
	tty = tty_struct_register((void *)uart, &g_uart_tty_ops,
				  UART_TTY_OPTION, NULL);
	if (tty == NULL) {
		udk_error("tty register failed, ret NULL\n");
		udk_free(uart);
		return NULL;
	}
	uart->tty = tty;
	uart_list_add(uart);

	return uart;
}

static void uart_destory(struct udk_uart *uart)
{
	uart_list_remove(uart);
	tty_struct_unregister(uart->tty);
	uart->desc = NULL;
	udk_free(uart);
}

struct udk_uart *udk_uart_register(struct udk_uart_desc *desc)
{
	int ret;
	struct udk_uart *uart = NULL;

	if (desc == NULL || desc->name == NULL ||
	    desc->name[0] == '\0' || desc->uart_ops == NULL) {
		udk_warn("invalid desc\n");
		return NULL;
	}
	uart = uart_list_lookup(desc->name);
	if (uart != NULL) {
		udk_error("uart drv has been registered\n");
		return NULL;
	}
	uart = uart_create(desc);
	if (uart == NULL) {
		udk_error("uart create failed\n");
		return NULL;
	}
	ret = uart_request_irq(uart);
	if (ret < 0) {
		uart_destory(uart);
		return NULL;
	}

	return uart;
}

void udk_uart_unregister(struct udk_uart *uart)
{
	if (uart == NULL) {
		udk_warn("invalid uart\n");
		return;
	}
	uart_free_irq(uart);
	uart_destory(uart);
}

void *udk_uart_get_description(const struct udk_uart *uart)
{
	if (uart == NULL || uart->desc == NULL) {
		return NULL;
	}

	return uart->desc;
}

static int uart_get_icount(void *backend, struct serial_icounter_struct *icount)
{
	struct udk_uart *uart = NULL;

	if (backend == NULL || icount == NULL) {
		return -EINVAL;
	}
	uart = (struct udk_uart *)backend;
	NOFAIL(memcpy_s(icount, sizeof(struct serial_icounter_struct),
			&(uart->icount), sizeof(struct serial_icounter_struct)));

	return 0;
}

static int udk_uart_init(void)
{
	dlist_init(&g_uart_manager.uart_list);
	udk_mutex_init(&g_uart_manager.mutex);

	return 0;
}

udk_init_call(udk_uart_init);

#else

struct udk_uart *udk_uart_register(struct udk_uart_desc *desc)
{
	UNUSED(desc);

	return NULL;
}

void udk_uart_unregister(struct udk_uart *uart)
{
	UNUSED(uart);
}

void *udk_uart_get_description(const struct udk_uart *uart)
{
	UNUSED(uart);

	return NULL;
}

int udk_uart_rx_write(const struct udk_uart *uart, const char *buf, unsigned int size)
{
	UNUSED(uart, buf, size);

	return -ENOSYS;
}

int udk_uart_tx_read(const struct udk_uart *uart, char *buf, unsigned int size)
{
	UNUSED(uart, buf, size);

	return -ENOSYS;
}

int udk_uart_tx_wake_up(const struct udk_uart *uart)
{
	UNUSED(uart);

	return -ENOSYS;
}

void udk_uart_tx_inc(struct udk_uart *uart)
{
	UNUSED(uart);
}

void udk_uart_parity_inc(struct udk_uart *uart)
{
	UNUSED(uart);
}

void udk_uart_frame_inc(struct udk_uart *uart)
{
	UNUSED(uart);
}

#endif
