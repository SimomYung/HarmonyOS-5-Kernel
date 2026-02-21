/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Mempool page allocator
 * Author: Huawei OS Kernel Lab
 * Create: Thu Sep 23 11:36:50 2021
 */

#include <libhmsync/bitops.h>
#include <libhmsrv_sys/hm_mem.h>
#include <devhost/log.h>
#include "../internal.h"
#include "mempool.h"

static void __mempool_destroy(const struct liblinux_page_allocator *_allocator)
{
	struct mempool_allocator *allocator = container_of(_allocator, struct mempool_allocator, impl);

	(void)mem_pool_destroy(allocator->pool);
	page_pool_destroy(&allocator->page_pool);
	free(allocator);
}

static int __check_page_pool_size(struct mempool_allocator *allocator,
				  struct page_pool *pool, unsigned long extend_size)
{
	raw_mutex_lock(&allocator->pool->mutex);
	if (unlikely(((pool->cur_size + extend_size) < pool->cur_size) ||
		     ((pool->watermark + extend_size) < pool->watermark))) {
		dh_debug("ldk memory overflow:%lu,current memory:%lu, extend size: %lu\n",
			 pool->watermark, pool->cur_size, extend_size);
		raw_mutex_unlock(&allocator->pool->mutex);
		return -EINVAL;
	}
	pool->cur_size += extend_size;
	raw_mutex_unlock(&allocator->pool->mutex);
	return 0;
}

static int __mempool_extend_alloc_size(struct mempool_allocator *allocator,
				       unsigned long extend_size, unsigned long alloc_size,
				       unsigned long *rvaddr, int flags)
{
	struct page_pool *pool = NULL;
	struct pool_base *base = NULL;
	unsigned long vaddr;
	void *map_va;
	unsigned long long paddr;
	int ret;

	BUG_ON(extend_size != ALIGN_UP(extend_size, (1UL << allocator->min_alloc_shift)));

	pool = &allocator->page_pool;
	base = allocator->page_pool.base;

	ret = __check_page_pool_size(allocator, pool, extend_size);
	if (ret != 0) {
		return ret;
	}

	ret = liblinux_pal_page_alloc_extend(base->kmap_virt_start - (unsigned long)base->ram_phys_start,
			base->kmap_virt_end, extend_size, &vaddr, &paddr, flags);
	if (ret < 0) {
		goto err_extend;
	}

	map_va = page_pool_prepare_map(&allocator->page_pool, paddr, extend_size);
	if ((map_va == NULL) || (ptr_to_ulong(map_va) != vaddr)) {
		ret = E_HM_INVAL;
		goto err_prepare;
	}

	ret = mem_pool_insert_alloc(allocator->pool, vaddr, paddr, extend_size, alloc_size);
	if (ret < 0) {
		dh_error("extend memory failed, ret=%s\n", hmstrerror(ret));
		goto err_alloc;
	}

	if (rvaddr != NULL) {
		*rvaddr = vaddr;
	}

	(void)raw_atomic_ullong_add_relaxed(&allocator->page_pool.alloc_size, alloc_size);
	return E_HM_OK;
err_alloc:
	page_pool_prepare_unmap(&allocator->page_pool, ulong_to_ptr(vaddr, void), extend_size);
err_prepare:
	(void)liblinux_pal_vm_unmap(ulong_to_ptr(vaddr, void));
err_extend:
	raw_mutex_lock(&allocator->pool->mutex);
	pool->cur_size -= extend_size;
	raw_mutex_unlock(&allocator->pool->mutex);
	return ret;
}

static int __mempool_extend_ex(const struct liblinux_page_allocator *_allocator,
			       unsigned int order, unsigned long *rvaddr, int flags)
{
	struct mempool_allocator *allocator = NULL;
	unsigned long size;

	BUG_ON(_allocator == NULL);
	allocator = container_of(_allocator, struct mempool_allocator, impl);
	size = 1UL << (order + allocator->min_alloc_shift);

	return __mempool_extend_alloc_size(allocator, size, 0, rvaddr, flags);
}

