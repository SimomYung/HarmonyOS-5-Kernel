/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Weak alias implementation for dump interfaces
 * Author: Huawei OS Kernel Lab
 * Create: Thu Nov 15 10:22:06 2021
 */

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <mem/dump.h>

dump_stack_func_t __dump_stack_func = NULL;

static int __dump_init(void)
{
	return 0;
}

static int __dump_stack(uintptr_t *buf, int max)
{
	int depth = 0;

	if (__dump_stack_func != NULL) {
		depth = __dump_stack_func(buf, max);
	}
	return depth;
}

static int __load_symbol(const char *name)
{
	return 0;
}

static int __unload_symbol(void)
{
	return 0;
}

static char *__addr2sym(uintptr_t addr, char *sym, int max)
{
	return NULL;
}

weak_alias(__dump_init, dump_init);
weak_alias(__dump_stack, dump_stack);
weak_alias(__load_symbol, load_symbol);
weak_alias(__unload_symbol, unload_symbol);
weak_alias(__addr2sym, addr2sym);
