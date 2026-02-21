/*
 * npu_ffts_exception_ldk.h
 *
 * about npu aicore plat
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
#ifndef __NPU_FFTS_EXCEPETION_LDK_H
#define __NPU_FFTS_EXCEPETION_LDK_H

#include <linux/io.h>
#include "npu_common.h"
#ifdef CONFIG_NPU_SINK_UDK
#include "npu_udk.h"
#endif

#define MAX_NPU_ENTRY 8

int npu_ffts_clear_entry_by_taskidx(uint16_t task_idx);
int npu_proc_block_resource_clear(u16 uid);
#endif