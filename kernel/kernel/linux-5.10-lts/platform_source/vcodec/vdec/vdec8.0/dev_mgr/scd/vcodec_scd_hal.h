/*
 * vcodec_scd_hal.h
 *
 * Interface description related to scd register operations
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
#ifndef VCODEC_SCD_HAL_H
#define VCODEC_SCD_HAL_H

#include <linux/types.h>
#include "vcodec_scd.h"

int32_t vcodec_scd_init_hal(struct vcodec_dev_scd *dev, uint8_t *reg_base);
void vcodec_scd_deinit_hal(struct vcodec_dev_scd *dev);
void vcodec_scd_hal_irq_mask(struct vcodec_dev_scd* dev);

#endif