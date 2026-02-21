/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2023-2024. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#undef TRACE_SYSTEM
#define TRACE_SYSTEM hvgr 

#if !defined(HVGR_TRACE_H) || defined(TRACE_HEADER_MULTI_READ)
#define HVGR_TRACE_H

#include <linux/types.h>
#include <linux/tracepoint.h>

TRACE_EVENT(tracing_mark_write,
	TP_PROTO(int pid, const char *name, bool trace_begin),
	TP_ARGS(pid, name, trace_begin),
	TP_STRUCT__entry(
		__field(int, pid)
		__string(trace_name, name)
		__field(bool, trace_begin)
	),
	TP_fast_assign(
		__entry->pid = pid;
		__assign_str(trace_name, name);
		__entry->trace_begin = trace_begin;
	),
	TP_printk("%s|%d|%s", __entry->trace_begin ? "B" : "E",
		__entry->pid, __get_str(trace_name))
)

void hvgr_systrace_begin(const char *fmt, ...);
void hvgr_systrace_end(void);

#endif

/* This part must be outside protection */
#undef TRACE_INCLUDE_PATH
#define TRACE_INCLUDE_PATH HVGR_TRACE_INCLUDE_PATH

#undef TRACE_INCLUDE_FILE
#define TRACE_INCLUDE_FILE hvgr_trace

#include <trace/define_trace.h>
