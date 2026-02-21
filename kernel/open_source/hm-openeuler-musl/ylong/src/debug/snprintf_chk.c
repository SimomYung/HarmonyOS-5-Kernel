/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: __snprintf_chk
 */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "stdio_impl.h"

int __snprintf_chk(char *s, size_t n, int flag, size_t slen, const char *fmt, ...)
{
	int ret;
	(void)flag;

	if (slen < n) {
		printf("*** buffer overflow detected ***: terminated\n");
		abort();
	}

	va_list ap;
	va_start(ap, fmt);
	ret = vsnprintf(s, n, fmt, ap);
	va_end(ap);
	return ret;
}
