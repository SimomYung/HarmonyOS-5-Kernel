/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2021-2022. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */


#ifndef HVGR_DM_DRIVER_BASAE_H
#define HVGR_DM_DRIVER_BASAE_H

#include "hvgr_defs.h"
#include "hvgr_dm_dfx.h"

#define HVGR_L2FAULT_GET_SROUCEID(data)  (((data) >> 59) & 0x1FU)
#define HVGR_L2FAULT_GET_STREAMID(data)  (((data) >> 54) & 0x1FU)

char *hvgr_get_irq_name(int i);
uint32_t hvgr_get_chipid_reg(void);
void hvgr_get_info_from_reg(struct hvgr_device *gdev);
uint32_t hvgr_get_gpu_id(struct hvgr_device *gdev);
bool hvgr_gpu_reset_needed_by_fault(uint32_t fault_code);
const char *hvgr_exception_name(uint32_t exception_code);
void hvgr_gpu_fault_proc(struct hvgr_device *gdev, uint32_t val);
#endif
