/* Copyright (c) Hisilicon Technologies Co., Ltd. 2001-2021. All rights reserved.
 * FileName: mm_dmaheap_alloc_debug.c
 * Description: This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation;
 * either version 2 of the License,
 * or (at your option) any later version.
 */

#include <linux/atomic.h>
#include <linux/ktime.h>
#include <linux/mm.h>
#include <linux/vmstat.h>
#include <securec.h>
#ifdef CONFIG_SCHED_INFO
#include <linux/sched/cputime.h>
#endif
#ifdef CONFIG_HISYSEVENT
#include <dfx/hiview_hisysevent.h>
#endif

#include <linux/dmabuf/mm_dma_heap.h>
#include "mm_dmaheap_alloc_debug.h"

#define MM_DMAHEAP_ALLOC_MAX_LATENCY_US	(500 * 1000) /* 500ms */

void mm_dmaheap_debug_get_meminfo(struct mm_dmaheap_debug_info *dmaheap_dbg_info)
{
	dmaheap_dbg_info->kernel_stack = global_node_page_state(NR_KERNEL_STACK_KB);
	dmaheap_dbg_info->free_cma = global_zone_page_state(NR_FREE_CMA_PAGES);
	dmaheap_dbg_info->free_page = global_zone_page_state(NR_FREE_PAGES);
	dmaheap_dbg_info->nr_inactive_file = global_node_page_state(NR_INACTIVE_FILE);
	dmaheap_dbg_info->nr_isolated_anon = global_node_page_state(NR_ISOLATED_ANON);
	dmaheap_dbg_info->nr_isolated_file = global_node_page_state(NR_ISOLATED_FILE);
}

#ifdef CONFIG_HISYSEVENT
#define MAX_MSG_LEN 50
#define ION_OVER_LIMIT "ION_OVER_LIMIT"
#define BYTE_2_KB_SHIFT 10

void mm_dmaheap_report_leak_event(unsigned long total_used)
{
	int ret;
	struct hiview_hisysevent *event = NULL;
	static DEFINE_RATELIMIT_STATE(retelimit_dmaheap_leak_report, (30 * 60 * HZ), 1); // 30 mins
	char msg[MAX_MSG_LEN] = { 0 };
	unsigned long dmaheap_total_used = total_used >> BYTE_2_KB_SHIFT;

	if (!__ratelimit(&retelimit_dmaheap_leak_report))
		return;

	ret = snprintf_s(msg, MAX_MSG_LEN, MAX_MSG_LEN - 1, "dmaheap total used:%lu Kb\n", dmaheap_total_used);
	if (ret < 0) {
		pr_err("%s: snprintf_s failed, ret:%d", __func__, ret);
		return;
	}

	event = hisysevent_create("KERNEL_VENDOR", "LOWMEM", FAULT);
	if (event == NULL) {
		pr_err("%s: create hisysevent failed: LOWMEM\n", __func__);
		return;
	}

	hisysevent_put_string(event, "REASON", ION_OVER_LIMIT);
	hisysevent_put_string(event, "MSG", msg);

	ret = hisysevent_write(event);
	if (ret < 0)
		pr_err("%s: failed to write hisysevent, ret:%d, dmaheap total used:%lu Kb", __func__, ret, dmaheap_total_used);
	else
		pr_info("%s: send hisysevent success, dmaheap total used:%lu Kb\n", __func__, dmaheap_total_used);

	hisysevent_destroy(&event);
	return;
}
#endif

void mm_dmaheap_debug_get_timeinfo(struct mm_dmaheap_debug_info *dmaheap_dbg_info, unsigned int runtime_stage)
{
#ifdef CONFIG_LIBLINUX
	struct liblinux_tcb_time time;
#endif

	switch (runtime_stage) {
	case DMAHEAP_ALLOC_START:
		dmaheap_dbg_info->msleep_count_s = atomic64_read(&shrink_msleep_count);
		dmaheap_dbg_info->_stime = ktime_get();
#if defined(CONFIG_LIBLINUX)
		if (liblinux_pal_get_task_sched_time(&time) < 0) {
			pr_err("liblinux_pal_get_task_sched_time failed!");
		} else {
			dmaheap_dbg_info->run_time = time.runtime;
			dmaheap_dbg_info->run_delay = time.ready;
		}
#elif defined(CONFIG_SCHED_INFO)
		dmaheap_dbg_info->run_time = task_sched_runtime(current);
		dmaheap_dbg_info->run_delay = current->sched_info.run_delay;
#endif
		break;
	case DMAHEAP_ALLOC_END:
		dmaheap_dbg_info->_etime = ktime_get();
		dmaheap_dbg_info->_timedelta = ktime_us_delta(dmaheap_dbg_info->_etime, dmaheap_dbg_info->_stime);
#if defined(CONFIG_LIBLINUX)
		if (liblinux_pal_get_task_sched_time(&time) < 0) {
			pr_err("liblinux_pal_get_task_sched_time failed!");
		} else {
			dmaheap_dbg_info->run_time = time.runtime - dmaheap_dbg_info->run_time;
			dmaheap_dbg_info->run_delay = time.ready - dmaheap_dbg_info->run_delay;
		}
#elif defined(CONFIG_SCHED_INFO)
		dmaheap_dbg_info->run_time = task_sched_runtime(current) - dmaheap_dbg_info->run_time;
		dmaheap_dbg_info->run_delay = current->sched_info.run_delay - dmaheap_dbg_info->run_delay;
#endif
		dmaheap_dbg_info->msleep_count_e = atomic64_read(&shrink_msleep_count);
		break;
	default:
		pr_err("%s: Invalid runtime_stage %u\n", __func__, runtime_stage);
	}
}

