/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: performance monitor unit
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 19 19:53:03 CST 2021
 */
#ifndef KLIBS_PERFRA_PMU_H
#define KLIBS_PERFRA_PMU_H

#ifdef __arm__
#include <lib/perfra/pmu_arm.h>
#endif

#ifdef __aarch64__
#include <lib/perfra/pmu_aarch64.h>
#endif

#endif
