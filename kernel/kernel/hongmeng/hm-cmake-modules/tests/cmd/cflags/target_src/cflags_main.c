/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: cflags_main.c
* Author: Huawei OS Kernel Lab
* Create: Tue Oct 26 04:12:52 2021
*/

#include <stdio.h>

int main(void)
{
	printf("This is cflags target src test\n");
#ifdef HAVE_SRC
	printf("SUCCESS HAVE_SRC\n");
#endif
	return 0;
}
