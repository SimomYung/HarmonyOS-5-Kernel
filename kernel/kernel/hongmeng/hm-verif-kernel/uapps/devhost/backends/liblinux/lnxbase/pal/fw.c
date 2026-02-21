/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: liblinux PAL implement for firmware operations
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 28 19:27:55 2020
 */
#include <liblinux/pal.h>

#include <devhost/backend.h>

int liblinux_pal_fw_acquire_contents(const char *filename, void *data,
				     unsigned int size, unsigned int *rsize)
{
	return devhost_pal_fw_acquire_contents(filename, data, size, rsize);
}
