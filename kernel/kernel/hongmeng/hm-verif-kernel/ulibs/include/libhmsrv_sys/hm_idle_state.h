/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Native interfaces of idle state related operation
 * Author: Huawei OS Kernel Lab
 * Create: Tue Oct 19 19:30:05 2021
 */

#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HM_IDLE_STATE_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_HM_IDLE_STATE_H

#include <hongmeng/ext_fast_syscall.h>
#include <hmkernel/drivers/idle_state.h>

__EXT_FASTCALL_GLOBAL(2, idle_get_latency_info, EXT_FASTCALL_GLOBAL_IDLE_LATENCY_INFO,
		      struct idle_latency_info *, latency,
		      unsigned int, size)

int hm_idle_get_latency_info(struct idle_latency_info *latency, unsigned int size);

#endif
