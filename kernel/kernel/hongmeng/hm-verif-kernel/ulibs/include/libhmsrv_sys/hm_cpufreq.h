/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Native interfaces of cpufreq
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jan 24 23:11:26 2023
 */

#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HM_CPUFREQ_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_HM_CPUFREQ_H

#include <hongmeng/ext_fast_syscall.h>
#include <hmkernel/power/cpufreq.h>

__EXT_FASTCALL_GLOBAL(4, cpufreq_update, __EXT_FASTCALL_GLOBAL_CPUFREQ_UPDATE,
		      unsigned int, type,
		      unsigned int, cpu_id,
		      unsigned int, index, unsigned int, ratio)

#endif
