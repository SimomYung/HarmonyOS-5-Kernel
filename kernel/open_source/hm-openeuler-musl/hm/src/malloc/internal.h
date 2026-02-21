/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Tue Feb 11 11:50:30 2020
 */

#ifndef HEAPMGR_INTERNAL_H
#define HEAPMGR_INTERNAL_H

#include <heapmgr.h>

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "atomic.h"
#include "memtrace.h"
#include "pthread_impl.h"

/* basic assumptions */
static_assert(sizeof(size_t) == sizeof(void *), "size mismatch");
#if SIZE_MAX == UINT_MAX
static_assert(sizeof(void *) == 4, "invalid pointer size");
#else
static_assert(sizeof(void *) == 8, "invalid pointer size");
#endif
static_assert(PTRDIFF_MAX <= SIZE_MAX / 2, "invalid PTRDIFF_MAX and SIZE_MAX");

/*
 * Current heap data structure layout:
 *   +--- ...
 *   | chunk (256K & 256K aligned)
 *   +--------------------------+--------------------------+
 *   |      heap page (2K)      |      heap page (2K)      |
 *   +-----------------------------------------------------+--- ...
 *   |                    meta data (4K)                   | user data (252K)
 *   +-----------------------------------------------------+--- ...
 *   | chunk_t | page_t  | page_t  |
 *   +-----------------------------+ ...
 *   |<- 32B ->|   ^
 *                 +-- This one is currently unused.
 */

/* basic definitions */
#define CACHE_LINE_SIZE 64UL
#define SYSTEM_PAGE_SIZE 4096UL
/* internal heapmgr page size is 2048 to reduce fragmentation */
#define HEAPMGR_PAGE_SIZE 2048UL
/* natural alignment of alloc'ed pointer */
#define SMALL_ALIGN (2UL * sizeof(size_t))

#if SIZE_MAX == UINT_MAX
/* chunk size and its natural alignment is 256K */
#define CHUNK_SHIFT 7UL
/* page meta data uses 4096 bytes */
#define META_PAGE_NUM 2UL
#define NUM_SMALL_CLASSES 16UL
#else
/* make chunk size 512K on 64-bit platform */
#define CHUNK_SHIFT 8UL
#define META_PAGE_NUM 4UL
#define NUM_SMALL_CLASSES 13UL
#endif

#define CHUNK_SIZE (HEAPMGR_PAGE_SIZE << CHUNK_SHIFT)
#define PAGES_PER_CHUNK (CHUNK_SIZE / HEAPMGR_PAGE_SIZE)
/* remaining pages are for allocation */
#define DATA_PAGE_NUM (PAGES_PER_CHUNK - META_PAGE_NUM)
#define META_PAGE_SIZE (HEAPMGR_PAGE_SIZE * META_PAGE_NUM)
/* meta data size for one page */
#define PAGE_DESC_SIZE (META_PAGE_SIZE / PAGES_PER_CHUNK)
/* chunk meta data has the same size */
#define CHUNK_DESC_SIZE PAGE_DESC_SIZE
/* number of unsigned long longs required for bitmap of page free list */
#define NUM_BITMAP_ULLS (size_t)((DATA_PAGE_NUM + 63) / 64)
/* maximum size that goes to small alloc */
#define SMALL_SIZE_MAX 1024UL
/* number of cached pages for small alloc fastpath */
#define NUM_DIRECT (SMALL_SIZE_MAX / SMALL_ALIGN + 1)

#define DEFAULT_TRIM_THRESHOLD (SYSTEM_PAGE_SIZE * 30)

static_assert(CHUNK_SIZE == HEAPMGR_CHUNK_ALIGNMENT, "invalid CHUNK_SIZE");

#define HEAP_NUM_MAX 32
#define CACHED_NUM_MAX 8

#define CHUNK_STATE_NORMAL 0x11U
#define CHUNK_STATE_HUGE 0x12U
#define CHUNK_STATE_HUGE_ALIGNED 0x13U
#define PAGE_STATE_FREE_HEAD 0x21U
#define PAGE_STATE_FREE_TAIL 0x22U
#define PAGE_STATE_SMALL 0x23U
#define PAGE_STATE_LARGE_HEAD 0x24U
#define PAGE_STATE_LARGE_TAIL 0x25U
#define PAGE_STATE_LARGE_HEAD_MIXED 0x26U
#define PAGE_STATE_LARGE_TAIL_MIXED 0x27U

