/*
 * drivers/dma-buf/heaps/mm_heap_priv.h
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

#ifndef __MM_HEAP_PRIV_H
#define __MM_HEAP_PRIV_H

#include <linux/dma-heap.h>
#include <linux/dma-buf.h>

#define MM_DMA_HEAP_FLUSH_ALL_CPUS_CACHES	(18 * SZ_1M)

struct mm_heap_helper_buffer;

#ifndef CONFIG_MM_LB_FULL_COHERENCY
void dma_heap_flush_all_cpus_caches(void);
#ifdef CONFIG_MM_DMA_HEAP_FLUSH_CACHE_ALL
void dma_heap_flush_all_cpus_caches_raw(void);
#endif
#else
void dma_ci_sgt(struct sg_table *table);
void dma_ci_pages(struct page *start_pg, u64 size);
#endif

void dma_heap_flush_buffer_cache(struct mm_heap_helper_buffer *buffer,
				unsigned long heap_flags);
#ifdef CONFIG_DMABUF_RECLAIM
void dma_heap_direct_flush_buffer_cache(struct mm_heap_helper_buffer *buffer);
#endif
/* dma_pool_heap */
#ifdef CONFIG_DMABUF_HEAPS_DMA_POOL
void dma_pool_heap_register_dma_camera_cma(void *p);
#else
static inline void dma_pool_heap_register_dma_camera_cma(void *p)
{
}
#endif /* CONFIG_DMABUF_HEAPS_DMA_POOL */

#ifdef CONFIG_DMABUF_MM
extern atomic_long_t dmaheap_alloc_total_size;
extern atomic_long_t dmaheap_magic;
extern struct rb_root dmaheap_buffers_rb;
extern struct mutex dmaheap_buffers_lock;
extern struct dentry *heaps_debug_root;
extern const struct file_operations debug_heap_fops;

unsigned long get_dmaheap_alloc_total_size(void);
void mm_dmaheap_buffer_add(struct mm_heap_helper_buffer *buffer);
void mm_dmaheap_buffer_del(struct mm_heap_helper_buffer *buffer);

struct dma_buf *cpa_fallback_alloc(struct dma_heap *heap, size_t len,
		unsigned int fd_flags, unsigned long heap_flags);
int mm_dmaheap_debug_node_init(void);
void mm_dmaheap_init_dump(struct mm_heap_helper_buffer *buffer);
void mm_svc_secmem_info(void);
int mm_dmaheap_heap_show(struct dma_heap *heap);
void mm_dmaheap_system_heap_info(struct seq_file *s, struct dma_heap *heap);
#else
static inline unsigned long get_dmaheap_alloc_total_size(void)
{
	return 0;
}

static inline void mm_dmaheap_buffer_add(struct mm_heap_helper_buffer *buffer)
{
}

static inline void mm_dmaheap_buffer_del(struct mm_heap_helper_buffer *buffer)
{
}

static inline struct dma_buf *cpa_fallback_alloc(struct dma_heap *heap, size_t len,
		unsigned int fd_flags, unsigned int heap_flags);
{
	return ERR_PTR(-ENOMEM);
}

static inline int mm_dmaheap_debug_node_init(void)
{
	return -EINVAL;
}

static inline void mm_dmaheap_init_dump(struct mm_heap_helper_buffer *buffer)
{
}

static inline void mm_svc_secmem_info(void)
{
}

static int mm_dmaheap_heap_show(struct dma_heap *heap)
{
	return 0;
}

static inline void mm_dmaheap_system_heap_info(struct dma_heap *heap)
{
}
#endif /* CONFIG_DMABUF_MM */

#endif
