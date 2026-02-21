/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2022. All rights reserved.
 * Description: Implementation of ulibs panic
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan 14 10:53:33 2019
 */

#include <hongmeng/panic.h>
#include <libstrict/strict.h>

#define DEFAULT_REBOOT_REASON 0U

void __noreturn
__panic_impl_with_reason(const char *file, const char *func, int line, unsigned int reboot_reason)
{
	UNUSED(file, func, line, reboot_reason);
#ifdef __HOST_LLT__
	abort();
#else
	__builtin_trap();
	__builtin_unreachable();
#endif
}

void __noreturn
__panic_impl(const char *file, const char *func, int line)
{
	__panic_impl_with_reason(file, func, line, DEFAULT_REBOOT_REASON);
}
