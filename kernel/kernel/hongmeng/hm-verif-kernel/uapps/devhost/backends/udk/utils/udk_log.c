/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Implementation of log in udk
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 20 18:01:36 2021
 */
#include <udk/log.h>

#include <libhmlog/hmlog.h>
#include <libhwsecurec/securec.h>

#define UNUSED(x) ((void)(x))
void __attribute__((format(printf, 5, 6)))
udk_printf(enum udklog_level level, const char *file, const char *func,
	   int line, const char *fmt, ...)
{
	va_list vargs;

	UNUSED(file);
	va_start(vargs, fmt);
	__hmlog_vprintf_stkchk((enum hmlog_level)level, func, line, fmt, vargs);
	va_end(vargs);
}
