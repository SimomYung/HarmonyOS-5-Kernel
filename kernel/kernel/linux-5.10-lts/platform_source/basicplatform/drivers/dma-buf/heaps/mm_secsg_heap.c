/*
 * drivers/dma-buf/heaps/mm_secsg_heap.c
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
#define pr_fmt(fmt) "[secsg_heap]" fmt

#include <linux/dma-buf.h>
#include <linux/dma-mapping.h>
#include <linux/dma-heap.h>
#include <linux/cma.h>
#include <linux/err.h>
#include <linux/genalloc.h>
#include <linux/mm.h>
#include <linux/mutex.h>
#include <linux/highmem.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/scatterlist.h>
#include <linux/sizes.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <linux/security.h>
#include <linux/dmabuf/mm_dma_heap.h>

#include <asm/cacheflush.h>
#include <asm/tlbflush.h>

#include "mm_heap_helpers.h"
#include "mm_tee_op.h"
#include "mm_sec_alloc.h"
#include "mm_heap_priv.h"

#define MM_ALLOC_MAX_LATENCY_US	(20 * 1000) /* 20ms */
#define PAGE_ALLOC_MAX_LATENCY_MS 50 /* 50ms */
#define SECMEM_SET_MAX_LATENCY_MS 20 /* 20ms */

struct secsg_heap {
	struct dma_heap heap;
	struct cma *cma;
	struct gen_pool *pool;
	const char *name;
	void *priv;
	/* heap mutex */
	struct mutex mutex;
	/* heap base */
	phys_addr_t heap_base;
	/* heap align */
	phys_addr_t align;
	/* heap total size */
	size_t heap_size;
	/* heap allocated size */
	unsigned long alloc_size;
	/* align size: 1MB or 2MB */
	u64 per_bit_sz;
	/* each sg section size */
	u64 per_alloc_sz;
	struct dma_heap_sec_tee tee;
	/* heap attr: secure, protect */
	u32 heap_attr;
	u32 pool_flag;
	int ta_init;
	unsigned int id;
#ifdef CONFIG_DMABUF_HEAPS_SECSG_IRIS_SUPPORT_CAMERA
	unsigned long last_buffer_heap_flags;
#endif /* CONFIG_DMABUF_HEAPS_SECSG_IRIS_SUPPORT_CAMERA */
};

#ifdef CONFIG_MM_SECMEM_TEST
#define DMA_HEAP_FLAG_ALLOC_TEST (1U << 31)

static void double_alloc_test(struct secsg_heap *secsg_heap,
			      struct mm_heap_helper_buffer *buffer, u32 cmd)
{
	struct sg_table *table = buffer->sg_table;
	struct scatterlist *sg = NULL;
	struct page *page = NULL;
	struct mem_chunk_list mcl;
	struct tz_pageinfo *pageinfo = NULL;
	unsigned int nents = table->nents;
	int ret;
	u32 size = 0;
	u32 i = 0;

	pageinfo = kzalloc(nents * sizeof(*pageinfo), GFP_KERNEL);
	if (!pageinfo)
		return;

	for_each_sg(table->sgl, sg, table->nents, i) {
		page = sg_page(sg);
		pageinfo[i].addr = page_to_phys(page);
		pageinfo[i].nr_pages = sg->length / PAGE_SIZE;
		size += sg->length;
	}

	mcl.phys_addr = (void *)pageinfo;
	mcl.nents = nents;
	mcl.protect_id = SEC_TASK_DRM;
	mcl.size = size;

	ret = secmem_tee_exec_cmd(secsg_heap->tee.session, &mcl, cmd);
	if (!ret)
		pr_err("Test double alloc Fail\n");
	else
		pr_err("Test double alloc Succ\n");

	kfree(pageinfo);
}

static void smem_dma_heap_test(struct secsg_heap *secsg_heap,
		      struct mm_heap_helper_buffer *buffer)
{
	struct sg_table *table = buffer->sg_table;
	struct scatterlist *sg = NULL;
	struct page *page = NULL;
	struct mem_chunk_list mcl;
	struct tz_pageinfo *pageinfo = NULL;
	unsigned int nents = table->nents;
	int ret = 0;
	u32 size = 0;
	u32 i;

	if (!secsg_heap->ta_init) {
		ret = sec_tee_alloc(&secsg_heap->tee);
		if (ret) {
			pr_err("[%s] sec_tee_alloc failed.\n", __func__);
			return;
		}

		ret = sec_tee_init(&secsg_heap->tee, DMA_HEAP_SESSIONS_TEST);
		if (ret) {
			pr_err("[%s] TA init failed\n", __func__);
			sec_tee_free(&secsg_heap->tee);
			return;
		}
		secsg_heap->ta_init = 1;
	}

	pageinfo = kzalloc(nents * sizeof(*pageinfo), GFP_KERNEL);
	if (!pageinfo)
		return;

	for_each_sg(table->sgl, sg, table->nents, i) {
		page = sg_page(sg);
		pageinfo[i].addr = page_to_phys(page);
		pageinfo[i].nr_pages = sg->length / PAGE_SIZE;
		size += sg->length;
	}

