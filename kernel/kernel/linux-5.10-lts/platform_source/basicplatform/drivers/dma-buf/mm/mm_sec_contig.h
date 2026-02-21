/*
 * drivers/dma-buf/mm/mm_sec_contig.h
 *
 * Copyright(C) 2022 Hisilicon Technologies Co., Ltd. All rights reserved.
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

#ifndef _MM_SEC_CONTIG_H
#define _MM_SEC_CONTIG_H

#include <linux/err.h>
#include <linux/version.h>
#include <linux/cma.h>
#include <linux/dma-buf.h>
#include <linux/dma-heap.h>
#include <linux/genalloc.h>
#include "mm_tee_op.h"

enum SECCG_CMA_TYPE {
	CMA_STATIC = 0,
	CMA_DYNAMIC = 1,
	CMA_TYPE_MAX,
};

#define DMABUF_PBL_SHIFT 12
#define DMABUF_NORMAL_SHIFT 16

struct seccg_heap {
	struct dma_heap heap;
	struct cma *cma;
	struct gen_pool *pool;
	const char *name;
	/* heap mutex */
	struct mutex mutex;
	/* heap attr: secure, normal */
	u32 heap_attr;
	u32 pool_shift;
	phys_addr_t heap_base;
	/* heap total size */
	size_t heap_size;
	/* heap allocated size */
	unsigned long alloc_size;
	/* heap alloc cma size */
	unsigned long cma_alloc_size;
	/* heap flag */
	u64 flag;
	u64 per_alloc_sz;
	/* align size */
	u64 per_bit_sz;
	u64 water_mark;
	struct list_head allocate_head;
	struct dma_heap_sec_tee tee;
	int ta_init;
	u32 cma_type;
	int heap_svc_id;
	bool pool_disabled;
	struct gen_pool *static_cma_region;
};

struct dma_heap_sec_cma {
	struct cma *cma_region;
	const char *cma_name;
};

struct alloc_list {
	u64 addr;
	u32 size;
	struct list_head list;
};

struct mm_heap_helper_buffer;

struct dma_buf *seccg_alloc_contig(struct seccg_heap *seccg_heap,
		struct mm_heap_helper_buffer *buffer, unsigned long size, unsigned long fd_flags);
void seccg_free_contig(struct seccg_heap *seccg_heap,
			 struct mm_heap_helper_buffer *buffer);
int seccg_fill_watermark(struct seccg_heap *seccg_heap);
#endif
