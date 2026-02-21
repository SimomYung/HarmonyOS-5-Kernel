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
#include <linux/delay.h>
#include <dpu/dpu_dm.h>
#include <dpu/soc_dpu_define.h>
#include <linux/interrupt.h>
#include <platform_include/basicplatform/linux/dfx_bbox_diaginfo.h>
#include "dpu/dpu_base_addr.h"
#include "ukmd_listener.h"
#include "dpu_comp_mgr.h"
#include "dpu_conn_mgr.h"
#include "dpu_comp_vactive.h"
#include "dpu_comp_abnormal_handle.h"
#include "dpu_comp_smmu.h"
#include "dpu_opr_config.h"
#include "dpu_dacc.h"
#include "dpu_config_utils.h"
#include "cmdlist_interface.h"
#include "dksm_debug.h"
#include "dpu_cmdlist.h"
#include "mipi_dsi_dev.h"
#include "dkmd_rect.h"
#include "dkmd_base_layer.h"
#include "opr_mgr/operators/opr_format.h"
#include "dpu_debug_dump.h"
#include "dksm_dmd.h"
#include "dpu_comp_config_utils.h"
#include "dpu_ppc_status_control.h"
#include "ddr_dvfs.h"
#include "dvfs.h"
#include "dpu_mntn.h"


struct dpu_dsm_record_info g_dpu_dsm_info;

static int32_t dpu_comp_handle_underflow_clear(struct dpu_composer *dpu_comp)
{
	uint32_t ret = 0;
	uint32_t displaying_scene_id = 0;
	uint32_t displayed_scene_id = 0;
	struct dkmd_connector_info *pinfo = NULL;
	struct comp_online_present *present = NULL;

    /* skip underflow clear, goto reset hardware */
    if (g_debug_dpu_reset_hardware != 0) {
        g_debug_dpu_reset_hardware = 0;
        return -1;
    }

	pinfo = dpu_comp->conn_info;
	present = (struct comp_online_present *)dpu_comp->present_data;

	/* 1. after disable ldi and set clear, wait 16ms */
	dpu_pr_info("underflow clear start!");
	dpu_comp_set_glb_rs_reg(dpu_comp, true);

	mdelay(16);
 	/* 2. enable dacc_crg clr_rsicv_start */
	if (present->displaying_idx < COMP_FRAME_MAX &&  present->displayed_idx < COMP_FRAME_MAX) {
		displaying_scene_id = (uint32_t)present->frames[present->displaying_idx].in_frame.scene_id;
		displayed_scene_id = (uint32_t)present->frames[present->displayed_idx].in_frame.scene_id;
	}

	ret |= dpu_dacc_handle_clear(dpu_comp->comp_mgr->dpu_base, displaying_scene_id);
	if (displaying_scene_id != displayed_scene_id)
		ret |= dpu_dacc_handle_clear(dpu_comp->comp_mgr->dpu_base, displayed_scene_id);

	dpu_comp_set_glb_rs_reg(dpu_comp, false);

	composer_mgr_disable_dimming(dpu_comp->comp_mgr, dpu_comp->comp.index);

	/* 3. check it in vactive_end intr */
	if (ret == 0)
		ret = (uint32_t)pipeline_next_ops_handle(pinfo->conn_device, pinfo,
												 DO_CLEAR, (void *)dpu_comp->comp_mgr->dpu_base);

	if (is_ppc_status_resume(&present->ppc_ctrl) && (dpu_comp->comp.set_active_rect)) {
		process_ppc_event(dpu_comp, PPC_EVENT_UNDERFLOW_CLEAR);
		if (dpu_comp->comp.set_active_rect(&dpu_comp->comp, pinfo->ppc_config_id_record) != 0)
			dpu_pr_warn("set active rect record ppc config id %u failed", pinfo->ppc_config_id_record);
	}

	if (!is_offline_panel(&dpu_comp->conn_info->base)) {
		present->vactive_end_flag = 0;
		present->frame_start_flag = 0;
		if ((dpu_comp->comp.comp_err_status & ABNORMAL_WAIT_VACTIVE_TIMEOUT) != 0) {
			present->vactive_start_flag = 1;
			if (dpu_comp->multi_present_ctrl.ops && dpu_comp->multi_present_ctrl.ops->init_param)
				dpu_comp->multi_present_ctrl.ops->init_param(&(dpu_comp->multi_present_ctrl));
		}
	}

	dpu_backlight_update(&dpu_comp->bl_ctrl, true);
	dpu_pr_info("update backlight: %d", dpu_comp->bl_ctrl.bl_level);

	dpu_ops_deep_sleep_enter(dpu_comp);

	return (int32_t)ret;
}