	mcl.phys_addr = (void *)pageinfo;
	mcl.nents = nents;
	mcl.size = size;
	if (buffer->id) {
		mcl.protect_id = SEC_TASK_DRM;
		mcl.buff_id = buffer->id;
	} else {
		mcl.protect_id = SEC_TASK_SEC;
	}

	ret = secmem_tee_exec_cmd(secsg_heap->tee.session, &mcl, DMA_HEAP_SEC_CMD_TEST);
	if (ret)
		pr_err("Test sion Fail\n");
	else
		pr_err("Test sion Succ\n");

	secmem_tee_destroy(secsg_heap->tee.context, secsg_heap->tee.session);
	secsg_heap->ta_init = 0;
	kfree(pageinfo);
}
#endif

static bool secsg_type_filer(u32 heap_attr, u32 flag)
{
	if (dma_buf_is_unsecure(flag))
		return false;

	switch (heap_attr) {
	case SEC_DRM_TEE:
	case SEC_DRM_REDRAW:
#if defined(CONFIG_ARM_SMMU_V3) || defined(CONFIG_MM_SMMU_V3)
	case SEC_FACE_ID:
	case SEC_FACE_ID_3D:
#endif
	case SEC_TINY:
		return true;
	default:
		break;
	}

	return false;
}

static bool secsg_tiny_heap_check(struct secsg_heap *secsg_heap)
{
	if (secsg_heap->heap_attr == SEC_TINY)
		return true;
	return false;
}

static inline bool secsg_is_iris_heap(struct secsg_heap *secsg_heap)
{
	if (secsg_heap->heap_attr == SEC_FACE_ID_3D)
		return true;
	else
		return false;
}

static u32 __secsg_get_protect_id(u32 heap_attr)
{
	if (heap_attr == SEC_DRM_TEE || heap_attr == SEC_DRM_REDRAW)
		return SEC_TASK_DRM;
	else if (heap_attr == SEC_TINY)
		return SEC_TASK_TINY;
	else
		return SEC_TASK_SEC;
}

static int change_scatter_prop(struct secsg_heap *secsg_heap,
			       struct mm_heap_helper_buffer *buffer,
			       u32 cmd)
{
	struct sg_table *table = buffer->sg_table;
	struct scatterlist *sg = NULL;
	struct page *page = NULL;
	struct mem_chunk_list mcl;
	struct tz_pageinfo *pageinfo = NULL;
	unsigned int nents = table->nents;
	int ret;
	u32 i;

	if (!secsg_heap->ta_init) {
		pr_err("[%s] TA not inited.\n", __func__);
		return -EINVAL;
	}

	mcl.protect_id = __secsg_get_protect_id(secsg_heap->heap_attr);

	if (cmd == DMA_HEAP_SEC_CMD_ALLOC) {
		pageinfo = kzalloc(nents * sizeof(*pageinfo), GFP_KERNEL);
		if (!pageinfo)
			return -ENOMEM;

		for_each_sg(table->sgl, sg, table->nents, i) {
			page = sg_page(sg);
			pageinfo[i].addr = page_to_phys(page);
			pageinfo[i].nr_pages = sg->length / PAGE_SIZE;
		}

		mcl.phys_addr = (void *)pageinfo;
		mcl.nents = nents;
#ifdef CONFIG_MM_SECMEM_DRM_TEE_API_SUPPORT
		if (mcl.protect_id == SEC_TASK_DRM &&
			(buffer->flags & DMA_HEAP_FLAGS_DRM_TEE_MASK))
			mcl.ta_rw_index = DMA_HEAP_FLAGS_2_TA_RW_INDEX(buffer->flags);
#endif
	} else if (cmd == DMA_HEAP_SEC_CMD_FREE) {
		mcl.buff_id = buffer->id;
		mcl.phys_addr = NULL;
	} else {
		pr_err("%s: Error cmd\n", __func__);
		return -EINVAL;
	}

	ret = secmem_tee_exec_cmd(secsg_heap->tee.session, &mcl, cmd);
	if (ret) {
		pr_err("%s:exec cmd[%d] fail\n", __func__, cmd);
		ret = -EINVAL;
	} else {
		if (cmd == DMA_HEAP_SEC_CMD_ALLOC)
			buffer->id = mcl.buff_id;
	}

	kfree(pageinfo);

	return ret;
}

static struct page *secsg_alloc_page(struct secsg_heap *secsg_heap,
				       unsigned long size,
				       unsigned long align)
{
	struct page *page = NULL;
	unsigned long offset;
	u32 count = 0;
	struct genpool_data_align data = { .align = align };
	ktime_t stime = 0, etime = 0;
	s64 timedelta = 0;

	stime = ktime_get();
	if (secsg_heap->pool_flag) {
		if (secsg_heap->heap_attr == SEC_FACE_ID_3D)
			offset = gen_pool_alloc_algo(secsg_heap->pool, size, gen_pool_first_fit_align, &data);
		else
			offset = gen_pool_alloc(secsg_heap->pool, size);
		if (!offset) {
			return NULL;
		}

		page = pfn_to_page(PFN_DOWN(offset));
	} else {
		count = (u32)size / PAGE_SIZE;
		page = cma_alloc(secsg_heap->cma, count,
				 (u32)get_order((u32)align), GFP_KERNEL);
	}

	etime = ktime_get();
	timedelta = ktime_ms_delta(etime, stime);
	if (unlikely(timedelta > PAGE_ALLOC_MAX_LATENCY_MS))
		pr_err("%s: page alloc cost > %d ms, size:%lu time_cost:%lld",
			__func__, PAGE_ALLOC_MAX_LATENCY_MS, size, timedelta);

	return page;
}

