/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: __fprintf_chk
 */

#include <stdio.h>
#include <stdarg.h>
#include "stdio_impl.h"

int __fprintf_chk(FILE *fp, int flag, const char *fmt, ...)
{
	va_list ap;

	(void)flag;
	va_start(ap, fmt);
	int ret = vfprintf(fp, fmt, ap);
	va_end(ap);

	return ret;
}
