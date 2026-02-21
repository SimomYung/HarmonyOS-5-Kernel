/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Declaration of sched module for uapi
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 25 11:03:00 2019
 */
#ifndef UAPI_MODULE_SCHED_MODULE_H
#define UAPI_MODULE_SCHED_MODULE_H

#include <hmkernel/capability.h>
#include <hmkernel/cpuset.h>

#define SCHED_TIMESLICE_MS_MAX_SIZE	5

/* Thread(tcb) operaiton */
#define __SCHED_SET_CONF 0UL
#define __SCHED_GET_CONF 1UL
#define __SCHED_TCB_SET_ATTR 2UL
#define __SCHED_TCB_GET_ATTR 3UL
#define __SCHED_TRANS 4UL
#define __SCHED_MARK_LOAD_WEIGHT 5UL
#define __SCHED_MARK_RESTRICTED_RT 6UL
#define __SCHED_GET_RESTRICTED_RT_STATE 7UL
/* Sched operation */
#define __SCHED_LOAD_BALANCER_SWITCH 100UL
#define __SCHED_GET_SCHED_DEBUG_ATTR 101UL
#define __SCHED_GET_LOADAVG 102UL
#define __SCHED_CPUISO_SET_CONF 103UL
#define __SCHED_CPUISO_GET_CONF 104UL
#define __SCHED_SET_CHILD_RUNS_FIRST 105UL
#define __SCHED_GET_CHILD_RUNS_FIRST 106UL
#define __SCHED_GET_SCHED_STAT 107UL
#define __SCHED_SET_SCHED_SCHEDSTATS 108UL
/* Preempt operation */
#define __SCHED_PREEMPT_CONF 203UL
#define __SCHED_PREEMPT_PROCESS_EXIT 204UL
#define __SCHED_PREEMPT_SET_SLV 205UL
#define __SCHED_PREEMPT_RESCHED 206UL
/* Bandwidth operation */
#define __SCHED_BANDWIDTH_SET_CONF 301UL
#define __SCHED_BANDWIDTH_GET_CONF 302UL
/* Stat operation */
#define __SCHED_GET_STAT_CONF 401UL
/* Load tracking operation */
#define __SCHED_LT_SET_CONF 500UL
#define __SCHED_LT_GET_CONF 501UL
#define __SCHED_LT_SET_GLOBAL_CONF 502UL
#define __SCHED_LT_GET_GLOBAL_CONF 503UL
#define __SCHED_LT_GROUP_ALLOC 504UL
#define __SCHED_LT_GROUP_FREE 505UL
#define __SCHED_LT_SET_GROUP_CONF 506UL
#define __SCHED_SET_CPU_CAPACITY_MARGIN  507UL
#define __SCHED_GET_CPU_CAPACITY_MARGIN  508UL
#define __SCHED_SET_TCB_CAPACITY_MARGIN  509UL
#define __SCHED_GET_TCB_CAPACITY_MARGIN  510UL
#define __SCHED_GET_CAPACITY_DISCOUNT_MT  511UL
#define __SCHED_SET_CAPACITY_DISCOUNT_MT  512UL
/* ICES operation */
#define __SCHED_SET_GLOBAL_BOOST 600UL
/* boost for system startup */
#define __SCHED_SET_BOOT_BOOST 601UL
/* mark current tcb as an irq tcb */
#define __SCHED_MARK_IRQ 602UL
/* resched to interrupt idle cpu */
#define __SCHED_IDLE_INTERRUPT 603UL
#define __SCHED_DEADLINE_GROUP_DEBUG 604UL
/* restore saved uctx from fs info */
#define __SCHED_RESTORE_UCTX 607UL
/* core scheduling operation */
#define __SCHED_CORESCHED_SET_CONF 608UL
#define __SCHED_SET_ICES_MIN_RUNTIME 609UL
#define __SCHED_GET_ICES_MIN_RUNTIME 610UL
/* render rt operation */
#define __SCHED_REDNER_RT_INIT 611UL
#define __SCHED_REDNER_RT_DESTORY 612UL
#define __SCHED_REDNER_RT_GET_RTHREAD 613UL
/* unified collection of thread info */
#define __SCHED_COLLECT_CPU_LOAD_INFO 614UL
/* smt time stats */
#define __SCHED_SMT_TIME_STATS_READ 615UL
/* bind/unbind cpu */
#define __SCHED_BIND_CPU 616UL
#define __SCHED_UNBIND_CPU 617UL
/* energy mode */
#define __SCHED_SET_ENERGY_MODE 618UL
/* Set/Get capacity aware for sched cpu quota */
#define __SCHED_SET_CAPACITY_AWARE 619UL
#define __SCHED_GET_CAPACITY_AWARE 620UL
#define __FFTS_THD_SCHED_INFO_SYNC 621UL
/* mark preempted tcb SCHED_FLAG_BALAN_SKIP flag */
#define __SCHED_PIN_PREEMPTED_TCB 622UL