static inline bool secsg_dma_buf_is_cam_cma(unsigned long flags)
{
#ifdef CONFIG_DMABUF_HEAPS_SECSG_IRIS_SUPPORT_CAMERA
	return (bool)(flags & DMA_HEAP_FLAG_CAM_CMA_BUFFER);
#else
	return false;
#endif /* CONFIG_DMABUF_HEAPS_SECSG_IRIS_SUPPORT_CAMERA */
}

static void __secsg_free_page(struct secsg_heap *secsg_heap,
			      struct page *page,
			      unsigned long size)
{
	if (secsg_heap->pool_flag)
		gen_pool_free(secsg_heap->pool, page_to_phys(page), size);
	else
		cma_release(secsg_heap->cma, page, (u32)size / PAGE_SIZE);
}

static void secsg_free_scatter(struct secsg_heap *secsg_heap,
				 struct mm_heap_helper_buffer *buffer)
{
	struct sg_table *table = buffer->sg_table;
	struct scatterlist *sg = NULL;
	u32 i;
	int ret;

	if (secsg_type_filer(secsg_heap->heap_attr, buffer->flags)) {
		ret = change_scatter_prop(secsg_heap, buffer, DMA_HEAP_SEC_CMD_FREE);
		if (ret) {
			pr_err("release MPU protect fail! Need check runtime\n");
			return;
		}
	}

	if (secsg_heap->pool_flag) {
		buffer->flags = dma_buf_set_cache(buffer->flags);
		mm_dma_heap_buffer_zero(buffer);

#ifndef CONFIG_MM_LB_FULL_COHERENCY
#ifdef CONFIG_DISABLE_FLUSH_ALL_CPUS_CACHES
		dma_ci_cache_by_sg(table, DMA_FROM_DEVICE);
#else
#ifdef CONFIG_MM_DMA_HEAP_FLUSH_CACHE_ALL
		dma_heap_flush_all_cpus_caches_raw();
#else
		dma_heap_flush_all_cpus_caches();
#endif
#endif
#else
		dma_ci_sgt(table);
#endif
	}

	for_each_sg(table->sgl, sg, table->nents, i) {
#ifdef CONFIG_NEED_CHANGE_MAPPING
		if (!secsg_dma_buf_is_cam_cma(buffer->flags)) {
			change_secpage_range(page_to_phys(sg_page(sg)),
						(unsigned long)page_address(sg_page(sg)),
						sg->length, PAGE_KERNEL);
			flush_tlb_all();
		}
#endif
		__secsg_free_page(secsg_heap, sg_page(sg), sg->length);
	}
#ifdef CONFIG_NEED_CHANGE_MAPPING
	flush_tlb_all();
#endif
	sg_free_table(table);
	kfree(table);

	secsg_heap->alloc_size -= buffer->size;
}

static int secsg_heap_phys(struct dma_heap *heap, struct dma_buf *dmabuf,
				phys_addr_t *addr, size_t *len)
{
	struct mm_heap_helper_buffer *buffer = NULL;
	struct secsg_heap *secsg_heap = NULL;

	if (IS_ERR_OR_NULL(heap) || IS_ERR_OR_NULL(dmabuf) ||
		IS_ERR_OR_NULL(addr) || IS_ERR_OR_NULL(len)) {
		pr_err("%s: invalid input para!\n", __func__);
		return -EINVAL;
	}

	buffer = dmabuf->priv;
	if (!buffer) {
		pr_err("%s: get heap_helper buffer fail!\n", __func__);
		return -EINVAL;
	}

	secsg_heap = container_of(heap, struct secsg_heap, heap);
	if (!secsg_type_filer(secsg_heap->heap_attr, buffer->flags)) {
		pr_err("%s: sec buffer don't support get phys!\n", __func__);
		return -EINVAL;
	}

	*addr = buffer->id;
	*len = buffer->size;

	return 0;
}

static struct page *secsg_alloc_large(struct secsg_heap *secsg_heap,
					unsigned long size,
					unsigned long align)
{
	struct page *page = NULL;

	page = secsg_alloc_page(secsg_heap, size, align);
	if (!page)
		return NULL;

	if (!secsg_heap->pool_flag)
		memset(page_to_virt(page), 0, size);

	return page;
}

