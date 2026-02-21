/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: interface of ucfi
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jul 22 17:15:39 2022
 */

#include <stdint.h>
#include <libstrict/strict.h>

extern int ucfi_fail(unsigned long addr);

/* Here is the stub function for ucfi_fail. The formal
 * implementation of the function is in the hm-security
 * code repository */
int ucfi_fail(unsigned long addr)
{
	UNUSED(addr);
	return 0;
}
