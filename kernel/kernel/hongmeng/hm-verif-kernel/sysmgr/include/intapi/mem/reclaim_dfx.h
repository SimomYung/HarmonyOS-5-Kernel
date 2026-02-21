/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Memory Reclaim Dfx
 * Author: Huawei OS Kernel Lab
 * Create: Mon Mar 10 09:14:21 2025
 */
#ifndef SYSMGR_MEM_RECLAIM_DFX_H
#define SYSMGR_MEM_RECLAIM_DFX_H

struct reclaim_dump_info_s {
	uint32_t order;
	uint32_t paf;
	bool dump_stack;
};

#define LOWMEM_LOG_VAL 0
#define LOWMEM_LOG_RATELIMIT_VAL 1
#define LOWMEM_LOG_ONLY_MEM_STATUS 2

enum lowmem_wakeup_reason_e {
	LOWMEM_LRU_ANON_AVAIL,
	LOWMEM_PAGE_ALLOC_FAIL,
	LOWMEM_LMKD_PROCFS_WRITE,
	LOWMEM_LMKD_OOM,
	LOWMEM_FS_PROTECT_SHRINK,
#ifdef CONFIG_ALLOC_ACCT
	LOWMEM_PAGE_ALLOC_TIMEOUT,
#endif
#ifdef CONFIG_RECLAIM_ACCT
	LOWMEM_RECLAIM_MEM_TIMEOUT,
#endif
	LOWMEM_WAKEUP_REASON_MAX,
};

int mem_dump_call(int val, enum lowmem_wakeup_reason_e reason, struct reclaim_dump_info_s *info);
#endif
