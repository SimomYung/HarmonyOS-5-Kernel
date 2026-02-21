/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Fri Apr 9 15:25:05 2020
 */

#include <malloc.h>
#include "internal.h"

static void page_block_stats(page_t *page, size_t *blk_cnt, size_t *blk_size)
{
	DCHECK(page->state == PAGE_STATE_SMALL);
	DCHECK(blk_cnt != NULL);
	DCHECK(blk_size != NULL);
	block_t *blk = page->small.free;
	while(page_block_valid(page, blk)) {
		*blk_cnt += 1;
		*blk_size += page->small.size;
		if (blk == NULL) {
			break;
		}
		blk = blk->next;
	}
}

static void heap_fast_blks_locked(heap_t *heap, size_t *fast_blk_cnt, size_t *fast_blk_size)
{
	DCHECK(heap != NULL);
	DCHECK(fast_blk_cnt != NULL);
	DCHECK(fast_blk_size != NULL);
	for (size_t i = 0UL; i < NUM_DIRECT; i++) {
		page_t *page = heap->direct[i];
		if (page == NULL) {
			continue;
		}
		page_block_stats(page, fast_blk_cnt, fast_blk_size);
	}
}

static size_t heap_small_blks_locked(heap_t *heap)
{
	DCHECK(heap != NULL);
	size_t blk_cnt = 0UL;
	size_t blk_size = 0UL;
	for (size_t i = 0UL; i < NUM_SMALL_CLASSES; i++) {
		list_t *head = &heap->bin[i];
		list_t *curr = head->next;
		while (curr != head) {
			page_t *page = list_to_page(curr);
			page_block_stats(page, &blk_cnt, &blk_size);
			curr = curr->next;
		}
	}
	return blk_cnt;
}

static size_t chunk_free_size_locked(chunk_t *chunk)
{
	size_t size = 0UL;
	DCHECK(chunk->normal.used <= DATA_PAGE_NUM);

	size_t skip = 0UL;
	for (size_t i = META_PAGE_NUM; i < PAGES_PER_CHUNK; ++i) {
		page_t *page = (page_t *)((char *)chunk + i * PAGE_DESC_SIZE);
		if (skip > 0UL) {
			/* skip internal pages */
			skip--;
			if (skip > 0UL) {
				DCHECK(page->state == 0U);
			}
		} else if (page->state == PAGE_STATE_FREE_HEAD) {
			DCHECK(page->free_head.npages > 0UL);
			skip = page->free_head.npages - 1UL;
			if (skip > 0UL) {
				DCHECK(page[skip].state == PAGE_STATE_FREE_TAIL);
				DCHECK(page[skip].free_tail.npages == skip + 1UL);
			}
			size += page[skip].free_tail.npages * HEAPMGR_PAGE_SIZE;
		} else if (page->state == PAGE_STATE_SMALL) {
			size_t total_blocks = HEAPMGR_PAGE_SIZE / page->small.size;
			DCHECK(page->small.used <= total_blocks);
			size += (total_blocks - page->small.used) * page->small.size;
		} else {
			continue;
		}
	}
	return size;
}

static size_t heap_free_size_locked(heap_t *heap)
{
	DCHECK(heap != NULL);
	size_t size = 0UL;
	list_t *curr = heap->chunks.next;
	while (curr != &heap->chunks) {
		chunk_t *c = list_to_chunk(curr);
		DCHECK(c->state == CHUNK_STATE_NORMAL);
		size += chunk_free_size_locked(c);
		DCHECK(c->normal.heap == heap);
		curr = curr->next;
	}
	size += heap->n_cached * (CHUNK_SIZE - META_PAGE_SIZE);
	return size;
}

static void heapmgr_blks_stats(size_t *fast_blk_cnt, size_t *fast_blk_size,
			       size_t *small_blk_cnt, size_t *total_free_size)
{
	DCHECK(fast_blk_cnt != NULL);
	DCHECK(fast_blk_size != NULL);
	DCHECK(small_blk_cnt != NULL);
	DCHECK(total_free_size != NULL);
	heapmgr_mutex_lock((void *)(g_heapmgr_set_heaps_lock));
	for (size_t i = 0UL; i < g_heapmgr_num_heaps; ++i) {
		heapmgr_mutex_lock((void *)(g_heapmgr_heaps[i]->lock));
		heap_fast_blks_locked(g_heapmgr_heaps[i], fast_blk_cnt, fast_blk_size);
		*small_blk_cnt += heap_small_blks_locked(g_heapmgr_heaps[i]);
		*total_free_size += heap_free_size_locked(g_heapmgr_heaps[i]);
		heapmgr_mutex_unlock((void *)(g_heapmgr_heaps[i]->lock));
	}
	heapmgr_mutex_unlock((void *)(g_heapmgr_set_heaps_lock));
}

struct mallinfo heapmgr_mallinfo(void)
{
	heapmgr_state_t s;
	heapmgr_dump_state(0, &s);

	size_t fast_blk_cnt = 0UL;
	size_t fast_blk_size = 0UL;
	size_t small_blk_cnt = 0UL;
	size_t total_free_size = 0UL;
	heapmgr_blks_stats(&fast_blk_cnt, &fast_blk_size, &small_blk_cnt, &total_free_size);

	struct mallinfo m;
	m.arena = s.total;
	m.ordblks = small_blk_cnt;
	m.smblks = fast_blk_cnt;
	m.hblks = g_heapmgr_hugealloc_cnt;
	m.hblkhd = g_heapmgr_hugealloc_size;
	m.usmblks = 0; /* This field is unused */
	m.fsmblks = fast_blk_size;
	m.uordblks = s.used;
	m.fordblks = total_free_size;
	m.keepcost = s.total + s.cached;

	return m;
}

WEAK_ALIAS(heapmgr_mallinfo, mallinfo);
