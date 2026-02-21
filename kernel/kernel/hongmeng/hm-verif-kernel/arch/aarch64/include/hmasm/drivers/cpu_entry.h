/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: Header of secondary startup entry functions
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 16 11:04:03 2020
 */
#ifndef HMKERNEL_AARCH64_CPU_ENTRY_H
#define HMKERNEL_AARCH64_CPU_ENTRY_H

#ifdef CONFIG_SMP

extern void arm_init_secondary_cpu(void);
extern void arm_cpu_restore_entry(void);

#else

static inline void arm_init_secondary_cpu(void)
{
}

static inline void arm_cpu_restore_entry(void)
{
}

#endif

#endif
