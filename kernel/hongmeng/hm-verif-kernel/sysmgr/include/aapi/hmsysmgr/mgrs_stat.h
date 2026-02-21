/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Definition for all sub managers information statistics
 * Author: Huawei OS Kernel Lab
 * Create: Fri Mar 22 15:24:17 2019
 */
#ifndef SYSMGR_AAPI_MGRS_STAT_H
#define SYSMGR_AAPI_MGRS_STAT_H

#define SYSMGR_STAT_ALL		"$sysmgr_stat_all$"

#define proc_flag_mask_of(f)	(((f) == 0U) ? (1U) : (1U << (f)))
#define PROCESS_TIME_FLAG	proc_flag_mask_of((unsigned int)PROC_FLAGS_WITH_TIME)
#define THREAD_TIME_FLAG	(proc_flag_mask_of((unsigned int)PROC_FLAGS_WITH_THREAD) | \
				 PROCESS_TIME_FLAG)

enum {
	PROC_FLAGS_WITH_THREAD,
	PROC_FLAGS_WITH_TIME,
	PROC_FLAGS_WITH_MEM,
	PROC_FLAGS_WITH_MEM_LAYOUT,
	PROC_FLAGS_WITH_MEM_VREGION,
	PROC_FLAGS_WITH_MEM_NOLOCK, /* query memmory info without holding the lock */
	PROC_FLAGS_WITH_THREAD_CSW,
	PROC_FLAGS_WITH_THREAD_FLT,
	PROC_FLAGS_WITH_THREAD_ID,
	PROC_FLAGS_WITH_HGUARD, /* thread scanned by hguard */
	PROC_FLAGS_WITH_SYSDUMP, /* thread scanned by sysdump */
	PROC_FLAGS_WITH_ULWT, /* dump ulwt info */
};

enum mem_query_flags {
	MEMQUERY_FLAG_SUMMARY,
	MEMQUERY_FLAG_SLAB,
	MEMQUERY_FLAG_BUDDY,
	MEMQUERY_FLAG_BUDDY_DETAIL,
	MEMQUERY_FLAG_MEM_BANK,
	MEMQUERY_FLAG_VM,
};

#endif
