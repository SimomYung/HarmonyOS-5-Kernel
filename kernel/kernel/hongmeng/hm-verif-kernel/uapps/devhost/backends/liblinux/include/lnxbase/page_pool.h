/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Base APIs for liblinux backend
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jun 23 09:40:43 2022
 */
#ifndef __LNXBASE_PAGE_POOL_H__
#define __LNXBASE_PAGE_POOL_H__

#ifndef __KERNEL__
#include <errno.h>
#include <stdlib.h> /* for NULL, no need for linux */
#endif
#include <liblinux/pal.h>
#include <hongmeng/errno.h>
#include <libhmsync/atomic.h>
#include <devhost/cfg.h>

struct pool_base {
	unsigned long kmap_virt_start;
	unsigned long kmap_virt_end;
	unsigned long ram_phys_start;
};

struct page_pool;
typedef int (*page_pool_setup_cb)(struct page_pool *pool);

struct page_pool {
	struct pool_base *base;
	struct pool_info *info;
	unsigned long cur_size;
	unsigned long watermark;
	unsigned int page_sizeorder;
	struct liblinux_page_allocator *page_allocator;
	void (*try_reclaim)(void);
	page_pool_setup_cb setup;
	raw_atomic_ullong_t alloc_size;
};

/*
 * lnxbase page allocator get.
 *
 * @param allocator_name	[I] name of allocator
 *
 * @return page allocator if operation succeeds.
 */
struct page_pool *lnxbase_page_pool_get(const char *name);

int lnxbase_page_pool_setup(struct page_pool *pool, const struct pool_base *base);

static inline int
lnxbase_page_pool_setup_linear(struct page_pool *pool,
			       unsigned long virt_start, unsigned long size,
			       unsigned long phys_start)
{
	struct pool_base base;
	base.kmap_virt_start = virt_start;
	base.kmap_virt_end = virt_start + size;
	base.ram_phys_start = phys_start;
	return lnxbase_page_pool_setup(pool, &base);
}

static inline int
lnxbase_page_pool_setup_default(struct page_pool *pool)
{
	struct page_pool *def_pool = NULL;
	def_pool = lnxbase_page_pool_get("common");
	if (def_pool == NULL) {
		return E_HM_INVAL;
	}
	return lnxbase_page_pool_setup(pool, def_pool->base);
}

#endif /* __LNXBASE_PAGE_POOL_H__ */
