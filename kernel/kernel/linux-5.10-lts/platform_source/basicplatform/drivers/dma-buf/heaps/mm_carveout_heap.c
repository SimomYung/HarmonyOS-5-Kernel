/*
 * drivers/dma-buf/heaps/mm_carveout_heap.c
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
#define pr_fmt(fmt) "[carveout_heap]" fmt

#include <linux/spinlock.h>
#include <linux/dma-buf.h>
#include <linux/dma-mapping.h>
#include <linux/dma-heap.h>
#include <linux/err.h>
#include <linux/genalloc.h>
#include <linux/highmem.h>
#include <linux/io.h>
#include <linux/mm.h>
#include <linux/of.h>
#include <linux/scatterlist.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/dmabuf/mm_dma_heap.h>

#include "mm_heap_helpers.h"
#include "deferred-free-helper.h"
#include "mm_heap_priv.h"

#define CARVEOUT_ALLOCATE_FAIL	(-1)

struct carveout_heap {
	struct dma_heap heap;
	const char *name;
	struct gen_pool *pool;
	phys_addr_t heap_base;
	size_t heap_size;
};

static void __carveout_heap_buf_free(struct deferred_freelist_item *item,
				 enum df_reason reason)
{
	struct mm_heap_helper_buffer *buffer = NULL;

	buffer = container_of(item, struct mm_heap_helper_buffer, deferred_free);
	/* Zero the buffer pages before adding back to the pool */
	mm_dma_heap_buffer_zero(buffer);

	if (buffer->vmap_cnt > 0) {
		pr_warn_once("%s: buffer still mapped in the kernel\n",
			     __func__);
		buffer->heap->ops->unmap_kernel(buffer->heap, buffer);
	}
	buffer->heap->ops->free(buffer);

	kfree(buffer);
}

static void carveout_heap_buf_free(struct mm_heap_helper_buffer *buffer)
{
	int npages = PAGE_ALIGN(buffer->size) / PAGE_SIZE;

	deferred_free(&buffer->deferred_free, __carveout_heap_buf_free, npages);
}

static phys_addr_t __carveout_allocate(struct carveout_heap *carveout_heap,
					 unsigned long size)
{
	unsigned long offset;

	offset = gen_pool_alloc(carveout_heap->pool, size);
	if (!offset)
		return CARVEOUT_ALLOCATE_FAIL;/*lint !e570*/

	return offset;
}

static void __carveout_free(struct carveout_heap *carveout_heap,
		phys_addr_t addr, unsigned long len)
{
	if (addr == CARVEOUT_ALLOCATE_FAIL)
		return;

	gen_pool_free(carveout_heap->pool, addr, len);
}

static struct dma_buf *carveout_heap_allocate(struct dma_heap *heap,
					    unsigned long len,
					    unsigned long fd_flags,
					    unsigned long heap_flags)
{
	struct carveout_heap *carveout_heap =
		container_of(heap, struct carveout_heap, heap);
	struct mm_heap_helper_buffer *buffer = NULL;
	struct sg_table *table = NULL;
	struct dma_buf *dmabuf = NULL;
	phys_addr_t paddr;
	int ret;

	if (dma_buf_is_secure(heap_flags)) {
		pr_err("%s: flag[0x%lx] err, sec_flags was set\n", __func__, heap_flags);
		return ERR_PTR(-EINVAL);
	}

	buffer = kzalloc(sizeof(*buffer), GFP_KERNEL);
	if (!buffer)
		return ERR_PTR(-ENOMEM);

	init_mm_heap_helper_buffer(buffer, carveout_heap_buf_free);
	buffer->heap = heap;
	buffer->size = len;
	buffer->flags = heap_flags;
	buffer->magic = atomic64_inc_return(&dmaheap_magic);

	table = kzalloc(sizeof(*table), GFP_KERNEL);
	if (!table)
		goto free_buffer;

	ret = sg_alloc_table(table, 1, GFP_KERNEL);
	if (ret)
		goto free_table;

	paddr = __carveout_allocate(carveout_heap, len);
	if (paddr == CARVEOUT_ALLOCATE_FAIL)
		goto free_sglist;

	sg_set_page(table->sgl, pfn_to_page(PFN_DOWN(paddr)), len, 0);
	sg_dma_address(table->sgl) = sg_phys(table->sgl);
	sg_dma_len(table->sgl) = table->sgl->length;
	buffer->sg_table = table;

	/* create the dmabuf */
	dmabuf = mm_heap_helper_export_dmabuf(buffer, fd_flags);
	if (IS_ERR(dmabuf)) {
		ret = PTR_ERR(dmabuf);
		goto free_paddr;
	}
	buffer->dmabuf = dmabuf;
	dma_heap_flush_buffer_cache(buffer, heap_flags);

	mm_dmaheap_init_dump(buffer);
	mm_dmaheap_buffer_add(buffer);

	return dmabuf;

free_paddr:
	__carveout_free(carveout_heap, paddr, len);
free_sglist:
	sg_free_table(table);
free_table:
	kfree(table);
free_buffer:
	kfree(buffer);

	pr_err("%s: [%s]alloc fail, size[0x%lx], heap_size [0x%lx]\n",
		__func__, carveout_heap->name, len, carveout_heap->heap_size);
	return ERR_PTR(-EINVAL);
}

