/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2021. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 23 00:47:11 2018
 */
#ifndef AARCH64_UAPI_ASM_UCONTEXT_H
#define AARCH64_UAPI_ASM_UCONTEXT_H

#include <hmkernel/types.h>
#include <hmasm/registers.h>

/*
 * layout of arch_uctx->flags (64 bits)
 * bit 0: ARCH_UCONTEXT_DATA_32BIT
 * bit 1: A64_UCONTEXT_PRIVILEGE_SERVICE
 * bit 2: A64_UCONTEXT_KCALL_RESTARTABLE
 * bit 3: ARCH_UCONTEXT_NEED_SYNC_CRED
 * bit 4: ARCH_UCONTEXT_EXEC_32BIT
 * bit 5: ARCH_UCONTEXT_LSYSCALL
 * bit 6: ARCH_UCONTEXT_SUPERVS
 * bit 7-16: 10 bits, lcallno (Linux Syscall Number)
 * bit 17-31: reserved or pac if enabled
 * bit 32-62: hcallno (HongMeng Syscall Number)
 * bit 63: ARCH_UCONTEXT_SYSTEM_SERVICE
 */

/* Set DATA_32BIT and EXEC_32BIT flag to indicate that is AArch32 process.
 * Only set DATA_32BIT flag to indicate that is ILP32 process.
 * Neither set DATA_32BIT nor EXEC_32BIT to indicate that is AArch64 process.
 */
#define ARCH_UCONTEXT_DATA_32BIT	((__u64)0x1UL)
#define ARCH_UCONTEXT_EXEC_32BIT	((__u64)0x10UL)
#define A64_UCONTEXT_PRIVILEGE_SERVICE	((__u64)0x2UL)
/*
 * The flag set to indicate that the tcb in the BLOCKED state is just woken up
 * due to an interrupt, and the tcb is able to restart the syscall or instruction
 * that blocked it by rewinding the pc. This is a kernel-internal flag, which
 * should be cleared before restoring user context.
 *
 * Currently, the kcall restartable flag is mainly used to help handle the
 * HMSIGNAL_ALARM: when the POSIX signal in sysmgr corresponding to the HMSIGNAL_ALARM
 * is masked, then the tcb raising the HMSIGNAL_ALARM should:
 *
 * - If the kcall restartable flag is true, it means the tcb was in BLOCKED
 *   state and woken up to handle the HMSIGNAL_ALARM. Then the tcb should
 *   rewind the pc to restore the original state of it => BLOCKED.
 *
 * - If the kcall restartable flag is false, then the tcb should do nothing and
 *   continue to execute based on the original context.
 */
#define A64_UCONTEXT_KCALL_RESTARTABLE	((__u64)0x4UL)

#define ARCH_UCONTEXT_NEED_SYNC_CRED ((__u64)0x8UL)
/*
 * Set ARCH_UCONTEXT_LSYSCALL flag to flags, which means in
 * context of lsyscall
 */
#define ARCH_UCONTEXT_LSYSCALL	((__u64)0x20UL)

#define ARCH_UCONTEXT_SUPERVS		((__u64)0x40UL)

/* Set ARCH_UCONTEXT_SYSTEM_SERVICE to flags, which means uctx is a system service. */
#define ARCH_UCONTEXT_SYSTEM_SERVICE	((__u64)0x1 << 63)
/*
 * make scno aligned with 32 bits so compiler is able to generate better insns.
 * See el0_sync, scno can be 0 ~ 0xffff. 0 is Linux syscall, so we make 0xffff
 * a invalid a syscall number.
 */
#define A64_UCONTEXT_FLAG_SCNO_OF(x)			\
	((__u64)(((x) >> 32u) & 0xffffULL))
#define A64_UCONTEXT_FLAG_SET_SCNO(flags, scno)		\
	(((flags) & ~(0xffffULL << 32u)) | (((scno) & 0xffffULL) << 32u))
#define A64_UCONTEXT_FLAG_SCNO_INVALID 0xffff

#define A64_UCONTEXT_FLAG_LSCNO_OF(x)			\
	((__u64)(((x) >> 7u) & 0x3ffULL))
#define A64_UCONTEXT_FLAG_SET_LSCNO(flags, lscno)		\
	(((flags) & ~(0x3ffULL << 7u)) | (((lscno) & 0x3ffULL) << 7u))
#define A64_UCONTEXT_FLAG_LSCNO_INVALID 0x3ff

