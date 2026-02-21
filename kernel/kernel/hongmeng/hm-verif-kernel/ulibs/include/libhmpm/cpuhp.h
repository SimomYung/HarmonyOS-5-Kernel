/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Definition for hongmeng system cpuhp native api
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 10 18:23:54 2022
 */
#ifndef ULIBS_LIBHMPM_CPUHP_H
#define ULIBS_LIBHMPM_CPUHP_H

int hm_cpu_up(unsigned int cpu);
int hm_cpu_down(unsigned int cpu);

#endif
