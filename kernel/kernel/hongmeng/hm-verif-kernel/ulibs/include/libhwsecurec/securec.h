/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Securec
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 7 15:15:14 2018
 */
#ifndef ULIBS_LIBHWSECUREC_SECUREC_H
#define ULIBS_LIBHWSECUREC_SECUREC_H

#include <errno.h>
#include <securectype.h>
#include <securec.h>

#include <hongmeng/panic.h>
#include <hmkernel/strict.h>
#include <stdint.h>
#include <string.h>

#define MEM_OP_STEP_LEN (0x40000000UL)

#if MEM_OP_STEP_LEN > SECUREC_STRING_MAX_LEN
#error "Invalid memory operation step length"
#endif

#define mem_assign_s(dest, src) do {(dest) = (src);} while (1 != 1)
#define mem_assign_a(dest, src)					\
do {								\
	BUG_ON(sizeof(dest) != sizeof(src));			\
	unsigned long ___i;					\
	unsigned long ___num = sizeof(dest) / sizeof((dest)[0]);	\
	for (___i = 0; ___i < ___num; ___i++) {			\
		((dest)[___i]) = ((src)[___i]);			\
	}							\
} while (1 != 1)

static inline void __bzero(void *ptr, size_t len)
{
	(void)memset(ptr, '\0', len);
}

#define __BZERO_FAST_ASSIGN_AT_OFFSET_2 2
#define __BZERO_FAST_ASSIGN_AT_OFFSET_3 3
#define __BZERO_FAST_ASSIGN_AT_OFFSET_4 4

#define __BZERO_SIZE_4BYTE 4
static inline __always_inline void __bzero_4byte(void *ptr)
{
	BUG_ON(ptr == NULL);
	((uint32_t *)ptr)[0] = 0;
}
#define __BZERO_SIZE_8BYTE 8
static inline __always_inline void __bzero_8byte(void *ptr)
{
	BUG_ON(ptr == NULL);
	((uint64_t *)ptr)[0] = 0;
}
#define __BZERO_SIZE_12BYTE 12
static inline __always_inline void __bzero_12byte(void *ptr)
{
	BUG_ON(ptr == NULL);
	((uint32_t *)ptr)[0] = 0;
	((uint32_t *)ptr)[1] = 0;
	((uint32_t *)ptr)[__BZERO_FAST_ASSIGN_AT_OFFSET_2] = 0;
}
#define __BZERO_SIZE_16BYTE 16
static inline __always_inline void __bzero_16byte(void *ptr)
{
	BUG_ON(ptr == NULL);
	((uint64_t *)ptr)[0] = 0;
	((uint64_t *)ptr)[1] = 0;
}
#define __BZERO_SIZE_20BYTE 20
static inline __always_inline void __bzero_20byte(void *ptr)
{
	BUG_ON(ptr == NULL);
	((uint32_t *)ptr)[0] = 0;
	((uint32_t *)ptr)[1] = 0;
	((uint32_t *)ptr)[__BZERO_FAST_ASSIGN_AT_OFFSET_2] = 0;
	((uint32_t *)ptr)[__BZERO_FAST_ASSIGN_AT_OFFSET_3] = 0;
	((uint32_t *)ptr)[__BZERO_FAST_ASSIGN_AT_OFFSET_4] = 0;
}
#define __BZERO_SIZE_24BYTE 24
static inline __always_inline void __bzero_24byte(void *ptr)
{
	BUG_ON(ptr == NULL);
	((uint64_t *)ptr)[0] = 0;
	((uint64_t *)ptr)[1] = 0;
	((uint64_t *)ptr)[__BZERO_FAST_ASSIGN_AT_OFFSET_2] = 0;
}
#define __BZERO_SIZE_32BYTE 32
static inline __always_inline void __bzero_32byte(void *ptr)
{
	BUG_ON(ptr == NULL);
	((uint64_t *)ptr)[0] = 0;
	((uint64_t *)ptr)[1] = 0;
	((uint64_t *)ptr)[__BZERO_FAST_ASSIGN_AT_OFFSET_2] = 0;
	((uint64_t *)ptr)[__BZERO_FAST_ASSIGN_AT_OFFSET_3] = 0;
}
#define __BZERO_SIZE_40BYTE 40
static inline __always_inline void __bzero_40byte(void *ptr)
{
	BUG_ON(ptr == NULL);
	((uint64_t *)ptr)[0] = 0;
	((uint64_t *)ptr)[1] = 0;
	((uint64_t *)ptr)[__BZERO_FAST_ASSIGN_AT_OFFSET_2] = 0;
	((uint64_t *)ptr)[__BZERO_FAST_ASSIGN_AT_OFFSET_3] = 0;
	((uint64_t *)ptr)[__BZERO_FAST_ASSIGN_AT_OFFSET_4] = 0;
}

