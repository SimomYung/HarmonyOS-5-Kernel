/*
 * drivers/dma-buf/heaps/mm_seccg_heap.c
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
#define pr_fmt(fmt) "[seccg_heap]" fmt

#include <linux/cma.h>
#include <linux/delay.h>
#include <linux/version.h>
#include <linux/dma-buf.h>
#include <linux/dma-mapping.h>
#include <linux/dma-heap.h>
#include <linux/err.h>
#include <linux/genalloc.h>
#include <linux/list.h>
#include <linux/memblock.h>
#include <linux/mm.h>
#include <linux/mutex.h>
#include <linux/of_address.h>
#include <linux/of_fdt.h>
#include <linux/of_reserved_mem.h>
#include <linux/platform_device.h>
#include <linux/scatterlist.h>
#include <linux/sched.h>
#include <linux/sizes.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <linux/dmabuf/mm_dma_heap.h>

#include <asm/cacheflush.h>
#include <asm/tlbflush.h>

#include "mm_sec_contig.h"
#include "mm_tee_op.h"
#include "mm_sec_alloc.h"
#include "mm_heap_helpers.h"
#include "mm_heap_priv.h"

static void seccg_heap_buf_free(struct mm_heap_helper_buffer *buffer)
{
	if (!buffer->heap->ops->free) {
		pr_err("%s: this heap does not define a method for free\n",
		       __func__);
		return;
	}

	mutex_lock(&buffer->lock);
	buffer->heap->ops->free(buffer);
	mutex_unlock(&buffer->lock);

	kfree(buffer);
}

static int seccg_heap_input_check(struct seccg_heap *seccg_heap,
				    unsigned long size, unsigned long flag)
{
	if (seccg_heap->alloc_size + size <= seccg_heap->alloc_size) {
		pr_err("%s, size overflow!"
			"heap alloc_size = 0x%lx, size = 0x%lx\n",
			__func__, seccg_heap->alloc_size, size);
		return -EINVAL;
	}

	if ((seccg_heap->alloc_size + size) > seccg_heap->heap_size) {
		pr_err("%s, size not enough!"
			"heap_alloc_size = 0x%lx, size = 0x%lx, heap_size = 0x%lx\n",
			__func__, seccg_heap->alloc_size, size, seccg_heap->heap_size);
		return -EINVAL;
	}

	if (size > seccg_heap->per_alloc_sz) {
		pr_err("%s, size too large!"
			"size 0x%lx, per_alloc_sz 0x%llx\n",
			__func__, size, seccg_heap->per_alloc_sz);
		return -EINVAL;
	}

	if ((seccg_heap->heap_attr != SEC_DRM_TEE) && (seccg_heap->heap_attr != SEC_AOD) &&
			dma_buf_is_secure(flag)) {
		pr_err("%s, heap[%s] flags[0x%lx] err, sec_flags was setted\n",
			__func__, seccg_heap->name, flag);
		return -EINVAL;
	}

	seccg_heap->flag = flag;

	return 0;
}

static int seccg_create_static_cma_region(struct seccg_heap *seccg_heap,
					    u64 cma_base, u64 cma_size)
{
	struct page *page = NULL;

	page = cma_alloc(seccg_heap->cma, cma_size >> PAGE_SHIFT, 0, GFP_KERNEL);
	if (!page) {
		pr_err("%s, cma_alloc failed! base 0x%llx size 0x%llx\n",
		       __func__, cma_base, cma_size);
		return -ENOMEM;
	}

	seccg_heap->static_cma_region =
		gen_pool_create((int)seccg_heap->pool_shift, -1);

	if (!seccg_heap->static_cma_region) {
		pr_err("%s, static_cma_region create failed\n", __func__);
		goto err_cma_release;
	}

	gen_pool_set_algo(seccg_heap->static_cma_region,
			  	gen_pool_best_fit, NULL);

	if (gen_pool_add(seccg_heap->static_cma_region,
			 page_to_phys(page), cma_size, -1)) {
		pr_err("%s, pool add failed, base[0x%llx], size[0x%llx]\n",
		       __func__, cma_base, cma_size);
		goto err_pool_destroy;
	}

	return 0;

err_pool_destroy:
	gen_pool_destroy(seccg_heap->static_cma_region);
	seccg_heap->static_cma_region = NULL;
err_cma_release:
	cma_release(seccg_heap->cma, page, (u32)cma_size >> PAGE_SHIFT);

	return -ENOMEM;
}

static int seccg_create_pool(struct seccg_heap *seccg_heap)
{
	u64 cma_base;
	u64 cma_size;
	int ret = 0;

	if (seccg_heap->pool_disabled) {
		pr_info("%s [%s] no need to create pool\n", __func__, seccg_heap->name);
		return 0;
	}

	/* Allocate on 4KB boundaries (1 << ION_PBL_SHIFT) */
	seccg_heap->pool = gen_pool_create((int)seccg_heap->pool_shift, -1);
	if (!seccg_heap->pool) {
		pr_err("%s, pool create failed\n", __func__);
		return -ENOMEM;
	}
	gen_pool_set_algo(seccg_heap->pool, gen_pool_best_fit, NULL);

	/* Add all memory to genpool first, one chunk only */
	cma_base = cma_get_base(seccg_heap->cma);
	cma_size = cma_get_size(seccg_heap->cma);
	if (gen_pool_add(seccg_heap->pool, cma_base, cma_size, -1)) {
		pr_err("%s, pool add failed, base[0x%llx], size[0x%llx]\n",
		       __func__, cma_base, cma_size);
		ret = -ENOMEM;
		goto err_pool_create;
	}

	if (!gen_pool_alloc(seccg_heap->pool, cma_size)) {
		pr_err("%s, pool alloc failed\n", __func__);
		ret = -ENOMEM;
		goto err_pool_create;
	}

	if (seccg_heap->cma_type == CMA_STATIC) {
		ret = seccg_create_static_cma_region(seccg_heap,
						       cma_base,
						       cma_size);
		if (ret)
			goto err_cma_alloc;
	}

	return 0;

