/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Strlen
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 9 03:21:08 2018
 */
#ifdef __KERNEL__
#include <lib/string.h>
#else
#include <string.h>
#endif
#include <stdint.h>

#ifndef __KERNEL__
size_t strlen(const char *str);
#endif

#define has_zero_byte(v) (~(((((v) & 0x7F7F7F7FU) + 0x7F7F7F7FU) | (v)) | 0x7F7F7F7FU))
size_t strlen(const char *str)
{
	const char *ptr = str;
	const unsigned int *w = NULL;

	for (; (uintptr_t)ptr % sizeof(unsigned int) != 0UL; ptr++) {
		if (*ptr == '\0') {
			break;
		}
	}
	if (*ptr != '\0') {
		for (w = (const unsigned int *)(uintptr_t)ptr; has_zero_byte(*w) == 0U; w++) {
			;
		}
		for (ptr = (const char *)w; *ptr != '\0'; ptr++) {
			;
		}
	}
	return (size_t)(long)(ptr - str);
}
