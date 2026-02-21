/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Implementations for per-cpu slab
 * Author: Huawei OS Kernel Lab
 * Create: Sat Feb 25 12:53:24 2023
 */
#include "slab_per_cpu.h"

#include <libmem/slab.h>
#include <libmem/redzone.h>

#include <libmem/utils.h>
#include <libmem/memm_slab.h>
#include <libmem/slab_utils.h>
#ifdef CONFIG_SLAB_DEBUG
#include <libmem/debug.h>
#endif
#ifdef CONFIG_SLAB_DEBUG_USAGE
#include <libmem/slab_track.h>
#endif
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <hongmeng/panic.h>
#include <hongmeng/compiler.h>
#include <hmkernel/compiler.h>
#include <libpreempt/preempt.h>
#include <libhwsecurec/securec.h>
#include <libhmsync/raw_mutex_guard.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define BATCHCOUNT_POLICY(limit) (((limit) + 1) / 2)
#define CACHE_LIMIT_LEVEL 4

#define SLOT_SIZE_FOR_VREGION MEMM_SLAB_SIZE_OF_VREGION
#define CACHE_LIMIT_FOR_VREGION 120
#define BATCHCOUNT_MAX (BATCHCOUNT_POLICY(CACHE_LIMIT_FOR_VREGION))
static const uint32_t g_cache_limit[CACHE_LIMIT_LEVEL] = {4U, 8U, 30U, 60U};
static const uint32_t g_cache_range[CACHE_LIMIT_LEVEL + 1] = {SLAB_PAGE_SIZE, 1024U, 512U, 256U, 0U};
static uint32_t g_num_cpus = 0U;

#ifdef CONFIG_SLAB_DOUBLE_CHECK
#define DOUBLE_FREE_CHECK_FLAG 0xAAAAAAAA
static int caches_contain_obj(struct array_cache *cache, const void *obj)
{
	int rc = SLAB_SLOT_USED;
	for (uint32_t i = 0; i < cache->remain; i++) {
		if (cache->array[i] == obj) {
			rc = SLAB_SLOT_FREE;
			break;
		}
	}
	return rc;
}

static int check_double_free(struct array_cache *cache, const void *obj)
{
	int ret = SLAB_SLOT_FREE;
	const uint32_t *temp = slab_redzone_slot_to_obj(obj);
	ret = (int)(*temp^DOUBLE_FREE_CHECK_FLAG);
	if (unlikely(ret)) {
		ret = caches_contain_obj(cache, obj);
	}
	return ret;
}

static void format_obj_content(void *obj)
{
	/* format the first four bytes with special pattern */
	uint32_t *temp = slab_redzone_slot_to_obj(obj);

	*temp = DOUBLE_FREE_CHECK_FLAG;
}
#endif /* CONFIG_SLAB_DOUBLE_CHECK */

uint32_t slab_ator_cache_limit(uint32_t obj_size)
{
	uint32_t limit = 0;
	for (int i = 0; i < CACHE_LIMIT_LEVEL; i++) {
		if ((g_cache_range[i] > obj_size) &&
		    (obj_size >= g_cache_range[i + 1])) {
			limit = g_cache_limit[i];
			break;
		}
	}
	/* empirical value */
	if (obj_size == SLOT_SIZE_FOR_VREGION) {
		limit = CACHE_LIMIT_FOR_VREGION;
	}
	return limit;
}

size_t slab_cache_pool_size(const uint32_t *array, size_t array_len)
{
	size_t total_size = 0;
	for(size_t i = 0; i < array_len; i++) {
		total_size += slab_ator_cache_limit(array[i]) * g_num_cpus * sizeof(void *);
	}
	return total_size;
}

void slab_cpu_num_setup(uint32_t num_cpus)
{
	if (g_num_cpus == 0U) {
		if ((num_cpus > 0) && (num_cpus <= SLAB_CACHE_NUM)) {
			g_num_cpus = num_cpus;
		} else {
			hm_panic("invalid number of cpu:%u\n", num_cpus);
		}
	} else {
		hm_panic("number of cpus set twice, current:%u, new:%u\n",
			 g_num_cpus, num_cpus);
	}
}

void slab_ator_cache_setup(struct slab_ator *s_ator, uintptr_t *cache_pool,
			   uint32_t limit)
{
	struct array_cache *caches = s_ator->caches;
	for(size_t i = 0; i < g_num_cpus; i++) {
		caches[i].limit = limit;
		caches[i].batchcount = BATCHCOUNT_POLICY(limit);
		caches[i].remain = 0;
		raw_mutex_init(&caches[i].lock);
		caches[i].array = (void **)*cache_pool;
		*cache_pool += limit * sizeof(void *);
	}
}

