/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Tue May 19 09:58:57 2020
 */
#include <hmsched.h>
#include <syscall.h>
#include <hmsyscall.h>

int sched_get_available_priority_max(void)
{
	return (int)syscall((long)SYS_sched_get_avail_prio_max);
}
