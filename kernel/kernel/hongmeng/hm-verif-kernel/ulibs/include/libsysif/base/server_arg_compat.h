/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Utils for the server to handle the aarch32 apps under compatible mode
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 24 18:25:07 2020
 */

#ifndef LIBSYSIF_BASE_SERVER_ARG_COMPAT_H
#define LIBSYSIF_BASE_SERVER_ARG_COMPAT_H

#include <hmkernel/errno.h>
#include <libhwsecurec/securec.h>
#include <hongmeng/compiler.h>

/*
 * The _Bool for 1 byte same as char, well, the enum for 4 bytes
 * and take as struct.
 */
enum __type {
	_TYPE_CHAR,
	_TYPE_SHORT,
	_TYPE_INT,
	_TYPE_LONG,
	_TYPE_LONGLONG,
	_TYPE_UCHAR,
	_TYPE_USHORT,
	_TYPE_UINT,
	_TYPE_ULONG,
	_TYPE_ULONGLONG,
	_TYPE_POINTER,
	_TYPE_STRUCT,
};

enum __data_model {
	_DATA_MODEL_LP64,
	_DATA_MODEL_ILP32,
};

/*
 * The struct pointer, union pointer, enum pointer are not allowed, use
 * `void *` or `void **` instead. It's rare to use the triple pointers,
 * thus there are no triple pointers allowed in sysif interfaces too.
 */
#define __type_of(t)	( \
		__same_type(t, char)				? _TYPE_CHAR : ( \
		__same_type(t, _Bool)				? _TYPE_CHAR : ( \
		__same_type(t, short)				? _TYPE_SHORT : ( \
		__same_type(t, int)				? _TYPE_INT : ( \
		__same_type(t, long)				? _TYPE_LONG : ( \
		__same_type(t, long long)			? _TYPE_LONGLONG : ( \
		__same_type(t, unsigned char)			? _TYPE_UCHAR: ( \
		__same_type(t, unsigned short)			? _TYPE_USHORT : ( \
		__same_type(t, unsigned int)			? _TYPE_UINT : ( \
		__same_type(t, unsigned long)			? _TYPE_ULONG : ( \
		__same_type(t, unsigned long long)		? _TYPE_ULONGLONG : ( \
		__same_type(t, char *)				? _TYPE_POINTER : ( \
		__same_type(t, _Bool *)				? _TYPE_POINTER : ( \
		__same_type(t, short *)				? _TYPE_POINTER : ( \
		__same_type(t, int *)				? _TYPE_POINTER : ( \
		__same_type(t, long *)				? _TYPE_POINTER : ( \
		__same_type(t, long long *)			? _TYPE_POINTER : ( \
		__same_type(t, unsigned char *)			? _TYPE_POINTER : ( \
		__same_type(t, unsigned short *)		? _TYPE_POINTER : ( \
		__same_type(t, unsigned int *)			? _TYPE_POINTER : ( \
		__same_type(t, unsigned long *)			? _TYPE_POINTER : ( \
		__same_type(t, unsigned long long *)		? _TYPE_POINTER : ( \
		__same_type(t, const char *)			? _TYPE_POINTER : ( \
		__same_type(t, const _Bool *)			? _TYPE_POINTER : ( \
		__same_type(t, const short *)			? _TYPE_POINTER : ( \
		__same_type(t, const int *)			? _TYPE_POINTER : ( \
		__same_type(t, const long *)			? _TYPE_POINTER : ( \
		__same_type(t, const long long *)		? _TYPE_POINTER : ( \
		__same_type(t, const unsigned char *)		? _TYPE_POINTER : ( \
		__same_type(t, const unsigned short *)		? _TYPE_POINTER : ( \
		__same_type(t, const unsigned int *)		? _TYPE_POINTER : ( \
		__same_type(t, const unsigned long *)		? _TYPE_POINTER : ( \
		__same_type(t, const unsigned long long *)	? _TYPE_POINTER : ( \
		__same_type(t, char **)				? _TYPE_POINTER : ( \
		__same_type(t, _Bool **)			? _TYPE_POINTER : ( \
		__same_type(t, short **)			? _TYPE_POINTER : ( \
		__same_type(t, int **)				? _TYPE_POINTER : ( \
		__same_type(t, long **)				? _TYPE_POINTER : ( \
		__same_type(t, long long **)			? _TYPE_POINTER : ( \
		__same_type(t, unsigned char **)		? _TYPE_POINTER : ( \
		__same_type(t, unsigned short **)		? _TYPE_POINTER : ( \
		__same_type(t, unsigned int **)			? _TYPE_POINTER : ( \
		__same_type(t, unsigned long **)		? _TYPE_POINTER : ( \
		__same_type(t, unsigned long long **)		? _TYPE_POINTER : ( \
		__same_type(t, const char **)			? _TYPE_POINTER : ( \
		__same_type(t, const _Bool **)			? _TYPE_POINTER : ( \
		__same_type(t, const short **)			? _TYPE_POINTER : ( \
		__same_type(t, const int **)			? _TYPE_POINTER : ( \
		__same_type(t, const long **)			? _TYPE_POINTER : ( \
		__same_type(t, const long long **)		? _TYPE_POINTER : ( \
		__same_type(t, const unsigned char **)		? _TYPE_POINTER : ( \
		__same_type(t, const unsigned short **)		? _TYPE_POINTER : ( \
		__same_type(t, const unsigned int **)		? _TYPE_POINTER : ( \
		__same_type(t, const unsigned long **)		? _TYPE_POINTER : ( \
		__same_type(t, const unsigned long long **)	? _TYPE_POINTER : ( \
		__same_type(t, void **)				? _TYPE_POINTER : ( \
		__same_type(t, const void *)			? _TYPE_POINTER : ( \
		__same_type(t, const void **)			? _TYPE_POINTER : ( \
		__same_type(t, void *)				? _TYPE_POINTER : _TYPE_STRUCT \
		)))))))))))))))))))))))))))))))))))))))))))))))))))))))))))

