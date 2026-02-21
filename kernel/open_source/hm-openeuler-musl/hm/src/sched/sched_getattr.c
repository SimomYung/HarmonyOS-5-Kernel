/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 06 15:21:48 2021
 */
#include <hmsched.h>
#include <syscall.h>
#include <hmsyscall.h>

int sched_getattr(pid_t pid, const struct sched_attr *attr,
		  unsigned int size, unsigned int flags)
{
	return (int)syscall((long)SYS_sched_getattr, pid, attr, size, flags);
}
