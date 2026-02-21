/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include "hvgr_mem_pool.h"

#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/dma-mapping.h>
#include <linux/shrinker.h>
#include <linux/highmem.h>

#include "hvgr_defs.h"
#include "hvgr_pool.h"
#include "hvgr_mem_api.h"
#include "hvgr_memory_page.h"
#include "hvgr_log_api.h"
#include "hvgr_dm_api.h"

#ifdef CONFIG_DFX_OHOS
#include "hvgr_mem_quick_report.h"
#endif

static void hvgr_get_sc_pool_index(uint64_t attr, uint32_t *index)
{
	/* 0: NOOSH-BIG 1:NOOSH-LBW 2:OSH-BIG 3:OSH-LBW */
	if ((attr & HVGR_MEM_MASK_OSH) != 0)
		(*index) |= 0x2U;

	if ((attr & HVGR_MEM_ATTR_LESS_BANDWIDTH) != 0)
		(*index) |= 0x1U;
}

static void hvgr_get_normal_pool_index(uint64_t attr, uint32_t *index)
{
	/* 0: NOOSH 1:OSH */
	if ((attr & HVGR_MEM_MASK_OSH) != 0)
		(*index) |= 0x1U;
}

static uint32_t hvgr_get_pool_index(uint64_t attr)
{
	uint32_t index = 0;

	hvgr_get_normal_pool_index(attr, &index);

	if (WARN_ON(index >= MEM_POOL_NUM))
		index = 0;

	return index;
}

static struct hvgr_mem_pool *hvgr_mem_pool_get_ctx_pool(struct hvgr_ctx *ctx, uint64_t attr)
{
	struct hvgr_mem_pool *mem_pool = NULL;
	uint32_t pool_index = hvgr_get_pool_index(attr);

	if (mem_pool == NULL)
		mem_pool = &ctx->mem_ctx.mem_pool[pool_index];

	return mem_pool;
}

static struct hvgr_mem_pool *hvgr_mem_pool_get_dev_pool(struct hvgr_device *gdev,
	uint32_t pool_id, uint32_t flag)
{
	struct hvgr_mem_pool *mem_pool = NULL;

	if (mem_pool == NULL)
		mem_pool = &gdev->mem_dev.mem_pool[flag];

	return mem_pool;
}

static long hvgr_mem_pool_kernel_alloc(void *target, uint32_t nums, void **array)
{
	struct hvgr_mem_pool *mem_pool = (struct hvgr_mem_pool *)target;
	struct page **pages = (struct page **)array;

	return hvgr_mem_alloc_page(mem_pool->gdev->dev, pages, nums, mem_pool->order,
		mem_pool->pool_id, mem_pool->flag);
}

static void hvgr_mem_pool_kernel_free(void *target, uint32_t nums, void **array, uint32_t flag)
{
	struct hvgr_mem_pool *mem_pool = (struct hvgr_mem_pool *)target;
	struct page **pages = (struct page **)array;

	hvgr_mem_free_page(mem_pool->gdev->dev, pages, nums, mem_pool->order, mem_pool->pool_id);
}

static long hvgr_mem_pool_dev_alloc(void *target, uint32_t nums, void **array)
{
	struct hvgr_mem_pool *mem_pool = (struct hvgr_mem_pool *)target;
	struct hvgr_mem_pool *next_mem_pool =
		hvgr_mem_pool_get_dev_pool(mem_pool->gdev, mem_pool->pool_id, mem_pool->flag);

	return hvgr_pool_alloc(&next_mem_pool->pool, nums, array);
}

