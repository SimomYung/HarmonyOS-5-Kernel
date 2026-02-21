/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Sysevent header file
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 9 20:44:58 2024
 */

#ifndef MAPI_HMKERNEL_SYSEVENT_KERNEL_H
#define MAPI_HMKERNEL_SYSEVENT_KERNEL_H

#include <hmkernel/timer/time.h>
#include <hmkernel/time.h>
#include <hmkernel/tcb.h>
#include <hmkernel/types.h>
#include <hmkernel/miscdata.h>

static inline int sysevent_get_realtime(__u64 *timestamp)
{
	struct time_real_t time_t = time_get_real();
	*timestamp = (__u64)((time_t.sec * __MSEC_PER_SEC) +
			     (time_t.nsec / __NSEC_PER_MSEC));
	return E_HM_OK;
}

#define SYSEVENT_CURRENT_PID() (*(ptr_cast(tcb_miscdata_of(tcb_current(), MISCDATA_TCB_PID), __u32)))
#define SYSEVENT_CURRENT_TID() (*(ptr_cast(tcb_miscdata_of(tcb_current(), MISCDATA_TCB_TID), __u32)))
#define SYSEVENT_CURRENT_UID() 0
#define SYSEVENT_CURRENT_REALTIME(timestamp_addr) sysevent_get_realtime(timestamp_addr)

#include <lib/sysevent/sysevent_api.h>

#endif
