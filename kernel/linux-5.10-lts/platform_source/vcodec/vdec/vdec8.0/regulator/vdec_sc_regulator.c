/*
 * vdec_sc_regulator.c
 *
 * This is for vdec sc related regulator
 *
 * Copyright (c) 2024-2024 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/io.h>
#include <linux/mm_lb/mm_lb.h>
#include "vdec_sc_regulator.h"
#include "vcodec_vdec.h"
#include "vdec_memory_map.h"

void vdec_hal_cfg_pxpc_syscache(void)
{
#ifdef CONFIG_VCODEC_OHOS
	return;
#else
	vdec_entry *entry = vdec_get_entry();
	uint32_t offset;
	uint32_t swid;
	vdec_cache_as_ful cache_as_ful;
	smmu_tbu_swid_cfg smmu_tbu_cfg;

	for (swid = VDEC_RCHN_PXPC_START_SWID; swid < VDEC_RCHN_PXPC_END_SWID; swid++) {
		offset = OFFSET_OF_VDEC_BASE(SMMU_PRE_REG_BASE) + swid * 0x4 + VDEC_CACHE_AS_FUL_OFS;
		cache_as_ful.u32 = readl(entry->reg_base_vir_addr + offset);
		cache_as_ful.bits.sc_gid_hint_nsafe = PID_VDEC;
		cache_as_ful.bits.sc_new_hint_nsafe = 0x9;
		cache_as_ful.bits.cache_nsafe = 0x3;
		cache_as_ful.bits.domain_nsafe = 0x3;
		writel(cache_as_ful.u32, (entry->reg_base_vir_addr + offset));

		offset = OFFSET_OF_VDEC_BASE(SMMU_TBU_REG_BASE) + swid * 0x4 + SMMU_TBU_SWID_CFG_OFS;
		smmu_tbu_cfg.u32 = readl(entry->reg_base_vir_addr + offset);
		smmu_tbu_cfg.bits.syscache_hint_sel = 0x2;
		writel(smmu_tbu_cfg.u32, (entry->reg_base_vir_addr + offset));
	}

	for (swid = VDEC_WCHN_PXPC_START_SWID; swid < VDEC_WCHN_PXPC_END_SWID; swid++) {
		offset = OFFSET_OF_VDEC_BASE(SMMU_PRE_REG_BASE) + swid * 0x4 + VDEC_CACHE_AS_FUL_OFS;
		cache_as_ful.u32 = readl(entry->reg_base_vir_addr + offset);
		cache_as_ful.bits.sc_gid_hint_nsafe = PID_VDEC;
		cache_as_ful.bits.sc_new_hint_nsafe = 0xA;
		cache_as_ful.bits.cache_nsafe = 0x3;
		cache_as_ful.bits.domain_nsafe = 0x3;
		writel(cache_as_ful.u32, (entry->reg_base_vir_addr + offset));

		offset = OFFSET_OF_VDEC_BASE(SMMU_TBU_REG_BASE) + swid * 0x4 + SMMU_TBU_SWID_CFG_OFS;
		smmu_tbu_cfg.u32 = readl(entry->reg_base_vir_addr + offset);
		smmu_tbu_cfg.bits.syscache_hint_sel = 0x2;
		writel(smmu_tbu_cfg.u32, (entry->reg_base_vir_addr + offset));
	}
#endif
}