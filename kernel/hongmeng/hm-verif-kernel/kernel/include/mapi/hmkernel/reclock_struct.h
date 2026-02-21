/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Definition for rec_spinlock
 * Author: Huawei OS Kernel Lab
 * Create: Fri Oct 09 15:56:12 2020
 */

#ifndef MAPI_HMKERNEL_RECLOCK_STRUCT_H
#define MAPI_HMKERNEL_RECLOCK_STRUCT_H

#ifdef CONFIG_SMP
#include <lib/atomic.h>

struct rec_spinlock_s {
	vatomic32_t owner;
	unsigned int cnt;
#ifdef CONFIG_KLOCK_TRACE
	unsigned int lock_type;
#endif
};
#else
/* On UP system just use empty stubs */
struct rec_spinlock_s {};
#endif /* !CONFIG_SMP */

#endif
