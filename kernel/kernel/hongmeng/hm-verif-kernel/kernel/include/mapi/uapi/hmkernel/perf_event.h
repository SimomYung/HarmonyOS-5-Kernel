/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Definition of perf event
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jul 20 10:25:28 2020
 */
#ifndef UAPI_HMKERNEL_PERF_EVENT_H
#define UAPI_HMKERNEL_PERF_EVENT_H

#include <hmasm/types.h>
#include <hmkernel/capability.h>

enum __perf_pmu_config_cmd {
	__PERF_PMU_CONFIG_INVALID = 0,
	__PERF_PMU_CONFIG_USER_ENABLE,
	__PERF_PMU_CONFIG_USER_DISABLE,
	__PERF_PMU_CONFIG_EVTYPE_ACQUIRE,
	__PERF_PMU_CONFIG_EVTYPE_RELEASE,
	__PERF_PMU_CONFIG_EVTCNT_GET,
	__PERF_PMU_CONFIG_EVTYPE_ACQUIRE_SINGLE,
	__PERF_PMU_CONFIG_EVTYPE_RELEASE_SINGLE,
	__PERF_PMU_CONFIG_EVTCNT_GET_SINGLE,
	__PERF_PMU_CONFIG_EVTCNT_GET_BATCH,
};

#define __PERF_EVTCNT_GET_BATCH_NUM	8U

/* This structure saves values that copy to sysmgr */
struct __perf_event_read_values_s {
	__u64 counter;
	__u64 total_time_enabled;
	__u64 total_time_running;
};

struct __perf_event_timeinfo_s {
	__u64 total_time_enabled;
	__u64 total_time_running;
};

struct __perf_event_batch_read_values_s {
	/* This struct should put before counter */
	struct __perf_event_timeinfo_s timeinfo;
	__u64 counters[__PERF_EVTCNT_GET_BATCH_NUM];
};

enum __perf_dsu_pmu_config_cmd {
	__PERF_DSU_PMU_CONFIG_INVALID = 0,
	__PERF_DSU_PMU_CONFIG_EVTYPE_ACQUIRE,
	__PERF_DSU_PMU_CONFIG_EVTYPE_RELEASE,
	__PERF_DSU_PMU_CONFIG_EVTCNT_GET,
};

enum __perf_event_type {
	__PERF_EVENT_TYPE_INVALID = 0u,
	__PERF_EVENT_TYPE_HARDWARE,
	__PERF_EVENT_TYPE_HARDWARE_ARM_DSU,
	__PERF_EVENT_TYPE_HARDWARE_BREAKPOINT,
	__PERF_EVENT_TYPE_SPE,
	__PERF_EVENT_TYPE_NAMTSO,
	__PERF_EVENT_TYPE_RAW,
	/* SOFTWARE EVENTS SHOULD PUT AFTER HARDWARE EVENTS */
	__PERF_EVENT_TYPE_SOFTWARE,
	__PERF_EVENT_TYPE_SOFTWARE_CPU_CLOCK,
	__PERF_EVENT_TYPE_SOFTWARE_TASK_CLOCK,
	__PERF_EVENT_TYPE_TRACEPOINT,

	__PERF_EVENT_TYPE_MAX,
};

enum __perf_sw_ids {
	__PERF_COUNT_SW_CPU_CLOCK		= 0,
	__PERF_COUNT_SW_TASK_CLOCK		= 1,
	__PERF_COUNT_SW_PAGE_FAULTS		= 2,
	__PERF_COUNT_SW_CONTEXT_SWITCHES	= 3,
	__PERF_COUNT_SW_CPU_MIGRATIONS		= 4,
	__PERF_COUNT_SW_PAGE_FAULTS_MIN		= 5,
	__PERF_COUNT_SW_PAGE_FAULTS_MAJ		= 6,
	__PERF_COUNT_SW_ALIGNMENT_FAULTS	= 7,
	__PERF_COUNT_SW_EMULATION_FAULTS	= 8,
	__PERF_COUNT_SW_DUMMY			= 9,
	__PERF_COUNT_SW_BPF_OUTPUT		= 10,

	__PERF_COUNT_SW_MAX
};

struct __perf_event_attr_s {
	/* PMU event type */
	__u32 type;
	__u32 config;
	__u64 period;
};

struct __perf_breakpoint_attr_s {
	__u32 bp_type;
	__u64 bp_addr;
	__u64 bp_len;
};

