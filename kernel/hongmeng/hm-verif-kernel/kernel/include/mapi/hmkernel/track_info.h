/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: track info mapi
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 2 08:00:40 PM 2024
 */
#ifndef MAPI_HMKERNEL_TRACK_INFO_H
#define MAPI_HMKERNEL_TRACK_INFO_H
#include <hmkernel/types.h>
#include <hmkernel/macro.h>
#include <uapi/hmkernel/track_info/track_info.h>

#define TRACK_INFO_IRQ_ENTRY		0
#define TRACK_INFO_IRQ_EXIT		1

#ifdef CONFIG_TRACK_INFO
#define HMCAP_THREAD_NAME_SIZE 16
#define __TRACK_INFO_STRING(name, size)    name[size]
#define __TRACK_INFO_ARRAY(name, size)     name[size]

/* generate track info record struct */
#define TRACK_INFO_STRUCT(...) __VA_ARGS__
#define TRACK_INFO_STRUCT_GEN(record_type, n, ...)			\
	struct record_type##_record_s {					\
		RECORD_COMMON_ENTRY					\
		__MAPSTAT_DEFVAR(n, __SC_DEF, __VA_ARGS__);		\
	} __aligned(4);
#define DEFINE_TRACK_INFO(RECORD_TYPE, record_type, args, strs, ...)	\
	TRACK_INFO_STRUCT_GEN(record_type, strs)
#include <hmkernel/track_info/track_info_list.h>
#undef DEFINE_TRACK_INFO
#undef TRACK_INFO_STRUCT

/* generate track info record function */
#define TRACK_INFO_ARGS(...) __VA_ARGS__
#define TRACK_INFO_PROTO_GEN(n, ...) __PROTO_MAP(n, __SC_DEF, __VA_ARGS__)
#define TRACK_INFO_CALL_GEN(n, ...) __MAP(n, __SC_CALL, __VA_ARGS__)
#define DEFINE_TRACK_INFO(RECORD_TYPE, record_type, args, ...)			\
extern void __track_info_record_##record_type(TRACK_INFO_PROTO_GEN(args));	\
static inline void track_info_record_##record_type(TRACK_INFO_PROTO_GEN(args))	\
{										\
	__track_info_record_##record_type(TRACK_INFO_CALL_GEN(args));		\
}
#include <hmkernel/track_info/track_info_list.h>
#undef DEFINE_TRACK_INFO
#undef TRACK_INFO_ARGS

#else /* !CONFIG_TRACK_INFO */
#define TRACK_INFO_ARGS(...) __VA_ARGS__
#define TRACK_INFO_PROTO_GEN(n, ...) __PROTO_MAP(n, __SC_DEF, __VA_ARGS__)
#define TRACK_INFO_CALL_GEN(n, ...) __MAP(n, __SC_CALL, __VA_ARGS__)
#define DEFINE_TRACK_INFO(RECORD_TYPE, record_type, args, ...)			\
static inline void track_info_record_##record_type(TRACK_INFO_PROTO_GEN(args))	\
{										\
	UNUSED(TRACK_INFO_CALL_GEN(args));					\
}
#include <hmkernel/track_info/track_info_list.h>
#undef DEFINE_TRACK_INFO
#undef TRACK_INFO_ARGS
#endif /* CONFIG_TRACK_INFO */
#endif
