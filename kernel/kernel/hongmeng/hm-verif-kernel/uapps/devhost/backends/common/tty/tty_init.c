/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Init tty driver
 * Author: Huawei OS Kernel Lab
 * Create: Sun Mar 10 18:33:04 2019
 */

/* This file contains basic tty init and exit */

#include "tty_core.h"
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include "tty_io.h"
#include "tty_api.h"
#include "tty_ioctl.h"
#include "tty_wrap.h"
#include "tty_chrdev.h"
#include "tty_workqueue.h"

#define TTY_DRV_NAME "tty"
#define TTY_CLASS_NAME "tty_class"

/* g_tty_driver is initializied in module_init and set as NULL in module_exit */
static struct tty_driver *g_tty_driver = NULL;

struct tty_driver *current_tty_driver(void)
{
	return g_tty_driver;
}

static struct tty_context *file_to_context(struct file *file)
{
	if (file == NULL || file->private_data == NULL) {
		return NULL;
	}
	return ((struct tty_context *)file->private_data);
}

static long tty_fops_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct tty_context *ctx = NULL;
	const struct tty_operations *ops = NULL;
	struct tty_driver *tty_drv = NULL;
	long ret;

	ctx = file_to_context(file);
	if (ctx == NULL) {
		return -EINVAL;
	}
	tty_drv = current_tty_driver();
	if (tty_drv == NULL || tty_drv->ops == NULL || tty_drv->ops->ioctl == NULL) {
		return -EIO;
	}

	ops = tty_drv->ops;
	ret = (long)ops->ioctl(ctx, cmd, arg);
	return ret;
}

static ssize_t tty_fops_write(struct file *file, const char __user *buf, size_t size,
			      loff_t *offset)
{
	int err;
	ssize_t ret;
	struct tty_context *ctx = NULL;
	const struct tty_operations *ops = NULL;
	struct tty_driver *tty_drv = NULL;
	/* Since r/w is called frequently, we use buf in stack but not heap */
	char write_buf[TTY_MAX_RW_BUF_SIZE];

	if (file == NULL || size == 0 || buf == NULL) {
		return -EINVAL;
	}
	ctx = file_to_context(file);
	if (ctx == NULL) {
		return -EINVAL;
	}
	tty_drv = current_tty_driver();
	if (tty_drv == NULL || tty_drv->ops == NULL || tty_drv->ops->write == NULL) {
		return -EIO;
	}

	mem_zero_a(write_buf);
	if (size >= TTY_MAX_RW_BUF_SIZE) {
		size = TTY_MAX_RW_BUF_SIZE;
	}
	err = (int)copy_from_user(write_buf, buf, size);
	if (err != 0) {
		return -EFAULT;
	}

	ops = tty_drv->ops;
	ret = (ssize_t)ops->write(ctx, write_buf, size);
	return ret;
}

static ssize_t tty_fops_read(struct file *file, char __user *buf, size_t size,
			     loff_t *offset)
{
	ssize_t ret;
	struct tty_context *ctx = NULL;
	const struct tty_operations *ops = NULL;
	struct tty_driver *tty_drv = NULL;
	/* Since r/w is called frequently, we use buf in stack but not heap */
	char read_buf[TTY_MAX_RW_BUF_SIZE];

	if (file == NULL || buf == NULL || size == 0) {
		return -EINVAL;
	}
	ctx = file_to_context(file);
	if (ctx == NULL) {
		return -EINVAL;
	}
	tty_drv = current_tty_driver();
	if (tty_drv == NULL || tty_drv->ops == NULL || tty_drv->ops->read == NULL) {
		return -EIO;
	}

	ctx->flag = file->f_flags;
	if (size >= TTY_MAX_RW_BUF_SIZE) {
		size = TTY_MAX_RW_BUF_SIZE;
	}
	mem_zero_a(read_buf);

	/* call tty_ldisc_read to get input from linebuf */
	ops = tty_drv->ops;
	ret = ops->read(ctx, read_buf, size);
	if (ret > 0) {
		/* only need to copy back ret size bytes */
		int err = (int)copy_to_user(buf, read_buf, ret);
		if (err != 0) {
			ret = -EFAULT;
		}
	}
	return ret;
}

static inline dev_t inode_to_devt(struct inode *inode)
{
	return inode->i_rdev;
}

/*
 * Open /dev/tty device,
 * bind tty_struct with file
 */
