/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_MEMORY_DATA_H
#define HVGR_MEMORY_DATA_H

#include <linux/mm.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/atomic.h>
#include <linux/dma-mapping.h>
#include <linux/shrinker.h>
#include <linux/rbtree.h>
#include <linux/kref.h>
#ifdef CONFIG_LIBLINUX
#include <linux/rwsem.h>
#endif

#include "hvgr_ioctl_mem.h"
#include "hvgr_kv.h"
#include "hvgr_mmu_api.h"
#include "hvgr_pool.h"
#include "hvgr_mem_space.h"
#include "hvgr_mmu_driver.h"
#if defined(HVGR_STATE_BUNDLE2) && HVGR_STATE_BUNDLE2 > 0
#include "hvgr_mcu_common_defs.h"
#endif

struct hvgr_mem_zone;
struct hvgr_device;
struct hvgr_ctx;
struct hvgr_mmu_stack;

#define HVGR_PAGE_SYNC  (1U << 0)
#define HVGR_PAGE_CLEAR (1U << 1)

#define COHERENCY_ACE_LITE 0
#define COHERENCY_ACE 1
#define COHERENCY_NONE 31

#define MEM_PHY_ALLOC_THRESHOLD 512
#define HVGR_MEM_AREA_FLAG_2M (1UL << 0)
#define HVGR_MEM_AREA_FLAG_SPARSE_CACHE (1UL << 1)
#define HVGR_MEM_IMPORT_MAX 16
#define HVGR_MEM_IMPORT_UMM 1

#define HVGR_MEM_SC_UPDATE_POLICY_MAX 200U

#define HVGR_2M_PAGE_ORDER 9

struct hvgr_mem_pool {
	struct hvgr_device *gdev;
	struct hvgr_pool pool;
	uint32_t order;

	bool refuse_shrink;
	struct shrinker sys_shrink;

	uint32_t pool_id;
	uint32_t flag;
	bool growable;
	bool prefill;
};

struct hvgr_dma_buf_record {
	struct list_head dma_buf_list;
	struct dma_buf *dmabuf;
	struct hvgr_mem_area *area;
	struct hvgr_ctx *ctx;
	uint32_t record_idx;
	uint32_t start_page_nr;
	uint32_t page_nr;
};

struct hvgr_dmabuf_ctx_rec {
	struct list_head node;
	struct hvgr_ctx *ctx;
};

struct hvgr_mem_import_record {
	struct dma_buf *dmabuf;
	struct dma_buf_attachment *dmabuf_attach;
	struct sg_table *sg_table;
	struct hvgr_dma_buf_record *dma_buf_rec;
};

struct hvgr_mem_import {
	union {
		struct {
		uint32_t cnt;
		bool mapped;
		atomic_t free_tlb_cnt;
		atomic_t restore_tlb_cnt;
		struct hvgr_mem_import_record record[HVGR_MEM_IMPORT_MAX];
		} umm;
		struct {
			struct mm_struct *mm;
			dma_addr_t *dma_addrs;
			uint64_t size;
		} user_buf;
	} data;
	uint32_t type;
};

struct hvgr_mem_alias {
	uint32_t count;
	struct hvgr_mem_alias_info *info;
	struct hvgr_mem_area **areas;
};

struct hvgr_mem_interval_tree_node {
	struct rb_node rb;
	unsigned long start;
	unsigned long last;
	unsigned long __subtree_last;
};

struct hvgr_mem_area {
	struct hvgr_kv_node node;

	struct hvgr_mem_interval_tree_node itn;

	struct vm_area_struct *vma;

	/* Every memory area belong to a memory zone. */
	struct hvgr_mem_zone *zone;

	/*
	 * The Page array start address.
	 */
	struct page **page_array;

	/* This area is a map area. */
	struct hvgr_mem_area *dep_area;

	uint64_t flags;

	uint64_t kva;   /* Kernel virtual address */
	uint64_t uva;   /* Userspace virtual address */
	/*
	 * GPU virtural address, some memory allocations are used to
	 * pass user-specified gva. e.g. replay memory.
	 */
	uint64_t gva;

	/* The memory attribute. */
	uint64_t property;
	uint64_t attribute;

	/* How many pages in page_array now. */
	uint64_t pages;

	/* The max pages numbers. */
	uint64_t max_pages;

	/* First alloced physical page numbers. */
	uint64_t phy_pages;

	/* The grow pages in page fault */
	uint32_t extent;

	atomic_t growable_pages;

	gfp_t gfp;

	uint64_t mmu_flag;

	/* The offset pages from area start address witch to be mapped. */
	uint64_t page_offset;

	struct hvgr_mem_import import;

	/* Alloc 2^order pages in continuous addr */
	uint32_t order;

	struct hvgr_mem_alias alias;

	/* Va is freed, pages are in used */
	bool freed;

	/* Whether invoked by hvgr_mem_ioctl_alloc_memory */
	bool user_buf;

#ifdef CONFIG_LIBLINUX
	pgprot_t vm_page_prot;
#endif
	struct list_head jit_node;
	bool kmd_sparse_bind;

