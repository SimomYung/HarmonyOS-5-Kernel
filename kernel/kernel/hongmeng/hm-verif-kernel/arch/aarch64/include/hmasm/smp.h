/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jun 29 21:34:30 2018
 */
#ifndef AARCH64_ASM_SMP_H
#define AARCH64_ASM_SMP_H

#ifndef __ASSEMBLY__

#include <hmasm/processor.h>
#include <hmasm/sysreg.h>
#include <hmasm/cpu_local.h>
#include <hmkernel/smp.h>
#include <hmkernel/cpuid.h>
#include <hmkernel/compiler.h>

#ifdef CONFIG_SMP
static inline void
__smp_set_current_cpu_id(unsigned int logic_cpuid)
{
	arch_current_cpu_local()->cpu = logic_cpuid;
}

extern void __noreturn arch_start_secondary_cpu(void);
extern void __noreturn halting(unsigned long cpuid);

#else

static inline void
__smp_set_current_cpu_id(unsigned int logic_cpuid)
{
	arch_current_cpu_local()->cpu = 0;
}

extern void arch_start_secondary_cpu(void);
extern void halting(unsigned long cpuid);

#endif

void smp_prepare_boot_cpu(void);

#endif

#endif
