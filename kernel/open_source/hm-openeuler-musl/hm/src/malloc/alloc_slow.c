/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 11 17:57:25 2020
 */

#include "internal.h"

/*
 * Size class generation algorithm:
 * gap <- SMALL_ALIGN
 * cls <- SMALL_ALIGN
 * while: cls <= SMALL_SIZE_MAX
 *     ## skip unnecessary size class ##
 *     if: HEAPMGR_PAGE_SIZE / cls = HEAPMGR_PAGE_SIZE / (cls + gap)
 *         cls <- cls + gap
 *         continue
 *     output: cls
 *     ## increase gap between size classes ##
 *     if: gap < CACHE_LINE_SIZE and cls / gap >= 2
 *         gap <- gap * 2
 *     cls <- cls + gap
 * done
 */

#if SIZE_MAX == UINT_MAX
ALWAYS_INLINE size_t size2bin(size_t size)
{
	static CACHE_ALIGNED const uint8_t bin_class[] = {
		0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5,
		6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 9,
		10, 10, 10, 10, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 12,
		13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
		14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
		14, 14, 14, 14, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
		15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
		15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15
	};
	static_assert(sizeof(bin_class) / sizeof(bin_class[0UL]) == (SMALL_SIZE_MAX + 7UL) / 8UL,
		      "unmatched number of size class table entries");
	size_t i = (size + 7UL) / 8UL - 1UL;
	DCHECK(i * sizeof(uint8_t) < sizeof(bin_class));
	return bin_class[i];
}

static const size_t g_bin_class_sizes[NUM_SMALL_CLASSES] = {
	8, 16, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 384, 512, 672, 1024
};
#else
ALWAYS_INLINE size_t size2bin(size_t size)
{
	static CACHE_ALIGNED const uint8_t bin_class[] = {
		0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5,
		6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8,
		9, 9, 9, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 10, 10, 10,
		11, 11, 11, 11, 11, 11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 12
	};
	static_assert(sizeof(bin_class) / sizeof(bin_class[0UL]) == (SMALL_SIZE_MAX + 15UL) / 16UL,
		      "unmatched number of size class table entries");
	size_t i = (size + 15UL) / 16UL - 1UL;
	DCHECK(i * sizeof(uint8_t) < sizeof(bin_class));
	return bin_class[i];
}

static const size_t g_bin_class_sizes[NUM_SMALL_CLASSES] = {
	16, 32, 64, 128, 192, 256, 320, 384, 512, 640, 768, 896, 1024
};
#endif

static size_t g_max_cached = 0UL;

ALWAYS_INLINE chunk_t *chunk_alloc(void)
{
	chunk_t *c = heapmgr_memory_map(CHUNK_SIZE, CHUNK_SIZE);
	DCHECK(ptr_aligned(c, CHUNK_SIZE));
	if (c == NULL) {
		heapmgr_set_errno(ENOMEM, __func__, __LINE__);
	}
	return c;
}

ALWAYS_INLINE void chunk_init_locked(chunk_t *chunk, heap_t *heap)
{
	DCHECK(ptr_aligned(chunk, CHUNK_SIZE));

	chunk->state = CHUNK_STATE_NORMAL;
	chunk->normal.used = 0U;
	chunk->normal.heap = heap;
	list_insert_before(&heap->chunks, &chunk->normal.list);

	/*
	 * page_t and chunk_t are both PAGE_DESC_SIZE
	 * chunk is actually page[0]
	 */
	unsigned long tmp_addr = ptr_to_ulong(chunk);
	page_t *page = ulong_to_ptr(tmp_addr, page_t);

	/* This one is not used currently */
	page[1].state = 0U;

	/* mark remaining page_t states invalid */
	for (size_t i = META_PAGE_NUM; i < PAGES_PER_CHUNK; ++i) {
		page[i].state = 0U;
	}

	/* construct a free page for this new chunk */
	page_t *first = &page[META_PAGE_NUM];
	first->state = PAGE_STATE_FREE_HEAD;
	first->free_head.npages = DATA_PAGE_NUM;
	page_t *last = &page[PAGES_PER_CHUNK - 1UL];
	last->state = PAGE_STATE_FREE_TAIL;
	last->free_tail.npages = DATA_PAGE_NUM;

	heap_insert_free_page(heap, first);
}

ALWAYS_INLINE void heap_expand_locked(heap_t *heap)
{
	chunk_t *c = NULL;
	if (heap->n_cached > 0UL) {
		/* use a cached chunk which is not munmap'ed */
		DCHECK(heap->n_cached <= g_max_cached);
		heap->n_cached--;
		c = heap->cache[heap->n_cached];
		DCHECK(c != NULL);
		DCHECK(ptr_aligned(c, CHUNK_SIZE));
		heap->cache[heap->n_cached] = NULL;
	} else {
		/*
		 * unlock here is safe because heap states accessed
		 * before unlocking will be re-evaluated after re-locking
		 */
		heapmgr_mutex_unlock((void *)(heap->lock));
		c = chunk_alloc();
		heapmgr_mutex_lock((void *)(heap->lock));
	}
	if (c != NULL) {
		chunk_init_locked(c, heap);
	}
}

/* reduce size of page to npages, and return the remaining rear page */
ALWAYS_INLINE page_t *page_split_locked(page_t *page, size_t npages)
{
	DCHECK(page->state == PAGE_STATE_FREE_HEAD);
	size_t total = page->free_head.npages;
	DCHECK(total > npages);
	page_t *tail = page + (total - 1UL);
	DCHECK(tail->state == PAGE_STATE_FREE_TAIL);
	DCHECK(tail->free_tail.npages == total);

	page->free_head.npages = npages;

	size_t split = total - npages;
	page_t *sp = page + npages;
	DCHECK(split == 1UL || sp->state == 0U);
	DCHECK(split != 1UL || sp->state == PAGE_STATE_FREE_TAIL);
	sp->state = PAGE_STATE_FREE_HEAD;
	sp->free_head.npages = split;

	if (npages > 1UL) {
		page_t *newtail = page + (npages - 1UL);
		DCHECK(newtail->state == 0U);
		newtail->state = PAGE_STATE_FREE_TAIL;
		newtail->free_tail.npages = npages;
	}

	if (split > 1UL) {
		DCHECK(tail->state == PAGE_STATE_FREE_TAIL);
		tail->free_tail.npages = split;
	}

	return sp;
}

