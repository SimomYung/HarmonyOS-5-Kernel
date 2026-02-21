// SPDX-License-Identifier: GPL-2.0
/*
 * lcd_kit_hybrid_core.c
 *
 * source file for hybrid switch core
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
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
#include "lcd_kit_hybrid_core.h"

#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/device.h>
#include <linux/mutex.h>
#include <linux/spinlock.h>
#include <uapi/linux/sched/types.h>
#include <platform_include/smart/linux/iomcu_config.h>
#ifdef CONFIG_HISYSEVENT
#include <dfx/hiview_hisysevent.h>
#endif
#include "securec.h"
#include "lcd_kit_hybrid_swctrl.h"
#include "lcd_kit_hybrid_aod.h"
#include "lcd_kit_hybrid_force.h"
#include "huawei_ts_kit_hybrid_core.h"
#include "ext_sensorhub_inner_cmd.h"
#include "lcd_kit_adapt.h"
#include "lcd_kit_hybrid_control.h"
#include "lcd_kit_hybrid_msg_trans.h"
#include "lcd_kit_hybrid_force.h"
#include "lcd_kit_hybrid_trace.h"
#ifdef CONFIG_EXT_INPUTHUB_MATTING_ALSC
#include "matting_algo_task.h"
#endif
#include "dsi_clk_upt/dpu_comp_mipi_dsi_bit_clk_upt.h"

#define MAX_BUF_LEN 60
#ifdef CONFIG_HISYSEVENT
#define HISYSEVENT_BUF_SIZE 100
#endif
#define REQUEST_CONTROL_MAX_RETRY 3
#define QUEUE_WORK_MAX_RETRY 3

#define LCD_VCI_GPIO 161
#define LCD_RST_GPIO 160

#define QON_LOCK_RT_STEP_MS 5
#define QON_LOCK_RT_TIMEOUT 500
#define GES_QON_SYNC_TIMEOUT 50
#define DISPLAY_MAX_BRIGHTNESS 255

enum DISPLAY_STR_CMD {
	DISPLAY_STR_CTRL = 0x07,
	DISPLAY_STA_STATE = 0x08,
	DISPLAY_STR_GET_DDIC = 0x0A,
	DISPLAY_STR_AP_DONE = 0x0B,
	DISPLAY_STR_BRIGHTNESS = 0x0D,
	DISPLAY_SET_MIPI_CLK = 0x0E,
};

enum HYBRID_QUICK_ON_LOCK {
	QON_UNUSED = 0x00,
	QON_USED = 0x01,
};

enum HYBRID_QUICK_ON_STATUS {
	QON_NOT_READY = 0x00,
	QON_ALREADY = 0x01,
};

enum HYBRID_GESTURE_QON_STATUS {
	GES_QON_SEQ_IDLE = 0x00,
	GES_QON_SEQ_ONGOING = 0x01,
};

static DEFINE_MUTEX(ctrl_hybrid_mode_lock);
static DEFINE_SPINLOCK(hybrid_spin_lock);

struct hybrid_handler {
	enum hybrid_mode mode;
	int (*hybrid_handle)(struct display_hybrid_ctrl *display_ctrl);
};

struct hybrid_work {
	struct kthread_work work;
	enum hybrid_mode mode;
	u32 panel_id;
	int ret;
	bool finish;
};

static struct lcd_kit_common_ops hybrid_common_ops;

static struct display_hybrid_ctrl *display_ctrl;
static struct hybrid_display_msg *display_msg;
static u8 current_gesture_status;
static struct hybrid_handler handle_table[] = {
	/* switch to mcu */
	{ HYBRID_ON_SWITCH_MCU, hybrid_on_switch_mcu },
	{ HYBRID_IDLE_SWITCH_MCU, hybrid_idle_switch_mcu },
	{ HYBRID_OFF_SWITCH_MCU, hybrid_off_switch_mcu },
	/* switch to ap */
	{ HYBRID_ON_SWITCH_AP, hybrid_on_switch_ap },
	{ HYBRID_OFF_SWITCH_AP, hybrid_off_switch_ap},
	{ HYBRID_IDLE_SWITCH_AP, hybrid_idle_switch_ap},

	/* aod on */
	{ HYBRID_ALPM_ON_HIGH_LIGHT, alpm_on_high_light },
	{ HYBRID_ALPM_ON_LOW_LIGHT, alpm_on_low_light },
	{ HYBRID_ALPM_ON_MIDDLE_LIGHT, alpm_on_middle_light },
	{ HYBRID_ALPM_ON_HBM_LIGHT, alpm_on_hbm_light },
	{ HYBRID_ALPM_ON_NO_LIGHT, alpm_on_no_light },
	/* aod send frame */
	{ HYBRID_ALPM_TE_ON, alpm_send_frame },
	{ HYBRID_ALPM_TE_OFF, alpm_stop_send_frame },
	{ HYBRID_ALPM_EXIT, alpm_exit },
	{ HYBRID_ALPM_ENTER, alpm_doze },

	/* enter str mode */
	{ HYBRID_ON_SWITCH_STR, hybrid_str_switch_mcu },
	{ HYBRID_OFF_SWITCH_STR, hybrid_on_switch_ap },
};

static bool enter_recovery = false;
static bool enter_erecovery = false;

static int hybrid_get_display_power_state(void);
static int hybrid_get_display_iovcc_state(void);

struct lcd_kit_common_ops *get_hybrid_common_ops(void)
{
	return &hybrid_common_ops;
}

bool lcd_kit_get_enter_recovery_mode(void)
{
	if (enter_recovery || enter_erecovery)
		return true;

	return false;
}

static int __init early_parse_recovery_cmdline(char *p)
{
	if (p) {
		enter_recovery = (strncmp(p, "1", strlen("1")) == 0) ? true : false;
		LCD_KIT_INFO("enter_mode = %s\n", enter_recovery ? "recovery" : "normal");
	}

	return 0;
}
early_param("enter_recovery", early_parse_recovery_cmdline);

static int __init early_parse_erecovery_cmdline(char *p)
{
	if (p) {
		enter_erecovery = (strncmp(p, "1", strlen("1")) == 0) ? true : false;
		LCD_KIT_INFO("enter_mode = %s\n", enter_erecovery ? "erecovery" : "normal");
	}

	return 0;
}
early_param("enter_erecovery", early_parse_erecovery_cmdline);

static bool hybrid_quick_on_valid(void)
{
	struct iomcu_quick_on_config *quick_on_config =
		iomcu_config_get(IOMCU_CONFIG_USR_IOMCU_QUICK_ON,
		sizeof(struct iomcu_quick_on_config));

	LCD_KIT_INFO("quickon_status: 0x%x", quick_on_config->quickon_status);

	/* MCU quick on work or not, reset flag after use */
	if (quick_on_config->quickon_status == QON_ALREADY) {
		quick_on_config->quickon_status = QON_NOT_READY;
		return true;
	}

	return false;
}

