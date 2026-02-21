/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Parameters for arm pmu cmd
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jan 16 11:10:56 2019
 */
#ifndef UAPI_HMKERNEL_DRIVERS_ARM_PMU_H
#define UAPI_HMKERNEL_DRIVERS_ARM_PMU_H

#include <hmkernel/types.h>
#include <hmkernel/capability.h>

/*
 * Command allow to pass from cpucontext.config to common arm PMU
 */
enum __arm_pmu_config_cmd {
	ARM_PMU_CONFIG_INVALID = 0,
	ARM_PMU_CONFIG_USER_ENABLE,
	ARM_PMU_CONFIG_USER_DISABLE,
	ARM_PMU_CONFIG_EVTYPE_SET,
	ARM_PMU_CONFIG_EVTCNT_GET,
};

struct __arm_pmu_counters {
	__u64 cycles;
	__u32 evts[31];
};

#define __ARM_PMU_CONFIGURABLE_EVENT_COUNTER_NUM 31UL
struct __arm_pmu_evts_config_s {
	__u32 type[__ARM_PMU_CONFIGURABLE_EVENT_COUNTER_NUM];
};

struct __arm_pmu_evts_ret_s {
	__u32 cnts[__ARM_PMU_CONFIGURABLE_EVENT_COUNTER_NUM];
};
#endif