static page_t *heap_alloc_page_locked(heap_t *heap, size_t npages)
{
	page_t *page = NULL;
	int rc = 0;

	DCHECK(npages > 0UL);
	DCHECK(npages <= DATA_PAGE_NUM);

	size_t i = bitmap_ffs_from(heap->freemap, npages - 1UL);
	if (i == 0UL) {
		/* no free page that meets requirement, expand heap */
		heap_expand_locked(heap);

		/* check again */
		i = bitmap_ffs_from(heap->freemap, npages - 1UL);
		if (i == 0UL) {
			rc = -1;
		}
	}
	if (rc == 0) {
		DCHECK(i >= npages);
		DCHECK(i <= DATA_PAGE_NUM);
		DCHECK(!list_empty(&heap->free[i - 1UL]));

		page = list_to_page(heap->free[i - 1UL].next);
		DCHECK(page->state == PAGE_STATE_FREE_HEAD);
		heap_remove_free_page(heap, page);
		if (i > npages) {
			/* page is larger than required, split and put back extra part */
			page_t *sp = page_split_locked(page, npages);
			heap_insert_free_page(heap, sp);
		}

		chunk_t *c = page_chunk_of(page);
		DCHECK(c->state == CHUNK_STATE_NORMAL);
		c->normal.used += npages;
		DCHECK(c->normal.used >= npages);
		DCHECK(c->normal.used <= DATA_PAGE_NUM);
	}

	return page;
}

/* put page back to heap's free list */
ALWAYS_INLINE void heap_free_whole_page_locked(heap_t *heap, page_t *page, size_t npages)
{
	page->state = PAGE_STATE_FREE_HEAD;
	page->free_head.npages = npages;
	if (npages > 1UL) {
		page_t *tail = page + (npages - 1UL);
		tail->state = PAGE_STATE_FREE_TAIL;
		tail->free_tail.npages = npages;
	}
	heap_insert_free_page(heap, page);
}

/**
 * reclaim_judge()
 * @npages_new: Page number of new free page list.
 * @npages_old: Page number of page list to be merged by new.
 *
 * Return: 0 - not reclaim.
 *	   positive - reclaim.
 */
ALWAYS_INLINE size_t reclaim_judge(size_t npages_new, size_t npages_old)
{
	size_t new_size = (npages_new + npages_old) * HEAPMGR_PAGE_SIZE;
	size_t cnt = 0UL;
	DCHECK(g_heapmgr_trim_threshold % SYSTEM_PAGE_SIZE == 0UL);
	/*
	 * Because there are at most 126 pages in one chunk, so reclaim_level[]
	 * is 64, 32, 16, 8, 4, 2.
	 * Page reclaim occurs if any of the following conditions is met:
	 * 1. npages_new > npages_old
	 * 2. (npages_old < reclaim_level[i] && npages_new + npages_old > reclaim_level[i])
	 */
	if ((new_size > g_heapmgr_trim_threshold) &&
	    (((npages_new + npages_old) ^ npages_old) > npages_old)) {
		/* at least 1 extra SYSTEM_PAGE for alignment in reclaim_pages() */
		cnt = (new_size - g_heapmgr_trim_threshold) / SYSTEM_PAGE_SIZE;
	}

	return cnt;
}

ALWAYS_INLINE void reclaim_pages(page_t *page, size_t npages)
{
	void *start = page_base_addr_of(page);
	size_t len = npages * HEAPMGR_PAGE_SIZE;

	DCHECK(len >= SYSTEM_PAGE_SIZE + g_heapmgr_trim_threshold);
	heapmgr_memory_trim(start, len, SYSTEM_PAGE_SIZE);
}

/* merge a free page with previous and/or following free pages */
ALWAYS_INLINE page_t *chunk_page_merge_locked(chunk_t *chunk, page_t *page, size_t *npages)
{
	heap_t *heap = chunk->normal.heap;
	page_t *ret = page;
	size_t org_n = *npages;
	size_t idx = (uintptr_t)page % META_PAGE_SIZE / PAGE_DESC_SIZE;
	DCHECK(idx >= META_PAGE_NUM);
	DCHECK(idx + org_n <= PAGES_PER_CHUNK);
	size_t reclaim = 0UL;

	if (idx > META_PAGE_NUM) {
		/* merge with previous page */
		page_t *prev = page - 1UL;
		if (prev->state == PAGE_STATE_FREE_HEAD) {
			/* single free page */
			DCHECK(prev->free_head.npages == 1UL);
			heap_remove_free_page(heap, prev);
			ret = prev;
			reclaim += reclaim_judge(*npages, 1UL);
			(*npages)++;
			page->state = 0U;
		} else if (prev->state == PAGE_STATE_FREE_TAIL) {
			/* contiguous free pages */
			page_t *head = prev - (prev->free_tail.npages - 1UL);
			size_t pn = prev->free_tail.npages;
			DCHECK(head->state == PAGE_STATE_FREE_HEAD);
			DCHECK(head->free_head.npages == pn);
			heap_remove_free_page(heap, head);
			ret = head;
			reclaim += reclaim_judge(*npages, pn);
			*npages += pn;
			prev->state = 0U;
			page->state = 0U;
		}
	}
	if (idx + org_n < PAGES_PER_CHUNK) {
		/* merge with following page */
		page_t *next = page + org_n;
		if (next->state == PAGE_STATE_FREE_HEAD) {
			size_t nn = next->free_head.npages;
			heap_remove_free_page(heap, next);
			reclaim += reclaim_judge(*npages, nn);
			*npages += nn;
			next->state = 0U;
			if (org_n > 1UL) {
				page[org_n - 1UL].state = 0U;
			}
		}
	}

	if ((reclaim != 0UL) && (*npages != DATA_PAGE_NUM)) {
		reclaim_pages(ret, *npages);
	}
	return ret;
}

/* free [page, page + npages) to heap */
static void heap_free_page_locked(heap_t *heap, page_t *page, size_t npages)
{
	chunk_t *c = page_chunk_of(page);
	DCHECK(c->normal.used >= npages);

	size_t curr_n = npages;
	page_t *curr = chunk_page_merge_locked(c, page, &curr_n);

	c->normal.used -= npages;
	if (c->normal.used == 0U) {
		/* whole chunk is free */
		DCHECK(curr_n == DATA_PAGE_NUM);
		if (curr_n > 2UL) {
			DCHECK(page_range_empty(curr + 1UL, curr + (curr_n - 1UL)));
		}
		DCHECK(!list_empty(&c->normal.list));
		list_delete(&c->normal.list);
		if (heap->n_cached < g_max_cached) {
			/* cache this chunk */
			DCHECK(heap->cache[heap->n_cached] == NULL);
			heap->cache[heap->n_cached] = c;
			heap->n_cached++;
		} else {
			/* unlock here is safe because anything related to c will not be used */
			heapmgr_mutex_unlock((void *)(heap->lock));
			heapmgr_memory_unmap(c, CHUNK_SIZE);
			heapmgr_mutex_lock((void *)(heap->lock));
		}
	} else {
		heap_free_whole_page_locked(heap, curr, curr_n);
	}
}

static void *heap_prepare_large_locked_merge(heap_t *heap, page_t *page,
					     size_t npages, size_t size);
