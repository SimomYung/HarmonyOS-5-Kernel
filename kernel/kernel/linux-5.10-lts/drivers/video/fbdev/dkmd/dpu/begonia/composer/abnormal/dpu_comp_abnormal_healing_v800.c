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

#include <linux/delay.h>
#include <linux/time.h>
#include <linux/rtc.h>
#include <dpu/dpu_dm.h>
#include <dpu/soc_dpu_define.h>
#include "../common/dfx_sh_common.h"
#include "dpu_comp_abnormal_handle.h"
#include "dksm_dmd.h"
#include "dksm_debug.h"
#include "dpu_comp_mgr.h"
#include "dpu_comp_abnormal_handle.h"
#include "dksm_debug.h"
#include "dpu_debug_dump.h"
#include "platform_include/basicplatform/linux/dfx_bbox_diaginfo.h"
#include "bbox_diaginfo_id_def.h"
#include "platform_include/basicplatform/linux/util.h"
#include "platform_include/basicplatform/linux/ipc_rproc.h"
#include "platform_include/basicplatform/linux/ipc_msg.h"

#define DIAGINFO_MBX_MSG_LEN 2
#define MSG_TYPE_AP_DSS_SH IPC_CMD(OBJ_AP, OBJ_LPM3, CMD_INQUIRY, 10)
#define DSS_MAX_CHECK_TIME (60 * 60 * 24)
#define DSS_MAX_UNDERFLOW_CNT 3
#define SH_PV_ON   1
#define SH_PV_OFF  0

static int dpu_self_healing_send_lpm3_msg(void)
{
	int msg[DIAGINFO_MBX_MSG_LEN] = {0};
	int ret;

	msg[0] = MSG_TYPE_AP_DSS_SH;
	msg[1] = SH_PV_ON;
	ret = RPROC_ASYNC_SEND(IPC_ACPU_LPM3_MBX_5, (mbox_msg_t *)msg, DIAGINFO_MBX_MSG_LEN);
	if (ret != 0) {
		dpu_pr_err("RPROC_ASYNC_SEND failed! return 0x%x, msg:[0x%x 0x%x]", ret, msg[0], msg[1]);
	} else {
		dpu_pr_info("notify lpmcu succcess");
	}
	return ret;
}

static void dpu_self_healing_early_stage(struct kthread_work *work)
{
	struct dpu_self_healing_ctrl *healing_ctrl = NULL;
	healing_ctrl = container_of(work, struct dpu_self_healing_ctrl, sh_work);
	int dss_sh_flag = early_get_dss_sh_flag();
	dpu_pr_debug("get_dss_sh_flag=%d", dss_sh_flag);
	healing_ctrl->early_stage_sh_flag = true;
	if (dss_sh_flag == SH_PV_OFF) {
		dpu_pr_debug("not set nv, no need healing");
		return;
	}

	int ret = dpu_self_healing_send_lpm3_msg();
	if (ret == 0)
		healing_ctrl->vote = true;
}

void dpu_self_healing_init(struct dpu_self_healing_ctrl *self_healing_ctrl)
{
	if (self_healing_ctrl == NULL) {
		dpu_pr_err("healing_ctrl is null");
		return;
	}
	self_healing_ctrl->first_time = 0;
	self_healing_ctrl->second_time = 0;
	self_healing_ctrl->early_stage_sh_flag = false;
	self_healing_ctrl->vote = false;
	self_healing_ctrl->stop_try_sh = false;
	self_healing_ctrl->underflow_cnt = 0;
	kthread_init_work(&self_healing_ctrl->sh_work, dpu_self_healing_early_stage);
}

static bool dpu_comp_check_underflow_threshold(struct dpu_self_healing_ctrl *healing_ctrl, time64_t cur_time)
{
	dpu_pr_debug("curr_time = %llu, underflow_time = %llu, %llu, underflow_cnt = %u",
		cur_time, healing_ctrl->first_time, healing_ctrl->second_time, healing_ctrl->underflow_cnt);

	if (healing_ctrl->first_time == 0) {
		healing_ctrl->first_time = cur_time;
		return false;
	}

	if (healing_ctrl->second_time == 0) {
		if ((cur_time - healing_ctrl->first_time) >= DSS_MAX_CHECK_TIME) {
			healing_ctrl->first_time = cur_time;
			healing_ctrl->underflow_cnt = 1;
			return false;
		}
		healing_ctrl->second_time = cur_time;
		return false;
	}

	if ((cur_time - healing_ctrl->first_time) >= DSS_MAX_CHECK_TIME) {
		if ((cur_time - healing_ctrl->second_time) >= DSS_MAX_CHECK_TIME) {
			healing_ctrl->first_time = cur_time;
			healing_ctrl->second_time = 0;
			healing_ctrl->underflow_cnt = 1;
			return false;
		}

		healing_ctrl->first_time = healing_ctrl->second_time;
		healing_ctrl->second_time = cur_time;
		healing_ctrl->underflow_cnt = 2;
		return false;
	}

	dpu_pr_info("underflow occur three times 24h");
	return true;
}

static void dpu_comp_abnormal_judge_back_nv(struct dpu_self_healing_ctrl *healing_ctrl, time64_t cur_time)
{
	if (dpu_comp_check_underflow_threshold(healing_ctrl, cur_time)) {
		dpu_pr_debug("sh healing is invalid");
		early_set_dss_sh_flag(SH_PV_OFF);
		healing_ctrl->stop_try_sh = true;
		bbox_diaginfo_record(DMD_DSS_SH_VOTE_M1, NULL, "underflow cnt %u back dss nv",
				healing_ctrl->underflow_cnt);
	}
}

static void dpu_comp_abnormal_self_healing(struct dpu_self_healing_ctrl *healing_ctrl, time64_t cur_time)
{
	if (dpu_comp_check_underflow_threshold(healing_ctrl, cur_time)) {
		int ret = dpu_self_healing_send_lpm3_msg();
		if (ret == 0) {
			healing_ctrl->underflow_cnt = 0;
			healing_ctrl->first_time = 0;
			healing_ctrl->second_time = 0;
			healing_ctrl->vote = true;
			early_set_dss_sh_flag(SH_PV_ON);
			bbox_diaginfo_record(DMD_DSS_SH_VOTE_M1, NULL, "underflow cnt %u to self healing",
				healing_ctrl->underflow_cnt);
		}
	}
}

void dpu_comp_self_healing_process(struct dpu_composer *dpu_comp)
{
	time64_t cur_time;
	struct dpu_self_healing_ctrl *healing_ctrl = NULL;

	if (dpu_comp == NULL) {
		dpu_pr_err("dpu_comp is NULL");
		return;
	}

	if (g_dpu_dsm_info.underflow_clear_type == CLEAR_SUC || g_dpu_dsm_info.layer_num == 0 ||
		g_dpu_dsm_info.hebc_err > 0)
		return;

	healing_ctrl = &dpu_comp->self_healing_ctrl;
	if((healing_ctrl == NULL) || (healing_ctrl->stop_try_sh)) {
		return;
	}

	healing_ctrl->underflow_cnt++;
	cur_time = ktime_get_boottime_seconds();
	dpu_pr_debug("self_healing_process: underflow_cnt=%d", healing_ctrl->underflow_cnt);

	if (healing_ctrl->vote) {
		dpu_comp_abnormal_judge_back_nv(healing_ctrl, cur_time);
	} else {
		dpu_comp_abnormal_self_healing(healing_ctrl, cur_time);
	}
}