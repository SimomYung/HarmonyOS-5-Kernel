/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: UDK compat interfaces defination
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 26 15:13:44 2019
 */

#ifndef __UDK_INTERNAL_COMPAT_H__
#define __UDK_INTERNAL_COMPAT_H__

#include <udk/device.h>

struct device;

struct udk_compat_ops {
	int (*unflatten_tree)(void);
	int (*get_device)(struct device_info *info, struct device **pdev_out);
};

void udk_compat_register_ops(const struct udk_compat_ops *ops);

int udk_compat_unflatten_tree(void);
int udk_compat_get_device(struct device_info *info, struct device **pdev_out);

#endif /* __UDK_INTERNAL_COMPAT_H__ */