static void hvgr_mem_pool_dev_free(void *target, uint32_t nums, void **array, uint32_t flag)
{
	uint32_t pool_left;
	uint32_t i;
	dma_addr_t dma_addr;
	struct page *page = NULL;
	struct hvgr_mem_pool *mem_pool = (struct hvgr_mem_pool *)target;
	struct hvgr_mem_pool *next_mem_pool =
		hvgr_mem_pool_get_dev_pool(mem_pool->gdev, mem_pool->pool_id, mem_pool->flag);

	pool_left = next_mem_pool->pool.max_size - next_mem_pool->pool.cur_size;
	if (pool_left <= nums) {
		hvgr_mem_pool_kernel_free((void *)next_mem_pool, nums, array, flag);
		mem_pool->gdev->mem_dev.statics.dev_free_pages++;
		return;
	}

	for (i = 0; i < nums; i++) {
		page = (struct page *)array[i];
		clear_highpage(page);
		mem_pool->gdev->mem_dev.statics.pool_clear_pages++;

		dma_addr = hvgr_mem_get_page_dma_addr(page);
		dma_sync_single_for_device(mem_pool->gdev->dev, dma_addr, PAGE_SIZE,
			DMA_BIDIRECTIONAL);
		mem_pool->gdev->mem_dev.statics.pool_sync_pages++;
	}

	hvgr_pool_free(&next_mem_pool->pool, nums, array, 0U);
}

static unsigned long hvgr_mem_shrink_size(struct hvgr_mem_pool *mem_pool, unsigned long sys_nr)
{
	if (!mem_pool->prefill)
		return min((unsigned long)mem_pool->pool.cur_size, sys_nr);

	if (mem_pool->pool.cur_size <= mem_pool->pool.threshold)
		return 0U;

	return mem_pool->pool.cur_size - mem_pool->pool.threshold;
}

static unsigned long hvgr_mem_pool_shrink_free(struct shrinker *shrink,
	struct shrink_control *shrink_c)
{
	struct hvgr_mem_pool *mem_pool = NULL;
	unsigned long free_size;
	uint32_t i;
	struct page *page = NULL;
	struct list_head free_list_head;

	mem_pool = container_of(shrink, struct hvgr_mem_pool, sys_shrink);
	mutex_lock(&mem_pool->pool.pool_lock);
	if (mem_pool->refuse_shrink) {
		mutex_unlock(&mem_pool->pool.pool_lock);
		return 0U;
	}

	free_size = hvgr_mem_shrink_size(mem_pool, shrink_c->nr_to_scan);
	if (free_size == 0U) {
		mutex_unlock(&mem_pool->pool.pool_lock);
		return 0U;
	}

	INIT_LIST_HEAD(&free_list_head);

	for (i = 0; i < free_size; i++) {
		page = list_first_entry(&mem_pool->pool.list_head, struct page, lru);
		list_del_init(&page->lru);
		list_add(&page->lru, &free_list_head);
	}

	mem_pool->pool.cur_size -= (uint32_t)free_size;
	mutex_unlock(&mem_pool->pool.pool_lock);

	for (i = 0; i < free_size; i++) {
		page = list_first_entry(&free_list_head, struct page, lru);
		list_del_init(&page->lru);

		/* v300 and beyond no pgid in mem_pool, use pool_id to free */
		hvgr_mem_free_page(mem_pool->gdev->dev, &page, 1u, mem_pool->order,
			mem_pool->pool_id);
	}

	WARN_ON(!(list_empty(&free_list_head)));
	mem_pool->gdev->mem_dev.statics.pool_shrink_pages += free_size;
	return free_size;
}

static unsigned long hvgr_mem_pool_shrink_count(struct shrinker *shrink,
	struct shrink_control *shrink_c)
{
	struct hvgr_mem_pool *mem_pool = NULL;

	mem_pool = container_of(shrink, struct hvgr_mem_pool, sys_shrink);
	return hvgr_mem_shrink_size(mem_pool, mem_pool->pool.max_size);
}

