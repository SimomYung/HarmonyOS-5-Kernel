/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: rec ticketlock struct for mapi
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 28 10:45:14 2024
 */
#ifndef MAPI_HMKERNEL_REC_TICKETLOCK_STRUCT_H
#define MAPI_HMKERNEL_REC_TICKETLOCK_STRUCT_H

#ifdef CONFIG_SMP

#ifdef __HOST_LLT__
#undef ASSERT
#endif

#ifndef ASSERT
#define ASSERT(V) BUG_ON(!(V))
#endif

#include <vsync/spinlock/rec_ticketlock.h>

struct krec_ticketlock_s {
	struct rec_ticketlock_s __lock;
#ifdef CONFIG_KLOCK_TRACE
	unsigned int lock_type;
#endif
};

#else

/* On UP system just use empty stubs */
struct krec_ticketlock_s {};

#endif /* CONFIG_SMP */

#endif
