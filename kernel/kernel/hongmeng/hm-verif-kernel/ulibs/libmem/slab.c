/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Slab function implementation
 * Author: Huawei OS Kernel Lab
 * Create: Wed May 22 21:44:31 2019
 */
#include "slab.h"

#ifdef CONFIG_PER_CPU_SLAB
#include "slab_per_cpu.h"
#endif
#include <libmem/redzone.h>
#include <libmem/poison.h>

#include <limits.h>
#include <stdbool.h>
#include <malloc.h>
#include <libmem/slab_utils.h>
#include <libmem/memm_slab.h>
#include <libmem/utils.h>
#include <libmem/debug.h>
#ifdef CONFIG_SLAB_DEBUG_USAGE
#include <libmem/slab_track.h>
#endif
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hongmeng/panic.h>
#include <hmkernel/compiler.h>
#include <libhwsecurec/securec.h>
#include <libhmsync/raw_mutex_guard.h>
#include <libhmlog/hmlog_ratelimit.h>

#define SLAB_EMPTY_REMAIN_PAGE 3U /* slab empty list will store number of page */

#ifdef CONFIG_SLAB_FREELIST_RANDOM

static unsigned long slab_random_seed = 0;

static uint32_t slab_fetch_random_idx(uint32_t range)
{
	BUG_ON(range == 0U);
	return (uint32_t)slab_random_seed % range;
}

static uint32_t raw_bitmap_find_random_zero(const unsigned long long *bitmap,
					    unsigned int size)
{
	uint32_t random_idx;
	uint32_t idx;

	random_idx = slab_fetch_random_idx(size);
	idx = raw_bitmap_find_next_zero(bitmap, size, random_idx);
	if (idx >= size) {
		idx = raw_bitmap_find_first_zero(bitmap, size);
	}
	return idx;
}

uint32_t slab_fetch_idx_in_freemap(const unsigned long long *bitmap,
				   unsigned int size)
{
	return raw_bitmap_find_random_zero(bitmap, size);
}

void slab_update_random_seed(unsigned long random_value)
{
	slab_random_seed = random_value;
}
#else
uint32_t slab_fetch_idx_in_freemap(const unsigned long long *bitmap,
				   unsigned int size)
{
	return raw_bitmap_find_first_zero(bitmap, size);
}
#endif

/* return slab can support max alloc size */
size_t slab_max_malloc(void)
{
	return SLAB_MAX_MALLOC_SIZE;
}

struct slab_ator *slab_obj_to_ator(const void *obj)
{
	struct slab_ator *ret = NULL;
	struct slab_desc *s_desc = NULL;

	obj = slab_redzone_obj_to_slot(obj);
	/* NOTE: Here should cast slab_size, otherwise will bug in aarch64 */
	s_desc = slab_obj_to_slab_desc(obj, (size_t)SLAB_PAGE_SIZE);
	if (s_desc != NULL) {
		ret = s_desc->ator;
	}

	return ret;
}

struct slab_pool *slab_ator_to_pool(const struct slab_ator *ator)
{
	struct slab_pool *pool = NULL;

	if (ator != NULL) {
		pool = ator->pool;
	}

	return pool;
}

static inline bool slab_desc_is_used(const struct slab_desc *s_desc)
{
	/*
	 * NOTE: we reuse the pointer of ator to indicate
	 * whether this slab_page is used. If ator is NULL,
	 * it means that this slab_page is free in the pool.
	 * If ator is not empty, it may be used by an allocator
	 */
	return s_desc->ator != NULL;
}

static struct slab_desc *slab_desc_init(uintptr_t slab_page_vaddr,
					const struct slab_desc_range *s_range)
{
	struct slab_desc *s_desc = NULL;

	BUG_ON(s_range == NULL);

	s_desc = (struct slab_desc *)slab_page_vaddr;
	mem_zero_a(s_desc->free_map);
	s_desc->used_num = 0;
	s_desc->s_range.ori_start = s_range->ori_start;
	s_desc->s_range.ori_size = s_range->ori_size;
	dlist_init(&s_desc->node);
	s_desc->ator = NULL;
	s_desc->magic = SLAB_POOL_MAGIC;

	return s_desc;
}


