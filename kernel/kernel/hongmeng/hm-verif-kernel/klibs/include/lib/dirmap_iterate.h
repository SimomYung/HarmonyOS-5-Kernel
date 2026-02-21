/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Dirmap iterate
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 27 10:00:02 2020
 */
#ifndef KLIBS_DIRMAP_ITERATE_H
#define KLIBS_DIRMAP_ITERATE_H

#ifndef GEN_DIRMAP_ITERATE_API
# error "This header should be used by dirmap.h"
#endif

#define __DECLARE_dirmap_for_each_slot_lv_n(suffix, nlv_pgsb, nlv_sltsb, lv, nlv,	\
					    slot_type, func, call, proto, args)		\
static inline int									\
__dirmap_foreach_##suffix##_##func##_lv##lv(const struct __dirmap_##suffix *map, const void *slot, u32 idx proto)\
{											\
	int ret = 0;									\
	unsigned int nlv_page_sz;							\
	unsigned int nlv_slot_sz;							\
	unsigned int shift = (unsigned int)((__dirmap_##suffix##_shift_lv##nlv()) - __dirmap_##suffix##_unused_bits());\
	struct __dirmap_nlv_slot {							\
		u64 header;								\
		u8 data[(1U << (nlv_sltsb == 0U ? 3U : nlv_sltsb))  - sizeof(u64)];	\
	} __attribute__((packed));							\
	struct __dirmap_nlv_slot *nlv_page;						\
	struct __dirmap_nlv_slot *nlv_slot;						\
	struct __dirmap_nlv_slot *end;							\
											\
	if (nlv_pgsb == 0U) {								\
		ret = call(map, (slot_type *)(uptr_t)slot, idx args);			\
	} else {									\
		nlv_page_sz = 1U << (nlv_pgsb);						\
		nlv_slot_sz = 1U << (nlv_sltsb);					\
		nlv_page = (struct __dirmap_nlv_slot *)(uptr_t)(*((u64 *)(uptr_t)slot));\
		nlv_slot = &nlv_page[__dirmap_##suffix##_decode_lv##nlv(idx << __dirmap_##suffix##_unused_bits())];\
		end = &nlv_page[nlv_page_sz / nlv_slot_sz];				\
		if ((void *)nlv_slot != (void *)&map->top_level) {			\
			if ((void *)nlv_slot == (void *)(&nlv_page[0])) {		\
				nlv_slot++;						\
				idx = ((idx >> shift) + 1U) << shift;			\
			}								\
		}									\
		while (nlv_slot < end) {						\
			u64 slot_val = nlv_slot->header;				\
											\
			if ((slot_val & DIRMAP_SLOT_BIT_FREE) == 0U) {			\
				int err;						\
											\
				err = call(map, (slot_type *)(uptr_t)(nlv_slot), idx args);\
				if (err < 0) {						\
					ret = err;					\
					break;						\
				}							\
			}								\
			nlv_slot++;							\
			idx = ((idx >> shift) + 1U) << shift;				\
		}									\
	}										\
											\
	return ret;									\
}

#define DECLARE_dirmap_for_each_slot(suffix, func, proto, args)			\
	static inline int									\
	dirmap_foreach_##suffix##_##func(const struct __dirmap_##suffix *map proto)			\
	{											\
		u64 ptr;									\
		ptr = (u64)(uptr_t)&map->top_level;						\
		return __dirmap_foreach_##suffix##_##func##_lv_start(map, &ptr, 0U args);	\
	}											\
	static inline int									\
	dirmap_foreach_from_##suffix##_##func(const struct __dirmap_##suffix *map, u32 from proto)	\
	{											\
		u64 ptr;									\
		ptr = (u64)(uptr_t)&map->top_level;						\
		return __dirmap_foreach_##suffix##_##func##_lv_start(map, &ptr, from args);	\
	}

#define __DIRMAP_PROTO(...)	, ##__VA_ARGS__
#define __DIRMAP_ARGS(...)	, ##__VA_ARGS__
#define DIRMAP_PROTO(...)	__VA_ARGS__
#define DIRMAP_ARGS(...)	__VA_ARGS__

#define __DECLARE_DIRMAP_FOREACH(suffix, pgsb4, sltsb4, pgsb3, sltsb3, pgsb2, sltsb2, 	\
				 pgsb1, sltsb1, pgsb0, sltsb0, type, func, proto, args)	\
__DECLARE_dirmap_for_each_slot_lv_n(suffix, pgsb0, sltsb0, 1, 0, type,		\
				    func, func,					\
				    __DIRMAP_PROTO(proto), __DIRMAP_ARGS(args))	\
__DECLARE_dirmap_for_each_slot_lv_n(suffix, pgsb1, sltsb1, 2, 1, void,		\
				    func, __dirmap_foreach_##suffix##_##func##_lv1,\
				    __DIRMAP_PROTO(proto), __DIRMAP_ARGS(args))	\
__DECLARE_dirmap_for_each_slot_lv_n(suffix, pgsb2, sltsb2, 3, 2, void,		\
				    func, __dirmap_foreach_##suffix##_##func##_lv2,\
				    __DIRMAP_PROTO(proto), __DIRMAP_ARGS(args))	\
__DECLARE_dirmap_for_each_slot_lv_n(suffix, pgsb3, sltsb3, 4, 3, void,		\
				    func, __dirmap_foreach_##suffix##_##func##_lv3,\
				    __DIRMAP_PROTO(proto), __DIRMAP_ARGS(args))	\
__DECLARE_dirmap_for_each_slot_lv_n(suffix, pgsb4, sltsb4, _top, 4, void,	\
				    func, __dirmap_foreach_##suffix##_##func##_lv4,\
				    __DIRMAP_PROTO(proto), __DIRMAP_ARGS(args))	\
__DECLARE_dirmap_for_each_slot_lv_n(suffix, 3U, 3U, _start, _top, void,		\
				    func, __dirmap_foreach_##suffix##_##func##_lv_top,\
				    __DIRMAP_PROTO(proto), __DIRMAP_ARGS(args))	\
DECLARE_dirmap_for_each_slot(suffix, func, __DIRMAP_PROTO(proto), __DIRMAP_ARGS(args))


#define __DECLARE_dirmap_iterate_iterator(suffix, type)				\
	static inline int __dirmap_##suffix##_iterate_start(const struct __dirmap_##suffix *map,\
							     const type *slot, u32 idx,	\
							     u32 *pidx)\
	{											\
		UNUSED(map, slot);								\
		*pidx = idx;									\
		return -1;									\
	}											\
	static inline int __dirmap_##suffix##_iterate_next(const struct __dirmap_##suffix *map,	\
							    const type *slot, u32 idx,		\
							    u32 start_idx,			\
							    u32 *pidx)				\
	{											\
		UNUSED(map, slot);								\
		int ret = 0;									\
		if (idx != start_idx) {								\
			*pidx = idx;								\
			ret = -1;								\
		}										\
		return ret;									\
	}

