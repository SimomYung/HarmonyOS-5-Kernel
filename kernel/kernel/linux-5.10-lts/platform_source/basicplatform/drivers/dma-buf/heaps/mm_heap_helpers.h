/*
 * drivers/dma-buf/heaps/mm_heap_helpers.h
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
#ifndef _MM_HEAP_HELPERS_H
#define _MM_HEAP_HELPERS_H

#include <linux/dma-heap.h>
#include <linux/dma-buf.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/mutex.h>
#include <linux/scatterlist.h>
#include "deferred-free-helper.h"


/**
 * struct mm_heap_helper_buffer - helper buffer metadata
 * @magic:		buffer magic
 * @node:		buffer node
 * @list:		point to helper_buffer list
 * @heap:		back pointer to the heap the buffer came from
 * @dmabuf:		backing dma-buf for this buffer
 * @size:		size of the buffer
 * @priv_virt		pointer to heap specific private value
 * @lock		mutext to protect the data in this structure
 * @vmap_cnt		count of vmap references on the buffer
 * @vaddr		vmap'ed virtual address
 * @attachments		list of device attachments
 * @task_comm		allocated task name for buffer
 * @pid		allocated process id for buffer
 * @id			secmem buffer id
 *
 * @free		heap callback to free the buffer
 * @deferred_freelist_item  deferred free item
 */
struct mm_heap_helper_buffer {
	u64 magic;
	union {
		struct rb_node node;
		struct list_head list;
	};
	struct dma_heap *heap;
	struct dma_buf *dmabuf;
	size_t size;
	unsigned long flags;
	void *priv_virt;
	struct mutex lock;
	int vmap_cnt;
	void *vaddr;
	struct sg_table *sg_table;
	struct list_head attachments;
	char task_comm[TASK_COMM_LEN];
	pid_t pid;
	unsigned int id;
	ktime_t alloc_time_start;
	ktime_t alloc_time_end;

#ifdef CONFIG_MM_LB
	unsigned int plc_id;
	unsigned long offset;
	size_t lb_size;
#endif
#ifdef CONFIG_DMABUF_RECLAIM
	int *extid_table;
	int extid_num;
	atomic_t attach_num;
	struct buffer_map_info *info;
	struct rw_semaphore rwsem;
	bool is_reclaim;
#endif
	void (*free)(struct mm_heap_helper_buffer *buffer);
	struct deferred_freelist_item deferred_free;
	unsigned int adjust_prot_flag;
};

void init_mm_heap_helper_buffer(struct mm_heap_helper_buffer *buffer,
			     void (*free)(struct mm_heap_helper_buffer *));

struct dma_buf *mm_heap_helper_export_dmabuf(struct mm_heap_helper_buffer *buffer,
					  int fd_flags);

extern const struct dma_buf_ops mm_heap_helper_ops;
#ifdef CONFIG_TRANSPARENT_HUGEPAGE_DEVICE
extern void prep_compound_page(struct page *page, unsigned int order);
#endif
/* dma_heap ops */
int mm_dma_heap_map_user(struct dma_heap *heap,
		struct mm_heap_helper_buffer *buffer, struct vm_area_struct *vma);
void *mm_dma_heap_map_kernel(struct dma_heap *heap,
		struct mm_heap_helper_buffer *buffer);
void mm_dma_heap_unmap_kernel(struct dma_heap *heap,
		struct mm_heap_helper_buffer *buffer);
int mm_heap_mutex_lock_recursive(struct mutex *lock);
void mm_heap_mutex_unlock_recursive(struct mutex *lock, int is_lock_recursive);
#endif /* _MM_HEAP_HELPERS_H */