static bool hybrid_quick_on_trylock_timeout(int msec, int retry_step)
{
	LCD_KIT_INFO("get in\n");
	int i = 0;
	int count = msec / retry_step;
	int usec_step = retry_step * 1000;
	struct iomcu_quick_on_config *quick_on_config =
		iomcu_config_get(IOMCU_CONFIG_USR_IOMCU_QUICK_ON,
		sizeof(struct iomcu_quick_on_config));

	if (quick_on_config->ap_status) {
		LCD_KIT_WARNING("AP status is 1 before on, force set to 0");
		quick_on_config->ap_status = QON_UNUSED;
	}

	while (i < count) {
		LCD_KIT_INFO("ap status: %d, mcu status: %d, retry: %d, total count: %d",
			quick_on_config->ap_status, quick_on_config->mcu_status, i, count);

		if (!(quick_on_config->ap_status | quick_on_config->mcu_status)) {
			if (quick_on_config->mcu_status) {
				LCD_KIT_WARNING("mcu used from 0 to 1 after retry count: %d", i);
				i = 0;
				continue;
			}
			quick_on_config->ap_status = QON_USED;
			return true;
		}
		i++;
		usleep_range(usec_step, usec_step + 1);
	}

	return false;
}

static bool hybrid_quick_on_force_lock(void)
{
	LCD_KIT_INFO("get in");
	struct iomcu_quick_on_config *quick_on_config =
		iomcu_config_get(IOMCU_CONFIG_USR_IOMCU_QUICK_ON,
		sizeof(struct iomcu_quick_on_config));

	quick_on_config->ap_status = QON_USED;
	return false;
}

static bool hybrid_quick_on_get_lock_status(void)
{
	struct iomcu_quick_on_config *quick_on_config =
		iomcu_config_get(IOMCU_CONFIG_USR_IOMCU_QUICK_ON,
		sizeof(struct iomcu_quick_on_config));

	LCD_KIT_INFO("Quick on lock status: ap_status[%d], mcu_status[%d]",
		quick_on_config->ap_status, quick_on_config->mcu_status);

	return quick_on_config->ap_status == QON_USED &&
		quick_on_config->mcu_status == QON_UNUSED;
}

static bool hybrid_quick_on_mcu_locked(void)
{
	struct iomcu_quick_on_config *quick_on_config =
		iomcu_config_get(IOMCU_CONFIG_USR_IOMCU_QUICK_ON,
		sizeof(struct iomcu_quick_on_config));

	if (!quick_on_config)
		return false;

	LCD_KIT_INFO("mcu_status[%d]", quick_on_config->mcu_status);
	return quick_on_config->mcu_status;
}

static void hybrid_quick_on_unlock(void)
{
	LCD_KIT_INFO("get in");
	struct iomcu_quick_on_config *quick_on_config =
		iomcu_config_get(IOMCU_CONFIG_USR_IOMCU_QUICK_ON,
		sizeof(struct iomcu_quick_on_config));

	quick_on_config->ap_status = QON_UNUSED;
}

static void hybrid_gesture_qon_update(enum HYBRID_GESTURE_QON_STATUS state)
{
	LCD_KIT_INFO("get in, %d", state);

	if (!display_ctrl)
		return;
	spin_lock(&display_ctrl->gesture_quickon_spin);
	display_ctrl->gesture_quickon_seq = state;
	spin_unlock(&display_ctrl->gesture_quickon_spin);
}

static int hybrid_gesture_qon_state(void)
{
	int ret;

	if (!display_ctrl)
		return LCD_KIT_FAIL;

	spin_lock(&display_ctrl->gesture_quickon_spin);
	ret = display_ctrl->gesture_quickon_seq;
	spin_unlock(&display_ctrl->gesture_quickon_spin);

	return ret;
}

static void hybrid_gesture_qon_sync(void)
{
	int ret;

	if (!display_ctrl)
		return;

	if (!get_current_gesture_status())
		return;

	LCD_KIT_INFO("get in");
	ret = down_timeout(&display_ctrl->gesture_qon_sema, msecs_to_jiffies(GES_QON_SYNC_TIMEOUT));
	if (ret == -ETIME) {
		LCD_KIT_ERR("down gesture_qon_sema timeout");
		return;
	}

	up(&display_ctrl->gesture_qon_sema);
}

void lcd_kit_hybrid_gesture_qon_reset(void)
{
	LCD_KIT_INFO("get in");

	if (!get_current_gesture_status())
		return;

	hybrid_gesture_qon_update(GES_QON_SEQ_IDLE);
}

void lcd_kit_hybrid_gesture_qon(void)
{
	LCD_KIT_INFO("get in");

	if (!display_ctrl)
		return;

	if (alpm_get_state() || !get_current_gesture_status())
		return;

	if (down_interruptible(&display_ctrl->gesture_qon_sema)) {
		LCD_KIT_ERR("down gesture_qon_sema error");
		return;
	}

	if (!mutex_trylock(&display_ctrl->hybrid_lock)) {
		LCD_KIT_INFO("Skip, hybrid lock is busy, switching[%d], control[%d]\n",
			display_ctrl->hybrid_control_switching, hybrid_control_check());
		up(&display_ctrl->gesture_qon_sema);
		return;
	}

	if (hybrid_gesture_qon_state() != GES_QON_SEQ_IDLE) {
		LCD_KIT_WARNING("Already under gesture quick on sequence\n");
		goto out;
	}

	if (display_ctrl->composer_on_ongoing || hybrid_quick_on_mcu_locked()) {
		LCD_KIT_WARNING("composer on ongoing, abort gesture qon");
		goto out;
	}

	/* return when no hybrid control or display on */
	if (display_ctrl->hybrid_control_switching ||
		!hybrid_control_check() ||
		hybrid_check_power_on(display_ctrl)) {
		LCD_KIT_WARNING("no control or alrady on\n");
		goto out;
	}

	LCD_KIT_INFO("gesture quick on start, seq = %d\n", display_ctrl->gesture_quickon_seq);
	lcd_kit_hybrid_hw_reset();
	hybrid_gesture_qon_update(GES_QON_SEQ_ONGOING);

out:
	mutex_unlock(&display_ctrl->hybrid_lock);
	up(&display_ctrl->gesture_qon_sema);
}

static void hybrid_composer_on_state(bool state)
{
	if (!display_ctrl)
		return;

	spin_lock(&display_ctrl->comp_on_spin);
	display_ctrl->composer_on_ongoing = state;
	spin_unlock(&display_ctrl->comp_on_spin);
}

static void lcd_kit_send_hybrid_state(u8 state)
{
	int ret = 0;

	if (!display_ctrl || !display_ctrl->display_sw_ops ||
		!display_ctrl->display_sw_ops->send_hybrid_state)
		return;

	ret = display_ctrl->display_sw_ops->send_hybrid_state(display_ctrl->display_sw_ops, state);
	if (ret < 0)
		LCD_KIT_WARNING("send status %d to mcu, ret: %d\n", state, ret);
}

