/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Mempool page allocator
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 23 11:40:15 2021
 */

#ifndef __DEVHOST_LIBLINUX_MEMPOOL_H__
#define __DEVHOST_LIBLINUX_MEMPOOL_H__

#include <liblinux/pal.h>
#include <lnxbase/lnxbase.h>
#include <libmem/mem_pool.h>
#include <devhost/cfg.h>

#include "page_pool.h"
#include "mem_shrinker.h"

struct mempool_allocator {
	struct liblinux_page_allocator impl;
	struct lnxbase_mem_shrinker shrinker;
	struct mem_pool_s *pool;
	unsigned int min_alloc_shift;
	struct page_pool page_pool;
};

struct liblinux_page_allocator *mempool_allocator_init(unsigned int min_alloc_shift);

/*
 * Pool of mem initialization.
 *
 * @param pool		[I] pointer of page pool
 *
 * @return page allocator if operation succeeds.
 */
struct page_pool *mempool_allocator_create(struct pool_info *info,
					   unsigned int page_sizeorder);

#endif /* __DEVHOST_LIBLINUX_MEMPOOL_H__ */
