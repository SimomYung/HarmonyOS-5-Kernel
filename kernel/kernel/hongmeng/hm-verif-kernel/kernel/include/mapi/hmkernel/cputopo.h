/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Declaration of CPU topology parser in module
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 02 19:03:20 2020
 */
#ifndef MAPI_HMKERNEL_CPUTOPO_H
#define MAPI_HMKERNEL_CPUTOPO_H

extern const unsigned int *cputopo_lv_of_cpu(unsigned int logic_cpuid);

extern unsigned int cputopo_nr_cluster(void);

extern unsigned int cputopo_cluster_id_of(unsigned int cpu);

extern unsigned int *cpu_topology(void);
#endif
