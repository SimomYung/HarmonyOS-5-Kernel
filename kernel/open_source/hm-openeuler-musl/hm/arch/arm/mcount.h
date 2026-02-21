/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Thursday Aug 29, 2019 09:09:30 CST
 */

#ifndef HM_ARCH_ARM_MCOUNT_H
#define HM_ARCH_ARM_MCOUNT_H

#define MCOUNT_INTERNAL(frompc, selfpc)	\
void mcount_internal(unsigned long (frompc), unsigned long (selfpc))

/*
 * ARM profiling entry point is '__gnu_mcount_nc' since gcc 4.4.
 * Define MCOUNT_ENTRY as empty since we have the implementation in arm/mcount.S file.
 */
#define MCOUNT_ENTRY

#endif
