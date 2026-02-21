/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: flush cache API
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jun 12 09:40:43 2023
 */
#include <libhmsrv_sys/hm_flush_cache.h>
#include <libsysif/sysmgr/api.h>
#include <hongmeng/syscall.h>
#include <hongmeng/ext_fast_syscall.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libmem/cache.h>
#include "hm_flush_cache_internal.h"

#ifdef CONFIG_FLUSH_CACHE_ALL_WATERMARK_ENABLED
#define FLUSH_ALL_ELEVATION	5
static bool init_flush_cache_all = false;
#else
#define FLUSH_CACHE_ALL_WATERMARK 0x2000000
#endif

static inline int __flush_cache_range(unsigned long vstart, unsigned long vend)
{
	hm_vspace_flush_cache(0, vstart, vend);
	return 0;
}

static inline int __flush_dcache_range(unsigned long vstart, unsigned long vend)
{
	hm_vspace_flush_dcache(0, vstart, vend);
	return 0;
}

static inline int __flush_icache_range(unsigned long vstart, unsigned long vend)
{
	hm_vspace_flush_icache(0, vstart, vend);
	return 0;
}

static inline int __clean_dcache_range(unsigned long vstart, unsigned long vend)
{
	hm_vspace_clean_dcache(0, vstart, vend);
	return 0;
}

static inline int __inval_dcache_range(unsigned long vstart, unsigned long vend)
{
	hm_vspace_inval_dcache(0, vstart, vend);
	return 0;
}

typedef int (*flush_cache_range_func)(unsigned long vstart, unsigned long vend);
static flush_cache_range_func g_flush_cache_range[] = {
	__flush_cache_range,
	__flush_dcache_range,
	__flush_icache_range,
	__clean_dcache_range,
	__inval_dcache_range
};

/* depend on `CONFIG_EXT_FLUSH_CACHE_ALL` */
#define EXT_FASTCALL_PER_CNODE_FLUSH_CACHE_ALL_IDX 0
__EXT_FASTCALL_PER_CNODE(1, ext_flush_cache_all, EXT_FASTCALL_PER_CNODE_FLUSH_CACHE_ALL_IDX, unsigned int, cmd)

static int __init_flush_cache_all(void)
{
	int ret;
	ret = syscap_CNodeExtFastcallBind(0, __ext_fastcall_scno_of_ext_flush_cache_all());
	if ((ret < 0) && (ret != E_HM_OBJEXIST)) {
		hm_error("bind ext_flush_cache_all failed, err=%s\n", hmstrerror(ret));
		return ret;
	}
#ifdef CONFIG_FLUSH_CACHE_ALL_WATERMARK_ENABLED
	init_flush_cache_all = true;
#endif

	return 0;
}

static void __init_flush_cache_direct(void)
{
#ifdef CONFIG_AARCH64
	g_flush_cache_range[__DCACHE_FLUSH_RANGE] = flush_dcache_range_el0;
	g_flush_cache_range[__DCACHE_CLEAN_RANGE] = clean_dcache_range_el0;
#endif
}

int hm_flush_cache_init(unsigned int flags)
{
	int ret;
	if ((flags & HM_FLUSH_CACHE_ALL) == HM_FLUSH_CACHE_ALL) {
		ret = __init_flush_cache_all();
		if (ret < 0) {
			return ret;
		}
	}

	if ((flags & HM_FLUSH_CACHE_DIRECT) == HM_FLUSH_CACHE_DIRECT) {
		__init_flush_cache_direct();
	}

	return 0;
}

static void __flush_cache_all(unsigned int cmd)
{
	int ret;
	if ((cmd > __DCACHE_CLEAN_ALL) || (cmd < __CACHE_FLUSH_ALL)) {
		hm_error("flush cache all have bad command, cmd=%u\n", cmd);
		return;
	}

	ret = sysfast_ext_flush_cache_all(cmd);
	if (ret < 0) {
		hm_error("do flush cache all failed [%u], err = (%s)\n", cmd, hmstrerror(ret));
	}
}

static void hm_flush_cache_range(unsigned int cmd, unsigned long start, unsigned long end)
{
	int ret;
	if (cmd > __DCACHE_INVAL_RANGE) {
		hm_error("flush cache range have bad command, cmd=%u\n", cmd);
		return;
	}

	ret = g_flush_cache_range[cmd](start, end);
	if (ret < 0) {
		hm_error("flush cache range cmd[%u][0x%lx, 0x%lx] failed, ret=%d\n",
				cmd, start, end, ret);
	}
}

void hm_flush_cache(unsigned int cmd, unsigned long start, unsigned long end)
{
	if ((start == 0) && (end == (unsigned long)-1)) {
		__flush_cache_all(cmd);
		return;
	}
#ifdef CONFIG_FLUSH_CACHE_ALL_WATERMARK_ENABLED
	int ret = E_HM_OK;

	if ((start < end) && ((end - start) > CONFIG_FLUSH_CACHE_ALL_WATERMARK) &&
	    (cmd < __DCACHE_INVAL_RANGE)) {
		if (!init_flush_cache_all) {
			ret = __init_flush_cache_all();
		}
		if (ret == E_HM_OK) {
			__flush_cache_all(cmd + FLUSH_ALL_ELEVATION);
		} else {
			hm_flush_cache_range(cmd, start, end);
		}
	} else {
		hm_flush_cache_range(cmd, start, end);
	}
#else
	if ((start < end) && ((end - start) > FLUSH_CACHE_ALL_WATERMARK) &&
	    (cmd < __DCACHE_INVAL_RANGE)) {
		hm_info("flush extensive range start = 0x%lx, end = 0x%lx, range = %lu, cmd = %u\n",
			start, end, end - start, cmd);
	}
	hm_flush_cache_range(cmd, start, end);
#endif
}

static void __flush_cache_by_pa(unsigned int cmd, unsigned long long start, unsigned long long end)
{
	unsigned long long cur = start;
	int ret;
	while (cur < end) {
		ret = sysfast_flush_cache_by_pa(cmd, cur, end);
		if (ret <= 0) {
			hm_error("flush cache by pa [0x%llx, 0x%llx) failed, ret=%s\n",
				start, end, hmstrerror(ret));
			return;
		}
		cur += (unsigned long long)(unsigned int)ret;
	}
}

void hm_flush_cache_by_pa(unsigned int cmd, unsigned long long start, unsigned long long end)
{
	if ((start == 0) && (end == (unsigned long long)-1)) {
		__flush_cache_all(cmd);
		return;
	}
	__flush_cache_by_pa(cmd, start, end);
}

void hm_flush_caller_pgtbl_dcache(int pid, unsigned long vstart, unsigned long vend)
{
	int ret;

	if (pid == 0) {
		ret = sysfast_flush_caller_pgtbl_dcache(NULL, vstart, vend);
	} else {
		ret = actvcall_hmcall_mem_flush_user_dcache(pid, vstart, vend);
	}
	if (ret <= 0) {
		hm_debug("flush pgtbl dcache failed [0x%lx, 0x%lx) failed, ret=%s\n",
			 vstart, vend, hmstrerror(ret));
	}
}
