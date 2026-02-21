/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Functions about char device api for tty
 * Author: Huawei OS Kernel Lab
 * Create: Sun Nov 24 15:31:39 2019
 */

#include "tty_chrdev.h"
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/cred.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/stat.h>
#include <linux/printk.h>
#include <linux/uidgid.h>
#include "tty_wrap.h"
#include "stream/stream.h"

static char *tty_type_devnode(struct device *dev, umode_t *mode, kuid_t *uid, kgid_t *gid)
{
	if (!mode) {
		return NULL;
	}
	/* ptmx's mode should be 0666, see drivers/tty/tty_io.c */
	if ((dev->devt == MKDEV(DEFAULT_PTMX_MAJOR, DEFAULT_PTMX_MINOR))) {
		*mode = S_IRUGO | S_IWUGO;
		return NULL;
	}
	/*
	 * Since only pts use TTY_DEFAULT_MAJOR in IOT scenarios, use tty's major
	 * to separate pts from ptmx devices
	 */
	if (MAJOR(dev->devt) == TTY_DEFAULT_MAJOR) {
		*uid = current_fsuid();
		*gid = current_fsgid();
		return NULL;
	}
	return NULL;
}

static void tty_type_release(struct device *dev)
{
	tty_mem_free(dev);
}

static struct device_type g_tty_device_type = {
	.name = "tty_device",
	.devnode = tty_type_devnode,
	.release = tty_type_release,
};

int tty_chrdrv_init(struct tty_chrdrv *drv, unsigned int max_dev_num,
		    const char *dev_name)
{
	dev_t base_cdev_no;
	struct class *chrdrv_class;
	int err;

	if (drv == NULL || dev_name == NULL) {
		return -EINVAL;
	}
	/* init class in liblinux */
	chrdrv_class = class_create(THIS_MODULE, drv->class_name);
	if (IS_ERR(chrdrv_class)) {
		err = PTR_ERR(chrdrv_class);
		tty_err("tty: class_create failed: %s\n", strerror(-err));
		return err;
	}

	/* alloc char regions for devices */
	if (drv->major == 0) {
		/* if not set major, alloc one dynamic major no */
		err = alloc_chrdev_region(&base_cdev_no, drv->minor_start,
					  max_dev_num, dev_name);
		if (err < 0) {
			tty_err("tty: alloc cdev region err: %s\n", strerror(-err));
			goto error;
		}
		drv->major = MAJOR(base_cdev_no);
		drv->minor_start = MINOR(base_cdev_no);
	} else {
		/* if already set major, register it */
		base_cdev_no = MKDEV(drv->major, drv->minor_start);
		err = register_chrdev_region(base_cdev_no, max_dev_num, dev_name);
		if (err < 0) {
			tty_err("tty: register cdev region err: %s\n", strerror(-err));
			goto error;
		}
	}
	drv->tty_class = chrdrv_class;
	return 0;
error:
	class_destroy(chrdrv_class);
	return err;
}
tty_export_symbol(tty_chrdrv_init);

void tty_chrdrv_exit(const struct tty_chrdrv *drv, unsigned int max_dev_num)
{
	if (drv == NULL) {
		tty_err("tty: err: try to exit from null generic chrdev\n");
		return;
	}
	class_destroy(drv->tty_class);
	unregister_chrdev_region(MKDEV(drv->major, drv->minor_start), max_dev_num);
}
tty_export_symbol(tty_chrdrv_exit);

int tty_chrdev_create(struct tty_chrdev *chrdev, struct tty_chrdrv *chrdrv,
		      unsigned int max_dev_num)
{
	int err;
	struct device *dev = NULL;

	if (chrdrv == NULL || chrdev == NULL) {
		return -EINVAL;
	}

	if (chrdrv->tty_class == NULL) {
		tty_err("tty: cannot create device with a null tty_class\n");
		return -EINVAL;
	}

	chrdev->cdev = cdev_alloc();
	if (chrdev->cdev == NULL) {
		return -ENOMEM;
	}
	chrdev->cdev->ops = chrdrv->fops;
	err = cdev_add(chrdev->cdev, chrdev->dev_no, max_dev_num);
	if (err != 0) {
		tty_err("tty: add cdev failed: %s\n", strerror(-err));
		tty_mem_free(chrdev->cdev);
		chrdev->cdev = NULL;
		return err;
	}

	dev = (struct device *)tty_mem_zalloc(sizeof(struct device));
	if (dev == NULL) {
		tty_err("tty: malloc struct device failed\n");
		cdev_del(chrdev->cdev);
		chrdev->cdev = NULL;
		return -ENOMEM;
	}
	/* initialize device and register it */
	dev->devt = chrdev->dev_no;
	dev->class = chrdrv->tty_class;
	dev->type = &g_tty_device_type;
	dev_set_name(dev, "%s", chrdev->name);
	/* supress uevent to avoid udev change pts's uid and gid */
	dev_set_uevent_suppress(dev, 1);
	err = device_register(dev);
	if (err < 0) {
		tty_err("tty: register device failed, err: %s\n", strerror(-err));
		tty_mem_free(dev);
		cdev_del(chrdev->cdev);
		chrdev->cdev = NULL;
		return err;
	}
	chrdev->dev = dev;
	chrdev->drv = chrdrv;

	return 0;
}
tty_export_symbol(tty_chrdev_create);

void tty_chrdev_destroy(struct tty_chrdev *chrdev)
{
	if (chrdev == NULL || chrdev->drv == NULL) {
		tty_err("tty: err: destroy a NULL chrdev\n");
		return;
	}

	/*
	 * Since cdev will be freed after device unregister in LDK when config
	 * DEVTMPFS is not set, here we should guarantee cdev_del before device_unregister
	 */
	if (chrdev->cdev == NULL) {
		tty_err("tty: destroy chrdev cdev with NULL\n");
		return;
	} else {
		cdev_del(chrdev->cdev);
		chrdev->cdev = NULL;
	}

	if (chrdev->dev == NULL) {
		tty_err("tty: destroy chrdev dev with NULL\n");
		return;
	} else {
		device_unregister(chrdev->dev);
		chrdev->dev = NULL;
	}
}
tty_export_symbol(tty_chrdev_destroy);
