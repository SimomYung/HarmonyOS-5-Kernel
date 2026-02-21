/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Interfaces for malloc with types
 * Author: Huawei OS Kernel Lab
 * Create: Tue Dec 17 10:00:00 2024
 */
#ifndef INTAPI_MEM_MALLOC_TYPE_H
#define INTAPI_MEM_MALLOC_TYPE_H

#include <hmasm/page.h>
#include <libmem/slab_utils.h>
#include <intapi/mem/mcache.h>
#include <libmem/memm_slab.h>

#include <hmkernel/attributes/hmattributes.h>
__hmattr_tag(MEM);

#define __CONCAT(x, y)  x ## y
#define MALLOC_CONCAT(x, y) __CONCAT(x, y)
#define _MALLOC_ARGS_COUNT(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, N, ...) N
#define MALLOC_ARGS_COUNT(...) _MALLOC_ARGS_COUNT(, ##__VA_ARGS__, _9, _8, _7, _6, _5, _4, _3, _2, _1, _0)
#define _MALLOC_DISPATCH(macro_base, N, ...) __CONCAT(macro_base, N)(__VA_ARGS__)
#define MALLOC_DISPATCH(macro_base, ...) \
	_MALLOC_DISPATCH(macro_base, MALLOC_ARGS_COUNT(__VA_ARGS__), ##__VA_ARGS__)
#define _MALLOC_DISPATCH_R(macro_base, N, ...) __CONCAT(macro_base, N)(__VA_ARGS__)
#define MALLOC_DISPATCH_R(macro_base, ...) \
	_MALLOC_DISPATCH_R(macro_base, MALLOC_ARGS_COUNT(__VA_ARGS__), ##__VA_ARGS__)

#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

#if __has_builtin(__builtin_classify_type)
/* enum gcc_type_class, the value of pointer_type_class is 5. */
#define pointer_type_class 5
#define obj_is_ptr(obj) (__builtin_classify_type(obj) == pointer_type_class)
#else
#define obj_is_ptr(obj) (sizeof(obj) == sizeof(void *))
#endif

#define ptr_temp_obtain_and_erase(elem) ({					\
	_Static_assert(obj_is_ptr(elem), "elem is not pointer sized");		\
	__typeof__(elem) *elem_addr = &(elem);					\
	__typeof__(elem) tmp_elem = *elem_addr;					\
	*elem_addr = NULL;							\
	tmp_elem;								\
})

#define MAX_TYPE_VIEW_SLAB_SIZE (SLAB_MAX_LOWER_BOUND - 8)
#define BITS_PER_BYTE 8U

enum malloc_type_flags_s {
	MT_DEFAULT	= 0x0001,
	MT_DATA_ONLY	= 0x0002,
	MT_VM		= 0x0004,
	MT_PTR_ARRAY	= 0x0008
};

struct malloc_type_view_s {
	struct mcache_s *mcache;
	const char *mt_name;
	struct malloc_type_view_s *mt_next;
	const char *mt_signature;
	uint32_t mt_flags;
	uint32_t mt_size;
};

struct var_malloc_type_view_s {
	struct mcache_s **mcaches;
	const char *mt_name;
	struct var_malloc_type_view_s *mt_next;
	const char *mt_sig_hdr;
	const char *mt_sig_elem;
	uint32_t mt_flags;
	uint32_t mt_size_hdr;
	uint32_t mt_size_elem;
};

void *malloc_type_impl(struct malloc_type_view_s *mt_view, uint32_t flags);
void free_type_impl(struct malloc_type_view_s *mt_view, void *ptr);
void *malloc_var_type_impl(struct var_malloc_type_view_s *mt_view, size_t size, uint32_t flags);
void free_var_type_impl(struct var_malloc_type_view_s *mt_view, void *ptr, size_t size, size_t count);

