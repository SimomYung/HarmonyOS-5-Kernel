/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Implementation for mem_pool mechanism.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Mar 01 14:57:37 2021
 */
#include <libmem/mem_pool.h>
#include <libhwsecurec/securec.h>
#include <libhmsync/bitops.h>
#include <libstrict/strict.h>
#include <libhmsrv_sys/hm_mem.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <malloc.h>

#define MP_SIZE_TO_COUNT(size, order) (uint32_t)((size) >> (order))
/*
 * Memory position of bitmap corresponding to the relative mem_pool_range_s.
 * Actually it resides just behind the range->bmap.
 */
#define MP_BITMAP_OFFSET 1

#if defined(__HOST_LLT__) && defined(CONFIG_MEMPOOL_RANGE_MMAP)
static void *llt_hm_mem_mmap(void *addr, size_t len, uint32_t prot, uint32_t flags, int32_t fd, int64_t offset);
static int llt_hm_mem_munmap(void *addr, size_t len);
#define hm_mem_mmap(addr, len, prot, flags, fd, offset) llt_hm_mem_mmap(addr, len, prot, flags, fd, offset)
#define hm_mem_munmap(addr, len) llt_hm_mem_munmap(addr, len)
#endif

typedef int (*update_func)(unsigned long long *bitmap,
			   unsigned int sz,
			   unsigned int bit_off);

/* Check wthether given range is in memory pool range node. */
static inline bool range_is_valid(uintptr_t va, size_t len,
				  const struct mem_pool_range_s *mpr)
{
	return ((mpr == NULL) ||
		(va < mpr->start) || (va >= mpr->end) ||
		(len > (SIZE_MAX - va)) ||
		((uintptr_t)(va + len) > mpr->end)) ? false : true;
}

/* Check wthether the given parameters is valid pool memory or not. */
static inline int mem_pool_check_valid(const struct mem_pool_s *p,
				       uintptr_t va,
				       size_t len)
{
	/*
	 * Mem pool granularity alignment
	 *
	 * Here may be changed if the caller required
	 * different alignment.
	 */
	return ((p == NULL) || (len == 0UL) ||
		!IS_ALIGNED(va, (1UL << p->order)) ||
		!IS_ALIGNED(len, (1UL << p->order)) ||
		(len > (SIZE_MAX - va))) ?
	       E_HM_INVAL : E_HM_OK;
}

/* Check whether the modifying range resides in bitmap or not. */
static inline int check_bitmap_range_valid(const unsigned long long *bmap,
					   uint32_t sz,
					   uint32_t start,
					   uint32_t end)
{
	int rc = E_HM_OK;

	if ((start >= sz) || (end > sz) ||
	    (end <= start) || (bmap == NULL)) {
		rc = E_HM_INVAL;
	}

	return rc;
}

static int do_bitmap_update(unsigned long long *bmap,
			    uint32_t sz,
			    uint32_t start,
			    uint32_t end,
			    update_func func)
{
	uint32_t idx = start;
	int err;

	err = check_bitmap_range_valid(bmap, sz, start, end);
	while ((err == E_HM_OK) && (idx < end)) {
		err = func(bmap, sz, idx);
		BUG_ON(err != E_HM_OK);
		idx++;
	}

	return err;
}

/* Set bit value of the given range to zero: [start, end) */
static int do_bitmap_clear_range(unsigned long long *bmap,
				 uint32_t sz,
				 uint32_t start,
				 uint32_t end)
{
	return do_bitmap_update(bmap, sz, start, end, raw_bitmap_clear_bit);
}

/* Set bit value of the given range to one: [start, end) */
static int do_bitmap_set_range(unsigned long long *bmap,
			       uint32_t sz,
			       uint32_t start,
			       uint32_t end)
{
	return do_bitmap_update(bmap, sz, start, end, raw_bitmap_set_bit);
}

