/*
 * lcd_kit_get_safe_frm_rate_by_bl.c
 *
 * display engine dkmd safe frm rate function in lcd
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
 *
 */

#include "lcd_kit_safe_frm_rate.h"

#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/mutex.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/thermal.h>
#include <linux/ktime.h>
#include <linux/delay.h>

#include "display_engine_kernel.h"
#include "lcd_kit_adapt.h"
#include "lcd_kit_parse.h"
#include "lcd_kit_utils.h"
#include "lcd_kit_disp.h"

#define FPS_60HZ 60
#define FPS_90HZ 90
#define FPS_120HZ 120
#define FPS_144HZ 144

/* tp safe frm rate tag with touch detected or release */
#define SAFE_FRAMERATE_TP_TOUCH_DETECTED_TAG 1
#define SAFE_FRAMERATE_TP_TOUCH_RELEASE_TAG 0
/* tp safe frm rate with touch detected or release */
#define SAFE_FRAMERATE_TP_TOUCH_DETECTED_HZ 10
#define SAFE_FRAMERATE_TP_TOUCH_RELEASE_HZ 1
#define SAFE_FRAMERATE_ACROSS_BRIGHTNESS_HZ 10
#define SAFE_FRAMERATE_ACROSS_BRIGHTNESS_END_HZ 1

/* tp safe frm rate touch release time */
#define SAFE_FRAMERATE_TP_TOUCH_RELEASE_DELAY_MS 500
/* default sfr value is 1 */
#define SFR_DEFAULT 1
struct safe_frm_rate_work {
	enum display_engine_panel_id handle_panel_id;
	struct kthread_work handle_work_detected;
	struct kthread_work handle_work_release;
	struct delayed_work tp_release_delay_work;
	struct kthread_work handle_work_brightness_change;
	struct delayed_work handle_work_brightness_change_delay_work;
};

struct safe_frm_rate_work_info {
	const char *name;
	struct kthread_worker handle_worker;
	struct task_struct *handle_thread;
	struct safe_frm_rate_work handle_work[DISPLAY_ENGINE_PANEL_COUNT];
};

static struct safe_frm_rate_work_info g_sfr_work = {
	.name = "safe_frm_rate_work_info",
	.handle_thread = NULL,
	.handle_work[DISPLAY_ENGINE_PANEL_INNER].handle_panel_id = DISPLAY_ENGINE_PANEL_INNER,
	.handle_work[DISPLAY_ENGINE_PANEL_OUTER].handle_panel_id = DISPLAY_ENGINE_PANEL_OUTER,
};

static struct timespec64 last_temperature_timestamp;
static uint64_t get_temperature_time_interval = 120;
static struct thermal_zone_device *thermal_zone = NULL;
static int cur_thermal = 0;
static bool is_thermal_enable = false;
static bool safe_frm_rate_init = false;
static struct mutex sfr_mutex;
/* tp safe frm rate touch detected handler */
static void tp_touch_detected_work_handler(struct kthread_work *work);
/* tp safe frm rate touch release handler */
static void safe_frm_rate_tp_release_work_handler(struct kthread_work *work);
/* tp safe frm rate touch release delay handler */
static void safe_frm_rate_tp_release_delay_work_handler(struct work_struct *work);
static void acorss_brightness_node_delay_work_handler(struct work_struct *work);
static void across_brightness_node_work_handler(struct kthread_work *work);

static bool low_brightness_flag = false;
static bool is_wq_work_init[DISPLAY_ENGINE_PANEL_COUNT] = {0};
static bool is_workqueue_init = false;
static bool current_in_partial_mode = false; /* if in partial mode now */
static unsigned int current_brightness_sfr_full[DISPLAY_ENGINE_PANEL_COUNT] = {0}; /* full refresh mode sfr */
static unsigned int current_brightness_sfr_partial = 1; /* partial refresh mode sfr */
static unsigned int current_tp_sfr[DISPLAY_ENGINE_PANEL_COUNT] = {0};
static unsigned int current_ppu_sfr = 1; /* partial refresh algo sfr from de-hal */
static unsigned int current_across_brightss_sfr = 1; /* across brightness sfr */
/* adjust sfr by active frame rate */
static unsigned int current_afr;
static bool afr_without_sfr_event = false; /* recieve afr event without sfr */

static unsigned int last_final_sfr[DISPLAY_ENGINE_PANEL_COUNT] = {0}; /* last sfr sent to dkmd */
static bool force_send_sfr[DISPLAY_ENGINE_PANEL_COUNT] = {false}; /* need to send sfr even sfr doesn't change */
static unsigned int last_brightness = 0;
static unsigned int brightness_node = 0;
static unsigned int de_hal_sfr[DISPLAY_ENGINE_PANEL_COUNT] = {0}; /* full refresh mode sfr from de-hal */

