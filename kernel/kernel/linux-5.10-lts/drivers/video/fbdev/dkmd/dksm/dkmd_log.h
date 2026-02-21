/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef _DKMD_LOG_H_
#define _DKMD_LOG_H_

#include <linux/types.h>
#include <linux/ktime.h>
#include <linux/semaphore.h>
#include "dksm_debug.h"
#include "ukmd_log.h"

#ifdef CONFIG_MDFX_KMD
#include <platform_include/display/linux/hisi_mdfx.h>
#endif

#define LOG_INTER_COUNT 5

#ifdef CONFIG_MDFX_KMD
////////////////////////////////////////////////////////////////////////
/*
 * Message printing priorities:
 * LEVEL 0 KERN_ERR (highest priority)
 * LEVEL 1 KERN_WARNING
 * LEVEL 2 KERN_NOTICE
 * LEVEL 3 KERN_INFO
 * LEVEL 4 KERN_DEBUG (Lowest priority)
 */
#define dpu_pr_err(msg, ...) \
	do { \
		MDFX_ERR(g_dkmd_mdfx_id, "[DKMD]"msg, ##__VA_ARGS__); \
		xkmd_pr_err("DKMD", msg, ##__VA_ARGS__); \
	} while (0)
 
#define dpu_pr_warn(msg, ...) \
	do { \
		MDFX_WARNING(g_dkmd_mdfx_id, "[DKMD]"msg, ##__VA_ARGS__); \
		xkmd_pr_warn("DKMD", g_dkmd_log_level, msg, ##__VA_ARGS__); \
	} while (0)
 
#define dpu_pr_notice(msg, ...) MDFX_NOTICE(g_dkmd_mdfx_id, "[DKMD]"msg, ##__VA_ARGS__)
#define dpu_pr_info(msg, ...) \
	do { \
		MDFX_INFO(g_dkmd_mdfx_id, "[DKMD]"msg, ##__VA_ARGS__); \
		xkmd_pr_info("DKMD", g_dkmd_log_level, msg, ##__VA_ARGS__); \
	} while (0)
 
#define dpu_pr_debug(msg, ...) \
	do { \
		MDFX_DEBUG(g_dkmd_mdfx_id, "[DKMD]"msg, ##__VA_ARGS__); \
		xkmd_pr_debug("DKMD", g_dkmd_log_level, msg, ##__VA_ARGS__); \
	} while (0)

#else

#define dpu_pr_err(msg, ...)  xkmd_pr_err("DKMD", msg, ##__VA_ARGS__)
#define dpu_pr_warn(msg, ...) xkmd_pr_warn("DKMD", g_dkmd_log_level, msg, ##__VA_ARGS__)
#define dpu_pr_info(msg, ...) xkmd_pr_info("DKMD", g_dkmd_log_level, msg, ##__VA_ARGS__)
#define dpu_pr_debug(msg, ...) xkmd_pr_debug("DKMD", g_dkmd_log_level, msg, ##__VA_ARGS__)
#endif

#define dpu_check_and_return(condition, ret, level, msg, ...) \
	do { \
		if (condition) { \
			dpu_pr_##level(msg, ##__VA_ARGS__);\
			return ret; \
		} \
	} while (0)

#define dpu_check_and_no_retval(condition, level, msg, ...) \
	do { \
		if (condition) { \
			dpu_pr_##level(msg, ##__VA_ARGS__);\
			return; \
		} \
	} while (0)

#ifdef CONFIG_DKMD_ASSERT_ENABLE
#define dpu_assert(cond)  do { \
		if (cond) { \
			dpu_pr_err("assertion failed! %s,%s:%d,%s\n", #cond,  __FILE__, __LINE__, __func__); \
			BUG(); \
		} \
	} while (0)
#else
#define dpu_assert(ptr)
#endif

static inline void dpu_get_timestamp(uint64_t *start_ns)
{
	*start_ns = ktime_get_mono_fast_ns();
}

static inline bool log_should_dump_data(uint32_t frame_no)
{
	return (frame_no <= LOG_INTER_COUNT) || (frame_no % LOG_INTER_COUNT == 0);
}

#define dpu_trace_ts_begin(start_ns) dpu_get_timestamp(start_ns)
#define dpu_trace_ts_end(start_ns, msg)  do { \
		if (g_debug_fence_timeline) { \
			uint64_t end_ns; \
			dpu_get_timestamp(&end_ns); \
			dpu_pr_info("%s timediff=%llu us", msg, div_u64(end_ns - *(start_ns), 1000)); \
		} \
	} while (0)

#define dpu_print_timestamp(msg) do { \
		if (g_dkmd_log_level >= UKMD_LOG_LVL_DEBUG) { \
			ktime_t timestamp; \
			timestamp = ktime_get();; \
			dpu_pr_debug("%s timestamp_ns=%lld ns", msg,  ktime_to_ns(timestamp)); \
		} \
	} while (0)

static inline void dpu_print_sem_count(struct semaphore *sem, bool is_down)
{
	if (is_down) {
		if (unlikely(sem->count != 0))
			dpu_pr_warn("down semaphore abnormal, count = %u", sem->count);

		return;
	}

	if (unlikely(sem->count != 0))
		dpu_pr_warn("up semaphore abnormal, count = %u", sem->count);
}

#endif /* DKMD_LOG_H */
