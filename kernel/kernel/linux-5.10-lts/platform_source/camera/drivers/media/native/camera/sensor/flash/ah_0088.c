/*
 * AH_0088.c
 *
 * driver for AH_0088
 *
 * Copyright (c) 2024-2025 Huawei Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "hw_flash.h"
#include <linux/device.h>
#include <linux/pm_wakeup.h>
#include <linux/interrupt.h>
#include <linux/of.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/workqueue.h>
#include "public_flash.h"

/* AH_0088 Registers define */
#define AH_0088_SLAVE_ADDRESS 0x64

#define AH_0088_REG_DEVICE_ID 0x06
#define AH_0088_DEVICE_ID 0x21
#define REG_MODE 0x01

/* LED: back flash / torch; LED2: front IR flash */
#define AH_0088_REG_L1_FLASH 0x03
#define AH_0088_REG_L1_TORCH 0x04
#define REG_FLAGS 0x05

#define AH_0088_MODE_STANDBY 0x00
#define AH_0088_MODE_TORCH 0x02
#define AH_0088_MODE_FLASH 0x03
#define AH_0088_MODE_IR_DRIVER 0x01
#define AH_0088_STROBE_ENABLE (0x01 << 2)
#define AH_0088_STROBE_DISABLE 0x00
#define AH_0088_BOOST_MODEBY_PASS (0x01 << 7)

#define INDUCTOR_CURRENT_LIMMIT 0x80
#define FLASH_TIMEOUT_TIME 0x18 /* bit[4:1] = 1100, 1000ms */

#define AH_0088_TORCH_MAX_CUR 388 /* mA */
#define AH_0088_TORCH_SEPARATE_CUR 101 /* mA */
#define AH_0088_FLASH_MAX_CUR 1500 /* mA */
#define AH_0088_FLASH_SEPARATE_CUR 381 /* mA */

#define AH_0088_FLASH_DEFAULT_CUR_LEV 63 /* 729mA */
#define AH_0088_TORCH_DEFAULT_CUR_LEV 63 /* 178.6mA */

#define FLASH_LED_LEVEL_INVALID 0xff

#define AH_0088_OVER_VOLTAGE_PROTECT 0x02
#define AH_0088_LED_OPEN_SHORT  0x70
#define AH_0088_OVER_TEMP_PROTECT 0x04
#define AH_0088_OVER_CURRENT_PROTECT 0x20

#define REG_FLASH_TIMEOUT 0x02

/* AH_0088 define */
#define AH_0088_STANDBY_MODE_CUR 0
#define WAKE_LOCK_ENABLE 1
#define WAKE_LOCK_DISABLE 0
#define UNDER_VOLTAGE_LOCKOUT 0x04
#define REG_MASK_0XFF 0xFF

/* 1 means I2C control, 0 means GPIO control */
#define AH_0088_CTRL_TYPE_STROBE 0
#define AH_0088_CTRL_TYPE_I2C 1
#define ENABLE_SHORT_PROTECT 0x80
#define AH_0088_TORCH_MAX_VAL_SHIFT 4
#define MAX_LIGHTNESS_PARAM_NUM_RT 3
#define LIGHTNESS_PARAM_FD_INDEX_RT 0
#define LIGHTNESS_PARAM_MODE_INDEX_RT 1
#define LIGHTNESS_PARAM_CURRENT_INDEX_RT 2
#define MAX_FLASH_ID 0xFF /* used to check flash_lightness_sotre */
#define MAX_BRIGHTNESS_FORMMI 0x09

enum AH_0088_current_conf {
	CURRENT_TORCH_LEVEL_MMI_BACK = 0,
	CURRENT_TORCH_LEVEL_RT_BACK = 1,
	CURRENT_MIX_MAX = 2,
};

/* flash mode for single back */
enum AH_0088_single_mode_t {
	SINGLE_STANDBY_MODE = 0x00,
	SINGLE_FLASH_MODE = 0x01,
	SINGLE_TORCH_MODE = 0x02,
	SINGLE_IR_MODE = 0x03,
};

/* ir flash RT self test param */
enum AH_0088_selftest_param {
	IR_SELFTEST_ON_TIME_INDEX = 0,
	IR_SELFTEST_OFF_TIME_INDEX = 1,
	IR_SELFTEST_CURRENT_INDEX = 2,
	IR_SELFTEST_TIMES_INDEX = 3,
	IR_SELFTEST_MAX_INDEX = 4,
};

enum AH_0088_chip_type_t {
	TYPE_MIX,
	AH_0088, /* AH_0088 is 1 */
	TYPE_MAX
};

/* Internal data struct define */
struct hw_AH_0088_private_data_t {
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
	struct wakeup_source AH_0088_wakelock;
#else
	struct wakeup_source *AH_0088_wakelock;
#endif
	unsigned int need_wakelock;
	/* flash electric current config */
	unsigned int ecurrent[CURRENT_MIX_MAX];
	unsigned int selftest_param[IR_SELFTEST_MAX_INDEX];
	unsigned int back_cur;
	enum AH_0088_single_mode_t back_mode;
	enum AH_0088_chip_type_t chip_type;
};

/* Internal varible define */
static struct hw_AH_0088_private_data_t g_AH_0088_pdata;
struct hw_flash_ctrl_t g_AH_0088_ctrl;
define_kernel_flash_mutex(AH_0088);
/* Function define */
static int hw_AH_0088_param_check(char *buf, unsigned long *param,
	int num_of_par);