#define DECLARE_DIRMAP_ITERATE(suffix)								\
	static inline int dirmap_##suffix##_iterate_start(const struct __dirmap_##suffix *map, u32 *pidx)\
	{											\
		u32 first_idx;									\
		int err, ret;									\
												\
		err = dirmap_foreach_##suffix##___dirmap_##suffix##_iterate_start(map, &first_idx);\
		if (err == 0) {									\
			ret = -1;								\
		} else {									\
			if (pidx != NULL) {							\
				*pidx = first_idx;						\
			}									\
			ret = 0;								\
		}										\
												\
		return ret;									\
	}											\
	static inline int dirmap_##suffix##_iterate_next(const struct __dirmap_##suffix *map, u32 idx, u32 *pidx)\
	{											\
		u32 next_idx;									\
		int err, ret;									\
		\
		err = dirmap_foreach_from_##suffix##___dirmap_##suffix##_iterate_next(map, idx, idx, &next_idx);\
		if (err == 0) {									\
			ret = -1;								\
		} else {									\
			if (pidx != NULL) {							\
				*pidx = next_idx;						\
			}									\
			ret = 0;								\
		}										\
												\
		return ret;									\
	}

#define __DECLARE_DIRMAP_ITERATE(suffix, pgsb4, sltsb4, pgsb3, sltsb3, pgsb2, sltsb2,	\
				 pgsb1, sltsb1, pgsb0, sltsb0, type)			\
__DECLARE_dirmap_iterate_iterator(suffix, type)						\
__DECLARE_DIRMAP_FOREACH(suffix, pgsb4, sltsb4, pgsb3, sltsb3, pgsb2, sltsb2,		\
			 pgsb1, sltsb1, pgsb0, sltsb0, type, __dirmap_##suffix##_iterate_start,\
			 DIRMAP_PROTO(u32 *pidx), DIRMAP_ARGS(pidx))			\