static void ator_cache_free_obj_nolock(struct slab_ator *s_ator, const void *obj)
{
	struct slab_desc *s_desc = NULL;
	if (likely(obj != NULL)) {
		/* search for slab desc from this obj */
		s_desc = slab_obj_to_slab_desc(obj, (size_t)SLAB_PAGE_SIZE);
		if (likely(!IS_INVALID_PTR(s_desc))) {
			/* judge this obj whether belong to this allocator */
			if (likely(s_desc->ator == s_ator)) {
				free_slab_obj_in_desc(s_ator, obj, s_desc);
			} else {
				/* wrong allocator */
				hm_panic("The allocator does not match the one of "
					 "the obj:(0x%lx)\n", ptr_to_ulong(obj));
			}
		} else {
			hm_panic("Obj to slab desc failed. "
				 "Maybe the obj:(0x%lx) does not belong to slab\n",
				 vaddr_of(obj));
		}
	}
}

static inline void ator_batch_free(struct slab_ator *s_ator, void **array, int cnt)
{
	int i;
	/*
	 * if this s_ator is illegal, this GUARD will be dead lock
	 * Remain Problem: may be added checker whether s_ator is
	 * belongs to g_sized_slab_ator which cause performance bad
	 */
	ator_lock(s_ator);
	for (i = 0; i < cnt; i++) {
#ifdef CONFIG_SLAB_DEBUG_USAGE
		clear_in_cache_mark(s_ator, array[i]);
#endif
		ator_cache_free_obj_nolock(s_ator, array[i]);
	}
	ator_unlock(s_ator);
}

void ator_cache_clear(struct slab_ator *s_ator)
{
	struct array_cache *cache = NULL;

	for (size_t i = 0; i < g_num_cpus; i++) {
		cache = &s_ator->caches[i];
		ator_batch_free(s_ator, cache->array, cache->remain);
		cache->remain = 0;
	}
}

static uint32_t ator_batch_alloc(struct slab_ator *s_ator, void **array, uint32_t cnt)
{
	struct slab_desc *s_desc = NULL;
	int rc = E_HM_OK;
	uint32_t batchcount;
	uint32_t idx;
	uint32_t i = 0;

	ator_lock(s_ator);
	rc = slab_ator_prepare_no_lock(s_ator);
	if (rc == E_HM_OK) {
		/* alloc a slot from partial */
		s_desc = dlist_first_entry(&s_ator->partial, struct slab_desc, node);
		if (IS_INVALID_PTR(s_desc)) {
			hm_panic("s_ator:(0x%lx), partial.prev:(0x%lx), partial.next:(0x%lx)\n"
				 "empty_num:(%u), partial_num:(%u), full_num:(%u)\n",
				 vaddr_of(s_ator), vaddr_of(s_ator->partial.prev), vaddr_of(s_ator->partial.next),
				 s_ator->empty_num, s_ator->partial_num, s_ator->full_num);
		}
		if (s_desc->magic != SLAB_MAGIC || s_desc->used_num >= s_ator->max_slot_count) {
			memm_slab_error_cb((uintptr_t)s_desc);
			hm_panic("s_ator:(0x%lx), partial.prev:(0x%lx), partial.next:(0x%lx)\n"
				 "empty_num:(%u), partial_num:(%u), full_num:(%u)\n"
				 "max_slot_count:(%u), slab_page_hold:(%u), name:(%s), obj_size:(%d)",
				 ptr_to_ulong(s_ator), ptr_to_ulong(s_ator->partial.prev), ptr_to_ulong(s_ator->partial.next),
				 s_ator->empty_num, s_ator->partial_num, s_ator->full_num,
				 s_ator->max_slot_count, vatomic32_read_rlx(&s_ator->slab_page_hold), s_ator->name,
				 s_ator->obj_size);
		}
		batchcount = min(cnt, s_ator->max_slot_count - s_desc->used_num);
		for (; i < batchcount; i++) {
			/* finds the next zero from the given location */
			idx = slab_fetch_idx_in_freemap(s_desc->free_map, s_ator->max_slot_count);
			/* partial list must have at least one free slot */
			if (unlikely(idx >= s_ator->max_slot_count)) {
				memm_slab_error_cb((uintptr_t)s_desc);
				hm_panic("s_desc:(0x%lx), idx:(%"PRIu32"), "
					 "obj_size:(%"PRIu32"), sizeof(struct slab_desc):(%zu)\n"
					 "slab in error stat, partial list must have one free slot\n",
					 vaddr_of(s_desc), idx,
					 s_desc->ator->obj_size, sizeof(struct slab_desc));
			}
			(void)raw_bitmap_set_bit(s_desc->free_map, s_ator->max_slot_count, idx);
			array[i] = ulong_to_ptr(slot_idx_to_ptr(s_ator, s_desc, idx), void);
#ifdef CONFIG_SLAB_DEBUG_USAGE
			set_in_cache_mark(s_ator, array[i]);
#endif
			s_desc->used_num++;
			vatomic32_inc(&s_ator->used_slot_num);
		}
		if (s_desc->used_num == s_ator->max_slot_count) {
			/* remove from partial and into full */
			move_partial_to_full(s_ator, s_desc);
		}
	}
	ator_unlock(s_ator);
	return i;
}

