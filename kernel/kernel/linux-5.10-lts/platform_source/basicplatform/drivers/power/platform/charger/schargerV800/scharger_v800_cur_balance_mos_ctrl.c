/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 * Description: Device driver for schargerV800 current balance mos control.
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

#include <linux/power_supply.h>

#include "scharger_v800_buck.h"
#include "scharger_v800_adc.h"
#include "scharger_v800_cur_balance_mos_ctrl.h"
#include <chipset_common/hwpower/common_module/power_event_ne.h>

#define PERMILLAGE                   1000
#define BATT_MAIN_ID   0
#define BATT_AUX_ID    1
#define BATT_NAME_NOT_EXIST 1
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define DEFAULT_SUSPEND_VBAT_THRE 3500
#define DEFAULT_SUSPEND_CAP_THRE  2
#define DEFAULT_VBAT_DIFF_THRE 50
#define DEFAULT_SMALL_BATT_ID 1
#define DEFAULT_IBAT_AVG_THRE 500
#define DEFAULT_SMALL_BATT_VOL_THRE 3200

struct cur_balance_mos_param {
	unsigned int suspend_vbat_thre;
	unsigned int suspend_cap_thre;
	unsigned int vbat_diff_thre;
	unsigned int small_batt_id;
	unsigned int ibat_avg_thre;
	unsigned int small_batt_vol_thre;
};

struct cur_balance_mos_info {
	void *dev_data;
	struct cur_balance_mos_param cbm_param;
	struct mutex cbm_lock;
	struct notifier_block chg_nb;
	struct delayed_work discharge_monitor_work;
};

struct cur_balance_mos_info *cbm_info = NULL;
static enum charge_state g_current_state = STATE_DISCHARGING_ABOVE_THRE;
bool cbm_test_flag = FALSE;

static void scharger_cbm_parse_dts(struct scharger_buck *buck)
{
	int ret;
	struct device_node *np = buck->dev->of_node;

	if (np == NULL) {
		scharger_err("%s np is NULL!\n", __func__);
		return;
	}

	ret = of_property_read_u32(np, "suspend_vbat_thre", &(cbm_info->cbm_param.suspend_vbat_thre));
	if (ret != 0) {
		cbm_info->cbm_param.suspend_vbat_thre = DEFAULT_SUSPEND_VBAT_THRE;
		scharger_err("get suspend_vbat_thre failed, use default val %u\n", cbm_info->cbm_param.suspend_vbat_thre);
	}
	buck->param_dts.vsys_mos_suspend_vbat_thre = cbm_info->cbm_param.suspend_vbat_thre;
	scharger_inf("prase_dts suspend_vbat_thre=%u\n", cbm_info->cbm_param.suspend_vbat_thre);

	ret = of_property_read_u32(np, "suspend_cap_thre", &(cbm_info->cbm_param.suspend_cap_thre));
	if (ret != 0) {
		cbm_info->cbm_param.suspend_cap_thre = DEFAULT_SUSPEND_CAP_THRE;
		scharger_err("get suspend_cap_thre failed, use default val %u\n", cbm_info->cbm_param.suspend_cap_thre);
	}
	buck->param_dts.vsys_mos_suspend_cap_thre = cbm_info->cbm_param.suspend_cap_thre;
	scharger_inf("prase_dts suspend_cap_thre=%u\n", cbm_info->cbm_param.suspend_cap_thre);

	ret = of_property_read_u32(np, "vbat_diff_thre", &(cbm_info->cbm_param.vbat_diff_thre));
	if (ret != 0) {
		cbm_info->cbm_param.vbat_diff_thre = DEFAULT_VBAT_DIFF_THRE;
		scharger_err("get vbat_diff_thre failed, use default val %u\n", cbm_info->cbm_param.vbat_diff_thre);
	}
	scharger_inf("prase_dts vbat_diff_thre=%u\n", cbm_info->cbm_param.vbat_diff_thre);

	ret = of_property_read_u32(np, "small_batt_id", &(cbm_info->cbm_param.small_batt_id));
	if (ret != 0) {
		cbm_info->cbm_param.small_batt_id = DEFAULT_SMALL_BATT_ID;
		scharger_err("get small_batt_id failed, use default val %u\n", cbm_info->cbm_param.small_batt_id);
	}
	scharger_inf("prase_dts small_batt_id=%u\n", cbm_info->cbm_param.small_batt_id);

	ret = of_property_read_u32(np, "ibat_avg_thre", &(cbm_info->cbm_param.ibat_avg_thre));
	if (ret != 0) {
		cbm_info->cbm_param.ibat_avg_thre = DEFAULT_IBAT_AVG_THRE;
		scharger_err("get ibat_avg_thre failed, use default val %u\n", cbm_info->cbm_param.ibat_avg_thre);
	}
	scharger_inf("prase_dts ibat_avg_thre=%u\n", cbm_info->cbm_param.ibat_avg_thre);

	ret = of_property_read_u32(np, "small_batt_vol_thre", &(cbm_info->cbm_param.small_batt_vol_thre));
	if (ret != 0) {
		cbm_info->cbm_param.small_batt_vol_thre = DEFAULT_SMALL_BATT_VOL_THRE;
		scharger_err("get small_batt_vol_thre failed, use default val %u\n", cbm_info->cbm_param.small_batt_vol_thre);
	}
	scharger_inf("prase_dts small_batt_vol_thre=%u\n", cbm_info->cbm_param.small_batt_vol_thre);
}

