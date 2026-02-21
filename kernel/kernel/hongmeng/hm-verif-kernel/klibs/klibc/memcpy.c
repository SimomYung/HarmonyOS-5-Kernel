/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Memcpy
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

#ifndef __HOST_LLT__
#ifdef USE_LIBOPT
/* __memcpy_aarch64() is provided by hm-optimized-routines */
void *__memcpy_aarch64(void *dst, const void *src, size_t len);

void *memcpy(void *dst, const void *src, size_t len)
{
	return __memcpy_aarch64(dst, src, len);
}
#else
/* memcpy() is an alias to raw_cpymem() */
void *memcpy(void *dst, const void *src, size_t len) __attribute__((alias("raw_cpymem")));
#endif
#endif /* __HOST_LLT__ */

#ifndef __KERNEL__
void *raw_cpymem(void *dst, const void *src, size_t len);
#endif

void *
__no_stack_protector raw_cpymem(void *dst, const void *src, size_t len)
{
	unsigned char *to  = ptr_from_void(dst, unsigned char);
	const unsigned char *from = (const unsigned char *)(uintptr_t)src;

	while ((((uintptr_t)to & 0x3UL) != 0UL) && (len != 0UL)) {
		*to++ = *from++;
		--len;
	}
	if (((uintptr_t)from & 0x3UL) == 0UL) {
		/* copy multiple bytes per loop for better performance */
		while (len >= 16UL) {
			*((uint32_t *)(uintptr_t)to) = *((const uint32_t *)(uintptr_t)from);
			*((uint32_t *)(uintptr_t)to + 1UL) = *((const uint32_t *)(uintptr_t)from + 1UL);
			*((uint32_t *)(uintptr_t)to + 2UL) = *((const uint32_t *)(uintptr_t)from + 2UL);
			*((uint32_t *)(uintptr_t)to + 3UL) = *((const uint32_t *)(uintptr_t)from + 3UL);
			len -= 16UL;
			to += 16UL;
			from += 16UL;
		}
		if (len >= 8UL) {
			*((uint32_t *)(uintptr_t)to) = *((const uint32_t *)(uintptr_t)from);
			*((uint32_t *)(uintptr_t)to + 1UL) = *((const uint32_t *)(uintptr_t)from + 1UL);
			len -= 8UL;
			to += 8UL;
			from += 8UL;
		}
		if (len >= 4UL) {
			*((uint32_t *)(uintptr_t)to) = *((const uint32_t *)(uintptr_t)from);
			len -= 4UL;
			to += 4UL;
			from += 4UL;
		}
	}

	while (len != 0UL) {
		*to++ = *from++;
		--len;
	}
	return dst;
}