static void hvgr_mem_pool_callback(struct hvgr_pool *pool, void *element, uint32_t flag)
{
	dma_addr_t dma_addr;
	struct hvgr_mem_pool *mem_pool = container_of(pool, struct hvgr_mem_pool, pool);
	struct page *page = (struct page *)element;

	if (page == NULL)
		return;

	if ((flag & HVGR_PAGE_CLEAR) != 0) {
		clear_highpage(page);
		mem_pool->gdev->mem_dev.statics.pool_clear_pages++;
	}

	dma_addr = hvgr_mem_get_page_dma_addr(page);
	dma_sync_single_for_device(mem_pool->gdev->dev, dma_addr, PAGE_SIZE, DMA_BIDIRECTIONAL);

	if ((flag & HVGR_PAGE_SYNC) != 0)
		mem_pool->gdev->mem_dev.statics.pool_sync_pages++;
}

static long hvgr_mem_pool_init(struct hvgr_mem_pool *mem_pool, struct hvgr_pool_data *data,
	uint32_t pool_size, uint32_t page_order)
{
	int ret;

	mem_pool->pool_id = 0;
	mem_pool->order = page_order;
	mem_pool->sys_shrink.count_objects = hvgr_mem_pool_shrink_count;
	mem_pool->sys_shrink.scan_objects = hvgr_mem_pool_shrink_free;
	mem_pool->sys_shrink.seeks = DEFAULT_SEEKS;
	mem_pool->sys_shrink.batch = 0;

	data->priv = mem_pool;
	data->max_size = pool_size;

	ret = (int)hvgr_pool_init(&mem_pool->pool, data);
	if (ret != 0) {
		hvgr_err(mem_pool->gdev, HVGR_MEM, "dev pool init fail %d", ret);
		return ret;
	}

	ret = register_shrinker(&mem_pool->sys_shrink);
	if (ret != 0) {
		hvgr_pool_term(&mem_pool->pool);
		hvgr_err(mem_pool->gdev, HVGR_MEM, "dev pool regist shrinker fail %d", ret);
	}

	return ret;
}

static long hvgr_mem_pool_grow(void *target, uint32_t size, void **array)
{
	return hvgr_mem_pool_kernel_alloc(target, size, array);
}

long hvgr_mem_pool_dev_init(struct hvgr_device * const gdev, struct hvgr_mem_pool *mem_pool,
	uint32_t pool_size, uint32_t page_order)
{
	struct hvgr_pool_data data;

	if (gdev == NULL || mem_pool == NULL || pool_size == 0)
		return -EINVAL;

	mem_pool->gdev = gdev;
	data.alloc = hvgr_mem_pool_kernel_alloc;
	data.free = hvgr_mem_pool_kernel_free;
	data.callback = hvgr_mem_pool_callback;
	data.prefill = mem_pool->prefill;
	if (mem_pool->growable)
		data.grow = hvgr_mem_pool_grow;
	else
		data.grow = NULL;

	return hvgr_mem_pool_init(mem_pool, &data, pool_size, page_order);
}

long hvgr_mem_pool_ctx_init(struct hvgr_ctx * const ctx, struct hvgr_mem_pool *mem_pool,
	uint32_t pool_size, uint32_t page_order)
{
	struct hvgr_pool_data data;

	if (ctx == NULL || mem_pool == NULL || pool_size == 0)
		return -EINVAL;

	mem_pool->gdev = ctx->gdev;
	data.alloc = hvgr_mem_pool_dev_alloc;
	data.free = hvgr_mem_pool_dev_free;
	data.callback = hvgr_mem_pool_callback;
	data.grow = NULL;

	return hvgr_mem_pool_init(mem_pool, &data, pool_size, page_order);
}

