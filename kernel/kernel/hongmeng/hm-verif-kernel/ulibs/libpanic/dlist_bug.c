/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Implementation of dlist bug implement
 * Author: Huawei OS Kernel Lab
 * Create: Tue Dec 1 10:05:16 2020
 */
#include <hongmeng/panic.h>

void  __noreturn __attribute__((format(printf, 1, 2)))
__dlist_bug(const char *fmt, ...)
{
	va_list args;

	/* output warning with level as HMLOG_PANIC */
	va_start(args, fmt);
	__hmlogv_panic(fmt, args);
	va_end(args);

	/* trigger a panic */
	__panic_impl(NULL, __func__, __LINE__);
}
