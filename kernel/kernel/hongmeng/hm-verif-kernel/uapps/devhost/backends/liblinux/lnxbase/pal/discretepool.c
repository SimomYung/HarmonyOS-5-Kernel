/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 * Description: Discrete page allocator
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 23 12:16:00 2022
 */

#include "discretepool.h"

#include <lnxbase/iofast.h>
#include <lnxbase/vmap.h>

#include <hmasm/page.h>
#include <devhost/log.h>
#include <libmem/utils.h>
#include <libstrict/strict.h>

struct discretepool_allocator_group {
	struct raw_rwlock rwlock;
	struct dlist_node head;
};

static struct discretepool_allocator_group allocator_group = {
	.rwlock = RAW_RWLOCK_INITIALIZER(allocator_group.rwlock),
	.head = DLIST_HEAD_INIT(allocator_group.head),
};

static void discretepool_allocator_register(struct discretepool_allocator *allocator)
{
	BUG_ON(allocator == NULL);
	RAW_RWLOCK_WR_GUARD(_, &allocator_group.rwlock);
	dlist_insert_tail(&allocator_group.head, &allocator->dnode);
}

static unsigned long discretepool_used_pages_stat(void)
{
	struct discretepool_allocator *allocator = NULL;
	unsigned long used_pages = 0;

	raw_rwlock_rdlock(&allocator_group.rwlock);
	dlist_for_each_entry(allocator, &allocator_group.head, struct discretepool_allocator, dnode) {
		used_pages += (unsigned long)raw_atomic_ullong_read_relaxed(
				&allocator->page_pool.alloc_size) >> (allocator->min_alloc_shift);
	}
	raw_rwlock_unlock(&allocator_group.rwlock);

	return used_pages;
}

static unsigned long discretepool_free_pages_stat(void)
{
	struct discretepool_allocator *allocator = NULL;
	unsigned long free_pages = 0;

	raw_rwlock_rdlock(&allocator_group.rwlock);
	dlist_for_each_entry(allocator, &allocator_group.head, struct discretepool_allocator, dnode) {
		free_pages += libdh_buddy_get_nr_pages(&allocator->buddy);
	}
	raw_rwlock_unlock(&allocator_group.rwlock);

	return free_pages;
}

static void __page_pool_init(struct page_pool *pool,
			     unsigned int page_sizeorder,
			     struct pool_info *info,
			     struct liblinux_page_allocator *allocator_impl)
{
	page_pool_init(pool, page_sizeorder, info, NULL);
	pool->page_allocator = allocator_impl;
}

static void __buddy_init(struct libdh_buddy_s *buddy, unsigned int page_sizeorder)
{
	buddy->page_sizeorder = page_sizeorder;
	libdh_buddy_init(buddy);
}

static void
__discretepool_destroy(const struct liblinux_page_allocator *allocator_impl)
{
	struct discretepool_allocator *allocator;

	BUG_ON(allocator_impl == NULL);
	allocator = page_to_allocator(allocator_impl,
				      struct discretepool_allocator, impl);
	page_pool_destroy(&allocator->page_pool);
	free(allocator);
}

static void *
__discretepool_alloc(const struct liblinux_page_allocator *allocator_impl,
		     unsigned int order, unsigned long long *paddr)
{
	struct discretepool_allocator *allocator;
	struct libdh_buddy_page_s *page = NULL;
	UNUSED(paddr);

	BUG_ON(allocator_impl == NULL);
	allocator = page_to_allocator(allocator_impl,
				      struct discretepool_allocator, impl);
	page = libdh_buddy_alloc(&allocator->buddy, order,
				 LIBDEVHOST_PAGE_T_BUDDY);
	if (page != NULL) {
		(void)raw_atomic_ullong_add_relaxed(&allocator->page_pool.alloc_size,
			1UL << (order + allocator->min_alloc_shift));
	}
	return (void *)page;
}

