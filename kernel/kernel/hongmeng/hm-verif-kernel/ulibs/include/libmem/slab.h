/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2023. All rights reserved.
 * Description: Slab function declaration
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 22 21:44:33 2019
 */
#ifndef _ULIBS_LIBMEM_SLAB_H_
#define _ULIBS_LIBMEM_SLAB_H_

#include <stdint.h>

#include <lib/dlist.h>
#include <libmem/slab_utils.h>
#include <libmem/slab_track.h>
#include <libstrict/strict.h>
#include <libhmsync/atomic.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/bitops/generic.h>
#include <libhmsync/uspinlock.h>
#include <libmem/redzone.h>

#define SLAB_SLOT_ALIGN	16U
#define SLAB_SLOT_MAX	(SLAB_PAGE_SIZE / SLAB_SLOT_ALIGN)
#define SLAB_SLOT_FREE	0
#define SLAB_SLOT_USED	1

#define SLAB_MAGIC	0x44455343
#define SLAB_POOL_MAGIC	0x5A5AA5A5

#ifdef __aarch64__
#define __CURRENTEL_EL1	1
#define __current_ttbr(reg)	(((reg) >> 63ULL) & 0x01ULL)

static inline unsigned long long __a64_curr_el(void)
{
	unsigned long long pc;

	asm volatile ("adr %0, ." : "=r" (pc));
	return __current_ttbr(pc);
}

#define IS_INVALID_PTR(ptr) ({ \
	unsigned long long el = __a64_curr_el(); \
	((el == __CURRENTEL_EL1 && ((uintptr_t)(ptr) < 0xffff000000000000)) || (ptr) == NULL); \
})
#else
#define IS_INVALID_PTR(ptr)    ((ptr) == NULL)
#endif

/*
 * slab pool struct:
 *
 *
 *                +--- raw_mutex lock
 * +-----------+  |
 * | slab_pool | -+--- total_size
 * +-----------+  |
 *                +--- free_size
 *                +--- grown_size
 *                |
 *                |                 +-----------+      +-----------+      +-----------+      +-----------+      +-----------+      +-----------+
 *                +--- mem_list --- | slab_page | ---- | slab_page | ---- | slab_page | ---- | slab_page | ---- | slab_page | ---- | slab_page |
 *                                  +-----------+      +-----------+      +-----------+      +-----------+      +-----------+      +-----------+
 *                                  (free, meta)    (free, reclaimable)  (free, reclaimable)  (free, meta)      (free, meta)   (free, reclaimable)
 *
 * slab allocator struct:
 *
 *
 *                      +-----------+      +-----------------------+
 *                +---- | caches[i] | ---- | ptr | ptr | ... | ptr |
 *                |     +-----------+      +-----------------------+
 *                | (i = 0, 1,..., NR_CPUS - 1)
 *                |     +-----------+      +-----------+      +-----------+
 *                +---- |   empty   | ---- | slab_page | ---- | slab_page |
 *                |     +-----------+      +-----------+      +-----------+
 *                |                        (free, meta)      (free, reclaimable)
 * +-----------+  |     +-----------+      +-----------+      +-----------+      +-----------+
 * | slab_ator | -+---- |  partial  | ---- | slab_page | ---- | slab_page | ---- | slab_page |
 * +-----------+  |     +-----------+      +-----------+      +-----------+      +-----------+
 *                |
 *                |     +-----------+      +-----------+
 *                +---- |    full   | ---- | slab_page |
 *                |     +-----------+      +-----------+
 *                |
 *                +---- other_meta_data
 *
 * slab page memory layout:
 *
 * |<-------------------------------------- PAGE_SIZE -------------------------------------->|
 *
 * +-----------------------------------------------------------------------------------------+
 * | is_reclaimable | free_map | used_num | s_range | node | ator | slot | slot | ... | slot |
 * +-----------------------------------------------------------------------------------------+
 *
 * |<------------------------- slab desc ------------------------>|<--------- data --------->|
 *
 * is_reclaimable: slab page can be release to another allocator
 * free_map:       free slot map, 0 for free, 1 for used
 * used_num:       number of used slot
 * s_range:        desctiption of slab page range
 * node:           slab page will be in ator or pool
 * ator:           which allocator belongs to, or null in pool
 */
struct slab_desc_range {
	uintptr_t ori_start; /* alloc start vaddr this must be align to slab_page */
	size_t ori_size; /* alloc size can be large than slab_page_size */
};

struct slab_desc {
	/* free slot map, 0 for free and 1 for used */
	unsigned long long free_map[BITS_TO_LONGLONGS(SLAB_SLOT_MAX)];
	uint32_t magic;
	uint32_t used_num; /* number of this slab has used slot */
	struct slab_desc_range s_range; /* slab page's self range description */
	struct dlist_node node; /* node will in either partial, empty, full or pool */
	struct slab_ator *ator; /* pointer to slab allocator of which belongs to */
};

