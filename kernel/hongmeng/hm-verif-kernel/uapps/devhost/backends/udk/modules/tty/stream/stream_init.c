/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Stream ptmx init
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */

#include <libstrict/strict.h>
#include <udk/char.h>
#include <udk/module.h>

#include "stream.h"
#include "stream_core.h"
#include "../tty_wrap.h"

/* g_ptmx is initialized in module_init and set as NULL in module_exit */
static struct stream_control_device *g_ptmx = NULL;
static struct tty_chrdrv g_stream_chrdrv;

static int ptmx_fops_open(struct udk_char_context *uctx)
{
	struct stream_device_pair *pair = NULL;
	struct stream_context *stream_ctx = NULL;

	if (uctx == NULL) {
		return -EINVAL;
	}
	stream_ctx = (struct stream_context *)tty_mem_zalloc(sizeof(*stream_ctx));
	if (stream_ctx == NULL) {
		return -ENOMEM;
	}

	pair = stream_device_pair_create(g_ptmx);
	if (pair == NULL) {
		tty_mem_free(stream_ctx);
		return -ENODEV;
	}

	stream_ctx->pair = pair;
	pair->ctx = stream_ctx;
	stream_ctx->flag = uctx->io_flags;
	/*
	 * In udk, stream_ctx is used to identify
	 * the opened file in stream_ctx->file
	 */
	uctx->ctx = (void *)stream_ctx;
	/*
	 * In udk, ctx is encapsulate to struct file_ctx, we use stream_ctx->file
	 * to identify different file, devhost_epoll_add add key with struct file_ctx,
	 * so stream_ctx->file store struct file_ctx to match devhost.
	 */
	stream_ctx->file = (void *)uctx;
	return 0;
}

static int ptmx_fops_close(struct udk_char_context *uctx)
{
	struct stream_context *stream_ctx = NULL;

	if (uctx == NULL || uctx->ctx == NULL) {
		return -EINVAL;
	}
	stream_ctx = (struct stream_context *)uctx->ctx;
	if (stream_ctx->pair == NULL) {
		return -EINVAL;
	}

	stream_device_pair_release(stream_ctx->pair);
	stream_ctx->pair = NULL;
	stream_ctx->file = NULL;
	tty_mem_free(stream_ctx);
	return 0;
}

static int ptmx_fops_read(struct udk_char_context *uctx, void *buf,
			  unsigned long size, unsigned long long *offset)
{
	int ret;
	/* Since r/w is called frequently, we use buf in stack but not heap */
	char read_buf[TTY_MAX_RW_BUF_SIZE];
	struct stream_context *stream_ctx = NULL;

	if (uctx == NULL || uctx->ctx == NULL || size == 0UL || buf == NULL) {
		return -EINVAL;
	}
	stream_ctx = (struct stream_context *)uctx->ctx;
	if (stream_ctx->pair == NULL) {
		return -EINVAL;
	}

	stream_ctx->flag = uctx->io_flags;
	if (size >= TTY_MAX_RW_BUF_SIZE) {
		size = TTY_MAX_RW_BUF_SIZE;
	}
	mem_zero_a(read_buf);

	ret = stream_ptmx_read(stream_ctx, read_buf, (size_t)(long)size);
	if (ret > 0) {
		/* only need to copy back ret size bytes */
		int err = copy_to_user(buf, read_buf, (unsigned long)(long)ret);
		if (err != 0) {
			ret = -EFAULT;
		} else {
			*offset += (unsigned long long)(long long)ret;
		}
	}
	return ret;
}

static int ptmx_fops_write(struct udk_char_context *uctx, const void *buf,
			   unsigned long size, unsigned long long *offset)
{
	int err;
	int ret;
	/* Since r/w is called frequently, we use buf in stack but not heap */
	char write_buf[TTY_MAX_RW_BUF_SIZE];
	struct stream_context *stream_ctx = NULL;

	if (uctx == NULL || uctx->ctx == NULL || size == 0UL || buf == NULL) {
		return -EINVAL;
	}
	stream_ctx = (struct stream_context *)uctx->ctx;
	if (stream_ctx->pair == NULL) {
		return -EINVAL;
	}

	if (size >= TTY_MAX_RW_BUF_SIZE) {
		size = TTY_MAX_RW_BUF_SIZE;
	}
	mem_zero_a(write_buf);

	err = copy_from_user(write_buf, buf, size);
	if (err != 0) {
		return -EFAULT;
	}

	ret = stream_ptmx_write(stream_ctx, write_buf, (size_t)(long)size);
	if (ret >= 0) {
		*offset += (unsigned long long)(long long)ret;
	}
	return ret;
}

