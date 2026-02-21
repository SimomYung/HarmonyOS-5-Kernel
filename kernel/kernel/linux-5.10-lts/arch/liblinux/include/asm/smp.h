/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef __LIBLINUX_ASM_SMP_H
#define __LIBLINUX_ASM_SMP_H

void liblinux_flush_smp_call_function(void);
void register_user_call_func_ipi(unsigned int cpu);

#include_next <asm/smp.h>

#undef  raw_smp_processor_id

static inline int raw_smp_processor_id(void)
{
	return __get_preempt_cpuid(current_thread_info()->preempt);
}

#define raw_smp_processor_id raw_smp_processor_id

int invoke_on_cpu_single_async(int cpu, struct __call_single_data *csd);

#endif /* __LIBLINUX_ASM_SMP_H */
