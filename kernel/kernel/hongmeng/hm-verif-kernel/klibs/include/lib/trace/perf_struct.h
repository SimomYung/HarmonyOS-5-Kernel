/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Define the control struct of perf
 * Author: Huawei OS Kernel Lab
 * Create: Wed Oct 25 10:12:42 2023
 */
#ifndef KLIBS_PERF_STRUCT_H
#define KLIBS_PERF_STRUCT_H

#include <lib/trace/trace_event.h>

#include "tracepoint_defs.h"

#endif /* KLIBS_PERF_STRUCT_H */

#ifdef CONFIG_HMPERF_NG
#undef __field
#define __field(type, item) type item;
#undef __array
#define __array(type, item, len) type item[len];
#undef __dynamic_array
#define __dynamic_array(type, item, len) __u32 __data_loc_##item;
#undef __get_dynamic_array
#define __get_dynamic_array(item) ((void *)((char *)__entry + (__entry->__data_loc_##item & 0xffff)))
/* You must use this function to get dynamic_array! */
#undef __get_dynamic_array_len
#define __get_dynamic_array_len(item) ((void *)((char *)__entry + ((__entry->__data_loc_##item >> 16) & 0xffff)))
#undef __get_str
#define __get_str(field) ((char *)__get_dynamic_array(field))

#undef DECLARE_EVENT_CLASS_STRUCT
#define DECLARE_EVENT_CLASS_STRUCT(name, fields)		\
struct trace_event_entry_##name {						\
	struct trace_header ent;						\
	fields								\
	char __data[];							\
};
#else
#define DECLARE_EVENT_CLASS_STRUCT(name, fields)
#endif /* CONFIG_HMPERF_NG */
