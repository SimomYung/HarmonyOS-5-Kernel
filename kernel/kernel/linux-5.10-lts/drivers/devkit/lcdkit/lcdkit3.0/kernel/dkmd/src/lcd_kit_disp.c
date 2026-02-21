/*
 * lcd_kit_disp.c
 *
 * lcdkit display function for lcd driver
 *
 * Copyright (c) 2022-2024 Huawei Technologies Co., Ltd.
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

#include <linux/gpio.h>
#include <linux/interrupt.h>
#include "lcd_kit_disp.h"
#include "lcd_kit_panel.h"
#include "lcd_kit_parse.h"
#include "lcd_kit_adapt.h"
#include "lcd_kit_sysfs_dkmd.h"
#include "lcd_kit_power.h"
#include "lcd_kit_ioctl.h"
#include "lcd_kit_local_hbm.h"
#include "lcd_kit_panel_pfm.h"
#include "lcd_kit_ext_power.h"
#include "lcd_kit_power_key.h"
#include "lcd_kit_displayengine.h"
#include "dpu_aod_device.h"
#include "lcd_kit_elvss_control.h"
#include "lcd_kit_safe_frm_rate.h"
#include "platform_include/basicplatform/linux/hi9030_gpio.h"
#include "cam_cfgdev.h"
#ifdef CONFIG_LCD_KIT_HYBRID
#include "hybrid/lcd_kit_hybrid_core.h"
#endif
#include "lcd_kit_scp.h"
#ifdef LCD_KIT_DEBUG_ENABLE
#include "lcd_kit_dbg.h"
#endif
#ifdef CONFIG_DFX_ZEROHUNG
#include <platform/trace/hooks/zerohung.h>
#endif
#ifdef LCD_FACTORY_MODE
#include "lcd_kit_fac.h"
#endif
#include "dkmd_lcd_interface.h"
#if (defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM))
#include <dsm/dsm_pub.h>
#endif
#ifdef CONFIG_INPUTHUB_20
#include <huawei_platform/inputhub/sensor_feima_ext.h>
#endif
#ifdef CONFIG_POWER_DUBAI
#include <huawei_platform/log/log_jank.h>
#endif
#include <linux/errno.h>
#include <linux/printk.h>
#ifdef CONFIG_HISYSEVENT
#include <dfx/hiview_hisysevent.h>
#endif
#ifdef CONFIG_EXT_INPUTHUB_MATTING
#include "matting_algo_task.h"
#endif
#ifdef CONFIG_EXT_INPUTHUB_MATTING_ALSC
#include "matting_algo_hisi_alsc_interface.h"
#endif
enum {
	LCD_KIT_SET_DUBAI_HISYSEVENT_POWER_OFF = 0,
	LCD_KIT_SET_DUBAI_HISYSEVENT_POWER_ON,
	LCD_KIT_SET_DUBAI_HISYSEVENT_FAKE_PWOER_OFF	
};
enum {
	TRUE_POWER_MODE,
	FAKE_POWER_MODE
};

#if (defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM))
struct dsm_client *lcd_dclient = NULL;
#endif
static int g_init_order[MAX_PANEL] = { PRIMARY_PANEL, SECONDARY_PANEL };
static char *g_disp_compatibles[MAX_PANEL] = { "lcd_panel_type", "sec_lcd_panel_type" };
static struct lcd_kit_disp_info g_lcd_kit_disp_info[MAX_PANEL];
static struct dpu_panel_info g_dpu_pinfo[MAX_PANEL];
static struct dpu_dynamic_panel_info g_dpu_dynamic_panel_info[MAX_PANEL];
struct lcd_kit_public_info g_lcd_kit_public_info = {
	NORMAL_TYPE,
	PRIMARY_PANEL,
};
static struct lcd_kit_disp_lock g_lcd_kit_disp_lock[MAX_PANEL];
static struct lcd_kit_panel_params g_lcd_kit_panels[] = {
	{3000, 0x00, "190_c07_7p847"},
	{3000, 0x400, "190_c07_1_7p847"},
	{3000, 0x04, "310_c07_7p847"},
	{3000, 0x404, "310_c07_1_7p847"},
	{3000, 0x15, "default_7p847"},
	{4300, 0x00, "raydium_6d030_1p44_cmd"},
};

int lcd_kit_get_bl_async_cmds(int panel_id,
	const struct dkmd_cmds_info *cmds_info, struct mipi_dsi_tx_params *params);
int lcd_kit_get_part_region_refresh_cmds(int panel_id,
	const struct dkmd_cmds_info *cmds_info, struct mipi_dsi_tx_params *params);
int lcd_kit_calc_partial_rect(int panel_id,
	const struct dkmd_cmds_info *cmds_info, struct mipi_dsi_tx_params *params);
int lcd_kit_get_part_region_control_cmds(int panel_id,
	const struct dkmd_cmds_info *cmds_info, struct mipi_dsi_tx_params *params);
int lcd_kit_get_safe_frame_part_cmds(int panel_id,
	const struct dkmd_cmds_info *cmds_info, struct mipi_dsi_tx_params *params);

int lcd_kit_get_dual_bl_async_cmds(int panel_id,
	const struct dkmd_cmds_info *cmds_info, struct mipi_dsi_tx_params *params0,
	struct mipi_dsi_tx_params *params1);
int lcd_kit_get_dual_part_region_refresh_cmds(int panel_id,
	const struct dkmd_cmds_info *cmds_info, struct mipi_dsi_tx_params *params0,
	struct mipi_dsi_tx_params *params1);
int lcd_kit_dual_calc_partial_rect(int panel_id,
	const struct dkmd_cmds_info *cmds_info, struct mipi_dsi_tx_params *params0,
	struct mipi_dsi_tx_params *params1);
int lcd_kit_get_dual_part_region_control_cmds(int panel_id,
	const struct dkmd_cmds_info *cmds_info, struct mipi_dsi_tx_params *params0,
	struct mipi_dsi_tx_params *params1);
int lcd_kit_get_dual_safe_frame_part_cmds(int panel_id,
	const struct dkmd_cmds_info *cmds_info, struct mipi_dsi_tx_params *params0,
	struct mipi_dsi_tx_params *params1);

static struct lcd_kit_async_params async_tx_params[] = {
	{ DKMD_CMDS_TYPE_BL, lcd_kit_get_bl_async_cmds },
	{ DKMD_CMDS_TYPE_PT_RECT, lcd_kit_get_part_region_refresh_cmds },
	{ DKMD_CMDS_TYPE_PPU_RECT, lcd_kit_calc_partial_rect },
	{ DKMD_CMDS_TYPE_PPU_ENTER, lcd_kit_get_part_region_control_cmds },
	{ DKMD_CMDS_TYPE_PPU_EXIT, lcd_kit_get_part_region_control_cmds },
	{ DKMD_CMDS_TYPE_SFR, lcd_kit_get_safe_frame_part_cmds },
};

 
static struct lcd_kit_dual_async_params dual_async_tx_params[] = {
	{ DKMD_CMDS_TYPE_BL, lcd_kit_get_dual_bl_async_cmds },
	{ DKMD_CMDS_TYPE_PT_RECT, lcd_kit_get_dual_part_region_refresh_cmds },
	{ DKMD_CMDS_TYPE_PPU_RECT, lcd_kit_dual_calc_partial_rect },
	{ DKMD_CMDS_TYPE_PPU_ENTER, lcd_kit_get_dual_part_region_control_cmds },
	{ DKMD_CMDS_TYPE_PPU_EXIT, lcd_kit_get_dual_part_region_control_cmds },
	{ DKMD_CMDS_TYPE_SFR, lcd_kit_get_dual_safe_frame_part_cmds },
};

static int lcd_kit_updt_fps(int panel_id, unsigned int fps);
static int lcd_kit_updt_fps_te_mode(int panel_id, unsigned int fps, bool is_need_delay);
static int lcd_kit_fake_power_off(int panel_id);

static int lcd_kit_get_init_display_length(void)
{
	if (PUBLIC_INFO->product_type == NORMAL_TYPE)
		return 1;
	else
		return MAX_PANEL;
}

#if defined(CONFIG_DKMD_DPU_AOD)
extern uint32_t dpu_aod_get_panel_id(void);
#endif

void lcd_kit_sensor_notify_panel_version(uint32_t panel_id,
	char *panel_version)
{
	if (!panel_version) {
		LCD_KIT_ERR("invaild params\n");
		return;
	}

	if (strncpy_s((char *)panel_version,
		LCD_PANEL_VERSION_SIZE,
		(char *)disp_info->panel_version.lcd_panel_version,
		LCD_PANEL_VERSION_SIZE - 1) != EOK) {
		LCD_KIT_ERR("strncpy_s fail\n");
		return;
	}
}

int lcd_kit_get_active_panel_id(void)
{
	int panel_len = lcd_kit_get_init_display_length();
	int i;
	int panel_id;

	for (i = 0; i < panel_len; i++) {
		panel_id = g_init_order[i];
		if (lcd_kit_get_panel_on_state(panel_id))
			return panel_id;
	}
#if defined(CONFIG_DKMD_DPU_AOD)
	panel_id = dpu_aod_get_panel_id();
	if (panel_id >= panel_len || panel_id < PRIMARY_PANEL) {
		LCD_KIT_INFO("panel id is out of bounds\n");
		return PRIMARY_PANEL;
	}
	return panel_id;
#else
	return PRIMARY_PANEL;
#endif
}

int lcd_kit_get_display_type(void)
{
	return PUBLIC_INFO->product_type;
}

struct lcd_kit_disp_info *lcd_kit_get_disp_info(int panel_id)
{
	if (lcd_kit_check_panel_id((unsigned int)panel_id) != LCD_KIT_OK)
		return &g_lcd_kit_disp_info[0];
	return &g_lcd_kit_disp_info[panel_id];
}

struct dpu_panel_info *lcd_kit_get_dpu_pinfo(int panel_id)
{
	if (lcd_kit_check_panel_id((unsigned int)panel_id) != LCD_KIT_OK)
		return NULL;
	return &g_dpu_pinfo[panel_id];
}

struct dpu_dynamic_panel_info *lcd_kit_get_dpu_dynamic_panel_info(int panel_id)
{
	if (lcd_kit_check_panel_id((unsigned int)panel_id) != LCD_KIT_OK)
		return NULL;
	return &g_dpu_dynamic_panel_info[panel_id];
}

struct lcd_kit_public_info *lcd_kit_get_public_info(void)
{
	return &g_lcd_kit_public_info;
}

struct lcd_kit_disp_lock *lcd_kit_get_disp_lock(int panel_id)
{
	if (lcd_kit_check_panel_id((unsigned int)panel_id) != LCD_KIT_OK)
		return &g_lcd_kit_disp_lock[0];
	return &g_lcd_kit_disp_lock[panel_id];
}

static int lcd_kit_set_fastboot(int panel_id)
{
	// lcd panel version
	if (disp_info->panel_version.support) {
		if (!lcd_kit_panel_version_init(panel_id))
			LCD_KIT_INFO("read panel version successful\n");
		else
			LCD_KIT_INFO("read panel version fail\n");
	}
	/* RM Partition Refresh Reg */
	if (common_info->part_refresh.support)
		lcd_kit_get_part_region_ip_val(panel_id);
	return LCD_KIT_OK;
}

static void lcd_kit_fps_handle_on(int panel_id)
{
	struct dpu_panel_info *pinfo = NULL;

	pinfo = lcd_kit_get_dpu_pinfo(panel_id);
	if (!pinfo) {
		LCD_KIT_ERR("pinfo is null\n");
		return;
	}
	if (!disp_info->fps.support) {
		LCD_KIT_DEBUG("not support fps\n");
		return;
	}
	if (pinfo->dfr_info.oled_info.oled_type == LTPS)
		lcd_kit_updt_fps(panel_id, disp_info->fps.current_fps);

	if (disp_info->updt_fps_te.support)
		lcd_kit_updt_fps_te_mode(panel_id, disp_info->fps.current_fps, true);
}

static void lcd_kit_aod_handle(int panel_id)
{
#if defined(CONFIG_DKMD_DPU_AOD)
	struct lcd_kit_adapt_ops *adapt_ops = NULL;
	unsigned int ret;

	if (!common_info->aod.support)
		return;
	adapt_ops = lcd_kit_get_adapt_ops();
	if (!adapt_ops || !adapt_ops->mipi_tx) {
		LCD_KIT_ERR("can not get adapt_ops or mipi_tx!\n");
		return;
	}
	ret = get_lpm_on();
	LCD_KIT_INFO("ret is %u\n", ret);
	if (ret != 1) // 1 means in aod mode, 0 means exit aod mode
		return;
	if (adapt_ops->mipi_tx(panel_id, NULL, &common_info->aod.exit_cmd) != LCD_KIT_OK)
		LCD_KIT_ERR("aod exit cmds tx error\n");
#endif
}

