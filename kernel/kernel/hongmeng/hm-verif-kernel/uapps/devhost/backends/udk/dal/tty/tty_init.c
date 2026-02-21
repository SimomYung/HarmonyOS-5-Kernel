/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Init tty driver
 * Author: Huawei OS Kernel Lab
 * Create: Sun Mar 10 18:33:04 2019
 */

/* This file contains basic tty init and exit */

#include "tty_io.h"
#include "tty_core.h"
#include "tty_wrap.h"
#include "tty_chrdev.h"
#include "tty_workqueue.h"

#include <udk/char.h>
#include <internal/init.h>

#define TTY_DRV_NAME "udk_tty"

/* g_tty_driver is initializied in module_init and set as NULL in module_exit */
static struct tty_driver *g_tty_driver = NULL;

struct tty_driver *current_tty_driver(void)
{
	return g_tty_driver;
}

static int tty_fops_ioctl(struct udk_char_context *uctx, unsigned int cmd, unsigned long arg)
{
	struct tty_context *ctx = NULL;
	const struct tty_operations *ops = NULL;
	struct tty_driver *tty_drv = NULL;
	int ret;

	if (uctx == NULL || uctx->ctx == NULL) {
		return -EINVAL;
	}
	ctx = (struct tty_context *)uctx->ctx;
	tty_drv = current_tty_driver();
	if (tty_drv == NULL || tty_drv->ops == NULL || tty_drv->ops->ioctl == NULL) {
		return -EIO;
	}

	ops = tty_drv->ops;
	ret = ops->ioctl(ctx, cmd, arg);
	return ret;
}

static int tty_fops_write(struct udk_char_context *uctx, const void *buf, unsigned long size,
			  unsigned long long *offset)
{
	int err;
	int ret;
	struct tty_context *ctx = NULL;
	const struct tty_operations *ops = NULL;
	struct tty_driver *tty_drv = NULL;
	/* Since write is called frequently, we use buf in stack but not heap */
	char write_buf[TTY_MAX_RW_BUF_SIZE];

	UNUSED(offset);
	if (uctx == NULL || uctx->ctx == NULL || size == 0UL || buf == NULL) {
		return -EINVAL;
	}
	ctx = (struct tty_context *)uctx->ctx;
	tty_drv = current_tty_driver();
	if (tty_drv == NULL || tty_drv->ops == NULL || tty_drv->ops->write == NULL) {
		return -EIO;
	}

	if (size >= TTY_MAX_RW_BUF_SIZE) {
		size = TTY_MAX_RW_BUF_SIZE;
	}
	mem_zero_a(write_buf);
	err = copy_from_user(write_buf, buf, size);
	if (err != 0) {
		return -EFAULT;
	}

	ops = tty_drv->ops;
	ret = ops->write(ctx, write_buf, (ssize_t)size);
	if (ret > 0) {
		*offset += ret;
	}
	return ret;
}

static int tty_fops_read(struct udk_char_context *uctx, void *buf, unsigned long size,
			 unsigned long long *offset)
{
	int ret;
	struct tty_context *ctx = NULL;
	const struct tty_operations *ops = NULL;
	struct tty_driver *tty_drv = NULL;
	/* Since r/w is called frequently, we use buf in stack but not heap */
	char read_buf[TTY_MAX_RW_BUF_SIZE];

	if (uctx == NULL || uctx->ctx == NULL) {
		return -EINVAL;
	}
	ctx = (struct tty_context *)uctx->ctx;
	tty_drv = current_tty_driver();
	if (tty_drv == NULL || tty_drv->ops == NULL || tty_drv->ops->read == NULL) {
		return -EIO;
	}

	if (size == 0 || buf == NULL) {
		return -EINVAL;
	}

	ctx->flag = uctx->io_flags;
	if (size >= TTY_MAX_RW_BUF_SIZE) {
		size = TTY_MAX_RW_BUF_SIZE;
	}
	mem_zero_a(read_buf);

	ops = tty_drv->ops;
	ret = ops->read(ctx, read_buf, (ssize_t)size);
	if (ret > 0) {
		/* only need to copy back ret size bytes */
		int err = copy_to_user(buf, read_buf, (unsigned long)(long)ret);
		if (err != 0) {
			ret = -EFAULT;
		} else {
			*offset += ret;
		}
	}
	return ret;
}

