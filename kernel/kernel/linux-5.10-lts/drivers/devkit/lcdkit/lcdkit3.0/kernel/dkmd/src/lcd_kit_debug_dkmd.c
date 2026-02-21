/*
 * lcd_kit_debug_dkmd.c
 *
 * lcdkit debug function for lcd driver of dkmd
 *
 * Copyright (c) 2019-2020 Huawei Technologies Co., Ltd.
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

#include "lcd_kit_common.h"
#include "lcd_kit_ddic_ram.h"
#include "lcd_kit_disp.h"
#include "lcd_kit_dbg.h"
#include "lcd_kit_adapt.h"
#include "lcd_kit_utils.h"
#include "base/base.h"
#include "dkmd_lcd_interface.h"
#include "lcd_kit_power.h"
#include "lcd_kit_parse.h"

extern struct delayed_work detect_fs_work;
extern int g_dpd_mode;
// Hsync area
static int dbg_hsync_area(int val)
{
	struct dpu_panel_info *pinfo = NULL;
	int panel_id = lcd_kit_get_active_panel_id();
	pinfo = lcd_kit_get_dpu_pinfo(panel_id);
	if (!pinfo) {
		LCD_KIT_ERR("pinfo is null\n");
		return LCD_KIT_FAIL;
	}
	pinfo->mipi.hsa = (uint32_t)val;
	update_panel_info(pinfo, panel_id);
	LCD_KIT_INFO("HSyncArea = %u\n", pinfo->mipi.hsa);
	return LCD_KIT_OK;
}

// HBack porch
static int dbg_hback_porch(int val)
{
	struct dpu_panel_info *pinfo = NULL;
	int panel_id = lcd_kit_get_active_panel_id();
	pinfo = lcd_kit_get_dpu_pinfo(panel_id);
	if (!pinfo) {
		LCD_KIT_ERR("pinfo is null\n");
		return LCD_KIT_FAIL;
	}
	pinfo->mipi.hbp = (uint32_t)val;
	update_panel_info(pinfo, panel_id);
	LCD_KIT_INFO("HBackPorch = %u\n", pinfo->mipi.hbp);
	return LCD_KIT_OK;
}

// HLine Time
static int dbg_hline_time(int val)
{
	struct dpu_panel_info *pinfo = NULL;
	int panel_id = lcd_kit_get_active_panel_id();
	pinfo = lcd_kit_get_dpu_pinfo(panel_id);
	if (!pinfo) {
		LCD_KIT_ERR("pinfo is null\n");
		return LCD_KIT_FAIL;
	}
	pinfo->mipi.hline_time = (uint32_t)val;
	update_panel_info(pinfo, panel_id);
	LCD_KIT_INFO("HLineTime = %u\n", pinfo->mipi.hline_time);
	return LCD_KIT_OK;
}

// DpiHSize
static int dbg_dpi_hsize(int val)
{
	struct dpu_panel_info *pinfo = NULL;
	int panel_id = lcd_kit_get_active_panel_id();
	pinfo = lcd_kit_get_dpu_pinfo(panel_id);
	if (!pinfo) {
		LCD_KIT_ERR("pinfo is null\n");
		return LCD_KIT_FAIL;
	}
	pinfo->mipi.dpi_hsize = (uint32_t)val;
	update_panel_info(pinfo, panel_id);
	LCD_KIT_INFO("DpiHSize = %u\n", pinfo->mipi.dpi_hsize);
	return LCD_KIT_OK;
}

// VSyncArea
static int dbg_vsync_area(int val)
{
	struct dpu_panel_info *pinfo = NULL;
	int panel_id = lcd_kit_get_active_panel_id();
	pinfo = lcd_kit_get_dpu_pinfo(panel_id);
	if (!pinfo) {
		LCD_KIT_ERR("pinfo is null\n");
		return LCD_KIT_FAIL;
	}
	pinfo->mipi.vsa = val;
	update_panel_info(pinfo, panel_id);
	LCD_KIT_INFO("VSyncArea = %u\n", pinfo->mipi.vsa);
	return LCD_KIT_OK;
}

// VBackPorch
static int dbg_vback_porch(int val)
{
	struct dpu_panel_info *pinfo = NULL;
	int panel_id = lcd_kit_get_active_panel_id();
	pinfo = lcd_kit_get_dpu_pinfo(panel_id);
	if (!pinfo) {
		LCD_KIT_ERR("pinfo is null\n");
		return LCD_KIT_FAIL;
	}
	pinfo->mipi.vbp = val;
	update_panel_info(pinfo, panel_id);
	LCD_KIT_INFO("VBackPorch = %u\n", pinfo->mipi.vbp);
	return LCD_KIT_OK;
}

// VFrontPorch
static int dbg_vfront_porch(int val)
{
	struct dpu_panel_info *pinfo = NULL;
	int panel_id = lcd_kit_get_active_panel_id();
	pinfo = lcd_kit_get_dpu_pinfo(panel_id);
	if (!pinfo) {
		LCD_KIT_ERR("pinfo is null\n");
		return LCD_KIT_FAIL;
	}
	pinfo->mipi.vfp = val;
	update_panel_info(pinfo, panel_id);
	LCD_KIT_INFO("VFrontPorch = %u\n", pinfo->mipi.vfp);
	return LCD_KIT_OK;
}

static int dbg_panel_fps(int val)
{
	struct dpu_panel_info *pinfo = NULL;
	int panel_id = lcd_kit_get_active_panel_id();
	pinfo = lcd_kit_get_dpu_pinfo(panel_id);
	if (!pinfo) {
		LCD_KIT_ERR("pinfo is null\n");
		return LCD_KIT_FAIL;
	}
	pinfo->fps = val;
	update_panel_info(pinfo, panel_id);
	LCD_KIT_INFO("PanelType = %u\n", pinfo->fps);
	return LCD_KIT_OK;
}

// PanelXres
static int dbg_panel_xres(int val)
{
	struct dpu_panel_info *pinfo = NULL;
	int panel_id = lcd_kit_get_active_panel_id();
	pinfo = lcd_kit_get_dpu_pinfo(panel_id);
	if (!pinfo) {
		LCD_KIT_ERR("pinfo is null\n");
		return LCD_KIT_FAIL;
	}
	pinfo->xres = val;
	update_panel_info(pinfo, panel_id);
	LCD_KIT_INFO("PanelXres = %u\n", pinfo->xres);
	return LCD_KIT_OK;
}

// PanelYres
static int dbg_panel_yres(int val)
{
	struct dpu_panel_info *pinfo = NULL;
	int panel_id = lcd_kit_get_active_panel_id();
	pinfo = lcd_kit_get_dpu_pinfo(panel_id);
	if (!pinfo) {
		LCD_KIT_ERR("pinfo is null\n");
		return LCD_KIT_FAIL;
	}
	pinfo->yres = val;
	update_panel_info(pinfo, panel_id);
	LCD_KIT_INFO("PanelYres = %u\n", pinfo->yres);
	return LCD_KIT_OK;
}

// vsync_ctrl_type
static int dbg_vsync_ctrl_type(int val)
{
	struct dpu_panel_info *pinfo = NULL;
	int panel_id = lcd_kit_get_active_panel_id();
	pinfo = lcd_kit_get_dpu_pinfo(panel_id);
	if (!pinfo) {
		LCD_KIT_ERR("pinfo is null\n");
		return LCD_KIT_FAIL;
	}
	pinfo->vsync_ctrl_type = val;
	update_panel_info(pinfo, panel_id);
	LCD_KIT_INFO("PanelHeight= %u\n", pinfo->vsync_ctrl_type);
	return LCD_KIT_OK;
}

static int dbg_dsi_cmds_rx(uint8_t *out, int out_len,
	struct lcd_kit_dsi_panel_cmds *cmds)
{
	uint32_t panel_id = (uint32_t)lcd_kit_get_active_panel_id();

	if (!out || !cmds) {
		LCD_KIT_ERR("out is null or cmds is null\n");
		return LCD_KIT_FAIL;
	}

	if (lcd_kit_get_panel_on_state(panel_id) == 0) {
		LCD_KIT_ERR("panel state is not on!\n");
		return LCD_KIT_FAIL;
	}

	return lcd_kit_cmds_rx(panel_id, NULL, out, out_len, cmds);
}

static int dbg_dual_diff_cmds_rx(unsigned char *out0, unsigned char *out1,
		int out_len, struct lcd_kit_dsi_panel_cmds *cmds0, struct lcd_kit_dsi_panel_cmds *cmds1)
{
	uint32_t panel_id = (uint32_t)lcd_kit_get_active_panel_id();

	if (!out0 || !out1 || !cmds0 || !cmds1) {
		LCD_KIT_ERR("out is null or cmds is null\n");
		return LCD_KIT_FAIL;
	}

	return lcd_kit_dual_diff_cmds_rx(panel_id, out0, out1,
		out_len, cmds0, cmds1);
}

static bool dbg_is_dual_mipi_panel(int panel_id)
{
	return lcd_kit_is_dual_mipi_panel(panel_id);
}

static bool dbg_panel_power_on(void)
{
	LCD_KIT_INFO("power on: %d\n", common_ops->panel_power_on);
	return common_ops->panel_power_on;
}

static struct lcd_kit_dbg_ops dkmd_dbg_ops = {
	.dbg_mipi_rx = dbg_dsi_cmds_rx,
	.dbg_dual_mipi_diff_rx = dbg_dual_diff_cmds_rx,
	.is_dual_mipi_panel = dbg_is_dual_mipi_panel,
	.panel_power_on = dbg_panel_power_on,
	.read_ddic_bin = lcd_kit_ddic_ram_read_bin,
	.write_ddic_bin = lcd_kit_ddic_ram_write_bin,
	.fps_updt_support = dbg_panel_fps,
	.hsync_area = dbg_hsync_area,
	.hback_porch = dbg_hback_porch,
	.hline_time = dbg_hline_time,
	.dpi_hsize = dbg_dpi_hsize,
	.vsync_area = dbg_vsync_area,
	.vback_porch = dbg_vback_porch,
	.vfront_porch = dbg_vfront_porch,
	.vsync_ctrl_type = dbg_vsync_ctrl_type,
	.panel_xres = dbg_panel_xres,
	.panel_yres = dbg_panel_yres,
};

int lcd_kit_debug_init(void)
{
	LCD_KIT_INFO("enter\n");
	return lcd_kit_debug_register(&dkmd_dbg_ops);
}

static void dpd_power_init(struct device_node *np)
{
	int panel_id = lcd_kit_get_active_panel_id();
	LCD_KIT_INFO("panel_id = %d\n", panel_id);
 
	/* vci */
	if (power_hdl->lcd_vci.buf == NULL)
		lcd_kit_parse_array_data(np, "lcd-kit,lcd-vci",
			&power_hdl->lcd_vci);
	/* iovcc */
	if (power_hdl->lcd_iovcc.buf == NULL)
		lcd_kit_parse_array_data(np, "lcd-kit,lcd-iovcc",
			&power_hdl->lcd_iovcc);
	/* vsp */
	if (power_hdl->lcd_vsp.buf == NULL)
		lcd_kit_parse_array_data(np, "lcd-kit,lcd-vsp",
			&power_hdl->lcd_vsp);
	/* vsn */
	if (power_hdl->lcd_vsn.buf == NULL)
		lcd_kit_parse_array_data(np, "lcd-kit,lcd-vsn",
			&power_hdl->lcd_vsn);
	/* backlight */
	if (power_hdl->lcd_backlight.buf == NULL)
		lcd_kit_parse_array_data(np, "lcd-kit,lcd-backlight",
			&power_hdl->lcd_backlight);
	/* vdd */
	if (power_hdl->lcd_vdd.buf == NULL)
		lcd_kit_parse_array_data(np, "lcd-kit,lcd-vdd",
			&power_hdl->lcd_vdd);
}
 
int dpd_init(struct platform_device *pdev)
{
	static bool is_first = true;
	struct device_node *np = NULL;
	int panel_id = lcd_kit_get_active_panel_id();
    LCD_KIT_INFO("enter dpd_init\n");
	if (!pdev) {
		LCD_KIT_ERR("pdev is null\n");
		return LCD_KIT_FAIL;
	}
	if (!g_dpd_mode)
		return LCD_KIT_OK;
	np = pdev->dev.of_node;
	if (!np) {
		LCD_KIT_ERR("NOT FOUND device node\n");
		return LCD_KIT_FAIL;
	}
	if (is_first) {
		is_first = false;
		dpd_power_init(np);
		lcd_kit_power_init(panel_id, pdev);
		/* register lcd ops */
		lcd_kit_ops_register(&g_lcd_ops);
		INIT_DELAYED_WORK(&detect_fs_work, detect_fs_wq_handler);
		/* delay 500ms schedule work */
		schedule_delayed_work(&detect_fs_work, msecs_to_jiffies(500));
	}
	if (file_sys_is_ready()) {
		LCD_KIT_INFO("sysfs is not ready!\n");
		return LCD_KIT_FAIL;
	}
	return LCD_KIT_OK;
}