err_cma_alloc:
	gen_pool_free(seccg_heap->pool, cma_base, cma_size);
err_pool_create:
	gen_pool_destroy(seccg_heap->pool);
	seccg_heap->pool = NULL;

	return ret;
}

static int seccg_heap_phys(struct dma_heap *heap, struct dma_buf *dmabuf,
			phys_addr_t *addr, size_t *len)
{
	struct mm_heap_helper_buffer *buffer = NULL;
	struct sg_table *table = NULL;
	struct page *page = NULL;
	phys_addr_t paddr = 0;

	if (IS_ERR_OR_NULL(heap) || IS_ERR_OR_NULL(dmabuf) ||
		IS_ERR_OR_NULL(addr) || IS_ERR_OR_NULL(len)) {
		pr_err("%s, invalid input para!\n", __func__);
		return -EINVAL;
	}

	buffer = dmabuf->priv;
	if (!buffer) {
		pr_err("%s, get heap_helper buffer fail!\n", __func__);
		return -EINVAL;
	}

	table = buffer->sg_table;
	page = sg_page(table->sgl);
	paddr = PFN_PHYS(page_to_pfn(page));

	*addr = paddr;
	*len = buffer->size;

	return 0;
}

static struct dma_buf *seccg_heap_allocate(struct dma_heap *heap,
					    unsigned long len,
					    unsigned long fd_flags,
					    unsigned long heap_flags)
{
	struct seccg_heap *seccg_heap =
		container_of(heap, struct seccg_heap, heap);
	struct mm_heap_helper_buffer *buffer = NULL;
	struct dma_buf *dmabuf = NULL;
	int ret = 0;

	buffer = kzalloc(sizeof(*buffer), GFP_KERNEL);
	if (!buffer)
		return ERR_PTR(-ENOMEM);

	init_mm_heap_helper_buffer(buffer, seccg_heap_buf_free);
	buffer->heap = heap;
	buffer->size = len;
	buffer->flags = heap_flags;
	buffer->magic = atomic64_inc_return(&dmaheap_magic);

	mutex_lock(&seccg_heap->mutex);
	if (seccg_heap_input_check(seccg_heap, len, heap_flags)) {
		pr_err("%s, input params failed\n", __func__);
		ret = -EINVAL;
		goto out;
	}

	/* init the TA conversion here */
	if (!seccg_heap->ta_init &&
	    ((seccg_heap->heap_attr == SEC_DRM_TEE) ||
	    (seccg_heap->heap_attr == SEC_AOD))) {
		ret = sec_tee_init(&seccg_heap->tee, DMA_HEAP_SESSIONS_SECMEM);
		if (ret) {
			pr_err("%s, TA init failed\n", __func__);
			goto out;
		}
		seccg_heap->ta_init = 1;
	}

	dmabuf = seccg_alloc_contig(seccg_heap, buffer, len, fd_flags);
	if (IS_ERR(dmabuf))
		goto out;

	mutex_unlock(&seccg_heap->mutex);

	if (dma_buf_is_unsecure(heap_flags))
		dma_heap_flush_buffer_cache(buffer, heap_flags);

	mm_dmaheap_init_dump(buffer);
	mm_dmaheap_buffer_add(buffer);

	return dmabuf;

out:
	mutex_unlock(&seccg_heap->mutex);
	kfree(buffer);

	pr_err("%s, [%s]alloc fail, size[0x%lx], heap all alloc_size[0x%lx], heap_size [0x%lx]\n",
		__func__, seccg_heap->name, len, seccg_heap->alloc_size, seccg_heap->heap_size);
	return ERR_PTR(-EINVAL);
}

