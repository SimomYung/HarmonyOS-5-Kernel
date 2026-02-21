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

#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>

#include "dkmd_log.h"
#include "dkmd_object.h"
#include "dkmd_opr_id.h"
#include "dpu_opr_config.h"
#include "dpu_config_utils.h"

static uint32_t s_scf_lut_addr_tlb[] = {
	DPU_WCH1_OFFSET,  /* only wch1 support scf */
	DPU_ARSR0_OFFSET,
	DPU_ARSR1_OFFSET,
	DPU_VSCF0_OFFSET,
	DPU_VSCF1_OFFSET,
};

struct dbuf_config_info g_dbuf_config_info;

// RQOS_IN, RQOS_OUT, FLUX_REQ_BEF_IN, FLUX_REQ_BEF_OUT
static uint32_t g_dbuf_thd_req[DBUF_THD_TYPE_MAX] = {
	DBUF_THD_DBUF_DEPTH_MAX * 80 / 100, // THD_RQOS_IN :80% of Depth
	DBUF_THD_DBUF_DEPTH_MAX * 90 / 100, // THD_RQOS_OUT :90% of Depth
	DBUF_THD_DBUF_DEPTH_MAX * 70 / 100, // THD_FLUX_BEF_DFS_IN :70% of Depth
	DBUF_THD_DBUF_DEPTH_MAX * 80 / 100, // THD_FLUX_BEF_DFS_OUT :80% of Depth
};

static struct lbuf_part_res_config_info g_lb_res_config[] = {
	{LBUF_LB0_PART_ID, PART0_LB_NUM_MAX},
	{LBUF_LB1_PART_ID, PART1_LB_NUM_FOR_FREQ_CONSTRAINT},
	{LBUF_LB2_PART_ID, PART2_LB_NUM_MAX},
	{LBUF_LB3_PART_ID, PART3_LB_NUM_MAX},
};

static struct lbuf_part_res_config_info g_lb_res_config_es[] = {
	{LBUF_LB0_PART_ID, PART0_LB_NUM_MAX},
	{LBUF_LB1_PART_ID, PART1_LB_NUM_FOR_FREQ_CONSTRAINT_ES},
	{LBUF_LB2_PART_ID, PART2_LB_NUM_MAX},
	{LBUF_LB3_PART_ID, PART3_LB_NUM_MAX},
};

static struct lbuf_static_reserved_config g_lb_static_reserved[] = {
	{LBUF_LB0_PART_ID, DPU_SCENE_OFFLINE_0, LB0_RSERVED_NUM_SCENE4},
	{LBUF_LB0_PART_ID, DPU_SCENE_MAX, LB0_RSERVED_NUM_SCENEALL},
	{LBUF_LB1_PART_ID, DPU_SCENE_OFFLINE_0, LB1_RSERVED_NUM_SCENE4},
	{LBUF_LB3_PART_ID, DPU_SCENE_OFFLINE_0, LB3_RSERVED_NUM_SCENE4},
};

static struct lbuf_static_reserved_configs g_lb_static_reserved_cfg = {
	array_len(g_lb_static_reserved), (struct lbuf_static_reserved_config*)g_lb_static_reserved
};

static struct lbuf_part_node_config_info g_lb_part_node_config_info = {
	array_len(g_lb_res_config), (struct lbuf_part_res_config_info*)g_lb_res_config
};

static struct lbuf_part_node_config_info g_lb_part_node_config_info_es = {
	array_len(g_lb_res_config_es), (struct lbuf_part_res_config_info*)g_lb_res_config_es
};

uint32_t *dpu_config_get_scf_lut_addr_tlb(uint32_t *length)
{
	// TODO: According to the different DTS to parse platform

	*length = ARRAY_SIZE(s_scf_lut_addr_tlb);
	return s_scf_lut_addr_tlb;
}

uint32_t *dpu_config_get_arsr_lut_addr_tlb(uint32_t *length)
{
	// TODO: According to the different DTS to parse platform

	*length = (uint32_t)ARRAY_SIZE(g_arsr_offset);
	return g_arsr_offset;
}

uint32_t dpu_config_get_version(void)
{
	return DPU_ACCEL_DPUV800;
}

