/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#undef TRACE_SYSTEM
#define TRACE_SYSTEM memcheck

#undef TRACE_INCLUDE_PATH
#define TRACE_INCLUDE_PATH platform/trace/hooks

#if !defined(_TRACE_MEMCHECK_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_MEMCHECK_H

#include <linux/tracepoint.h>
#include <platform_include/basicplatform/linux/hck_vendor_native_hooks.h>
#include <platform_include/basicplatform/linux/hck_vendor_restricted_hooks.h>

DECLARE_HCK_VRH(mm_mem_stats_show,
	TP_PROTO(int unused),
	TP_ARGS(unused), 1);

#if defined(CONFIG_DFX_OHOS) && defined(CONFIG_MALI_MIDGARD)
	DECLARE_HCK_VRH(gpu_get_total_used,
		TP_PROTO(unsigned long *gpu_total),
		TP_ARGS(gpu_total), 1);
#else
	DECLARE_HCK_VNH(gpu_get_total_used,
		TP_PROTO(unsigned long *gpu_total),
		TP_ARGS(gpu_total));
#endif /* hisi and qcom gpu */

DECLARE_HCK_VRH(mm_memcheck_gpumem_info_show,
	TP_PROTO(int unused),
	TP_ARGS(unused), 1);

DECLARE_HCK_VRH(mm_memcheck_gpumem_get_usage,
	TP_PROTO(pid_t pid, u64 *mapped, u64 *total),
	TP_ARGS(pid, mapped, total), 1);

DECLARE_HCK_VRH(cma_report,
	TP_PROTO(char *name, unsigned long total, unsigned long req),
	TP_ARGS(name, total, req), 1);

DECLARE_HCK_VRH(slub_obj_report,
	TP_PROTO(struct kmem_cache *s),
	TP_ARGS(s), 1);

DECLARE_HCK_VRH(lowmem_report,
	TP_PROTO(struct task_struct *p, unsigned long points),
	TP_ARGS(p, points), 1);

#endif /* _TRACE_MEMCHECK_H */

/* This part must be outside protection */
#include <trace/define_trace.h>
