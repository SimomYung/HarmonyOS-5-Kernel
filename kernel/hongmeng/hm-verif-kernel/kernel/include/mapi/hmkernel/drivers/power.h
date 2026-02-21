/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Export kernel power interfaces to mapi
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 08 10:47:19 2019
 */
#ifndef MAPI_HMKERNEL_POWER_H
#define MAPI_HMKERNEL_POWER_H

#include <mapi/hmasm/memory.h>
#include <hmkernel/types.h>

struct power_reboot_ops {
	int (*power_off)(void);
	int (*reboot)(unsigned int reboot_type);
};

struct power_cpu_ops {
	int (*cpu_up)(unsigned long mpidr, paddr_t boot_addr);
	int (*cpu_suspend)(unsigned long suspend_state, bool may_lose_context);
	int (*cpu_off)(void);
	int (*cpu_affinity)(unsigned int logic_cpuid);
	bool (*check_lose_context)(unsigned long suspend_state);
};

int pm_register_reboot(const struct power_reboot_ops *const reboot_ops);
int pm_register_cpu(const struct power_cpu_ops *const cpu_ops);
int pm_power_off(void);
void reboot_info_record(const char *reboot_reason, unsigned int reboot_code);
int pm_reboot(const char *reboot_str, unsigned int reboot_type);
int pm_reboot_emergency(unsigned int reboot_type);
int pm_cpu_up(unsigned long mpdir, paddr_t boot_addr);
int pm_cpu_suspend(unsigned long suspend_state, bool may_lose_context);
int pm_cpu_off(void);
int pm_cpu_affinity(unsigned int logic_cpuid);
int pm_check_lose_context(unsigned long suspend_state, bool *may_lose_context);
void pm_rebootcode_record(unsigned int reboot_type);

#endif