uint32_t mem_pool_find_first(const struct mem_pool_s *pool,
			     const unsigned long long *bmap,
			     uint32_t sz,
			     uint32_t start,
			     uint32_t len)
{
	uint32_t zbit_idx = start; /* index of zero bit. */
	uint32_t obit_idx = 0U; /* index of one bit. */
	uint32_t end = 0U;
	uint32_t rc = 0U;
	int err;

	UNUSED(pool);
	err = (bmap == NULL) ? E_HM_INVAL : E_HM_OK;
	/*
	 * Bitmap
	 * [ --- 1 --- 1 --- 1 --- 0 --- 0 --- 0 --- 1 --- 1 --- 1 --- ]
	 * [			   |		     |		       ]
	 * [			zero idx	   one idx	       ]
	 * If `len <= (one_idx - zero_idx)` condition was matched, then
	 * give the zero bit index as available start position.
	 */
	if (err == E_HM_OK) {
		if (raw_bitmap_test_bit(bmap, sz, start) != 0) {
			zbit_idx = raw_bitmap_find_next_zero(bmap, sz, start);
		}
	}
	while ((err == E_HM_OK) && (end <= sz) && (zbit_idx < sz)) {
		/* Get range of which the size matchs request len length. */
		end = zbit_idx + len;
		if (end > sz) {
			break;
		}
		obit_idx = raw_bitmap_find_next_one(bmap, sz, zbit_idx);
		/* Find available range. */
		if (obit_idx >= end) {
			break;
		}
		zbit_idx = raw_bitmap_find_next_zero(bmap, sz, obit_idx);
	}
	/*
	 * If the searched range end or zero bit index exceeded bitmap size,
	 * just return bitmap size for no-available memory meaning.
	 */
	if ((end > sz) || (err != E_HM_OK)) {
		rc = sz;
	} else {
		rc = zbit_idx;
	}

	return rc;
}

/*
 * find_best algo is implemented based on find_first. The best_first algo is used to
 * find multiple consecutive available memory areas and select the minimum memory area
 * that meets the allocated length.
 */
uint32_t mem_pool_find_best(const struct mem_pool_s *pool,
			    const unsigned long long *bmap,
			    uint32_t sz,
			    uint32_t start,
			    uint32_t len)
{
	int rc;
	uint32_t start_index = sz;
	uint32_t zbit_index = 0;
	uint32_t obit_index = 0;
	uint32_t area_len = sz + 1;

	rc = ((bmap == NULL) || (pool == NULL)) ? E_HM_INVAL : E_HM_OK;
	if (rc == E_HM_OK) {
		zbit_index = mem_pool_find_first(pool, bmap, sz, start, len);
	}

	while((zbit_index < sz) && (rc == E_HM_OK)) {
		obit_index = raw_bitmap_find_next_one(bmap, sz, zbit_index + len - 1);
		if ((obit_index - zbit_index) < area_len) {
			start_index = zbit_index;
			area_len = obit_index - zbit_index;
			if (area_len == len) {
				break;
			}
		}
		zbit_index = (obit_index >= sz) ? sz :
			     mem_pool_find_first(pool, bmap, sz, obit_index + 1, len);
	}

	return start_index;
}

void mem_pool_set_algo(struct mem_pool_s *pool, mempool_algo_t algo)
{
	if ((pool != NULL) && (algo != NULL)) {
		raw_mutex_lock(&pool->mutex);
		pool->algo = algo;
		raw_mutex_unlock(&pool->mutex);
	}
}

