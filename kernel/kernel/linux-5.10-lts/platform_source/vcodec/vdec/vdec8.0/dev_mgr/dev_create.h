/*
 * dev_create.h
 *
 * Headers define of vcodec create
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

#ifndef VCODEC_DEV_CREATE_H
#define VCODEC_DEV_CREATE_H
#include "dev_mgr.h"

struct vcodec_dev* vcodec_mcu_create_dev(struct device *device);
struct vcodec_dev* vcodec_scd_create_dev(struct device *device);
struct vcodec_dev* vcodec_bsp_create_dev(struct device *device);
struct vcodec_dev* vcodec_pxp_create_dev(struct device *device);
#endif