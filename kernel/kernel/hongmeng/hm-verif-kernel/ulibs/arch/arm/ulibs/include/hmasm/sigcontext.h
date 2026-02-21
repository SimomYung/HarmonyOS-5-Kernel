/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Signal context header
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 3 00:47:11 2020
 */
#ifndef ARM_ULIBS_ASM_SIGCONTEXT_H
#define ARM_ULIBS_ASM_SIGCONTEXT_H

#include <hmasm/ucontext.h>

/*
 * The structure should keep the same as the 'mcontext_t'
 * in hm-libc.
 */
typedef struct arch_sigctx {
	unsigned long trap_no, error_code, oldmask;
	unsigned long arm_r0, arm_r1, arm_r2, arm_r3;
	unsigned long arm_r4, arm_r5, arm_r6, arm_r7;
	unsigned long arm_r8, arm_r9, arm_r10, arm_fp;
	unsigned long arm_ip, arm_sp, arm_lr, arm_pc;
	unsigned long arm_cpsr, fault_address;
} arch_sigctx_t;

void arch_uctx_to_arch_sigctx(const struct arch_uctx *uctx,
			      arch_sigctx_t *sigctx);
void arch_sigctx_to_arch_uctx(const arch_sigctx_t *sigctx,
			      struct arch_uctx *uctx);

#endif