static int32_t hybrid_composer_on_hook(struct composer *comp, uint8_t on_mode)
{
	static bool first_composer_on = true;
	int32_t ret = 0;

	LCD_KIT_INFO("get in");
	if (!comp || !display_ctrl || !display_ctrl->dpu_composer || !display_ctrl->composer_on) {
		LCD_KIT_INFO("comp or dpu_composer or composer_on is null");
		return -EINVAL;
	}

	hybrid_composer_on_state(true);

	lcd_hybrid_trace_begin(__func__);
	if (display_ctrl->hybrid_control_switching || !hybrid_control_check()) {
		LCD_KIT_WARNING("Skip comp on, switching[%d], control[%d]",
			display_ctrl->hybrid_control_switching, hybrid_control_check());
		hybrid_composer_on_state(false);
		hybrid_gesture_qon_sync();
		lcd_kit_hybrid_gesture_qon_reset();
		lcd_hybrid_trace_end(__func__);
		return 0;
	}

	/* Cannot tell current display on is coming from gesture or other */
	if (!alpm_get_state() && on_mode != COMPOSER_ON_MODE_DOZE) {
		/* If timeout is occurred, it is possible remote side still keep mipi */
		if (!hybrid_quick_on_trylock_timeout(QON_LOCK_RT_TIMEOUT, QON_LOCK_RT_STEP_MS)) {
			LCD_KIT_ERR("Timeout to lock for quick on, clear flag to force lock");
			hybrid_quick_on_force_lock();
		}
		hybrid_gesture_qon_sync();
	}

#ifdef CONFIG_EXT_INPUTHUB_MATTING_ALSC
	notify_lcd_state_to_matting_algo_task(MATTING_ALGO_AP_SW, MATTING_ALGO_AP_POWER_NORMAL);
#endif

	if (!lcd_kit_get_enter_recovery_mode() && !first_composer_on &&
		!alpm_get_state() && !hybrid_check_power_on(display_ctrl) &&
		!display_ctrl->als_done) {
		/* change poweroff bl_level to 20 issue:Transition between bright and dark environments */
		LCD_KIT_INFO("org=%u set default bl_level=20", display_ctrl->dpu_composer->bl_ctrl.bl_level);
		display_ctrl->dpu_composer->bl_ctrl.bl_level = 20;
	}
	first_composer_on = false;

	hybrid_quick_on_get_lock_status();
	ret = display_ctrl->composer_on(comp, on_mode);
	hybrid_composer_on_state(false);
	lcd_hybrid_trace_end(__func__);
	return ret;
}

static int32_t hybrid_composer_off_hook(struct composer *comp, uint8_t off_mode)
{
	LCD_KIT_INFO("get in\n");

	if (!comp || !display_ctrl) {
		LCD_KIT_INFO("comp or display_ctrl is null");
		return -EINVAL;
	}

	lcd_kit_hybrid_gesture_qon_reset();
	display_ctrl->als_done = false;
	if (display_ctrl->hybrid_control_switching || !hybrid_control_check()) {
		LCD_KIT_WARNING("Skip comp off, switching[%d], control[%d]",
			display_ctrl->hybrid_control_switching, hybrid_control_check());
		return 0;
	}

#ifdef CONFIG_EXT_INPUTHUB_MATTING_ALSC
	notify_lcd_state_to_matting_algo_task(0, 0);
#endif
	return display_ctrl->composer_off(comp, off_mode);
}

static int32_t hybrid_composer_present_hook(struct composer *comp, void *frame)
{
	if (!comp || !frame || !display_ctrl || !display_ctrl->composer_present) {
		LCD_KIT_INFO("pointer is null");
		return -EINVAL;
	}

	if (display_ctrl->hybrid_control_switching || !hybrid_control_check() ||
		display_ctrl->sta_state) {
		LCD_KIT_WARNING("Skip present, switching[%d], control[%d]",
			display_ctrl->hybrid_control_switching, hybrid_control_check());
		return -EACCES;
	}

	return display_ctrl->composer_present(comp, frame);
}

static int lcd_kit_hybrid_set_mipi_backlight(int panel_id, void *hld, u32 bl_level)
{
	if (hybrid_get_block_brightness_flag(display_ctrl)) {
		LCD_KIT_INFO("block_brightness_flag return");
		return 0;
	}

	return hybrid_common_ops.set_mipi_backlight(panel_id, hld, bl_level);
}

static int lcd_kit_hybrid_set_hbm_backlight(int panel_id, void *hld, u32 level)
{
	if (hybrid_get_block_brightness_flag(display_ctrl)) {
		LCD_KIT_INFO("block_brightness_flag return");
		return 0;
	}

	return hybrid_common_ops.set_hbm_backlight(panel_id, hld, level);
}

static void lcd_kit_str_off_done(void)
{
	LCD_KIT_INFO("get in");

	unsigned char data[] = { DISPLAY_STR_AP_DONE, HYBRID_OFF_SWITCH_STR };
	const unsigned char sid = 0x01;
	const unsigned char cid = 0x9f;

	send_inner_command(sid, cid, data, ARRAY_SIZE(data));
}

static int lcd_kit_str_msg_handle(int data)
{
	int ret;
	u32 panel_id = lcd_kit_get_active_panel_id();

	LCD_KIT_INFO("get in");
	/* wait 3 HZ timeout for resume */
	ret = down_timeout(&display_ctrl->sr_wake_sema, 3 * HZ);
	if (ret == -ETIME) {
		LCD_KIT_INFO("down sr wake sema timeout");
		return -EINVAL;
	}
	if (data == 0) {
		lcd_kit_hybrid_mode_async(panel_id, HYBRID_OFF_SWITCH_STR);
		lcd_kit_str_off_done();
	} else if (data == 1) {
		lcd_kit_hybrid_mode_async(panel_id, HYBRID_ON_SWITCH_STR);
	} else {
	}
	up(&display_ctrl->sr_wake_sema);

	return 0;
}

static struct device_fb *mipi_switch_get_fb(void)
{
	struct device_fb *fb = NULL;

	if (display_ctrl->dpu_composer == NULL || display_ctrl->dpu_composer->comp.device_data == NULL) {
		LCD_KIT_ERR("mipi_switch_get_fb display_ctrl->dpu_composer is null\n");
		return NULL;
	}
 
	fb = (struct device_fb *)display_ctrl->dpu_composer->comp.device_data;
	if (fb == NULL || fb->fbi_info == NULL || fb->fbi_info->dev == NULL) {
		LCD_KIT_ERR("mipi_switch_get_fb fb->fbi_info is null\n");
		return NULL;
	}
	return fb;
}

static void mipiclk_to_string(unsigned int num, char *buffer, unsigned int buffer_size)
{
    if (buffer == NULL || buffer_size != CONT_MIPI_CLK) {
        LCD_KIT_INFO("mipiclk_to_string buffer is NULL");
        return;
    }

    int mipiclk_str_len = buffer_size - MIPI_CLK_BIT_OFFSET;
    for (int i = mipiclk_str_len - MIPI_CLK_BIT_OFFSET; i >= 0; i--) {
        buffer[mipiclk_str_len - MIPI_CLK_BIT_OFFSET - i] = (num & (1 << i)) ? '1' : '0';
    }
    buffer[buffer_size - 1] = '\0';
    LCD_KIT_DEBUG("uint8 data to string buf=%s\n", buffer);
}

static void mipiclk_upt_store(int data)
{
    char mipiclk[CONT_MIPI_CLK] = { 0 };

    struct device_fb *fb = mipi_switch_get_fb();
    if (fb == NULL) {
        LCD_KIT_ERR("mipi_switch_handle fb is null\n");
        return;
    }

	if ((data == MIPI_CLK_255MHZ) || (data == MIPI_CLK_260MHZ) || (data == MIPI_CLK_252MHZ) ||
        (data == MIPI_CLK_245MHZ) || (data == MIPI_CLK_240MHZ)) {
        mipiclk_to_string(data, mipiclk, CONT_MIPI_CLK);
        mipi_dsi_bit_clk_upt_store(fb->fbi_info->dev, NULL, mipiclk, NULL);
        LCD_KIT_INFO("LCD: mipi_switch_handle fb %d, fb->fbi_info %d, fb->fbi_info->dev %d, data:%d\n",
            fb, fb->fbi_info, fb->fbi_info->dev, data);
	}
}