static int hw_AH_0088_init(struct hw_flash_ctrl_t *flash_ctrl)
{
	struct hw_AH_0088_private_data_t *pdata = NULL;
	int rc = 0;

	if (!flash_ctrl || !flash_ctrl->pdata) {
		cam_err("%s invalid params", __func__);
		return -EINVAL;
	}

	/* init data */
	flash_ctrl->flash_type = FLASH_MIX; /* mix flash */
	flash_ctrl->pctrl = devm_pinctrl_get_select(flash_ctrl->dev,
		PINCTRL_STATE_DEFAULT);
	if (!flash_ctrl->pctrl) {
		cam_err("%s invalid params", __func__);
		return -EINVAL;
	}

	/* stub:check other params which need to init */
	pdata = (struct hw_AH_0088_private_data_t *)flash_ctrl->pdata;
	pdata->back_cur = AH_0088_STANDBY_MODE_CUR; /* init current as 0 */
	pdata->back_mode = SINGLE_STANDBY_MODE;

	mutex_lock(flash_ctrl->hw_flash_mutex);
	flash_ctrl->state.mode = STANDBY_MODE;
	/* set init value 0 */
	flash_ctrl->state.data = AH_0088_STANDBY_MODE_CUR;

	mutex_unlock(flash_ctrl->hw_flash_mutex);

	if (pdata->need_wakelock == WAKE_LOCK_ENABLE) {
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
		wakeup_source_init(&pdata->AH_0088_wakelock, "AH_0088");
#else
		pdata->AH_0088_wakelock = wakeup_source_register(flash_ctrl->dev, "AH_0088");
		if (!pdata->AH_0088_wakelock) {
			rc = -EINVAL;
			cam_err("%s: wakeup source register failed", __func__);
		}
#endif
    }
	return rc;
}

static int hw_AH_0088_exit(struct hw_flash_ctrl_t *flash_ctrl)
{
	cam_debug("%s enter", __func__);

	if (!flash_ctrl || !flash_ctrl->func_tbl || !flash_ctrl->func_tbl->flash_off) {
		cam_err("%s invalid params", __func__);
		return -EINVAL;
	}

	flash_ctrl->func_tbl->flash_off(flash_ctrl);

	flash_ctrl->pctrl = devm_pinctrl_get_select(flash_ctrl->dev,
		PINCTRL_STATE_IDLE);

	return 0;
}

static void report_dsm(void)
{
	if (!dsm_client_ocuppy(client_flash)) {
		dsm_client_record(client_flash, "flash i2c transfer fail\n");
		dsm_client_notify(client_flash, DSM_FLASH_I2C_ERROR_NO);
		cam_warn("[I/DSM] %s dsm_client_notify",
			client_flash->client_name);
	}
}

static void check_fault_and_report_dsm(unsigned char fault_val)
{
	if (fault_val & (AH_0088_OVER_VOLTAGE_PROTECT |
		AH_0088_OVER_CURRENT_PROTECT)) {
		if (!dsm_client_ocuppy(client_flash)) {
			dsm_client_record(client_flash,
				"flash OVP or OCP FlagReg1[0x%x]\n",
				fault_val);
			dsm_client_notify(client_flash,
				DSM_FLASH_OPEN_SHOTR_ERROR_NO);
			cam_warn("[I/DSM] %s dsm_client_notify",
				client_flash->client_name);
		}
	}
	if (fault_val & AH_0088_OVER_TEMP_PROTECT) {
		if (!dsm_client_ocuppy(client_flash)) {
			dsm_client_record(client_flash,
				"flash temperature is too hot FlagReg1[0x%x]\n",
				fault_val);
			dsm_client_notify(client_flash,
				DSM_FLASH_HOT_DIE_ERROR_NO);
			cam_warn("[I/DSM] %s dsm_client_notify",
				client_flash->client_name);
		}
	}
	if (fault_val & UNDER_VOLTAGE_LOCKOUT) {
		if (!dsm_client_ocuppy(client_flash)) {
			dsm_client_record(client_flash,
				"flash UVLO FlagReg1[0x%x]\n",
				fault_val);
			dsm_client_notify(client_flash,
				DSM_FLASH_UNDER_VOLTAGE_LOCKOUT_ERROR_NO);
			cam_warn("[I/DSM] %s dsm_client_notify",
				client_flash->client_name);
		}
	}
}

static unsigned int flash_current_to_level_AH_0088(unsigned int led_cur)
{
	if(led_cur <= AH_0088_FLASH_SEPARATE_CUR && led_cur >= 0) {
		led_cur = ((led_cur * 100) - 1585) / 1198;
	} else if (led_cur <= AH_0088_FLASH_MAX_CUR && led_cur > AH_0088_FLASH_SEPARATE_CUR) {
		led_cur = ((led_cur * 100) - 2036) / 1165;
	} else {
		cam_err("%s invalid level", __func__);
	}

	return led_cur;
}

static unsigned int torch_current_to_level_AH_0088(unsigned int led_cur)
{
	if(led_cur <= AH_0088_TORCH_SEPARATE_CUR && led_cur >= 0) {
		led_cur = ((led_cur * 100) - 421) / 322;
	} else if (led_cur <= AH_0088_TORCH_MAX_CUR && led_cur > AH_0088_TORCH_SEPARATE_CUR) {
		led_cur = ((led_cur * 100) - 924) / 298;
	} else {
		cam_err("%s invalid level", __func__);
	}

	return led_cur;
}

