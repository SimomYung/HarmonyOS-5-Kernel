
/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2023-2023. All rights reserved.
 *
 * This program is free software and is provided to you under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation, and any use by you of this program is subject to the terms
 * of such GNU licence.
 */

#ifndef HVGR_MNTN_H
#define HVGR_MNTN_H
#include <platform_include/basicplatform/linux/rdr_pub.h>
#include <platform_include/basicplatform/linux/rdr_platform.h>
#include "mntn_public_interface.h"

#define HVGR_RDR_MODULE_NAME	"GPU"
#define INVALID_MODID_TYPE	0U
#define SOC_MID_DRM             255U

enum gpu_module_id {
	MODID_GPU_HANG_EXCEPTION = DFX_BB_MOD_GPU_START,
	MODID_GPU_FAULT_EXCEPTION,
	MODID_GPU_PAGE_FAULT_EXCEPTION,
	MODID_GPU_CRASH_EXCEPTION,
	MODID_GPU_DATA_QIC_EXCEPTION,
	MODID_GPU_EXCEPTION_END = DFX_BB_MOD_GPU_END,
};

struct hvgr_device;

int hvgr_mntn_init(struct hvgr_device * const gdev);

void hvgr_mntn_term(struct hvgr_device * const gdev);

void hvgr_mntn_error_report(struct hvgr_device *gdev);

void hvgr_mntn_dump(u32 modid, u32 etype, u64 coreid, char *pathname, pfn_cb_dump_done pfn_cb);
#endif /* HVGR_MNTN_H END */