#ifdef CONFIG_DEBUG_SLAB_POISON
static inline void alloc_init_slab_slot(struct slab_ator *s_ator, struct slab_desc *s_desc)
{
	unsigned int slot_size;
	unsigned int idx;
	void *slot = NULL;

	slot_size = s_ator->obj_size;
	for (idx = 0; idx < s_ator->max_slot_count; idx++) {
		slot = ulong_to_ptr(slot_idx_to_ptr(s_ator, s_desc, idx), void);
		NOFAIL(memset_s((void *)slot, slot_size, POISON_END, slot_size));
		NOFAIL(memset_s((void *)slot, slot_size - 1, POISON_FREE, slot_size - 1));
	}
}
#else /* !CONFIG_DEBUG_SLAB_POISON */
static inline void alloc_init_slab_slot(struct slab_ator *s_ator, struct slab_desc *s_desc)
{
	UNUSED(s_ator, s_desc);
}
#endif /* CONFIG_DEBUG_SLAB_POISON */

static void slab_desc_insert(struct slab_ator *s_ator, struct slab_desc *s_desc)
{
	s_desc->ator = s_ator;
	s_desc->magic = SLAB_MAGIC;

	alloc_init_slab_slot(s_ator, s_desc);

	/* insert this descript into slab_allocator empty list */
	dlist_insert_head(&s_ator->empty, &s_desc->node);
	s_ator->empty_num++;

	/* every slab descript hold one slab_page */
	if (unlikely(vatomic32_read_rlx(&s_ator->slab_page_hold) == (unsigned int)INT_MAX)) {
		hm_warn_ratelimited("allocator[%s] slab page hold number reach to maximum, stop counting\n",
			 s_ator->name);
	} else {
		/* here only insert into empty so just one */
		vatomic32_inc(&s_ator->slab_page_hold);
	}
}

static void slab_desc_remove(struct slab_ator *s_ator, struct slab_desc *s_desc)
{
	dlist_del_entry(s_desc, struct slab_desc, node);
	s_ator->empty_num--;
	/* slab page also not use */
	s_desc->ator = NULL;
	s_desc->magic = 0;

	if (unlikely(vatomic32_read_rlx(&s_ator->slab_page_hold) == 0U)) {
		hm_warn_ratelimited("allocator[%s] slab page hold number reach to zero, stop counting\n",
			 s_ator->name);
	} else {
		vatomic32_dec(&s_ator->slab_page_hold);
	}
}

static void move_empty_to_partial(struct slab_ator *s_ator,
				  struct slab_desc *s_desc)
{
	/* delete s_desc->node from empty */
	dlist_del_entry(s_desc, struct slab_desc, node);
	s_ator->empty_num--;

	dlist_insert_head(&s_ator->partial, &s_desc->node);
	s_ator->partial_num++;
}

void move_partial_to_full(struct slab_ator *s_ator, struct slab_desc *s_desc)
{
	/* delete s_desc->node from partial */
	dlist_del_entry(s_desc, struct slab_desc, node);
	s_ator->partial_num--;

	dlist_insert_head(&s_ator->full, &s_desc->node);
	s_ator->full_num++;
}

static void move_full_to_partial(struct slab_ator *s_ator,
				 struct slab_desc *s_desc)
{
	/* delete s_desc->node from full */
	dlist_del_entry(s_desc, struct slab_desc, node);
	s_ator->full_num--;

	dlist_insert_head(&s_ator->partial, &s_desc->node);
	s_ator->partial_num++;
}

static void move_partial_to_empty(struct slab_ator *s_ator,
				  struct slab_desc *s_desc)
{
	/* delete s_desc->node from partial */
	dlist_del_entry(s_desc, struct slab_desc, node);
	s_ator->partial_num--;

	dlist_insert_head(&s_ator->empty, &s_desc->node);
	s_ator->empty_num++;
}

static int alloc_one_slab_page_from_pool(struct slab_ator *s_ator)
{
	struct slab_pool *s_pool = s_ator->pool;
	struct slab_desc *s_desc = NULL;
	int rc = E_HM_OK;

	slab_pool_lock(s_pool);
	if (dlist_empty(&(s_pool->mem_list))) {
		rc = E_HM_NOMEM;
	}

	if (rc == E_HM_OK) {
		s_desc = dlist_first_entry(&(s_pool->mem_list), struct slab_desc, node);
		dlist_del_entry(s_desc, struct slab_desc, node);
		(void)raw_atomic_ullong_sub_relaxed(&s_pool->free_size, (uint64_t)s_desc->s_range.ori_size);
		slab_desc_insert(s_ator, s_desc);
	}
	slab_pool_unlock(s_pool);

	return rc;
}

