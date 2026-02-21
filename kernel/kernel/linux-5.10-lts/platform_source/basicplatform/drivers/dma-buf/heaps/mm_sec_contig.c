/*
 * drivers/dma-buf/heaps/mm_sec_contig.c
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
#include <linux/dma-mapping.h>
#include <linux/err.h>
#include <linux/highmem.h>
#include <linux/list.h>
#include <linux/mm.h>
#include <linux/mutex.h>
#include <linux/platform_device.h>
#include <linux/scatterlist.h>
#include <linux/sizes.h>
#include <linux/slab.h>
#include <linux/dmabuf/mm_dma_heap.h>

#include <asm/cacheflush.h>
#include <asm/tlbflush.h>

#include "mm_sec_contig.h"
#include "mm_tee_op.h"
#include "mm_sec_alloc.h"
#include "mm_heap_helpers.h"
#include "mm_heap_priv.h"

static inline void free_alloc_list(struct list_head *head)
{
	struct alloc_list *pos = NULL;

	while (!list_empty(head)) {
		pos = list_first_entry(head, struct alloc_list, list);
		if (pos->addr || pos->size)
			pr_err("in %s %pK %x failed\n", __func__,
			       (void *)(uintptr_t)pos->addr, pos->size);

		list_del(&pos->list);
		kfree(pos);
	}
}

static u32 count_list_nr(struct seccg_heap *seccg_heap)
{
	u32 nr = 0;
	struct list_head *head = &seccg_heap->allocate_head;
	struct list_head *pos = NULL;

	list_for_each(pos, head)
		nr++;

	return nr;
}

static struct page *__seccg_cma_alloc(struct seccg_heap *seccg_heap,
				      size_t count, u64 size,
				      unsigned int align)
{
	unsigned long offset = 0;

	if (!seccg_heap->static_cma_region)
		return cma_alloc(seccg_heap->cma, count, align, GFP_KERNEL);

	offset = gen_pool_alloc(seccg_heap->static_cma_region,
				size);
	if (!offset) {
		pr_err("__cma_alloc failed!size:0x%llx\n", size);
		return NULL;
	}

	return pfn_to_page(PFN_DOWN(offset));
}

static bool __seccg_cma_release(struct seccg_heap *seccg_heap,
				const struct page *pages,
				unsigned int count,
				u64 size)
{
	if (!seccg_heap->static_cma_region)
		return cma_release(seccg_heap->cma, pages, count);

	gen_pool_free(seccg_heap->static_cma_region, page_to_phys(pages),
		      size);

	return 0;
}

static int cons_phys_struct(struct seccg_heap *seccg_heap, u32 nents,
			    struct list_head *head, u32 cmd_to_ta)
{
	struct tz_pageinfo *pageinfo = NULL;
	struct mem_chunk_list mcl;
	struct list_head *pos = head->prev;
	struct alloc_list *tmp_list = NULL;
	unsigned long size = nents * sizeof(*pageinfo);
	u32 i;
	int ret = 0;

	if (!seccg_heap->ta_init) {
		pr_err("[%s] TA not inited.\n", __func__);
		return -EINVAL;
	}

	pageinfo = kzalloc(size, GFP_KERNEL);
	if (!pageinfo)
		return -ENOMEM;

	for (i = 0; (i < nents) && (pos != head); i++) {
		tmp_list = list_entry(pos, struct alloc_list, list);
		pageinfo[i].addr = tmp_list->addr;
		pageinfo[i].nr_pages = tmp_list->size / PAGE_SIZE;
		pos = pos->prev;
	}

	if (i < nents) {
		pr_err("[%s], invalid nents %d or head!\n", __func__, nents);
		ret = -EINVAL;
		goto out;
	}

	/* Call Tzdriver here */
	mcl.nents = nents;
	mcl.phys_addr = (void *)pageinfo;
	mcl.protect_id = SEC_TASK_SEC;
	mcl.size = nents * sizeof(struct tz_pageinfo);
	ret = secmem_tee_exec_cmd(seccg_heap->tee.session, &mcl, cmd_to_ta);
out:
	kfree(pageinfo);

	return ret;
}

static int alloc_range_update(struct page *pg, u64 size,
			struct seccg_heap *seccg_heap, unsigned int count)
{
	int ret = 0;
	struct alloc_list *alloc = NULL;
#ifdef CONFIG_NEED_CHANGE_MAPPING
	unsigned long virt;
#endif

