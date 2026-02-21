/*
 * npu_platform_exception.h
 *
 * about npu proc hwts exception
 *
 * Copyright (c) 2019-2024 Huawei Technologies Co., Ltd.
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

#ifndef __NPU_PLATFORM_EXCEPTION_H
#define __NPU_PLATFORM_EXCEPTION_H

#include "npu_common.h"

void npu_hwts_exception_dump(u8 exception_type, u16 hwts_sq_id, u16 channel_id);
int npu_timeout_exception_dump(void);
int npu_exception_aicore_reset(u16 channel_id);
void npu_ffrt_error_exception_dump(uint16_t entry_id);
#endif
