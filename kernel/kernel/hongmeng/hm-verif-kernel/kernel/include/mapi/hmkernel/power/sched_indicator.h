/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Mapi header file for scheduler indicator
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 12 11:35:31 2023
 */

#ifndef MAPI_HMKERNEL_POWER_SCHED_INDICATOR_H
#define MAPI_HMKERNEL_POWER_SCHED_INDICATOR_H

#include <hmkernel/errno.h>
#include <hmkernel/trigger.h>
#include <uapi/hmkernel/power/sched_indicator.h>

#ifdef CONFIG_PM_SCHED_INDICATOR_TRIGGER

struct trigger_source_ops *sched_ind_trigger_source_ops_of(void);

int sched_ind_load_chg_update(unsigned int cluster_id, unsigned int cpu_id,
			      unsigned int curr_freq, unsigned int next_freq);

int sched_ind_mips_chg_update(unsigned int cpu_id, struct __sched_ind_mips_cpu_data mips_cpu_data,
			      unsigned long long cur_time, unsigned int load_change_policy);

int sched_ind_mips_mem_update(unsigned int cluster_id, unsigned int type, unsigned int target_freq);

void sched_ind_load_chg_data_reset(void);

void sched_ind_do_trigger(void);

bool sched_ind_is_need_trigger(void);

#else

static inline struct trigger_source_ops *sched_ind_trigger_source_ops_of(void)
{
	return NULL;
}

static inline int sched_ind_load_chg_update(unsigned int cluster_id, unsigned int cpu_id,
					    unsigned int curr_freq, unsigned int next_freq)
{
	UNUSED(cluster_id, cpu_id, curr_freq, next_freq);
	return E_HM_NOSYS;
}

static inline int sched_ind_mips_chg_update(unsigned int cpu_id, struct __sched_ind_mips_cpu_data mips_cpu_data,
					    unsigned long long cur_time, unsigned int load_change_policy)
{
	return E_HM_NOSYS;
}
static inline int sched_ind_mips_mem_update(unsigned int cluster_id, unsigned int type, unsigned int target_freq)
{
	UNUSED(cluster_id, type, target_freq);
	return E_HM_NOSYS;
}

static inline void sched_ind_load_chg_data_reset(void)
{
}

static inline void sched_ind_do_trigger(void)
{
}

static inline bool sched_ind_is_need_trigger(void)
{
	return false;
}

#endif /* CONFIG_PM_SCHED_INDICATOR_TRIGGER */

#endif /* MAPI_HMKERNEL_POWER_SCHED_INDICATOR_H */