static void free_one_slab_page_to_pool(struct slab_ator *s_ator)
{
	struct slab_pool *s_pool = s_ator->pool;
	struct slab_desc *s_desc = NULL;
	bool is_empty = false;

	/* here caller make sure dlist_empty is not null */
	is_empty = !!dlist_empty(&s_ator->empty);
	BUG_ON(is_empty);

	/* get the free empty slab page */
	s_desc = dlist_first_entry(&s_ator->empty, struct slab_desc, node);
	/* remove from allocator */
	slab_desc_remove(s_ator, s_desc);

	/* move this slab page to free in slab pool */
	slab_pool_lock(s_pool);
	/* s_desc->ator has been set NULL in slab_desc_remove */
	(void)raw_atomic_ullong_add_relaxed(&s_pool->free_size, (uint64_t)s_desc->s_range.ori_size);
	dlist_insert_head(&(s_pool->mem_list), &s_desc->node);
	slab_pool_unlock(s_pool);
}

static void ator_init(struct slab_ator *s_ator, uint32_t obj_size, const char *name,
		      struct slab_pool *s_pool, uint32_t s_flags)
{
	int ret;
#ifdef CONFIG_SLAB_DEBUG
	uint32_t size;
	uint32_t flags;
#endif
	mem_zero_s(*s_ator);
	dlist_init(&s_ator->empty);
	dlist_init(&s_ator->partial);
	dlist_init(&s_ator->full);

#ifdef CONFIG_USE_PREEMPT_PROCESS
	uspinlock_init(&s_ator->lock);
#else
	raw_mutex_init(&s_ator->lock);
#endif

	vatomic32_init(&s_ator->slab_page_hold, 0);
	vatomic32_init(&s_ator->used_slot_num, 0);
#ifdef CONFIG_SLAB_DEBUG
	flags = slab_set_debug_flags(s_flags, name);
	s_ator->flags |= flags;
	size = slab_calc_obj_size(obj_size, flags);
	s_ator->obj_orig_size = obj_size;
	s_ator->obj_size = size;
#else
	s_ator->flags |= s_flags;
	s_ator->obj_size = obj_size;
#endif
	ret = strncpy_s(s_ator->name, SLAB_NAME_LEN, name, SLAB_NAME_LEN - 1U);
	BUG_ON(ret != 0);
	s_ator->pool = s_pool;

	s_ator->max_slot_count = (SLAB_PAGE_SIZE - (uint32_t)sizeof(struct slab_desc)) / (s_ator->obj_size);

	if (unlikely(s_ator->max_slot_count == 0U)) {
		hm_panic("calculate max slot count error, slab size:(%u), sizeof(struct slab_desc):(%zu)\n",
			 SLAB_PAGE_SIZE, sizeof(struct slab_desc));
	}
}

int slab_ator_init(struct slab_pool *s_pool, struct slab_ator *s_ator, uint32_t obj_size,
		   const char *name, struct slab_cache_info_s *info, uint32_t s_flags)
{
	int rc = E_HM_OK;

	if (unlikely((s_pool == NULL) || (s_ator == NULL) ||
		     (name == NULL) || (obj_size == 0U))) {
		hm_warn("failed to init slab allocator, wrong parameter\n");
		rc = E_HM_INVAL;
	}
	if (rc == E_HM_OK) {
		ator_init(s_ator, obj_size, name, s_pool, s_flags);
#ifdef CONFIG_PER_CPU_SLAB
		if (info == NULL) {
			hm_warn("failed to init slab allocator, wrong parameter\n");
			rc = E_HM_INVAL;
		} else {
			slab_ator_cache_setup(s_ator, info->cache_pool, info->limit);
		}
#else
		UNUSED(info);
#endif
	}

	return rc;
}

