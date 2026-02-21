/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Interfaces of udk uio sysfs
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 3 20:18:56 2021
 */
#ifndef __UDK_UIO_SYSFS_H__
#define __UDK_UIO_SYSFS_H__

#include <udk/uio.h>
#include <udk/device.h>

int uio_sysfs_create(const struct udk_device *dev, struct udk_uio_info *info);
int uio_sysfs_delete(const struct udk_device *dev, struct udk_uio_info *info);

#endif /* __UDK_UIO_SYSFS_H__ */
