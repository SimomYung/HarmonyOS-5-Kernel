/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 06 15:21:48 2021
 */
#include <hmsched.h>
#include "syscall.h"

int getcpu(unsigned int *cpu, unsigned int *node)
{
	return (int)syscall((long)SYS_getcpu, cpu, node);
}
