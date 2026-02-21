/*
 * npu_ffts_exception_udk.h
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
#ifndef __NPU_FFTS_EXCEPETION_UDK_H
#define __NPU_FFTS_EXCEPETION_UDK_H

#include "npu_common.h"

#define MAX_NPU_ENTRY 8
#define HWTS_RETRY_TIMES 1000
#define PUSHTASKSYNC 0
#define PUSHTASKSYNCALL 1

int npd_dump_exception_ffts(struct ffts_exception_report_info *report);
int npu_handle_ffts_exception(struct ffts_exception_report_info *report);
int npu_ffts_entry_is_clear(void);
int npu_ffts_clear_entry(uint16_t task_idx, uint16_t entry_id);
int npu_proc_block_kill_task(u16 uid);
int npu_handle_global_exception_for_ffts(void);

#endif