int scharger_get_batt_capacity(int *cap)
{
	int ret;
	struct power_supply *psy = NULL;
	union power_supply_propval val = {0};

	if (cap == NULL)
		return -EINVAL;

	psy = power_supply_get_by_name("Battery");
	if (!psy) {
		scharger_err("%s power supply Battery not exist\n", __func__);
		return -EINVAL;
	}
	ret = power_supply_get_property(psy, POWER_SUPPLY_PROP_CAPACITY, &val);
	power_supply_put(psy);
	*cap = val.intval;

	scharger_inf("%s batt_cap:%d\n", __func__, *cap);
	return ret;
}

int scharger_get_coul_vbat(char *batt_name, int *vbat)
{
	int ret;
	struct power_supply *psy = NULL;
	union power_supply_propval val = {0};

	if (vbat == NULL)
		return -EINVAL;

	psy = power_supply_get_by_name(batt_name);
	if (!psy) {
		scharger_err("%s power supply Battery not exist\n", __func__);
		return -EINVAL;
	}
	ret = power_supply_get_property(psy, POWER_SUPPLY_PROP_VOLTAGE_NOW, &val);
	power_supply_put(psy);
	*vbat = val.intval / PERMILLAGE;

	scharger_inf("%s vbat:%d\n", __func__, *vbat);
	return ret;
}

static int get_coul_ibat_avg(char *batt_name, int *ibat)
{
	int ret;
	struct power_supply *psy = NULL;
	union power_supply_propval val = {0};

	if (ibat == NULL)
		return -EINVAL;

	psy = power_supply_get_by_name(batt_name);
	if (!psy) {
		scharger_err("power supply %s not exist.\n", batt_name);
		return BATT_NAME_NOT_EXIST;
	}
	ret = power_supply_get_property(psy, POWER_SUPPLY_PROP_CURRENT_AVG, &val);
	power_supply_put(psy);
	*ibat = val.intval;

	scharger_inf("%s %s ibat_avg:%d\n", __func__, batt_name, *ibat);
	return ret;
}

static void enter_charging(void)
{
	if (scharger_v800_set_cur_balance_mos_open(0))
		scharger_inf("%s scharger_v800_set_cur_balance_mos_open fail\n", __func__);
	g_current_state = STATE_CHARGING;

	scharger_inf("%s enter charge state %d\n", __func__, g_current_state);
}