#define __fetch_align(type, d_type, align_size) \
	case type: {			\
		align = align_size;	\
		break;		\
	}

/* The alignment should be 4 or 8 according to the aapcs */
static inline __always_inline unsigned long
__do_align_ilp32(enum __type type)
{
	unsigned long align = 0UL;

	switch (type) {
	__fetch_align(_TYPE_CHAR, char, sizeof(int8_t))
	__fetch_align(_TYPE_SHORT, short, sizeof(int16_t))
	__fetch_align(_TYPE_INT, int, sizeof(int32_t))
	/* For the ILP32 client, long is 32bits width */
	__fetch_align(_TYPE_LONG, long, sizeof(int32_t))
	__fetch_align(_TYPE_LONGLONG, long long, sizeof(int64_t))
	__fetch_align(_TYPE_UCHAR, unsigned char, sizeof(uint8_t))
	__fetch_align(_TYPE_USHORT, unsigned short, sizeof(uint16_t))
	__fetch_align(_TYPE_UINT, unsigned int, sizeof(uint32_t))
	/* For the ILP32 client, unsigned long is 32bits width */
	__fetch_align(_TYPE_ULONG, unsigned long, sizeof(uint32_t))
	__fetch_align(_TYPE_ULONGLONG, unsigned long long, sizeof(uint64_t))
	/* For the ILP32 client, the ptr is 32bits width */
	__fetch_align(_TYPE_POINTER, void *, sizeof(uint32_t))
	default:
		break;
	}

	return (align > 4UL) ? 8UL : 4UL;
}

static inline __always_inline unsigned long
___alignment_of_one_arg(enum __data_model client_model, enum __type type)
{
	unsigned long align = 0UL;

	switch (client_model) {
	case _DATA_MODEL_LP64:
		align = (unsigned long)E_HM_NOSYS;
		break;
	case _DATA_MODEL_ILP32:
		align = __do_align_ilp32(type);
		break;
	default:
		/* do nothing */
		break;
	}
	return align;
}

/*
 * We assum the client and server have the same sizeof(type) for
 * the Composite Type. And this is mandatory requirement.
 */