	alloc = kzalloc(sizeof(*alloc), GFP_KERNEL);
	if (!alloc) {
		ret = -ENOMEM;
		goto err_out1;
	}
	alloc->addr = page_to_phys(pg);
	alloc->size = (unsigned int)size;
	list_add_tail(&alloc->list, &seccg_heap->allocate_head);

	if (seccg_heap->heap_attr == SEC_DRM_TEE) {
#ifdef CONFIG_NEED_CHANGE_MAPPING
		virt = (uintptr_t)__va(alloc->addr);
		change_secpage_range(alloc->addr, virt, size,
				     __pgprot(PROT_DEVICE_nGnRE));
		flush_tlb_all();
#endif
#ifndef CONFIG_MM_LB_FULL_COHERENCY
#ifdef CONFIG_MM_DMA_HEAP_FLUSH_CACHE_ALL
		dma_heap_flush_all_cpus_caches_raw();
#else
		dma_heap_flush_all_cpus_caches();
#endif
#else
		dma_ci_pages(pg, size);
#endif
		if (cons_phys_struct(seccg_heap, 1,
				     &seccg_heap->allocate_head,
				     DMA_HEAP_SEC_CMD_TABLE_SET)) {
			pr_err("cons_phys_struct failed\n");
			ret = -EINVAL;
			goto err_out2;
		}
	}

	seccg_heap->cma_alloc_size +=  size;
	gen_pool_free(seccg_heap->pool, page_to_phys(pg), size);
	pr_debug("out %s %llu MB memory(ret = %d)\n",
		    __func__, size / SZ_1M, ret);

	return 0;

err_out2:
#ifdef CONFIG_NEED_CHANGE_MAPPING
	change_secpage_range(alloc->addr, virt, size, PAGE_KERNEL);
	flush_tlb_all();
#endif
	list_del(&alloc->list);
	kfree(alloc);
err_out1:
	__seccg_cma_release(seccg_heap, pg, count, size);

	return ret;
}

static int add_cma_to_pool(struct seccg_heap *seccg_heap,
			   unsigned long user_alloc_size)
{
	int ret = 0;
	unsigned long size_remain;
	unsigned long allocated_size;
	unsigned long cma_remain;
	u64 size = seccg_heap->per_alloc_sz;
	u64 per_bit_sz = seccg_heap->per_bit_sz;
	u64 cma_size;
	/* Add for TEEOS ++, per_alloc_size = 64M */
	struct page *pg = NULL;
	unsigned int count = 0;
	/* Add for TEEOS -- */

	pr_debug("into %s\n", __func__);
	/* add 64M for every times
	 * per_alloc_sz = 64M, per_bit_sz = 16M(the original min_size)
	 */
	allocated_size = seccg_heap->alloc_size;
	size_remain = gen_pool_avail(seccg_heap->pool);
	cma_size = cma_get_size(seccg_heap->cma);
	cma_remain = cma_size - (allocated_size + size_remain);
	if (seccg_heap->heap_size <= (allocated_size + size_remain)) {
		pr_err("heap full! allocated_size(0x%lx), remain_size(0x%lx),"
			" heap_size(0x%lx), cma_remain(0x%lx)\n",
			allocated_size, size_remain,
			seccg_heap->heap_size, cma_remain);
		return -ENOMEM;
	}

	/* we allocate as much contiguous memory as we can. */
	count = (unsigned int)size >> PAGE_SHIFT;

	pg = __seccg_cma_alloc(seccg_heap, (size_t)count, size,
			       (u32)get_order((u32)per_bit_sz));
	if (!pg) {
		size = ALIGN(user_alloc_size, per_bit_sz);
		count = (unsigned int)size >> PAGE_SHIFT;
		pg = __seccg_cma_alloc(seccg_heap, (size_t)count, size,
				       (u32)get_order((u32)per_bit_sz));
		if (!pg) {
			pr_err("out of memory,cma: 0x%llx, type: %u, usesize: 0x%lx\n",
			       cma_size, seccg_heap->cma_type, allocated_size);
			return -ENOMEM;
		}
	}

	ret = alloc_range_update(pg, size, seccg_heap, count);
	if (ret)
		return ret;

	return 0;
}

static bool gen_pool_bulk_free(struct gen_pool *pool, u32 size)
{
	u32 i;
	unsigned long offset = 0;

	for (i = 0; i < (size / PAGE_SIZE); i++) {
		offset = gen_pool_alloc(pool, PAGE_SIZE);
		if (!offset) {
			pr_err("%s, gen_pool_alloc failed!\n", __func__);
			return false;
		}
	}

	return true;
}