/* attribute abbreviations */
#ifndef DEBUG
#define ALWAYS_INLINE __attribute__((always_inline)) static inline
#else
#define ALWAYS_INLINE static inline
#endif
#define CACHE_ALIGNED __attribute__((aligned(CACHE_LINE_SIZE)))
#define WEAK_ALIAS(name, alias) \
	extern __typeof(name) alias __attribute__((__weak__, __alias__(#name)))

/* utilities */
#define UNUSED(x) (void)(x)
#define UNLIKELY(x) __builtin_expect((x), 0)
#define LIKELY(x) __builtin_expect((x), 1)

#ifdef DEBUG
#define DCHECK(x) if (UNLIKELY(!(x))) heapmgr_fatal("check failed: " #x, __func__, __LINE__)
#define MAY_UNUSED(x) (void)0
#else
#define DCHECK(x) (void)0
#define MAY_UNUSED(x) (void)(x)
#endif

#define align_up(x, a) ({typeof(x) __mask = (typeof(x))(a) - (typeof(x))1;\
			(((x) + (__mask)) & ~(__mask));})
#define align_down(x, a) ({typeof(x) __mask = (typeof(x))(a) - (typeof(x))1;\
			  ((x) & ~(__mask));})

#define ptr_to_type(ptr, type)	((type)(uintptr_t)(ptr))
#define ptr_to_ulong(ptr)	ptr_to_type(ptr, unsigned long)
#define ulong_to_ptr(ulong, type)	({type *__p; __p = (type *)(uintptr_t)(ulong); __p;})

ALWAYS_INLINE bool mul_overflow(size_t a, size_t b, size_t *r)
{
#if SIZE_MAX == UINT_MAX
	return __builtin_umul_overflow(a, b, r);
#else
	return __builtin_umull_overflow(a, b, r);
#endif
}

ALWAYS_INLINE size_t size2direct(size_t size)
{
	DCHECK(size <= SMALL_SIZE_MAX);
	return (size + SMALL_ALIGN - 1UL) / SMALL_ALIGN;
}

ALWAYS_INLINE void *align_ptr(void *p, uintptr_t alignment)
{
	uintptr_t v = (uintptr_t)p;
	v &= ~(alignment - 1UL);
	return (void *)v;
}

ALWAYS_INLINE bool ptr_aligned(void *p, uintptr_t alignment)
{
	uintptr_t v = (uintptr_t)p;
	v &= (alignment - 1UL);
	return (v == 0UL);
}

/* data types */
typedef struct list_s {
	struct list_s *prev;
	struct list_s *next;
} list_t;

typedef uint64_t bitmap_t[NUM_BITMAP_ULLS];

typedef char lock_t[CACHE_LINE_SIZE] CACHE_ALIGNED;

typedef struct block_s {
	struct block_s *next;
} block_t;

typedef struct page_s {
	union {
		/* common page state header */
		struct {
			uint16_t state;
		};

		/* head page of contiguous free pages */
		struct {
			uint16_t state;
			uint16_t _padding0;
			uint32_t _padding1;
			/* number of contiguous free pages */
			size_t npages;
			list_t list;
		} free_head;

		/* tail page of contiguous free pages */
		struct {
			uint16_t state;
			uint16_t _padding0;
			uint32_t _padding1;
			/* number of contiguous free pages */
			size_t npages;
		} free_tail;

		/* page for small block allocation */
		struct {
			uint16_t state;
			uint16_t _padding0;
			/* size of small block */
			uint16_t size;
			/* number of used blocks */
			uint16_t used;
			/* block freelist pointer */
			block_t *free;
			/* in heap->bin[size2bin(small.size)] or full */
			list_t list;
		} small;

		/* head page of contiguous allocated large pages */
		struct {
			uint16_t state;
			uint16_t _padding0;
			uint32_t _padding1;
			/* number of contiguous allocated pages */
			size_t npages;
			/* in heap->free[free_head.npages - 1] */
			list_t list;
		} large_head;

		/* tail page of contiguous allocated large pages */
		struct {
			uint16_t state;
			uint16_t _padding0;
			uint32_t _padding1;
			/* number of contiguous allocated pages */
			size_t npages;
		} large_tail;

		/* large_head, shared between 2 allocations */
		struct {
			uint16_t state;
			/* whether first part is used */
			uint8_t used0;
			/* whether second part is used */
			uint8_t used1;
			/* size of first part */
			uint32_t size0;
			/* number of pages is always 1 */
			size_t npages;
		} large_head_mixed;

		/* large_tail, shared between 2 allocations */
		struct {
			uint16_t state;
			/* whether first part is used */
			uint8_t used0;
			/* whether second part is used */
			uint8_t used1;
			/* size of first part */
			uint32_t size0;
			/* number of pages */
			size_t npages;
		} large_tail_mixed;

		/* ensure PAGE_DESC_SIZE */
		struct {
			char _space[PAGE_DESC_SIZE];
		};
	};
} page_t;
static_assert(sizeof(page_t) == PAGE_DESC_SIZE, "invalid page_t size");
static_assert(offsetof(page_t, free_head.npages) == offsetof(page_t, large_head.npages),
	      "page_t wrong layout");
