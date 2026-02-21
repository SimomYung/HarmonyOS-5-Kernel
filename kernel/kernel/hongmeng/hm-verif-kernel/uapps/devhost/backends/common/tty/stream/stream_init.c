/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Stream ptmx init
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */

#include <linux/io.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include "stream.h"
#include "stream_core.h"
#include "../tty_wrap.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Peitao Shi");
MODULE_DESCRIPTION("simple linux module for stream telstream");
MODULE_VERSION("0.01");

#define STREAM_DRV_NAME "stream_tty_drv"
#define STREAM_CLASS_NAME "stream_tty_class"

/* g_ptmx is initialized in module_init and set as NULL in module_exit */
static struct stream_control_device *g_ptmx = NULL;
static struct tty_chrdrv g_stream_chrdrv;

static int ptmx_fops_open(struct inode *inode, struct file *file)
{
	struct stream_device_pair *pair = NULL;
	struct stream_context *ctx = NULL;

	if (inode == NULL || file == NULL) {
		return -EINVAL;
	}
	ctx = (struct stream_context *)tty_mem_zalloc(sizeof(*ctx));
	if (ctx == NULL) {
		return -ENOMEM;
	}

	pair = stream_device_pair_create(g_ptmx);
	if (pair == NULL) {
		tty_mem_free(ctx);
		return -ENODEV;
	}

	ctx->pair = pair;
	ctx->file = file;
	ctx->flag = file->f_flags;
	file->private_data = (void *)ctx;
	pair->ctx = ctx;

	return 0;
}

static int ptmx_fops_release(struct inode *inode, struct file *file)
{
	struct stream_context *ctx = NULL;
	if (inode == NULL || file == NULL) {
		return -EINVAL;
	}
	ctx = (struct stream_context *)file->private_data;
	if (ctx == NULL || ctx->pair == NULL) {
		return -EINVAL;
	}

	stream_device_pair_release(ctx->pair);
	ctx->pair->ctx = NULL;
	ctx->pair = NULL;
	ctx->flag = 0;
	ctx->file = NULL;
	tty_mem_free(ctx);
	file->private_data = NULL;
	return 0;
}

static ssize_t ptmx_fops_read(struct file *file, char __user *buf,
			      size_t size, loff_t *offset)
{
	ssize_t ret;
	/* Since r/w is called frequently, we use buf in stack but not heap */
	char read_buf[TTY_MAX_RW_BUF_SIZE];
	struct stream_context *ctx = NULL;

	if (size == 0) {
		return 0;
	}
	if (file == NULL || buf == NULL) {
		return -EINVAL;
	}
	ctx = (struct stream_context *)file->private_data;

	if (ctx == NULL || ctx->pair == NULL) {
		return -EINVAL;
	}

	ctx->flag = file->f_flags;
	if (size >= TTY_MAX_RW_BUF_SIZE) {
		size = TTY_MAX_RW_BUF_SIZE;
	}
	mem_zero_a(read_buf);

	ret = (ssize_t)stream_ptmx_read(ctx, read_buf, size);
	if (ret > 0) {
		/* only need to copy back ret size bytes */
		int err = (int)copy_to_user(buf, read_buf, ret);
		if (err != 0) {
			ret = -EFAULT;
		}
	}
	return ret;
}

static ssize_t ptmx_fops_write(struct file *file, const char __user *buf,
			       size_t size, loff_t *offset)
{
	int err;
	ssize_t ret;
	/* Since r/w is called frequently, we use buf in stack but not heap */
	char write_buf[TTY_MAX_RW_BUF_SIZE];
	struct stream_context *ctx = NULL;
	if (file == NULL) {
		return -EINVAL;
	}
	ctx = (struct stream_context *)file->private_data;

	if (ctx == NULL || ctx->pair == NULL) {
		return -EINVAL;
	}

	if (size == 0 || buf == NULL) {
		return -EINVAL;
	}
	if (size >= TTY_MAX_RW_BUF_SIZE) {
		size = TTY_MAX_RW_BUF_SIZE;
	}

	mem_zero_a(write_buf);
	err = (int)copy_from_user(write_buf, buf, size);
	if (err != 0) {
		return -EFAULT;
	}

	ret = (ssize_t)stream_ptmx_write(ctx, write_buf, size);
	return ret;
}

