/*
 * npu_pm_ffap1.h
 *
 * about npu hwts plat
 *
 * Copyright (c) 2023-2023 Huawei Technologies Co., Ltd.
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

#ifndef __NPU_PM_FFAP1_H
#define __NPU_PM_FFAP1_H
#include "npu_common.h"
int npu_ffap1_connect(u32 work_mode);
int npu_ffap1_disconnect(u32 work_mode);
int npu_ffap1_status_hold(void);
#endif