#define A64_UCONTEXT_FLAG_PAC_OF(x)			\
	((__u64)(((x) >> 17u) & 0x7fffULL))
#define A64_UCONTEXT_FLAG_SET_PAC(flags, pac)		\
	(((flags) & ~(0x7fffULL << 17u)) | (((pac) & 0x7fffULL) << 17u))
#define A64_UCONTEXT_FLAG_PAC_BITS 15

struct arch_uctx {
	struct arch_regs regs;
	__u64 flags;
	__u64 orig_r0;
};

struct arch_clone_regs {
	__u64 sp;
	__u64 tls;
	__u64 pc;
	__u64 arg0;
	__u64 ctx_flags;
};

/*
 * These called-saved regs will be used in actv rpc entry.
 * So backup them of pre alloc actv for uspinhdlr fallback.
 */
struct arch_rpc_regs {
	__u64 x25;
	__u64 x26;
	__u64 x27;
	__u64 x28;
	__u64 x29;
	__u64 pc;
	__u64 sp;
};

static inline void
arch_rpc_regs_backup(struct arch_uctx *ctx, struct arch_rpc_regs *backup_regs)
{
	backup_regs->x25 = __REG_X25(&ctx->regs);
	backup_regs->x26 = __REG_X26(&ctx->regs);
	backup_regs->x27 = __REG_X27(&ctx->regs);
	backup_regs->x28 = __REG_X28(&ctx->regs);
	backup_regs->x29 = __REG_FP(&ctx->regs);
	backup_regs->pc = __REG_PC(&ctx->regs);
	backup_regs->sp = __REG_SP(&ctx->regs);
}

static inline void
arch_clone_regs_set_privileged_service(struct arch_clone_regs *clone_regs)
{
	clone_regs->ctx_flags |= A64_UCONTEXT_PRIVILEGE_SERVICE;
}

static inline void
arch_clone_regs_flush_uctx_flags(struct arch_clone_regs *clone_regs, __u32 flags)
{
	clone_regs->ctx_flags &= (~(ARCH_UCONTEXT_DATA_32BIT | ARCH_UCONTEXT_EXEC_32BIT));
	clone_regs->ctx_flags |= (flags & (ARCH_UCONTEXT_DATA_32BIT | ARCH_UCONTEXT_EXEC_32BIT));
}

static inline void
arch_uctx_set_privileged_service(struct arch_uctx *ctx)
{
	ctx->flags |= A64_UCONTEXT_PRIVILEGE_SERVICE;
}

static inline void
arch_uctx_set_supervs(struct arch_uctx *ctx)
{
	ctx->flags |= ARCH_UCONTEXT_SUPERVS;
}

static inline __bool
arch_uctx_is_supervs(const struct arch_uctx *ctx)
{
	return (ctx->flags & ARCH_UCONTEXT_SUPERVS) ? __true : __false;
}

static inline void
arch_flush_uctx_flags(struct arch_uctx *ctx, __u64 flags)
{
	ctx->flags &= (~(ARCH_UCONTEXT_DATA_32BIT | ARCH_UCONTEXT_EXEC_32BIT));
	ctx->flags |= (flags & (ARCH_UCONTEXT_DATA_32BIT | ARCH_UCONTEXT_EXEC_32BIT));
}

static inline void
arch_uctx_set_default_arch_flags(struct arch_uctx *ctx)
{
	/* clear ARCH_UCONTEXT_DATA_32BIT and ARCH_UCONTEXT_EXEC_32BIT for aarch64 */
	ctx->flags &= (~(ARCH_UCONTEXT_DATA_32BIT | ARCH_UCONTEXT_EXEC_32BIT));
}

static inline void
arch_uctx_set_system_service(struct arch_uctx *ctx)
{
	ctx->flags |= ARCH_UCONTEXT_SYSTEM_SERVICE;
}

static inline void
arch_uctx_clr_system_service(struct arch_uctx *ctx)
{
	ctx->flags &= (~ARCH_UCONTEXT_SYSTEM_SERVICE);
}

#define __ARCH_UCTX_ZEROED {__ARCH_REGS_ZEROED, (__u64)0, (__u64)0}
#define DEFINE_ARCH_UCTX(name) struct arch_uctx name = __ARCH_UCTX_ZEROED

#define __ARCH_UCTX_TLS_MINIMAL_SIZE	4096

#endif
