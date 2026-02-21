/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: liblinux PAL implement for cache operations
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 03 10:45:12 2019
 */
#include <liblinux/pal.h>
#include "../internal.h"

void liblinux_pal_flush_cache_range(unsigned int cmd,
				    unsigned long start, unsigned long end)
{
	lnxbase_flush_cache_range(cmd, start, end);
}

void liblinux_pal_flush_pgtbl_cache(unsigned int pid, unsigned long vstart, unsigned long vend)
{
	lnxbase_flush_pgtbl_cache(pid, vstart, vend);
}
