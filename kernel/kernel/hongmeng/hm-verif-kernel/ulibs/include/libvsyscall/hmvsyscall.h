/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Hmvsyscall
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 6 14:33:05 2018
 */
#ifndef HMVSYSCALL_H
#define HMVSYSCALL_H

#include <stdint.h>
#include <stdarg.h>
#include <unistd.h>

#ifdef CONFIG_DOPRA_TICK
#define MAX_SYS_NO 501L
#elif defined __arm__
#define MAX_SYS_NO 462L
#else
#define MAX_SYS_NO 415L
#endif

typedef long (*hmsyscall_t)(va_list);

#ifdef CONFIG_RRU_PERF_HACK
#define __HMHACK_PROC_TASK_STATUS_STATE_ACTIVE		1
#define __HMHACK_PROC_TASK_STATUS_STATE_ZOMBIE		2
#define __HMHACK_PROC_TASK_STATUS_STATE_STOPPED		4
#define __HMHACK_PROC_TASK_STATUS_STATE_DEAD		5
#endif

#endif