/* This should be used after memm_slab has been already initialized */
struct slab_ator *slab_ator_create(struct slab_pool *s_pool,
				   uint32_t obj_size, const char *name,
				   struct slab_cache_info_s *info,
				   uint32_t s_flags)
{
	int rc = E_HM_OK;
	struct slab_ator *ator = NULL;

	ator = (struct slab_ator *)malloc(sizeof(struct slab_ator));
	if (ator != NULL) {
		rc = slab_ator_init(s_pool, ator, obj_size, name, info, s_flags);
		if (rc != E_HM_OK) {
			free(ator);
			ator = NULL;
		}
	}

	return ator;
}
bool slab_reclaimable(const struct slab_ator *ator)
{
	bool flags = false;
	if (ator != NULL) {
		flags = ator->flags & SLAB_RECLAIMABLE;
	}
	return flags;
}
/* NOTE: should free slab allocator outside */
int slab_ator_destroy(struct slab_ator *s_ator)
{
	int rc = E_HM_OK;

	if (unlikely(s_ator == NULL)) {
		rc = E_HM_INVAL;
	}
#ifdef CONFIG_PER_CPU_SLAB
	if (rc == E_HM_OK) {
		ator_cache_clear(s_ator);
	}
#endif
	if ((rc == E_HM_OK) &&
	    (unlikely(!dlist_empty(&s_ator->partial) ||
		      !dlist_empty(&s_ator->full)))) {
		/* there still has obj in slab allocator */
		hm_warn_ratelimited("allocator[%s] still have allocated slot, do not destroy\n",
			 s_ator->name);
		rc = E_HM_BUSY;
	}

	if (rc == E_HM_OK) {
		/* remove this slab_allocator and all slab_descript */
		while (!dlist_empty(&s_ator->empty)) {
			free_one_slab_page_to_pool(s_ator);
		}
		s_ator->pool = NULL;
	}

	return rc;
}

uint32_t slab_fetch_ator_obj_size(const struct slab_ator *s_ator)
{
	return s_ator->obj_size;
}

void slab_pool_init(struct slab_pool *s_pool)
{
	if (unlikely(s_pool == NULL)) {
		hm_panic("slab pool is NULL\n");
	}

	/* initial pool state */
	mem_zero_s(*s_pool);
	dlist_init(&(s_pool->mem_list));
#ifdef CONFIG_USE_PREEMPT_PROCESS
	uspinlock_init(&s_pool->lock);
#else
	raw_mutex_init(&s_pool->lock);
#endif
	raw_mutex_init(&s_pool->extend_lock);
	raw_mutex_init(&s_pool->shrink_lock);
}

struct slab_pool *slab_pool_create(void)
{
	struct slab_pool *s_pool = NULL;

	s_pool = (struct slab_pool *)malloc(sizeof(struct slab_pool));
	if (s_pool != NULL) {
		slab_pool_init(s_pool);
	}

	return s_pool;
}

/*
 * The s_pool to be freed must be malloced.
 * The caller should ensure that the s_pool will not be in use,
 * otherwise there may be memory remained in the slab pool and
 * may cause memory leak.
 */
void slab_pool_destroy(struct slab_pool *s_pool)
{
	if (s_pool != NULL) {
		while (!dlist_empty(&(s_pool->mem_list))) {
			(void)slab_pool_pop_memory(s_pool);
		}
		free(s_pool);
	}
}

void slab_pool_extend_lock(struct slab_pool *s_pool)
{
	raw_mutex_lock(&s_pool->extend_lock);
}

void slab_pool_extend_unlock(struct slab_pool *s_pool)
{
	raw_mutex_unlock(&s_pool->extend_lock);
}

void slab_pool_shrink_lock(struct slab_pool *s_pool)
{
	raw_mutex_lock(&s_pool->shrink_lock);
}

void slab_pool_shrink_unlock(struct slab_pool *s_pool)
{
	raw_mutex_unlock(&s_pool->shrink_lock);
}

int slab_ator_prepare_no_lock(struct slab_ator *s_ator)
{
	int rc = E_HM_OK;
	struct slab_desc *s_desc = NULL;

	if (likely(s_ator != NULL)) {
		/* if there's no free slot in partial list, find free slot in empty list */
		if (dlist_empty(&s_ator->partial)) {
			/* if there's still no free slot in empty list, fill the empty list from slab pool */
			if (dlist_empty(&s_ator->empty)) {
				rc = alloc_one_slab_page_from_pool(s_ator);
			}
			if (rc == E_HM_OK) {
				/*
				 * alloc from empty list, and move the slab desc from empty
				 * list to partial list.
				 */
				s_desc = dlist_first_entry(&s_ator->empty, struct slab_desc, node);
				move_empty_to_partial(s_ator, s_desc);
			}
		}
	} else {
		rc = E_HM_INVAL;
	}

	return rc;
}

