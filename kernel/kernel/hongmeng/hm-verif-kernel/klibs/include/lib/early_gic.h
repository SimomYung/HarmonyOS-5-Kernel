/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 02 11:51:27 2018
 */
#ifndef KLIBS_EARLY_GIC_H
#define KLIBS_EARLY_GIC_H

#include <hmkernel/memory.h>

extern void el2_gic_percpu_init_v2(int cpuid);
extern void el2_gic_percpu_init_v3(int cpuid);

extern void el3_gic_percpu_init(void);
extern void el3_gic_pre_init(const paddr_t __gicr_base, const u32 __redist_stride,
			     const u32 __redist_regions);
#endif
