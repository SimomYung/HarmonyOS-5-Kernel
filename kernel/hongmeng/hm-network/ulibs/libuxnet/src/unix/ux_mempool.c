/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Source file unix mempool
 * Author: Huawei OS Kernel Lab
 * Create: Tue Nov 28 02:39:31 PM 2023
 */

#include "ux_mempool.h"
#include "ux_netlog.h"
#include "ux_fusion_ops.h"

#include <stddef.h>
#include <sys/mman.h>
#include <lib/dlist.h>
#include <lib/dlist_ext.h>
#include <libhmsync/raw_mutex.h>

#define SLABS_COUNT			1024
#define MEMPOOL_MAX_CACHED_SIZE		(1024 * 1024 * 16)
#ifndef PAGE_SHIFT
#define PAGE_SHIFT 12
#endif

struct ux_mempool {
	struct raw_mutex lock;

	raw_atomic_long_t total_alloced;
	raw_atomic_long_t cached_in_pool;

	size_t water_mark;

	struct dlist_node slabs[SLABS_COUNT];
} __mempool;

static inline int size_to_index(size_t size)
{
	return (int)(size >> PAGE_SHIFT);
}

static inline size_t index_to_size(int index)
{
	return (index + 1) << PAGE_SHIFT;
}

void *ux_mempool_alloc(size_t size)
{
	void *ptr = NULL;
	int index = size_to_index(size);
	if (index >= SLABS_COUNT) {
		ptr = fusion_valloc(index_to_size(index));
	} else {
		size_t alloc_size = index_to_size(index);
		struct dlist_node *head = &__mempool.slabs[index];

		/* try alloc from pool and need lock the mempool */
		raw_mutex_lock(&__mempool.lock);
		if (!dlist_empty(head)) {
			ptr = dlist_pop_first(head);
			raw_atomic_long_sub(&__mempool.cached_in_pool, alloc_size);
		}
		raw_mutex_unlock(&__mempool.lock);

		/*
		 * alloc from pool failed, try alloc from kvmalloc, kvmalloc can not do
		 * in mempool lock, for kvmalloc may trigger shrinker which will call
		 * @mempool_shrinker_shrink, and cause dead lock
		 */
		if (ptr == NULL) {
			ptr = fusion_valloc(alloc_size);
			if (ptr != NULL) {
				raw_atomic_long_add(&__mempool.total_alloced, alloc_size);
			}
		}
	}

	return ptr;
}

void ux_mempool_free(void *ptr, size_t size)
{
	int index = size_to_index(size);
	if (index >= SLABS_COUNT) {
		fusion_vfree(ptr);
	} else {
		size_t alloc_size = index_to_size(index);
		size_t total_cached;

		raw_mutex_lock(&__mempool.lock);
		total_cached = (size_t)raw_atomic_long_read(&__mempool.cached_in_pool);
		if (total_cached < __mempool.water_mark) {
			struct dlist_node *head = &__mempool.slabs[index];
			dlist_insert(head, (struct dlist_node*)ptr);
			raw_atomic_long_add(&__mempool.cached_in_pool, alloc_size);

			ptr = NULL;
		}
		raw_mutex_unlock(&__mempool.lock);

		if (ptr != NULL) {
			fusion_vfree(ptr);
			raw_atomic_long_sub(&__mempool.total_alloced, alloc_size);
		}
	}
}

static unsigned long mempool_shrinker_query(void *priv)
{
	struct ux_mempool *pool = (struct ux_mempool*)priv;

	return (unsigned long)raw_atomic_long_read(&pool->cached_in_pool) >> PAGE_SHIFT;
}

static unsigned long mempool_shrinker_shrink(void *priv, int req_nr)
{
	struct ux_mempool *pool = (struct ux_mempool*)priv;
	int index = SLABS_COUNT - 1;  /* iterator from the biggest one */
	int pages_freed = 0;

	raw_mutex_lock(&pool->lock);

	while (index > 0 && pages_freed < req_nr) {
		struct dlist_node *head = &pool->slabs[index];

		while (!dlist_empty(head) && pages_freed < req_nr) {
			void *ptr = dlist_pop_first(head);

			fusion_vfree(ptr);
			pages_freed += (index + 1);
		}

		--index;
	}

	raw_mutex_unlock(&pool->lock);

	return (unsigned long)pages_freed;
}

void ux_mempool_init(void)
{
	int rc;

	__mempool.water_mark = MEMPOOL_MAX_CACHED_SIZE;
	raw_atomic_long_init(&__mempool.total_alloced, 0);
	raw_atomic_long_init(&__mempool.cached_in_pool, 0);
	raw_mutex_init(&__mempool.lock);

	for (int i = 0; i < SLABS_COUNT; ++i) {
		dlist_init(&__mempool.slabs[i]);
	}

	rc = fusion_register_shrinker(mempool_shrinker_query, mempool_shrinker_shrink, &__mempool);
	if (rc != E_HM_OK) {
		BUG_ON(true, "unix mempool register shrinker failed %s\n", hmstrerror(rc));
	}
}
