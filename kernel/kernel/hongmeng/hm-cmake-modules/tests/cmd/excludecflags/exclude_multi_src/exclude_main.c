/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: main.c for testcase exclude_multi_src
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 25 13:05:31 2022
 */

#include <stdio.h>
#include <fooa.h>
#include <foob.h>
#include <fooc.h>

int main(void)
{
	printf("This is exclude_multi_src test\n");
	fooa();
	foob();
	fooc();
	return 0;
}