void _mm_dmaheap_debug_print_info(struct mm_dmaheap_debug_info *dmaheap_dbg_info, unsigned int print_flag)
{
	switch (print_flag) {
	case DMAHEAP_ALLOC_SIZE_GT_200MB:
		pr_err("dmaheap_alloc_size > 200MB: inactive_file:%lu, isolated_file:%lu, isolated_anon:%lu,"
			"free pages:%lu, kernel_stack:%luKB, free_cma:%lu\n",
			dmaheap_dbg_info->nr_inactive_file, dmaheap_dbg_info->nr_isolated_file,
			dmaheap_dbg_info->nr_isolated_anon, dmaheap_dbg_info->free_page,
			dmaheap_dbg_info->kernel_stack, dmaheap_dbg_info->free_cma);
		break;
	case DMAHEAP_ALLOC_TIME_GT_100MS:
#if defined(CONFIG_LIBLINUX) || defined(CONFIG_SCHED_INFO)
		pr_err("dmaheap_alloc_time > 100MS: PID:%d, process_name:%s,"
			"free_pages:%lu, time_cost:%lld,"
			"runtime:%llu, run_delay:%llu\n",
			current->pid, current->comm, dmaheap_dbg_info->free_page,
			dmaheap_dbg_info->_timedelta, dmaheap_dbg_info->run_time / 1000,
			dmaheap_dbg_info->run_delay / 1000); /* 1000:ns To us */
#endif
		break;
	case DMAHEAP_ALLOC_TIME_GT_500MS:
		pr_err("dmaheap_alloc_time > 500MS: inactive_file:%lu, isolated_file:%lu,"
			"isolated_anon:%lu, free pages:%lu, kernel_stack:%luKB,"
			"free_cma:%lu, msleep_count_s:%ld, msleep_count_e:%ld,"
			"time cost:%lld\n",
			dmaheap_dbg_info->nr_inactive_file, dmaheap_dbg_info->nr_isolated_file,
			dmaheap_dbg_info->nr_isolated_anon, dmaheap_dbg_info->free_page,
			dmaheap_dbg_info->kernel_stack, dmaheap_dbg_info->free_cma,
			dmaheap_dbg_info->msleep_count_s, dmaheap_dbg_info->msleep_count_e,
			dmaheap_dbg_info->_timedelta);
		break;
	default:
		pr_err("%s: Invalid print_flag %u\n", __func__, print_flag);
	}
}

void mm_dmaheap_debug_print_info(struct mm_dmaheap_debug_info *dmaheap_dbg_info, size_t len,
			  struct dma_heap *heap, unsigned long flags)
{
	if ((dmaheap_dbg_info->_timedelta > 50000) && (len > DMABUF_LARGE_BUF_SIZE)) /* 50000 : means 50ms */
		pr_err("%s: dmaheap_alloc time :%ld us, [%s]alloc size = 0x%lx, flags = 0x%x\n",
			__func__, dmaheap_dbg_info->_timedelta, heap->name, len, flags);
	if (dmaheap_dbg_info->_timedelta > MM_DMAHEAP_ALLOC_MAX_LATENCY_US) {
		pr_err("%s: dmaheap_alloc time more than 500ms, [%s]alloc size = 0x%lx, flags=0x%x,\n",
			__func__, heap->name, len, flags);
		_mm_dmaheap_debug_print_info(dmaheap_dbg_info, DMAHEAP_ALLOC_TIME_GT_500MS);
		mm_dmaheap_proecss_info();
		show_mem(0, NULL);
	}

#if defined(CONFIG_LIBLINUX) || defined(CONFIG_SCHED_INFO)
	if (dmaheap_dbg_info->_timedelta > 100000) { /* 100000 : means 100ms */
		pr_err("%s: dmaheap_alloc time more than 100ms, [%s]alloc size = 0x%lx, flags = 0x%x\n",
			__func__, heap->name, len, flags);
		_mm_dmaheap_debug_print_info(dmaheap_dbg_info, DMAHEAP_ALLOC_TIME_GT_100MS);
	}
	pr_debug("dmaheap_alloc_time: PID:%d, process_name:%s,"
		"free_pages:%lu, time_cost:%lld,"
		"runtime:%llu, run_delay:%llu\n",
		current->pid, current->comm, dmaheap_dbg_info->free_page,
		dmaheap_dbg_info->_timedelta, dmaheap_dbg_info->run_time / 1000,
		dmaheap_dbg_info->run_delay / 1000); /* 1000:ns To us */
#endif
}
