/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: ldk kernfs
 * Author: Huawei OS Kernel Lab
 * Create: Sun Aug 21 11:33:17 2022
 */

#include <liblinux/pal.h>

#include <libdevhost/sysfs.h>

#ifndef BUILD_BUG_ON
#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*((!!(condition)) ? 1 : 0)]))
#endif

static liblinux_require_frange_fn_t ldk_pal_require_list[(int)DEVHOST_FS_MAX] = {
	[DEVHOST_COMMON]	= NULL,
	[DEVHOST_SYSFS]		= devhost_require_sysfs_range,
};

int liblinux_pal_require_frange(int type, int *min, int *max)
{
	BUILD_BUG_ON(LIBLINUX_SYSFS_MAX != LIBDH_KERNFS_TAG_MAX);
	if ((type >= (int)DEVHOST_FS_MAX) || (type < 0)) {
		return -EINVAL;
	}
	if (ldk_pal_require_list[type] == NULL) {
		return -EOPNOTSUPP;
	}

	return ldk_pal_require_list[type](min, max);
}
