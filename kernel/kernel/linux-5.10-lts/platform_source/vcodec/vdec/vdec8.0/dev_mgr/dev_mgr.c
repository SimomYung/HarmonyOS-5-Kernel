/*
 * dev_mgr.c
 *
 * Implement of vcodec sub device manager module
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
#include "dev_mgr.h"
#include "dev_create.h"

struct vcodec_dev* create_vcodec_dev(enum vcodec_dev_type type, struct device *device)
{
	switch (type) {
	case VCODEC_DEV_MCU:
		return vcodec_mcu_create_dev(device);
	case VCODEC_DEV_SCD:
		return vcodec_scd_create_dev(device);
	case VCODEC_DEV_BSP:
		return vcodec_bsp_create_dev(device);
	case VCODEC_DEV_PXP:
		return vcodec_pxp_create_dev(device);
	default:
		return NULL;
	}
}

void destroy_vcodec_dev(struct vcodec_dev* dev)
{
	if (dev)
		dev->destroy(dev);
}