static void lcd_kit_hs_handle(int panel_id)
{
	/* fps on handle */
	lcd_kit_fps_handle_on(panel_id);
	lcd_kit_aod_handle(panel_id);
	lcd_kit_set_aod_panel_state(panel_id, 1);
	check_panel_on_state(panel_id);
}

void lcd_on_time_record(int panel_id)
{
	static struct timespec64 end_time = {0};
	uint32_t delta;
	int i;

	ktime_get_real_ts64(&end_time);
	/* change s to ms */
	delta = (uint32_t)((end_time.tv_sec - disp_info->lcd_on_time.start_time.tv_sec) * 1000 +
		(end_time.tv_nsec - disp_info->lcd_on_time.start_time.tv_nsec) / 1000000);
	LCD_KIT_INFO("delta is %u\n", delta);
	for (i = TIME_COUNT - 1; i > 0; i--)
		disp_info->lcd_on_time.delta[i] = disp_info->lcd_on_time.delta[i - 1];
	disp_info->lcd_on_time.delta[0] = delta;
}

static void lcd_update_power_seq(int panel_id)
{
	disp_info->lcd_power_seq = LCD_KIT_POWER_ON_SEQ;
	ktime_get_real_ts64(&disp_info->lcd_on_time.start_time);
}

#ifdef CONFIG_HISYSEVENT
static void lcd_kit_hiview_hisysevent(const char *domain, const char *casename, int level, const char *value, int state)
{
	struct hiview_hisysevent *event = NULL;
	int ret;

	event = hisysevent_create(domain, casename, BEHAVIOR);
	if (!event) {
		LCD_KIT_ERR("failed to create event\n");
		return;
	}

	if (value == NULL) {
		ret = hisysevent_put_integer(event, "MODULE", level);
	} else {
		ret = hisysevent_put_string(event, "MODULE", value);
		if (ret != 0) {
			LCD_KIT_ERR("failed to put value to event, ret=%d\n", ret);
			goto hisysevent_end;
		}
		/* The state field is added to the power-on and power-off events */
		ret = hisysevent_put_integer(event, "STATE", state);
		if (ret != 0) {
			LCD_KIT_ERR("failed to put state to event, ret=%d\n", ret);
			goto hisysevent_end;
		}
	}

	ret = hisysevent_write(event);
	if (ret != 0)
		LCD_KIT_ERR("lcd_kit_hiview_hisysevent fail, %s\n", casename);

hisysevent_end:
	hisysevent_destroy(&event);
}
#endif

static void lcd_kit_set_dubai_hisysevent_power_state(int panel_id, int enable)
{
#ifdef CONFIG_POWER_DUBAI
	if (enable)
		LOG_JANK_D(JLID_KERNEL_LCD_POWER_ON, "%s", "LCD_POWER_ON");
	else
		LOG_JANK_D(JLID_KERNEL_LCD_POWER_OFF, "%s", "LCD_POWER_OFF");
#endif
#ifdef CONFIG_HISYSEVENT
	switch (enable) {
	case LCD_KIT_SET_DUBAI_HISYSEVENT_POWER_OFF:
		lcd_kit_hiview_hisysevent("LCD", "LCD_POWER_OFF", 0, common_info->panel_name, TRUE_POWER_MODE);
		break;
	case LCD_KIT_SET_DUBAI_HISYSEVENT_POWER_ON:
		lcd_kit_hiview_hisysevent("LCD", "LCD_POWER_ON", 0, common_info->panel_name, TRUE_POWER_MODE);
		break;
	default:
		lcd_kit_hiview_hisysevent("LCD", "LCD_POWER_OFF", 0, common_info->panel_name, FAKE_POWER_MODE);
	}
#endif
}

static void lcd_kit_set_dubai_hisysevent_backlight(int panel_id, unsigned int bl_level)
{
	if ((disp_info->jank_last_bl_level == 0) && (bl_level != 0)) {
#ifdef CONFIG_POWER_DUBAI
		LOG_JANK_D(JLID_KERNEL_LCD_BACKLIGHT_ON, "LCD_BACKLIGHT_ON,%u", bl_level);
#endif
#ifdef CONFIG_HISYSEVENT
		/* The state is not required. Any value can be written */
		lcd_kit_hiview_hisysevent("LCD", "LCD_BACKLIGHT_ON", (int)bl_level, NULL, 0);
#endif
		disp_info->jank_last_bl_level = bl_level;
	} else if ((disp_info->jank_last_bl_level != 0) && (bl_level == 0)) {
#ifdef CONFIG_POWER_DUBAI
		LOG_JANK_D(JLID_KERNEL_LCD_BACKLIGHT_OFF, "LCD_BACKLIGHT_OFF");
#endif
#ifdef CONFIG_HISYSEVENT
		/* The state is not required. Any value can be written */
		lcd_kit_hiview_hisysevent("LCD", "LCD_BACKLIGHT_OFF", (int)bl_level, NULL, 0);
#endif
		disp_info->jank_last_bl_level = bl_level;
	}
}

void mipi_occupy_notify(int panel_id, bool value)
{
	LCD_KIT_INFO("panel id is %d,value is %d\n", panel_id, value);
	if (disp_info->lcd_completion.comp_inited != true) {
		LCD_KIT_INFO("comp not inited\n");
		return;
	}
	disp_info->lcd_completion.mipi_occupy = value;
	if (value == false)
		complete_all(&disp_info->lcd_completion.mipi_switch_done);
}

int get_mipi_state(int panel_id)
{
	if (!common_info->cam_lcd_mipi_reuse)
		return LCD_KIT_OK;
	if (common_info->mipi_switch_end == true)
		return LCD_KIT_OK;
	return LCD_KIT_FAIL;
}

static void wait_mipi_switch_done(int panel_id)
{
	LCD_KIT_INFO("panel_id is %d +\n", panel_id);
	if (common_info->cam_lcd_mipi_reuse == 0) {
		LCD_KIT_INFO("not support cam lcd mipi reuse\n");
		return;
	}
	LCD_KIT_INFO("disp_info->lcd_power_seq is %d\n", disp_info->lcd_power_seq);
	/* only power on first stage judge if camera use mipi */
	if (disp_info->lcd_power_seq != LCD_KIT_POWER_ON_SEQ &&
		disp_info->lcd_power_seq != LCD_KIT_POWER_DBL_ON_SEQ &&
		disp_info->lcd_power_seq != LCD_KIT_FAKE_POWER_OFF)
		return;
	/* notify camera inner lcd use mipi */
#ifdef CONFIG_KERNEL_CAMERA_LCD_BOTH_MIPI_SWITCH
	LCD_KIT_INFO("notify camera lcd start mipi switch\n");
	camera_lcd_start_switch_mipi_store(panel_id);
#endif
	if (panel_id == SECONDARY_PANEL)
		return;
	if (disp_info->lcd_completion.mipi_occupy == false)
		return;
	reinit_completion(&disp_info->lcd_completion.mipi_switch_done);
	if (!wait_for_completion_timeout(&disp_info->lcd_completion.mipi_switch_done,
		msecs_to_jiffies(MAX_MIPI_SWITCH_WAIT)))
		LCD_KIT_ERR("wait mipi_switch_done time out!\n");
	disp_info->lcd_completion.mipi_occupy = false;
	LCD_KIT_INFO("panel_id is %d -\n", panel_id);
}

static void lcd_kit_fake_power_on_temper(int panel_id)
{
	if (disp_info->lcd_power_seq == LCD_KIT_POWER_DBL_ON_HS_SEQ){
		if (common_ops->start_temper_hrtimer &&
			common_info->temper_thread.enable &&
			common_info->temper_thread.dark_enable &&
			lcd_kit_get_power_status(panel_id) != 0) {
				common_ops->start_temper_hrtimer(panel_id);
				LCD_KIT_INFO("fake power on, temper timer is starting\n");
		}
	}
}

static void lcd_kit_prepare_enable(int panel_id)
{
	wait_mipi_switch_done(panel_id);
	if (disp_info->lcd_power_seq == LCD_KIT_POWER_OFF) {
		disp_info->lcd_power_seq = LCD_KIT_POWER_ON_SEQ;
	} else if ((disp_info->lcd_power_seq == LCD_KIT_POWER_ON ||
		disp_info->lcd_power_seq == LCD_KIT_FAKE_POWER_OFF) &&
		common_ops->panel_mipi_switch) {
			/* notify displayengine when power on */
			if (common_info->notify_de_support)
				display_engine_sfr_enter_ap_event(panel_id, POWER_ON);
			disp_info->lcd_power_seq = LCD_KIT_POWER_DBL_ON_SEQ;
	}

	lcd_kit_fake_power_on_temper(panel_id);
	LCD_KIT_INFO("disp_info->lcd_power_seq is %u\n", disp_info->lcd_power_seq);
	if (disp_info->lcd_power_seq == LCD_KIT_POWER_DBL_ON_SEQ) {
		if (lcd_kit_get_power_status(panel_id) == 0) {
			disp_info->lcd_power_seq = LCD_KIT_POWER_ON_SEQ;
		}
	}

	if (disp_info->lcd_power_seq == LCD_KIT_POWER_DBL_ON_LP_SEQ && common_ops->panel_on_tp) {
		LCD_KIT_INFO("tp on notify\n");
		common_ops->panel_on_tp(panel_id, NULL);
	}
}

static void lcd_kit_instruct_process(int panel_id, uint8_t *read_val, int cnt)
{
	int index;
	unsigned int level;
	/* read_value * disp_info->effect_protect.ratio / 10 means gamma para transfor */
	level = ((read_val[READ_REG_H_OFFSET] << BYTE_OFFSET) |
		read_val[READ_REG_L_OFFSET]) * disp_info->effect_protect.ratio / 10;
	disp_info->effect_protect.write_cmds.cmds[REG_EFFECT_L_OFFSET + REG_OFFSET].payload[1] =
		level & 0xFF;
	disp_info->effect_protect.write_cmds.cmds[REG_EFFECT_H_OFFSET + REG_OFFSET].payload[1] =
		(level >> BYTE_OFFSET) & 0xFF;

	for (index = 0; index < cnt; index++) {
		if (index != REG_EFFECT_H_OFFSET && index != REG_EFFECT_L_OFFSET)
			disp_info->effect_protect.write_cmds.cmds[index + REG_OFFSET].payload[1] =
				read_val[index];
	}
}

static void lcd_kit_get_effect_cmd(int panel_id)
{
	int ret = LCD_KIT_OK;
	int i;
	uint8_t read_array[MAX_READ_SIZE] = {0};
	struct lcd_kit_adapt_ops *adapt_ops = NULL;

	adapt_ops = lcd_kit_get_adapt_ops();
	if (!adapt_ops) {
		LCD_KIT_ERR("can not register adapt_ops!\n");
		return;
	}
	/* get effect cmd */
	if (disp_info->effect_protect.support) {
		ret = adapt_ops->mipi_rx(panel_id, NULL, read_array, MAX_READ_SIZE - 1,
			&disp_info->effect_protect.read_cmds);
		lcd_kit_instruct_process(panel_id, read_array, MAX_READ_SIZE);
		for (i = 0; i < MAX_READ_SIZE; i++)
			LCD_KIT_DEBUG("read_array[%d] = 0x%x\n", i, read_array[i]);
	}
}

static void lcd_kit_effect_protect(int panel_id)
{
	int ret = LCD_KIT_OK;
	struct lcd_kit_adapt_ops *adapt_ops = NULL;

	adapt_ops = lcd_kit_get_adapt_ops();
	if (!adapt_ops) {
		LCD_KIT_ERR("can not register adapt_ops!\n");
		return;
	}

	if (disp_info->effect_protect.write_cmds.cmds != NULL)
		ret = adapt_ops->mipi_tx(panel_id, NULL, &disp_info->effect_protect.write_cmds);
	if (ret)
		LCD_KIT_ERR("send effect cmd fail!\n");
}

static int lcd_kit_power_on_seq(int panel_id)
{
	int ret = LCD_KIT_OK;

	wait_power_off_done(panel_id);
	if (common_ops->panel_power_on)
			ret = common_ops->panel_power_on(panel_id, NULL);
#ifdef CONFIG_KERNEL_CAMERA_LCD_BOTH_MIPI_SWITCH
	if (common_info->cam_lcd_mipi_reuse != 0)
		camera_lcd_mipi_switch_status_store(panel_id);
#endif
	lcd_kit_set_dubai_hisysevent_power_state(panel_id, LCD_KIT_SET_DUBAI_HISYSEVENT_POWER_ON);
	/* set next step */
	disp_info->lcd_power_seq = LCD_KIT_POWER_ON_LP_SEQ;
	return ret;
}