static int flush_tlb_cache_send_cmd(struct mm_heap_helper_buffer *buffer,
				    unsigned long size,
				    struct sg_table *table,
				    struct secsg_heap *secsg_heap)
{
	int ret;
	ktime_t stime = 0, etime = 0;
	s64 timedelta = 0;

#ifdef CONFIG_NEED_CHANGE_MAPPING
	flush_tlb_all();
#endif

#if !defined(CONFIG_MM_LB_FULL_COHERENCY) && !defined(CONFIG_DISABLE_FLUSH_ALL_CPUS_CACHES)
	if (size >= MM_DMA_HEAP_FLUSH_ALL_CPUS_CACHES) {
#ifdef CONFIG_MM_DMA_HEAP_FLUSH_CACHE_ALL
		dma_heap_flush_all_cpus_caches_raw();
#else
		dma_heap_flush_all_cpus_caches();
#endif
	} else {
#endif
		dma_sync_sg_for_cpu(secsg_heap->heap.heap_dev,
				    table->sgl,
				    table->nents,
				    DMA_FROM_DEVICE);
#if !defined(CONFIG_MM_LB_FULL_COHERENCY) && !defined(CONFIG_DISABLE_FLUSH_ALL_CPUS_CACHES)
	}
#endif

	/*
	 * Send cmd to secos change the memory form normal to protect when
	 * user is DRM, and record some information of the sg_list for secos
	 * va map.
	 * Other user don't need to do this.
	 */
	if (secsg_type_filer(secsg_heap->heap_attr, buffer->flags)) {
		stime = ktime_get();
		ret = change_scatter_prop(secsg_heap, buffer,
					  DMA_HEAP_SEC_CMD_ALLOC);
		if (ret)
			return -EINVAL;
		etime = ktime_get();
		timedelta = ktime_ms_delta(etime, stime);
		if (unlikely(timedelta > SECMEM_SET_MAX_LATENCY_MS))
			pr_err("%s: secmem set cost > %d ms, time_cost:%lld",
				__func__, SECMEM_SET_MAX_LATENCY_MS, timedelta);
	}
#ifdef CONFIG_MM_SECMEM_TEST
	if ((buffer->flags & DMA_HEAP_FLAG_ALLOC_TEST) &&
		secsg_type_filer(secsg_heap->heap_attr, buffer->flags)) {
		pr_err("smem_dma_heap TEST start!\n");
		double_alloc_test(secsg_heap, buffer, DMA_HEAP_SEC_CMD_ALLOC);
		smem_dma_heap_test(secsg_heap, buffer);
		pr_err("smem_dma_heap TEST end!\n");
	}
#endif

	return 0;
}

static void secsg_heap_buf_free(struct mm_heap_helper_buffer *buffer)
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

static inline bool is_iris_camera_support(struct secsg_heap *secsg_heap, unsigned long flags) {
#ifdef CONFIG_DMABUF_HEAPS_SECSG_IRIS_SUPPORT_CAMERA
	return (secsg_is_iris_heap(secsg_heap) && secsg_dma_buf_is_cam_cma(flags));
#else
	return false;
#endif /* CONFIG_DMABUF_HEAPS_SECSG_IRIS_SUPPORT_CAMERA */
}

