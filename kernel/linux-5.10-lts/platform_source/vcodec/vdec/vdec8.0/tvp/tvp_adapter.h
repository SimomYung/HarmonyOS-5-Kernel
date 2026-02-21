/*
 * tvp_adapter.h
 *
 * This is for vdec tvp adapter.
 *
 * Copyright (c) 2017-2020 Huawei Technologies CO., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef TVP_ADAPTER_H
#define TVP_ADAPTER_H

#include "dbg.h"
#include "vfmw_ext.h"

int32_t tvp_vdec_secure_init(void);
int32_t tvp_vdec_secure_exit(void);
int32_t tvp_vdec_suspend(void);
int32_t tvp_vdec_resume(void);
int32_t tvp_vdec_bsp_open(bsp_powerup_cfg *info);
int32_t tvp_vdec_pxp_open(pxp_powerup_cfg *info);
int32_t tvp_vdec_bsp_close(void);
int32_t tvp_vdec_pxp_close(void);
#ifdef ENABLE_VDEC_DUMP_REG
int32_t tvp_vdec_dump_reg(void *buffer, uint32_t size);
#endif

#define TEEC_OPERATION_PARA_INDEX_FIRST  1
#define TEEC_OPERATION_PARA_INDEX_SECOND 2
#define TEEC_OPERATION_PARA_INDEX_THIRD  3

#define TEE_INVOKE_EXECUTION_TIME_THRESHOLD 40000
#endif

