/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: __printf_chk
 */

#include <stdarg.h>
#include <stdio.h>
#include "stdio_impl.h"

int __printf_chk(int flag, const char *fmt, ...)
{
	va_list ap;

	(void)flag;
	va_start(ap, fmt);
	int ret = vfprintf(stdout, fmt, ap);
	va_end(ap);

	return ret;
}
