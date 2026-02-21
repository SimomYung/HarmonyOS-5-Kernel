/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Interface of insn
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 08 14:27:17 2023
 */
#ifndef AARCH64_ASM_INSN_H
#define AARCH64_ASM_INSN_H

#include <hmasm/types.h>
#include <hmkernel/strict.h>

#ifdef CONFIG_JUMP_LABEL
void arch_jump_label_write_insn(uptr_t va, const unsigned char *insn);
#else
static inline void arch_jump_label_write_insn(uptr_t va, const unsigned char *insn)
{
	UNUSED(va, insn);
}
#endif /* CONFIG_JUMP_LABEL */
#endif /* AARCH64_ASM_INSN_H */
