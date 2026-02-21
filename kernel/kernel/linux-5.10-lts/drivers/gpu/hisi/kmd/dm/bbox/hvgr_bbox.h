/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2025-2025. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_BBOX_H
#define HVGR_BBOX_H
#include "hvgr_version.h"
#include "hvgr_defs.h"

#define HVGR_BBOX_ONCE_LOG_LEN 200U

int hvgr_bbox_init(struct hvgr_device * const gdev);
int hvgr_bbox_term(struct hvgr_device * const gdev);

uint64_t hvgr_get_current_kernel_time(void);
#if hvgr_version_ge(350)
void hvgr_bbox_log(struct hvgr_device *gdev, const char *format, ...);
#else
static inline void hvgr_bbox_log(struct hvgr_device *gdev, const char *format, ...)
{
	unused(gdev);
	unused(format);
}
#endif
#endif