static int __mempool_extend_alloc(const struct liblinux_page_allocator *_allocator,
				  unsigned int extend_order, unsigned int alloc_order,
				  unsigned long *rvaddr, int flags)
{
	struct mempool_allocator *allocator = NULL;
	unsigned long extend_size;
	unsigned long alloc_size;

	BUG_ON(_allocator == NULL);

	if (alloc_order > extend_order) {
		return -EINVAL;
	}

	allocator = container_of(_allocator, struct mempool_allocator, impl);
	extend_size = 1UL << (extend_order + allocator->min_alloc_shift);
	alloc_size = 1UL << (alloc_order + allocator->min_alloc_shift);

	return __mempool_extend_alloc_size(allocator, extend_size, alloc_size, rvaddr, flags);
}

static int __mempool_extend_exact(const struct liblinux_page_allocator *_allocator,
				  unsigned long size, unsigned long *rvaddr, int flags)
{
	struct mempool_allocator *allocator = NULL;

	if (size == 0) {
		return -EINVAL;
	}

	BUG_ON(_allocator == NULL);
	allocator = container_of(_allocator, struct mempool_allocator, impl);

	return __mempool_extend_alloc_size(allocator, size, 0, rvaddr, flags);
}

static int __mempool_extend_alloc_exact(const struct liblinux_page_allocator *_allocator,
					unsigned long extend_size, unsigned long alloc_size,
					unsigned long *rvaddr, int flags)
{
	struct mempool_allocator *allocator = NULL;

	if ((extend_size == 0) || (alloc_size == 0) || (alloc_size > extend_size)) {
		return -EINVAL;
	}

	BUG_ON(_allocator == NULL);
	BUG_ON((extend_size & PAGE_MASK) != extend_size);
	BUG_ON((alloc_size & PAGE_MASK) != alloc_size);
	allocator = container_of(_allocator, struct mempool_allocator, impl);

	return __mempool_extend_alloc_size(allocator, extend_size, alloc_size, rvaddr, flags);
}

#ifdef CONFIG_MEMPOOL_RANGE_MMAP
static void __mempool_free_range(const struct mem_pool_s *pool, struct mem_pool_range_s *curr)
{
	uint32_t sz;
	size_t bytes;
	int ret;

	sz = (uint32_t)(curr->end - curr->start) >> pool->order;
	bytes = sizeof(struct mem_pool_range_s) +
		(BITS_TO_LONGLONGS(sz) * sizeof(long long));
	ret = hm_mem_munmap(curr, bytes);
	if (ret != E_HM_OK) {
		dh_error("unmap pool range failed, ret=%s\n", hmstrerror(ret));
	}
}
#else
static void __mempool_free_range(const struct mem_pool_s *pool, struct mem_pool_range_s *curr)
{
	UNUSED(pool);
	free(curr);
}
#endif

static unsigned long __mempool_unmap_chunk_list(const struct liblinux_page_allocator *_allocator,
				       struct mem_pool_s *pool, struct dlist_node *shrink_list)
{
	struct mem_pool_range_s *curr = NULL;
	struct mem_pool_range_s *next = NULL;
	int ret;
	unsigned long unmap_size = 0;

	dlist_for_each_entry_safe(curr, next, shrink_list,
				  struct mem_pool_range_s, node) {
		uintptr_t start;
		dlist_delete(&curr->node);
		start = curr->start;
		ret = liblinux_pal_vm_unmap((void *)start);
		if (ret == 0) {
			unmap_size += (curr->end - curr->start);
			if (_allocator->shrink != NULL) {
				_allocator->shrink(_allocator, curr->start, curr->paddr, curr->end - curr->start);
			}
			__mempool_free_range(pool, curr);
		} else if (ret == -ENOSYS) {
			raw_mutex_lock(&pool->mutex);
			dlist_insert_tail(&pool->dnode, &curr->node);
			raw_mutex_unlock(&pool->mutex);
		} else {
			unmap_size += (curr->end - curr->start);
			dh_warn("unexpected liblinux_pal_vm_unmap failure, err=%d\n", ret);
		}
	}

