/* SPDX-License-Identifier: GPL-2.0 */
/*
 * DMABUF Heaps Allocation Infrastructure
 *
 * Copyright (C) 2011 Google, Inc.
 * Copyright (C) 2019 Linaro Ltd.
 */

#ifndef _DMA_HEAPS_H
#define _DMA_HEAPS_H

#include <linux/cdev.h>
#include <linux/types.h>

#include "../uapi/linux/dma-heap.h"

#ifdef CONFIG_DMABUF_MM
extern struct device_node *dma_heap_node;
struct mm_heap_helper_buffer;
#endif
#define MAX_WATER_MARK (SZ_1M * 400) /* 400MB */

/**
 * struct dma_heap - represents a dmabuf heap in the system
 * @name:		used for debugging/device-node name
 * @ops:		ops struct for this heap
 * @heap_devt		heap device node
 * @list		list head connecting to list of heaps
 * @heap_cdev		heap char device
 * @heap_dev		heap device struct
 *
 * Represents a heap of memory from which buffers can be made.
 */
struct dma_heap {
	const char *name;
	const struct dma_heap_ops *ops;
	void *priv;
	dev_t heap_devt;
	struct list_head list;
	struct cdev heap_cdev;
	struct kref refcount;
	struct device *heap_dev;
};

struct dma_heap_attachment {
	struct device *dev;
	struct sg_table *table;
	struct list_head list;
	bool mapped;
	bool uncached;
};

/**
 * struct dma_heap_ops - ops to operate on a given heap
 * @allocate:		allocate dmabuf and return fd
 *
 * allocate returns dmabuf fd  on success, -errno on error.
 */
struct dma_heap_ops {
	struct dma_buf *(*allocate)(struct dma_heap *heap,
			unsigned long len,
			unsigned long fd_flags,
			unsigned long heap_flags);
#ifdef CONFIG_DMABUF_MM
	void (*free)(struct mm_heap_helper_buffer *buffer);
	void * (*map_kernel)(struct dma_heap *heap,
			struct mm_heap_helper_buffer *buffer);
	void (*unmap_kernel)(struct dma_heap *heap,
			struct mm_heap_helper_buffer *buffer);
	int (*map_user)(struct dma_heap *mapper,
			struct mm_heap_helper_buffer *buffer, struct vm_area_struct *vma);
	int (*get_heap_phys)(struct dma_heap *heap, struct dma_buf *dmabuf,
				phys_addr_t *addr, size_t *len);
	long (*get_pool_size)(struct dma_heap *heap);
#endif
};

/**
 * struct dma_heap_export_info - information needed to export a new dmabuf heap
 * @name:	used for debugging/device-node name
 * @ops:	ops struct for this heap
 * @priv:	heap exporter private data
 *
 * Information needed to export a new dmabuf heap.
 */
struct dma_heap_export_info {
	const char *name;
	const struct dma_heap_ops *ops;
	void *priv;
};

/**
 * dma_heap_get_drvdata() - get per-heap driver data
 * @heap: DMA-Heap to retrieve private data for
 *
 * Returns:
 * The per-heap data for the heap.
 */
void *dma_heap_get_drvdata(struct dma_heap *heap);

/**
 * dma_heap_get_name() - get heap name
 * @heap: DMA-Heap to retrieve private data for
 *
 * Returns:
 * The char* for the heap name.
 */
const char *dma_heap_get_name(struct dma_heap *heap);

/**
 * dma_heap_add - adds a heap to dmabuf heaps
 * @heap:		heap pointer
 */
int dma_heap_add(struct dma_heap *heap);

#ifdef CONFIG_DMABUF_MM
/**
 * dma_heap_get_dev() - get device struct for the heap
 * @heap: DMA-Heap to retrieve device struct from
 *
 * Returns:
 * The device struct for the heap.
 */
struct device *dma_heap_get_dev(struct dma_heap *heap);

/**
 * dma_heap_get_name() - get heap name
 * @heap: DMA-Heap to retrieve private data for
 *
 * Returns:
 * The char* for the heap name.
 */
const char *dma_heap_get_name(struct dma_heap *heap);

/**
 * dma_heap_put - drops a reference to a dmabuf heaps, potentially freeing it
 * @heap:		heap pointer
 */
void dma_heap_put(struct dma_heap *heap);

/**
 * dma_heap_find - Returns the registered dma_heap with the specified name
 * @name: Name of the heap to find
 *
 * NOTE: dma_heaps returned from this function MUST be released
 * using dma_heap_put() when the user is done.
 */
struct dma_heap *dma_heap_find(const char *name);

/**
 * dma_heap_buffer_alloc - Allocate dma-buf from a dma_heap
 * @heap:	dma_heap to allocate from
 * @len:	size to allocate
 * @fd_flags:	flags to set on returned dma-buf fd
 * @heap_flags:	flags to pass to the dma heap
 *
 * This is for internal dma-buf allocations only.
 */
struct dma_buf *dma_heap_buffer_alloc(struct dma_heap *heap, size_t len,
				      unsigned int fd_flags,
				      unsigned long heap_flags);

/** dma_heap_buffer_free - Free dma_buf allocated by dma_heap_buffer_alloc
 * @dma_buf:	dma_buf to free
 *
 * This is really only a simple wrapper to dma_buf_put()
 */
void dma_heap_buffer_free(struct dma_buf *);

/**
 * dma_heap_buffer_alloc_kernel - Allocate dma-buf from a dma_heap
 * @name:	dma_heap name to allocate from
 * @len:	size to allocate
 * @heap_flags:	flags to pass to the dma heap
 *
 * This is for kernel dma-buf allocations only (Video/Camera).
 */
struct dma_buf *dma_heap_buffer_alloc_kernel(const char *name, size_t len,
				unsigned long heap_flags);

/**
 * dma_heap_buffer_free_kernel - Free dma_buf allocated by dma_heap_buffer_alloc_kernel
 * @dma_buf:	dma_buf to free
 *
 * This is for kernel dma-buf free only (Video/Camera).
 */
void dma_heap_buffer_free_kernel(struct dma_buf *dmabuf);

/**
 * dma_heap_bufferfd_alloc - Allocate dma-buf fd from a dma_heap
 * @heap:	dma_heap to allocate from
 * @len:	size to allocate
 * @fd_flags:	flags to set on returned dma-buf fd
 * @heap_flags:	flags to pass to the dma heap
 */
int dma_heap_bufferfd_alloc(struct dma_heap *heap, size_t len,
			    unsigned int fd_flags,
			    unsigned long heap_flags);

void dma_heap_bufferfd_free(int fd);
#ifdef CONFIG_LIBLINUX_CDC
/* Currently no heap flags */
#define DMA_HEAP_VALID_HEAP_FLAGS (0)
/**
 * dma_heap_alloc - Allocate dma-buf from a dma_heap
 * @heap_id:	type of dma heap
 * @len:	size to allocate
 * @heap_flags:	flags to pass to the dma heap
 */
int dma_heap_alloc(unsigned int heap_id, size_t len, unsigned int heap_flags);
#endif /* CONFIG_LIBLINUX_CDC */
#endif /* CONFIG_DMABUF_MM */

#endif /* _DMA_HEAPS_H */
