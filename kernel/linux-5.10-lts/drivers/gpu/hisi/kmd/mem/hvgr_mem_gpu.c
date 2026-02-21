/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include "hvgr_mem_gpu.h"

#include <linux/slab.h>

#include "hvgr_mem_zone.h"
#include "hvgr_mem_api.h"
#include "hvgr_kv.h"
#include "hvgr_mem_space.h"
#include "hvgr_log_api.h"

static bool hvgr_mem_gpu_check_para(struct hvgr_mem_area * const area)
{
	struct hvgr_ctx *ctx = NULL;

	if (unlikely((area == NULL) || (area->zone == NULL)))
		return false;

	if (((area->attribute & HVGR_MEM_MASK_KMD_ACCESS) != 0) ||
		((area->attribute & HVGR_MEM_MASK_GPU_ACCESS) == 0) ||
		((area->attribute & HVGR_MEM_ATTR_MEM_SPARSE) != 0) ||
		((area->attribute & HVGR_MEM_ATTR_GPU_EXEC) != 0) ||
		((area->attribute & HVGR_MEM_MASK_COHERENT) != 0)) {
		ctx = container_of(area->zone->pmem, struct hvgr_ctx, mem_ctx);
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u gpu memory attribute INVALID. 0x%lx",
			ctx->id, area->attribute);
		return false;
	}

	return true;
}

static long hvgr_mem_gpu_alloc(struct hvgr_mem_area * const area)
{
	struct hvgr_ctx *ctx = container_of(area->zone->pmem, struct hvgr_ctx, mem_ctx);
	uint64_t va_size = (area->max_pages << PAGE_SHIFT);

	area->gva = hvgr_mem_zone_alloc_gva(area->zone->pmem, ZONE_TYPE_GPU, 0U, va_size);
	if (area->gva == 0)
		return -ENOMEM;

	hvgr_mem_attr2flag(area);

	if (!hvgr_mem_map_gva(NULL, area)) {
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u gpu alloc map gva FAIL. 0x%lx/%lu/%lu",
			ctx->id, area->gva, area->pages, area->max_pages);
		hvgr_mem_zone_free_gva(area->zone->pmem, ZONE_TYPE_GPU, area->gva, va_size);
		return -EFAULT;
	}

	area->uva = area->gva;
	return 0;
}

static long hvgr_mem_gpu_free(struct hvgr_mem_area * const area)
{
	hvgr_mem_unmap_gva(NULL, area);
	hvgr_mem_zone_free_gva(area->zone->pmem, ZONE_TYPE_GPU, area->gva,
		(area->max_pages << PAGE_SHIFT));
	hvgr_mem_area_ref_put(area);

	return 0;
}

static const struct hvgr_zone_opts g_mem_zone_gpu = {
	.init = hvgr_mem_opt_init,
	.term = hvgr_mem_opt_term,
	.check_para = hvgr_mem_gpu_check_para,
	.alloc = hvgr_mem_gpu_alloc,
	.free = hvgr_mem_gpu_free,
};

bool hvgr_mem_gpu_config(struct hvgr_mem_ctx * const pmem)
{
	if (unlikely(pmem == NULL))
		return false;

	return hvgr_mem_zone_create(pmem, HVGR_MEM_PROPERTY_GPU_MEM,
		&g_mem_zone_gpu, NULL);
}

int hvgr_gpu_mem_change_attr(struct hvgr_ctx *ctx,
	struct hvgr_mem_area *area, uint64_t attr)
{
	uint64_t temp_attr;

	if (unlikely(ctx == NULL || area == NULL || attr == 0))
		return -ENOENT;

	/* nothing to do */
	if (area->attribute == attr)
		return 0;

	if ((area->attribute & (~HVGR_MEM_ATTR_GPU_CACHEABLE)) !=
		(attr & (~HVGR_MEM_ATTR_GPU_CACHEABLE))) {
		hvgr_err(ctx->gdev, HVGR_MEM, "gpu mem attribute change to much");
		return -EFAULT;
	}

#ifdef CONFIG_HVGR_MMU_HM
	hvgr_mem_unmap_by_gva(ctx, area->gva, area->pages, area);
#endif
	temp_attr = area->attribute;
	area->attribute = attr;

	hvgr_mem_attr2flag(area);
	if (!hvgr_mem_map_gva(NULL, area)) {
		area->attribute = temp_attr;
		hvgr_err(ctx->gdev, HVGR_MEM, "ctx_%u gpu mem attribute change map fail",
			ctx->id);
		return -EFAULT;
	}

	return 0;
}
