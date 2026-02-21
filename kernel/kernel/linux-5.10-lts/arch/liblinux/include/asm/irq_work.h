/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_ASM_IRQ_WORK_H
#define __LIBLINUX_ASM_IRQ_WORK_H

#define arch_irq_work_has_interrupt __arch_irq_work_has_interrupt
#include_next <asm/irq_work.h>
#undef arch_irq_work_has_interrupt

static inline bool arch_irq_work_has_interrupt(void)
{
	return false;
}

#endif /* __LIBLINUX_ASM_IRQ_WORK_H */
