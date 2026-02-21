/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2023-2023. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include <linux/slab.h>
#include <linux/kthread.h>
#include <linux/dma-mapping.h>
#include <linux/dma-direct.h>
#include <linux/io.h>
#include <linux/page-flags.h>
#include <linux/compiler.h>

#include "hvgr_mem_api.h"
#include "hvgr_dm_api.h"
#include "hvgr_log_api.h"
#include "hvgr_memory_page.h"

#ifdef CONFIG_HVGR_ASYNC_MEM_RECYCLE


#define ASYNC_MEM_RECYCLE_THRESHOLD 64

static bool hvgr_mem_page_is_fine(struct hvgr_device *gdev, struct page *page)
{
	dma_addr_t dma_addr;
	phys_addr_t phy_addr;
	unsigned long long pfn;

	if (likely(test_bit(PG_hvgr, &page->flags))) {
		clear_bit(PG_hvgr, &page->flags);
		return true;
	}

	dma_addr = hvgr_mem_get_page_dma_addr(page);
	phy_addr = dma_to_phys(gdev->dev, dma_addr);
	pfn = phys_to_pfn(phy_addr);

	hvgr_err(gdev, HVGR_MEM, "page err 0x%llx %d %d %d",
		page->flags, (int)pfn_valid(pfn), (int)(&page->lru == page->lru.next ? 1 : 0),
		(int)(&page->lru == page->lru.prev ? 1 : 0));
	return false;
}

void hvgr_mem_free_page_sync(struct device *dev, struct page *page,
	uint32_t order, uint32_t policy_id)
{
	dma_addr_t dma_addr;
	struct hvgr_device *gdev = hvgr_get_device();

	if (gdev == NULL)
		return;

	if (unlikely(!hvgr_mem_page_is_fine(gdev, page))) {
		hvgr_err(gdev, HVGR_MEM, "free page %u %u err, skip it", order, policy_id);
		WARN_ON(true);
		return;
	}

	dma_addr = hvgr_mem_get_page_dma_addr(page);
	dma_unmap_page(dev, dma_addr, (PAGE_SIZE << order), DMA_BIDIRECTIONAL);
	__free_pages(page, order);
	gdev->mem_dev.statics.free_pages += 1 << order;
}

void hvgr_mem_free_page(struct device *dev, struct page **pages, uint32_t nums,
	uint32_t order, uint32_t policy_id)
{
	dma_addr_t dma_addr;
	uint32_t steps = nums >> order;
	uint32_t step;
	struct page *page = NULL;
	struct hvgr_device *gdev = hvgr_get_device();

	if (gdev == NULL)
		return;

	if (steps >= ASYNC_MEM_RECYCLE_THRESHOLD) {
		spin_lock(&gdev->mem_dev.recycle_lock);
		for (step = 0; step < steps; step++) {
			page = pages[step << order];
			pages[step << order] = NULL;
			page->index = order;
			list_add_tail(&page->lru, &gdev->mem_dev.recycle_list);
		}
		spin_unlock(&gdev->mem_dev.recycle_lock);
		wake_up(&gdev->mem_dev.recycle_wq);
		return;
	}

	for (step = 0; step < steps; step++) {
		page = pages[step << order];
		pages[step << order] = NULL;
		hvgr_mem_free_page_sync(dev, page, order, policy_id);
	}
}

static int hvgr_mem_recycle(struct hvgr_device * const gdev, struct list_head *list)
{
	spin_lock(&gdev->mem_dev.recycle_lock);
	list_splice_tail_init(&gdev->mem_dev.recycle_list, list);
	spin_unlock(&gdev->mem_dev.recycle_lock);

	return !list_empty(list);
}

static int hvgr_mem_recycle_task(void *data)
{
	struct hvgr_device * const gdev = data;
	struct page *page = NULL;
	uint32_t order, policy_id;
	dma_addr_t addr;
	LIST_HEAD(list);

	while (!kthread_should_stop()) {
		wait_event(gdev->mem_dev.recycle_wq,
			hvgr_mem_recycle(gdev, &list) || kthread_should_stop());

		while (!list_empty(&list)) {
			page = list_first_entry(&list, struct page, lru);
			list_del_init(&page->lru);
			order = (uint32_t)page->index;
			hvgr_mem_free_page_sync(gdev->dev, page, order, 0);
		}
	}

	return 0;
}

int hvgr_mem_recycle_init(struct hvgr_device * const gdev)
{
	spin_lock_init(&gdev->mem_dev.recycle_lock);
	INIT_LIST_HEAD(&gdev->mem_dev.recycle_list);
	init_waitqueue_head(&gdev->mem_dev.recycle_wq);

	gdev->mem_dev.recycle_task = kthread_run(hvgr_mem_recycle_task, (void *)gdev,
		"hvgr-recycle");
	if (IS_ERR(gdev->mem_dev.recycle_task)) {
		hvgr_err(gdev, HVGR_MEM, "failed to create recycle kthread");
		gdev->mem_dev.recycle_task = NULL;
		return -1;
	}

	return 0;
}

void hvgr_mem_recycle_term(struct hvgr_device * const gdev)
{
	if (gdev->mem_dev.recycle_task != NULL) {
		kthread_stop(gdev->mem_dev.recycle_task);
		gdev->mem_dev.recycle_task = NULL;
	}
}

#endif /* CONFIG_HVGR_ASYNC_MEM_RECYCLE */