static_assert(offsetof(page_t, free_tail.npages) == offsetof(page_t, large_tail.npages),
	      "page_t wrong layout");
static_assert(offsetof(page_t, large_head_mixed.npages) == offsetof(page_t, large_head.npages),
	      "page_t wrong layout");
static_assert(offsetof(page_t, large_tail_mixed.npages) == offsetof(page_t, large_tail.npages),
	      "page_t wrong layout");
static_assert(offsetof(page_t, large_head_mixed.used0) ==
	      offsetof(page_t, large_tail_mixed.used0),
	      "page_t wrong layout");
static_assert(offsetof(page_t, large_head_mixed.used1) ==
	      offsetof(page_t, large_tail_mixed.used1),
	      "page_t wrong layout");
static_assert(offsetof(page_t, large_head_mixed.npages) ==
	      offsetof(page_t, large_tail_mixed.npages),
	      "page_t wrong layout");
static_assert(offsetof(page_t, small.list) == offsetof(page_t, free_head.list),
	      "page_t wrong layout");

typedef struct chunk_s {
	union {
		/* common chunk state header, match with page_t */
		struct {
			uint16_t state;
		};

		/* normal chunk managed by heap manager */
		struct {
			uint16_t state;
			uint32_t used;
			struct heap_s *heap;
			list_t list;
		} normal;

		/* unmanaged huge chunk directly from mmap */
		struct {
			uint16_t state;
			size_t size;
			void *base;
		} huge;

		/* unmanaged huge aligned chunk directly from mmap */
		struct {
			uint16_t state;
			size_t size;
			void *base;
		} huge_aligned;

		/* ensure CHUNK_DESC_SIZE */
		struct {
			char _space[CHUNK_DESC_SIZE];
		};
	};
} chunk_t;
static_assert(sizeof(chunk_t) == CHUNK_DESC_SIZE, "invalid chunk_t size");
static_assert(PAGE_DESC_SIZE == CHUNK_DESC_SIZE, "invalid chunk_t size");

typedef struct heap_s {
	/* Assume zero init is valid lock, 64 bytes at most */
	lock_t lock;
	/* Page cache for small alloc fastpath */
	page_t *direct[NUM_DIRECT];
	/* Page freelist for small alloc */
	list_t bin[NUM_SMALL_CLASSES];
	/* Full page list for small alloc */
	list_t full;
	/* Free page bitmap for large alloc */
	bitmap_t freemap;
	/* Page freelist for large alloc */
	list_t free[DATA_PAGE_NUM];
	/* Cached normal chunks that are not unmapped */
	chunk_t *cache[CACHED_NUM_MAX];
	size_t n_cached;
	/* List of heap chunks */
	list_t chunks;
} heap_t;

/* internal variables */
extern heap_t g_heapmgr_mainheap ATTR_HIDDEN;
extern heap_t *g_heapmgr_heaps[HEAP_NUM_MAX] ATTR_HIDDEN;
extern size_t g_heapmgr_num_heaps ATTR_HIDDEN;
extern lock_t g_heapmgr_set_heaps_lock ATTR_HIDDEN;
extern size_t g_heapmgr_trim_threshold ATTR_HIDDEN;
extern bool g_heapmgr_enable_mcheck ATTR_HIDDEN;
extern size_t g_heapmgr_hugealloc_cnt ATTR_HIDDEN;
extern size_t g_heapmgr_hugealloc_size ATTR_HIDDEN;