static int tty_fops_open(struct inode *inode, struct file *file)
{
	struct tty_struct *tty = NULL;
	struct tty_context *ctx = NULL;
	dev_t devt;
	int err;

	if (inode == NULL || file == NULL) {
		return -EINVAL;
	}
	devt = inode_to_devt(inode);

	tty = find_tty_by_devt(devt);
	if (tty == NULL) {
		return -EIO;
	}
	if (tty->drv == NULL || tty->drv->ops == NULL || tty->drv->ops->open == NULL) {
		return -EIO;
	}

	/* Zalloc contex and bind with file */
	ctx = tty_mem_zalloc(sizeof(struct tty_context));
	if (ctx == NULL) {
		return -ENOMEM;
	}
	ctx->tty = tty;
	ctx->flag = file->f_flags;
	file->private_data = ctx;

	err = tty->drv->ops->open(ctx);
	if (err < 0) {
		tty_mem_free(ctx);
	}
	return err;
}

/*
 * Release /dev/tty device.
 */
static int tty_fops_release(struct inode *inode, struct file *file)
{
	struct tty_context *ctx = NULL;
	struct tty_struct *tty = NULL;

	ctx = file_to_context(file);
	if (ctx == NULL || ctx->tty == NULL) {
		tty_warn("tty: try to free NULL ctx\n");
		return -EINVAL;
	}
	tty = ctx->tty;

	if (tty->drv == NULL || tty->drv->ops == NULL || tty->drv->ops->close == NULL) {
		return -EIO;
	}
	tty->drv->ops->close(ctx);

	/* Unbind with file and free context */
	ctx->tty = NULL;
	ctx->flag = 0;
	file->private_data = NULL;
	tty_mem_free(ctx);
	return 0;
}

/*
 * Poll /dev/tty interface
 */
static unsigned int tty_fops_poll(struct file *file, struct poll_table_struct *wait)
{
	struct tty_context *ctx = NULL;
	const struct tty_operations *ops = NULL;
	struct tty_driver *tty_drv = NULL;
	struct tty_struct *tty = NULL;
	unsigned int ret;

	ctx = file_to_context(file);
	if (ctx == NULL || ctx->tty == NULL || ctx->tty->dev == NULL) {
		return POLLNVAL;
	}
	tty = ctx->tty;
	tty_drv = current_tty_driver();
	if (tty_drv == NULL || tty_drv->ops == NULL || tty_drv->ops->poll == NULL) {
		return POLLERR;
	}

	/* do poll wait for current tty's wait queue */
	poll_wait(file, &tty->wq, wait);
	ops = tty_drv->ops;
	ret = ops->poll(ctx);

	return ret;
}

static const struct file_operations g_tty_fops = {
	.owner			= THIS_MODULE,
	.unlocked_ioctl		= tty_fops_ioctl,
	.compat_ioctl		= tty_fops_ioctl,
	.open			= tty_fops_open,
	.release		= tty_fops_release,
	.write			= tty_fops_write,
	.read			= tty_fops_read,
	.poll			= tty_fops_poll,
};

static int tty_setup_chrdrv(struct tty_chrdrv *chrdrv)
{
	int ret;
	chrdrv->major = TTY_DEFAULT_MAJOR;
	chrdrv->minor_start = TTY_DEFAULT_BASE_MINOR;
	chrdrv->class_name = TTY_CLASS_NAME;
	chrdrv->dev_base_name = TTY_DEV_NAME;
	chrdrv->fops = &g_tty_fops;

	ret = tty_chrdrv_init(chrdrv, TTY_MAX_DEV_NUM, TTY_DEV_NAME);
	if (ret < 0) {
		tty_err("tty: tty_chrdrv_init failed, err: %s\n", strerror(-ret));
		return ret;
	}
	return 0;
}

static int __init tty_module_init(void)
{
	int err;
	struct tty_driver *tty_drv = NULL;
	tty_info("tty: init TTY common driver\n");

	tty_drv = tty_mem_zalloc(sizeof(struct tty_driver));
	if (tty_drv == NULL) {
		tty_err("tty: malloc for tty_driver failed\n");
		return -ENOMEM;
	}

	/* tty workqueue init */
	err = tty_workq_init();
	if (err < 0) {
		tty_err("tty: workqueue init failed\n");
		tty_mem_free(tty_drv);
		return err;
	}

	/* init for tty_driver */
	tty_drv->num = 0;
	tty_drv->max_num = TTY_MAX_DEV_NUM;
	tty_drv->name = TTY_DRV_NAME;
	tty_drv->ops = current_tty_operations();
	tty_mutex_init(&tty_drv->mutex);
	tty_list_init(&tty_drv->tty_list);

	err = tty_setup_chrdrv(&(tty_drv->chrdrv));
	if (err < 0) {
		tty_err("tty: setup_chrdrv failed, err: %s\n", strerror(-err));
		tty_workq_exit();
		tty_mem_free(tty_drv);
		return err;
	}

	g_tty_driver = tty_drv;
	tty_debug("tty: init TTY common driver %s success\n", tty_drv->name);
	return 0;
}

module_init(tty_module_init);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Peitao Shi");
MODULE_DESCRIPTION("simple tty module");
MODULE_VERSION("0.01");
