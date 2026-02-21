/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: memset implementation
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 21 14:19:23 CST 2023
 */
#include <libmem/memzero.h>

void __arch_memzero(void *const addr, const size_t size)
{
	unsigned long *va_arr = addr;
	const size_t step = 8U;
	const size_t loop = size / sizeof(unsigned long);

	for (size_t i = 0U; i < loop; i += step) {
		va_arr[i] = 0UL;
		va_arr[i + 1U] = 0UL;
		va_arr[i + 2U] = 0UL;
		va_arr[i + 3U] = 0UL;
		va_arr[i + 4U] = 0UL;
		va_arr[i + 5U] = 0UL;
		va_arr[i + 6U] = 0UL;
		va_arr[i + 7U] = 0UL;
	}
}
