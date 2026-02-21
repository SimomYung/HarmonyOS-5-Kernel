/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Test if ucfi is enabled by checking gcc -S result
 * Author: Huawei OS Kernel Lab
 * Create: Tue May 31 14:20:09 2022
 */

static void func1(void)
{
	printf("func1\n");
}

extern int func(void);
int func(void)
{
	void (*ptr)(void) = func1;
	return 0;
}
