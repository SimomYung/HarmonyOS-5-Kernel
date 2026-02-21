/*
 * drivers/dma-buf/heaps/mm_dma_pool_heap.c
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
#define pr_fmt(fmt) "[dma_pool_heap]" fmt

#include <linux/atomic.h>
#include <linux/cma.h>
#include <linux/dma-buf.h>
#include <linux/dma-mapping.h>
#include <linux/dma-heap.h>
#include <linux/err.h>
#include <linux/genalloc.h>
#include <linux/highmem.h>
#include <linux/io.h>
#include <linux/mm.h>
#include <linux/of.h>
#include <linux/of_fdt.h>
#include <linux/of_reserved_mem.h>
#include <linux/platform_device.h>
#include <linux/scatterlist.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/time.h>
#include <linux/vmalloc.h>
#include <linux/version.h>
#include <linux/workqueue.h>
#include <linux/dmabuf/mm_dma_heap.h>

#include <asm/cacheflush.h>
#include <asm/tlbflush.h>

#include "mm_heap_helpers.h"
#include "deferred-free-helper.h"
#include "mm_heap_priv.h"

/*
 * Why pre-allocation size is 64MB?
 * 1.The time of 64MB memory cma releasing is short
 * 2.64MB memory is larger then iris's memory size
 */

/* align must be modify with count */
#define PREALLOC_ALIGN			14U
#define PREALLOC_CNT			(64UL * SZ_1M / PAGE_SIZE)
#define PREALLOC_NWK			(PREALLOC_CNT * 4U)

#define DMA_CAMERA_WATER_MARK	(512 * SZ_1M)
#define POOL_SZ_2M	(21U)

static void pre_alloc_wk_func(struct work_struct *work);

struct dma_pool_heap {
	struct dma_heap heap;
	const char *name;
	struct gen_pool *pool;
	phys_addr_t base;
	size_t size;
	atomic64_t alloc_size;
	atomic64_t prealloc_cnt;
};

/* static_dma_pool */
static struct cma *mm_camera_pool;
/* dynamic_dma_pool */
static struct cma *dma_camera_cma;
static struct dma_pool_heap *dma_camera_heap;
static DECLARE_WORK(dmaheap_buffer_pre_alloc_wk, pre_alloc_wk_func);

static int mm_camera_pool_set_up(struct reserved_mem *rmem)
{
	phys_addr_t align = PAGE_SIZE << max(MAX_ORDER - 1, pageblock_order);
	phys_addr_t mask = align - 1;
	unsigned long node = rmem->fdt_node;
	struct cma *cma = NULL;
	int err;

	if (!of_get_flat_dt_prop(node, "reusable", NULL) ||
	    of_get_flat_dt_prop(node, "no-map", NULL))
		return -EINVAL;
	if ((rmem->base & mask) || (rmem->size & mask)) {
		pr_err("Reserved memory: incorrect alignment of CMA region\n");
		return -EINVAL;
	}

	err = cma_init_reserved_mem(rmem->base, rmem->size, 0, rmem->name, &cma);
	if (err) {
		pr_err("Reserved memory: unable to setup CMA region\n");
		return err;
	}
	mm_camera_pool = cma;

	return 0;
}
RESERVEDMEM_OF_DECLARE(mm_camera_pool, "mm-camera-pool", mm_camera_pool_set_up);

void dma_pool_heap_register_dma_camera_cma(void *p)
{
	struct cma *cma = (struct cma *)p;

	if (cma && cma_get_size(cma) >= DMA_CAMERA_WATER_MARK) {
		dma_camera_cma = cma;
		pr_info("register_dma_camera_cma is ok\n");
	} else {
		pr_err("dma_camera_cma is not regested\n");
	}
}

static void clean_dma_camera_cma(void)
{
	phys_addr_t addr;
	unsigned long size_remain = 0;

	if (!dma_camera_heap)
		return;

	if (!atomic64_sub_and_test(0L, &dma_camera_heap->alloc_size))
		return;

	while (!!(addr = gen_pool_alloc(dma_camera_heap->pool, PAGE_SIZE))) {
		if (dma_camera_cma) {
			if (cma_release(dma_camera_cma,  phys_to_page(addr), 1))
				atomic64_sub(1, &dma_camera_heap->prealloc_cnt);
			else
				pr_err("cma release failed\n");
		}

		/* here is mean the camera is start again */
		if (!atomic64_sub_and_test(0L, &dma_camera_heap->alloc_size)) {
			pr_err("@free memory camera is start again, alloc sz %lx\n",
					atomic64_read(&dma_camera_heap->alloc_size));
			break;
		}
	}

	size_remain = gen_pool_avail(dma_camera_heap->pool);
	if (!!size_remain)
		pr_err("out %s, size_remain = 0x%lx\n", __func__, size_remain);

	pr_info("quit %s, prealloc_cnt now: %ld\n",
		__func__, atomic64_read(&dma_camera_heap->prealloc_cnt));
}

