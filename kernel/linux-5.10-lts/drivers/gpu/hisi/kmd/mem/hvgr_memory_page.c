/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include "hvgr_memory_page.h"

#include <linux/slab.h>
#include <linux/dma-mapping.h>

#include "hvgr_mem_api.h"
#include "hvgr_dm_api.h"
#include "hvgr_log_api.h"
#include "hvgr_platform_api.h"

#ifndef CONFIG_HVGR_ASYNC_MEM_RECYCLE
void hvgr_mem_free_page(struct device *dev, struct page **pages, uint32_t nums,
	uint32_t order, uint32_t policy_id)
{
	dma_addr_t dma_addr;
	uint32_t steps = nums >> order;
	uint32_t step;
	struct page *page = NULL;
	struct hvgr_device *gdev = hvgr_get_device();

	for (step = 0; step < steps; step++) {
		page = pages[step << order];
		pages[step << order] = NULL;
		dma_addr = hvgr_mem_get_page_dma_addr(page);
		dma_unmap_page(dev, dma_addr, (PAGE_SIZE << order), DMA_BIDIRECTIONAL);
		__free_pages(page, order);
	}

	if (gdev != NULL) {
			gdev->mem_dev.statics.free_pages += nums;
	}
}
#endif

long hvgr_mem_alloc_page(struct device *dev, struct page **pages,
	uint32_t nums, uint32_t order, uint32_t policy_id, uint32_t flag)
{
	uint32_t step;
	uint32_t i;
	dma_addr_t dma_addr;
	gfp_t gfp = GFP_HIGHUSER | __GFP_ZERO;
	struct page *page = NULL;
	uint32_t steps = nums >> order;
	struct hvgr_device *gdev = hvgr_get_device();

	hvgr_systrace_begin("%s %u:%u:%u", __func__, nums, order, policy_id);

	if (order != 0)
		gfp |= __GFP_NOWARN;

	for (step = 0; step < steps; step++) {
		page = hvgr_platform_alloc_page(gfp, order, policy_id, flag);
		if (page == NULL) {
			hvgr_mem_free_page(dev, pages, (step << order), order, policy_id);
			hvgr_err(gdev, HVGR_MEM, "alloc step %u steps %u order %u nums %u fail",
				step, steps, order, nums);
			hvgr_systrace_end();
			return -ENOMEM;
		}

		dma_addr = dma_map_page(dev, page, 0, (PAGE_SIZE << order), DMA_BIDIRECTIONAL);
		if (dma_mapping_error(dev, dma_addr)) {
			__free_pages(page, order);
			hvgr_mem_free_page(dev, pages, (step << order), order, policy_id);
			hvgr_err(gdev, HVGR_MEM, "dma map step %u steps %u order %u nums %u fail",
				step, steps, order, nums);
			hvgr_systrace_end();
			return -EFAULT;
		}

#ifdef CONFIG_HVGR_ASYNC_MEM_RECYCLE
		set_bit(PG_hvgr, &page->flags);
#endif
		WARN_ON(dma_addr != page_to_phys(page));
		for (i = 0; i < (1u << order); i++)
			pages[(step << order) + i] = page + i;
	}

		gdev->mem_dev.statics.alloc_pages += nums;

	if ((flag & 0x1U) == 0U)
		gdev->mem_dev.statics.ama_alloc_big += nums;
	else
		gdev->mem_dev.statics.ama_alloc_low += nums;

	hvgr_systrace_end();
	return 0;
}

unsigned long hvgr_mem_statics_used_pages(void)
{
	struct hvgr_device *dev = hvgr_get_device();

	if (dev == NULL)
		return 0;

	return atomic_read(&(dev->mem_dev.used_pages));
}