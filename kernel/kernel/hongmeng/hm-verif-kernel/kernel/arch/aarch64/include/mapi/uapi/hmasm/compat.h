/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Compat aarch32 uctx
 * Author: Huawei OS Kernel Lab
 * Create: Mon Aug 24 21:39:41 2020
 */
#ifndef AARCH64_UAPI_ASM_COMPAT_H
#define AARCH64_UAPI_ASM_COMPAT_H

#include <hmasm/registers.h>
#include <hmasm/ucontext.h>
#include <hmkernel/compiler.h>

struct aarch32_regs {
	/* 18 represents the number of registers in aarch32, the same as arm */
	__u32 __regs[18];
};

struct aarch32_uctx {
	/* make sure regs is aligned with 8 so interrupt handler never
	 * minus 4 incorrectly */
	struct aarch32_regs regs __aligned(8);
	__u64 flags;
	__u32 orig_r0;
};

struct compat_arch_fpu_regs {
	/* 32 represents the number of registers */
	__u64 vregs[32];
	__u32 fpscr;
};

/* Masks for extracting the FPSR and FPCR from the FPSCR or reverse */
#define VFP_FPSCR_STAT_MASK	0xF800009FU
#define VFP_FPSCR_CTRL_MASK	0x07FF9F00U

/* Create an AArch32 fpscr from the fpsr and the fpcr. */
static inline __u32
__arch_fpsr_fpcr_to_fpscr(__u32 fpsr, __u32 fpcr)
{
	return (fpsr & VFP_FPSCR_STAT_MASK) |
		(fpcr & VFP_FPSCR_CTRL_MASK);
}

static inline void
__arch_fpscr_to_fpsr_fpcr(__u32 fpscr, __u32 *fpsr, __u32 *fpcr)
{
	*fpsr = (fpscr & VFP_FPSCR_STAT_MASK);
	*fpcr = (fpscr & VFP_FPSCR_CTRL_MASK);
}

/*
 * The AArch32 Execution state is compatible with the Armv7-A
 * architecture profile, we can safely assume that the Advanced SIMD
 * and Floating-point extension registers set consists of 32 doublewords registers.
 */
#define VFP_REG_SIZE		(32 * 8)

/* aarch32 regs */
#define __REG32_NORMAL(regs, n)	__REG_NORMAL(regs, n)
#define __REG32_SP(regs)	__REG_NORMAL(regs, 13)
#define __REG32_LR(regs)	__REG_NORMAL(regs, 14)
#define __REG32_PC(regs)	__REG_NORMAL(regs, 15)
#define __REG32_SPSR(regs)	__REG_NORMAL(regs, 16)
#define __REG32_TPIDRURO(regs)	__REG_NORMAL(regs, 17)
#define __REG32_NORMAL_REG_NUM	13

/* aarch32 flags */
#define A32_UCONTEXT_32BIT             ((__u32)0x1UL)
#define A32_UCONTEXT_PRIVILEGE_SERVICE ((__u32)0x2UL)
#define A32_UCONTEXT_KCALL_RESTARTABLE ((__u32)0x4UL)

static inline void
__arch_reg64_to_reg32(struct aarch32_regs *reg32,
		      const struct arch_regs *reg64)
{
	/* convert regs, aarch32 save r0-r12 normal register */
	for (int idx = 0; idx < __REG32_NORMAL_REG_NUM; idx++) {
		__REG32_NORMAL(reg32, idx) = (__u32)__REG_NORMAL(reg64, idx);
	}
	__REG32_SP(reg32)       = (__u32)__REG_SP(reg64);
	__REG32_LR(reg32)       = (__u32)__REG_LR(reg64);
	__REG32_PC(reg32)       = (__u32)__REG_PC(reg64);
	__REG32_SPSR(reg32)     = (__u32)__REG_SPSR(reg64);
	__REG32_TPIDRURO(reg32) = (__u32)__REG_TPID(reg64);
}

static inline void
__arch64_flag_to_arch32_flag(__u64 *arch32_flag, __u64 arch64_flag)
{
	__u64 tmp_flag = 0u;
	tmp_flag |= (((arch64_flag & ARCH_UCONTEXT_DATA_32BIT) != 0ULL) ?
		     A32_UCONTEXT_32BIT : 0u);
	tmp_flag |= (((arch64_flag & A64_UCONTEXT_PRIVILEGE_SERVICE) != 0ULL) ?
		     A32_UCONTEXT_PRIVILEGE_SERVICE : 0u);
	tmp_flag |= (((arch64_flag & A64_UCONTEXT_KCALL_RESTARTABLE) != 0ULL) ?
		     A32_UCONTEXT_KCALL_RESTARTABLE : 0u);
	*arch32_flag = tmp_flag;
}

static inline void
arch_uctx64_to_uctx32(struct aarch32_uctx *uctx32,
		      const struct arch_uctx *uctx64)
{
	/* convert regs */
	__arch_reg64_to_reg32(&uctx32->regs, &uctx64->regs);
	/* convert flags */
	__arch64_flag_to_arch32_flag(&uctx32->flags, uctx64->flags);
	/* convert orig_r0 */
	uctx32->orig_r0 = (__u32)uctx64->orig_r0;
}

static inline void
__arch_reg32_to_reg64(struct arch_regs *reg64,
		      const struct aarch32_regs *reg32)
{
	/* convert regs, aarch32 save r0-r12 normal register */
	for (int idx = 0; idx < __REG32_NORMAL_REG_NUM; idx++) {
		__REG_NORMAL(reg64, idx) = (__u64)__REG32_NORMAL(reg32, idx);
	}
	__REG_SP(reg64)   = (__u64)__REG32_SP(reg32);
	__REG_LR(reg64)   = (__u64)__REG32_LR(reg32);
	__REG_PC(reg64)   = (__u64)__REG32_PC(reg32);
	__REG_SPSR(reg64) = (__u64)__REG32_SPSR(reg32);
	__REG_TPID(reg64) = (__u64)__REG32_TPIDRURO(reg32);
}

static inline void
__arch32_flag_to_arch64_flag(__u64 *arch64_flag, __u64 arch32_flag)
{
	__u64 tmp_flag = 0ULL;
	tmp_flag |= (((arch32_flag & A32_UCONTEXT_32BIT) != 0u) ?
		     ARCH_UCONTEXT_DATA_32BIT : 0ULL);
	tmp_flag |= (((arch32_flag & A32_UCONTEXT_PRIVILEGE_SERVICE) != 0u) ?
		     A64_UCONTEXT_KCALL_RESTARTABLE : 0ULL);
	tmp_flag |= (((arch32_flag & A32_UCONTEXT_KCALL_RESTARTABLE) != 0u) ?
		     A64_UCONTEXT_KCALL_RESTARTABLE : 0ULL);
	*arch64_flag = tmp_flag;
}

static inline void
arch_uctx32_to_uctx64(struct arch_uctx *uctx64,
		      const struct aarch32_uctx *uctx32)
{
	/* convert regs */
	__arch_reg32_to_reg64(&uctx64->regs, &uctx32->regs);
	/* convert flags */
	__arch32_flag_to_arch64_flag(&uctx64->flags, uctx32->flags);
	/* convert orig_r0 */
	uctx64->orig_r0 = (__u64)uctx32->orig_r0;
}

#endif
