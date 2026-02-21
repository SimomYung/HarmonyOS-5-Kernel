/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 23 00:47:11 2018
 */
#ifndef ASM_UCONTEXT_H
#define ASM_UCONTEXT_H

#include <hmasm/processor.h>
#include <hmkernel/types.h>
#include <hmkernel/memory.h>
#include <hmkernel/kernel.h>
#include <mapi/hmasm/ucontext.h>

#include <hmkernel/pac_func.h>

#define UCTX_REG_ITEM_OF(reg_nr) regs.__regs[reg_nr]

#if defined(PAC_DFI_KERNEL) || defined(PAC_DFI_UCTX_USER)
static inline u64 arch_uctx_get_pac(const struct arch_uctx *ctx)
{
	return A64_UCONTEXT_FLAG_PAC_OF(ctx->flags);
}

static inline void arch_uctx_set_pac(struct arch_uctx *ctx, u64 pac)
{
	ctx->flags = A64_UCONTEXT_FLAG_SET_PAC(ctx->flags, pac);
}
#endif

GEN_PAC_UCTX_VERIFIER(struct arch_uctx, arch_uctx, A64_UCONTEXT_FLAG_PAC_BITS,
		arch_uctx_get_pac, arch_uctx_set_pac,
		UCTX_REG_ITEM_OF(__REG_NR_LR), UCTX_REG_ITEM_OF(__REG_NR_PC),
		UCTX_REG_ITEM_OF(__REG_NR_SP), UCTX_REG_ITEM_OF(__REG_NR_SPSR));

GEN_PAC_UCTX_USER_VERIFIER(struct arch_uctx, arch_uctx_user, A64_UCONTEXT_FLAG_PAC_BITS,
		arch_uctx_get_pac, arch_uctx_set_pac,
		UCTX_REG_ITEM_OF(__REG_NR_LR), UCTX_REG_ITEM_OF(__REG_NR_PC),
		UCTX_REG_ITEM_OF(__REG_NR_SP), UCTX_REG_ITEM_OF(__REG_NR_SPSR));

static inline struct arch_uctx *
arch_uctx_of_regs(struct arch_regs *regs)
{
	return container_of(regs, struct arch_uctx, regs);
}

static inline struct arch_regs *
arch_uctx_regs_of(struct arch_uctx *uctx)
{
	return &uctx->regs;
}

static inline void
arch_uctx_setpc(struct arch_uctx *ctx, vaddr_t pc)
{
	REG_PC(&ctx->regs) = (u64)(pc);
	arch_uctx_pacga(ctx);
}

static inline void
arch_uctx_setsp(struct arch_uctx *ctx, vaddr_t sp)
{
	REG_SP(&ctx->regs) = (u64)(sp);
	arch_uctx_pacga(ctx);
}

static inline void
arch_uctx_setfp(struct arch_uctx *ctx, vaddr_t fp)
{
	REG_FP(&ctx->regs) = (u64)(fp);
	arch_uctx_pacga(ctx);
}

static inline void
arch_uctx_setlr(struct arch_uctx *ctx, vaddr_t lr)
{
	REG_LR(&ctx->regs) = (u64)(lr);
	arch_uctx_pacga(ctx);
}

static inline void
arch_uctx_settls(struct arch_uctx *ctx, vaddr_t tls)
{
	REG_TLS(&ctx->regs) = (u64)(tls);
}

static inline void
__arch_uctx_setret(struct arch_uctx *uctx, u64 retval)
{
	REG_X0(&uctx->regs) = retval;
}

static inline void
arch_uctx_setret_ul(struct arch_uctx *uctx, unsigned long retval)
{
	__arch_uctx_setret(uctx, (u64)(retval));
}

static inline void
arch_uctx_setret_l(struct arch_uctx *uctx, long retval)
{
	__arch_uctx_setret(uctx, (u64)(unsigned long)(retval));
}

static inline void
arch_uctx_setret(struct arch_uctx *uctx, int retval)
{
	__arch_uctx_setret(uctx, (u64)(unsigned long)(retval));
}

static inline void
arch_uctx_setret_long(struct arch_uctx *uctx, long retval)
{
	REG_X0(&uctx->regs) = (u64)(unsigned long)(retval);
}

static inline long
arch_uctx_retval_of(const struct arch_uctx *uctx)
{
	return (long)REG_X0(&uctx->regs);
}

static inline unsigned int
arch_uctx_scno_of(const struct arch_uctx *ctx)
{
	return (unsigned int)A64_UCONTEXT_FLAG_SCNO_OF(ctx->flags);
}

