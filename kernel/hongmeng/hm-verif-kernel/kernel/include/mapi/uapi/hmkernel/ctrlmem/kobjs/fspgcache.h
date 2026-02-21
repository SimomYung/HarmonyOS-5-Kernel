/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Definitions for fspgcache
 * Author: Huawei OS Kernel Lab
 * Created: Wed Jul 05 2023
 */

#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_FSPGCACHE_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_KOBJS_FSPGCACHE_H

#include <hmkernel/ctrlmem/kobjs/rwlock.h>
#include <hmkernel/types.h>
#include <hmasm/memory.h>
#include <vsync/atomic.h>

#define __CTRLMEM_FSPGCACHE_RDT_SHIFT_SIZE 6
#define __CTRLMEM_FSPGCACHE_RDT_SLOTS_SIZE (1U << __CTRLMEM_FSPGCACHE_RDT_SHIFT_SIZE)
#define __CTRLMEM_FSPGCACHE_RDT_MASK (__CTRLMEM_FSPGCACHE_RDT_SLOTS_SIZE - 1U)

#ifdef CONFIG_PAGE_META_MERGE
#define CTRLMEM_PAGE_HIGGBITS_SHIFT	32
#define __CTRLMEM_FSPGCACHE_PAGEFL_UPTODATE	((0x00000002UL) << CTRLMEM_PAGE_HIGGBITS_SHIFT)
#define __CTRLMEM_FSPGCACHE_PAGEFL_READAHEAD	((0x00001000UL) << CTRLMEM_PAGE_HIGGBITS_SHIFT)
#else
#define __CTRLMEM_FSPGCACHE_PAGEFL_UPTODATE	(1 << 1UL)
#define __CTRLMEM_FSPGCACHE_PAGEFL_READAHEAD	(1 << 12UL)
#endif /* CONFIG_PAGE_META_MERGE */
#define __CTRLMEM_FSPGCACHE_PAGEFL_MMAP		(1 << 4UL)

#define __CTRLMEM_FSPGCACHE_SHADOW_SHIFT_TYPE	(0x0)
#define __CTRLMEM_FSPGCACHE_SHADOW_MASK_TYPE	(0x1)
#define __CTRLMEM_FSPGCACHE_SHADOW_PAGE_NORMAL	(0x0)

struct __ctrlmem_kobj_fspgcache_dnode_s {
#if __SIZEOF_LONG__ == 4
	__u32 prev;
	__u32 next;
#else
	__u64 prev;
	__u64 next;
#endif
};

struct __ctrlmem_kobj_fspgcache_page_s {
	__u32 unused;
	struct __ctrlmem_kobj_fspgcache_dnode_s link;
#if __SIZEOF_LONG__ == 4
	__u32 data;
#else
	__u64 data;
#endif
#ifdef CONFIG_CTRLMEM_PAGEFAULT_PAGE_META_MERGE
	struct __syspg_knode_s *sysmgr_page;
#else
	__u32 refcnt;
	__u32 flags;
	vatomic32_t mmapcnt;
	__u32 lock;
	struct __ctrlmem_kobj_rwlock_s rwlock;
#endif
};

struct __ctrlmem_kobj_fspgcache_rdtnode_s {
	__u32 height;
	__u32 offset;
	__u32 count;
#if __SIZEOF_LONG__ == 4
	__u32 parent;
	__u32 slots[__CTRLMEM_FSPGCACHE_RDT_SLOTS_SIZE];
#else
	__u64 parent;
	__u64 slots[__CTRLMEM_FSPGCACHE_RDT_SLOTS_SIZE];
#endif
};

struct __ctrlmem_kobj_fspgcache_rdtroot_s {
#if __SIZEOF_LONG__ == 4
	__u32 rnode;
#else
	__u64 rnode;
#endif
	__bool padding;
	__bool use_pool;
};

struct __ctrlmem_kobj_fspgcache_rdtree_s {
#if __SIZEOF_LONG__ == 4
	__u32 padding;
#else
	__u64 padding;
#endif
	struct __ctrlmem_kobj_fspgcache_rdtroot_s root;
};

struct __ctrlmem_kobj_fspgcache_mapping_s {
	struct __ctrlmem_kobj_fspgcache_rdtree_s tree_root;
	__u32 pages_mmap;
	struct __ctrlmem_kobj_rwlock_s rwlock;
};

struct __ctrlmem_kobj_fspgcache_node_s {
#if __SIZEOF_LONG__ == 4
	__u64 size;
	__u32 i_op;
	__u32 i_sb;
	__u32 mode;
	__u32 v_flags;
	__u32 mapping;
#else
	__u64 size;
	__u64 i_op;
	__u64 i_sb;
	__u32 mode;
	__u32 v_flags;
	__u64 mapping;
#endif
	struct __ctrlmem_kobj_rwlock_s rwlock;
};

struct __ctrlmem_kobj_fspgcache_file_s {
#if __SIZEOF_LONG__ == 4
	__u32 node;
#else
	__u64 node;
#endif
};

struct __ctrlmem_kobj_fspgcache_fscache_stat_s {
#if __SIZEOF_LONG__ == 4
	__u32 pages_mmap;
	__u32 pages_free;
#else
	__u64 pages_mmap;
	__u64 pages_free;
#endif
};

struct __ctrlmem_kobj_fspgcache_fscache_s {
#if __SIZEOF_LONG__ == 4
	__u32 stat;
#else
	__u64 stat;
#endif
};

#endif
