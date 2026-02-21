/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Compat for linux/perf_event.h
 * Author: Huawei OS Kernel Lab
 * Create: Tue Mar 15 21:24:39 2022
 */
#ifndef _LINUX_PERF_EVENT_H
#define _LINUX_PERF_EVENT_H

#include <linux/types.h>
#include <linux/ioctl.h>
#include <asm/byteorder.h>

#define MAX_REV_NUM (118 * 8 + 4)  /* align to 1k. */

enum perf_type_id {
	PERF_TYPE_HARDWARE			= 0,
	PERF_TYPE_SOFTWARE			= 1,
	PERF_TYPE_TRACEPOINT			= 2,
	PERF_TYPE_HW_CACHE			= 3,
	PERF_TYPE_RAW				= 4,
	PERF_TYPE_BREAKPOINT			= 5,
	PERF_TYPE_MAX,
};

enum perf_hw_id {
	PERF_COUNT_HW_CPU_CYCLES		= 0,
	PERF_COUNT_HW_INSTRUCTIONS		= 1,
	PERF_COUNT_HW_CACHE_REFERENCES		= 2,
	PERF_COUNT_HW_CACHE_MISSES		= 3,
	PERF_COUNT_HW_BRANCH_INSTRUCTIONS	= 4,
	PERF_COUNT_HW_BRANCH_MISSES		= 5,
	PERF_COUNT_HW_BUS_CYCLES		= 6,
	PERF_COUNT_HW_STALLED_CYCLES_FRONTEND	= 7,
	PERF_COUNT_HW_STALLED_CYCLES_BACKEND	= 8,
	PERF_COUNT_HW_REF_CPU_CYCLES		= 9,

	PERF_COUNT_HW_MAX,
};

enum perf_hw_cache_id {
	PERF_COUNT_HW_CACHE_L1D			= 0,
	PERF_COUNT_HW_CACHE_L1I			= 1,
	PERF_COUNT_HW_CACHE_LL			= 2,
	PERF_COUNT_HW_CACHE_DTLB		= 3,
	PERF_COUNT_HW_CACHE_ITLB		= 4,
	PERF_COUNT_HW_CACHE_BPU			= 5,
	PERF_COUNT_HW_CACHE_NODE		= 6,

	PERF_COUNT_HW_CACHE_MAX,
};

enum perf_hw_cache_op_id {
	PERF_COUNT_HW_CACHE_OP_READ		= 0,
	PERF_COUNT_HW_CACHE_OP_WRITE		= 1,
	PERF_COUNT_HW_CACHE_OP_PREFETCH		= 2,

	PERF_COUNT_HW_CACHE_OP_MAX,
};

enum perf_hw_cache_op_result_id {
	PERF_COUNT_HW_CACHE_RESULT_ACCESS	= 0,
	PERF_COUNT_HW_CACHE_RESULT_MISS		= 1,

	PERF_COUNT_HW_CACHE_RESULT_MAX,
};

enum perf_sw_ids {
	PERF_COUNT_SW_CPU_CLOCK			= 0,
	PERF_COUNT_SW_TASK_CLOCK		= 1,
	PERF_COUNT_SW_PAGE_FAULTS		= 2,
	PERF_COUNT_SW_CONTEXT_SWITCHES		= 3,
	PERF_COUNT_SW_CPU_MIGRATIONS		= 4,
	PERF_COUNT_SW_PAGE_FAULTS_MIN		= 5,
	PERF_COUNT_SW_PAGE_FAULTS_MAJ		= 6,
	PERF_COUNT_SW_ALIGNMENT_FAULTS		= 7,
	PERF_COUNT_SW_EMULATION_FAULTS		= 8,
	PERF_COUNT_SW_DUMMY			= 9,
	PERF_COUNT_SW_BPF_OUTPUT		= 10,
	PERF_COUNT_SW_CGROUP_SWITCHES		= 11,

	PERF_COUNT_SW_MAX,			/* non-ABI */
};

