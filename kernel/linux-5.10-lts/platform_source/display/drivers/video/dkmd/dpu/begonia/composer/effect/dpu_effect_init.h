/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
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

#ifndef DPU_EFFECT_INIT_H
#define DPU_EFFECT_INIT_H

#include "dkmd_comp.h"
#include "dpu_comp_mgr.h"

void dpu_effect_thread_setup(struct dpu_composer *dpu_comp);
void dpu_effect_thread_relase(struct dpu_composer *dpu_comp);
bool is_vstate_in_vfp(char __iomem *dpu_base);
#endif /* DPU_EFFECT_INIT_H */