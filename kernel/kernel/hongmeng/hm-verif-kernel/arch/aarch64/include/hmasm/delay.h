/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Nov 06 17:01:56 2019
 */
#ifndef AARCH64_ASM_DELAY_H
#define AARCH64_ASM_DELAY_H

#include <hmkernel/types.h>

static inline void arch_delay(u32 cycle)
{
	asm volatile (
		"    cbz     %0, 2f\n\t"
		"1:  subs    %0, %0, #1\n\t"
		"    bne     1b\n\t"
		"2:\n"
		: "+r" (cycle));
}

#endif
