/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Declare interface of page
 * Author: Huawei OS Kernel Lab
 * Create: Tue Jul 18 11:08:48 UTC 2023
 */

#ifndef __SYSMGR_INTAPI_MEM_PAGE_H__
#define __SYSMGR_INTAPI_MEM_PAGE_H__

#include <hmasm/types.h>
#include <hongmeng/types.h>
#include <hongmeng/compiler.h>

#include <intapi/ctrlmem/ubdqueue.h>
#include <libhmsync/raw_scopedptr.h>
#include <libhmebr/ebr.h>
#include <libmem/utils.h>
#include <intapi/mem/lru.h>
#include <intapi/mem/stat.h>
#include <intapi/mem/vmpressure.h>
#include <intapi/res/memcg.h>
#include <lib/dlist.h>
#include <intapi/mem/paf.h>
#include <intapi/mem/ama.h>
#include <vsync/spinlock.h>

struct vrgroup_s;
struct memcg_s;
struct vspace_s;
enum kobj_type;
struct process_s;

typedef union pg_lock {
	vatomic32_t v;
	struct {
		uint32_t lock    : 7;
		uint32_t ctrlmem_lock : 1;
		uint32_t waiters : 24;
	} _v;
} pg_lock_t;

struct res_group;
struct res_controller;
/*
 * A res_obj (resource object) is embedded in a particular resource,
 * such as memory or cpu.
 */
struct res_obj {
	struct raw_refcnt refcnt;
	struct res_group *rgrp;
	struct res_controller *rctr;
	struct ebr_entry entry;
};

/*
 * Limits in number of pages.
 * It should be unsigned long, but we don't have atomic
 * functions for ulong, so we just use long.
 */
struct mem_limits {
	raw_atomic_long_t min;
	raw_atomic_long_t low;
	raw_atomic_long_t high;
	raw_atomic_long_t max;
};

struct mem_stats {
	raw_atomic_long_t nr_anon_pages;
	/* number of pages FS used for this memcg and its descendent memcgs */
	raw_atomic_long_t nr_file_pages;
	raw_atomic_long_t nr_pmem_pages;
	raw_atomic_long_t nr_shmm_pages;
};

#ifdef CONFIG_HYPERHOLD
#define MEM_CGROUP_NAME_MAX_LEN	100
#define RECLAIM_MAX_LEVEL_NUM	10

struct memcg_reclaim_param_s {
	char name[MEM_CGROUP_NAME_MAX_LEN];
	raw_atomic_int_t app_score;
	raw_atomic_int_t ub_zram2ufs_ratio;
	raw_atomic_int_t ub_mem2zram_ratio;
	raw_atomic_int_t ub_ufs2zram_ratio;
	raw_atomic_int_t refault_threshold;
#ifdef CONFIG_HYPERHOLD_NC
	raw_atomic_int_t nocompress_limit_mb;
	raw_atomic_int_t force_shrink_nocompress_ratio;
	raw_atomic_int_t lru_anon_nocompress_ratio;
#endif
};
#endif

#ifdef CONFIG_HISI_SYSMGR_PARA_RECLAIM
#define IN_PARA_ANON_RECLAIM 1
struct para_anon_entry {
	struct dlist_node node;
	struct memcg_s *memcg;
	struct scan_control *sc;
	raw_atomic_int_t in_para_reclaim;
	size_t nr_to_reclaim;
};
#endif

struct memcg_s {
	struct res_obj robj;
	struct mem_limits limits;
	struct mem_stats stats;
	raw_atomic_long_t usage;
	/* number of pages FS used for this memcg directly */
	raw_atomic_long_t fs_usage;
	struct lru_s mem_lru[AMA_MAX_NODES];
	struct vmpressure_s vmpressure;
	unsigned int id;
#ifdef CONFIG_HYPERHOLD
	struct memcg_reclaim_param_s reclaim_param;
	struct dlist_node app_score_list_node;
	bool in_app_score_list;
	raw_atomic_long_t gpu_reclaimed;
#endif
#ifdef CONFIG_HISI_SYSMGR_PARA_RECLAIM
	struct para_anon_entry anon_entry;
#endif
#ifdef CONFIG_HYPERHOLD_NC
	bool nocompress;
#endif
};

