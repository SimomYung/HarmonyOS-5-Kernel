/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Definitions for fspgcache
 * Author: Huawei OS Kernel Lab
 * Created: Wed Jul 05 2023
 */

#ifndef MAPI_HMKERNEL_CTRLMEM_KOBJS_FSPGCACHE_H
#define MAPI_HMKERNEL_CTRLMEM_KOBJS_FSPGCACHE_H

#include <uapi/hmkernel/ctrlmem/kobjs/fspgcache.h>
#include <hmkernel/ctrlmem/kobjs/rwlock.h>
#include <hmkernel/ctrlmem/kobjs/syspa.h>
#include <hmkernel/atomic.h>
#include <hmkernel/types.h>
#include <hmasm/memory.h>

struct ctrlmem_kobj_fspgcache_dnode_s {
	ctrlmem_syspa_t prev;
	ctrlmem_syspa_t next;
};

struct ctrlmem_kobj_fspgcache_page_s {
	__u32 unused;
	struct ctrlmem_kobj_fspgcache_dnode_s link;
	ctrlmem_syspa_t data;
#ifdef CONFIG_CTRLMEM_PAGEFAULT_PAGE_META_MERGE
	struct syspg_knode_s *sysmgr_page;
#else
	vatomic32_t refcnt;
	vatomic32_t flags;
	vatomic32_t mmapcnt;
	__u32 lock;
	struct ctrlmem_kobj_rwlock_s rwlock;
#endif
};

struct ctrlmem_kobj_fspgcache_rdtnode_s {
	u32 height;
	u32 offset;
	u32 count;
	ctrlmem_syspa_t parent;
	ctrlmem_syspa_t slots[__CTRLMEM_FSPGCACHE_RDT_SLOTS_SIZE];
};

struct ctrlmem_kobj_fspgcache_rdtroot_s {
	ctrlmem_syspa_t rnode;
	bool padding;
	bool use_pool;
};

struct ctrlmem_kobj_fspgcache_rdtree_s {
#if __SIZEOF_LONG__ == 4
	u32 padding;
#else
	u64 padding;
#endif
	struct ctrlmem_kobj_fspgcache_rdtroot_s root;
};

struct ctrlmem_kobj_fspgcache_mapping_s {
	struct ctrlmem_kobj_fspgcache_rdtree_s tree_root;
	u32 pages_mmap;
	struct ctrlmem_kobj_rwlock_s rwlock;
};

struct ctrlmem_kobj_fspgcache_node_s {
	u64 size;
	ctrlmem_syspa_t i_op;
	ctrlmem_syspa_t i_sb;
	u32 mode;
	u32 v_flags;
	ctrlmem_syspa_t mapping;
	struct ctrlmem_kobj_rwlock_s rwlock;
};

struct ctrlmem_kobj_fspgcache_file_s {
	ctrlmem_syspa_t node;
};

struct ctrlmem_kobj_fspgcache_fscache_stat_s {
#if __SIZEOF_LONG__ == 4
	vatomic32_t pages_mmap;
	vatomic32_t pages_free;
#else
	vatomic64_t pages_mmap;
	vatomic64_t pages_free;
#endif
};

struct ctrlmem_kobj_fspgcache_fscache_s {
	ctrlmem_syspa_t stat;
};

struct ctrlmem_kobj_fspgcache_searchpgc_state_s {
	struct ctrlmem_kobj_fspgcache_node_s *node_kva;
	struct ctrlmem_kobj_fspgcache_mapping_s *mapping_kva;
	struct ctrlmem_kobj_fspgcache_page_s *pgc_kva;
	struct pgstr_s *node_pgstr;
	struct pgstr_s *mapping_pgstr;
	struct pgstr_s *pgc_pgstr;
};

#define CTRLMEM_KOBJ_FSPGCACHE_SEARCHPGC_STATE_ZERO { \
	.node_kva = NULL, \
	.mapping_kva = NULL, \
	.pgc_kva = NULL, \
	.node_pgstr = NULL, \
	.mapping_pgstr = NULL, \
	.pgc_pgstr = NULL, \
}

