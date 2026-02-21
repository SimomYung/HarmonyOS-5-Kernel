
/* Copyright (c) 2023-2023, Hisilicon Tech. Co., Ltd. All rights reserved.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2 and
* only version 2 as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
* GNU General Public License for more details.
*
*/

#ifndef DPU_EFFECT_HDR_H
#define DPU_EFFECT_HDR_H

#include <linux/mutex.h>
#include <linux/spinlock.h>
#include <linux/workqueue.h>
#include "dkmd_comp.h"
#include "dpu_comp_mgr.h"

void dpu_effect_hdr_set_mean(struct dpu_composer *dpu_comp, uint32_t hdr_mean);
void dpu_effect_hdr_init(struct dpu_composer *dpu_comp, uint32_t hdr_offset);
void dpu_effect_hdr_deinit(struct dpu_composer *dpu_comp);
int dpu_effect_hdr_get_mean(struct composer *comp, uint32_t *hdr_mean);
void dpu_effect_hdr_gtm_queue_work(struct dpu_composer *dpu_comp);
void dpu_effect_hdr_ltm_queue_work(struct dpu_composer *dpu_comp);
int dpu_effect_hdr_get_statistic(struct composer *comp, void __user* argp);
#endif