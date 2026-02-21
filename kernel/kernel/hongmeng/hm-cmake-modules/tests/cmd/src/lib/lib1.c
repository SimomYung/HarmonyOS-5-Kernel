/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: lib2.c
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 06 17:16:31 2021
*/

#include <stdio.h>

struct info_s {
	int test_val;
};

int fooa(int val);
int fooa(int val)
{
	printf("this is fooa\n");
	struct info_s info;
	info.test_val = val;
	return info.test_val;
}
