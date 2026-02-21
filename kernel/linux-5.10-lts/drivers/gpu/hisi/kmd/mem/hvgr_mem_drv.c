/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include "hvgr_mem_drv.h"

#include "hvgr_mem_api.h"
#include "hvgr_mem_zone.h"
#include "hvgr_log_api.h"

static bool hvgr_mem_driver_check_para(struct hvgr_mem_area * const area)
{
	struct hvgr_ctx *ctx = NULL;

	if (unlikely((area == NULL) || (area->zone == NULL)))
		return false;

	if (((~(HVGR_MEM_ATTR_CPU_RD | HVGR_MEM_ATTR_CPU_WR | HVGR_MEM_ATTR_KMD_ACCESS)) &
		area->attribute) != 0) {
		ctx = container_of(area->zone->pmem, struct hvgr_ctx, mem_ctx);
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u drv attribute INVALID. 0x%lx",
			ctx->id, area->attribute);
		return false;
	}

	area->max_pages = area->pages;

	return true;
}

static long hvgr_mem_driver_alloc(struct hvgr_mem_area * const area)
{
	return 0;
}

static long hvgr_mem_driver_free(struct hvgr_mem_area * const area)
{
	hvgr_mem_unmap_kva(area);
	hvgr_mem_area_ref_put(area);
	return 0;
}

static const struct hvgr_zone_opts g_mem_zone_driver = {
	.init = hvgr_mem_opt_init,
	.term = hvgr_mem_opt_term,
	.check_para = hvgr_mem_driver_check_para,
	.alloc = hvgr_mem_driver_alloc,
	.free = hvgr_mem_driver_free,
};

bool hvgr_mem_driver_config(struct hvgr_mem_ctx * const pmem)
{
	if (unlikely(pmem == NULL))
		return false;

	return hvgr_mem_zone_create(pmem, HVGR_MEM_PROPERTY_DRIVER,
		&g_mem_zone_driver, NULL);
}
