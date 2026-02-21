/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Device driver for schargerV800 flash
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/err.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/delay.h>
#include <linux/regmap.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>

#include <platform_include/basicplatform/linux/power/platform/coul/coul_drv.h>
#include <securec.h>
#ifdef CONFIG_BOOST_5V
#include <chipset_common/hwpower/hardware_ic/boost_5v.h>
#endif
#include "scharger_v800_regs.h"
#include "scharger_v800_buck.h"
#include "scharger_v800_flash_mix.h"

#define scharger_flash_err(fmt, args...)      pr_err("[schargerV800_flash]" fmt, ## args)
#define scharger_flash_evt(fmt, args...)      pr_warn("[schargerV800_flash]" fmt, ## args)
#define scharger_flash_inf(fmt, args...)      pr_info("[schargerV800_flash]" fmt, ## args)
#define scharger_flash_dbg(fmt, args...)      pr_debug("[schargerV800_flash]" fmt, ## args)

#define FLASH_MODE_VBAT_THRE 3650
#define FLASH_LED_LEVEL_INVALID 0xff

#define SCHARGERV800_TORCH_MAX_CUR 384
#define SCHARGERV800_TORCH_MIN_CUR 3
#define SCHARGERV800_TORCH_CUR_STEP 3
#define SCHARGERV800_FLASH_DEFAULT_CUR 384
#define SCHARGERV800_FLASH_DEFAULT_CUR_LEV 0x7F
#define SCHARGERV800_STANDBY_MODE_CUR 0
#define SCHARGERV800_TORCH_LED_VAL_MASK 0
#define MAX_BRIGHTNESS_FORMMI 0x09
#define MAX_LIGHTNESS_PARAM_NUM_RT 3
#define LIGHTNESS_PARAM_FD_INDEX_RT 0
#define LIGHTNESS_PARAM_MODE_INDEX_RT 1
#define LIGHTNESS_PARAM_CURRENT_INDEX_RT 2
#define MAX_FLASH_ID 0xFF

#define WAKE_LOCK_ENABLE 1
#define WAKE_LOCK_DISABLE 0

#define BACK_FLASH_USE_LED2 1

#define LED_EN TRUE
#define LED_DIS FALSE

define_kernel_flash_mutex(scharger_v800);

static struct opt_regs torch_pass_through_mode_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg(0x931, 0xff, 0, 0x56),
	reg_cfg(0x932, 0xff, 0, 0xD1),
	reg_cfg(0x9C5, 0x01, 0, 0x01),
	reg_cfg(0x958, 0xff, 0, 0xA9),
};

static struct opt_regs torch_boost_mode_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg(0x930, 0xff, 0, 0x02),
	reg_cfg(0x931, 0xff, 0, 0xEA),
	reg_cfg(0x932, 0xff, 0, 0x7C),
	reg_cfg(0x970, 0xff, 0, 0x0E),
	reg_cfg(0x96A, 0xff, 0, 0x0C),
	reg_cfg(0x966, 0xff, 0, 0x0D),
	reg_cfg(0x9B2, 0xff, 0, 0x08),
	reg_cfg(0x9BD, 0xff, 0, 0x10),
	reg_cfg(0x9E8, 0xff, 0, 0x02),
	reg_cfg(0x935, 0xff, 0, 0x04),
	reg_cfg(0x374, 0xff, 0, 0x13),
	reg_cfg(0x375, 0xff, 0, 0x1B),
	reg_cfg(0x376, 0xff, 0, 0x61),
	reg_cfg(0x9A3, 0xff, 0, 0x09),
	reg_cfg(0x9A2, 0xff, 0, 0x0A),
	reg_cfg(0x9A0, 0xff, 0, 0x0A),
	reg_cfg(0x946, 0xff, 0, 0x20),
	reg_cfg(0x972, 0xff, 0, 0x05),
	reg_cfg(0x957, 0xff, 0, 0x84),
	reg_cfg(0x96C, 0xff, 0, 0x06),
	reg_cfg(0x335, 0xff, 0, 0x01),
};

static struct opt_regs external_boost_mode_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg(0x931, 0xff, 0, 0x46),
	reg_cfg(0x932, 0xff, 0, 0xDF),
	reg_cfg(0x9C5, 0x01, 0, 0x01),
	reg_cfg(0x958, 0xff, 0, 0xA9),
};

static struct opt_regs torch_pass_through_mode_exit_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg(0x319, 0xff, 0, 0x00),
	reg_cfg(0x1001, 0xff, 0, 0x00),
	reg_cfg(0x1006, 0xff, 0, 0x7F),
	reg_cfg(0x1007, 0xff, 0, 0x7F),
	reg_cfg(0x1000, 0xff, 0, 0x00),
	reg_cfg(0x931, 0xff, 0, 0x00),
	reg_cfg(0x932, 0xff, 0, 0x00),
	reg_cfg(0x9C5, 0x01, 0, 0x00),
	reg_cfg(0x958, 0xff, 0, 0xAD),
};

static struct opt_regs torch_boost_mode_exit_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg(0x319, 0xff, 0, 0x00),
	reg_cfg(0x1001, 0xff, 0, 0x00),
	reg_cfg(0x1006, 0xff, 0, 0x7F),
	reg_cfg(0x1007, 0xff, 0, 0x7F),
	reg_cfg(0x1000, 0xff, 0, 0x00),
	reg_cfg(0x335, 0xff, 0, 0x00),
	reg_cfg(0x931, 0xff, 0, 0x00),
	reg_cfg(0x932, 0xff, 0, 0x00),
	reg_cfg(0x9BE, 0xff, 0, 0x00),
	reg_cfg(0x1009, 0xff, 0, 0x11),
	reg_cfg(0x946, 0xff, 0, 0x00),
	reg_cfg(0x963, 0xff, 0, 0x2A),
	reg_cfg(0x970, 0xff, 0, 0x06),
	reg_cfg(0x96A, 0xff, 0, 0x00),
	reg_cfg(0x966, 0xff, 0, 0x09),
	reg_cfg(0x935, 0xff, 0, 0x00),
	reg_cfg(0x930, 0xff, 0, 0x02),
	reg_cfg(0x9B2, 0xff, 0, 0x00),
	reg_cfg(0x9BD, 0xff, 0, 0x00),
	reg_cfg(0x9E8, 0xff, 0, 0x00),
	reg_cfg(0x374, 0xff, 0, 0x04),
	reg_cfg(0x375, 0xff, 0, 0x1C),
	reg_cfg(0x376, 0xff, 0, 0xB1),
	reg_cfg(0x9A3, 0xff, 0, 0x00),
	reg_cfg(0x9A2, 0xff, 0, 0x00),
	reg_cfg(0x9A0, 0xff, 0, 0x00),
	reg_cfg(0x946, 0xff, 0, 0x00),
	reg_cfg(0x972, 0xff, 0, 0x08),
	reg_cfg(0x96C, 0xff, 0, 0x16),
};