static void mipi_switch_handle(int cmd, int data)
{
	int ret;

	if (!display_ctrl || !display_msg)
		return;

	switch (cmd) {
	case DISPLAY_STR_CTRL:
		lcd_kit_str_msg_handle(data);
		break;
	case DISPLAY_STA_STATE:
#ifdef CONFIG_FACTORY_MODE
		LCD_KIT_INFO("DISPLAY_STA_STATE...\n");
		display_ctrl->sta_state = true;
#endif /* CONFIG_FACTORY_MODE */
		break;
	case DISPLAY_STR_GET_DDIC:
		hybrid_get_display_msg();
		ret = send_msg_to_mcu(display_msg);
		LCD_KIT_INFO("send msg ret: %d\n", ret);
		break;
	case DISPLAY_STR_BRIGHTNESS:
		if (data > 0 && data <= DISPLAY_MAX_BRIGHTNESS) {
			if (!lcd_kit_get_enter_recovery_mode() && !alpm_get_state()) {
				LCD_KIT_INFO("update bl_level=%u", data);
				display_ctrl->als_done = true;
				display_ctrl->dpu_composer->bl_ctrl.bl_level = data;
			}
		}
		break;
	case DISPLAY_SET_MIPI_CLK:
        mipiclk_upt_store(data);
		break;
	default:
		break;
	}
}

static u8 composer_power_restarting(void)
{
	struct composer_manager *mgr = get_dpu_composer_manager();

	if (!mgr)
		return 0;

	return mgr->is_power_restarting;
}

u8 get_current_gesture_status(void)
{
	return current_gesture_status;
}

void set_current_gesture_status(u8 status)
{
	current_gesture_status = status;
}

void lcd_kit_hybrid_hw_reset(void)
{
	int panel_id = lcd_kit_get_active_panel_id();

	mutex_lock(&COMMON_LOCK->mipi_lock);
	ts_kit_hybrid_irq_close();
	lcd_kit_reset_power_ctrl(panel_id, LCD_RESET_LOW);
	/* reset pull low need 10ms */
	usleep_range(10000, 10001);
	lcd_kit_reset_power_ctrl(panel_id, LCD_RESET_HIGH);
	/* reset pull high need 20ms */
	msleep(21);
	ts_kit_hybrid_irq_open();
	mutex_unlock(&COMMON_LOCK->mipi_lock);
}

static int lcd_kit_hybrid_power_on(int panel_id, void *hld)
{
	int ret = 0;
	int iovcc_state = 0;

	LCD_KIT_INFO("get in");
	if (!display_ctrl || !hybrid_common_ops.panel_power_on)
		return -EINVAL;

	if (display_ctrl && display_ctrl->skip_panel_onoff)
		return 0;

	if (!mutex_trylock(&display_ctrl->hybrid_lock)) {
		LCD_KIT_INFO("Skip, hybrid is working, switching[%d], control[%d]",
			display_ctrl->hybrid_control_switching, hybrid_control_check());
		return 0;
	}

	if (display_ctrl->hybrid_control_switching || !hybrid_control_check()) {
		mutex_unlock(&display_ctrl->hybrid_lock);
		return 0;
	}

	LCD_KIT_INFO("current_gesture_status = %u\n", current_gesture_status);
	if (display_ctrl->display_sw_ops && display_ctrl->display_sw_ops->power_control) {
		LCD_KIT_INFO("Send power on cmd to mcu...\n");
		ret = display_ctrl->display_sw_ops->power_control(display_ctrl->display_sw_ops, 1);
		if (ret >= 0) {
			mutex_unlock(&display_ctrl->hybrid_lock);
			return ret;
		}
	}

	LCD_KIT_INFO("power on in mcu, ret: %d, fallback to ap side\n", ret);

	/*
     * When power on at remote side failed, it is possible
	 * remote side crashed, in this case, only check iovcc.
	 * VCI, RST GPIO may not ready, no need check them.
	 */
	iovcc_state = hybrid_get_display_iovcc_state();
	if (alpm_get_state() && iovcc_state != LCD_KIT_FAIL && iovcc_state) {
		LCD_KIT_INFO("AOD with power, return");
		mutex_unlock(&display_ctrl->hybrid_lock);
		return LCD_KIT_OK;
	}

	ret = hybrid_common_ops.panel_power_on(panel_id, hld);
	mutex_unlock(&display_ctrl->hybrid_lock);
	return ret;
}

static int lcd_kit_hybrid_power_off(int panel_id, void *hld)
{
	int ret = 0;
	u8 comp_pwr_restart = composer_power_restarting();

	LCD_KIT_INFO("get in");
	if (!hybrid_common_ops.panel_power_off)
		return -EINVAL;

	if (display_ctrl && display_ctrl->skip_panel_onoff)
		return 0;

	if (!mutex_trylock(&display_ctrl->hybrid_lock)) {
		LCD_KIT_INFO("Skip, hybrid is working, switching[%d], control[%d]",
			display_ctrl->hybrid_control_switching, hybrid_control_check());
		return 0;
	}

	if (display_ctrl->hybrid_control_switching || !hybrid_control_check()) {
		mutex_unlock(&display_ctrl->hybrid_lock);
		return 0;
	}

	LCD_KIT_INFO("current_gesture_status = %u %u, comp_pwr_restart = %u\n",
		current_gesture_status, get_gesture_switch(), comp_pwr_restart);
	if (current_gesture_status == 1 && !comp_pwr_restart) {
		LCD_KIT_INFO("return\n");
		mutex_unlock(&display_ctrl->hybrid_lock);
		return ret;
	}

	if (display_ctrl->display_sw_ops && display_ctrl->display_sw_ops->power_control) {
		LCD_KIT_INFO("Send power off cmd to mcu...\n");
		ret = display_ctrl->display_sw_ops->power_control(display_ctrl->display_sw_ops, 0);
		if (ret >= 0) {
			mutex_unlock(&display_ctrl->hybrid_lock);
			return ret;
		}
	}
	LCD_KIT_INFO("power off in mcu, ret: %d, fallback to ap side\n", ret);
	ret = hybrid_common_ops.panel_power_off(panel_id, hld);
	mutex_unlock(&display_ctrl->hybrid_lock);
	return ret;
}

static int lcd_kit_hybrid_set_on(int panel_id, void *hld)
{
	int ret = LCD_KIT_OK;
	u8 lcd_status = 0;
	u8 comp_pwr_restarting = composer_power_restarting();

	if (!display_ctrl)
		return -EINVAL;

	/* Confirm again with remote side, to make sure quick on is work */
	if (hybrid_quick_on_get_lock_status() && hybrid_quick_on_valid()) {
		usleep_range(1000, 1001);
		ret = hybrid_get_lcd_status(display_ctrl, &lcd_status);
		if (ret)
			LCD_KIT_ERR("Fail to get lcd status");
	} else {
		LCD_KIT_INFO("quick on not work");
	}

	if (lcd_status != STATUS_NORMAL) {
		if (current_gesture_status && !comp_pwr_restarting) {
			if (hybrid_gesture_qon_state() != GES_QON_SEQ_ONGOING) {
				LCD_KIT_INFO("lcd hw reset, lcd_status: 0x%x", lcd_status);
				lcd_kit_hybrid_hw_reset();
			}
		}
		ret = hybrid_common_ops.panel_on_lp(panel_id, hld);
	}

	return ret;
}