/* prepare [page, page + npages) for allocation of size */
static void *heap_prepare_large_locked(heap_t *heap, page_t *page, size_t npages, size_t size)
{
	char *ret = page_base_addr_of(page);
	size_t extra = size % HEAPMGR_PAGE_SIZE;
	page_t *tail = NULL;

	if (npages == 1UL) {
		if (extra == 0UL) {
			page->state = PAGE_STATE_LARGE_HEAD;
			page->large_head.npages = 1UL;
		} else {
			/* single page does not merge but is available for merging */
			DCHECK(size < HEAPMGR_PAGE_SIZE);
			page->state = PAGE_STATE_LARGE_HEAD_MIXED;
			page->large_head_mixed.used0 = 1U;
			page->large_head_mixed.used1 = 0U;
			page->large_head_mixed.size0 = size;
			page->large_head_mixed.npages = 1UL;
		}
	} else if (extra == 0UL) {
		DCHECK(npages > 1UL);
		/* perfect fit, no need to merge */
		page->state = PAGE_STATE_LARGE_HEAD;
		page->large_head.npages = npages;
		tail = page + (npages - 1UL);
		tail->state = PAGE_STATE_LARGE_TAIL;
		tail->large_tail.npages = npages;
	} else {
		DCHECK(npages > 1UL);
		void *merge = heap_prepare_large_locked_merge(heap, page, npages, size);
		if (merge != NULL) {
			ret = merge;
		} else {
			/* merging does not happen, but tail page is available for merging */
			page->state = PAGE_STATE_LARGE_HEAD;
			page->large_head.npages = npages;
			tail = page + (npages - 1UL);
			tail->state = PAGE_STATE_LARGE_TAIL_MIXED;
			tail->large_tail_mixed.used0 = 1U;
			tail->large_tail_mixed.used1 = 0U;
			tail->large_tail_mixed.size0 = extra;
			tail->large_tail_mixed.npages = npages;
		}
	}

	return ret;
}

static void *heap_prepare_large_locked_merge(heap_t *heap, page_t *page,
					     size_t npages, size_t size)
{
	char *base_addr = page_base_addr_of(page);
	size_t extra = size % HEAPMGR_PAGE_SIZE;
	size_t idx = (uintptr_t)page % META_PAGE_SIZE / PAGE_DESC_SIZE;
	void *ret = NULL;

	if (idx > META_PAGE_NUM) {
		/*
		 * Check previous page for possible merging.
		 * Merging can only happen when one page can be saved.
		 */
		page_t *tail = NULL;
		page_t *prev = page - 1UL;
		size_t psz = 0UL;
		bool merge = false;
		if (prev->state == PAGE_STATE_LARGE_HEAD_MIXED) {
			DCHECK(prev->large_head_mixed.used0 != 0U);
			psz = HEAPMGR_PAGE_SIZE - prev->large_head_mixed.size0;
			if (prev->large_head_mixed.used1 == 0U && psz >= extra) {
				merge = true;
				prev->large_head_mixed.used1 = 1U;
			}
		} else if (prev->state == PAGE_STATE_LARGE_TAIL_MIXED) {
			DCHECK(prev->large_tail_mixed.used0 != 0U);
			psz = HEAPMGR_PAGE_SIZE - prev->large_tail_mixed.size0;
			if (prev->large_tail_mixed.used1 == 0U && psz >= extra) {
				merge = true;
				prev->large_tail_mixed.used1 = 1U;
			}
		}
		if (merge) {
			/* This does not recurse because it will not re-enter this branch */
			(void)heap_prepare_large_locked(heap, page, npages - 1UL, size - psz);
			tail = page + (npages - 1UL);
			/* free the extra page saved by merging */
			heap_free_page_locked(heap, tail, 1UL);
			ret = base_addr - psz;
		}
	}

	return ret;
}


ALWAYS_INLINE void *heap_alloc_pages(heap_t *heap, size_t npages, size_t size)
{
	void *r = NULL;
	heapmgr_mutex_lock((void *)(heap->lock));
	page_t *page = heap_alloc_page_locked(heap, npages);
	if (page != NULL) {
		DCHECK(page->state == PAGE_STATE_FREE_HEAD);
		DCHECK(page->free_head.npages == npages);
		if (npages > 1UL) {
			DCHECK(page[npages - 1UL].state == PAGE_STATE_FREE_TAIL);
			DCHECK(page[npages - 1UL].free_tail.npages == npages);
		}
		/* align size up to CACHE_LINE_SIZE to avoid false sharing */
		size_t a_size = (size + CACHE_LINE_SIZE - 1UL) & ~(CACHE_LINE_SIZE - 1UL);
		r = heap_prepare_large_locked(heap, page, npages, a_size);
	}
	heapmgr_mutex_unlock((void *)(heap->lock));
	return r;
}

ALWAYS_INLINE void *alloc_huge_pages(size_t size)
{
	void *r = NULL;
	/* extra CHUNK_DESC_SIZE used to store chunk_t meta data */
	size_t sz = (size + CHUNK_DESC_SIZE + SYSTEM_PAGE_SIZE - 1UL) & ~(SYSTEM_PAGE_SIZE - 1UL);
	chunk_t *c = heapmgr_memory_map(sz, CHUNK_SIZE);
	DCHECK(ptr_aligned(c, CHUNK_SIZE));
	if (c != NULL) {
		c->state = CHUNK_STATE_HUGE;
		c->huge.size = sz;
		c->huge.base = (char *)c + CHUNK_DESC_SIZE;
		r = (void *)c->huge.base;
		__atomic_fetch_add(&g_heapmgr_hugealloc_cnt, 1, __ATOMIC_RELAXED);
		__atomic_fetch_add(&g_heapmgr_hugealloc_size, sz, __ATOMIC_RELAXED);
	} else {
		heapmgr_set_errno(ENOMEM, __func__, __LINE__);
	}

	return r;
}

ALWAYS_INLINE void *alloc_aligned_huge_pages(size_t size_arg, size_t align)
{
	void *ret = NULL;
	size_t size = size_arg;
	size_t alignment = align;

	if (alignment < CHUNK_SIZE) {
		alignment = CHUNK_SIZE;
	}
	size = (size + SYSTEM_PAGE_SIZE - 1UL) & ~(SYSTEM_PAGE_SIZE - 1UL);
	size_t sz = size + alignment;
	/*
	 * can not pass alignment directly to heapmgr_memory_map here because
	 * one extra page is needed for chunk header before the aligned region
	 */
	void *p = heapmgr_memory_map(sz, 0UL);
	DCHECK(ptr_aligned(p, SYSTEM_PAGE_SIZE));
	if (p == NULL) {
		heapmgr_set_errno(ENOMEM, __func__, __LINE__);
	} else {
		uintptr_t begin = (uintptr_t)p;
		uintptr_t end = begin + sz;
		/* extra SYSTEM_PAGE_SIZE used to store chunk_t before aligned pointer */
		uintptr_t addr = (begin + SYSTEM_PAGE_SIZE + alignment - 1UL) & ~(alignment - 1UL);
		DCHECK(begin + SYSTEM_PAGE_SIZE <= addr);
		DCHECK(addr + size <= end);
		if (begin + SYSTEM_PAGE_SIZE < addr) {
			heapmgr_memory_unmap(p, addr - begin - SYSTEM_PAGE_SIZE);
		}
		if (addr + size < end) {
			heapmgr_memory_unmap((void *)(addr + size), end - addr - size);
		}

		chunk_t *c = (chunk_t *)(addr - SYSTEM_PAGE_SIZE);
		c->state = CHUNK_STATE_HUGE_ALIGNED;
		c->huge_aligned.size = size + SYSTEM_PAGE_SIZE;
		c->huge_aligned.base = (void *)addr;
		ret = c->huge_aligned.base;
		__atomic_fetch_add(&g_heapmgr_hugealloc_cnt, 1, __ATOMIC_RELAXED);
		__atomic_fetch_add(&g_heapmgr_hugealloc_size, c->huge_aligned.size, __ATOMIC_RELAXED);
	}

	return ret;
}

