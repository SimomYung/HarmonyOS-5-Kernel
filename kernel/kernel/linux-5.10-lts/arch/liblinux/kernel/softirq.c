// SPDX-License-Identifier: GPL-2.0
#include <linux/interrupt.h>
#include <linux/kthread.h>
#include <linux/smp.h>
#include <linux/irq.h>

static inline int __preempt_count(struct thread_info *info)
{
	return READ_ONCE(info->preempt_count);
}

#define __hardirq_count(info)	(__preempt_count(info) & HARDIRQ_MASK)
#define __softirq_count(info)	(__preempt_count(info) & SOFTIRQ_MASK)
#define __irq_count(info)	(__preempt_count(info) & (HARDIRQ_MASK \
					| SOFTIRQ_MASK | NMI_MASK))

#define __in_irq(info)		(__hardirq_count(info))
#define __in_interrupt(info)	(__irq_count(info))

static inline void ____preempt_count_sub(struct thread_info *info, int val)
{
	volatile int *ptr = &info->preempt_count;
	*ptr -= val;
	if (*ptr == 0) {
		if (!info->preempt_bypass) {
			__preempt_process_enable(info->preempt);
		}
	}
	BUG_ON(*ptr < 0);
}

void __override __local_bh_enable_ip(unsigned long ip, unsigned int cnt)
{
	struct thread_info *info = current_thread_info();

	WARN_ON_ONCE(__in_irq(info));
	lockdep_assert_irqs_enabled();
#ifdef CONFIG_TRACE_IRQFLAGS
	local_irq_disable();
#endif

	/*
	 * Are softirqs going to be turned on now:
	 */
	if (__softirq_count(info) == SOFTIRQ_DISABLE_OFFSET)
		lockdep_softirqs_on(ip);

	/*
	 * Keep preemption disabled until we are done with
	 * softirq processing:
	 */
	____preempt_count_sub(info, cnt - 1);

	if (unlikely(!__in_interrupt(info) && local_softirq_pending())) {
		/*
		 * Run softirq if any pending. And do it in its own stack
		 * as we may be calling this deep in a task call stack already.
		 */
		do_softirq();
	}

	____preempt_count_sub(info, 1);
#ifdef CONFIG_TRACE_IRQFLAGS
	local_irq_enable();
#endif
	preempt_check_resched();
}
