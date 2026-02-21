/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef _UKMD_CHRDEV_H_
#define _UKMD_CHRDEV_H_

#include <linux/cdev.h>

/* ukmd char device create interface */
struct ukmd_chrdev {
	const char *name;
	dev_t devno;
	struct class *chr_class;
	struct device *chr_dev;
	struct file_operations *fops;
	struct cdev cdev;
	void *drv_data;
};

int ukmd_create_chrdev(struct ukmd_chrdev *chrdev);
void ukmd_destroy_chrdev(struct ukmd_chrdev *chrdev);
void ukmd_create_attrs(struct device *dev, struct device_attribute *device_attrs, uint32_t len);
void ukmd_cleanup_attrs(struct device *dev, struct device_attribute *device_attrs, uint32_t len);

#endif