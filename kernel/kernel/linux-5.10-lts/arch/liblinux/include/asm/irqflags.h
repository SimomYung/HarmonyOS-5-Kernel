/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_ASM_IRQFLAGS_H
#define __LIBLINUX_ASM_IRQFLAGS_H

#include <asm/thread_info.h>
#include <linux/preempt.h>

#define IRQFLAG_DISABLED	1
#define IRQFLAG_ENABLED		0
#define IRQFLAG_IS_DISABLED(f)	(((f) == IRQFLAG_DISABLED))

static inline unsigned long arch_local_irq_save(void)
{
	int old_state;
	struct thread_info *ti = current_thread_info();

	if (ti->preempt_bypass) {
		return IRQFLAG_ENABLED;
	}

	old_state = READ_ONCE(ti->irq_disabled);
	if (!old_state) {
		ti->irq_save_caller = __builtin_return_address(0);
		preempt_disable();
		WRITE_ONCE(ti->irq_disabled, 1);
	}

	return old_state ? IRQFLAG_DISABLED : IRQFLAG_ENABLED;
}

static inline void arch_local_irq_enable(void)
{
	struct thread_info *ti = current_thread_info();

	if (ti->preempt_bypass) {
		return;
	}

	if (READ_ONCE(ti->irq_disabled)) {
		ti->irq_save_caller = NULL;
		preempt_enable();
		WRITE_ONCE(ti->irq_disabled, 0);
	}
}

static inline void arch_local_irq_disable(void)
{
	struct thread_info *ti = current_thread_info();

	if (ti->preempt_bypass) {
		return;
	}

	if (!READ_ONCE(ti->irq_disabled)) {
		ti->irq_save_caller = __builtin_return_address(0);
		preempt_disable();
		WRITE_ONCE(ti->irq_disabled, 1);
	}
}

static inline unsigned long arch_local_save_flags(void)
{
	struct thread_info *ti = current_thread_info();

	if (ti->preempt_bypass) {
		return IRQFLAG_DISABLED;
	}

	return READ_ONCE(ti->irq_disabled) ? IRQFLAG_DISABLED : IRQFLAG_ENABLED;
}

static inline void arch_local_irq_restore(unsigned long flags)
{
	struct thread_info *ti = current_thread_info();

	if (ti->preempt_bypass) {
		return;
	}

	BUG_ON(!READ_ONCE(ti->irq_disabled));

	if (IRQFLAG_IS_DISABLED(flags) == 0) {
		ti->irq_save_caller = NULL;
		preempt_enable();
		WRITE_ONCE(ti->irq_disabled, 0);
	}
}

static inline int arch_irqs_disabled_flags(unsigned long flags)
{
	return IRQFLAG_IS_DISABLED(flags);
}

static inline int arch_irqs_disabled(void)
{
	return arch_irqs_disabled_flags(arch_local_save_flags());
}

#endif /* __LIBLINUX_ASM_IRQFLAGS_H */
