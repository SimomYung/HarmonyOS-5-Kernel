/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Native interfaces of idle state related operation
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 19 19:30:05 2021
 */

#include <hmkernel/errno.h>
#include <libstrict/strict.h>
#include <libhmsrv_sys/hm_idle_state.h>

#ifdef CONFIG_DEEP_IDLE
int hm_idle_get_latency_info(struct idle_latency_info *latency,
			     unsigned int size)
{
	return sysfast_idle_get_latency_info(latency, size);
}
#else
int hm_idle_get_latency_info(struct idle_latency_info *latency,
			     unsigned int size)
{
	UNUSED(latency, size);
	return E_HM_NOSYS;
}
#endif
