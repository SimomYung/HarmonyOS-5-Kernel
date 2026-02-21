/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: exclude_main.c
* Author: Huawei OS Kernel Lab
* Create: Fri Oct 22 02:21:49 2021
*/

#include <stdio.h>

int main(void)
{
	printf("This is exclude regex test\n");
#ifdef MACRO_VALUE
	printf("SUCCESS MACRO_VALUE=%d\n",MACRO_VALUE);
#else
	printf("SUCCESS NO_MACRO_VALUE\n");
#endif
	return 0;
}
