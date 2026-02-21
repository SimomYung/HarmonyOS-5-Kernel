/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 26 12:07:36 2018
 */
#ifndef ARRCH64_IRQFLAGS_H
#define ARRCH64_IRQFLAGS_H

#ifndef __ASSEMBLY__
#include <mapi/uapi/hmasm/irqflags.h>
#endif

#ifdef CONFIG_SUPPORT_NONMASKABLE_FIQ
#define IRQ_MASK_SET 0x2
#else
#define IRQ_MASK_SET 0x3
#endif

#define DEBUG_MASK_SET 0x8
#define FIQ_MASK_SET 0x1
#define SERROR_MASK_SET 0x4

#ifndef __ASSEMBLY__
/* cpu related definitions */
static inline unsigned long arch_local_irq_save(void)
{
	unsigned long flags;
	asm volatile (
		"mrs    %0, daif\n"
		/* msr daifset, IRQ_MASK_SET will only disable irq and fiq,
		 * won't disable debug bit, so no need check.
		 */
		"msr    daifset, %1\n"
		: "=r"(flags)
		: "i"(IRQ_MASK_SET)
		: "memory");
	return flags;
}

static inline void arch_local_irq_restore(unsigned long flags)
{
	return __arch_local_irq_restore(flags);
}

static inline void arch_local_irq_enable(void)
{
	return __arch_local_irq_enable();
}

static inline void arch_local_irq_disable(void)
{
	return __arch_local_irq_disable();
}

static inline void arch_local_irq_mask(void)
{
	asm volatile (
		/* arch_local_irq_mask is only called by smp_ipi_cpu_stop,
		 * is not a useful gadget to disable debug
		 */
		"msr	daifset, #0xf\n"
		:
		:
		: "memory");
}

#define arch_local_daif_enable(flags) \
do { \
	asm volatile ( \
		/* msr    daifclr, %0 won't disable debug, no need check */ \
		"msr    daifclr, %0\n" \
		: \
		: "i"(flags) \
		: "memory"); \
} while (0)

#endif

#endif /* AARCH64_IRQFLAGS_H */