static int lcd_kit_power_on_lp_seq(int panel_id)
{
	int ret = LCD_KIT_OK;

	if (common_ops->panel_on_lp)
		ret = common_ops->panel_on_lp(panel_id, NULL);
	if (!lcd_kit_get_aod_panel_state(panel_id) && disp_info->effect_protect.support)
		lcd_kit_effect_protect(panel_id);
	/* set next step */
	disp_info->lcd_power_seq = LCD_KIT_POWER_ON_HS_SEQ;
	return ret;
}

static int lcd_kit_power_on_hs_seq(int panel_id)
{
	int ret = LCD_KIT_OK;

	/* send mipi command by high speed */
	if (common_ops->panel_on_hs)
		ret = common_ops->panel_on_hs(panel_id, NULL);
	/* set next step */
	disp_info->lcd_power_seq = LCD_KIT_POWER_ON;
	/* notify displayengine when power on */
	if (common_info->notify_de_support)
		display_engine_sfr_enter_ap_event(panel_id, POWER_ON);
	lcd_kit_hs_handle(panel_id);
	lcd_on_time_record(panel_id);
	return ret;
}

static int lcd_kit_power_dbl_on_seq(int panel_id)
{
	int ret = LCD_KIT_OK;

	disp_info->lcd_power_seq = LCD_KIT_POWER_DBL_ON_LP_SEQ;
	ret = common_ops->panel_mipi_switch(panel_id, NULL);
#ifdef CONFIG_KERNEL_CAMERA_LCD_BOTH_MIPI_SWITCH
	if (common_info->cam_lcd_mipi_reuse != 0)
		camera_lcd_mipi_switch_status_store(panel_id);
#endif
	lcd_kit_set_dubai_hisysevent_power_state(panel_id, LCD_KIT_SET_DUBAI_HISYSEVENT_POWER_ON);
	return ret;
}

static int lcd_kit_power_dbl_on_lp_seq(int panel_id)
{
	if (common_ops->panel_mipi_switch_on_lp)
		common_ops->panel_mipi_switch_on_lp(panel_id, NULL);
	disp_info->lcd_power_seq = LCD_KIT_POWER_DBL_ON_HS_SEQ;
	return LCD_KIT_OK;
}

static int lcd_kit_power_dbl_on_hs_seq(int panel_id)
{
	(void)panel_id;
	disp_info->lcd_power_seq = LCD_KIT_POWER_ON;
	lcd_kit_fps_handle_on(panel_id);
	check_panel_on_state(panel_id);
	return LCD_KIT_OK;
}

static struct lcd_kit_on_seq g_lcd_kit_handle[] = {
	{LCD_KIT_POWER_ON_SEQ, lcd_kit_power_on_seq},
	{LCD_KIT_POWER_ON_LP_SEQ, lcd_kit_power_on_lp_seq},
	{LCD_KIT_POWER_ON_HS_SEQ, lcd_kit_power_on_hs_seq},
	{LCD_KIT_POWER_DBL_ON_SEQ, lcd_kit_power_dbl_on_seq},
	{LCD_KIT_POWER_DBL_ON_LP_SEQ, lcd_kit_power_dbl_on_lp_seq},
	{LCD_KIT_POWER_DBL_ON_HS_SEQ, lcd_kit_power_dbl_on_hs_seq},
};

static int lcd_kit_on(int panel_id)
{
	int ret = LCD_KIT_OK;
	int i = 0;
	int handle_size = sizeof(g_lcd_kit_handle) / sizeof(struct lcd_kit_on_seq);

	LCD_KIT_INFO("fb%d, +!\n", panel_id);
	if (disp_info->lcd_power_seq == LCD_KIT_POWER_OFF)
		lcd_update_power_seq(panel_id);

	lcd_kit_prepare_enable(panel_id);
	// clean skip power off
	lcd_kit_set_skip_power_on_off(false);
	for (i = 0; i < handle_size; i++) {
		if (g_lcd_kit_handle[i].lcd_power_seq == (int)disp_info->lcd_power_seq &&
			g_lcd_kit_handle[i].func_ptr != NULL) {
			ret = g_lcd_kit_handle[i].func_ptr(panel_id);
			break;
		}
	}

	LCD_KIT_INFO("fb%d, -!\n", panel_id);
	return ret;
}

static void lcd_kit_hs_off_handle(int panel_id)
{
	/* if aod no panel off, clear lcd status */
	if (lcd_kit_get_power_status(panel_id))
		lcd_kit_set_aod_panel_state(panel_id, 0);
}

static void clear_elvss_ctrl(int panel_id)
{
	int bl_level = 0;
	struct dpu_elvss_state *el_state = NULL;

	/* el ctrl clear vsync backlight state */
	need_upate_bl = 0;
	if (lcd_kit_el_ctrl_is_support(panel_id, bl_level)) {
		el_state = get_elvss_state(panel_id);
		if (!el_state) {
			LCD_KIT_ERR("panel elvss_state is null");
			return;
		}
#if defined(CONFIG_DKMD_DPU_AOD)
		if (get_lcd_always_on() != 0 && el_state->cur_ddic_enable) {
			LCD_KIT_INFO("reset_elvss_state CONFIG_DKMD_DPU_AOD");
			lcd_kit_el_ctrl_close(panel_id);
		}
#endif
		if (disp_info->lcd_power_seq >= LCD_KIT_POWER_OFF_HS_SEQ)
			return;
		reset_elvss_state(panel_id);
	}

	if (lcd_kit_el2_is_support(panel_id) && disp_info->lcd_power_seq < LCD_KIT_POWER_OFF_HS_SEQ)
		reset_elvss_state(panel_id);
}

static void lcd_kit_prepare_disable(int panel_id)
{
	if (common_ops->stop_temper_hrtimer &&
		common_info->temper_thread.enable &&
		common_info->temper_thread.dark_enable)
		common_ops->stop_temper_hrtimer(panel_id);
	clear_elvss_ctrl(panel_id);
	if (disp_info->lcd_power_seq != LCD_KIT_POWER_ON) {
		LCD_KIT_INFO("not power on power seq\n");
		return;
	}
#if defined(CONFIG_DKMD_DPU_AOD)
	if (get_lcd_always_on() != 0) {
		lcd_kit_fake_power_off(panel_id);
	} else {
		common_info->fake_power_off_state = false;
	}
#endif
	if (disp_info->lcd_power_seq == LCD_KIT_POWER_ON)
		disp_info->lcd_power_seq = LCD_KIT_POWER_OFF_HS_SEQ;
}

static int lcd_kit_update_fw(int panel_id)
{
	int ret = LCD_KIT_OK;
	struct lcd_kit_tcon_update *upd = &disp_info->update_info;
	struct lcd_kit_panel_ops *ops = lcd_kit_panel_get_ops();

	if (!upd || !upd->support) {
		LCD_KIT_DEBUG("upd is null or not support\n");
		return LCD_KIT_FAIL;
	}

	if (upd->tcon_upd_switch == TCON_UPDATE_DISABLE) {
		LCD_KIT_INFO("upd switch is disable\n");
		return LCD_KIT_FAIL;
	}

	if (ops && ops->lcd_kit_update_fw) {
		ret = ops->lcd_kit_update_fw(panel_id);
		if (ret) {
			LCD_KIT_ERR("failed\n");
			return LCD_KIT_FAIL;
		}
	}

	LCD_KIT_INFO("success\n");
	return ret;
}

static int lcd_kit_off(int panel_id)
{
	int ret = LCD_KIT_OK;

	LCD_KIT_INFO("fb%d, +!\n", panel_id);
	lcd_kit_prepare_disable(panel_id);
	display_engine_compensation_set_dbv(0, 0, panel_id);
	switch (disp_info->lcd_power_seq) {
	case LCD_KIT_POWER_OFF_HS_SEQ:
		lcd_kit_update_fw(panel_id);
		/* send mipi command by high speed */
		if (disp_info->effect_protect.first_off_flag) {
			lcd_kit_get_effect_cmd(panel_id);
			disp_info->effect_protect.first_off_flag = 0;
		}
		if (common_ops->panel_off_hs)
			ret = common_ops->panel_off_hs(panel_id, NULL);

		/* set next step */
		disp_info->lcd_power_seq = LCD_KIT_POWER_OFF_LP_SEQ;
		lcd_kit_set_dubai_hisysevent_power_state(panel_id, LCD_KIT_SET_DUBAI_HISYSEVENT_POWER_OFF);
		break;
	case LCD_KIT_POWER_OFF_LP_SEQ:
		/* send mipi command by low power */
		if (common_ops->panel_off_lp)
			ret = common_ops->panel_off_lp(panel_id, NULL);

		/* set next step */
		disp_info->lcd_power_seq = LCD_KIT_POWER_OFF_SEQ;
		break;
	case LCD_KIT_POWER_OFF_SEQ:
		lcd_kit_power_off_optimize_handle(panel_id);
		/* send mipi command by high speed */
		if (common_ops->panel_power_off)
			ret = common_ops->panel_power_off(panel_id, NULL);
		lcd_kit_hs_off_handle(panel_id);
		/* set next step */
		disp_info->lcd_power_seq = LCD_KIT_POWER_OFF;
		break;
	default:
		break;
	}

	LCD_KIT_INFO("fb%d, -!\n", panel_id);
	return ret;
}

static int lcd_kit_doze(int panel_id)
{
	return common_ops->panel_doze_lp(panel_id, NULL);
}

static int lcd_kit_doze_suspend(int panel_id)
{
	return common_ops->panel_doze_suspend(panel_id, NULL);
}

static int lcd_kit_fake_power_off(int panel_id)
{
	LCD_KIT_INFO("fb%d, +!\n", panel_id);
	if (common_ops->stop_temper_hrtimer &&
		common_info->temper_thread.enable &&
		common_info->temper_thread.dark_enable)
		common_ops->stop_temper_hrtimer(panel_id);

	common_info->fake_power_off_state = true;
	if (disp_info->lcd_power_seq == LCD_KIT_FAKE_POWER_OFF)
		return LCD_KIT_OK;
	display_engine_fake_off_handle(panel_id);
	display_engine_compensation_set_dbv(0, 0, panel_id);
	disp_info->lcd_power_seq = LCD_KIT_FAKE_POWER_OFF;
	lcd_kit_set_aod_panel_state(panel_id, 1);
	lcd_kit_set_dubai_hisysevent_backlight(panel_id, 0);
	lcd_kit_set_dubai_hisysevent_power_state(panel_id, LCD_KIT_SET_DUBAI_HISYSEVENT_FAKE_PWOER_OFF);
#ifdef CONFIG_DFX_ZEROHUNG
	trace_hck_hung_wp_screen_fake_power_off(0);
#endif
	if (common_ops->panel_off_tp) {
		LCD_KIT_INFO("tp off notify\n");
		common_ops->panel_off_tp(panel_id, NULL);
	}

	if (lcd_kit_el2_is_support(panel_id))
		reset_elvss_state(panel_id);

	LCD_KIT_INFO("fb%d, -!\n", panel_id);
	return LCD_KIT_OK;
}

static int lcd_kit_hbm_backlight_handle(int panel_id, unsigned int bl_level)
{
	int ret = LCD_KIT_OK;
	unsigned int hbm_level;
	unsigned int bl_max = common_info->backlight.bl_max;

	if (bl_level > common_info->hbm_backlight.normal_bl_max) {
		hbm_level = display_engine_get_hbm_bl(panel_id, bl_level);
		if (common_ops->enter_hbm && common_info->hbm_backlight.hbm_status == LCD_KIT_HBM_OFF)
			ret = common_ops->enter_hbm(panel_id, NULL, LCD_KIT_BACKLIGHT_HBM);
		if (common_ops->set_hbm_backlight)
			ret = common_ops->set_hbm_backlight(panel_id, NULL, hbm_level);
	} else {
		if (common_ops->exit_hbm && common_info->hbm_backlight.hbm_status == LCD_KIT_HBM_ON)
			ret = common_ops->exit_hbm(panel_id, NULL, LCD_KIT_BACKLIGHT_HBM);
		common_info->backlight.bl_max = common_info->hbm_backlight.normal_bl_max;
		ret = lcd_kit_mipi_set_backlight(panel_id, bl_level);
		common_info->backlight.bl_max = bl_max;
	}

	return ret;
}

