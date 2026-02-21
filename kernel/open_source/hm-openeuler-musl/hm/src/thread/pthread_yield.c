/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan 13 14:52:07 2020
 */
#include <pthread.h>

#include "pthread_impl.h"
#include "syscall.h"

int pthread_yield(void)
{
	/*
	 * MISRA Rule 20.12:
	 *   A macro parameter used as an operand to the # or ## operators,
	 *   which is itself subject to further macro replacement, shall
	 *   only be used as an operand to these operators.
	 */
	long call_num = (long)SYS_sched_yield;
	return (int)syscall(call_num);
}