#define SLAB_PAGE_HOLD_WATERMARK_NUM	((1 * 1024 * 1024 * 1024) >> PAGE_SHIFT) /* 1G */
bool slab_report(struct slab_ator *s_ator)
{
	return (vatomic32_read_rlx(&s_ator->slab_page_hold) > SLAB_PAGE_HOLD_WATERMARK_NUM);
}

void *slab_alloc(struct slab_ator *s_ator)
{
#ifdef CONFIG_PER_CPU_SLAB
	return slab_alloc_per_cpu(s_ator);
#else
	struct slab_desc *s_desc = NULL;
	void *slot = NULL;
	uint32_t idx;
	int rc = E_HM_OK;

	if (likely(s_ator != NULL)) {
		ator_lock(s_ator);
		rc = slab_ator_prepare_no_lock(s_ator);
		if (rc == E_HM_OK) {
			/* alloc a slot from partial */
			s_desc = dlist_first_entry(&s_ator->partial, struct slab_desc, node);
			/* finds the next zero from the given location */
			idx = slab_fetch_idx_in_freemap(s_desc->free_map, s_ator->max_slot_count);
			/* partial list must have at least one free slot */
			if (unlikely(idx >= s_ator->max_slot_count)) {
				hm_error("s_desc:(0x%lx), idx:(%"PRIu32"), "
					 "obj_size:(%"PRIu32"), sizeof(struct slab_desc):(%zu)\n"
					 "slab in error state, partial list must have one free slot\n",
					 vaddr_of(s_desc), idx,
					 s_desc->ator->obj_size, sizeof(struct slab_desc));
				memm_slab_error_cb((uintptr_t)s_desc);
				hm_panic("Slab in error state!");
			}

			(void)raw_bitmap_set_bit(s_desc->free_map, s_ator->max_slot_count, idx);
			slot = ulong_to_ptr(slot_idx_to_ptr(s_ator, s_desc, idx), void);
#ifdef CONFIG_SLAB_DEBUG
			slot = slab_init_debug_zone(s_ator, slot);
#endif
			s_desc->used_num++;
			/* if used_num == max_slot_count move to full */
			if (s_desc->used_num == s_ator->max_slot_count) {
				/* remove from partial and into full */
				move_partial_to_full(s_ator, s_desc);
			}
			vatomic32_inc(&s_ator->used_slot_num);
		}
		ator_unlock(s_ator);
	}
	return slot;
#endif
}

#define NR_WORDS_DUMP	4
static void dump_slab_page_content(const void *obj)
{
	uintptr_t pgva = PAGE_ALIGN_DOWN((uintptr_t)obj);
	uintptr_t va = pgva;
	unsigned long words[NR_WORDS_DUMP];

	hm_error("=====dump page content begin=====\n");
	while (va < pgva + PAGE_SIZE) {
		for (int i = 0; i < NR_WORDS_DUMP; i++) {
			words[i] = ((unsigned long *)va)[i];
		}
		hm_error("0x%lx: %08lx %08lx %08lx %08lx\n",
			 vaddr_of(ptr_to_void(va)), words[0u], words[1u], words[2u], words[3u]);
		va += NR_WORDS_DUMP * sizeof(unsigned long);
	}
	hm_error("=====dump page content end=====\n");
}

void free_slab_obj_in_desc(struct slab_ator *s_ator, const void *obj,
			   struct slab_desc *s_desc)
{
	uint32_t idx;
	int rc;

	/* update first free lost */
	idx = slot_ptr_to_idx(s_ator, s_desc, obj);
	if (unlikely(idx >= s_ator->max_slot_count)) {
		hm_panic("s_desc:(0x%lx), obj:(0x%lx), "
			 "obj_size:(%"PRIu32"), sizeof(struct slab_desc):(%zu)\n"
			 "obj is not correct addr:(0x%lx), slot idx:(%"PRIu32")\n",
			 vaddr_of(s_desc), ptr_to_ulong(obj),
			 s_desc->ator->obj_size, sizeof(struct slab_desc),
			 ptr_to_ulong(obj), idx);
	}
	rc = raw_bitmap_test_bit(s_desc->free_map, s_ator->max_slot_count, idx);
	if (unlikely(rc == SLAB_SLOT_FREE)) {
		dump_slab_page_content(obj);
		hm_error("double free obj name: %s\n", s_ator->name);
		hm_panic("s_desc:(0x%lx), obj:(0x%lx), "
			 "obj_size:(%"PRIu32"), sizeof(struct slab_desc):(%zu)\n"
			 "double free for obj:(0x%lx), idx:(%"PRIu32")\n",
			 vaddr_of(s_desc), ptr_to_ulong(obj),
			 s_desc->ator->obj_size, sizeof(struct slab_desc),
			 ptr_to_ulong(obj), idx);
	}

