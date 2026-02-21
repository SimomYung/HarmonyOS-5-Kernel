/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2025. All rights reserved.
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

#include "dp_ctrl.h"
#include "dpu_conn_mgr.h"
#include "dp_drv.h"
#include "dp_aux.h"
#include "dp_ctrl_config.h"
#include "dp_dmd_report.h"
#include <platform_include/display/linux/dpu_dss_dp.h>
#include <platform_include/basicplatform/linux/dfx_bbox_diaginfo.h>
#include "drm_dp_helper_additions.h"
#include "dpu_connector.h"
#include "dp_hdmi_common.h"
#include "dksm_utils.h"
#include "dpu_dp_dbg.h"
#include <securec.h>

void dp_add_error_log(struct dp_ctrl *dptx, const char *error_str)
{
	int error_str_length;
	dpu_check_and_no_retval(!dptx || !error_str, err, "[DP] NULL Pointer\n");
	error_str_length = strlen(error_str);
	if (error_str_length >= ERROR_lOG_SIZE) {
		dpu_pr_info("[DP] str too long compared to max size\n");
		error_str_length = ERROR_lOG_SIZE;
	}

	mutex_lock(&dptx->dptx_dmd_mutex);
	if (memcpy_s(dptx->err_logs_stc.error_logs[dptx->err_logs_stc.error_current_point],
			ERROR_lOG_SIZE, error_str, error_str_length) != EOK) {
		dpu_pr_err("[DP] memcpy error_logs buffer error!");
		mutex_unlock(&dptx->dptx_dmd_mutex);
		return;
	}
	dptx->err_logs_stc.error_logs[dptx->err_logs_stc.error_current_point][error_str_length] = '\0';

	dptx->err_logs_stc.error_count++;
	dptx->err_logs_stc.error_current_point++;
	dptx->err_logs_stc.error_current_point = dptx->err_logs_stc.error_current_point % ERROR_lOG_ROW;

	if (dptx->err_logs_stc.error_count > ERROR_lOG_ROW) {
		dptx->err_logs_stc.error_count = ERROR_lOG_ROW;
		dptx->err_logs_stc.error_start_point++;
		dptx->err_logs_stc.error_start_point = dptx->err_logs_stc.error_start_point % ERROR_lOG_ROW;
	}
	mutex_unlock(&dptx->dptx_dmd_mutex);
}

void dp_reset_error_log(struct dp_ctrl *dptx)
{
	mutex_lock(&dptx->dptx_dmd_mutex);
	dptx->err_logs_stc.error_count = 0;
	dptx->err_logs_stc.error_current_point = 0;
	dptx->err_logs_stc.error_start_point = 0;
	mutex_unlock(&dptx->dptx_dmd_mutex);
}

