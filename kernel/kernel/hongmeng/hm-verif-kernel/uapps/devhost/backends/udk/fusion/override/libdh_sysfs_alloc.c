/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Udk fusion override implement for libdh_sysfs_entry.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 24 11:32:50 2024
 */
#include <libdevhost/sysfs.h>
#include <intapi/mem/mcache.h>

DEFINE_MCACHE_STATIC(libdh_sysfs_entry, 60, libdh_sysfs_entry_size())

void *libdh_sysfs_entry_malloc(void)
{
	return mcache_libdh_sysfs_entry_malloc();
}

void libdh_sysfs_entry_free(void *ptr)
{
	if (likely(ptr != NULL)) {
		mcache_libdh_sysfs_entry_free(ptr);
	}
}
