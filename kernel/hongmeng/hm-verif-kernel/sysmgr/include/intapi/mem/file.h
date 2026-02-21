/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: File management of virtual file system
 * Author: Huawei OS Kernel Lab
 * Create: Sun Mar 31 07:27:40 2019
 */
#ifndef SYSMGR_MEM_FILE_H
#define SYSMGR_MEM_FILE_H

#include <vfs.h>
#include <stdint.h>
#include <hmkernel/capability.h>
#include <intapi/mem/vspace.h>

/* vfs flags */
#define FILE_F_PAGE_UNMAPPED		(unsigned)MMAP_FL_PAGE_UNMAPPED
#define FILE_F_PAGE_UNMOVE		(unsigned)MMAP_FL_PAGE_UNMOVE
#define FILE_F_PAGE_DIRTIED		(unsigned)MMAP_FL_PAGE_DIRTIED
#define FILE_F_PAGE_ASYNCED		(unsigned)MMAP_FL_PAGE_ASYNCED
#define FILE_F_PAGE_SYNCED		(unsigned)MMAP_FL_PAGE_SYNCED
#define FILE_F_PAGE_INVALIDATED		(unsigned)MMAP_FL_PAGE_INVALIDATED
#define FILE_F_PAGE_RECLAIMED		(unsigned)MMAP_FL_PAGE_RECLAIM
#define FILE_F_PAGE_SET_MLOCK		(unsigned)MMAP_FL_PAGE_MLOCK
#define FILE_F_PAGE_CLEAR_MLOCK		(unsigned)MMAP_FL_PAGE_MUNLOCK
#define FILE_F_PAGE_SWAPLESS		(unsigned)MMAP_FL_PAGE_SWAPLESS
#define FILE_F_PAGE_ADDR		(unsigned)MMAP_FL_PAGE_ADDR
#define FILE_F_PAGE_HKIP		(unsigned)MMAP_FL_PAGE_HKIP

struct vregion_s;

typedef vfs_mmap_node_t file_node_t;
typedef vfs_stat_common_t raw_vfs_stat_t;

struct file_info_s {
	file_node_t fnode;
	unsigned long fid;
	uint32_t cnode_idx;
	cref_t vfs_cref;
	rref_t sys2vfs_rref;
	uint32_t page_order;
	uint32_t seals;
	bool hhp;
};

struct io_uring_info {
	uintptr_t vaddr;
	unsigned long fid;
};

/* arguments for mmap create for file */
struct file_map_args_s {
	unsigned long fd_file;
	uint32_t cnode_idx;
	uint32_t prot;
	uint32_t flags;
	uint64_t offset;
	uint64_t length;
};

struct mapped_proc_s {
	struct dlist_node node;
	struct process_s *proc;
};

extern void free_mapped_procs(struct dlist_node *proc_list);
extern int find_mapped_procs_by_file_path(const char *file_path, struct dlist_node *proc_list, vfs_mmap_node_t *pnode);

int file_page_set_info(uintptr_t va, void *fpg);

int file_page_clear_info(uintptr_t va);

uintptr_t file_page_try_acquire_info(uintptr_t va);

int file_page_inc_ref(uintptr_t va);
int file_page_dec_ref(uintptr_t va);

void file_page_clear_dcache_clean(uintptr_t va);

struct vrgroup_s;
void file_mapping_destroy_vrgroup(struct vrgroup_s *vrg);
#endif
