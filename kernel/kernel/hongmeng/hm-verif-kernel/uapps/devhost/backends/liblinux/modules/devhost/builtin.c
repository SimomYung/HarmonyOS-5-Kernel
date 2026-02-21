/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 27 17:46:18 2019
 */
#include <linux/device.h>
#include "linux_device.h"

static int devhost_builtin_bind(void* ctx, struct device_info *info)
{
	struct device *dev = NULL;
	int ret;

	if (info == NULL) {
		return -EINVAL;
	}
	dev = linux_device_get_device(info);
	if (dev == NULL) {
		return -EINVAL;
	}
	ret = device_attach(dev);
	if (ret == 1) {
		return 0;
	} else if (ret == 0) {
		return -ENOENT;
	} else {
		return ret;
	}
}

struct hm_driver_ops devhost_builtin_ops __read_mostly = {
	.bind = devhost_builtin_bind,
	.create = NULL,
};
