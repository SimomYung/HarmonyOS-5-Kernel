/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Handle kvic in kconsole backend
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 05 07:27:36 2020
 */

#include "kconsole_kvic.h"
#include "kconsole.h"
#include "../tty_api.h"
#include "../tty_wrap.h"
#include "../tty_line_buf.h"
#include "../tty_keyboard.h"

#define KVIC_INPUT_TYPE 0x0 /* KVIC_IRQ_TYPE_INPUT is 0 */
#define KVIC_INPUT_MAX 1024 /* same as KVIC_UART_DATA_MAX in devhost_kvic */
#define TTY_DEFAULT_KVIC_IRQ_NO 0
#define KCONSOLE_DEV_NAME "tty_kconsole_dev"

/*
 * Note: Currently devhost_kvic will use part of kconsole_kvic_input struct to
 * construct its kvic_input struct and it should be modified later
 */
/*
 * The buffer for kernel kconsole input.
 * input_len: the input len got from kernel kconsole
 * read_idx: currently input idx we are handling
 */
struct kconsole_kvic_input {
	char input_buffer[KVIC_INPUT_MAX];
	unsigned int input_len;
	unsigned int read_idx;
} g_kvic_input, g_cache_input;

/* cache buf is a cycle buf, if buf is full, trailing chars are discarded */
static void cache_input_to_buf(struct kconsole_kvic_input *cache,
			       const char *input, unsigned int len)
{
	unsigned int i;
	unsigned int tail;

	tail = (cache->read_idx + cache->input_len) % KVIC_INPUT_MAX;
	for (i = 0; i < len; i++) {
		if (cache->input_len >= KVIC_INPUT_MAX) {
			break;
		}
		cache->input_buffer[tail] = input[i];
		cache->input_len++;
		tail = (tail + 1U) % KVIC_INPUT_MAX;
	}
}

static inline bool cache_buf_is_empty(const struct kconsole_kvic_input *cache)
{
	return cache->input_len == 0U;
}

static char read_from_cache_buf(struct kconsole_kvic_input *cache)
{
	char ch;

	if (cache->read_idx >= KVIC_INPUT_MAX || cache->input_len == 0U) {
		tty_warn("tty: read index is out of range, cache read_idx=%u\n", cache->read_idx);
		return UART_ASCII_NUL;
	}
	ch = cache->input_buffer[cache->read_idx];
	cache->read_idx = (cache->read_idx + 1U) % KVIC_INPUT_MAX;
	cache->input_len--;

	return ch;
}

#define MAX_IRQ_INPUT_LEN 1024

static int kconsole_irq_read(struct kconsole_device *kconsole,
			     struct tty_struct *tty)
{
	char echo_inputs[MAX_IRQ_INPUT_LEN];
	int echo_len = 0;
	int input_len = 0;
	int err;
	unsigned int ret;
	struct kconsole_kvic_input *cache_input = &g_cache_input;

	mem_zero_a(echo_inputs);
	/* keep get char until empty or overflow */
	while ((cache_buf_is_empty(cache_input) != true) &&
	       (echo_len < MAX_IRQ_INPUT_LEN)) {
		char c = read_from_cache_buf(cache_input);
		if ((tty->tio.c_cflag & CREAD) == 0) {
			/* ignore all characters if CREAD is not set */
			continue;
		}
		kconsole->icount.rx++;

		ret = (unsigned int)tty_handle_shortcuts_key(c, tty);
		if (SHORTCUT_NEED_ECHO(ret)) {
			/* Store all the inputs in stack for echo display */
			echo_inputs[echo_len] = c;
			echo_len++;
		}
		if (SHORTCUT_NEED_INPUT(ret)) {
			(void)tty_line_buf_write(kconsole->input_buf, &c, sizeof(c));
			input_len++;
		}
	}

	/* echo display input from kconsole */
	err = tty_echo(tty, echo_inputs, echo_len);
	if (err < 0) {
		tty_warn("tty: tty_echo failed, err: %s\n", strerror(-err));
		return err;
	}

	return input_len;
}

static int kconsole_irq_bottom_half(void *data)
{
	int input_len = 0;
	struct tty_struct *tty = NULL;
	struct kconsole_device *kconsole = NULL;

	kconsole = (struct kconsole_device *)data;
	if ((kconsole == NULL) || (kconsole->tty == NULL)) {
		return -EINVAL;
	}

	tty = kconsole->tty;
	/* Do read data from kvic input */
	input_len = kconsole_irq_read(kconsole, tty);
	if (input_len < 0) {
		tty_warn("tty: read from irq failed, err: %s\n",
			 strerror(-input_len));
		return -EFAULT;
	}

	/*
	 * After reading data, notify the waiting poll and wake up the potential
	 * waiter who is blocking on read the input_buf
	 */
	if (input_len > 0) {
		tty_poll_notify(&tty->wq, POLLIN);
		tty_line_buf_wake_up_reader(kconsole->input_buf);
	}

	return 0;
}

/* We should avoid holding locks in the top half of the interrupt */
static tty_irqreturn_t kconsole_irq_handler(int irq, void *dev_id)
{
	int ret;
	struct kconsole_device *kconsole = (struct kconsole_device *)dev_id;

	UNUSED(irq);
	if (kconsole == NULL) {
		return TTY_IRQ_NONE;
	}
	/*
	 * In the top half of the interrupt, we cache the input to the kvic and
	 * then process it in the bottom half
	 */
	cache_input_to_buf(&g_cache_input, g_kvic_input.input_buffer,
			   g_kvic_input.input_len);

	ret = tty_irq_bottom_half_schedule(&kconsole->bottom);
	if (ret < 0) {
		tty_warn("tty: schedule irq bottom half failed, ret: %s\n", strerror(-ret));
		return TTY_IRQ_NONE;
	}

	return TTY_IRQ_HANDLED;
}

int kconsole_init_kvic(struct kconsole_device *kconsole)
{
	int err;
	unsigned int virq;
	if (kconsole == NULL) {
		return -EINVAL;
	}

	mem_zero_s(g_cache_input);
	err = tty_kvic_prepare_irq(KVIC_INPUT_TYPE, &g_kvic_input,
				   KVIC_INPUT_MAX);
	if (err < 0) {
		tty_err("tty kconsole: kvic prepare irq failed: %s\n", strerror(-err));
		return err;
	}

	virq = tty_kvic_create_mapping(KVIC_INPUT_TYPE);
	if (virq == 0) {
		tty_err("tty kconsole: kvic map irq failed\n");
		return -EFAULT;
	}

	err = tty_irq_bottom_half_init(&kconsole->bottom,
				       kconsole_irq_bottom_half, kconsole);
	if (err < 0) {
		tty_warn("tty: init irq bottom half failed, ret: %s\n", strerror(-err));
		return err;
	}

	/* The forth argument of tty_request_irq can be passed to irq handler */
	err = tty_request_irq(virq, kconsole_irq_handler, TTY_DEFAULT_KVIC_IRQ_NO,
			      KCONSOLE_DEV_NAME, kconsole);
	if (err != 0) {
		tty_err("tty: fail to request_irq, irq=%u, err: %s\n",
			virq, strerror(-err));
		return err;
	}
	kconsole->irq = virq;

	tty_info("tty: successfully enable kvic irq, irq_no=%u\n", virq);
	return 0;
}

void kconsole_uninit_kvic(const struct kconsole_device *kconsole)
{
	if (kconsole == NULL) {
		return;
	}
	/* Currenlty kvic do not support uninitialize */
	tty_free_irq(kconsole->irq);
}