/*
 * Poll /dev/pty interface
 */
static unsigned int ptmx_fops_poll(struct file *file, struct poll_table_struct *wait)
{
	struct stream_context *ctx = NULL;

	if (file == NULL) {
		return -EINVAL;
	}
	ctx = (struct stream_context *)file->private_data;

	if (ctx == NULL || ctx->pair == NULL ||
	    ctx->pair->ptmx == NULL || ctx->pair->ptmx->chrdev == NULL) {
		return -EINVAL;
	}

	poll_wait(file, &ctx->pair->wq, wait);

	return stream_ptmx_poll(ctx);
}

static long ptmx_fops_ioctl(struct file *file,
			    unsigned int cmd, unsigned long arg)
{
	struct stream_context *ctx = NULL;
	if (file == NULL) {
		return -EINVAL;
	}
	ctx = (struct stream_context *)file->private_data;

	if (ctx == NULL || ctx->pair == NULL) {
		return -EINVAL;
	}
	return stream_ptmx_ioctl(ctx, cmd, arg);
}

static const struct file_operations g_stream_ptmx_fops = {
	.owner		= THIS_MODULE,
	.open		= ptmx_fops_open,
	.release	= ptmx_fops_release,
	.write		= ptmx_fops_write,
	.read		= ptmx_fops_read,
	.poll		= ptmx_fops_poll,
	.unlocked_ioctl	= ptmx_fops_ioctl,
	.compat_ioctl	= ptmx_fops_ioctl,
};

/*
 * Since TTY will not call stream getc/putc,
 * so we do not implement it
 */
static const struct tty_backend_operations g_bops = {
	.putc = stream_put_char,
	.flush = stream_flush,
	.poll_notify = stream_poll_notify,
	.break_ctl = NULL,
};

static int stream_chrdrv_setup(struct tty_chrdrv *chrdrv)
{
	int ret;

	chrdrv->major = DEFAULT_PTMX_MAJOR;
	chrdrv->minor_start = DEFAULT_PTMX_MINOR;
	chrdrv->class_name = STREAM_CLASS_NAME;
	chrdrv->dev_base_name = STREAM_PTMX_DEV_NAME;
	chrdrv->fops = &g_stream_ptmx_fops;

	ret = tty_chrdrv_init(chrdrv, MAX_DEVICE_COUNT, STREAM_PTMX_DEV_NAME);
	if (ret < 0) {
		tty_warn("tty: stream tty_chrdrv_init failed, err: %s\n", strerror(-ret));
		return ret;
	}
	return 0;
}

static void stream_chrdrv_release(const struct tty_chrdrv *chrdrv)
{
	tty_chrdrv_exit(chrdrv, MAX_DEVICE_COUNT);
}

static int __init stream_init(void)
{
	int ret;
	struct stream_control_device *ptmx = NULL;

	tty_info("tty: start to init module of tty stream\n");
	/* init chrdrv for stream driver */
	ret = stream_chrdrv_setup(&g_stream_chrdrv);
	if (ret < 0) {
		tty_warn("tty: stream_chrdrv_setup failed, err: %s\n", strerror(-ret));
		return ret;
	}

	/* alloc stream_control_device ptmx and init it */
	ptmx = (struct stream_control_device *)
	       tty_mem_zalloc(sizeof(struct stream_control_device));
	if (ptmx == NULL) {
		tty_warn("tty: alloc ptmx failed\n");
		stream_chrdrv_release(&g_stream_chrdrv);
		return -ENOMEM;
	}

	ret = stream_ptmx_init(ptmx, &g_stream_chrdrv, &g_bops);
	if (ret < 0) {
		tty_warn("tty: stream_ptmx_init failed, err: %s\n", strerror(-ret));
		stream_chrdrv_release(&g_stream_chrdrv);
		tty_mem_free(ptmx);
		return ret;
	}
	g_ptmx = ptmx;

	tty_info("tty: finish to init module of tty stream\n");
	return 0;
}

module_init(stream_init);