static inline __always_inline void __bzero_const_maybe_32byte_40byte(void *ptr, size_t len)
{
	if (____IS(__builtin_constant_p(len)) && (len == __BZERO_SIZE_32BYTE)) {
		__bzero_32byte(ptr);
	} else if (____IS(__builtin_constant_p(len)) && (len == __BZERO_SIZE_40BYTE)) {
		__bzero_40byte(ptr);
	} else {
		__bzero(ptr, len);
	}
}

static inline __always_inline void __bzero_const_maybe_20byte_24byte(void *ptr, size_t len)
{
	if (____IS(__builtin_constant_p(len)) && (len == __BZERO_SIZE_20BYTE)) {
		__bzero_20byte(ptr);
	} else if (____IS(__builtin_constant_p(len)) && (len == __BZERO_SIZE_24BYTE)) {
		__bzero_24byte(ptr);
	} else {
		__bzero_const_maybe_32byte_40byte(ptr, len);
	}
}

static inline __always_inline void __bzero_const_maybe_12byte_16byte(void *ptr, size_t len)
{
	if (____IS(__builtin_constant_p(len)) && (len == __BZERO_SIZE_12BYTE)) {
		__bzero_12byte(ptr);
	} else if (____IS(__builtin_constant_p(len)) && (len == __BZERO_SIZE_16BYTE)) {
		__bzero_16byte(ptr);
	} else {
		__bzero_const_maybe_20byte_24byte(ptr, len);
	}
}

static inline __always_inline void __bzero_const_maybe_4byte_8byte(void *ptr, size_t len)
{
	if (____IS(__builtin_constant_p(len)) && (len == __BZERO_SIZE_4BYTE)) {
		__bzero_4byte(ptr);
	} else if (____IS(__builtin_constant_p(len)) && (len == __BZERO_SIZE_8BYTE)) {
		__bzero_8byte(ptr);
	} else {
		__bzero_const_maybe_12byte_16byte(ptr, len);
	}
}

static inline __always_inline void __bzero_const(void *ptr, size_t len)
{
	__bzero_const_maybe_4byte_8byte(ptr, len);
}

#define mem_zero_s(data)	__bzero_const((void *)&(data), sizeof(data))
#define mem_zero_a(array)	__bzero((void*)(&((array)[0])), sizeof(array))
#define mem_zero_b(mem, len)	__bzero((void *)(mem), (len))

#define mem_zero_p(mem, dest_len, src_len)			\
do {								\
	int ___ret;						\
	___ret = memset_s((void*)(mem), (dest_len),		\
			  0, (src_len));			\
	BUG_ON(___ret != 0);					\
} while (1 != 1)

#define ptr_add(b, o) ({				\
	uintptr_t ____b = (uintptr_t)(b);			\
	uintptr_t ____p = (uintptr_t)&(((uint8_t *)(____b))[o]);\
							\
	BUG_ON(____b > ____p);				\
	ptr_to_void(____p);					\
})

#define ptr_diff(a, b) ({			\
	uintptr_t  ____a = (uintptr_t)(a);		\
	uintptr_t ____b = (uintptr_t)(b);		\
						\
	BUG_ON(____a < ____b);			\
	(____a - ____b);			\
})

#define NOFAIL(s) do {                         \
	int ___err;                             \
	___err = (s);                           \
	BUG_ON(___err != 0);                    \
} while (1 != 1)

#endif
