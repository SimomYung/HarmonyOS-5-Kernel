/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
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

#include <dpu/soc_dpu_define.h>
#include "opr_cmd_data.h"
#include "dkmd_log.h"

static uint32_t get_opr_module_index(int32_t opr_dpu_type, int32_t opr_index)
{
	switch (opr_dpu_type) {
	case OPERATOR_OV:
		return OPR_MOD_IDX_OV0;
	case OPERATOR_HDR:
		return OPR_MOD_IDX_HDR;
	case OPERATOR_UVUP:
		return OPR_MOD_IDX_UVUP0;
	case OPERATOR_CLD:
		return OPR_MOD_IDX_CLD0;
	case OPERATOR_ARSR:
		return OPR_MOD_IDX_ARSR0 + opr_index;
	case OPERATOR_VSCF:
		return OPR_MOD_IDX_VSCF0 + opr_index;
	case OPERATOR_ITFSW:
		return OPR_MOD_IDX_ITFSW0 + opr_index;
	case OPERATOR_DPP:
		return OPR_MOD_IDX_DPP0 + opr_index;
	case OPERATOR_DSC:
		return OPR_MOD_IDX_DSC0;
	case OPERATOR_WCH:
		return OPR_MOD_IDX_WCH0 + opr_index;
	case OPERATOR_DDIC:
		return OPR_MOD_IDX_DDIC0;
	default:
		break;
	}

	return OPR_MOD_IDX_INVALID;
}

uint32_t get_opr_oder(int32_t opr_dpu_type, int32_t opr_index)
{
	return get_opr_module_index(opr_dpu_type, opr_index);
}
