/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Implementation of panic in udk
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 20 20:24:21 2021
 */
#include <udk/panic.h>

void __noreturn
udk_bug(const char *assertion, const char *file, const char *function, int line)
{
	__bug_impl(assertion, file, function, line);
}