void display_engine_sfr_update_with_brightness(int panel_id, uint32_t brightness_dbv)
{
	if (!disp_info->safe_frm_decision.across_brightness_support)
		return;
	if (((last_brightness <= brightness_node && brightness_dbv > brightness_node) ||
		(last_brightness > brightness_node && brightness_dbv <= brightness_node)) &&
		lcd_kit_get_panel_on_state(panel_id)) {
		kthread_queue_work(&(g_sfr_work.handle_worker),
			&(g_sfr_work.handle_work[panel_id].handle_work_brightness_change));
		LCD_KIT_INFO("brightness_dbv:%u last_brightness:%u update across brightness sfr\n",
			brightness_dbv, last_brightness);
	}
	LCD_KIT_DEBUG("brightness_dbv:%u, last_brightness:%u brightness_node:%u\n",
		brightness_dbv, last_brightness, brightness_node);
	last_brightness = brightness_dbv;
}

void display_engine_sfr_enter_ap_event(int panel_id, uint32_t value)
{
	if (!disp_info->safe_frm_decision.across_brightness_support)
		return;
	kthread_queue_work(&(g_sfr_work.handle_worker),
		&(g_sfr_work.handle_work[panel_id].handle_work_brightness_change));
	LCD_KIT_INFO("sfr enter ap event value:%u true\n", value);
}

/* get real safe frm rate by current fps */
static unsigned int lcd_kit_get_real_safe_frm_by_fps(int panel_id, unsigned int current_fps)
{
	if (!low_brightness_flag) {
		switch (current_fps) {
		case FPS_144HZ:
			return disp_info->safe_frm_decision.fps_144_safe_frm;
		case FPS_120HZ:
			return disp_info->safe_frm_decision.fps_120_safe_frm;
		case FPS_90HZ:
			return disp_info->safe_frm_decision.fps_90_safe_frm;
		default:
			return current_fps;
		}
	}

	return current_fps;
}

/* update safe frm rate by low brightness status and current fps */
static unsigned int lcd_kit_upt_safe_frm_rate_by_fps(int panel_id, unsigned int safe_frm_rate, unsigned int current_fps)
{
	if (!afr_without_sfr_event) {
		/* notify low brightness status change */
		if (safe_frm_rate == FPS_120HZ) {
			low_brightness_flag = true;
			LCD_KIT_INFO("notify low brightness scene enter\n");
		} else {
			low_brightness_flag = false;
			LCD_KIT_INFO("notify low brightness scene exit\n");
		}
	}

	afr_without_sfr_event = false;
	return lcd_kit_get_real_safe_frm_by_fps(panel_id, current_fps);
}

/* reset safe frame rate when fps changed */
static unsigned int lcd_kit_reset_sfr_by_fps(int panel_id, unsigned int current_sfr)
{
	if ((current_afr != FPS_120HZ && current_afr != FPS_60HZ) || de_hal_sfr[panel_id] == current_sfr)
		return current_sfr;
	LCD_KIT_INFO("resend sfr=%u\n", de_hal_sfr[panel_id]);
	return de_hal_sfr[panel_id] >= SFR_DEFAULT ? de_hal_sfr[panel_id] : current_sfr;
}

static bool is_panel_id_invalid(int panel_id)
{
	if (panel_id <= DISPLAY_ENGINE_PANEL_INVALID ||	panel_id >= DISPLAY_ENGINE_PANEL_COUNT) {
		LCD_KIT_ERR("panel id error %d\n", panel_id);
		return true;
	}
	return false;
}

/* get final safe frame rate */
static unsigned int get_final_safe_frm_rate(int panel_id)
{
	unsigned int final_sfr = 1;
	unsigned int brightness_sfr = 1;

	/* get max sfr by tp and brightness */
	if (is_panel_id_invalid(panel_id))
		return final_sfr;
	brightness_sfr = (current_in_partial_mode) ? current_brightness_sfr_partial : current_brightness_sfr_full[panel_id];
	if (disp_info->safe_frm_decision.tp_support)
		final_sfr = (brightness_sfr > current_tp_sfr[panel_id]) ? brightness_sfr : current_tp_sfr[panel_id];
	else
		final_sfr = brightness_sfr;
	/* get max sfr by across brightness node scene */
	if (disp_info->safe_frm_decision.across_brightness_support)
		final_sfr = (current_across_brightss_sfr > final_sfr) ? current_across_brightss_sfr : final_sfr;
	/* if in partial mode, take ppu sfr into account */
	if (current_in_partial_mode && current_ppu_sfr > final_sfr)
		final_sfr = current_ppu_sfr;

	if (disp_info->safe_frm_decision.fps_support == 1) /* 1 means pad need send sfr when fps switch */
		final_sfr = lcd_kit_upt_safe_frm_rate_by_fps(panel_id, brightness_sfr, current_afr);
	else if (disp_info->safe_frm_decision.fps_support == 3) /* 3 means reset sfr when fps switch */
		final_sfr = lcd_kit_reset_sfr_by_fps(panel_id, final_sfr);

	LCD_KIT_INFO("support[tp:%u, fps:%u, acrbri:%u], in_partial[%d], current[%u %u %u %u %u], afr[%u], final[%u]\n",
		disp_info->safe_frm_decision.tp_support, disp_info->safe_frm_decision.fps_support,
		disp_info->safe_frm_decision.across_brightness_support, current_in_partial_mode,
		current_brightness_sfr_full[panel_id], current_brightness_sfr_partial, current_tp_sfr[panel_id],
		current_ppu_sfr, current_across_brightss_sfr, current_afr, final_sfr);
	return final_sfr;
}