#define __PERF_CONFIGURABLE_EVENT_COUNTER_NUM 32UL
#define __PERF_CONFIGURABLE_BREAKPOINT_NUM 16UL
struct __perf_event_config_s {
	struct __perf_event_attr_s evts[__PERF_CONFIGURABLE_EVENT_COUNTER_NUM];
	struct __perf_breakpoint_attr_s bp_evts[__PERF_CONFIGURABLE_BREAKPOINT_NUM];
	unsigned int perf_cnode_idx;
	/* PMU or DEBUG */
	unsigned int event_hardware_type;
};

struct __perf_event_config_single_s {
	/* perf event attr */
	__u32 type;
	__u64 config;
	union {
		__u64 period;
		__u64 sample_freq;
	};
	__u64 sample_type;
#ifdef CONFIG_HISI_AARCH64_BRBE
	__u64 branch_sample_type; /* enum perf_branch_sample_type */
#endif
	__u64 event_id;
	/* If we inherit events, we want to return the parent event id to userspace */
	__u64 primary_event_id;
	__u64 rb;
	__u64 exclude_kernel : 1;
	__u64 exclude_user : 1;
	__u64 sample_id_all : 1;
	__u64 exclude_callchain_kernel : 1;
	__u64 exclude_callchain_user : 1;
	__u64 freq : 1;
	__u64 context_switch : 1;
	__u64 pinned : 1;
	__u64 __reserved : 56;
	union {
		__u64		bp_addr;
		__u64		config1;
	};
	union {
		__u64		bp_len;
		__u64		config2;
	};
	__u64 sample_regs_user;
	__u32 sample_stack_user;
	__s32 clockid;
	__s32 cpu;
	__u32 pid;
	__u32 tid;
	__s32 group_leader_idx;

	__bool cycle_event;
	/*
	 * The value from the last time we stopped counting, with which we
	 * will restart counting. If this is the first time it is enabled,
	 * this value should be 0.
	 */
	__u64 last_count;
	__u64 total_time_enabled;
	__u64 total_time_running;
	__u32 perf_cnode_idx;
};

struct __perf_event_ret_s {
	__u64 counters[__PERF_CONFIGURABLE_EVENT_COUNTER_NUM];
};

#define PMU_SAMPLE_DATA_LEN sizeof(struct __perf_pmu_sample_data)

/*
 * perf report uses cnode_idx to connect the sampled event with the process event.
 * Hence, to display the symbol of the VM, we also need to connect the sampled event
 * in guest's address with the guest image. In perf report, for each vm, we create
 * a new process with a single mmap event to reuse existing symbol parsing & merging
 * mechanism.
 */
#define PMU_SAMPLE_VM_CNODE_IDX_BASE 0x80000000
struct __perf_pmu_sample_data {
	__u64 pc;
	__u64 timestamp;
	__u32 pid;
	__u32 tid;
	__u32 cpuid;
	__u32 cnode_idx;
	char name[HMCAP_THREAD_NAME_SIZE];
};

#define __PERF_RECORD_MISC_KERNEL	(1 << 0)
#define __PERF_RECORD_MISC_USER		(2 << 0)
#define __PERF_RECORD_MISC_MMAP_DATA	(1 << 13)
#define __PERF_RECORD_MISC_COMM_EXEC	(1 << 13)

enum __perf_event_sample_type {
	__PERF_RECORD_LOST = 2,

	__PERF_RECORD_SAMPLE = 9,

	__PERF_RECORD_AUX = 11,

	__PERF_RECORD_SWITCH = 14,
	__PERF_RECORD_SWITCH_CPU_WIDE = 15,

	__PERF_RECORD_MAX,
};

enum __perf_event_sample_format {
	__PERF_SAMPLE_IP = 1U << 0,
	__PERF_SAMPLE_TID = 1U << 1,
	__PERF_SAMPLE_TIME = 1U << 2,
	__PERF_SAMPLE_ADDR = 1U << 3,
	__PERF_SAMPLE_CALLCHAIN = 1U << 5,
	__PERF_SAMPLE_ID = 1U << 6,
	__PERF_SAMPLE_CPU = 1U << 7,
	__PERF_SAMPLE_PERIOD = 1U << 8,
	__PERF_SAMPLE_STREAM_ID = 1U << 9,
	__PERF_SAMPLE_RAW = 1U << 10,
	__PERF_SAMPLE_BRANCH_STACK = 1U << 11,
	__PERF_SAMPLE_REGS_USER = 1U << 12,
	__PERF_SAMPLE_STACK_USER = 1U << 13,
	__PERF_SAMPLE_IDENTIFIER = 1U << 16,

