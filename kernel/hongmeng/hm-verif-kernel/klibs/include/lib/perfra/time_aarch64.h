/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * Description: The generic time operation
 * Author: Huawei OS Kernel Lab
 * Create: Thy Dec 23 06:23:51 2021
 */
#ifndef KLIBS_PERFRA_TIME_AARCH64_H
#define KLIBS_PERFRA_TIME_AARCH64_H

#include <hmasm/types.h>
#include <hmasm/sysreg_utils.h>

#ifdef __aarch64__

static inline __u64 __time_cntpct_el0_read(void)
{
	return __aarch64_read_sysreg("CNTPCT_EL0");
}

static inline __u64 __time_cntfrq_el0_read(void)
{
	return __aarch64_read_sysreg("CNTFRQ_EL0");
}

static inline __u64 __time_timer_read(void)
{
	return __time_cntpct_el0_read();
}

static inline __u64 generic_timer_cntfrq(void)
{
	return __time_cntfrq_el0_read();
}
#endif

#endif