/* update safe frm rate by tp */
static void lcd_kit_upt_safe_frm_rate_by_tp(int panel_id, unsigned int safe_frm_rate)
{
	if (is_panel_id_invalid(panel_id))
		return;
	current_tp_sfr[panel_id] = safe_frm_rate;
	LCD_KIT_DEBUG("current_tp_sfr %u\n", current_tp_sfr);
}

/* update safe frm rate by brightness */
static void lcd_kit_upt_safe_frm_rate_by_brightness(
	int panel_id, unsigned int full_mode_sfr, unsigned int partial_mode_sfr)
{
	if (is_panel_id_invalid(panel_id))
		return;
	if (full_mode_sfr >= SFR_DEFAULT)
		current_brightness_sfr_full[panel_id] = full_mode_sfr;
	current_brightness_sfr_partial = partial_mode_sfr;
	LCD_KIT_DEBUG("current_brightness_sfr_full %u, current_brightness_sfr_partial %u\n",
		current_brightness_sfr_full[panel_id], current_brightness_sfr_partial);
}

/* update safe frm rate by ppu algo from DE-HAL */
static void lcd_kit_upt_safe_frm_rate_by_ppu(unsigned int safe_frm_rate)
{
	current_ppu_sfr = safe_frm_rate;
}

/* update safe frm rate by across bright */
static void lcd_kit_upt_safe_frm_rate_by_across_brightss(int panel_id, unsigned int safe_frm_rate)
{
	if (is_panel_id_invalid(panel_id))
		return;
	current_across_brightss_sfr = safe_frm_rate;
	LCD_KIT_DEBUG("current_across_brightss_sfr %u\n", current_across_brightss_sfr);
}

/* update safe frm rate info by dkmd */
static void lcd_kit_upt_current_sfr_info(int panel_id, const struct sfr_info *info)
{
	LCD_KIT_INFO("get info_type[%d] from dkmd", info->info_type);
	if (info->info_type & INFO_TYPE_PPU_ENTER) {
		force_send_sfr[panel_id] = true;
		current_in_partial_mode = true;
	}
	if (info->info_type & INFO_TYPE_PPU_EXIT) {
		force_send_sfr[panel_id] = true;
		current_in_partial_mode = false;
		current_ppu_sfr = 1;
	}
	// use for pad
	if (info->info_type & INFO_TYPE_AFR_SEND) {
		current_afr = info->afr;
		if ((info->info_type & INFO_TYPE_SFR_SEND) == 0)
			afr_without_sfr_event = true;
	}
	if (info->info_type & INFO_TYPE_SFR_SEND)
		lcd_kit_upt_safe_frm_rate_by_brightness(panel_id, info->full_mode_sfr, info->ppu_mode_sfr);
	if (info->info_type & INFO_TYPE_SFR_RESEND)
		force_send_sfr[panel_id] = true;
	if (info->info_type == INFO_TYPE_SFR_SEND)
		de_hal_sfr[panel_id] = info->full_mode_sfr;
}

/* send final safe frm rate */
static int lcd_kit_send_safe_frm_rate(int panel_id, unsigned int safe_frm_rate)
{
	int ret;
	unsigned int dsi0_index = 0;
	unsigned int connector_id;

	if (is_panel_id_invalid(panel_id)) {
		LCD_KIT_INFO("panel %d is power off\n", panel_id);
		return LCD_KIT_FAIL;
	}
	connector_id = DPU_PINFO->connector_id;
	ret = lcd_kit_get_dsi_index(&dsi0_index, connector_id);
	if (ret) {
		LCD_KIT_ERR("get dsi0 index error\n");
		return LCD_KIT_FAIL;
	}
	dkmd_dfr_send_safe_frm_rate(dsi0_index, DPU_PINFO->type, safe_frm_rate);
	LCD_KIT_INFO("lcd kit send final safe frm rate: %u panel id:%d\n",
		safe_frm_rate, panel_id);
	return LCD_KIT_OK;
}

