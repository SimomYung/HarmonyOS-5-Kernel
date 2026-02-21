/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: Common macro for kernel that need to be exported to modules.
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 08 10:41:52 2019
 */
#ifndef MAPI_HMKERNEL_KERNEL_H
#define MAPI_HMKERNEL_KERNEL_H

#include <hmkernel/types.h>
#include <hmkernel/panic.h>
#include <hmkernel/const.h>

#define __ENUM_MAP0(m, ...)
#define __ENUM_MAP1(m, prefix, a, ...) m(prefix, a)
#define __ENUM_MAP2(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP1(m, prefix, __VA_ARGS__)
#define __ENUM_MAP3(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP2(m, prefix, __VA_ARGS__)
#define __ENUM_MAP4(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP3(m, prefix, __VA_ARGS__)
#define __ENUM_MAP5(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP4(m, prefix, __VA_ARGS__)
#define __ENUM_MAP6(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP5(m, prefix, __VA_ARGS__)
#define __ENUM_MAP7(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP6(m, prefix, __VA_ARGS__)
#define __ENUM_MAP8(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP7(m, prefix, __VA_ARGS__)
#define __ENUM_MAP9(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP8(m, prefix, __VA_ARGS__)
#define __ENUM_MAP10(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP9(m, prefix, __VA_ARGS__)
#define __ENUM_MAP11(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP10(m, prefix, __VA_ARGS__)
#define __ENUM_MAP12(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP11(m, prefix, __VA_ARGS__)
#define __ENUM_MAP13(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP12(m, prefix, __VA_ARGS__)
#define __ENUM_MAP14(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP13(m, prefix, __VA_ARGS__)
#define __ENUM_MAP15(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP14(m, prefix, __VA_ARGS__)
#define __ENUM_MAP16(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP15(m, prefix, __VA_ARGS__)
#define __ENUM_MAP17(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP16(m, prefix, __VA_ARGS__)
#define __ENUM_MAP18(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP17(m, prefix, __VA_ARGS__)
#define __ENUM_MAP19(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP18(m, prefix, __VA_ARGS__)
#define __ENUM_MAP20(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP19(m, prefix, __VA_ARGS__)
#define __ENUM_MAP21(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP20(m, prefix, __VA_ARGS__)
#define __ENUM_MAP22(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP21(m, prefix, __VA_ARGS__)
#define __ENUM_MAP23(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP22(m, prefix, __VA_ARGS__)
#define __ENUM_MAP24(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP23(m, prefix, __VA_ARGS__)
#define __ENUM_MAP25(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP24(m, prefix, __VA_ARGS__)
#define __ENUM_MAP26(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP25(m, prefix, __VA_ARGS__)
#define __ENUM_MAP27(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP26(m, prefix, __VA_ARGS__)
#define __ENUM_MAP28(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP27(m, prefix, __VA_ARGS__)
#define __ENUM_MAP29(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP28(m, prefix, __VA_ARGS__)
#define __ENUM_MAP30(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP29(m, prefix, __VA_ARGS__)
#define __ENUM_MAP31(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP30(m, prefix, __VA_ARGS__)
#define __ENUM_MAP32(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP31(m, prefix, __VA_ARGS__)
#define __ENUM_MAP33(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP32(m, prefix, __VA_ARGS__)
#define __ENUM_MAP34(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP33(m, prefix, __VA_ARGS__)
#define __ENUM_MAP35(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP34(m, prefix, __VA_ARGS__)
#define __ENUM_MAP36(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP35(m, prefix, __VA_ARGS__)
#define __ENUM_MAP37(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP36(m, prefix, __VA_ARGS__)
#define __ENUM_MAP38(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP37(m, prefix, __VA_ARGS__)
#define __ENUM_MAP39(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP38(m, prefix, __VA_ARGS__)
#define __ENUM_MAP40(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP39(m, prefix, __VA_ARGS__)
#define __ENUM_MAP41(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP40(m, prefix, __VA_ARGS__)
#define __ENUM_MAP42(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP41(m, prefix, __VA_ARGS__)
#define __ENUM_MAP43(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP42(m, prefix, __VA_ARGS__)
#define __ENUM_MAP44(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP43(m, prefix, __VA_ARGS__)
#define __ENUM_MAP45(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP44(m, prefix, __VA_ARGS__)
#define __ENUM_MAP46(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP45(m, prefix, __VA_ARGS__)
#define __ENUM_MAP47(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP46(m, prefix, __VA_ARGS__)
#define __ENUM_MAP48(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP47(m, prefix, __VA_ARGS__)
#define __ENUM_MAP49(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP48(m, prefix, __VA_ARGS__)
#define __ENUM_MAP50(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP49(m, prefix, __VA_ARGS__)
#define __ENUM_MAP51(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP50(m, prefix, __VA_ARGS__)
#define __ENUM_MAP52(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP51(m, prefix, __VA_ARGS__)
#define __ENUM_MAP53(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP52(m, prefix, __VA_ARGS__)
#define __ENUM_MAP54(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP53(m, prefix, __VA_ARGS__)
#define __ENUM_MAP55(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP54(m, prefix, __VA_ARGS__)
#define __ENUM_MAP56(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP55(m, prefix, __VA_ARGS__)
#define __ENUM_MAP57(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP56(m, prefix, __VA_ARGS__)
#define __ENUM_MAP58(m, prefix, a, ...) m(prefix, a)	__ENUM_MAP57(m, prefix, __VA_ARGS__)
#define __ENUM_MAP(n, ...) __ENUM_MAP##n(__VA_ARGS__)