void free_huge_pages(void *p)
{
	chunk_t *c = align_ptr(p, CHUNK_SIZE);
	unsigned long tmp_addr;
	if (UNLIKELY(c == p)) {
		tmp_addr = ptr_to_ulong(p);
		tmp_addr -= SYSTEM_PAGE_SIZE;
		c = ulong_to_ptr(tmp_addr, chunk_t);
		DCHECK(c->state == CHUNK_STATE_HUGE_ALIGNED);
		DCHECK(c->huge_aligned.base == p);
		__atomic_fetch_sub(&g_heapmgr_hugealloc_cnt, 1, __ATOMIC_RELAXED);
		__atomic_fetch_sub(&g_heapmgr_hugealloc_size, c->huge_aligned.size, __ATOMIC_RELAXED);
		heapmgr_memory_unmap(c, c->huge_aligned.size);
	} else {
		DCHECK(c->state == CHUNK_STATE_HUGE);
		DCHECK(c->huge.base == p);
		__atomic_fetch_sub(&g_heapmgr_hugealloc_cnt, 1, __ATOMIC_RELAXED);
		__atomic_fetch_sub(&g_heapmgr_hugealloc_size, c->huge_aligned.size, __ATOMIC_RELAXED);
		heapmgr_memory_unmap(c, c->huge.size);
	}
}

void *heap_alloc_large(heap_t *heap, size_t size)
{
	void *r = NULL;

	DCHECK(size > SMALL_SIZE_MAX);
	if (UNLIKELY(size > (size_t)PTRDIFF_MAX)) {
		/*
		 * Refer to
		 * sourceware.org/ml/libc-announce/2019/msg00001.html
		 */
		heapmgr_set_errno(ENOMEM, __func__, __LINE__);
	} else {
		size_t npages = (size + HEAPMGR_PAGE_SIZE - 1UL) / HEAPMGR_PAGE_SIZE;
		if (npages < DATA_PAGE_NUM) {
			r = heap_alloc_pages(heap, npages, size);
		} else {
			r = alloc_huge_pages(size);
		}
	}

	return r;
}

ALWAYS_INLINE void *heap_alloc_small_new_locked(heap_t *heap, size_t cl)
{
	block_t *curr = NULL;
	block_t *next = NULL;

	page_t *page = heap_alloc_page_locked(heap, 1UL);
	if (page != NULL) {
		DCHECK(heap_owns_page(heap, page));
		DCHECK(page->state == PAGE_STATE_FREE_HEAD);
		DCHECK(page->free_head.npages == 1UL);
		DCHECK(list_empty(&page->free_head.list));

		page->state = PAGE_STATE_SMALL;
		page->small.size = g_bin_class_sizes[cl];

		curr = page_base_addr_of(page);
		/* sec: randomize free list order and page_pop_block from list */
		next = (block_t *)((char *)curr + g_bin_class_sizes[cl]);

		page->small.used = 1U;
		page->small.free = next;

		unsigned long tmp_addr = ptr_to_ulong(curr);
		tmp_addr = tmp_addr + HEAPMGR_PAGE_SIZE - (2UL * g_bin_class_sizes[cl]);
		char *end = ulong_to_ptr(tmp_addr, char);
		while ((char *)next <= end) {
			next->next = (block_t *)((char *)next + g_bin_class_sizes[cl]);
			next = next->next;
		}
		next->next = NULL;

		DCHECK(page->small.free != NULL);
		list_insert_before(&heap->bin[cl], &page->small.list);
		size_t d = size2direct(g_bin_class_sizes[cl]);
		DCHECK(d < NUM_DIRECT);
		heap->direct[d] = page;
	}

	return (void *)curr;
}

void *heap_alloc_small_locked(heap_t *heap, size_t size_arg)
{
	size_t size = size_arg;

	if (UNLIKELY(size == 0UL)) {
		/* for compatibility with glibc */
		size = 1UL;
	}

	size_t cl = size2bin(size);
	DCHECK(cl < NUM_SMALL_CLASSES);
	DCHECK(g_bin_class_sizes[cl] >= size);

	list_t *head = &heap->bin[cl];
	list_t *curr = head->next;
	list_t *next = curr->next;
	block_t *b = NULL;
	/*
	 * find a page with empty block in free list
	 * move full page to full list
	 */
	while (curr != head) {
		page_t *page = list_to_page(curr);
		DCHECK(page->state == PAGE_STATE_SMALL);
		DCHECK(g_bin_class_sizes[cl] == page->small.size);
		if (b == NULL && page->small.free != NULL) {
			b = page_pop_block(page);
		}
		if (page->small.free == NULL) {
			heap_move_page_to_full(heap, page);
		} else {
			size_t d = size2direct(size);
			DCHECK(d < NUM_DIRECT);
			heap->direct[d] = page;
			DCHECK(b != NULL);
			break;
		}
		curr = next;
		next = curr->next;
	}
	if (b == NULL) {
		/* no available block in free list, alloc a new page */
		b = heap_alloc_small_new_locked(heap, cl);
	}

	return b;
}

ALWAYS_INLINE page_t *heap_aligned_page_split_locked(heap_t *heap, page_t *page, size_t size, size_t alignment)
{
	uintptr_t start = (uintptr_t)page_base_addr_of(page);
	uintptr_t end = start + page->free_head.npages * HEAPMGR_PAGE_SIZE;
	uintptr_t addr = (start + alignment - 1UL) & ~(alignment - 1UL);
	uintptr_t sz = (size + HEAPMGR_PAGE_SIZE - 1UL) & ~(HEAPMGR_PAGE_SIZE - 1UL);
	DCHECK(addr + sz <= end);

	page_t *curr = page;
	if (addr > start) {
		/* put back front unaligned region */
		size_t npages = (addr - start) / HEAPMGR_PAGE_SIZE;
		page_t *sp = page_split_locked(curr, npages);
		/* prevent merging when free */
		sp->state = 0U;
		heap_free_page_locked(heap, curr, npages);
		sp->state = PAGE_STATE_FREE_HEAD;
		curr = sp;
		DCHECK(page_chunk_of(page)->normal.used >= curr->free_head.npages);
	}
	DCHECK(addr + curr->free_head.npages * HEAPMGR_PAGE_SIZE == end);
	if (addr + sz < end) {
		/* put back rear unaligned region */
		page_t *sp = page_split_locked(curr, sz / HEAPMGR_PAGE_SIZE);
		page_t *back = sp - 1UL;
		size_t back_state = back->state;
		/* prevent merging when free */
		back->state = 0UL;
		heap_free_page_locked(heap, sp, sp->free_head.npages);
		back->state = back_state;
		DCHECK(page_chunk_of(page)->normal.used >= curr->free_head.npages);
	}

	return curr;
}

