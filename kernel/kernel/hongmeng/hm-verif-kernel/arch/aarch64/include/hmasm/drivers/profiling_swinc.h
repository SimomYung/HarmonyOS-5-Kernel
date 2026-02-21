/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 25 15:18:42 2019
 */

#ifndef AARCH64_ASM_PROFILING_SWINC
#define AARCH64_ASM_PROFILING_SWINC

#include <hmkernel/kernel.h>
#include <hmasm/drivers/pmu.h>

#ifdef CONFIG_PROFILING_SWINC

static inline void
profiling_swinc_evt_inc(unsigned int event)
{
	arm_pmu_pmswinc_write((u32)(1u << event));
}

#else

static inline void
profiling_swinc_evt_inc(unsigned int event)
{
	UNUSED(event);
	nop();
}

#endif

#endif