static void dpu_comp_abnormal_dump_cmdlist_status(char __iomem *dpu_base, uint32_t scene_id)
{
	char __iomem *module_base = dpu_base + DPU_CMDLIST_OFFSET;

	g_dpu_dsm_info.cmdlist_status = inp32(DPU_CMD_CMDLIST_CH_INTS_ADDR(module_base, scene_id));
	dpu_pr_warn("CMDLIST:\n"
		"\tCMDLIST_DBG=%#x\tSCE0_CFG_SIGNAL=%#x\tSCE1_CFG_SIGNAL=%#x\tSCE2_CFG_SIGNAL=%#x\n\n"
		"\tSCE3_CFG_SIGNAL=%#x\tSCE4_CFG_SIGNAL=%#x\tSCE5_CFG_SIGNAL=%#x\tSCE6_CFG_SIGNAL=%#x\n\n",
		inp32(DPU_CMD_CMDLIST_DBG_ADDR(module_base)), inp32(DPU_CMD_SCE0_CFG_SIGNAL_ADDR(module_base)),
		inp32(DPU_CMD_SCE1_CFG_SIGNAL_ADDR(module_base)), inp32(DPU_CMD_SCE2_CFG_SIGNAL_ADDR(module_base)),
		inp32(DPU_CMD_SCE3_CFG_SIGNAL_ADDR(module_base)), inp32(DPU_CMD_SCE4_CFG_SIGNAL_ADDR(module_base)),
		inp32(DPU_CMD_SCE5_CFG_SIGNAL_ADDR(module_base)), inp32(DPU_CMD_SCE6_CFG_SIGNAL_ADDR(module_base)));
	dpu_pr_warn("CMDLIST:\n"
		"\tCMDLIST_CH_CURAD=%#x\n\n",
		inp32(DPU_CMD_CMDLIST_CH_CURAD_ADDR(module_base, scene_id)));
	dpu_pr_warn("CMDLIST:\n\n"
		"CMDLIST_CH_STATUS=%#x\t\tCMDLIST_CH_INTS=%#x\n\n",
		inp32(DPU_CMD_CMDLIST_CH_STATUS_ADDR(module_base, scene_id)),
		g_dpu_dsm_info.cmdlist_status);
}


void dpu_comp_abnormal_dump_reg_dm(char __iomem *dpu_base, uint32_t scene_id)
{
	uint32_t i = 0;
	uint32_t inc_step = 4;
	uint32_t val1, val2, val3, val4;
	uint32_t *dm_addr = NULL;
	uint32_t dpu_addr_offset = 0;

	if (scene_id >= DPU_SCENE_MAX) {
		dpu_pr_err("scene_id is err %u", scene_id);
		return;
	}
	dm_addr = (uint32_t *)(dpu_base + g_dm_tlb_info[scene_id].dm_data_addr);
	dpu_addr_offset = g_dm_tlb_info[scene_id].dm_data_addr;

	dpu_pr_err("DM reg dump begin(scene_id:%d):", scene_id);

	for (; i < ((DM_LAYER_HEIGHT_OFFSET + INPUT_LAYER_OFFSET * INPUT_LAYER_CNT) / (inc_step * 4)); i++) {
		val1 = inp32(dm_addr++);
		val2 = inp32(dm_addr++);
		val3 = inp32(dm_addr++);
		val4 = inp32(dm_addr++);
		dpu_pr_err("dpu_addr_offset:%#x val:0x%08x 0x%08x 0x%08x 0x%08x \n", dpu_addr_offset, val1, val2, val3, val4);
		dpu_addr_offset += inc_step * 4;
	}
}

static void dpu_comp_abnormal_dump_opr_rxx_wxx(char* opr, uint32_t opr_idx,
	char __iomem *read_base, char __iomem *write_base)
{
	uint32_t rxx_ycnt, wxx_ycnt;

	if (read_base != NULL) {
		/* rxx_ycnt: R_LB_DEBUG2.bit13~25 */
		rxx_ycnt = inp32(read_base + 0x8);
		rxx_ycnt &= (BIT(26) - 1);
		rxx_ycnt >>= 13;
	}

	if (write_base != NULL) {
		/* wxx_ycnt: W_LB_DEBUG1.bit19~31 */
		wxx_ycnt = inp32(write_base + 0x4);
		wxx_ycnt >>= 19;
	}

	if (read_base != NULL && write_base != NULL) {
		dpu_pr_warn("%s[%u]:rxx_ycnt=%u, wxx_ycnt=%u\n"
			"\tR_LB_DEBUG0=%#x\t\tR_LB_DEBUG1=%#x\t\tR_LB_DEBUG2=%#x\t\tR_LB_DEBUG3=%#x\n"
			"\tW_LB_DEBUG0=%#x\t\tW_LB_DEBUG1=%#x\t\tW_LB_DEBUG2=%#x\t\tW_LB_DEBUG3=%#x\n\n",
			opr, opr_idx, rxx_ycnt, wxx_ycnt,
			inp32(read_base), inp32(read_base + 0x4), inp32(read_base + 0x8), inp32(read_base + 0xc),
			inp32(write_base), inp32(write_base + 0x4), inp32(write_base + 0x8), inp32(write_base + 0xc));
		return;
	}

	if (read_base != NULL) {
		dpu_pr_warn("%s[%u]:rxx_ycnt=%u\n"
			"\tR_LB_DEBUG0=%#x\t\tR_LB_DEBUG1=%#x\t\tR_LB_DEBUG2=%#x\t\tR_LB_DEBUG3=%#x\n\n", opr, opr_idx, rxx_ycnt,
			inp32(read_base), inp32(read_base + 0x4), inp32(read_base + 0x8), inp32(read_base + 0xc));
		return;
	}

	if (write_base != NULL) {
		dpu_pr_warn("%s[%u]:wxx_ycnt=%u\n"
			"\tW_LB_DEBUG0=%#x\t\tW_LB_DEBUG1=%#x\t\tW_LB_DEBUG2=%#x\t\tW_LB_DEBUG3=%#x\n\n", opr, opr_idx, wxx_ycnt,
			inp32(write_base), inp32(write_base + 0x4), inp32(write_base + 0x8), inp32(write_base + 0xc));
		return;
	}
}

