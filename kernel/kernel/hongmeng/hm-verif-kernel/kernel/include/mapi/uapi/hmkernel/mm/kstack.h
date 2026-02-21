/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: kstack in uapi
 * Author: Huawei OS Kernel Lab
 * Create: Thu Dec 21 09:36:27 2023
 */

#ifndef MAPI_UAPI_HMKERNEL_MM_KSTACK_H
#define MAPI_UAPI_HMKERNEL_MM_KSTACK_H

#include <hmasm/page.h>
#include <hmasm/types.h>
#include <hmkernel/const.h>

/*
 * Static assert in kstack.c
 */
#define __KSTACK_REGION_SIZE_CHANNEL_PAGES_LOCKABLE 		(__PAGE_SIZE * 5U)
#define __KSTACK_REGION_ALIGN_SHIFT_CHANNEL_PAGES_LOCKABLE	15
#define __KSTACK_REGION_ALIGN_CHANNEL_PAGES_LOCKABLE	\
	(1UL << __U(__KSTACK_REGION_ALIGN_SHIFT_CHANNEL_PAGES_LOCKABLE))
#define __KSTACK_REGION_ALIGN_PAGES_CHANNEL_PAGES_LOCKABLE	\
	(1UL << (__U(__KSTACK_REGION_ALIGN_SHIFT_CHANNEL_PAGES_LOCKABLE) - __U(__PAGE_SHIFT)))

#ifndef __ASSEMBLY__

struct __kstack_channel_pages_lockable_head_s {
	__u32 cpuid;
	__u32 locked;
	__u64 selfpa;
};

#endif

#endif