static struct dma_buf *secsg_alloc_scatter(struct secsg_heap *secsg_heap,
			  struct mm_heap_helper_buffer *buffer, unsigned long size,
			  unsigned long fd_flags)
{
	struct dma_buf *dmabuf = NULL;
	struct sg_table *table = NULL;
	struct scatterlist *sg = NULL;
	struct page *page = NULL;
	unsigned long per_bit_sz = secsg_heap->per_bit_sz;
	unsigned long per_alloc_sz = secsg_heap->per_alloc_sz;
	unsigned long alloc_size = 0;
	unsigned long size_remaining;
	unsigned long nents;
	int ret;
	u32 i = 0;
	bool is_camera = false;

#ifdef CONFIG_DMABUF_HEAPS_SECSG_IRIS_SUPPORT_CAMERA
	u64 phys_addr = 0;

	is_camera = secsg_dma_buf_is_cam_cma(buffer->flags);
	if (is_iris_camera_support(secsg_heap, buffer->flags)) {
		/* camera alloc min size 4K */
		per_bit_sz = SZ_4K;
		per_alloc_sz = size;
	}
#endif /* CONFIG_DMABUF_HEAPS_SECSG_IRIS_SUPPORT_CAMERA */

	size_remaining = ALIGN(size, per_bit_sz);
	nents = ALIGN(size, per_alloc_sz) / per_alloc_sz;

	table = kzalloc(sizeof(*table), GFP_KERNEL);
	if (!table)
		return ERR_PTR(-ENOMEM);

	if (sg_alloc_table(table, (u32)nents, GFP_KERNEL))
		goto free_table;

	/*
	 * DRM memory alloc from CMA pool.
	 * In order to speed up the allocation, we will apply for memory
	 * in units of 2MB, and the memory portion of less than 2MB will
	 * be applied for one time.
	 */
	sg = table->sgl;

	while (size_remaining) {
		if (size_remaining > per_alloc_sz)
			alloc_size = per_alloc_sz;
		else
			alloc_size = size_remaining;

		page = secsg_alloc_large(secsg_heap, alloc_size, per_bit_sz);
		if (!page)
			goto free_pages;

#ifdef CONFIG_DMABUF_HEAPS_SECSG_IRIS_SUPPORT_CAMERA
		if (secsg_is_iris_heap(secsg_heap) && (!secsg_dma_buf_is_cam_cma(buffer->flags))) {
			phys_addr = page_to_phys(page);
			if ((phys_addr % SZ_2M) != 0) {
				pr_err("%s, iris heap buffer phys addr not assign to 2M: 0x%x\n", __func__, phys_addr);
				mm_dmaheap_heap_show(&secsg_heap->heap);
			}
		}
#endif /* CONFIG_DMABUF_HEAPS_SECSG_IRIS_SUPPORT_CAMERA */

#ifdef CONFIG_NEED_CHANGE_MAPPING
		/*
		 * Before set memory in secure region, we need change kernel
		 * pgtable from normal to device to avoid big CPU Speculative
		 * read.
		 */
		if (!is_camera)
			change_secpage_range(page_to_phys(page),
				(unsigned long)page_address(page), alloc_size, __pgprot(PROT_DEVICE_nGnRE));
#endif
		size_remaining -= alloc_size;
		sg_set_page(sg, page, (u32)alloc_size, 0);
		sg_dma_address(sg) = sg_phys(sg);
		sg_dma_len(sg) = sg->length;
		sg = sg_next(sg);
		i++;
	}
	buffer->sg_table = table;

	/*
	 * After change the pgtable prot, we need flush TLB and cache.
	 */
	if (!is_camera) {
		ret = flush_tlb_cache_send_cmd(buffer, size, table, secsg_heap);
		if (ret)
			goto free_pages;
	}

	/* create the dmabuf */
	dmabuf = mm_heap_helper_export_dmabuf(buffer, fd_flags);
	if (IS_ERR(dmabuf)) {
		ret = PTR_ERR(dmabuf);
		goto unconfig_prop;
	}

	return dmabuf;

unconfig_prop:
	if (secsg_type_filer(secsg_heap->heap_attr, buffer->flags)) {
		ret = change_scatter_prop(secsg_heap, buffer, DMA_HEAP_SEC_CMD_FREE);
		if (ret) {
			pr_err("%s, [%s]alloc fail, unconfig prop fail!\n",
					__func__, secsg_heap->name);
			return ERR_PTR(-ENOMEM);
		}
	}
free_pages:
	nents = i;
	for_each_sg(table->sgl, sg, nents, i) {
		page = sg_page(sg);
#ifdef CONFIG_NEED_CHANGE_MAPPING
		if (!is_camera) {
			change_secpage_range(page_to_phys(page),
				(unsigned long)page_address(page), sg->length, PAGE_KERNEL);
			flush_tlb_all();
		}
#endif
		__secsg_free_page(secsg_heap, page, sg->length);
	}
#ifdef CONFIG_NEED_CHANGE_MAPPING
	flush_tlb_all();
#endif
	sg_free_table(table);
free_table:
	kfree(table);
	pr_err("%s: [%s]alloc fail, size[0x%lx], heap all alloc_size[0x%lx], heap_size[0x%lx]\n",
		__func__, secsg_heap->name, size, secsg_heap->alloc_size, secsg_heap->heap_size);

	return ERR_PTR(-ENOMEM);
}

static int secsg_heap_input_check(struct secsg_heap *secsg_heap,
				    unsigned long size, unsigned long flag)
{
	if (secsg_heap->alloc_size + size <= secsg_heap->alloc_size) {
		pr_err("size overflow! alloc_size = 0x%lx, size = 0x%lx,\n",
			secsg_heap->alloc_size, size);
		return -EINVAL;
	}

	if ((secsg_heap->alloc_size + size) > secsg_heap->heap_size) {
		pr_err("alloc size = 0x%lx, size = 0x%lx, heap size = 0x%lx\n",
			secsg_heap->alloc_size, size, secsg_heap->heap_size);
		if (secsg_is_iris_heap(secsg_heap) && !secsg_dma_buf_is_cam_cma(flag))
			mm_dmaheap_heap_show(&secsg_heap->heap);
		return -EINVAL;
	}

#ifdef CONFIG_DMABUF_HEAPS_SECSG_IRIS_SUPPORT_CAMERA
	if (secsg_is_iris_heap(secsg_heap) && (secsg_heap->alloc_size != 0)) {
		if (secsg_dma_buf_is_cam_cma(flag) != 
			secsg_dma_buf_is_cam_cma(secsg_heap->last_buffer_heap_flags)) {
			pr_err("iris heap invalid heap_flags: 0x%lx, last buffer heap_flags: 0x%lx\n",
				flag, secsg_heap->last_buffer_heap_flags);
			mm_dmaheap_heap_show(&secsg_heap->heap);
			return -EINVAL;
		}
	}

	if (is_iris_camera_support(secsg_heap, flag)) {
		if (!secsg_heap->pool_flag) {
			pr_err("iris heap without use-static-pool not support camera\n");
			return -EINVAL;
		}
		if (dma_buf_is_secure(flag)) {
			pr_err("invalid alloc flag in heap(%u) flag(%lx)\n",
				secsg_heap->heap_attr, flag);
			return -EINVAL;
		}
		if (size % SZ_4K != 0) {
			pr_err("camera alloc form iris invalid size: 0x%lx\n", size);
			return -EINVAL;
		}
		return 0;
	}
#endif /* CONFIG_DMABUF_HEAPS_SECSG_IRIS_SUPPORT_CAMERA */

	if (!secsg_tiny_heap_check(secsg_heap) &&
		dma_buf_is_unsecure(flag)) {
		pr_err("invalid alloc flag in heap(%u) flag(%lx)\n",
		       secsg_heap->heap_attr, flag);
		return -EINVAL;
	}

	return 0;
}

