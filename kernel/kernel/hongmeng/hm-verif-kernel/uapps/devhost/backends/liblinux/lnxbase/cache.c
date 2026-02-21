/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2023. All rights reserved.
 * Description: Kernel syscall wrappers for cache operations
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 27 09:21:20 2021
 */
#include <liblinux/pal.h>

#include <hmkernel/cache.h>
#include <hongmeng/errno.h>
#include <hongmeng/panic.h>
#include <libsysif/sysmgr/api.h>
#include <libhmsrv_sys/hm_flush_cache.h>

#include <devhost/log.h>
#include "internal.h"

#define LIBLINUX_PAL_CACHE_MAX	(9U)
static const unsigned int g_cmd_2kern[LIBLINUX_PAL_CACHE_MAX] = {
	__CACHE_FLUSH_RANGE,	/* LIBLINUX_PAL_CACHE_FLUSH_RANGE = 0 */
	__DCACHE_FLUSH_RANGE,	/* LIBLINUX_PAL_DCACHE_FLUSH_RANGE = 1 */
	__ICACHE_FLUSH_RANGE,	/* LIBLINUX_PAL_ICACHE_FLUSH_RANGE = 2 */
	__DCACHE_CLEAN_RANGE,	/* LIBLINUX_PAL_DCACHE_CLEAN_RANGE = 3 */
	__DCACHE_INVAL_RANGE,	/* LIBLINUX_PAL_DCACHE_INVAL_RANGE = 4 */
	__CACHE_FLUSH_ALL,	/* LIBLINUX_PAL_CACHE_FLUSH_ALL = 5 */
	__DCACHE_FLUSH_ALL,	/* LIBLINUX_PAL_DCACHE_FLUSH_ALL = 6 */
	__ICACHE_FLUSH_ALL,	/* LIBLINUX_PAL_ICACHE_FLUSH_ALL = 7 */
	__DCACHE_CLEAN_ALL,	/* LIBLINUX_PAL_DCACHE_CLEAN_ALL = 8 */
};

int lnxbase_cache_init(void)
{
	int ret = hm_flush_cache_init(HM_FLUSH_CACHE_ALL | HM_FLUSH_CACHE_DIRECT);
	if (ret < 0) {
		dh_error("flush cache all init err=%s\n", hmstrerror(ret));
		return -hmerrno2posix(ret);
	}
	return 0;
}

static void __flush_user_mem_cache(unsigned int cmd, unsigned long start, unsigned long end)
{
	unsigned long cur;
	int ret;
	cur = start;
	while (cur < end) {
		ret = actvxactcall_hmcall_mem_flush_cache(true, true, cur, end, cmd);
		ret = (ret == 0) ? ((int)(end - cur)) : ret;

		if (ret <= 0) {
			dh_error("flush[%u] user_mem cache range [0x%p, 0x%p) failed, err=%s\n",
				 cmd, ulong_to_ptr(start, void), ulong_to_ptr(end, void), hmstrerror(ret));
			return;
		}
		cur += (unsigned long)(unsigned int)ret;
	}
}

void lnxbase_flush_cache_range(unsigned int cmd, unsigned long start, unsigned long end)
{
	unsigned int flush_cmd, _cmd, is_user;

	is_user = cmd & LIBLINUX_PAL_FLUSH_USER_ADDR;

	flush_cmd = cmd & LIBLINUX_PAL_FLUSH_CMD_MASK;

	if (flush_cmd > LIBLINUX_PAL_DCACHE_CLEAN_ALL) {
		dh_error("lnxbase flush cache range no support cmd[%u]\n",cmd);
		return;
	}
	_cmd = g_cmd_2kern[flush_cmd];

	if (is_user) {
		__flush_user_mem_cache(_cmd, start, end);
	} else {
		hm_flush_cache(_cmd, start, end);
	}
}

void lnxbase_flush_cache_range_by_pa(unsigned int cmd, unsigned long long start, unsigned long long end)
{
	unsigned int flush_cmd, _cmd;
	flush_cmd = cmd & LIBLINUX_PAL_FLUSH_CMD_MASK;
	if (flush_cmd > LIBLINUX_PAL_DCACHE_CLEAN_ALL) {
		dh_error("lnxbase flush cache range by pa, no support cmd[%u]\n",cmd);
		return;
	}
	_cmd = g_cmd_2kern[flush_cmd];

	hm_flush_cache_by_pa(_cmd, start, end);
}

void lnxbase_flush_pgtbl_cache(unsigned int pid, unsigned long vstart, unsigned long vend)
{
	hm_flush_caller_pgtbl_dcache((int)pid, vstart, vend);
}