static int lcd_kit_set_backlight_sub(int panel_id, unsigned int bl_level)
{
	int ret = LCD_KIT_OK;
	int bl_type;
	unsigned int tuned_level;

	if (!lcd_kit_get_panel_on_state(panel_id)) {
		LCD_KIT_ERR("panel is power off\n");
		return LCD_KIT_FAIL;
	}
#if defined(CONFIG_DKMD_DPU_AOD)
	if (bl_level == 0 && (disp_info->lcd_power_seq == LCD_KIT_FAKE_POWER_OFF ||
		get_lcd_always_on() != 0)) {
		LCD_KIT_INFO("lcd fake off\n");
		return LCD_KIT_OK;
	}
#endif

	bl_type = lcd_kit_get_bl_set_type(panel_id);

	switch (bl_type) {
	case BL_SET_BY_PWM:
		break;
	case BL_SET_BY_BLPWM:
		ret = lcd_kit_blpwm_set_backlight(panel_id, bl_level);
		break;
	case BL_SET_BY_MIPI:
		if (common_info->hbm_backlight.support)
			ret = lcd_kit_hbm_backlight_handle(panel_id, bl_level);
		else
			ret = lcd_kit_mipi_set_backlight(panel_id, bl_level);
		break;
	default:
		LCD_KIT_ERR("not support bl_type\n");
		ret = -1;
		break;
	}

	tuned_level = display_engine_local_hbm_get_mipi_level();
	if (tuned_level == 0)
		tuned_level = bl_level;
	display_engine_compensation_set_dbv(bl_level, tuned_level, panel_id);
#ifdef CONFIG_DFX_ZEROHUNG
	if (ret == LCD_KIT_OK)
		trace_hck_hung_wp_screen_setbl(bl_level);
#endif
#ifdef CONFIG_INPUTHUB_20
#ifdef LCD_ALSC_ENABLE
	save_light_to_sensorhub_panel_id(bl_level, bl_level, panel_id);
#endif
#endif
	lcd_kit_set_dubai_hisysevent_backlight(panel_id, bl_level);
	LCD_KIT_INFO("backlight_level%d = %d\n", panel_id, bl_level);

	return ret;
}

static int lcd_kit_set_backlight(int panel_id, unsigned int bl_level)
{
	int ret = LCD_KIT_OK;
	unsigned int dsi0_index = 0;
	unsigned int connector_id;

	el_ctrl_backlight = bl_level;
	if (lcd_kit_el_ctrl_is_support(panel_id, bl_level)) {
		LCD_KIT_DEBUG("el ctrl support\n");
		need_upate_bl = 1;
		return ret;
	}

	ret = lcd_kit_set_backlight_sub(panel_id, bl_level);
	if (ret == LCD_KIT_OK) {
		connector_id = DPU_PINFO->connector_id;
		ret = lcd_kit_get_dsi_index(&dsi0_index, connector_id);
		if (ret) {
			LCD_KIT_ERR("get dsi0 index error\n");
			return LCD_KIT_FAIL;
		}
		dkmd_dfr_send_refresh(dsi0_index, DPU_PINFO->type);
#ifdef CONFIG_EXT_INPUTHUB_MATTING
		matting_algo_get_current_brightness_callback(bl_level);
#endif
	}

	if (lcd_kit_el2_is_support(panel_id))
		report_el2_dubai_info(panel_id, bl_level);
	return ret;
}

static int lcd_kit_vsync_trigger(int panel_id)
{
	int ret = LCD_KIT_OK;
	unsigned int current_bl_level = el_ctrl_backlight;
	bool need_refresh = false;
	unsigned int dsi0_index = 0;
	unsigned int connector_id;

	if (!lcd_kit_el_ctrl_is_support(panel_id, current_bl_level)) {
		LCD_KIT_DEBUG("el ctrl not support\n");
		return LCD_KIT_FAIL;
	}

	LCD_KIT_DEBUG("current backlight level = %d\n", current_bl_level);
	if (need_upate_bl) {
		ret = lcd_kit_set_backlight_sub(panel_id, current_bl_level);
		if (ret) {
			LCD_KIT_ERR("backlight tx failed\n");
			need_upate_bl = 0;
			return LCD_KIT_FAIL;
		}
		need_upate_bl = 0;
		need_refresh = true;
	}
	ret = lcd_kit_set_el_ctrl_cmds(panel_id, current_bl_level);
	if (!need_refresh && ret == LCD_KIT_OK)
		need_refresh = true;

	if (need_refresh) {
		connector_id = DPU_PINFO->connector_id;
		ret = lcd_kit_get_dsi_index(&dsi0_index, connector_id);
		if (ret) {
			LCD_KIT_ERR("get dsi0 index error\n");
			return LCD_KIT_FAIL;
		}
		dkmd_dfr_send_refresh(dsi0_index, DPU_PINFO->type);
	}

	return ret;
}

static int lcd_kit_get_ddic_cmds(int panel_id,
	const struct disp_effect_params* effect_params, struct dsi_cmds *dsi_cmds)
{
	int i;
	struct disp_effect_item item;
	int bl_level;

	if (!effect_params || !dsi_cmds) {
		LCD_KIT_ERR("effect_params or dsi_cmds is null\n");
		return LCD_KIT_FAIL;
	}

	if (effect_params->effect_num > EFFECT_PARAMS_MAX_NUM) {
		LCD_KIT_ERR("effect num abnormal, %u\n", effect_params->effect_num);
		return LCD_KIT_FAIL;
	}

	LCD_KIT_INFO("effect_num is %u\n", effect_params->effect_num);

	for (i = 0; i < effect_params->effect_num; i++) {
		item =  effect_params->params[i];
		if (item.effect_type == DISP_EFFECT_WITH_BL) {
			bl_level = item.effect_values[0];
			lcd_kit_get_bl_cmds(panel_id, bl_level, dsi_cmds);
		} else if (item.effect_type == DISP_EFFECT_TE2_FIXED) {
			lcd_kit_get_fixed_te2_cmds(panel_id, dsi_cmds);
		} else if (item.effect_type == DISP_EFFECT_TE2_FOLLOWED) {
			lcd_kit_get_follow_te2_cmds(panel_id, dsi_cmds);
		}
	}

	return LCD_KIT_OK;
}

static int lcd_kit_get_cmds_tx_params(int panel_id,
	const struct dkmd_cmds_info *cmds_info, struct mipi_dsi_tx_params *params)
{
	int i;

	if (!cmds_info || !params) {
		LCD_KIT_ERR("cmds_info or params is null\n");
		return LCD_KIT_FAIL;
	}

	if (cmds_info->cmds_type > DKMD_CMDS_TYPE_MAX) {
		LCD_KIT_ERR("cmds_type abnormal, %u\n", cmds_info->cmds_type);
		return LCD_KIT_FAIL;
	}
	if (!lcd_kit_get_panel_on_state(panel_id)) {
		LCD_KIT_ERR("panel is power off\n");
		return LCD_KIT_FAIL;
	}

	for (i = 0; i < ARRAY_SIZE(async_tx_params); i++) {
		if (cmds_info->cmds_type != async_tx_params[i].type)
			continue;
		LCD_KIT_INFO("cmds_info->cmds_type = %u\n", cmds_info->cmds_type);
		if (async_tx_params[i].func)
			(*async_tx_params[i].func)(panel_id, cmds_info, params);
	}

	return LCD_KIT_OK;
}

static int lcd_kit_get_dual_cmds_tx_params(int panel_id,
	const struct dkmd_cmds_info *cmds_info, struct mipi_dsi_tx_params *params0,
	struct mipi_dsi_tx_params *params1)
{
	int i;
 
	if (!cmds_info || !params0 || !params1) {
		LCD_KIT_ERR("cmds_info or params is null\n");
		return LCD_KIT_FAIL;
	}
 
	if (cmds_info->cmds_type > DKMD_CMDS_TYPE_MAX) {
		LCD_KIT_ERR("cmds_type abnormal, %u\n", cmds_info->cmds_type);
		return LCD_KIT_FAIL;
	}
	if (!lcd_kit_get_panel_on_state(panel_id)) {
		LCD_KIT_ERR("panel is power off\n");
		return LCD_KIT_FAIL;
	}
 
	for (i = 0; i < ARRAY_SIZE(dual_async_tx_params); i++) {
		if (cmds_info->cmds_type != dual_async_tx_params[i].type)
			continue;
		LCD_KIT_INFO("cmds_info->cmds_type = %u\n", cmds_info->cmds_type);
		if (dual_async_tx_params[i].func)
			(*dual_async_tx_params[i].func)(panel_id, cmds_info, params0, params1);
	}

	return LCD_KIT_OK;
}
 

static int lcd_kit_trace_screen_bl(int panel_id, unsigned int bl_level)
{
	int ret = LCD_KIT_OK;
	unsigned int tuned_level;
#ifdef CONFIG_DFX_ZEROHUNG
	trace_hck_hung_wp_screen_setbl(bl_level);
#endif
#ifdef CONFIG_INPUTHUB_20
#ifdef LCD_ALSC_ENABLE
	save_light_to_sensorhub_panel_id(bl_level, bl_level, panel_id);
#endif
#endif

	if (common_info->grayscale_backlight.support && common_ops->set_gray_backlight_cmd)
		ret = common_ops->set_gray_backlight_cmd(panel_id, NULL, bl_level);

	lcd_kit_set_dubai_hisysevent_backlight(panel_id, bl_level);
	LCD_KIT_INFO("backlight_level%d = %d\n", panel_id, bl_level);
	display_engine_brightness_update(panel_id, bl_level);

	tuned_level = display_engine_local_hbm_get_mipi_level();
	if (tuned_level == 0)
		tuned_level = bl_level;
	display_engine_compensation_set_dbv(bl_level, tuned_level, panel_id);

	if (lcd_kit_el2_is_support(panel_id))
		report_el2_dubai_info(panel_id, bl_level);
	return ret;
}

static int lcd_kit_post_process(struct device *dev)
{
	/* create sysfs */
	LCD_KIT_INFO("post process begin\n");
	lcd_kit_sysfs_init(dev);

	LCD_KIT_INFO("post process end\n");
	return LCD_KIT_OK;
}

static int lcd_kit_esd_check(int panel_id)
{
	int ret = LCD_KIT_OK;
	unsigned int esd_support;

	if (!disp_info->esd_enable) {
		LCD_KIT_INFO("ESD disable\n");
		return ret;
	}

	if (runmode_is_factory())
		esd_support = common_info->esd.fac_esd_support;
	else
		esd_support = common_info->esd.support;

	if (esd_support && common_ops->esd_handle)
		ret = common_ops->esd_handle(panel_id, NULL);

	if (runmode_is_factory() && disp_info->check_mipi.support) {
		LCD_KIT_INFO("check mipi start\n");
		lcd_kit_check_mipi_error(panel_id);
	}

	return ret;
}

static void lcd_kit_set_rd_cmds(int panel_id, void *hld, struct region_rect *dirty)
{
	struct region_rect *dirty_region = NULL;

	dirty_region = (struct region_rect *)dirty;
	/* change region to dsi cmds */
	common_info->dirty_region.cmds.cmds[0].payload[1] =
		((dirty_region->x) >> 8) & 0xff;
	common_info->dirty_region.cmds.cmds[0].payload[2] =
		(dirty_region->x) & 0xff;
	common_info->dirty_region.cmds.cmds[0].payload[3] =
		((dirty_region->x + dirty_region->w - 1) >> 8) & 0xff;
	common_info->dirty_region.cmds.cmds[0].payload[4] =
		(dirty_region->x + dirty_region->w - 1) & 0xff;
	common_info->dirty_region.cmds.cmds[1].payload[1] =
		((dirty_region->y) >> 8) & 0xff;
	common_info->dirty_region.cmds.cmds[1].payload[2] =
		(dirty_region->y) & 0xff;
	common_info->dirty_region.cmds.cmds[1].payload[3] =
		((dirty_region->y + dirty_region->h - 1) >> 8) & 0xff;
	common_info->dirty_region.cmds.cmds[1].payload[4] =
		(dirty_region->y + dirty_region->h - 1) & 0xff;
}

static void lcd_kit_set_xhs_cmds(int panel_id, void *hld, struct region_rect *dirty)
{
	struct region_rect *dirty_region = NULL;

	dirty_region = (struct region_rect *)dirty;
	/* change region to dsi cmds */
	common_info->dirty_region.cmds.cmds[0].payload[1] =
		(dirty_region->x) & 0xff;
	common_info->dirty_region.cmds.cmds[0].payload[2] =
		((dirty_region->x) >> 8) & 0xff;
	common_info->dirty_region.cmds.cmds[0].payload[3] =
		(dirty_region->x + dirty_region->w - 1) & 0xff;
	common_info->dirty_region.cmds.cmds[0].payload[4] =
		((dirty_region->x + dirty_region->w - 1) >> 8) & 0xff;
	common_info->dirty_region.cmds.cmds[1].payload[1] =
		(dirty_region->y) & 0xff;
	common_info->dirty_region.cmds.cmds[1].payload[2] =
		((dirty_region->y) >> 8) & 0xff;
	common_info->dirty_region.cmds.cmds[1].payload[3] =
		(dirty_region->y + dirty_region->h - 1) & 0xff;
	common_info->dirty_region.cmds.cmds[1].payload[4] =
		((dirty_region->y + dirty_region->h - 1) >> 8) & 0xff;
}