static inline void
arch_uctx_set_kcall_restartable(struct arch_uctx *ctx)
{
	 ctx->flags |= A64_UCONTEXT_KCALL_RESTARTABLE;
}

static inline void
__arch_uctx_clr_kcall_restartable(struct arch_uctx *ctx)
{
	 ctx->flags &= ~(__u64)(A64_UCONTEXT_KCALL_RESTARTABLE);
}

static inline bool
arch_uctx_is_kcall_restartable(const struct arch_uctx *ctx)
{
	return  ((ctx->flags & A64_UCONTEXT_KCALL_RESTARTABLE) != (__u64)0UL) ? true : false;
}

static inline void
arch_uctx_set_scno(struct arch_uctx *ctx, unsigned int scno)
{
	ctx->flags = A64_UCONTEXT_FLAG_SET_SCNO(ctx->flags, (__u64)scno);
}

static inline void
arch_uctx_clr_scno(struct arch_uctx *ctx)
{
	arch_uctx_set_scno(ctx, A64_UCONTEXT_FLAG_SCNO_INVALID);
	__arch_uctx_clr_kcall_restartable(ctx);
}

static inline void
arch_uctx_backup_r0(struct arch_uctx *uctx)
{
	uctx->orig_r0 = __REG_ARG0(&uctx->regs);
}

static inline void
arch_uctx_restore_r0(struct arch_uctx *uctx)
{
	__REG_ARG0(&uctx->regs) = uctx->orig_r0;
}

#define A64_INSN_SIZE	4
#define T32_INSN_SIZE	2

#ifdef CONFIG_COMPAT
static inline bool
arch_uctx_is_thumb(const struct arch_uctx *ctx)
{
	return __IS(REG_SPSR(&ctx->regs) & PSR_T_BIT);
}
#else
static inline bool
arch_uctx_is_thumb(const struct arch_uctx *ctx)
{
	UNUSED(ctx);
	return false;
}
#endif

static inline bool arch_uctx_is_user_mode(const struct arch_uctx *uctx)
{
	return __IS((REG_SPSR(&uctx->regs) & PSR_M_MASK) == PSR_M_EL0T);
}

static inline void
arch_uctx_rewind_pc(struct arch_uctx *ctx)
{
	__u64 pc = REG_PC(&ctx->regs);
	__u32 sz = arch_uctx_is_thumb(ctx) ? U(T32_INSN_SIZE) : U(A64_INSN_SIZE);

	REG_PC(&ctx->regs) = pc - sz;
	arch_uctx_pacga(ctx);
}

static inline __u64
arch_uctx_last_pc(struct arch_uctx *ctx)
{
	__u64 pc = REG_PC(&ctx->regs);
	__u64 sz = arch_uctx_is_thumb(ctx) ? U(T32_INSN_SIZE) : U(A64_INSN_SIZE);
	return pc - sz;
}

static inline bool
arch_uctx_is_ucontext_32bit(const struct arch_uctx *ctx)
{
	return ((ctx->flags & ARCH_UCONTEXT_DATA_32BIT) != 0UL);
}

static inline void
arch_uctx_set_need_sync_cred(struct arch_uctx *ctx)
{
	ctx->flags |= ARCH_UCONTEXT_NEED_SYNC_CRED;
}

static inline void
arch_uctx_clr_need_sync_cred(struct arch_uctx *ctx)
{
	ctx->flags &= ~(__u64)ARCH_UCONTEXT_NEED_SYNC_CRED;
}

static inline bool
arch_uctx_is_need_sync_cred(const struct arch_uctx *ctx)
{
	return (ctx->flags & ARCH_UCONTEXT_NEED_SYNC_CRED) != (__u64)0UL ? true : false;
}

#ifdef CONFIG_COMPAT
static inline bool arch_uctx_is_compat(struct arch_uctx *uctx)
{
	return __IS(uctx->flags & ARCH_UCONTEXT_EXEC_32BIT);
}
#else
static inline bool arch_uctx_is_compat(struct arch_uctx *uctx)
{
	return false;
}
#endif

struct ctrlmem_kobj_vspace_s;
extern void arch_uctx_init(struct arch_uctx *uctx, const struct ctrlmem_kobj_vspace_s *kobjvs,
			   bool is_system_service);

void arch_uctx_copy_from_user(struct arch_uctx *dst, const struct arch_uctx *src);

extern int arch_uctx_fetch_opcode(const struct arch_uctx *uctx, u32 *inst);

#endif