	return unmap_size;
}

static unsigned long __mempool_do_shrink(const struct liblinux_page_allocator *_allocator, int size)
{
	struct mempool_allocator *allocator = container_of(_allocator, struct mempool_allocator, impl);
	struct mem_pool_s *pool = allocator->pool;
	struct mem_pool_range_s *curr = NULL;
	struct mem_pool_range_s *next = NULL;
	uint32_t obit_idx; /* index of one bit. */
	uint32_t sz;
	unsigned long shrink_size = 0;
	unsigned long unmap_size = 0;
	DLIST_HEAD(shrink_list);
	if (pool != NULL) {
		raw_mutex_lock(&pool->mutex);
		dlist_for_each_entry_safe(curr, next, &pool->dnode,
					  struct mem_pool_range_s, node) {
			sz = (uint32_t)(curr->end - curr->start) >> pool->order;
			obit_idx = raw_bitmap_find_first_one(curr->bmap, sz);
			/* Ensure that the memory in current range is not used. */
			if (obit_idx < sz) {
				continue;
			}
			if (size != SHRINK_SIZE_ALL && (int)shrink_size >= size) {
				break;
			}
			dlist_delete(&curr->node);
			dlist_insert_tail(&shrink_list, &curr->node);
			shrink_size += (curr->end - curr->start);
		}
		raw_mutex_unlock(&pool->mutex);
		unmap_size = __mempool_unmap_chunk_list(_allocator, pool, &shrink_list);
	}

	return unmap_size;
}

static void *__mempool_dma_alloc_size(struct mempool_allocator *allocator,
				      unsigned long size, unsigned long long *paddr)
{
	int ret = 0;
	void *vaddr = NULL;
	uint64_t pa;

	vaddr = (void *)mem_pool_alloc(allocator->pool, size);
	if ((vaddr != NULL) && (paddr != NULL)) {
		ret = mem_pool_paddr_from_vaddr(allocator->pool, (uintptr_t)vaddr, &pa);
		if (ret < 0) {
			(void)mem_pool_free(allocator->pool, (uintptr_t)vaddr, size);
			vaddr = NULL;
		} else {
			*paddr = (unsigned long long)pa;
		}
	}

	if (vaddr != NULL) {
		(void)raw_atomic_ullong_add_relaxed(&allocator->page_pool.alloc_size, size);
	}

	return vaddr;
}

static void *__mempool_dma_alloc(const struct liblinux_page_allocator *_allocator,
				 unsigned int order, unsigned long long *paddr)
{
	struct mempool_allocator *allocator = container_of(_allocator, struct mempool_allocator, impl);
	size_t size = 1UL << (order + allocator->min_alloc_shift);

	return __mempool_dma_alloc_size(allocator, size, paddr);
}

static void *__mempool_dma_alloc_exact(const struct liblinux_page_allocator *_allocator,
				       unsigned long size, unsigned long long *paddr)
{
	struct mempool_allocator *allocator;

	BUG_ON(_allocator == NULL);

	if (size == 0) {
		return NULL;
	}

	allocator = container_of(_allocator, struct mempool_allocator, impl);

	return __mempool_dma_alloc_size(allocator, size, paddr);
}

static void __mempool_free(const struct liblinux_page_allocator *_allocator,
			   unsigned long vaddr, unsigned int order)
{
	struct mempool_allocator *allocator = container_of(_allocator, struct mempool_allocator, impl);
	unsigned long size = 1UL << (order + allocator->min_alloc_shift);
	int ret = mem_pool_free(allocator->pool, vaddr, size);
	if (ret < 0) {
		dh_warn("__mempool_free failed, size=0x%lx, ret=%s\n", size, hmstrerror(ret));
	} else {
		(void)raw_atomic_ullong_sub_relaxed(&allocator->page_pool.alloc_size, size);
	}
}