	__PERF_SAMPLE_MAX = 1U << 26,

	__PERF_SAMPLE_SERVER_PID = 1U << 31,
};

#ifdef CONFIG_HISI_AARCH64_BRBE
enum perf_branch_sample_type_shift {
	__PERF_SAMPLE_BRANCH_USER_SHIFT		= 0, /* user branches */
	__PERF_SAMPLE_BRANCH_KERNEL_SHIFT		= 1, /* kernel branches */
	__PERF_SAMPLE_BRANCH_HV_SHIFT			= 2, /* hypervisor branches */

	__PERF_SAMPLE_BRANCH_ANY_SHIFT		= 3, /* any branch types */
	__PERF_SAMPLE_BRANCH_ANY_CALL_SHIFT	= 4, /* any call branch */
	__PERF_SAMPLE_BRANCH_ANY_RETURN_SHIFT	= 5, /* any return branch */
	__PERF_SAMPLE_BRANCH_IND_CALL_SHIFT	= 6, /* indirect calls */
	__PERF_SAMPLE_BRANCH_ABORT_TX_SHIFT	= 7, /* transaction aborts */
	__PERF_SAMPLE_BRANCH_IN_TX_SHIFT		= 8, /* in transaction */
	__PERF_SAMPLE_BRANCH_NO_TX_SHIFT		= 9, /* not in transaction */
	__PERF_SAMPLE_BRANCH_COND_SHIFT		= 10, /* conditional branches */

	__PERF_SAMPLE_BRANCH_CALL_STACK_SHIFT	= 11, /* call/ret stack */
	__PERF_SAMPLE_BRANCH_IND_JUMP_SHIFT	= 12, /* indirect jumps */
	__PERF_SAMPLE_BRANCH_CALL_SHIFT		= 13, /* direct call */

	__PERF_SAMPLE_BRANCH_NO_FLAGS_SHIFT	= 14, /* no flags */
	__PERF_SAMPLE_BRANCH_NO_CYCLES_SHIFT	= 15, /* no cycles */

	__PERF_SAMPLE_BRANCH_TYPE_SAVE_SHIFT	= 16, /* save branch type */
	__PERF_SAMPLE_BRANCH_HW_INDEX_SHIFT	= 17, /* save low level index of raw branch records */
	__PERF_SAMPLE_BRANCH_PRIV_SAVE_SHIFT	= 18, /* save privilege mode */
	__PERF_SAMPLE_BRANCH_COUNTERS_SHIFT	= 19, /* save occurrences of events on a branch */
	__PERF_SAMPLE_BRANCH_MAX_SHIFT
};

enum perf_branch_sample_type {
	__PERF_SAMPLE_BRANCH_USER			= 1U << __PERF_SAMPLE_BRANCH_USER_SHIFT,
	__PERF_SAMPLE_BRANCH_KERNEL		= 1U << __PERF_SAMPLE_BRANCH_KERNEL_SHIFT,
	__PERF_SAMPLE_BRANCH_HV			= 1U << __PERF_SAMPLE_BRANCH_HV_SHIFT,

	__PERF_SAMPLE_BRANCH_ANY			= 1U << __PERF_SAMPLE_BRANCH_ANY_SHIFT,
	__PERF_SAMPLE_BRANCH_ANY_CALL		= 1U << __PERF_SAMPLE_BRANCH_ANY_CALL_SHIFT,
	__PERF_SAMPLE_BRANCH_ANY_RETURN	= 1U << __PERF_SAMPLE_BRANCH_ANY_RETURN_SHIFT,
	__PERF_SAMPLE_BRANCH_IND_CALL		= 1U << __PERF_SAMPLE_BRANCH_IND_CALL_SHIFT,
	__PERF_SAMPLE_BRANCH_ABORT_TX		= 1U << __PERF_SAMPLE_BRANCH_ABORT_TX_SHIFT,
	__PERF_SAMPLE_BRANCH_IN_TX		= 1U << __PERF_SAMPLE_BRANCH_IN_TX_SHIFT,
	__PERF_SAMPLE_BRANCH_NO_TX		= 1U << __PERF_SAMPLE_BRANCH_NO_TX_SHIFT,
	__PERF_SAMPLE_BRANCH_COND			= 1U << __PERF_SAMPLE_BRANCH_COND_SHIFT,

