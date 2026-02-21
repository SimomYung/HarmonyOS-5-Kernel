/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Strncpy
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 9 03:21:08 2018
 */
#ifdef __KERNEL__
#include <lib/string.h>
#endif
#include <stdint.h>
#include <string.h>

#ifndef __KERNEL__
char *raw_strncpy(char *dst, const char *src, size_t len);
#endif

#ifndef __HOST_LLT__
char *strncpy(char *dst, const char *src, size_t len) __attribute__((alias("raw_strncpy")));
#endif

#define has_zero_byte(v) (~(((((v) & 0x7F7F7F7FU) + 0x7F7F7F7FU) | (v)) | 0x7F7F7F7FU))
char *
raw_strncpy(char *dst, const char *src, size_t len)
{
	char *ret = dst;
	int is_none = 0;
	char *d = dst;
	const char *s = src;
	size_t l = len;

	if (((uintptr_t)d & 0x3UL) == ((uintptr_t)s & 0x3UL)) {
		while ((((uintptr_t)d & 0x3UL) != 0UL) && (l != 0UL)) {
			*d = *s;
			if (*d == '\0') {
				break;
			}
			++d;
			++s;
			--l;
		}
		if (*s == '\0' || l == 0UL) {
			is_none = 1;
		}

		if (is_none == 0) {
			while (l >= 4UL && has_zero_byte((*(const uint32_t *)(uintptr_t)s)) == 0U) {
				(*(uint32_t *)(uintptr_t)d) = (*(const uint32_t *)(uintptr_t)s);
				l -= 4UL;
				s += 4UL;
				d += 4UL;
			}
		}
	}

	if (is_none == 0) {
		while (l != 0UL) {
			*d = *s;
			if (*d == '\0') {
				break;
			}
			++d;
			++s;
			--l;
		}
	}

	(void)memset((void *)d, 0, l);
	return ret;
}