static int lcd_kit_dirty_region_handle_ddic(int panel_id, void *hld, struct region_rect *dirty)
{
	int ret = LCD_KIT_OK;
	unsigned int ddic_type;
	struct lcd_kit_adapt_ops *adapt_ops = NULL;
	struct dpu_panel_info *pinfo = NULL;

	pinfo = lcd_kit_get_dpu_pinfo(panel_id);
	if (!pinfo) {
		LCD_KIT_ERR("pinfo is null\n");
		return LCD_KIT_FAIL;
	}
	adapt_ops = lcd_kit_get_adapt_ops();
	if (!adapt_ops || !adapt_ops->mipi_tx) {
		LCD_KIT_ERR("can not register adapt_ops or mipi_tx!\n");
		return LCD_KIT_FAIL;
	}
	if (dirty == NULL) {
		LCD_KIT_ERR("dirty is null point!\n");
		return LCD_KIT_FAIL;
	}
	ddic_type = pinfo->dfr_info.ddic_type;
	LCD_KIT_INFO("the ddic type is %d\n", ddic_type);
	if (common_info->dirty_region.support) {
		if (ddic_type)
			lcd_kit_set_xhs_cmds(panel_id, hld, dirty);
		else 
			lcd_kit_set_rd_cmds(panel_id, hld, dirty);
		ret = adapt_ops->mipi_tx(panel_id, hld, &common_info->dirty_region.cmds);
	}
	return ret;
}

static int lcd_kit_set_display_region(int panel_id, struct dkmd_rect *dirty)
{
	int ret = LCD_KIT_OK;

	LCD_KIT_INFO("set display_region\n");
	ret = lcd_kit_dirty_region_handle_ddic(panel_id, NULL,
			(struct region_rect *)dirty);

	return ret;
}

static int lcd_kit_common_probe(int panel_id,
	struct device_node *np)
{
	struct dpu_panel_info *pinfo = NULL;
	struct dpu_dynamic_panel_info *dynamic_pinfo = NULL;

	pinfo = lcd_kit_get_dpu_pinfo(panel_id);
	if (!pinfo) {
		LCD_KIT_ERR("pinfo is null\n");
		return LCD_KIT_FAIL;
	}

	dynamic_pinfo = lcd_kit_get_dpu_dynamic_panel_info(panel_id);
	if (!dynamic_pinfo) {
		LCD_KIT_ERR("dynamic_pinfo is null\n");
		return LCD_KIT_FAIL;
	}

	memset(pinfo, 0, sizeof(struct dpu_panel_info));
	memset(dynamic_pinfo, 0, sizeof(struct dpu_dynamic_panel_info));

	pinfo->product_type = PUBLIC_INFO->product_type;
	/* common init */
	if (common_ops->common_init)
		common_ops->common_init(panel_id, np);

	/* utils init */
	lcd_kit_utils_init(panel_id, np, pinfo);

	/* dynamic_panel_info init */
	lcd_kit_dynamic_panel_info_init(panel_id, np, pinfo, dynamic_pinfo);

#ifdef LCD_FACTORY_MODE
	lcd_factory_init(panel_id, np);
#endif
#ifdef LCD_KIT_DEBUG_ENABLE
	/* debugfs init */
	lcd_kit_debug_init();
	lcd_kit_debugfs_init();
#endif
	lcd_kit_register_power_key_notify(panel_id);
	lcd_kit_scp_init(panel_id, np);

	return LCD_KIT_OK;
}

static int lcd_kit_set_fps_scence(int panel_id, unsigned int current_fps)
{
	LCD_KIT_INFO("current_fps = %u\n", current_fps);
	disp_info->fps.current_fps = current_fps;
	common_info->fps_on.current_fps = disp_info->fps.current_fps;
	return LCD_KIT_OK;
}

static int fps_cmd_send(int panel_id, struct lcd_kit_dsi_panel_cmds *fps_cmds, unsigned int fps)
{
	struct lcd_kit_adapt_ops *adapt_ops = NULL;
	struct dpu_panel_info *pinfo = NULL;
	adapt_ops = lcd_kit_get_adapt_ops();
	pinfo = lcd_kit_get_dpu_pinfo(panel_id);

	if (!adapt_ops) {
		LCD_KIT_ERR("can not get adapt_ops!\n");
		return LCD_KIT_FAIL;
	}
	if (!pinfo) {
		LCD_KIT_ERR("can not get dpu_pinfo!\n");
		return LCD_KIT_FAIL;
	}
	if (!lcd_kit_get_panel_on_state(panel_id)) {
		if (pinfo->dfr_info.oled_info.oled_type == LTPS) {
			disp_info->fps.current_fps = fps;
			common_info->fps_on.current_fps = disp_info->fps.current_fps;
			LCD_KIT_WARNING("ltps change fps in power down stage\n");
		}
		LCD_KIT_ERR("panel is power off\n");
		return LCD_KIT_FAIL;
	}
	if (adapt_ops->mipi_tx_no_lock == NULL) {
		LCD_KIT_ERR("mipi_tx_no_lock is NULL\n");
		return LCD_KIT_FAIL;
	}

	return adapt_ops->mipi_tx_no_lock(panel_id, NULL, fps_cmds);
}

static int lcd_kit_updt_fps(int panel_id, unsigned int fps)
{
	int ret = LCD_KIT_OK;
	int table_num;
	int i;
	struct ltps_fps_cmd_table fps_lock_cmd[] = {
		{ FPS_30, &disp_info->fps.fps_to_30_cmds },
		{ FPS_60, &disp_info->fps.fps_to_60_cmds },
		{ FPS_90, &disp_info->fps.fps_to_90_cmds },
		{ FPS_120, &disp_info->fps.fps_to_120_cmds },
		{ FPS_144, &disp_info->fps.fps_to_144_cmds },
	};

	LCD_KIT_INFO("fps = %u\n", fps);
	table_num = sizeof(fps_lock_cmd) / sizeof(fps_lock_cmd[0]);
	for (i = 0; i < table_num; i++) {
		if (fps == (unsigned int)fps_lock_cmd[i].val &&
			(fps_lock_cmd[i].cmds != NULL && fps_lock_cmd[i].cmds->cmds != NULL))
			ret = fps_cmd_send(panel_id, fps_lock_cmd[i].cmds, fps);
	}
	LCD_KIT_INFO("ret = %d\n", ret);
	if (ret == LCD_KIT_OK) {
		disp_info->fps.current_fps = fps;
		common_info->fps_on.current_fps = disp_info->fps.current_fps;
	}
	return ret;
}

static int te_config_cmd_send(int panel_id, struct lcd_kit_dsi_panel_cmds *te_cmds)
{
	struct lcd_kit_adapt_ops *adapt_ops = NULL;

	adapt_ops = lcd_kit_get_adapt_ops();
	if (!adapt_ops) {
		LCD_KIT_ERR("can not get adapt_ops!\n");
		return LCD_KIT_FAIL;
	}

	if (!lcd_kit_get_panel_on_state(panel_id)) {
		LCD_KIT_ERR("panel is power off\n");
		return LCD_KIT_FAIL;
	}

	if (adapt_ops->mipi_tx == NULL) {
		LCD_KIT_ERR("mipi_tx_no_lock is NULL\n");
		return LCD_KIT_FAIL;
	}

	return adapt_ops->mipi_tx(panel_id, NULL, te_cmds);
}

static int lcd_kit_updt_fps_te_mode(int panel_id, unsigned int fps, bool is_need_delay)
{
	int ret = LCD_KIT_OK;
	int table_num;
	int i;
	struct ltps_fps_cmd_table te_config_cmd[] = {
		{ FPS_60, &disp_info->updt_fps_te.fps_60_cmds },
		{ FPS_120, &disp_info->updt_fps_te.fps_120_cmds },
		{ FPS_144, &disp_info->updt_fps_te.fps_144_cmds },
	};

	if (!disp_info->updt_fps_te.support) {
		LCD_KIT_INFO("not support updt_fps_te\n");
		return ret;
	}

	table_num = ARRAY_SIZE(te_config_cmd);
	for (i = 0; i < table_num; i++) {
		if (fps == (unsigned int)te_config_cmd[i].val)
			ret = te_config_cmd_send(panel_id, te_config_cmd[i].cmds);
	}

	if (is_need_delay && disp_info->updt_fps_te.cmds_delay)
		lcd_kit_delay(disp_info->updt_fps_te.cmds_delay, LCD_KIT_WAIT_MS, true);

	return ret;
}

static int lcd_kit_updt_fps_pfm_mode(int panel_id, unsigned int current_fps)
{
	if (disp_info->panel_pfm.support) {
		LCD_KIT_INFO("current_fps:%d", current_fps);
		lcd_kit_fps_pfm_ctrl(panel_id, current_fps);
	}
	return LCD_KIT_OK;
}

static int lcd_kit_set_ppc_config_id(int panel_id, unsigned int ppc_config_id)
{
	if (!common_info->ppc_para.panel_partial_ctrl_support) {
		LCD_KIT_INFO("panel partial ctrl not support\n");
		return LCD_KIT_OK;
	}
	LCD_KIT_INFO("panel partial ctrl id:%u\n", ppc_config_id);
	common_info->ppc_para.ppc_config_id = ppc_config_id;
	return LCD_KIT_OK;
}

static int lcd_kit_set_dsc_econfig(int panel_id, uint8_t dsc_enable)
{
	if (!common_info->dsc_switch.support) {
		LCD_KIT_INFO("panel dsc switch ctrl not support\n");
		return LCD_KIT_OK;
	}
	LCD_KIT_INFO("panel_id = %d, dsc ctrl id:%u\n", panel_id, dsc_enable);
	common_info->dsc_switch.power_compress_enable = dsc_enable;
	return LCD_KIT_OK;
}

static int change_exce_mode(int panel_id, uint32_t mode)
{
	int ret = LCD_KIT_OK;
	struct lcd_kit_adapt_ops *adapt_ops = lcd_kit_get_adapt_ops();
	int recover_power_seq = LCD_KIT_POWER_NONE;

    if (!adapt_ops || !adapt_ops->mipi_tx) {
        LCD_KIT_DEBUG("can not register adaptOps!\n");
        return LCD_KIT_FAIL;
    }
	if (mode == 1) { // before dss off , enter exce mode
		LCD_KIT_INFO("disp_info->lcd_power_seq is %d\n", disp_info->lcd_power_seq);
		// modify power seq to ensure mipi tx
		if (disp_info->lcd_power_seq == LCD_KIT_POWER_OFF_SEQ || disp_info->lcd_power_seq == LCD_KIT_FAKE_POWER_OFF) {
			recover_power_seq = disp_info->lcd_power_seq;
			disp_info->lcd_power_seq = LCD_KIT_POWER_OFF_LP_SEQ;
		}
		ret = adapt_ops->mipi_tx(panel_id, NULL, &common_info->exce_mode.enter_cmd);
		if (ret != LCD_KIT_OK) {
			LCD_KIT_ERR("mipi_tx fail\n");
			return ret;
		}
		LCD_KIT_INFO("enter exce mode\n");
		// recover power seq
		if (recover_power_seq != LCD_KIT_POWER_NONE)
			disp_info->lcd_power_seq = recover_power_seq;
		LCD_KIT_INFO("disp_info->lcd_power_seq is %d\n", disp_info->lcd_power_seq);
	}
	if (mode == 0) { // after dss on , exit exce mode
		ret = adapt_ops->mipi_tx(panel_id, NULL, &common_info->exce_mode.exit_cmd);
		if (ret != LCD_KIT_OK) {
			LCD_KIT_ERR("mipi_tx fail\n");
			return ret;
		}
		LCD_KIT_INFO("exit exce mode\n");
	}
	return ret;
}