#define UNMAP_IGNORE_MLOCK	0x01U /* ignore mlock */

struct page_s {
#ifdef CONFIG_PAGE_META_MERGE
	union {
		uint64_t flags;
		struct {
#ifdef CONFIG_BIG_ENDIAN
			int high_flags_bits;
			int low_flags_bits;
#else
			int low_flags_bits;
			int high_flags_bits;
#endif
		} _flags;
	};
#else
	uint32_t flags;
	pg_lock_t lock;
#endif
	struct raw_refcnt_nr refcnt;
	raw_atomic_int_t mapcnt;
	union {
		struct vrgroup_s *vrg;
		/* pcache page info */
		struct {
			__u16 p_cpu_id;
			__u16 p_list_type;
		};
		/*
		* For now, the lock is only used in
		* type of page table when do page fault
		* process to control the concurrency
		* of ptes write in page table page.
		*/
		spinlock_t spinlock;
	};
#ifdef CONFIG_PAGE_META_MERGE
	uint64_t index;
#else
	uint64_t pos;
#endif
	union {
		struct dlist_node dnode;
		struct page_s *l2_meta;
		struct __ctrlmem_ubdq_node_s ubdqnode;
		struct {
			void *backlink;
			struct page_s *next_page;
		};
	};
#ifdef CONFIG_RESMGR_MEMORY
	struct memcg_s *memcg;
#endif
	/* slab may use this to acquire the virtual address when page is slaballoc */
	unsigned long private_data;
};

static inline raw_atomic_int_t *__page_mapcnt(struct page_s *pg)
{
	return (pg != NULL) ? &pg->mapcnt : NULL;
}

static inline unsigned int page_mapcnt_of(struct page_s *pg)
{
	raw_atomic_int_t *__mapcnt = __page_mapcnt(pg);
	return (__mapcnt != NULL) ? (unsigned int)raw_atomic_int_read_relaxed(__mapcnt) : 0U;
}

/*
 * Get page by sysmgr linear virtual address
 *
 * @param va	[I] Virtual address of sysmgr linear memory
 *
 * @return	The page corresponding to the virtual address if success,
 *		otherwise, return NULL.
 */
struct page_s *page_from_vaddr(uintptr_t va);

/*
 * Get page by sysmgr virtual address
 *
 * @param va	[I] Virtual address of sysmgr
 *
 * @return	The page corresponding to the virtual address if success,
 *		otherwise, return NULL.
 */
struct page_s *page_from_sys_vaddr(uintptr_t va);

__no_sanitize_thread void page_lock(struct page_s *pg);
__no_sanitize_thread int page_lock_timeout(struct page_s *pg, unsigned long long *timeout);
__no_sanitize_thread int page_trylock(struct page_s *pg);
__no_sanitize_thread void page_unlock(struct page_s *pg);
bool sysmgr_page_locked(struct page_s *pg);

#ifdef CONFIG_PAGE_META_MERGE
#define PAGE_FLAG_CAST(x)	((uint64_t)(x))
#define PAGE_FLAG_MASK		0xFFFFFFFF0310FFF0
#else
#define PAGE_FLAG_CAST(x)	((uint32_t)(x))
#define PAGE_FLAG_MASK		0x0310FFF0
#endif

#define PAGE_TYPE_SHIFT		0U
#define PAGE_ZONE_SHIFT		21U

/*
 *     5    3   3         17          4
 * |-----|---|----|----------------|----|
 * |<order>|F|ZONE|<- page_flags ->|TYPE|
 * For now we only use the lower 27 bits for page flags,
 * when PAGE_META_MERGE is off. Both lower 27 bits and
 * higher 32 bits will be used for page flags, with
 * PAGE_META_MERGE on.
 */
#define PAGE_ZONE_MASK		PAGE_FLAG_CAST(0x00E00000)

