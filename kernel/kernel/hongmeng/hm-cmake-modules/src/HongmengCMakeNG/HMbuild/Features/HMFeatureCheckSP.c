/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Test if stack protector is enabled by checking gcc -S result
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 16 17:41:43 2022
 */

#define ARRAY_SIZE 10

int func(void)
{
	char arr[ARRAY_SIZE];
	arr[0] = 0;
	arr[1] = 1;
	int sum = 0;
	for(int i  = 0; i < ARRAY_SIZE; i++)
		sum += i;
	return sum;
}
