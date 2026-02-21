/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Page pool
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 22 04:15:18 2024
 */
#ifndef UAPI_HMKERNEL_PAGE_POOL_H
#define UAPI_HMKERNEL_PAGE_POOL_H

#include <hmkernel/types.h>

#ifdef __HOST_LLT__
#define NR_RECLAIM_PFN 10
#else
#define NR_RECLAIM_PFN (PAGE_SIZE / sizeof(__u64) - 1)
#endif

struct reclaim_pfn {
	int cnt;
	__u64 pfn[NR_RECLAIM_PFN];
};

#endif
