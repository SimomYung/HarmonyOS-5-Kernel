/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Function implementation of pmuv3
 * Author: Huawei OS Kernel Lab
 * Create: Tue Dec 25 17:29:29 2018
 */
#include <errno.h>
#include <libperf/arm_pmu_common.h>

/* PMUv3 HW events mapping. */
static const unsigned pmuv3_hw_event_num_map[(int)HM_HW_PMU_MAX] = {
	[HM_HW_CPU_CYCLES]		= PMUV3_PERFCTR_CLOCK_CYCLES,
	[HM_HW_INSTRUCTIONS]		= PMUV3_PERFCTR_INSTR_EXECUTED,
	[HM_HW_CACHE_REFERENCES]	= PMUV3_PERFCTR_L1_DCACHE_ACCESS,
	[HM_HW_CACHE_MISSES]		= PMUV3_PERFCTR_L1_DCACHE_REFILL,
	[HM_HW_BRANCH_INSTRUCTIONS]	= PMUV3_PERFCTR_PC_IMM_BRANCH,
	[HM_HW_BRANCH_MISSES]		= PMUV3_PERFCTR_PC_BRANCH_MIS_PRED,
	[HM_HW_BUS_CYCLES]		= PMUV3_PERFCTR_BUS_CYCLES,
	[HM_HW_STALLED_CYCLES_FRONTEND]	= HW_OP_UNSUPPORTED,
	[HM_HW_STALLED_CYCLES_BACKEND]	= HW_OP_UNSUPPORTED,
	[HM_HW_REF_CPU_CYCLES]		= HW_OP_UNSUPPORTED,
};

/* arm_pmu_common.c will call function(s) below */
int check_implemented_event(unsigned int idx, unsigned int *eventid)
{
	int ret = -EINVAL;

	if ((idx < HM_HW_PMU_MAX) && (pmuv3_hw_event_num_map[idx] != HW_OP_UNSUPPORTED)) {
		*eventid = pmuv3_hw_event_num_map[idx];
		ret = 0;
	}

	return ret;
}