static int __seccg_pool_clear_sec_attr(struct seccg_heap *seccg_heap)
{
	u32 nents;
#ifdef CONFIG_NEED_CHANGE_MAPPING
	u64 addr;
	u32 size;
	unsigned long virt;
	struct alloc_list *pos = NULL;
#endif

	if (seccg_heap->heap_attr == SEC_DRM_TEE) {
		nents = count_list_nr(seccg_heap);
		if (nents &&
		    cons_phys_struct(seccg_heap, nents,
				     &seccg_heap->allocate_head,
				     DMA_HEAP_SEC_CMD_TABLE_CLEAN)) {
			pr_err("heap_attr: [%u] unconfig failed\n",
			       seccg_heap->heap_attr);
			return -EINVAL;
		}
	}

#ifdef CONFIG_NEED_CHANGE_MAPPING
	if (dma_buf_is_secure(seccg_heap->flag)) {
		list_for_each_entry(pos, &seccg_heap->allocate_head, list) {
			addr = pos->addr;
			size = pos->size;
			virt = (unsigned long)__va(addr);
			change_secpage_range(addr, virt, size,
					     PAGE_KERNEL);
			flush_tlb_all();
		}
	}
	flush_tlb_all();
#endif

	return 0;
}

static void __seccg_pool_release(struct seccg_heap *seccg_heap)
{
	u64 addr;
	u32 size;
	unsigned long size_remain;
	unsigned long offset;
	struct alloc_list *pos = NULL;

	if (list_empty(&seccg_heap->allocate_head)) {
		pr_err("%s : list empty\n", __func__);
		return;
	}

	list_for_each_entry(pos, &seccg_heap->allocate_head, list) {
		addr = pos->addr;
		size = pos->size;
		offset = gen_pool_alloc(seccg_heap->pool, size);
		if (!offset) {
			pr_err("%s:%d:gen_pool_alloc failed! %llx %x\n",
			       __func__, __LINE__, addr, size);
			if (unlikely(!gen_pool_bulk_free(seccg_heap->pool,
							 size))) {
				pr_err("%s:bulk_free failed! %llx %x\n",
				       __func__, addr, size);
				continue;
			}
		}
		__seccg_cma_release(seccg_heap, phys_to_page(addr),
				    size >> PAGE_SHIFT, size);
		pos->addr = 0;
		pos->size = 0;
	}
	free_alloc_list(&seccg_heap->allocate_head);

	size_remain = gen_pool_avail(seccg_heap->pool);
	if (size_remain)
		pr_err("out %s, size_remain = 0x%lx(0x%lx)\n",
		       __func__, size_remain, offset);
}

static int __cons_phys_set_sec(struct seccg_heap *seccg_heap, u32 nents,
	u32 cmd_to_ta, struct page *page, unsigned int pages_size)
{
	struct tz_pageinfo *pageinfo = NULL;
	struct mem_chunk_list mcl = {0};
	unsigned long size = nents * sizeof(*pageinfo);
	u32 i;
	int ret = 0;

	if (!seccg_heap->ta_init) {
		pr_err("[%s] TA not inited.\n", __func__);
		return -EINVAL;
	}

	pageinfo = kzalloc(size, GFP_KERNEL);
	if (!pageinfo)
		return -ENOMEM;

	for (i = 0; i < nents; i++) {
		pageinfo[i].addr = page_to_phys(page);
		pageinfo[i].nr_pages = pages_size / PAGE_SIZE;
	}

	/* Call Tzdriver here */
	if (seccg_heap->heap_attr == SEC_AOD)
		mcl.protect_id = SEC_TASK_AOD;
	else
		mcl.protect_id = SEC_TASK_SEC;
	mcl.nents = nents;
	mcl.phys_addr = (void *)pageinfo;
	mcl.size = nents * sizeof(struct tz_pageinfo);
	ret = secmem_tee_exec_cmd(seccg_heap->tee.session, &mcl, cmd_to_ta);
out:
	kfree(pageinfo);

	return ret;
}


static bool __set_sec_mem_attribute(struct page *pg, u64 size,
			struct seccg_heap *seccg_heap, unsigned int count)
{
#ifdef CONFIG_NEED_CHANGE_MAPPING
	unsigned long virt = (uintptr_t)__va(page_to_phys(pg));

