/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 23 09:19:38 2018
 */
#ifndef AARCH64_ASM_PANIC_H
#define AARCH64_ASM_PANIC_H

#include <mapi/hmasm/panic.h>
#include <hmkernel/drivers/power.h>
#include <hmkernel/power_reboot.h>

static inline void __noreturn
arch_halt(unsigned int reboot_type)
{
	unsigned int __reboot_type = reboot_type;
 
	if (unlikely(__reboot_type == 0U)) {
		__reboot_type = (unsigned int)SYSTEM_REBOOT_KERNEL_PANIC;
	}
	(void)pm_reboot_emergency(__reboot_type);

	asm volatile("1: wfe; wfi; b 1b;\n");
	unreachable();
}

static inline void __noreturn
arch_panic(void)
{
	asm volatile ("brk 0xdead\n");
	unreachable();
}

#endif
