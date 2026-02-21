/*
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: soname_test.c
* Author: Huawei OS Kernel Lab
* Create: Sat Nov 27 02:16:33 2021
*/

#include <stdio.h>
#include "fooa.h"

int fooa(void)
{
	printf("this is fooa\n");
	return 0;
}
