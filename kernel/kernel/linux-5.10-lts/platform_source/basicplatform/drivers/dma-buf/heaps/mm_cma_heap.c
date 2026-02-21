/*
 * drivers/dma-buf/heaps/mm_cma_heap.c
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
#define pr_fmt(fmt) "[cma_heap]" fmt

#include <linux/atomic.h>
#include <linux/cma.h>
#include <linux/dma-buf.h>
#include <linux/dma-mapping.h>
#include <linux/dma-heap.h>
#include <linux/dma-map-ops.h>
#include <linux/err.h>
#include <linux/highmem.h>
#include <linux/io.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/sizes.h>
#include <linux/scatterlist.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/dmabuf/mm_dma_heap.h>

#ifdef CONFIG_TRANSPARENT_HUGEPAGE_DEVICE  
#include <linux/of_fdt.h>
#include <linux/of_reserved_mem.h>
#include <linux/page-flags.h>
#include <linux/huge_mm.h>
#endif

#include "mm_heap_priv.h"
#include "mm_heap_helpers.h"

#define CMA_HEAP_SUCC	(1)

struct cma_heap {
	struct dma_heap heap;
	const char *heap_name;
	struct cma *cma;
	const char *cma_name;
	/* heap total size */
	size_t heap_size;
	/* heap allocated size */
	unsigned long alloc_size;
#ifdef CONFIG_TRANSPARENT_HUGEPAGE_DEVICE
	size_t heap_align_size;
#endif
};

#ifdef CONFIG_TRANSPARENT_HUGEPAGE_DEVICE
static int mm_hugepage_cma_setup(struct reserved_mem *rmem)
{
	phys_addr_t align = PAGE_SIZE << max(MAX_ORDER - 1, pageblock_order);
	phys_addr_t mask = align - 1;
	unsigned long node = rmem->fdt_node;
	struct cma *cma = NULL;
	int err;

	if (!of_get_flat_dt_prop(node, "reusable", NULL) ||
	    of_get_flat_dt_prop(node, "no-map", NULL)) {
		pr_err("%s err node\n", __func__);
		return -EINVAL;
	}

	if ((rmem->base & mask) || (rmem->size & mask)) {
		pr_err("Reserved memory: incorrect alignment of CMA region.\n");
		return -EINVAL;
	}

	err = cma_init_reserved_mem(rmem->base, rmem->size, 0,
				    rmem->name, &cma);
	if (err) {
		pr_err("Reserved memory: unable to setup CMA region\n");
		return err;
	}

	pr_info("Reserved memory: created CMA memory pool at %pa, size %lu MiB\n",
		&rmem->base, (unsigned long)rmem->size / SZ_1M);

	return 0;
}
RESERVEDMEM_OF_DECLARE(npu_hugepage_cma, "npu-hugepage-pool", mm_hugepage_cma_setup);

static unsigned long mm_cma_alloc_huge_pages(struct mm_heap_helper_buffer *buffer, unsigned long len, struct list_head *huge_pages)
{
	struct cma_heap *cma_heap = dma_heap_get_drvdata(buffer->heap);
	unsigned long nr_huge_pages = 0;
	struct page *page;
	unsigned long align = cma_heap->heap_align_size;
 
	if (len < SZ_2M)
		return nr_huge_pages;

	while (len > 0) {
		page = cma_alloc(cma_heap->cma, SZ_2M >> PAGE_SHIFT, (u32)get_order((u32)align), GFP_KERNEL);
		if (!page)
			break;
		len -= SZ_2M;
		prep_compound_page(page, get_order(SZ_2M));
		list_add_tail(&page->lru, huge_pages);
		nr_huge_pages++;
	}

	pr_err("%s: alloced hugepage nr %lu\n", __func__, nr_huge_pages);
	return nr_huge_pages;
}

static unsigned long mm_cma_alloc_buddy_pages(unsigned long len, struct list_head *buddy_pages)
{
	unsigned long nr_buddy_pages = 0;
	struct page *page = NULL, *tmp_page = NULL;
	if (len < SZ_4K)
		return nr_buddy_pages;

	while (len > 0) {
		struct page *page = alloc_pages(GFP_KERNEL, 0);
		if (!page)
			goto free_pages;
		list_add_tail(&page->lru, buddy_pages);
		len -= SZ_4K;
		nr_buddy_pages++;
	}
	
	return nr_buddy_pages;
free_pages:
	list_for_each_entry_safe(page, tmp_page, buddy_pages, lru) {
		__free_pages(page, 0);
	}
	return 0;
}

