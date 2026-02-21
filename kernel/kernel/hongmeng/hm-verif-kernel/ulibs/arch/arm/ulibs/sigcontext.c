/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Transmit signal context
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 03 12:16:22 2018
 */

#include <hmasm/sigcontext.h>

/*
 * 1. This func transmits arch_uctx to arch_sigctx, which is used
 * in signal handler.
 * 2. The structure 'arch_sigctx_t' should keep the same as the
 * 'mcontext_t' in hm-libc.
 * 3. This func works only when _GNU_SOURCE was defined.
 */
void arch_uctx_to_arch_sigctx(const struct arch_uctx *uctx,
			      arch_sigctx_t *sigctx)
{
	sigctx->arm_r0 = __REG_R0(&uctx->regs);
	sigctx->arm_r1 = __REG_R1(&uctx->regs);
	sigctx->arm_r2 = __REG_R2(&uctx->regs);
	sigctx->arm_r3 = __REG_R3(&uctx->regs);
	sigctx->arm_r4 = __REG_R4(&uctx->regs);
	sigctx->arm_r5 = __REG_R5(&uctx->regs);
	sigctx->arm_r6 = __REG_R6(&uctx->regs);
	sigctx->arm_r7 = __REG_R7(&uctx->regs);
	sigctx->arm_r8 = __REG_R8(&uctx->regs);
	sigctx->arm_r9 = __REG_R9(&uctx->regs);
	sigctx->arm_r10 = __REG_R10(&uctx->regs);
	sigctx->arm_fp = __REG_FP(&uctx->regs);
	sigctx->arm_ip = __REG_IP(&uctx->regs);
	sigctx->arm_sp = __REG_SP(&uctx->regs);
	sigctx->arm_lr = __REG_LR(&uctx->regs);
	sigctx->arm_pc = __REG_PC(&uctx->regs);
	sigctx->arm_cpsr = __REG_SPSR(&uctx->regs);
}

void arch_sigctx_to_arch_uctx(const arch_sigctx_t *sigctx,
			      struct arch_uctx *uctx)
{
	__REG_R0(&uctx->regs) = (__u32)sigctx->arm_r0;
	__REG_R1(&uctx->regs) = (__u32)sigctx->arm_r1;
	__REG_R2(&uctx->regs) = (__u32)sigctx->arm_r2;
	__REG_R3(&uctx->regs) = (__u32)sigctx->arm_r3;
	__REG_R4(&uctx->regs) = (__u32)sigctx->arm_r4;
	__REG_R5(&uctx->regs) = (__u32)sigctx->arm_r5;
	__REG_R6(&uctx->regs) = (__u32)sigctx->arm_r6;
	__REG_R7(&uctx->regs) = (__u32)sigctx->arm_r7;
	__REG_R8(&uctx->regs) = (__u32)sigctx->arm_r8;
	__REG_R9(&uctx->regs) = (__u32)sigctx->arm_r9;
	__REG_R10(&uctx->regs) = (__u32)sigctx->arm_r10;
	__REG_FP(&uctx->regs) = (__u32)sigctx->arm_fp;
	__REG_IP(&uctx->regs) = (__u32)sigctx->arm_ip;
	__REG_SP(&uctx->regs) = (__u32)sigctx->arm_sp;
	__REG_LR(&uctx->regs) = (__u32)sigctx->arm_lr;
	__REG_PC(&uctx->regs) = (__u32)sigctx->arm_pc;
	__REG_SPSR(&uctx->regs) = (__u32)sigctx->arm_cpsr;
}