static void enter_discharging_above_thre(void)
{
	if (scharger_v800_set_cur_balance_mos_open(0))
		scharger_inf("%s scharger_v800_set_cur_balance_mos_open fail\n", __func__);
	g_current_state = STATE_DISCHARGING_ABOVE_THRE;

	scharger_inf("%s enter charge state %d\n", __func__, g_current_state);
}

static void enter_discharging_below_thre(void)
{
	if (scharger_v800_set_cur_balance_mos_open(1))
		scharger_inf("%s scharger_v800_set_cur_balance_mos_open fail\n", __func__);
	g_current_state = STATE_DISCHARGING_BELOW_THRE;

	scharger_inf("%s enter charge state %d\n", __func__, g_current_state);
}

void enter_suspend(void)
{
	int vbat = 0;
	int cap = 0;
	int ret;

	scharger_get_coul_vbat("Battery", &vbat);
	scharger_get_batt_capacity(&cap);
	if (vbat < cbm_info->cbm_param.suspend_vbat_thre || cap < cbm_info->cbm_param.suspend_cap_thre) {
		ret = scharger_v800_set_cur_balance_mos_open(1);
		scharger_inf("%s vbat=%d, cap=%d, set cur_balance_mos open\n", __func__, vbat, cap);
	} else {
		ret = scharger_v800_set_cur_balance_mos_open(0);
	}
	if (ret)
		scharger_inf("%s scharger_v800_set_cur_balance_mos_open fail\n", __func__);

	g_current_state = STATE_SUSPEND;

	scharger_inf("%s enter charge state %d\n", __func__, g_current_state);
}

static bool is_delta_v_above_thre(void)
{
	int vbat_main = 0;
	int vbat_aux = 0;
	int ibat_avg;
	int ibat_main = 0;
	int ibat_aux = 0;
	bool ret = false;

	scharger_get_coul_vbat(BATT_MAIN_NAME, &vbat_main);
	scharger_get_coul_vbat(BATT_AUX_NAME, &vbat_aux);
	get_coul_ibat_avg(BATT_MAIN_NAME, &ibat_main);
	get_coul_ibat_avg(BATT_AUX_NAME, &ibat_aux);
	ibat_avg = ibat_main + ibat_aux;
	if (cbm_info->cbm_param.small_batt_id == BATT_MAIN_ID) {
		ret = ((0 - ibat_avg > cbm_info->cbm_param.ibat_avg_thre) && (vbat_aux > vbat_main) &&
			(vbat_aux - vbat_main > cbm_info->cbm_param.vbat_diff_thre) &&
			(vbat_main > cbm_info->cbm_param.small_batt_vol_thre)) ? TRUE : FALSE;
	} else if (cbm_info->cbm_param.small_batt_id == BATT_AUX_ID) {
		ret = ((0 - ibat_avg > cbm_info->cbm_param.ibat_avg_thre) && (vbat_main > vbat_aux) &&
			(vbat_main - vbat_aux > cbm_info->cbm_param.vbat_diff_thre) &&
			(vbat_aux > cbm_info->cbm_param.small_batt_vol_thre)) ? TRUE : FALSE;
	} else {
		scharger_err("%s small_batt_id:%u is invaild\n", __func__, cbm_info->cbm_param.small_batt_id);
		ret = TRUE;
	}

	return ret;
}

static void scharger_v800_discharge_monitor_work(struct work_struct *work)
{
	scharger_inf("%s\n", __func__);
	if (g_current_state == STATE_CHARGING || g_current_state == STATE_SUSPEND) {
		scharger_inf("%s current_state:%d, cancel work\n", __func__, g_current_state);
		return;
	}

	if (g_current_state == STATE_DISCHARGING_BELOW_THRE && is_delta_v_above_thre() == TRUE)
		enter_discharging_above_thre();
	if (g_current_state == STATE_DISCHARGING_ABOVE_THRE && is_delta_v_above_thre() == FALSE)
		enter_discharging_below_thre();

	queue_delayed_work(system_power_efficient_wq, &cbm_info->discharge_monitor_work,
			msecs_to_jiffies(DISCHARGE_MONITOR_WORK_DELAY));
}