/* get final sfr and send to dkmd */
static int lcd_kit_send_final_safe_frm_rate(int panel_id)
{
	int ret;
	unsigned int final_safe_frm_rate;

	final_safe_frm_rate = get_final_safe_frm_rate(panel_id);
	if (!final_safe_frm_rate) {
		LCD_KIT_ERR("final_safe_frm_rate[%d] not valid\n", final_safe_frm_rate);
		return LCD_KIT_FAIL;
	}

	if (!lcd_kit_get_panel_on_state(panel_id)) {
		LCD_KIT_DEBUG("panel %d is power off\n", panel_id);
		return ret;
	}

	LCD_KIT_INFO("final sfr:%u, last sfr:%u, force send sfr:%d, panel id:%d",
		final_safe_frm_rate, last_final_sfr[panel_id], force_send_sfr[panel_id], panel_id);
	if (final_safe_frm_rate == last_final_sfr[panel_id] && !force_send_sfr[panel_id]) {
		LCD_KIT_DEBUG("sfr[%u] doesn't change, no need to send", final_safe_frm_rate);
		return LCD_KIT_OK;
	}
	
	ret = lcd_kit_send_safe_frm_rate(panel_id, final_safe_frm_rate);
	if (ret) {
		LCD_KIT_ERR("send safe frm rate fail\n");
		return LCD_KIT_FAIL;
	}
	LCD_KIT_INFO("send safe frm rate success\n");
	force_send_sfr[panel_id] = false;
	last_final_sfr[panel_id] = final_safe_frm_rate;
	return ret;
}

/* de-hal update birghtness sfr and trigger send */
int lcd_kit_brightness_trigger_send_sfr(int panel_id, unsigned int full_mode_sfr, unsigned int partial_mode_sfr)
{
	if (!safe_frm_rate_init)
		return LCD_KIT_FAIL;
	int ret;
	if (is_panel_id_invalid(panel_id))
		return LCD_KIT_FAIL;

	mutex_lock(&sfr_mutex);
	de_hal_sfr[panel_id] = full_mode_sfr;
	lcd_kit_upt_safe_frm_rate_by_brightness(panel_id, full_mode_sfr, partial_mode_sfr);
	ret = lcd_kit_send_final_safe_frm_rate(panel_id);
	mutex_unlock(&sfr_mutex);
	
	return ret;
}

/* de-hal ppu algorithm update sfr and trigger send */
int lcd_kit_ppu_trigger_send_sfr(int panel_id, unsigned int ppu_sfr)
{
	if (!safe_frm_rate_init)
		return LCD_KIT_FAIL;
	int ret;

	mutex_lock(&sfr_mutex);
	lcd_kit_upt_safe_frm_rate_by_ppu(ppu_sfr);
	ret = lcd_kit_send_final_safe_frm_rate(panel_id);
	mutex_unlock(&sfr_mutex);
	
	return ret;
}

/* safe frame rate second decision, time delay is not allowed in this function */
int lcd_kit_safe_frm_rate_ctrl(int panel_id, const struct sfr_info *sfr_info)
{
	int ret;

	if (!sfr_info) {
		LCD_KIT_ERR("sfr_info is NULL!\n");
		return LCD_KIT_FAIL;
	}
	if (!safe_frm_rate_init)
		return LCD_KIT_FAIL;
	if (is_panel_id_invalid(panel_id))
		return LCD_KIT_FAIL;

	mutex_lock(&sfr_mutex);
	lcd_kit_upt_current_sfr_info(panel_id, sfr_info);
	ret = lcd_kit_send_final_safe_frm_rate(panel_id);
	mutex_unlock(&sfr_mutex);
	
	return ret;
}

static void lcd_kit_safe_frm_init_tp_parameter(int panel_id, struct device_node *np)
{
	lcd_kit_parse_u32(np, "lcd-kit,safe-frm-decision-by-tp-support", &disp_info->safe_frm_decision.tp_support, 0);
	if (disp_info->safe_frm_decision.tp_support) {
		lcd_kit_parse_u32(np, "lcd-kit,tp-detected-safe-frm-rate",
			&disp_info->safe_frm_decision.tp_detected_safe_frm, 0);
		lcd_kit_parse_u32(np, "lcd-kit,tp-release-safe-frm-rate-delay-time",
			&disp_info->safe_frm_decision.tp_release_safe_frm_delay_time, 0);
		LCD_KIT_INFO("tp support:%d, detected sfr:%d, delay time:%d\n",
			disp_info->safe_frm_decision.tp_support, disp_info->safe_frm_decision.tp_detected_safe_frm,
			disp_info->safe_frm_decision.tp_release_safe_frm_delay_time);
		lcd_kit_parse_u32(np, "lcd-kit,safe-frm-decision-by-tp-temperature-support",
			&disp_info->safe_frm_decision.tp_temperature_support, 0);
		lcd_kit_parse_u32(np, "lcd-kit,tp-normal-temperature-safe-frm",
			&disp_info->safe_frm_decision.tp_normal_temperature_safe_frm, 0);
		lcd_kit_parse_u32(np, "lcd-kit,tp-low-temperature-safe-frm",
			&disp_info->safe_frm_decision.tp_low_temperature_safe_frm, 0);
		lcd_kit_parse_u32(np, "lcd-kit,safe-frm-tp-temperature-thresholds",
			&disp_info->safe_frm_decision.safe_frm_tp_temperature_thresholds, 0);
		lcd_kit_parse_u32(np, "lcd-kit,safe-frm-tp-temperature-time-interval",
			&disp_info->safe_frm_decision.safe_frm_tp_temperature_time_interval, 0);
		if (disp_info->safe_frm_decision.safe_frm_tp_temperature_time_interval)
			get_temperature_time_interval = disp_info->safe_frm_decision.safe_frm_tp_temperature_time_interval;
		LCD_KIT_INFO("temp support:%d, th:%d, normal sfr:%d, low sfr:%d, time interval:%d, panel id:%d\n",
			disp_info->safe_frm_decision.tp_temperature_support,
			disp_info->safe_frm_decision.safe_frm_tp_temperature_thresholds,
			disp_info->safe_frm_decision.tp_normal_temperature_safe_frm,
			disp_info->safe_frm_decision.tp_low_temperature_safe_frm,
			disp_info->safe_frm_decision.safe_frm_tp_temperature_time_interval, panel_id);
	}
}

