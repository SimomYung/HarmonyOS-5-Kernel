/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Conversion between timespec and tocks
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 24 16:23:50 2021
 */

#include <libhmsrv_sys/hm_timer.h>

int tocks_to_timespec(uint64_t tocks, struct timespec *ts)
{
	return tocks_to_timespec_udata(tocks, ts);
}

int timespec_to_values(const struct timespec *ts, __u64 *value)
{
	return timespec_to_values_udata(ts, value);
}
