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
#include <linux/types.h>
#include "dkmd_log.h"
#include "dpu_comp_dfr.h"
#include "dpu_comp_multi_present.h"
#include "dpu_comp_mgr.h"
#include "dpu_comp_dfr_te_skip_acpu.h"
#define UNSAFE_PERIOD 1500    // 1500us

static void dpu_multi_present_clear_sfu2vactive_param(struct dpu_comp_dfr_ctrl *dfr_ctrl)
{
	int32_t i = 0;
	dpu_pr_debug("+");

	for(; i < PRESENT_CLIENT_TYPE_MAX; ++i) {
		dpu_pr_debug("[multi_present] need_execute_present = %d, clien_type = %d",
			dfr_ctrl->dpu_comp->multi_present_ctrl.clients[i].need_execute_present, i);
		dfr_ctrl->dpu_comp->multi_present_ctrl.clients[i].need_execute_present = false;
	}
	dfr_ctrl->dpu_comp->multi_present_ctrl.is_sfu2vactive_period = false;

	dpu_pr_debug("-");
	return;
}

static bool dpu_multi_present_is_safe_period(struct dpu_comp_dfr_ctrl *dfr_ctrl)
{
	struct dfr_te_skip_acpu_ctrl *priv = NULL;
	priv = (struct dfr_te_skip_acpu_ctrl *)dfr_ctrl->priv_data;
	dpu_check_and_return(!priv, false, err, "priv is null");
	return !dfr_ctrl->dpu_comp->multi_present_ctrl.is_sfu2vactive_period && !priv->is_sampling_config_period &&
		ktime_us_delta(ktime_add_ns(priv->te_isr_timestamp, NSEC_PER_SEC / dfr_ctrl->cur_te_rate), ktime_get()) >
		UNSAFE_PERIOD;
}

static struct multi_present_ops multi_present_func = {
    .init_param = dpu_multi_present_init_param,
    .present_request_set = dpu_multi_present_request_set,
    .set_sfu2vactive_param = dpu_multi_present_set_sfu2vactive_param,
    .need_config_sfu = dpu_multi_present_need_config_sfu,
    .clear_sfu2vactive = dpu_multi_present_clear_sfu2vactive_param,
    .is_safe_period = dpu_multi_present_is_safe_period,
};

void dpu_multi_present_register_ops_dfr_by_acpu(struct dpu_multi_present_ctrl *multi_present_ctrl)
{
    multi_present_ctrl->ops = &multi_present_func;
    multi_present_ctrl->ops->init_param(multi_present_ctrl);
}