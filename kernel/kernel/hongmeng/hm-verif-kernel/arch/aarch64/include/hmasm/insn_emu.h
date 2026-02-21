/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2025. All rights reserved.
 * Description: AArch64 insn_emu
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 16 17:39:32 2025
 */

#ifndef AARCH64_HMASM_INSN_EMU_H
#define AARCH64_HMASM_INSN_EMU_H

#include <hmasm/registers.h>

/*
 * In AArch64, both XZR and SP use register index 31.
 * This handles the {read, write} regs where 31 means XZR, not SP.
 */
#define __AARCH64_REG_NR_XZR		31

static inline void arch_emuinsn_write_reg(struct arch_regs *regs, unsigned int r, unsigned long val)
{
	if (r != __AARCH64_REG_NR_XZR) {
		regs->__regs[r] = val;
	}
}

static inline unsigned long arch_emuinsn_read_reg(const struct arch_regs *regs, unsigned int r)
{
	return (r == __AARCH64_REG_NR_XZR) ? 0UL : regs->__regs[r];
}

#endif
