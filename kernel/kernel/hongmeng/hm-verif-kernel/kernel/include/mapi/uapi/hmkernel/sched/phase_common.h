#ifndef UAPI_HMKERNEL_SCHED_PHASE_COMMON_H
#define UAPI_HMKERNEL_SCHED_PHASE_COMMON_H

#include <hmasm/types.h>

#define __PHASE_TRACKING_MODULE_NAME "phase_class"

#define __PHASE_SET_LT_ENABLE 0UL
#define __PHASE_SET_LOG_ENABLE 1UL
#define __PHASE_SET_CPUMASK 2UL
#define __PHASE_SET_CNODE_IDX 3UL
#define __PHASE_AMU_EVENT_SET 4UL

#define PHASE_EVENT_NUM 10

enum phase_event_list {
	NR_INST_RETIRED = 0,
	NR_L2_CACHE_REFILL,
	NR_L3D_CACHE_REFILL,
	NR_L3D_CACHE_REFILL_C0,
	NR_BUS_ACCESS,
#ifdef CONFIG_HISI_CPU_INTELLISENSE
	NR_MEMSTALL_L3MISS,
#endif
	NR_CPU_CYCLES,
	NR_STALL_FRONTEND,
	NR_STALL_BACKEND,
	NR_PHASE_PERF_USED = PHASE_EVENT_NUM,
};

#define PHASE_EVENT_OVERFLOW	(0xFFFFFFFF)
#define L1I_CACHE_REFILL		0x0001
#define L1D_CACHE			0x0004
#define INST_RETIRED			0x0008
#define BR_MIS_PRED			0x0010
#define CYCLES				0x0011
#define L2D_CACHE			0x0016
#define L2D_CACHE_REFILL		0x0017
#define BUS_ACCESS			0x0019
#define INST_SPEC			0x001b
#define STALL_FRONTEND			0x0023
#define STALL_BACKEND			0x0024
#define L3D_CACHE_REFILL		0x002a
#define OP_SPEC				0x003b
#define L3D_CACHE_REFILL_C0 		0x00a2
#define IF_RSURC_CHK_OK			0x104b
#define DSP_STALL			0x200e
#define ROB_FLUSH			0x2010
#define FETCH_BUBBLE			0x2011
#define FSU_ISQ_STALL			0x2015
#define RETIRED_LOAD			0x2037
#define STQ_STALL			0x50b6
#define MEMSTALL_L3MISS			0x7009
#define PHASE_EVENT_FINAL_TERMINATOR		(UINT_MAX - 1)

#define PHASE_PEVENT_NUM		10
#define PHASE_TASK_TICK		0x08

struct phase_event_pcount {
	unsigned long data[PHASE_PEVENT_NUM];
};

/* phase perf amu ioctl args */
struct __phase_amu_event_args_s {
	__u32 cpuid;
	__u32 enable; // 1: enable, 0: disable;
	__u32 event_cnt;
	__u32 event_ids[PHASE_PEVENT_NUM];
} __attribute__((aligned(8)));

#endif
