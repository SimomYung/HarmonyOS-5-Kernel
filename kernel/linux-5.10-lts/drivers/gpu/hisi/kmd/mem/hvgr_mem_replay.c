/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */
#include "hvgr_mem_replay.h"

#include "hvgr_mem_zone.h"
#include "hvgr_mem_opts.h"
#include "hvgr_assert.h"
#include "hvgr_log_api.h"

static bool hvgr_mem_replay_check_para(struct hvgr_mem_area * const area)
{
	struct hvgr_ctx *ctx = NULL;

	if (unlikely(area == NULL || area->zone == NULL))
		return false;

	if (((~(HVGR_MEM_ATTR_GPU_RD | HVGR_MEM_ATTR_GPU_WR | HVGR_MEM_ATTR_GPU_CACHEABLE |
		HVGR_MEM_ATTR_CPU_RD | HVGR_MEM_ATTR_CPU_WR)) &
		area->attribute) != 0) {
		ctx = container_of(area->zone->pmem, struct hvgr_ctx, mem_ctx);
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u replay memory attribute INVALID. 0x%lx",
			ctx->id, area->attribute);
		return false;
	}

	area->max_pages = area->pages;

	return true;
}

static long hvgr_mem_replay_alloc(struct hvgr_mem_area * const area)
{
	uint64_t size;
	struct hvgr_ctx *ctx = container_of(area->zone->pmem, struct hvgr_ctx, mem_ctx);

	size = area->max_pages << PAGE_SHIFT;
	area->gva = hvgr_mem_zone_alloc_gva(area->zone->pmem, ZONE_TYPE_REPLAY, area->gva, size);
	if (area->gva == 0) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u replay alloc va FAIL. 0x%lx:%lu:%lu",
			ctx->id, area->gva, area->pages, area->max_pages);
		return -ENOMEM;
	}

	hvgr_mem_attr2flag(area);

	if (hvgr_mem_map_gva(NULL, area) != true) {
		hvgr_mem_zone_free_gva(area->zone->pmem, ZONE_TYPE_REPLAY, area->gva, size);
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u replay map gva fail, 0x%lx:%lu:%lu",
			ctx->id, area->gva, area->pages, area->max_pages);
		return -EFAULT;
	}

	return 0;
}

static long hvgr_mem_replay_free(struct hvgr_mem_area * const area)
{
	uint64_t size = area->max_pages << PAGE_SHIFT;

	hvgr_mem_unmap_gva(NULL, area);
	hvgr_mem_zone_free_gva(area->zone->pmem, ZONE_TYPE_REPLAY, area->gva, size);
	hvgr_mem_area_ref_put(area);

	return 0;
}

static const struct hvgr_zone_opts g_mem_zone_replay = {
	.init = hvgr_mem_opt_init,
	.term = hvgr_mem_opt_term,
	.check_para = hvgr_mem_replay_check_para,
	.alloc = hvgr_mem_replay_alloc,
	.free = hvgr_mem_replay_free,
};

bool hvgr_mem_replay_config(struct hvgr_mem_ctx * const pmem,
	struct hvgr_mem_cfg_para *para)
{
	if (unlikely(pmem == NULL || para == NULL))
		return false;

	/* replay mem is not enabled */
	if (para->mem_replay_size == 0)
		return true;

	return hvgr_mem_zone_create(pmem, HVGR_MEM_PROPERTY_REPLAY,
		&g_mem_zone_replay, (void *)para);
}