static int lcd_set_mipi_clk(void)
{
	char mipiclk_string[CONT_MIPI_CLK] = { 0 };
	struct device_fb *fb = NULL;

	LCD_KIT_INFO("LCD: lcd_set_mipi_clk start!\n");
    fb = mipi_switch_get_fb();
	if (fb == NULL || fb->fbi_info == NULL) {
		LCD_KIT_ERR("lcd_set_mipi_clk fb is null\n");
		return -EINVAL;
	}

	struct iomcu_mipi_clk_config *mipi_clk_config =
		iomcu_config_get(IOMCU_CONFIG_USR_IOMCU_MIPI_CLK,
		sizeof(struct iomcu_mipi_clk_config));

	if (!mipi_clk_config) {
		LCD_KIT_ERR("get mipi clk data from SHM failed!\n");
		return -EINVAL;
	}

	if ((mipi_clk_config->data == MIPI_CLK_260MHZ) || (mipi_clk_config->data == MIPI_CLK_255MHZ) ||
	    (mipi_clk_config->data == MIPI_CLK_252MHZ) || (mipi_clk_config->data == MIPI_CLK_245MHZ) ||
        (mipi_clk_config->data == MIPI_CLK_240MHZ)) {
		mipiclk_to_string(mipi_clk_config->data, mipiclk_string, CONT_MIPI_CLK);
		mipi_dsi_bit_clk_upt_store(fb->fbi_info->dev, NULL, mipiclk_string, NULL);
        LCD_KIT_INFO("LCD: lcd_set_mipi_clk fb %d, fb->fbi_info %d, fb->fbi_info->dev %d, data:%d\n",
		    fb, fb->fbi_info, fb->fbi_info->dev, mipi_clk_config->data);
		return LCD_KIT_OK;
	}
	LCD_KIT_INFO("mipi clk data not support!\n");

	return -EINVAL;
}

static int lcd_kit_hybrid_on_lp(int panel_id, void *hld)
{
	int ret = 0;

	LCD_KIT_INFO("get in");
	if (!display_ctrl)
		return -EINVAL;

	if (display_ctrl->skip_panel_onoff)
		return 0;

	lcd_hybrid_trace_begin(__func__);
	hybrid_gesture_qon_sync();
	if (!mutex_trylock(&display_ctrl->hybrid_lock)) {
		LCD_KIT_INFO("Skip, hybrid is working, switching[%d], control[%d]",
			display_ctrl->hybrid_control_switching, hybrid_control_check());
		hybrid_composer_on_state(false);
		lcd_kit_hybrid_gesture_qon_reset();
		lcd_hybrid_trace_end(__func__);
		return 0;
	}

	if (display_ctrl->hybrid_control_switching || !hybrid_control_check())
		goto out;

	if (alpm_get_state()) {
		ret = alpm_exit(display_ctrl);
		goto out;
	}

	ret = lcd_kit_hybrid_set_on(panel_id, hld);
	send_hybrid_ts_cmd(TS_KIT_HYBRID_RESUME);
	lcd_kit_hybrid_esd_on();
	lcd_kit_send_hybrid_state(AP_DISPLAY_ON);
	hybrid_quick_on_unlock();
	if (lcd_set_mipi_clk() != LCD_KIT_OK) {
		LCD_KIT_INFO("set mipi clk error\n");
	}
out:
	mutex_unlock(&display_ctrl->hybrid_lock);
	hybrid_composer_on_state(false);
	lcd_kit_hybrid_gesture_qon_reset();
	lcd_hybrid_trace_end(__func__);
	return ret;
}

static int lcd_kit_hybrid_mipi_switch(int panel_id, void *hld)
{
	if (display_ctrl->hybrid_control_switching || !hybrid_control_check())
		return 0;

	/* power on */
	if (display_ctrl->display_sw_ops && display_ctrl->display_sw_ops->power_control)
		display_ctrl->display_sw_ops->power_control(display_ctrl->display_sw_ops, 1);

	return 0;
}

static int lcd_kit_hybrid_mipi_switch_on_lp(int panel_id, void *hld)
{
	if (!display_ctrl)
		return -EINVAL;

	if (display_ctrl->hybrid_control_switching || !hybrid_control_check())
		return 0;

	lcd_kit_set_normal(display_ctrl);
	send_hybrid_ts_cmd(TS_KIT_HYBRID_RESUME);

	return 0;
}

static int lcd_kit_hybrid_esd_handle(int panel_id, void *hld)
{
	int ret;

	mutex_lock(&COMMON_LOCK->mipi_lock);
	ret = hybrid_common_ops.esd_handle(panel_id, hld);
	mutex_unlock(&COMMON_LOCK->mipi_lock);

	return ret;
}

void lcd_kit_hybrid_esd_on(void)
{
	int panel_id = lcd_kit_get_active_panel_id();

	mutex_lock(&COMMON_LOCK->mipi_lock);
	if (common_info->esd.support)
		common_info->esd.status = ESD_RUNNING;
	mutex_unlock(&COMMON_LOCK->mipi_lock);
}

void lcd_kit_hybrid_esd_off(void)
{
	int panel_id = lcd_kit_get_active_panel_id();

	mutex_lock(&COMMON_LOCK->mipi_lock);
	if (common_info->esd.support)
		common_info->esd.status = ESD_STOP;
	mutex_unlock(&COMMON_LOCK->mipi_lock);
}

static int lcd_kit_hybrid_off_lp(int panel_id, void *hld)
{
	int ret = 0;

	LCD_KIT_INFO("get in");
	if (!display_ctrl)
		return -EINVAL;

	if (display_ctrl && display_ctrl->skip_panel_onoff)
		return 0;

	if (!mutex_trylock(&display_ctrl->hybrid_lock)) {
		LCD_KIT_INFO("Skip, hybrid is working, switching[%d], control[%d]",
			display_ctrl->hybrid_control_switching, hybrid_control_check());
		return 0;
	}

	if (display_ctrl->hybrid_control_switching || !hybrid_control_check()) {
		mutex_unlock(&display_ctrl->hybrid_lock);
		return 0;
	}

	alpm_set_state_flag(ALPM_STATE_OUT);
	lcd_kit_hybrid_esd_off();

	set_current_gesture_status(get_gesture_switch());
	LCD_KIT_INFO("current_gesture_status = %u %u, comp_pwr_restart: %u\n",
		current_gesture_status, get_gesture_switch(), composer_power_restarting());
	if (current_gesture_status == 1 && !composer_power_restarting()) {
		LCD_KIT_INFO("Send deep sleep cmd to lcd...\n");
		send_hybrid_ts_cmd(TS_KIT_HYBRID_IDLE);
		ret = lcd_kit_enter_deep_sleep(display_ctrl);
		if (ret)
			LCD_KIT_WARNING("fail to deep sleep!\n");

		lcd_kit_send_hybrid_state(AP_DISPLAY_DEEP_SLEEP);
	} else {
		ts_kit_set_resp_state(0);
		send_hybrid_ts_cmd(TS_KIT_HYBRID_SUSPEND);
		if (hybrid_common_ops.panel_off_lp) {
			ret = hybrid_common_ops.panel_off_lp(panel_id, hld);
			if (ret < 0)
				LCD_KIT_WARNING("set DISPLAY_OFF to mcu, ret: %d\n", ret);
		}
		ts_hybrid_wait_resp();
	}

	lcd_kit_send_hybrid_state(AP_DISPLAY_OFF);

	mutex_unlock(&display_ctrl->hybrid_lock);
	return ret;
}