/* page common flags */
#define PAGE_F_IOFASTRSV	PAGE_FLAG_CAST(0x01000000)    /* page is alloc from iofast alloc */
#define PAGE_F_FILE_SWAPPABLE	PAGE_F_IOFASTRSV              /* page is file shmem swappable, reuse with iofastrsv */
#define PAGE_F_SHMMBACKED	PAGE_FLAG_CAST(0x02000000)    /* page is shmm-backed page */
#define PAGE_F_MIGRATED		PAGE_FLAG_CAST(0X04000000)    /* page is in migrated dlist */
#define PAGE_F_COMPOUND		PAGE_FLAG_CAST(0x00000010)    /* compound page */
#define PAGE_F_MLOCKED		PAGE_FLAG_CAST(0x00000020)
#ifndef CONFIG_PAGE_META_MERGE
#define PAGE_F_DIRTY		PAGE_FLAG_CAST(0x00000040)
#endif
#define PAGE_F_REFERENCED	PAGE_FLAG_CAST(0x00000080)    /* recently accessed page */
#define PAGE_F_LRU		PAGE_FLAG_CAST(0x00000100)    /* LRU page */
#define PAGE_F_ACTIVE		PAGE_FLAG_CAST(0x00000200)    /* active LRU page , shared with PAGE_F_NONCACHE */
#define PAGE_F_ISOLATED		PAGE_FLAG_CAST(0x00000400)    /* isolated LRU page */
#define PAGE_F_INEVICTABLE	PAGE_FLAG_CAST(0x00000800)    /* inevictable LRU page */
#ifdef CONFIG_MEM_PURGEABLE
#define PAGE_F_PURGEABLE	PAGE_FLAG_CAST(0x00002000)    /* mem purgeable flags for terminal */
#endif

#define MIGRATE_FLAG_MASK	PAGE_FLAG_CAST(0x00002060)    /* inherit page flags when migrate */
/*
 * the dcache clean flag occupied by
 * PURGEABLE which only used in anon page
 * and can be reused, is aim to file page
 */
#define PAGE_F_DCACHE_CLEAN	PAGE_FLAG_CAST(0x00002000)    /* page is dcache clean */
#define PAGE_F_SWAP		PAGE_FLAG_CAST(0x00004000)    /* page is being swapped */
#define PAGE_F_UNMOVABLE	PAGE_FLAG_CAST(0x00008000)    /* page can not be migrated */
#define PAGE_F_VALID		PAGE_FLAG_CAST(0x00010000)    /* valid or not of current meta */
#define PAGE_F_BUDDY		PAGE_FLAG_CAST(0x00020000)    /* page is in buddy */

/* linear map of current raw page is noncache, flag shared with active */
#define PAGE_F_NONCACHE		PAGE_FLAG_CAST(0x00000200)

#define PAGE_F_DEGRADED		PAGE_FLAG_CAST(0x00040000)    /* page in main array that is degraded to secondary */
#define PAGE_F_SECONDARY	PAGE_FLAG_CAST(0x00080000)    /* page in secondary array that is dynamically generated */
#define PAGE_F_DYNAMIC		PAGE_FLAG_CAST(0x000C0000)    /* page is populated into the dynamic page array */
#define PAGE_ARRAY_MASK		PAGE_FLAG_CAST(0x000C0000)

#define PAGE_F_LAZYFREE		PAGE_FLAG_CAST(0x00100000)

/* flags to clear when free hugetlb page to ator, need to save order/zid/compound/valid/tid */
#define PAGE_HGTLB_RESET_MASK (PAGE_FLAG_MASK & (~PAGE_F_COMPOUND))

#if defined(CONFIG_KASAN_DUMPSTACK) || defined(CONFIG_SLAB_VIRTUAL)
/* When kasan is enabled, reuse PAGE_F_LAZYFREE  to mark the memory allocated by the slab
 * PAGE_F_LAZYFREE is used for anonymous pages. Using lazyfree to mark slab pages does not conflict.
 */
#define PAGE_F_SLABALLOC	PAGE_F_LAZYFREE
#endif

raw_static_assert(NR_ZONE <= (PAGE_ZONE_MASK >> PAGE_ZONE_SHIFT), NR_ZONE_too_large);

