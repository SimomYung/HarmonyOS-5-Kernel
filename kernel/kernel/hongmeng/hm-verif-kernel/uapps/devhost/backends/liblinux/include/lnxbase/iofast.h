/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: IOFAST APIs for page pool
 * Author: Huawei OS Kernel Lab
 * Create: Fri May 12 10:51:04 2023
 */
#ifndef __IOFAST_H__
#define __IOFAST_H__

#include <stdbool.h>

struct lnxbase_iofast_region {
	unsigned long vstart;
	unsigned long length;
};

bool lnxbase_use_iofast(void);
struct liblinux_pool_base;
int lnxbase_iofast_declare(const char *range_name, struct liblinux_pool_base *pool_base);
int lnxbase_iofast_alloc_pfn_v(unsigned int order, unsigned long long *pa_array,
			       unsigned long num, unsigned int flags, unsigned int *pa_num);
int lnxbase_iofast_free_pfn_v(unsigned long long *pa_array, unsigned int pa_num);
int lnxbase_iofast_acquire_regions(struct lnxbase_iofast_region **regions);

#endif /* __IOFAST_H__ */
