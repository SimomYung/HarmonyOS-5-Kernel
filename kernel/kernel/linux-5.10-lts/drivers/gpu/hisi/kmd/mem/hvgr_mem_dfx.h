/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2025-2025. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_MEM_DFX_H
#define HVGR_MEM_DFX_H

#include <linux/types.h>

#include "hvgr_defs.h"

void hvgr_dfx_find_pa_in_ctx(struct hvgr_ctx *ctx, uint64_t align_pa);
void hvgr_dfx_show_pa(struct hvgr_device *gdev, uint64_t pa);
#endif
