/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Preempt interface
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 13 15:16:15 2020
 */
#ifndef ULIBS_LIBPREEMPT_PREEMPT_H
#define ULIBS_LIBPREEMPT_PREEMPT_H

#include <stdbool.h>

struct __sched_info_s;
struct local_control_s {
	/* the recursive preempt disable depth. */
	int disable_cnt;
	 /* local area info used to sync with kernel */
	struct __sched_info_s *info;
};

#define OPTIMIZE(var) __asm__ ("" : "=r" (var) : "0" (var))

void preempt_enable(void);
void preempt_disable(void);

void enable_process_preempt_feature(void);
void disable_process_preempt_feature(void);
_Bool is_process_preempt_feature_disabled(void);

int preempt_process_enable(void);
int preempt_process_disable(void);
int preempt_process_register(void);
int preempt_process_unregister(void);
void preempt_process_reset(void);
int preempt_process_cpu_get(void);
bool preempt_process_feature_enabled(void);
void preempt_process_disable_timeout_init(void);

bool preempt_process_is_registered(void);
void preempt_process_init(void);
struct local_control_s *preempt_process_get_lc(void);

/* actv local changed, local preempt data reset */
void preempt_process_lc_reset(void);

int preempt_process_set_slv(unsigned int cpu, unsigned int slv);
int preempt_thread_set_slv(int tid, unsigned int slv);

void *preempt_process_info(void);
void preempt_module_resched(void);

int hm_user_preempt_disable(void);
int hm_user_preempt_enable(void);

void preempt_process_timeout_set(unsigned long long new_timeout);
unsigned int preempt_process_timeout_get(void);
#endif