static bool __discretepool_need_shrink(struct discretepool_allocator *allocator)
{
	unsigned long nr_free_pages;
	unsigned int high_watermark;

	high_watermark = allocator->page_pool.info->high_watermark;
	if (high_watermark != 0) {
		nr_free_pages = libdh_buddy_get_nr_pages(&allocator->buddy);
		if (nr_free_pages > high_watermark) {
			return true;
		}
	}

	return false;
}

static void
__discretepool_free(const struct liblinux_page_allocator *allocator_impl,
		    unsigned long vaddr, unsigned int order)
{
	struct discretepool_allocator *allocator;
	struct libdh_buddy_page_s *page = NULL;
	unsigned long size;

	BUG_ON(allocator_impl == NULL);
	allocator = page_to_allocator(allocator_impl,
				      struct discretepool_allocator, impl);
	page = ulong_to_ptr(vaddr, struct libdh_buddy_page_s);
	page->order = order;
	page->type = LIBDEVHOST_PAGE_T_BUDDY;
	page->zone_type = 0;
	libdh_buddy_free(&allocator->buddy, page);

	(void)raw_atomic_ullong_sub_relaxed(&allocator->page_pool.alloc_size,
		1UL << (order + allocator->min_alloc_shift));

	/* sync free to sysmgr */
	if (__discretepool_need_shrink(allocator)) {
		size = 1UL << (order + allocator->min_alloc_shift);
		(void)allocator->shrinker.shrink(&allocator->shrinker, size);
	}
}

static unsigned int g_min_extend_order = LIBDEVHOST_BUDDY_MAX_ORDER;
static int __normal_extend_alloc(struct discretepool_allocator *allocator,
				 unsigned int extend_order, unsigned int alloc_order,
				 unsigned long *rvaddr, int flags)
{
	struct page_pool *pool = NULL;
	struct pool_base *base = NULL;
	struct libdh_buddy_page_s *alloc_page = NULL;
	struct libdh_buddy_page_s *extend_page = NULL;
	unsigned long vaddr;
	void *map_va = NULL;
	unsigned long long paddr;
	unsigned long extend_size;
	int ret;

	pool = &allocator->page_pool;
	base = allocator->page_pool.base;
	extend_size = 1UL << (extend_order + allocator->min_alloc_shift);

	ret = liblinux_pal_page_alloc_extend(
			base->kmap_virt_start - (unsigned long)base->ram_phys_start,
			base->kmap_virt_end, extend_size, &vaddr, &paddr, flags);
	if (ret < 0) {
		return ret;
	}

	map_va = page_pool_prepare_map(pool, paddr, extend_size);
	if ((map_va == NULL) || (ptr_to_ulong(map_va) != vaddr)) {
		(void)liblinux_pal_vm_unmap(ulong_to_ptr(vaddr, void));
		return -EINVAL;
	}

	extend_page = ulong_to_ptr(vaddr, struct libdh_buddy_page_s);
	extend_page->order = extend_order;
	extend_page->type = LIBDEVHOST_PAGE_T_BUDDY;
	extend_page->zone_type = 0;
	alloc_page = libdh_buddy_extend_alloc(&allocator->buddy, extend_page, alloc_order);
	if (alloc_page != NULL) {
		(void)raw_atomic_ullong_add_relaxed(&pool->alloc_size,
			1UL << (alloc_order + allocator->min_alloc_shift));
	}

	raw_mutex_lock(&allocator->mutex);
	pool->cur_size += extend_size;
	if (g_min_extend_order > extend_order) {
		g_min_extend_order = extend_order;
	}
	raw_mutex_unlock(&allocator->mutex);

	if (rvaddr != NULL) {
		*rvaddr = vaddr;
	}

	return E_HM_OK;
}

