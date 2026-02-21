/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Declaration of load tracking policy
 * Author: Huawei OS Kernel Lab
 * Create: Tue Dec 03 19:01:23 2024
 */

#ifndef HMKERNEL_SCHED_LT_H
#define HMKERNEL_SCHED_LT_H

#include <hmkernel/tcb.h>
#ifdef CONFIG_MODULE_LOAD_TRACKING
#include <hmkernel/sched_module.h>
#include <hmkernel/tcb/struct.h>
#include <hmkernel/sched/sched_mdata.h>

#define lt_group_is_non_group(group) \
	(group == SCHED_LT_NON_GROUP)

#define lt_group_is_capacity_eager(group) \
	(group == SCHED_LT_CAPACITY_GROUP)

#define lt_group_is_power_eager(group) \
	(group == SCHED_LT_POWER_GROUP)

#define lt_group_is_energy_eager(group) \
	(group ==  SCHED_LT_ENERGY_GROUP)

/* uclame id with cluster */
enum lt_uclamp_type {
	LT_UCLAMP_TYPE_MIN = 0,
	LT_UCLAMP_TYPE_MAX = 1,
	LT_UCLAMP_TYPE_NR,
};

unsigned int lt_cluster_cpu_load_of(unsigned int cpu);
unsigned int lt_cluster_top_load_of(unsigned int cluster_id);
unsigned int lt_capacity_level_top_load_of(unsigned int level);
unsigned int lt_capacity_level_avg_cpu_load_of(unsigned int level);
unsigned int lt_capacity_level_avg_load_of(unsigned int level);
unsigned int lt_capacity_level_avg_tcb_load_of(unsigned int level);
bool lt_is_level_full_load_nr(unsigned int level);
unsigned int lt_total_load_nr(void);
unsigned int lt_cluster_uclamp_util_of(unsigned int cpu, enum lt_uclamp_type type);
unsigned int lt_cluster_max_util_of(unsigned int cpu);
unsigned int lt_running_topload_of(unsigned int cpu);
unsigned int lt_running_load_of(unsigned int cpu);
#ifdef CONFIG_SCHED_HMLT_IRQ_LOAD
bool lt_cpu_high_irqload(unsigned int cpu);
u64 lt_irq_load_of(unsigned int cpu);
#else
static inline bool lt_cpu_high_irqload(unsigned int cpu)
{
	UNUSED(cpu);
	return false;
}

static inline u64 lt_irq_load_of(unsigned int cpu)
{
	UNUSED(cpu);
	return 0ULL;
}
#endif
unsigned int lt_tcb_load_of(const struct tcb_s *tcb);
unsigned int lt_cluster_min_cpu_load_of(unsigned int cpu);
unsigned int lt_cluster_max_tcb_load_of(unsigned int cpu);
unsigned int lt_cluster_raw_load_of(unsigned int cpu);
#ifdef CONFIG_UNIFIED_COLLECTION
u64 lt_tcb_running_load_of(const struct tcb_s *tcb);
#endif
unsigned int lt_tcb_origin_load_of(const struct tcb_s *tcb);
u64 lt_tcb_load_sum_of(const struct tcb_s *tcb);
bool lt_capacity_level_load_overutilized(unsigned int level);
void lt_update_freq(unsigned int cpu_id, unsigned int freq_idx, unsigned int freq_ratio);
void lt_update_cpu_ddl_activity(unsigned int cpu, bool active);
unsigned int lt_uclamp_util_with_cluster(unsigned int cpu, unsigned int util);

unsigned int tcb_lt_group_of(const struct tcb_s *tcb);
int tcb_lt_boost_of(const struct tcb_s *tcb);
s8 tcb_lt_task_boost_of(const struct tcb_s *tcb);
unsigned int tcb_lt_min_load_of(const struct tcb_s *tcb);
unsigned int tcb_lt_max_load_of(const struct tcb_s *tcb);
unsigned int tcb_lt_window_sz_of(const struct tcb_s *tcb);
bool tcb_lt_prefer_idle_of(const struct tcb_s *tcb);
unsigned int tcb_lt_smartio_group_of(const struct tcb_s *tcb);
#ifdef CONFIG_TIMER_SLACK
unsigned int tcb_lt_ts_exempt_tock_of(const struct tcb_s *tcb);
unsigned int tcb_lt_ts_max_delay_tock_of(const struct tcb_s *tcb);
unsigned int tcb_lt_ts_pct_of(const struct tcb_s *tcb);
#endif
bool tcb_lt_expected_quick_run(const struct tcb_s *tcb, unsigned int cpu);
unsigned int tcb_lt_smt_expeller_of(const struct tcb_s *tcb);
unsigned int tcb_lt_cgroup_id_of(const struct tcb_s *tcb);
void tcb_lt_set_task_min_load(struct tcb_s *tcb, unsigned int min);
bool tcb_lt_topload_enable_of(const struct tcb_s *tcb);
unsigned int lt_capacity_cpu_margin_of(unsigned int cluster_id);
unsigned int lt_capacity_tcb_margin_of(unsigned int cluster_id);
unsigned int lt_capacity_discount_mt_of(unsigned int type);

bool global_is_boot_boost(void);
void set_global_boot_boost(bool is_boost);
bool global_is_capacity_eager(void);
void set_global_capacity_eager(bool is_eager);
bool global_is_cpufreq_energy_aware(void);
void set_global_cpufreq_energy_aware(bool is_energy_aware);
unsigned int get_lt_global_init_load(void);

unsigned int boost_flag_of(const struct tcb_s *tcb);
bool tcb_is_boosted(const struct tcb_s *tcb);

