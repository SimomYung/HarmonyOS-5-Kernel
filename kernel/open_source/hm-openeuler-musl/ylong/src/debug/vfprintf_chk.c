/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: __vfprintf_chk
 */
#include <stdarg.h>
#include <stdio.h>
#include "stdio_impl.h"

int __vfprintf_chk(FILE *fp, int flag, const char *fmt, va_list ap)
{
	(void)flag;

	FLOCK(fp);
	int ret = vfprintf(fp, fmt, ap);
	FUNLOCK(fp);

	return ret;
}