static struct opt_regs external_boost_mode_exit_opt_regs[] = {
	/* reg, mask, shift, val */
	reg_cfg(0x319, 0xff, 0, 0x00),
	reg_cfg(0x1001, 0xff, 0, 0x00),
	reg_cfg(0x1006, 0xff, 0, 0x7F),
	reg_cfg(0x1007, 0xff, 0, 0x7F),
	reg_cfg(0x1000, 0xff, 0, 0x00),
	reg_cfg(0x931, 0xff, 0, 0x00),
	reg_cfg(0x932, 0xff, 0, 0x00),
	reg_cfg(0x9C5, 0x01, 0, 0x00),
	reg_cfg(0x958, 0xff, 0, 0xAD),
};

static struct scharger_v800_private_data_t g_scharger_v800_pdata;
static struct hw_flash_ctrl_t g_scharger_v800_ctrl;

static int opt_regs_set(struct scharger_v800_private_data_t *pdata,
	struct opt_regs *opt, unsigned int len)
{
	unsigned int i;
	int ret = 0;

	for (i = 0; i < len; i++)
		ret += regmap_update_bits(pdata->regmap, opt[i].reg, opt[i].mask,
			 opt[i].val << opt[i].shift);

	return ret;
}

static int scharger_v800_flash_init(struct hw_flash_ctrl_t *flash_ctrl)
{
	struct scharger_v800_private_data_t *pdata = NULL;

	scharger_flash_inf("%s enter\n", __func__);
	if (!flash_ctrl || !flash_ctrl->pdata) {
		scharger_flash_err("%s flash_ctrl is NULL\n", __func__);
		return -EINVAL;
	}

	/* init data */
	flash_ctrl->flash_type = FLASH_MIX; /* mix flash */

	/* stub:check other params which need to init */
	pdata = (struct scharger_v800_private_data_t *)flash_ctrl->pdata;
	pdata->front_cur = SCHARGERV800_STANDBY_MODE_CUR; /* init current as 0 */
	pdata->back_cur = SCHARGERV800_STANDBY_MODE_CUR; /* init current as 0 */
	pdata->front_mode = SINGLE_STANDBY_MODE;
	pdata->back_mode = SINGLE_STANDBY_MODE;
	pdata->entire_ic_mode = ENTIRE_IC_STANDBY_MODE;

	mutex_lock(flash_ctrl->hw_flash_mutex);
	flash_ctrl->state.mode = STANDBY_MODE;
	/* set init value 0 */
	flash_ctrl->state.data = SCHARGERV800_STANDBY_MODE_CUR;
	mutex_unlock(flash_ctrl->hw_flash_mutex);

	if (pdata->need_wakelock == WAKE_LOCK_ENABLE) {
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
		wakeup_source_init(&pdata->scharger_v800_flash_wakelock, "scharger_v800_flash");
#else
		pdata->scharger_v800_flash_wakelock = wakeup_source_register(flash_ctrl->dev, "scharger_v800_flash");
		if (!pdata->scharger_v800_flash_wakelock) {
			scharger_flash_err("%s: wakeup source register failed\n", __func__);
			return -EINVAL;
		}
#endif
	}

	return 0;
}

static int scharger_v800_flash_exit(struct hw_flash_ctrl_t *flash_ctrl)
{
	scharger_flash_inf("%s enter\n", __func__);
	if (!flash_ctrl || !flash_ctrl->func_tbl ||
		!flash_ctrl->func_tbl->flash_off) {
		scharger_flash_err("%s flash_ctrl is NULL\n", __func__);
		return -EINVAL;
	}

	flash_ctrl->func_tbl->flash_off(flash_ctrl);
	return 0;
}

enum scharger_v800_entire_ic_mode_t scharger_v800_flash_state(void)
{
	return g_scharger_v800_pdata.entire_ic_mode;
}

int scharger_v800_flash_set_led_en(unsigned int reg_val)
{
	if (g_scharger_v800_pdata.regmap == NULL) {
		scharger_flash_err("%s regmap is NULL\n", __func__);
		return -1;
	}
	scharger_flash_inf("%s reg_val=0x%x\n", __func__, reg_val);
	return regmap_update_bits(g_scharger_v800_pdata.regmap, CHG_FLASH_LED_EN_REG, CHG_LED_EN_MSK,
					reg_val << CHG_LED_EN_SHIFT);
}

static int torch_mode_enable(struct scharger_v800_private_data_t *pdata,
	unsigned int led1_val, unsigned int led2_val, unsigned int reg_mode)
{
	int ret = 0;

	scharger_flash_inf("%s power_mode:%d\n", __func__, pdata->power_mode);

	switch(pdata->power_mode) {
	case PASS_THROUGH_MODE:
		ret += opt_regs_set(pdata, torch_pass_through_mode_opt_regs,
						ARRAY_SIZE(torch_pass_through_mode_opt_regs));
		break;
	case BOOST_MODE:
		ret += opt_regs_set(pdata, torch_boost_mode_opt_regs,
					ARRAY_SIZE(torch_boost_mode_opt_regs));
		/* wait 50ms for boost ready */
		mdelay(50);
		break;
	case EXTERNAL_BOOST_MODE:
		ret += opt_regs_set(pdata, external_boost_mode_opt_regs,
						ARRAY_SIZE(external_boost_mode_opt_regs));
		__set_input_current(scharger_get_flash_limit_input_current());
		ret += regmap_update_bits(pdata->regmap, CHG_BUCK_OTG_TOP_CFG_REG_15_REG, CHG_CC_EN_MSK,
					0 << CHG_CC_EN_SHIFT);
		break;
	default:
		scharger_flash_err("%s enable torch error\n", __func__);
		return -EINVAL;
	}

	/* clock en */
	ret += regmap_update_bits(pdata->regmap, CHG_SC_CRG_CLK_EN_CTRL_1_REG, CHG_FLASH_CLK_EN_MSK,
					 1 << CHG_FLASH_CLK_EN_SHIFT);
	/* set torch mode */
	ret += regmap_update_bits(pdata->regmap, CHG_FLASH_LED_MD_REG, CHG_FLASH_LED_MD_MSK,
					 (LED1_TORCH_MODE | LED2_TORCH_MODE) << CHG_FLASH_LED_MD_SHIFT);
	/* set LED ramp */
	ret += regmap_write(pdata->regmap, CHG_FLASH_RAMP_STEP_REG, SET_CURRENT_RAMP);
	/* set LED current */
	ret += regmap_write(pdata->regmap, CHG_FLASH_LED1_CODE_REG, led1_val);
	ret += regmap_write(pdata->regmap, CHG_FLASH_LED2_CODE_REG, led2_val);
	/* LED1/2/3 Buck-boost enable */
	ret += regmap_update_bits(pdata->regmap, CHG_FLASH_DIG_CFG_0_REG, CHG_FLASH_BB_EN_MSK,
					 FLASH_BB_ENABLE << CHG_FLASH_BB_EN_SHIFT);
	/* LED enable */
	ret += scharger_v800_flash_set_led_en(reg_mode);

	return ret;
}