static void seccg_heap_free(struct mm_heap_helper_buffer *buffer)
{
	struct seccg_heap *seccg_heap =
		container_of(buffer->heap, struct seccg_heap, heap);

	mutex_lock(&seccg_heap->mutex);
	seccg_free_contig(seccg_heap, buffer);
	mutex_unlock(&seccg_heap->mutex);
}

static int seccg_heap_map_user(struct dma_heap *heap,
		struct mm_heap_helper_buffer *buffer, struct vm_area_struct *vma)
{
	struct seccg_heap *seccg_heap =
		container_of(heap, struct seccg_heap, heap);

	if (seccg_heap->heap_attr == SEC_DRM_TEE || seccg_heap->heap_attr == SEC_AOD) {
		pr_err("%s, [%s]don't support map user\n",
					__func__, seccg_heap->name);
		return -EINVAL;
	}

	return mm_dma_heap_map_user(heap, buffer, vma);
}

static void *seccg_heap_map_kernel(struct dma_heap *heap,
				struct mm_heap_helper_buffer *buffer)
{
	struct seccg_heap *seccg_heap =
		container_of(heap, struct seccg_heap, heap);

	if (seccg_heap->heap_attr == SEC_DRM_TEE || seccg_heap->heap_attr == SEC_AOD) {
		pr_err("%s, [%s]don't support map kernel\n",
					__func__, seccg_heap->name);
		return NULL;
	}

	return mm_dma_heap_map_kernel(heap, buffer);
}

static void seccg_heap_unmap_kernel(struct dma_heap *heap,
				struct mm_heap_helper_buffer *buffer)
{
	struct seccg_heap *seccg_heap =
		container_of(heap, struct seccg_heap, heap);

	if (seccg_heap->heap_attr == SEC_DRM_TEE || seccg_heap->heap_attr == SEC_AOD) {
		pr_err("%s, [%s]don't support unmap kernel\n",
					__func__, seccg_heap->name);
		return;
	}

	mm_dma_heap_unmap_kernel(heap, buffer);
}

static struct dma_heap_ops seccg_heap_ops = {
	.allocate = seccg_heap_allocate,
	.free = seccg_heap_free,
	.map_user = seccg_heap_map_user,
	.map_kernel = seccg_heap_map_kernel,
	.unmap_kernel = seccg_heap_unmap_kernel,
	.get_heap_phys = seccg_heap_phys,
};