static void lcd_kit_safe_frm_init_fps_parameter(int panel_id, struct device_node *np)
{
	lcd_kit_parse_u32(np, "lcd-kit,safe-frm-decision-by-fps-support", &disp_info->safe_frm_decision.fps_support, 0);
	if (disp_info->safe_frm_decision.fps_support) {
		lcd_kit_parse_u32(np, "lcd-kit,fps-144-safe-frm", &disp_info->safe_frm_decision.fps_144_safe_frm, 0);
		lcd_kit_parse_u32(np, "lcd-kit,fps-120-safe-frm", &disp_info->safe_frm_decision.fps_120_safe_frm, 0);
		lcd_kit_parse_u32(np, "lcd-kit,fps-90-safe-frm", &disp_info->safe_frm_decision.fps_90_safe_frm, 0);
	}
}

static void lcd_kit_safe_frm_init_brightness_node_parameter(int panel_id, struct device_node *np)
{
	lcd_kit_parse_u32(np, "lcd-kit,safe-frm-across-brightness-node-support",
		&disp_info->safe_frm_decision.across_brightness_support, 0);
	if (disp_info->safe_frm_decision.across_brightness_support) {
		lcd_kit_parse_u32(np, "lcd-kit,safe-frm-across-brightness-node-frame-value",
			&disp_info->safe_frm_decision.across_brightness_frame_value, 0);
		lcd_kit_parse_u32(np, "lcd-kit,safe-frm-across-brightness-node-dbv-value",
			&disp_info->safe_frm_decision.across_brightness_dbv_value, 0);
		lcd_kit_parse_u32(np, "lcd-kit,safe-frm-across-brightness-node-delay-time",
			&disp_info->safe_frm_decision.across_brightness_delay_time, 0);
		if (disp_info->safe_frm_decision.across_brightness_dbv_value != 0)
			brightness_node = disp_info->safe_frm_decision.across_brightness_dbv_value;
		if (disp_info->safe_frm_decision.across_brightness_frame_value == 0 || 
			disp_info->safe_frm_decision.across_brightness_dbv_value == 0)
			disp_info->safe_frm_decision.across_brightness_support = 0;
		LCD_KIT_INFO("across brightness frame value:%u, dbv value:%u, delay time:%u\n",
			disp_info->safe_frm_decision.across_brightness_frame_value,
			disp_info->safe_frm_decision.across_brightness_dbv_value,
			disp_info->safe_frm_decision.across_brightness_delay_time);
	}
}

/* parse safe frm rate decsion info */
void lcd_kit_safe_frm_init(int panel_id, struct device_node *np)
{
	LCD_KIT_INFO("DISPLAY_ENGINE_SAFE_FRM_INFO parse panel xml data %d\n", panel_id);
	if (is_panel_id_invalid(panel_id))
		return;
	lcd_kit_safe_frm_init_tp_parameter(panel_id, np);
	lcd_kit_safe_frm_init_fps_parameter(panel_id, np);
	lcd_kit_safe_frm_init_brightness_node_parameter(panel_id, np);
	current_brightness_sfr_full[panel_id] = 1;
	current_tp_sfr[panel_id] = 1;
	last_final_sfr[panel_id] = 1;
	force_send_sfr[panel_id] = false;
	de_hal_sfr[panel_id] = 1;
	if (!safe_frm_rate_init) {
		mutex_init(&sfr_mutex);
		safe_frm_rate_init = true;
	}
	mutex_init(&sfr_mutex);
	safe_frm_rate_init = true;
}

static void lcd_kit_sfr_get_temper_zone(void)
{
	thermal_zone = thermal_zone_get_zone_by_name("shell_frame");
}

