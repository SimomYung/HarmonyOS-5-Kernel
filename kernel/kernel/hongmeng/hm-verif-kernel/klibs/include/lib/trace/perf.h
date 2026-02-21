/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Macro of tracepoint
 * Author: Huawei OS Kernel Lab
 * Create: Mon SEP 11 11:11:53 2023
 */
#ifndef KLIBS_PERF_H
#define KLIBS_PERF_H

#include <hmkernel/errno.h>
#include <hmkernel/compiler.h>
#include <hmkernel/types.h>
#include <hmkernel/jump_label.h>
#include <hmkernel/strict.h>
#include <lib/dlist.h>
#include <lib/trace/trace_perf.h>
#include <lib/trace/trace_event.h>

#include "tracepoint_defs.h"

#undef DECLARE_EVENT_CLASS

#ifdef CONFIG_HMPERF_NG
#define DECLARE_EVENT_CLASS(group, name, etsize, proto, fields, assign)		\
static struct dlist_node etable_##name[etsize];					\
										\
__maybe_unused									\
static void perf_trace_##name(void *__data, TP_FUNC_PROTO(proto))		\
{										\
	struct dlist_node *etable = __data;					\
	struct trace_event_entry_##name *__entry = NULL;			\
	struct trace_##group##_##name##_offset_s data_offsets;			\
	struct arch_regs *__regs = NULL;					\
	__u32 entry_size;							\
	__maybe_unused bool entry_data_alloc = true;				\
	entry_size = (__u32)ALIGN_UP(sizeof(*__entry) +			\
			(size_t)trace_cal_##name##_offsets(&data_offsets, TP_FUNC_ARGS(proto)) + \
			sizeof(__u32), sizeof(__u64));			\
	entry_size -= (__u32)sizeof(__u32);				\
									\
	__entry = (struct trace_event_entry_##name *)perf_trace_alloc_entry(entry_size); \
	if (__entry == NULL) {						\
		return;							\
	}								\
	__regs = perf_trace_alloc_regs();				\
	if (__regs == NULL) {						\
		perf_trace_free_entry(__ptr_to_u64(__entry));		\
		return;							\
	}								\
									\
	mem_zero_s(*__entry);						\
	mem_zero_s(*__regs);						\
	perf_fetch_caller_regs(__regs);					\
	fields								\
	{ assign }							\
									\
	perf_trace_buf_submit(__ptr_to_u64(__entry), entry_size, __regs, etable, etsize); \
									\
	perf_trace_free_entry(__ptr_to_u64(__entry));			\
	perf_trace_free_regs(__regs);					\
}

#else
#define DECLARE_EVENT_CLASS(group, name, etsize, proto, fields, assign)	\
static struct dlist_node etable_##name[etsize];				\
									\
__maybe_unused								\
static void perf_trace_##name(void *__data, TP_FUNC_PROTO(proto))	\
{									\
}

#endif
#endif /* KLIBS_PERF_H */
