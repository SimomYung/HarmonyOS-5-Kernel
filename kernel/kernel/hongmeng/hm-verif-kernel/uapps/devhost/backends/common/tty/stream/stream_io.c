/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Stream ptmx I/O functions
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#include <hmkernel/types.h>

#include "stream_core.h"
#include "../tty_api.h"
#include "../tty_wrap.h"
#include "../tty_line_buf.h"
#include "../tty_keyboard.h"

unsigned int stream_ptmx_poll(struct stream_context *ctx)
{
	unsigned int mask = 0;
	int input_status;
	int output_status;
	struct stream_device_pair *pair = NULL;
	struct tty_struct *pts = NULL;

	if (ctx == NULL || ctx->pair == NULL) {
		return POLLNVAL;
	}
	pair = ctx->pair;
	if (pair->tty == NULL) {
		return POLLNVAL;
	}
	pts = pair->tty;

	/* pty's POLL status is opposite to tty */
	output_status = tty_line_buf_state(pair->master_output);
	input_status = tty_line_buf_state(pair->master_input);
	/* if input buf is not empty, set POLLIN */
	if (input_status > 0 && input_status != ((int)LINE_BUF_STATUS_EMPTY)) {
		mask |= POLLIN | POLLRDNORM;
	}

	/*
	 * Currently stream is overwritten, so always writable, but when buf
	 * is full, will lose part of the input.
	 */
	mask |= POLLOUT | POLLWRNORM;

	/* When tty is not using, set POLLHUP */
	if (!tty_struct_is_using(pts)) {
		mask |= POLLHUP;
	}

	/* if status < 0, set POLLERR */
	if ((input_status < 0) || (output_status < 0)) {
		mask |= POLLERR;
	}

	return mask;
}

int stream_ptmx_ioctl(struct stream_context *ctx,
		      unsigned int cmd, unsigned long arg)
{
	struct stream_device_pair *pair = NULL;
	int ret = 0;

	if (ctx == NULL || ctx->pair == NULL) {
		return -EINVAL;
	}
	pair = ctx->pair;

	switch (cmd) {
	case TIOCGPTN:
		ret = (int)copy_to_user((void __user *)(__uptr_t)(arg),
					&pair->slave_id, sizeof(unsigned int));
		if (ret != 0) {
			tty_warn("tty: copy_to_user failed, err: %s\n", strerror(-ret));
			if (ret > 0) {
				ret = -EIO;
			}
		}
		break;
	default:
		/* some ioctl cmds will pass through to tty */
		ret = tty_stream_master_ioctl(pair->tty, cmd, arg);
		if (ret < 0) {
			tty_warn("tty: Stream ioctl 0x%x return %d\n", cmd, ret);
		}
		break;
	}

	return ret;
}

/*
 * stream_ptmx_read_input is called with helding m_input_mutex lock,
 * so it must return with helding the lock
 */
static int stream_ptmx_read_input(struct stream_device_pair *pair,
				  char *buf, size_t size, unsigned int flag)
{
	int ret = 0;
	struct tty_struct *pts = NULL;

	pts = pair->tty;
	/* read until we get data from output_buf */
	while (ret == 0) {
		int err;
		ret = tty_line_buf_read(pair->master_input, buf, (unsigned int)size);
		/* just ret if we read something */
		if (ret > 0) {
			break;
		}
		if (ret < 0) {
			return ret;
		}

		/* case ret equals to 0 */
		if ((pts->flag & TTY_FLAG_BE_USING) != TTY_FLAG_BE_USING) {
			/* return EIO when tty is not using any longer */
			return -EIO;
		}
		if (!file_block(flag)) {
			/* if non block when read no data, we just break */
			break;
		}
		/* schedule thread when read is blocked */
		(void)tty_mutex_unlock(pair->m_input_mutex);
		err = tty_line_buf_schedule_reader(pair->master_input);
		(void)tty_mutex_lock(pair->m_input_mutex);
		if (err < 0) {
			tty_warn("tty: tty_schedule_output_reader failed, err: %s\n", strerror(-err));
			return ((err == -EINTR) ? -ERESTARTSYS : err);
		}
	}
	return ret;
}

/*
 * Stream_ptmx_read return until get read buf len > 0
 */
