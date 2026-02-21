/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Functions about char device api for tty
 * Author: Huawei OS Kernel Lab
 * Create: Sun Nov 24 15:31:39 2019
 */

#include "tty_chrdev.h"

#include <udk/char.h>
#include <udk/errno.h>
#include <udk/device.h>
#include <udk/module.h>
#include "tty_wrap.h"

int tty_chrdrv_init(struct tty_chrdrv *drv, unsigned int max_dev_num,
		    const char *dev_name)
{
	devnum_t base_cdev_no;
	int err;

	if (drv == NULL || dev_name == NULL) {
		return -EINVAL;
	}

	/* alloc char regions for devices */
	if (drv->major == 0) {
		/* if not set major, alloc one dynamic major no */
		err = udk_alloc_chrdev_region(&base_cdev_no, drv->minor_start,
					      max_dev_num, dev_name);
		if (err < 0) {
			tty_err("tty: alloc cdev region err: %s\n", strerror(-err));
			return err;
		}
		drv->major = MAJOR(base_cdev_no);
		drv->minor_start = MINOR(base_cdev_no);
	} else {
		/* if already set major, register it */
		base_cdev_no = MKDEV(drv->major, drv->minor_start);
		err = udk_register_chrdev_region(base_cdev_no, max_dev_num,
						 dev_name);
		if (err < 0) {
			tty_err("tty: register cdev region err: %s\n", strerror(-err));
			return err;
		}
	}
	return 0;
}

void tty_chrdrv_exit(const struct tty_chrdrv *drv, unsigned int max_dev_num)
{
	if (drv == NULL) {
		tty_err("tty: try to exit from null generic chrdev\n");
		return;
	}
	udk_unregister_chrdev_region(MKDEV(drv->major, drv->minor_start),
				     max_dev_num);
}

int tty_chrdev_create(struct tty_chrdev *chrdev, struct tty_chrdrv *chrdrv,
		      unsigned int max_dev_num)
{
	struct udk_char_device *udk_chrdev = NULL;

	if (chrdrv == NULL || chrdev == NULL) {
		return -EINVAL;
	}

	UNUSED(max_dev_num);
	udk_chrdev = udk_chrdev_create(chrdev->name, chrdev->dev_no,
				       (struct udk_char_operations *)chrdrv->fops);
	if (udk_chrdev == NULL) {
		tty_err("tty: failed to alloc device with name %s\n", chrdev->name);
		return -EINVAL;
	}

	chrdev->drv = chrdrv;
	chrdev->dev = &udk_chrdev->udkdev;
	return 0;
}

void tty_chrdev_destroy(struct tty_chrdev *chrdev)
{
	if (chrdev == NULL || chrdev->dev == NULL) {
		tty_err("tty: try to destroy NULL chrdev\n");
		return;
	}
	udk_chrdev_destroy(container_of(chrdev->dev, struct udk_char_device, udkdev));
}
