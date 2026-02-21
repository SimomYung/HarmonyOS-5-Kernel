/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: arch trap implementation
 * Author: Huawei OS Kernel Lab
 * Create: Oct 25 09:00:20 2024
 */
#ifndef AARCH64_ASM_PANIC_TRAP_H
#define AARCH64_ASM_PANIC_TRAP_H

#include <hmasm/panic_trap_lib.h>
#include <hmkernel/types.h>
#include <hmkernel/strict.h>
#include <hmasm/exception.h>
#include <hmasm/memory.h>

#define ESR_EC_BRK64     0x3c
#define ESR_EC_extract(x) (((x) >> 26) & 0x3fU)
#define ESR_IMM_extract(x) ((x) & 0xffffU)
#define INSN_SIZE 4

static inline __bool arch_panic_trap_need_dump(const struct __arch_exception_info *info)
{
	__bool need = __false;
	__u64 esr = __arch_exception_info_dabort_status(info);
	unsigned int ec = ESR_EC_extract(esr);
	if (ec == ESR_EC_BRK64 && ESR_IMM_extract(esr) == BRK_PANIC_TRAP) {
		need = __true;
	}
	return need;
}

static inline __vaddr_t arch_panic_trap_next_pc(const struct __arch_exception_info *info)
{
	__vaddr_t next_pc = (__vaddr_t)__arch_exception_info_elr(info);
	next_pc += INSN_SIZE;
	return next_pc;
}

#endif
