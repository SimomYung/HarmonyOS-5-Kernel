/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Aug  5 10:37:33 UTC 2019
 */
#ifndef __DEVHOST_LINUX_DEVICE_H__
#define __DEVHOST_LINUX_DEVICE_H__

#include <linux/device.h>
#include <linux/fs.h>

#include <devhost/device.h>

struct linux_device_object {
	struct device_info info;
	struct inode *inode;
	struct device *dev;
	unsigned char dead;
};

int linux_device_init(struct device *dev);

static inline
struct linux_device_object* linux_device_object_of(struct device *dev)
{
	struct device_info *info =
		(struct device_info *)dev->archdata.liblinux_private;
	return container_of(info, struct linux_device_object, info);
}

static inline
struct device *linux_device_get_device(struct device_info *info)
{
	struct linux_device_object *obj = NULL;
	obj = container_of(info, struct linux_device_object, info);
	return obj->dev;
}

static inline
struct device *linux_device_get_locked(struct linux_device_object *obj)
{
	device_lock(obj->dev);
	if (obj->dead != 0U) {
		device_unlock(obj->dev);
		return ERR_PTR(-EBUSY);
	}
	return obj->dev;
}

static inline
void linux_device_set_dead(struct linux_device_object *obj)
{
	device_lock(obj->dev);
	obj->dead = 1U;
	device_unlock(obj->dev);
}

#endif /* __DEVHOST_LINUX_DEVICE_H__ */