	change_secpage_range(page_to_phys(pg), virt, size,
		__pgprot(PROT_DEVICE_nGnRE));
	flush_tlb_all();
#endif

#ifndef CONFIG_MM_LB_FULL_COHERENCY
#ifdef CONFIG_MM_DMA_HEAP_FLUSH_CACHE_ALL
	dma_heap_flush_all_cpus_caches_raw();
#else
	dma_heap_flush_all_cpus_caches();
#endif
#else
	dma_ci_pages(pg, size);
#endif

	if (__cons_phys_set_sec(seccg_heap, 1, DMA_HEAP_SEC_CMD_TABLE_SET, pg, size)) {
		pr_err("cons_phys_struct failed\n");
#ifdef CONFIG_NEED_CHANGE_MAPPING
		change_secpage_range(page_to_phys(pg), virt, size, PAGE_KERNEL);
		flush_tlb_all();
#endif
		return false;
	}
	return true;
}

static struct page *__alloc_mem_direct_from_cma(
	struct seccg_heap *seccg_heap, unsigned long user_alloc_size)
{
	int ret = 0;
	unsigned long allocated_size = seccg_heap->cma_alloc_size;
	u64 per_bit_sz = seccg_heap->per_bit_sz;
	u64 size = ALIGN(user_alloc_size, per_bit_sz);
	u64 cma_size = cma_get_size(seccg_heap->cma);
	unsigned long cma_remain = cma_size - allocated_size;
	struct page *pg = NULL;
	unsigned int count = (unsigned int)size >> PAGE_SHIFT;

	if ((seccg_heap->heap_size <= allocated_size) ||
		(seccg_heap->heap_size < allocated_size + size) ||
		(size > seccg_heap->per_alloc_sz)) {
		pr_err("heap full! allocated_size(0x%lx),"
			" heap_size(0x%lx), cma_remain(0x%lx)\n",
			allocated_size, seccg_heap->heap_size, cma_remain);
		return NULL;
	}

	pg = cma_alloc(seccg_heap->cma, count, (u32)get_order((u32)per_bit_sz), GFP_KERNEL);
	if (!pg) {
		pr_err("out of memory,cma remain: 0x%llx, type: %u, usesize: 0x%lx\n",
			cma_remain, seccg_heap->cma_type, allocated_size);
		return NULL;
	}

	if (!__set_sec_mem_attribute(pg, size, seccg_heap, count)) {
		ret = cma_release(seccg_heap->cma, pg, count);
		pr_err("%s %llu MB memory sec config failed, release[%d]\n",
			__func__, size / SZ_1M, ret);
		return NULL;
	}
	seccg_heap->cma_alloc_size += size;
	pr_info("out %s %llu MB memory(ret = %d)\n", __func__, size / SZ_1M, ret);
	return pg;
}

static bool __clear_sec_mem_attribute(struct seccg_heap *seccg_heap,
	struct page *page, unsigned long size)
{
	if (__cons_phys_set_sec(seccg_heap, 1,
		DMA_HEAP_SEC_CMD_TABLE_CLEAN, page, size)) {
		pr_err("%s heap_attr: [%u] unconfig failed\n",
			__func__, seccg_heap->heap_attr);
		return false;
	}

#ifdef CONFIG_NEED_CHANGE_MAPPING
	if (dma_buf_is_secure(seccg_heap->flag)) {
		change_secpage_range(page_to_phys(page),
			(unsigned long)page_address(page), size, PAGE_KERNEL);
		flush_tlb_all();
		return true;
	} else {
		WARN_ON(1);
		pr_err("%s heap_attr: [%u] buffer flag is err\n",
			       __func__, seccg_heap->heap_attr);
		return false;
	}
#endif
	return true;
}

static int __free_mem_to_cma(struct seccg_heap *seccg_heap, struct page *page,
	unsigned long size)
{
	u64 free_size;

	free_size = ALIGN(size, seccg_heap->per_bit_sz);
	if (!__clear_sec_mem_attribute(seccg_heap, page, free_size)) {
		pr_err("%s, [%s] unset failed, page-0x%llx, free-size-0x%llx\n",
			__func__, seccg_heap->name, page_to_phys(page), free_size);
		return -EFAULT;
	}

	(void)cma_release(seccg_heap->cma, page, free_size >> PAGE_SHIFT);
	seccg_heap->cma_alloc_size -= free_size;
	return 0;
}

