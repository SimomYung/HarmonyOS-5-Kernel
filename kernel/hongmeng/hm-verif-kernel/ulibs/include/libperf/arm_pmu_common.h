/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Common layer of arm pmu in libperf
 * Author: Huawei OS Kernel Lab
 * Create: Tue Dec 25 17:05:22 2018
 */
#ifndef ULIBS_LIBPERF_ARM_PMU_COMMON_H
#define ULIBS_LIBPERF_ARM_PMU_COMMON_H

#include "perf.h"

#ifdef CONFIG_AARCH64_NAMTSO_PMU
#define ULIBS_LIBPERF_PLATFORM_NAMTSO_DEF
#include <libperf/perf_platform_namtso_event.h>
#undef ULIBS_LIBPERF_PLATFORM_NAMTSO_DEF
#endif

#define HW_MAX_COUNTER_NUM		7U

/* Perf Event counter' index */
#define	ARM_PMU_IDX_COUNTER_START	0U

/*
 * PMU HW is implemented with one default Cycle Counter,
 * and sereval Event Counters PMEVCNTR<n>, n=[0-5]
 */
#define	ARM_PMU_IDX_EVENT_COUNTER0	1U

#define ARM_PMU_MAX_COUNTERS	32U
#define ARM_PMU_COUNTER_MASK	(ARM_PMU_MAX_COUNTERS - 1U)

/*
 * Perf Event to low level counters mapping
 * Cycle Counter idx is C=bit[31], 0x1F=ARMV8_COUNTER_MASK
 */
#define	ARM_PMU_IDX_TO_COUNTER(x)	\
	(((x) - ARM_PMU_IDX_EVENT_COUNTER0) & ARM_PMU_COUNTER_MASK)

enum armv7_pmuv1_perf_types {
	/* Common ARMV7 event types */
	PMUV1_PERFCTR_PMNC_SW_INCR		= 0x00,
	PMUV1_PERFCTR_L1_ICACHE_REFILL		= 0x01,
	PMUV1_PERFCTR_ITLB_REFILL		= 0x02,
	PMUV1_PERFCTR_L1_DCACHE_REFILL		= 0x03,
	PMUV1_PERFCTR_L1_DCACHE_ACCESS		= 0x04,
	PMUV1_PERFCTR_DTLB_REFILL		= 0x05,
	PMUV1_PERFCTR_MEM_READ			= 0x06,
	PMUV1_PERFCTR_MEM_WRITE			= 0x07,
	/*
	 * this event is not implemented in arm cortex-a9.
	 * However,similar functionality is provided by event number 0x68,Instructions
	 * comming out of the core renaming stage.
	 * For more information,please see the arm cortex-a9 trm documentation.
	 * */
	PMUV1_PERFCTR_INSTR_EXECUTED		= 0x08,
	PMUV1_PERFCTR_EXC_TAKEN			= 0x09,
	PMUV1_PERFCTR_EXC_EXECUTED		= 0x0A,
	PMUV1_PERFCTR_CID_WRITE			= 0x0B,
	/*
	 * ARMV7_PERFCTR_PC_WRITE is equivalent to HW_BRANCH_INSTRUCTIONS.
	 * It counts:
	 *  - all (taken) branch instructions,
	 *  - instructions that explicitly write the PC,
	 *  - exception generating instructions.
	 */
	PMUV1_PERFCTR_PC_WRITE			= 0x0C,
	PMUV1_PERFCTR_PC_IMM_BRANCH		= 0x0D,
	PMUV1_PERFCTR_PC_PROC_RETURN		= 0x0E,
	PMUV1_PERFCTR_MEM_UNALIGNED_ACCESS	= 0x0F,
	PMUV1_PERFCTR_PC_BRANCH_MIS_PRED	= 0x10,
	PMUV1_PERFCTR_CLOCK_CYCLES		= 0x11,
	PMUV1_PERFCTR_PC_BRANCH_PRED		= 0x12,

	/* PMUV1 supplement events. */
	PMUV1_PERFCTR_MEM_ACCESS		= 0x13,
	PMUV1_PERFCTR_L1_ICACHE_ACCESS		= 0x14,
	PMUV1_PERFCTR_L1_DCACHE_WB		= 0x15,
	PMUV1_PERFCTR_L2_CACHE_ACCESS		= 0x16,
	PMUV1_PERFCTR_L2_CACHE_REFILL		= 0x17,
	PMUV1_PERFCTR_L2_CACHE_WB		= 0x18,
	PMUV1_PERFCTR_BUS_ACCESS		= 0x19,
	PMUV1_PERFCTR_MEM_ERROR			= 0x1A,
	PMUV1_PERFCTR_INSTR_SPEC		= 0x1B,
	PMUV1_PERFCTR_TTBR_WRITE		= 0x1C,
	PMUV1_PERFCTR_BUS_CYCLES		= 0x1D,
	PMUV1_PERFCTR_CPU_CYCLES		= 0xFF,

	/* ARMv7 Cortex-A9 specific event types */
	PMUV1_A9_PERFCTR_INSTR_CORE_RENAME	= 0x68,
	PMUV1_A9_PERFCTR_STALL_ICACHE		= 0x60,
	PMUV1_A9_PERFCTR_STALL_DISPATCH		= 0x66,
};

/*
 * ARMv8 PMUv3 Event Type Definition:
 * refer to "ARMv8-A architecture profile: D5.10.2 The PMU event number space and common events".
 * but under the limit of PMCEID0_EL0 and PMCEID1_EL0, maxinum 64 event types realized!
 * refer to "Cortex-A53 Manual Rev r0p4: 12.4 AArch64 PMU register descriptions", actually only 33
 * event types are defined while not fully implemented!
 */
#define PMUV3_PERFCTR_L1_DCACHE_REFILL		0x0003
#define PMUV3_PERFCTR_L1_DCACHE_ACCESS		0x0004
#define PMUV3_PERFCTR_PC_BRANCH_MIS_PRED	0x0010
#define PMUV3_PERFCTR_CLOCK_CYCLES		0x0011
#define PMUV3_PERFCTR_INSTR_EXECUTED		0x0008
#define PMUV3_PERFCTR_PC_IMM_BRANCH		0x000D
#define PMUV3_PERFCTR_BUS_CYCLES		0x001D

#define DSU_PMU_CYCLES				0x0011
#define DSU_PMU_BUS_ACCESS			0x0019
#define DSU_PMU_MEMORY_ERROR			0x001a
#define DSU_PMU_BUS_CYCLES			0x001d
#define DSU_PMU_L3D_CACHE_ALLOCATE		0x0029
#define DSU_PMU_L3D_CACHE_REFILL		0x002a
#define DSU_PMU_L3D_CACHE			0x002b
#define DSU_PMU_L3D_CACHE_WB			0x002c

/* common pmu init functions. */
int hw_pmu_init(struct generic_pmu *pmu, const struct perf_evset_attr *attr);
int sw_pmu_init(const struct generic_pmu *pmu, const struct perf_evset_attr *attr);

/* function(s) in pmuv(1,2,3...).c */
unsigned int pmu_event_counters_num(void);
int check_implemented_event(unsigned int idx, unsigned int *eventid);
int check_implemented_dsu_event(unsigned int idx, unsigned int *eventid);

#endif