static int torch_mode_disable(struct scharger_v800_private_data_t *pdata)
{
	int ret = 0;

	scharger_flash_inf("%s power_mode:%d\n", __func__, pdata->power_mode);

	switch(pdata->power_mode) {
	case PASS_THROUGH_MODE:
		ret += scharger_v800_flash_set_led_en(0);
		mdelay(1);
		ret += opt_regs_set(pdata, torch_pass_through_mode_exit_opt_regs,
				ARRAY_SIZE(torch_pass_through_mode_exit_opt_regs));
		break;
	case BOOST_MODE:
		ret += scharger_v800_flash_set_led_en(0);
		mdelay(1);
		ret += opt_regs_set(pdata, torch_boost_mode_exit_opt_regs,
					ARRAY_SIZE(torch_boost_mode_exit_opt_regs));
		break;
	case EXTERNAL_BOOST_MODE:
		ret += scharger_v800_flash_set_led_en(0);
		mdelay(1);
		ret += opt_regs_set(pdata, external_boost_mode_exit_opt_regs,
						ARRAY_SIZE(external_boost_mode_exit_opt_regs));
		break;
	default:
		scharger_flash_err("%s disable torch error\n", __func__);
		return -EINVAL;
	}

	ret += regmap_update_bits(pdata->regmap, CHG_BUCK_OTG_TOP_CFG_REG_15_REG, CHG_CC_EN_MSK,
			1 << CHG_CC_EN_SHIFT);
	ret += scharger_flash_restore_input_current();

	return ret;
}

static int scharger_v800_update_inner_status(struct hw_flash_ctrl_t *flash_ctrl,
	struct hw_flash_cfg_data *cdata)
{
	struct scharger_v800_private_data_t *pdata = NULL;
	enum scharger_v800_single_mode_t inner_mode = SINGLE_STANDBY_MODE;
	unsigned int cur = 0;

	if (!flash_ctrl || !cdata || !flash_ctrl->pdata) {
		scharger_flash_err("%s invalid params\n", __func__);
		return -EINVAL;
	}

	pdata = (struct scharger_v800_private_data_t *)flash_ctrl->pdata;
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

	if ((flash_ctrl->mix_pos == HWFLASH_POSITION_FORE) ||
		(flash_ctrl->mix_pos == HWFLASH_POSITION_REAR1)) {
		pdata->front_mode = inner_mode;
		pdata->front_cur  = cur;
		scharger_flash_inf("%s update front mode:%d, front cur %u\n", __func__,
			pdata->front_mode, pdata->front_cur);
	} else {
		pdata->back_mode  = inner_mode;
		pdata->back_cur   = cur;
		scharger_flash_inf("%s update back mode:%d, back cur %u\n",  __func__,
			pdata->back_mode, pdata->back_cur);
	}

	return 0;
}

static int scharger_v800_get_work_mode_strategy(struct hw_flash_ctrl_t *flash_ctrl,
	enum scharger_v800_entire_ic_mode_t *ic_work_mode)
{
	struct scharger_v800_private_data_t *pdata = NULL;
	unsigned int front_mode = SINGLE_STANDBY_MODE;
	unsigned int back_mode  = SINGLE_STANDBY_MODE;

	if (!flash_ctrl || !ic_work_mode || !flash_ctrl->pdata) {
		scharger_flash_err("%s invalid params\n", __func__);
		return -EINVAL;
	}

	pdata = (struct scharger_v800_private_data_t *)flash_ctrl->pdata;
	front_mode = pdata->front_mode;
	back_mode  = pdata->back_mode;

	*ic_work_mode = ENTIRE_IC_STANDBY_MODE; /* set as default */

	if ((front_mode | back_mode) == SINGLE_STANDBY_MODE)
		*ic_work_mode = ENTIRE_IC_STANDBY_MODE;
	else if (((front_mode | back_mode) & SINGLE_FLASH_MODE) == SINGLE_FLASH_MODE)
		*ic_work_mode = ENTIRE_IC_FLASH_MODE;
	else if (((front_mode | back_mode) & SINGLE_TORCH_MODE) == SINGLE_TORCH_MODE)
		*ic_work_mode = ENTIRE_IC_TORCH_MODE;

	if (pdata->front_cur == SCHARGERV800_STANDBY_MODE_CUR
		&& pdata->back_cur == SCHARGERV800_STANDBY_MODE_CUR)
		*ic_work_mode = ENTIRE_IC_STANDBY_MODE;
	scharger_flash_inf("%s ic_work_mode:%d\n", __func__, *ic_work_mode);

	return 0;
}

static int scharger_v800_action_ic_standby(struct hw_flash_ctrl_t *flash_ctrl)
{
	int ret = 0;
	struct scharger_v800_private_data_t *pdata = NULL;

	if (!flash_ctrl || !flash_ctrl->pdata) {
		scharger_flash_err("%s invalid params\n", __func__);
		return -EINVAL;
	}

	pdata = (struct scharger_v800_private_data_t *)flash_ctrl->pdata;

	ret += torch_mode_disable(pdata);
	pdata->power_mode = POWER_STANDBY_MODE;
#ifdef CONFIG_BOOST_5V
	if (pdata->power_mode == EXTERNAL_BOOST_MODE)
		ret += boost_5v_enable(BOOST_5V_DISABLE, BOOST_CTRL_FLASH);
#endif

	if (pdata->need_wakelock == WAKE_LOCK_ENABLE) {
		/*
		 * wake_unlock() will check
		 * whether the wakeup source is active before
		 * doing the unlock operation,
		 * so there is no dangers for the mutex being
		 * unlocked before locked.
		 */
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
		__pm_relax(&pdata->scharger_v800_flash_wakelock);
#else
		__pm_relax(pdata->scharger_v800_flash_wakelock);
#endif
	}
	return ret;
}

