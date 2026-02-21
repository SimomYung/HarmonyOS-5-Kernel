/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: Export header file of cpufreq hook.
 * Create: Fri Feb 21 17:12:12 2025
 */

#ifndef CPUFREQ_HOOK_H
#define CPUFREQ_HOOK_H

#include <mapi/hmkernel/lite_hook.h>

LITE_HOOK_DECLARE_VOID(cpufreq_hook_upd_limit_frequency,
	LH_PROTO(unsigned int type, unsigned int cpu_id, unsigned int freq_idx, unsigned int qos),
	LH_ARGS(type, cpu_id, freq_idx, qos));
#endif
