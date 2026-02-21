/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: kernel spinlock struct
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 14 15:25:38 2023
 */
#ifndef MAPI_HMKERNEL_SPINLOCK_STRUCT_H
#define MAPI_HMKERNEL_SPINLOCK_STRUCT_H

#ifdef CONFIG_SMP
#include <lib/atomic.h>
#include <vsync/spinlock.h>

struct kspinlock_s {
	struct spinlock_s __lock;
#ifdef CONFIG_KLOCK_TRACE
	unsigned int lock_type;
#endif
};
#else
/* On UP system just use empty stubs */
struct kspinlock_s {};
#endif /* !CONFIG_SMP */

#endif