static bool  scharger_v800_flash_current_calculate(unsigned int *cur,
	struct scharger_v800_private_data_t *pdata)
{
	unsigned int cur_calc = 0;
	unsigned int cur_val = *cur;

	if (cur_val == FLASH_LED_LEVEL_INVALID)
		cur_val = SCHARGERV800_FLASH_DEFAULT_CUR;

	if (cur_val > SCHARGERV800_TORCH_MAX_CUR)
		cur_val = SCHARGERV800_TORCH_MAX_CUR;
	else if ((cur_val < SCHARGERV800_TORCH_MIN_CUR) &&
		(cur_val != SCHARGERV800_STANDBY_MODE_CUR))
		cur_val = SCHARGERV800_TORCH_MIN_CUR;

	if (cur_val != SCHARGERV800_STANDBY_MODE_CUR) {
		cur_calc = (cur_val / SCHARGERV800_TORCH_CUR_STEP) - 1;
		*cur = cur_calc;
		return LED_EN;
	} else {
		cur_calc = SCHARGERV800_TORCH_LED_VAL_MASK;
		return LED_DIS;
	}
}

static int scharger_v800_cfg_calculate(unsigned int *led1_val,
	unsigned int *led2_val,
	unsigned int *reg_mode,
	struct scharger_v800_private_data_t *pdata)
{
	unsigned int led1_cur = 0;
	unsigned int led2_cur = 0;

	scharger_flash_inf("%s led_type = %d, front_cur = %u, back_cur = %u\n",
		__func__, pdata->led_type, pdata->front_cur,  pdata->back_cur);

	/* check hardware menu and check DTS */
	if (pdata->led_type == BACK_FLASH_USE_LED2) {
		led1_cur = pdata->front_cur;
		led2_cur = pdata->back_cur;
	} else {
		led1_cur = pdata->back_cur;
		led2_cur = pdata->front_cur;
	}

	if (scharger_v800_flash_current_calculate(&led1_cur, pdata)) {
		*reg_mode |= LED1_EN;
		*led1_val = led1_cur;
	} else {
		*reg_mode &= LED1_DIS;
		*led1_val = SCHARGERV800_TORCH_LED_VAL_MASK;
	}
	if (scharger_v800_flash_current_calculate(&led2_cur, pdata)) {
		*reg_mode |= LED2_EN;
		*led2_val = led2_cur;
	} else {
		*reg_mode &= LED2_DIS;
		*led2_val = SCHARGERV800_TORCH_LED_VAL_MASK;
	}
	if (*reg_mode == 0) {
		scharger_flash_err("%s flash current error LED1=%u LED2=%u\n",
			__func__, led1_cur, led2_cur);
		return -EINVAL;
	}

	scharger_flash_inf("%s reg_mode:%u, led1_val:%u, led2_val:%u\n",
		__func__, *reg_mode, *led1_val, *led2_val);

	return 0;
}

static int scharger_v800_torch_mode_regs_cfg(struct hw_flash_ctrl_t *flash_ctrl,
	unsigned int led1_val,
	unsigned int led2_val,
	unsigned int reg_mode)
{
	int ret = 0;
	struct scharger_v800_private_data_t *pdata = NULL;

	if (!flash_ctrl) {
		scharger_flash_err("%s invalid params\n", __func__);
		return -EINVAL;
	}

	pdata = (struct scharger_v800_private_data_t *)flash_ctrl->pdata;

	ret = torch_mode_enable(pdata, led1_val, led2_val, reg_mode);

	scharger_flash_inf("%s config torch1_val = 0x%02x, torch2_val = 0x%02x, reg_mode = 0x%02x\n",
		__func__, led1_val, led2_val, reg_mode);

	return ret;
}

static int scharger_v800_action_ic_torch_mode(struct hw_flash_ctrl_t *flash_ctrl)
{
	int ret;
	unsigned int led1_val = 0;
	unsigned int led2_val = 0;
	unsigned int reg_mode = 0;
	struct scharger_v800_private_data_t *pdata = NULL;

	if (!flash_ctrl || !flash_ctrl->pdata) {
		scharger_flash_err("%s invalid params\n", __func__);
		return -EINVAL;
	}

	pdata = (struct scharger_v800_private_data_t *)flash_ctrl->pdata;

#ifdef CONFIG_BOOST_5V
	if (pdata->power_mode == EXTERNAL_BOOST_MODE) {
		ret = boost_5v_enable(BOOST_5V_ENABLE, BOOST_CTRL_FLASH);
		if (ret < 0) {
			scharger_flash_err("%s boost 5v en faild\n", __func__);
			return ret;
		}
	}
#endif

	ret = scharger_v800_cfg_calculate(&led1_val, &led2_val,
		&reg_mode, pdata);
	if (ret < 0) {
		scharger_flash_err("%s torch mode cfg calculate faild\n", __func__);
		return ret;
	}

	ret = scharger_v800_torch_mode_regs_cfg(flash_ctrl, led1_val,
		led2_val, reg_mode);
	if (ret < 0) {
		scharger_flash_err("%s torch mode reg cfg faild\n", __func__);
		return ret;
	}

	return ret;
}

static int scharger_v800_action_ic_flash_mode(struct hw_flash_ctrl_t *flash_ctrl)
{
	return scharger_v800_action_ic_torch_mode(flash_ctrl);
}

static int scharger_v800_update_work_mode_dispatch(struct hw_flash_ctrl_t *flash_ctrl,
	enum scharger_v800_entire_ic_mode_t ic_work_mode,
	struct hw_flash_cfg_data *cdata)
{
	int ret = 0;
	struct scharger_v800_private_data_t *pdata = NULL;

	if (!flash_ctrl || !flash_ctrl->pdata) {
		scharger_flash_err("%s invalid params\n", __func__);
		return -EINVAL;
	}

	pdata = (struct scharger_v800_private_data_t *)flash_ctrl->pdata;

