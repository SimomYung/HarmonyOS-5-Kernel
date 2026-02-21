/*
 * npu_udk.c
 *
 * about ldk_npu to udk communicate
 *
 * Copyright (c) 2012-2024 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#include "npu_udk.h"

#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <liblinux/pal.h>
#include <securec.h>
#include "sd_proxy.h"
#include "npu_common.h"
#include "npu_log.h"
#include "npu_except_mgr.h"
#ifdef CONFIG_FFRT_FFTS_OHOS
#include "ffts_black_box.h"
#endif

int npd_dump_exception_without_power_check(struct npu_exception_record *er)
{
	int ret = -1;
	int invoke_ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_NPU_MAJOR_CMD,
		UDK_MINOR_CMD_NPU_DUMP_EXCEPTION), er, sizeof(struct npu_exception_record), &ret, sizeof(int));
	if (invoke_ret != 0) {
		npu_drv_err("proxy npu dump exception failed, invoke_ret = %d\n", invoke_ret);
		return invoke_ret;
	}
	return ret;
}

int npu_exception_aicore_reset(u16 channel_id)
{
	int ret = -1;
	int proxy_ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_NPU_MAJOR_CMD,
		UDK_MINOR_CMD_NPU_HANDLE_TS_EXCEPTION), &channel_id, sizeof(u16), &ret, sizeof(int));
	if (proxy_ret != 0) {
		npu_drv_err("proxy npu exception aicore reset failed, proxy_ret = %d\n", proxy_ret);
		return proxy_ret;
	}
	return ret;
}

#ifdef CONFIG_NPU_SWTS
void hwts_driver_start_exec_swap_buff_cfg(struct hwts_sq_cq *sqcq_info, uint16_t channel_id)
{
	struct npu_hwts_schedule_sq swap_buff = {0};

	memcpy_s(&swap_buff.sqcq_info, sizeof(struct hwts_sq_cq), sqcq_info, sizeof(struct hwts_sq_cq));
	swap_buff.channel_id = channel_id;
 
	int proxy_ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_NPU_MAJOR_CMD,
		UDK_MINOR_CMD_NPU_HWTS_SCHEDULE_SQ), &swap_buff, sizeof(struct npu_hwts_schedule_sq), NULL, 0);
	if (proxy_ret != 0) {
		npu_drv_err("proxy npu hwts driver swap buff configuration failed, proxy_ret = %d\n", proxy_ret);
	}
	return;
}

void hwts_driver_start_exec_dfx_cfg(struct hwts_sq_cq *sqcq_info, uint16_t channel_id,bool log_en, bool profiling_en)
{
	struct npu_hwts_schedule_sq dfx_cfg = {0};

	memcpy_s(&dfx_cfg.sqcq_info, sizeof(struct hwts_sq_cq), sqcq_info, sizeof(struct hwts_sq_cq));
	dfx_cfg.channel_id = channel_id;
	dfx_cfg.log_en = log_en;
    dfx_cfg.profiling_en = profiling_en;
 
	int proxy_ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_NPU_MAJOR_CMD,
		UDK_MINOR_CMD_NPU_HWTS_DFX_CFG), &dfx_cfg, sizeof(struct npu_hwts_schedule_sq), NULL, 0);
	if (proxy_ret != 0) {
		npu_drv_err("proxy npu hwts driver dfx configuration failed, proxy_ret = %d\n", proxy_ret);
	}
	return;
}
#endif

int npu_hwts_query_sqe_type(u16 channel_id, u8 *reg_val)
{
	u8 sqe_type = NPU_HWTS_SQE_TYPE_RESV;
	int proxy_ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_NPU_MAJOR_CMD,
		UDK_MINOR_CMD_NPU_HWTS_QUERY_SQE_TYPE), &channel_id, sizeof(u16), &sqe_type, sizeof(u8));
	if (proxy_ret != 0) {
		npu_drv_err("proxy npu hwts query sqe type failed, proxy_ret = %d\n", proxy_ret);
		return proxy_ret;
	}

	*reg_val = sqe_type;
	return 0;
}

#ifdef CONFIG_NPU_FFRT
#ifdef CONFIG_NPU_FFRT_V100
int ffts_check_global_block_state(ffts_hw_type hw_type)
{
	int ret = 0;
	int glb_state = -1;

	npu_drv_debug("ffts_check_global_block_state start\n");
	ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_FFTS_MAJOR_CMD,
		CMD_FFTS_CHECK_GROBAL_BLOCK_STATE), &hw_type, sizeof(uint32_t), &glb_state, sizeof(int));
	if (ret != 0) {
		npu_drv_err("ioctl ffts_check_global_block_state failed, ret = %d\n", ret);
		return ret;
	}
	return glb_state;
}

uint32_t ffts_check_hw_block_state(ffts_hw_type hw_type)
{
	int ret = 0;
	uint32_t hw_state = UINT32_MAX;

	npu_drv_debug("ffts_check_hw_block_state start\n");
	ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_FFTS_MAJOR_CMD,
		CMD_FFTS_CHECK_HW_BLOCK_STATE), &hw_type, sizeof(uint32_t), &hw_state, sizeof(uint32_t));
	if (ret != 0) {
		npu_drv_err("ioctl ffts_check_hw_block_state failed, ret = %d\n", ret);
		return UINT32_MAX;
	}
	return hw_state;
}

void ffts_hw_block_dis(ffts_hw_type hw_type)
{
	npu_drv_debug("ffts_hw_block_dis start\n");
	(void)liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_FFTS_MAJOR_CMD,
		CMD_FFTS_BLOCK_DISABLE), &hw_type, sizeof(uint32_t), NULL, 0);
	return;
}

int ffts_hw_block_en(ffts_hw_type hw_type)
{
	int ret = 0;
	int glb_state = -1;
	ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_FFTS_MAJOR_CMD,
		CMD_FFTS_BLOCK_ENABLE), &hw_type, sizeof(uint32_t), &glb_state, sizeof(int));
	if (ret != 0) {
		npu_drv_err("ioctl ffts_hw_block_en failed, ret = %d\n", ret);
		return ret;
	}
	return glb_state;
}

void ffts_insert_global_excp_info(uint16_t uid, uint16_t modelid, uint16_t taskid)
{
	int ret = 0;
	struct npu_block_ffts_global_excp_info global_excp_info  = { 0 };

	npu_drv_debug("ffts_insert_global_excp_info start\n");
	global_excp_info.ssid = uid;
	global_excp_info.modelid = modelid;
	global_excp_info.taskidx = taskid;
	ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_FFTS_MAJOR_CMD,
		CMD_FFTS_INSERT_EXCP_INFO), &global_excp_info, sizeof(struct npu_block_ffts_global_excp_info), NULL, 0);
	if (ret != 0) {
		npu_drv_err("ioctl ffts_insert_global_excp_info failed, ret = %d\n", ret);
	}
	return;
}

int npu_proc_block_kill_task(u16 uid)
{
	int ret = -1;
	int proxy_ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_NPU_MAJOR_CMD,
		UDK_MINOR_CMD_NPU_HANDLE_CLEAR_BLOCK_RESOURCE), &uid, sizeof(uid), &ret, sizeof(int));
	if (proxy_ret != 0) {
		npu_drv_err("proxy npu handle clear block resource failed, proxy_ret = %d\n", proxy_ret);
		return proxy_ret;
	}
	return ret;
}

int npu_ffts_clear_entry(uint16_t task_idx, uint16_t entry_id)
{
	int ret = -1;
	struct npu_ffrt_task_entry_info task_entry_info = {0};

	task_entry_info.task_id = task_idx;
	task_entry_info.entry_id = entry_id;

	int proxy_ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_NPU_MAJOR_CMD,
		UDK_MINOR_CMD_NPU_HWTS_CLEAR_ENTRY), &task_entry_info,
		sizeof(struct npu_ffrt_task_entry_info), &ret, sizeof(int));
	if (proxy_ret != 0) {
		npu_drv_err("proxy npu clear entry failed, proxy_ret = %d\n", proxy_ret);
		return proxy_ret;
	}
	return ret;
}
#endif

int npu_handle_ffts_exception(struct ffts_exception_report_info *report)
{
	int ret = -1;
	int proxy_ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_NPU_MAJOR_CMD,
		UDK_MINOR_CMD_NPU_HANDLE_FFTS_EXCEPTION), report, sizeof(struct ffts_exception_report_info), &ret, sizeof(int));
	if (proxy_ret != 0) {
		npu_drv_err("proxy npu handle ffts exception failed, proxy_ret = %d\n", proxy_ret);
		return proxy_ret;
	}
	return ret;
}

int npu_handle_global_exception_for_ffts(void)
{
	int ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_NPU_MAJOR_CMD,
		UDK_MINOR_CMD_NPU_HANDLE_GLOBAL_EXCEPTION), NULL, 0, NULL, 0);
	if (ret != 0) {
		npu_drv_err("proxy npu handle global exception for ffts failed, ret = %d\n", ret);
	}
	return ret;
}

int npu_clear_hts_event_tbl(uint32_t event_id)
{
	int ret = -1;
	int proxy_ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_NPU_MAJOR_CMD,
		UDK_MINOR_CMD_NPU_CLEAR_HTS_EVENT_TBL), &event_id, sizeof(uint32_t), &ret, sizeof(int));
	if (proxy_ret != 0) {
		npu_drv_err("proxy npu clear hts event tbl failed, proxy_ret = %d\n", proxy_ret);
		return proxy_ret;
	}
	return ret;
}

int npu_ffrt_load_model_buf(struct npu_ffrt_load_model_buf_info *model_desc)
{
	int ret;
	ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_NPU_MAJOR_CMD, UDK_MINOR_CMD_NPU_FFTS_LOAD_MODEL_BUF),
		model_desc, sizeof(struct npu_ffrt_load_model_buf_info), NULL, 0);
	if (ret != 0) {
		npu_drv_warn("UDK_MINOR_CMD_NPU_FFTS_LOAD_MODEL_BUF rerutn fail !, ret = %d\n", ret);
	}
	return ret;
}

struct npu_ffrt_model_ssid_info {
	uint32_t model_id;
	uint16_t ssid;
};

int npu_ffrt_set_ssid(uint32_t model_id, uint16_t ssid)
{
	int ret = -1;
	struct npu_ffrt_model_ssid_info model_ssid_info = {0};

	model_ssid_info.model_id = model_id;
	model_ssid_info.ssid = ssid;

	int proxy_ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_NPU_MAJOR_CMD,
		UDK_MINOR_CMD_NPU_HWTS_SET_SSID), &model_ssid_info,
		sizeof(struct npu_ffrt_model_ssid_info), &ret, sizeof(int));
	if (proxy_ret != 0) {
		npu_drv_err("proxy npu set id failed, proxy_ret = %d\n", proxy_ret);
		return proxy_ret;
	}
	return ret;
}

int npu_ffts_entry_is_clear(void)
{
	int ret = -1;
	int proxy_ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_NPU_MAJOR_CMD,
		UDK_MINOR_CMD_NPU_FFTS_ENTRY_IS_CLEAR), NULL, 0, &ret, sizeof(ret));
	if (proxy_ret != 0) {
		npu_drv_warn("UDK_MINOR_CMD_NPU_FFTS_ENTRY_IS_CLEAR rerutn fail !, proxy_ret = %d\n", proxy_ret);
	}
	return ret;
}
#endif

#ifdef CONFIG_HIBERNATION
int npu_s4_rsv_ddr_backup(void)
{
	int ret;

	ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_NPU_MAJOR_CMD,
		UDK_MINOR_CMD_NPU_S4_RSV_DDR_BACKUP), NULL, 0, NULL, 0);
	if (ret != 0) {
		npu_drv_err("UDK_MINOR_CMD_NPU_S4_RSV_DDR_BACKUP failed, ret = %d\n", ret);
	}
	return ret;
}

int npu_s4_rsv_ddr_recover(void)
{
	int ret;

	ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_NPU_MAJOR_CMD,
		UDK_MINOR_CMD_NPU_S4_RSV_DDR_RECOVER), NULL, 0, NULL, 0);
	if (ret != 0) {
		npu_drv_err("UDK_MINOR_CMD_NPU_S4_RSV_DDR_RECOVER failed, ret = %d\n", ret);
	}
	return ret;
}
#endif

void npu_udk_bypass(void)
{
	int ret = liblinux_pal_proxy_udk_invoke(PAL_PROXY_COMBINE_CMD(UDK_NPU_MAJOR_CMD,
		UDK_MINOR_CMD_NPU_UDK_BYPASS), NULL, 0, NULL, 0);
	if (ret != 0) {
		npu_drv_err("UDK_MINOR_CMD_NPU_UDK_BYPASS failed, ret = %d\n", ret);
	}
}