/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: AArch64 traps
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 26 14:27:38 2022
 */

#ifndef AARCH64_HMASM_TRAPS_H
#define AARCH64_HMASM_TRAPS_H

#include <hmasm/types.h>

#include <hmkernel/compiler.h>

struct arch_uctx;

void __noreturn arch_insn_trap(struct arch_uctx *uctx, u64 esr, u64 elr, u64 far);

#endif /* !AARCH64_HMASM_TRAPS_H */
