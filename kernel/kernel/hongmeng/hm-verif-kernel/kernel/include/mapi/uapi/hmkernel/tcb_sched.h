/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Definition for tcb uapi
 * Author: Huawei OS Kernel Lab
 * Create: Thu Aug 19 13:36:28 2024
 */
#ifndef UAPI_TCB_SCHED_H
#define UAPI_TCB_SCHED_H

#include <hmkernel/types.h>
#include <lib/dlist.h>

struct __tcb_ext_sched_s {
	/*
	 * for cpu isolation
	 *
	 * no need to handle tcb_exit cause tcb_exit only called by self
	 * running task will never in rechoose list
	 */
	struct dlist_node rt_rechoose_link;
};

#endif /* UAPI_TCB_SCHED_H */