	(void)raw_bitmap_clear_bit(s_desc->free_map, s_ator->max_slot_count, idx);
	/* return this slot into slab descript */
	s_desc->used_num--;
	/* if used_num == max_count - 1 move this slab_desc into partial */
	if (s_desc->used_num == s_ator->max_slot_count - 1U) {
		/* remove from full and into partial */
		move_full_to_partial(s_ator, s_desc);
	}
	/* if used_num == 0 move this slab_desc into empty */
	if (s_desc->used_num == 0) {
		/*
		 * we have only one slab page in empty
		 * if there are two, free one into slab pool
		 */
		if (!dlist_empty(&s_ator->empty) &&
		    (s_ator->empty_num >= SLAB_EMPTY_REMAIN_PAGE)) {
			/* first move slab page in empty now to slab pool */
			free_one_slab_page_to_pool(s_ator);
		}
		/* move this slab page into empty */
		move_partial_to_empty(s_ator, s_desc);
	}
	vatomic32_dec(&s_ator->used_slot_num);
}

void slab_free(struct slab_ator *s_ator, void *obj)
{
	struct slab_desc *s_desc = NULL;
	int ret = E_HM_OK;
	__maybe_unused pid_t pid = 0;

	if (likely((s_ator != NULL) && (obj != NULL))) {
		obj = slab_redzone_obj_to_slot(obj);
		/* search for slab desc from this obj */
		s_desc = slab_obj_to_slab_desc(obj, (size_t)SLAB_PAGE_SIZE);
		if (likely(s_desc != NULL)) {
			/* judge this obj whether belong to this allocator */
			if (unlikely(s_desc->ator != s_ator)) {
				/* wrong allocator */
				hm_warn_ratelimited("The allocator does not match the one of "
					 "the obj:(0x%lx)\n", ptr_to_ulong(obj));
				ret = E_HM_INVAL;
			}
		} else {
			hm_warn_ratelimited("Obj to slab desc failed. "
				 "Maybe the obj:(0x%lx) does not belong to slab\n",
				 ptr_to_ulong(obj));
			ret = E_HM_INVAL;
		}

		if (ret == E_HM_OK) {
#ifdef CONFIG_SLAB_DEBUG_USAGE
			slab_clear_user_info(s_ator, obj, &pid);
#endif
			process_sub_slab_size(pid, slab_slot_size(s_ator), s_ator);
#ifdef CONFIG_PER_CPU_SLAB
			free_slab_obj_per_cpu(s_ator, obj);
#else
			/*
			 * if this s_ator is illegal, this GUARD will be dead lock
			 * Remain Problem: may be added checker whether s_ator is
			 * belongs to g_sized_slab_ator which cause performance bad
			 */
			ator_lock(s_ator);
			free_slab_obj_in_desc(s_ator, obj, s_desc);
			ator_unlock(s_ator);
#endif
		}
	}
}

int slab_pool_push_memory(struct slab_pool *s_pool, uintptr_t start, size_t size)
{
	struct slab_desc *s_desc = NULL;
	struct slab_desc_range s_range;
	int ret = E_HM_OK;

	if (unlikely(s_pool == NULL)) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		ret = slab_check_range(start, size, (size_t)SLAB_PAGE_SIZE);
	}

	if (ret == E_HM_OK) {
		/* initial slab_desc_range */
		mem_zero_s(s_range);
		s_range.ori_start = start;
		s_range.ori_size = size;
		s_desc = slab_desc_init(start, &s_range);

		slab_pool_lock(s_pool);

		dlist_insert_head(&(s_pool->mem_list), &s_desc->node);

		/* update total_size free_size grown_size */
		(void)raw_atomic_ullong_add_relaxed(&s_pool->total_size, (uint64_t)s_range.ori_size);
		(void)raw_atomic_ullong_add_relaxed(&s_pool->free_size, (uint64_t)s_range.ori_size);
		(void)raw_atomic_ullong_add_relaxed(&s_pool->grown_size, (uint64_t)s_range.ori_size);
		slab_pool_unlock(s_pool);
	}

	return ret;
}

