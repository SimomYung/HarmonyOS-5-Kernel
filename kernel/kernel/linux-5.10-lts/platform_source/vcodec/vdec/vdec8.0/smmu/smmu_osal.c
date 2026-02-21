/*
 * smmu_osal.c
 *
 * This is for smmu linux osal.
 *
 * Copyright (c) 2019-2020 Huawei Technologies CO., Ltd.
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

#include <linux/iommu/mm_iommu.h>
#include <linux/iommu/mm_svm.h>
#include "dbg.h"
#include "smmu_regs.h"
#include "smmu_osal.h"
#include "vfmw_ext.h"
#include "vcodec_vdec_regulator.h"
#include "vcodec_vdec.h"
#include "vdec_memory_map.h"

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0))
#define IO_REMAP_NO_CACHE ioremap
#else
#define IO_REMAP_NO_CACHE ioremap_nocache
#endif

int32_t smmu_power_on_tcu(void)
{
	struct regulator *tcu_regulator =
		vdec_plat_get_entry()->regulator_info.regulators[SMMU_TCU_REGULATOR];

	return regulator_enable(tcu_regulator);
}

void smmu_power_off_tcu(void)
{
	int32_t ret;
	struct regulator *tcu_regulator =
		vdec_plat_get_entry()->regulator_info.regulators[SMMU_TCU_REGULATOR];

	ret = regulator_disable(tcu_regulator);
	if (ret)
		dprint(PRN_ERROR, "power off tcu failed\n");
}

int32_t smmu_map_reg_osal(struct smmu_entry *entry)
{
	uint8_t *reg_vaddr = NULL;
	vdec_entry *vdec = platform_get_drvdata(vdec_get_device());

	reg_vaddr = vdec->reg_base_vir_addr + 0x406000;
	entry->reg_info.smmu_sid_reg_vir = reg_vaddr;

	reg_vaddr = vdec->reg_base_vir_addr + 0x401000;
	entry->reg_info.smmu_tbu_reg_vir = reg_vaddr;

	return SMMU_OK;
}

void smmu_unmap_reg_osal(struct smmu_entry *entry)
{
	if (entry->reg_info.smmu_tbu_reg_vir)
		entry->reg_info.smmu_tbu_reg_vir = VCODEC_NULL;

	if (entry->reg_info.smmu_sid_reg_vir)
		entry->reg_info.smmu_sid_reg_vir = VCODEC_NULL;
}

void smmu_media1_tcu_dump(void)
{
	arm_smmu_media1_tcu_dump();
}

