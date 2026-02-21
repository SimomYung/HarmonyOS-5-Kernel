/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Test if sfi is enabled by checking if nosfi attribute is supported
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jul 30 11:08:52 2021
 */
extern int func(void);
int __attribute__((nosfi)) func(void)
{
	return 0;
}