/* threshold for control smt expeller util */
#define __SCHED_SMT_SET_EXPELLER_UTIL 623UL
#define __SCHED_SMT_GET_EXPELLER_UTIL 624UL
/* toptask policy enable */
#define __SCHED_SET_TOPTASK_POLICY 625UL
/* cpu time matrix */
#define __SCHED_GET_CPU_BUSYTIME 626UL
#define __SCHED_COLLECT_CAP_INFO 	627UL

#define __SCHED_GET_CLUSTER_UTIL 628UL
#define __SCHED_SET_CLUSTER_FREQ 629UL

struct sched_param_s {
	cref_t thread_cref;
	int nice;
	int policy;
	int priority;
	int max_available_priority;
	__cpuset_t affinity;
	cref_t prev_tcb_cref;
	cref_t preempt_tcb_cref;
};

struct sched_config_s {
	/* SCHED_RT */
	unsigned int sched_timeslice_ms;
};

#ifdef CONFIG_UNIFIED_COLLECTION
struct __ucollection_cpu_load_info {
	__u64 run_utime;
	__u64 run_stime;
	__u64 load;
};
#endif

#define LOADAVG_NR 3U
/* 3 items of loadavg summary */
#define LOADAVG_1MIN_IDX 0U
#define LOADAVG_5MIN_IDX 1U
#define LOADAVG_15MIN_IDX 2U
#define LOADAVG_ACCURACY_SHIFT 16ULL

/* loadavg = sum / div */
struct loadavg_s {
	__u64 load_sum[LOADAVG_NR];
	__u64 load_scale[LOADAVG_NR];
};

#define SCHED_RT_BANDWIDTH_RUNTIME 0U
#define SCHED_RT_BANDWIDTH_PERIOD 1U
#define SCHED_OTHER_BANDWIDTH_QUOTA 2U
#define SCHED_OTHER_BANDWIDTH_PERIOD 3U
#define SCHED_OTHER_BANDWIDTH_CPU_MAX 4U
#define SCHED_OTHER_BANDWIDTH_CPU_RT_QUOTA 5U
#define SCHED_RT_SOFT_BANDWIDTH_RUNTIME 6U

struct sched_bandwidth_args_s {
	unsigned int opt;

	int period;
	int max_runtime;
	cref_t scheduler; 	/* for ices ml sched only */
};

#define SCHED_CPU_STAT 0U

struct sched_cpu_stat_s {
	__u64 run_usec;
	cref_t scheduler;
};

struct sched_stat_args_s {
	__u32 opt;
	struct sched_cpu_stat_s cpu_stat;
};

struct sched_module_args_s {
	struct sched_config_s sched_conf;
	struct sched_param_s tcb_param;
	__u32 flag;
};

enum sched_load_balancer_cmd_e {
	__LOAD_BALANCER_SWITCH_ON,
	__LOAD_BALANCER_SWITCH_OFF,
	__LOAD_BALANCER_STAT,
};

struct sched_load_balancer_args_s {
	enum sched_load_balancer_cmd_e cmd;
	_Bool stat;
};

/* Used by tcb SET/GET attr operation */
#define SCHED_TCB_FLAG_PARAM				1U
#define SCHED_TCB_FLAG_AFFINITY				(1U << 1)
#define SCHED_TCB_FLAG_SCHEDULER			(1U << 2)
#define SCHED_TCB_FLAG_NICE				(1U << 3)

/* global preempt setting */
#define PREEMPT_ENABLE			0U /* enable reschedule (default) */
#define PREEMPT_DISABLE			1U /* disable reschedule in module */

