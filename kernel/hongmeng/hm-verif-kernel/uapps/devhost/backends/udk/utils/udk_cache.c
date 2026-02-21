/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Cache functions in udk
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 14 19:00:09 2019
 */
#include <udk/cache.h>

#include <hmkernel/cache.h>
#include <hongmeng/types.h>
#include <hongmeng/errno.h>
#include <hongmeng/syscall.h>
#include <libstrict/strict.h>
#include <udk/log.h>

static
void __flush_cache_range(unsigned long start, unsigned long end,
			 unsigned int cmd)
{
	unsigned long pos = start;

	while (pos < end) {
		int err = 0;
		err = sysfast_flush_cache(cmd, pos, end);
		if (err <= 0) {
			udk_error("flush cache error, cmd=%u\n", cmd);
			return;
		}
		pos += (unsigned long)(unsigned int)err;
	}
}

#ifndef UDK_FUSION
static void __flush_cache_range_by_pa_default(unsigned long start, unsigned long end,
					      unsigned int cmd)
{
	unsigned long pos = start;

	while (pos < end) {
		int err = 0;
		err = sysfast_flush_cache_by_pa(cmd, pos, end);
		if (err <= 0) {
			udk_error("flush cache error, cmd=%u\n", cmd);
			return;
		}
		pos += (unsigned long)(unsigned int)err;
	}
}
#endif

static void __flush_cache_range_by_pa(unsigned long start, unsigned long end,
				      unsigned int cmd)
{
#ifdef UDK_FUSION
	fusion_flush_cache_range_by_pa(start, end, cmd);
#else
	__flush_cache_range_by_pa_default(start, end, cmd);
#endif
}

void udk_flush_dcache_range(unsigned long start, unsigned long end)
{
	__flush_cache_range(start, end, __DCACHE_FLUSH_RANGE);
}

void udk_dma_sync_cpu_to_dev(unsigned long start,
			     unsigned long end)
{
	__flush_cache_range(start, end, __DCACHE_CLEAN_RANGE);
}

void udk_dma_sync_dev_to_cpu(unsigned long start,
			     unsigned long end)
{
	__flush_cache_range(start, end, __DCACHE_INVAL_RANGE);
}

void udk_dma_sync_cpu_to_dev_by_pa(unsigned long start,
			     unsigned long end)
{
	__flush_cache_range_by_pa(start, end, __DCACHE_CLEAN_RANGE);
}

void udk_dma_sync_dev_to_cpu_by_pa(unsigned long start,
			     unsigned long end)
{
	__flush_cache_range_by_pa(start, end, __DCACHE_INVAL_RANGE);
}
