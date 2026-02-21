/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: liblinux PAL implement for blkdev number operations
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#include <liblinux/pal.h>

#include <libdevhost/devhost.h>

int
liblinux_pal_register_blkdev_region(unsigned int major,
				    const char *name,
				    unsigned int *major_out)
{
	int ret;

	ret = libdh_register_blkdev_region(major, name, major_out);
	return -hmerrno2posix(ret);
}

void
liblinux_pal_unregister_blkdev_region(unsigned int major, const char *name)
{
	libdh_unregister_blkdev_region(major, name);
}