static int tty_fops_open(struct udk_char_context *uctx)
{
	struct tty_struct *tty = NULL;
	struct tty_context *tty_ctx = NULL;
	struct udk_device *udk_dev = NULL;
	devnum_t devt;
	int err;

	if (uctx == NULL || uctx->chrdev == NULL) {
		return -EINVAL;
	}
	udk_dev = &uctx->chrdev->udkdev;
	if (udk_dev == NULL) {
		return -EIO;
	}
	devt = udk_dev->devt;

	tty = find_tty_by_devt(devt);
	if (tty == NULL) {
		return -EINVAL;
	}
	if (tty->drv == NULL || tty->drv->ops == NULL || tty->drv->ops->open == NULL) {
		return -EIO;
	}

	/* alloc and init tty_context */
	tty_ctx = (struct tty_context *)tty_mem_zalloc(sizeof(struct tty_context));
	if (tty_ctx == NULL) {
		return -ENOMEM;
	}

	tty_ctx->tty = tty;
	tty_ctx->flag = uctx->io_flags;
	uctx->ctx = (void *)tty_ctx;

	err = tty->drv->ops->open(tty_ctx);
	if (err < 0) {
		tty_mem_free(tty_ctx);
	}
	return err;
}

static int tty_fops_release(struct udk_char_context *uctx)
{
	struct tty_context *tty_ctx = NULL;
	struct tty_struct *tty = NULL;

	if (uctx == NULL || uctx->ctx == NULL) {
		return -EINVAL;
	}
	tty_ctx = (struct tty_context *)uctx->ctx;
	if (tty_ctx->tty == NULL) {
		return -EINVAL;
	}
	tty = tty_ctx->tty;

	if (tty->drv == NULL || tty->drv->ops == NULL || tty->drv->ops->close == NULL) {
		return -EIO;
	}
	tty->drv->ops->close(tty_ctx);

	tty_ctx->tty = NULL;
	tty_mem_free(tty_ctx);

	return 0;
}

static int tty_fops_poll(struct udk_char_context *uctx, struct udk_poll_wevent *pevent, unsigned int *revent)
{
	struct tty_context *ctx = NULL;
	const struct tty_operations *ops = NULL;
	struct tty_driver *tty_drv = NULL;
	struct tty_struct *tty = NULL;
	int ret;

	if (uctx == NULL || uctx->ctx == NULL) {
		return -EINVAL;
	}
	ctx = (struct tty_context *)uctx->ctx;
	tty = ctx->tty;
	tty_drv = current_tty_driver();
	if (tty == NULL ||tty_drv == NULL || tty_drv->ops == NULL ||
	    tty_drv->ops->poll == NULL || revent == NULL) {
		return -EIO;
	}

	ops = tty_drv->ops;
	ret = udk_poll_wqueue_add(&tty->wq, pevent);
	if (ret < 0) {
		return ret;
	}
	/* to fit liblinux, tty_ldisc_poll return event_out */
	*revent = ops->poll(ctx);
	return 0;
}

static const struct udk_char_operations g_tty_fops = {
	.open = tty_fops_open,
	.close = tty_fops_release,
	.read = tty_fops_read,
	.write = tty_fops_write,
	.llseek = NULL,
	.poll = tty_fops_poll,
	.ioctl = tty_fops_ioctl,
	.compat_ioctl = tty_fops_ioctl,
	.mmap = NULL,
	.__rsv = {0},
	.owner = NULL,
};

static int tty_setup_chrdrv(struct tty_chrdrv *chrdrv)
{
	int ret;

	chrdrv->major = TTY_DEFAULT_MAJOR;
	chrdrv->minor_start = TTY_DEFAULT_BASE_MINOR;
	chrdrv->dev_base_name = TTY_DEV_NAME;
	chrdrv->fops = &g_tty_fops;

	ret = tty_chrdrv_init(chrdrv, TTY_MAX_DEV_NUM, TTY_DEV_NAME);
	if (ret < 0) {
		tty_err("tty: tty_chrdrv_init failed, err: %s\n", strerror(-ret));
		return ret;
	}
	return 0;
}

static int tty_module_init(void)
{
	int err;
	struct tty_driver *tty_drv = NULL;

	tty_info("tty: init tty common driver\n");
	tty_drv = tty_mem_zalloc(sizeof(struct tty_driver));
	if (tty_drv == NULL) {
		tty_err("tty: malloc for tty_driver failed\n");
		return -ENOMEM;
	}

	/* init tty workqueue */
	err = tty_workq_init();
	if (err < 0) {
		tty_err("tty: init workqueue failed\n");
		tty_mem_free(tty_drv);
		return err;
	}

	/* init for tty_driver */
	tty_drv->num = 0;
	tty_drv->max_num = TTY_MAX_DEV_NUM;
	tty_drv->name = TTY_DRV_NAME;
	tty_mutex_init(&tty_drv->mutex);
	tty_list_init(&tty_drv->tty_list);
	tty_drv->ops = current_tty_operations();

	err = tty_setup_chrdrv(&(tty_drv->chrdrv));
	if (err < 0) {
		tty_err("tty: setup_chrdrv failed, err: %s\n", strerror(-err));
		tty_workq_exit();
		tty_mem_free(tty_drv);
		return err;
	}

	g_tty_driver = tty_drv;
	return 0;
}

/*
 * register tty char device will fail since actv poll is not ready,
 * postpone tty initialization.
 */
udk_post_init_call(tty_module_init);
