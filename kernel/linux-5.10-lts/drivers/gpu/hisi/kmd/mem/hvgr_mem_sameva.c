/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include "hvgr_mem_sameva.h"

#include "hvgr_mem_zone.h"
#include "hvgr_mem_api.h"
#include "hvgr_log_api.h"

static bool hvgr_mem_sameva_check_para(struct hvgr_mem_area * const area)
{
	struct hvgr_ctx *ctx = NULL;

	if (unlikely((area == NULL) || (area->zone == NULL)))
		return false;

	ctx = container_of(area->zone->pmem, struct hvgr_ctx, mem_ctx);
	if (((area->attribute & HVGR_MEM_MASK_GPU_ACCESS) == 0) ||
		((area->attribute & HVGR_MEM_ATTR_MEM_SPARSE) != 0)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u sameva attribute INVALID. 0x%lx",
			ctx->id, area->attribute);
		return false;
	}

	return true;
}

static long hvgr_mem_sameva_alloc(struct hvgr_mem_area * const area)
{
	hvgr_mem_attr2flag(area);
	return 0;
}

long __attribute__((weak)) hvgr_mem_sameva_free(struct hvgr_mem_area * const area)
{
	/* when process exiting, the unfreed same va come here */
	hvgr_mem_unmap_gva(NULL, area);
	hvgr_mem_area_ref_put(area);

	return 0;
}

static const struct hvgr_zone_opts g_mem_zone_sameva = {
	.init = hvgr_mem_opt_init,
	.term = hvgr_mem_opt_term,
	.check_para = hvgr_mem_sameva_check_para,
	.alloc = hvgr_mem_sameva_alloc,
	.free = hvgr_mem_sameva_free,
};

bool hvgr_mem_sameva_config(struct hvgr_mem_ctx * const pmem)
{
	if (pmem == NULL)
		return false;

	return hvgr_mem_zone_create(pmem, HVGR_MEM_PROPERTY_SAME_VA,
		&g_mem_zone_sameva, NULL);
}
