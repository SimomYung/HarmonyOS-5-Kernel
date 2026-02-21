/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Function implementation of pmuv1
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jan 21 09:45:37 2019
 */
#include <errno.h>
#include <libperf/arm_pmu_common.h>

/* Cortex-A9 HW events mapping */
static const unsigned pmuv1_hw_event_num_map[(int)HM_HW_PMU_MAX] = {
	[HM_HW_CPU_CYCLES]		= PMUV1_PERFCTR_CLOCK_CYCLES,
	/*
	 * this event is not supported in arm cortex-a9,
	 * so use alternative events to approximate measurements the instructions
	 */
	[HM_HW_INSTRUCTIONS]		= PMUV1_A9_PERFCTR_INSTR_CORE_RENAME,
	[HM_HW_CACHE_REFERENCES]	= PMUV1_PERFCTR_L1_DCACHE_ACCESS,
	[HM_HW_CACHE_MISSES]		= PMUV1_PERFCTR_L1_DCACHE_REFILL,
	[HM_HW_BRANCH_INSTRUCTIONS]	= PMUV1_PERFCTR_PC_IMM_BRANCH,
	[HM_HW_BRANCH_MISSES]		= PMUV1_PERFCTR_PC_BRANCH_MIS_PRED,
	[HM_HW_BUS_CYCLES]		= PMUV1_PERFCTR_BUS_CYCLES,
	[HM_HW_STALLED_CYCLES_FRONTEND]	= HW_OP_UNSUPPORTED,
	[HM_HW_STALLED_CYCLES_BACKEND]	= HW_OP_UNSUPPORTED,
	[HM_HW_REF_CPU_CYCLES]		= HW_OP_UNSUPPORTED,
};

/* arm_pmu_common.c will call function(s) below */
int check_implemented_event(unsigned int idx, unsigned int *eventid)
{
	int ret = -EINVAL;

	/* Note: should check PMCEIDx_EL0 if event implemented or not */
	if ((idx < HM_HW_PMU_MAX) && (pmuv1_hw_event_num_map[idx] != HW_OP_UNSUPPORTED)) {
		*eventid = pmuv1_hw_event_num_map[idx];
		ret = 0;
	}

	return ret;
}