static int check_exce_mode(int panel_id, uint32_t mode)
{
	int ret;
	int i;
	uint8_t read_value[MAX_REG_READ_COUNT] = {0};
	struct lcd_kit_adapt_ops *adapt_ops = lcd_kit_get_adapt_ops();

    if (!adapt_ops || !adapt_ops->mipi_rx) {
        LCD_KIT_DEBUG("can not register adaptOps!\n");
        return LCD_KIT_FAIL;
    }
	ret = adapt_ops->mipi_rx(panel_id, NULL, (u8 *)read_value, MAX_REG_READ_COUNT - 1,
		&common_info->exce_mode.check_cmd);
	for (i = 0; i < MAX_REG_READ_COUNT; i++)
		LCD_KIT_INFO("read_value[%d] = 0x%x!\n", i, read_value[i]);
	return ret;
}

static int lcd_kit_ddic_exception_mode_cfg(int panel_id, uint32_t mode)
{
	int ret = LCD_KIT_OK;

	if (!common_info->exce_mode.support) {
		LCD_KIT_INFO("panel exce_mode not support\n");
		return LCD_KIT_OK;
	}
	LCD_KIT_INFO("panel_id = %d, mode:%d\n", panel_id, mode);

	ret = change_exce_mode(panel_id, mode);
	if (ret != LCD_KIT_OK) {
		LCD_KIT_ERR("change_exce_mode fail\n");
		return ret;
	}
	if (mode == 0) // after exit mode,read reg
		ret = check_exce_mode(panel_id, mode);
	return ret;
}

static int lcd_kit_skip_tp_notify(int panel_id, bool skip_tp)
{
	common_info->need_skip_tp_notify = skip_tp;
	LCD_KIT_INFO("need skip tp notify is %s\n",
		common_info->need_skip_tp_notify == true ? "need" : "Doesn't need");
	return LCD_KIT_OK;
}

static int lcd_kit_primary_set_fastboot(void)
{
	return lcd_kit_set_fastboot(PRIMARY_PANEL);
}

static int lcd_kit_primary_on(void)
{
	return lcd_kit_on(PRIMARY_PANEL);
}

static int lcd_kit_primary_off(void)
{
	return lcd_kit_off(PRIMARY_PANEL);
}

static int lcd_kit_primary_doze(void)
{
	return lcd_kit_doze(PRIMARY_PANEL);
}

static int lcd_kit_primary_doze_suspend(void)
{
	return lcd_kit_doze_suspend(PRIMARY_PANEL);
}

static int lcd_kit_primary_set_backlight(unsigned int bl_level)
{
	return lcd_kit_set_backlight(PRIMARY_PANEL, bl_level);
}

static int lcd_kit_primary_vsync_trigger(void)
{
	return lcd_kit_vsync_trigger(PRIMARY_PANEL);
}

struct dpu_panel_info *lcd_kit_get_primary_dpu_pinfo(void)
{
	return lcd_kit_get_dpu_pinfo(PRIMARY_PANEL);
}

struct dpu_dynamic_panel_info *lcd_kit_get_primary_dynamic_panel_info(uint32_t panel_id)
{
	return lcd_kit_get_dpu_dynamic_panel_info(PRIMARY_PANEL);
}

static int lcd_kit_primary_post_process(struct device *dev)
{
	return lcd_kit_post_process(dev);
}

static int lcd_kit_primary_dump_info(void)
{
	check_panel_on_state(PRIMARY_PANEL);
	return LCD_KIT_OK;
}

static int lcd_kit_set_primary_fps_scence(unsigned int current_fps)
{
	return lcd_kit_set_fps_scence(PRIMARY_PANEL, current_fps);
}

static int lcd_kit_primary_get_ddic_cmds(const struct disp_effect_params* effect_params,
	struct dsi_cmds *dsi_cmds)
{
	return lcd_kit_get_ddic_cmds(PRIMARY_PANEL, effect_params, dsi_cmds);
}

static int lcd_kit_primary_get_cmds_tx_params(const struct dkmd_cmds_info *cmds_info,
	struct mipi_dsi_tx_params *params)
{
	return lcd_kit_get_cmds_tx_params(PRIMARY_PANEL, cmds_info, params);
}

static int lcd_kit_primary_get_dual_cmds_tx_params(const struct dkmd_cmds_info *cmds_info,
	struct mipi_dsi_tx_params *params0, struct mipi_dsi_tx_params *params1)
{
	return lcd_kit_get_dual_cmds_tx_params(PRIMARY_PANEL, cmds_info, params0, params1);
}

static int lcd_kit_primary_trace_screen_bl(unsigned int bl_level)
{
	return lcd_kit_trace_screen_bl(PRIMARY_PANEL, bl_level);
}

static int lcd_kit_primary_updt_fps(unsigned int fps)
{
	return lcd_kit_updt_fps(PRIMARY_PANEL, fps);
}

static int lcd_kit_primary_esd_check(void)
{
	return lcd_kit_esd_check(PRIMARY_PANEL);
}

static int lcd_kit_primary_set_display_region(struct dkmd_rect *dirty)
{
	return lcd_kit_set_display_region(PRIMARY_PANEL, dirty);
}

static int lcd_kit_primary_fake_power_off(void)
{
	return lcd_kit_fake_power_off(PRIMARY_PANEL);
}

static int lcd_kit_primary_updt_fps_te_mode(unsigned int fps)
{
	return lcd_kit_updt_fps_te_mode(PRIMARY_PANEL, fps, false);
}

static int lcd_kit_primary_updt_fps_pfm_mode(unsigned int fps)
{
	return lcd_kit_updt_fps_pfm_mode(PRIMARY_PANEL, fps);
}

static int lcd_kit_primary_set_ppc_config_id(unsigned int ppc_config_id)
{
	return lcd_kit_set_ppc_config_id(PRIMARY_PANEL, ppc_config_id);
}

static int lcd_kit_primary_notify_sfr_info(const struct sfr_info *sfr_info)
{
	return lcd_kit_safe_frm_rate_ctrl(PRIMARY_PANEL, sfr_info);
}

static int lcd_kit_set_primary_dsc_econfig(uint8_t dsc_enable)
{
	return lcd_kit_set_dsc_econfig(PRIMARY_PANEL, dsc_enable);
}

static int lcd_kit_primary_ddic_exception_mode_cfg(uint32_t mode)
{
	return lcd_kit_ddic_exception_mode_cfg(PRIMARY_PANEL, mode);
}

static int lcd_kit_primary_skip_tp_notify(bool skip_tp)
{
	return lcd_kit_skip_tp_notify(PRIMARY_PANEL, skip_tp);
}

static struct dpu_panel_ops primary_dpu_panel_data = {
	.set_fastboot = lcd_kit_primary_set_fastboot,
	.on = lcd_kit_primary_on,
	.off = lcd_kit_primary_off,
	.doze = lcd_kit_primary_doze,
	.doze_suspend = lcd_kit_primary_doze_suspend,
	.esd_handle = lcd_kit_primary_esd_check,
	.set_backlight = lcd_kit_primary_set_backlight,
	.send_cmds_at_vsync = lcd_kit_primary_vsync_trigger,
	.get_panel_info = lcd_kit_get_primary_dpu_pinfo,
	.disp_postprocess = lcd_kit_primary_post_process,
	.dump_exception_info = lcd_kit_primary_dump_info,
	.set_current_fps = lcd_kit_set_primary_fps_scence,
	.get_ddic_cmds = lcd_kit_primary_get_ddic_cmds,
	.get_cmds_tx_params = lcd_kit_primary_get_cmds_tx_params,
	.get_dual_cmds_tx_params = lcd_kit_primary_get_dual_cmds_tx_params,
	.trace_screen_bl = lcd_kit_primary_trace_screen_bl,
	.update_lcd_fps = lcd_kit_primary_updt_fps,
	.set_display_region = lcd_kit_primary_set_display_region,
	.fake_power_off = lcd_kit_primary_fake_power_off,
	.update_fps_te_mode = lcd_kit_primary_updt_fps_te_mode,
	.update_fps_pfm_mode = lcd_kit_primary_updt_fps_pfm_mode,
	.set_ppc_config_id = lcd_kit_primary_set_ppc_config_id,
	.notify_sfr_info = lcd_kit_primary_notify_sfr_info,
	.get_dynamic_panel_info = lcd_kit_get_primary_dynamic_panel_info,
	.set_dsc_config = lcd_kit_set_primary_dsc_econfig,
	.ddic_exception_mode_cfg = lcd_kit_primary_ddic_exception_mode_cfg,
	.skip_tp_notify = lcd_kit_primary_skip_tp_notify,
};

static int lcd_kit_io_crtl(uint32_t cmd, uint64_t arg)
{
	int ret = 0;
	void __user *argp = (void __user *)(uintptr_t)arg;
	if (unlikely(!argp))
		return -1;
	switch (cmd) {
	case LCDKIT_IOCTL_DISPLAY_ENGINE_PARAM_GET:
		ret = display_engine_get_param(argp);
		break;
	case LCDKIT_IOCTL_DISPLAY_ENGINE_PARAM_SET:
		ret = display_engine_set_param(argp);
		break;
	default:
		LCD_KIT_ERR("unknow cmd %u\n", cmd);
		return -1;
	}
	return ret;
}

static struct product_display_ops product_io_ops = {
	.fb_ioctl = lcd_kit_io_crtl,
};

#if (defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM))
/* uefi dmd report */
static void lcd_kit_primary_probe_dmd_report(void)
{
	if (!lcd_dclient)
		return;

	if (PUBLIC_INFO->dvdd_avs_dmd && strlen(PUBLIC_INFO->dvdd_avs_dmd) > 0) {
		if (dsm_client_ocuppy(lcd_dclient))
			return;
		/* lcdHpmDmd */
		LCD_KIT_ERR("report dvdd_avs_dmd\n");
		dsm_client_record(lcd_dclient,
			"lcd dvdd_avs_dmd hpm invalid:%s\n",
			PUBLIC_INFO->dvdd_avs_dmd);
		dsm_client_notify(lcd_dclient, DSM_LCD_POWER_STATUS_ERROR_NO);
	}
}
#endif

#if (defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM))
/* uefi dmd report */
static void lcd_kit_secondary_probe_dmd_report(void)
{
	if (!lcd_dclient)
		return;

	if (PUBLIC_INFO->sub_dvdd_avs_dmd && strlen(PUBLIC_INFO->sub_dvdd_avs_dmd) > 0) {
		if (dsm_client_ocuppy(lcd_dclient))
			return;
		/* lcdHpmDmd */
		LCD_KIT_ERR("report sub_dvdd_avs_dmd\n");
		dsm_client_record(lcd_dclient,
			"lcd sub_dvdd_avs_dmd hpm invalid:%s\n",
			PUBLIC_INFO->sub_dvdd_avs_dmd);
		dsm_client_notify(lcd_dclient, DSM_LCD_POWER_STATUS_ERROR_NO);
	}
}
#endif

static void lcd_kit_completion_init(int panel_id)
{
	init_completion(&disp_info->lcd_completion.tx_no_lock_done);
	disp_info->lcd_completion.is_in_tx_no_lock = false;
	init_completion(&disp_info->lcd_completion.mipi_switch_done);
	disp_info->lcd_completion.mipi_occupy = false;
	common_info->mipi_switch_end = false;
	disp_info->lcd_completion.comp_inited = true;
}

#ifdef CONFIG_GPIO_HI9030
static void lcd_kit_pmic_9030_irq_handler(int gpio_id, void *data)
{
	int panel_id = lcd_kit_get_active_panel_id();
	int ret;
	static int pmic_number = 0;
	LCD_KIT_INFO("PMIC test response one pmic irq\n");
	if (pmic_number > (int)disp_info->pmic_irq_check.pmic_reset_number) {
		LCD_KIT_ERR("PMIC reset not work\n");
		return;
	}
	ret = dpu_gfxdev_power_ctl(panel_id);
	if (!ret) {
		pmic_number++;
		if (pmic_number > (int)disp_info->pmic_irq_check.pmic_reset_number)
			lcd_kit_pmic_number_dmd_report(panel_id);
	}
}
#endif

static irqreturn_t lcd_kit_pmic_irq_handler(int irq, void *dev_id)
{
	int panel_id = lcd_kit_get_active_panel_id();
	int ret;
	static int pmic_number = 0;
	LCD_KIT_INFO("PMIC test response one pmic irq\n");

	if (disp_info->pmic_irq_check.adc_support) {
		lcd_kit_pmic_adc_dmd_report(panel_id);
		return IRQ_HANDLED;
	}

	if (pmic_number > (int)disp_info->pmic_irq_check.pmic_reset_number) {
		LCD_KIT_ERR("PMIC reset not work\n");
		return IRQ_HANDLED;
	}
	lcd_kit_pmic_irq_dmd_report(panel_id);
	ret = dpu_gfxdev_power_ctl(panel_id);
	if (!ret) {
		pmic_number++;
		if (pmic_number > (int)disp_info->pmic_irq_check.pmic_reset_number)
			lcd_kit_pmic_number_dmd_report(panel_id);
	}

	return IRQ_HANDLED;
}