enum perf_event_sample_format {
	PERF_SAMPLE_IP				= 1U << 0,
	PERF_SAMPLE_TID				= 1U << 1,
	PERF_SAMPLE_TIME			= 1U << 2,
	PERF_SAMPLE_ADDR			= 1U << 3,
	PERF_SAMPLE_READ			= 1U << 4,
	PERF_SAMPLE_CALLCHAIN			= 1U << 5,
	PERF_SAMPLE_ID				= 1U << 6,
	PERF_SAMPLE_CPU				= 1U << 7,
	PERF_SAMPLE_PERIOD			= 1U << 8,
	PERF_SAMPLE_STREAM_ID			= 1U << 9,
	PERF_SAMPLE_RAW				= 1U << 10,
	PERF_SAMPLE_BRANCH_STACK		= 1U << 11,
	PERF_SAMPLE_REGS_USER			= 1U << 12,
	PERF_SAMPLE_STACK_USER			= 1U << 13,
	PERF_SAMPLE_WEIGHT			= 1U << 14,
	PERF_SAMPLE_DATA_SRC			= 1U << 15,
	PERF_SAMPLE_IDENTIFIER			= 1U << 16,
	PERF_SAMPLE_TRANSACTION			= 1U << 17,
	PERF_SAMPLE_REGS_INTR			= 1U << 18,
	PERF_SAMPLE_PHYS_ADDR			= 1U << 19,
	PERF_SAMPLE_MAX = 1U << 20,
};

enum perf_event_type {
	PERF_RECORD_MMAP			= 1,
	PERF_RECORD_LOST			= 2,
	PERF_RECORD_COMM			= 3,
	PERF_RECORD_EXIT			= 4,
	PERF_RECORD_THROTTLE			= 5,
	PERF_RECORD_UNTHROTTLE			= 6,
	PERF_RECORD_FORK			= 7,
	PERF_RECORD_READ			= 8,
	PERF_RECORD_SAMPLE			= 9,
	PERF_RECORD_MMAP2			= 10,
	PERF_RECORD_AUX				= 11,
	PERF_RECORD_ITRACE_START		= 12,
	PERF_RECORD_LOST_SAMPLES		= 13,
	PERF_RECORD_SWITCH			= 14,
	PERF_RECORD_SWITCH_CPU_WIDE		= 15,
	PERF_RECORD_NAMESPACES			= 16,
	PERF_RECORD_MAX,
};

#define PERF_EVENT_IOC_ENABLE		_IO ('$', 0)
#define PERF_EVENT_IOC_DISABLE		_IO ('$', 1)
#define PERF_EVENT_IOC_REFRESH		_IO ('$', 2)
#define PERF_EVENT_IOC_RESET		_IO ('$', 3)
#define PERF_EVENT_IOC_PERIOD		_IOW('$', 4, __u64)
#define PERF_EVENT_IOC_SET_OUTPUT	_IO ('$', 5)
#define PERF_EVENT_IOC_SET_FILTER	_IOW('$', 6, char *)
#define PERF_EVENT_IOC_ID		_IOR('$', 7, __u64 *)
#define PERF_EVENT_IOC_SET_BPF		_IOW('$', 8, __u32)
#define PERF_EVENT_IOC_PAUSE_OUTPUT	_IOW('$', 9, __u32)

/* HongMeng Only */
#define PERF_EVENT_IOC_HM_NODE_ID	_IOR('$', 200, __u32 *)
#define PERF_EVENT_IOC_HM_EVENT_ID	_IOR('$', 201, __u64 *)

enum perf_event_ioc_flags {
	PERF_IOC_FLAG_GROUP		= 1U << 0,
};

#define PERF_FLAG_FD_NO_GROUP		(1UL << 0)
#define PERF_FLAG_FD_OUTPUT		(1UL << 1)
#define PERF_FLAG_FD_CLOEXEC            (1UL << 3) /* O_CLOEXEC */