#define ___actv_rpc_stk_alignment(type)	\
	({ struct { type ____v; unsigned char ____c; } ____x; (sizeof(____x) - sizeof(type)) > 4UL ? 8UL : 4UL; })

#define __alignment_of_one_arg(model, type, t)	\
	if (type == _TYPE_STRUCT) {				\
		align = ___actv_rpc_stk_alignment(t);		\
	} else {						\
		align = ___alignment_of_one_arg(model, type);	\
	}							\

#define __fetch_type(ca, d_type, d_int_type, s_int_type, dst, src, ret) \
	case ca: { \
		d_type *d = (d_type *)dst; \
		s_int_type *s = (s_int_type *)src; \
		*d = (d_type)(d_int_type)(*s);\
		ret = (int)(unsigned int)sizeof(s_int_type); \
		break;		\
	}

/*
 * Here, to cover the endian issue, we fetch 4bytes for the types
 * whose size are less than 4bytes, e.g., char, short, _Bool.
 */
static inline __always_inline int
__do_fetch_ilp32(void *dst, void *src, enum __type type)
{
	int ret;
	switch (type) {
	__fetch_type(_TYPE_CHAR, char, int8_t, int32_t, dst, src, ret) // fetch 32bits
	__fetch_type(_TYPE_SHORT, short, int16_t, int32_t, dst, src, ret) // fetch 32bits
	__fetch_type(_TYPE_INT, int, int32_t, int32_t, dst, src, ret)
	/* For the ILP32 client, the server extends to 64bits width */
	__fetch_type(_TYPE_LONG, long, int64_t, int32_t, dst, src, ret)
	__fetch_type(_TYPE_LONGLONG, long long, int64_t, int64_t, dst, src, ret)
	__fetch_type(_TYPE_UCHAR, unsigned char, uint8_t, uint32_t, dst, src, ret) // fetch 32bits
	__fetch_type(_TYPE_USHORT, unsigned short, uint16_t, uint32_t, dst, src, ret) // fetch 32bits
	__fetch_type(_TYPE_UINT, unsigned int, uint32_t, uint32_t, dst, src, ret)
	/* For the ILP32 client, the server extends to 64bits width */
	__fetch_type(_TYPE_ULONG, unsigned long, uint64_t, uint32_t, dst, src, ret)
	__fetch_type(_TYPE_ULONGLONG, unsigned long long, uint64_t, uint64_t, dst, src, ret)
	__fetch_type(_TYPE_POINTER, void *, uintptr_t, uint32_t, dst, src, ret)
	default:
		ret = E_HM_INVAL;
		break;
	}
	return ret;
}

static inline __always_inline int
___fetch_one_arg(void *dst, void *src, unsigned int nr,
		 enum __data_model client_model,
		 enum __type type)
{
	UNUSED(nr);
	int fetch_sz;

	if (type == _TYPE_STRUCT) {
		__hmattr_sideff_resolved("no relative side effect");
		return E_HM_INVAL;
	}

	switch (client_model) {
	case _DATA_MODEL_LP64:
		fetch_sz = E_HM_NOSYS;
		break;
	case _DATA_MODEL_ILP32:
		fetch_sz = __do_fetch_ilp32(dst, src, type);
		break;
	default:
		fetch_sz = E_HM_INVAL;
		break;
	}
	return fetch_sz;
}

/*
 * dst: ptr to the local arg
 * src: ptr to the stack
 * n: arg number
 * arg_sz: arg size
 * model: data model
 * type: client arg's enum type
 * t: arg type
 *
 * As for passing args through r3, it depends on the alignment of the args
 * and the actvcall type.
 *
 * The prerequiest that `r3` is used to passing the arg is that
 * the arg is the first arg in the ARG(...) and only normal
 * actvcalls have this chance. Neither actvcapcalls nor the fallback calls.
 *
 * If not the first arg, must not passing from `r3` and all args from the stack.
 *
 * For _TYPE_STRUCT may passing `r3`:
 * 1. align == 4 && arg_sz > 4, the arg passing by the `r3` and the stack
 * 2. align == 4 && arg_sz <= 4, the arg passing by the `r3`
 * 3. align == 8, the arg passing by the stack, the `r3` is skipped
 *
 * For other types may passing `r3`, e.g., fundamental types:
 * 1. align == 4, e.g., arg_sz <= 4, the arg passing by the `r3`
 * 2. align == 8, the arg passing by the stack, the `r3` is skipped
 */
