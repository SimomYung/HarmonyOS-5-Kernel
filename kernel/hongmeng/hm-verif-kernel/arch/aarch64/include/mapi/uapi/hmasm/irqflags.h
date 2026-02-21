/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: irq manipulations
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 22 10:05:53 2023
 */
#ifndef ARCH_AARCH64_INCLUDE_MAPI_UAPI_IRQFLAGS_H
#define ARCH_AARCH64_INCLUDE_MAPI_UAPI_IRQFLAGS_H

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
	asm volatile (
		"mrs     x18, daif\n"
		"bfxil   x18, %0, #0, #9\n"
		"msr     daif, x18\n"
#ifdef CONFIG_KDP_STRICT_CHECK
		"and     x18, x18, #1 << 9\n"
		"cbz     x18, 1f\n"
		"mrs     x18, mdscr_el1\n"
		"and     x18, x18, #1 << 13\n"
		"cbz     x18, 1f\n"
		"brk     #0xdead\n"
		"1:\n"
#endif
		:
		: "r"(flags)
#ifdef __hmfeature_ffixed_x18__
		: "memory");
#else
		: "x18", "memory");
#endif
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