static void dpu_abnormal_dump_peri_voltage(void)
{
	char __iomem *pmctrl_base = dpu_config_get_ip_base(DISP_IP_BASE_PMCTRL);
	dpu_check_and_no_retval(!pmctrl_base, err, "pmctrl_base is NULL!\n");

	dpu_pr_debug("curr peri stat %#x", inp32(SOC_PMCTRL_PERI_CTRL5_ADDR(pmctrl_base)));
}

void dpu_offline_debug_dump(struct dpu_composer *dpu_comp)
{
	dpu_pr_warn("isr1_offline_glb_state =%#x\tisr2_offline_state=%#x\tisr2_offline_state=%#x",
		inp32(DPU_GLB_NS_OFFLINE0_TO_GIC_O_ADDR(dpu_comp->comp_mgr->dpu_base + DPU_GLB0_OFFSET)),
		inp32(DPU_GLB_WCH1_NS_INT_O_ADDR(dpu_comp->comp_mgr->dpu_base + DPU_GLB0_OFFSET)),
		inp32(DPU_GLB_WCH2_NS_INT_O_ADDR(dpu_comp->comp_mgr->dpu_base + DPU_GLB0_OFFSET)));
}

void dpu_comp_abnormal_debug_dump(struct dpu_composer *dpu_comp, uint32_t scene_id)
{
	uint32_t i;
	char __iomem *module_base = NULL;

	if (!dpu_comp) {
		dpu_pr_err("dpu_comp is null");
		return;
	}
	module_base = dpu_comp->comp_mgr->dpu_base + DPU_GLB0_OFFSET;

	dpu_pr_warn("scene_id=%u ip_status: DBG_IP_STATE0=%#x DBG_IP_STATE1=%#x", scene_id,
		inp32(DPU_GLB_DBG_IP_STATE0_ADDR(module_base)), inp32(DPU_GLB_DBG_IP_STATE1_ADDR(module_base)));

	dpu_print_dvfs_vote_status();
	dpu_abnormal_dump_peri_voltage();
	dpu_print_curr_ddr_freq();
	dpu_comp_abnormal_dump_ov_info(dpu_comp->comp_mgr->dpu_base);

	module_base = dpu_comp->comp_mgr->dpu_base + DPU_SDMA_OFFSET;
	dpu_comp_abnormal_dump_sdma_info(module_base);

	for (i = 0; i < OPR_CLD_NUM; ++i) {
		module_base = dpu_comp->comp_mgr->dpu_base + g_cld_offset[i];
		dpu_comp_abnormal_dump_opr_rxx_wxx("CLD", i,
			DPU_CLD_R_LB_DEBUG0_ADDR(module_base), DPU_CLD_W_LB_DEBUG0_ADDR(module_base));
	}

	for (i = 0; i < OPR_VSCF_NUM; ++i) {
		module_base = dpu_comp->comp_mgr->dpu_base + g_vscf_offset[i];
		dpu_comp_abnormal_dump_opr_rxx_wxx("VSCF", i,
			DPU_VSCF_R_LB_DEBUG0_ADDR(module_base), DPU_VSCF_W_LB_DEBUG0_ADDR(module_base));
	}

	for (i = 0; i < OPR_ARSR_NUM; ++i) {
		module_base = dpu_comp->comp_mgr->dpu_base + g_arsr_offset[i];
		dpu_comp_abnormal_dump_opr_rxx_wxx("ARSR", i,
			DPU_ARSR_R_LB_DEBUG0_ADDR(module_base), DPU_ARSR_W_LB_DEBUG0_ADDR(module_base));
	}

#ifdef DPU_UVUP_OFFSET
	module_base = dpu_comp->comp_mgr->dpu_base + DPU_UVUP_OFFSET;
	dpu_comp_abnormal_dump_opr_rxx_wxx("UVUP", 0,
		DPU_UVSAMP_R_LB_DEBUG0_ADDR(module_base), DPU_UVSAMP_W_LB_DEBUG0_ADDR(module_base));
#endif

	module_base = dpu_comp->comp_mgr->dpu_base + DPU_HDR_OFFSET;
	dpu_comp_abnormal_dump_opr_rxx_wxx("HDR", 0,
		DPU_HDR_R_LB_DEBUG0_ADDR(module_base), DPU_HDR_W_LB_DEBUG0_ADDR(module_base));

	for (i = 0; i < OPR_DPP_NUM; ++i) {
		module_base = dpu_comp->comp_mgr->dpu_base + g_dpp_offset[i];
		dpu_comp_abnormal_dump_opr_rxx_wxx("DPP", i,
			DPU_DPP_R_LB_DEBUG0_ADDR(module_base), DPU_DPP_W_LB_DEBUG0_ADDR(module_base));
	}

	module_base = dpu_comp->comp_mgr->dpu_base + DPU_DSC0_OFFSET;
	dpu_comp_abnormal_dump_opr_rxx_wxx("DSC", 0,
		DPU_DSC_R_LB_DEBUG0_ADDR(module_base), DPU_DSC_W_LB_DEBUG0_ADDR(module_base));

	module_base = dpu_comp->comp_mgr->dpu_base + DPU_DBCU_OFFSET;
	dpu_pr_warn("DBCU:\n"
		"\tMONITOR_OS_R0=%#x\t\tMONITOR_OS_R1=%#x\t\tMONITOR_OS_R2=%#x\t\tMONITOR_OS_R3=%#x\t\tMONITOR_OS_R4=%#x\n"
		"\tMONITOR_OS_W0=%#x\t\tMONITOR_OS_W1=%#x\t\tMONITOR_OS_W2=%#x\t\tMONITOR_OS_W3=%#x\n"
		"\tAIF_STAT_0=%#x\t\tAIF_STAT_1=%#x\t\tAIF_STAT_2=%#x\t\tAIF_STAT_3=%#x\n"
		"\tAIF_STAT_4=%#x\t\tAIF_STAT_5=%#x\t\tAIF_STAT_6=%#x\t\tAIF_STAT_7=%#x\n"
		"\tAIF_STAT_8=%#x\t\tAIF_STAT_9=%#x\t\tAIF_STAT_10=%#x\t\tAIF_STAT_11=%#x\n"
		"\tAIF_STAT_12=%#x\t\tAIF_STAT_13=%#x\t\tAIF_STAT_14=%#x\t\tAIF_STAT_15=%#x\n\n",
		inp32(DPU_DBCU_MONITOR_OS_R0_ADDR(module_base)), inp32(DPU_DBCU_MONITOR_OS_R1_ADDR(module_base)),
		inp32(DPU_DBCU_MONITOR_OS_R2_ADDR(module_base)), inp32(DPU_DBCU_MONITOR_OS_R3_ADDR(module_base)),
		inp32(DPU_DBCU_MONITOR_OS_R4_ADDR(module_base)),
		inp32(DPU_DBCU_MONITOR_OS_W0_ADDR(module_base)), inp32(DPU_DBCU_MONITOR_OS_W1_ADDR(module_base)),
		inp32(DPU_DBCU_MONITOR_OS_W2_ADDR(module_base)), inp32(DPU_DBCU_MONITOR_OS_W3_ADDR(module_base)),
		inp32(DPU_DBCU_AIF_STAT_0_ADDR(module_base)), inp32(DPU_DBCU_AIF_STAT_1_ADDR(module_base)),
		inp32(DPU_DBCU_AIF_STAT_2_ADDR(module_base)), inp32(DPU_DBCU_AIF_STAT_3_ADDR(module_base)),
		inp32(DPU_DBCU_AIF_STAT_4_ADDR(module_base)), inp32(DPU_DBCU_AIF_STAT_5_ADDR(module_base)),
		inp32(DPU_DBCU_AIF_STAT_6_ADDR(module_base)), inp32(DPU_DBCU_AIF_STAT_7_ADDR(module_base)),
		inp32(DPU_DBCU_AIF_STAT_8_ADDR(module_base)), inp32(DPU_DBCU_AIF_STAT_9_ADDR(module_base)),
		inp32(DPU_DBCU_AIF_STAT_10_ADDR(module_base)), inp32(DPU_DBCU_AIF_STAT_11_ADDR(module_base)),
		inp32(DPU_DBCU_AIF_STAT_12_ADDR(module_base)), inp32(DPU_DBCU_AIF_STAT_13_ADDR(module_base)),
		inp32(DPU_DBCU_AIF_STAT_14_ADDR(module_base)), inp32(DPU_DBCU_AIF_STAT_15_ADDR(module_base)));

	if (scene_id <= DPU_SCENE_ONLINE_3) {
		module_base = dpu_comp->comp_mgr->dpu_base + g_itfsw_offset[scene_id];
		dpu_comp_abnormal_dump_opr_rxx_wxx("ITFSW", scene_id, DPU_ITF_CH_R_LB_DEBUG0_ADDR(module_base), NULL);
	}

	dpu_comp_abnormal_dump_dbuf(dpu_comp->comp_mgr->dpu_base, scene_id);
	dpu_comp_abnormal_dump_pll(dpu_comp);
	dpu_comp_abnormal_dump_lbuf(dpu_comp->comp_mgr->dpu_base);
	dpu_comp_abnormal_dump_dacc(dpu_comp->comp_mgr->dpu_base, scene_id);
	dpu_comp_abnormal_dump_cmdlist_status(dpu_comp->comp_mgr->dpu_base, scene_id);
}