struct ctrlmem_filepg_state_s {
	struct pgstr_s *pgstr;
	paddr_t pa;
	ctrlmem_syspa_t syspa;
};

#define CTRLMEM_FILEPG_STATE_ZERO { \
	.pgstr = NULL, \
	.pa = (paddr_t)(0ULL), \
	.syspa = {.val = 0UL}, \
}

static inline bool ctrlmem_kobj_fspgcache_is_normal_page(struct ctrlmem_kobj_fspgcache_rdtnode_s *pgcache)
{
	return ((((uptr_t)pgcache >> __CTRLMEM_FSPGCACHE_SHADOW_SHIFT_TYPE) & __CTRLMEM_FSPGCACHE_SHADOW_MASK_TYPE) ==
		__CTRLMEM_FSPGCACHE_SHADOW_PAGE_NORMAL);
}

static inline bool ctrlmem_kobj_fspgcache_is_uptodate(struct ctrlmem_kobj_fspgcache_page_s *pgcache)
{
#ifdef CONFIG_CTRLMEM_PAGEFAULT_PAGE_META_MERGE
	return ((vatomic64_read((vatomic64_t *)&pgcache->sysmgr_page->flags) & CTRLMEM_PAGE_F_UPTODATE) != 0) && 
		((vatomic64_read((vatomic64_t *)&pgcache->sysmgr_page->flags) & CTRLMEM_PAGE_F_READAHEAD)) != 0;
#else
	__u32 flags = vatomic32_read(&pgcache->flags);

	return ((flags & __CTRLMEM_FSPGCACHE_PAGEFL_UPTODATE) != 0) &&
	       ((flags & __CTRLMEM_FSPGCACHE_PAGEFL_READAHEAD) == 0);
#endif
}

static inline int ctrlmem_kobj_fspgcache_node_trylock(struct ctrlmem_kobj_fspgcache_node_s *node)
{
	return ctrlmem_kobj_tryrdlock(&node->rwlock);
}

static inline void ctrlmem_kobj_fspgcache_node_unlock(struct ctrlmem_kobj_fspgcache_node_s *node)
{
	ctrlmem_kobj_rdunlock(&node->rwlock);
}

static inline int ctrlmem_kobj_fspgcache_mapping_trylock(struct ctrlmem_kobj_fspgcache_mapping_s *mapping)
{
	return ctrlmem_kobj_tryrdlock(&mapping->rwlock);
}

static inline void ctrlmem_kobj_fspgcache_mapping_unlock(struct ctrlmem_kobj_fspgcache_mapping_s *mapping)
{
	ctrlmem_kobj_rdunlock(&mapping->rwlock);
}

int ctrlmem_kobj_fspgcache_searchpgc_check_acquire(u64 fid, u64 pg_index,
						   const struct ctrlmem_sysparule_s *sysparule,
						   struct ctrlmem_kobj_fspgcache_searchpgc_state_s *state);

void ctrlmem_kobj_fspgcache_searchpgc_release(struct ctrlmem_kobj_fspgcache_searchpgc_state_s *state);

struct ctrlmem_pcache_dequeue_state_s;
struct ctrlmem_filepg_state_s;
int ctrlmem_kobj_fspgcache_getsyspg_acquire(const struct ctrlmem_sysparule_s *sysparule,
					    struct ctrlmem_pcache_dequeue_state_s *state);
int ctrlmem_kobj_fspgcache_getpgpa_acquire(const struct ctrlmem_sysparule_s *sysparule,
					 struct ctrlmem_filepg_state_s *state);
void ctrlmem_kobj_fspgcache_getsyspg_finish(struct ctrlmem_pcache_dequeue_state_s *state);
void ctrlmem_kobj_fspgcache_getpgpa_finish(struct ctrlmem_filepg_state_s *state);
void ctrlmem_kobj_fspgcache_update_stat(struct ctrlmem_kobj_fspgcache_fscache_stat_s *fscache_stat, bool increase);

#endif
