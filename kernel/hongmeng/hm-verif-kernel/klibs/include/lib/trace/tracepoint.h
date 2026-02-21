/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Macro of tracepoint
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 25 11:11:53 2023
 */
#ifndef KLIBS_TRACEPOINT_H
#define KLIBS_TRACEPOINT_H
#include <hmkernel/errno.h>
#include <hmkernel/compiler.h>
#include <hmkernel/types.h>
#include <hmkernel/jump_label.h>
#include <hmkernel/strict.h>
#include <lib/trace/trace_misc.h>
#include <lib/trace/perf.h>
#include <lib/trace/define/define_raw_args.h>

#include "tracepoint_defs.h"

extern struct tracepoint __start___tracepoints[];
extern struct tracepoint __stop___tracepoints[];

extern char *__start___tracepoint_string[];
extern char *__stop___tracepoint_string[];

extern unsigned int __start___tracepoint_string_id[];
extern unsigned int __stop___tracepoint_string_id[];

#define tracepoint_string(str)	\
	({	\
		static const char*  ___tp_str __attribute__((used)) __attribute__((section("__tracepoint_string"))) = str;	\
		static unsigned int ___tp_str_id __attribute__((used)) __attribute__((section("__tracepoint_string_id")));	\
		___tp_str_id;	\
	})

#endif /* KLIBS_TRACEPOINT_H */

#ifdef __HOST_UT__
#define hm_rmq_enqueue_get_entry(a, b, c, d) d
#define hm_rmq_enqueue_confirm_entry(a) a
#define hm_rmq_is_record_enabled(a) a
#endif

#undef DEFINE_TRACE
#undef DECLARE_TRACE
/*
 * klibs can used by different apps, which have their own configs,
 * So you need define '__HM_TRACEPOINT__' before including this head file,
 * if you want enable tracepoint in your apps.
 */
#ifdef __HM_TRACEPOINT__
/*
 * This is key header file you need to include if you want to add a new tracepoint to a new system service.
 * This header file will define the input function of the tracepoint.
 */

/*
 * In the following cases, the func is Y, but the data is NULL,
 * it causes bug. So before calling func, the func and data need
 * be compared with tp_func.
 * case #1:
 * in __trace_probe_##_name()		in register_handler()
 * data = tp_func.data(data is NULL)
 * 					tp_func.data = X
 * 					tp_func.func = Y
 * func = tp_func.func(func is Y)
 *
 * case #2:
 * in __trace_probe_##_name()		in unregister_handler()
 * func = tp_func.func(func is Y)
 * 					tp_func.data = NULL
 * 					tp_func.func = NULL
 * data = tp_func.data(data is NULL)
 */
#ifdef __KERNEL__
#define is_tracing_mark_write(tp)\
	(&tp == &tracepoint_tracing_mark_write)
#else
#define is_tracing_mark_write(tp)\
	(0)
#endif /* __KERNEL__ */

