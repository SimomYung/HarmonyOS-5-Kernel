/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Export header files of cpu energy model to mapi
 * Author: Huawei OS Kernel Lab
 * Create: Tue Nov 29 11:52:42 2022
 */

#ifndef MAPI_HMKERNEL_ENERGY_MODEL_H
#define MAPI_HMKERNEL_ENERGY_MODEL_H

#include <hmkernel/cpuset.h>
#include <uapi/hmkernel/power/energy_model.h>
#include <hmkernel/cputopo.h>

#ifdef CONFIG_ENERGY_MODEL_NR_POWER_POINT
#define ENERGY_MODEL_NR_POWER_POINT CONFIG_ENERGY_MODEL_NR_POWER_POINT
#else
#define ENERGY_MODEL_NR_POWER_POINT 18
#endif

#ifdef CONFIG_ENERGY_MODEL_NR_EER_POINT
#define ENERGY_MODEL_NR_EER_POINT CONFIG_ENERGY_MODEL_NR_EER_POINT
#else
#define ENERGY_MODEL_NR_EER_POINT 26
#endif

#ifdef CONFIG_ENERGY_MODEL_NR_FREQ_POINT
#define ENERGY_MODEL_NR_FREQ_POINT CONFIG_ENERGY_MODEL_NR_FREQ_POINT
#else
#define ENERGY_MODEL_NR_FREQ_POINT 18
#endif

enum __em_capacity_type {
	__EM_CURR_CAPACITY,
#ifdef CONFIG_SMT
	__EM_CURR_SMT_CAPACITY,
#endif
	__EM_MIN_CAPACITY,
	__EM_MAX_CAPACITY,
	__EM_INVAL_CAPACITY,
};

struct em_cpu_capacity_s {
	unsigned int index;
	unsigned int ratio;
	unsigned int capacity;
#ifdef CONFIG_SMT
	unsigned int smt_capacity;
#endif
};

struct em_cluster_dynamic_energy_model_s {
	struct em_cpu_capacity_s curr_cap;
	struct em_cpu_capacity_s max_cap;
	struct em_cpu_capacity_s min_cap;
};

struct em_cluster_energy_model_s {
	/* Below elements are static data */
	unsigned int dmips; /* dmips per mhz */

	/* default hardware frequency index */
	unsigned int default_freq_idx;

	unsigned int min_capacity;
	unsigned int max_capacity;
	unsigned int capacity_level;

	unsigned int nr_avail_freq;
	unsigned int freq_list[ENERGY_MODEL_NR_FREQ_POINT];
	unsigned int amp_list[ENERGY_MODEL_NR_FREQ_POINT];
	unsigned int power_list[ENERGY_MODEL_NR_FREQ_POINT];
	/* energy effciency ratio */
	unsigned int eer_list[ENERGY_MODEL_NR_EER_POINT];
	/* thermal capacity with freq point */
	unsigned int thermal_cap_list[ENERGY_MODEL_NR_FREQ_POINT];

	/* limit capacity to update freq when cluster migration */
	unsigned int migration_cap_limit;
	/* suspend boost freq */
	unsigned int suspend_freq;
};

#ifdef CONFIG_PM_ENERGY_MODEL

/* APIs for em_capacity */
void em_capacity_update(struct em_cpu_capacity_s *cap, unsigned int capacity,
			unsigned int freq_idx, unsigned int freq_ratio);
#ifdef CONFIG_SMT
void em_smt_capacity_update(unsigned int cpu_id, unsigned int sibling_util, unsigned int discount);
#endif

/* APIs for system */
unsigned int em_max_capacity_of_system(void);

/* API for cpu energy model */
const struct em_cluster_energy_model_s *energy_model_of_cpu(unsigned int cpu);

unsigned int em_cpu_capacity_of(unsigned int cpu, enum __em_capacity_type type);

unsigned int em_cpu_frequency_of(unsigned int cpu, enum __em_capacity_type type);

unsigned int em_cpu_target_freq_of(unsigned int cpu, unsigned int index);

unsigned int em_cpu_target_last_freq_of(unsigned int cpu, unsigned int index);

unsigned int em_cpu_freq_table_level_lookup(unsigned int cpu, unsigned int freq);

unsigned int em_cpu_avail_freq_of(unsigned int cpu, unsigned int freq);

unsigned int em_cluster_id_of(unsigned int cpu);

bool em_is_at_min_cap(unsigned int cpu);

bool em_is_at_max_cap(unsigned int cpu);

bool em_is_fast_cpu(unsigned int cpu);

