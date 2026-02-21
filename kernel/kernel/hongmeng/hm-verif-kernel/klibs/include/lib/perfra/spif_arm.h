/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * Description: The System/Software Performance Impact Factor
 * Author: Huawei OS Kernel Lab
 * Create: Thy Dec 23 06:23:51 2021
 */
#ifndef KLIBS_PERFRA_SPIF_ARM_H
#define KLIBS_PERFRA_SPIF_ARM_H

#include <hmasm/types.h>

#include <lib/perfra/time_arm.h>
#include <lib/perfra/pmu_arm.h>

#ifdef __arm__

static inline __u64 __spif_current_timestamp(void)
{
	/*
	 * LIMIT: The scheme is correct only in the single cpu.
	 *	  When more than one cpu, use generic timer(some arm
	 *	  core do not implement generic timer)
	 *
	 */
	return pmu_pmccntr_read();
}

#endif

#endif