#ifdef CONFIG_SLAB_DOUBLE_CHECK
static inline void dfc_malloc_hook(void *p)
{
	uint32_t *tmp = slab_redzone_slot_to_obj(p);
	/* clear the first four bytes */
	mem_zero_s(*tmp);
}

static inline void dfc_free_hook(struct array_cache *c, void *p, uint32_t sz)
{
	int double_free = check_double_free(c, p);
	if (unlikely(double_free == SLAB_SLOT_FREE)) {
		hm_panic("slab_ator obj_size:(%"PRIu32"), double free for obj:(0x%lx)\n",
			 sz, ptr_to_ulong(p));
	} else {
		format_obj_content(p);
	}
}
#else
static inline void dfc_malloc_hook(void *p)
{
	UNUSED(p);
}

static inline void dfc_free_hook(struct array_cache *c, void *p, uint32_t sz)
{
	UNUSED(c, p, sz);
}
#endif

#ifdef CONFIG_USE_PREEMPT_PROCESS
void *slab_alloc_per_cpu(struct slab_ator *s_ator)
{
	struct array_cache *cache = NULL;
	void *obj = NULL;
	int cpu;
	uint32_t cnt;

	if (likely(s_ator != NULL)) {
		cpu = preempt_process_disable();
		cache = &s_ator->caches[cpu];
		while (cache->remain == 0) {
			/* it should be cautious to enlarge size of array */
			void *array[BATCHCOUNT_MAX];
			cnt = ator_batch_alloc(s_ator, array, cache->batchcount);
			/* finish loop if it fails to allocate from slab */
			if (cnt == 0) {
				break;
			}
			for (; cnt > 0 && cache->remain < cache->limit; cnt--) {
				cache->array[(cache->remain)++] = array[cnt - 1];
			}
			if (cnt > 0) {
				ator_batch_free(s_ator, array, cnt);
			}
		}
		if (cache->remain > 0) {
			obj = cache->array[--(cache->remain)];
			dfc_malloc_hook(obj);
		}
		preempt_process_enable();
#ifdef CONFIG_SLAB_DEBUG
		if (obj) {
			obj = slab_init_debug_zone(s_ator, obj);
		}
#endif
	}

	return obj;
}
#else
void *slab_alloc_per_cpu(struct slab_ator *s_ator)
{
	struct array_cache *cache = NULL;
	void *obj = NULL;
	int cpu;
	uint32_t cnt;

	if (likely(s_ator != NULL)) {
		cpu = preempt_process_disable();
		cache = &s_ator->caches[cpu];
		while (cache->remain == 0) {
			preempt_process_enable();
			/* it should be cautious to enlarge size of array */
			void *array[BATCHCOUNT_MAX];
			cnt = ator_batch_alloc(s_ator, array, cache->batchcount);
			cpu = preempt_process_disable();
			/* update cache as cpu may be swithed when preempt enable */
			cache = &s_ator->caches[cpu];
			/* finish loop if it fails to allocate from slab */
			if (cnt == 0) {
				break;
			}
			for (; cnt > 0 && cache->remain < cache->limit; cnt--) {
				cache->array[(cache->remain)++] = array[cnt - 1];
			}
			if (cnt > 0) {
				preempt_process_enable();
				ator_batch_free(s_ator, array, cnt);
				cpu = preempt_process_disable();
				/* update cache as cpu may be swithed when preempt enable */
				cache = &s_ator->caches[cpu];
			}
		}
		if (cache->remain > 0) {
			obj = cache->array[--(cache->remain)];
			dfc_malloc_hook(obj);
		}
		preempt_process_enable();
		BUG_ON((obj != NULL) && PAGE_ALIGNED(obj));
#ifdef CONFIG_SLAB_DEBUG
		if (obj) {
			obj = slab_init_debug_zone(s_ator, obj);
		}
#endif
	}

	return obj;
}
#endif

void free_slab_obj_per_cpu(struct slab_ator *s_ator, void *obj)
{
	struct array_cache *cache = NULL;
	int cpu;
	int i;

	if (likely(s_ator != NULL)) {
		cpu = preempt_process_disable();
		cache = &s_ator->caches[cpu];
		dfc_free_hook(cache, obj, s_ator->obj_size);
		if (cache->remain < cache->limit) {
			cache->array[(cache->remain)++] = obj;
			preempt_process_enable();
		} else {
			void *array[BATCHCOUNT_MAX] __uninitialized;
			for (i = 0; i < (int)cache->batchcount; i++) {
				array[i] = cache->array[--(cache->remain)];
			}
			cache->array[(cache->remain)++] = obj;
			preempt_process_enable();
			ator_batch_free(s_ator, array, i);
		}
	}
}