	struct list_head swaped_node;

	struct kref kref;

	struct hvgr_kv_node k_node;

	bool is_freed;

	uint32_t mmap_cnt;

	uint64_t pt_map_nr;
	uint64_t pt_unmap_nr;

	bool is_record;
};

struct hvgr_mem_area_va {
	uint64_t gva;   /* GPU virtural address */
	uint64_t kva;   /* Kernel virtual address */
};


struct hvgr_mem_zone;

/**
 * Every zone implementation must include every functions
 * in struct hvgr_zone_opts.
 */
struct hvgr_zone_opts {
	bool (*init)(struct hvgr_mem_zone * const zone, void * const para);
	bool (*term)(struct hvgr_mem_zone * const zone);
	bool (*check_para)(struct hvgr_mem_area * const area);

	long (*alloc)(struct hvgr_mem_area * const area);
	long (*free)(struct hvgr_mem_area * const area);
	long (*import)(struct hvgr_mem_area * const area, int fd);
};

struct hvgr_mem_zone {
	struct hvgr_mem_ctx *pmem;
	uint64_t property;

	struct hvgr_zone_opts const *opts;
	void *prv_data;

	struct hvgr_mem_space va_space;
};

struct hvgr_mem_4gva_cb {
	struct hvgr_mem_space va_space;
	atomic_t refcounter;
	struct hvgr_mem_cpu_map *cpu_map;
};

struct hvgr_mem_process_res_cb {
	/* key is tgid */
	struct hvgr_kv_node kv_node;
	atomic_t ref_cnt;
	struct hvgr_mem_4gva_cb *mem_4gva;
	atomic_t rsv_cnt;
	void *mem_replay;
#if defined(MALEOON_2D_MCU_FEATURE) && MALEOON_2D_MCU_FEATURE != 0
	struct hvgr_mem_area *area_2d_mcu_fm;
	struct mutex mcu_2d_mutex;
	bool mcu_2d_fm_load;
#endif
};

#ifdef HVGR_FEATURE_SYSTEM_CACHE
#if hvgr_version_lt(HVGR_V300)
#define SC_ID_MIN 1
#define SC_ID_MAX 15
#define SC_MEM_POOL_NUM 1
#else
#define SC_ID_MIN 1
#define SC_ID_MAX 255
/* 0: NOOSH-BIG 1:NOOSH-LBW 2:OSH-BIG 3:OSH-LBW */
#define SC_MEM_POOL_NUM 4
#endif
#define SCS_REMAP_REG_ADDR_START 0x0u
#define SCS_REMAP_REG_ADDR_END   0xFCu
#define SCS_REMAP_REG_NUM        64u

struct hvgr_sc_policy_info {
	struct list_head node;
	/* The cache policy id */
	uint32_t policy_id;
	/* The cache policy config */
	uint32_t cache_policy;
	/* The cache quota */
	uint32_t cache_quota;
};

struct hvgr_sc_dev_data {
	 /* hvgr_sc_policy_info head */
	struct list_head policy_info_list;
	 /* sc mem pool head */
	struct list_head mem_pool_list[SC_MEM_POOL_NUM];
	/* The GID for page table. */
	uint32_t pgd_mem_gid;
	/* System cache partial stream config. */
	uint32_t sc_ps_cfg;
	uint32_t sc_ps_hebc_only;
	uint32_t pgd_cache_enable;
	uint32_t pbha_mode;
	uint32_t remap_regs[SCS_REMAP_REG_NUM];
};

struct hvgr_sc_ctx_data {
	/* The GID for page table of this context */
	uint32_t pgd_mem_gid;
	/* System cache partial stream config */
	uint32_t sc_ps_cfg;

	uint32_t pgd_cache_enable;

	struct list_head mem_pool_list[SC_MEM_POOL_NUM];
};
#endif

#if hvgr_version_ge(HVGR_V350)
struct hvgr_mmu_buf {
	struct page *buf_page;
	uint32_t page_nums;
	uint32_t page_order;
	void * kva;
	uint32_t idx;
	uint32_t wrap;
};
#endif

#if hvgr_version_lt(HVGR_V300)
#define MEM_POOL_NUM 1
#else
#ifdef CONFIG_MM_AMA
/* 0:NOOSH-BIG 1:NOOSH-LBW 2:OSH-BIG 3:OSH-LBW */
#define MEM_POOL_NUM 4
#else
/* 0:NOOSH 1:OSH */
#define MEM_POOL_NUM 2
#endif
#endif

struct hvgr_mem_ctx {
	uint64_t pgd_pa;

	struct hvgr_mem_zone zones[HVGR_MEM_PROPERTY_MAX];

	/* All areas belong to this context. */
	struct hvgr_kv_map area_rec;

	struct hvgr_mem_process_res_cb *reserve_cb;
#ifdef CONFIG_HVGR_MMU_HM
	struct mutex tbl_lock;
	struct hvgr_mmu_stack *tbl_buf;
#else
	uint64_t *mmu_buf;
#endif
	struct mutex area_mutex;