#define __ENUM_DEF_ONE(prefix, a)	prefix##a,

/*
 * Genereate enum_xxx_from/to_uint for misra requirement.
 * Directly convert int to enum is not allowed.
 *
 * In a very special case like utable_node and rtable_node, we need to control
 * which one use 'zero', so INVALID should be the last one.
 */
#define __ENUM_DEF(n, name, prefix, ...)	\
	enum name {				\
		__ENUM_MAP(n, __ENUM_DEF_ONE, prefix, __VA_ARGS__) \
		prefix##INVALID,		\
	};					\
	enum enum_##name##_nr {			\
		prefix##NR = n,	\
	};

#define __ENUM_FROM_UINT_ONE(prefix, a)		\
	case (unsigned int)(prefix##a): {	\
		ret = prefix##a;		\
		break;				\
	}

#define __ENUM_FROM_UINT(n, name, prefix, ...)			\
static inline enum name enum_##name##_from_uint(unsigned int v)	\
{								\
	enum name ret = prefix##INVALID;			\
								\
	switch(v) {						\
		__ENUM_MAP(n, __ENUM_FROM_UINT_ONE, prefix, __VA_ARGS__) \
		case (unsigned int)(prefix##INVALID): {		\
			break;					\
		}						\
		default: {					\
			break;					\
		}						\
	}							\
	return ret;						\
}

#define __ENUM_TO_UINT_ONE(prefix, a)		\
	case (prefix##a): {			\
		ret = (unsigned int)prefix##a;	\
		break;				\
	}

#define __ENUM_TO_UINT(n, name, prefix, ...)			\
static inline unsigned int enum_##name##_to_uint(enum name v)	\
{								\
	unsigned int ret = (unsigned int)prefix##INVALID;	\
								\
	switch(v) {						\
		__ENUM_MAP(n, __ENUM_TO_UINT_ONE, prefix, __VA_ARGS__) \
		case (prefix##INVALID): {			\
			break;					\
		}						\
		default: {					\
			break;					\
		}						\
	}							\
	return ret;						\
}

#define __ENUM_TO_STRING_ONE(prefix, a)		\
	#prefix #a,

/*
 * Do not use switch-case to create __ENUM_TO_STRING like enum_to_uint:
 * when enum is large, gcc is not able to use the most efficient way to implement
 * switch-case _to_string function. Give GCC a hint about it.
 * Tried in populate_pgstrv error output. When pgstr type has 10 entries in the
 * ENUM type, switch to array-based to_string function reduce code size from
 * ~0x1000 to ~0xc58
 */
#define __ENUM_TO_STRING(n, name, prefix, ...)			\
static inline const char *enum_##name##_to_string(enum name v)	\
{								\
	const char *ret = #prefix "INVALID";			\
								\
	const char *const array[n] = {				\
		__ENUM_MAP(n, __ENUM_TO_STRING_ONE, prefix, __VA_ARGS__) \
	};							\
								\
	unsigned int i = enum_##name##_to_uint(v);		\
	if (i < n) {						\
		ret = array[i];					\
	}							\
	return ret;						\
}

/* Make sure the enum type have same size as int */
#define __ENUM_NR_STRUCT(n, name, prefix, ...)			\
	struct ____enum_##prefix##nr_s { unsigned int array[n]; };

/* ENUM(num, enum_name, prefix, value_body_name) */
#define ENUM(...)			\
	__ENUM_DEF(__VA_ARGS__)		\
	__ENUM_FROM_UINT(__VA_ARGS__)	\
	__ENUM_TO_UINT(__VA_ARGS__)	\
	__ENUM_TO_STRING(__VA_ARGS__)	\
	__ENUM_NR_STRUCT(__VA_ARGS__)

#define ENUM_NR(prefix)	((unsigned int)(sizeof(struct ____enum_##prefix##nr_s) / sizeof(unsigned int)))

struct __ptr_add_result {
	void *ptr;
	bool is_overflow;
};

#define __ptr_add_check_ovfl(b, o) ({			\
	uptr_t ____b = (uptr_t)(b);			\
	uptr_t ____p = (uptr_t)&(((u8 *)(____b))[o]);	\
	struct __ptr_add_result ____r;			\
							\
	____r.ptr = (void *)____p;			\
	____r.is_overflow =  (____b > ____p) ? true : false;\
	____r;						\
})

#define ptr_add(b, o) ({				\
	struct __ptr_add_result _____r = __ptr_add_check_ovfl(b, o);\
							\
	BUG_ON(_____r.is_overflow);			\
	_____r.ptr;					\
})

#define ptr_diff(a, b) ({			\
	uptr_t ____a = (uptr_t)(a);		\
	uptr_t ____b = (uptr_t)(b);		\
						\
	BUG_ON(____a < ____b);			\
	(____a - ____b);			\
})

#define ptr_sub(b, o) ({			\
	u8 *____b = (u8 *)(b);			\
	u8 *____p = ____b - (size_t)(o);	\
						\
	BUG_ON(____b < ____p);			\
	(void *)____p;				\
})

/*
 * This __max() is defined without statement expression
 * but be careful for side effects.
 */
#define __max(a, b)	(((a) > (b)) ? (a) : (b))

#define max(x, y) ({				\
	typeof(x) ____x = (x);			\
	typeof(y) ____y = (y);			\
	(____x > ____y) ? ____x : ____y;	\
})

#define min(x, y) ({				\
	typeof(x) ____x = (x);			\
	typeof(y) ____y = (y);			\
	(____x < ____y) ? ____x : ____y;	\
})

#endif
