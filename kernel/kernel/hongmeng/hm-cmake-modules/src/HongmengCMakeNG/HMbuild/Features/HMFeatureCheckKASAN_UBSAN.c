/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Check if kasan is enabled (check -fsanitize=kernel-address option)
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jul 31 16:23:06 2021
 */

/*
 * If -fsanitize=kernel-address is enabled, __asan_store4_noabort whould be found
 * in resulting asm.
 */
extern void func(unsigned int *x);
void func(unsigned int *x)
{
	x[10u] = 0u;
}