static void lcd_kit_response_pmic_irq(int panel_id)
{
	int ret;
	int irq_num;
	unsigned int gpio_num;

	if (!disp_info->pmic_irq_check.support) {
		LCD_KIT_ERR("response pmic irq not support\n");
		return;
	}
	if (power_hdl->pmic_irq.buf == NULL)
		return;

	gpio_num = power_hdl->pmic_irq.buf[POWER_NUM];
	ret = gpio_request(gpio_num, "irq_pmic_gpio");
	if (ret) {
		LCD_KIT_ERR("lcd_kit_gpio[%u] request fail\n", gpio_num);
		return;
	}
	ret = gpio_direction_input(gpio_num);
	if (ret) {
		gpio_free(gpio_num);
		LCD_KIT_ERR("lcd_kit_gpio[%u] direction input set fail\n", gpio_num);
		return;
	}
	if (disp_info->pmic_irq_check.hi9030_support) {
#ifdef CONFIG_GPIO_HI9030
		LCD_KIT_INFO("in HI9030 pmic irq\n");
		ret = hi9030_gpio_irq_request(gpio_num, lcd_kit_pmic_9030_irq_handler, IRQF_TRIGGER_FALLING, NULL);
#endif
	} else {
		irq_num = gpio_to_irq(gpio_num);
		LCD_KIT_INFO("pmic irq irq_num = %d\n", irq_num);
		(void)request_threaded_irq(irq_num, NULL, lcd_kit_pmic_irq_handler,
				IRQF_TRIGGER_FALLING|IRQF_ONESHOT, "pmic_irq", NULL);
	}
	if (ret) {
		LCD_KIT_ERR("gpio_irq_request[%u] request fail\n", gpio_num);
		return;
	}
}

static void lcd_kit_execute_lcd_ops(int panel_id)
{
	int ret = LCD_KIT_OK;
	struct lcd_kit_ops *lcd_ops = NULL;

	lcd_ops = lcd_kit_get_ops();
	if (!lcd_ops) {
		LCD_KIT_ERR("lcd ops is NULL\n");
		return;
	}
	if (lcd_ops->get_sn_code) {
		ret = lcd_ops->get_sn_code(panel_id);
		if (ret < 0)
			LCD_KIT_ERR("get sn code failed!\n");
	}
}

static void lcd_kit_primary_sub_probe(struct platform_device *pdev)
{
#if (defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM))
	lcd_dclient = dkmd_get_dmd_client();
	lcd_kit_primary_probe_dmd_report();
#endif

	// lcd color calib check
	lcd_kit_color_calib_check(PRIMARY_PANEL);
	/* execute lcd ops */
	lcd_kit_execute_lcd_ops(PRIMARY_PANEL);
	lcd_kit_get_el_ctrl_info(PRIMARY_PANEL);
	lcd_kit_get_el_ctrl_info_ver_two(PRIMARY_PANEL);
	lcd_kit_response_pmic_irq(PRIMARY_PANEL);
	lcd_kit_panel_init(PRIMARY_PANEL, pdev);
	dpu_elvss_info_init(PRIMARY_PANEL);
	clear_el_ctrl_input_info(PRIMARY_PANEL);

#ifdef CONFIG_LCD_KIT_HYBRID
	lcd_kit_hybrid_init();
#endif
}

static int lcd_kit_primary_probe(struct platform_device *pdev)
{
	struct device_node *np = NULL;
	int ret;

	np = pdev->dev.of_node;
	if (!np) {
		LCD_KIT_ERR("not find device node\n");
		return LCD_KIT_FAIL;
	}

#ifdef LCD_KIT_DEBUG_ENABLE
	if (dpd_init(pdev)) {
		LCD_KIT_INFO("sysfs is not ready!\n");
		return -EPROBE_DEFER;
	}
#endif

	LCD_KIT_INFO("primary panel porbe+\n");
	lcd_kit_adapt_init();
	lcd_kit_completion_init(PRIMARY_PANEL);

	ret = lcd_kit_common_probe(PRIMARY_PANEL, np);
	if (ret) {
		LCD_KIT_ERR("primary panel common probe fail\n");
		return LCD_KIT_FAIL;
	}
#ifdef LCD_KIT_DEBUG_ENABLE
	if (is_dpd_mode())
		dpd_regu_init(pdev);
	else
		lcd_kit_power_init(PRIMARY_PANEL, pdev);
#else
	/* power init */
	lcd_kit_power_init(PRIMARY_PANEL, pdev);
#endif
	/* register to dkmd */
	ret = register_panel(&primary_dpu_panel_data, PRIMARY_PANEL);
	if (ret) {
		LCD_KIT_ERR("dpu_add_driver failed!\n");
		goto err_device_put;
	}

	/* register product_io_ops to dkmd */
	ret = regitster_product_ops(&product_io_ops);
	if (ret) {
		LCD_KIT_ERR("regitster_product_ops failed!\n");
		goto err_device_put;
	}
	lcd_kit_primary_sub_probe(pdev);
	LCD_KIT_INFO("primary panel porbe-\n");

#ifdef CONFIG_EXT_INPUTHUB_MATTING_ALSC
	matting_algo_als_alsc_expand_status_config();
#endif

	return LCD_KIT_OK;
err_device_put:
	return -EPROBE_DEFER;
}

static int lcd_kit_secondary_set_fastboot(void)
{
	return lcd_kit_set_fastboot(SECONDARY_PANEL);
}

static int lcd_kit_secondary_on(void)
{
	return lcd_kit_on(SECONDARY_PANEL);
}

static int lcd_kit_secondary_off(void)
{
	return lcd_kit_off(SECONDARY_PANEL);
}

static int lcd_kit_secondary_set_backlight(unsigned int bl_level)
{
	return lcd_kit_set_backlight(SECONDARY_PANEL, bl_level);
}

struct dpu_panel_info *lcd_kit_get_secondary_dpu_pinfo(void)
{
	return lcd_kit_get_dpu_pinfo(SECONDARY_PANEL);
}

struct dpu_dynamic_panel_info *lcd_kit_get_secondary_dynamic_panel_info(uint32_t panel_id)
{
	return lcd_kit_get_dpu_dynamic_panel_info(SECONDARY_PANEL);
}

static int lcd_kit_secondary_get_ddic_cmds(const struct disp_effect_params* effect_params,
	struct dsi_cmds *dsi_cmds)
{
	return lcd_kit_get_ddic_cmds(SECONDARY_PANEL, effect_params, dsi_cmds);
}

static int lcd_kit_secondary_get_cmds_tx_params(const struct dkmd_cmds_info *cmds_info,
	struct mipi_dsi_tx_params *params)
{
	return lcd_kit_get_cmds_tx_params(SECONDARY_PANEL, cmds_info, params);
}

static int lcd_kit_secondary_get_dual_cmds_tx_params(const struct dkmd_cmds_info *cmds_info,
	struct mipi_dsi_tx_params *params0, struct mipi_dsi_tx_params *params1)
{
	return lcd_kit_get_dual_cmds_tx_params(SECONDARY_PANEL, cmds_info, params0, params1);
}

static int lcd_kit_secondary_dump_info(void)
{
	check_panel_on_state(SECONDARY_PANEL);
	return LCD_KIT_OK;
}

static int lcd_kit_set_secondary_fps_scence(unsigned int current_fps)
{
	return lcd_kit_set_fps_scence(SECONDARY_PANEL, current_fps);
}

static int lcd_kit_secondary_trace_screen_bl(unsigned int bl_level)
{
	return lcd_kit_trace_screen_bl(SECONDARY_PANEL, bl_level);
}

static int lcd_kit_second_updt_fps(unsigned int fps)
{
	return lcd_kit_updt_fps(SECONDARY_PANEL, fps);
}

static int lcd_kit_secondary_esd_check(void)
{
	return lcd_kit_esd_check(SECONDARY_PANEL);
}

static int lcd_kit_secondary_set_display_region(struct dkmd_rect *dirty)
{
	return lcd_kit_set_display_region(SECONDARY_PANEL, dirty);
}

static int lcd_kit_secondary_fake_power_off(void)
{
	return lcd_kit_fake_power_off(SECONDARY_PANEL);
}

static int lcd_kit_secondary_set_ppc_config_id(unsigned int ppc_config_id)
{
	return lcd_kit_set_ppc_config_id(SECONDARY_PANEL, ppc_config_id);
}

static int lcd_kit_secondary_notify_sfr_info(const struct sfr_info *sfr_info)
{
	return lcd_kit_safe_frm_rate_ctrl(SECONDARY_PANEL, sfr_info);
}

static int lcd_kit_set_secondary_dsc_econfig(uint8_t dsc_enable)
{
	return lcd_kit_set_dsc_econfig(SECONDARY_PANEL, dsc_enable);
}

static int lcd_kit_secondary_ddic_exception_mode_cfg(uint32_t mode)
{
	return lcd_kit_ddic_exception_mode_cfg(SECONDARY_PANEL, mode);
}

static int lcd_kit_secondary_skip_tp_notify(bool skip_tp)
{
	return lcd_kit_skip_tp_notify(SECONDARY_PANEL, skip_tp);
}

static struct dpu_panel_ops secondary_dpu_panel_data = {
	.set_fastboot = lcd_kit_secondary_set_fastboot,
	.on = lcd_kit_secondary_on,
	.off = lcd_kit_secondary_off,
	.esd_handle = lcd_kit_secondary_esd_check,
	.set_backlight = lcd_kit_secondary_set_backlight,
	.get_panel_info = lcd_kit_get_secondary_dpu_pinfo,
	.dump_exception_info = lcd_kit_secondary_dump_info,
	.set_current_fps = lcd_kit_set_secondary_fps_scence,
	.get_ddic_cmds = lcd_kit_secondary_get_ddic_cmds,
	.get_cmds_tx_params = lcd_kit_secondary_get_cmds_tx_params,
	.get_dual_cmds_tx_params = lcd_kit_secondary_get_dual_cmds_tx_params,
	.trace_screen_bl = lcd_kit_secondary_trace_screen_bl,
	.update_lcd_fps = lcd_kit_second_updt_fps,
	.set_display_region = lcd_kit_secondary_set_display_region,
	.fake_power_off = lcd_kit_secondary_fake_power_off,
	.set_ppc_config_id = lcd_kit_secondary_set_ppc_config_id,
	.notify_sfr_info = lcd_kit_secondary_notify_sfr_info,
	.get_dynamic_panel_info = lcd_kit_get_secondary_dynamic_panel_info,
	.set_dsc_config = lcd_kit_set_secondary_dsc_econfig,
	.ddic_exception_mode_cfg = lcd_kit_secondary_ddic_exception_mode_cfg,
	.skip_tp_notify = lcd_kit_secondary_skip_tp_notify,
};

static int lcd_kit_secondary_probe(struct platform_device *pdev)
{
	struct device_node *np = NULL;
	int ret;

	LCD_KIT_INFO("secondary panel porbe+");
	np = pdev->dev.of_node;
	if (!np) {
		LCD_KIT_ERR("not find device node\n");
		return LCD_KIT_FAIL;
	}

#ifdef LCD_KIT_DEBUG_ENABLE
	if (dpd_init(pdev)) {
		LCD_KIT_INFO("sysfs is not ready!\n");
		return -EPROBE_DEFER;
	}
#endif

	ret = lcd_kit_common_probe(SECONDARY_PANEL, np);
	if (ret) {
		LCD_KIT_ERR("secondary panel common probe fail\n");
		return LCD_KIT_FAIL;
	}
#ifdef LCD_KIT_DEBUG_ENABLE
	if (is_dpd_mode())
		dpd_regu_init(pdev);
	else
		lcd_kit_power_init(SECONDARY_PANEL, pdev);
#else
	lcd_kit_power_init(SECONDARY_PANEL, pdev);
#endif
	/* register to dkmd */
	ret = register_panel(&secondary_dpu_panel_data, SECONDARY_PANEL);
	if (ret) {
		LCD_KIT_ERR("dpu_add_driver failed!\n");
		goto err_device_put;
	}
#if (defined(CONFIG_HUAWEI_DSM) || defined(CONFIG_HUAWEI_OHOS_DSM))
	lcd_dclient = dkmd_get_dmd_client();
	lcd_kit_secondary_probe_dmd_report();
#endif
	lcd_kit_get_el_ctrl_info_ver_two(SECONDARY_PANEL);
	lcd_kit_response_pmic_irq(SECONDARY_PANEL);
	lcd_kit_register_power_off_optimize();
	lcd_kit_completion_init(SECONDARY_PANEL);
	// lcd panel version
	lcd_kit_panel_version_init(SECONDARY_PANEL);
	LCD_KIT_INFO("secondary panel porbe-");
	return LCD_KIT_OK;
err_device_put:
	return -EPROBE_DEFER;
}

