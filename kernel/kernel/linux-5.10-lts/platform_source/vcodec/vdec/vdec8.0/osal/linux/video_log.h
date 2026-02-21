/*
 * video_log.h
 *
 * video log operations
 *
 * Copyright (c) 2024 Huawei Technologies Co., Ltd.
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

#ifndef __VIDEO_LOG_H__
#define __VIDEO_LOG_H__

#include "dbg.h"

int32_t vdec_log_init_entry(void);

void vdec_log_deinit_entry(void);

int32_t vdec_write_log(int8_t *buffer, uint32_t size, bool is_panic);

#endif