enum mt_type {
    MALLOC_TYPE_PADDING = 0,
    MALLOC_TYPE_POINTER = 1,
    MALLOC_TYPE_DATA    = 2,
    MALLOC_TYPE_CTRL    = 4,
    MALLOC_TYPE_PAC     = 8
};

#define MALLOC_TYPE_SUMMARY_MASK_BITS (0xffff)

#define MALLOC_TYPE_SUMMARY_MASK_DATA					\
	(1 << MALLOC_TYPE_PADDING | 1 << MALLOC_TYPE_DATA)

#define MALLOC_TYPE_SUMMARY_MASK_POINTER				\
	(1 << MALLOC_TYPE_PADDING | 1 << MALLOC_TYPE_POINTER | 1 << MALLOC_TYPE_PAC)

#define MALLOC_TYPE_SUMMARY_MASK_ALL					\
	(1 << MALLOC_TYPE_PADDING | 1 << MALLOC_TYPE_POINTER | 1 << MALLOC_TYPE_DATA | \
	 1 << MALLOC_TYPE_CTRL | 1 << MALLOC_TYPE_PAC)

#define MALLOC_TYPE_SUMMARY(type)					\
	(__builtin_hm_type_summary(type) & MALLOC_TYPE_SUMMARY_MASK_BITS)

#define MALLOC_TYPE_SIG_CHECK(mask, type)				\
	((MALLOC_TYPE_SUMMARY(type) & ~(mask)) == 0)

#define MALLOC_TYPE_IS_DATA_ONLY(type)					\
	MALLOC_TYPE_SIG_CHECK(MALLOC_TYPE_SUMMARY_MASK_DATA, type)

#define MALLOC_TYPE_IS_POINTER_ONLY(type)				\
	MALLOC_TYPE_SIG_CHECK(MALLOC_TYPE_SUMMARY_MASK_POINTER, type)

#define MALLOC_TYPE_CHECK_2(checker, type) (MALLOC_TYPE_SIG_CHECK(checker, type))

#define MALLOC_TYPE_CHECK_3(checker, type1, type2) 			\
	(MALLOC_TYPE_SIG_CHECK(checker, type1) &&			\
	 MALLOC_TYPE_SIG_CHECK(checker, type2))

#define MALLOC_TYPE_CHECK(...) MALLOC_DISPATCH_R(MALLOC_TYPE_CHECK, ##__VA_ARGS__)

#define MALLOC_TYPE_VM_SIZE_CHECK_1(type) (sizeof(type) > MAX_TYPE_VIEW_SLAB_SIZE)

#define MALLOC_TYPE_VM_SIZE_CHECK_2(type1, type2)			\
	((sizeof(type1) + sizeof(type2) > MAX_TYPE_VIEW_SLAB_SIZE))

#define MALLOC_TYPE_VM_SIZE_CHECK(...) MALLOC_DISPATCH_R(MALLOC_TYPE_VM_SIZE_CHECK, ##__VA_ARGS__)

#if !__has_builtin(__builtin_hm_type_signature) || !__has_builtin(__builtin_hm_type_summary)
#define __builtin_hm_type_signature(type) #type
#define __builtin_hm_type_summary(type) MALLOC_TYPE_SUMMARY_MASK_ALL
#endif

/*
 * generate signature only when type is not data only and not too large.
 */