void hvgr_mem_free_memctx_pool(struct hvgr_ctx *ctx, struct hvgr_mem_pool *mem_pool,
	uint32_t *recycle_pages)
{
	uint32_t i;
	struct page *page = NULL;

	if (unlikely(mem_pool == NULL))
		return;

	if (mem_pool->pool.cur_size == 0)
		return;

	mutex_lock(&mem_pool->pool.pool_lock);
	if (mem_pool->refuse_shrink) {
		mutex_unlock(&mem_pool->pool.pool_lock);
		return;
	}

	for (i = 0; i < mem_pool->pool.cur_size; i++) {
		page = list_first_entry(&mem_pool->pool.list_head, struct page, lru);
		list_del_init(&page->lru);

		/* v300 and beyond no pgid in mem_pool, use pool_id to free */
		hvgr_mem_free_page(mem_pool->gdev->dev, &page, 1u, mem_pool->order,
			mem_pool->pool_id);
	}

	*recycle_pages += mem_pool->pool.cur_size;
	mem_pool->pool.cur_size = 0;
	mutex_unlock(&mem_pool->pool.pool_lock);
}

void hvgr_mem_pool_term(struct hvgr_mem_pool *mem_pool)
{
	struct hvgr_pool *pool = NULL;
	void **array = NULL;
	bool array_is_null = false;
	struct page *page = NULL;
	uint32_t i;

	if (mem_pool == NULL)
		return;

	unregister_shrinker(&mem_pool->sys_shrink);

	pool = &mem_pool->pool;

	if (pool->grow_wq != NULL)
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
		flush_workqueue(pool->grow_wq);
#else
		kthread_flush_worker(pool->grow_wq);
#endif

	if (pool->cur_size != 0) {
		array = vzalloc((pool->cur_size * sizeof(void *)));
		if (array == NULL)
			array_is_null = true;

		mutex_lock(&pool->pool_lock);
		for (i = 0; i < pool->cur_size; i++) {
			page = list_first_entry(&pool->list_head, struct page, lru);
			list_del_init(&page->lru);

			if (array_is_null)
				/* v300 and beyond no pgid in mem_pool, use pool_id to free */
				hvgr_mem_free_page(mem_pool->gdev->dev, &page, 1u, mem_pool->order,
					mem_pool->pool_id);
			else
				array[i] = page;
		}

		pool->cur_size = 0u;
		mutex_unlock(&pool->pool_lock);

		if (!array_is_null) {
			pool->free(pool->priv, i, array, false);
			kvfree(array);
		}
	}

	hvgr_pool_term(&mem_pool->pool);
}

long hvgr_mem_pool_ctx_alloc(struct hvgr_ctx *ctx, uint64_t nums, void **array, uint64_t attr)
{
	long ret;
	struct hvgr_mem_pool *mem_pool = NULL;
#ifdef CONFIG_DFX_OHOS
	unsigned long used_pages;
#endif

	if (ctx == NULL || array == NULL)
		return -EINVAL;

	if (nums == 0)
		return 0;

	mem_pool = hvgr_mem_pool_get_ctx_pool(ctx, attr);
	if (unlikely(mem_pool == NULL))
		return -EINVAL;

	mem_pool->refuse_shrink = true;
	hvgr_systrace_begin("%s %lu", __func__, nums);
	ret = hvgr_pool_alloc(&mem_pool->pool, (uint32_t)nums, array);
	hvgr_systrace_end();
	mem_pool->refuse_shrink = false;

	if (ret == 0) {
		atomic_add((int)nums, &ctx->mem_ctx.used_pages);
		atomic_add((int)nums, &ctx->gdev->mem_dev.used_pages);
		if ((attr & HVGR_MEM_ATTR_KMD_ACCESS) != 0)
			atomic_add((int)nums, &ctx->mem_ctx.used_kmd_pages);
#ifdef CONFIG_DFX_OHOS
		used_pages = (unsigned long)atomic_read(&ctx->mem_ctx.used_pages);
		report_gpu_quick_leak_event(used_pages, ctx);
#endif
	}

	hvgr_info(ctx->gdev, HVGR_MEM, "ctx_%u page alloc pages %u ret=%d attr=0x%lx",
		ctx->id, nums, ret, attr);
	return ret;
}