// primary panel probe match table
static struct of_device_id lcd_kit_primary_match_table[] = {
	{
		.compatible = "auo_otm1901a_5p2_1080p_video",
		.data = NULL,
	},
	{},
};

// secondary panel probe match table
static struct of_device_id lcd_kit_secondary_match_table[] = {
	{
		.compatible = "lcd_ext_panel_default",
		.data = NULL,
	},
	{},
};

// panel platform driver
static struct platform_driver lcd_kit_panel_driver[MAX_PANEL] = {
	{
		.probe = lcd_kit_primary_probe,
		.remove = NULL,
		.suspend = NULL,
		.resume = NULL,
		.shutdown = NULL,
		.driver = {
			.name = "lcd_kit_mipi_panel",
			.of_match_table = lcd_kit_primary_match_table,
		},
	},
	{
		.probe = lcd_kit_secondary_probe,
		.remove = NULL,
		.suspend = NULL,
		.resume = NULL,
		.shutdown = NULL,
		.driver = {
			.name = "lcd_kit_sec_mipi_panel",
			.of_match_table = lcd_kit_secondary_match_table,
		},
	},
};

static int lcd_kit_get_comp(int panel_id, struct device_node *np, int index)
{
	unsigned int pinid = 0;
	int i;
	unsigned int gpio_value;
	int panel_length = sizeof(g_lcd_kit_panels) / sizeof(g_lcd_kit_panels[0]);
	struct lcd_kit_array_data lcd_ids = {0};
	unsigned int poweric_det_support = 0;
	unsigned int poweric_gpio = POWERIC_DEFAULT_GPIO;

	if (index == 0)
		lcd_kit_parse_array_data(np, "gpio_id", &lcd_ids);
	else
		lcd_kit_parse_array_data(np, "sec_gpio_id", &lcd_ids);

	for (i = 0; i < lcd_ids.cnt; i++) {
		gpio_value = lcd_kit_get_pinid(lcd_ids.buf[i]);
		pinid |= (gpio_value << (2 * i));
	}
	kfree(lcd_ids.buf);
	if (index == 0) {
		/* ext idpin config */
		lcd_kit_parse_u32(np, "poweric_det_support", &poweric_det_support, 0);
		lcd_kit_parse_u32(np, "poweric_gpio_id", &poweric_gpio, 0);
	} else {
		/* ext idpin config */
		lcd_kit_parse_u32(np, "sec_poweric_det_support", &poweric_det_support, 0);
		lcd_kit_parse_u32(np, "sec_poweric_gpio_id", &poweric_gpio, 0);
	}
	pinid |= lcd_kit_get_ext_pinid(np, poweric_det_support, poweric_gpio);

	/* sec panel panid */
	pinid |= (0x40 * (unsigned int)index);

	LCD_KIT_INFO("pinid = %u\n", pinid);
	for (i = 0; i < panel_length; i++) {
		if (g_lcd_kit_panels[i].product_id == disp_info->product_id &&
			g_lcd_kit_panels[i].pin_id == pinid) {
			LCD_KIT_INFO("lcd has matched panel table, panel_name is %s\n",
				g_lcd_kit_panels[i].panel_name);
			disp_info->compatible = (char *)g_lcd_kit_panels[i].panel_name;
			return LCD_KIT_OK;
		}
	}

	return LCD_KIT_FAIL;
}

static int lcd_kit_parse_comp(int panel_id, struct device_node *np, int index)
{
	unsigned int uefi_bypass = 0;
	int ret;

	lcd_kit_parse_u32(np, "uefi_bypass", &uefi_bypass, 0);
	if (uefi_bypass) {
		ret = lcd_kit_get_comp(panel_id, np, index);
		if (ret) {
			LCD_KIT_ERR("get lcd compatible error\n");
			return ret;
		}
	} else {
		disp_info->compatible = (char *)of_get_property(np,
			g_disp_compatibles[index], NULL);
		if (!disp_info->compatible) {
			LCD_KIT_ERR("can not get lcd kit compatible\n");
			return LCD_KIT_FAIL;
		}
	}

	return LCD_KIT_OK;
}

static void lcd_kit_parse_panel_software_id_number(struct device_node *np)
{
	int panel_id = PRIMARY_PANEL;
	lcd_kit_parse_u32(np, "panel_software_id_number", &common_info->panel_software_id.number, 0);
	LCD_KIT_INFO("panel_software_id_number = %u", common_info->panel_software_id.number);
}

/* read el ctrl plus elvss from dts */
static void lcd_kit_parse_el_ctrl_plus_elvss(struct device_node *np)
{
	int panel_id = PRIMARY_PANEL;
	lcd_kit_parse_u32(np, "el_ctrl_elvss_primary", &common_info->el_ctrl_info.el_ctrl_plus_elvss, 0);
	LCD_KIT_INFO("el_ctrl_plus_elvss_primary = %x\n", common_info->el_ctrl_info.el_ctrl_plus_elvss);

	if (lcd_kit_get_display_type() == NORMAL_TYPE) {
		LCD_KIT_INFO("no need get secondary panel elvss info:%d!\n", lcd_kit_get_display_type());
		return;
	}
	panel_id = SECONDARY_PANEL;
	lcd_kit_parse_u32(np, "el_ctrl_elvss_secondary", &common_info->el_ctrl_info.el_ctrl_plus_elvss, 0);
	LCD_KIT_INFO("el_ctrl_plus_elvss_secondary = %x\n", common_info->el_ctrl_info.el_ctrl_plus_elvss);
}

static void lcd_kit_parse_ppc_software_id_number(struct device_node *np)
{
	int panel_id = PRIMARY_PANEL;
	lcd_kit_parse_u32(np, "ppc_software_id_number", &common_info->ppc_para.soft_id_number, 0);
	LCD_KIT_INFO("ppc_software_id_number = %d\n", common_info->ppc_para.soft_id_number);
}

static void lcd_kit_parse_public_info(struct device_node *np)
{
	/* parse product_type */
	lcd_kit_parse_u32(np, "product_type", &PUBLIC_INFO->product_type, 0);
	LCD_KIT_INFO("product_type = %d", PUBLIC_INFO->product_type);
	lcd_kit_parse_u32(np, "dvdd_avs_volt", &PUBLIC_INFO->dvdd_avs_volt, 0);
	LCD_KIT_INFO("dvdd_avs_volt = %d", PUBLIC_INFO->dvdd_avs_volt);
	lcd_kit_parse_u32(np, "sub_dvdd_avs_volt", &PUBLIC_INFO->sub_dvdd_avs_volt, 0);
	LCD_KIT_INFO("sub_dvdd_avs_volt = %d", PUBLIC_INFO->sub_dvdd_avs_volt);
	lcd_kit_parse_u32(np, "dvdd_avs_vmin", &PUBLIC_INFO->dvdd_avs_vmin, 0);
	LCD_KIT_INFO("dvdd_avs_vmin = %d", PUBLIC_INFO->dvdd_avs_vmin);
	lcd_kit_parse_u32(np, "sub_dvdd_avs_vmin", &PUBLIC_INFO->sub_dvdd_avs_vmin, 0);
	LCD_KIT_INFO("sub_dvdd_avs_vmin = %d", PUBLIC_INFO->sub_dvdd_avs_vmin);
	lcd_kit_parse_string(np, "dvdd_avs_dmd", (const char **)&PUBLIC_INFO->dvdd_avs_dmd);
	if (PUBLIC_INFO->dvdd_avs_dmd)
		LCD_KIT_INFO("dvdd_avs_dmd = %s", PUBLIC_INFO->dvdd_avs_dmd);
	lcd_kit_parse_string(np, "sub_dvdd_avs_dmd", (const char **)&PUBLIC_INFO->sub_dvdd_avs_dmd);
	if (PUBLIC_INFO->sub_dvdd_avs_dmd)
		LCD_KIT_INFO("sub_dvdd_avs_dmd = %s", PUBLIC_INFO->sub_dvdd_avs_dmd);
	lcd_kit_parse_u32(np, "need_ddic_c_calib", &PUBLIC_INFO->need_ddic_c_calib, 0);
	LCD_KIT_INFO("need_ddic_c_calib = %u", PUBLIC_INFO->need_ddic_c_calib);
	/* parse panel version */
	lcd_kit_parse_u32(np, "primary_panel_version",
		&PUBLIC_INFO->primary_panel_version, 0);
	lcd_kit_parse_u32(np, "secondary_panel_version",
		&PUBLIC_INFO->secondary_panel_version, 0);
	/* parse panel lcd_power_seq */
	lcd_kit_parse_u32(np, "lcd_power_seq",
		&PUBLIC_INFO->lcd_power_seq, LCD_KIT_POWER_OFF);
	LCD_KIT_INFO("lcd_power_seq = %d", PUBLIC_INFO->lcd_power_seq);
	lcd_kit_parse_u32(np, "sec_lcd_power_seq",
		&PUBLIC_INFO->sec_lcd_power_seq, LCD_KIT_POWER_OFF);
	LCD_KIT_INFO("sec_lcd_power_seq = %d", PUBLIC_INFO->sec_lcd_power_seq);
	/* panel software id */
	lcd_kit_parse_panel_software_id_number(np);
	/* el ctrl plus elvss */
	lcd_kit_parse_el_ctrl_plus_elvss(np);
	/* ppc software id */
	lcd_kit_parse_ppc_software_id_number(np);
	lcd_kit_parse_u32(np, "el_ctrl_level",
		&PUBLIC_INFO->calibrate_level, 256);
	LCD_KIT_DEBUG("el_ctrl_level = %d", PUBLIC_INFO->calibrate_level);
	lcd_kit_parse_u32(np, "fake_compatible_panel",
		&PUBLIC_INFO->fake_compatible_panel, 1);
	LCD_KIT_INFO("fake_compatible_panel = %d", PUBLIC_INFO->fake_compatible_panel);
	lcd_kit_parse_string(np, "dieid_info", (const char **)&PUBLIC_INFO->dieid_info);
	lcd_kit_parse_string(np, "sec_dieid_info", (const char **)&PUBLIC_INFO->sec_dieid_info);
}

static int __init lcd_kit_init(void)
{
	int ret = LCD_KIT_OK;
	int init_length, len, i;
	int panel_id;
	struct device_node *np = NULL;

	if (!lcd_kit_support()) {
		LCD_KIT_INFO("not lcd_kit driver and return\n");
		return ret;
	}

	np = of_find_compatible_node(NULL, NULL, DTS_COMP_LCD_KIT_PANEL_TYPE);
	if (!np) {
		LCD_KIT_ERR("NOT FOUND device node %s!\n", DTS_COMP_LCD_KIT_PANEL_TYPE);
		return LCD_KIT_FAIL;
	}
#ifdef LCD_KIT_DEBUG_ENABLE
	of_property_read_u32(np, "lcd_dpd", &g_dpd_mode);
	LCD_KIT_INFO("dpd_mode is %d", g_dpd_mode);
#endif

	lcd_kit_parse_public_info(np);
	init_length = lcd_kit_get_init_display_length();

	for (i = 0; i < init_length; i++) {
		panel_id = g_init_order[i];

		lcd_kit_parse_u32(np, "board_version", &disp_info->board_version, 0);
		LCD_KIT_INFO("board_version = %u", disp_info->board_version);
		lcd_kit_parse_u32(np, "product_id", &disp_info->product_id, 0);
		LCD_KIT_INFO("product_id = %u", disp_info->product_id);

		ret = lcd_kit_parse_comp(panel_id, np, i);
		if (ret) {
			LCD_KIT_ERR("get compatible is null and return\n");
			return ret;
		}

		len = strlen(disp_info->compatible);

		memset((char *)lcd_kit_panel_driver[i].driver.of_match_table->compatible, 0, LCD_KIT_PANEL_COMP_LENGTH);

		strncpy((char *)lcd_kit_panel_driver[i].driver.of_match_table->compatible,
				disp_info->compatible, len > (LCD_KIT_PANEL_COMP_LENGTH - 1) ?
				(LCD_KIT_PANEL_COMP_LENGTH - 1) : len);

		/* register driver */
		ret = platform_driver_register(&lcd_kit_panel_driver[i]);
		if (ret) {
			LCD_KIT_ERR("driver register failed, error = %d!\n", ret);
			return ret;
		}
	}

	return LCD_KIT_OK;
}

module_init(lcd_kit_init);