static void dp_bbox_upload_record(struct dp_ctrl *dptx, int dfx_type, uint64_t current_ms, int error_type)
{
	uint64_t connect_jiffies = 0;
	uint32_t i = 0;
	char tmpstr[ERROR_TMP_SIZE + 1] = {0};

	(void)memset_s(dptx->poststr, DMD_STR_LEN, 0, DMD_STR_LEN);
	(void)memset_s(dptx->logstr, DMD_ERR_LEN, 0, DMD_ERR_LEN);

	if (dfx_type != DMD_DP_FAILED) {
		if (dfx_type == DMD_DP_DISCONNECT && dptx->plugin_jiffies > 0 && dptx->plugin_jiffies < jiffies)
			dptx->holdon_time += (jiffies - dptx->plugin_jiffies) / HZ;
		connect_jiffies = dptx->holdon_time;
		dptx->holdon_time = 0;
	} else {
		for (i = 0; i < dptx->err_logs_stc.error_count; i++) {
			if (sprintf_s(tmpstr, ERROR_TMP_SIZE, "%s|", dptx->err_logs_stc.error_logs[(
					dptx->err_logs_stc.error_start_point + i) % ERROR_lOG_ROW]) <= 0) {
				dpu_pr_err("[DP] dmd tmp create failed");
				return;
			}
			dpu_pr_info("[DP] DMD_DP_DFX: tmpstr str : %s, cnt: %d", tmpstr, dptx->err_logs_stc.error_count);
			if (strcat_s(dptx->logstr, DMD_ERR_LEN, tmpstr) != 0) {
				dpu_pr_err("[DP] dmd logstr create failed, tmpstr:%s, logstr:%s,", tmpstr, dptx->logstr);
				return;
			}
		}
		dpu_pr_info("[DP] DMD_DP_DFX: logstr str : %s, cnt: %u", dptx->logstr, dptx->err_logs_stc.error_count);
		if (strlen(dptx->logstr) > 0 && strlen(dptx->logstr) < DMD_ERR_LEN)
			dptx->logstr[strlen(dptx->logstr) - 1] = '\0';
	}

	if (sprintf_s(dptx->poststr, DMD_STR_LEN, "portid:%d;dfx_type:%d;error:%d;mode:%llu;in_times:%u;out_times:%u;\
hold_on:%lu;rate=%u;lanes=%u;xres:%d;yres:%d;hbp=%d;hfp:%d;hpw:%d;vbp:%d;vfp:%d;vpw:%d;hplr:%hhu;vplr:%hhu;pcr_div=%d;\
pc_rate:%llu;fps:%hhu;dsc:%d;fec:%d;dpcd_rev:%x;delay:%d;logstr:%s;",
			dptx->port_id, dfx_type, error_type, dptx->mode, dptx->dmd_connect_cnt, dptx->dmd_disconnect_cnt,
			connect_jiffies, dptx->link.rate, dptx->link.lanes,
			dptx->connector->conn_info->base.xres, dptx->connector->conn_info->base.yres,
			dptx->connector->ldi.h_back_porch, dptx->connector->ldi.h_front_porch, dptx->connector->ldi.h_pulse_width,
			dptx->connector->ldi.v_back_porch, dptx->connector->ldi.v_front_porch, dptx->connector->ldi.v_pulse_width,
			dptx->connector->ldi.hsync_plr, dptx->connector->ldi.vsync_plr, dptx->connector->ldi.pxl_clk_rate_div,
			dptx->connector->ldi.pxl_clk_rate, dptx->vparams.m_fps, dptx->dsc, dptx->dptx_link_params.compress_params.fec,
			dptx->dpcd_rev, g_dp_dmd_delay_time, dptx->logstr) <= 0) {
		dpu_pr_err("[DP] dmd poststr create failed");
		return;
	}

	dpu_pr_info("[DP] DMD_DP_DFX: dmd str : %s", dptx->poststr);
	if (dfx_type != DMD_DP_FAILED) {
		if (connect_jiffies > 0)
			bbox_diaginfo_record(DMD_SOC_DSS_DP_INFO, NULL, dptx->poststr);
		dptx->dmd_disconnect_cnt = 0;
		dptx->dmd_connect_cnt = 0;
		dptx->update_dmd_time = current_ms;
        dpu_pr_info("[DP] set update_dmd_time: %lu", dptx->update_dmd_time);
		return;
	}
	bbox_diaginfo_record(DMD_SOC_DSS_DP_ERROR, NULL, dptx->poststr);
}

void dp_dmd_print_dfx(struct dp_ctrl *dptx, int dfx_type, int error_type)
{
	uint64_t current_ms = 0;
	current_ms = (uint64_t)ktime_to_ms(ktime_get());

	if (dfx_type == DMD_DP_CONNECT && dptx->error_type == DMD_DP_FAILED_SAFEMODE)
		dfx_type = DMD_DP_FAILED;
	else if (dfx_type == DMD_DP_FAILED && dptx->error_type == DMD_DP_SUCCESS)
		error_type = DMD_DP_FAILED_COMMON;

	mutex_lock(&dptx->dptx_dmd_mutex);
	if (dptx->update_dmd_time == 0) {
		dptx->update_dmd_time = current_ms;
        dpu_pr_info("[DP] set update_dmd_time: %lu", dptx->update_dmd_time);
    }

	dpu_pr_info("[DP] DMD_DP_DFX:[%d-%d] dptx->update_dmd_time: %lu, current_ms: %lu, delay: %d",
		dfx_type, error_type, dptx->update_dmd_time, current_ms, g_dp_dmd_delay_time);

	if (dfx_type == DMD_DP_CONNECT) {
		dptx->dmd_connect_cnt++;
		if (current_ms - dptx->update_dmd_time < g_dp_dmd_delay_time) {
			mutex_unlock(&dptx->dptx_dmd_mutex);
			return;
		}
	} else if (dfx_type == DMD_DP_DISCONNECT) {
		dptx->dmd_disconnect_cnt++;
		if (current_ms - dptx->update_dmd_time < g_dp_dmd_delay_time) {
			if(dptx->plugin_jiffies > 0 && jiffies > dptx->plugin_jiffies)
				dptx->holdon_time += (jiffies - dptx->plugin_jiffies) / HZ;
			mutex_unlock(&dptx->dptx_dmd_mutex);
			return;
		}
	}

	// upload dmd report.
	dp_bbox_upload_record(dptx, dfx_type, current_ms, error_type);

	// reset had lock of dmd_mutex, so unlock first.
	mutex_unlock(&dptx->dptx_dmd_mutex);
	dp_reset_error_log(dptx);
}
