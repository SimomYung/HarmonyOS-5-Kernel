/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Kernel pretch abort fault
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 7 09:58:12 2020
 */

#ifndef A64_ASM_PERMISSION_FAULT_H
#define A64_ASM_PERMISSION_FAULT_H

#include <hmkernel/compiler.h>
#include <hmasm/registers.h>

extern void kernel_inst_abort(const struct arch_regs *regs, u64 esr, u64 far);
extern void kernel_pac_fault(struct arch_regs *regs, u64 esr);

#endif