static int lcd_kit_hybrid_doze_lp(int panel_id, void *hld)
{
	int ret;

	LCD_KIT_INFO("get in");
	if (display_ctrl && display_ctrl->skip_panel_onoff)
		return 0;

	if (!mutex_trylock(&display_ctrl->hybrid_lock)) {
		LCD_KIT_INFO("hybrid is busy, skip");
		return -EACCES;
	}
	if (display_ctrl->hybrid_control_switching || !hybrid_control_check()) {
		LCD_KIT_INFO("skip, control[%d], switching[%d]",
			hybrid_control_check(), display_ctrl->hybrid_control_switching);
		hybrid_composer_on_state(false);
		mutex_unlock(&display_ctrl->hybrid_lock);
		return -EACCES;
	}
	ret = alpm_doze(display_ctrl);
	mutex_unlock(&display_ctrl->hybrid_lock);

	hybrid_composer_on_state(false);
	return ret;
}

static int lcd_kit_hybrid_doze_suspend(int panel_id, void *hld)
{
	int ret;

	LCD_KIT_INFO("get in");
	if (display_ctrl && display_ctrl->skip_panel_onoff)
		return 0;
	if (!mutex_trylock(&display_ctrl->hybrid_lock)) {
		LCD_KIT_INFO("hybrid is busy, skip");
		return -EACCES;
	}
	if (display_ctrl->hybrid_control_switching || !hybrid_control_check()) {
		LCD_KIT_INFO("skip, control[%d], switching[%d]",
			hybrid_control_check(), display_ctrl->hybrid_control_switching);
		mutex_unlock(&display_ctrl->hybrid_lock);
		return -EACCES;
	}
	ret = alpm_doze_suspend(display_ctrl);
	mutex_unlock(&display_ctrl->hybrid_lock);
	return ret;
}

static void lcd_kit_hybrid_task_init(void)
{
	int ret;
	struct sched_param param = { .sched_priority = 5 };

	init_waitqueue_head(&display_ctrl->resp_wait);
	kthread_init_worker(&display_ctrl->hybrid_worker);
	display_ctrl->hybrid_worker_task =
		kthread_run(kthread_worker_fn, &display_ctrl->hybrid_worker, "%s", "hybrid_worker");
	if (IS_ERR(display_ctrl->hybrid_worker_task)) {
		LCD_KIT_ERR("failed to create hybrid_worker_task");
		return;
	}

	ret = sched_setscheduler(display_ctrl->hybrid_worker_task,
				SCHED_FIFO, &param);

	LCD_KIT_INFO("get init sched_setscheduler ret:%d", ret);
}

static int lcd_kit_hybrid_queue_work(struct kthread_worker *worker, struct kthread_work *work)
{
	int i;
	bool ret;

	if (!worker || !work)
		return LCD_KIT_FAIL;

	for (i = 0; i < QUEUE_WORK_MAX_RETRY; ++i) {
		ret = kthread_queue_work(worker, work);
		if (ret)
			break;
		LCD_KIT_ERR("fail to queue work, retry: %d\n", i);
	}
	return ret ? LCD_KIT_OK : LCD_KIT_FAIL;
}

static void lcd_kit_hybrid_mode_work_func(struct kthread_work *hb_work)
{
	int ret;
	struct hybrid_work *hybrid_switch_work;

	if (!hb_work || !display_ctrl) {
		LCD_KIT_ERR("hb_work or display_ctrl is null");
		return;
	}

	hybrid_switch_work = container_of(hb_work, struct hybrid_work, work);
	if (!hybrid_switch_work) {
		LCD_KIT_ERR("hybrid_switch_work is null");
		spin_lock(&hybrid_spin_lock);
		hybrid_switch_work->ret = -EFAULT;
		hybrid_switch_work->finish = 1;
		spin_unlock(&hybrid_spin_lock);

		wake_up(&display_ctrl->resp_wait);
		return;
	}

	ret = lcd_kit_hybrid_mode(hybrid_switch_work->panel_id, hybrid_switch_work->mode);
	spin_lock(&hybrid_spin_lock);
	hybrid_switch_work->ret = ret;
	hybrid_switch_work->finish = 1;
	spin_unlock(&hybrid_spin_lock);

	wake_up(&display_ctrl->resp_wait);
}

int lcd_kit_hybrid_mode_async(u32 panel_id, enum hybrid_mode mode)
{
	int ret;
	struct hybrid_work *hybrid_switch_work = NULL;

	LCD_KIT_INFO("get in mode send:%u\n", mode);
	if (!display_ctrl) {
		LCD_KIT_ERR("display_ctrl is null");
		return -EINVAL;
	}

	lcd_hybrid_trace_begin(__func__);
	mutex_lock(&ctrl_hybrid_mode_lock);
	hybrid_switch_work = kzalloc(sizeof(*hybrid_switch_work), GFP_KERNEL);
	if (!hybrid_switch_work) {
		LCD_KIT_ERR("fail to create hybrid work, return");
		mutex_unlock(&ctrl_hybrid_mode_lock);
		lcd_hybrid_trace_end(__func__);
		return -EFAULT;
	}
	kthread_init_work(&hybrid_switch_work->work, lcd_kit_hybrid_mode_work_func);

	spin_lock(&hybrid_spin_lock);
	hybrid_switch_work->panel_id = panel_id;
	hybrid_switch_work->mode = mode;
	hybrid_switch_work->finish = 0;
	spin_unlock(&hybrid_spin_lock);

	ret = lcd_kit_hybrid_queue_work(&display_ctrl->hybrid_worker, &hybrid_switch_work->work);
	if (ret != LCD_KIT_OK) {
		kfree(hybrid_switch_work);
		hybrid_switch_work = NULL;
		mutex_unlock(&ctrl_hybrid_mode_lock);
		lcd_hybrid_trace_end(__func__);
		return -EFAULT;
	}

	ret = wait_event_interruptible(display_ctrl->resp_wait, hybrid_switch_work->finish == 1);
	if (!ret)
		ret = hybrid_switch_work->ret;
	else {
		LCD_KIT_ERR("hybrid_switch_work resp_wait error interrupt");
		ret = -EINTR;
	}

	kfree(hybrid_switch_work);
	hybrid_switch_work = NULL;
	mutex_unlock(&ctrl_hybrid_mode_lock);
	LCD_KIT_INFO("get out mode send:%u\n", mode);
	lcd_hybrid_trace_end(__func__);
	return ret;
}