static struct page *__alloc_mem_from_pool(struct seccg_heap *seccg_heap,
	unsigned long size)
{
	unsigned long offset = 0;
	int ret = 0;
	/* align size */
	offset = gen_pool_alloc(seccg_heap->pool, size);
	if (!offset) {
		ret = add_cma_to_pool(seccg_heap, size);
		if (ret)
			return NULL;
		offset = gen_pool_alloc(seccg_heap->pool, size);
		if (!offset) {
			pr_err("%s, gen_pool_alloc failed\n", __func__);
			return NULL;
		}
	}
	return pfn_to_page(PFN_DOWN(offset));
}

static int __free_mem_to_pool(struct seccg_heap *seccg_heap, struct page *page,
	unsigned long size)
{
	if (__seccg_pool_clear_sec_attr(seccg_heap)) {
		pr_err("%s, [%s]clear_sec_attr failed\n", __func__, seccg_heap->name);
		return -EFAULT;
	}
	if (!seccg_heap->static_cma_region)
		(void)cma_release(seccg_heap->cma, page, size >> PAGE_SHIFT);
	else
		gen_pool_free(seccg_heap->static_cma_region, page_to_phys(page), size);
#ifdef CONFIG_NEED_CHANGE_MAPPING
	change_secpage_range(page_to_phys(page),
		(unsigned long)page_address(page), size, PAGE_KERNEL);
	flush_tlb_all();
#endif
	return 0;
}

struct dma_buf *seccg_alloc_contig(struct seccg_heap *seccg_heap,
						struct mm_heap_helper_buffer *buffer,
						unsigned long size, unsigned long fd_flags)
{
	int ret = 0;
	struct sg_table *table = NULL;
	struct page *page = NULL;
	struct dma_buf *dmabuf = NULL;
	bool heap_pool_disable = seccg_heap->pool_disabled;

	table = kzalloc(sizeof(*table), GFP_KERNEL);
	if (!table)
		return ERR_PTR(-ENOMEM);

	if (sg_alloc_table(table, 1, GFP_KERNEL)) {
		pr_err("%s, sg_alloc_table failed\n", __func__);
		goto err_out1;
	}

	if (heap_pool_disable) {
		if (dma_buf_is_unsecure(buffer->flags)) {
			pr_err("%s heap_attr: [%u] buffer flag is err\n",
			       __func__, seccg_heap->heap_attr);
			goto err_out2;
		}
		page = __alloc_mem_direct_from_cma(seccg_heap, size);
		if (!page)
			goto err_out2;
	} else {
		page = __alloc_mem_from_pool(seccg_heap, size);
		if (!page)
			goto err_out2;
	}

	if ((seccg_heap->heap_attr != SEC_DRM_TEE) &&
		(seccg_heap->heap_attr != SEC_AOD)) {
		(void)mm_dma_heap_pages_zero(page, size,
					  pgprot_writecombine(PAGE_KERNEL));
#ifndef CONFIG_MM_LB_FULL_COHERENCY
#ifdef CONFIG_MM_DMA_HEAP_FLUSH_CACHE_ALL
		dma_heap_flush_all_cpus_caches_raw();
#else
		dma_heap_flush_all_cpus_caches();
#endif
#else
		dma_ci_pages(page, size);
#endif
	}

	sg_set_page(table->sgl, page, (unsigned int)size, 0);
	sg_dma_address(table->sgl) = sg_phys(table->sgl);
	sg_dma_len(table->sgl) = table->sgl->length;
	buffer->sg_table = table;

	if ((seccg_heap->heap_attr == SEC_DRM_TEE) ||
		(seccg_heap->heap_attr == SEC_AOD))
		pr_info("%s, sec buffer[%d] phys[0x%lx], size[0x%lx]\n",
			__func__, seccg_heap->heap_attr, sg_phys(table->sgl), size);

	/* create the dmabuf */
	dmabuf = mm_heap_helper_export_dmabuf(buffer, fd_flags);
	if (IS_ERR(dmabuf))
		goto free_pages;
	buffer->dmabuf = dmabuf;
	seccg_heap->alloc_size += size;

	return dmabuf;

free_pages:
	if (heap_pool_disable)
		ret = __free_mem_to_cma(seccg_heap, page, size);
	else
		ret = __free_mem_to_pool(seccg_heap, page, size);
	if (ret)
		return ERR_PTR(-EINVAL);
err_out2:
	sg_free_table(table);
err_out1:
	kfree(table);

	return ERR_PTR(-EINVAL);
}