static void dpu_comp_abnormal_notify_ldi_status_invalid(struct dpu_composer *dpu_comp, int32_t ldi_status)
{
	dpu_check_and_no_retval(!dsm_lcd_client, info, "dsm_lcd_client is null!");

	if (dsm_client_ocuppy(dsm_lcd_client) == 0) {
		dsm_client_record(dsm_lcd_client, "ldi status %d is invalid", ldi_status);
		dsm_client_notify(dsm_lcd_client, DMD_DSS_DSI_FAULT);
		return;
	}

	dsm_client_unocuppy(dsm_lcd_client);
	usleep_range(500, 600);
	if (dsm_client_ocuppy(dsm_lcd_client) == 0) {
		dsm_client_record(dsm_lcd_client, "ldi status %d is invalid", ldi_status);
		dsm_client_notify(dsm_lcd_client, DMD_DSS_DSI_FAULT);
		return;
	}

	dpu_pr_warn("dsm_client_ocuppy failed");
}

void dpu_comp_reset_hardware_manually(struct dpu_composer *dpu_comp, struct comp_online_present *present)
{
	void_unused(dpu_comp);
    void_unused(present);
}

static void dpu_comp_pmic_abnormal_handle_work(struct kthread_work *work)
{
	struct comp_online_present *present = NULL;
	struct dpu_composer *dpu_comp = NULL;

	present = container_of(work, struct comp_online_present, pmic_abnormal_handle_work);
	if (unlikely(present == NULL)) {
		dpu_pr_err("present_data or dpu_comp is null");
		return;
	}

	dpu_comp = present->dpu_comp;
	if (dpu_comp == NULL) {
		dpu_pr_err("dpu_comp is null");
		return;
	}

	dpu_comp->pmic_abnormal_flag = true;
	composer_manager_power_down(dpu_comp);
	if (unlikely(dpu_comp_status_is_disable(&dpu_comp->comp_mgr->power_status))) {
		dpu_pr_info("already power off, do not need pmic_abnormal!");
		dpu_comp->pmic_abnormal_flag = false;
		composer_manager_power_up(dpu_comp);
		return;
	}

	composer_manager_reset_hardware(dpu_comp);
	composer_manager_power_up(dpu_comp);
	dpu_comp->pmic_abnormal_flag = false;
	dpu_pr_info("pmic_abnormal restore finish!!");
}

