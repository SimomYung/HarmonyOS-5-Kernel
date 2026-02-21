/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Implementations of __hmlog_printf api
 * Author: Huawei OS Kernel Lab
 * Create: Mon Aug 12 16:43:55 2024
 */
#include <libhmlog/hmlog.h>
#include <stdarg.h>
#include <libstrict/strict.h>

/*
 * hmld.so cannot find raw thread related symbol, which is used for
 * available stack size check. Also there must be enough stack in
 * hmld process. Therefore, we privide the override version here.
 */
void __attribute__((format(printf, 5, 6)))
__hmlog_printf(enum hmlog_level level, const char *file, const char *func,
	       int line, const char *fmt, ...)
{
	UNUSED(file);
	va_list args;

	va_start(args, fmt);
	__hmlog_vprintf(level, func, line, fmt, args);
	va_end(args);
}