/* API for cluster energy model */
const struct em_cluster_energy_model_s *energy_model_of_cluster(unsigned int cluster);

unsigned int em_cluster_capacity_of(unsigned int cluster, enum __em_capacity_type type);

unsigned int em_nr_cpu_of_cluster(unsigned int cluster);

cpuset_t em_cpumask_of_cluster(unsigned int cluster);

unsigned int em_cluster_curr_power(unsigned int cluster_id);

unsigned int em_cluster_max_freq_of(unsigned int cluster);

unsigned int em_nr_of_capacity_level(void);

unsigned int em_cpu_capacity_level_of(unsigned int cpu);

cpuset_t em_cpumask_of_capacity_level(unsigned int level);

unsigned int em_first_cluster_of_capacity_level(unsigned int level);

unsigned int em_last_cluster_of_capacity_level(unsigned int level);

unsigned int em_level_capacity_of(unsigned int level, enum __em_capacity_type type);

#else /* CONFIG_PM_ENERGY_MODEL */

static inline void em_capacity_update(struct em_cpu_capacity_s *cap, unsigned int capacity,
				      unsigned int freq_idx, unsigned int freq_ratio)
{
	UNUSED(cap, capacity, freq_idx, freq_ratio);
}

static inline unsigned int em_max_capacity_of_system(void)
{
	return 0U;
}

static inline const struct em_cluster_energy_model_s *energy_model_of_cpu(unsigned int cpu)
{
	UNUSED(cpu);
	return NULL;
}

static inline unsigned int em_cluster_curr_power(unsigned int cluster_id)
{
	UNUSED(cluster_id);
	return 0U;
}

static inline
unsigned int em_cpu_capacity_of(unsigned int cpu, enum __em_capacity_type type)
{
	UNUSED(cpu, type);
	return 0U;
}

static inline
unsigned int em_cpu_frequency_of(unsigned int cpu, enum __em_capacity_type type)
{
	return 0U;
}

static inline
unsigned int em_cpu_target_freq_of(unsigned int cpu, unsigned int index)
{
	return 0U;
}

static inline
unsigned int em_cpu_freq_table_level_lookup(unsigned int cpu, unsigned int freq)
{
	return 0U;
}

static inline
unsigned int em_cpu_avail_freq_of(unsigned int cpu, unsigned int freq)
{
	return 0U;
}

static inline unsigned int em_cluster_id_of(unsigned int cpu)
{
	UNUSED(cpu);
	return 0U;
}

static inline bool em_is_at_min_cap(unsigned int cpu)
{
	UNUSED(cpu);
	return false;
}

static inline bool em_is_at_max_cap(unsigned int cpu)
{
	UNUSED(cpu);
	return false;
}

static inline bool em_is_fast_cpu(unsigned int cpu)
{
	UNUSED(cpu);
	return false;
}

static inline const struct em_cluster_energy_model_s *energy_model_of_cluster(unsigned int cluster)
{
	UNUSED(cluster);
	return NULL;
}

static inline
unsigned int em_cluster_capacity_of(unsigned int cluster, enum __em_capacity_type type)
{
	UNUSED(cluster, type);
	return 0U;
}

static inline unsigned int em_cluster_nr_cpu_of(unsigned int cluster)
{
	UNUSED(cluster);
	return 0U;
}

static inline cpuset_t em_cpumask_of_cluster(unsigned int cluster)
{
	UNUSED(cluster);
	return cpuset_empty;
}

static inline unsigned int em_cluster_max_freq_of(unsigned int cluster)
{
	UNUSED(cluster);
	return 0U;
}

static inline unsigned int em_nr_of_capacity_level(void)
{
	return 0U;
}

static inline unsigned int em_cpu_capacity_level_of(unsigned int cpu)
{
	UNUSED(cpu);
	return 0U;
}

static inline cpuset_t em_cpumask_of_capacity_level(unsigned int level)
{
	cpuset_t cpumask = cpuset_empty;
	if (level == 0U) {
		cpumask = cpuset_all;
	}
	return cpumask;
}

static inline unsigned int em_first_cluster_of_capacity_level(unsigned int level)
{
	UNUSED(level);
	return 0U;
}

static inline unsigned int em_last_cluster_of_capacity_level(unsigned int level)
{
	UNUSED(level);
	return 0U;
}

static inline unsigned int em_level_capacity_of(unsigned int level, enum __em_capacity_type type)
{
	UNUSED(level, type);
	return 0U;
}
#endif /* CONFIG_PM_ENERGY_MODEL */

#ifdef  __HOST_LLT__
int llt_energy_model_init(void);
#endif /* __HOST_LLT__ */

#endif
