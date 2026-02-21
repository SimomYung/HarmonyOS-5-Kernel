/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 8 18:41:38 2023
 */
#ifndef ULIBS_TRACK_INFO_PARSE_HANDLER_H
#define ULIBS_TRACK_INFO_PARSE_HANDLER_H

#include <hongmeng/macro.h>
#include <hmkernel/compiler.h>
#include <hmkernel/types.h>
#include <hmkernel/track_info/track_info.h>

/* generate track info record struct */
#define HMCAP_THREAD_NAME_SIZE 16
#define __TRACK_INFO_STRING(name, size)    name[size]
#define __TRACK_INFO_ARRAY(name, size)     name[size]

#define TRACK_INFO_STRUCT(...) __VA_ARGS__
#define TRACK_INFO_STRUCT_GEN(record_type, n, ...)		\
	struct record_type##_record_s {				\
		RECORD_COMMON_ENTRY				\
		__SC_MAPSTAT(n, __SC_DECL, __VA_ARGS__);	\
	} __aligned(4);
#define DEFINE_TRACK_INFO(RECORD_TYPE, record_type, args, strs, ...)		\
	TRACK_INFO_STRUCT_GEN(record_type, strs)
#include <hmkernel/track_info/track_info_list.h>
#undef DEFINE_TRACK_INFO
#undef TRACK_INFO_STRUCT_GEN
#undef TRACK_INFO_STRUCT

typedef int (*hm_track_info_iter_func_t)(const char *buf, unsigned int len, uintptr_t arg);

const char *hm_track_info_file_name_of(__u8 record_type);
struct track_info_slot_header_s *
hm_track_info_slot_header_of(void *track_info_ptr, size_t size, bool dump_active);
struct track_info_region_header_s *
hm_track_info_region_header_of(struct track_info_slot_header_s *slot_header, unsigned int type);
int hm_track_info_record_for_each(struct track_info_region_header_s *region_header,
				  hm_track_info_iter_func_t func, uintptr_t arg);
int hm_track_info_set(bool pause);

#endif