static int __iofast_extend_alloc(struct discretepool_allocator *allocator,
				 unsigned int extend_order, unsigned int alloc_order,
				 unsigned long *rvaddr, unsigned int flags)
{
	int ret;
	unsigned int i;
	unsigned int pa_num;
	unsigned int fail_num = 0;
	struct page_pool *pool = NULL;
	unsigned long long extend_array[DISCRETEPOOL_DEF_ARRAY_LEN] = { 0 };
	unsigned long real_num = ____IS(flags & LIBLINUX_PAL_GFP_NOEXTRA) ?
				 1UL : (unsigned long)DISCRETEPOOL_DEF_ARRAY_LEN;

	ret = lnxbase_iofast_alloc_pfn_v(extend_order, extend_array,
					 real_num, flags, &pa_num);
	if (ret < 0) {
		dh_debug("discretepool alloc pfn failed, ret=%s\n", hmstrerror(ret));
		return -hmerrno2posix(ret);
	}

	pool = &allocator->page_pool;
	for (i = 0; i < pa_num; i++) {
		void *vaddr = NULL;
		unsigned int order;
		unsigned long long paddr;
		unsigned long extend_size;
		struct libdh_buddy_page_s *extend_page = NULL;

		paddr = extend_array[i] & PAGE_MASK;
		order = extend_array[i] & (~PAGE_MASK);
		extend_size = 1UL << (order + allocator->min_alloc_shift);
		vaddr = page_pool_prepare_map(pool, paddr, extend_size);
		if (vaddr == NULL) {
			extend_array[fail_num++] = extend_array[i];
			continue;
		}

		extend_page = vaddr;
		extend_page->order = order;
		extend_page->type = LIBDEVHOST_PAGE_T_BUDDY;
		extend_page->zone_type = 0;
		if ((alloc_order <= order) && (rvaddr != NULL) && (*rvaddr == 0UL)) {
			if (libdh_buddy_extend_alloc(&allocator->buddy, extend_page, alloc_order) != NULL) {
				*rvaddr = ptr_to_ulong(vaddr);
				(void)raw_atomic_ullong_add_relaxed(&pool->alloc_size,
					1UL << (alloc_order + allocator->min_alloc_shift));
			}
		} else {
			libdh_buddy_free(&allocator->buddy, extend_page);
		}

		raw_mutex_lock(&allocator->mutex);
		pool->cur_size += extend_size;
		raw_mutex_unlock(&allocator->mutex);
	}

	if (fail_num > 0) {
		/* free the memory that failed to populate */
		(void)lnxbase_iofast_free_pfn_v(extend_array, fail_num);
	}

	return (fail_num < pa_num) ? 0 : -ENOMEM;
}

static int
__discretepool_extend_alloc(const struct liblinux_page_allocator *allocator_impl,
			    unsigned int extend_order, unsigned int alloc_order,
			    unsigned long *rvaddr, int flags)
{
	int ret;
	struct discretepool_allocator *allocator;

	BUG_ON(allocator_impl == NULL);
	allocator = container_of(allocator_impl, struct discretepool_allocator, impl);

	if (lnxbase_use_iofast()) {
		ret = __iofast_extend_alloc(allocator, extend_order, alloc_order, rvaddr, (unsigned int)flags);
	} else {
		ret = __normal_extend_alloc(allocator, extend_order, alloc_order, rvaddr, flags);
	}

	return ret;
}

static int
__discretepool_extend_ex(const struct liblinux_page_allocator *allocator_impl,
			 unsigned int order, unsigned long *rvaddr, int flags)
{
	/* When alloc_order is greater than extend_order, no memory is allocated */
	return __discretepool_extend_alloc(allocator_impl, order, order + 1, rvaddr, flags);
}

static int __normal_do_buddy_shrink(struct page_pool *pool, struct pool_base *base,
				    void *vaddr, unsigned int order)
{
	struct vmap_node *node = NULL;
	unsigned long long paddr;
	unsigned long size;
	unsigned long offset;
	void *map_va = NULL;
	int ret;

	node = vmap_tree_find(vaddr);
	if (node == NULL) {
		return E_HM_INVAL;
	}

	size = 1UL << (order + pool->page_sizeorder);
	if (size != node->size) {
		return E_HM_INVAL;
	}

	page_pool_prepare_unmap(pool, vaddr, size);
	ret = liblinux_pal_vm_unmap(vaddr);
	if (ret < 0) {
		dh_warn("discretepool unmap shm failed\n");
		offset = ptr_to_ulong(vaddr) - base->kmap_virt_start;
		paddr = base->ram_phys_start + offset;
		map_va = page_pool_prepare_map(pool, paddr, size);
		if ((map_va == NULL) || (map_va != vaddr)) {
			dh_warn("discretepool remap mem failed\n");
		}
		return ret;
	}

	return E_HM_OK;
}

