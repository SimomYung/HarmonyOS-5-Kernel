/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Declaration of aarch64 dependent CPU topology parser
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 02 14:13:02 2020
 */
#ifndef AARCH64_ASM_CPU_CPUTOPO_H
#define AARCH64_ASM_CPU_CPUTOPO_H

int arch_cpu_cputopo_init(unsigned int *lv, unsigned int logic_cpuid);

/* In aarch64, the cpu topology hierarchy levels are thread, core and cluster */
#define ARCH_CPUTOPO_CORE_INDEX		1
#define ARCH_CPUTOPO_CLUSTER_INDEX	2

#endif
