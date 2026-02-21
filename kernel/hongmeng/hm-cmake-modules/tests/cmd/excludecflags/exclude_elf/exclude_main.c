/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: exclude_main.c
* Author: Huawei OS Kernel Lab
* Create: Fri Oct 22 02:21:49 2021
*/

#include <stdio.h>

int main(void)
{
	printf("This is exclude elf test\n");
#ifdef HAVE_FOO
	printf("SUCCESS HAVE_FOO");
#else
	printf("SUCCESS NO_FOO");
#endif
	return 0;
}
