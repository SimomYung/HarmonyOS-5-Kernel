/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: liblinux PAL implement for chrdev number operations
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 11 10:45:12 2019
 */
#include <liblinux/pal.h>

#include <hmkernel/errno.h>
#include <hongmeng/errno.h>
#include <libsysif/devmgr/api.h>
#include <libdevhost/devhost.h>
#include <libhwsecurec/securec.h>

int
liblinux_pal_register_chrdev_region(unsigned int major,
				    unsigned int baseminor, int minorct,
				    const char *name,
				    unsigned int *major_out)
{
	int ret;

	ret = libdh_register_chrdev_region(major, baseminor, minorct,
					    name, major_out);
	return -hmerrno2posix(ret);
}

void
liblinux_pal_unregister_chrdev_region(unsigned int major, unsigned int baseminor, int minorct)
{
	libdh_unregister_chrdev_region(major, baseminor, minorct);
}