void hvgr_mem_pool_ctx_free(struct hvgr_ctx *ctx, uint64_t nums, void **array, uint32_t flag,
	uint64_t attr)
{
	struct hvgr_mem_pool *mem_pool = NULL;
	uint64_t idx;
	uint64_t sync_num;
	dma_addr_t dma_addr;
	struct page *page = NULL;

	if (ctx == NULL || nums == 0 || array == NULL)
		return;

	hvgr_info(ctx->gdev, HVGR_MEM, "ctx_%u page free pages %u attr 0x%lx", ctx->id, nums, attr);
	mem_pool = hvgr_mem_pool_get_ctx_pool(ctx, attr);
	if (unlikely(mem_pool == NULL))
		return;

	if ((flag & HVGR_PAGE_SYNC) != 0) {
		sync_num = min(nums, HVGR_MEM_POOL_CTX_SIZE);
		for (idx = 0; idx < sync_num; idx++) {
			page = (struct page *)array[idx];
			if (unlikely(page == NULL)) {
				hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u page arr %lu/%lu is null",
					ctx->id, idx, nums);
				continue;
			}

			dma_addr = hvgr_mem_get_page_dma_addr(page);
			dma_sync_single_for_device(ctx->gdev->dev, dma_addr, PAGE_SIZE,
				DMA_BIDIRECTIONAL);
		}
	}

	hvgr_systrace_begin("%s %lu", __func__, nums);
	mem_pool->refuse_shrink = true;
	hvgr_pool_free(&mem_pool->pool, (uint32_t)nums, array, 0U);
	/* clear page array */
	for (idx = 0; idx < nums; idx++)
		array[idx] = NULL;
	mem_pool->refuse_shrink = false;
	hvgr_systrace_end();

	atomic_sub((int)nums, &ctx->mem_ctx.used_pages);
	atomic_sub((int)nums, &ctx->gdev->mem_dev.used_pages);
	if ((attr & HVGR_MEM_ATTR_KMD_ACCESS) != 0)
		atomic_sub((int)nums, &ctx->mem_ctx.used_kmd_pages);
}

struct page *hvgr_mem_pool_ctx_alloc_pt(struct hvgr_ctx *ctx)
{
	long ret;
	struct page *pages = NULL;
	struct hvgr_mem_pool *mem_pool = NULL;

	if (mem_pool == NULL)
		mem_pool = &ctx->mem_ctx.mem_pool[0];

	mem_pool->refuse_shrink = true;
	ret = hvgr_pool_alloc_direct(&mem_pool->pool, 1, (void **)&pages);
	mem_pool->refuse_shrink = false;
	if (ret != 0)
		return NULL;

	atomic_add(1, &ctx->mem_ctx.used_pages);
	atomic_add(1, &ctx->gdev->mem_dev.used_pages);
	atomic_add(1, &ctx->mem_ctx.used_pt_pages);

	return pages;
}

void hvgr_mem_pool_ctx_free_pt(struct hvgr_ctx * const ctx, struct page *pages)
{
	struct hvgr_mem_pool *mem_pool = NULL;
	dma_addr_t dma_addr;

	if (mem_pool == NULL)
		mem_pool = &ctx->mem_ctx.mem_pool[0];

	dma_addr = hvgr_mem_get_page_dma_addr(pages);
	dma_sync_single_for_device(ctx->gdev->dev, dma_addr, PAGE_SIZE,
		DMA_BIDIRECTIONAL);

	mem_pool->refuse_shrink = true;
	hvgr_pool_free(&mem_pool->pool, 1, (void **)&pages, 0U);
	mem_pool->refuse_shrink = false;

	atomic_sub(1, &ctx->mem_ctx.used_pages);
	atomic_sub(1, &ctx->gdev->mem_dev.used_pages);
	atomic_sub(1, &ctx->mem_ctx.used_pt_pages);
}