static struct dma_buf *secsg_heap_allocate(struct dma_heap *heap,
					    unsigned long len,
					    unsigned long fd_flags,
					    unsigned long heap_flags)
{
	struct secsg_heap *secsg_heap =
		container_of(heap, struct secsg_heap, heap);
	struct mm_heap_helper_buffer *buffer = NULL;
	struct dma_buf *dmabuf = NULL;
	int ret;

	buffer = kzalloc(sizeof(*buffer), GFP_KERNEL);
	if (!buffer)
		return ERR_PTR(-ENOMEM);

#ifdef CONFIG_DMABUF_HEAPS_SECSG_IRIS_SUPPORT_CAMERA
	if (heap_flags & DMA_HEAP_FLAG_CAM_CMA_BUFFER) {
		dmabuf_systrace_begin("security_iris_heap_allow");
		if (security_iris_heap_allow() != 0) {
			heap_flags &= ~DMA_HEAP_FLAG_CAM_CMA_BUFFER;
		}
		dmabuf_systrace_end();
	}
#endif

	buffer->alloc_time_start = ktime_get();
	init_mm_heap_helper_buffer(buffer, secsg_heap_buf_free);
	buffer->heap = heap;
	buffer->size = len;
	buffer->flags = heap_flags;
	buffer->magic = atomic64_inc_return(&dmaheap_magic);

	mutex_lock(&secsg_heap->mutex);
	if (secsg_heap_input_check(secsg_heap, len, heap_flags)) {
		pr_err("input params failed\n");
		goto out;
	}

	/* init the TA conversion here */
	if (secsg_type_filer(secsg_heap->heap_attr, heap_flags) &&
		!secsg_heap->ta_init) {
		ret = sec_tee_init(&secsg_heap->tee, DMA_HEAP_SESSIONS_SECMEM);
		if (ret) {
			pr_err("[%s] TA init failed\n", __func__);
			goto out;
		}
		secsg_heap->ta_init = 1;
	}

	dmabuf = secsg_alloc_scatter(secsg_heap, buffer, len, fd_flags);
	if (IS_ERR(dmabuf)) {
		if (secsg_is_iris_heap(secsg_heap) && !secsg_dma_buf_is_cam_cma(heap_flags))
			mm_dmaheap_heap_show(&secsg_heap->heap);
		goto out;
	}

	buffer->dmabuf = dmabuf;
	secsg_heap->alloc_size += len;

#ifdef CONFIG_DMABUF_HEAPS_SECSG_IRIS_SUPPORT_CAMERA
	secsg_heap->last_buffer_heap_flags = heap_flags;
#endif /* CONFIG_DMABUF_HEAPS_SECSG_IRIS_SUPPORT_CAMERA */
	mutex_unlock(&secsg_heap->mutex);

	if (dma_buf_is_unsecure(heap_flags))
		dma_heap_flush_buffer_cache(buffer, heap_flags);

	buffer->alloc_time_end = ktime_get();
	mm_dmaheap_init_dump(buffer);
	mm_dmaheap_buffer_add(buffer);

	return dmabuf;

out:
	mutex_unlock(&secsg_heap->mutex);
	kfree(buffer);
	pr_err("%s, [%s]alloc fail, size[0x%lx], heap all alloc_size[0x%lx], heap_size [0x%lx]\n",
			__func__, secsg_heap->name, len,
			secsg_heap->alloc_size, secsg_heap->heap_size);
	return ERR_PTR(-EINVAL);
}

static void secsg_heap_free(struct mm_heap_helper_buffer *buffer)
{
	struct secsg_heap *secsg_heap =
			container_of(buffer->heap, struct secsg_heap, heap);
	int is_lock_recursive;

	is_lock_recursive = mm_heap_mutex_lock_recursive(&secsg_heap->mutex);
	secsg_free_scatter(secsg_heap, buffer);
	mm_heap_mutex_unlock_recursive(&secsg_heap->mutex, is_lock_recursive);
}

static int secsg_heap_map_user(struct dma_heap *heap,
		struct mm_heap_helper_buffer *buffer, struct vm_area_struct *vma)
{
	struct secsg_heap *secsg_heap =
		container_of(heap, struct secsg_heap, heap);