bool is_dp_video_primary_panel(const struct dkmd_object_info *pinfo)
{
	// V800 dp primary panel reset hardware when underflow
	if (g_dpu_config_data.version.info.version != DPU_ACCEL_DPUV800)
		return false;
	return is_dp_primary_panel(pinfo);
}

static void dpu_comp_abnormal_handle_work(struct kthread_work *work)
{
	int32_t ret;
	int32_t res;
	uint64_t tv0;
	struct comp_online_present *present = NULL;
	struct dpu_composer *dpu_comp = NULL;
	struct dpu_connector *connector = NULL;

	present = container_of(work, struct comp_online_present, abnormal_handle_work);
	if (!present || !present->dpu_comp) {
		dpu_pr_err("present_data or dpu_comp is null");
		return;
	}
	present->dfr_ctrl.present_buffer_num = 0;
	dpu_comp = present->dpu_comp;
	dpu_trace_ts_begin(&tv0);
	connector = get_primary_connector(dpu_comp->conn_info);
	if (!connector) {
		dpu_pr_err("primary connector is NULL!\n");
		return;
	}

	if (g_debug_dpu_clear_enable == 0) {
		dpu_pr_info("g_debug_dpu_clear_enable is false, no need to clear.");
		dpu_comp->comp.comp_err_status = 0;
		return ;
	}

	if ((dpu_comp->comp.comp_err_status & ABNORMAL_CHECK_LDI_FAILED) != 0) {
		dpu_pr_err("ldi status %d is invalid, notify to dsm", present->vactive_ldi_status);
		dpu_comp_abnormal_notify_ldi_status_invalid(dpu_comp, present->vactive_ldi_status);
		present->vactive_ldi_status = 0;
		dpu_comp->comp.comp_err_status &= ~ABNORMAL_CHECK_LDI_FAILED;
	}

	if (dpu_comp->comp.comp_err_status == 0)
		return;

	composer_manager_power_down(dpu_comp);
	if (unlikely(dpu_comp_status_is_disable(&dpu_comp->comp_mgr->power_status))) {
		dpu_pr_info("already power off, do not need handle underflow clear!");
		composer_manager_power_up(dpu_comp);
		return;
	}
	dpu_pr_warn("dpu underflow process!");
	dpu_comp_active_vsync(dpu_comp);

	// dump dss regs for mdfx
	dpu_mntn_report_event(DEF_EVENT_UNDER_FLOW, &(present->last_cmdlist_frm_info));

	if (g_debug_underflow_dump_enable) {
		memset_s(&g_dpu_dsm_info, sizeof(g_dpu_dsm_info), 0, sizeof(g_dpu_dsm_info));
		dpu_comp_abnormal_dump_reg_dsi(dpu_comp->comp_mgr->dpu_base);
		dpu_comp_abnormal_dump_reg_itf(dpu_comp->comp_mgr->dpu_base);
		dpu_comp_abnormal_debug_dump(dpu_comp, (uint32_t)present->frames[present->displayed_idx].in_frame.scene_id);
		dpu_comp_abnormal_dump_reg_dm(dpu_comp->comp_mgr->dpu_base,
					(uint32_t)present->frames[present->displayed_idx].in_frame.scene_id);
		dpu_parse_layer_info(dpu_comp->comp_mgr->dpu_base,
					(uint32_t)present->frames[present->displayed_idx].in_frame.scene_id);
		dpu_comp_abnormal_dsm_dump_reg_dm(dpu_comp,
					(uint32_t)present->frames[present->displayed_idx].in_frame.scene_id);
		/* lcd status */
		ret = is_mipi_cmd_panel(&dpu_comp->conn_info->base) && !is_fake_panel(&dpu_comp->conn_info->base);
		if (ret != 0) {
			g_dpu_dsm_info.lcd_status = (dpu_comp->conn_info->lcd_status_err == 0) ? 0 : 1;
			dpu_pr_debug("get lcd_status = %d", g_dpu_dsm_info.lcd_status);
		}
	}

	/* dump hardware cursor info */
	dpu_abnormal_dump_hardware_cursor(dpu_comp, present);
	/* dump panel info */
	(void)pipeline_next_ops_handle(dpu_comp->conn_info->conn_device, dpu_comp->conn_info, DUMP_EXCEPTION_INFO, NULL);

	dpu_comp->isr_ctrl.handle_func(&dpu_comp->isr_ctrl, UKMD_ISR_DISABLE);
	if (dpu_comp->conn_info->disable_ldi)
		dpu_comp->conn_info->disable_ldi(dpu_comp->conn_info);
	/* underflow dvfs:
		1.when underflow clear, dvfs unable to receive ov signal from dss
		2.dvfs unable to execute cmd，voltage mismatch between software and hardware
	*/
	dpu_comp_abnormal_dvfs(dpu_comp->comp.index);


	ret = dpu_comp_handle_underflow_clear(dpu_comp);
	if (ret != 0 || is_mipi_video_panel(&dpu_comp->conn_info->base) || is_dp_video_primary_panel(&dpu_comp->conn_info->base)) {
		dpu_pr_warn("underflow clear failed, reset hardware!!");
		g_dpu_dsm_info.underflow_clear_type = CLEAR_FAIL_RESET;
		dpu_comp_dsm_notify(dpu_comp, (uint32_t)present->frames[present->displayed_idx].in_frame.scene_id);
		dpu_comp_self_healing_process(dpu_comp);
		/* to balance isr_enable in reset hardware, we need enable isr here */
		dpu_comp->isr_ctrl.handle_func(&dpu_comp->isr_ctrl, UKMD_ISR_ENABLE);
		if (present->buffers > 0) {
			present->buffers--;
			dpu_pr_debug("unpresent buffer num is %d when underflow", present->buffers);
		}
		composer_manager_reset_hardware(dpu_comp);
		dpu_comp->isr_ctrl.handle_func(&dpu_comp->isr_ctrl, UKMD_ISR_DISABLE);
	} else {
		dpu_pr_warn("underflow clear successfully!");
		connector->enable_ser_vp_sync = true;
		dpu_pr_info("vp sync:%d", connector->enable_ser_vp_sync);
		ret = (dpu_comp->clear_succ_frame_index_queue[0] != 0) &&
			(dpu_comp->clear_succ_frame_index_queue[1] != 0) &&
			(dpu_comp->comp.comp_frame_index - dpu_comp->clear_succ_frame_index_queue[1] <= COMP_TIMEOUT_FRAME_COUNT);
		if (ret != 0) {
			dpu_pr_warn("underflow clear occurs continuously, need reset hardware");
			g_dpu_dsm_info.underflow_clear_type = CLEAR_SUC_BUT_RESET;
			dpu_comp_dsm_notify(dpu_comp, (uint32_t)present->frames[present->displayed_idx].in_frame.scene_id);
			dpu_comp_self_healing_process(dpu_comp);
			dpu_comp->isr_ctrl.handle_func(&dpu_comp->isr_ctrl, UKMD_ISR_ENABLE);
			composer_manager_reset_hardware(dpu_comp);
			dpu_comp->isr_ctrl.handle_func(&dpu_comp->isr_ctrl, UKMD_ISR_DISABLE);
			dpu_comp->clear_succ_frame_index_queue[0] = 0;
		} else {
			dpu_comp->clear_succ_frame_index_queue[1] = dpu_comp->clear_succ_frame_index_queue[0];
			dpu_comp->clear_succ_frame_index_queue[0] = dpu_comp->comp.comp_frame_index;
			g_dpu_dsm_info.underflow_clear_type = CLEAR_SUC;
			dpu_comp_dsm_notify(dpu_comp, (uint32_t)present->frames[present->displayed_idx].in_frame.scene_id);
		}
	}
	dpu_comp->isr_ctrl.handle_func(&dpu_comp->isr_ctrl, UKMD_ISR_ENABLE);
	dpu_comp->comp.comp_err_status = 0;

	dpu_comp_deactive_vsync(dpu_comp);
	composer_manager_power_up(dpu_comp);

	dpu_trace_ts_end(&tv0, "handle underflow clear");
}