struct array_cache {
	uint32_t limit;
	uint32_t remain;
	uint32_t batchcount;
	struct raw_mutex lock;
	void **array;
};

struct slab_ator {
#ifdef CONFIG_PER_CPU_SLAB
	struct array_cache caches[SLAB_CACHE_NUM];
#endif
	struct dlist_node empty;
	struct dlist_node partial;
	struct dlist_node full;

	unsigned int empty_num; /* number of SLAB_PAGE in empty list */
	unsigned int partial_num; /* number of SLAB_PAGE in partial list */
	unsigned int full_num; /* number of SLAB_PAGE in full list */

	uint32_t max_slot_count; /* max number of slot this slab_page */
	vatomic32_t slab_page_hold; /* number of slab_page have */
	vatomic32_t used_slot_num; /* the number of all used slots */
	uint32_t obj_size;
#ifdef CONFIG_SLAB_DEBUG
	uint32_t obj_orig_size; /* origin obj size */
#endif
	uint32_t flags;

	struct slab_pool *pool;

#ifdef CONFIG_USE_USPINLOCK
	struct uspinlock_s lock;
#else
	struct raw_mutex lock;
#endif

	/* NOTE: this domain used to calculate slab used info */
	char name[SLAB_NAME_LEN]; /* which sysmgr service has used */
};

struct slab_pool {
	struct dlist_node mem_list; /* free slab_page in pool */
#ifdef CONFIG_USE_USPINLOCK
	struct uspinlock_s lock;
#else
	struct raw_mutex lock;
#endif
	struct raw_mutex extend_lock;
	struct raw_mutex shrink_lock;
	/* sysdump and slab pool info */
	raw_atomic_ullong_t total_size;
	raw_atomic_ullong_t free_size;
	raw_atomic_ullong_t grown_size;
};

struct process_s;
#define SLAB_MAX_MALLOC_SIZE	(SLAB_PAGE_SIZE - sizeof(struct slab_desc) - REDZONE_SIZE - SLAB_DEBUG_SIZE)

raw_static_assert(SLAB_MAX_MALLOC_SIZE >= SLAB_MAX_LOWER_BOUND, slab_max_malloc_size_too_small);

void slab_pool_init(struct slab_pool *s_pool);
#ifdef CONFIG_SLAB_FREELIST_RANDOM
void slab_update_random_seed(unsigned long random_value);
#else
static inline void slab_update_random_seed(unsigned long random_value)
{
	UNUSED(random_value);
};
#endif

int slab_ator_prepare_no_lock(struct slab_ator *s_ator);

uint32_t slab_slot_size(const struct slab_ator *ator);
uint32_t slab_page_usable_size(const struct slab_ator *ator);

struct slab_desc *slab_obj_to_slab_desc(const void *obj, size_t slab_size);
int slab_check_range(uintptr_t start, size_t size, size_t slab_size);

void process_add_slab_size(struct process_s *process, int64_t size, struct slab_ator *s_ator);
void process_sub_slab_size(pid_t pid, int64_t size, struct slab_ator *s_ator);

uintptr_t slot_idx_to_ptr(const struct slab_ator *s_ator,
			  const struct slab_desc *s_desc,
			  uint32_t idx);
uint32_t slot_ptr_to_idx(const struct slab_ator *s_ator,
			 const struct slab_desc *s_desc,
			 const void *ptr);

uint32_t slab_fetch_idx_in_freemap(const unsigned long long *bitmap, unsigned int size);
void move_partial_to_full(struct slab_ator *s_ator, struct slab_desc *s_desc);
void free_slab_obj_in_desc(struct slab_ator *s_ator, const void *obj, struct slab_desc *s_desc);

#ifdef CONFIG_USE_USPINLOCK
static inline void ator_lock(struct slab_ator *ator)
{
	uspinlock_lock(&ator->lock);
}

static inline void ator_unlock(struct slab_ator *ator)
{
	uspinlock_unlock(&ator->lock);
}
static inline void slab_pool_lock(struct slab_pool *s_pool)
{
	uspinlock_lock(&s_pool->lock);
}

static inline void slab_pool_unlock(struct slab_pool *s_pool)
{
	uspinlock_unlock(&s_pool->lock);
}
#else
static inline void ator_lock(struct slab_ator *ator)
{
	raw_mutex_lock(&ator->lock);
}

static inline void ator_unlock(struct slab_ator *ator)
{
	raw_mutex_unlock(&ator->lock);
}
static inline void slab_pool_lock(struct slab_pool *s_pool)
{
	raw_mutex_lock(&s_pool->lock);
}

static inline void slab_pool_unlock(struct slab_pool *s_pool)
{
	raw_mutex_unlock(&s_pool->lock);
}
#endif
#endif /* _ULIBS_LIBMEM_SLAB_H_ */
