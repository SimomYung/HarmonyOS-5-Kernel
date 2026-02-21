/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Memchr
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 5 13:51:50 2018
 */
#include <string.h>
#include <hmkernel/compiler.h>

void __no_stack_protector *memchr(const void *s, int c, size_t n)
{
	void *ret = NULL;
	const unsigned char *ptr = (const unsigned char *)(uptr_t)s;

	while (n > 0U) {
		--n;
		if ((unsigned char)(c) == *ptr++) {
			ret = ulong_to_ptr((uptr_t)(ptr - 1), void);
			break;
		}
	}
	return ret;
}