static int seccg_parse_dt(struct seccg_heap *seccg_heap,
								struct device_node *np)
{
	u32 heap_base = 0;
	u32 heap_size = 0;
	u64 per_bit_sz = 0;
	u64 per_alloc_sz = 0;
	u64 water_mark = 0;
	int ret = 0;

	ret = of_property_read_string(np, "heap-name", &seccg_heap->name);
	if (ret < 0) {
		pr_err("%s, can not find heap-name\n", __func__);
		goto out;
	}

	ret = of_property_read_u32(np, "heap-base", &heap_base);
	if (ret < 0) {
		pr_err("%s, [%s]can not find prop:heap-base\n",
					__func__, seccg_heap->name);
		goto out;
	}
	seccg_heap->heap_base = heap_base;

	ret = of_property_read_u32(np, "heap-size", &heap_size);
	if (ret < 0) {
		pr_err("%s, [%s]can not find prop:heap-size\n",
					__func__, seccg_heap->name);
		goto out;
	}
	seccg_heap->heap_size = heap_size;

	ret = of_property_read_u32(np, "cma-type", &seccg_heap->cma_type);
	if (ret < 0) {
		seccg_heap->cma_type = CMA_DYNAMIC;
		pr_err("%s, [%s]can not find prop:cma-type\n",
					__func__, seccg_heap->name);
	}

	ret = of_property_read_u64(np, "per-alloc-size", &per_alloc_sz);
	if (ret < 0 || per_alloc_sz == 0) {
		pr_err("%s, [%s]can not find prop:per-alloc-size\n",
					__func__, seccg_heap->name);
		goto out;
	}
	seccg_heap->per_alloc_sz = PAGE_ALIGN(per_alloc_sz);

	ret = of_property_read_u64(np, "per-bit-size", &per_bit_sz);
	if (ret < 0 || per_bit_sz == 0) {
		pr_err("%s, [%s]can not find prop:per-bit-size\n",
					__func__, seccg_heap->name);
		goto out;
	}
	seccg_heap->per_bit_sz = PAGE_ALIGN(per_bit_sz);

	ret = of_property_read_u32(np, "heap-attr", &seccg_heap->heap_attr);
	if (ret < 0) {
		pr_err("%s, [%s]can not find prop:heap-attr\n",
					__func__, seccg_heap->name);
		goto out;
	}

	ret = of_property_read_u64(np, "water-mark", &water_mark);
	if (ret < 0) {
		pr_err("%s, can not find[%s] prop:water-mark\n",
					__func__, seccg_heap->name);
		water_mark = 0;
	}
	seccg_heap->water_mark = PAGE_ALIGN(water_mark);

	ret = of_property_read_u32(np, "pool-shift", &seccg_heap->pool_shift);
	if (ret < 0) {
		pr_err("%s, [%s]can not find prop:pool-shift\n",
					__func__, seccg_heap->name);
		seccg_heap->pool_shift = DMABUF_PBL_SHIFT;
	}

	if (of_property_read_bool(np, "pool-disable"))
		seccg_heap->pool_disabled = true;
	else
		seccg_heap->pool_disabled = false;

out:
	return ret;
}

static int seccg_init_heap_properties(struct seccg_heap *seccg_heap, struct device_node *np)
{
	int ret;

	if (seccg_parse_dt(seccg_heap, np))
		return -EINVAL;

	seccg_heap->heap.name = seccg_heap->name;
	seccg_heap->heap.ops = &seccg_heap_ops;
	seccg_heap->heap.priv = NULL;
	kref_init(&seccg_heap->heap.refcount);
	seccg_heap->alloc_size = 0;
	seccg_heap->cma_alloc_size = 0;
	seccg_heap->pool = NULL;

	seccg_heap->cma = get_svc_cma((int)seccg_heap->heap_attr);
	if (!seccg_heap->cma) {
		pr_err("%s, [%s]can not get cma\n", __func__, seccg_heap->name);
		return -ENOMEM;
	}

	ret = seccg_create_pool(seccg_heap);
	if (ret) {
		pr_err("%s, [%s]pool create failed\n", __func__, seccg_heap->name);
		return -ENOMEM;
	}

	return 0;
}