int lcd_kit_hybrid_mode(u32 panel_id, enum hybrid_mode mode)
{
	int i;
	int ret = 0;

	LCD_KIT_INFO("get in mode send:%u\n", mode);
	if (!display_ctrl)
		return -EFAULT;

	if (display_ctrl->current_state == HYBRID_ON_SWITCH_STR &&
		mode != HYBRID_OFF_SWITCH_STR) {
		LCD_KIT_INFO("cur mode: STR, err mode:%u\n", mode);
		return -EFAULT;
	}
	if (mode == HYBRID_OFF_SWITCH_STR &&
		display_ctrl->current_state != HYBRID_ON_SWITCH_STR) {
		LCD_KIT_INFO("cur mode: STR, err mode:%u\n", mode);
		return -EFAULT;
	}
	display_ctrl->current_state = mode;

	/* match mode and handle */
	for (i = 0; i < ARRAY_SIZE(handle_table); ++i) {
		if (mode != handle_table[i].mode)
			continue;
		if (handle_table[i].hybrid_handle)
			ret = handle_table[i].hybrid_handle(display_ctrl);

		LCD_KIT_INFO("get out mode send:%u\n", mode);
		return ret;
	}
	return -EINVAL;
}

static int lcd_kit_hybrid_sr_notify(struct notifier_block *notify_block,
	unsigned long mode, void *unused)
{
	if (!display_ctrl) {
		LCD_KIT_ERR("display_ctrl is invalid!\n");
		return 0;
	}

	switch (mode) {
	case PM_POST_SUSPEND:
		LCD_KIT_INFO("get in PM_POST_SUSPEND, resume");
		up(&display_ctrl->sr_wake_sema);
		break;
	case PM_SUSPEND_PREPARE:
		LCD_KIT_INFO("get in PM_SUSPEND_PREPARE, suspend");
		if (down_interruptible(&display_ctrl->sr_wake_sema))
			LCD_KIT_ERR("down sr wake sema error");
		break;
	default:
		break;
	}

	return 0;
}

static struct notifier_block g_lcdkit_sr_notifier = {
	.notifier_call = lcd_kit_hybrid_sr_notify,
	.priority = INT_MIN,
};

static void lcd_kit_set_common_ops(void)
{
	if (!common_ops)
		return;

	common_ops->panel_power_on = lcd_kit_hybrid_power_on;
	common_ops->panel_power_off = lcd_kit_hybrid_power_off;
	common_ops->panel_on_lp = lcd_kit_hybrid_on_lp;
	common_ops->panel_off_lp = lcd_kit_hybrid_off_lp;
	common_ops->panel_doze_lp = lcd_kit_hybrid_doze_lp;
	common_ops->panel_doze_suspend = lcd_kit_hybrid_doze_suspend;
	common_ops->panel_mipi_switch = lcd_kit_hybrid_mipi_switch;
	common_ops->panel_mipi_switch_on_lp = lcd_kit_hybrid_mipi_switch_on_lp;
	common_ops->set_mipi_backlight = lcd_kit_hybrid_set_mipi_backlight;
	common_ops->set_hbm_backlight = lcd_kit_hybrid_set_hbm_backlight;
	common_ops->esd_handle = lcd_kit_hybrid_esd_handle;
}

static ssize_t lcd_kit_hybrid_read(struct file *file, char __user *buf, size_t count, loff_t *pos)
{
	int ret;
	/* 2 char string */
	char str[2] = { 0 };
	bool control = hybrid_control_check();
	static bool last_control = true;

	LCD_KIT_INFO("get in\n");
	if (!buf)
		return -EINVAL;

	if (count < sizeof(str)) {
		LCD_KIT_ERR("param count is invalid\n");
		return -EINVAL;
	}

	if (display_ctrl->hybrid_control_switching) {
		str[0] = last_control ? '1' : '0';
	} else {
		str[0] = control ? '1' : '0';
		last_control = control;
	}

	ret = copy_to_user(buf, str, sizeof(str));
	if (ret != 0) {
		LCD_KIT_ERR("copy to user buffer failed\n");
		return -EFAULT;
	}

	return count;
}

static ssize_t lcd_kit_hybrid_write(struct file *file, const char __user *data,
	size_t len, loff_t *ppos)
{
	int ret;
	int mode = 0;
	char buf[MAX_BUF_LEN];
	u32 panel_id = lcd_kit_get_active_panel_id();

	LCD_KIT_INFO("get in len:%d\n", len);
	if (!data)
		return -EINVAL;

	if (len >= MAX_BUF_LEN) {
		LCD_KIT_ERR("buf overflow!\n");
		return -EINVAL;
	}

	if (copy_from_user(buf, data, len))
		return -EFAULT;

	ret = sscanf_s(buf, "%d", &mode);
	if (ret != 1) {
		LCD_KIT_ERR("sscanf return invaild:%d\n", ret);
		return -EFAULT;
	}

	ret = lcd_kit_hybrid_mode_async(panel_id, mode);
	LCD_KIT_INFO("get mode:%d, ret:%d\n", mode, ret);

	return ret;
}

static const struct file_operations lcd_kit_hybrid_ops = {
	.owner = THIS_MODULE,
	.read = lcd_kit_hybrid_read,
	.write = lcd_kit_hybrid_write,
};

static struct miscdevice lcd_kit_hybrid_miscdev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "lcd_kit_hybrid",
	.fops = &lcd_kit_hybrid_ops,
};

static void report_mipi_timeout(int value)
{
#ifdef CONFIG_HISYSEVENT
	int ret;
	struct hiview_hisysevent *event = NULL;
	s8 record_buf[HISYSEVENT_BUF_SIZE] = {'\0'};

	event = hisysevent_create("LCD", "DSM_LCD_MIPI_TRANSMIT_ERROR_NO", FAULT);
	if (!event) {
		LCD_KIT_ERR("failed to create event\n");
		return;
	}

	ret = snprintf_s(record_buf, HISYSEVENT_BUF_SIZE,
			 HISYSEVENT_BUF_SIZE - 1, "mipi request:%d timeout\n", value);
	if (ret < 0) {
		LCD_KIT_ERR("snprintf happened error!\n");
		goto hisysevent_end;
	}

	hisysevent_put_string(event, "DESCRIPTION", record_buf);
	hisysevent_put_string(event, "MODULE_NAME", "lcd_kit_hybrid");

	ret = hisysevent_write(event);
	if (ret != 0)
		LCD_KIT_ERR("report_mipi_timeout fail, ret: %d\n", ret);

hisysevent_end:
	hisysevent_destroy(&event);
#endif
}

int hybrid_request_display(int value)
{
	int ret;
	int retry = 0;

	/* get mipi switch interface to request mipi */
	if (!display_ctrl || !display_ctrl->display_sw_ops)
		return -EINVAL;

	if (hybrid_control_check() == !!value)
		LCD_KIT_INFO("control state already match!\n");

	while (retry < REQUEST_CONTROL_MAX_RETRY) {
		if (display_ctrl->display_sw_ops->request_control) {
			ret = display_ctrl->display_sw_ops->request_control(display_ctrl->display_sw_ops, value);
			if (ret >= 0)
				break;
		}
		LCD_KIT_INFO("request lcd control fail, retry:%d\n", retry);
		report_mipi_timeout(value);
		retry++;
	}
	return ret;
}

bool hybrid_control_check(void)
{
	if (!display_ctrl)
		return false;

	/* get mipi switch interface to check mipi status */
	if (display_ctrl->display_sw_ops && display_ctrl->display_sw_ops->sw_status_check)
		return display_ctrl->display_sw_ops->sw_status_check(display_ctrl->display_sw_ops);

	return false;
}

