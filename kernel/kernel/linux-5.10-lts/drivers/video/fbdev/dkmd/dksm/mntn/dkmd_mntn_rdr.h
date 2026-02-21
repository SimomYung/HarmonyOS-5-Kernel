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

#ifndef _DKMD_MNTN_RDR_H_
#define _DKMD_MNTN_RDR_H_

#include <platform_include/basicplatform/linux/rdr_platform.h>
#include <mntn_subtype_exception.h>

enum dpu_module_id {
	MODID_DSS_NOC_EXCEPTION = DFX_BB_MOD_DSS_START,
	MODID_DSS_VACT_IRQ_TIMEOUT,
	MODID_DSS_EXCEPTION_END = DFX_BB_MOD_DSS_END,
};

int32_t dkmd_rdr_dump_system_init(void);
void dkmd_rdr_dump_exception_info(uint32_t modid, uint64_t arg1, uint64_t arg2);

#endif