static void mm_add_hybrid_pages_to_sg(struct sg_table *table, struct list_head *huge_pages, struct list_head *buddy_pages)
{
	struct page *page = NULL, *tmp_page = NULL;
	struct scatterlist *sg = table->sgl;

	if (!sg)
		return;

	if (huge_pages != NULL) {
		list_for_each_entry_safe(page, tmp_page, huge_pages, lru) {
			sg_set_page(sg, page, page_size(page), 0);
			sg_dma_address(sg) = sg_phys(sg);
			sg_dma_len(sg) = sg->length;
			sg = sg_next(sg);
			list_del(&page->lru);
		}
	}

	if (buddy_pages != NULL) {
			list_for_each_entry_safe(page, tmp_page, buddy_pages, lru) {
			sg_set_page(sg, page, page_size(page), 0);
			sg_dma_address(sg) = sg_phys(sg);
			sg_dma_len(sg) = sg->length;
			sg = sg_next(sg);
			list_del(&page->lru);
		}
	}
}

static void mm_free_hybrid_pages(struct cma_heap *cma_heap, struct sg_table *table)
{
	struct page *page;
	int i;
	int cnt = 0;
	struct scatterlist *sg;

	for_each_sg(table->sgl, sg, table->nents, i) {
		page = sg_page(sg);
		if (!page)
			continue;
		if (PageCompound(page)) {
			split_huge_page(page);
			cma_release(cma_heap->cma, page, page_size(page) >> PAGE_SHIFT);
			cnt++;
		} else {
			__free_pages(page, 0);
		}
	}
	cma_heap->alloc_size -= cnt * SZ_2M;
	pr_err("%s: hugepage check release cma count %d\n", __func__, cnt);
}
#endif

static void cma_heap_buf_free(struct mm_heap_helper_buffer *buffer)
{
	struct cma_heap *cma_heap = dma_heap_get_drvdata(buffer->heap);
	struct sg_table *table = NULL;
	pgoff_t pagecount;

	if (buffer->vmap_cnt > 0) {
		WARN(1, "%s: buffer still mapped in the kernel\n", __func__);
		vunmap(buffer->vaddr);
	}

	table = buffer->sg_table;
#ifdef CONFIG_TRANSPARENT_HUGEPAGE_DEVICE  
	if (dma_buf_is_hugepage(buffer->flags)) {
		mm_free_hybrid_pages(cma_heap, table);
	} else {
#endif
	/* release memory */
	cma_heap->alloc_size -= buffer->size;
	pagecount = buffer->size >> PAGE_SHIFT;
	cma_release(cma_heap->cma, sg_page(table->sgl), pagecount);
#ifdef CONFIG_TRANSPARENT_HUGEPAGE_DEVICE  
	}
#endif

	sg_free_table(table);
	kfree(table);
	kfree(buffer);
}