/* every time return only one start addr of slab_page */
uintptr_t slab_pool_pop_memory(struct slab_pool *s_pool)
{
	bool used = false;
	uintptr_t ori_start = 0;
	void *start_ptr = NULL;
	struct slab_desc *s_desc = NULL;

	if (likely(s_pool != NULL)) {
		slab_pool_lock(s_pool);
		if (!dlist_empty(&(s_pool->mem_list))) {
			s_desc = dlist_first_entry(&(s_pool->mem_list), struct slab_desc, node);

			used = slab_desc_is_used(s_desc);
			/* The slab page in mem list should be unused */
			if (used) {
				start_ptr = ulong_to_ptr(s_desc->s_range.ori_start, void);
				memm_slab_error_cb((uintptr_t)s_desc);
				hm_panic("slab page 0x%lx in mem list is in use, start is 0x%lx,"
					 "size is %zu, ator is %s\n", vaddr_of(s_desc),
					 vaddr_of(start_ptr), s_desc->s_range.ori_size, s_desc->ator->name);
			}

			ori_start = s_desc->s_range.ori_start;
			/* update total_size free_size and grown_size */
			(void)raw_atomic_ullong_sub_relaxed(&s_pool->total_size, (uint64_t)s_desc->s_range.ori_size);
			(void)raw_atomic_ullong_sub_relaxed(&s_pool->free_size, (uint64_t)s_desc->s_range.ori_size);
			(void)raw_atomic_ullong_sub_relaxed(&s_pool->grown_size, (uint64_t)s_desc->s_range.ori_size);

			dlist_del_entry(s_desc, struct slab_desc, node);
		}
		slab_pool_unlock(s_pool);
	}

	return ori_start;
}

uint32_t slab_slot_size(const struct slab_ator *ator)
{
	uint32_t ret_size = 0;
	if (likely(ator != NULL)) {
		ret_size = ator->obj_size;
	}
	return ret_size;
}

size_t slab_mem_size(struct slab_pool *s_pool, enum slab_size_type types)
{
	uint64_t ret_size = 0ULL;

	if (likely(s_pool != NULL)) {
		switch (types) {
		case SLAB_TOTAL_SIZE:
			ret_size = raw_atomic_ullong_read_relaxed(&s_pool->total_size);
			break;
		case SLAB_FREE_SIZE:
			ret_size = raw_atomic_ullong_read_relaxed(&s_pool->free_size);
			break;
		case SLAB_GROWN_SIZE:
			ret_size = raw_atomic_ullong_read_relaxed(&s_pool->grown_size);
			break;
		case SLAB_USED_SIZE:
			ret_size = raw_atomic_ullong_read_relaxed(&s_pool->total_size);
			ret_size -= raw_atomic_ullong_read_relaxed(&s_pool->free_size);
			break;
		default:
			ret_size = 0ULL;
			break;
		}
	}

	return (size_t)ret_size;
}

/* Get struct slab_info_s info from slab pool. */
void slab_dump_nolock(struct slab_info_s *info, struct slab_pool *s_pool)
{
	mem_zero_s(*info);
	info->total_slab_size = slab_mem_size(s_pool, SLAB_TOTAL_SIZE);
	info->free_slab_size = slab_mem_size(s_pool, SLAB_FREE_SIZE);
	info->grown_slab_size = slab_mem_size(s_pool, SLAB_GROWN_SIZE);
}

void slab_dump_total_info(struct slab_info_s *info, struct slab_pool *s_pool)
{
	slab_pool_lock(s_pool);
	slab_dump_nolock(info, s_pool);
	slab_pool_unlock(s_pool);
}

static void dump_slab_ator(const struct slab_ator *ator,
			   struct slab_ator_info_s *ator_info,
			   struct slab_slot_info_s *slot_info)
{
	uint32_t total_page_num = 0;
	uint32_t total_slot = 0;
	int rc;