static void dpu_comp_edp_abnormal_handle_work(struct kthread_work *work)
{
	struct comp_online_present *present = NULL;
	struct dpu_composer *dpu_comp = NULL;

	present = container_of(work, struct comp_online_present, edp_abnormal_handle_work);
	if (unlikely(present == NULL)) {
		dpu_pr_err("present_data or dpu_comp is null");
		return;
	}

	dpu_comp = present->dpu_comp;
	if (dpu_comp == NULL) {
		dpu_pr_err("dpu_comp is null");
		return;
	}

	composer_manager_power_down(dpu_comp);
	if (unlikely(dpu_comp_status_is_disable(&dpu_comp->comp_mgr->power_status))) {
		dpu_pr_info("already power off, do not need edp_abnormal!");
		composer_manager_power_up(dpu_comp);
		return;
	}

	composer_manager_reset_hardware(dpu_comp);
	composer_manager_power_up(dpu_comp);
	dpu_pr_info("edp_abnormal restore finish!!");
}

static int32_t dpu_comp_abnormal_handle_isr_notify(struct notifier_block *self, unsigned long action, void *data)
{
	uint32_t displaying_scene_id, displayed_scene_id;
	uint32_t displaying_frame_index, displayed_frame_index;
	uint32_t vstate;
	struct comp_online_present *present = NULL;
	struct dpu_connector *connector = NULL;
	struct ukmd_listener_data *listener_data = (struct ukmd_listener_data *)data;
	struct dpu_composer *dpu_comp = (struct dpu_composer *)(listener_data->data);
	int32_t lcd_status = 0;
	uint32_t phy_status = 0;

	dpu_check_and_return(!dpu_comp, -1, err, "dpu_comp is NULL!");

	mdfx_trace_begin("dpu_irq");

	connector = get_primary_connector(dpu_comp->conn_info);
	present = (struct comp_online_present *)dpu_comp->present_data;
	displaying_frame_index = present->frames[present->displaying_idx].in_frame.frame_index;
	displayed_frame_index = present->frames[present->displayed_idx].in_frame.frame_index;

	dpu_pr_warn("action=%#x enter, frame_index=%u, displaying_frame_index = %u, displayed_frame_index = %u,\
		dpp_dbg=0x%x, vcount=0x%x, vstate=0x%x, phy_status=0x%x",
		action, dpu_comp->comp.comp_frame_index,
		displaying_frame_index, displayed_frame_index,
		inp32(DPU_DPP_CH_DBG_CNT_ADDR(dpu_comp->comp_mgr->dpu_base + DPU_DPP0_OFFSET)),
		inp32(DPU_DSI_VCOUNT_ADDR(connector->connector_base)),
		inp32(DPU_DSI_VSTATE_ADDR(connector->connector_base)),
		inp32(DPU_DSI_CDPHY_STATUS_ADDR(connector->connector_base)));

	dpu_comp->isr_ctrl.unmask |= DSI_INT_UNDER_FLOW;

	displaying_scene_id = (uint32_t)(present->frames[present->displaying_idx].in_frame.scene_id);
	displayed_scene_id = (uint32_t)(present->frames[present->displayed_idx].in_frame.scene_id);

	dpu_comp_abnormal_debug_dump(dpu_comp, displaying_scene_id);
	if (displaying_scene_id != displayed_scene_id) {
		dpu_pr_info("scene_id has changed: %d --> %d", displayed_scene_id, displaying_scene_id);
		dpu_comp_abnormal_debug_dump(dpu_comp, displayed_scene_id);
	}

	vstate = inp32(DPU_DSI_VSTATE_ADDR(connector->connector_base));
	phy_status = inp32(DPU_DSI_CDPHY_STATUS_ADDR(connector->connector_base));

	/* lcd status */
	if (is_mipi_cmd_panel(&dpu_comp->conn_info->base) && !is_fake_panel(&dpu_comp->conn_info->base))
		lcd_status = dpu_comp->conn_info->lcd_status_err;

	if (dpu_comp->conn_info->esd_enable != 0) {
		if (vstate == LDI_VSTATE_V_WAIT_TE0 || ((lcd_status != 0) && ((phy_status & BIT(1)) != 0))) {
			dpu_comp->esd_ctrl.esd_recover_state = ESD_RECOVER_STATE_START;
			if (dpu_comp->esd_ctrl.esd_check_wq)
				queue_work(dpu_comp->esd_ctrl.esd_check_wq, &(dpu_comp->esd_ctrl.esd_check_work));
		}
	}

	if (g_debug_present_hold)
		dpu_comp->comp.comp_err_status |= ABNORMAL_UNDERFLOW;

	if (g_debug_dpu_clear_enable != 0) {
		dpu_comp->comp.comp_err_status |= ABNORMAL_UNDERFLOW;
		kthread_queue_work(&dpu_comp->handle_worker, &present->abnormal_handle_work);
	}
	mdfx_trace_end("dpu_irq");
	return 0;
}

