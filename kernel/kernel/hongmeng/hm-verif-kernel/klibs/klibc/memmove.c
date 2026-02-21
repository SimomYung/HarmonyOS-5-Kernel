/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Memmove
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 5 13:51:50 2018
 */
#include <lib/string.h>
#include <string.h>
#include <stdint.h>

#ifndef __HOST_LLT__
void *memmove(void *to, const void *from, size_t len) __attribute__((alias("raw_movemem")));
#endif

void *raw_movemem(void *to, const void *from, size_t len)
{
	uint8_t *d = ptr_from_void(to, uint8_t);
	const uint8_t *s = (const uint8_t *)(uptr_t)from;
	size_t count = len;

	if (ptr_from_void(to, const void) <= from) {
		while (count > 0U) {
			count--;
			*d++ = *s++;
		}
	} else {
		d += count;
		s += count;

		while (count > 0U) {
			count--;
			*--d = *--s;
		}
	}
	return to;
}
