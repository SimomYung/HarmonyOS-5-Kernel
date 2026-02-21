/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Definition of ucontext for modules
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 23 10:53:46 2021
 */
#ifndef MAPI_ASM_UCONTEXT_H
#define MAPI_ASM_UCONTEXT_H

#define __ARCH_REGS_PAUTH_IA 0
#define __ARCH_REGS_PAUTH_IB 1
#define __ARCH_REGS_PAUTH_DA 2
#define __ARCH_REGS_PAUTH_GA 3 /* this is GA, not DB */
#define __ARCH_REGS_PAUTH_DB 4 /* DB is not set at context switch */

#define __ARCH_REGS_PAUTH_LO(reg) ((reg) << 1)
#define __ARCH_REGS_PAUTH_HI(reg) (((reg) << 1) + 1)

#ifndef __ASSEMBLY__

#include <hmkernel/types.h>
#include <uapi/hmasm/ucontext.h>

static inline bool
arch_uctx_is_privileged(const struct arch_uctx *ctx)
{
#ifdef CONFIG_PRIVILEGED_SERVICE
	return ((ctx->flags & A64_UCONTEXT_PRIVILEGE_SERVICE) != 0);
#else
	return false;
#endif
}

#define __ARCH_REGS_PAUTH 10
struct arch_secret_uctx {
	__u64 pauth_regs[__ARCH_REGS_PAUTH];
	__u64 sctlr;
};

#define __REG_APIAKeyLo(suctx)	((suctx)->pauth_regs[__ARCH_REGS_PAUTH_LO(__ARCH_REGS_PAUTH_IA)])
#define __REG_APIAKeyHi(suctx)	((suctx)->pauth_regs[__ARCH_REGS_PAUTH_HI(__ARCH_REGS_PAUTH_IA)])
#define __REG_APIBKeyLo(suctx)	((suctx)->pauth_regs[__ARCH_REGS_PAUTH_LO(__ARCH_REGS_PAUTH_IB)])
#define __REG_APIBKeyHi(suctx)	((suctx)->pauth_regs[__ARCH_REGS_PAUTH_HI(__ARCH_REGS_PAUTH_IB)])
#define __REG_APDAKeyLo(suctx)	((suctx)->pauth_regs[__ARCH_REGS_PAUTH_LO(__ARCH_REGS_PAUTH_DA)])
#define __REG_APDAKeyHi(suctx)	((suctx)->pauth_regs[__ARCH_REGS_PAUTH_HI(__ARCH_REGS_PAUTH_DA)])
#define __REG_APDBKeyLo(suctx)	((suctx)->pauth_regs[__ARCH_REGS_PAUTH_LO(__ARCH_REGS_PAUTH_DB)])
#define __REG_APDBKeyHi(suctx)	((suctx)->pauth_regs[__ARCH_REGS_PAUTH_HI(__ARCH_REGS_PAUTH_DB)])
#define __REG_APGAKeyLo(suctx)	((suctx)->pauth_regs[__ARCH_REGS_PAUTH_LO(__ARCH_REGS_PAUTH_GA)])
#define __REG_APGAKeyHi(suctx)	((suctx)->pauth_regs[__ARCH_REGS_PAUTH_HI(__ARCH_REGS_PAUTH_GA)])
#define __REG_SCTLR(suctx)	((suctx)->sctlr)

#endif

#endif
