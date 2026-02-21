/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
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

#include <linux/delay.h>
#include <linux/time.h>
#include <linux/rtc.h>
#include <dpu/dpu_dm.h>
#include <dpu/soc_dpu_define.h>
#include "dpu_comp_abnormal_handle.h"
#include "dksm_dmd.h"
#include "dksm_debug.h"
#include "dpu_comp_mgr.h"

void dpu_self_healing_init(struct dpu_self_healing_ctrl *self_healing_ctrl)
{
	void_unused(self_healing_ctrl);
}

void dpu_comp_self_healing_process(struct dpu_composer *dpu_comp)
{
	void_unused(dpu_comp);
}