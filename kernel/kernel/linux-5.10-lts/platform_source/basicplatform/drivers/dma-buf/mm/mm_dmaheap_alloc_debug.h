/* Copyright (c) Hisilicon Technologies Co., Ltd. 2001-2021. All rights reserved.
 * FileName: mm_dmaheap_alloc_debug.h
 * Description: This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation;
 * either version 2 of the License,
 * or (at your option) any later version.
 */

#ifndef _MM_DMAHEAP_ALLOC_DEBUG_H
#define _MM_DMAHEAP_ALLOC_DEBUG_H

#include <linux/ktime.h>
#include <linux/dma-heap.h>

enum mm_dmaheap_alloc_runtime_stage {
	DMAHEAP_ALLOC_START,
	DMAHEAP_ALLOC_END,
};

enum mm_dmaheap_alloc_print_flag {
	DMAHEAP_ALLOC_SIZE_GT_200MB,
	DMAHEAP_ALLOC_TIME_GT_100MS,
	DMAHEAP_ALLOC_TIME_GT_500MS,
};

struct mm_dmaheap_debug_info {
	s64 _timedelta;
	long msleep_count_s;
	long msleep_count_e;
	unsigned long kernel_stack;
	unsigned long free_cma;
	unsigned long free_page;
	unsigned long nr_inactive_file;
	unsigned long nr_isolated_anon;
	unsigned long nr_isolated_file;
#if defined(CONFIG_LIBLINUX) || defined(CONFIG_SCHED_INFO)
	unsigned long long run_time;
	unsigned long long run_delay;
#endif
	ktime_t _stime, _etime, _htime;
};

extern atomic64_t shrink_msleep_count;

void mm_dmaheap_debug_get_meminfo(struct mm_dmaheap_debug_info *dmaheap_dbg_info);
void mm_dmaheap_debug_get_timeinfo(struct mm_dmaheap_debug_info *dmaheap_dbg_info, unsigned int runtime_stage);
void _mm_dmaheap_debug_print_info(struct mm_dmaheap_debug_info *dmaheap_dbg_info, unsigned int print_flag);
void mm_dmaheap_debug_print_info(struct mm_dmaheap_debug_info *dmaheap_dbg_info, size_t len,
			  struct dma_heap *heap, unsigned long flags);
#ifdef CONFIG_HISYSEVENT
void mm_dmaheap_report_leak_event(unsigned long total_used);
#else
static inline void mm_dmaheap_report_leak_event(unsigned long total_used)
{
}
#endif
#endif
