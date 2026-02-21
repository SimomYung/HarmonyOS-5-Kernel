/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Header file for scheduler indicator
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan 16 15:06:09 2023
 */

#ifndef HMKERNEL_SCHED_SCHED_INDICATOR_H
#define HMKERNEL_SCHED_SCHED_INDICATOR_H

#include <hmkernel/tcb.h>
#include <hmkernel/power/sched_indicator.h>

#ifdef CONFIG_MODULE_SCHED_INDICATOR

void sched_ind_notify_load_change(const struct tcb_s *tcb, unsigned int cpu_id,
				  unsigned long long curr_time,
				  enum load_change_event event);

unsigned int get_cluster_tuner_type(unsigned int cluster);
bool sched_ind_is_at_max_freq(unsigned int cpu_id);
unsigned int top_task_hist_size_of(unsigned int cluster_id);
bool top_task_stats_empty_window_of(unsigned int cluster_id);
unsigned int top_task_stats_policy_of(unsigned int cluster_id);
struct sched_ind_cluster_state *id_to_cluster_state(unsigned int cluster_id);
#ifdef CONFIG_HISI_MODULE_MIPS_LOAD_TRACKING
void sched_ind_notify_mips_change(struct tcb_s *tcb, unsigned int cpu_id,
				  unsigned long long curr_time, unsigned int event);
#else
static inline void sched_ind_notify_mips_change(struct tcb_s *tcb, unsigned int cpu_id,
						unsigned long long curr_time, unsigned int event)
{
	UNUSED(tcb, cpu_id, curr_time, event);
}
#endif

#else /* CONFIG_MODULE_SCHED_INDICATOR */

static inline void sched_ind_notify_load_change(const struct tcb_s *tcb, unsigned int cpu_id,
						unsigned long long curr_time,
						unsigned int event)
{
	UNUSED(tcb, cpu_id, curr_time, event);
}

static inline unsigned int get_cluster_tuner_type(unsigned int cluster)
{
	UNUSED(cluster);
	return 0;
}

static inline bool sched_ind_is_at_max_freq(unsigned int cpu_id)
{
	UNUSED(cpu_id);
	return false;
}
#endif /* CONFIG_MODULE_SCHED_INDICATOR */

#endif
