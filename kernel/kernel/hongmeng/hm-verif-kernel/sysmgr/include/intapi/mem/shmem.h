/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: file shmem memory
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 10 20:46:42 2023
 */
#ifndef INTAPI_MEM_SHMEM_H
#define INTAPI_MEM_SHMEM_H

#include <libstrict/strict.h>
#include <intapi/mem/page.h>
#include <intapi/mem/lru.h>

#if defined(CONFIG_SWAP_SHMEM) && defined(CONFIG_DEBUG_BUILD)
#define SHMEM_BUG_ON(page, cond, fmt, ...) \
	do { \
		BUG_ON((cond), "page=%lx index=%lu flags=%x,%x refcnt=%u mapcnt=%u vrg=%lx memcg=%lx empty=%u " fmt, \
			(unsigned long)(page), (page)->index, \
			(page)->_flags.high_flags_bits, (page)->_flags.low_flags_bits, \
			raw_refcnt_nr_counter(&(page)->refcnt), page_mapcnt_of(page), \
			(unsigned long)(page)->vrg, (unsigned long)(page)->memcg, \
			dlist_empty(&(page)->dnode), ##__VA_ARGS__); \
	} while(0)
#else
#define SHMEM_BUG_ON(page, cond, fmt, ...) do { } while(0)
#endif

#define shmem_page_assert(page) SHMEM_BUG_ON((page), page_is_file_shmem(page), "shmem page never come here\n")
#define shmem_page_check(page) SHMEM_BUG_ON((page), !page_is_file_shmem(page), "other page never come here\n")
#define shmem_zone_assert(page) \
	SHMEM_BUG_ON((page), !shmem_page_check_zone(page), "shmem page from HKIP/CMA/KERN zone\n")

#ifdef CONFIG_SWAP_SHMEM
int  vfs_reclaim_shmem_page(struct page_s *page, struct dlist_node *fail, bool force);
void vfs_add_page_to_protect_lru(struct page_s *page);
void vfs_del_page_from_protect_lru(struct page_s *page);
void lru_shmem_init_percpu_pagevecs(int cpu);
void shmem_percpu_add_lru(struct page_s *page);
void shmem_percpu_drain_lrus(struct page_s *page);
void shmem_percpu_move_page(struct page_s *page);
bool shmem_percpu_move_check(struct page_s *page);
void shmem_page_lru_lock(struct page_s *page);
void shmem_page_lru_unlock(struct page_s *page);
void shmem_page_lru_del_locked(struct page_s *page, struct lru_cache_s *lru_cache);
void shmem_page_lru_add_locked(struct page_s *page, struct lru_cache_s *lru_cache);
void shmem_page_lru_add_tail(struct page_s *page);
int  shmem_page_lru_remove_locked(struct page_s *page);
int  shmem_page_lru_remove(struct page_s *page, bool set_reclaim);
void shmem_page_move_to_active(struct page_s *page);
void shmem_page_shrink_active_locked(struct page_s *page);
void shmem_page_isolate_locked(struct page_s *page, struct lru_cache_s *lru_cache);
void shmem_page_putback_locked(struct page_s *page, struct lru_cache_s *lru_cache);
int  shmem_page_swap_writepage(struct page_s *page, unsigned long *handle);
int  shmem_page_swap_readpage(struct page_s *page, unsigned long handle);
int  shmem_page_swap_freepage(unsigned long handle);
bool shmem_page_is_hkip_zone(struct page_s *page);
bool shmem_page_is_cma_zone(struct page_s *page);
bool shmem_page_is_kern_zone(struct page_s *page);
bool shmem_page_check_zone(struct page_s *page);
void shmem_page_set_swappable(struct page_s *page);
void shmem_page_clear_swappable(struct page_s *page);
int  shmem_page_memcg_set(struct page_s *page, uint32_t cnode_idx, uintptr_t vr);
void shmem_page_memcg_clear(struct page_s *page);
int  shmem_page_reclaim(struct page_s *page, struct dlist_node *fail, bool force);
#else
static inline void lru_shmem_init_percpu_pagevecs(unsigned int cpuid)
{
	UNUSED(cpuid);
}
static inline void shmem_percpu_add_lru(struct page_s *page)
{
	UNUSED(page);
}
static inline void shmem_percpu_drain_lrus(struct page_s *page)
{
	UNUSED(page);
}
static inline void shmem_percpu_move_page(struct page_s *page)
{
	UNUSED(page);
}
static inline bool shmem_percpu_move_check(struct page_s *page)
{
	UNUSED(page);
	return false;
}
static inline void shmem_page_lru_lock(struct page_s *page)
{
	UNUSED(page);
}
static inline void shmem_page_lru_unlock(struct page_s *page)
{
	UNUSED(page);
}
static inline void shmem_page_lru_del_locked(struct page_s *page, struct lru_cache_s *lru_cache)
{
	UNUSED(page, lru_cache);
}
static inline void shmem_page_lru_add_locked(struct page_s *page, struct lru_cache_s *lru_cache)
{
	UNUSED(page, lru_cache);
}
static inline void shmem_page_lru_add_tail(struct page_s *page)
{
	UNUSED(page);
}
static inline int shmem_page_lru_remove_locked(struct page_s *page)
{
	UNUSED(page);
	return E_HM_NOSYS;
}
static inline int shmem_page_lru_remove(struct page_s *page, bool set_reclaim)
{
	UNUSED(page);
	return E_HM_NOSYS;
}
static inline void shmem_page_move_to_active(struct page_s *page)
{
	UNUSED(page);
}
static inline void shmem_page_shrink_active_locked(struct page_s *page)
{
	UNUSED(page);
}
static inline void shmem_page_isolate_locked(struct page_s *page, struct lru_cache_s *lru_cache)
{
	UNUSED(page, lru_cache);
}
static inline void shmem_page_putback_locked(struct page_s *page, struct lru_cache_s *lru_cache)
{
	UNUSED(page, lru_cache);
}
static inline int shmem_page_swap_writepage(struct page_s *page, unsigned long *handle)
{
	UNUSED(page, handle);
	return E_HM_NOSYS;
}
static inline int shmem_page_swap_readpage(struct page_s *page, unsigned long handle)
{
	UNUSED(page, handle);
	return E_HM_NOSYS;
}
static inline int shmem_page_swap_freepage(unsigned long handle)
{
	UNUSED(handle);
	return E_HM_NOSYS;
}
static inline bool shmem_page_is_hkip_zone(struct page_s *page)
{
	UNUSED(page);
	return false;
}
static inline bool shmem_page_is_cma_zone(struct page_s *page)
{
	UNUSED(page);
	return false;
}
static inline bool shmem_page_is_kern_zone(struct page_s *page)
{
	UNUSED(page);
	return false;
}
static inline bool shmem_page_check_zone(struct page_s *page)
{
	UNUSED(page);
	return false;
}
static inline void shmem_page_set_swappable(struct page_s *page)
{
	UNUSED(page);
}
static inline void shmem_page_clear_swappable(struct page_s *page)
{
	UNUSED(page);
}
static inline int shmem_page_memcg_set(struct page_s *page, uint32_t cnode_idx, uintptr_t vr)
{
	UNUSED(page, cnode_idx, vr);
	return E_HM_NOSYS;
}
static inline void shmem_page_memcg_clear(struct page_s *page)
{
	UNUSED(page);
}
static inline int shmem_page_reclaim(struct page_s *page, struct dlist_node *fail, bool force)
{
	UNUSED(page, fail, force);
	return E_HM_NOSYS;
}
#endif

#endif /* INTAPI_MEM_SHMEM_H */
