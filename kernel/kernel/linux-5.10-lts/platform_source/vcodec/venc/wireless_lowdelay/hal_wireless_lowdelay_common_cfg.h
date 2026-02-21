/*
 * hal_lowdelay_cfg_reg.h
 *
 * This is for wireless low latency solution.
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

#ifndef __HAL_WIRELESS_LOWDELAY_CFG_H__
#define __HAL_WIRELESS_LOWDELAY_CFG_H__

#include "drv_venc_ioctl.h"

void venc_hal_cfg_wireless_lowdelay(struct encode_info *channel_cfg, uint32_t *reg_base);

#endif