static struct dma_buf *cma_heap_allocate(struct dma_heap *heap,
					 unsigned long len,
					 unsigned long fd_flags,
					 unsigned long heap_flags)
{
	struct cma_heap *cma_heap = dma_heap_get_drvdata(heap);
	struct mm_heap_helper_buffer *buffer = NULL;
	struct sg_table *table = NULL;
	size_t size = PAGE_ALIGN(len);
	pgoff_t pagecount = size >> PAGE_SHIFT;
	unsigned long align = get_order(size);
	struct page *cma_pages = NULL;
	struct dma_buf *dmabuf = NULL;
	int ret = -ENOMEM;

#ifdef CONFIG_TRANSPARENT_HUGEPAGE_DEVICE  
	LIST_HEAD(huge_pages);
	LIST_HEAD(buddy_pages);
	unsigned long nr_huge_pages = 0, nr_buddy_pages = 0;
	struct page *page = NULL, *tmp_page = NULL;
	unsigned long size_remaining = PAGE_ALIGN(len);
#endif

	if (dma_buf_is_secure(heap_flags)) {
		pr_err("%s: flag[0x%lx] err, sec_flags was set\n", __func__, heap_flags);
		return ERR_PTR(-EINVAL);
	}

	buffer = kzalloc(sizeof(*buffer), GFP_KERNEL);
	if (!buffer)
		return ERR_PTR(-ENOMEM);

	init_mm_heap_helper_buffer(buffer, cma_heap_buf_free);
	buffer->heap = heap;
	buffer->size = size;
	buffer->flags = heap_flags;
	buffer->magic = atomic64_inc_return(&dmaheap_magic);

	if (align > CONFIG_CMA_ALIGNMENT)
		align = CONFIG_CMA_ALIGNMENT;

	if (!strncmp("aod_heap", heap->name, strlen("aod_heap")))
		align = 0;

#ifdef CONFIG_TRANSPARENT_HUGEPAGE_DEVICE  
	if (dma_buf_is_hugepage(buffer->flags)) {
		nr_huge_pages = mm_cma_alloc_huge_pages(buffer, size_remaining, &huge_pages);
		unsigned long size_allocated = nr_huge_pages * SZ_2M;
		size_remaining = (size_remaining > size_allocated) ? (size_remaining - size_allocated) : 0;
		nr_buddy_pages = mm_cma_alloc_buddy_pages(size_remaining, &buddy_pages);
		size_allocated = nr_buddy_pages * SZ_4K;
		size_remaining = (size_remaining > size_allocated) ? (size_remaining - size_allocated) : 0;
		if (size_remaining)
			goto free_cma;
	} else {
#endif
	cma_pages = cma_alloc(cma_heap->cma, pagecount, align, GFP_KERNEL);
	if (!cma_pages)
		goto free_buffer;

	/* Clear the cma pages */
	if (PageHighMem(cma_pages)) {
		unsigned long nr_clear_pages = pagecount;
		struct page *page = cma_pages;

		while (nr_clear_pages > 0) {
			void *vaddr = kmap_atomic(page);

			memset(vaddr, 0, PAGE_SIZE);
			kunmap_atomic(vaddr);
			/*
			 * Avoid wasting time zeroing memory if the process
			 * has been killed by by SIGKILL
			 */
			if (fatal_signal_pending(current))
				goto free_cma;
			page++;
			nr_clear_pages--;
		}
	} else {
		memset(page_address(cma_pages), 0, size);
	}

#ifdef CONFIG_TRANSPARENT_HUGEPAGE_DEVICE  
	}
#endif

	table = kmalloc(sizeof(*table), GFP_KERNEL);
	if (!table)
		goto free_cma;

#ifdef CONFIG_TRANSPARENT_HUGEPAGE_DEVICE  
	if (dma_buf_is_hugepage(buffer->flags)) {
		if (sg_alloc_table(table, nr_huge_pages + nr_buddy_pages, GFP_KERNEL))
			goto free_table;
		mm_add_hybrid_pages_to_sg(table, &huge_pages, &buddy_pages);
	} else {
#endif
	if (sg_alloc_table(table, 1, GFP_KERNEL))
		goto free_table;

	sg_set_page(table->sgl, cma_pages, size, 0);
	sg_dma_address(table->sgl) = sg_phys(table->sgl);
	sg_dma_len(table->sgl) = table->sgl->length;

	buffer->priv_virt = cma_pages;

#ifdef CONFIG_TRANSPARENT_HUGEPAGE_DEVICE 
	}
#endif

	buffer->sg_table = table;
	/* create the dmabuf */
	dmabuf = mm_heap_helper_export_dmabuf(buffer, fd_flags);
	if (IS_ERR(dmabuf)) {
		ret = PTR_ERR(dmabuf);
		goto free_mem;
	}
	buffer->dmabuf = dmabuf;
#ifdef CONFIG_TRANSPARENT_HUGEPAGE_DEVICE
	if (dma_buf_is_hugepage(buffer->flags)) {
		cma_heap->alloc_size += nr_huge_pages * SZ_2M;
	} else {
#endif
	cma_heap->alloc_size += size;

#ifdef CONFIG_TRANSPARENT_HUGEPAGE_DEVICE 
	}
#endif
	dma_heap_flush_buffer_cache(buffer, heap_flags);

	mm_dmaheap_init_dump(buffer);
	mm_dmaheap_buffer_add(buffer);

	return dmabuf;

free_mem:
	sg_free_table(table);
free_table:
	kfree(table);
free_cma:
#ifdef CONFIG_TRANSPARENT_HUGEPAGE_DEVICE  
	list_for_each_entry_safe(page, tmp_page, &huge_pages, lru) {
		split_huge_page(page);
		cma_release(cma_heap->cma, page, page_size(page) >> PAGE_SHIFT);
	}
	list_for_each_entry_safe(page, tmp_page, &buddy_pages, lru) {
		__free_pages(page, 0);
	}
#endif
	cma_release(cma_heap->cma, cma_pages, pagecount);
free_buffer:
	kfree(buffer);

	pr_err("%s: [%s]alloc fail, size[0x%lx], heap all alloc_size[0x%lx], heap_size [0x%lx]\n",
		    __func__, cma_heap->heap_name, size, cma_heap->alloc_size, cma_heap->heap_size);
	return ERR_PTR(ret);
}

static const struct dma_heap_ops mm_cma_heap_ops = {
	.allocate = cma_heap_allocate,
	.map_user = mm_dma_heap_map_user,
	.map_kernel = mm_dma_heap_map_kernel,
	.unmap_kernel = mm_dma_heap_unmap_kernel,
};