static int32_t dpu_comp_edp_abnormal_handle_isr_notify(struct notifier_block *self, unsigned long action, void *data)
{
	struct comp_online_present *present = NULL;
	struct ukmd_listener_data *listener_data = (struct ukmd_listener_data *)data;
	struct dpu_composer *dpu_comp = (struct dpu_composer *)(listener_data->data);
	present = (struct comp_online_present *)dpu_comp->present_data;

	kthread_queue_work(&dpu_comp->handle_worker, &present->edp_abnormal_handle_work);

	return 0;
}

static struct notifier_block abnormal_handle_isr_notifier = {
	.notifier_call = dpu_comp_abnormal_handle_isr_notify,
};

static struct notifier_block edp_abnormal_handle_isr_notifier = {
	.notifier_call = dpu_comp_edp_abnormal_handle_isr_notify,
};

void dpu_comp_abnormal_handle_init(struct ukmd_isr *isr, struct dpu_composer *dpu_comp, uint32_t listening_bit)
{
	struct comp_online_present *present = (struct comp_online_present *)dpu_comp->present_data;

	if (listening_bit == DSI_INT_UNDER_FLOW) {
		kthread_init_work(&present->abnormal_handle_work, dpu_comp_abnormal_handle_work);
		kthread_init_work(&present->pmic_abnormal_handle_work, dpu_comp_pmic_abnormal_handle_work);
		ukmd_isr_register_listener(isr, &abnormal_handle_isr_notifier, listening_bit, dpu_comp);
	} else if (listening_bit == DSI_EDP_IRQ_HPD) {
		kthread_init_work(&present->edp_abnormal_handle_work, dpu_comp_edp_abnormal_handle_work);
		ukmd_isr_register_listener(isr, &edp_abnormal_handle_isr_notifier, listening_bit, dpu_comp);
	}
}