	__PERF_SAMPLE_BRANCH_CALL_STACK	= 1U << __PERF_SAMPLE_BRANCH_CALL_STACK_SHIFT,
	__PERF_SAMPLE_BRANCH_IND_JUMP		= 1U << __PERF_SAMPLE_BRANCH_IND_JUMP_SHIFT,
	__PERF_SAMPLE_BRANCH_CALL			= 1U << __PERF_SAMPLE_BRANCH_CALL_SHIFT,

	__PERF_SAMPLE_BRANCH_NO_FLAGS		= 1U << __PERF_SAMPLE_BRANCH_NO_FLAGS_SHIFT,
	__PERF_SAMPLE_BRANCH_NO_CYCLES	= 1U << __PERF_SAMPLE_BRANCH_NO_CYCLES_SHIFT,

	__PERF_SAMPLE_BRANCH_TYPE_SAVE	=
		1U << __PERF_SAMPLE_BRANCH_TYPE_SAVE_SHIFT,
	__PERF_SAMPLE_BRANCH_HW_INDEX		= 1U << __PERF_SAMPLE_BRANCH_HW_INDEX_SHIFT,
	__PERF_SAMPLE_BRANCH_PRIV_SAVE	= 1U << __PERF_SAMPLE_BRANCH_PRIV_SAVE_SHIFT,
	__PERF_SAMPLE_BRANCH_COUNTERS		= 1U << __PERF_SAMPLE_BRANCH_COUNTERS_SHIFT,
	__PERF_SAMPLE_BRANCH_MAX			= 1U << __PERF_SAMPLE_BRANCH_MAX_SHIFT,
};

#define __BRBE_ALLOWED_BASE_BRANCH_FILTERS (__PERF_SAMPLE_BRANCH_USER		| \
						__PERF_SAMPLE_BRANCH_ANY			| \
						__PERF_SAMPLE_BRANCH_ANY_CALL		| \
						__PERF_SAMPLE_BRANCH_ANY_RETURN	| \
						__PERF_SAMPLE_BRANCH_IND_CALL		| \
						__PERF_SAMPLE_BRANCH_COND			| \
						__PERF_SAMPLE_BRANCH_IND_JUMP		| \
						__PERF_SAMPLE_BRANCH_CALL			| \
						__PERF_SAMPLE_BRANCH_NO_FLAGS		| \
						__PERF_SAMPLE_BRANCH_NO_CYCLES	| \
						__PERF_SAMPLE_BRANCH_TYPE_SAVE	| \
						__PERF_SAMPLE_BRANCH_HW_INDEX		| \
						__PERF_SAMPLE_BRANCH_PRIV_SAVE)
#ifdef CONFIG_HISI_AARCH64_BRBE_EL1_SUPPORT
#define __BRBE_ALLOWED_BRANCH_FILTERS (__BRBE_ALLOWED_BASE_BRANCH_FILTERS		| \
						__PERF_SAMPLE_BRANCH_KERNEL)
#else
#define __BRBE_ALLOWED_BRANCH_FILTERS __BRBE_ALLOWED_BASE_BRANCH_FILTERS
#endif

#ifdef CONFIG_HISI_AARCH64_BRBE_64_ENTRY
#define __PERF_MAX_BRANCH_NR 64
#else
#define __PERF_MAX_BRANCH_NR 32
#endif

struct __perf_branch_entry {
	__u64 from;
	__u64 to;
	__u64 flags;
};

struct __perf_branch_stack {
	__u64 nr;
	struct __perf_branch_entry br_entry[__PERF_MAX_BRANCH_NR];
};

struct arm_brbe_ability {
	unsigned int cpu_brbe_nr[CONFIG_NR_CPUS];
	struct __perf_branch_stack branch_records[CONFIG_NR_CPUS];
};

enum brbe_support_state {
	BRBE_UN_INIT = -1,
	BRBE_SUPPORT = 0,
	BRBE_NOT_SUPPORT = 1
};
#endif

struct __perf_event_header {
	__u32 type;
	__u16 misc;
	__u16 size;
};

struct __perf_raw_frag {
	union {
		struct __perf_raw_frag *next;
		unsigned long pad;
	};
	__u64 data;
	__u32 size;
};

