/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: header for perf ring buffer
 * Author: Huawei OS Kernel Lab
 * Create: Sat Oct 14 14:51:01 CST 2023
 */

#ifndef KLIBS_PERF_USER_H
#define KLIBS_PERF_USER_H

#include <hmasm/registers.h>
#include <hmkernel/errno.h>

enum perf_sample_regs_abi {
	__PERF_SAMPLE_REGS_ABI_NONE	= 0,
	__PERF_SAMPLE_REGS_ABI_32	= 1,
	__PERF_SAMPLE_REGS_ABI_64	= 2,
};

#define PERF_SAMPLE_REGS_MAX __PERF_REG_ARM64_MAX
#define PERF_SAMPLE_REGS_DEF_ABI __PERF_SAMPLE_REGS_ABI_64

enum perf_user_regs {
	__PERF_REG_ARM64_X0,
	__PERF_REG_ARM64_X1,
	__PERF_REG_ARM64_X2,
	__PERF_REG_ARM64_X3,
	__PERF_REG_ARM64_X4,
	__PERF_REG_ARM64_X5,
	__PERF_REG_ARM64_X6,
	__PERF_REG_ARM64_X7,
	__PERF_REG_ARM64_X8,
	__PERF_REG_ARM64_X9,
	__PERF_REG_ARM64_X10,
	__PERF_REG_ARM64_X11,
	__PERF_REG_ARM64_X12,
	__PERF_REG_ARM64_X13,
	__PERF_REG_ARM64_X14,
	__PERF_REG_ARM64_X15,
	__PERF_REG_ARM64_X16,
	__PERF_REG_ARM64_X17,
	__PERF_REG_ARM64_X18,
	__PERF_REG_ARM64_X19,
	__PERF_REG_ARM64_X20,
	__PERF_REG_ARM64_X21,
	__PERF_REG_ARM64_X22,
	__PERF_REG_ARM64_X23,
	__PERF_REG_ARM64_X24,
	__PERF_REG_ARM64_X25,
	__PERF_REG_ARM64_X26,
	__PERF_REG_ARM64_X27,
	__PERF_REG_ARM64_X28,
	__PERF_REG_ARM64_X29,
	__PERF_REG_ARM64_LR,
	__PERF_REG_ARM64_SP,
	__PERF_REG_ARM64_PC,
	__PERF_REG_ARM64_MAX,
};

__u64 perf_reg_value(struct arch_regs *regs, unsigned int idx);
__u16 perf_ustack_size(__u16 attr_size, __u16 sample_size, struct arch_regs *regs);

static inline bool perf_reg_validate(__u64 mask)
{
	return ((mask != 0ULL) && !(mask >> __PERF_REG_ARM64_MAX));
}

#endif
