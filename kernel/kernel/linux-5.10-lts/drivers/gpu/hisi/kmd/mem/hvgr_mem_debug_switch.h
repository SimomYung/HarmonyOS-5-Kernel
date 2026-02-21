/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_MEM_DEBUG_SWITCH_H
#define HVGR_MEM_DEBUG_SWITCH_H

#include "hvgr_defs.h"

#ifdef CONFIG_DFX_DEBUG_FS
void hvgr_mem_debugfs_init(struct hvgr_device * const gdev);
uint64_t hvgr_mmu_dfx_get_curtime(struct hvgr_device *gdev);
#if hvgr_version_ge(350)
void hvgr_mmu_dfx_cmdq_statis(struct hvgr_device *gdev, uint32_t type, uint64_t time);
uint64_t hvgr_mmu_fault_get_curtime(struct hvgr_device *gdev);
void hvgr_mmu_fault_time_fill(struct hvgr_mmu_fault_work *work,
	enum hvgr_mmu_fault_proc_step step, uint64_t time);
void hvgr_mmu_fault_set_page_num(struct hvgr_mmu_fault_work *work, uint64_t pages);
void hvgr_mmu_fault_time_show(struct hvgr_mmu_fault_work *work);
#endif
#if hvgr_version_eq(350)
void hvgr_mmu_buffer_print(struct hvgr_device * const gdev);
#endif

#else
static inline void hvgr_mem_debugfs_init(struct hvgr_device * const gdev)
{
}

#if hvgr_version_ge(350)
struct hvgr_mmu_fault_work;
static inline uint64_t hvgr_mmu_dfx_get_curtime(struct hvgr_device *gdev)
{
	return 0UL;
}

static inline void hvgr_mmu_dfx_cmdq_statis(struct hvgr_device *gdev, uint32_t type,
	uint64_t time)
{
}

static inline uint64_t hvgr_mmu_fault_get_curtime(struct hvgr_device *gdev)
{
	return 0UL;
}

static inline void hvgr_mmu_fault_time_fill(struct hvgr_mmu_fault_work *work,
	enum hvgr_mmu_fault_proc_step step, uint64_t time)
{
}

static inline void hvgr_mmu_fault_set_page_num(struct hvgr_mmu_fault_work *work, uint64_t pages)
{
}

static inline void hvgr_mmu_fault_time_show(struct hvgr_mmu_fault_work *work)
{
}
#endif
#if hvgr_version_eq(350)
static inline void hvgr_mmu_buffer_print(struct hvgr_device * const gdev)
{
}
#endif

#endif

#endif
