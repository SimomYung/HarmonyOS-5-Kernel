/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Definition of cpu init context for aarch64
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 30 16:23:36 2018
 */
#ifndef AARCH64_ASM_CPU_BOOT_H
#define AARCH64_ASM_CPU_BOOT_H

#include <hmkernel/compiler.h>

/* According to Calling Convention ARM DEN 0028B: for AArch64
 * Callee Save regs are x19~x28, and mandatory func prolog regs
 * x29(FP), x30(LR), SP, so totally 13 regs shall be taken into
 * cpuinitctx snapshot for cpu_ops such as cpu_suspend, resume.
 */
#define CPUINITCTX_CALLEE_SAVED_REGS_BASE	0
#define CPUINITCTX_CALLEE_SAVED_FP		CPUINITCTX_CALLEE_SAVED_REGS_BASE
#define CPUINITCTX_CALLEE_SAVED_LR		(CPUINITCTX_CALLEE_SAVED_REGS_BASE + 1)
/* NOTE: there's no need to define macros for each general regs x19~x28 currently. */
#define CPUINITCTX_CALLEE_SAVED_SP		(CPUINITCTX_CALLEE_SAVED_REGS_BASE + 12)
#define CPUINITCTX_CALLEE_SAVED_REGS_NR		13

#define CPUINITCTX_SYS_REGS_BASE		CPUINITCTX_CALLEE_SAVED_REGS_NR
#define CPUINITCTX_SYS_REGS_TPIDR_EL0		CPUINITCTX_SYS_REGS_BASE
#define CPUINITCTX_SYS_REGS_SP_EL0		(CPUINITCTX_SYS_REGS_BASE + 1)
#define CPUINITCTX_SYS_REGS_CPACR_EL1		(CPUINITCTX_SYS_REGS_BASE + 2)
#define CPUINITCTX_SYS_REGS_MDSCR_EL1		(CPUINITCTX_SYS_REGS_BASE + 3)
#define CPUINITCTX_SYS_REGS_TPIDR_EL1		(CPUINITCTX_SYS_REGS_BASE + 4)
#define CPUINITCTX_SYS_REGS_VBAR_EL1		(CPUINITCTX_SYS_REGS_BASE + 5)
#define CPUINITCTX_SYS_REGS_SCTLR_EL1		(CPUINITCTX_SYS_REGS_BASE + 6)
#define CPUINITCTX_SYS_REGS_TTBR0_EL1		(CPUINITCTX_SYS_REGS_BASE + 7)
#define CPUINITCTX_SYS_REGS_TTBR1_EL1		(CPUINITCTX_SYS_REGS_BASE + 8)
#define CPUINITCTX_SYS_REGS_APIAKEYLO_EL1	(CPUINITCTX_SYS_REGS_BASE + 9)
#define CPUINITCTX_SYS_REGS_APIAKEYHI_EL1	(CPUINITCTX_SYS_REGS_BASE + 10)
#define CPUINITCTX_SYS_REGS_APIBKEYLO_EL1	(CPUINITCTX_SYS_REGS_BASE + 11)
#define CPUINITCTX_SYS_REGS_APIBKEYHI_EL1	(CPUINITCTX_SYS_REGS_BASE + 12)
#define CPUINITCTX_SYS_REGS_APDAKEYLO_EL1	(CPUINITCTX_SYS_REGS_BASE + 13)
#define CPUINITCTX_SYS_REGS_APDAKEYHI_EL1	(CPUINITCTX_SYS_REGS_BASE + 14)
#define CPUINITCTX_SYS_REGS_APDBKEYLO_EL1	(CPUINITCTX_SYS_REGS_BASE + 15)
#define CPUINITCTX_SYS_REGS_APDBKEYHI_EL1	(CPUINITCTX_SYS_REGS_BASE + 16)
#define CPUINITCTX_SYS_REGS_APGAKEYLO_EL1	(CPUINITCTX_SYS_REGS_BASE + 17)
#define CPUINITCTX_SYS_REGS_APGAKEYHI_EL1	(CPUINITCTX_SYS_REGS_BASE + 18)
#define CPUINITCTX_SYS_REGS_NR			19

#define CPUINITCTX_REGS_NR			(CPUINITCTX_CALLEE_SAVED_REGS_NR + CPUINITCTX_SYS_REGS_NR)

#ifdef CONFIG_PAC
#define ARCH_CPU_LOCAL_APIB_KEY_SIZE 16
#else
#define ARCH_CPU_LOCAL_APIB_KEY_SIZE 0
#endif
#define ARCH_CPU_LOCAL_IDMAP_OFFSET		(64 + ARCH_CPU_LOCAL_APIB_KEY_SIZE)
#define ARCH_CPU_LOCAL_CPUINITCTX_OFFSET	(72 + ARCH_CPU_LOCAL_APIB_KEY_SIZE)

#ifndef __ASSEMBLY__

struct cpuinitctx_s {
	unsigned long callee_saved_regs[CPUINITCTX_CALLEE_SAVED_REGS_NR];
	unsigned long sys_regs[CPUINITCTX_SYS_REGS_NR];
	unsigned long bitmask;
};

void arch_cpu_init(void);
void arch_cpu_up_ctx_snapshot(unsigned int cpuid);
void arch_sysregs_recover(const struct cpuinitctx_s *cpuinitctx);

typedef void __noreturn (*arch_switch_stack_next_stage_t)(void);

#ifndef __HOST_LLT__
void __noreturn
arch_switch_stack(arch_switch_stack_next_stage_t next_stage,
		  void *stack);
int arch_cpuinitctx_snapshot(struct cpuinitctx_s *cpuinitctx);
void arch_hibernate_restore_cpu(void);
#ifdef CONFIG_PAC
void arch_first_enter_user(void);
#else
static inline void arch_first_enter_user(void) { }
#endif
#else
static inline void __noreturn
arch_switch_stack(arch_switch_stack_next_stage_t next_stage,
		  void *stack) { while (1) { } }
static inline int arch_cpuinitctx_snapshot(struct cpuinitctx_s *cpuinitctx)
{
	UNUSED(cpuinitctx);
	return 0;
}
static inline void arch_first_enter_user(void) { }
static inline void arch_hibernate_restore_cpu(void) { }
#endif

#endif

#endif
