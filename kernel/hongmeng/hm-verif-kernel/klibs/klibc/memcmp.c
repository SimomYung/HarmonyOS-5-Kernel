/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Memcmp
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 14 04:12:13 2018
 */
#ifdef __KERNEL__
#include <lib/string.h>
#else
#include <hmkernel/compiler.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h> /* for size_t */
typedef uintptr_t uptr_t;
#endif

int __no_stack_protector memcmp(const void *vl, const void *vr, size_t n)
{
	const unsigned char *l = (const unsigned char *)(uptr_t)vl;
	const unsigned char *r = (const unsigned char *)(uptr_t)vr;
	int ret;

	while (n > 0UL && *l == *r) {
		n--;
		l++;
		r++;
	}

	if (n == 0UL) {
		ret = 0;
	} else if (*l > *r) {
		ret = 1;
	} else {
		ret = -1;
	}

	return ret;
}