	switch (ic_work_mode) {
	case ENTIRE_IC_STANDBY_MODE:
		if (pdata->entire_ic_mode != ENTIRE_IC_STANDBY_MODE) {
			/* only when current mode is not in standby, */
			/* action the standby mode */
			ret = scharger_v800_action_ic_standby(flash_ctrl);
			pdata->entire_ic_mode = ENTIRE_IC_STANDBY_MODE;
			flash_ctrl->state.mode = STANDBY_MODE;
			flash_ctrl->state.data = SCHARGERV800_STANDBY_MODE_CUR;
		}
		break;
	case ENTIRE_IC_FLASH_MODE:
		ret = scharger_v800_action_ic_flash_mode(flash_ctrl);
		pdata->entire_ic_mode = ENTIRE_IC_FLASH_MODE;
		break;
	case ENTIRE_IC_TORCH_MODE:
		ret = scharger_v800_action_ic_torch_mode(flash_ctrl);
		pdata->entire_ic_mode = ENTIRE_IC_TORCH_MODE;
		break;
	default:
		scharger_flash_err("%s invalid ic work mode\n", __func__);
		ret = -EINVAL;
		break;
	}

	return ret;
}

static int scharger_v800_update_work_mode(struct hw_flash_ctrl_t *flash_ctrl,
	struct hw_flash_cfg_data *cdata)
{
	int ret;
	enum scharger_v800_entire_ic_mode_t ic_work_mode = ENTIRE_IC_STANDBY_MODE;

	if (!flash_ctrl || !cdata) {
		scharger_flash_err("%s invalid params\n", __func__);
		return -EINVAL;
	}

	ret = scharger_v800_update_inner_status(flash_ctrl, cdata);
	if (ret < 0) {
		scharger_flash_err("%s update inner status failed\n", __func__);
		return ret;
	}

	ret = scharger_v800_get_work_mode_strategy(flash_ctrl, &ic_work_mode);
	if (ret < 0) {
		scharger_flash_err("%s get work mode strategy failed\n", __func__);
		return ret;
	}

	ret = scharger_v800_update_work_mode_dispatch(flash_ctrl, ic_work_mode, cdata);
	if (ret < 0) {
		scharger_flash_err("%s update work mode dispatch failed\n", __func__);
		return ret;
	}

	return ret;
}

static int scharger_v800_flash_on(struct hw_flash_ctrl_t *flash_ctrl, void *data)
{
	struct hw_flash_cfg_data *cdata = (struct hw_flash_cfg_data *)data;
	struct scharger_v800_private_data_t *pdata = NULL;
	int ret = -1;
	int vbat;
	unsigned int buck_state;

	if (!flash_ctrl || !flash_ctrl->pdata || !cdata) {
		scharger_flash_err("%s flash_ctrl or cdata is NULL\n", __func__);
		return -EINVAL;
	}

	pdata = (struct scharger_v800_private_data_t *)flash_ctrl->pdata;

	scharger_flash_inf("%s mode = %d, cur = %d\n", __func__,
		cdata->mode, cdata->data);

	if (!pdata->use_external_boost) {
		switch (cdata->mode) {
		case STANDBY_MODE:
			pdata->power_mode = POWER_STANDBY_MODE;
			break;
		case FLASH_MODE:
		case FLASH_STROBE_MODE:
			vbat = coul_drv_battery_voltage();
			(void)regmap_read(pdata->regmap, CHG_BUCK_OTG_TOP_RO_REG_45_REG, &buck_state);
			scharger_flash_inf("%s, vbat=%d, buck_state=0x%x\n", __func__, vbat, buck_state);
			if (vbat < FLASH_MODE_VBAT_THRE && ((buck_state & CHG_BUCK_OK) != CHG_BUCK_OK))
				pdata->power_mode = BOOST_MODE;
			else
				pdata->power_mode = PASS_THROUGH_MODE;

			if (pdata->power_mode == BOOST_MODE) {
				scharger_v800_mode_lock();
				pdata->use_mode_lock = TRUE;
			}
			break;
		default:
			pdata->power_mode = PASS_THROUGH_MODE;
			break;
		}
	} else {
		pdata->power_mode = EXTERNAL_BOOST_MODE;
	}

	scharger_flash_inf("%s power_mode = %d\n", __func__, pdata->power_mode);

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
		__pm_stay_awake(&pdata->scharger_v800_flash_wakelock);
#else
		__pm_stay_awake(pdata->scharger_v800_flash_wakelock);
#endif
	}
	ret = scharger_v800_update_work_mode(flash_ctrl, cdata);

	mutex_unlock(flash_ctrl->hw_flash_mutex);

	return ret;
}

static int scharger_v800_flash_off(struct hw_flash_ctrl_t *flash_ctrl)
{
	struct hw_flash_cfg_data cdata = {0};
	struct scharger_v800_private_data_t *pdata = NULL;
	int ret;

	if (!flash_ctrl || !flash_ctrl->pdata) {
		scharger_flash_err("%s invalid params\n", __func__);
		return -EINVAL;
	}

	scharger_flash_inf("%s enter\n", __func__);

	pdata = (struct scharger_v800_private_data_t *)flash_ctrl->pdata;
	cdata.mode = (flash_mode)SINGLE_STANDBY_MODE;
	cdata.data = SCHARGERV800_STANDBY_MODE_CUR; /* set standby current as 0 */

	mutex_lock(flash_ctrl->hw_flash_mutex);
	ret = scharger_v800_update_work_mode(flash_ctrl, &cdata);
	mutex_unlock(flash_ctrl->hw_flash_mutex);

	if (pdata->use_mode_lock) {
		scharger_v800_mode_unlock();
		pdata->use_mode_lock = FALSE;
	}

	return ret;
}

static int scharger_v800_flash_match(struct hw_flash_ctrl_t *flash_ctrl)
{
	struct scharger_v800_private_data_t *pdata = NULL;
	int ret = -1;

	scharger_flash_inf("%s enter\n", __func__);
	if (!flash_ctrl || !flash_ctrl->pdata) {
		scharger_flash_err("%s flash_ctrl is NULL\n", __func__);
		return -EINVAL;
	}
	pdata = (struct scharger_v800_private_data_t *)flash_ctrl->pdata;
	
	/* byte length of version_4 charger version is 2 */
	ret = regmap_bulk_read(pdata->regmap, CHG_VERSION4_RO_REG_0_REG, &pdata->scharger_version, 2);
	if (ret != 0) {
		scharger_flash_err("%s:read scharger_version fail\n", __func__);
		return -EINVAL;
	}
	if (pdata->scharger_version != CHIP_ID_V810) {
		scharger_flash_err("%s:scharger_version check fail, version:0x%x\n",
			__func__, pdata->scharger_version);
		return -EINVAL;
	}

	return 0;
}

