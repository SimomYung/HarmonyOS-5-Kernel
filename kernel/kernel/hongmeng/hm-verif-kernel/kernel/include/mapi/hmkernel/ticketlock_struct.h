/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: mapi ticketlock struct
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jun 28 11:11:52 2023
 */
#ifndef MAPI_HMKERNEL_TICKETLOCK_STRUCT_H
#define MAPI_HMKERNEL_TICKETLOCK_STRUCT_H

#ifdef CONFIG_SMP
#include <vsync/ticketlock.h>

struct kticketlock_s {
	struct ticketlock_s __lock;
#ifdef CONFIG_KLOCK_TRACE
	unsigned int lock_type;
#endif
};
#else
/* On UP system just use empty stubs */
struct kticketlock_s {};
#endif /* !CONFIG_SMP */

#endif
