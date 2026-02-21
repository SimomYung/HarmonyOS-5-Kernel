/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: main.c for testcase target_src_regex.
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jul 23 20:49:48 2022
 */

#include <stdio.h>
int main(void)
{
	printf("This is target_src_regex test\n");
#ifdef HAVE_FOO
	printf("SUCCESS HAVE_FOO\n");
#endif
	return 0;
}
