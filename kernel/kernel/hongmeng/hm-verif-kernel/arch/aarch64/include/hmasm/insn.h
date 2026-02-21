/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Header for AArch64 instruction decoding
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 09 17:16:12 2024
 */
#ifndef AARCH64_HMASM_INSN_H
#define AARCH64_HMASM_INSN_H

#include <hmasm/types.h>

enum aarch64_insn_imm_type {
	AARCH64_INSN_IMM_ADR,
	AARCH64_INSN_IMM_6,
	AARCH64_INSN_IMM_7,
	AARCH64_INSN_IMM_9,
	AARCH64_INSN_IMM_12,
	AARCH64_INSN_IMM_14,
	AARCH64_INSN_IMM_16,
	AARCH64_INSN_IMM_19,
	AARCH64_INSN_IMM_26,
};

enum aarch64_insn_reg_type {
	AARCH64_INSN_REGTYPE_RT,
};

/* caller should gurantee the type valid, otherwise default vaule returned */
u32 aarch64_insn_decode_imm(enum aarch64_insn_imm_type type, u32 insn);
u32 aarch64_insn_decode_reg(enum aarch64_insn_reg_type type, u32 insn);

#endif
