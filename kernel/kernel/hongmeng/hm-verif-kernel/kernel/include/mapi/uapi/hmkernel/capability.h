/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Capability
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 12 08:14:44 2018
 */
#ifndef UAPI_HMKERNEL_CAPABILITY_H
#define UAPI_HMKERNEL_CAPABILITY_H

#include <hmkernel/types.h>
#include <hmkernel/strict.h>
#include <hmkernel/compiler.h>

enum hmobj {
	hmobj_NULL = 0,
#define CAPTYPE(name, num, grant, method)       hmobj_##name = num,
#include <hmkernel/capability/captypes.h>
#undef CAPTYPE
	hmobj_MAX,
};

static inline __always_inline enum hmobj
enum_hmobj_from_uint(unsigned int captype)
{
	enum hmobj __ret = hmobj_MAX;
	switch (captype) {
	case (unsigned int)hmobj_NULL:
		__ret = hmobj_NULL;
		break;
#define CAPTYPE(name, num, grant, method)       \
	case (unsigned int)hmobj_##name:        \
		__ret = hmobj_##name;           \
		break;
#include <hmkernel/capability/captypes.h>
#undef CAPTYPE
	default:
		break;
	}

	return __ret;
}

static inline unsigned int
enum_hmobj_to_uint(enum hmobj captype)
{
	return (unsigned int)captype;
}

/* following 3 types are intentionally to be injected into user's
 * namespace, so remove the '__' prefix. */
typedef unsigned long long cref_t;
typedef unsigned long long uref_t;
typedef unsigned long long rref_t;

typedef union {
	cref_t cref;
	uref_t uref;
	rref_t rref;
} xref_t;

/* kernel object size < PAGE_SIZE */
#define __CAP_ALIGN_SMALL_KOBJ(size) (__builtin_popcountl((unsigned long)size) > 1 ? \
		1UL << (sizeof(size_t) * 8UL - (size_t)(unsigned int)__builtin_clzl((unsigned long)size)) :\
		(size))

/* kernel object size >= PAGE_SIZE */
#define __CAP_ALIGN_BIG_KOBJ(size) (((size) + (unsigned long)PAGE_SIZE - 1UL)&(~((unsigned long)PAGE_SIZE - 1UL)))

/*
 * Align kernel objects:
 * 1.for kernel objects whose size is smaller than PAGE_SIZE, align their size to
 *   the n-th power of 2 bytes;
 * 2.for kernel objects whose size is larger than or equal to PAGE_SIZE, align their
 *   size to an integer multiple of PAGE_SIZE;
 */
#define __CAP_ALIGN_KOBJ(size) ((size) < PAGE_SIZE ? \
		__CAP_ALIGN_SMALL_KOBJ(size) : __CAP_ALIGN_BIG_KOBJ(size))

#define PRIxref __PRIx64

#define __CAP_BITS		5U
#define __CAP_RIGHTS_BITS	8U
#define __CAP_RIGHTS_ALL	((__u8)((1U << __CAP_RIGHTS_BITS) - 1U))

#define __REF_CNODE_IDX_OF(ref)	(__u32)(((__u64)(ref) >> 32) & 0x7FFFFFFFUL)
#define __REF_INNER_IDX_OF(ref)	(__u32)((__u64)(ref) & 0xFFFFFFFFUL)
#define __REF_IS_RREF(ref)	____IS((((__u64)(ref) >> 63) & 0x1UL))

#define CNODE_ITERATE_STATE_BITS	3
#define CNODE_ITERATE_STATE_ALL		((1U << CNODE_ITERATE_STATE_BITS) - 1U)
#define CNODE_IDX_INVALD_ZERO		0U

/*
 * As cnode_idx_tbl dirmap has 5 bits unused,
 * 0x7fffffff maybe a valid cnode idx,
 * it's not used now.
 */
#define CNODE_IDX_VALID_UNUSED		0x7fffffff

#define CAP_EXPORT_API
#define CAPTYPE(name, num, grant, method)
#include <hmkernel/capability/captypes.h>
#undef CAPTYPE
#undef CAP_EXPORT_API

#define CAPMETHOD(prefix, label)        prefix##label,
#define CAPERRNO(type, perrno, ename, desc)
#define CAPTYPE(name, num, grant, methods)      \
	enum name##_method {		\
		name##Invalid = 0,      \
		methods                 \
		NR_##name##_Methods     \
	};
#include <hmkernel/capability/captypes.h>
#undef CAPTYPE
#undef CAPERRNO
#undef CAPMETHOD

#endif