static int hw_AH_0088_flash_mode_cfg_calculate(unsigned char *led_val, unsigned char *reg_mode,
	struct hw_AH_0088_private_data_t *pdata,
	struct hw_flash_cfg_data *cdata)
{
	unsigned int led_cur = 0;
	unsigned int back_cur = 0;
	unsigned int led_cur_calc = 0;

	if (!led_val || !reg_mode || !pdata) {
		cam_err("%s invalid params", __func__);
		return -EINVAL;
	}

	back_cur = pdata->back_cur;

	if (back_cur == FLASH_LED_LEVEL_INVALID)
		back_cur = AH_0088_FLASH_DEFAULT_CUR_LEV;

	/* driver ic: AH_0088 */
	cam_info("%s flash driver ic is %u", __func__, g_AH_0088_pdata.chip_type);

	led_cur = back_cur;
	switch (g_AH_0088_pdata.chip_type) {
	case AH_0088:
		led_cur_calc = flash_current_to_level_AH_0088(led_cur);
		break;
	default:
		led_cur_calc = flash_current_to_level_AH_0088(led_cur);
		break;
	}

	if (led_cur != AH_0088_STANDBY_MODE_CUR) {
		/* using LED only */
		/* LED = flash1 current value ; disable LED2 reg bit */
		*reg_mode =  (AH_0088_MODE_FLASH | AH_0088_BOOST_MODEBY_PASS);
		*led_val = led_cur_calc;
	} else {
		cam_err("%s flash current error LED=%u",__func__, led_cur);
		return -EINVAL;
	}

	return 0;
}

static int hw_AH_0088_torch_mode_cfg_calculate(unsigned char *led_val,
	unsigned char *reg_mode,
	struct hw_AH_0088_private_data_t *pdata)
{
	unsigned int led_cur = 0;
	unsigned int back_cur = 0;
	unsigned int led_cur_calc = 0;

	if (!led_val || !reg_mode || !pdata) {
		cam_err("%s invalid params", __func__);
		return -EINVAL;
	}

	back_cur  = pdata->back_cur;

	if (back_cur == FLASH_LED_LEVEL_INVALID)
		back_cur = AH_0088_TORCH_DEFAULT_CUR_LEV;

	/* driver ic: AH_0088: 1 */
	cam_info("%s flash driver ic is %u", __func__, g_AH_0088_pdata.chip_type);
	/* check hardware menu and check DTS */
	
	led_cur = back_cur;

	switch (g_AH_0088_pdata.chip_type) {
	case AH_0088:
		led_cur_calc = torch_current_to_level_AH_0088(led_cur);
		break;
	default:
		led_cur_calc = torch_current_to_level_AH_0088(led_cur);
		break;
	}

	if (led_cur != AH_0088_STANDBY_MODE_CUR) {
		/* using LED only */
		/* LED = torch1 current Value ; disable LED2 reg bit */
		*reg_mode = (AH_0088_MODE_TORCH | AH_0088_BOOST_MODEBY_PASS);
		*led_val = led_cur_calc;
	} else {
		cam_err("%s flash current error LED=%u",__func__, led_cur);
		return -EINVAL;
	}
	/* touch I2C mode */
	*reg_mode |= AH_0088_STROBE_DISABLE;

	return 0;
}

static int hw_AH_0088_flash_mode_regs_cfg(struct hw_flash_ctrl_t *flash_ctrl,
	unsigned char led_val,
	unsigned char reg_mode)
{
	int rc = 0;
	unsigned char reg_fault_clean = 0;
	struct hw_flash_i2c_fn_t *i2c_func = NULL;
	struct hw_flash_i2c_client *i2c_client = NULL;
	int flash_time = 0;

	if (!flash_ctrl || !flash_ctrl->flash_i2c_client ||
		!flash_ctrl->flash_i2c_client->i2c_func_tbl ||
		!flash_ctrl->flash_i2c_client->i2c_func_tbl->i2c_read ||
		!flash_ctrl->flash_i2c_client->i2c_func_tbl->i2c_write) {
		cam_err("%s invalid params", __func__);
		return -EINVAL;
	}

	i2c_client = flash_ctrl->flash_i2c_client;
	i2c_func = i2c_client->i2c_func_tbl;

	/* clear error status */
	i2c_func->i2c_read(i2c_client, REG_FLAGS, &reg_fault_clean);
	check_fault_and_report_dsm(reg_fault_clean);

	switch (g_AH_0088_pdata.chip_type) {
	case AH_0088:
		flash_time = FLASH_TIMEOUT_TIME;
		break;
	default:
		flash_time = FLASH_TIMEOUT_TIME;
		break;
	}

	rc += i2c_func->i2c_write(i2c_client, REG_FLASH_TIMEOUT, flash_time);
    rc += i2c_func->i2c_write(i2c_client, AH_0088_REG_L1_FLASH, led_val);
	rc += i2c_func->i2c_write(i2c_client, REG_MODE, reg_mode);

	cam_info("%s config flash1_val = 0x%02x, reg_mode = 0x%02x",
		__func__, led_val, reg_mode);

	if (rc < 0)
		report_dsm();

	return rc;
}