struct __perf_raw_record {
	struct __perf_raw_frag frag;
	__u32 size;
};

#define __PERF_MAX_SERVER_NR 8

struct __perf_server_entry {
	__u64 nr;
	__u64 server_pid[__PERF_MAX_SERVER_NR];
};

struct __perf_sample_data {
	__u64 sample_flags;
	__u64 type;
	__u16 dyn_size;
	__u16 ustack_size;

	__u64 ip;
	struct __perf_raw_record *raw;
	struct {
		__u32 pid;
		__u32 tid;
	} tid_entry;
	__u64 time;
	__u64 id;
	__u64 stream_id;
	struct {
		__u32 cpu;
		__u32 reserved;
	} cpu_entry;
	__u64 period;
	__u64 addr;
	void *callchain;
	struct __perf_server_entry server_entry;
	struct {
		__u64 abi;
		/* NOTE: If regs is not NULL, it must correspond to a real actv */
		struct arch_regs *regs;
	} uregs;
#ifdef CONFIG_HISI_AARCH64_BRBE
	struct __perf_branch_stack *br_stack;
#endif
};

enum __perf_callchain_context {
	__PERF_CONTEXT_KERNEL		= (__u64)-128,
	__PERF_CONTEXT_USER		= (__u64)-512,

	__PERF_CONTEXT_MAX		= (__u64)-4095,
};

/* User ABI */
#define CONFIG_ts_enable_CFG		config
#define CONFIG_ts_enable_LO		0
#define CONFIG_ts_enable_HI		0
#define CONFIG_pa_enable_CFG		config
#define CONFIG_pa_enable_LO		1
#define CONFIG_pa_enable_HI		1
#define CONFIG_pct_enable_CFG		config
#define CONFIG_pct_enable_LO		2
#define CONFIG_pct_enable_HI		2
#define CONFIG_jitter_CFG			config
#define CONFIG_jitter_LO			16
#define CONFIG_jitter_HI			16
#define CONFIG_branch_filter_CFG		config
#define CONFIG_branch_filter_LO		32
#define CONFIG_branch_filter_HI		32
#define CONFIG_load_filter_CFG		config
#define CONFIG_load_filter_LO		33
#define CONFIG_load_filter_HI		33
#define CONFIG_store_filter_CFG		config
#define CONFIG_store_filter_LO		34
#define CONFIG_store_filter_HI		34

#define CONFIG_event_filter_CFG		config1
#define CONFIG_event_filter_LO		0
#define CONFIG_event_filter_HI		63

#define CONFIG_min_latency_CFG		config2
#define CONFIG_min_latency_LO		0
#define CONFIG_min_latency_HI		11

#define CONFIG_discard_CFG		config2
#define CONFIG_discard_LO		12
#define CONFIG_discard_HI		12

#define CONFIG_freeze_CFG		config2
#define CONFIG_freeze_LO		13
#define CONFIG_freeze_HI		13

#define CONFIG_nevent0_filter_CFG		config2
#define CONFIG_nevent0_filter_LO		16
#define CONFIG_nevent0_filter_HI		31

#define CONFIG_nevent1_filter_CFG		config2
#define CONFIG_nevent1_filter_LO		32
#define CONFIG_nevent1_filter_HI		63

#define GENMASK(h, l) \
	(((~__UL(0)) - (__UL(1) << (l)) + 1) & \
	 (~__UL(0) >> (BITS_PER_LONG - 1 - (h))))

#define _CONFIG_GET_VAL(attr, cfg, lo, hi)				\
	((((attr)->cfg) >> (lo)) & GENMASK((hi) - (lo), 0))

#define CONFIG_GET_VAL(attr, name)					\
	_CONFIG_GET_VAL(attr,						\
			  CONFIG_##name##_CFG,			\
			  CONFIG_##name##_LO,			\
			  CONFIG_##name##_HI)

/**
 * PERF_RECORD_AUX::flags bits
 */
 /* record was truncated to fit */
#define PERF_AUX_FLAG_TRUNCATED		0x01
/* snapshot from overwrite mode */
#define PERF_AUX_FLAG_OVERWRITE		0x02
/* record contains gaps */
#define PERF_AUX_FLAG_PARTIAL		0x04
/* sample collided with another */
#define PERF_AUX_FLAG_COLLISION		0x08

#endif
