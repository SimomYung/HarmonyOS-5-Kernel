/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: bsearch
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 16 19:34:38 2024
 */
#include <stdlib.h>
#ifdef __KERNEL__
#include <hmkernel/types.h>
#else
#include <hongmeng/types.h>
#endif

/* The contents of the array should already be in ascending sorted order */
void *bsearch(const void *key, const void *base, size_t num, size_t size, cmp_func_t cmp)
{
	void *ret = NULL;
	const void *try;
	int result;

	while (num > 0) {
		try = (const char *)base + (num >> 1) * size;
		result = cmp(key, try);
		if (result == 0) {
			ret = ptr_to_void(try);
			break;
		}

		if (result > 0) {
			base = (const char *)try + size;
			num--;
		}
		num >>= 1;
	}

	return ret;
}
