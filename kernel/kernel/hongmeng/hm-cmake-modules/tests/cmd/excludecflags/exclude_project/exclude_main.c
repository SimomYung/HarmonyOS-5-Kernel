/*
* Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
* Description: exclude_main.c
* Author: Huawei OS Kernel Lab
* Create: Thu Jan 06 01:52:11 2022
*/

#include <stdio.h>
int fooa(void);

int main(void)
{
	printf("This is exclude dir test \n");
	fooa();
	return 0;
}