/* process preempt setting */
#define PREEMPT_PROCESS_REGISTER	100U /* register process preempt */
#define PREEMPT_PROCESS_UNREGISTER	101U /* unregister process preempt */

/*
 * process sched level setting
 *
 * sched level is a per-process sched control.
 * Normally, only threads having >= current sched level of process at current cpu
 * level can run, that is: thread.slv >= process.slv(cpu)
 * Specially, when a lower sched level thread disable process preempt, it will be allowed to
 * run and throttled as long as preempt is enabled again.
 *
 * Some default config:
 * 1. all processes start with sched level 0 at all available cpus
 * 2. by default, all process's threads are at sched level 0, and CAN be adjusted;
 * 3. all actvs run with sched level 0, and CANNOT be adjusted.
 */
#define PREEMPT_SLV_SET_PROCESS		200U
#define PREEMPT_SLV_SET_THREAD		201U

/* preempt ctrl setting */
#define PREEMPT_CLEAR_SYS_EXCEPTION	301U /* preempt exception handling is completed */
#define PREEMPT_PROCESS_STAT_QUERY	302U /* query if preempt process feature is enabled or not */

struct sched_preempt_args_s {
	cref_t thread_cref;
	unsigned int type;
};

struct sched_preempt_slv_args_s {
	__u32 cmd;
	__u32 cpu;
	__u32 slv;
	cref_t thread_cref;
};

#ifdef CONFIG_HM_SCHED_TRANS
struct sched_trans_args_s {
	cref_t src_cref;
	cref_t dst_cref;
	__u32 restore;
};
#endif

/* load tracking setting */
#define SCHED_LT_NON_GROUP		0U
#define SCHED_LT_POWER_GROUP		1U
#define SCHED_LT_ENERGY_GROUP		2U
#define SCHED_LT_CAPACITY_GROUP		3U
#define SCHED_LT_NR_GROUP		4U

#define SCHED_LT_DEFAULT_GROUP		SCHED_LT_NON_GROUP

#define SCHED_LT_FLAG_GROUP			1U
#define SCHED_LT_FLAG_BOOST			(1U << 1)
#define SCHED_LT_FLAG_MIN			(1U << 2)
#define SCHED_LT_FLAG_MAX			(1U << 3)
#define SCHED_LT_FLAG_INIT_TASK_LOAD 		(1U << 4)
#define SCHED_LT_FLAG_TCB_WINDOW_SZ		(1U << 5)
#define SCHED_LT_FLAG_DEADLINE			(1U << 6)
#define SCHED_LT_FLAG_TASK_MIN			(1U << 7)
#define SCHED_LT_FLAG_TASK_MAX			(1U << 8)
#define SCHED_LT_FLAG_TASK_ALL			(SCHED_LT_FLAG_TASK_MIN | SCHED_LT_FLAG_TASK_MAX)
#define SCHED_LT_FLAG_PREFER_IDLE		(1U << 9)
#define SCHED_LT_FLAG_SMARTIO_GROUP		(1U << 10)
#define SCHED_LT_FLAG_ATTACH			(1U << 11)
#ifdef CONFIG_TIMER_SLACK
#define SCHED_LT_FLAG_TS_EXEMPT_MS		(1U << 13)
#define SCHED_LT_FLAG_TS_MAX_DELAY_MS		(1U << 14)
#define SCHED_LT_FLAG_TS_PCT			(1U << 15)
#endif
#define SCHED_LT_FLAG_SMT_EXPELLER	(1U << 16)
#define SCHED_LT_FLAG_TASK_UCLAMP_EFF	(1U << 17)
#define SCHED_LT_FLAG_LOAD_TRANS	(1U << 20)
#define SCHED_LT_FLAGS_INIT			(SCHED_LT_FLAG_ATTACH |		\
						 SCHED_LT_FLAG_INIT_TASK_LOAD |	\
						 SCHED_LT_FLAG_SMARTIO_GROUP)

#define SCHED_LT_FLAG_TASK_BOOST		(1U << 18)
#define SCHED_LT_FLAG_TOPLOAD_ENABLE		(1u << 19)

#define SCHED_LT_FLAG_GLOBAL_INIT_LOAD		1U
#define SCHED_LT_FLAG_GLOBAL_UCLAMP_MAX		2U
#define SCHED_LT_FLAG_GLOBAL_UCLAMP_MIN		4U
#define SCHED_LT_FLAG_GLOBAL_UCLAMP_MIN_RT	8U