#define MALLOC_TYPE_EMIT_SIG(type, ...)							\
	(MALLOC_TYPE_CHECK(MALLOC_TYPE_SUMMARY_MASK_DATA, type, ##__VA_ARGS__) ||	\
	MALLOC_TYPE_VM_SIZE_CHECK(type, ##__VA_ARGS__)) ?				\
	"" : __builtin_hm_type_signature(type)

/* malloc type flagss are adjusted to indicate if the type is "data-only" or "vm" or "pointer array" */
#define MALLOC_TYPE_ADJUST_FLAGS(flags, ...)							\
	((flags) |							\
	(MALLOC_TYPE_CHECK(MALLOC_TYPE_SUMMARY_MASK_DATA, __VA_ARGS__) ? MT_DATA_ONLY : 0) |	\
	(MALLOC_TYPE_CHECK(MALLOC_TYPE_SUMMARY_MASK_POINTER, __VA_ARGS__) ? MT_PTR_ARRAY : 0) |	\
	(MALLOC_TYPE_VM_SIZE_CHECK(__VA_ARGS__) ? MT_VM : 0))

/* type size cannot exceed MAX_TYPE_VIEW_SLAB_SIZE when malloc with type */
#define MALLOC_TYPE_SIZE_CHECK(size)					\
	_Static_assert(size <= MAX_TYPE_VIEW_SLAB_SIZE, "type is too large")

/* pointer type compatible check */
#define MALLOC_TYPE_IS_COMPATIBLE_POINTER(ptr, type)			\
	(__builtin_types_compatible_p(__typeof__(*ptr), type) ||	\
	__builtin_types_compatible_p(__typeof__(*ptr), void))

#define MALLOC_TYPE_ASSERT_COMPATIBLE_POINTER(ptr, type)		\
	_Static_assert(MALLOC_TYPE_IS_COMPATIBLE_POINTER(ptr, type),	\
	"pointer type is not compatible with expected type")

/* Define fixed sized malloc_type_view variable */
#define _MALLOC_TYPE_FIXED_DEFINE(var, type, flags)			\
	__attribute__((used, section("fixed_type_view_tab")))		\
	struct malloc_type_view_s var[1] = {{				\
		.mt_name = "type." #type,				\
		.mt_flags = MALLOC_TYPE_ADJUST_FLAGS(flags, type),	\
		.mt_size = sizeof(type),				\
		.mt_signature = MALLOC_TYPE_EMIT_SIG(type),		\
	}};								\
	MALLOC_TYPE_SIZE_CHECK(sizeof(type));

/* Define variable size */
#define VAR_TYPE_SIZE_LIMIT_SIZE	(1ull << (BITS_PER_BYTE * sizeof(size_t) - 1))
static inline size_t var_type_size(size_t hdr_size, size_t elem_size, size_t count)
{
	size_t res;
	/* malloc such size will return NULL */
	if (__builtin_mul_overflow(elem_size, count, &res)) {
		return VAR_TYPE_SIZE_LIMIT_SIZE;
	}
	if (__builtin_add_overflow(hdr_size, res, &res)) {
		return VAR_TYPE_SIZE_LIMIT_SIZE;
	}
	return (((res & VAR_TYPE_SIZE_LIMIT_SIZE) != 0) ? VAR_TYPE_SIZE_LIMIT_SIZE : res);
}

/* Define specific variable name in source file */
#define malloc_type_view MALLOC_CONCAT(malloc_type_view_, __LINE__)

#define malloc_type_2(type, flags) ({					\
	static _MALLOC_TYPE_FIXED_DEFINE(malloc_type_view, type, 0);	\
	(type *)malloc_type_impl(malloc_type_view, flags);		\
})

#define free_type_2(type, elem) ({						\
	MALLOC_TYPE_ASSERT_COMPATIBLE_POINTER(elem, type);			\
	static _MALLOC_TYPE_FIXED_DEFINE(malloc_type_view, type, 0);		\
	free_type_impl(malloc_type_view, ptr_temp_obtain_and_erase(elem));	\
})

/* Define specific variable name in source file */
#define var_malloc_type_view MALLOC_CONCAT(var_malloc_type_view_, __LINE__)

/* Define flexible sized malloc_type_view variable */
#define _MALLOC_TYPE_VAR_DEFINE_3(var, elem_type, flags)		\
	__attribute__((used, section("var_type_view_tab")))		\
	struct var_malloc_type_view_s var[1] = {{			\
		.mt_name = "type." #elem_type,				\
		.mt_flags = MALLOC_TYPE_ADJUST_FLAGS(flags, elem_type),	\
		.mt_size_hdr = 0,					\
		.mt_sig_hdr = NULL,					\
		.mt_size_elem = sizeof(elem_type),			\
		.mt_sig_elem = MALLOC_TYPE_EMIT_SIG(elem_type),		\
	}};								\
	MALLOC_TYPE_SIZE_CHECK(sizeof(elem_type));