ALWAYS_INLINE void *heap_aligned_page_alloc(heap_t *heap, size_t size, size_t align)
{
	void *r = NULL;
	size_t npages;
	size_t alignment = align;

	alignment = (alignment < HEAPMGR_PAGE_SIZE) ? HEAPMGR_PAGE_SIZE : alignment;
	npages = (size + alignment - 1UL) / HEAPMGR_PAGE_SIZE;
	if (npages >= DATA_PAGE_NUM) {
		r = alloc_aligned_huge_pages(size, alignment);
	} else {
		heapmgr_mutex_lock((void *)(heap->lock));
		page_t *page = heap_alloc_page_locked(heap, npages);
		if (page != NULL) {
			DCHECK(page->state == PAGE_STATE_FREE_HEAD);
			DCHECK(page->free_head.npages == npages);
			page = heap_aligned_page_split_locked(heap, page, size, alignment);
			DCHECK(page->state == PAGE_STATE_FREE_HEAD);
			DCHECK(page->free_head.npages * HEAPMGR_PAGE_SIZE >= size);
			r = page_base_addr_of(page);
			DCHECK(ptr_aligned(rc, alignment));
			page->state = PAGE_STATE_LARGE_HEAD;
			if (page->large_head.npages > 1UL) {
				page_t *tail = page + (page->large_head.npages - 1UL);
				DCHECK(tail->state == PAGE_STATE_FREE_TAIL);
				DCHECK(tail->free_tail.npages == page->large_head.npages);
				tail->state = PAGE_STATE_LARGE_TAIL;
			}
		}
		heapmgr_mutex_unlock((void *)(heap->lock));
	}

	return r;
}

void *do_heap_aligned_alloc(heap_t *heap, size_t size, size_t align)
{
	void *ret = NULL;
	bool alloced = false;
	size_t alignment = align;

	if (alignment <= SMALL_ALIGN) {
		/* naturally aligned */
		ret = heapmgr_malloc(size);
		alloced = true;
	}
	if (!alloced && (size <= SMALL_SIZE_MAX)) {
		size_t cl = size2bin(size);
		DCHECK(g_bin_class_sizes[cl] >= size);
		if (g_bin_class_sizes[cl] % alignment == 0UL) {
			/* this size class naturally aligned */
			ret = heapmgr_malloc(size);
			alloced = true;
		}
		if (!alloced && (alignment <= SMALL_SIZE_MAX)) {
			while (alignment < size) {
				alignment *= 2UL;
			}
			/* size class for alignment naturally aligned */
			ret = heapmgr_malloc(alignment);
			alloced = true;
		}
	}
	if (!alloced) {
		ret = heap_aligned_page_alloc(heap, size, alignment);
#ifdef STC_MEMTRACE
		if (LIKELY(ret != NULL)) {
			mtrace_record_malloc(ret, size);
		}
#endif
	}

	return ret;
}

void *heap_aligned_alloc(heap_t *heap, size_t size_arg, size_t alignment)
{
	void *ret = NULL;
	size_t size = size_arg;

	if (UNLIKELY((alignment & (alignment - 1UL)) != 0UL)) {
		heapmgr_set_errno(EINVAL, __func__, __LINE__);
	} else {
		if (UNLIKELY(size == 0UL)) {
			/* for compatibility with glibc */
			size = 1UL;
		}
		if (UNLIKELY(size > (size_t)PTRDIFF_MAX)) {
			/*
			 * Refer to
			 * sourceware.org/ml/libc-announce/2019/msg00001.html
			 */
			heapmgr_set_errno(ENOMEM, __func__, __LINE__);
		} else {
			ret = do_heap_aligned_alloc(heap, size, alignment);
		}
	}

	return ret;
}

static void heap_free_large_head_locked(heap_t *heap, page_t *page)
{
	size_t npages = page->large_head.npages;
	if (npages == 1UL) {
		/* free single page */
		heap_free_page_locked(heap, page, 1UL);
	} else {
		DCHECK(npages > 1UL);
		page_t *tail = page + (npages - 1UL);
		if (tail->state == PAGE_STATE_LARGE_TAIL) {
			/* perfect fit case when alloc */
			DCHECK(tail->large_tail.npages == npages);
			heap_free_page_locked(heap, page, npages);
		} else {
			DCHECK(tail->state == PAGE_STATE_LARGE_TAIL_MIXED);
			DCHECK(tail->large_tail_mixed.npages == npages);
			DCHECK(tail->large_tail_mixed.used0 != 0U);
			if (tail->large_tail_mixed.used1 != 0U) {
				/*
				 * Second block of tail page is in use.
				 * Change tail page to mixed head page.
				 * Free the front (npages - 1) pages.
				 */
				heap_free_page_locked(heap, page, npages - 1UL);
				tail->state = PAGE_STATE_LARGE_HEAD_MIXED;
				tail->large_head_mixed.npages = 1UL;
				tail->large_head_mixed.used0 = 0U;
			} else {
				/* free whole [page, page + npages) */
				heap_free_page_locked(heap, page, npages);
			}
		}
	}
}

static void heap_free_large_head_mixed_locked(heap_t *heap, page_t *page, const char *ptr)
{
	/* mixed head is always single page */
	DCHECK(page->large_head_mixed.npages == 1UL);
	char *base = page_base_addr_of(page);
	if (ptr == base) {
		/* free first block */
		DCHECK(page->large_head_mixed.used0 != 0U);
		page->large_head_mixed.used0 = 0U;
		if (page->large_head_mixed.used1 == 0U) {
			heap_free_page_locked(heap, page, 1UL);
		}
	} else {
		/*
		 * Free second block.
		 * Next page should also be freed because single page alloc does not
		 * merge with previous page. Second block from mixed head always has
		 * consecutive pages.
		 */
		DCHECK(ptr == base + page->large_head_mixed.size0);
		DCHECK(page->large_head_mixed.used1 != 0U);
		page->large_head_mixed.used1 = 0U;
		page_t *next = page + 1UL;
		if (page->large_head_mixed.used0 == 0U) {
			/* first blocked has already been freed */
			heap_free_page_locked(heap, page, 1UL);
		}
		if (next->state == PAGE_STATE_LARGE_HEAD) {
			heap_free_large_head_locked(heap, next);
		} else {
			DCHECK(next->state == PAGE_STATE_LARGE_HEAD_MIXED);
			/* This does not recurse because it will not re-enter this branch */
			heap_free_large_head_mixed_locked(heap, next, page_base_addr_of(next));
		}
	}
}

