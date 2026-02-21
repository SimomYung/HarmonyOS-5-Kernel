/*
 * smmu_tbu.h
 *
 * This is for smmu tbu.
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

#ifndef VDEC_SMMU_TBU_H
#define VDEC_SMMU_TBU_H

#include <linux/types.h>
#include "smmu_regs.h"

void smmu_init_one_sid(uint32_t reg_base_offset);
void smmu_init_entry(struct smmu_entry *entry);
int32_t smmu_init_one_tbu(uint32_t reg_base_offset);
int32_t smmu_deinit_one_tbu(uint32_t reg_base_offset);
int show_smmu_pre_intercept(void);
void smmu_pre_update_rdstart(uint32_t dstart);
void smmu_pre_update_rdend(uint32_t dend);
#endif
