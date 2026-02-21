/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: soname_test.c
* Author: Huawei OS Kernel Lab
* Create: Sat Nov 27 02:16:33 2021
*/

#include <stdio.h>
int fooa(void);
int fooa(void)
{
	printf("test soname\n");
	return 0;
}
