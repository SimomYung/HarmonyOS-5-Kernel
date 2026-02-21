/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Function for hmprobe
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 27 14:42:19 2024
 */
#ifndef KLIBS_TRACE_HMPROBE_H
#define KLIBS_TRACE_HMPROBE_H

#include <hmkernel/errno.h>
#include <hmkernel/compiler.h>
#include <hmkernel/jump_label.h>

#include "tracepoint_defs.h"

#undef DECLARE_HMPROBE_CLASS

#ifdef CONFIG_HMPROBE
#define DECLARE_HMPROBE_CLASS(group, name, tcb_event, args, proto, fields, assign) \
__maybe_unused									\
static void hmprobe_update_##name##_tp_func(struct trace_event_entry_##name *__entry,	\
			struct trace_##group##_##name##_offset_s data_offsets,	\
			TP_FUNC_PROTO(proto))					\
{ 										\
	struct tracepoint_func *tp_func = &tracepoint_##name.tp_func;		\
	volatile void *func = tp_func->func;					\
	volatile void *data = tp_func->data;					\
	__maybe_unused bool entry_data_alloc = false;				\
	if ((func != NULL) && (func == tp_func->func) &&			\
	    (data == tp_func->data) &&						\
	    !is_tracing_mark_write(tracepoint_##name)) {			\
		struct trace_##group##_##name##_ctx_s ctx;			\
		bool func_running = is_tp_func_running();			\
										\
		if (!func_running) {						\
			if (__entry == NULL) {					\
				__entry = &ctx.entry;				\
				__entry->th.common_pid = hm_trace_tid_of(tcb_event);	\
				__entry->th.common_type = tracepoint_##name.type;	\
				fields							\
				{assign}						\
			} else {							\
				ctx.entry = *__entry;					\
			}								\
			CONCATENATE(copy_trace_args, COUNT_ARGS(args))(ctx.raw_args, CAST_TO_U64(args)); \
			set_tp_func_running(true);				\
			((void(*)(void *, __u64, void *, void *))(func))(&ctx, sizeof(ctx), (void *)&tracepoint_##name, (void *)data); \
			set_tp_func_running(false);				\
		}								\
	}									\
}
#else
#define DECLARE_HMPROBE_CLASS(group, name, tcb_event, args, proto, fields, assign) \
__maybe_unused								\
static void hmprobe_update_##name##_tp_func(struct trace_event_entry_##name *__entry,	\
			struct trace_##group##_##name##_offset_s data_offsets,	\
			TP_FUNC_PROTO(proto))					\
{ 										\
}
#endif
#endif
