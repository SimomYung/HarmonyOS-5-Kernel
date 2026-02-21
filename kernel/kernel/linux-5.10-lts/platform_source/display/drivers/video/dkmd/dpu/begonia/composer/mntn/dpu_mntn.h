/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#ifndef _DPU_MNTN_H_
#define _DPU_MNTN_H_

#ifdef CONFIG_MDFX_KMD
#include <linux/fs.h>
#include "hisi_mdfx.h"
#include "dpu_cmdlist.h"

///// struct define /////
#define DPU_MNTN_DUMP_MODULE_COUNT 10
struct dpu_mntn_reg_module {
    uint32_t reg_offset;
    uint32_t reg_count;
};

struct dpu_mntn_dump_ctrl {
    uint32_t scene_id;
    uint32_t frame_index;
    struct dpu_mntn_reg_module modules[DPU_MNTN_DUMP_MODULE_COUNT];
};

extern int64_t g_dkmd_mdfx_id;

void dkmd_create_mdfx_client(void *data);
void dkmd_destroy_mdfx_client(void);
void dpu_mntn_report_event(uint32_t event_type, const struct dpu_cmdlist_frame_info *last_cmdlist_frm_info);

#define mdfx_trace_begin(type)   mdfx_tracing_point(g_dkmd_mdfx_id, type, __func__, " begin!")
#define mdfx_trace_end(type)     mdfx_tracing_point(g_dkmd_mdfx_id, type, __func__, " end!")

#else
#define dkmd_create_mdfx_client(data)
#define dkmd_destroy_mdfx_client()
#define dpu_mntn_report_event(event_type, data)
#define mdfx_trace_begin(type)
#define mdfx_trace_end(type)

#endif // #ifdef CONFIG_MDFX_KMD

#endif