static int hw_AH_0088_torch_mode_regs_cfg(struct hw_flash_ctrl_t *flash_ctrl,
	unsigned char led_val,
	unsigned char reg_mode)
{
	int rc = 0;
	unsigned char reg_fault_clean = 0;
	struct hw_flash_i2c_fn_t *i2c_func = NULL;
	struct hw_flash_i2c_client *i2c_client = NULL;

	if (!flash_ctrl || !flash_ctrl->flash_i2c_client ||
		!flash_ctrl->flash_i2c_client->i2c_func_tbl ||
		!flash_ctrl->flash_i2c_client->i2c_func_tbl->i2c_read ||
		!flash_ctrl->flash_i2c_client->i2c_func_tbl->i2c_write) {
		cam_err("%s invalid params", __func__);
		return -EINVAL;
	}

	i2c_client = flash_ctrl->flash_i2c_client;
	i2c_func = i2c_client->i2c_func_tbl;

	/* clear error status */
	i2c_func->i2c_read(i2c_client, REG_FLAGS, &reg_fault_clean);
	check_fault_and_report_dsm(reg_fault_clean);
	rc += i2c_func->i2c_write(i2c_client, AH_0088_REG_L1_TORCH, led_val);
	rc += i2c_func->i2c_write(i2c_client, REG_MODE, reg_mode);
	cam_info("%s config torch1_val = 0x%02x, reg_mode = 0x%02x",__func__, led_val, reg_mode);

	if (rc < 0)
		report_dsm();

	return rc;
}

static int hw_AH_0088_action_ic_standby(struct hw_flash_ctrl_t *flash_ctrl)
{
	int rc = 0;
	unsigned char reg_fault_clean = 0;
	struct hw_flash_i2c_fn_t *i2c_func = NULL;
	struct hw_flash_i2c_client *i2c_client = NULL;
	struct hw_AH_0088_private_data_t *pdata = NULL;

	if (!flash_ctrl || !flash_ctrl->pdata ||
		!flash_ctrl->flash_i2c_client ||
		!flash_ctrl->flash_i2c_client->i2c_func_tbl ||
		!flash_ctrl->flash_i2c_client->i2c_func_tbl->i2c_read ||
		!flash_ctrl->flash_i2c_client->i2c_func_tbl->i2c_write) {
		cam_err("%s invalid params", __func__);
		return -EINVAL;
	}

	pdata = (struct hw_AH_0088_private_data_t *)flash_ctrl->pdata;
	i2c_client = flash_ctrl->flash_i2c_client;
	i2c_func = i2c_client->i2c_func_tbl;

	/* read back error status */
	i2c_func->i2c_read(i2c_client, REG_FLAGS, &reg_fault_clean);
	check_fault_and_report_dsm(reg_fault_clean);

	rc += i2c_func->i2c_write(i2c_client, REG_MODE, AH_0088_MODE_STANDBY);
	if (rc < 0)
		report_dsm();

	if (pdata->need_wakelock == WAKE_LOCK_ENABLE) {
		/*
		 * wake_unlock() will check
		 * whether the wakeup source is active before
		 * doing the unlock operation,
		 * so there is no dangers for the mutex being
		 * unlocked before locked.
		 */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
		__pm_relax(&pdata->AH_0088_wakelock);
#else
		__pm_relax(pdata->AH_0088_wakelock);
#endif
	}
	return rc;
}

static int hw_AH_0088_action_ic_flash_mode(struct hw_flash_ctrl_t *flash_ctrl,
	struct hw_flash_cfg_data *cdata)
{
	int rc;
	unsigned char reg_mode = 0;
	unsigned char led_val = 0;
	struct hw_AH_0088_private_data_t *pdata = NULL;

	if (!flash_ctrl || !flash_ctrl->pdata) {
		cam_err("%s invalid params", __func__);
		return -EINVAL;
	}

	pdata = (struct hw_AH_0088_private_data_t *)flash_ctrl->pdata;

	rc = hw_AH_0088_flash_mode_cfg_calculate(&led_val,
		&reg_mode, pdata, cdata);
	if (rc < 0) {
		cam_err("%s flash mode cfg calculate faild", __func__);
		return rc;
	}

	rc = hw_AH_0088_flash_mode_regs_cfg(flash_ctrl, led_val, reg_mode);
	if (rc < 0) {
		cam_err("%s flash mode regs cfg faild", __func__);
		return rc;
	}

	return rc;
}

static int hw_AH_0088_action_ic_torch_mode(struct hw_flash_ctrl_t *flash_ctrl)
{
	int rc;
	unsigned char led_val = 0;
	unsigned char reg_mode = 0;
	struct hw_AH_0088_private_data_t *pdata = NULL;

	if (!flash_ctrl || !flash_ctrl->pdata) {
		cam_err("%s invalid params", __func__);
		return -EINVAL;
	}

	pdata = (struct hw_AH_0088_private_data_t *)flash_ctrl->pdata;

	rc = hw_AH_0088_torch_mode_cfg_calculate(&led_val, &reg_mode, pdata);
	if (rc < 0) {
		cam_err("%s torch mode cfg calculate faild", __func__);
		return rc;
	}

	rc = hw_AH_0088_torch_mode_regs_cfg(flash_ctrl, led_val, reg_mode);
	if (rc < 0) {
		cam_err("%s torch mode reg cfg faild", __func__);
		return rc;
	}

	return rc;
}

static int hw_AH_0088_update_inner_status(struct hw_flash_ctrl_t *flash_ctrl,
	struct hw_flash_cfg_data *cdata)
{
	struct hw_AH_0088_private_data_t *pdata = NULL;
	enum AH_0088_single_mode_t inner_mode = SINGLE_STANDBY_MODE;
	unsigned int cur = 0;

	if (!flash_ctrl || !cdata || !flash_ctrl->pdata) {
		cam_err("%s invalid params", __func__);
		return -EINVAL;
	}