static int __iofast_do_buddy_shrink(struct page_pool *pool, struct pool_base *base,
				    void *vaddr, unsigned int order)
{
	int ret;
	unsigned long size;
	unsigned long offset;
	unsigned long long pa, paddr;

	offset = ptr_to_ulong(vaddr) - base->kmap_virt_start;
	pa = base->ram_phys_start + offset;
	/* use low 12bit to storage the order */
	paddr = pa | (order & (~PAGE_MASK));
	size = 1UL << (order + pool->page_sizeorder);
	page_pool_prepare_unmap(pool, vaddr, size);

	ret = lnxbase_iofast_free_pfn_v(&paddr, 1);
	/*
	 * this ret is not normal return value,
	 * it means the count of memory which is failed to be freed,
	 * so, check it by !=
	 */
	if (ret != 0) {
		void *map_va = page_pool_prepare_map(pool, pa, size);
		if ((map_va == NULL) || (map_va != vaddr)) {
			dh_warn("discretepool iofast remap mem failed\n");
		}
		return ret;
	}

	return E_HM_OK;
}

static int __discretepool_do_buddy_shrink(struct libdh_buddy_s *buddy,
					  void *vaddr, unsigned int order)
{
	int ret;
	struct pool_base *base = NULL;
	struct page_pool *pool = NULL;
	struct discretepool_allocator *allocator = NULL;

	BUG_ON(buddy == NULL);
	allocator = container_of(buddy, struct discretepool_allocator, buddy);
	pool = &allocator->page_pool;
	base = pool->base;

	if (lnxbase_use_iofast()) {
		ret = __iofast_do_buddy_shrink(pool, base, vaddr, order);
	} else {
		ret = __normal_do_buddy_shrink(pool, base, vaddr, order);
	}

	return ret;
}

static unsigned long __discretepool_free_page_cnt(struct discretepool_allocator *allocator)
{
	unsigned long nr_free_pages;
	unsigned int low_watermark;

	low_watermark = allocator->page_pool.info->low_watermark;
	nr_free_pages = libdh_buddy_get_nr_pages(&allocator->buddy);
	if (nr_free_pages <= low_watermark) {
		return 0;
	}

	return (nr_free_pages - low_watermark);
}

static unsigned long
__discretepool_do_shrink(struct discretepool_allocator *allocator, int size)
{
	unsigned long shrinked = 0;
	unsigned long free_page_cnt = 0;
	int shrink_size = 0;

	BUG_ON(allocator == NULL);
	free_page_cnt = __discretepool_free_page_cnt(allocator);
	if ((free_page_cnt << allocator->min_alloc_shift) > INT_MAX) {
		shrink_size = size;
	} else {
		shrink_size = MIN((int)(unsigned int)(free_page_cnt << allocator->min_alloc_shift), size);
	}
	if (shrink_size == 0) {
		return 0;
	}

	shrinked = libdh_buddy_shrinked(&allocator->buddy, shrink_size,
					allocator->page_pool.page_sizeorder,
					__discretepool_do_buddy_shrink);

	raw_mutex_lock(&allocator->mutex);
	allocator->page_pool.cur_size -= shrinked;
	raw_mutex_unlock(&allocator->mutex);

	return shrinked;
}

static unsigned long
__discretepool_shrink(struct lnxbase_mem_shrinker *_shrinker, int size)
{
	struct discretepool_allocator *allocator;
	unsigned long shrinked_size = 0;

	BUG_ON(_shrinker == NULL);
	allocator = container_of(_shrinker, struct discretepool_allocator, shrinker);

	shrinked_size = __discretepool_do_shrink(allocator, size);

	return shrinked_size;
}

