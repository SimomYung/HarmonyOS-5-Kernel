/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Implementation of ulibs bug implement
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan 14 10:53:33 2019
 */

#include <hongmeng/panic.h>
#include <libstrict/strict.h>

void __noreturn
__bug_impl(const char *assertion, const char *bug_file,
	   const char *function, int line_num)
{
	UNUSED(bug_file);
	hm_panic("Panic at %s:%d: Assertion fail: %s\n",
		 function, line_num, assertion);
}