static void pre_alloc_wk_func(struct work_struct *work)
{
	struct page *page = NULL;
	phys_addr_t addr;

	if (!dma_camera_heap)
		return;

	if ((unsigned long)atomic64_read(&dma_camera_heap->prealloc_cnt) > PREALLOC_NWK)
		return;
	page = cma_alloc(dma_camera_cma, PREALLOC_CNT, PREALLOC_ALIGN, GFP_KERNEL);
	if (page) {
		addr = page_to_phys(page);
		memset(phys_to_virt(addr), 0x0, PREALLOC_CNT * PAGE_SIZE);
		atomic64_add(PREALLOC_CNT, &dma_camera_heap->prealloc_cnt);
		gen_pool_free(dma_camera_heap->pool,
			      page_to_phys(page),
			      PREALLOC_CNT * PAGE_SIZE);

		pr_info("enter %s, prealloc_cnt now: %ld\n",
			__func__, atomic64_read(&dma_camera_heap->prealloc_cnt));
	}
}

static void __dma_pool_heap_buf_free(struct deferred_freelist_item *item,
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

static void dma_pool_heap_buf_free(struct mm_heap_helper_buffer *buffer)
{
	int npages = PAGE_ALIGN(buffer->size) / PAGE_SIZE;

	deferred_free(&buffer->deferred_free, __dma_pool_heap_buf_free, npages);
}

static phys_addr_t dma_pool_allocate(struct dma_heap *dma_heap,
					unsigned long size,
					unsigned long align,
					unsigned long heap_flags)
{
	unsigned long offset = 0;
	struct dma_pool_heap *dma_pool_heap =
		container_of(dma_heap, struct dma_pool_heap, heap);

	if (!dma_pool_heap)
		return (phys_addr_t)-1L;

	offset = gen_pool_alloc(dma_pool_heap->pool, size);
	if (!offset) {
		if ((!strcmp(dma_heap->name, "camera_daemon_heap"))
			/*
			 * When the camera can only use 7/16 of all CMA size,
			 * the watermark its looks ok.
			 * v    wm    used   maxchun
			 * NA   NA    400M
			 * 1/2  304M  360M   160M
			 * 1/3  200M  260M   300M
			 * 3/8  228M  300M   240M
			 * 7/16 266M  340M   256M
			 */
			&& (atomic64_read(&dma_pool_heap->alloc_size) < dma_pool_heap->size / 16 * 7)) /*lint !e574*/
			schedule_work(&dmaheap_buffer_pre_alloc_wk);
		return (phys_addr_t)-1L;
	}
	atomic64_add(size, &dma_pool_heap->alloc_size);

	return offset;
}

static void dma_pool_free(struct dma_heap *heap, phys_addr_t addr,
		       unsigned long size)
{
	struct dma_pool_heap *dma_pool_heap =
		container_of(heap, struct dma_pool_heap, heap);

	if (addr == (phys_addr_t)-1L)
		return;

	gen_pool_free(dma_pool_heap->pool, addr, size);
	atomic64_sub(size, &dma_pool_heap->alloc_size);

	if (atomic64_sub_and_test(0L, &dma_pool_heap->alloc_size))
		clean_dma_camera_cma();
}

static struct dma_buf *dma_pool_heap_allocate(struct dma_heap *heap,
		unsigned long len, unsigned long fd_flags, unsigned long heap_flags)
{
	struct dma_pool_heap *dma_pool_heap =
		container_of(heap, struct dma_pool_heap, heap);
	struct mm_heap_helper_buffer *buffer;
	struct sg_table *table = NULL;
	struct scatterlist *sg = NULL;
	struct page *page = NULL;
	struct dma_buf *dmabuf = NULL;
	phys_addr_t paddr;
	int i, ret;

	if (dma_buf_is_secure(heap_flags)) {
		pr_err("%s: flag[0x%lx] err, sec_flags was set\n", __func__, heap_flags);
		return ERR_PTR(-EINVAL);
	}

	buffer = kzalloc(sizeof(*buffer), GFP_KERNEL);
	if (!buffer)
		return ERR_PTR(-ENOMEM);

	init_mm_heap_helper_buffer(buffer, dma_pool_heap_buf_free);
	buffer->heap = heap;
	buffer->size = len;
	buffer->flags = heap_flags;
	buffer->magic = atomic64_inc_return(&dmaheap_magic);

	table = kzalloc(sizeof(struct sg_table), GFP_KERNEL);
	if (!table)
		goto free_buffer;

	ret = sg_alloc_table(table, 1, GFP_KERNEL);
	if (ret)
		goto free_table;

	paddr = dma_pool_allocate(heap, len, 0, heap_flags);
	if (paddr == (phys_addr_t)-1L)
		goto free_sglist;

	page = pfn_to_page(PFN_DOWN(paddr));
	sg_set_page(table->sgl, page, (unsigned int)len, (unsigned int)0);
	sg_dma_address(table->sgl) = sg_phys(table->sgl);
	sg_dma_len(table->sgl) = table->sgl->length;
	buffer->sg_table = table;

	if (!strncmp(heap->name, "camera_daemon_heap", MM_DMA_HEAP_NAME_LEN))
		(void)mm_dma_heap_pages_zero(page, len,
				pgprot_writecombine(PAGE_KERNEL));

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
	for_each_sg(table->sgl, sg, table->nents, i) {
		page = sg_page(sg);
		paddr = PFN_PHYS(page_to_pfn(page));
		dma_pool_free(heap, paddr, sg->length);
	}
free_sglist:
	sg_free_table(table);
free_table:
	kfree(table);
free_buffer:
	kfree(buffer);

	pr_err("%s: [%s]alloc fail, size[0x%lx], heap all alloc_size[0x%lx], heap_size [0x%lx]\n",
		    	__func__, dma_pool_heap->name, len,
		    	atomic64_read(&dma_pool_heap->alloc_size), dma_pool_heap->size);
	return ERR_PTR(-ENOMEM);
}

static void dma_pool_heap_free(struct mm_heap_helper_buffer *buffer)
{
	struct dma_heap *heap = buffer->heap;
	struct sg_table *table = buffer->sg_table;
	struct page *page = NULL;
	struct scatterlist *sg = NULL;
	phys_addr_t paddr = 0;
	u32 i;

	if (dma_buf_is_cached(buffer->flags))
		dma_sync_sg_for_device(heap->heap_dev, table->sgl,
				(int)table->nents, DMA_BIDIRECTIONAL);

	for_each_sg(table->sgl, sg, table->nents, i) {
		page = sg_page(sg);
		paddr = PFN_PHYS(page_to_pfn(page));
		dma_pool_free(heap, paddr, sg->length);
	}
	sg_free_table(table);
	kfree(table);
}

static int dma_pool_heap_map_user(struct dma_heap *heap,
		struct mm_heap_helper_buffer *buffer, struct vm_area_struct *vma)
{
	if (dma_buf_is_secure(buffer->flags))
		return -EINVAL;

	return mm_dma_heap_map_user(heap, buffer, vma);
}

static void *dma_pool_heap_map_kernel(struct dma_heap *heap,
		struct mm_heap_helper_buffer *buffer)
{
	if (dma_buf_is_secure(buffer->flags))
		return ERR_PTR(-EINVAL);

	return mm_dma_heap_map_kernel(heap, buffer);
}

static void dma_pool_heap_unmap_kernel(struct dma_heap *heap,
		struct mm_heap_helper_buffer *buffer)
{
	if (dma_buf_is_secure(buffer->flags))
		return;

	mm_dma_heap_unmap_kernel(heap, buffer);
}

static struct dma_heap_ops dma_pool_heap_ops = {
	.allocate = dma_pool_heap_allocate,
	.free = dma_pool_heap_free,
	.map_user = dma_pool_heap_map_user,
	.map_kernel = dma_pool_heap_map_kernel,
	.unmap_kernel = dma_pool_heap_unmap_kernel,
};

static int dynamic_dma_pool_heap_create(struct dma_pool_heap *dma_pool_heap)
{
	dma_pool_heap->heap.name = dma_pool_heap->name;
	dma_pool_heap->heap.ops = &dma_pool_heap_ops;
	dma_pool_heap->heap.priv = NULL;
	kref_init(&dma_pool_heap->heap.refcount);

	if (!dma_camera_cma)
		return -EINVAL;

	dma_pool_heap->base = cma_get_base(dma_camera_cma);
	dma_pool_heap->size = cma_get_size(dma_camera_cma);
	dma_pool_heap->pool = gen_pool_create(PAGE_SHIFT, -1);
	if (!dma_pool_heap->pool)
		return -EINVAL;
	if (gen_pool_add(dma_pool_heap->pool,
			 dma_pool_heap->base,
			 dma_pool_heap->size,
			 -1))
		goto pool_create_err;
	if (!gen_pool_alloc(dma_pool_heap->pool, dma_pool_heap->size))
		goto pool_create_err;

	atomic64_set(&dma_pool_heap->alloc_size, 0);
	atomic64_set(&dma_pool_heap->prealloc_cnt, 0);

	dma_camera_heap = dma_pool_heap;

	return 0;

pool_create_err:
	gen_pool_destroy(dma_pool_heap->pool);

	return -EINVAL;
}

static int static_dma_pool_heap_create(struct dma_pool_heap *dma_pool_heap)
{
	struct page *page = NULL;
	int ret;
	size_t size;

	if (!mm_camera_pool) {
		pr_err("%s, mm_camera_pool not found!\n", __func__);
		return -ENOMEM;
	}

	size = cma_get_size(mm_camera_pool);
	page = cma_alloc(mm_camera_pool, size >> PAGE_SHIFT, 0, GFP_KERNEL);
	if (!page) {
		pr_err("%s, mm_camera_pool cma_alloc out of memory!\n",
				__func__);
		return -ENOMEM;
	}

	ret = mm_dma_heap_pages_zero(page, size,
				pgprot_writecombine(PAGE_KERNEL));
	if (ret) {
		pr_err("%s, mm_camera_pool zero fail, error: 0x%x\n",
				__func__, ret);
		goto err;
	}

	dma_pool_heap->pool = gen_pool_create(PAGE_SHIFT, -1);
	if (!dma_pool_heap->pool) {
		pr_err("%s, mm_camera_pool gen_pool_create fail!\n", __func__);
		goto err;
	}

	dma_pool_heap->base = page_to_phys(page);
	dma_pool_heap->size = size;
	gen_pool_add(dma_pool_heap->pool, dma_pool_heap->base,
			dma_pool_heap->size, -1);
	dma_pool_heap->heap.name = dma_pool_heap->name;
	dma_pool_heap->heap.ops = &dma_pool_heap_ops;
	dma_pool_heap->heap.priv = NULL;
	kref_init(&dma_pool_heap->heap.refcount);
	atomic64_set(&dma_pool_heap->alloc_size, 0);
	atomic64_set(&dma_pool_heap->prealloc_cnt, 0);

	return 0;

err:
	cma_release(mm_camera_pool, page, (u32)size >> PAGE_SHIFT);

	return -EINVAL;
}

static int dma_pool_parse_dt(struct dma_pool_heap *dma_pool_heap,
						struct device_node *np)
{
	int ret;

	ret = of_property_read_string(np, "heap-name", &dma_pool_heap->name);
	if (ret < 0)
		pr_err("%s, can not find heap-name\n", __func__);

	return ret;
}

static int dma_pool_heap_create(struct device_node *np)
{
	struct dma_pool_heap *dma_pool_heap = NULL;
	int ret;

	dma_pool_heap = kzalloc(sizeof(struct dma_pool_heap), GFP_KERNEL);
	if (!dma_pool_heap)
		return -ENOMEM;

	ret = dma_pool_parse_dt(dma_pool_heap, np);
	if (ret) {
		pr_err("%s, dma_pool_heap parse fail!\n", __func__);
		goto free_heap;
	}

	if (!strcmp(dma_pool_heap->name, "camera_heap")) {
		ret = static_dma_pool_heap_create(dma_pool_heap);
		if (ret) {
			pr_err("%s, static_dma_pool_heap create fail!\n",
						__func__);
			goto free_heap;
		}
	} else {
		ret = dynamic_dma_pool_heap_create(dma_pool_heap);
		if (ret) {
			pr_err("%s, dynamic_dma_pool_heap create fail!\n",
						__func__);
			goto free_heap;
		}
	}

	ret = dma_heap_add(&dma_pool_heap->heap);
	if (ret) {
		pr_err("%s: dma_heap_add [%s] failed!\n",
						__func__, dma_pool_heap->name);
		goto free_heap;
	}
	dma_coerce_mask_and_coherent(dma_heap_get_dev(&dma_pool_heap->heap),
						DMA_BIT_MASK(64));

	return 0;

free_heap:
	kfree(dma_pool_heap);

	return -EINVAL;
}

static int dma_pool_heap_init(void)
{
	struct device_node *dma_pool_np = NULL, *child_np = NULL;
	int ret = 0;

	if (!dma_heap_node) {
		pr_err("%s, can not find dma_heap node\n", __func__);
		return -1;
	}

	dma_pool_np = of_get_child_by_name(dma_heap_node, "dma_pool_heap");
	if (!dma_pool_np) {
		pr_err("%s, can not find dma_pool_heap node\n", __func__);
		return -1;
	}

	for_each_available_child_of_node(dma_pool_np, child_np) {
		ret = dma_pool_heap_create(child_np);
		if (ret)
			pr_err("%s, create[%s] fail\n", __func__, child_np->name);
	}

	return 0;
}

module_init(dma_pool_heap_init);
MODULE_LICENSE("GPL v2");
