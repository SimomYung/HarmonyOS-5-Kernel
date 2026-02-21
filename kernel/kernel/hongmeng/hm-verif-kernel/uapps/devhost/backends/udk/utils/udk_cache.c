/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Cache functions in udk
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 14 19:00:09 2019
 */
#include <udk/cache.h>
#include <udk/log.h>

#include <hmkernel/cache.h>
#include <hongmeng/types.h>
#include <hongmeng/errno.h>
#include <hongmeng/ext_fast_syscall.h>
#include <hongmeng/syscall.h>

#include <libhmsrv_sys/hm_flush_cache.h>
#include <libstrict/strict.h>

static
void __flush_cache_range(unsigned long start, unsigned long end,
			 unsigned int cmd)
{
	unsigned long pos = start;

	while (pos < end) {
		int err = 0;
		err = sysfast_flush_cache(cmd, pos, end);
		if (err <= 0) {
#ifdef CONFIG_DEBUG_BUILD
			udk_error("flush cache error, cmd=%u, err=%s, pos=%lx, end=%lx\n", cmd, hmstrerror(err), pos, end);
#else
			udk_error("flush cache error, cmd=%u, err=%s\n", cmd, hmstrerror(err));
#endif
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
	unsigned long curr = start, range;

	if (end <= start) {
		return;
	}
	while (curr < end) {
		range = fusion_flush_cache_range_by_pa(curr, end, cmd);
		curr += range;
	}
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

/* copied from ulibs hm_flush_cache */
#define EXT_FASTCALL_PER_CNODE_FLUSH_CACHE_ALL_IDX 0
__EXT_FASTCALL_PER_CNODE(1, ext_flush_cache_all, EXT_FASTCALL_PER_CNODE_FLUSH_CACHE_ALL_IDX, unsigned int, cmd)

static bool init_flush_cache_all = false;
void udk_flush_cache_all(void)
{
	int err = 0;

	if (!init_flush_cache_all) {
		err = hm_flush_cache_init(HM_FLUSH_CACHE_ALL);
		if (err != E_HM_OK) {
			udk_error("flush cache init failed, won't flush, err=%s\n", hmstrerror(err));
			return;
		}
		init_flush_cache_all = true;
	}

	err = sysfast_ext_flush_cache_all(__CACHE_FLUSH_ALL);
	if (err < 0) {
		udk_error("do flush cache all failed, err = (%s)\n", hmstrerror(err));
	}
}
