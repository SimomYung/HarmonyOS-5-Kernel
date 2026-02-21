/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 3 00:47:11 2020
 */
#ifndef AARCH64_ULIBS_ASM_SIGCONTEXT_H
#define AARCH64_ULIBS_ASM_SIGCONTEXT_H

#include <hmasm/ucontext.h>

#define A64_SIGCONTEXT_NORMAL_REGS_NUM	31
#define A64_SIGCONTEXT_RESERVED_NUM	256

/*
 * The structure should keep the same as the 'mcontext_t'
 * in hm-libc.
 */
typedef struct arch_sigctx {
	unsigned long fault_address;
	unsigned long regs[A64_SIGCONTEXT_NORMAL_REGS_NUM];
	unsigned long sp, pc, pstate;
	long double __reserved[A64_SIGCONTEXT_RESERVED_NUM];
} arch_sigctx_t;

void arch_uctx_to_arch_sigctx(const struct arch_uctx *uctx,
			      arch_sigctx_t *sigctx);
void arch_sigctx_to_arch_uctx(const arch_sigctx_t *sigctx,
			      struct arch_uctx *uctx);

#endif