	pdata = (struct hw_AH_0088_private_data_t *)flash_ctrl->pdata;
	cur = (unsigned int)(cdata->data);

	switch (cdata->mode) {
	case STANDBY_MODE:
		inner_mode = SINGLE_STANDBY_MODE;
		/* flash_ctrl->state will be update */
		/* when ic standby mode cfg is finished */
		break;
	case FLASH_MODE:
	case FLASH_STROBE_MODE:
		inner_mode = SINGLE_FLASH_MODE;
		/* flash_ctrl->state will be update here */
		/* and may be used in thermal protect */
		flash_ctrl->state.mode = cdata->mode;
		flash_ctrl->state.data = cdata->data;
		break;
	default:
		inner_mode = SINGLE_TORCH_MODE; /* set as default */
		/* flash_ctrl->state will be update here */
		/* and may be used in thermal protect */
		flash_ctrl->state.mode = cdata->mode;
		flash_ctrl->state.data = cdata->data;
		break;
	}

	pdata->back_mode  = inner_mode;
	pdata->back_cur   = cur;
	cam_info("%s update back mode:%d, back cur %u",  __func__,
		pdata->back_mode, pdata->back_cur);

	return 0;
}

static int hw_AH_0088_get_work_mode_strategy(struct hw_flash_ctrl_t *flash_ctrl,
	enum AH_0088_single_mode_t *ic_work_mode)
{
	struct hw_AH_0088_private_data_t *pdata = NULL;
	unsigned int back_mode  = SINGLE_STANDBY_MODE;

	if (!flash_ctrl || !ic_work_mode || !flash_ctrl->pdata) {
		cam_err("%s invalid params", __func__);
		return -EINVAL;
	}

	pdata = (struct hw_AH_0088_private_data_t *)flash_ctrl->pdata;
	back_mode  = pdata->back_mode;

	*ic_work_mode = back_mode; /* set as default */

	return 0;
}

static int hw_AH_0088_update_work_mode_dispatch(struct hw_flash_ctrl_t *flash_ctrl,
	enum AH_0088_single_mode_t ic_work_mode,
	struct hw_flash_cfg_data *cdata)
{
	int rc = 0;
	struct hw_AH_0088_private_data_t *pdata = NULL;

	if (!flash_ctrl || !flash_ctrl->pdata) {
		cam_err("%s invalid params", __func__);
		return -EINVAL;
	}

	pdata = (struct hw_AH_0088_private_data_t *)flash_ctrl->pdata;

	switch (ic_work_mode) {
	case SINGLE_STANDBY_MODE:
		rc = hw_AH_0088_action_ic_standby(flash_ctrl);
		flash_ctrl->state.mode = STANDBY_MODE;
		flash_ctrl->state.data =
				AH_0088_STANDBY_MODE_CUR;
		break;
	case SINGLE_FLASH_MODE:
		rc = hw_AH_0088_action_ic_flash_mode(flash_ctrl,
			cdata);
		break;
	case SINGLE_TORCH_MODE:
		rc = hw_AH_0088_action_ic_torch_mode(flash_ctrl);
		break;
	default:
		cam_err("%s invalid ic work mode", __func__);
		rc = -EINVAL;
		break;
	}

	return rc;
}

static int hw_AH_0088_update_work_mode(struct hw_flash_ctrl_t *flash_ctrl,
	struct hw_flash_cfg_data *cdata)
{
	int rc;
	enum AH_0088_single_mode_t ic_work_mode = SINGLE_STANDBY_MODE;

	if (!flash_ctrl || !cdata) {
		cam_err("%s invalid params", __func__);
		return -EINVAL;
	}

	rc = hw_AH_0088_update_inner_status(flash_ctrl, cdata);
	if (rc < 0) {
		cam_err("%s update inner status failed", __func__);
		goto work_mode_err;
	}

	rc = hw_AH_0088_get_work_mode_strategy(flash_ctrl,
		&ic_work_mode);
	if (rc < 0) {
		cam_err("%s get work mode strategy failed", __func__);
		goto work_mode_err;
	}

	rc = hw_AH_0088_update_work_mode_dispatch(flash_ctrl,
		ic_work_mode, cdata);
	if (rc < 0) {
		cam_err("%s update work mode dispatch failed", __func__);
		goto work_mode_err;
	}

	return rc;

work_mode_err:
	/* error handler may need to add here */
	return rc;
}

static int hw_AH_0088_on(struct hw_flash_ctrl_t *flash_ctrl, void *data)
{
	struct hw_AH_0088_private_data_t *pdata = NULL;
	struct hw_flash_cfg_data *cdata = NULL;
	int rc = 0;

	if (!data || !flash_ctrl || !flash_ctrl->pdata) {
		cam_err("%s invalid params", __func__);
		return -EINVAL;
	}

	cdata = (struct hw_flash_cfg_data *)data;
	pdata = (struct hw_AH_0088_private_data_t *)flash_ctrl->pdata;

	cam_info("%s mode=%d, cur=%d", __func__, cdata->mode, cdata->data);

	mutex_lock(flash_ctrl->hw_flash_mutex);

	if (pdata->need_wakelock == WAKE_LOCK_ENABLE) {
		/*
		 * if the wake lock source has already been active,
		 * repeat call of
		 * 'wake_lock' will just add the event_count
		 * of the wake lock source,
		 * and will not cause evil effects.
		 */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
		__pm_stay_awake(&pdata->AH_0088_wakelock);
#else
		__pm_stay_awake(pdata->AH_0088_wakelock);
#endif
	}
	rc = hw_AH_0088_update_work_mode(flash_ctrl, cdata);

	mutex_unlock(flash_ctrl->hw_flash_mutex);

	return rc;
}

