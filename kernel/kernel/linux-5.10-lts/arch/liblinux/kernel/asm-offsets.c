// SPDX-License-Identifier: GPL-2.0-only
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/dma-mapping.h>
#include <linux/preempt.h>
#include <asm/thread_info.h>
#include <linux/kbuild.h>
#ifdef CONFIG_ARM64
#include <linux/arm-smccc.h>
#endif

int main(void)
{
  DEFINE(TSK_STACK,		offsetof(struct task_struct, stack));
#ifdef CONFIG_STACKPROTECTOR
  DEFINE(TSK_STACK_CANARY,	offsetof(struct task_struct, stack_canary));
#endif
  BLANK();
  DEFINE(S_X0,			offsetof(struct pt_regs, regs[0]));
  DEFINE(S_X2,			offsetof(struct pt_regs, regs[2]));
  DEFINE(S_X4,			offsetof(struct pt_regs, regs[4]));
  DEFINE(S_X6,			offsetof(struct pt_regs, regs[6]));
  DEFINE(S_X8,			offsetof(struct pt_regs, regs[8]));
  DEFINE(S_X10,			offsetof(struct pt_regs, regs[10]));
  DEFINE(S_X12,			offsetof(struct pt_regs, regs[12]));
  DEFINE(S_X14,			offsetof(struct pt_regs, regs[14]));
  DEFINE(S_X16,			offsetof(struct pt_regs, regs[16]));
  DEFINE(S_X18,			offsetof(struct pt_regs, regs[18]));
  DEFINE(S_X20,			offsetof(struct pt_regs, regs[20]));
  DEFINE(S_X22,			offsetof(struct pt_regs, regs[22]));
  DEFINE(S_X24,			offsetof(struct pt_regs, regs[24]));
  DEFINE(S_X26,			offsetof(struct pt_regs, regs[26]));
  DEFINE(S_X28,			offsetof(struct pt_regs, regs[28]));
  DEFINE(S_FP,			offsetof(struct pt_regs, regs[29]));
  DEFINE(S_LR,			offsetof(struct pt_regs, regs[30]));
  DEFINE(S_SP,			offsetof(struct pt_regs, sp));
  DEFINE(S_PSTATE,		offsetof(struct pt_regs, pstate));
  DEFINE(S_PC,			offsetof(struct pt_regs, pc));
  DEFINE(S_SYSCALLNO,		offsetof(struct pt_regs, syscallno));
  DEFINE(S_ORIG_ADDR_LIMIT,	offsetof(struct pt_regs, orig_addr_limit));
  DEFINE(S_PMR_SAVE,		offsetof(struct pt_regs, pmr_save));
  DEFINE(S_STACKFRAME,		offsetof(struct pt_regs, stackframe));
  DEFINE(S_FRAME_SIZE,		sizeof(struct pt_regs));
  BLANK();
  BLANK();
  DEFINE(PAGE_SZ,	       	PAGE_SIZE);
  BLANK();
  DEFINE(DMA_TO_DEVICE,		DMA_TO_DEVICE);
  DEFINE(DMA_FROM_DEVICE,	DMA_FROM_DEVICE);
  BLANK();
  DEFINE(PREEMPT_DISABLE_OFFSET, PREEMPT_DISABLE_OFFSET);
  DEFINE(SOFTIRQ_SHIFT, SOFTIRQ_SHIFT);
#ifdef CONFIG_ARM64
  DEFINE(ARM_SMCCC_RES_X0_OFFS,		offsetof(struct arm_smccc_res, a0));
  DEFINE(ARM_SMCCC_RES_X2_OFFS,		offsetof(struct arm_smccc_res, a2));
  DEFINE(ARM_SMCCC_QUIRK_ID_OFFS,	offsetof(struct arm_smccc_quirk, id));
  DEFINE(ARM_SMCCC_QUIRK_STATE_OFFS,	offsetof(struct arm_smccc_quirk, state));
#endif
  DEFINE(ARM_SMCCC_1_2_REGS_X0_OFFS,	offsetof(struct arm_smccc_1_2_regs, a0));
  DEFINE(ARM_SMCCC_1_2_REGS_X2_OFFS,	offsetof(struct arm_smccc_1_2_regs, a2));
  DEFINE(ARM_SMCCC_1_2_REGS_X4_OFFS,	offsetof(struct arm_smccc_1_2_regs, a4));
  DEFINE(ARM_SMCCC_1_2_REGS_X6_OFFS,	offsetof(struct arm_smccc_1_2_regs, a6));
  DEFINE(ARM_SMCCC_1_2_REGS_X8_OFFS,	offsetof(struct arm_smccc_1_2_regs, a8));
  DEFINE(ARM_SMCCC_1_2_REGS_X10_OFFS,	offsetof(struct arm_smccc_1_2_regs, a10));
  DEFINE(ARM_SMCCC_1_2_REGS_X12_OFFS,	offsetof(struct arm_smccc_1_2_regs, a12));
  DEFINE(ARM_SMCCC_1_2_REGS_X14_OFFS,	offsetof(struct arm_smccc_1_2_regs, a14));
  DEFINE(ARM_SMCCC_1_2_REGS_X16_OFFS,	offsetof(struct arm_smccc_1_2_regs, a16));
  return 0;
}
