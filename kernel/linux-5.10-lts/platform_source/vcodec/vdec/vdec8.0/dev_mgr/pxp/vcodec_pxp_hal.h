/*
 * vcodec_pxp_hal.h
 *
 * Interface description related to pxp register operations
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
#ifndef VCODEC_PXP_HAL_H
#define VCODEC_PXP_HAL_H

#include <linux/types.h>
#include <linux/interrupt.h>
#include "vcodec_pxp.h"

int32_t vcodec_pxp_init_hal(struct vcodec_dev_pxp* dev, uint8_t *reg_base);
void vcodec_pxp_deinit_hal(struct vcodec_dev_pxp *dev);
int32_t vcodec_pxp_hal_poweron(struct vcodec_dev_pxp* dev);
int32_t vcodec_pxp_hal_poweroff(struct vcodec_dev_pxp* dev);

void vcodec_pxp_hal_unreset_host_process(struct vcodec_dev_pxp* dev);

void vcodec_pxp_hal_irq_mask(struct vcodec_dev_pxp* dev);
irqreturn_t vcodec_pxp_hal_interrupt_handling(struct vcodec_dev_pxp* dev);

#endif