static int lcd_kit_sfr_get_temper_handler(int *current_thermal)
{
	int ret = LCD_KIT_OK;
	if (!current_thermal) {
		LCD_KIT_ERR("current thermal is null\n");
		return LCD_KIT_FAIL;
	}
	lcd_kit_sfr_get_temper_zone();
	if (IS_ERR_OR_NULL(thermal_zone)) {
		LCD_KIT_ERR("Failed getting thermal zone!\n");
		return LCD_KIT_FAIL;
	}
	ret = thermal_zone_get_temp(thermal_zone, current_thermal);
	if (ret) {
		LCD_KIT_ERR("get_temperature fail!!");
		return LCD_KIT_FAIL;
	}
	return ret;
}

static unsigned int update_temperature(int panel_id)
{
	ktime_get_real_ts64(&last_temperature_timestamp);
	int ret = LCD_KIT_OK;
	ret = lcd_kit_sfr_get_temper_handler(&cur_thermal);
	if (ret) {
		is_thermal_enable = false;
		LCD_KIT_ERR("get thermal is error!\n");
	} else {
		is_thermal_enable = true;
		cur_thermal = cur_thermal / 1000; /* Divide by 1000 to get the current degree Celsius */ 
	}
	LCD_KIT_INFO("start time:[%lld], current temperature :%d\n", last_temperature_timestamp.tv_sec, cur_thermal);
	return ret;
}

static bool should_update_temperature(int panel_id)
{
	struct timespec64 get_temperature_time_now;
	ktime_get_real_ts64(&get_temperature_time_now);
	uint64_t diff_t = (uint64_t)(get_temperature_time_now.tv_sec - last_temperature_timestamp.tv_sec);
	LCD_KIT_DEBUG("start time:[%lld], current time:[%lld] diff_t:[%lld] current temperature:%d, panel id:%d\n",
		last_temperature_timestamp.tv_sec, get_temperature_time_now.tv_sec, diff_t, cur_thermal, panel_id);
	return diff_t > get_temperature_time_interval;
}

static unsigned int get_tp_touch_detected_sfr_by_temperature(int panel_id)
{
	unsigned int safe_frm_rate = SAFE_FRAMERATE_TP_TOUCH_DETECTED_HZ;
	if (!disp_info->safe_frm_decision.tp_normal_temperature_safe_frm ||
		!disp_info->safe_frm_decision.tp_low_temperature_safe_frm) {
		LCD_KIT_DEBUG("not find temperature info, normal sfr: %d low sfr %d panel id:%d\n",
			disp_info->safe_frm_decision.tp_normal_temperature_safe_frm,
			disp_info->safe_frm_decision.tp_low_temperature_safe_frm,
			panel_id);
		return safe_frm_rate;
	}
	if (should_update_temperature(panel_id))
		update_temperature(panel_id);
	LCD_KIT_DEBUG("tp touch detected get current thermal: %d normal sfr: %d low sfr:%d th thermal:%d\n",
		cur_thermal, disp_info->safe_frm_decision.tp_normal_temperature_safe_frm, 
		disp_info->safe_frm_decision.tp_low_temperature_safe_frm,
		disp_info->safe_frm_decision.safe_frm_tp_temperature_thresholds);
	if (cur_thermal > disp_info->safe_frm_decision.safe_frm_tp_temperature_thresholds) {
		safe_frm_rate = disp_info->safe_frm_decision.tp_normal_temperature_safe_frm;
		LCD_KIT_DEBUG("tp touch detected use normal sfr\n");
	} else {
		safe_frm_rate = disp_info->safe_frm_decision.tp_low_temperature_safe_frm;
		LCD_KIT_DEBUG("tp touch detected use low sfr\n");
	}
	LCD_KIT_DEBUG("tp touch detected get current thermal: %d safe frm rate: %d panel id:%d\n",
		cur_thermal, safe_frm_rate, panel_id);
	return safe_frm_rate;
}

static void safe_frm_rate_init_works(int panel_id, struct safe_frm_rate_work *sfr_work)
{
	if (!sfr_work)
		return;
	if (disp_info->safe_frm_decision.tp_support) {
		kthread_init_work(&(sfr_work->handle_work_detected), tp_touch_detected_work_handler);
		kthread_init_work(&(sfr_work->handle_work_release), safe_frm_rate_tp_release_work_handler);
		INIT_DELAYED_WORK(&(sfr_work->tp_release_delay_work), safe_frm_rate_tp_release_delay_work_handler);
		LCD_KIT_DEBUG("tp sfr init work done\n");
	}
	if (disp_info->safe_frm_decision.across_brightness_support) {
		kthread_init_work(&(sfr_work->handle_work_brightness_change), across_brightness_node_work_handler);
		INIT_DELAYED_WORK(&(sfr_work->handle_work_brightness_change_delay_work), acorss_brightness_node_delay_work_handler);
		LCD_KIT_DEBUG("across brightness sfr init work done\n");
	}
	return;
}