static int hw_AH_0088_off(struct hw_flash_ctrl_t *flash_ctrl)
{
	struct hw_flash_cfg_data cdata = {0};
	struct hw_AH_0088_private_data_t *pdata = NULL;
	int rc;

	if (!flash_ctrl || !flash_ctrl->pdata) {
		cam_err("%s invalid params", __func__);
		return -EINVAL;
	}

	cam_debug("%s enter", __func__);

	pdata = (struct hw_AH_0088_private_data_t *)flash_ctrl->pdata;
	cdata.mode = (flash_mode)SINGLE_STANDBY_MODE;
	cdata.data = AH_0088_STANDBY_MODE_CUR; /* set standby current as 0 */

	mutex_lock(flash_ctrl->hw_flash_mutex);
	rc = hw_AH_0088_update_work_mode(flash_ctrl, &cdata);
	mutex_unlock(flash_ctrl->hw_flash_mutex);

	cam_info("%s end", __func__);

	return rc;
}

static int hw_AH_0088_get_dt_data(struct hw_flash_ctrl_t *flash_ctrl)
{
	struct hw_AH_0088_private_data_t *pdata = NULL;
	struct device_node *node = NULL;
	int i;
	int rc = -EINVAL;

	if (!flash_ctrl || !flash_ctrl->pdata || !flash_ctrl->dev ||
		!flash_ctrl->dev->of_node) {
		cam_err("%s invalid params", __func__);
		return -EINVAL;
	}

	pdata = (struct hw_AH_0088_private_data_t *)flash_ctrl->pdata;
	node = flash_ctrl->dev->of_node;

	rc = of_property_read_u32(node, "vendor,need-wakelock",
		(u32 *)&pdata->need_wakelock);
	cam_info("%s vendor,need-wakelock %d, rc %d\n", __func__,
		pdata->need_wakelock, rc);
	if (rc < 0) {
		pdata->need_wakelock = WAKE_LOCK_DISABLE;
		cam_err("%s failed %d\n", __func__, __LINE__);
		return rc;
	}

	rc = of_property_read_u32_array(node, "vendor,flash-current",
		pdata->ecurrent, CURRENT_MIX_MAX);
	if (rc < 0) {
		cam_err("%s read flash-current failed line %d\n",
			__func__, __LINE__);
		return rc;
	}
	for (i = 0; i < CURRENT_MIX_MAX; i++)
		cam_info("%s ecurrent[%d]=%d\n", __func__,
			i, pdata->ecurrent[i]);

	rc = of_property_read_u32_array(node, "vendor,selftest-param",
		pdata->selftest_param, IR_SELFTEST_MAX_INDEX);
	if (rc < 0) {
		cam_err("%s read selftest-param failed line %d\n",
			__func__, __LINE__);
		return rc;
	}
	for (i = 0; i < IR_SELFTEST_MAX_INDEX; i++)
		cam_info("%s selftest_param[%d]=%d\n", __func__, i,
			pdata->selftest_param[i]);

	return rc;
}

static ssize_t hw_AH_0088_flash_lightness_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int rc;

	if (!buf) {
		cam_err("%s invalid params", __func__);
		return -EINVAL;
	}

	/* PAGE_SIZE = 4096 */
	rc = scnprintf(buf, PAGE_SIZE, "back_mode=%d, data=%d\n",
		g_AH_0088_pdata.back_mode,
		g_AH_0088_pdata.back_cur);

	return rc;
}

static int hw_AH_0088_param_check(char *buf, unsigned long *param,
	int num_of_par)
{
	char *token = NULL;
	unsigned int base = 0;
	int cnt;

	if (!buf || !param) {
		cam_err("%s invalid params", __func__);
		return -EINVAL;
	}

	token = strsep(&buf, " ");

	for (cnt = 0; cnt < num_of_par; cnt++) {
		if (token) {
			/* add for check token[1] has valid element */
			if (strlen(token) < 2) {
				base = 10; /* decimal */
			} else {
				/* format 0x** */
				if ((token[1] == 'x') || (token[1] == 'X'))
					base = 16; /* hex */
				else
					base = 10; /* decimal */
			}
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
			if (strict_strtoul(token, base, &param[cnt]) != 0)
#else
			if (kstrtoul(token, base, &param[cnt]) != 0)
#endif
				return -EINVAL;
			token = strsep(&buf, " ");
		} else {
			cam_info("%s the %d param is null\n",
				__func__, num_of_par);
		}
	}

	return 0;
}

