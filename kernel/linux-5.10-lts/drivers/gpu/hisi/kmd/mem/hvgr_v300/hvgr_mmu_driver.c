/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#include "hvgr_mmu_driver.h"

#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/highmem.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <securec.h>

#include "hvgr_regmap.h"
#include "hvgr_dm_api.h"
#include "hvgr_mem_api.h"
#include "hvgr_mmu_base.h"
#include "hvgr_mmu_entry.h"
#include "hvgr_mmu_struct.h"
#include "hvgr_log_api.h"
#include "hvgr_datan_api.h"

char *hvgr_mmu_other_fault_process(struct hvgr_ctx *ctx, struct hvgr_mmu_as *mmu_as)
{
	char *reset_reason = NULL;
	uint32_t fault_code = (mmu_as->fault_stat >> AS_EXCEPTION_CODE_OFFSET) &
		AS_EXCEPTION_CODE_MASK;

	if (fault_code <= PERMISSION_FAULT_HIGH && fault_code >= PERMISSION_FAULT_LOW)
		reset_reason = "Permission fault";
	else if (fault_code == ADDR_SZ_FAULT)
		reset_reason = "address_size_fault";
	else if (fault_code == BLOCK_AT_SL_FAULT)
		reset_reason = "block_at_sl_fault";
	else if (fault_code <= ACCESS_FLAG_FAULT_HIGH && fault_code >= ACCESS_FLAG_FAULT_LOW)
		reset_reason = "Access flag unexpectedly set";
	else if (fault_code <= MEMATTR_INDEX_FAULT_HIGH && fault_code >= MEMATTR_INDEX_FAULT_LOW)
		reset_reason = "memattr_index_fault";
	else
		reset_reason = "Unknown fault code";

	return reset_reason;
}

void hvgr_mem_flush_pt(struct hvgr_ctx * const ctx, uint64_t gva,
	uint64_t pages, uint32_t flag, uint32_t level)
{
	hvgr_mem_flush_pt_serial(ctx, gva, pages, flag, level);
}

int hvgr_mmu_init(struct hvgr_device * const gdev)
{
	return 0;
}

void hvgr_mmu_term(struct hvgr_device * const gdev)
{
	int i;
	struct hvgr_mmu_as *mmu_as = NULL;

	for (i = 0; i < MMU_ADDR_SPACE_MAX; i++) {
		mmu_as = &gdev->mem_dev.mmu_as[i];
		if (IS_ERR_OR_NULL(mmu_as->fault_wq))
			continue;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
		destroy_workqueue(mmu_as->fault_wq);
#else
		kthread_destroy_worker(mmu_as->fault_wq);
#endif
	}
}