static int scharger_v800_flash_get_dt_data(struct hw_flash_ctrl_t *flash_ctrl)
{
	struct scharger_v800_private_data_t *pdata = NULL;
	struct device_node *of_node = NULL;
	int i;
	int ret = -1;
	int count = 0;

	scharger_flash_inf("%s enter\n", __func__);
	if (!flash_ctrl || !flash_ctrl->pdata || !flash_ctrl->dev) {
		scharger_flash_err("%s flash_ctrl is NULL\n", __func__);
		return ret;
	}

	pdata = (struct scharger_v800_private_data_t *)flash_ctrl->pdata;
	of_node = flash_ctrl->dev->of_node;

	ret = of_property_read_u32(of_node, "vendor,external-boost", &pdata->use_external_boost);
	scharger_flash_inf("%s vendor,external-boost %d, ret %d\n", __func__,
		pdata->use_external_boost, ret);
	if (ret < 0) {
		scharger_flash_err("%s read external-boost failed %d\n", __func__, __LINE__);
		return ret;
	}

	ret = of_property_read_u32(of_node, "vendor,led-type", &pdata->led_type);
	scharger_flash_inf("%s vendor,led-type %d, ret %d\n", __func__,
		pdata->led_type, ret);
	if (ret < 0) {
		scharger_flash_err("%s read led-type failed %d\n", __func__, __LINE__);
		return ret;
	}

	ret = of_property_read_u32(of_node, "vendor,need-wakelock",
		(u32 *)&pdata->need_wakelock);
	scharger_flash_inf("%s vendor,need-wakelock %d, ret %d\n", __func__,
		pdata->need_wakelock, ret);
	if (ret < 0) {
		pdata->need_wakelock = WAKE_LOCK_DISABLE;
		scharger_flash_err("%s failed %d\n", __func__, __LINE__);
		return ret;
	}

	ret = of_property_read_u32_array(of_node, "vendor,flash-current",
		pdata->ecurrent, CURRENT_MIX_MAX);
	if (ret < 0) {
		scharger_flash_err("%s read flash-current failed line %d\n",
			__func__, __LINE__);
		return ret;
	}
	for (i = 0; i < CURRENT_MIX_MAX; i++)
		scharger_flash_inf("%s ecurrent[%d]=%d\n", __func__,
			i, pdata->ecurrent[i]);

	count = of_property_count_elems_of_size(of_node, "vendor,led-id", sizeof(u32));
	if (count > 0 && count <= HWFLASH_POSITION_MAX) {
		cam_info("%s:%d vendor,led-id set count:%d", __func__, __LINE__, count);
		ret = of_property_read_u32_array(of_node, "vendor,led-id",
			pdata->id_list, count);
		if (ret < 0) {
			cam_info("%s get led-id fail %d\n",
				__func__, __LINE__);
			ret = 0;
		}
	}

	return ret;
}

static int scharger_v800_param_check(char *buf, unsigned long *param,
	int num_of_par)
{
	char *token = NULL;
	unsigned int base = 0;
	int cnt;

	if (!buf || !param) {
		scharger_flash_err("%s invalid params\n", __func__);
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
			scharger_flash_inf("%s the %d param is null\n",
				__func__, num_of_par);
		}
	}

	return 0;
}

static ssize_t scharger_v800_flash_lightness_store_imp(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct hw_flash_cfg_data cdata = {0};
	unsigned long param[MAX_LIGHTNESS_PARAM_NUM_RT] = {0};
	int ret;
	int flash_id = 0;

	if (!buf) {
		scharger_flash_err("%s invalid params\n", __func__);
		return -EINVAL;
	}

	ret = scharger_v800_param_check((char *)buf, param,
		MAX_LIGHTNESS_PARAM_NUM_RT);
	if (ret < 0) {
		scharger_flash_err("%s failed to check param\n", __func__);
		return ret;
	}

	/* 0 - flash id */
	flash_id = (int)param[LIGHTNESS_PARAM_FD_INDEX_RT];
	/* 1 - mode */
	cdata.mode = (int)param[LIGHTNESS_PARAM_MODE_INDEX_RT];
	/* 2 - current */
	cdata.data = (int)param[LIGHTNESS_PARAM_CURRENT_INDEX_RT];
	scharger_flash_inf("%s flash_id=%d, cdata.mode=%d, cdata.data=%d\n", __func__,
		flash_id, cdata.mode, cdata.data);

	if ((cdata.mode < STANDBY_MODE) || (cdata.mode > MAX_MODE)) {
		scharger_flash_err("%s cdata.mode out of bounds\n", __func__);
		return -EINVAL;
	}
	/* 0~384 mA */
	if ((cdata.data < 0) || (cdata.data > SCHARGERV800_TORCH_MAX_CUR)) {
		scharger_flash_err("%s cdata.data out of bounds\n", __func__);
		return -EINVAL;
	}
	/* 0~0xFF, pass from HAL, but not use just print */
	if ((flash_id < 0) || (flash_id > MAX_FLASH_ID)) {
		scharger_flash_err("%s flash_id out of bounds\n", __func__);
		return -EINVAL;
	}

	g_scharger_v800_ctrl.mix_pos = flash_id;

	if (cdata.mode == STANDBY_MODE) {
		ret = scharger_v800_flash_off(&g_scharger_v800_ctrl);
		if (ret < 0) {
			scharger_flash_err("%s flash off error\n", __func__);
			return ret;
		}
	} else if (cdata.mode == TORCH_MODE) {
		if ((g_scharger_v800_ctrl.mix_pos == HWFLASH_POSITION_FORE) ||
			(g_scharger_v800_ctrl.mix_pos == HWFLASH_POSITION_REAR1))
			cdata.data =
				g_scharger_v800_pdata.ecurrent[CURRENT_TORCH_LEVEL_RT_FRONT];
		else
			cdata.data =
				g_scharger_v800_pdata.ecurrent[CURRENT_TORCH_LEVEL_RT_BACK];

		scharger_flash_inf("%s mode=%d, current=%d\n", __func__, cdata.mode, cdata.data);

		ret = scharger_v800_flash_on(&g_scharger_v800_ctrl, &cdata);
		if (ret < 0) {
			scharger_flash_err("%s flash on error\n", __func__);
			return ret;
		}
	} else if (cdata.mode == FLASH_MODE) {
		scharger_flash_inf("%s mode=%d, current=%d\n", __func__, cdata.mode, cdata.data);
		ret = scharger_v800_flash_on(&g_scharger_v800_ctrl, &cdata);
		if (ret < 0) {
			scharger_flash_err("%s flash on error\n", __func__);
			return ret;
		}
	} else {
		scharger_flash_err("%s wrong mode=%d\n", __func__, cdata.mode);
		return -EINVAL;
	}

	return count;
}

