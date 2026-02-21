/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022. All rights reserved.
 * Description: Mempool page allocator
 * Author: Huawei OS Kernel Lab
 * Create: Thu Mar 21 11:33:17 2022
 */

#include "buddypool.h"

#include <sys/time.h>
#include <libhmsync/bitops.h>
#include <libmem/utils.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libdevhost/procfs.h>
#include <devhost/memory.h>
#include <devhost/log.h>
#include <devhost/cfg.h>
#include <libhwsecurec/securec.h>

#define BUDDYPOOL_CMA_MIN_FREE_ORDER 4
static unsigned long __buddypool_size_shrink(struct buddypool_allocator *buddy_allocator, int size)
{
	int shrink_order = (int)buddy_allocator->pool.info->sizeorder;
	struct libdh_buddy_page_s *page = NULL;
	int shrink_size;
	unsigned int page_sizeorder = buddy_allocator->pool.page_sizeorder;
	int ret;

	shrink_size = size;

	while (shrink_size > (int)(1U << page_sizeorder) && shrink_order >= BUDDYPOOL_CMA_MIN_FREE_ORDER) {
		if ((int)(1U << ((unsigned int)shrink_order + page_sizeorder)) > shrink_size) {
			shrink_order--;
			continue;
		}
		page = libdh_buddy_alloc(&buddy_allocator->buddy, (unsigned int)shrink_order,
								 LIBDEVHOST_PAGE_T_BUDDY);
		if (page == NULL) {
			shrink_order--;
			continue;
		}

		ret = devhost_backend_cma_free(buddy_allocator->cma,
					       (void *)page,
					       (size_t)1 << ((unsigned int)shrink_order + page_sizeorder));
		if (ret < 0) {
			dh_error("buddypool cma shrink free failed, ret=%s\n", hmstrerror(ret));
			libdh_buddy_free(&buddy_allocator->buddy, page);
			break;
		}

		page_pool_prepare_unmap(&buddy_allocator->pool, page,
					1UL << ((unsigned int)shrink_order + page_sizeorder));

		shrink_size -= (int)(1U << ((unsigned int)shrink_order + page_sizeorder));
	}

	return (unsigned long)(size - shrink_size);
}

static unsigned long buddypool_do_shrink(struct liblinux_page_allocator *page_allocator, int size)
{
	struct buddypool_allocator *buddy_allocator = NULL;
	unsigned long shrunk_size;
	int work_size = size;

	if (page_allocator == NULL) {
		dh_error("buddypool free page_allocator is NULL\n");
		return 0;
	}

	buddy_allocator = container_of(page_allocator, struct buddypool_allocator, impl);
	RAW_MUTEX_GUARD(_, &buddy_allocator->mutex);
	if (size == SHRINK_SIZE_ALL) {
		work_size = (int)(1U << (buddy_allocator->pool.info->sizeorder +\
			     buddy_allocator->pool.page_sizeorder));
	}

	shrunk_size = __buddypool_size_shrink(buddy_allocator, work_size);
	buddy_allocator->pool.cur_size = buddy_allocator->pool.cur_size >= shrunk_size ?\
					 buddy_allocator->pool.cur_size - shrunk_size :\
					 buddy_allocator->pool.cur_size;
	return shrunk_size;
}

static unsigned long buddypool_shrink(struct lnxbase_mem_shrinker *_shrinker, int size)
{
	struct buddypool_allocator *allocator = container_of(_shrinker, struct buddypool_allocator, shrinker);
	return buddypool_do_shrink(&allocator->impl, size);
}

static void *buddypool_alloc(const struct liblinux_page_allocator *page_allocator,
				 unsigned int order, unsigned long long *paddr)
{
	struct libdh_buddy_page_s *page = NULL;
	struct buddypool_allocator *buddy_allocator = container_of(page_allocator, struct buddypool_allocator, impl);

	raw_mutex_lock(&buddy_allocator->mutex);
	page = libdh_buddy_alloc(&buddy_allocator->buddy, order, LIBDEVHOST_PAGE_T_BUDDY);
	raw_mutex_unlock(&buddy_allocator->mutex);

	if ((page != NULL) && (paddr != NULL)) {
		*paddr = devhost_cma_virt_to_phys(buddy_allocator->cma, (void *)page);
	}

	return (void *)page;
}