bool hybrid_control_check_mcu(void)
{
	if (!display_ctrl)
		return false;

	/* get mipi switch interface to check mipi status */
	if (display_ctrl->display_sw_ops && display_ctrl->display_sw_ops->sw_status_check_mcu)
		return display_ctrl->display_sw_ops->sw_status_check_mcu(display_ctrl->display_sw_ops);

	return false;
}

static int hybrid_get_display_power_state(void)
{
	int panel_id = 0;
	int vci_gpio = 0;
	int iovcc_reg = 0;
	int rst_gpio = 0;
	struct lcd_kit_adapt_ops *adapt_ops = NULL;

	panel_id = lcd_kit_get_active_panel_id();
	adapt_ops = lcd_kit_get_adapt_ops();
	if (!adapt_ops || !adapt_ops->regulator_read) {
		LCD_KIT_ERR("get adapt_ops or adapt_ops->regulator_read error\n");
		return LCD_KIT_FAIL;
	}

	vci_gpio = gpio_get_value(LCD_VCI_GPIO);
	iovcc_reg = adapt_ops->regulator_read(panel_id, LCD_KIT_IOVCC, IS_ENABLED);
	rst_gpio = gpio_get_value(LCD_RST_GPIO);
	LCD_KIT_INFO("LCD_VCI_GPIO = %d, LCD_KIT_IOVCC = %d, LCD_RST_GPIO = %d\n",
		vci_gpio, iovcc_reg, rst_gpio);

	return (vci_gpio && iovcc_reg && rst_gpio);
}

static int hybrid_get_display_iovcc_state(void)
{
	int panel_id = 0;
	int iovcc_reg = 0;
	struct lcd_kit_adapt_ops *adapt_ops = NULL;

	panel_id = lcd_kit_get_active_panel_id();
	adapt_ops = lcd_kit_get_adapt_ops();
	if (!adapt_ops || !adapt_ops->regulator_read) {
		LCD_KIT_ERR("get adapt_ops or adapt_ops->regulator_read error\n");
		return LCD_KIT_FAIL;
	}

	iovcc_reg = adapt_ops->regulator_read(panel_id, LCD_KIT_IOVCC, IS_ENABLED);
	LCD_KIT_INFO("LCD_KIT_IOVCC = %d", iovcc_reg);

	return iovcc_reg;
}

void hybrid_set_display_msg(u8 state)
{
	if (!display_msg || state < AP_DISPLAY_ON)
		return;

	display_msg->lcd_state = state;
	if (state == AP_DISPLAY_DEEP_SLEEP)
		display_msg->deep_sleep = 1;
	else if (state == AP_DISPLAY_ON)
		display_msg->deep_sleep = 0;
}

void hybrid_get_display_msg(void)
{
	if (!display_msg)
		return;

	display_msg->hybrid_ctr = hybrid_control_check();
	if (hybrid_get_display_power_state())
		display_msg->power_state = 1;
	else
		display_msg->power_state = 0;
}

int lcd_display_msg_init(void)
{
	display_msg = kzalloc(sizeof(*display_msg), GFP_KERNEL);
	if (!display_msg)
		return LCD_KIT_FAIL;

	if (memset_s(display_msg, sizeof(struct hybrid_display_msg), 0, sizeof(struct hybrid_display_msg)) != EOK) {
		kfree(display_msg);
		display_msg = NULL;
		return LCD_KIT_FAIL;
	}

	if (hybrid_get_display_power_state())
		display_msg->lcd_state = AP_DISPLAY_ON;
	else
		display_msg->lcd_state = AP_DISPLAY_OFF;

	return LCD_KIT_OK;
}

static void lcd_kit_hybrid_composer_init(struct dpu_composer *dpu_composer)
{
	if (!display_ctrl || !dpu_composer) {
		LCD_KIT_ERR("display_ctrl,dpu_composer is null!\n");
		return;
	}

	if (!dpu_composer->comp.on || !dpu_composer->comp.off) {
		LCD_KIT_ERR("dpu_composer->comp is null!\n");
		return;
	}

	display_ctrl->composer_on = dpu_composer->comp.on;
	display_ctrl->composer_off = dpu_composer->comp.off;
	if (display_ctrl->composer_on && display_ctrl->composer_off) {
		dpu_composer->comp.on = hybrid_composer_on_hook;
		dpu_composer->comp.off = hybrid_composer_off_hook;
	}
	display_ctrl->composer_present = dpu_composer->comp.present;
	if (display_ctrl->composer_present)
		dpu_composer->comp.present = hybrid_composer_present_hook;
}

void lcd_kit_hybrid_init(void)
{
	struct composer_manager *mgr = NULL;
	struct dpu_composer *dpu_composer = NULL;

	LCD_KIT_INFO("+\n");
	mgr = get_dpu_composer_manager();
	if (!mgr || !(mgr->dpu_comps[DEVICE_COMP_PRIMARY_ID]))
		return;

	dpu_composer = mgr->dpu_comps[DEVICE_COMP_PRIMARY_ID];
	if (is_offline_panel(&dpu_composer->comp.base))
		return;

	display_ctrl = kzalloc(sizeof(*display_ctrl), GFP_KERNEL);
	if (!display_ctrl)
		return;

	/* get the mipi switch interface */
	display_ctrl->display_sw_ops = hybrid_swctrl_init(mipi_switch_handle,
		"display", DISPLAY_CHANNEL);
	if (!display_ctrl->display_sw_ops) {
		LCD_KIT_ERR("init mipi switch failed!\n");
		kfree(display_ctrl);
		display_ctrl = NULL;
		return;
	}

	display_ctrl->dpu_composer = dpu_composer;
	mutex_init(&display_ctrl->hybrid_lock);
	sema_init(&display_ctrl->sr_wake_sema, 1);

	hybrid_gesture_qon_update(GES_QON_SEQ_IDLE);
	sema_init(&display_ctrl->gesture_qon_sema, 1);

	hybrid_parse_dt(&display_ctrl->hybrid_info);

	spin_lock_init(&display_ctrl->comp_on_spin);
	spin_lock_init(&display_ctrl->gesture_quickon_spin);

	if (register_pm_notifier(&g_lcdkit_sr_notifier) < 0)
		LCD_KIT_ERR("register lcd hybrid pm_notifier failed!");

	lcd_kit_hybrid_str_init();
	lcd_kit_hybrid_register_inner_cmd();

	/* get composer info */
	lcd_kit_hybrid_composer_init(dpu_composer);

	/* lcd kit ops */
	memcpy_s(&hybrid_common_ops, sizeof(hybrid_common_ops),
		common_ops, sizeof(hybrid_common_ops));
	/* power on of lpon lp off hooks */
	lcd_kit_set_common_ops();

	if (misc_register(&lcd_kit_hybrid_miscdev) != 0) {
		LCD_KIT_ERR("cannot register miscdev\n");
		kfree(display_ctrl);
		display_ctrl = NULL;
		return;
	}

	lcd_kit_hybrid_task_init();
	lcd_kit_hybrid_msg_trans_init();
	if (lcd_display_msg_init() != LCD_KIT_OK)
		LCD_KIT_ERR("msg init failed\n");

	LCD_KIT_INFO("-\n");
}
