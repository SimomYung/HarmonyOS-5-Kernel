/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Hm proc_affinity header file
 * Author: Huawei OS Kernel Lab
 * Create: Mon Nov 29 16:40:25 2021
 */
#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HM_PROC_AFFINITY_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_HM_PROC_AFFINITY_H

#include <sys/types.h>
#include <hmkernel/cpuset.h>

struct proc_affinity_info_s {
	__u32 privileged;
	__cpuset_t boot_affinity;
	__cpuset_t default_affinity;
};

void hm_proc_set_default_affinity(pid_t pid);
int hm_proc_affinity_get(pid_t pid, struct proc_affinity_info_s *proc_affinity_info);

#endif