static int ptmx_fops_poll(struct udk_char_context *uctx, struct udk_poll_wevent *pevent, unsigned int *revent)
{
	struct stream_context *stream_ctx = NULL;

	if (uctx == NULL || uctx->ctx == NULL || revent == NULL) {
		return -EINVAL;
	}
	stream_ctx = (struct stream_context *)uctx->ctx;
	if (stream_ctx->pair == NULL) {
		return -EINVAL;
	}

	udk_poll_wqueue_add(&stream_ctx->pair->wq, pevent);
	/* to fit liblinux, tty_ldisc_poll return event_out */
	*revent = (unsigned long)stream_ptmx_poll(stream_ctx);
	return 0;
}

static int ptmx_fops_ioctl(struct udk_char_context *uctx, unsigned int cmd, unsigned long arg)
{
	struct stream_context *stream_ctx = NULL;

	if (uctx == NULL || uctx->ctx == NULL) {
		return -EINVAL;
	}
	stream_ctx = (struct stream_context *)uctx->ctx;
	if (stream_ctx->pair == NULL) {
		return -EINVAL;
	}
	return stream_ptmx_ioctl(stream_ctx, cmd, arg);
}

static const struct udk_char_operations g_stream_ptmx_fops = {
	.open		= ptmx_fops_open,
	.close		= ptmx_fops_close,
	.write		= ptmx_fops_write,
	.read		= ptmx_fops_read,
	.poll		= ptmx_fops_poll,
	.ioctl		= ptmx_fops_ioctl,
	.compat_ioctl	= ptmx_fops_ioctl,
	.mmap		= NULL,
	.llseek		= NULL,
	.owner		= UDK_THIS_MODULE,
};

static const struct tty_backend_operations g_bops = {
	.putc = stream_put_char,
	.flush = stream_flush,
	.set_speed = NULL,
	.get_speed = NULL,
	.stop_tx = NULL,
	.start_tx = NULL,
	.stop_rx = NULL,
	.start_rx = NULL,
	.set_termios = NULL,
	.get_termios = NULL,
	.ioctl = NULL,
	.poll_notify = stream_poll_notify,
	.get_icount = NULL,
	.break_ctl = NULL,
};

static int stream_chrdrv_setup(struct tty_chrdrv *chrdrv)
{
	int ret;

	chrdrv->major = DEFAULT_PTMX_MAJOR;
	chrdrv->minor_start = DEFAULT_PTMX_MINOR;
	chrdrv->dev_base_name = STREAM_PTMX_DEV_NAME;
	chrdrv->fops = &g_stream_ptmx_fops;

	ret = tty_chrdrv_init(chrdrv, MAX_DEVICE_COUNT, STREAM_PTMX_DEV_NAME);
	if (ret < 0) {
		tty_err("tty: stream: tty_chrdrv_init failed, err: %s\n", strerror(-ret));
		return ret;
	}
	return 0;
}

static void stream_chrdrv_release(const struct tty_chrdrv *chrdrv)
{
	tty_chrdrv_exit(chrdrv, MAX_DEVICE_COUNT);
}

static int stream_init(void)
{
	int ret;
	struct stream_control_device *ptmx = NULL;

	tty_info("tty: start to init module of tty stream\n");
	/* init chrdrv for stream driver */
	ret = stream_chrdrv_setup(&g_stream_chrdrv);
	if (ret < 0) {
		tty_err("tty: stream_chrdrv_setup failed, err: %s\n", strerror(-ret));
		return ret;
	}

	/* alloc and init stream_control_device ptmx */
	ptmx = (struct stream_control_device *)
	       tty_mem_zalloc(sizeof(struct stream_control_device));
	if (ptmx == NULL) {
		stream_chrdrv_release(&g_stream_chrdrv);
		return -ENOMEM;
	}

	ret = stream_ptmx_init(ptmx, &g_stream_chrdrv, &g_bops);
	if (ret < 0) {
		tty_err("tty: stream_ptmx_init failed, err: %s\n", strerror(-ret));
		stream_chrdrv_release(&g_stream_chrdrv);
		tty_mem_free(ptmx);
		return ret;
	}
	g_ptmx = ptmx;

	tty_info("tty: finish to init module of tty stream\n");
	return 0;
}

static int stream_exit(void)
{
	tty_warn("tty: Currently stream does not support uninstall\n");
	return -EPERM;
}

udk_module_init(stream_init);
udk_module_exit(stream_exit);
