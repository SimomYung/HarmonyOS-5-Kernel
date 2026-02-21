/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Memset
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 9 03:21:08 2018
 */
#ifdef __KERNEL__
#include <lib/string.h>
#endif
#include <hmkernel/compiler.h>
#include <stdint.h>
#include <string.h>

#ifndef MAPI_HMKERNEL_TYPES_H
#include <hongmeng/types.h>
#endif

#ifndef __KERNEL__
void *raw_setmem(void *dst, int c, size_t len);
#endif

#if defined(__clang__) && (__clang_major__ >= 15)
__attribute__((no_builtin("memset")))
#endif
void *
__no_stack_protector raw_setmem(void *dst, int c, size_t len)
{
	unsigned char *to = ptr_from_void(dst, unsigned char);
	while ((((uintptr_t)to & 0x7UL) != 0UL)  && (len != 0UL)) {
		*to++ = (unsigned char)c;
		--len;
	}

	{
		union {
			uint64_t x;
			unsigned char buf[8];
		} tmp;

		tmp.buf[0] = (unsigned char)c;
		tmp.buf[1] = (unsigned char)c;
		tmp.buf[2] = (unsigned char)c;
		tmp.buf[3] = (unsigned char)c;
		tmp.buf[4] = (unsigned char)c;
		tmp.buf[5] = (unsigned char)c;
		tmp.buf[6] = (unsigned char)c;
		tmp.buf[7] = (unsigned char)c;

		for (; len >= 16UL; len -= 16UL) {
			*((uint64_t *)(uintptr_t)to) = tmp.x;
			*((uint64_t *)(uintptr_t)to + 1) = tmp.x;
			to += 16UL;
		}
		if (len >= 8UL) {
			*((uint64_t *)(uintptr_t)to) = tmp.x;
			len -= 8UL;
			to += 8UL;
		}

		switch (len) {
		case 7:
			*to++ = (unsigned char)c;
			/* fall-through */
		case 6:
			*to++ = (unsigned char)c;
			/* fall-through */
		case 5:
			*to++ = (unsigned char)c;
			/* fall-through */
		case 4:
			*to++ = (unsigned char)c;
			/* fall-through */
		case 3:
			*to++ = (unsigned char)c;
			/* fall-through */
		case 2:
			*to++ = (unsigned char)c;
			/* fall-through */
		case 1:
			*to++ = (unsigned char)c;
			/* fall-through */
		default:
			break;
		}
	}

	return dst;
}

#ifndef __HOST_LLT__
void *memset(void *dst, int c, size_t len) __attribute__((alias("raw_setmem")));
#endif