struct lbuf_part_node_config_info *dpu_get_lb_part_node_config_info(void)
{
	if (g_dpu_config_data.version.info.soc_type == DPU_SOC_TYPE_ES)
		return &g_lb_part_node_config_info_es;
	return &g_lb_part_node_config_info;
}

struct lbuf_static_reserved_configs *dpu_get_lb_static_reserved_config(void)
{
	return &g_lb_static_reserved_cfg;
}

uint32_t dpu_is_support_ebit(void)
{
	return 0;
}

static int32_t dbuf_calc_thd(uint32_t last_width, uint32_t yres, uint32_t fps, uint32_t *dbuf_thd)
{
	uint32_t last_unit_per_line =  ceil_div(last_width, DPU_DBUF_UNIT);
	uint32_t time_per_line;
	uint32_t req_line;
	uint32_t i;

	if (unlikely(last_unit_per_line == 0 || yres == 0 || fps == 0)) {
		dpu_pr_err("[dbuf]error!, last_unit_per_line=%u, yres=%u, fps=%u", last_unit_per_line, yres, fps);
		return -1;
	}

	/* use panel yres as vtotal to calculate time_per_line */
	time_per_line = PERCENT * HZ_TO_US / fps / yres;
	if (unlikely(time_per_line == 0)) {
		dpu_pr_err("[dbuf]time_per_line is 0");
		return -1;
	}

	/* last opr afford all request_line */
	for (i = 0; i < (uint32_t)DBUF_THD_TYPE_MAX; ++i) {
		req_line = g_dbuf_thd_req[i] * PERCENT / time_per_line;
		dbuf_thd[i] = req_line * last_unit_per_line;
		dpu_pr_debug("[dbuf]dbuf_thd[%d]=%d", i, dbuf_thd[i]);
	}
	return 0;
}

static void dbuf_set_thd_params(uint32_t *dbuf_thd, struct dpu_lbuf_dbuf0_reg *dbuf0_reg)
{
	// scene0
	dbuf0_reg->thd_rqos.reg.dbuf0_thd_rqos_in = dbuf_thd[DBUF_THD_RQOS_IN];
	dbuf0_reg->thd_rqos.reg.dbuf0_thd_rqos_out = dbuf_thd[DBUF_THD_RQOS_OUT];
	dbuf0_reg->thd_flux_req_bef.reg.dbuf0_thd_flux_req_bef_in = dbuf_thd[DBUF_THD_FLUX_REQ_BEF_IN];
	dbuf0_reg->thd_flux_req_bef.reg.dbuf0_thd_flux_req_bef_out = dbuf_thd[DBUF_THD_FLUX_REQ_BEF_OUT];
	dbuf0_reg->thd_flux_req_aft.reg.dbuf0_thd_flux_req_aft_in = dbuf_thd[DBUF_THD_FLUX_REQ_BEF_IN];
	dbuf0_reg->thd_flux_req_aft.reg.dbuf0_thd_flux_req_aft_out = dbuf_thd[DBUF_THD_FLUX_REQ_BEF_OUT];
	dbuf0_reg->thd_dfs_ok.reg.dbuf0_thd_dfs_ok = dbuf_thd[DBUF_THD_FLUX_REQ_BEF_IN];
}

static struct dbuf_config_info *dbuf_set_config_info(struct dpu_lbuf_dbuf0_reg *dbuf0_reg, uint32_t scene_id)
{
	g_dbuf_config_info.addr_offset[0] = DPU_LBUF_DBUF0_CTRL_ADDR(DPU_LBUF_OFFSET +
		DPU_LBUF_SCNX_DBUF_THD_OFFSET(scene_id));
	g_dbuf_config_info.value[0] = dbuf0_reg->ctrl.value;
	g_dbuf_config_info.addr_offset[1] = DPU_LBUF_DBUF0_THD_RQOS_ADDR(DPU_LBUF_OFFSET +
		DPU_LBUF_SCNX_DBUF_THD_OFFSET(scene_id));
	g_dbuf_config_info.value[1] = dbuf0_reg->thd_rqos.value;
	g_dbuf_config_info.addr_offset[2] = DPU_LBUF_DBUF0_THD_DFS_OK_ADDR(DPU_LBUF_OFFSET +
		DPU_LBUF_SCNX_DBUF_THD_OFFSET(scene_id));
	g_dbuf_config_info.value[2] = dbuf0_reg->thd_dfs_ok.value;
	g_dbuf_config_info.addr_offset[3] = DPU_LBUF_DBUF0_THD_FLUX_REQ_BEF_ADDR(DPU_LBUF_OFFSET +
		DPU_LBUF_SCNX_DBUF_THD_OFFSET(scene_id));
	g_dbuf_config_info.value[3] = dbuf0_reg->thd_flux_req_bef.value;
	g_dbuf_config_info.addr_offset[4] = DPU_LBUF_DBUF0_THD_FLUX_REQ_AFT_ADDR(DPU_LBUF_OFFSET +
		DPU_LBUF_SCNX_DBUF_THD_OFFSET(scene_id));
	g_dbuf_config_info.value[4] = dbuf0_reg->thd_flux_req_aft.value;