	if (secsg_type_filer(secsg_heap->heap_attr, buffer->flags)) {
		pr_err("%s, [%s]don't support map user\n",
					__func__, secsg_heap->name);
		return -EINVAL;
	}

	return mm_dma_heap_map_user(heap, buffer, vma);
}

static void *secsg_heap_map_kernel(struct dma_heap *heap,
				struct mm_heap_helper_buffer *buffer)
{
	struct secsg_heap *secsg_heap =
		container_of(heap, struct secsg_heap, heap);

	if (secsg_type_filer(secsg_heap->heap_attr, buffer->flags)) {
		pr_err("%s, [%s]don't support map kernel\n",
					__func__, secsg_heap->name);
		return ERR_PTR(-EINVAL);
	}

	return mm_dma_heap_map_kernel(heap, buffer);
}

static void secsg_heap_unmap_kernel(struct dma_heap *heap,
				struct mm_heap_helper_buffer *buffer)
{
	struct secsg_heap *secsg_heap =
		container_of(heap, struct secsg_heap, heap);

	if (secsg_type_filer(secsg_heap->heap_attr, buffer->flags)) {
		pr_err("%s, [%s]don't support unmap kernel\n",
					__func__, secsg_heap->name);
		return;
	}

	mm_dma_heap_unmap_kernel(heap, buffer);
}

static struct dma_heap_ops secsg_heap_ops = {
	.allocate = secsg_heap_allocate,
	.free = secsg_heap_free,
	.map_kernel = secsg_heap_map_kernel,
	.unmap_kernel = secsg_heap_unmap_kernel,
	.map_user = secsg_heap_map_user,
	.get_heap_phys = secsg_heap_phys,
};

static int secsg_parse_dt(struct secsg_heap *secsg_heap, struct device_node *np)
{
	u32 heap_base = 0;
	u32 heap_size = 0;
	u64 per_alloc_sz = 0;
	u64 per_bit_sz = 0;
	int ret;

	ret = of_property_read_string(np, "heap-name", &secsg_heap->name);
	if (ret < 0) {
		pr_err("%s, can not find heap-name\n", __func__);
		goto out;
	}

	ret = of_property_read_u32(np, "heap-base", &heap_base);
	if (ret < 0) {
		pr_err("%s, [%s]can not find prop:heap-base\n",
					__func__, secsg_heap->name);
		goto out;
	}
	secsg_heap->heap_base = heap_base;

	ret = of_property_read_u32(np, "heap-size", &heap_size);
	if (ret < 0) {
		pr_err("%s, [%s]can not find prop:heap-size\n",
					__func__, secsg_heap->name);
		goto out;
	}
	secsg_heap->heap_size = heap_size;

	ret = of_property_read_u32(np, "heap-attr", &secsg_heap->heap_attr);
	if (ret < 0) {
		pr_err("%s, [%s]can not find prop:heap-attr\n",
					__func__, secsg_heap->name);
		goto out;
	}

	if (secsg_heap->heap_attr >= SEC_SVC_MAX) {
		pr_err("%s, [%s]invalid heap-attr\n",
					__func__, secsg_heap->name);
		goto out;
	}

	ret = of_property_read_u64(np, "per-alloc-size", &per_alloc_sz);
	if (ret < 0) {
		pr_err("%s, [%s]can not find prop:per-alloc-size\n",
					__func__, secsg_heap->name);
		goto out;
	}
	secsg_heap->per_alloc_sz = PAGE_ALIGN(per_alloc_sz);

	ret = of_property_read_u64(np, "per-bit-size", &per_bit_sz);
	if (ret < 0) {
		pr_err("%s, [%s]can not find prop:per-bit-size\n",
					__func__, secsg_heap->name);
		goto out;
	}
	secsg_heap->per_bit_sz = PAGE_ALIGN(per_bit_sz);

	ret = of_property_read_u32(np, "use-static-pool", &secsg_heap->pool_flag);
	if (ret < 0) {
		pr_err("%s, [%s]can not find prop:use-static-pool\n",
					__func__, secsg_heap->name);
		secsg_heap->pool_flag = 0;
		ret = 0;
	}

out:
	return ret;
}

