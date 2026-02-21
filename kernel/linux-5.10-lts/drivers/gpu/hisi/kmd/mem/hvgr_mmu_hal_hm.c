/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */
#include <linux/kernel.h>

#include "hvgr_defs.h"
#include "dm/fcp/hvgr_fcp.h"
#include "hvgr_mmu_driver.h"
#include "hvgr_log_api.h"
#include "hvgr_memory_data.h"

long hvgr_mmu_hal_unlock(struct hvgr_ctx *ctx, uint32_t asid)
{
	return (long)liblinux_ext_hvgr_mmu_hal_unlock(asid);
}

long hvgr_mmu_hal_enable(struct hvgr_ctx *ctx)
{
	uint64_t pgd = ptr_to_u64(ctx->mem_ctx.pgd_pa);

	return (long)liblinux_ext_hvgr_mmu_hal_enable(pgd, ctx->asid);
}

long hvgr_mmu_hal_flush_pt(struct hvgr_ctx *ctx, uint64_t gva, uint32_t pages, uint32_t level)
{
	return (long)liblinux_ext_hvgr_mmu_hal_flush_pt(ctx->asid, gva, pages, level);
}

long hvgr_mmu_hal_disable(struct hvgr_ctx *ctx)
{
	return (long)liblinux_ext_hvgr_mmu_hal_disable(ctx->asid);
}

void hvgr_mmu_cbit_config(struct hvgr_device *gdev)
{
	(void)liblinux_ext_hvgr_mmu_cbit_config();
}

void hvgr_mmu_flush_l2_caches(struct hvgr_device *gdev)
{
	struct hvgr_ctx *ctx = gdev->dm_dev.fcp_data->ctx;

	if (!hvgr_mmu_flush_l2_is_needed())
		return;
	if (ctx == NULL) {
		hvgr_err(gdev, HVGR_MEM, "mmu flush L2 ctx is null.");
		return;
	}
	(void)liblinux_ext_hvgr_mmu_hal_flush_l2_caches();
}

#if hvgr_version_lt(HVGR_V300)
void hvgr_mmu_fault_clear_flush_unmask(struct hvgr_ctx *ctx, uint64_t gva, uint32_t pages,
	uint32_t as_bits)
{
	unsigned long flags;
	struct hvgr_device * const gdev = ctx->gdev;

	spin_lock_irqsave(&gdev->mem_dev.mmu_hw_lock, flags);
	(void)liblinux_ext_hvgr_mmu_fault_clear_as(ctx->asid, gva, pages, as_bits);
	spin_unlock_irqrestore(&gdev->mem_dev.mmu_hw_lock, flags);
}
#else
void hvgr_mmu_fault_clear_all_as(struct hvgr_ctx *ctx, struct hvgr_device *gdev)
{
	unsigned long flags;

	spin_lock_irqsave(&gdev->mem_dev.mmu_hw_lock, flags);

	if (gdev->mem_dev.fault_as_num != 1U) {
		gdev->mem_dev.fault_as_num--;
		hvgr_err(gdev, HVGR_MEM, "mmu fault clear fault_as_num %u",
			gdev->mem_dev.fault_as_num);
		spin_unlock_irqrestore(&gdev->mem_dev.mmu_hw_lock, flags);
		return;
	}

	(void)liblinux_ext_hvgr_mmu_fault_clear_as(0U, 0U, 0U, 0U);

	gdev->mem_dev.fault_as_num--;
	spin_unlock_irqrestore(&gdev->mem_dev.mmu_hw_lock, flags);
}
#endif

