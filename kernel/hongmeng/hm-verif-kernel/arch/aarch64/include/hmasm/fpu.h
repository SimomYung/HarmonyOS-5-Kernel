/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: FPU head file
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 23 06:49:43 2019
 */
#ifndef AARCH64_ASM_FPU_H
#define AARCH64_ASM_FPU_H

#include <hmasm/exception.h>
#include <hmasm/registers.h>

static inline void
arch_fpu_set_using(struct arch_fpu_regs *fpu)
{
	fpu->flags |= __A64_FPU_REGS_FLAG_IS_USING;
}

static inline void
arch_fpu_clr_using(struct arch_fpu_regs *fpu)
{
	fpu->flags &= ~(__A64_FPU_REGS_FLAG_IS_USING);
}

static inline bool
arch_fpu_is_using(const struct arch_fpu_regs *fpu)
{
	return ((fpu->flags & __A64_FPU_REGS_FLAG_IS_USING) != 0U);
}

void arch_fpu_init(void);
void arch_fpu_save(struct arch_fpu_regs *fpu);
void arch_actv_pop_fpu_save(struct arch_fpu_regs *fpu);
void arch_fpu_restore(struct arch_fpu_regs *fpu, void *owner);
void arch_fpu_flush(struct arch_fpu_regs *fpu);
void arch_actv_fpu_enable(struct arch_fpu_regs *fpu, void *owner);
void __noreturn arch_handle_fpu_simd_access_fault(struct arch_uctx *uctx);
void __noreturn arch_handle_fpu_fault(struct arch_uctx *uctx);

#endif /* ! AARCH64_ASM_FPU_H */
