/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 3 00:47:11 2020
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
	for (int idx = 0; idx < A64_SIGCONTEXT_NORMAL_REGS_NUM; idx++) {
		sigctx->regs[idx] = __REG_NORMAL(&uctx->regs, idx);
	}
	sigctx->sp = __REG_SP(&uctx->regs);
	sigctx->pc = __REG_PC(&uctx->regs);
	sigctx->pstate = __REG_SPSR(&uctx->regs);
}

void arch_sigctx_to_arch_uctx(const arch_sigctx_t *sigctx,
			      struct arch_uctx *uctx)
{
	for (int idx = 0; idx < A64_SIGCONTEXT_NORMAL_REGS_NUM; idx++) {
		__REG_NORMAL(&uctx->regs, idx) = sigctx->regs[idx];
	}
	__REG_SP(&uctx->regs) = sigctx->sp;
	__REG_PC(&uctx->regs) = sigctx->pc;
	__REG_SPSR(&uctx->regs) = sigctx->pstate;
}