int stream_ptmx_read(struct stream_context *ctx, char *buf, size_t size)
{
	int ret;
	unsigned int flag;
	int input_status;
	struct stream_device_pair *pair = NULL;
	struct tty_struct *pts = NULL;

	if (ctx == NULL || ctx->pair == NULL ||
	    ctx->pair->tty == NULL || buf == NULL) {
		return -EINVAL;
	}
	pair = ctx->pair;
	pts = pair->tty;
	flag = ctx->flag;

	(void)tty_mutex_lock(pair->m_input_mutex);
	input_status = tty_line_buf_state(pair->master_input);
	ret = stream_ptmx_read_input(pair, buf, size, flag);
	/*
	 * Only notify POLLOUT when read linebuf from full to half-full,
	 * since when linebuf is not full, we will always return POLLOUT
	 * for poll func and we do not need to notify again.
	 */
	if ((input_status == ((int)LINE_BUF_STATUS_FULL)) && (ret > 0)) {
		tty_poll_notify(&pts->wq, POLLOUT);
	}
	if (ret == 0) {
		(void)tty_mutex_unlock(pair->m_input_mutex);
		/* return EAGAIN if in nonblock mode & read nothing */
		return -EAGAIN;
	}
	if (ret > 0) {
		tty_line_buf_wake_up_writer(pair->master_input);
	}
	(void)tty_mutex_unlock(pair->m_input_mutex);

	return ret;
}

#define MAX_STREAM_INPUT_LEN TTY_MAX_RW_BUF_SIZE

/* try to write to pty */
int stream_ptmx_write(struct stream_context *ctx, const char *buf, size_t size)
{
	size_t i;
	struct stream_device_pair *pair = NULL;
	struct tty_struct *tty = NULL;
	char echo_inputs[MAX_STREAM_INPUT_LEN];
	int echo_len = 0;
	int input_len = 0;

	if (ctx == NULL || ctx->pair == NULL ||
	    ctx->pair->tty == NULL || buf == NULL) {
		return -EINVAL;
	}

	pair = ctx->pair;
	tty = pair->tty;
	size = min(size, (size_t)MAX_STREAM_INPUT_LEN);
	mem_zero_a(echo_inputs);

	for (i = 0; i < size; i++) {
		unsigned int key_handle_ret;
		key_handle_ret = (unsigned int)tty_handle_shortcuts_key(buf[i], tty);
		if (SHORTCUT_NEED_ECHO(key_handle_ret)) {
			/* Store all the inputs in stack for echo display */
			echo_inputs[echo_len] = buf[i];
			echo_len++;
		}
		if (SHORTCUT_NEED_INPUT(key_handle_ret)) {
			(void)tty_line_buf_write_extend(pair->master_output,
							&buf[i], sizeof(char));
			input_len++;
		}
	}

	(void)tty_mutex_lock(pair->m_output_mutex);
	/* echo display input from telnet stream device */
	(void)tty_echo(tty, echo_inputs, echo_len);

	/* here we wake up thread which is reading tty */
	if (input_len > 0) {
		tty_poll_notify(&tty->wq, POLLIN);
		tty_line_buf_wake_up_reader(pair->master_output);
	}
	(void)tty_mutex_unlock(pair->m_output_mutex);

	return (int)size;
}

int stream_put_char(void *backend, char c)
{
	int ret = 0;
	struct tty_struct *tty = NULL;
	struct stream_device_pair *pair = (struct stream_device_pair *)backend;

	if (pair == NULL || pair->tty == NULL) {
		return -EIO;
	}
	tty = pair->tty;

	/*
	 * The echo process should keep the same tty_line_buf strategy
	 * as the input process, so here we use write_override same as
	 * in stream_ptmx_write
	 */
	ret = tty_line_buf_write_override(tty->output_buf, &c, sizeof(char));
	/* try to wake the thread which is reading pty */
	if (ret > 0) {
		tty_line_buf_wake_up_reader(tty->output_buf);
	}
	return ret;
}

/* notify stream ptmx device POLLIN event */
static void __stream_poll_notify(struct stream_device_pair *pair, unsigned int event)
{
	tty_poll_notify(&pair->wq, event);
}

int stream_flush(void *backend)
{
	int i;
	struct tty_struct *tty = NULL;
	struct stream_device_pair *pair = (struct stream_device_pair *)backend;

	if (pair == NULL || pair->tty == NULL) {
		return -EIO;
	}
	tty = pair->tty;

	/*
	 * Because tty shares line buf with stream, there is no need to
	 * actually flush the data here, just notify or wake up the caller
	 * based on whether there is data in the line buf.
	 */
	i = tty_line_buf_space_used(tty->output_buf);
	/*
	 * try to wake the thread which is reading pty
	 * when we flush tty's output buf
	 */
	if (i > 0) {
		__stream_poll_notify(pair, POLLIN);
		tty_line_buf_wake_up_reader(tty->output_buf);
	}

	return i;
}

int stream_poll_notify(void *backend, unsigned int event)
{
	struct stream_device_pair *pair = (struct stream_device_pair *)backend;

	if (pair == NULL) {
		return -EIO;
	}
	__stream_poll_notify(pair, event);

	return 0;
}
