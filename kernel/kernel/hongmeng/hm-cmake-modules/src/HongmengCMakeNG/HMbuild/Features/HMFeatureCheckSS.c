/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Test if shadow stack is enabled by checking if nosstack attribute is supported
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jul 30 11:08:52 2021
 */
extern int func(void);
int __attribute__((nosstack)) func(void)
{
	return 0;
}
