/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 29 07:23:29 2018
 */
#ifndef A64_ASM_SWAB_H
#define A64_ASM_SWAB_H

#include <hmkernel/types.h>
#include <hmkernel/bitops/swab.h>
#include <hmasm/swab.h>

#ifdef CONFIG_HAVE_ARM_REV_INSN
static inline u16 _arch_swab16(u16 val)
{
	asm volatile ("rev16 %0, %1\n" : "=r"(val) : "r" (val));
	return val;
}
#define arch_swab16 _arch_swab16

static inline u32 _arch_swab32(u32 val)
{
	asm volatile ("rev32 %0, %1\n" : "=r"(val) : "r" (val));
	return val;
}
#define arch_swab32 _arch_swab32

static inline u64 _arch_swab64(u64 val)
{
	asm volatile ("rev64 %0, %1\n" : "=r"(val) : "r" (val));
	return val;
}
#define arch_swab64 _arch_swab64
#endif

#endif