static int charge_event_notifier_call(struct notifier_block *chg_nb,
				    unsigned long event, void *data)
{
	struct cur_balance_mos_info *di = container_of(chg_nb, struct cur_balance_mos_info, chg_nb);

	if (di == NULL) {
		scharger_err("%s buck is NULL\n", __func__);
		return -EINVAL;
	}

	scharger_inf("%s event:%u\n", __func__, event);

	switch (event) {
	case POWER_NE_CHG_START_CHARGING:
		scharger_v800_set_cur_balance_mos_state(di->dev_data, STATE_CHARGING);
		break;
	case POWER_NE_CHG_CHARGING_DONE:
	case POWER_NE_CHG_STOP_CHARGING:
		if (is_delta_v_above_thre())
			scharger_v800_set_cur_balance_mos_state(di->dev_data, STATE_DISCHARGING_ABOVE_THRE);
		else
			scharger_v800_set_cur_balance_mos_state(di->dev_data, STATE_DISCHARGING_BELOW_THRE);
		break;
	default:
		break;
	}

	return NOTIFY_OK;
}

void scharger_cur_balance_mos_init(struct scharger_buck *buck)
{
	scharger_inf("%s +\n", __func__);
	cbm_info = (struct cur_balance_mos_info *)devm_kzalloc(buck->dev, sizeof(struct cur_balance_mos_info), GFP_KERNEL);
	if (cbm_info == NULL) {
		scharger_err("[%s]cbm_info is null.\n", __func__);
		return;
	}
	cbm_info->dev_data = buck;
	cbm_info->chg_nb.notifier_call = charge_event_notifier_call;
	if (power_event_bnc_register(POWER_BNT_CHG, &cbm_info->chg_nb))
		scharger_err("%s charge_event_notifier_call register failed\n", __func__);
	scharger_cbm_parse_dts(buck);
	mutex_init(&cbm_info->cbm_lock);
	INIT_DELAYED_WORK(&cbm_info->discharge_monitor_work, scharger_v800_discharge_monitor_work);
}

#ifdef CONFIG_DFX_DEBUG_FS
void scharger_v800_cbm_test(bool enable)
{
	cbm_test_flag = enable;
	scharger_inf("%s set cbm_test_flag %d\n", __func__, cbm_test_flag);
}
#endif

void scharger_v800_set_cur_balance_mos_state(struct scharger_buck *buck, enum charge_state state)
{
#ifdef CONFIG_DFX_DEBUG_FS
	if (cbm_test_flag == TRUE) {
		scharger_inf("%s cbm_test_flag is TRUE\n", __func__);
		return;
	}
#endif

	scharger_inf("%s state:%d\n", __func__, state);

	mutex_lock(&cbm_info->cbm_lock);
	switch (state) {
	case STATE_CHARGING:
		cancel_delayed_work(&cbm_info->discharge_monitor_work);
		enter_charging();
		break;
	case STATE_DISCHARGING_ABOVE_THRE:
		enter_discharging_above_thre();
		queue_delayed_work(system_power_efficient_wq, &cbm_info->discharge_monitor_work,
			msecs_to_jiffies(0));
		break;
	case STATE_DISCHARGING_BELOW_THRE:
		enter_discharging_below_thre();
		queue_delayed_work(system_power_efficient_wq, &cbm_info->discharge_monitor_work,
			msecs_to_jiffies(0));
		break;
	case STATE_SUSPEND:
		enter_suspend();
		break;
	default:
		break;
	}
	mutex_unlock(&cbm_info->cbm_lock);
}