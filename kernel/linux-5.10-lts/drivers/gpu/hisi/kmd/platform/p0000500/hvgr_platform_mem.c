/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2022-2022. All rights reserved.
 */

#include "hvgr_platform_api.h"

#include <linux/types.h>
#include <linux/mm_types.h>
#include <linux/dma-buf.h>
#include <securec.h>

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
#include <linux/hisi/mm_lb.h>
#else
#include <linux/mm_lb/mm_lb.h>
#endif

#include "hvgr_defs.h"
#include "hvgr_ioctl_mem.h"
#include "hvgr_dm_api.h"
#include "hvgr_log_api.h"

typedef struct page *(*lb_alloc_func)(uint32_t policy_id, gfp_t gfp, uint32_t order);
typedef struct page *(*ama_alloc_func)(gfp_t gfp, uint32_t order);


static struct page *hvgr_platform_alloc_normal_page(gfp_t gfp, uint32_t order, uint32_t flag)
{
	struct page *pages = NULL;

	pages = alloc_pages(gfp, order);
	return pages;
}

struct page *hvgr_platform_alloc_page(gfp_t gfp, uint32_t order, uint32_t policy_id, uint32_t flag)
{
	struct page *pages = NULL;

	if (policy_id != 0) {
		pages = hvgr_platform_alloc_normal_page(gfp, order, flag);
	} else {
		pages = hvgr_platform_alloc_normal_page(gfp, order, flag);
	}

	return pages;
}

void hvgr_platform_mem_import_osh(struct dma_buf *dmabuf, struct hvgr_mem_area *area)
{
	struct hvgr_device *gdev = hvgr_get_device();

	if (!dma_buf_is_osh(dmabuf))
		return;

	if (unlikely((area->attribute & HVGR_MEM_ATTR_SECURE) != 0)) {
		hvgr_err(gdev, HVGR_MEM, "area 0x%llx %llu:0x%llx:%llu:%llu has secure cant be osh",
			area->gva, area->property, area->attribute, area->pages, area->max_pages);
		return;
	}

	area->attribute |= HVGR_MEM_ATTR_COHERENT_SYSTEM;
}

