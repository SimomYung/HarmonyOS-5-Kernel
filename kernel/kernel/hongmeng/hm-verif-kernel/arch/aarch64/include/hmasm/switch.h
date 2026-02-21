/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Description: The header file for switch user context
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 24 11:40:43 2018
 */
#ifndef A64_ASM_SWITCH_H
#define A64_ASM_SWITCH_H

#include <hmkernel/compiler.h>
#include <hmkernel/strict.h>
#include <hmasm/cpu_local.h>

extern void __noreturn
arch_restore_user_context(struct arch_uctx *ctx);

/*
 * Used when server actv exit without syscall, such as:
 * spinhdlr_ptlwfe_commit/spinhdlr_ptlwfe_exphdlr, where arch_restore_user_context_uspin
 * is invoked,
 * and spinhdlr_dfrwfe, where fastpath_defered_prepare_wait_for_event is invoked.
 * In these scenarios, We need sync spsr info explicitly.
 *
 * Should be used only under SPIN and spinhdlr related process.
 */
static inline void
arch_clean_sysreg_uspin_exit(struct arch_cpu_local_s *cpu_local,
			     struct arch_uctx *ctx)
{
	REG_SPSR(&ctx->regs) &= (~PSR_I_BIT);
	arch_cpu_clr_restore_action(cpu_local,
				ARCH_RESTORE_FLAG_A64_IRQ_OFF);
}

static inline void
arch_restore_user_context_uspin(struct arch_cpu_local_s *cpu_local,
				struct arch_uctx *pre_ctx, struct arch_uctx *next_ctx)
{
	arch_clean_sysreg_uspin_exit(cpu_local, pre_ctx);
	arch_restore_user_context(next_ctx);
}

extern void __noreturn
arch_restore_kernel_context(const struct arch_regs *regs);

static inline void
arch_uctx_sysreg_amend(struct arch_uctx *ctx, const struct arch_cpu_local_s *cpu_local)
{
	arch_restore_daif(ctx, cpu_local);
}

extern void __noreturn
aarch64_context_switch(struct arch_regs *reg, unsigned long spsr);

#ifdef CONFIG_LASP_WP
extern void __noreturn
aarch64_context_switch_lasp(struct arch_regs *reg, unsigned long spsr);
#endif

/* allow some special driver mask irq/fiq separately */
void aarch64_mask_local_irq(void);
void aarch64_mask_local_fiq(void);
void aarch64_unmask_local_irq(void);
void aarch64_unmask_local_fiq(void);

#endif