ALWAYS_INLINE void heap_free_large_tail_mixed_locked(heap_t *heap, page_t *page)
{
	DCHECK(page->large_tail_mixed.used0 != 0U);
	DCHECK(page->large_tail_mixed.used1 != 0U);
	DCHECK((page - (page->large_tail_mixed.npages - 1UL))->state ==
	       PAGE_STATE_LARGE_HEAD);
	DCHECK((page - (page->large_tail_mixed.npages - 1UL))->large_head.npages ==
	       page->large_tail_mixed.npages);
	page->large_tail_mixed.used1 = 0U;

	/*
	 * Next page should also be freed because single page alloc does not
	 * merge with previous page. Second block from mixed tail always has
	 * consecutive pages.
	 */
	page_t *next = page + 1UL;
	if (next->state == PAGE_STATE_LARGE_HEAD) {
		heap_free_large_head_locked(heap, next);
	} else {
		DCHECK(next->state == PAGE_STATE_LARGE_HEAD_MIXED);
		heap_free_large_head_mixed_locked(heap, next, page_base_addr_of(next));
	}
}

void heap_free_large_locked(heap_t *heap, void *p)
{
	chunk_t *c = align_ptr(p, CHUNK_SIZE);
	DCHECK(c->state == CHUNK_STATE_NORMAL);
	DCHECK(c->normal.heap == heap);
	DCHECK((char *)p < (char *)c + CHUNK_SIZE);
	size_t idx = ((uintptr_t)p % CHUNK_SIZE) / HEAPMGR_PAGE_SIZE;
	DCHECK(idx >= META_PAGE_NUM);
	page_t *page = NULL;
	unsigned long tmp_addr;

	tmp_addr = ptr_to_ulong(c);
	tmp_addr += (idx * PAGE_DESC_SIZE);
	page = ulong_to_ptr(tmp_addr, page_t);

	/* valid large alloc'ed pointer may reside in large head, mixed head or mixed tail */
	if (page->state == PAGE_STATE_LARGE_HEAD) {
		DCHECK(p == page_base_addr_of(page));
		heap_free_large_head_locked(heap, page);
	} else if (page->state == PAGE_STATE_LARGE_TAIL_MIXED) {
		/* only the second block is valid to free directly */
		DCHECK((char *)p == (char *)page_base_addr_of(page) +
				    page->large_tail_mixed.size0);
		heap_free_large_tail_mixed_locked(heap, page);
	} else {
		DCHECK(page->state == PAGE_STATE_LARGE_HEAD_MIXED);
		heap_free_large_head_mixed_locked(heap, page, p);
	}
}

/* clear heap->direct cache for page */
ALWAYS_INLINE void heap_free_direct_page_locked(heap_t *heap, page_t *page)
{
	size_t cl = size2bin(page->small.size);
	DCHECK(g_bin_class_sizes[cl] == page->small.size);
	if (cl == 0UL) {
		if (heap->direct[1UL] == page) {
			heap->direct[1UL] = NULL;
		}
	} else {
		size_t d = size2direct(page->small.size);
		for (size_t i = size2direct(g_bin_class_sizes[cl - 1UL]) + 1UL; i <= d; ++i) {
			if (heap->direct[i] == page) {
				heap->direct[i] = NULL;
			}
		}
	}
}

void heap_free_small_locked(heap_t *heap, page_t *page, block_t *block)
{
	DCHECK(heap_owns_page(heap, page));
	DCHECK(page->state == PAGE_STATE_SMALL);
	DCHECK(!list_empty(&page->small.list));

	if (page->small.used == 1U) {
		if (page->small.list.next == page->small.list.prev) {
			/* the only page in free list, keep it for performance */
			page_push_block(page, block);
		} else {
			/* otherwise free this page */
			list_delete(&page->small.list);
			heap_free_direct_page_locked(heap, page);
			heap_free_page_locked(heap, page, 1UL);
		}
	} else {
		/* move page from full list to the tail of free list */
		DCHECK(page->small.used == HEAPMGR_PAGE_SIZE / page->small.size);
		page_push_block(page, block);
		list_delete(&page->small.list);
		size_t cl = size2bin(page->small.size);
		DCHECK(g_bin_class_sizes[cl] == page->small.size);
		list_insert_before(&heap->bin[cl], &page->small.list);
	}
}

size_t large_page_usable_size(page_t *pg, char *pointer)
{
	size_t sz;
	size_t npages;
	page_t *page = pg;
	char *ptr = pointer;
	char *base = page_base_addr_of(page);

	if (page->state == PAGE_STATE_LARGE_HEAD) {
		DCHECK(ptr == base);
		npages = page->large_head.npages;
		sz = npages * HEAPMGR_PAGE_SIZE;
		if (npages != 1UL) {
			page_t *tail = page + (npages - 1UL);
			if (tail->state == PAGE_STATE_LARGE_TAIL) {
				/* perfect fit multiple pages */
				DCHECK(tail->large_tail.npages == npages);
			} else {
				/* tail page is shared */
				DCHECK(tail->state == PAGE_STATE_LARGE_TAIL_MIXED);
				DCHECK(tail->large_tail_mixed.used0 == 1U);
				DCHECK(tail->large_tail_mixed.npages == npages);
				sz -= (HEAPMGR_PAGE_SIZE - tail->large_tail_mixed.size0);
			}
		}
	} else if (page->state == PAGE_STATE_LARGE_TAIL_MIXED) {
		/* valid pointer only from second block */
		npages = page->large_tail_mixed.npages;
		MAY_UNUSED(npages);
		DCHECK((page - (npages - 1UL))->state == PAGE_STATE_LARGE_HEAD);
		DCHECK((page - (npages - 1UL))->large_head.npages == npages);
		DCHECK(page->large_tail_mixed.used1 != 0U);
		DCHECK(ptr == base + page->large_tail_mixed.size0);
		sz = HEAPMGR_PAGE_SIZE - page->large_tail_mixed.size0;
		page = page + 1UL;
		ptr += sz;
		/* This does not recurse because it will not re-enter this branch */
		sz += large_page_usable_size(page, ptr);
	} else {
		DCHECK(page->state == PAGE_STATE_LARGE_HEAD_MIXED);
		DCHECK(page->large_head_mixed.npages == 1UL);
		if (ptr == base) {
			/* pointer from first block */
			DCHECK(page->large_head_mixed.used0 != 0U);
			sz = page->large_head_mixed.size0;
		} else {
			/* pointer from second block */
			DCHECK(page->large_head_mixed.used1 != 0U);
			DCHECK(ptr == base + page->large_head_mixed.size0);
			sz = HEAPMGR_PAGE_SIZE - page->large_head_mixed.size0;
			page = page + 1UL;
			ptr += sz;
			/* This does not recurse because it will not re-enter this branch */
			sz += large_page_usable_size(page, ptr);
		}
	}

	return sz;
}

ALWAYS_INLINE void init_heap(heap_t *heap)
{
	for (size_t i = 0UL; i < NUM_SMALL_CLASSES; ++i) {
		list_init(&heap->bin[i]);
	}
	list_init(&heap->full);
	for (size_t i = 0UL; i < DATA_PAGE_NUM; ++i) {
		list_init(&heap->free[i]);
	}
	list_init(&heap->chunks);
	/* other fields are left zero */
}

