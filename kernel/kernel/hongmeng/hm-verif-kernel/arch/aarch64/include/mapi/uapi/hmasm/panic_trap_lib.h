/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: arch trap implementation
 * Author: Huawei OS Kernel Lab
 * Create: Oct 25 09:00:20 2024
 */
#ifndef AARCH64_ULIBS_ASM_PANIC_TRAP_H
#define AARCH64_ULIBS_ASM_PANIC_TRAP_H

#define __imm_stringify(x)  #x
#define imm_stringify(x) __imm_stringify(x)

#define PANIC_TRAP_IMM 0xbeaf

static inline void arch_panic_trap_preproc(void)
{
	/* insert brk for dumpping, after dumpping,
	 * it will move to next instruction */
#ifdef __SYSMGR__
	/* only supported on sysmgr currently */
	asm volatile("brk " imm_stringify(PANIC_TRAP_IMM) "\n" : : : "memory");
#endif
}

#endif
