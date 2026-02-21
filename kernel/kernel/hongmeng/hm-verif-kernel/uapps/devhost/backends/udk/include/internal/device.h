/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Internal udk device
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun 01 10:30:30 2020
 */

#ifndef __UDK_INTERNAL_DEVICE_H__
#define __UDK_INTERNAL_DEVICE_H__

#include <udk/device.h>
#include <devhost/device.h>
#include <hongmeng/errno.h>

/*
 * Drivers are not aware of the size of device info,
 * so udk_device_base is also hidden from drivers.
 */
struct udk_device_base {
	struct device_info info;
	struct udk_device  *udkdev;
};

/*
 * Check the sanity of udk_device before operations upon
 * the converted device_info. Not necessary if only udk_device
 * members are accessed.
 * All devices obtained from udk_device_init or udk_device_alloc
 * should pass the sanity check.
 */
static inline int udk_device_sanity_check(const struct udk_device *udkdev)
{
	if (udkdev != NULL && udkdev->base != NULL &&
	    udkdev->base->udkdev == udkdev) {
		return E_HM_OK;
	}
	return E_HM_INVAL;
}

/*
 * Should be used ONLY WHEN udkdev has been checked
 * against udk_device_sanity_check.
 */
static inline struct device_info *
__udk_device_devinfo_of(struct udk_device *udkdev)
{
	return &(udkdev->base->info);
}

#endif /* __UDK_INTERNAL_DEVICE_H__ */
