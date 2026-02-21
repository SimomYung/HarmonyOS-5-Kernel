/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Export some helper functions of cpu to modules
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 14 13:58:42 2018
 */
#ifndef MAPI_HMKERNEL_CPU_H
#define MAPI_HMKERNEL_CPU_H

#include <hmkernel/cpuset.h>

ENUM(5, cpu_rstate, CPU_RSTATE_,
	/* offline */
	OFFLINE,
	SUSPEND_OFFLINE,
	/* online */
	RUNNING,
	SUSPEND,
	HOLDING
)

/*
 * CPUs can be offline in the two cases:
 *   1: cpu hotplug unplug CPUs
 *   2: suspend disable noboot CPUs
 * cpu_is_online returns true only when the cpu is not in the above two cases.
 * cpu_is_online returns false if the cpu in either of the two cases.
 */
bool cpu_is_online(unsigned int cpuid);
bool cpu_is_running(unsigned int cpuid);
bool cpu_is_offline(unsigned int cpuid, bool suspend);
bool cpu_is_holding(unsigned int cpuid);
enum cpu_rstate cpu_rstate_of(unsigned int cpuid);
const char *cpu_rstate_string_of(unsigned int cpuid);
cpuset_t cpu_online_set(void);
unsigned int cpu_register_num(void);
u64 cpu_latest_kernel_time(unsigned int cpuid);

#endif