/* reduce number of cached chunks in heap */
static void heap_clear_cache_locked(heap_t *heap, size_t to)
{
	while (heap->n_cached > to) {
		heap->n_cached--;
		chunk_t *c = heap->cache[heap->n_cached];
		heap->cache[heap->n_cached] = NULL;
		DCHECK(c != NULL);
		heapmgr_memory_unmap(c, CHUNK_SIZE);
	}
}

void heap_shrink_locked(heap_t *heap)
{
	/* release empty page that has been kept in free list */
	for (size_t i = 0UL; i < NUM_SMALL_CLASSES; ++i) {
		if (list_empty(&heap->bin[i])) {
			continue;
		}
		if (heap->bin[i].next != heap->bin[i].prev) {
			continue;
		}
		page_t *page = list_to_page(heap->bin[i].next);
		DCHECK(page->state == PAGE_STATE_SMALL);
		if (page->small.used > 0U) {
			continue;
		}
		list_delete(&page->small.list);
		heap_free_direct_page_locked(heap, page);
		heap_free_page_locked(heap, page, 1UL);
	}

	/* release all cached chunks */
	heap_clear_cache_locked(heap, 0UL);
}

ALWAYS_INLINE void print_page_large_head(const page_t *page, FILE *f, size_t i, size_t cd)
{
	const page_t *tail = page + cd;

	if (cd == 0UL) {
		heapmgr_fprintf(f, "  large page #%zu\n", i);
	} else if (tail->state == PAGE_STATE_LARGE_TAIL) {
		DCHECK(tail->large_tail.npages == cd + 1UL);
		heapmgr_fprintf(f, "  large pages #%zu -- %zu\n", i, i + cd);
	} else {
		DCHECK(tail->state == PAGE_STATE_LARGE_TAIL_MIXED);
		DCHECK(tail->large_tail_mixed.npages == cd + 1UL);
		DCHECK(tail->large_tail_mixed.used0 != 0U);
		size_t sz0 = tail->large_tail_mixed.size0;
		size_t sz1 = HEAPMGR_PAGE_SIZE - sz0;
		const char *s = (tail->large_tail_mixed.used1 == 0U) ? "free" : "used";
		heapmgr_fprintf(f, "  large pages #%zu -- %zu (tail: %zu used, %zu %s)\n",
				i, i + cd, sz0, sz1, s);
	}
}

ALWAYS_INLINE void print_page_large_head_mixed(const page_t *page, FILE *f, size_t i)
{
	DCHECK(page->large_head_mixed.npages == 1U);
	DCHECK(page->large_head_mixed.used0 != 0U ||
	       page->large_head_mixed.used1 != 0U);
	size_t sz0 = page->large_head_mixed.size0;
	size_t sz1 = HEAPMGR_PAGE_SIZE - sz0;
	const char *s0 = (page->large_head_mixed.used0 == 0U) ? "free" : "used";
	const char *s1 = (page->large_head_mixed.used1 == 0U) ? "free" : "used";
	heapmgr_fprintf(f, "  mixed page #%zu (%zu %s, %zu %s)\n",
			i, sz0, s0, sz1, s1);
}

ALWAYS_INLINE void print_page_small(const page_t *page, FILE *f, size_t i)
{
	DCHECK(page->state == PAGE_STATE_SMALL);
	size_t total_blocks = HEAPMGR_PAGE_SIZE / page->small.size;
	heapmgr_fprintf(f, "  small page #%zu, block size %zu, %zu of %zu blocks used\n",
			i, (size_t)page->small.size, (size_t)page->small.used, total_blocks);
}

ALWAYS_INLINE void chunk_print_state_locked(chunk_t *chunk, FILE *f)
{
	DCHECK(chunk->normal.used <= DATA_PAGE_NUM);
	size_t total_pages = DATA_PAGE_NUM;
	heapmgr_fprintf(f, "  %zu of %zu pages used\n", (size_t)chunk->normal.used, total_pages);

	size_t cd = 0UL;
	for (size_t i = META_PAGE_NUM; i < PAGES_PER_CHUNK; ++i) {
		unsigned long tmp_addr = ptr_to_ulong(chunk);
		tmp_addr += (i * PAGE_DESC_SIZE);
		page_t *page = ulong_to_ptr(tmp_addr, page_t);
		if (cd > 0UL) {
			/* skip internal pages */
			cd--;
			if (cd > 0UL) {
				DCHECK(page->state == 0U);
			}
		} else if (page->state == PAGE_STATE_FREE_HEAD) {
			DCHECK(page->free_head.npages > 0UL);
			cd = page->free_head.npages - 1UL;
			if (cd > 0UL) {
				DCHECK(page[cd].state == PAGE_STATE_FREE_TAIL);
				DCHECK(page[cd].free_tail.npages == cd + 1);
				heapmgr_fprintf(f, "  free pages #%zu -- %zu\n", i, i + cd);
			} else {
				heapmgr_fprintf(f, "  free page #%zu\n", i);
			}
		} else if (page->state == PAGE_STATE_LARGE_HEAD) {
			DCHECK(page->large_head.npages > 0UL);
			cd = page->large_head.npages - 1UL;
			print_page_large_head(page, f, i, cd);
		} else if (page->state == PAGE_STATE_LARGE_HEAD_MIXED) {
			print_page_large_head_mixed(page, f, i);
		} else {
			print_page_small(page, f, i);
		}
	}
}

void heap_print_state_locked(heap_t *heap, FILE *f)
{
	list_t *curr = heap->chunks.next;
	size_t num_chunks = 0UL;
	while (curr != &heap->chunks) {
		chunk_t *c = list_to_chunk(curr);
		DCHECK(c->state == CHUNK_STATE_NORMAL);
		DCHECK(c->normal.heap == heap);
		heapmgr_fprintf(f, "----- Chunk #%zu -----\n", num_chunks);
		chunk_print_state_locked(c, f);
		num_chunks++;
		curr = curr->next;
	}
	for (size_t i = 0UL; i < heap->n_cached; ++i) {
		size_t sz_kb = CHUNK_SIZE / 1024UL;
		heapmgr_fprintf(f, "----- Cached chunk #%zu size %zu KB -----\n", i, sz_kb);
	}
}

ALWAYS_INLINE void dump_page_large_head_mixed(const page_t *page, heapmgr_state_t *s)
{
	DCHECK(page->large_head_mixed.npages == 1UL);
	DCHECK(page->large_head_mixed.used0 != 0U ||
	       page->large_head_mixed.used1 != 0U);
	s->used += HEAPMGR_PAGE_SIZE;
	if (page->large_head_mixed.used0 != 0U) {
		s->allocated += page->large_head_mixed.size0;
	}
	if (page->large_head_mixed.used1 != 0U) {
		s->allocated += HEAPMGR_PAGE_SIZE - page->large_head_mixed.size0;
	}
}