int lt_set_conf(struct tcb_s *tcb, struct sched_lt_args_s *lt_args);
int lt_get_conf(const struct tcb_s *tcb, struct sched_lt_args_s *lt_args);
int lt_set_global_conf(struct sched_lt_global_args_s *lt_global_args);
int lt_get_global_conf(struct sched_lt_global_args_s *lt_global_args);
int lt_group_alloc(void);
int lt_group_free(unsigned int idx);
int lt_set_group_conf(struct sched_lt_args_s *lt_args);
void tcb_set_lt_group(const struct tcb_s *tcb, unsigned int group);
bool lt_load_misfit(unsigned int capacity_level, const struct tcb_s *tcb, unsigned int task_load);
bool lt_load_cpu_misfit(unsigned int cpu, const struct tcb_s *tcb, unsigned int task_load);
bool lt_load_higher_cpu_capacity(unsigned int new_cpu, unsigned int old_cpu);
void set_global_cpu_capacity_margin(unsigned int *margins, size_t cluster_num);
void get_global_cpu_capacity_margin(unsigned int *margins, size_t cluster_num);
void set_global_tcb_capacity_margin(unsigned int *margins, size_t cluster_num);
void get_global_tcb_capacity_margin(unsigned int *margins, size_t cluster_num);
int get_global_capacity_discount_mt(unsigned int type, unsigned int *discount);
int set_global_capacity_discount_mt(unsigned int type, unsigned int discount);

unsigned int lt_curr_capacity_of(unsigned int cpu, unsigned int dc_type);
static inline u64 time_to_load(u64 time, unsigned int capacity)
{
	return time * capacity;
}

#ifdef CONFIG_HM_SCHED_TRANS
bool lt_trans_group_check(struct tcb_s *src_tcb, struct tcb_s *des_tcb,
			  bool restore, enum sched_trans_method method);
bool lt_trans_group(struct tcb_s *src_tcb, struct tcb_s *des_tcb,
		    bool restore, enum sched_trans_method method);
bool lt_trans_task_max_check(struct tcb_s *src_tcb, struct tcb_s *des_tcb,
			     bool restore, enum sched_trans_method method);
bool lt_trans_task_max(struct tcb_s *src_tcb, struct tcb_s *des_tcb,
		       bool restore, enum sched_trans_method method);
bool lt_trans_task_min_check(struct tcb_s *src_tcb, struct tcb_s *des_tcb,
			     bool restore, enum sched_trans_method method);
bool lt_trans_task_min(struct tcb_s *src_tcb, struct tcb_s *des_tcb,
		       bool restore, enum sched_trans_method method);
unsigned int tcb_lt_user_group_of(const struct tcb_s *tcb);
void tcb_lt_mark_load_trans(struct tcb_s *tcb, bool load_trans);
bool tcb_can_load_trans(const struct tcb_s *tcb);
#else
static inline bool
tcb_can_load_trans(const struct tcb_s *tcb)
{
	UNUSED(tcb);
	return false;
}

static inline void tcb_lt_mark_load_trans(struct tcb_s *tcb, bool load_trans)
{
	UNUSED(tcb, load_trans);
}
#endif

#ifdef __HOST_LLT__
int llt_lt_init(void);
void llt_tcb_set_lt_group(const struct tcb_s *tcb, unsigned int group);
#ifdef CONFIG_GROUP_BANDWIDTH_CAPACITY_AWARE
unsigned int llt_lt_sum_capacity_of_system(void);
#endif
#endif

#else /* CONFIG_MODULE_LOAD_TRACKING */

static inline bool lt_load_higher_cpu_capacity(unsigned int new_cpu, unsigned int old_cpu)
{
	UNUSED(new_cpu, old_cpu);
	return false;
}

static inline unsigned int lt_tcb_load_of(const struct tcb_s *tcb)
{
	UNUSED(tcb);
	return 0U;
}

static inline int
tcb_lt_boost_of(const struct tcb_s *tcb) {
	UNUSED(tcb);
	return 0U;
}

static inline unsigned int boost_flag_of(const struct tcb_s *tcb)
{
	UNUSED(tcb);
	return 0U;
}

static inline bool tcb_is_boosted(const struct tcb_s *tcb)
{
	UNUSED(tcb);
	return false;
}

static inline bool
tcb_can_load_trans(const struct tcb_s *tcb)
{
	UNUSED(tcb);
	return false;
}

static inline bool lt_cpu_high_irqload(unsigned int cpu)
{
	UNUSED(cpu);
	return false;
}

static inline bool tcb_lt_prefer_idle_of(const struct tcb_s *tcb)
{
	UNUSED(tcb);
	return false;
}

static inline unsigned int tcb_lt_group_of(const struct tcb_s *tcb)
{
	UNUSED(tcb);
	return 0U;
}

static inline bool global_is_cpufreq_energy_aware(void)
{
	return false;
}

static inline unsigned int lt_running_load_of(unsigned int cpu)
{
	UNUSED(cpu);
	return 0U;
}

static inline unsigned int tcb_lt_smt_expeller_of(const struct tcb_s *tcb)
{
	UNUSED(tcb);
	return 0U;
}
#endif /* CONFIG_MODULE_LOAD_TRACKING */

#ifdef CONFIG_GROUP_BANDWIDTH_CAPACITY_AWARE
u64 lt_runtime_to_load(u64 runtime);
#else
static inline u64 lt_runtime_to_load(u64 runtime)
{
	UNUSED(runtime);
	return 0ULL;
}
#endif
#endif