/* internal functions */
void *heap_alloc_large(heap_t *heap, size_t size) ATTR_MALLOC ATTR_HIDDEN;
void *heap_alloc_small_locked(heap_t *heap, size_t size_arg) ATTR_MALLOC ATTR_HIDDEN;
void heap_free_large_locked(heap_t *heap, void *p) ATTR_HIDDEN;
void heap_free_small_locked(heap_t *heap, page_t *page, block_t *block) ATTR_HIDDEN;
void free_huge_pages(void *p) ATTR_HIDDEN;
void *heap_aligned_alloc(heap_t *heap, size_t size_arg, size_t alignment) ATTR_MALLOC ATTR_HIDDEN;
size_t large_page_usable_size(page_t *pg, char *pointer) ATTR_HIDDEN;

int set_num_heaps(size_t num) ATTR_HIDDEN;
void set_num_cached(size_t num) ATTR_HIDDEN;
void set_trim_threshold(size_t n) ATTR_HIDDEN;

void heap_shrink_locked(heap_t *heap) ATTR_HIDDEN;
void heap_print_state_locked(heap_t *heap, FILE *f) ATTR_HIDDEN;
void heap_dump_state_locked(heap_t *heap, heapmgr_state_t *s) ATTR_HIDDEN;

ALWAYS_INLINE void list_init(list_t *head)
{
	head->prev = head;
	head->next = head;
}

ALWAYS_INLINE void list_delete(list_t *node)
{
	node->prev->next = node->next;
	node->next->prev = node->prev;
	list_init(node);
}

ALWAYS_INLINE void list_insert_before(list_t *head, list_t *node)
{
	node->prev = head->prev;
	node->next = head;
	head->prev->next = node;
	head->prev = node;
}

ALWAYS_INLINE bool list_empty(list_t *head)
{
	return (head->next == head && head->prev == head);
}

ALWAYS_INLINE void bitmap_clear(uint64_t *bitmap, size_t i)
{
	bitmap[i / 64UL] &= ~(1ULL << (i % 64UL));
}

ALWAYS_INLINE void bitmap_set(uint64_t *bitmap, size_t i)
{
	bitmap[i / 64UL] |= (1ULL << (i % 64UL));
}

/* return one plus index of first bit-1 from i, or 0 if no bit-1 found from i */
ALWAYS_INLINE size_t bitmap_ffs_from(uint64_t *bitmap, size_t i)
{
	/* mask and ignore bits till i */
	size_t ret = 0UL;
	uint64_t tmp = bitmap[i / 64UL] & ~((1ULL << (i % 64UL)) - 1UL);
	size_t r = (size_t)(unsigned)__builtin_ffsll(tmp);
	if (r != 0UL) {
		ret = r + (i / 64UL) * 64UL;
	} else {
		for (size_t j = i / 64UL + 1UL; j < NUM_BITMAP_ULLS; ++j) {
			r = (size_t)(unsigned) __builtin_ffsll(bitmap[j]);
			if (r != 0UL) {
				ret = r + j * 64UL;
				break;
			}
		}
	}
	return ret;
}

ALWAYS_INLINE bool page_is_in_list(page_t *page)
{
	return (page->state == PAGE_STATE_FREE_HEAD ||
		page->state == PAGE_STATE_SMALL);
}

ALWAYS_INLINE page_t *list_to_page(list_t *node)
{
	void *r = (char *)node - offsetof(page_t, small.list);
	DCHECK(page_is_in_list(r));
	return r;
}

ALWAYS_INLINE chunk_t *list_to_chunk(list_t *node)
{
	chunk_t *c = NULL;
	unsigned long tmp_addr = ptr_to_ulong(node);
	tmp_addr -= offsetof(chunk_t, normal.list);
	c = ulong_to_ptr(tmp_addr, chunk_t);
	DCHECK(ptr_aligned(c, CHUNK_SIZE));
	DCHECK(c->state == CHUNK_STATE_NORMAL);
	return c;
}