#ifdef CONFIG_DEBUG_BUILD
void page_set_flag_check(struct page_s *pg, uint64_t flag);
void page_clear_flag_check(struct page_s *pg, uint64_t flag);
#else /* !CONFIG_DEBUG_BUILD */
static inline void page_set_flag_check(struct page_s *pg, uint64_t flag)
{
	UNUSED(flag, pg);
}

static inline void page_clear_flag_check(struct page_s *pg, uint64_t flag)
{
	UNUSED(flag, pg);
}
#endif /* CONFIG_DEBUG_BUILD */

#ifdef CONFIG_PAGE_META_MERGE
int page_set_file_map_no_stat(struct page_s *pg, uintptr_t _vr, uintptr_t va, bool hkip_check, bool *first_load);
void page_set_file_map(struct page_s *pg, uintptr_t _vr, uintptr_t va, bool hkip_check, bool *first_load);

/*
 *              32                              32
 * |-----------------------------|----------------------------|
 * |-------high_flags_bits-------|-------low_flags_bits-------|
 */

/*
 * fscache page flags
 * MAKE SURE fscache page flags are on high 32 bits,
 * when page meta merge is on, coz fs need fs-related
 * page flags only sometimes.
 */
#define FSCACHE_PAGE_FLAG_SHIFT		32
#define PAGE_F_DIRTY		((0x00000001UL) << FSCACHE_PAGE_FLAG_SHIFT)
#define PAGE_F_UPTODATE		((0x00000002UL) << FSCACHE_PAGE_FLAG_SHIFT)
#define PAGE_F_CPOOL		((0x00000004UL) << FSCACHE_PAGE_FLAG_SHIFT)
/* Use 2 bits for lock */
#define PAGE_BITLOCK_LOCKBIT		4
#define PAGE_BITLOCK_WAITBIT		3
#define SYSMGR_PAGE_BITLOCK_LOCKBIT	(PAGE_BITLOCK_LOCKBIT + FSCACHE_PAGE_FLAG_SHIFT)
#define SYSMGR_PAGE_BITLOCK_WAITBIT	(PAGE_BITLOCK_WAITBIT + FSCACHE_PAGE_FLAG_SHIFT)
#define PAGE_BITLOCK_LOCK		((1UL << SYSMGR_PAGE_BITLOCK_LOCKBIT))
#define PAGE_BITLOCK_WAIT		((1UL << SYSMGR_PAGE_BITLOCK_WAITBIT))

#define PAGE_F_PRIVATE		((0x00000020UL) << FSCACHE_PAGE_FLAG_SHIFT)
#define PAGE_F_NOLRU		((0x00000040UL) << FSCACHE_PAGE_FLAG_SHIFT)
#define PAGE_F_WRITEBACK	((0x00000080UL) << FSCACHE_PAGE_FLAG_SHIFT)
#define PAGE_F_WAITER		((0x00000100UL) << FSCACHE_PAGE_FLAG_SHIFT)
#define PAGE_F_ERROR		((0x00000200UL) << FSCACHE_PAGE_FLAG_SHIFT)
#define PAGE_F_NOBACKEND	((0x00000400UL) << FSCACHE_PAGE_FLAG_SHIFT)

#define PAGE_FLAGS_LOCK_MASK	(PAGE_BITLOCK_LOCK | PAGE_BITLOCK_WAIT)

