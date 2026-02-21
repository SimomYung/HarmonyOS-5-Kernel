/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: binderfs shm populate header file
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jan 06 11:14:17 2024
 */

#ifndef FS_BINDERFS_H
#define FS_BINDERFS_H

#include <stdint.h>
#include <stdbool.h>
#include <hongmeng/types.h>

#ifndef CONFIG_BINDER_MEM
int binderfs_mem_populate(__u32 cnode_idx, uintptr_t user_vstart, uintptr_t binder_vstart, size_t size);
#else
struct binder_mem;
struct hmb_mem;

DECLARE_MCACHE_VARIANT(binder_pages_variant)

struct binder_mem_stats {
	int active_pages;
	int on_shrinker_pages;
	int free_pages;
	int pages_high;
};

void binderfs_mem_set_vfs_mem_info(struct binder_mem *bmem, struct hmb_mem *vfs_mem_info);
bool binderfs_mem_is_inited(struct binder_mem *bmem);
int binderfs_pages_shrinker_init(void);

int binderfs_mem_populate(struct binder_mem *bmem, uintptr_t vstart, uintptr_t vend);
void binderfs_mem_unpopulate(struct binder_mem *bmem, uintptr_t vstart, uintptr_t vend);
void binderfs_mem_release(struct binder_mem *bmem);

int binderfs_copy_user_to_binder_mem(struct binder_mem *bmem, uintptr_t vstart, void *from, size_t len);
int binderfs_copy_to_binder_mem(struct binder_mem *bmem, uintptr_t vstart, void *src, size_t len);
int binderfs_copy_from_binder_mem(struct binder_mem *bmem, uintptr_t vstart, void *dst, size_t len);
int binderfs_clear_binder_mem(struct binder_mem *bmem, uintptr_t vstart, size_t len);

void binderfs_query_binder_mem_stats(struct binder_mem *bmem, struct binder_mem_stats *mem_stats);
int binderfs_query_pages_count(void);
#endif

#endif /* FS_BINDERFS_H */
