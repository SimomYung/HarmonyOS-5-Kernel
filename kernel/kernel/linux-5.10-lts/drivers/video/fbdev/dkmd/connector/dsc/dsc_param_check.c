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

#ifndef DSC_VERSION_H
#define DSC_VERSION_H

#include "dkmd_log.h"
#include "dsc_algorithm.h"

#define BPC_ARRAY_SIZE 6
#define NATIVE422_ARRAY_SIZE 2

// version related param check.
struct vesa_dsc_param_range {
    uint32_t dsc_version;
    uint32_t bpc_range[BPC_ARRAY_SIZE];
    uint16_t native_422_range[NATIVE422_ARRAY_SIZE];
};

static struct vesa_dsc_param_range g_dsc_param_range[] = {
    {
        DSC_VERSION_V_1_1, 
        {DSC_8BPC, DSC_10BPC, DSC_12BPC}, 
        {0}
    },
    {
        DSC_VERSION_V_1_2, 
        {DSC_8BPC, DSC_10BPC, DSC_12BPC, DSC_14BPC, DSC_16BPC}, 
        {0, 1}
    },
};

void vesa_dsc_bpc_check(uint32_t version, uint32_t bpc)
{
    for (int i = 0; i < ARRAY_SIZE(g_dsc_param_range); i++) {
        if (g_dsc_param_range[i].dsc_version != version)
            continue;

        for (int j = 0; j < ARRAY_SIZE(g_dsc_param_range[i].bpc_range); j++) {
            if (g_dsc_param_range[i].bpc_range[j] == bpc)
                return;
        }
    }
    dpu_pr_err("bpc:%u not support in dsc version:%u", bpc, version);
}

void vesa_dsc_version_check(uint32_t version)
{
    for (int i = 0; i < ARRAY_SIZE(g_dsc_param_range); i++) {
        if (g_dsc_param_range[i].dsc_version == version)
            return;
    }
    dpu_pr_err("version:%u not support", version);
}

void vesa_dsc_native422_check(uint32_t version, uint16_t native_422)
{
    for (int i = 0; i < ARRAY_SIZE(g_dsc_param_range); i++) {
        if (g_dsc_param_range[i].dsc_version != version)
            continue;

        for (int j = 0; j < ARRAY_SIZE(g_dsc_param_range[i].native_422_range); j++) {
            if (g_dsc_param_range[i].native_422_range[j] == native_422)
                return;
        }
    }
    dpu_pr_err("native_422:%u not support in dsc version:%u", native_422, version);
}
// version related param check end.

#endif
