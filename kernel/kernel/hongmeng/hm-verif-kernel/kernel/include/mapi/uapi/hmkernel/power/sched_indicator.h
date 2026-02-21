/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: UAPI for scheduler indicator
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 25 18:54:02 2022
 */

#ifndef UAPI_HMKERNEL_POWER_SCHED_INDICATOR_H
#define UAPI_HMKERNEL_POWER_SCHED_INDICATOR_H

#include <hmkernel/sched/mips_common.h>
#ifdef CONFIG_PM_MAX_NR_CLUSTER
#define SCHED_IND_MAX_NR_CLUSTER CONFIG_PM_MAX_NR_CLUSTER
#else
#define SCHED_IND_MAX_NR_CLUSTER 3
#endif

#ifdef CONFIG_PM_MAX_NR_CPUS
#define SCHED_IND_MAX_NR_CPUS CONFIG_PM_MAX_NR_CPUS
#else
#define SCHED_IND_MAX_NR_CPUS 12
#endif

#define __SCHED_IND_DEFAULT_HISPEED_LOAD_RATIO 99U
#define __SCHED_IND_DEFAULT_RATE_LIMIT_MS 79U
#define __SCHED_IND_DEFAULT_TARGET_LOAD 90U
#define __SCHED_IND_CPUFREQ_JUMP_RATIO 120U
#define __SCHED_IND_CPUFREQ_RATIO_PERCENT 100U
#define __SCHED_IND_LOAD_RATIO_PERCENT __SCHED_IND_CPUFREQ_RATIO_PERCENT
#define __SCHED_IND_DEFAULT_IOWAIT_BOOST_STEP 200000 /* kHz */
#define __SCHED_IND_DEFAULT_IO_IS_BUSY 1
/*
 * LOAD_THRESHOLD_RATIO * CPUFREQ_JUMP_RATIO should equals to
 * CPUFREQ_RATIO_PERCENT * LOAD_RATIO_PERCENT
 */
#define __SCHED_IND_LOAD_THRESHOLD_RATIO 83U

#define NR_MIPS_WINDOW	6

enum load_change_event {
	NORMAL_SCALE,
	FAST_SCALE,
	FAST_DOWN, /* cluster migration src */
	FAST_UP, /* cluster migration dst */
	IOWAIT_BOOST, /* iowait trigger cpufreq adjustment */
};

/*
 * Some unused freq reporting policies are commented out.
 * The enum value should be same as the history usage.
 */
enum {
	FREQ_STAT_CPU_LOAD		= (1 << 0),
	FREQ_STAT_TOP_TASK		= (1 << 1),
	/* FREQ_STAT_PRED_LOAD_SUM	= (1 << 2), */
	FREQ_STAT_MAX_TCB_LOAD		= (1 << 3),
	FREQ_STAT_CPU_LOAD_MIN		= (1 << 4),
	/* FREQ_STAT_HWVIP_BOOST_TASK	= (1 << 5), */
	FREQ_STAT_CPU_RAW_LOAD		= (1 << 6)
};
#define DEFAULT_REPORTING_POLICY \
	(FREQ_STAT_CPU_LOAD | FREQ_STAT_TOP_TASK)

#define HM_LT	(1 << 0)
#define MIPS_CPU	(1 << 1)
#define MIPS_MEM	(1 << 2)

#define MIPS_TICK_EVENT	(1 << 0)
#define MIPS_MEM_EVENT	(1 << 1)
#define MIPS_CPU_EVENT	(1 << 2)

struct __sched_ind_mips_cpu_data {
	unsigned int cpu_ipc;
	/* for mips cpu freq */
	unsigned int cpu_demand;
	unsigned int task_demand;
	/* for mips mem freq */
	unsigned int inst;
	unsigned int cycle[NR_MIPS_WINDOW];
	bool is_updated;
	bool task_is_emerg;
	unsigned int event;
	unsigned int ddl_load;
	long long emerg_run_time;
	long long emerg_iowait_time;
	unsigned int top_load;
#ifdef CONFIG_HISI_THERMAL_INTELLIGENT_FREQ_CTRL
	unsigned int freq_limit;
#endif
};

struct __sched_ind_cluster_data {
	bool is_updated;
	unsigned int target_freq;
	bool is_mips_mem_update[DFC_MEM_TYPE_NUM]; /* for mips mem */
	unsigned int target_mem_freq[DFC_MEM_TYPE_NUM]; /* for mips mem */
	unsigned int ddl_load; /* for mips cpu */
	unsigned int top_load; /* for mips cpu */
#ifdef CONFIG_HISI_THERMAL_INTELLIGENT_FREQ_CTRL
	unsigned int freq_limit;
#endif
};