#define SCHED_LT_GROUP_MAX		7U
#define SCHED_LT_BOOST_MIN		(-100)
#define SCHED_LT_BOOST_MAX		100
#define SCHED_LT_RATIO_MAX		100U
#define SCHED_LT_TCB_WINDOW_SZ_MIN	5U
#define SCHED_LT_TCB_WINDOW_SZ_MAX	1000U
#ifdef CONFIG_TIMER_SLACK
#define SCHED_LT_TS_EXEMPT_MS_MIN	40U
#define SCHED_LT_TS_EXEMPT_MS_MAX	10000U
#define SCHED_LT_TS_MAX_DELAY_MS_MAX	200U
#define SCHED_LT_TS_PCT_MAX		100U
#endif
#define SCHED_LT_TASK_LOAD_MAX		1024U
#define SCHED_LT_TASK_LOAD_MIN		0U
#define SCHED_LT_SMARTIO_GROUP_DEF	0U
#define SCHED_LT_SMARTIO_GROUP_MAX	7U

#define SCHED_LT_TCB_WINDOW_SZ_DEFAULT	20U

struct sched_lt_config_s {
	__u32 id;
	__u32 group;
	__s32 boost;
	__s32 task_boost;
	__u32 min;
	__u32 max;
	__u32 grp_init_load;
	__u32 tcb_window_sz;
	__u32 prefer_idle;
	__u32 smartio_group;
	_Bool load_trans;
#ifdef CONFIG_TIMER_SLACK
	__u32 timer_slack_exempt_ms;
	__u32 timer_slack_max_delay_ms;
	__u32 timer_slack_pct;
#endif
	__u32 smt_expeller;
	__u16 effective_min;
	__u16 effective_max;
	_Bool topload_enable;
};

struct deadline_args_s {
	__u32 cmd;
	__u32 flags;
	__u32 interval;
	__u32 max_util;
	__u32 min_util;
	__u32 valid_util;
	__u32 prefer_cluster_id;
	__u32 freq;
	__s32 margin;
	__s32 grp_id;
	__s32 state;
	__s32 boost_duration;
	__s32 boost_util;
	__s32 ckp_up_rate;
	__s32 ckp_down_rate;
};

struct sched_lt_args_s {
	cref_t cref;
	__u32 flag;
	struct sched_lt_config_s configs;
	struct deadline_args_s dl_args;
} __attribute__((aligned(8)));

struct sched_lt_global_args_s {
	__u32 flag;
	__u32 init_load;
	__u16 max_load;
	__u16 min_load;
	__u16 rt_default_min_load;
} __attribute__((aligned(8)));

enum lt_cap_discount {
	LT_CPU_CAP_DC,
	LT_TOP_TASK_CAP_DC,
	LT_TASK_CAP_DC,
	LT_CAP_DC_NR,
};

struct sched_cap_dc_mt_args_s {
	__u32 type;
	__u32 discount;
} __attribute__((aligned(8)));

/*
 * For the deadline_set_config() calls
 */
#define DEADLINE_FLAG_INTERVAL		1U
#define DEADLINE_FLAG_MAX_UTIL		2U
#define DEADLINE_FLAG_VALID_UTIL	4U
#define DEADLINE_FLAG_PREFER_CLUSTER	8U
#define DEADLINE_FLAG_FREQ		16U
#define DEADLINE_FLAG_MIN_UTIL		32U
#define DEADLINE_FLAG_BOOST		64U
#define DEADLINE_FLAG_CKP_RATE		128U
#define DEADLINE_FLAG_ALL		  \
	(DEADLINE_FLAG_INTERVAL		| \
	 DEADLINE_FLAG_MAX_UTIL		| \
	 DEADLINE_FLAG_VALID_UTIL	| \
	 DEADLINE_FLAG_PREFER_CLUSTER	| \
	 DEADLINE_FLAG_FREQ		| \
	 DEADLINE_FLAG_MIN_UTIL		| \
	 DEADLINE_FLAG_BOOST		| \
	 DEADLINE_FLAG_CKP_RATE)

