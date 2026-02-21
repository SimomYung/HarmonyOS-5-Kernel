/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: DSU Pmu
 * Author: Huawei OS Kernel Lab
 * Create: Sat Sep 16:23:43 2021
 */
#include <libperf/arm_pmu_common.h>
#include <hmkernel/errno.h>

#define dsu_index(dsu_event)	(((unsigned int)(dsu_event) - (unsigned int)(HM_HW_DSU_CYCLES)))

/* DSU HW events mapping. */
static const unsigned dsu_pmu_hw_event_num_map[dsu_index(HM_HW_DSU_MAX)] = {
	[dsu_index(HM_HW_DSU_CYCLES)]			= DSU_PMU_CYCLES,
	[dsu_index(HM_HW_DSU_BUS_ACCESS)]		= DSU_PMU_BUS_ACCESS,
	[dsu_index(HM_HW_DSU_MEMORY_ERROR)]		= DSU_PMU_MEMORY_ERROR,
	[dsu_index(HM_HW_DSU_BUS_CYCLES)]		= DSU_PMU_BUS_CYCLES,
	[dsu_index(HM_HW_DSU_L3D_CACHE_ALLOCATE)]	= DSU_PMU_L3D_CACHE_ALLOCATE,
	[dsu_index(HM_HW_DSU_L3D_CACHE_REFILLE)]	= DSU_PMU_L3D_CACHE_REFILL,
	[dsu_index(HM_HW_DSU_L3D_CACHE)]		= DSU_PMU_L3D_CACHE,
	[dsu_index(HM_HW_DSU_L3D_CACHE_WB)]		= DSU_PMU_L3D_CACHE_WB,
};

int check_implemented_dsu_event(unsigned int idx, unsigned int *eventid)
{
	int ret = E_HM_INVAL;

	if ((idx < HM_HW_DSU_MAX) && (idx >= HM_HW_DSU_CYCLES) &&
	    (dsu_pmu_hw_event_num_map[dsu_index(idx)] != HW_OP_UNSUPPORTED)) {
		*eventid = dsu_pmu_hw_event_num_map[dsu_index(idx)];
		ret = E_HM_OK;
	}
	return ret;
}
