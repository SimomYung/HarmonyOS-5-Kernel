/*
 * vcodec_dev_mgr_util.h
 *
 * Declare util dev mgr function, it used by both formal version
 * and FPGA verification version
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

#ifndef VCODEC_DEV_MGR_UTIL_H
#define VCODEC_DEV_MGR_UTIL_H
#include "vcodec_types.h"

int32_t vcodec_dev_mgr_read_image(const char* path, void **buffer,
	size_t *size, size_t min_size, size_t max_size);
#endif
