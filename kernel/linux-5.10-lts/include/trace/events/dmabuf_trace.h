/* SPDX-License-Identifier: GPL-2.0-only */

#undef TRACE_SYSTEM
#define TRACE_SYSTEM dmabuf

#if !defined(__DMABUF_TRACE_H) || defined(TRACE_HEADER_MULTI_READ)
#define __DMABUF_TRACE_H

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
);

#endif

/* This part must be outside protection */
#undef TRACE_INCLUDE_PATH
#define TRACE_INCLUDE_PATH trace/events

#undef TRACE_INCLUDE_FILE
#define TRACE_INCLUDE_FILE dmabuf_trace

#include <trace/define_trace.h>
