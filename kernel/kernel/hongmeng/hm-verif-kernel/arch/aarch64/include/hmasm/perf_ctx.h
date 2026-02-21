/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 13 11:42:10 2023
 */

#ifndef ARCH64_ASM_PERF_CTX_H
#define ARCH64_ASM_PERF_CTX_H
#include <hmasm/sysreg.h>
#include <hmasm/registers.h>
#include <hmasm/processor.h>
#include <hmkernel/compiler.h>

#define arch_current_sp()			\
({						\
	unsigned long sp;			\
	asm("mov %0, sp" : "=r"(sp));		\
	sp;					\
})

static inline unsigned long current_spsr(void)
{
	unsigned long spsr;

	if (a64_curr_el() == CURRENTEL_EL2) {
		spsr = PSR_M_EL2H;
	} else {
		spsr = PSR_M_EL1H;
	}

	return spsr;
}

#define perf_arch_fetch_caller_regs(regs) \
do { \
	REG_PC(regs) = (unsigned long)__builtin_return_address(0); \
	REG_SP(regs) = arch_current_sp(); \
	REG_FP(regs) = (unsigned long)__builtin_frame_address(0); \
	REG_SPSR(regs) = current_spsr(); \
} while (0)

static inline bool arch_regs_is_kernel(const struct arch_regs *regs)
{
	return __IS((REG_SPSR(regs) & PSR_M_MASK) == current_spsr());
}

#endif