#define DEADLINE_INVALID_GROUP_ID	(-1)
#define DEADLINE_INTERVAL_FREE		(0)
#define DEADLINE_GROUP_NUM		500
#define DEADLINE_GROUP_DEBUG_SIZE	4096
#define DEADLINE_AI_GROUP_NUM		2
#define DEADLINE_AI_GROUP_ID		(DEADLINE_GROUP_NUM - DEADLINE_AI_GROUP_NUM)

enum sched_lt_deadline_cmd {
	SCHED_DL_BEGIN = 0,
	SCHED_DL_END,
	SCHED_DL_GET_STATE,
	SCHED_DL_STOP,
	SCHED_DL_CHECKPOINT,

	// The follow cmd not allows common permission app to use
	SCHED_DL_SET_CONFIG,
	SCHED_DL_SET_GROUP_ID,
	SCHED_DL_KERNEL_CMD_NR,

	SCHED_DL_ALLOC_GROUP = SCHED_DL_KERNEL_CMD_NR,
	SCHED_DL_FREE_GROUP,
	SCHED_DL_QUERY_GROUP,
	SCHED_DL_UPDATE_THREAD,
	SCHED_DL_UPDATE_CONFIG,
	SCHED_DL_CMD_NR
};

#define SCHED_CPUISO_FLAG_MIN_CPUS	1U
#define SCHED_CPUISO_FLAG_MAX_CPUS	(1U << 1)
#define SCHED_CPUISO_FLAG_TASK_BUSY	(1U << 2)
#define SCHED_CPUISO_FLAG_CPU_BUSY	(1U << 3)
#define SCHED_CPUISO_FLAG_ACTIVE_CPUS	(1U << 4)
#define SCHED_CPUISO_FLAG_ENABLE	(1U << 5)
#define SCHED_CPUISO_FLAG_ISO_CPU	(1U << 6)
#define SCHED_CPUISO_FLAG_SET_ISOLATE	(1U << 7)
#define SCHED_CPUISO_FLAG_SET_UNISOLATE	(1U << 8)
#define SCHED_CPUISO_FLAG_ENABLE_ALL	(1U << 9)
#define SCHED_CPUISO_FLAG_DISABLE_ALL	(1U << 10)

#define CPUISO_UNUSED_CLUSTER	(~0U)

struct sched_cpuiso_args_s {
	__u32 flag;
	__u32 cluster;
	__u32 value;
} __attribute__((aligned(8)));

#define SMT_MAX_EXPELLER_UTIL 1024U

enum smt_expel_type {
	SMT_EXPELLEE = 0,
	SMT_EXPELLER_UTIL,
	SMT_EXPELLER,
	SMT_EXPELLER_FORCE,
	SMT_EXPELLER_FORCE_LONG,
	SMT_EXPELLER_MAX,
};

enum {
	PERF_CTRL_INVALID = 0,
	GET_SCHED_STAT = 1,
	SET_TASK_UTIL,
	GET_IPA_STAT,
	GET_DDR_FLUX,
	GET_RELATED_TID,
	GET_DEV_FREQ,
	GET_THERMAL_CDEV_POWER,
	SET_FRAME_RATE,
	SET_FRAME_MARGIN,
	SET_FRAME_STATUS,
	SET_TASK_RTG,
	SET_RTG_CPUS,
	SET_RTG_FREQ,
	SET_RTG_FREQ_UPDATE_INTERVAL,
	SET_RTG_UTIL_INVALID_INTERVAL,
	GET_GPU_FENCE,
	/* render related threads */
	INIT_RENDER_PID,
	GET_RENDER_RT,
	STOP_RENDER_RT,
	GET_RENDER_HT,
	DESTROY_RENDER_RT,
	SET_RTG_LOAD_MODE,
	SET_RTG_ED_PARAMS,
	GET_DEV_CAP,
	SET_RT_EXTEND_PRIO,
	SET_FAVOR_SMALL_CAP,
	SET_TASK_RTG_MIN_FREQ,
	SET_TASK_MIN_UTIL,
	GET_GPU_BUFFER_SIZE,
	SET_LB_POLICY,
	RESET_LB_POLICY,
	GET_LB_LITE_INFO,
	ENABLE_GPU_LB,
	GET_CPU_BUSY_TIME,
	SET_TASK_MAX_UTIL,
	GET_TASK_YIELD_TIME,
	SET_TASK_L3C_PART,
	GET_HVGR_GPU_FENCE,
	GET_HVGR_GPU_BUFFER_SIZE,
	SET_TASK_CPU_HWP,
	SET_TASK_NO_SMT,
	/* render related threads global */
	GET_TID_NAME,
	GET_CGRP_NAME,
	INIT_RENDER_TID_GLO,
	GET_RENDER_RT_GLO,
	DESTROY_RENDER_RT_GLO,
	/* frame vip prio */
	SET_FRAME_VIP_PRIO,
	GET_TASK_PID,
	/* cfs bandwidth throttle */
	SET_TASK_USERSPACE_THROTTLE,
	SET_HECA_CONFIG,
	SET_FLO_CONFIG,
	SET_DDR_SCENE,
	SET_MICROARCHCFG_SCENE,

