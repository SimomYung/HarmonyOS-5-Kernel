/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Generate struct field offset
 * Author: Huawei OS Kernel Lab
 * Create: Mon July 07 14:47:19 2020
 */
#include <stddef.h>
#include "pthread_impl.h"

#define offset_define(sym, val) \
	asm volatile("offset.ascii \"##" #sym " %0# " #val "\"" : : "i" (val))

hidden int shadow_stack_offset(int argc, char *argv[])
{
#ifdef CONFIG_SHADOW_STACK
	offset_define(SHADOW_STACK_OFFSET, sizeof(struct pthread) - offsetof(struct pthread, sstack));
#endif

	return 0;
}
