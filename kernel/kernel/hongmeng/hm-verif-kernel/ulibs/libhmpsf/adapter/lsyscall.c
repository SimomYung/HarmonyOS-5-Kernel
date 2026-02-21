/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: libhmpsf xvm lsyscalls
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 19 10:18:05 2024
 */

#include <stdbool.h>
#include <libhmpsf/adapter/lsyscall.h>

static bool g_xvm_use_lsyscall = false;

int xvm_use_lsyscall(void)
{
	g_xvm_use_lsyscall = true;
	return 0;
}

bool xvm_is_using_lsyscall(void)
{
	return g_xvm_use_lsyscall;
}
