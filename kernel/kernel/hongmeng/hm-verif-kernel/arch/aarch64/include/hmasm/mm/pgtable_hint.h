/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 15 19:02:04 2020
 */
#ifndef AARCH64_ASM_PGTABLE_HINT_H
#define AARCH64_ASM_PGTABLE_HINT_H

#include <hmkernel/strict.h>

#include <hmasm/types.h>
#include <hmasm/memory.h>

static inline u64
arch_decorate_hint(u64 hint, vaddr_t va)
{
	UNUSED(va);
	return hint;
}

#endif /* ifndef AARCH64_ASM_PGTABLE_HINT_H */