static int seccg_heap_create(struct device_node *np)
{
	int ret;
	struct seccg_heap *seccg_heap = NULL;

	seccg_heap = kzalloc(sizeof(*seccg_heap), GFP_KERNEL);
	if (!seccg_heap)
		return -ENOMEM;

	mutex_init(&seccg_heap->mutex);
	INIT_LIST_HEAD(&seccg_heap->allocate_head);

	ret =  seccg_init_heap_properties(seccg_heap, np);
	if (ret)
		goto free_heap;

	seccg_heap->ta_init = 0;

	if (seccg_heap->water_mark &&
	    seccg_fill_watermark(seccg_heap))
		pr_err("%s, seccg_fill_watermark failed!\n", __func__);

	ret = dma_heap_add(&seccg_heap->heap);
	if (ret) {
		pr_err("%s, dma_heap_add[%s] failed!\n",
					__func__, seccg_heap->name);
		goto destroy_pool;
	}
	dma_coerce_mask_and_coherent(dma_heap_get_dev(&seccg_heap->heap),
						DMA_BIT_MASK(64));
	pr_err("seccg_heap [%s] info :\n"
		"\t\t\t\t\t\t\t heap attr : %u\n"
		"\t\t\t\t\t\t\t pool shift : %u\n"
		"\t\t\t\t\t\t\t pool disabled : %u\n"
		"\t\t\t\t\t\t\t heap size : %lu MB\n"
		"\t\t\t\t\t\t\t per alloc size :  %llu MB\n"
		"\t\t\t\t\t\t\t per bit size : %llu KB\n"
		"\t\t\t\t\t\t\t water_mark size : %llu MB\n"
		"\t\t\t\t\t\t\t heap cma-type : %u\n"
		"\t\t\t\t\t\t\t heap cma-name : %s\n"
		"\t\t\t\t\t\t\t cma-base : 0x%llx\n"
		"\t\t\t\t\t\t\t cma-size : 0x%lx\n",
		seccg_heap->name,
		seccg_heap->heap_attr,
		seccg_heap->pool_shift,
		seccg_heap->pool_disabled,
		seccg_heap->heap_size / SZ_1M,
		seccg_heap->per_alloc_sz / SZ_1M,
		seccg_heap->per_bit_sz / SZ_1K,
		seccg_heap->water_mark / SZ_1M,
		seccg_heap->cma_type,
		cma_get_name(seccg_heap->cma),
		cma_get_base(seccg_heap->cma),
		cma_get_size(seccg_heap->cma));

	return 0;

destroy_pool:
	if (seccg_heap->cma_type == CMA_STATIC) {
		gen_pool_destroy(seccg_heap->static_cma_region);
		seccg_heap->static_cma_region = NULL;
	}
	if (!seccg_heap->pool_disabled) {
		gen_pool_free(seccg_heap->pool, cma_get_base(seccg_heap->cma),
					cma_get_size(seccg_heap->cma));
		gen_pool_destroy(seccg_heap->pool);
	}
free_heap:
	kfree(seccg_heap);

	return -EINVAL;
}

static int seccg_heap_init(void)
{
	int ret = 0;
	struct device_node *cg_np = NULL, *child_np = NULL;

	if (!dma_heap_node) {
		pr_err("%s, can not find dam_heap node\n", __func__);
		return -1;
	}

	cg_np = of_get_child_by_name(dma_heap_node, "seccg_heap");
	if (!cg_np) {
		pr_err("%s, can not find seccg_heap node\n", __func__);
		return -1;
	}

	for_each_available_child_of_node(cg_np, child_np) {
		ret = seccg_heap_create(child_np);
		if (ret)
			pr_err("%s, create[%s] fail\n", __func__, child_np->name);
	}

	return 0;
}

module_init(seccg_heap_init);
MODULE_LICENSE("GPL v2");
