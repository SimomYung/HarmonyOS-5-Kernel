/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2023-2023. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_DATAN_CT_HAL_H
#define HVGR_DATAN_CT_HAL_H

#include "hvgr_defs.h"
#include "hvgr_datan_ct_config.h"

void hvgr_datan_ct_open_config(struct hvgr_device *gdev, struct hvgr_ctx *ctx,
	struct hvgr_datan_ct_inner_config *config);

void hvgr_datan_ct_close_config(struct hvgr_device *gdev);

void hvgr_datan_ct_dump_data(struct hvgr_device *gdev);

#endif /* HVGR_DATAN_CT_HAL_H */
