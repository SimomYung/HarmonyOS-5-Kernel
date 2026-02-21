/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Vsprintf
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 15 15:57:04 2019
 */
#include <stdio.h>
#include <limits.h>

int vsprintf(char *restrict s, const char *restrict fmt, va_list ap)
{
	return vsnprintf(s, INT_MAX, fmt, ap);
}