static void safe_frm_rate_init_workqueue_thread(struct safe_frm_rate_work_info *sfr_work)
{
	LCD_KIT_INFO("workqueue thread init\n");
	if (!sfr_work)
		return;
	if (is_workqueue_init)
		return;
	kthread_init_worker(&(sfr_work->handle_worker));
	sfr_work->handle_thread =
		kthread_create(kthread_worker_fn, &(sfr_work->handle_worker), sfr_work->name);
	if (!sfr_work->handle_thread) {
		LCD_KIT_ERR("%s failed to create handle_thread!\n", sfr_work->name);
		return;
	}
	wake_up_process(sfr_work->handle_thread);
	is_workqueue_init = true;
	LCD_KIT_INFO("workqueue thread init done\n");
	return;
}

void safe_frm_rate_work_info_workqueue_init(int panel_id)
{
	LCD_KIT_INFO("workqueue init %d\n", panel_id);
	if (!safe_frm_rate_init) {
		LCD_KIT_INFO("not init\n");
		return;
	}
	if (is_panel_id_invalid(panel_id))
		return;
	if (!disp_info->safe_frm_decision.tp_support && !disp_info->safe_frm_decision.across_brightness_support)
		return;
	mutex_lock(&sfr_mutex);
	safe_frm_rate_init_workqueue_thread(&g_sfr_work);
	if (!is_wq_work_init[panel_id]) {
		safe_frm_rate_init_works(panel_id, &(g_sfr_work.handle_work[panel_id]));
		is_wq_work_init[panel_id] = true;
	}
	mutex_unlock(&sfr_mutex);
	LCD_KIT_INFO("workqueue init done %d\n", panel_id);
}

static void safe_frm_rate_tp_release_delay_work_handler(struct work_struct *work)
{
	int ret;
	int panel_id;
	unsigned int final_safe_frm_rate;

	if (!work) {
		LCD_KIT_ERR("work struct is null\n");
		return;
	}
	struct safe_frm_rate_work *sfr_work = container_of(work, struct safe_frm_rate_work, tp_release_delay_work.work);
	if (!sfr_work) {
		LCD_KIT_ERR("safe frm rate work is null\n");
		return;
	}
	panel_id = sfr_work->handle_panel_id;
	if (!lcd_kit_get_panel_on_state(panel_id)) {
		LCD_KIT_ERR("power is not on\n");
		return;
	}
	mutex_lock(&sfr_mutex);
	lcd_kit_upt_safe_frm_rate_by_tp(panel_id, SAFE_FRAMERATE_TP_TOUCH_RELEASE_HZ);
	final_safe_frm_rate = get_final_safe_frm_rate(panel_id);
	LCD_KIT_INFO("tp touch release send safe frm rate %u panel_id:%d\n", final_safe_frm_rate, panel_id);
	ret = lcd_kit_send_final_safe_frm_rate(panel_id);
	mutex_unlock(&sfr_mutex);
	
	if (ret)
		LCD_KIT_ERR("tp touch release send safe frm rate fail\n");
	else
		LCD_KIT_DEBUG("tp touch release send safe frm rate sucess\n");
}

static void safe_frm_rate_tp_release_work_handler(struct kthread_work *work)
{
	if (!work) {
		LCD_KIT_ERR("kthread work is null\n");
		return;
	}
	struct safe_frm_rate_work *sfr_work = container_of(work, struct safe_frm_rate_work, handle_work_release);
	if (!sfr_work) {
		LCD_KIT_ERR("safe frm rate work is null\n");
		return;
	}
	int panel_id = sfr_work->handle_panel_id;
	cancel_delayed_work_sync(&(sfr_work->tp_release_delay_work));
	unsigned int tp_release_safe_frm_rate_delay_time = 0;
	if (disp_info->safe_frm_decision.tp_release_safe_frm_delay_time)
		tp_release_safe_frm_rate_delay_time = disp_info->safe_frm_decision.tp_release_safe_frm_delay_time;
	schedule_delayed_work(&(sfr_work->tp_release_delay_work), msecs_to_jiffies(tp_release_safe_frm_rate_delay_time));
}