static int remove_range(const struct mem_pool_s *pool, struct mem_pool_range_s *range)
{
	uint32_t obit_idx;
	uint32_t sz;
#ifdef CONFIG_MEMPOOL_RANGE_MMAP
	size_t bytes;
#endif
	int rc = E_HM_OK;

	sz = MP_SIZE_TO_COUNT((range->end - range->start), pool->order);
	obit_idx = raw_bitmap_find_first_one(range->bmap, sz);
	/* Ensure that the memory in current range is not used. */
	if (obit_idx < sz) {
		hm_warn("Range is still in use\n");
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		dlist_delete(&range->node);
#ifdef CONFIG_MEMPOOL_RANGE_MMAP
		/* Calculate memory needed by the range. */
		bytes = sizeof(struct mem_pool_range_s) +
			(BITS_TO_LONGLONGS(sz) * sizeof(long long));
		rc = hm_mem_munmap(range, bytes);
		if (rc != E_HM_OK) {
			hm_warn("Failed to munmap: %s\n", hmstrerror(rc));
		}
#else
		free(range);
#endif
	}

	return rc;
}

int mem_pool_destroy(struct mem_pool_s *pool)
{
	struct mem_pool_range_s *curr = NULL;
	struct mem_pool_range_s *next = NULL;
	int rc = E_HM_OK;

	if (pool == NULL) {
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		dlist_for_each_entry_safe(curr, next, &pool->dnode,
					  struct mem_pool_range_s, node) {
			rc = remove_range(pool, curr);
			if (rc != E_HM_OK) {
				hm_warn("Failed to remove range: %s\n", hmstrerror(rc));
				break;
			}
		}
		if (rc == E_HM_OK) {
			free(pool);
		}
	}

	return rc;
}

static void mem_pool_insert_common(struct mem_pool_s *pool, struct mem_pool_range_s *range)
{
	if ((pool != NULL) && (range != NULL)) {
		dlist_insert_head(&pool->dnode, &range->node);
	}
}

/*
 * This function is advised not to be used independently.
 * It's better to be used through the `mem_pool_set_insert_behavior`
 */
void mem_pool_insert_asc_nolock(struct mem_pool_s *pool,
				struct mem_pool_range_s *range)
{
	struct mem_pool_range_s *pos = NULL;

	if ((pool != NULL) && (range != NULL)) {
		dlist_for_each_entry(pos, &pool->dnode, struct mem_pool_range_s, node) {
			if ((range->end - range->start) <= (pos->end - pos->start)) {
				break;
			}
		}
		dlist_insert(dlist_get_prev(&pos->node), &range->node);
	}
}

void mem_pool_set_insert_behavior(struct mem_pool_s *pool,
				  mempool_insert_t func)
{
	if ((pool != NULL) && (func != NULL)) {
		raw_mutex_lock(&pool->mutex);
		pool->insert = func;
		raw_mutex_unlock(&pool->mutex);
	}
}

struct mem_pool_s *mem_pool_create(uint32_t order)
{
	struct mem_pool_s *pool = NULL;

	pool = (struct mem_pool_s *)malloc(sizeof(struct mem_pool_s));
	if (pool != NULL) {
		raw_mutex_init(&pool->mutex);
		dlist_init(&pool->dnode);
		pool->algo = mem_pool_find_first;
		pool->insert = mem_pool_insert_common;
		pool->order = order;
	}

	return pool;
}

static bool range_is_insertable(const struct mem_pool_s *pool,
				uintptr_t va, size_t len)
{
	struct mem_pool_range_s *pos = NULL;
	bool insertable = true;
	uintptr_t end;

	BUG_ON(pool == NULL);
	end = va + len;
	dlist_for_each_entry(pos, &pool->dnode,
			     struct mem_pool_range_s, node) {
		if (((va >= pos->start) && (end <= pos->end)) ||
		    ((va <= pos->start) && (end > pos->start)) ||
		    ((va < pos->end) && (end >= pos->end))) {
			insertable = false;
			break;
		}
	}

	return insertable;
}

