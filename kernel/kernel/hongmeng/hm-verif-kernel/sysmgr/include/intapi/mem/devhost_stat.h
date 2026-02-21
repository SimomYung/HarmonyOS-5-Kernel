/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: Memory statistics for devhost/ldk
 * Author: Huawei OS Kernel Lab
 * Create: Fir Mar 15 12:05:01 2024
 */
#ifndef SYSMGR_MEM_DEVHOST_STAT_H
#define SYSMGR_MEM_DEVHOST_STAT_H

#include <intapi/mem/vspace.h>
#include <libhmsync/atomic.h>

/* for devhost/ldk share common devhost_stat (no more than PAGE_SIZE)
 * Note It should be consistent with ldk_stat (samples/ldk/ldk_stat.h)
 */
typedef struct devhost_mem_stat {
	raw_atomic_long_t dmaheap_free_pages;
	raw_atomic_long_t buddy_dma_free_pages;
	raw_atomic_long_t system_free_pages;
} raw_devhost_mem_stat_t;

typedef struct sys_mem_stat {
	raw_atomic_long_t avail_buf_wmark_min;
	raw_atomic_long_t avail_buf_wmark_mid;
	raw_atomic_long_t avail_buf_wmark_high;
	raw_atomic_long_t curr_avail_buf;
} raw_sys_mem_stat_t;

typedef struct devhost_stat {
	raw_devhost_mem_stat_t mem_stat;
	raw_sys_mem_stat_t sys_mem_stat;
} raw_devhost_stat_t;

bool devhost_stat_is_register(void);
int devhost_stat_register(struct vspace_s *vspace, const void *stat_addr, size_t stat_size);
int devhost_stat_deregister(struct vspace_s *vspace);
unsigned long devhost_stat_buddy_dma_free_pages_get(void);
unsigned long devhost_stat_dmaheap_free_pages_get(void);
unsigned long devhost_stat_fast_free_pages_get(void);
void devhost_stat_system_free_pages_set(uint64_t free_mem);
void set_avail_buf_wmarks_to_devhost(size_t avail_buf_mb, size_t min_avail_buf_mb, size_t high_avail_buf_mb);
void set_curr_avail_buf_to_devhost(size_t val);

#endif