static void __seccg_free_pool(struct seccg_heap *seccg_heap,
			      struct sg_table *table,
			      struct mm_heap_helper_buffer *buffer)
{
	struct page *page = sg_page(table->sgl);
	phys_addr_t paddr = PFN_PHYS(page_to_pfn(page));

	if (dma_buf_is_unsecure(buffer->flags)) {
		(void)mm_dma_heap_buffer_zero(buffer);
		if (dma_buf_is_cached(buffer->flags))
			dma_sync_sg_for_device(seccg_heap->heap.heap_dev, table->sgl,
					       (int)table->nents,
					       DMA_BIDIRECTIONAL);
	}
	gen_pool_free(seccg_heap->pool, paddr, buffer->size);

	if (seccg_heap->heap_attr == SEC_DRM_TEE)
		pr_info("%s, sec buffer phys[0x%lx], size[0x%lx]\n",
			__func__, (unsigned long)paddr, buffer->size);

	sg_free_table(table);
	kfree(table);
}

int seccg_fill_watermark(struct seccg_heap *seccg_heap)
{
	struct page *pg = NULL;
	u64 size = seccg_heap->water_mark;
	u64 per_bit_sz = seccg_heap->per_bit_sz;
	struct alloc_list *alloc = NULL;
	u32 count = (u32)size >> PAGE_SHIFT;
	u32 align = (u32)get_order((u32)per_bit_sz);

	if (!size || size > seccg_heap->per_alloc_sz)
		return -EINVAL;

	alloc = kzalloc(sizeof(*alloc), GFP_KERNEL);
	if (!alloc)
		return -ENOMEM;

	pg = __seccg_cma_alloc(seccg_heap, (size_t)count, size, align);
	if (!pg) {
		pr_err("%s, alloc cma fail\n", __func__);
		kfree(alloc);
		return -ENOMEM;
	}

	alloc->addr = page_to_phys(pg);
	alloc->size = (unsigned int)size;
	list_add_tail(&alloc->list, &seccg_heap->allocate_head);

	memset(page_address(pg), 0x0, size); /* unsafe_function_ignore: memset */
	gen_pool_free(seccg_heap->pool, page_to_phys(pg), size);
	pr_debug("%s, fill 0x%lx MB memory\n",
		    __func__, (size) / SZ_1M);

	return 0;
}

static void __disable_pool_free_contig(struct seccg_heap *seccg_heap,
			 struct mm_heap_helper_buffer *buffer)
{
	struct sg_table *table = buffer->sg_table;
	int ret;

	if (seccg_heap->alloc_size < buffer->size ||dma_buf_is_unsecure(buffer->flags)) {
		WARN_ON(1);
		pr_err("%s, free err, buffer size-0x%llx, alloc size-0x%llx\n",
				__func__, buffer->size, seccg_heap->alloc_size);
		return;
	}

#ifdef CONFIG_SECMEM_TEST
	pr_info("%s, sec buffer phys[0x%lx], size[0x%lx]\n", __func__,
		(unsigned long)sg_dma_address(table->sgl), buffer->size);
#endif

	ret = __free_mem_to_cma(seccg_heap, sg_page(table->sgl), table->sgl->length);
	if (ret)
		return;

	sg_free_table(table);
	kfree(table);
	seccg_heap->alloc_size -= buffer->size;
	return;
}

void seccg_free_contig(struct seccg_heap *seccg_heap,
			 struct mm_heap_helper_buffer *buffer)
{
	struct sg_table *table = buffer->sg_table;
	u64 water_mark;
	u64 avail;

	if (seccg_heap->pool_disabled) {
		__disable_pool_free_contig(seccg_heap, buffer);
		return;
	}

	__seccg_free_pool(seccg_heap, table, buffer);
	WARN_ON(seccg_heap->alloc_size < buffer->size);
	seccg_heap->alloc_size -= buffer->size;

	if (!seccg_heap->alloc_size) {
		if (__seccg_pool_clear_sec_attr(seccg_heap))
			goto out;

		avail = gen_pool_avail(seccg_heap->pool);
		water_mark = seccg_heap->water_mark;
		if (water_mark && avail == water_mark) {
			pr_info("%s, no need to release pool\n", __func__);
			return;
		}
		__seccg_pool_release(seccg_heap);

out:
		seccg_heap->cma_alloc_size = 0;
		if (seccg_heap->water_mark &&
		    seccg_fill_watermark(seccg_heap))
			pr_err("%s, seccg_fill_watermark failed!\n", __func__);
	}
}