int mem_pool_insert_alloc(struct mem_pool_s *pool, uintptr_t va,
			  uint64_t pa, size_t len, size_t alloc_len)
{
	struct mem_pool_range_s *range = NULL;
	uint32_t bm_sz = 0U;
	size_t bytes = 0UL;
	int rc = E_HM_OK;

	rc = (alloc_len > len) ? E_HM_INVAL : E_HM_OK;
	if (rc == E_HM_OK) {
		rc = mem_pool_check_valid(pool, va, len);
	}

	if (rc == E_HM_OK) {
		raw_mutex_lock(&pool->mutex);
		if (!range_is_insertable(pool, va, len)) {
			rc = E_HM_INVAL;
		}
		if (rc == E_HM_OK) {
			/* Size of bitmap. */
			bm_sz = MP_SIZE_TO_COUNT(len, pool->order);
			/* Calculate memory needed by the range. */
			bytes = sizeof(struct mem_pool_range_s) + (BITS_TO_LONGLONGS(bm_sz) * sizeof(long long));
#ifdef CONFIG_MEMPOOL_RANGE_MMAP
			range = (struct mem_pool_range_s *)hm_mem_mmap(NULL, bytes,
								       (uint32_t)PROT_READ | (uint32_t)PROT_WRITE,
								       (uint32_t)MAP_PRIVATE | (uint32_t)MAP_POPULATE |
								       (uint32_t)MAP_ANONYMOUS, -1, 0);
			rc = (range == MAP_FAILED) ? E_HM_NOMEM : E_HM_OK;
#else
			range = (struct mem_pool_range_s *)malloc(bytes);
			rc = (range == NULL) ? E_HM_NOMEM : E_HM_OK;
#endif
		}
		if (rc == E_HM_OK) {
			mem_zero_b(range, bytes);
			range->free = len - alloc_len;
			range->start = va;
			range->end = va + len;
			range->paddr = pa;
			range->bmap = (unsigned long long *)(void *)(range + MP_BITMAP_OFFSET);
			if (alloc_len != 0) {
				rc = do_bitmap_set_range(range->bmap, MP_SIZE_TO_COUNT(len, pool->order),
							 0, MP_SIZE_TO_COUNT(alloc_len, pool->order));
				BUG_ON(rc != 0);
			}
			BUG_ON(pool->insert == NULL);
			pool->insert(pool, range);
		}
		raw_mutex_unlock(&pool->mutex);
	}

	return rc;
}

int mem_pool_insert(struct mem_pool_s *pool, uintptr_t va,
		    uint64_t pa, size_t len)
{
	return mem_pool_insert_alloc(pool, va, pa, len, 0);
}

int mem_pool_remove(struct mem_pool_s *pool, uintptr_t va, size_t len)
{
	struct mem_pool_range_s *curr = NULL;
	struct mem_pool_range_s *next = NULL;
	bool found = false;
	int rc = E_HM_OK;

	rc = mem_pool_check_valid(pool, va, len);
	if (rc == E_HM_OK) {
		raw_mutex_lock(&pool->mutex);
		dlist_for_each_entry_safe(curr, next, &pool->dnode,
					  struct mem_pool_range_s, node) {
			if ((curr->start != va) || (curr->end != (va + len))) {
				continue;
			}
			rc = remove_range(pool, curr);
			if (rc != E_HM_OK) {
				hm_warn("Failed to remove range: %s\n", hmstrerror(rc));
				break;
			}
			found = true;
			break;
		}
		raw_mutex_unlock(&pool->mutex);
	}
	rc = ((rc == E_HM_OK) && !found) ? E_HM_INVAL : rc;

	return rc;
}

static uintptr_t do_mem_pool_alloc(struct mem_pool_s *pool, size_t len)
{
	struct mem_pool_range_s *pos = NULL;
	uint32_t start, need, sz;
	uintptr_t addr = 0UL;
	int rc;

	need = MP_SIZE_TO_COUNT(len, pool->order);
	raw_mutex_lock(&pool->mutex);
	/* Find the chunk which has enough space. */
	dlist_for_each_entry(pos, &pool->dnode,
			     struct mem_pool_range_s, node) {
		if (pos->free < len) {
			continue;
		}
		sz = MP_SIZE_TO_COUNT((pos->end - pos->start), pool->order);
		start = pool->algo(pool, pos->bmap, sz, 0UL, need);
		if (start < sz) {
			rc = do_bitmap_set_range(pos->bmap, sz, start,
						 (start + need));
			if (rc == E_HM_OK) {
				pos->free -= len;
				addr = pos->start + ((uintptr_t)start << pool->order);
				break;
			}
		}
	}
	raw_mutex_unlock(&pool->mutex);

	return addr;
}