static void tp_touch_detected_work_handler(struct kthread_work *work)
{
	int ret;
	unsigned int final_safe_frm_rate;
	int panel_id;

	if (!work) {
		LCD_KIT_ERR("kthread work is null\n");
		return;
	}

	struct safe_frm_rate_work *sfr_work = container_of(work, struct safe_frm_rate_work, handle_work_detected);
	if (!sfr_work) {
		LCD_KIT_ERR("safe frm rate work is null\n");
		return;
	}
		
	panel_id = sfr_work->handle_panel_id;
	cancel_delayed_work_sync(&(sfr_work->tp_release_delay_work));

	mutex_lock(&sfr_mutex);
	unsigned int safe_frm_rate = SAFE_FRAMERATE_TP_TOUCH_DETECTED_HZ;
	if (disp_info->safe_frm_decision.tp_temperature_support)
		safe_frm_rate = get_tp_touch_detected_sfr_by_temperature(panel_id);
	else if(disp_info->safe_frm_decision.tp_detected_safe_frm)
		safe_frm_rate = disp_info->safe_frm_decision.tp_detected_safe_frm;
	lcd_kit_upt_safe_frm_rate_by_tp(panel_id, safe_frm_rate);
	final_safe_frm_rate = get_final_safe_frm_rate(panel_id);
	LCD_KIT_INFO("tp touch detected send safe frm rate %u panel id: %d\n", final_safe_frm_rate, panel_id);
	ret = lcd_kit_send_final_safe_frm_rate(panel_id);
	mutex_unlock(&sfr_mutex);
	
	if (ret)
		LCD_KIT_ERR("send safe frm rate fail\n");
	else
		LCD_KIT_DEBUG("send safe frm rate sucess\n");
}

static void acorss_brightness_node_delay_work_handler(struct work_struct *work)
{
	int ret;
	int panel_id;
	unsigned int final_safe_frm_rate;

	if (!work) {
		LCD_KIT_ERR("work struct is null\n");
		return;
	}
	struct safe_frm_rate_work *sfr_work = container_of(work, struct safe_frm_rate_work,
		handle_work_brightness_change_delay_work.work);
	if (!sfr_work) {
		LCD_KIT_ERR("safe frm rate work is null\n");
		return;
	}
	panel_id = sfr_work->handle_panel_id;
	if (!lcd_kit_get_panel_on_state(panel_id)) {
		LCD_KIT_ERR("power is not on\n");
		return;
	}
	mutex_lock(&sfr_mutex);
	lcd_kit_upt_safe_frm_rate_by_across_brightss(panel_id, SAFE_FRAMERATE_ACROSS_BRIGHTNESS_END_HZ);
	final_safe_frm_rate = get_final_safe_frm_rate(panel_id);
	mutex_unlock(&sfr_mutex);

	LCD_KIT_INFO("across brightness node send safe frm rate %u panel_id:%d\n", final_safe_frm_rate, panel_id);
	ret = lcd_kit_send_final_safe_frm_rate(panel_id);
	if (ret)
		LCD_KIT_ERR("across brightness node send safe frm rate fail\n");
	else
		LCD_KIT_DEBUG("across brightness node send safe frm rate sucess\n");
}

static void across_brightness_node_work_handler(struct kthread_work *work)
{
	int ret;
	unsigned int final_safe_frm_rate;
	int panel_id;

	if (!work) {
		LCD_KIT_ERR("kthread work is null\n");
		return;
	}

	struct safe_frm_rate_work *sfr_work = container_of(work, struct safe_frm_rate_work, handle_work_brightness_change);
	if (!sfr_work) {
		LCD_KIT_ERR("safe frm rate work is null\n");
		return;
	}
		
	panel_id = sfr_work->handle_panel_id;
	cancel_delayed_work_sync(&(sfr_work->handle_work_brightness_change_delay_work));

	mutex_lock(&sfr_mutex);
	unsigned int safe_frm_rate = SAFE_FRAMERATE_ACROSS_BRIGHTNESS_HZ;
	safe_frm_rate = disp_info->safe_frm_decision.across_brightness_frame_value;
	lcd_kit_upt_safe_frm_rate_by_across_brightss(panel_id, safe_frm_rate);
	final_safe_frm_rate = get_final_safe_frm_rate(panel_id);
	mutex_unlock(&sfr_mutex);

	LCD_KIT_INFO("across brightness node send safe frm rate %u panel id: %d\n", final_safe_frm_rate, panel_id);
	ret = lcd_kit_send_final_safe_frm_rate(panel_id);
	if (ret)
		LCD_KIT_ERR("send safe frm rate fail\n");
	else
		LCD_KIT_DEBUG("send safe frm rate sucess\n");

	schedule_delayed_work(&(sfr_work->handle_work_brightness_change_delay_work),
		msecs_to_jiffies(disp_info->safe_frm_decision.across_brightness_delay_time));
}


void lcd_kit_tp_notify_de_event(int val, int panel_id)
{
	LCD_KIT_INFO("tp touch event val:%d panel id: %d\n", val, panel_id);
	if (!disp_info->safe_frm_decision.tp_support)
		return;
	if (is_panel_id_invalid(panel_id))
		return;
	if (!is_wq_work_init[panel_id] || !safe_frm_rate_init)
		return;	
	if (val == SAFE_FRAMERATE_TP_TOUCH_DETECTED_TAG)
		kthread_queue_work(&(g_sfr_work.handle_worker), &(g_sfr_work.handle_work[panel_id].handle_work_detected));
	else if (val == SAFE_FRAMERATE_TP_TOUCH_RELEASE_TAG)
		kthread_queue_work(&(g_sfr_work.handle_worker), &(g_sfr_work.handle_work[panel_id].handle_work_release));
}