static int secsg_create_pool(struct secsg_heap *secsg_heap)
{
	struct page *page = NULL;
	u64 cma_base;
	u64 cma_size;
	int min_alloc_order = 0;

	cma_base = cma_get_base(secsg_heap->cma);
	cma_size = cma_get_size(secsg_heap->cma);
	min_alloc_order = get_order(secsg_heap->per_bit_sz) + PAGE_SHIFT;
#ifdef CONFIG_DMABUF_HEAPS_SECSG_IRIS_SUPPORT_CAMERA
	/* camera alloc min size 4K */
	if (secsg_is_iris_heap(secsg_heap))
		min_alloc_order = get_order(SZ_4K) + PAGE_SHIFT;
#endif /* CONFIG_DMABUF_HEAPS_SECSG_IRIS_SUPPORT_CAMERA */

	secsg_heap->pool = gen_pool_create(min_alloc_order, -1);
	if (!secsg_heap->pool) {
		pr_err("%s, create pool failed\n", __func__);
		return -ENOMEM;
	}
	gen_pool_set_algo(secsg_heap->pool, gen_pool_best_fit, NULL);

	page = cma_alloc(secsg_heap->cma, cma_size >> PAGE_SHIFT, 0, GFP_KERNEL);
	if (!page) {
		pr_err("%s, cma_alloc failed! base[0x%llx], size[0x%llx]\n",
			__func__, cma_base, cma_size);
		goto destroy_pool;
	}

	if (gen_pool_add(secsg_heap->pool, cma_base, cma_size, -1)) {
		pr_err("%s, gen_pool_add failed! base[0x%llx], size[0x%llx]\n",
			__func__, cma_base, cma_size);
		goto free_cma;
	}

	return 0;

free_cma:
	cma_release(secsg_heap->cma, page, (u32)cma_size >> PAGE_SHIFT);
destroy_pool:
	gen_pool_destroy(secsg_heap->pool);

	return -ENOMEM;
}

static int secsg_init_heap_properties(struct secsg_heap *secsg_heap,
				struct device_node *np)
{
	int ret;

	if (secsg_parse_dt(secsg_heap, np))
		return -EINVAL;

	secsg_heap->heap.name = secsg_heap->name;
	secsg_heap->heap.ops = &secsg_heap_ops;
	secsg_heap->heap.priv = NULL;
	kref_init(&secsg_heap->heap.refcount);
	secsg_heap->alloc_size = 0;

	secsg_heap->cma = get_svc_cma((int)secsg_heap->heap_attr);
	if (!secsg_heap->cma) {
		pr_err("%s, [%s]can't get cma!\n", __func__, secsg_heap->name);
		return -ENOMEM;
	}

	if (secsg_heap->pool_flag) {
		ret = secsg_create_pool(secsg_heap);
		if (ret) {
			pr_err("can't crate [%s] pool!\n", secsg_heap->name);
			return -ENOMEM;
		}
	}

	return 0;
}

static int secsg_heap_create(struct device_node *np)
{
	struct secsg_heap *secsg_heap = NULL;
	int ret;

	secsg_heap = kzalloc(sizeof(*secsg_heap), GFP_KERNEL);
	if (!secsg_heap)
		return -ENOMEM;

	mutex_init(&secsg_heap->mutex);
	ret = secsg_init_heap_properties(secsg_heap, np);
	if (ret)
		goto free_heap;

	secsg_heap->ta_init = 0;

	ret = dma_heap_add(&secsg_heap->heap);
	if (ret) {
		pr_err("%s, dma_heap_add[%s] failed!\n", __func__, secsg_heap->name);
		goto destroy_pools;
	}
	dma_coerce_mask_and_coherent(dma_heap_get_dev(&secsg_heap->heap),
						DMA_BIT_MASK(64));

	pr_err("secsg_heap [%s] info :\n"
		"\t\t\t\t\t\t\t heap attr : %u\n"
		"\t\t\t\t\t\t\t heap size : %lu MB\n"
		"\t\t\t\t\t\t\t per bit size : %llu KB\n"
		"\t\t\t\t\t\t\t per alloc size : %llu KB\n"
		"\t\t\t\t\t\t\t heap pool flag : %u\n"
		"\t\t\t\t\t\t\t cma name : %s\n"
		"\t\t\t\t\t\t\t cma base : 0x%llx\n"
		"\t\t\t\t\t\t\t cma size : 0x%lx\n",
		secsg_heap->name,
		secsg_heap->heap_attr,
		secsg_heap->heap_size / SZ_1M,
		secsg_heap->per_bit_sz / SZ_1K,
		secsg_heap->per_alloc_sz / SZ_1K,
		secsg_heap->pool_flag,
		cma_get_name(secsg_heap->cma),
		cma_get_base(secsg_heap->cma),
		cma_get_size(secsg_heap->cma));

	return 0;

destroy_pools:
	if (secsg_heap->pool_flag) {
		cma_release(secsg_heap->cma,
			pfn_to_page(cma_get_base(secsg_heap->cma) >> PAGE_SHIFT),
			cma_get_size(secsg_heap->cma) >> PAGE_SHIFT);
		gen_pool_destroy(secsg_heap->pool);
	}
free_heap:
	kfree(secsg_heap);

	return ret;
}

static int secsg_heap_init(void)
{
	struct device_node *sg_np = NULL, *child_np = NULL;
	int ret = 0;

	if (!dma_heap_node) {
		pr_err("%s, can not find dma_heap node\n", __func__);
		return -1;
	}

	sg_np = of_get_child_by_name(dma_heap_node, "secsg_heap");
	if (!sg_np) {
		pr_err("%s, can not find secsg_heap node\n", __func__);
		return -1;
	}

	for_each_available_child_of_node(sg_np, child_np) {
		ret = secsg_heap_create(child_np);
		if (ret)
			pr_err("%s, create[%s] failed\n",
					__func__, child_np->name);
	}

	return 0;
}

module_init(secsg_heap_init);
MODULE_LICENSE("GPL v2");