ALWAYS_INLINE void dump_page_small(const page_t *page, heapmgr_state_t *s)
{
	size_t total_blocks = HEAPMGR_PAGE_SIZE / page->small.size;
	DCHECK(page->state == PAGE_STATE_SMALL);
	DCHECK(page->small.used <= total_blocks);
	size_t frag = HEAPMGR_PAGE_SIZE - total_blocks * page->small.size;
	s->allocated += (size_t)((unsigned int)page->small.used * (unsigned int)page->small.size);
	s->used += (size_t)((unsigned int)page->small.used * (unsigned int)page->small.size) + frag;
}

ALWAYS_INLINE void chunk_dump_state_locked(chunk_t *chunk, heapmgr_state_t *s)
{
	DCHECK(chunk->normal.used <= DATA_PAGE_NUM);
	s->total += CHUNK_SIZE;
	/* meta data page */
	s->used += META_PAGE_SIZE;

	size_t nr_skip = 0UL;
	for (size_t i = META_PAGE_NUM; i < PAGES_PER_CHUNK; ++i) {
		unsigned long tmp_addr = ptr_to_ulong(chunk);
		tmp_addr += (i * PAGE_DESC_SIZE);
		page_t *page = ulong_to_ptr(tmp_addr, page_t);
		if (nr_skip > 0UL) {
			/* skip internal pages */
			nr_skip--;
			if (nr_skip > 0UL) {
				DCHECK(page->state == 0U);
			}
		} else if (page->state == PAGE_STATE_FREE_HEAD) {
			DCHECK(page->free_head.npages > 0UL);
			nr_skip = page->free_head.npages - 1UL;
			if (nr_skip > 0UL) {
				DCHECK(page[nr_skip].state == PAGE_STATE_FREE_TAIL);
				DCHECK(page[nr_skip].free_tail.npages == nr_skip + 1UL);
			}
		} else if (page->state == PAGE_STATE_LARGE_HEAD) {
			DCHECK(page->large_head.npages > 0UL);
			nr_skip = page->large_head.npages - 1UL;
			s->allocated += page->large_head.npages * HEAPMGR_PAGE_SIZE;
			s->used += page->large_head.npages * HEAPMGR_PAGE_SIZE;
			if (nr_skip == 0UL) {
				continue;
			}
			page_t *tail = page + nr_skip;
			if (tail->state == PAGE_STATE_LARGE_TAIL) {
				DCHECK(tail->large_tail.npages == nr_skip + 1UL);
				continue;
			}
			DCHECK(tail->state == PAGE_STATE_LARGE_TAIL_MIXED);
			DCHECK(tail->large_tail_mixed.npages == nr_skip + 1UL);
			DCHECK(tail->large_tail_mixed.used0 != 0U);
			if (tail->large_tail_mixed.used1 == 0U) {
				s->allocated -= HEAPMGR_PAGE_SIZE - tail->large_tail_mixed.size0;
			}
		} else if (page->state == PAGE_STATE_LARGE_HEAD_MIXED) {
			dump_page_large_head_mixed(page, s);
		} else {
			dump_page_small(page, s);
		}
	}
}

void heap_dump_state_locked(heap_t *heap, heapmgr_state_t *s)
{
	list_t *curr = heap->chunks.next;
	while (curr != &heap->chunks) {
		chunk_t *c = list_to_chunk(curr);
		DCHECK(c->state == CHUNK_STATE_NORMAL);
		DCHECK(c->normal.heap == heap);
		chunk_dump_state_locked(c, s);
		curr = curr->next;
	}
	for (size_t i = 0UL; i < heap->n_cached; ++i) {
		s->total += CHUNK_SIZE;
		s->cached += CHUNK_SIZE;
	}
}

lock_t g_heapmgr_set_heaps_lock;

int set_num_heaps(size_t num)
{
	int rc;

	DCHECK(num > 0UL);
	DCHECK(num <= HEAP_NUM_MAX);
	heapmgr_mutex_lock((void *)(g_heapmgr_set_heaps_lock));
	DCHECK(g_heapmgr_num_heaps >= 1UL);
	DCHECK(g_heapmgr_num_heaps <= HEAP_NUM_MAX);

	/* number of heaps can only increase */
	if (num < g_heapmgr_num_heaps) {
		rc = -1;
	} else if (num == g_heapmgr_num_heaps) {
		rc = 0;
	} else {
		size_t newheaps = num - g_heapmgr_num_heaps;
		size_t heap_sz = (sizeof(heap_t) + CACHE_LINE_SIZE - 1UL) & ~(CACHE_LINE_SIZE - 1UL);
		size_t total_sz = heap_sz * newheaps;
		char *mem = heapmgr_memory_map(total_sz, 0UL);
		if (mem != NULL) {
			for (size_t i = g_heapmgr_num_heaps; i < num; ++i) {
				unsigned long tmp_addr = ptr_to_ulong(mem);
				heap_t *heap = ulong_to_ptr(tmp_addr, heap_t);
				mem += heap_sz;
				init_heap(heap);
				g_heapmgr_heaps[i] = heap;
			}

			/*
			 * Read of g_heapmgr_num_heaps in select_heap is lock-free.
			 * Make write to it sequentially consistent so that access to
			 * g_heapmgr_heaps in select_heap is always valid.
			 */
			__atomic_thread_fence(__ATOMIC_SEQ_CST);
			g_heapmgr_num_heaps = num;
			rc = 0;
		} else {
			rc = -1;
		}
	}
	heapmgr_mutex_unlock((void *)(g_heapmgr_set_heaps_lock));

	return rc;
}

void set_num_cached(size_t num)
{
	DCHECK(num <= CACHED_NUM_MAX);
	DCHECK(g_max_cached <= CACHED_NUM_MAX);

	heapmgr_mutex_lock((void *)(g_heapmgr_set_heaps_lock));
	if (num >= g_max_cached) {
		g_max_cached = num;
	} else {
		DCHECK(g_heapmgr_num_heaps >= 1UL);
		DCHECK(g_heapmgr_num_heaps <= HEAP_NUM_MAX);

		/* number of cached chunks may decrease, fully lock before changing it */
		for (size_t i = 0UL; i < g_heapmgr_num_heaps; ++i) {
			heapmgr_mutex_lock((void *)(g_heapmgr_heaps[i]->lock));
		}
		for (size_t i = 0UL; i < g_heapmgr_num_heaps; ++i) {
			heap_clear_cache_locked(g_heapmgr_heaps[i], num);
		}
		g_max_cached = num;
		for (size_t i = 0UL; i < g_heapmgr_num_heaps; ++i) {
			heapmgr_mutex_unlock((void *)(g_heapmgr_heaps[i]->lock));
		}
	}
	heapmgr_mutex_unlock((void *)(g_heapmgr_set_heaps_lock));
}

void set_trim_threshold(size_t n)
{
	heapmgr_mutex_lock((void *)(g_heapmgr_set_heaps_lock));
	g_heapmgr_trim_threshold = align_up(n, SYSTEM_PAGE_SIZE);
	heapmgr_mutex_unlock((void *)(g_heapmgr_set_heaps_lock));
}
