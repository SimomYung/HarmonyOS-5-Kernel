/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */
#include "hvgr_mem_2m.h"

#include <linux/highmem.h>
#include <linux/mman.h>
#include <linux/vmalloc.h>
#include <linux/rwsem.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/rcupdate.h>

#include "hvgr_kmd_defs.h"
#include "hvgr_ioctl_mem.h"
#include "hvgr_mem_api.h"
#include "hvgr_mem_zone.h"
#include "hvgr_kv.h"
#include "hvgr_mem_space.h"
#include "hvgr_memory_page.h"
#include "hvgr_mmu_api.h"
#include "hvgr_log_api.h"

#define HVGR_2M_PAGE_MASK 0x1FF

static bool hvgr_mem_2m_check_para(struct hvgr_mem_area * const area)
{
	struct hvgr_ctx *ctx = NULL;

	if (unlikely((area == NULL) || (area->zone == NULL)))
		return false;

	if (((area->attribute & (~(HVGR_MEM_ATTR_GPU_RD | HVGR_MEM_ATTR_GPU_WR |
		HVGR_MEM_ATTR_CPU_WR | HVGR_MEM_ATTR_CPU_RD))) != 0) ||
		((area->pages & HVGR_2M_PAGE_MASK) != 0)) {
		ctx = container_of(area->zone->pmem, struct hvgr_ctx, mem_ctx);
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u 2M para invalid attr:0x%x pages:%lu/%lu",
			ctx->id, area->attribute, area->pages, area->max_pages);
		return false;
	}

	area->max_pages = area->pages;
	return true;
}

static long hvgr_mem_2m_alloc(struct hvgr_mem_area * const area)
{
	uint64_t size;
	struct hvgr_ctx *ctx = NULL;

	ctx = container_of(area->zone->pmem, struct hvgr_ctx, mem_ctx);

	size = area->max_pages << PAGE_SHIFT;
	area->gva = hvgr_mem_zone_alloc_gva(area->zone->pmem, ZONE_TYPE_2M, 0U, size);
	if (area->gva == 0) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u 2M alloc va FAIL. %lu/%lu",
			ctx->id, area->pages, area->max_pages);
		return -ENOMEM;
	}

	area->flags |= HVGR_MEM_AREA_FLAG_2M;
	hvgr_mem_attr2flag(area);

	if (!hvgr_mem_map_gva(NULL, area)) {
		hvgr_mem_zone_free_gva(area->zone->pmem, ZONE_TYPE_2M, area->gva, size);
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u 2M map gva FAIL. 0x%lx %lu/%lu",
			ctx->id, area->gva, area->pages, area->max_pages);
		return -EFAULT;
	}

	return 0;
}

static long hvgr_mem_2m_free(struct hvgr_mem_area * const area)
{
	uint64_t addr = area->gva;
	uint64_t size = area->max_pages << PAGE_SHIFT;

	hvgr_mem_unmap_gva(NULL, area);
	hvgr_mem_zone_free_gva(area->zone->pmem, ZONE_TYPE_2M, addr, size);
	hvgr_mem_area_ref_put(area);

	return 0;
}

static const struct hvgr_zone_opts g_mem_zone_2m = {
	.init = hvgr_mem_opt_init,
	.term = hvgr_mem_opt_term,
	.check_para = hvgr_mem_2m_check_para,
	.alloc = hvgr_mem_2m_alloc,
	.free = hvgr_mem_2m_free,
};

bool hvgr_mem_2m_config(struct hvgr_mem_ctx * const pmem)
{
	if (unlikely(pmem == NULL))
		return false;

	return hvgr_mem_zone_create(pmem, HVGR_MEM_PROPERTY_2M_PAGE,
		&g_mem_zone_2m, NULL);
}

