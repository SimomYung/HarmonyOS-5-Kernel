/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Strchr
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 5 13:51:50 2018
 */
#include <string.h>

char *strchr(const char *s, int c)
{
	char *ret = NULL;
	const char* str = s;

	for (; *str != (char)c; str++) {
		if (*str == '\0') {
			break;
		}
	}

	if (*str == (char)c) {
		ret = (char *)str;
	} else {
		ret = NULL;
	}
	return ret;
}