/*
 * NOTE: Here we don't check the return value of the memcpy_s to reduce the
 * code complexity and gain the performance. Because
 * 1. There is no chance the cpy failed as the dst allocated in the stack
 *    and the arg_sz well maintained as we define the server hdlr.
 * 2. In case the cpy failed, the server hdlr will get the invalid parameter
 *    but will not cause the safty problem, the caller will receive a ERROR code.
 */
#define __fetch_one_arg(dst, src, n, arg_sz, model, type, t, arg)	\
	__alignment_of_one_arg(model, type, t)			\
	cursor = __arch_actv_rpc_stk_cursor_align(cursor, align); \
	if (type == _TYPE_STRUCT) {				\
		if ((n) == 0 && is_r3_passing && align == 4) {	\
			fetch_sz = 0;				\
			union {					\
				uint32_t ___buf;		\
				t v;				\
			} ___v = {0};				\
			___v.___buf = (uint32_t)___w3;		\
			arg = ___v.v;				\
			if (arg_sz > 4) {			\
				memcpy_s((char *)dst + sizeof(uint32_t), arg_sz - sizeof(uint32_t), \
					 (char *)(void *)&src[cursor], arg_sz - sizeof(uint32_t)); \
				fetch_sz = arg_sz - sizeof(uint32_t); \
			}					\
		} else {					\
			*(t *)(dst) = *(t *)(void *)(&src[cursor]); \
			fetch_sz = sizeof(t);			\
		}						\
	} else {						\
		if ((n) == 0 && is_r3_passing && align == 4) {	\
			uint32_t ___buf = (uint32_t)___w3;	\
			fetch_sz = ___fetch_one_arg(dst, (void *)&___buf, n, model, type); \
			if (fetch_sz > 0) {			\
				fetch_sz = 0;			\
			}					\
		} else {					\
			fetch_sz = ___fetch_one_arg(dst, &src[cursor], n, model, type); \
		}						\
	}							\

/*
 * Cast ilp32 type into lp64 type for aarch64ilp32 client
 */
#define __do_adjust_ilp32(ca, d_type, d_int_type, s_int_type, src)	\
	case ca: {							\
		d_type *d = (d_type *)(src);				\
		d_int_type *s = (d_int_type *)(src);			\
		*d = (d_type)(d_int_type)(s_int_type)(*s);		\
		break;							\
	}

/*
 * Fetch one arg from stack or registers for aarch64ilp32 client
 * Here, we don't need to care about the offset of parameters like compat mode
 * because aarch64ilp32 has the same register and stack layout as aarch64.
 *
 * But we have to process long, ulong and pointer type because these three
 * types have different bits on aarch64 and aarch64ilp32.
 *
 * There are two possible exceptional cases:
 *
 * 1. Undefined value
 * register or stack has 64bit, but parameter only occupies lower 32bit or
 * higher 32bit which depends on byte order. The left 32bit value is undefined,
 * so we have to cast it to make sure it's zero.
 *
 * 2. Negative number
 * only happens when parameter is long type
 * for example, 0xFFFFFFFF means -1 on aarch64ilp32.
 * if byte order is big endian, it's 0x00000000FFFFFFFF on aarch64
 * it means 4294967295
 *
 */
#define __adjust_one_arg(t, arg)							\
	enum __type ___t = __type_of(t);						\
	switch(___t) {									\
	__do_adjust_ilp32(_TYPE_LONG, long, int64_t, int32_t, &(arg))			\
	__do_adjust_ilp32(_TYPE_ULONG, unsigned long, uint64_t, uint32_t, &(arg))	\
	__do_adjust_ilp32(_TYPE_POINTER, void *, uintptr_t, uint32_t, &(arg))		\
	default:									\
		break;									\
	}

#endif