static void carveout_heap_free(struct mm_heap_helper_buffer *buffer)
{
	struct dma_heap *heap = buffer->heap;
	struct carveout_heap *carveout_heap =
		container_of(heap, struct carveout_heap, heap);
	struct sg_table *table = buffer->sg_table;
	struct page *page = sg_page(table->sgl);
	phys_addr_t paddr = PFN_PHYS(page_to_pfn(page));

	__carveout_free(carveout_heap, paddr, buffer->size);
	sg_free_table(table);
	kfree(table);
}

static struct dma_heap_ops carveout_heap_ops = {
	.allocate = carveout_heap_allocate,
	.free = carveout_heap_free,
	.map_user = mm_dma_heap_map_user,
	.map_kernel = mm_dma_heap_map_kernel,
	.unmap_kernel = mm_dma_heap_unmap_kernel,
};

static int carveout_parse_dt(struct carveout_heap *carveout_heap,
						struct device_node *np)
{
	unsigned int heap_base = 0;
	unsigned int heap_size = 0;
	int ret;

	ret = of_property_read_string(np, "heap-name", &carveout_heap->name);
	if (ret < 0) {
		pr_err("%s, can not find heap-name\n", __func__);
		goto out;
	}

	ret = of_property_read_u32(np, "heap-base", &heap_base);
	if (ret < 0) {
		pr_err("%s, can not find [%s] heap-base\n", __func__, carveout_heap->name);
		goto out;
	}
	carveout_heap->heap_base = heap_base;

	ret = of_property_read_u32(np, "heap-size", &heap_size);
	if (ret < 0) {
		pr_err("%s, can not find [%s] heap-size\n", __func__, carveout_heap->name);
		goto out;
	}
	carveout_heap->heap_size = heap_size;

out:
	return ret;
}

static int carveout_heap_create(struct device_node *np)
{
	struct carveout_heap *carveout_heap = NULL;
	struct page *page = NULL;
	size_t size;
	int ret;

	carveout_heap = kzalloc(sizeof(*carveout_heap), GFP_KERNEL);
	if (!carveout_heap)
		return -ENOMEM;

	ret = carveout_parse_dt(carveout_heap, np);
	if (ret) {
		pr_err("%s, carveout_heap parse err!\n", __func__);
		goto free_heap;
	}
	kref_init(&carveout_heap->heap.refcount);
	carveout_heap->heap.name = carveout_heap->name;
	carveout_heap->heap.ops = &carveout_heap_ops;
	carveout_heap->heap.priv = NULL;

	page = pfn_to_page(PFN_DOWN(carveout_heap->heap_base));
	size = carveout_heap->heap_size;

	ret = mm_dma_heap_pages_zero(page, size,
				pgprot_writecombine(PAGE_KERNEL));
	if (ret)
		goto free_heap;

	carveout_heap->pool = gen_pool_create(PAGE_SHIFT, -1);
	if (!carveout_heap->pool)
		goto free_heap;

	gen_pool_add(carveout_heap->pool, carveout_heap->heap_base,
				carveout_heap->heap_size, -1);

	ret = dma_heap_add(&carveout_heap->heap);
	if (ret) {
		pr_err("%s: dma_heap_add[%s] failed!\n", __func__, carveout_heap->name);
		goto free_pool;
	}
	dma_coerce_mask_and_coherent(dma_heap_get_dev(&carveout_heap->heap),
						DMA_BIT_MASK(64));

	return 0;

free_pool:
	gen_pool_destroy(carveout_heap->pool);
free_heap:
	kfree(carveout_heap);

	return -EINVAL;
}

static int carveout_heap_init(void)
{
	int ret = 0;
	struct device_node *carveout_np = NULL, *child_np = NULL;

	if (!dma_heap_node) {
		pr_err("%s, can not find dma_heap node\n", __func__);
		return -EINVAL;
	}

	carveout_np = of_get_child_by_name(dma_heap_node, "carveout_heap");
	if (!carveout_np) {
		pr_err("%s, can not find carveout_heap node\n", __func__);
		return -EINVAL;
	}

	for_each_available_child_of_node(carveout_np, child_np) {
		ret = carveout_heap_create(child_np);
		if (ret)
			pr_err("%s, create[%s] fail\n", __func__, child_np->name);
	}

	return 0;
}

module_init(carveout_heap_init);
MODULE_LICENSE("GPL v2");