static ssize_t hw_AH_0088_flash_lightness_store_imp(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct hw_flash_cfg_data cdata = {0};
	unsigned long param[MAX_LIGHTNESS_PARAM_NUM_RT] = {0};
	int rc;
	int flash_id = 0;

	if (!buf) {
		cam_err("%s invalid params", __func__);
		return -EINVAL;
	}

	rc = hw_AH_0088_param_check((char *)buf, param,
		MAX_LIGHTNESS_PARAM_NUM_RT);
	if (rc < 0) {
		cam_err("%s failed to check param", __func__);
		return rc;
	}

	/* 0 - flash id */
	flash_id = (int)param[LIGHTNESS_PARAM_FD_INDEX_RT];
	/* 1 - mode */
	cdata.mode = (int)param[LIGHTNESS_PARAM_MODE_INDEX_RT];
	/* 2 - current */
	cdata.data = (int)param[LIGHTNESS_PARAM_CURRENT_INDEX_RT];
	cam_info("%s flash_id=%d, cdata.mode=%d, cdata.data=%d", __func__,
		flash_id, cdata.mode, cdata.data);

	if ((cdata.mode < STANDBY_MODE) || (cdata.mode > MAX_MODE)) {
		cam_err("%s cdata.mode out of bounds", __func__);
		return -EINVAL;
	}
	/* 0~1500 mA */
	if ((cdata.data < 0) || (cdata.data > AH_0088_FLASH_MAX_CUR)) {
		cam_err("%s cdata.data out of bounds", __func__);
		return -EINVAL;
	}
	/* 0~0xFF, pass from HAL, but not use just print */
	if ((flash_id < 0) || (flash_id > MAX_FLASH_ID)) {
		cam_err("%s flash_id out of bounds", __func__);
		return -EINVAL;
	}

	if (cdata.mode == STANDBY_MODE) {
		rc = hw_AH_0088_off(&g_AH_0088_ctrl);
		if (rc < 0) {
			cam_err("%s flash off error", __func__);
			return rc;
		}
	} else if (cdata.mode == TORCH_MODE) {
		cdata.data = g_AH_0088_pdata.ecurrent[CURRENT_TORCH_LEVEL_RT_BACK];

		cam_info("%s mode=%d, current=%d", __func__,
			cdata.mode, cdata.data);

		rc = hw_AH_0088_on(&g_AH_0088_ctrl, &cdata);
		if (rc < 0) {
			cam_err("%s flash on error", __func__);
			return rc;
		}
	} else if (cdata.mode == FLASH_MODE) {
		cam_info("%s mode=%d, current=%d", __func__,
			cdata.mode, cdata.data);
		rc = hw_AH_0088_on(&g_AH_0088_ctrl, &cdata);
		if (rc < 0) {
			cam_err("%s flash on error", __func__);
			return rc;
		}
	} else {
		cam_err("%s wrong mode=%d", __func__, cdata.mode);
		return -EINVAL;
	}

	return count;
}

static ssize_t hw_AH_0088_flash_lightness_store(struct device *dev,
	struct device_attribute *attr,
	const char *buf,
	size_t count)
{
	/* call back flash */
	g_AH_0088_ctrl.mix_pos = HWFLASH_POSITION_REAR;
	return hw_AH_0088_flash_lightness_store_imp(dev, attr,
		buf, count);
}


static ssize_t hw_AH_0088_flash_mask_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int rc;

	if (!buf) {
		cam_err("%s invalid params", __func__);
		return -EINVAL;
	}

	rc = scnprintf(buf, PAGE_SIZE, "flash_mask_disabled=%d\n",
		g_AH_0088_ctrl.flash_mask_enable);

	return rc;
}

static ssize_t hw_AH_0088_flash_mask_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	if (!buf) {
		cam_err("%s invalid params", __func__);
		return -EINVAL;
	}

	if (buf[0] == '0') /* char '0' for mask disable */
		g_AH_0088_ctrl.flash_mask_enable = false;
	else
		g_AH_0088_ctrl.flash_mask_enable = true;

	cam_info("%s flash_mask_enable=%d", __func__,
		g_AH_0088_ctrl.flash_mask_enable);
	return (ssize_t)count;
}

static void hw_AH_0088_torch_brightness_set_imp(struct led_classdev *cdev,
	enum led_brightness brightness)
{
	int rc = 0;
	struct hw_flash_cfg_data cdata = {0};
	unsigned int led_bright = brightness;

	cam_info("%s brightness= %d", __func__, brightness);

	if (led_bright == LED_OFF) {
		rc = hw_AH_0088_off(&g_AH_0088_ctrl);
		if (rc < 0) {
			cam_err("%s pmu_led off error", __func__);
			return;
		}
	} else {
		cdata.mode = TORCH_MODE;
		
		cdata.data = g_AH_0088_pdata.ecurrent[CURRENT_TORCH_LEVEL_MMI_BACK];

		cam_info("%s brightness=0x%x, mode=%d, data=%d", __func__,
			brightness, cdata.mode, cdata.data);
		rc = hw_AH_0088_on(&g_AH_0088_ctrl, &cdata);
		if (rc < 0) {
			cam_err("%s flash on error", __func__);
			return;
		}
	}
}

static void hw_AH_0088_torch_brightness_set(struct led_classdev *cdev,
	enum led_brightness brightness)
{
	g_AH_0088_ctrl.mix_pos = HWFLASH_POSITION_REAR;
	hw_AH_0088_torch_brightness_set_imp(cdev, brightness);
}

/* for RT */
static struct device_attribute g_AH_0088_flash_lightness =
	__ATTR(flash_lightness, 0660, hw_AH_0088_flash_lightness_show,
	hw_AH_0088_flash_lightness_store); /* 660:-wr-wr--- */

static struct device_attribute g_AH_0088_flash_mask =
	__ATTR(flash_mask, 0660, hw_AH_0088_flash_mask_show,
	hw_AH_0088_flash_mask_store); /* 660:-wr-wr--- */

static int hw_AH_0088_register_attribute(struct hw_flash_ctrl_t *flash_ctrl,
	struct device *dev)
{
	int rc;

	if (!flash_ctrl || !dev) {
		cam_err("%s invalid params", __func__);
		return -EINVAL;
	}

	register_camerafs_attr(&g_AH_0088_flash_lightness);