static void __mempool_free_exact(const struct liblinux_page_allocator *_allocator,
				 unsigned long vaddr, unsigned long size)
{
	struct mempool_allocator *allocator = container_of(_allocator, struct mempool_allocator, impl);
	int ret = mem_pool_free(allocator->pool, vaddr, size);
	if (ret < 0) {
		dh_warn("mem_pool_free failed, size=0x%lx, ret=%s\n", size, hmstrerror(ret));
	} else {
		(void)raw_atomic_ullong_sub_relaxed(&allocator->page_pool.alloc_size, size);
	}
}


static unsigned long __mempool_shrink(struct lnxbase_mem_shrinker *_shrinker, int size)
{
	struct mempool_allocator *allocator = container_of(_shrinker, struct mempool_allocator, shrinker);
	unsigned long shrinked;

	shrinked = __mempool_do_shrink(&allocator->impl, size);

	raw_mutex_lock(&allocator->pool->mutex);
	allocator->page_pool.cur_size -= shrinked;
	raw_mutex_unlock(&allocator->pool->mutex);

	return shrinked;
}

static int __mempool_should_reclaim(void)
{
	return lnxbase_page_pool_watermark_reached();
}

struct liblinux_page_allocator *mempool_allocator_init(unsigned int min_alloc_shift)
{
	struct mem_pool_s *mem_pool = NULL;
	struct mempool_allocator *allocator = NULL;

	mem_pool = mem_pool_create(min_alloc_shift);
	if (mem_pool != NULL) {
		mem_pool_set_algo(mem_pool, mem_pool_find_best);
		mem_pool_set_insert_behavior(mem_pool, mem_pool_insert_asc_nolock);

		allocator = (struct mempool_allocator *)calloc(1U, sizeof(*allocator));
		if (allocator == NULL) {
			(void)mem_pool_destroy(mem_pool);
		}
	}

	if (allocator != NULL) {
		allocator->pool = mem_pool;
		allocator->min_alloc_shift = min_alloc_shift;
		allocator->impl.destroy = __mempool_destroy;
		allocator->impl.extend = __mempool_extend_ex;
		allocator->impl.alloc = __mempool_dma_alloc;
		allocator->impl.free = __mempool_free;

		allocator->shrinker.shrink = __mempool_shrink;
		lnxbase_register_mem_shrinker(&allocator->shrinker);
	}

	return (allocator == NULL) ? NULL : &allocator->impl;
}

struct page_pool *mempool_allocator_create(struct pool_info *info,
					   unsigned int page_sizeorder)
{
	struct mempool_allocator *allocator = NULL;
	struct liblinux_page_allocator *_allocator = NULL;
	int ret;

	_allocator = mempool_allocator_init(page_sizeorder);
	if (_allocator == NULL) {
		return NULL;
	}
	allocator = container_of(_allocator, struct mempool_allocator, impl);

	/* use new api for extend */
	allocator->impl.extend_ex = __mempool_extend_ex;

	/* for optimizing alloc_pages_exact */
	allocator->impl.extend_exact = __mempool_extend_exact;
	allocator->impl.alloc_exact = __mempool_dma_alloc_exact;
	allocator->impl.free_exact = __mempool_free_exact;

	allocator->impl.extend_alloc = __mempool_extend_alloc;
	allocator->impl.extend_alloc_exact = __mempool_extend_alloc_exact;

	allocator->impl.should_reclaim = __mempool_should_reclaim;

	/* init allocator as page pool */
	page_pool_init(&allocator->page_pool, page_sizeorder, info, NULL);
	allocator->page_pool.page_allocator = &allocator->impl;

	/* register procfs entry */
	ret = page_pool_procfs_register(&allocator->page_pool, NULL);
	if (ret != 0) {
		dh_error("mempool procfs register failed, err=%s\n", hmstrerror(ret));
	}

	return &allocator->page_pool;
}
