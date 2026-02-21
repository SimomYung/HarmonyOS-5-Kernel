/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2022. All rights reserved.
 * Description: The System/Software Performance Impact Factor
 * Author: Huawei OS Kernel Lab
 * Create: Thy Dec 23 06:23:51 2021
 */
#ifndef KLIBS_PERFRA_SPIF_AARCH64_H
#define KLIBS_PERFRA_SPIF_AARCH64_H

#ifdef __aarch64__

#include <hmasm/types.h>
#include <lib/perfra/time_aarch64.h>

static inline __u64 __spif_current_timestamp(void)
{
	return __time_timer_read();
}

#endif

#endif