/* data for kernel to trigger to tppmgr to notify load changing */
struct __sched_ind_load_chg_data {
	bool is_updated;
	unsigned int nr_cluster;
	unsigned int nr_cpu;
	unsigned int policy;
	struct __sched_ind_mips_cpu_data cpus_data[SCHED_IND_MAX_NR_CPUS];
	struct __sched_ind_cluster_data clusters_data[SCHED_IND_MAX_NR_CLUSTER];
};

#define __SCHED_IND_DEFAULT_SIZE 64U
#define __SCHED_IND_MODULE_NAME "sched_ind_class"
#define __SCHED_IND_CMD_TOGGLE 0x100U
#define __SCHED_IND_CMD_FAST_RAMP_UP 0x200U
#define __SCHED_IND_CMD_FAST_RAMP_DOWN 0x300U
#define __SCHED_IND_CMD_MIGRATION_LIMIT 0x400U
#define __SCHED_IND_CMD_TIMER_SLACK 0x500U
#define __SCHED_IND_CMD_WINDOW_SIZE 0x600U
#define __SCHED_IND_CMD_HISPEED_FREQ 0x700U
#define __SCHED_IND_CMD_GO_HISPEED_LOAD 0x800U
#define __SCHED_IND_CMD_ABOVE_HISPEED_DELAY 0x900U
#define __SCHED_IND_CMD_MIN_SAMPLE_TIME 0xA00U
#define __SCHED_IND_CMD_TARGET_LOADS 0xB00U
#define __SCHED_IND_CMD_BOOSTPULSE_ENDTIME 0xC00U
#define __SCHED_IND_CMD_TOPLOAD_SIZE 0xC01U
#define __SCHED_IND_CMD_TOPLOAD_EMPTY 0xC02U
#define __SCHED_IND_CMD_TOPLOAD_POLICY 0xC03U
#define __SCHED_IND_CMD_CLUSTER_TUNER_TYPE 0xD00U
#define __SCHED_IND_CMD_IOWAIT_BOOST_STEP 0xE00U
#define __SCHED_IND_CMD_IOWAIT_UPPER_LIMIT 0xE01U
#define __SCHED_IND_CMD_IO_IS_BUSY 0xE02U
#define __SCHED_IND_CMD_FREQ_REPORTING_POLICY 0xF00U

enum topload_policy {
	TOPLOAD_WINDOW_STATS_RECENT,
	TOPLOAD_WINDOW_STATS_MAX,
	TOPLOAD_WINDOW_STATS_MAX_RECENT_AVG,
	TOPLOAD_WINDOW_STATS_AVG,
	TOPLOAD_WINDOW_STATS_INVALID_POLICY
};

struct __sched_ind_toggle_data {
	unsigned int cluster_id;
	bool toggle_enabled;
};

enum __sched_ind_tuner_type {
	__SCHED_IND_TUNER_MISC,
	__SCHED_IND_TUNER_MIPS,
	__SCHED_IND_TUNER_OTHER,
};
 
struct __sched_ind_tuner_data {
	unsigned int cluster_id;
	enum __sched_ind_tuner_type tuner_type;
};

struct __sched_ind_attributes {
	unsigned int cluster_id;
	unsigned int fast_ramp_up;
	unsigned int fast_ramp_down;
	unsigned int hispeed_freq;
	unsigned int go_hispeed_load;
	unsigned int iowait_boost_step;
	unsigned int iowait_upper_limit;
	unsigned int io_is_busy;
	unsigned int above_hispeed_delay_nr;
	unsigned int top_task_hist_size;
	unsigned int top_task_stats_empty_window;
	unsigned int top_task_stats_policy;
	unsigned int above_hispeed_delay[__SCHED_IND_DEFAULT_SIZE];
	unsigned int min_sample_time_nr;
	unsigned int min_sample_time[__SCHED_IND_DEFAULT_SIZE];
	unsigned int target_loads_nr;
	unsigned int target_loads[__SCHED_IND_DEFAULT_SIZE];
	unsigned int freq_reporting_policy;
};

struct __sched_ind_migration_limit {
	unsigned int cluster_id;
	unsigned int migration_limit;
};

struct __sched_ind_timer_slack {
	unsigned int cluster_id;
	unsigned int idle_slack_us;
};

struct __sched_ind_boostpulse_endtime {
	unsigned int cluster_id;
	unsigned long long boostpulse_endtime;
};

#endif
