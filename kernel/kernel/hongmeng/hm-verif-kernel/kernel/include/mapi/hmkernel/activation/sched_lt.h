/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Sched load tracking info for modules
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 27 18:07:31 2023
 */
#ifndef MAPI_HMKERNEL_ACTIVATION_SCHED_LT_H
#define MAPI_HMKERNEL_ACTIVATION_SCHED_LT_H

#include <hmkernel/activation/sched.h>

#ifdef CONFIG_ACTV_SCHED_LT
JAM_INLINE void
actv_sched_sync_lt_group(struct actv_s *actv, unsigned int group);
#else
static inline void
actv_sched_sync_lt_group(struct actv_s *actv, unsigned int group)
{
	UNUSED(actv, group);
}
#endif

#endif
