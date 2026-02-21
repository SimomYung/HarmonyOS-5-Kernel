/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Buddypool page allocator
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 21 11:40:15 2022
 */

#ifndef __DEVHOST_LIBLINUX_BUDDYPOOL_H__
#define __DEVHOST_LIBLINUX_BUDDYPOOL_H__

#include <liblinux/pal.h>
#include <lnxbase/lnxbase.h>
#include <devhost/buddy.h>
#include <devhost/cma.h>

#include "page_pool.h"
#include "mem_shrinker.h"

struct buddypool_allocator {
	struct raw_mutex mutex;
	struct liblinux_page_allocator impl;
	struct lnxbase_mem_shrinker shrinker;
	struct libdh_buddy_s buddy;
	struct devhost_cma *cma;
	struct page_pool pool;
};

/*
 * Pool of buddy used cma initialization.
 *
 * @param pool		[I] pointer of page pool
 *
 * @return page allocator if operation succeeds.
 */
struct page_pool *buddypool_allocator_create(struct pool_info *info,
					     unsigned int page_sizeorder);
#endif /* __DEVHOST_LIBLINUX_BUDDYPOOL_H__ */
