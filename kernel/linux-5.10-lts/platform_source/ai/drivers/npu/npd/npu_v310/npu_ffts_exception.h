/*
 * npu_ffts_exception.h
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
#ifndef __NPU_FFTS_EXCEPETION_H
#define __NPU_FFTS_EXCEPETION_H

#include <linux/io.h>
#include "npu_common.h"
#ifdef CONFIG_NPU_OHOS
#include "npu_udk.h"
#endif

#define MAX_NPU_ENTRY 8
#define NPU_RTSQ_NUM_PER_ENTRY 2
#define HWTS_RETRY_TIMES 1000
#define PUSHTASKSYNC 0
#define PUSHTASKSYNCALL 1

#ifndef CONFIG_NPU_OHOS
int npd_dump_exception_ffts(struct ffts_exception_report_info *report);
int npu_handle_ffts_exception(struct ffts_exception_report_info *report);
int npu_handle_global_exception_for_ffts(void);
int npu_ffts_entry_is_clear(void);
#endif

int npu_ffts_clear_entry_by_taskidx(uint16_t task_idx);
int npu_proc_block_resource_clear(u16 uid);
#endif