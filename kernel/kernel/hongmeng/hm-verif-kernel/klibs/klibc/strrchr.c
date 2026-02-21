/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Strrchr
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 24 22:12:40 2019
 */
#include <string.h>

char *strrchr(const char *s, int c)
{
	char *ret = NULL;
	const char *src = s;

	for (; *src != '\0'; src++) {
		if (*src == (char)c) {
			ret = (char *)src;
		}
	}
	return ret;
}
