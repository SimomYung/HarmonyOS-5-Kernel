/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 26 12:07:36 2018
 */
#ifndef ARRCH64_IRQFLAGS_H
#define ARRCH64_IRQFLAGS_H

#ifndef __ASSEMBLY__
#include <mapi/uapi/hmasm/irqflags.h>
#include <mapi/uapi/hmasm/registers.h>
#include <hmasm/processor.h>
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

static inline void arch_local_daif_init(__maybe_unused const struct arch_regs *regs,
					__maybe_unused bool is_from_real_el0,
					__maybe_unused u64 flags)
{
#if defined(CONFIG_SUPPORT_NONMASKABLE_FIQ) || defined(CONFIG_ENABLE_NONMASKABLE_SERROR)
	u64 mask = 0ULL;
#ifdef CONFIG_SUPPORT_NONMASKABLE_FIQ
	mask |= PSR_F_BIT;
#endif
#ifdef CONFIG_ENABLE_NONMASKABLE_SERROR
	mask |= PSR_A_BIT;
#endif
	if (is_from_real_el0) {
		mask &= flags;
	} else {
		/*
		 * Inherit the AF state from spsr where the exception occurred:
		 * Avoid the kernel being trapped in a dead loop when an
		 *    exception was triggered in __el1_error/fiq, like:
		 *
		 *        +--------(respond pending SError/FIQ)----+
		 *        |                                        x <- avoid dead loop
		 *        V                                        |           ^
		 *   __el1_error/fiq --(trigger exception)--> __el1_sync       |
		 *     (mask A/F)                             (mask A/F) ------+
		 *                                       (inherit A/F form spsr)
		 */
		mask &= (flags & (~(REG_SPSR(regs))));
	}
#if defined(CONFIG_SUPPORT_NONMASKABLE_FIQ) && defined(CONFIG_ENABLE_NONMASKABLE_SERROR)
	if (mask == (PSR_F_BIT | PSR_A_BIT)) {
		arch_local_daif_enable(FIQ_MASK_SET | SERROR_MASK_SET);
	}
#endif
#ifdef CONFIG_SUPPORT_NONMASKABLE_FIQ
	if (mask == PSR_F_BIT) {
		arch_local_daif_enable(FIQ_MASK_SET);
	}
#endif
#ifdef CONFIG_ENABLE_NONMASKABLE_SERROR
	if (mask == PSR_A_BIT) {
		arch_local_daif_enable(SERROR_MASK_SET);
	}
#endif
#endif
}

#endif

#endif /* AARCH64_IRQFLAGS_H */
