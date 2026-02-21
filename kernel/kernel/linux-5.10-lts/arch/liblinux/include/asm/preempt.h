/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_ASM_PREEMPT_H
#define __LIBLINUX_ASM_PREEMPT_H

#define __preempt_count_add	___preempt_count_add
#define __preempt_count_sub	___preempt_count_sub

#include_next <asm-generic/preempt.h>

#undef __preempt_count_add
#undef __preempt_count_sub

#include <linux/threads.h>

#define __PREEMPT_SYNCED	0U /* state has been synced */
#define __PREEMPT_ENABLED	1U /* process preempt enabled */
#define __PREEMPT_DISABLED	2U /* process preempt disabled */
#define __PREEMPT_BUSY		3U /* should try again */

extern int __preempt_disabled[NR_CPUS];
extern void kernel_thread_preempt_disable(void);
extern void kernel_thread_preempt_enable(void);

static inline void __set_preempt_disabled(struct preempt_info *pi)
{
	WRITE_ONCE(pi->state, __PREEMPT_DISABLED);
}

static inline void __set_preempt_enabled(struct preempt_info *pi)
{
	WRITE_ONCE(pi->state, __PREEMPT_ENABLED);
}

static inline bool __test_preempt_busy(struct preempt_info *pi)
{
	return READ_ONCE(pi->state) == __PREEMPT_BUSY;
}

static inline bool __test_preempt_synced(struct preempt_info *pi)
{
	return READ_ONCE(pi->state) == __PREEMPT_SYNCED;
}

static inline bool __test_preempt_need_resched(struct preempt_info *pi)
{
	return READ_ONCE(pi->need_resched) != 0;
}

static inline void __clear_preempt_need_resched(struct preempt_info *pi)
{
	WRITE_ONCE(pi->need_resched, 0);
}

static inline u32 __get_preempt_cpuid(struct preempt_info *pi)
{
	return READ_ONCE(pi->cpuid);
}

#ifdef CONFIG_SCHED_DFX_OHOS
extern void preempt_latency_start(u32 cpu);
extern void preempt_latency_stop(u32 cpu);
#endif

static __always_inline void __preempt_process_disable(struct preempt_info *pi)
{
	u32 cpu;

	__set_preempt_disabled(pi);
	barrier();
	cpu = __get_preempt_cpuid(pi);

	if (unlikely(__preempt_disabled[cpu] > 0 || __test_preempt_busy(pi))) {
		kernel_thread_preempt_disable();
		cpu = __get_preempt_cpuid(pi);
	}

#ifdef CONFIG_SCHED_DFX_OHOS
	if (__preempt_disabled[cpu] == 0)
		preempt_latency_start(cpu);
#endif

	__preempt_disabled[cpu]++;
}

static __always_inline void __preempt_process_enable(struct preempt_info *pi)
{
	u32 cpu = __get_preempt_cpuid(pi);

	__preempt_disabled[cpu]--;
#ifdef CONFIG_SCHED_DFX_OHOS
	if (__preempt_disabled[cpu] == 0)
		preempt_latency_stop(cpu);
#endif

	__set_preempt_enabled(pi);

	if (unlikely(__test_preempt_need_resched(pi))) {
		kernel_thread_preempt_enable();
		__clear_preempt_need_resched(pi);
	}
}

static __always_inline void __preempt_count_add(int val)
{
	struct thread_info *ti = current_thread_info();
	volatile int *ptr = &ti->preempt_count;
	if (*ptr == 0) {
		if (!ti->preempt_bypass) {
			__preempt_process_disable(ti->preempt);
		}
	}
	*ptr += val;
}

static __always_inline void __preempt_count_sub(int val)
{
	struct thread_info *ti = current_thread_info();
	volatile int *ptr = &ti->preempt_count;
	*ptr -= val;
	if (*ptr == 0) {
		if (!ti->preempt_bypass) {
			__preempt_process_enable(ti->preempt);
		}
	}
	if (unlikely(*ptr < 0)) {
		*ptr = 0; /* Avoid dead loops caused by preempt disable on BUG_ON. */
		BUG_ON(1);
	}
}

#endif /* __LIBLINUX_ASM_PREEMPT_H */
