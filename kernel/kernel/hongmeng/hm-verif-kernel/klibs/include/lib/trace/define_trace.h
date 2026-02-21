/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Define trace
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 22 18:58:39 2023
 */
#ifndef KLIBS_DEFINE_TRACE_H
#define KLIBS_DEFINE_TRACE_H
#include <hmkernel/attributes/hmattributes.h>
__hmattr_tag(TRACE);
/* step 1, define the trace struct */
#include <lib/trace/define/define_struct.h>
#undef DEFINE_TRACE_STRUCT
#define DEFINE_TRACE_STRUCT(group, _name, proto, args, fields, assign)	\
	struct trace_event_entry_##_name {						\
		struct trace_header th;						\
		fields								\
		char __data[];							\
	};

#undef TRACE_TCB_EVENT
#define TRACE_TCB_EVENT TRACE_EVENT

#undef TRACE_TCB_EVENT_HOT
#define TRACE_TCB_EVENT_HOT TRACE_EVENT

#undef TRACE_EVENT
#define TRACE_EVENT(group, event, proto, args, fields, assign, print)	\
	DEFINE_TRACE_STRUCT(group, event, PARAMS(proto), PARAMS(args), PARAMS(fields), PARAMS(assign))
#include TRACE_INCLUDE_FILE

/* step2, define the struct offset for dynamic_array */
#include <lib/trace/define/define_offset.h>
#undef DEFINE_TRACE_OFFSET
#define DEFINE_TRACE_OFFSET(group, name, proto, args, fields, assign)	\
	struct trace_##group##_##name##_offset_s {	\
		fields					\
	};
#undef TRACE_EVENT
#define TRACE_EVENT(group, event, proto, args, fields, assign, print)	\
	DEFINE_TRACE_OFFSET(group, event, PARAMS(proto), PARAMS(args), PARAMS(fields), PARAMS(assign))
#include TRACE_INCLUDE_FILE

/* step 3, define the function to calculate size of dynamic_array */
#include <lib/trace/define/define_data_size.h>
#undef DEFINE_TRACE_DATA_SIZE
#define DEFINE_TRACE_DATA_SIZE(group, _name, proto, args, fields, assign)			\
	static inline int trace_cal_##_name##_offsets( 				\
			struct trace_##group##_##_name##_offset_s *__data_offsets, TP_FUNC_PROTO(proto))	\
	{ 											\
		int __data_size = 0;								\
		__maybe_unused unsigned int __item_length;						\
		__maybe_unused struct trace_event_entry_##_name *entry;			\
		fields										\
		return __data_size;								\
	}
#undef TRACE_EVENT
#define TRACE_EVENT(group, event, proto, args, fields, assign, print)	\
	DEFINE_TRACE_DATA_SIZE(group, event, PARAMS(proto), PARAMS(args), PARAMS(fields), PARAMS(assign))
#include TRACE_INCLUDE_FILE

/* step 5, define the trace function */
#include <lib/trace/define/define_assign.h>

#undef TRACE_TCB_EVENT
#define TRACE_TCB_EVENT(group, event, proto, args, fields, assign, print)	\
	DEFINE_TRACE(group, event, 1, true, PARAMS(proto), PARAMS(args), PARAMS(fields), PARAMS(assign))

#undef TRACE_TCB_EVENT_HOT
#define TRACE_TCB_EVENT_HOT(group, event, proto, args, fields, assign, print)	\
	DEFINE_TRACE(group, event, HMPERF_TP_HOT_ETSIZE, true, PARAMS(proto), PARAMS(args), PARAMS(fields), PARAMS(assign))

#undef TRACE_EVENT
#define TRACE_EVENT(group, event, proto, args, fields, assign, print)	\
	DEFINE_TRACE(group, event, 1, false, PARAMS(proto), PARAMS(args), PARAMS(fields), PARAMS(assign))

#include TRACE_INCLUDE_FILE

#endif /* KLIBS_DEFINE_TRACE_H */
