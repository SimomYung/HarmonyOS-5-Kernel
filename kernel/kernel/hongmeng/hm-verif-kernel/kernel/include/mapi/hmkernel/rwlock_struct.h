/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Definition for rwlock
 * Author: Huawei OS Kernel Lab
 * Create: Aug 13 15:43 2021
 */

#ifndef MAPI_HMKERNEL_RWLOCK_STRUCT_H
#define MAPI_HMKERNEL_RWLOCK_STRUCT_H

#ifdef CONFIG_SMP
#include <lib/atomic.h>
#include <vsync/rwlock.h>

struct krwlock_s {
	struct rwlock_s rwlock;
#ifdef CONFIG_KLOCK_TRACE
	unsigned int lock_type;
#endif
};
#else
/* On UP system just use empty stubs */
struct krwlock_s {};
#endif /* !CONFIG_SMP */

#endif