#ifdef CONFIG_TRACE_GLOBAL_KEY
extern bool g_trace_key;
#define trace_enabled(name) 						\
	(unlikely(g_trace_key) && trace_##name##_enabled())
#else
#define trace_enabled(name)						\
	trace_##name##_enabled()
#endif

#define DEFINE_TRACE(group, _name, etsize, tcb_event, proto, args, fields, assign)		\
	DECLARE_EVENT_CLASS(group, _name, etsize, PARAMS(proto), PARAMS(fields), PARAMS(assign)) \
	void __trace_probe_##_name(TP_FUNC_PROTO(proto));				\
	extern struct tracepoint tracepoint_tracing_mark_write;				\
	const char tracepoint_str_##_name[] __attribute__((section(TRACEPOINT_STR_SECTION))) = #_name; \
	struct static_key tracepoint_key_##_name = {.enabled = 0};			\
	struct tracepoint tracepoint_##_name __attribute__((section(TRACEPOINT_SECTION))) = { \
		.group_name = __group_##group,						\
		.name = tracepoint_str_##_name,						\
		.type = 0U,								\
		.key = &tracepoint_key_##_name,						\
		.tp_func.func = NULL,							\
		.tp_func.data = NULL,							\
		.etable = etable_##_name,						\
		.etable_size = etsize,							\
		.attached_events = 0,							\
	};										\
	struct trace_##group##_##_name##_ctx_s {					\
		__u64 raw_args[COUNT_ARGS(args)];					\
		struct trace_event_entry_##_name entry;					\
	};										\
	void __trace_probe_##_name(TP_FUNC_PROTO(proto))				\
	{										\
		__s64 ret = 0;								\
		struct trace_##group##_##_name##_offset_s data_offsets; 		\
		unsigned int array_len = (unsigned int)trace_cal_##_name##_offsets(&data_offsets, TP_FUNC_ARGS(proto));\
		__u64 len = sizeof(struct trace_event_entry_##_name) + array_len;	\
		struct trace_event_entry_##_name *__entry = NULL;			\
		struct tracepoint_func *tp_func = &tracepoint_##_name.tp_func;		\
		volatile void *func = NULL;						\
		volatile void *data = NULL;						\
		__maybe_unused bool entry_data_alloc = true;				\
		rmq_producer_entry_t pe = {0};						\
		void *q = hm_trace_shm_record_addr_of();				\
		if (q && unlikely(hm_rmq_is_record_enabled(q))) {			\
			unsigned int curr_cpu = hm_trace_current_cpu_of(); 		\
			ret = hm_rmq_enqueue_get_entry(q, curr_cpu, &pe, len);		\
			if (ret <= 0 || (__u64)ret != len) {				\
			} else {							\
				__entry = (struct trace_event_entry_##_name *)pe.addr;\
				__entry->th.common_flags = 0;			\
				__entry->th.common_preempt_count = 0;		\
				__entry->th.common_pid = hm_trace_tid_of(tcb_event);	\
				__entry->th.common_type = tracepoint_##_name.type;	\
				fields							\
				{assign} 						\
				data = tp_func->data;					\
				func = tp_func->func;					\
				if ((func != NULL) && (func == tp_func->func) &&	\
				    (data == tp_func->data) &&				\
				    !is_tracing_mark_write(tracepoint_##_name)) {	\
					struct trace_##group##_##_name##_ctx_s ctx;	\
					bool func_running = is_tp_func_running();	\
											\
					if (!func_running) {				\
						ctx.entry = *__entry;			\
						CONCATENATE(copy_trace_args, COUNT_ARGS(args))(ctx.raw_args, CAST_TO_U64(args)); \
						set_tp_func_running(true);		\
						((void(*)(void *, __u64, void *, void *))(func))(&ctx, sizeof(ctx), (void *)&tracepoint_##_name, (void *)data); \
						set_tp_func_running(false);		\
					}						\
				}							\
				hm_rmq_enqueue_confirm_entry(&pe);			\
				hm_trace_rmq_dequeue_futex_wake();		\
				hm_trace_set_taskinfo_save(curr_cpu, true);		\
			}								\
		}									\
		if (__entry == NULL) {							\
			data = tp_func->data;						\
			func = tp_func->func;						\
			if ((func != NULL) && (func == tp_func->func) &&		\
			    (data == tp_func->data) &&					\
			    !is_tracing_mark_write(tracepoint_##_name)) {		\
				struct trace_##group##_##_name##_ctx_s ctx;		\
				bool func_running = is_tp_func_running();		\
											\
				if (!func_running) {					\
					entry_data_alloc = false;			\
					__entry = &ctx.entry;				\
					__entry->th.common_pid = hm_trace_tid_of(tcb_event);	\
					__entry->th.common_type = tracepoint_##_name.type;	\
					fields						\
					{assign}					\
					CONCATENATE(copy_trace_args, COUNT_ARGS(args))(ctx.raw_args, CAST_TO_U64(args)); \
					set_tp_func_running(true);			\
					((void(*)(void *, __u64, void *, void *))(func))(&ctx, sizeof(ctx), (void *)&tracepoint_##_name, (void *)data); \
					set_tp_func_running(false);			\
				}							\
			}								\
		}									\
		if (__read_once(tracepoint_##_name.attached_events) > 0) {		\
			perf_trace_##_name(etable_##_name, TP_FUNC_ARGS(proto));	\
		}								\
	}

#define DECLARE_TRACE(name, proto)	\
	extern struct static_key tracepoint_key_##name;							\
	extern struct tracepoint tracepoint_##name;							\
	extern void __trace_probe_##name(TP_FUNC_PROTO(proto));						\
	static inline bool trace_##name##_enabled(void)                                                 \
	{												\
		return static_branch_unlikely((struct static_key * const)&tracepoint_key_##name);	\
	}												\
	static inline void trace_##name(TP_FUNC_PROTO(proto))						\
	{												\
		if (trace_enabled(name)) {								\
			__trace_probe_##name(TP_FUNC_ARGS(proto));					\
		}											\
	}
#else /* __HM_TRACEPOINT__ */
#define DEFINE_TRACE(group, name, etsize, tcb_event, proto, args, fields, assign)
#define DECLARE_TRACE(name, proto)	\
	static inline void trace_##name(TP_FUNC_PROTO(proto))	\
	{	\
	}	\
	static inline bool trace_##name##_enabled(void)	\
	{	\
		return false;	\
	}
#endif /* __HM_TRACEPOINT__ */

#undef TRACE_TCB_EVENT
#define TRACE_TCB_EVENT TRACE_EVENT

#undef TRACE_TCB_EVENT_HOT
#define TRACE_TCB_EVENT_HOT TRACE_EVENT

#undef __field
#define __field(type, item)
#undef __array
#define __array(type, item, len)
#undef __dynamic_array
#define __dynamic_array(type, item, len)
#undef __string
#define __string(item, src)

#ifndef TRACE_EVENT
#define TRACE_EVENT(group, name, proto, args, fields, assign, print)	\
	DECLARE_TRACE(name, PARAMS(proto))
#endif /* TRACE_EVENT */
