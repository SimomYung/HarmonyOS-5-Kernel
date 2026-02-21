/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: AARCH64 errata functions head file
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 30 04:33:09 2018
 */
#ifndef AARCH64_ASM_CPU_ERRATA_H
#define AARCH64_ASM_CPU_ERRATA_H

#include <hmkernel/strict.h>

#ifndef CONFIG_ENABLE_FIX_SPECV2_MITIGATION
static inline void arch_specv2_fix(void)
{
	return;
}

static inline void arch_specv2_fix_with_check(unsigned long addr)
{
	UNUSED(addr);
	return;
}
#else
void arch_specv2_fix(void);

void arch_specv2_fix_with_check(unsigned long addr);
#endif

void cpu_enable_ssbs(void);

#endif
