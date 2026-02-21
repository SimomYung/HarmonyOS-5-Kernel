/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * Description: The System/Software Performance Impact Factor
 * Author: Huawei OS Kernel Lab
 * Create: Thy Dec 23 06:23:51 2021
 */
#ifndef KLIBS_PERFRA_SPIF_H
#define KLIBS_PERFRA_SPIF_H

#include <lib/perfra/ksus.h>
#include <lib/perfra/paju.h>
#include <lib/perfra/pmu.h>
#include <lib/perfra/pfev.h>

#include <lib/perfra/spif_aarch64.h>
#include <lib/perfra/spif_arm.h>

#if defined(__HOST_LLT__) && !defined(__aarch64__) && !defined(__arm__)
static inline __u64 __spif_current_timestamp(void)
{
	return 0;
}
#endif

enum {
	SPIF_KSUS_DESC_INDEX = 0,
	SPIF_KSUS_CHUNK_INDEX,
	SPIF_KSUS_INDEX_NR,
};

static inline struct __pfev_fix16_desc_s *__spif_desc(struct ksus_desc_s *ksus_desc)
{
	return (struct __pfev_fix16_desc_s *)
		(__uptr_t)(ksus_desc->chunk[SPIF_KSUS_DESC_INDEX].base);
}

static inline struct __pfev_fix16_chunk_s *__spif_chunk(struct ksus_desc_s *ksus_desc)
{
	return (struct __pfev_fix16_chunk_s *)
		(__uptr_t)(ksus_desc->chunk[SPIF_KSUS_CHUNK_INDEX].base);
}

static inline __u32 __spif_chunk_size(struct ksus_desc_s *ksus_desc)
{
	return ksus_desc->chunk[SPIF_KSUS_CHUNK_INDEX].size;
}

static inline int __spif_init(__u64 chunk_size_order)
{
	int ret = E_HM_OK;
	struct ksus_desc_s *ksus_desc = get_ksus_desc();
	if (ksus_desc != NULL) {
		struct __pfev_fix16_desc_s *desc = __spif_desc(ksus_desc);
		struct __pfev_fix16_chunk_s *chunk = __spif_chunk(ksus_desc);
		ret = pfev_fix16_init(desc, chunk, chunk_size_order);
	} else {
		ret = E_HM_INVAL;
	}

	return ret;
}

#define SPIF_EVENTID_SHIFT		32
static inline __u64 __spif_event_create(__u64 eventid, __u64 eventdata)
{
	return (eventid << SPIF_EVENTID_SHIFT) | eventdata;
}

static inline __u64 __spif_event_extract_eventid(__u64 event)
{
	return event >> SPIF_EVENTID_SHIFT;
}

static inline __u64 __spif_event_extract_eventdata(__u64 event)
{
	return (__u64)((__u32)event);
}

#define SPIF_GROUP_INDEX_SHIFT		16
static inline __u64 __spif_eventid_create(__u64 spif_group_index, __u64 spif_patch_index)
{
	return ((spif_group_index & 0xffff) << SPIF_GROUP_INDEX_SHIFT) | (spif_patch_index & 0xffff);
}

#define __spif_eventid(spifgroup_name, spifid) \
	__spif_eventid_create(__paju_group_index(spifgroup_name), __paju_patch_index(spifgroup_name, spifid))

#define __spif_default_group spif_default_group

static inline void __spif_group(__u64 eventid)
{
	struct ksus_desc_s *ksus_desc = get_ksus_desc();
	if (likely(ksus_desc != NULL)) {
		struct __pfev_fix16_desc_s *desc = __spif_desc(ksus_desc);
		struct __pfev_fix16_chunk_s *chunk = __spif_chunk(ksus_desc);
		__u32 chunk_size = __spif_chunk_size(ksus_desc);
		__u64 timestamp = __spif_current_timestamp();
		__u64 event = __spif_event_create(eventid, 0);
		(void)pfev_fix16_u64x2_record(desc, chunk, chunk_size, timestamp, event);
	}
}

static inline int spif_init(__u64 chunk_size_order)
{
	return __spif_init(chunk_size_order);
}

#define spif_group_(spifgroup_name, spifid) \
	do { \
		if (pajugroup_false(spifgroup_name, spifid)) { \
			__u64 eventid = __spif_eventid(spifgroup_name, spifid); \
			__spif_group(eventid); \
		} \
	} while (0)
#if defined(NO_SPIF_GROUP)
#define spif_group(spifgroup_name, spifid)
#else

#ifdef __hmspif_enable__
#define spif_group(spifgroup_name, spifid) spif_group_(spifgroup_name, spifid)
#else
#define spif_group(spifgroup_name, spifid)
#endif

#endif

#define spif(spifid) spif_group(__spif_default_group, spifid)

#define __spif_trace_group(spifgroup_name, spifid) spif_group(spifgroup_name, spifid)
#define __spif_trace(spifid)			spif_group(__spif_default_group, spifid)

#ifndef __clang__
#define __spif_trace_group_scoped(group_name, point_name) \
	inline void __spif_trace_group_scoped_##group_name##_##point_name##_end(int *scoped_var __attribute__((unused))) { \
		__spif_trace_group(group_name, point_name##_end); \
	} \
	int __spif_trace_group_scoped_##group_name##_##point_name \
	__attribute__((cleanup(__spif_trace_group_scoped_##group_name##_##point_name##_end), unused)); \
	__spif_trace_group(group_name, point_name##_begin)
#else
#define __spif_trace_group_scoped(group_name, point_name)
#endif
#endif
