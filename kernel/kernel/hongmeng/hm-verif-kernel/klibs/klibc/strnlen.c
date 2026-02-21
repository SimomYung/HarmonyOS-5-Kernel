/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Strnlen
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 9 03:21:08 2018
 */
#ifdef __KERNEL__
#include <lib/string.h>
#else
#include <string.h>
#endif

size_t strnlen(const char *s, size_t maxlen)
{
	const char *sc = s;
	size_t len = maxlen;

	while (len != 0UL && *sc != '\0') {
		--len;
		++sc;
	}
	return (size_t)(long)(sc - s);
}
