/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: libdh sysfs entry allocation interfaces.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 24 11:32:50 2024
 */
#include <libdevhost/sysfs.h>

__weak void *libdh_sysfs_entry_malloc(void)
{
	return malloc(libdh_sysfs_entry_size());
}

__weak void libdh_sysfs_entry_free(void *ptr)
{
	if (likely(ptr != NULL)) {
		free(ptr);
	}
}
