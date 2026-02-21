/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Add linux/nsfs.h to compat linux service
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 17 19:07:21 2021
 */

#ifndef LINUX_NSFS_H
#define LINUX_NSFS_H

#include <linux/ioctl.h>

#define NSIO	0xb7

#define NS_GET_USERNS		_IO(NSIO, 0x1)
#define NS_GET_PARENT		_IO(NSIO, 0x2)

#endif