ALWAYS_INLINE chunk_t *page_chunk_of(page_t *page)
{
	DCHECK(ptr_aligned(page, PAGE_DESC_SIZE));
	chunk_t *c = align_ptr(page, CHUNK_SIZE);
	DCHECK((char *)page < (char *)c + META_PAGE_SIZE);
	DCHECK((char *)page > (char *)c + PAGE_DESC_SIZE);
	return c;
}

ALWAYS_INLINE bool heap_owns_page(heap_t *heap, page_t *page)
{
	chunk_t *c = page_chunk_of(page);
	DCHECK(c->state == CHUNK_STATE_NORMAL);
	return (heap == c->normal.heap);
}

ALWAYS_INLINE void heap_move_page_to_full(heap_t *heap, page_t *page)
{
	DCHECK(heap_owns_page(heap, page));
	DCHECK(page->state == PAGE_STATE_SMALL);
	DCHECK(page->small.used == HEAPMGR_PAGE_SIZE / page->small.size);
	list_delete(&page->small.list);
	list_insert_before(&heap->full, &page->small.list);
}

ALWAYS_INLINE void heap_insert_free_page(heap_t *heap, page_t *page)
{
	size_t npages = page->free_head.npages;
	list_insert_before(&heap->free[npages - 1UL], &page->free_head.list);
	bitmap_set(heap->freemap, npages - 1UL);
}

ALWAYS_INLINE void heap_remove_free_page(heap_t *heap, page_t *page)
{
	size_t npages = page->free_head.npages;
	list_delete(&page->free_head.list);
	if (list_empty(&heap->free[npages - 1UL])) {
		bitmap_clear(heap->freemap, npages - 1UL);
	}
}

ALWAYS_INLINE void *page_base_addr_of(page_t *page)
{
	chunk_t *c = page_chunk_of(page);
	size_t idx = (size_t)((char *)page - (char *)c) / PAGE_DESC_SIZE;
	DCHECK(idx >= META_PAGE_NUM);
	DCHECK(idx < PAGES_PER_CHUNK);
	return ((char *)c + idx * HEAPMGR_PAGE_SIZE);
}

ALWAYS_INLINE bool page_block_valid(page_t *page, block_t *block)
{
	bool ret = true;

	if (block != NULL) {
		char *p = (char *)block;
		char *base = page_base_addr_of(page);
		if (p < base || p >= base + HEAPMGR_PAGE_SIZE) {
			ret = false;
		} else {
			ret = ((uint16_t)(p - base) % page->small.size == 0U);
		}
	}

	return ret;
}

ALWAYS_INLINE block_t *page_pop_block(page_t *page)
{
	block_t *b = page->small.free;
	DCHECK(page_block_valid(page, b->next));
	page->small.free = b->next;
	page->small.used++;
	/* sec: erase b->next */
	return b;
}

ALWAYS_INLINE void page_push_block(page_t *page, block_t *block)
{
	DCHECK(page_block_valid(page, block));
	block->next = page->small.free;
	page->small.free = block;
	page->small.used--;
}

ALWAYS_INLINE bool page_block_free(page_t *page, block_t *block)
{
	bool ret = false;

	DCHECK(page->state == PAGE_STATE_SMALL);
	block_t *b = page->small.free;
	while (b != NULL) {
		if (b == block) {
			ret = true;
			break;
		}
		b = b->next;
	}

	return ret;
}

ALWAYS_INLINE bool page_range_empty(page_t *begin, page_t *end)
{
	bool ret = true;

	for (page_t *p = begin; p < end; ++p) {
		if (p->state != 0U) {
			ret = false;
			break;
		}
	}

	return ret;
}

ALWAYS_INLINE void heapmgr_mutex_init(void *lock)
{
	int *s = lock;
	s[0] = 0;
	s[1] = 0;
}

ALWAYS_INLINE void heapmgr_mutex_lock(void *lock)
{
	volatile int *lk = lock;
	while (a_swap(lk, 1) != 0) {
		__wait(lk, lk + 1, 1, 1);
	}
}

ALWAYS_INLINE void heapmgr_mutex_unlock(void *lock)
{
	volatile int *lk = lock;
	if (lk[0] != 0) {
		a_store(lk, 0);
		if (lk[1] != 0) {
			__wake(lk, 1, 1);
		}
	}
}

#endif /* HEAPMGR_INTERNAL_H */