#define PAGE_F_FREEING		((0x00000800UL) << FSCACHE_PAGE_FLAG_SHIFT)
#define PAGE_F_READAHEAD	((0x00001000UL) << FSCACHE_PAGE_FLAG_SHIFT)
#define PAGE_F_ASYNC_RQ		((0x00002000UL) << FSCACHE_PAGE_FLAG_SHIFT)
#define PAGE_F_ASYNC		((0x00004000UL) << FSCACHE_PAGE_FLAG_SHIFT)
#define PAGE_F_CHECKED		((0x00008000UL) << FSCACHE_PAGE_FLAG_SHIFT)
#define PAGE_F_BHALIGNED	((0x00010000UL) << FSCACHE_PAGE_FLAG_SHIFT)
#define PAGE_F_LRU_FS		((0x00020000UL) << FSCACHE_PAGE_FLAG_SHIFT)
#define PAGE_F_PERCPU		((0x00040000UL) << FSCACHE_PAGE_FLAG_SHIFT)
#define PAGE_F_RECLAIM		((0x00080000UL) << FSCACHE_PAGE_FLAG_SHIFT)
#define PAGE_F_UNEVICT		((0x00100000UL) << FSCACHE_PAGE_FLAG_SHIFT)
#define PAGE_F_ACTIVE_FS	((0x00200000UL) << FSCACHE_PAGE_FLAG_SHIFT)
#define PAGE_F_REFERENCED_FS	((0x00400000UL) << FSCACHE_PAGE_FLAG_SHIFT)
#define PAGE_F_PREDIRTY		((0x00800000UL) << FSCACHE_PAGE_FLAG_SHIFT)
#define PAGE_F_PROTECT_LRU	((0x01000000UL) << FSCACHE_PAGE_FLAG_SHIFT)
#define PAGE_F_LACTIVE		((0x02000000UL) << FSCACHE_PAGE_FLAG_SHIFT)
#define PAGE_F_VERIFIED		((0x04000000UL) << FSCACHE_PAGE_FLAG_SHIFT)
#define PAGE_F_SPOOL		((0x08000000UL) << FSCACHE_PAGE_FLAG_SHIFT)
#define PAGE_F_MRECLAIM		((0x10000000UL) << FSCACHE_PAGE_FLAG_SHIFT)
#define PAGE_F_SWAP_RAWPAGE	PAGE_F_MRECLAIM /* page swap with rawpage mode, reuse with PAGE_F_MRECLAIM */
#define PAGE_F_MAX		((0x20000000UL) << FSCACHE_PAGE_FLAG_SHIFT)
#define FSCACHE_PAGE_SHIFT_MAX		29