static void buddypool_free(const struct liblinux_page_allocator *page_allocator,
			   unsigned long vaddr, unsigned int order)
{
	struct libdh_buddy_page_s *page = ulong_to_ptr(vaddr, struct libdh_buddy_page_s);
	struct buddypool_allocator *buddy_allocator = NULL;
	unsigned long nr_pages;
	int shrink_size;
	unsigned int page_size;

	if (page == NULL) {
		dh_error("buddypool free page is NULL\n");
		return;
	}

	if (page_allocator == NULL) {
		dh_error("buddypool free page_allocator is NULL\n");
		return;
	}
	buddy_allocator = container_of(page_allocator, struct buddypool_allocator, impl);
	RAW_MUTEX_GUARD(_, &buddy_allocator->mutex);
	page->order = order;
	page->type = LIBDEVHOST_PAGE_T_BUDDY;
	page->zone_type = (uint32_t)buddy_allocator->cma->cma_id;
	libdh_buddy_free(&buddy_allocator->buddy, page);
	nr_pages = libdh_buddy_get_nr_pages(&buddy_allocator->buddy);
	page_size = 1U << buddy_allocator->pool.page_sizeorder;
	if (nr_pages * page_size > buddy_allocator->pool.watermark) {
		shrink_size = (int)(nr_pages * page_size - buddy_allocator->pool.watermark);
		(void)__buddypool_size_shrink(buddy_allocator, shrink_size);
	}
}

static void buddypool_destroy(const struct liblinux_page_allocator *page_allocator)
{
	struct buddypool_allocator *buddy_allocator = NULL;
	int ret;

	if (page_allocator == NULL) {
		dh_error("buddypool free page_allocator is NULL\n");
		return;
	}
	buddy_allocator = container_of(page_allocator, struct buddypool_allocator, impl);

	(void)buddypool_do_shrink(&buddy_allocator->impl, SHRINK_SIZE_ALL);

	raw_mutex_lock(&buddy_allocator->mutex);
	ret = devhost_backend_cma_dyn_destroy(buddy_allocator->cma);
	if (ret != 0) {
		dh_error("buddypool destroy cma failed, err=%s\n", hmstrerror(ret));
	}
	page_pool_destroy(&buddy_allocator->pool);
	raw_mutex_unlock(&buddy_allocator->mutex);

	free(buddy_allocator);
	return;
}

static int __buddypool_cma_alloc(struct buddypool_allocator *buddy_allocator,
				 size_t size,
				 unsigned long *rvaddr, unsigned long long *rpaddr)
{
	void *vaddr;
	unsigned long long paddr;
	uint32_t prot = PROT_READ | PROT_WRITE;

	vaddr = devhost_backend_cma_alloc(buddy_allocator->cma, size, prot, &paddr);
	if (vaddr == NULL) {
		dh_error("buddypool backend cma alloc failed, pool cur_size=%#lx\n",
						buddy_allocator->pool.cur_size);
		return -ENOMEM;
	}
	*rvaddr = ptr_to_ulong(vaddr);
	*rpaddr = paddr;

	return E_HM_OK;
}

static int buddypool_page_alloc_extend(struct buddypool_allocator *buddypool,
			       unsigned long size,
			       int flags,
			       unsigned long *rvaddr,
			       unsigned long long *rpaddr)
{
	struct page_pool *pool = &buddypool->pool;
	int ret;
	bool over;
	bool reclaim = ____IS((unsigned int)flags & LIBLINUX_PAL_GFP_DIRECT_RECLAIM);

	raw_mutex_lock(&buddypool->mutex);
	if (unlikely(((pool->cur_size + size) < pool->cur_size) ||
	    ((pool->watermark + size) < pool->watermark))) {
		dh_debug("buddypool memory overflow:%lu,current memory:%lu, alloc size: %lu\n",
			 pool->watermark, pool->cur_size, size);
		raw_mutex_unlock(&buddypool->mutex);
		return -EINVAL;
	}
	over = ((pool->cur_size + size) > pool->watermark);
	raw_mutex_unlock(&buddypool->mutex);

	if (over && ____IS(reclaim)) {
		dh_debug("buddypool memory over watermark:%lu, current memory:%lu, alloc size: %lu, do reclaim\n",
			 pool->watermark, pool->cur_size, size);
		pool->try_reclaim();
		return -EAGAIN;
	}

	ret = __buddypool_cma_alloc(buddypool, size, rvaddr, rpaddr);
	if (ret == 0) {
		raw_mutex_lock(&buddypool->mutex);
		if (over && ____NOT(reclaim)) {
			dh_debug("buddypool memory over watermark:%lu,current memory:%lu, alloc size: %lu, do extend\n",
				 pool->watermark, pool->cur_size, size);
			pool->watermark += size;
		}
		pool->cur_size += size;
		raw_mutex_unlock(&buddypool->mutex);
	}
	return ret;
}

static int buddypool_extend_ex(const struct liblinux_page_allocator *page_allocator,
			      unsigned int order, unsigned long *rvaddr, int flags)
{
	int ret;
	unsigned long size;
	unsigned long vaddr = 0;
	unsigned long long paddr = 0;
	struct buddypool_allocator *buddy_allocator = NULL;
	struct libdh_buddy_page_s *page = NULL;

	if (page_allocator == NULL) {
		dh_error("buddypool extend page_allocator is NULL\n");
		return -EINVAL;
	}

	buddy_allocator = container_of(page_allocator, struct buddypool_allocator, impl);

	size = 1UL << (order + buddy_allocator->pool.page_sizeorder);

	ret = buddypool_page_alloc_extend(buddy_allocator, size, flags,
					  &vaddr, &paddr);
	if (ret != 0) {
		return ret;
	}

	/* prepare for the extended region */
	if (page_pool_prepare_map(&buddy_allocator->pool, paddr, size) == NULL) {
		return E_HM_INVAL;
	}

	page = ulong_to_ptr(vaddr, struct libdh_buddy_page_s);
	page->order = order;
	page->type = LIBDEVHOST_PAGE_T_BUDDY;
	page->zone_type = (uint32_t)buddy_allocator->cma->cma_id;
	libdh_buddy_free(&buddy_allocator->buddy, page);

	if (rvaddr != NULL) {
		*rvaddr = vaddr;
	}

	return ret;
}