	flash_ctrl->cdev_torch.name = "torch";
	flash_ctrl->cdev_torch.max_brightness =
		(enum led_brightness)MAX_BRIGHTNESS_FORMMI;
	flash_ctrl->cdev_torch.brightness_set =
		hw_AH_0088_torch_brightness_set;
	rc = led_classdev_register((struct device *)dev,
		&flash_ctrl->cdev_torch);
	if (rc < 0) {
		cam_err("%s failed to register torch classdev", __func__);
		goto err_out;
	}

	rc = device_create_file(dev, &g_AH_0088_flash_lightness);
	if (rc < 0) {
		cam_err("%s failed to creat flash_lightness attribute",
			__func__);
		goto err_create_flash_lightness_file;
	}

	rc = device_create_file(dev, &g_AH_0088_flash_mask);
	if (rc < 0) {
		cam_err("%s failed to creat flash_mask attribute", __func__);
		goto err_create_flash_mask_file;
	}

	return 0;

err_create_flash_mask_file:
	device_remove_file(dev, &g_AH_0088_flash_lightness);
err_create_flash_lightness_file:
	led_classdev_unregister(&flash_ctrl->cdev_torch1);

err_out:
	return rc;
}

static int hw_AH_0088_match(struct hw_flash_ctrl_t *flash_ctrl)
{
	struct hw_flash_i2c_client *i2c_client = NULL;
	struct hw_flash_i2c_fn_t *i2c_func = NULL;
	struct hw_AH_0088_private_data_t *pdata = NULL;
	unsigned char AH_0088_chip_id = 0;
	unsigned char AH_0088_device_id = 0;

	if (!flash_ctrl || !flash_ctrl->pdata ||
		!flash_ctrl->flash_i2c_client ||
		!flash_ctrl->flash_i2c_client->i2c_func_tbl ||
		!flash_ctrl->flash_i2c_client->i2c_func_tbl->i2c_read) {
		cam_err("%s invalid params", __func__);
		return -EINVAL;
	}

	i2c_client = flash_ctrl->flash_i2c_client;

	/* change to AH_0088 actual slave address 0x64 */
	i2c_client->client->addr = AH_0088_SLAVE_ADDRESS;
	i2c_func = i2c_client->i2c_func_tbl;
	pdata = (struct hw_AH_0088_private_data_t *)flash_ctrl->pdata;

	i2c_func->i2c_read(i2c_client, AH_0088_REG_DEVICE_ID, &AH_0088_device_id);
	if (AH_0088_device_id == AH_0088_DEVICE_ID) 
	{
		g_AH_0088_pdata.chip_type = AH_0088;
		cam_info("%s chip_id 0x%x chip type is AH_0088", __func__, AH_0088_device_id);
	} else {
		cam_err("%s ah0088 match error, device id 0x%x!= 0x%x", __func__, AH_0088_device_id, AH_0088_DEVICE_ID);
		return -EINVAL;
	}
	
	return 0;
}

static int hw_AH_0088_remove(struct i2c_client *client)
{
	cam_debug("%s enter", __func__);
	if (!client) {
		cam_err("%s client is NULL", __func__);
		return -EINVAL;
	}
	g_AH_0088_ctrl.func_tbl->flash_exit(&g_AH_0088_ctrl);

	client->adapter = NULL;
	return 0;
}

static const struct i2c_device_id g_AH_0088_id[] = {
	{ "ah_0088", (unsigned long)&g_AH_0088_ctrl },
	{}
};

static const struct of_device_id g_AH_0088_dt_match[] = {
	{ .compatible = "vendor,ah_0088" },
	{}
};
MODULE_DEVICE_TABLE(of, AH_0088_dt_match);

static struct i2c_driver g_AH_0088_i2c_driver = {
	.probe = hw_flash_i2c_probe,
	.remove = hw_AH_0088_remove,
	.id_table = g_AH_0088_id,
	.driver = {
		.name = "hw_ah_0088",
		.of_match_table = g_AH_0088_dt_match,
	},
};

static int __init hw_AH_0088_module_init(void)
{
	cam_info("%s erter", __func__);
	return i2c_add_driver(&g_AH_0088_i2c_driver);
}

static void __exit hw_AH_0088_module_exit(void)
{
	cam_info("%s enter", __func__);
	i2c_del_driver(&g_AH_0088_i2c_driver);
}

static struct hw_flash_i2c_client g_AH_0088_i2c_client;

static struct hw_flash_fn_t g_AH_0088_func_tbl = {
	.flash_config = hw_flash_config,
	.flash_init = hw_AH_0088_init,
	.flash_exit = hw_AH_0088_exit,
	.flash_on = hw_AH_0088_on,
	.flash_off = hw_AH_0088_off,
	.flash_match = hw_AH_0088_match,
	.flash_get_dt_data = hw_AH_0088_get_dt_data,
	.flash_register_attribute = hw_AH_0088_register_attribute,
};

struct hw_flash_ctrl_t g_AH_0088_ctrl = {
	.flash_i2c_client = &g_AH_0088_i2c_client,
	.func_tbl = &g_AH_0088_func_tbl,
	.hw_flash_mutex = &flash_mut_AH_0088,
	.pdata = (void *)&g_AH_0088_pdata,
	.flash_mask_enable = true,
	.state = {
		.mode = STANDBY_MODE,
	},
};

module_init(hw_AH_0088_module_init);
module_exit(hw_AH_0088_module_exit);
MODULE_DESCRIPTION("AH_0088 FLASH");
MODULE_LICENSE("GPL v2");