#define PAGE(lowercase_flag, uppercase_flag)						\
static inline bool page_is_##lowercase_flag(const struct page_s *pg)			\
{											\
	BUG_ON_D(pg == NULL);								\
	return ((vatomic64_read((vatomic64_t *)(void *)&((pg)->flags)) &		\
		(PAGE_F_##uppercase_flag)) != 0);					\
}											\
static inline void page_set_##lowercase_flag(struct page_s *pg)				\
{											\
	BUG_ON_D(pg == NULL);								\
	page_set_flag_check(pg, PAGE_F_##uppercase_flag);				\
	(void)(vatomic64_or((vatomic64_t *)(void *)&((pg)->flags),			\
			    (PAGE_F_##uppercase_flag)));				\
}											\
static inline void page_clear_##lowercase_flag(struct page_s *pg)			\
{											\
	BUG_ON_D(pg == NULL);								\
	page_clear_flag_check(pg, PAGE_F_##uppercase_flag);				\
	(void)(vatomic64_and((vatomic64_t *)(void *)&((pg)->flags),			\
			     ~((unsigned long)PAGE_F_##uppercase_flag)));		\
}											\
static inline bool page_test_set_##lowercase_flag(struct page_s *pg)			\
{											\
	BUG_ON_D(pg == NULL);								\
	page_set_flag_check(pg, PAGE_F_##uppercase_flag);				\
	return ((vatomic64_get_or((vatomic64_t *)(void *)&((pg)->flags),		\
		(PAGE_F_##uppercase_flag)) & (PAGE_F_##uppercase_flag)) == 0);		\
}											\
static inline bool page_test_clear_##lowercase_flag(struct page_s *pg)			\
{											\
	BUG_ON_D(pg == NULL);								\
	page_clear_flag_check(pg, PAGE_F_##uppercase_flag);				\
	return ((vatomic64_get_and((vatomic64_t *)(void *)&(pg->flags),			\
		~(PAGE_F_##uppercase_flag)) & (PAGE_F_##uppercase_flag)) != 0);		\
}											\

/* sysmgr page flags ops */
PAGE(shmmbacked, SHMMBACKED)
#ifdef CONFIG_MEM_PURGEABLE
PAGE(purgeable, PURGEABLE)
#endif
PAGE(mlocked, MLOCKED)
PAGE(compound, COMPOUND)
PAGE(referenced, REFERENCED)
PAGE(lru, LRU)
PAGE(swap, SWAP)
PAGE(isolated, ISOLATED)
PAGE(migrated, MIGRATED)
PAGE(active, ACTIVE)
PAGE(inevictable, INEVICTABLE)
PAGE(valid, VALID)
PAGE(unmovable, UNMOVABLE)
PAGE(iofastrsv, IOFASTRSV)
PAGE(file_swappable, FILE_SWAPPABLE)
PAGE(dcache_clean, DCACHE_CLEAN)
PAGE(lazyfree, LAZYFREE)
#if defined(CONFIG_KASAN_DUMPSTACK) || defined(CONFIG_SLAB_VIRTUAL)
PAGE(slaballoc, SLABALLOC)
#endif
PAGE(noncache, NONCACHE)

/* fscache page flags ops */
PAGE(dirty, DIRTY)
PAGE(uptodate, UPTODATE)
PAGE(private, PRIVATE)
PAGE(nolru, NOLRU)
PAGE(writeback, WRITEBACK)
PAGE(waiter, WAITER)
PAGE(error, ERROR)
PAGE(freeing, FREEING)
PAGE(readahead, READAHEAD)
PAGE(async_rq, ASYNC_RQ)
PAGE(async, ASYNC)
PAGE(checked, CHECKED)
PAGE(bhaligned, BHALIGNED)
PAGE(lru_fs, LRU_FS)
PAGE(percpu, PERCPU)
PAGE(reclaim, RECLAIM)
PAGE(unevict, UNEVICT)
PAGE(active_fs, ACTIVE_FS)
PAGE(referenced_fs, REFERENCED_FS)
PAGE(predirty, PREDIRTY)
PAGE(protect_lru, PROTECT_LRU)
PAGE(lactive, LACTIVE)
PAGE(verified, VERIFIED)
PAGE(spool, SPOOL)
PAGE(mreclaim, MRECLAIM)
PAGE(swap_rawpage, SWAP_RAWPAGE)
PAGE(cpool, CPOOL)
PAGE(nobackend, NOBACKEND)

#define PG_FL_DIRTY		PAGE_F_DIRTY
#define PG_FL_UPTODATE		PAGE_F_UPTODATE
#define PG_FL_CPOOL		PAGE_F_CPOOL
#define PG_FL_PRIVATE		PAGE_F_PRIVATE
#define PG_FL_NOLRU		PAGE_F_NOLRU
#define PG_FL_WRITEBACK		PAGE_F_WRITEBACK
#define PG_FL_WAITER		PAGE_F_WAITER
#define PG_FL_ERROR		PAGE_F_ERROR
#define PG_FL_NOBACKEND		PAGE_F_NOBACKEND
#define PG_FL_FREEING		PAGE_F_FREEING
#define PG_FL_READAHEAD		PAGE_F_READAHEAD
#define PG_FL_ASYNC_RQ		PAGE_F_ASYNC_RQ
#define PG_FL_ASYNC		PAGE_F_ASYNC
#define PG_FL_CHECKED		PAGE_F_CHECKED
#define PG_FL_BHALIGNED		PAGE_F_BHALIGNED
#define PG_FL_LRU		PAGE_F_LRU_FS
#define PG_FL_PERCPU		PAGE_F_PERCPU
#define PG_FL_RECLAIM		PAGE_F_RECLAIM
#define PG_FL_UNEVICT		PAGE_F_UNEVICT
#define PG_FL_ACTIVE		PAGE_F_ACTIVE_FS
#define PG_FL_REFERENCED	PAGE_F_REFERENCED_FS
#define PG_FL_PREDIRTY		PAGE_F_PREDIRTY
#define PG_FL_PROTECT_LRU	PAGE_F_PROTECT_LRU
#define PG_FL_LACTIVE		PAGE_F_LACTIVE
#define PG_FL_SPOOL		PAGE_F_SPOOL
#define PG_FL_MRECLAIM		PAGE_F_MRECLAIM

#define SYSMGR_PAGE_FLAG_MASK	((1UL << FSCACHE_PAGE_FLAG_SHIFT) - 1)
#define FSCACHE_PAGE_FLAG_MASK ((((1UL << FSCACHE_PAGE_FLAG_SHIFT) - 1) << \
			        FSCACHE_PAGE_FLAG_SHIFT) & (~PAGE_FLAGS_LOCK_MASK))

#define PAGE_IL_FREE_F_MASK	(PAGE_F_LRU | PAGE_F_ISOLATED | PAGE_F_UPTODATE)

static inline uint64_t fscache_page_fs_flags_from_sysmgr(struct page_s *page)
{
	return (vatomic64_read((vatomic64_t *)&page->flags) & FSCACHE_PAGE_FLAG_MASK);
}

static inline void sysmgr_init_fscache_page_flags(struct page_s *page, uint64_t flags)
{
	uint64_t masked_flags = flags & FSCACHE_PAGE_FLAG_MASK;
	vatomic64_and((vatomic64_t *)&page->flags, ~FSCACHE_PAGE_FLAG_MASK);
	vatomic64_or((vatomic64_t *)&page->flags, masked_flags);
}

static inline void sysmgr_clear_fscache_page_flags(struct page_s *page)
{
	vatomic64_and((vatomic64_t *)&page->flags, SYSMGR_PAGE_FLAG_MASK | PAGE_FLAGS_LOCK_MASK);
}

#define SYSMGR_PROTECT_LRU_LEVEL_MASK	0x7UL
#define SYSMGR_PROTECT_LRU_LEVEL_OFFSET	(FSCACHE_PAGE_SHIFT_MAX + FSCACHE_PAGE_FLAG_SHIFT)
static inline void sysmgr_set_page_protect_level(struct page_s *page, int level)
{
	vatomic64_and((vatomic64_t *)&page->flags, ~(SYSMGR_PROTECT_LRU_LEVEL_MASK << SYSMGR_PROTECT_LRU_LEVEL_OFFSET));
	vatomic64_or((vatomic64_t *)&page->flags, ((unsigned long)level & SYSMGR_PROTECT_LRU_LEVEL_MASK) <<
		      SYSMGR_PROTECT_LRU_LEVEL_OFFSET);
}

static inline int sysmgr_get_page_protect_level(struct page_s *page)
{
	return (int)((vatomic64_read((vatomic64_t *)&page->flags) >> SYSMGR_PROTECT_LRU_LEVEL_OFFSET) &
		      SYSMGR_PROTECT_LRU_LEVEL_MASK);
}

static inline bool sysmgr_page_is_protect(struct page_s *page)
{
	return sysmgr_get_page_protect_level(page) != 0;
}
#endif /* CONFIG_PAGE_META_MERGE */

void disable_page_lock(void);
void enable_page_lock(void);
void pagedup(void *dst, void *src, unsigned long len);

struct page_s *page_get(struct page_s *pg);
void page_put(struct page_s *pg);
bool page_is_cma_zone(struct page_s *pg);
bool page_is_kern_zone(struct page_s *pg);
void dump_sysmgr_pgtbl_info(struct vspace_s *vs, uint64_t pa);

uintptr_t page_sysvaddr_of(const struct page_s *pg);

#define PAGE_ORDER_SHIFT	27U
#define PAGE_ORDER_MASK		PAGE_FLAG_CAST(0xF8000000)

/* transform between flags and page order */
#ifndef CONFIG_PAGE_META_MERGE
#define _from_flags(x, m, s)	(((uint32_t)(x) & (m)) >> (s))
#define _to_flags(x, m, s)	(((uint32_t)(x) << (s)) & (m))
#define order_from_flags(x)	((uint32_t)_from_flags(x, PAGE_ORDER_MASK, PAGE_ORDER_SHIFT))
#else
#define _from_flags(x, m, s)	(((uint64_t)(x) & (m)) >> (s))
#define _to_flags(x, m, s)	(((uint64_t)(x) << (s)) & (m))
#define order_from_flags(x)	((uint64_t)_from_flags(x, PAGE_ORDER_MASK, PAGE_ORDER_SHIFT))
#endif

#define order_to_flags(x)	_to_flags((x), PAGE_ORDER_MASK, PAGE_ORDER_SHIFT)

static inline uint32_t page_order_of(const struct page_s *pg)
{
	BUG_ON_D(pg == NULL);
	return order_from_flags(pg->flags);
}

static inline size_t page_size_of(const struct page_s *pg)
{
	BUG_ON_D(pg == NULL);
	return MEM_ORDER_TO_SIZE(page_order_of(pg));
}

#ifdef CONFIG_PAGE_META_MERGE
static inline void page_set_order(struct page_s *pg, uint32_t order)
{
	long old_val, new_val, ret_val;
	BUG_ON_D(pg == NULL);

	do {
		old_val = raw_atomic_long_read((raw_atomic_long_t *)(void *)&pg->flags);
		new_val = (long)(((uint64_t)old_val & ~PAGE_ORDER_MASK) | order_to_flags(order));
		ret_val = raw_atomic_long_cas((raw_atomic_long_t *)(void *)&pg->flags, old_val, new_val);
	} while (ret_val != old_val);
}

static inline void page_set_mlock(struct page_s *pg)
{
	BUG_ON_D(pg == NULL);
	if (!page_is_mlocked(pg)) {
		sys_stat_inc_size(SYS_MLOCKED, (uint64_t)page_size_of(pg));
		page_set_mlocked(pg);
	}
}

static inline void page_clear_mlock(struct page_s *pg)
{
	BUG_ON_D(pg == NULL);
	if (page_is_mlocked(pg)) {
		sys_stat_dec_size(SYS_MLOCKED, (uint64_t)page_size_of(pg));
		page_clear_mlocked(pg);
	}
}
#else
static inline void page_set_order(struct page_s *pg, uint32_t order)
{
	int old_val, new_val, ret_val;
	BUG_ON_D(pg == NULL);

	do {
		old_val = raw_atomic_int_read((raw_atomic_int_t *)(void *)&pg->flags);
		new_val = (int)(((uint32_t)old_val & ~PAGE_ORDER_MASK) | order_to_flags(order));
		ret_val = raw_atomic_int_cas((raw_atomic_int_t *)(void *)&pg->flags, old_val, new_val);
	} while (ret_val != old_val);
}
#endif

#define PAGE_TYPE_MASK		0x0000000FU

static inline enum pg_type page_tid_of(const struct page_s *page)
{
	BUG_ON_D(page == NULL);
	return (enum pg_type)(page->flags & PAGE_TYPE_MASK);
}

static inline bool page_is_file(const struct page_s *pg)
{
	uint32_t tid = page_tid_of(pg);
	return (tid == PAGE_FILE) || (tid == PAGE_HUGE_FILE);
}

static inline bool page_is_huge(struct page_s *page)
{
	return page_tid_of(page) == PAGE_HUGE;
}

static inline bool page_is_file_shmem(__maybe_unused struct page_s *pg)
{
#ifdef CONFIG_SWAP_SHMEM
	return (page_tid_of(pg) == PAGE_FILE) && page_is_file_swappable(pg);
#else
	return false;
#endif
}

static inline void page_clear_memcg(struct page_s *pg)
{
#ifdef CONFIG_RESMGR_MEMORY
	if (pg->memcg != NULL) {
		memcg_put(pg->memcg);
		pg->memcg = NULL;
	}
#else
	UNUSED(pg);
#endif
}

static inline void page_insert(struct dlist_node *list, struct page_s *pg)
{
	dlist_insert(list, &pg->dnode);
}

static inline void page_insert_tail(struct dlist_node *list, struct page_s *pg)
{
	dlist_insert_tail(list, &pg->dnode);
}

static inline void page_delete(struct page_s *pg)
{
	dlist_delete(&pg->dnode);
}

static inline struct page_s *page_first_of_list(struct dlist_node *list)
{
	return dlist_first_entry(list, struct page_s, dnode);
}

static inline struct page_s *page_last_of_list(struct dlist_node *list)
{
	return dlist_last_entry(list, struct page_s, dnode);
}

static inline bool page_empty(struct page_s *pg)
{
	return dlist_empty(&pg->dnode);
}
#endif /* __SYSMGR_INTAPI_MEM_PAGE_H__ */