long hvgr_mem_pool_ctx_grow_pt(struct hvgr_ctx * const ctx, uint32_t nums, struct page **pages)
{
	struct hvgr_mem_pool *mem_pool = NULL;
	long ret;
	uint32_t grow_nums;

	if (mem_pool == NULL)
		mem_pool = &ctx->mem_ctx.mem_pool[0];

	ret = hvgr_mem_alloc_page(ctx->gdev->dev, pages, nums, mem_pool->order,
		mem_pool->pool_id, mem_pool->flag);
	if (ret != 0) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u grow pt pool alloc page fail",
			ctx->id);
		return -ENOMEM;
	}

	mem_pool->refuse_shrink = true;
	grow_nums = hvgr_pool_grow(&mem_pool->pool, nums, (void **)pages);
	mem_pool->refuse_shrink = false;
	if (grow_nums == nums)
		return 0;

	hvgr_mem_free_page(ctx->gdev->dev, (pages + grow_nums), (nums - grow_nums),
		mem_pool->order, mem_pool->pool_id);
	return 0;
}

static void hvgr_mem_pool_free_pages(struct hvgr_ctx *ctx, struct page **pages,
	struct hvgr_mem_area * const area)
{
	uint64_t i;
	dma_addr_t dma_addr;

	for (i = 0u; i < area->pages; i++) {
		if (pages[i] == NULL)
			break;
		dma_addr = hvgr_mem_get_page_dma_addr(pages[i]);
		dma_unmap_page(ctx->gdev->dev, dma_addr, PAGE_SIZE, DMA_BIDIRECTIONAL);

		__free_pages(pages[i], 0);
		pages[i] = NULL;
	}
}

struct page **hvgr_mem_alloc_page_array(struct hvgr_ctx *ctx, struct hvgr_mem_area * const area)
{
	struct page **pages = NULL;
	long ret;
	uint32_t policy_id = 0;
	uint32_t flag = hvgr_get_pool_index(area->attribute);
#ifdef CONFIG_DFX_OHOS
	unsigned long used_pages;
#endif

	if (area->max_pages > (U64_MAX / sizeof(struct page *))) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u alloc page array invalid para",
			ctx->id);
		return NULL;
	}

	if (area->max_pages <= MEM_PHY_ALLOC_THRESHOLD)
		pages = kcalloc(area->max_pages, sizeof(struct page *), GFP_KERNEL);
	else
		pages = vzalloc((area->max_pages * sizeof(struct page *)));
	if (pages == NULL) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u alloc page array fail 0x%lx",
			ctx->id, area->max_pages);
		return NULL;
	}

	/*
	 * order 0: alloc discontinuous pages from pool.
	 * other order we need to alloc continuous pages from kernel.
	 */
	if (area->order == 0) {
		ret = hvgr_mem_pool_ctx_alloc(ctx, area->pages, (void **)pages,
			area->attribute);
		if (ret != 0) {
			hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u alloc %lu pages fail",
				ctx->id, area->pages);
			hvgr_mem_pool_free_pages(ctx, pages, area);
		}
	} else {
		ret = hvgr_mem_alloc_page(ctx->gdev->dev, pages, (uint32_t)area->pages, area->order,
			policy_id, flag);
		if (ret == 0) {
			atomic_add((int)area->pages, &ctx->mem_ctx.used_pages);
			atomic_add((int)area->pages, &ctx->gdev->mem_dev.used_pages);
			if ((area->attribute & HVGR_MEM_ATTR_KMD_ACCESS) != 0)
				atomic_add((int)area->pages, &ctx->mem_ctx.used_kmd_pages);
		}
	}

	if (ret != 0) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u alloc page fail %d", ctx->id, ret);
		kvfree(pages);
		pages = NULL;
	}

	return pages;
}