static int page_cnt_to_size(int max_nr)
{
	int size;
	if (max_nr > (int)((unsigned int)INT_MAX >> PAGE_SHIFT)) {
		dh_info("except to shrink too much memory %d pages\n", max_nr);
		size = SHRINK_SIZE_ALL;
	} else {
		size = (max_nr == SHRINK_SIZE_ALL ? max_nr : (int)((unsigned int)max_nr << PAGE_SHIFT));
	}
	return size;
}

static unsigned long
__discretepool_shrink_ex(void *_shrinker, int max_nr)
{
	struct discretepool_allocator *allocator;
	unsigned long shrinked_size = 0;
	int size = 0;

	BUG_ON(_shrinker == NULL);
	allocator = shrinker_to_allocator(_shrinker,
					  struct discretepool_allocator, impl);
	size = page_cnt_to_size(max_nr);

	shrinked_size = __discretepool_do_shrink(allocator, size);

	return shrinked_size;
}

static unsigned long
__discretepool_query_ex(void *_shrinker)
{
	struct discretepool_allocator *allocator;
	unsigned long free_size = 0;

	BUG_ON(_shrinker == NULL);
	allocator = shrinker_to_allocator(_shrinker,
					  struct discretepool_allocator, impl);
	free_size = __discretepool_free_page_cnt(allocator) << allocator->min_alloc_shift;

	return free_size >> PAGE_SHIFT;
}

static int __discretepool_stat(const struct page_pool *pool, char *buf,
			       size_t size, unsigned long *rlen)
{
	struct discretepool_allocator *allocator = NULL;
	BUG_ON(pool == NULL);
	allocator = page_to_allocator(pool->page_allocator,
				       struct discretepool_allocator, impl);

	return libdh_buddy_stat(&allocator->buddy, buf, size, rlen);
}

static void __discretepool_ops_init(struct discretepool_allocator *allocator)
{
	struct liblinux_page_allocator page_allocator = { 0 };

	page_allocator.destroy = __discretepool_destroy;
	page_allocator.alloc = __discretepool_alloc;
	page_allocator.free = __discretepool_free;
	page_allocator.extend = __discretepool_extend_ex;
	page_allocator.extend_ex = __discretepool_extend_ex;
	page_allocator.extend_alloc = __discretepool_extend_alloc;
	lnxbase_shrinker_allocator_init(&allocator->impl,
					&page_allocator,
					__discretepool_query_ex,
					__discretepool_shrink_ex);

	allocator->shrinker.shrink = __discretepool_shrink;
	lnxbase_register_mem_shrinker(&allocator->shrinker);
}

struct page_pool *discretepool_allocator_create(struct pool_info *info,
						unsigned int page_sizeorder)
{
	struct discretepool_allocator *allocator = NULL;
	int ret;

	allocator = (struct discretepool_allocator *)malloc(sizeof(*allocator));
	if (allocator == NULL) {
		return NULL;
	}

	allocator->min_alloc_shift = page_sizeorder;
	raw_mutex_init(&allocator->mutex);
	__page_pool_init(&allocator->page_pool, page_sizeorder, info,
			 &allocator->impl.allocator);
	__buddy_init(&allocator->buddy, page_sizeorder);
	__discretepool_ops_init(allocator);

	ret = page_pool_procfs_register(&allocator->page_pool,
					__discretepool_stat);
	if (ret != 0) {
		dh_error("discrete pool procfs register failed, err=%s\n",
			 hmstrerror(ret));
	}
	discretepool_allocator_register(allocator);

	lnxbase_stat_func_register(LNXBASE_DISCRETEPOOL_USED_PAGES,
					  discretepool_used_pages_stat);
	lnxbase_stat_func_register(LNXBASE_DISCRETEPOOL_FREE_PAGES,
					  discretepool_free_pages_stat);

	return &allocator->page_pool;
}