void dpu_comp_abnormal_handle_deinit(struct ukmd_isr *isr, uint32_t listening_bit)
{
	ukmd_isr_unregister_listener(isr, &abnormal_handle_isr_notifier, listening_bit);
}

void dpu_comp_abnormal_dump_offline(char __iomem *dpu_base)
{
	char __iomem *glb_base = dpu_base + DPU_GLB0_OFFSET;
	if ((g_dpu_config_data.version.info.version == DPU_ACCEL_DPUV800) ||
		(g_dpu_config_data.version.info.version == DPU_ACCEL_DPUV820)) {
		dpu_print_dvfs_vote_status();
		dpu_pr_warn("OV:\n"
			"\tDBG_OV_YCNT0=%#x\tDBG_OV_YCNT1=%#x\tDBG_OV_YCNT2=%#x\tDBG_OV_YCNT3=%#x\n\n",
			inp32(DPU_GLB_DBG_OV_YCNT0_ADDR(glb_base)), inp32(DPU_GLB_DBG_OV_YCNT0_ADDR(glb_base)),
			inp32(DPU_GLB_DBG_OV_YCNT0_ADDR(glb_base)), inp32(DPU_GLB_DBG_OV_YCNT0_ADDR(glb_base)));
		dpu_comp_abnormal_dump_sdma_info(glb_base);
		dpu_comp_abnormal_dump_lbuf(dpu_base);
		dpu_comp_abnormal_dump_dacc(dpu_base, DPU_SCENE_OFFLINE_0);
		dpu_comp_abnormal_dump_cmdlist_status(dpu_base, DPU_SCENE_OFFLINE_0);
	}
}