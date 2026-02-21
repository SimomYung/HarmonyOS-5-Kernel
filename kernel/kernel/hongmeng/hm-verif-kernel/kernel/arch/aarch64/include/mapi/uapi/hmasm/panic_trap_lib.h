/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: arch trap implementation
 * Author: Huawei OS Kernel Lab
 * Create: Oct 25 09:00:20 2024
 */
#ifndef AARCH64_ULIBS_ASM_PANIC_TRAP_H
#define AARCH64_ULIBS_ASM_PANIC_TRAP_H

#include <hmasm/debug.h>

static inline void arch_panic_trap_preproc(void)
{
	/* insert brk for dumpping, after dumpping,
	 * it will move to next instruction */
#ifdef __SYSMGR__
	/* only supported on sysmgr currently */
	asm volatile("brk " ASM_IMM(BRK_PANIC_TRAP_PRE) "\n" : : : "memory");
#endif
}

static inline void arch_panic_trap(void)
{
#ifdef __SYSMGR__
	asm volatile("brk " ASM_IMM(BRK_PANIC_TRAP) "\n" : : : "memory");
#endif
}

#endif
