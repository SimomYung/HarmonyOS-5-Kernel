/*
 * drivers/secmem/mm_vltmm.h
 *
 * Copyright (C) 2022 Hisilicon, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef __MM_SECMEM_H
#define __MM_SECMEM_H

#include <linux/cma.h>
#include <linux/of_reserved_mem.h>

enum {
	SMEM_SECCM_XM = 0,
	SMEM_SECOS_1M = 1,
	SMEM_SECOS_2M = 2,
	SMEM_HEAPS_MAX,
};

enum {
	ASYNC_ALLOC,
	ASYNC_FREE,
};

enum {
	SUB_CMD_FREE,
	SUB_CMD_DUMP,
	SUB_CMD_UT_MALLOC,
	SUB_CMD_UT_FREE,
	SUB_CMD_UT_SHAREMEM,
	SUB_CMD_UT_PERF,     /* malloc N timer, then all free */
	SUB_CMD_UT_PERF_EXT, /* malloc-free N times */
	SUB_CMD_GET_AVAIL,
};

struct smem_heap {
	u64 avail;
	u64 used;
	u64 min;
	u64 low;
	u64 high;
	u64 max;
	u64 gran;
	u64 sid;
	u64 init;
	struct cma *cma;
	phys_addr_t *phy_array;
	struct task_struct *alloc_task;
	atomic_t alloc_run;
	struct mutex mutex;
	wait_queue_head_t alloc_wq;
	atomic_t wait_flag;
	unsigned long watermark;
	u64 escape_flag;
};

struct smem_free_task {
	struct task_struct *free_task;
	wait_queue_head_t free_wq;
	atomic_t wait_flag;
};

#define SMEM_SCAN_CNT   8
#define SMEM_GRAN_CNT   128

/* num of gran */
#define SMEM_DEFAULT_MIN    0x2
#define SMEM_DEFAULT_LOW    0x20
#define SMEM_DEFAULT_HIGH   0x60

#define SMEM_GRAN_2M        0x200000
#define SMEM_POOL_MAX       0x10000000

#define SMEM_SHRINKER_NUM         16

#define SMEM_TEST_MAGIC 0xAA

int __smem_allocator_msg_alloc(uint32_t sid, phys_addr_t *data, uint32_t len, uint32_t num);
void __smem_allocator_msg_free(uint32_t sid, phys_addr_t *data, uint32_t num);
int smemheaps_init(void);
void smem_sysfs_init(void);

int smemheap_setup(struct reserved_mem *rmem, struct cma *cma);

#endif