	struct mutex mmu_mutex;

	struct mutex profile_mutex;

	struct hvgr_mem_pool mem_pool[MEM_POOL_NUM];
#ifdef HVGR_FEATURE_SYSTEM_CACHE
	struct hvgr_sc_ctx_data sc_data;
#endif
	DECLARE_BITMAP(pend_area_bitmap, BITS_PER_LONG);
	struct hvgr_mem_area *pend_areas[BITS_PER_LONG];
	struct mutex pend_area_mutex;

	struct mm_struct *process_mm;

	atomic_t used_pages;
	atomic_t used_grow_pages;
	atomic_t used_pt_pages;
	atomic_t used_kmd_pages;

	struct list_head dma_buf_rec_head;

	/* bg dma buf conut in ctx */
	uint32_t bg_dmabuf_cnt;
	/* Ctx has bd dma buf flag, if true, ctx need to add global ctx list. */
	bool bd_dmabuf_flag;
	/* bd dma buf ctx pointer */
	struct hvgr_dmabuf_ctx_rec *dmabuf_ctx;

	struct rb_root_cached itree;

	struct list_head jit_pool_head;
	struct list_head jit_using_head;
	struct mutex jit_shrink_mutex;
	struct shrinker jit_shrink;
	/* area has kernel access in this map */
	struct hvgr_kv_map area_k_rec;
	struct mutex area_k_mutex;

	struct mutex jit_mutex;
#if defined(HVGR_STATE_BUNDLE2) && HVGR_STATE_BUNDLE2 > 0
	struct hvgr_mem_slab_slice_buffer_data slab_slice_buffer_data;
#endif
#ifdef CONFIG_LIBLINUX
	struct rw_semaphore rw_4g_sem;
#endif
};

struct hvgr_mem_statics {
	uint64_t alloc_pages;
	uint64_t free_pages;

	uint64_t mmu_alloc_pages;
	uint64_t mmu_free_pages;
	uint64_t mmu_free;

	uint64_t pool_sync_pages;
	uint64_t pool_clear_pages;
#ifdef HVGR_FEATURE_SYSTEM_CACHE
	uint64_t sc_alloc_pages;
	uint64_t sc_free_pages;
#endif
	uint64_t ama_alloc_big;
	uint64_t ama_alloc_low;

	uint64_t grow_pages;
	uint64_t pool_shrink_pages;
	uint64_t dev_free_pages;
};

struct hvgr_mem_dev {
	/*
	 * mem_process_res_rec recorde hvgr_mem_process_res_cb objects of every process.
	 * The key is tgid.
	 */
	struct hvgr_kv_map mem_process_res_rec;
	struct mutex mem_process_res_lock;
#if hvgr_version_ge(HVGR_V350)
	struct hvgr_mmu_fault_worker mmu_as[MMU_ADDR_SPACE_MAX];
#else
	struct hvgr_mmu_as mmu_as[MMU_ADDR_SPACE_MAX];
#endif
	spinlock_t mmu_hw_lock;

	/* this page is used by sparse memory */
	struct page *sparse_zero_page;

	struct hvgr_mem_pool mem_pool[MEM_POOL_NUM];

#ifdef CONFIG_DFX_DEBUG_FS
	struct dentry *mem_dir;
	bool mem_kutf_flag;
#endif
	struct proc_dir_entry *proc_gpu_memory;
	struct hvgr_mem_statics statics;
#ifdef HVGR_FEATURE_SYSTEM_CACHE
	struct hvgr_sc_dev_data sc_data;
#endif
	uint32_t system_coherency;
	atomic_t pending_faults;
	atomic_t used_pages;
#if hvgr_version_lt(HVGR_V300)
	struct gen_pool *protect_pool;
	uint64_t protect_start;
	uint32_t protect_size;
#endif
	struct list_head dmabuf_ctx_list_head;
	struct mutex dmabuf_rec_lock;
#if hvgr_version_ge(HVGR_V350)
	struct hvgr_mmu_buf cd_buf;
	struct hvgr_mmu_buf cmdq_buf;
	struct hvgr_mmu_buf eventq_buf;
	/* cmdq used flag */
	DECLARE_BITMAP(cmdq_bitmap, MMU_CMDQ_MAX_CNT);
	spinlock_t cmdq_bitmap_lock;
#endif
#ifdef CONFIG_HVGR_ASYNC_MEM_RECYCLE
	spinlock_t recycle_lock;
	struct list_head recycle_list;
	struct wait_queue_head recycle_wq;
	struct task_struct *recycle_task;
#endif
	uint32_t fault_as_num;

#ifdef CONFIG_LIBLINUX
	uint64_t threshold_page;
#endif
};

struct hvgr_mem_cpu_map {
	struct hvgr_ctx *ctx;
	struct hvgr_mem_area *area;
	/* mmap/munmap count, make sure it ++/-- in mm lock */
 	uint32_t count;
	/* mmap's area has pages to free, close_free is true, same va/import mem... */
	bool close_free;
	bool is_4g_rsv;
};

#endif
