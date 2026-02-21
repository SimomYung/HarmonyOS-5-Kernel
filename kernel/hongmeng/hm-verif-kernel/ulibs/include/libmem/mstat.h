/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Memory status
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 02 11:19:25 2020
 */
#ifndef ULIBS_INCLUDE_LIBMEM_MSTAT_H
#define ULIBS_INCLUDE_LIBMEM_MSTAT_H

#include <libmem/memm_slab.h>

#define MEMSTAT_PAGE_TYPE_MAX_NUMBER (uint32_t)NR_MEM_PAGE_TYPE

struct memstat_fs_stat_s {
	uint64_t free_cache;
	uint64_t used_cache;
	uint64_t lru_slab_size;
	uint64_t dirty_wb_size;
	uint64_t pages_unevict;
	uint64_t pages_inactive;
	uint64_t pages_unmap;
	uint64_t pages_unmap_failed;
	uint64_t pages_isolated;
	uint64_t pages_tmpfs;
	uint64_t pages_overlayfs;
	uint64_t pages_devfs;
	uint64_t protect_free;
	uint64_t protect_free_active;
	uint64_t protect_overratio_reclaim_times;
	uint64_t protect_overlimit_reclaim_times;
	uint64_t protect_pages_reclaimed_overratio;
	uint64_t protect_pages_reclaimed_overlimit;
	uint64_t protect_min_fscache_pages;  /* minimum file pages when reclaim due to overratio */
	uint64_t workingset_refault_file;
	uint64_t workingset_activate_file;
	uint64_t workingset_restore_file;
	uint64_t workingset_eviction_file;
	uint64_t page_scanned;
	uint64_t ashmem_total_size;  /* size that has been setted   from ioctl ASHMFD_SET_SIZE */
	uint64_t ashmem_alloc_size;  /* size that has been alloced  from fs pagecache mapping */
	uint64_t ashmem_free_size;   /* size that has been lrued    from ioctl ASHMFD_UNPIN */
	uint64_t ashmem_shrink_size; /* size that has been shrinked from shrink (only for unpin) */
	uint64_t slabs_total;
	uint64_t pages_mmap; /* phymem size of mapped file pages */
	uint64_t mmap_private;
	uint64_t page_metadata;
	uint64_t mapping;
	uint64_t file;
	uint64_t dentry;
	uint64_t fat_node;
	uint64_t tmpfs_node;
	uint64_t ext2_node;
	uint64_t jffs2_node;
	uint64_t vnode_avail; /* bytes of vnode can be reclaimable */
	uint64_t dentry_avail; /* bytes of dentry can be reclaimable */
	uint64_t fs_hpage_2m;
	uint64_t fs_hpage_4m;
	uint64_t fs_hpage_1g;
#ifdef CONFIG_VFS_FUSION
	size_t mcache_reclaimable;
#endif
};

struct memstat_hgtlb_all_s {
	uint64_t huge_pages_total_size;
	uint64_t huge_pages_free_size;
	uint64_t huge_pages_rsvd_size;
	uint64_t huge_pages_surp_size;
};

struct memstat_summary_stat_s {
	uint64_t total;
	uint64_t free;
	uint64_t pcache_free;
	uint64_t total_elf;
	uint64_t shm_unused;
	uint64_t kobj_size;
	struct memstat_fs_stat_s fs_stat;
	struct memstat_hgtlb_all_s hgtlb_stat;
};

struct mem_summary_s {
	uint64_t total;
	uint64_t free;
	uint64_t cache;
	uint64_t total_elf;
};

struct memstat_all_proc_info {
	struct mem_size_info all_proc_mem;
	unsigned int all_process_cnt;
	unsigned int all_thread_cnt;
};

struct memstat_deviation {
	int64_t div_zone;
};

#endif