/*
 * The format of the data returned by read() on a perf event fd,
 * as specified by attr.read_format:
 *
 * struct read_format {
 *	{ u64		value;
 *	  { u64		time_enabled; } && PERF_FORMAT_TOTAL_TIME_ENABLED
 *	  { u64		time_running; } && PERF_FORMAT_TOTAL_TIME_RUNNING
 *	  { u64		id;           } && PERF_FORMAT_ID
 *	} && !PERF_FORMAT_GROUP
 *
 *	{ u64		nr;
 *	  { u64		time_enabled; } && PERF_FORMAT_TOTAL_TIME_ENABLED
 *	  { u64		time_running; } && PERF_FORMAT_TOTAL_TIME_RUNNING
 *	  { u64		value;
 *	    { u64	id;           } && PERF_FORMAT_ID
 *	  }		cntr[nr];
 *	} && PERF_FORMAT_GROUP
 * };
 */
enum perf_event_read_format {
	PERF_FORMAT_TOTAL_TIME_ENABLED		= 1U << 0,
	PERF_FORMAT_TOTAL_TIME_RUNNING		= 1U << 1,
	PERF_FORMAT_ID				= 1U << 2,
	PERF_FORMAT_GROUP			= 1U << 3,

	PERF_FORMAT_MAX = 1U << 4,		/* non-ABI */
};

struct perf_event_attr {
	__u32			type;
	__u32			size;
	__u64			config;

	union {
		__u64		sample_period;
		__u64		sample_freq;
	};

	__u64			sample_type;
	__u64			read_format;

	__u64			disabled       :  1,
				inherit	       :  1,
				pinned	       :  1,
				exclusive      :  1,
				exclude_user   :  1,
				exclude_kernel :  1,
				exclude_hv     :  1,
				exclude_idle   :  1,
				mmap           :  1,
				comm	       :  1,
				freq           :  1,
				inherit_stat   :  1,
				enable_on_exec :  1,
				task           :  1,
				watermark      :  1,
				precise_ip     :  2,
				mmap_data      :  1,
				sample_id_all  :  1,

				exclude_host   :  1,
				exclude_guest  :  1,

				exclude_callchain_kernel : 1,
				exclude_callchain_user   : 1,
				mmap2          :  1,
				comm_exec      :  1,
				use_clockid    :  1,
				context_switch :  1,
				write_backward :  1,
				namespaces     :  1,
				__reserved_1   : 35;

	union {
		__u32		wakeup_events;
		__u32		wakeup_watermark;
	};

	__u32			bp_type;
	union {
		__u64		bp_addr;
		__u64		config1;
	};
	union {
		__u64		bp_len;
		__u64		config2;
	};
	__u64	branch_sample_type;
	__u64	sample_regs_user;
	__u32	sample_stack_user;
	__s32	clockid;
	__u64	sample_regs_intr;
	__u32	aux_watermark;
	__u16	sample_max_stack;
	__u16	__reserved_2;
};

struct perf_event_mmap_page {
	__u32	version;
	__u32	compat_version;
	__u32	lock;
	__u32	index;
	__s64	offset;
	__u64	time_enabled;
	__u64	time_running;
	union {
		__u64	capabilities;
		struct {
			__u64	cap_bit0		: 1,
				cap_bit0_is_deprecated	: 1,

				cap_user_rdpmc		: 1,
				cap_user_time		: 1,
				cap_user_time_zero	: 1,
				cap_____res		: 59;
		};
	};
	__u16	pmc_width;
	__u16	time_shift;
	__u32	time_mult;
	__u64	time_offset;
	__u64	time_zero;
	__u32	size;
	__u8	__reserved[MAX_REV_NUM];
	__u64   data_head;
	__u64	data_tail;
	__u64	data_offset;
	__u64	data_size;
	__u64	aux_head;
	__u64	aux_tail;
	__u64	aux_offset;
	__u64	aux_size;
};

#endif
