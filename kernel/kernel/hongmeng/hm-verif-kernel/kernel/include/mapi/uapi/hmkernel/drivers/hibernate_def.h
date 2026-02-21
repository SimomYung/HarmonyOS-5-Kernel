/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Description: hibernation for uapi
 * Create: Fri May 10 20:10:36 2024
 */

#ifndef UAPI_HMKERNEL_DRIVERS_HIBERNATE_DEF_H
#define UAPI_HMKERNEL_DRIVERS_HIBERNATE_DEF_H

#ifdef __KERNEL__
#include <hmkernel/types.h>
#else
#include <hongmeng/types.h>
#endif
#include <hmasm/memory.h>

struct __hibernate_memory_bitmap {
	__paddr_t pfn; /* bit63: last extent flag */
	__u64 len;
	unsigned long bitmap[];
};

struct __hibernate_page_phy_info {
	__u64 src_pfn;
	__u64 dst_pfn;
	__paddr_t next;
};
#endif
