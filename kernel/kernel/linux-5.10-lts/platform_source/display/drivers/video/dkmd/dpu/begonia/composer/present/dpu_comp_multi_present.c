/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
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
#include "dkmd_log.h"
#include "dpu_comp_dfr.h"
#include "dpu_comp_multi_present.h"

void dpu_multi_present_init(struct dpu_multi_present_ctrl *multi_present_ctrl, uint32_t dfr_mode)
{
	multi_present_ctrl->ops = NULL;
	switch (dfr_mode) {
	case DFR_MODE_TE_SKIP_BY_ACPU:
		dpu_multi_present_register_ops_dfr_by_acpu(multi_present_ctrl);
		break;
	default:
		break;
	}
	return;
}

void dpu_multi_present_request_set(struct dpu_multi_present_ctrl *multi_present_ctrl, int32_t client_type)
{
	if (client_type >= 0 && client_type < PRESENT_CLIENT_TYPE_MAX)
		multi_present_ctrl->clients[client_type].has_present_request = true;
	else
		dpu_pr_err("[multi_present] client_type is invalid, client_type = %d", client_type);
	return;
}

void dpu_multi_present_init_param(struct dpu_multi_present_ctrl *multi_present_ctrl)
{
	int i = 0;
	for(;i < PRESENT_CLIENT_TYPE_MAX; ++i) {
		multi_present_ctrl->clients[i].has_present_request = false;
		multi_present_ctrl->clients[i].need_execute_present = false;
	}
	multi_present_ctrl->is_sfu2vactive_period = false;
	return;
}

bool dpu_multi_present_need_config_sfu(struct dpu_multi_present_ctrl *multi_present_ctrl)
{
	int32_t i = 0;
	bool has_present_request = false;
	for(; i < PRESENT_CLIENT_TYPE_MAX; ++i) {
		has_present_request = (has_present_request || multi_present_ctrl->clients[i].has_present_request);
		dpu_pr_debug("[multi_present] has_present_request = %d, clien_type = %d",
			multi_present_ctrl->clients[i].has_present_request, i);
	}

	return has_present_request;
}

void dpu_multi_present_set_sfu2vactive_param(struct dpu_multi_present_ctrl *multi_present_ctrl)
{
	int32_t i = 0;
	for(; i < PRESENT_CLIENT_TYPE_MAX; ++i) {
		dpu_pr_debug("[multi_present] has_present_request = %d, client_type = %d",
			multi_present_ctrl->clients[i].has_present_request, i);

		if (multi_present_ctrl->clients[i].has_present_request)
			multi_present_ctrl->clients[i].need_execute_present = true;

		multi_present_ctrl->clients[i].has_present_request = false;
	}

	return;
}