	GET_CLUSTER_UTIL,
	GET_TOP_PID_UTIL,
	SET_TOP_PID_CPUS,
	SET_CLUSTER_MIN_FREQ,

	PERF_CTRL_INIT_SPREAD_AFFINITY,
	PERF_CTRL_WRITE_SPREAD_AFFINITY,
	PERF_CTRL_READ_SPREAD_AFFINITY,

	SET_HIDE_RT_POLICY,

	PERF_CTRL_MAX_NR,
};

struct sched_mark_load_weight_args_s {
	cref_t cref;
	_Bool is_heavy_load_weight;
} __attribute__((aligned(8)));

struct thread_util_s {
	__s32 tid;
	unsigned long util;
	unsigned int woken_count;
	unsigned int prio;
};

#define REDNER_RT_THREAD_MAX 21
#define SCHED_RENDER_RT_FLAG_GET_RTHREAD		1U
#define SCHED_RENDER_RT_FLAG_GET_HTHREAD		(1U << 1)
struct sched_get_render_rt_args_s {
	__u32 flag;
	union {
		struct render_rt_s {
			__s32 render_pid;
			int num;
			struct thread_util_s utils[REDNER_RT_THREAD_MAX];
		} rt_result;
		struct render_ht_s {
			__s32 render_pid;
			struct thread_util_s utils[REDNER_RT_THREAD_MAX];
		} ht_result;
	} result;
}__attribute__((aligned(8)));


#define SCHED_RENDER_RT_FLAG_INIT		1U
#define SCHED_RENDER_RT_FLAG_INIT_RTHREAD	(1U << 1)
struct sched_render_init_args_s {
	cref_t thread_ref;
	__u32 flag;
	struct render_init_paras_s {
		__s32 render_pid;
		__bool force_init;
	} init_para;
}__attribute__((aligned(8)));

struct sched_mark_rt_restricted_args_s {
	cref_t cref;
	_Bool is_restricted_rt;
} __attribute__((aligned(8)));

struct sched_res_rt_state_args_s {
	cref_t cref;
} __attribute__((aligned(8)));

/* smt time stats */
#define SMT_TIME_MAX_BUF_SIZE 2048U

struct sched_smt_time_args_s {
	__u32 cpuid;
	__u64 rsize;
	char buf[SMT_TIME_MAX_BUF_SIZE];
} __attribute__((aligned(8)));

#define MAX_CPU_RUNTIME_CNT	12
struct sched_cpu_busytime_args_s {
	__s32 cpu_count;
	__u64 running_time[MAX_CPU_RUNTIME_CNT];
} __attribute__((aligned(8)));

struct sched_cpustat_args_s {
	__u32 yld_count;
	__u32 sched_count;
	__u32 sched_goidle;
	__u32 ttwu_count;
	__u32 ttwu_local;
	__u64 rq_cpu_time;
	__u64 run_delay;
	__u64 pcount;
};

/* Used by trace point */
enum sched_info_type {
	SCHED_INFO_AFFINITY = 0,
	SCHED_INFO_LOAD,
	SCHED_INFO_GROUP,
	SCHED_INFO_RESTRICTED,
	SCHED_INFO_EXPEL,
	NR_SCHED_INFO
};

typedef struct {
	__u32 affinity : 32;
	__u32 load : 10;
	__u32 group : 2;
	__u32 restricted : 1;
	__u32 expel : 3;
} sched_info_struct;

struct sched_cluster_util_args_s {
	__s32 cluster_id;
	__s32 sum_util;
	__s32 binded_util;
};

struct sched_cluster_freq_args_s {
	__u32 cluster_id;
	__u32 freq;
};

#endif
