/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: arch uctx for lsyscall
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 06 09:43:06 2023
 */
#ifndef ASM_LSYSCALL_UCONTEXT_H
#define ASM_LSYSCALL_UCONTEXT_H

#include <hmasm/ucontext.h>
#include <hmasm/cpu_local.h>
#include <hmkernel/types.h>
#include <hmkernel/memory.h>
#include <hmkernel/kernel.h>
#include <hmkernel/lsyscall.h>
#include <hmkernel/activation/struct.h>

static inline void
arch_uctx_clr_lsyscall_flag(struct arch_uctx *ctx)
{
	ctx->flags &= ~(__u64)(ARCH_UCONTEXT_LSYSCALL);
}

static inline void
arch_uctx_set_lsyscall_flag(struct arch_uctx *ctx)
{
	ctx->flags |= ARCH_UCONTEXT_LSYSCALL;
}

static inline void
arch_uctx_set_lsyscall_scno(struct arch_uctx *ctx, unsigned int lcallno)
{
	ctx->flags = A64_UCONTEXT_FLAG_SET_LSCNO(ctx->flags, (__u64)lcallno);
	arch_uctx_set_lsyscall_flag(ctx);
}

static inline void
arch_uctx_clr_lsyscall_scno(struct arch_uctx *ctx)
{
	ctx->flags = A64_UCONTEXT_FLAG_SET_LSCNO(ctx->flags, (__u64)A64_UCONTEXT_FLAG_LSCNO_INVALID);
	arch_uctx_clr_lsyscall_flag(ctx);
	__arch_uctx_clr_kcall_restartable(ctx);
}

static inline unsigned int
arch_uctx_lsyscall_scno_of(const struct arch_uctx *ctx)
{
	return (unsigned int)A64_UCONTEXT_FLAG_LSCNO_OF(ctx->flags);
}

static inline bool
arch_uctx_is_lsyscall(const struct arch_uctx *ctx)
{
	return ((ctx->flags & ARCH_UCONTEXT_LSYSCALL) != 0);
}

static inline unsigned int arch_uctx_lscno_from_abi_regs(const struct arch_uctx *uctx)
{
	/*
	 * Architecture calling conventions, reference 'man syscall':
	 * Arch/ABI    Instruction           System  Ret  Ret  Error    Notes
	 *                                   call #  val  val2
	 * ───────────────────────────────────────────────────────────────────
	 * arm64       svc #0                x8      x0   x1   -
	 */
	unsigned int lscno = (unsigned int)__REG_X8(&uctx->regs);
	return lscno;
}

static inline unsigned int arch_uctx_lscno_from_abi_regs_compat(const struct arch_uctx *uctx)
{
	unsigned int lscno;

	/*
	 * Architecture calling conventions, reference 'man syscall':
	 * Arch/ABI    Instruction           System  Ret  Ret  Error    Notes
	 *                                   call #  val  val2
	 * ───────────────────────────────────────────────────────────────────
	 * arm/EABI    swi 0x0               r7      r0   r1   -
	 * arm64       svc #0                x8      x0   x1   -
	 */

	if ((uctx->flags & ARCH_UCONTEXT_EXEC_32BIT) != 0ULL) {
		lscno = (unsigned int)__REG_X7(&uctx->regs);
	} else {
		lscno = (unsigned int)__REG_X8(&uctx->regs);
	}

	return lscno;
}

static inline void
arch_uctx_set_lscno_to_abi_regs(struct arch_uctx *uctx, unsigned int lscno,
				unsigned int lscno_compat)
{
	if (__IS(uctx->flags & ARCH_UCONTEXT_EXEC_32BIT)) {
		__REG_X7(&uctx->regs) = lscno_compat;
	} else {
		__REG_X8(&uctx->regs) = lscno;
	}
}

void arch_uctx_copy_lsyscall_stack_fastpath(struct actv_s *callee_actv,
					    struct actv_s *caller_actv,
					    struct arch_cpu_local_s *cpu_local,
					    unsigned int stack_num,
					    enum lsyscall_type type);

void arch_uctx_copy_lsyscall_regs(struct arch_uctx *callee_uctx,
				  const struct arch_uctx *caller_uctx,
				  enum lsyscall_type type);

int arch_uctx_copy_lsyscall_args_to_stack(struct actv_s *callee_actv,
					  struct actv_s *caller_actv,
					  unsigned int args_on_stack,
					  enum lsyscall_type type);
#endif
