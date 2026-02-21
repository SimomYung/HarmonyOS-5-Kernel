/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Export kernel smp.h to mapi
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 15 11:40:36 2019
 */
#ifndef MAPI_HMKERNEL_SMP_H
#define MAPI_HMKERNEL_SMP_H

#include <hmkernel/errno.h>
#include <hmkernel/compiler.h>

struct tcb_s;
#ifdef CONFIG_SMP
JAM_INLINE unsigned int __pure smp_current_cpu_id(void);

extern void
smp_update_tcb_timer(struct tcb_s *tcb, unsigned int origin_cpu);

#ifdef __HOST_LLT__
extern void llt_set_stopmachine_timeout(bool val);
#endif
extern int
smp_stop_machine(unsigned long timeout_us, bool ignore_hungry_cpus, bool resume_on_fail);
extern int
smp_resume_machine(void);
extern bool is_smp_machine_stoped(void);
extern int smp_call_function(cpuset_t cpuset, void (*func)(void *info),
			     void *data, bool wait);
extern int on_each_online_cpu(void (*func)(void *info), void *data, bool wait);
extern void
smp_send_stop(void);
extern void __noreturn
smp_ipi_cpu_stop(unsigned int cur_cpu);
#else
static inline unsigned int
smp_current_cpu_id(void)
{
	return 0;
}

static inline void
smp_update_tcb_timer(struct tcb_s *tcb, unsigned int origin_cpu)
{
	UNUSED(tcb, origin_cpu);
}

static inline int
smp_stop_machine(unsigned long timeout_us, bool ignore_hungry_cpus, bool resume_on_fail)
{
	UNUSED(timeout_us, ignore_hungry_cpus);
	return E_HM_OK;
}

static inline int
smp_resume_machine(void)
{
	return E_HM_OK;
}

static inline int
is_smp_machine_stoped(void)
{
	return true;
}

static inline int smp_call_function(cpuset_t cpuset, void (*func)(void *info),
				    void *data, bool wait)
{
	UNUSED(cpuset, func, data, wait);
	return E_HM_OK;
}

static inline int on_each_online_cpu(void (*func)(void *info),
				     void *data, bool wait)
{
	UNUSED(wait);

	func(data);

	return E_HM_OK;
}

static inline void
smp_send_stop(void) {}
static inline void
smp_ipi_cpu_stop(unsigned int cur_cpu)
{
	UNUSED(cur_cpu);
}
#endif
#endif