uintptr_t mem_pool_alloc(struct mem_pool_s *pool, size_t len)
{
	uintptr_t addr = 0UL;
	int rc;

	rc = mem_pool_check_valid(pool, 0UL, len);
	if ((rc == E_HM_OK) && (pool->algo == NULL)) {
		rc = E_HM_INVAL;
		hm_warn("allocate with invalid pool algorithm.\n");
	}
	if (rc == E_HM_OK) {
		addr = do_mem_pool_alloc(pool, len);
		if (addr == 0UL) {
			rc = E_HM_NOMEM;
		}
	}

	return addr;
}

/* Freeing valid but unallocated range is OK. */
static int do_mem_pool_free(struct mem_pool_s *pool, uintptr_t va, size_t len)
{
	struct mem_pool_range_s *pos = NULL;
	uint32_t start, to_free, sz;
	bool found = false;
	int rc = E_HM_OK;

	to_free = MP_SIZE_TO_COUNT(len, pool->order);
	raw_mutex_lock(&pool->mutex);
	dlist_for_each_entry(pos, &pool->dnode,
			     struct mem_pool_range_s, node) {
		if (!range_is_valid(va, len, pos)) {
			continue;
		}
		sz = MP_SIZE_TO_COUNT((pos->end - pos->start), pool->order);
		start = MP_SIZE_TO_COUNT((va - pos->start), pool->order);
		rc = do_bitmap_clear_range(pos->bmap, sz, start,
					   (start + to_free));
		if (rc != E_HM_OK) {
			break;
		}
		pos->free += len;
		found = true;
		break;
	}
	raw_mutex_unlock(&pool->mutex);
	rc = ((rc == E_HM_OK) && !found) ? E_HM_INVAL : rc;

	return rc;
}

int mem_pool_free(struct mem_pool_s *pool, uintptr_t va, size_t len)
{
	int rc;

	rc = mem_pool_check_valid(pool, va, len);
	if (rc == E_HM_OK) {
		rc = do_mem_pool_free(pool, va, len);
	}

	return rc;
}

int mem_pool_paddr_from_vaddr(struct mem_pool_s *pool,
			      uintptr_t va, uint64_t *pa)
{
	struct mem_pool_range_s *pos = NULL;
	int rc = E_HM_OK;

	if ((pool == NULL) || (pa == NULL)) {
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		rc = E_HM_INVAL;
		raw_mutex_lock(&pool->mutex);
		dlist_for_each_entry(pos, &pool->dnode,
				     struct mem_pool_range_s, node) {
			if (range_is_valid(va, 0UL, pos)) {
				*pa = pos->paddr + (uint64_t)va - (uint64_t)pos->start;
				rc = E_HM_OK;
				break;
			}
		}
		raw_mutex_unlock(&pool->mutex);
	}

	return rc;
}


#if defined(__HOST_LLT__) && defined(CONFIG_MEMPOOL_RANGE_MMAP)
static void *llt_hm_mem_mmap(void *addr, size_t len, uint32_t prot, uint32_t flags, int32_t fd, int64_t offset)
{
	void *rc = NULL;
	(void)addr;
	(void)prot;
	(void)flags;
	(void)fd;
	(void)offset;
	if (len == 0UL) {
		rc = MAP_FAILED;
	} else {
		rc = malloc(len);
		if (rc == NULL) {
			rc = MAP_FAILED;
		}
	}
	return rc;
}

static int llt_hm_mem_munmap(void *addr, size_t len)
{
	(void)len;
	free(addr);
	return E_HM_OK;
}
#endif