	g_dbuf_config_info.addr_offset[5] = DPU_LBUF_DBUF_RELOAD_SW_ADDR(DPU_LBUF_OFFSET);
	g_dbuf_config_info.value[5] = 1;

	return &g_dbuf_config_info;
}

/* scene0: OV->DPP0->DSC0->ITF0, OV->DPP0->ITF0 */
struct dbuf_config_info *dbuf_get_config_info(const struct dbuf_calc_thd_input *dbuf_calc_input)
{
	struct dpu_lbuf_dbuf0_reg dbuf0_reg = {0};
	uint32_t dbuf_thd[DBUF_THD_TYPE_MAX] = {0};
	uint32_t xres = dbuf_calc_input->xres;
	uint32_t yres = dbuf_calc_input->yres;
	uint32_t dsc_en = dbuf_calc_input->dsc_en;
	uint32_t dsc_out_width = dbuf_calc_input->dsc_out_width;
	uint32_t fps = dbuf_calc_input->fps;
	uint32_t scene_id = dbuf_calc_input->scene_id;
	uint32_t last_width = (dsc_en == 0) ? xres : dsc_out_width;

	if (unlikely(dbuf_calc_thd(last_width, yres, fps, dbuf_thd) != 0))
		return NULL;

	dbuf_set_thd_params(dbuf_thd, &dbuf0_reg);
	return dbuf_set_config_info(&dbuf0_reg, scene_id);
}

static void get_dbuf_depth(uint32_t height, uint32_t width, uint32_t fps, struct info_src_dbuf_depth *depth_info)
{
	uint32_t req_line = 0;
	uint32_t lb_num = 0;
	uint32_t time_per_line;
	uint32_t lb_num_perline;
	uint32_t valid_fps;

	dpu_pr_debug("height = %u, width %u, fps = %u", height, width, fps);

	valid_fps = (fps > 0) ? fps : DPU_DEFAULT_FPS;

	time_per_line = PERCENT * HZ_TO_US / valid_fps / height;
	if (time_per_line == 0) {
		dpu_pr_err("time_per_line is 0");

		depth_info->depth = INFO_SRC_DEPTH_DEFAULT;
		depth_info->level0 = depth_info->depth * INFO_SRC_DBUF_LEVEL_PERCENT;
		depth_info->level1 = depth_info->level0 * INFO_SRC_DBUF_LEVEL_PERCENT;
		depth_info->level2 = depth_info->level1 * INFO_SRC_DBUF_LEVEL_PERCENT;
		return;
	}
	lb_num_perline = (width + LB_PIXEL_NUM_PERLB - 1) / LB_PIXEL_NUM_PERLB;

	req_line = INFO_SRC_DBUF_DEPTH_MAX * PERCENT / time_per_line;
	lb_num = req_line * lb_num_perline;
	depth_info->depth = (lb_num < DPU_LB3_MIN_NUM) ? (DPU_LB3_MIN_NUM / lb_num_perline) : req_line;

	req_line = INFO_SRC_DBUF_DEPTH_LEVEL2 * PERCENT / time_per_line;
	depth_info->level2 = (req_line <= depth_info->depth) ? req_line : depth_info->depth;

	req_line = INFO_SRC_DBUF_DEPTH_LEVEL1 * PERCENT / time_per_line;
	depth_info->level1 = (req_line <= depth_info->depth) ? req_line : depth_info->depth;

	req_line = depth_info->level1 + INFO_SRC_LEVEL0_EXTEND;
	depth_info->level0 = (req_line <= depth_info->depth) ? req_line : depth_info->depth;
}

