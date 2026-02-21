/*
 * vcodec_bsp_hal.h
 *
 * Interface description related to bsp register operations
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
#ifndef VCODEC_BSP_HAL_H
#define VCODEC_BSP_HAL_H

#include <linux/types.h>
#include <linux/interrupt.h>
#include "vcodec_bsp.h"

int32_t vcodec_bsp_init_hal(struct vcodec_dev_bsp* dev, uint8_t *reg_base);
void vcodec_bsp_deinit_hal(struct vcodec_dev_bsp* dev);
int32_t vcodec_bsp_hal_poweron(struct vcodec_dev_bsp* dev);
int32_t vcodec_bsp_hal_poweroff(struct vcodec_dev_bsp* dev);

void vcodec_bsp_hal_reset_pre_process(struct vcodec_dev_bsp* dev);
void vcodec_bsp_hal_unreset_host_process(struct vcodec_dev_bsp* dev);

void vcodec_bsp_hal_irq_mask(struct vcodec_dev_bsp* dev);
irqreturn_t vcodec_bsp_hal_interrupt_handling(struct vcodec_dev_bsp* dev);

#endif