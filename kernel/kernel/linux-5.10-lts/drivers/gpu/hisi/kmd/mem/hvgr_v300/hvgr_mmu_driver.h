/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_MMU_DRIVER_H
#define HVGR_MMU_DRIVER_H

#include <linux/kernel.h>
#include "hvgr_regmap.h"
#include "hvgr_mmu_api.h"

struct hvgr_mmu_cfg;

#define MMU_FAULT_MASK 0xFFu
#define mmu_bus_fault_bit(as) ((uint32_t)1u << (as))
#define FREE_AS_MASK          ((u16)0x00BE)


char *hvgr_mmu_other_fault_process(struct hvgr_ctx *ctx, struct hvgr_mmu_as *mmu_as);

static inline uint32_t hvgr_mmu_get_fault_adrmode(uint32_t fault_stat)
{
	return (fault_stat >> MMU_ADRMODE_OFFSET) & MMU_ADRMODE_MASK;
}

static inline uint32_t hvgr_mmu_get_mem_info(uint32_t fault_stat)
{
	return (fault_stat >> MMU_MEM_INFO_OFFSET) & MMU_MEM_INFO_MASK;
}

static inline bool hvgr_mmu_is_trans_fault(uint32_t fault_stat)
{
	uint32_t fault_code = (fault_stat >> AS_EXCEPTION_CODE_OFFSET) & AS_EXCEPTION_CODE_MASK;
	return (fault_code <= TRANSLATION_FAULT_HIGH && fault_code >= TRANSLATION_FAULT_LOW);
}

static inline void hvgr_mmu_get_fault(uint32_t stat, uint32_t *page_fault, uint32_t *bus_fault)
{
	*bus_fault = 0u;
	*page_fault = (stat & MMU_FAULT_MASK);
}

static inline bool hvgr_mmu_flush_l2_is_needed(void)
{
	return false;
}

#endif
