/*
 * vcodec_scd_hal.c
 *
 * Implementation of interfaces related to scd register operations
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include "vcodec_scd.h"
#include "vcodec_scd_hal.h"
#include "vdec_memory_map.h"

int32_t vcodec_scd_init_hal(struct vcodec_dev_scd *dev, uint8_t *reg_base)
{
	dev->reg_base_vir_addr = reg_base;
	dev->subctrl_vir_addr =
		(S_VDE_SUB_CTRL_NS_REGS_TYPE *)(reg_base + OFFSET_OF_VDEC_BASE(SUB_CTRL_REG_BASE));
	return 0;
}

void vcodec_scd_deinit_hal(struct vcodec_dev_scd *dev)
{
	dev->reg_base_vir_addr = NULL;
	dev->subctrl_vir_addr = NULL;
}

void vcodec_scd_hal_irq_mask(struct vcodec_dev_scd* dev)
{
	/* open scd int to MCU, bit0:scd */
	dev->subctrl_vir_addr->INTR_MASK_NS_VDEC = 0xFFFFFFFE;
	/* open qic_xxx err int to ACPU,
	   bit5:qic_cfg err, bit6:qic_core_cfg err, bit7:qic_core_data err */
	dev->subctrl_vir_addr->INTR_MASK_NS_VDEC1 = 0xFFFFFF1F;
}