static int __buddypool_extend_stat(const struct page_pool *pool, char *buf, size_t size,
				   unsigned long *rlen)
{
	struct buddypool_allocator *allocator = NULL;
	BUG_ON(pool == NULL);
	allocator = container_of(pool->page_allocator, struct buddypool_allocator, impl);
	return libdh_buddy_stat(&allocator->buddy, buf, size, rlen);
}

static int buddypool_cma_init(struct buddypool_allocator *allocator, unsigned int page_sizeorder)
{
	struct page_pool *pool = &allocator->pool;
	struct devhost_cma *cma = NULL;
	int ret;

	if (pool->info->rsv == DEVHOST_RSV_TYPE_ST) {
		ret = devhost_backend_cma_st_init((size_t)1 << (pool->info->sizeorder + page_sizeorder), &cma);
		if (ret != 0) {
			dh_error("buddypool init cma st failed, max size order=0x%x, page size order=0x%x, err=%s\n",
					pool->info->sizeorder, page_sizeorder, hmstrerror(ret));
			return ret;
		}
	} else if (pool->info->rsv == DEVHOST_RSV_TYPE_DYN) {
		ret = devhost_backend_cma_dyn_init((size_t)1 << (pool->info->sizeorder + page_sizeorder), &cma);
		if (ret != 0) {
			dh_error("buddypool init cma dyn failed, max size order=0x%x, page size order=0x%x, err=%s\n",
					pool->info->sizeorder, page_sizeorder, hmstrerror(ret));
			return ret;
		}
	} else {
		dh_error("buddypool init cma rsv failed\n");
		return E_HM_INVAL;
	}

	allocator->cma = cma;

	return E_HM_OK;
}

static int __buddypool_setup_cb(struct page_pool *pool)
{
	struct buddypool_allocator *allocator = NULL;
	struct devhost_cma *cma = NULL;
	void *vaddr = NULL;
	int ret;

	BUG_ON(pool == NULL);
	allocator = container_of(pool, struct buddypool_allocator, pool);
	cma = allocator->cma;

	/* prepare the 1st page t query mapping vaddr */
	vaddr = page_pool_prepare_map(pool, cma->spaddr, PAGE_SIZE);
	if (vaddr == NULL) {
		return E_HM_INVAL;
	}

	ret = devhost_backend_cma_setup(cma, vaddr);
	if (ret < 0) {
		return ret;
	}

	return E_HM_OK;
}

struct page_pool *buddypool_allocator_create(struct pool_info *info,
					     unsigned int page_sizeorder)
{
	struct buddypool_allocator *allocator = NULL;
	struct page_pool *pool = NULL;
	int ret;

	allocator = (struct buddypool_allocator *)calloc(1U, sizeof(*allocator));
	if (allocator == NULL) {
		dh_error("buddypool init allocator failed\n");
		goto err_init_allocator;
	}
	pool = &allocator->pool;
	raw_mutex_init(&allocator->mutex);

	/* init allocator as page pool */
	page_pool_init(&allocator->pool, page_sizeorder, info,
		       __buddypool_setup_cb);
	allocator->pool.page_allocator = &allocator->impl;

	/* init buddy */
	allocator->buddy.page_sizeorder = page_sizeorder;
	libdh_buddy_init(&allocator->buddy);

	/* init cma */
	ret = buddypool_cma_init(allocator, page_sizeorder);
	if (ret < 0) {
		goto err_init_cma;
	}

	/* init allocator ops */
	allocator->impl.destroy = buddypool_destroy;
	allocator->impl.extend = buddypool_extend_ex;
	allocator->impl.extend_ex = buddypool_extend_ex;
	allocator->impl.alloc = buddypool_alloc;
	allocator->impl.free = buddypool_free;
	allocator->shrinker.shrink = buddypool_shrink;

	if (strcmp(pool->info->name, "common") == 0) {
		lnxbase_register_mem_shrinker(&allocator->shrinker);
	}

	/* register procfs entry */
	ret = page_pool_procfs_register(pool, __buddypool_extend_stat);
	if (ret != 0) {
		dh_error("buddypool procfs register failed, err=%s\n", hmstrerror(ret));
	}

	dh_info("buddypool alloctor %s init done\n", allocator->pool.info->name);

	return &allocator->pool;

err_init_cma:
	free(allocator);
err_init_allocator:
	return NULL;
}