static ssize_t scharger_v800_flash_lightness_show_f(struct device *dev,
	struct device_attribute *attr,
	char *buf)
{
	int ret;

	if (!buf) {
		scharger_flash_err("%s invalid params\n", __func__);
		return -EINVAL;
	}

	/* show for ir selftest in RT, PAGE_SIZE = 4096 */
	ret = scnprintf(buf, PAGE_SIZE, "%d %d %d %d",
		g_scharger_v800_pdata.selftest_param[IR_SELFTEST_ON_TIME_INDEX],
		g_scharger_v800_pdata.selftest_param[IR_SELFTEST_OFF_TIME_INDEX],
		g_scharger_v800_pdata.selftest_param[IR_SELFTEST_CURRENT_INDEX],
		g_scharger_v800_pdata.selftest_param[IR_SELFTEST_TIMES_INDEX]);

	return ret;
}

static ssize_t scharger_v800_flash_lightness_f_store(struct device *dev,
	struct device_attribute *attr,
	const char *buf,
	size_t count)
{
	/* call front flash */
	g_scharger_v800_ctrl.mix_pos = HWFLASH_POSITION_FORE;
	return scharger_v800_flash_lightness_store_imp(dev, attr,
		buf, count);
}

static ssize_t scharger_v800_flash_lightness_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int ret;

	if (!buf) {
		scharger_flash_err("%s invalid params\n", __func__);
		return -EINVAL;
	}

	/* PAGE_SIZE = 4096 */
	ret = scnprintf(buf, PAGE_SIZE,
		"front_mode=%d, data=%d; back_mode=%d, data=%d\n",
		g_scharger_v800_pdata.front_mode,
		g_scharger_v800_pdata.front_cur,
		g_scharger_v800_pdata.back_mode,
		g_scharger_v800_pdata.back_cur);

	return ret;
}

static ssize_t scharger_v800_flash_lightness_store(struct device *dev,
	struct device_attribute *attr,
	const char *buf,
	size_t count)
{
	/* call back flash */
	g_scharger_v800_ctrl.mix_pos = HWFLASH_POSITION_REAR;
	return scharger_v800_flash_lightness_store_imp(dev, attr,
		buf, count);
}

static int scharger_v800_torch_brightness_set_imp(struct led_classdev *cdev,
	enum led_brightness brightness)
{
	int ret = 0;
	struct hw_flash_cfg_data cdata = {0};
	unsigned int led_bright = brightness;

	scharger_flash_inf("%s brightness= %d\n", __func__, brightness);

	if (led_bright == LED_OFF) {
		ret = scharger_v800_flash_off(&g_scharger_v800_ctrl);
		if (ret < 0) {
			scharger_flash_err("%s pmu_led off error\n", __func__);
			return ret;
		}
	} else {
		cdata.mode = TORCH_MODE;
		if ((g_scharger_v800_ctrl.mix_pos == HWFLASH_POSITION_FORE) ||
			(g_scharger_v800_ctrl.mix_pos == HWFLASH_POSITION_REAR1))
			cdata.data =
				g_scharger_v800_pdata.ecurrent[CURRENT_TORCH_LEVEL_MMI_FRONT];
		else
			cdata.data =
				g_scharger_v800_pdata.ecurrent[CURRENT_TORCH_LEVEL_MMI_BACK];

		scharger_flash_inf("%s brightness=0x%x, mode=%d, data=%d\n", __func__,
			brightness, cdata.mode, cdata.data);
		ret = scharger_v800_flash_on(&g_scharger_v800_ctrl, &cdata);
		if (ret < 0) {
			scharger_flash_err("%s flash on error\n", __func__);
			return ret;
		}
	}

	return ret;
}

static int scharger_v800_torch_brightness_set(struct led_classdev *cdev,
	enum led_brightness brightness)
{
	g_scharger_v800_ctrl.mix_pos = HWFLASH_POSITION_REAR;
	return scharger_v800_torch_brightness_set_imp(cdev, brightness);
}

static int scharger_v800_torch_brightness_f_set(struct led_classdev *cdev,
	enum led_brightness brightness)
{
	g_scharger_v800_ctrl.mix_pos = HWFLASH_POSITION_FORE;
	return scharger_v800_torch_brightness_set_imp(cdev, brightness);
}

static ssize_t scharger_v800_flash_mask_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int ret;

	if (!buf) {
		scharger_flash_err("%s invalid params\n", __func__);
		return -EINVAL;
	}

	ret = scnprintf(buf, PAGE_SIZE, "flash_mask_disabled=%d\n",
		g_scharger_v800_ctrl.flash_mask_enable);

	return ret;
}

static ssize_t scharger_v800_flash_mask_store(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	if (!buf) {
		scharger_flash_err("%s invalid params\n", __func__);
		return -EINVAL;
	}

	if (buf[0] == '0') /* char '0' for mask disable */
		g_scharger_v800_ctrl.flash_mask_enable = false;
	else
		g_scharger_v800_ctrl.flash_mask_enable = true;

	scharger_flash_inf("%s flash_mask_enable=%d\n", __func__,
		g_scharger_v800_ctrl.flash_mask_enable);
	return (ssize_t)count;
}

/* for RT */
static struct device_attribute g_scharger_v800_flash_lightness =
	__ATTR(flash_lightness, 0660, scharger_v800_flash_lightness_show,
	scharger_v800_flash_lightness_store); /* 660:-wr-wr--- */

static struct device_attribute g_scharger_v800_flash_lightness_f =
	__ATTR(flash_lightness_f, 0660,
	scharger_v800_flash_lightness_show_f,
	scharger_v800_flash_lightness_f_store); /* 660:-wr-wr--- */

static struct device_attribute g_scharger_v800_flash_mask =
	__ATTR(flash_mask, 0660, scharger_v800_flash_mask_show,
	scharger_v800_flash_mask_store); /* 660:-wr-wr--- */

static ssize_t scharger_v800_flash_get_id_list_show(struct device *dev,
	struct device_attribute *attr, char *buf)
{
	int rc;
	struct scharger_v800_private_data_t *pdata = NULL;
	pdata = g_scharger_v800_ctrl.pdata;
	if (!pdata) {
		cam_err("%s invalid pdata", __func__);
		return -EINVAL;
	}
 
	if (!buf) {
		cam_err("%s invalid params", __func__);
		return -EINVAL;
	}
 
	rc = snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1,"%d %d\n",
		pdata->id_list[0],
		pdata->id_list[1]);
 
	return rc;
}
 
