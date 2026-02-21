/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 * Description: Discrete page allocator
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 23 12:16:00 2022
 */

#ifndef __DEVHOST_LIBLINUX_DISCRETEPOOL_H__
#define __DEVHOST_LIBLINUX_DISCRETEPOOL_H__

#include <devhost/buddy.h>
#include <liblinux/pal.h>
#include <lnxbase/lnxbase.h>

#include "mem_shrinker.h"
#include "shrinker.h"
#include "page_pool.h"

#define DISCRETEPOOL_DEF_ARRAY_LEN	16U

struct discretepool_allocator {
	struct raw_mutex mutex;
	struct shrinkable_allocator impl;
	struct lnxbase_mem_shrinker shrinker;
	unsigned int min_alloc_shift;
	struct page_pool page_pool;
	struct libdh_buddy_s buddy;
	struct dlist_node dnode;
};

/*
 * Create discrete shared memory pool.
 *
 * @param pool pointer to the creating pool_info struct.
 *
 * @return page allocator if succeed otherwise NULL.
 */
struct page_pool *discretepool_allocator_create(struct pool_info *info,
						unsigned int page_sizeorder);

#endif /* __DEVHOST_LIBLINUX_DISCRETEPOOL_H__ */