void dpu_get_dyn_config_info(struct dpu_dyn_configs *dyn_config, uint32_t xres, uint32_t yres,
	uint32_t dsc_en, uint32_t dsc_out_width, uint32_t fps)
{
	uint32_t height = yres;
	uint32_t total_height = yres * 110 / 100; // v porch ratio default 1.1
	uint32_t width = (dsc_en == 0) ? xres : dsc_out_width;
	DPU_GLB_SCENE0_INFO_SEL_UNION scene_info_sel;
	DPU_GLB_SCENE0_DBUF_CTRL_UNION scene_dbuf_ctrl;
	DPU_GLB_SCENE0_LAYER_SIZE_UNION layer_size;
	struct info_src_dbuf_depth depth_info;
	DPU_GLB_SCENE0_DBUF_DEPTH_UNION scene_depth;
	DPU_GLB_SCENE0_PRE_OS_NUM_UNION scene_pre_os;
	uint32_t line_per_lb;

	dyn_config->config_cnt = 0;

	scene_info_sel.value = 0;
	scene_info_sel.reg.scn0_info_pause_en = 0;
	scene_info_sel.reg.scn0_dbuf_qos_sel = 1;
	scene_info_sel.reg.scn0_dbuf_vact_qos_sel = 1;
	dyn_config->dyn_configs[dyn_config->config_cnt].addr_offset = DPU_GLB_SCENE0_INFO_SEL_ADDR(DPU_TOP_GLP_OFFSET);
	dyn_config->dyn_configs[dyn_config->config_cnt].value = scene_info_sel.value;
	dyn_config->config_cnt += 1;

	scene_dbuf_ctrl.value = 0;
	scene_dbuf_ctrl.reg.scn0_vact_qos_en = 1;
	dyn_config->dyn_configs[dyn_config->config_cnt].addr_offset = DPU_GLB_SCENE0_DBUF_CTRL_ADDR(DPU_TOP_GLP_OFFSET);
	dyn_config->dyn_configs[dyn_config->config_cnt].value = scene_dbuf_ctrl.value;
	dyn_config->config_cnt += 1;

	layer_size.value = 0;
	layer_size.reg.scn0_layer_height = (height > 0) ? (height - 1) : height;
	dyn_config->dyn_configs[dyn_config->config_cnt].addr_offset = DPU_GLB_SCENE0_LAYER_SIZE_ADDR(DPU_TOP_GLP_OFFSET);
	dyn_config->dyn_configs[dyn_config->config_cnt].value = layer_size.value;
	dyn_config->config_cnt += 1;

	get_dbuf_depth(total_height, width, fps, &depth_info);
	scene_depth.value = 0;
	scene_depth.reg.scn0_dbuf_depth = depth_info.depth;
	scene_depth.reg.scn0_dbuf_level0 = depth_info.level0;
	scene_depth.reg.scn0_dbuf_level1 = depth_info.level1;
	scene_depth.reg.scn0_dbuf_level2 = depth_info.level2;
	dyn_config->dyn_configs[dyn_config->config_cnt].addr_offset = DPU_GLB_SCENE0_DBUF_DEPTH_ADDR(DPU_TOP_GLP_OFFSET);
	dyn_config->dyn_configs[dyn_config->config_cnt].value = scene_depth.value;
	dyn_config->config_cnt += 1;

	scene_pre_os.value = 0;
	scene_pre_os.reg.scn0_pre_info_os = 16;

	line_per_lb = get_line_num_perlb(width);
	scene_pre_os.reg.scn0_line_num_per_lb =  (line_per_lb == 0) ? 0 : (line_per_lb - 1);
	dyn_config->dyn_configs[dyn_config->config_cnt].addr_offset = DPU_GLB_SCENE0_PRE_OS_NUM_ADDR(DPU_TOP_GLP_OFFSET);
	dyn_config->dyn_configs[dyn_config->config_cnt].value = scene_pre_os.value;
	dyn_config->config_cnt += 1;
}

#ifdef CONFIG_DKMD_DEBUG_ENABLE
EXPORT_SYMBOL(dpu_config_get_arsr_lut_addr_tlb);
EXPORT_SYMBOL(dpu_config_get_scf_lut_addr_tlb);
#endif
