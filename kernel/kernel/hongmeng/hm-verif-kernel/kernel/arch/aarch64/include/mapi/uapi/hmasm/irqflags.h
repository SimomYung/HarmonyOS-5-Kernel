/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: irq manipulations
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 22 10:05:53 2023
 */
#ifndef ARCH_AARCH64_INCLUDE_MAPI_UAPI_IRQFLAGS_H
#define ARCH_AARCH64_INCLUDE_MAPI_UAPI_IRQFLAGS_H

#include <hmasm/kdp.h>

/* cpu related definitions */
static inline unsigned long __arch_local_irq_save(void)
{
	unsigned long flags;
	asm volatile (
		"mrs    %0, daif\n"
		/* msr daifset, #2 will only disable irq, won't disable debug bit, so no need check */
		"msr    daifset, #2\n"
		: "=r"(flags)
		:
		: "memory");
	return flags;
}

static inline void __arch_local_irq_restore(unsigned long flags)
{
	unsigned long value;
	asm volatile("mrs %0, daif" : "=r" (value));
	value = (value & __DAIF_D_MASK) | (flags & __DAIF_D_CLEAR_MASK);
	__kdp_daif_set(value);
}

static inline void __arch_local_irq_enable(void)
{
	asm volatile (
		/* msr daifclr, #2 will only enable irq, won't disable debug bit, so no need check */
		"msr    daifclr, #2\n"
		:
		:
		: "memory");
}

static inline void __arch_local_irq_disable(void)
{
	asm volatile (
		/* msr daifset, #2 will only disable irq, won't disable debug bit, so no need check */
		"msr	daifset, #2\n"
		:
		:
		: "memory");
}

#endif /* ARCH_AARCH64_INCLUDE_MAPI_UAPI_IRQFLAGS_H */
