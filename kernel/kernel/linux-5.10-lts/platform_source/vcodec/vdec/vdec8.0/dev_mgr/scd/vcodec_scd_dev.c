/*
 * vcodec_scd_dev.c
 *
 * Implement vcodec scd function which is used only by
 * formal version
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

int32_t vcodec_scd_init_dev(struct vcodec_dev_scd *dev, uint8_t *reg_base)
{
	return vcodec_scd_init_hal(dev, reg_base);
}

void vcodec_scd_deinit_dev(struct vcodec_dev_scd *dev)
{
	vcodec_scd_deinit_hal(dev);
}

int32_t vcodec_scd_reset_dev(struct vcodec_dev_scd *dev)
{
	return 0;
}

void vcodec_scd_set_irq_mask(struct vcodec_dev_scd* dev)
{
	vcodec_scd_hal_irq_mask(dev);
}

irqreturn_t vcodec_scd_interrupt_handling(struct vcodec_dev_scd* dev)
{
	return IRQ_HANDLED;
}

const struct vcodec_scd_control_proc_tbl *vcodec_scd_get_control_proc_tbl(void)
{
	static struct vcodec_scd_control_proc_tbl tbl[] = {
		/* tbl end flag */
		{SCD_INVALID_CMD_ID, 0, VCODEC_SCD_STATUS_BUTT, NULL},
	};
	return tbl;
}