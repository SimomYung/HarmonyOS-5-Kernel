/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2024-2024, The Linux Foundation. All rights reserved.
 */

#undef TRACE_SYSTEM
#define TRACE_SYSTEM dma_buf

#if !defined(_TRACE_DMA_BUF_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_DMA_BUF_H

#include <linux/tracepoint.h>

TRACE_EVENT(dma_buf_alloc_log,
	TP_PROTO(int order, const char * status, struct task_struct *task),

	TP_ARGS(order, status, task),

	TP_STRUCT__entry(
		__field(int, order)
		__string(status, status)
		__field(int, pid)
		__string(comm, task->comm)
	),

	TP_fast_assign(
		__entry->order = order;
		__assign_str(status, status);
		__entry->pid = task->pid;
		__assign_str(comm, task->comm);
	),

	TP_printk("pid=%d comm=%s dma buf order %d status %s", __entry->pid, __get_str(comm), __entry->order,
		__get_str(status))
);

#endif /* !defined(_TRACE_DMA_BUF_H) || defined(TRACE_HEADER_MULTI_READ) */

/* This part must be outside protection */
#include <trace/define_trace.h>