void hvgr_mem_free_page_array(struct hvgr_ctx *ctx, struct hvgr_mem_area * const area)
{
	uint32_t policy_id = 0;
	int growable_pages;

	if (area->page_array == NULL)
		return;

	if (area->pages == 0)
		goto free_page_array;

	/*
	 * order 0: free discontinuous pages to pool.
	 * other order we need to free continuous pages to kernel.
	 */
	if (area->order == 0) {
		hvgr_mem_pool_ctx_free(ctx, area->pages, (void **)area->page_array,
			(((area->attribute & HVGR_MEM_ATTR_CPU_CACHEABLE) != 0 ?
				HVGR_PAGE_SYNC : 0)), area->attribute);
	} else {
		hvgr_mem_free_page(ctx->gdev->dev, area->page_array, (uint32_t)area->pages,
			area->order, policy_id);
		atomic_sub((int)area->pages, &ctx->mem_ctx.used_pages);
		atomic_sub((int)area->pages, &ctx->gdev->mem_dev.used_pages);
		if ((area->attribute & HVGR_MEM_ATTR_KMD_ACCESS) != 0)
			atomic_sub((int)area->pages, &ctx->mem_ctx.used_kmd_pages);
	}

	if (area->property != HVGR_MEM_PROPERTY_JIT) {
		growable_pages = atomic_read(&(area->growable_pages));
		if (growable_pages != 0)
			atomic_sub(growable_pages, &ctx->mem_ctx.used_grow_pages);
	}

free_page_array:
	kvfree(area->page_array);
	area->page_array = NULL;
}

void hvgr_dev_deinit_mem_pool(struct hvgr_device *gdev, uint32_t num)
{
	uint32_t index;

	for (index = 0; index < num; index++)
		hvgr_mem_pool_term(&gdev->mem_dev.mem_pool[index]);
}

static void hvgr_dev_mem_pool_cfg(struct hvgr_device *gdev, uint32_t index, uint32_t *size)
{
	struct hvgr_mem_pool *mem_pool = &gdev->mem_dev.mem_pool[index];
	bool grow_prefill = false;

#if defined(CONFIG_LIBLINUX) && !defined(CONFIG_LIBLINUX_CDC)
#if hvgr_version_lt(HVGR_V300)
	grow_prefill = true;
#else
	grow_prefill = true;
#endif
#endif
	if (grow_prefill) {
		mem_pool->growable = true;
		mem_pool->prefill = true;
		*size = HVGR_MEM_POOL_OHOS_DEV_SIZE;
	}
}

long hvgr_dev_init_mem_pool(struct hvgr_device *gdev)
{
	uint32_t index;
	long ret;
	uint32_t pool_size = HVGR_MEM_POOL_DEV_SIZE;

	for (index = 0; index < MEM_POOL_NUM; index++) {
		hvgr_dev_mem_pool_cfg(gdev, index, &pool_size);

		ret = hvgr_mem_pool_dev_init(gdev, &gdev->mem_dev.mem_pool[index],
			pool_size, 0);
		if (ret != 0) {
			hvgr_dev_deinit_mem_pool(gdev, index);
			hvgr_err(gdev, HVGR_MEM, "%s init fail", __func__);
			return -1;
		}

		gdev->mem_dev.mem_pool[index].flag = index;
	}

	return 0;
}

void hvgr_ctx_deinit_mem_pool(struct hvgr_ctx *ctx, uint32_t num)
{
	uint32_t index;

	for (index = 0; index < num; index++)
		hvgr_mem_pool_term(&ctx->mem_ctx.mem_pool[index]);
}

long hvgr_ctx_init_mem_pool(struct hvgr_ctx *ctx)
{
	uint32_t index;
	long ret;

	for (index = 0; index < MEM_POOL_NUM; index++) {
		ret = hvgr_mem_pool_ctx_init(ctx, &ctx->mem_ctx.mem_pool[index],
			HVGR_MEM_POOL_CTX_SIZE, 0);
		if (ret != 0) {
			hvgr_ctx_deinit_mem_pool(ctx, index);
			hvgr_err(ctx->gdev, HVGR_MEM, "%s init fail", __func__);
			return -1;
		}

		ctx->mem_ctx.mem_pool[index].flag = index;
	}

	return 0;
}

