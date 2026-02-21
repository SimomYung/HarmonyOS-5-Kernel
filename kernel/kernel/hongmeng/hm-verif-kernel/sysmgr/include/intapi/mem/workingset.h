/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: anon refault implement
 * Author: Huawei OS Kernel Lab
 * Create: Mon March 11 18:20:11 2024
 */
#ifndef SYSMGR_MEMMGR_WORKINGSET_H
#define SYSMGR_MEMMGR_WORKINGSET_H

#include <intapi/mem/page.h>

/*
 * Generate shadow like following format:
 * |       eviction      | memcg  |node|workingset|type|
 * |---------------------|--------|----|----------|----|
 *            42             16     3        1      2   (bits)
 */
#define SHADOW_SHIFT_TYPE         (0)
#define SHADOW_SHIFT_WORKINGSET   (2)
#define SHADOW_SHIFT_NODE         (3)
#define SHADOW_SHIFT_MEMCG        (6)
#define SHADOW_SHIFT_EVICTION_AGE (22)

#define SHADOW_MASK_TYPE          (0x3)
#define SHADOW_MASK_WORKINGSET    (0x1)
#define SHADOW_MASK_NODE          (0x7)
#define SHADOW_MASK_MEMCG         (0xffff)
#define SHADOW_MASK_EVICTION_AGE  (~0UL >> SHADOW_SHIFT_EVICTION_AGE)

#define SHADOW_PAGE_NORMAL        (0x0) /* radix tree item is a normal page */
#define SHADOW_PAGE_SHADOW        (0x1) /* radix tree item is a shadow page */
#define SHADOW_PAGE_SWAP          (0x3) /* radix tree item is a swap page */

#define PERCENT                     100
#define LRU_SIZE_DIVISOR            4
#define COST_VALUE_DIVISOR          2

static inline bool cache_is_normal_page(const void *item)
{
	return (item != NULL) && (((uintptr_t)item & SHADOW_MASK_TYPE) == SHADOW_PAGE_NORMAL);
}

static inline bool cache_is_shadow_page(const void *item)
{
	return (item != NULL) && (((uintptr_t)item & SHADOW_MASK_TYPE) == SHADOW_PAGE_SHADOW);
}

static inline bool cache_is_swap_page(const void *item)
{
	return (item != NULL) && (((uintptr_t)item & SHADOW_MASK_TYPE) == SHADOW_PAGE_SWAP);
}

#ifdef CONFIG_SWAP_REFAULT
extern void workingset_activation(struct page_s *page);
extern void workingset_age_nonresident(struct page_s *page);
extern void *workingset_eviction(struct page_s *page);
extern void workingset_refault(struct page_s *page, void *shadow);
#else
static inline void workingset_activation(struct page_s *page)
{
	UNUSED(page);
	return;
}

static inline void workingset_age_nonresident(struct page_s *page)
{
	UNUSED(page);
	return;
}

static inline void *workingset_eviction(struct page_s *page)
{
	UNUSED(page);
	return NULL;
}

static inline void workingset_refault(struct page_s *page, void *shadow)
{
	UNUSED(page, shadow);
	return;
}
#endif

#ifdef CONFIG_SWAP_LRU_COST
extern void lru_note_cost(struct page_s *page);
#else
static inline void lru_note_cost(struct page_s *page)
{
	UNUSED(page);
	return;
}
#endif

#ifdef CONFIG_REFAULT_SKIP
extern bool get_area_anon_refault_status(void);
#else
static inline bool get_area_anon_refault_status(void)
{
	return false;
}
#endif
#endif