#define malloc_type_3(elem_type, count, flags) ({				\
	static _MALLOC_TYPE_VAR_DEFINE_3(var_malloc_type_view, elem_type, 0);	\
	(elem_type *)malloc_var_type_impl(var_malloc_type_view,			\
		var_type_size(0, sizeof(elem_type), count), flags);		\
})

#define free_type_3(elem_type, count, ptr) ({					\
	MALLOC_TYPE_ASSERT_COMPATIBLE_POINTER(ptr, elem_type);			\
	static _MALLOC_TYPE_VAR_DEFINE_3(var_malloc_type_view, elem_type, 0);	\
	free_var_type_impl(var_malloc_type_view, ptr_temp_obtain_and_erase(ptr),\
		var_type_size(0, sizeof(elem_type), count), (size_t)(count));	\
})

/* Define flexible sized malloc_type_view variable */
#define _MALLOC_TYPE_VAR_DEFINE_4(var, hdr_type, elem_type, flags)			\
	__attribute__((used, section("var_type_view_tab")))				\
	struct var_malloc_type_view_s var[1] = {{					\
		.mt_name = "type." #hdr_type "." #elem_type,				\
		.mt_flags = MALLOC_TYPE_ADJUST_FLAGS(flags, hdr_type, elem_type),	\
		.mt_size_hdr = sizeof(hdr_type),					\
		.mt_sig_hdr = MALLOC_TYPE_EMIT_SIG(hdr_type),				\
		.mt_size_elem = sizeof(elem_type),					\
		.mt_sig_elem = MALLOC_TYPE_EMIT_SIG(elem_type),				\
	}};										\
	MALLOC_TYPE_SIZE_CHECK(sizeof(hdr_type));					\
	MALLOC_TYPE_SIZE_CHECK(sizeof(elem_type));

#define malloc_type_4(hdr_type, elem_type, count, flags) ({				\
	static _MALLOC_TYPE_VAR_DEFINE_4(var_malloc_type_view, hdr_type, elem_type, 0);	\
	(hdr_type *)malloc_var_type_impl(var_malloc_type_view,				\
		var_type_size(sizeof(hdr_type), sizeof(elem_type), count), flags);	\
})

#define free_type_4(hdr_type, elem_type, count, ptr) ({						\
	MALLOC_TYPE_ASSERT_COMPATIBLE_POINTER(ptr, hdr_type);					\
	static _MALLOC_TYPE_VAR_DEFINE_4(var_malloc_type_view, hdr_type, elem_type, 0);		\
	free_var_type_impl(var_malloc_type_view, ptr_temp_obtain_and_erase(ptr),		\
		var_type_size(sizeof(hdr_type), sizeof(elem_type), count), (size_t)(count));	\
})

/*
 * 1. malloc_type(type, flags)
 *    Use this macro for fixed sized allocation of a specific type
 * 2. malloc_type(elem_type, count, flags)
 *    Use this macro for variable sized allocations of a object array.
 *    malloc_type(elem_type, 1, flags) is not equal to malloc_type(elem_type, flags)
 * 3. malloc_type(hdr_type, elem_type, count, flags)
 *    Use this macro for variable sized allocations of a object containing zero-length array.
 */
#define malloc_type(...)  MALLOC_DISPATCH(malloc_type, ##__VA_ARGS__)

/*
 * 1. free_type(type, elem)
 * 2. free_type(elem_type, count, ptr)
 * 3. free_type(hdr_type, elem_type, count, ptr)
 */
#define free_type(...)  MALLOC_DISPATCH(free_type, ##__VA_ARGS__)
#endif /* INTAPI_MEM_ALLOC_TYPE_H */