__DECLARE_DIRMAP_FOREACH(suffix, pgsb4, sltsb4, pgsb3, sltsb3, pgsb2, sltsb2,		\
			 pgsb1, sltsb1, pgsb0, sltsb0, type, __dirmap_##suffix##_iterate_next,\
			 DIRMAP_PROTO(u32 start_idx, u32 *pidx), DIRMAP_ARGS(start_idx, pidx))\
DECLARE_DIRMAP_ITERATE(suffix)

#define __DECLARE_dirmap_cleanup_lv0(suffix)							\
	static inline void __dirmap_##suffix##_cleanup_lv0(					\
		const struct __dirmap_##suffix *map, const void *slot, u32 idx)			\
	{											\
		UNUSED(map, slot, idx);								\
	}

#define __DECLARE_dirmap_cleanup_lv_n(suffix, nlv_pgsb, nlv_sltsb, lv, nlv)			\
	static inline void __dirmap_##suffix##_cleanup_lv##lv(					\
		const struct __dirmap_##suffix *map, void *slot, u32 idx)			\
	{											\
		struct __dirmap_nlv_slot {							\
			u64 header;								\
			u8 data[(1U << (nlv_sltsb == 0U ? 3U : nlv_sltsb))  - sizeof(u64)];	\
		} __attribute__((packed));							\
		unsigned int nlv_page_sz;							\
		unsigned int nlv_slot_sz;							\
		unsigned int shift;								\
		struct __dirmap_nlv_slot *nlv_page = NULL;					\
		struct __dirmap_nlv_slot *nlv_slot = NULL;					\
		struct __dirmap_nlv_slot *end = NULL;						\
												\
		if ((nlv_pgsb) != 0U) {								\
			nlv_page = (struct __dirmap_nlv_slot *)(uptr_t)(*((u64 *)(uptr_t)slot)); \
			nlv_page_sz = 1U << (nlv_pgsb);						\
			nlv_slot_sz = 1U << (nlv_sltsb);					\
			end = &nlv_page[nlv_page_sz / nlv_slot_sz];				\
			nlv_slot = &nlv_page[__dirmap_##suffix##_decode_lv##nlv(idx << __dirmap_##suffix##_unused_bits())]; \
			shift = (unsigned int)((__dirmap_##suffix##_shift_lv##nlv()) - __dirmap_##suffix##_unused_bits()); \
			if (((void *)nlv_slot != (void *)&map->top_level) &&			\
			    ((void *)nlv_slot == (void *)(&nlv_page[0]))) {			\
				nlv_slot++;							\
				idx = ((idx >> shift) + 1U) << shift;				\
			}									\
			while (nlv_slot < end) {						\
				if ((nlv_slot->header & DIRMAP_SLOT_BIT_FREE) == 0U) {		\
					__dirmap_##suffix##_cleanup_lv##nlv(map, nlv_slot, idx); \
				}								\
				nlv_slot++;							\
				idx = ((idx >> shift) + 1U) << shift;				\
			}									\
			mem_zero_b((void *)(uptr_t)(*(u64 *)slot), nlv_page_sz);		\
		} else {									\
			__dirmap_##suffix##_cleanup_lv##nlv(map, slot, idx);			\
		}										\
	}

#define DECLARE_DIRMAP_CLEANUP(suffix)								\
	static inline void dirmap_##suffix##_cleanup(const struct __dirmap_##suffix *map)	\
	{											\
		u64 ptr;									\
		ptr = (u64)(uptr_t)&map->top_level;						\
		__dirmap_##suffix##_cleanup_lv_start(map, &ptr, 0U);				\
	}

#define __DECLARE_DIRMAP_CLEANUP(suffix, pgsb4, sltsb4, pgsb3, sltsb3, pgsb2, sltsb2,	\
				 pgsb1, sltsb1, pgsb0, sltsb0)				\
__DECLARE_dirmap_cleanup_lv0(suffix)							\
__DECLARE_dirmap_cleanup_lv_n(suffix, pgsb0, sltsb0, 1, 0)				\
__DECLARE_dirmap_cleanup_lv_n(suffix, pgsb1, sltsb1, 2, 1)				\
__DECLARE_dirmap_cleanup_lv_n(suffix, pgsb2, sltsb2, 3, 2)				\
__DECLARE_dirmap_cleanup_lv_n(suffix, pgsb3, sltsb3, 4, 3)				\
__DECLARE_dirmap_cleanup_lv_n(suffix, pgsb4, sltsb4, _top, 4)				\
__DECLARE_dirmap_cleanup_lv_n(suffix, 3U, 3U, _start, _top)				\
DECLARE_DIRMAP_CLEANUP(suffix)

#endif