static int add_mm_cma_heap(struct cma *cma, void *data)
{
	struct cma_heap *cma_heap = (struct cma_heap *)data;

	if (!cma_heap || !cma) {
		pr_err("cma_heap or cma is NULL\n");
		return 0;
	}

	if (strcmp(cma_get_name(cma), cma_heap->cma_name)) {
		pr_err("[%s] cma name: %s or %s\n", __func__,
			cma_get_name(cma), cma_heap->cma_name);
		return 0;
	}
	cma_heap->cma = cma;

	return CMA_HEAP_SUCC;
}

static int cma_heap_parse_dt(struct cma_heap *cma_heap,
						struct device_node *np)
{
	const char *cma_name = NULL;
	unsigned int heap_size = 0;
#ifdef CONFIG_TRANSPARENT_HUGEPAGE_DEVICE
	unsigned int heap_align_size = 0;
#endif
	int ret = 0;

	ret = of_property_read_string(np, "heap-name", &cma_heap->heap_name);
	if (!np) {
		pr_err("%s: can not find heap-name\n", __func__);
		ret = -EINVAL;
		goto out;
	}

	ret = of_property_read_u32(np, "heap-size", &heap_size);
	if (ret < 0) {
		pr_err("%s: [%s]invalid heap-size in node[%s]\n",
				__func__, cma_heap->heap_name, np->name);
		ret = -EINVAL;
		goto out;
	}
	cma_heap->heap_size = heap_size;

	ret = of_property_read_string(np, "cma-name",
			&cma_name);
	if (ret < 0) {
		pr_err("%s: [%s]invalid cma-name in node[%s].\n",
				__func__, cma_heap->heap_name, np->name);
		ret = -EINVAL;
		goto out;
	}
	cma_heap->cma_name = cma_name;

#ifdef CONFIG_TRANSPARENT_HUGEPAGE_DEVICE
	ret = of_property_read_u32(np, "heap-align", &heap_align_size);
	if (ret < 0) {
		pr_err("%s: [%s]invalid heap-align in node[%s].\n",
				__func__, cma_heap->heap_name, np->name);
		ret = -EINVAL;
		goto out;
	}
	cma_heap->heap_align_size = heap_align_size;
#endif

out:
	return ret;
}

static int cma_heap_prop(struct cma_heap *cma_heap,
						struct device_node *np)
{
	if (cma_heap_parse_dt(cma_heap, np))
		return -EINVAL;

	if (!cma_for_each_area(add_mm_cma_heap, cma_heap))
		return -EINVAL;

	cma_heap->heap.ops = &mm_cma_heap_ops;
	cma_heap->heap.name = cma_heap->heap_name;
	cma_heap->heap.priv = cma_heap;
	kref_init(&cma_heap->heap.refcount);
	cma_heap->alloc_size = 0;

	return 0;
}

static int cma_heap_create(struct device_node *np)
{
	struct cma_heap *cma_heap = NULL;
	int ret;

	cma_heap = kzalloc(sizeof(*cma_heap), GFP_KERNEL);
	if (!cma_heap)
		return -ENOMEM;

	ret = cma_heap_prop(cma_heap, np);
	if (ret)
		goto free_heap;

	ret = dma_heap_add(&cma_heap->heap);
	if (ret) {
		pr_err("%s: dma_heap_add[%s] failed!\n", __func__, cma_heap->heap_name);
		goto free_heap;
	}
	dma_coerce_mask_and_coherent(dma_heap_get_dev(&cma_heap->heap),
						DMA_BIT_MASK(64));

	pr_err("cma_heap[%s] info:\n"
		"\t\t\t\t\t\t\t heap size : %lu MB\n"
		"\t\t\t\t\t\t\t cma base : 0x%lx\n"
		"\t\t\t\t\t\t\t cma size : 0x%lx\n",
		cma_heap->heap_name,
		cma_heap->heap_size / SZ_1M,
		cma_get_base(cma_heap->cma),
		cma_get_size(cma_heap->cma));

	return 0;

free_heap:
	kfree(cma_heap);

	return -EINVAL;
}

static int cma_heap_init(void)
{
	int ret = 0;
	struct device_node *cma_heap_np = NULL, *child_np = NULL;

	if (!dma_heap_node) {
		pr_err("%s, can not find dam_heap node\n", __func__);
		return -EINVAL;
	}

	cma_heap_np = of_get_child_by_name(dma_heap_node, "cma_heap");
	if (!cma_heap_np) {
		pr_err("%s, can not find system_heap node\n", __func__);
		return -EINVAL;
	}

	for_each_available_child_of_node(cma_heap_np, child_np) {
		ret = cma_heap_create(child_np);
		if (ret)
			pr_err("%s, create[%s] fail\n", __func__, child_np->name);
	}

	return ret;
}

module_init(cma_heap_init);
MODULE_LICENSE("GPL v2");