static ssize_t scharger_v800_flash_get_id_list_store(struct device *dev,
	struct device_attribute *attr,
	const char *buf,
	size_t count)
{
	return (ssize_t)count;
}
 
// for dual flashlight test
static struct device_attribute scharger_v800_flash_get_id_list =
	__ATTR(flash_get_id_list, 0660, scharger_v800_flash_get_id_list_show,
	scharger_v800_flash_get_id_list_store); /* 660:-wr-wr--- */

extern int register_camerafs_attr(struct device_attribute *attr);
static int scharger_v800_flash_register_attribute(struct hw_flash_ctrl_t *flash_ctrl,
	struct device *dev)
{
	int ret;

	if (!flash_ctrl || !dev || !flash_ctrl->pdata) {
		scharger_flash_err("%s flash_ctrl or dev is NULL\n", __func__);
		return -EINVAL;
	}
	register_camerafs_attr(&g_scharger_v800_flash_lightness);
	register_camerafs_attr(&g_scharger_v800_flash_lightness_f);
	register_camerafs_attr(&scharger_v800_flash_get_id_list);
	flash_ctrl->cdev_torch.name = "torch";
	flash_ctrl->cdev_torch.max_brightness
		= (enum led_brightness)MAX_BRIGHTNESS_FORMMI;
	flash_ctrl->cdev_torch.brightness_set_blocking =
		scharger_v800_torch_brightness_set;
	ret = led_classdev_register((struct device *)dev,
		&flash_ctrl->cdev_torch);
	if (ret < 0) {
		scharger_flash_err("%s failed to register torch classdev\n", __func__);
		goto err_out;
	}

	flash_ctrl->cdev_torch1.name = "torch_front";
	flash_ctrl->cdev_torch1.max_brightness =
		(enum led_brightness)MAX_BRIGHTNESS_FORMMI;
	flash_ctrl->cdev_torch1.brightness_set_blocking =
		scharger_v800_torch_brightness_f_set;
	ret = led_classdev_register((struct device *)dev,
		&flash_ctrl->cdev_torch1);
	if (ret < 0) {
		scharger_flash_err("%s failed to register torch_front classdev\n", __func__);
		goto err_create_torch_front_file;
	}
	ret = device_create_file(dev, &g_scharger_v800_flash_lightness);
	if (ret < 0) {
		scharger_flash_err("%s failed to creat flash_lightness attribute\n",
			__func__);
		goto err_create_flash_lightness_file;
	}

	ret = device_create_file(dev, &g_scharger_v800_flash_lightness_f);
	if (ret < 0) {
		scharger_flash_err("%s failed to creat flash_f_lightness attribute\n",
			__func__);
		goto err_create_flash_f_lightness_file;
	}

	ret = device_create_file(dev, &g_scharger_v800_flash_mask);
	if (ret < 0) {
		scharger_flash_err("%s failed to creat flash_mask attribute\n", __func__);
		goto err_create_flash_mask_file;
	}

	return 0;

err_create_flash_mask_file:
	device_remove_file(dev, &g_scharger_v800_flash_lightness_f);
err_create_flash_f_lightness_file:
	device_remove_file(dev, &g_scharger_v800_flash_lightness);
err_create_flash_lightness_file:
	led_classdev_unregister(&flash_ctrl->cdev_torch1);
err_create_torch_front_file:
	led_classdev_unregister(&flash_ctrl->cdev_torch);
err_out:
	return ret;
}

static int scharger_flash_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	int ret;

	scharger_flash_err("%s +\n", __func__);
	ret = memset_s(&g_scharger_v800_pdata, sizeof(g_scharger_v800_pdata),
			0, sizeof(g_scharger_v800_pdata));
	if (ret != EOK)
		scharger_flash_err("%s failed to memset_s write_data\n", __func__);

	g_scharger_v800_pdata.dev = dev;
	g_scharger_v800_pdata.regmap = dev_get_regmap(dev->parent, NULL);
	if (g_scharger_v800_pdata.regmap == NULL) {
		scharger_flash_err("Parent regmap unavailable.\n");
		return -ENXIO;
	}
	INIT_WORK(&g_scharger_v800_pdata.flash_irq_work, scharger_v800_flash_irq_work);
	scharger_v800_flash_irq_init(&g_scharger_v800_pdata);
	g_scharger_v800_pdata.power_mode = POWER_STANDBY_MODE;

	g_scharger_v800_ctrl.pdata = &g_scharger_v800_pdata;
	platform_set_drvdata(pdev, &g_scharger_v800_ctrl);

	scharger_flash_err("%s -\n", __func__);
	return hw_flash_platform_probe(pdev, &g_scharger_v800_ctrl);
}

static struct hw_flash_fn_t g_scharger_v800_func_tbl = {
	.flash_config = hw_flash_config,
	.flash_init = scharger_v800_flash_init,
	.flash_exit = scharger_v800_flash_exit,
	.flash_on = scharger_v800_flash_on,
	.flash_off = scharger_v800_flash_off,
	.flash_match = scharger_v800_flash_match,
	.flash_get_dt_data = scharger_v800_flash_get_dt_data,
	.flash_register_attribute = scharger_v800_flash_register_attribute,
};

static struct hw_flash_ctrl_t g_scharger_v800_ctrl = {
	.func_tbl = &g_scharger_v800_func_tbl,
	.hw_flash_mutex = &flash_mut_scharger_v800,
	.pdata = (void *)&g_scharger_v800_pdata,
	.flash_mask_enable = true,
	.state = {
		.mode = STANDBY_MODE,
	},
};

static int scharger_flash_remove(struct platform_device *pdev)
{
	g_scharger_v800_ctrl.func_tbl->flash_exit(&g_scharger_v800_ctrl);

	return 0;
}

const static struct of_device_id of_scharger_flash_match_tbl[] = {
	{
		.compatible = "hisilicon,scharger-v800-flash",
		.data = NULL,
	},
	{},
};

static struct platform_driver scharger_flash_driver = {
	.driver = {
		.name = "scharger_v800_flash",
		.owner = THIS_MODULE,
		.of_match_table = of_scharger_flash_match_tbl,
	},
	.probe = scharger_flash_probe,
	.remove = scharger_flash_remove,
};

static int __init scharger_flash_init(void)
{
	return platform_driver_register(&scharger_flash_driver);
}

static void __exit scharger_flash_exit(void)
{
	platform_driver_unregister(&scharger_flash_driver);
}

late_initcall(scharger_flash_init);
module_exit(scharger_flash_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("schargerV800 flash driver");