	ator_info->in_use = 1;
	ator_info->slab_page_size = SLAB_PAGE_SIZE;
	ator_info->obj_size = ator->obj_size;
	ator_info->max_slot_count = ator->max_slot_count;
	ator_info->slab_page_hold = vatomic32_read_rlx(&ator->slab_page_hold);
	ator_info->flags = ator->flags;
	rc = strncpy_s(ator_info->name, SLAB_NAME_LEN, ator->name, SLAB_NAME_LEN - 1U);
	BUG_ON(rc != 0);

	ator_info->empty_num = ator->empty_num;
	ator_info->partial_num = ator->partial_num;
	ator_info->full_num = ator->full_num;

	total_page_num = ator->empty_num + ator->partial_num + ator->full_num;
	total_slot = total_page_num * ator->max_slot_count;
	ator_info->slot_used = vatomic32_read_rlx(&ator->used_slot_num);
	ator_info->slot_free = total_slot - ator_info->slot_used;
	/* statistics slot info */
	slot_info->all_slot_size += (size_t)ator_info->obj_size * (size_t)total_slot;
	slot_info->free_slot_size += (size_t)ator_info->obj_size * (size_t)ator_info->slot_free;
	slot_info->used_slot_size += (size_t)ator_info->obj_size * (size_t)ator_info->slot_used;
}

void slab_dump_ator(struct slab_ator *ator, struct slab_ator_info_s *ator_info,
		    struct slab_slot_info_s *slot_info)
{
	ator_lock(ator);
	if (vatomic32_read_rlx(&ator->slab_page_hold) != 0U) {
		dump_slab_ator(ator, ator_info, slot_info);
		if (ator->empty_num != ator_info->empty_num) {
			hm_error("slab %s in error state! ator->empty %u, info->empty %u\n",
				 ator->name, ator->empty_num, ator_info->empty_num);
		}
		if (ator->partial_num != ator_info->partial_num) {
			hm_error("slab %s in error state! ator->partial %u, info->partial %u\n",
				 ator->name, ator->partial_num, ator_info->partial_num);
		}
		if (ator->full_num != ator_info->full_num) {
			hm_error("slab %s in error state! ator->full %u, info->full %u\n",
				 ator->name, ator->full_num, ator_info->full_num);
		}
	}
	ator_unlock(ator);
}

uint32_t slab_get_ator_used_slot_num(struct slab_ator *ator, uint32_t flag)
{
	uint32_t used_slot_num = 0;

	if ((ator != NULL) && ((flag == 0) || (ator->flags & flag) != 0)) {
		used_slot_num = vatomic32_read_rlx(&ator->used_slot_num);
	}

	return used_slot_num;
}

size_t slab_get_ator_free_size(struct slab_ator *ator)
{
	size_t free_size = 0;

	if (ator != NULL) {
		uint32_t slab_page_hold = vatomic32_read_rlx(&ator->slab_page_hold);
		uint32_t used_slot_num = vatomic32_read_rlx(&ator->used_slot_num);
		free_size = (size_t)ator->obj_size * (ator->max_slot_count * slab_page_hold - used_slot_num);
	}

	return free_size;
}

const char *slab_ator_name_of(const struct slab_ator *ator)
{
	return (ator != NULL) ? ator->name : NULL;
}

bool slab_ptr_is_valid(struct slab_ator *ator, uintptr_t ptr)
{
	struct slab_ator *ptr_ator = NULL;
	struct slab_desc *s_desc = NULL;
	bool valid = true;

	if (ator == NULL) {
		valid = false;
	}
	/* check ator is equal */
	if (valid) {
		void *obj = slab_redzone_obj_to_slot(ptr);
		s_desc = slab_obj_to_slab_desc(obj, (size_t)SLAB_PAGE_SIZE);
		if (s_desc != NULL) {
			ptr_ator = s_desc->ator;
		}
		if (unlikely(ptr_ator != ator)) {
			valid = false;
		}
	}
	/* check ptr is not slab desc */
	if (valid) {
		if (unlikely(ptr < (ptr_to_ulong(s_desc) + sizeof(struct slab_desc)))) {
			valid = false;
		}
	}
	/* check ptr alignment */
	if (valid) {
		size_t obj_sz = ator->obj_size;
		size_t ptr_off = ptr - ptr_to_ulong(s_desc) - sizeof(struct slab_desc);
		if (unlikely(ptr_off % obj_sz != 0)) {
			valid = false;
		}
	}
	return valid;
}

int slab_desc_size_of(void)
{
	return sizeof(struct slab_desc);
}
