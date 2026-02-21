/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Bzero
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 16 12:44:56 2019
 */

#ifdef __KERNEL__
#include <lib/string.h>
#else
#include <hmkernel/compiler.h>
#include <string.h>
void *raw_setmem(void *dst, int c, size_t len);
#endif

void __no_stack_protector bzero(void *dst, size_t sz)
{
	(void)raw_setmem(dst, '\0', sz);
}
