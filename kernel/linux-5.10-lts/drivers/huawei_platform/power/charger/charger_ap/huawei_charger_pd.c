// SPDX-License-Identifier: GPL-2.0
/*
 * huawei_charger_pd.c
 *
 * huawei charger pd driver
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

#include <chipset_common/hwpower/adapter/adapter_detect.h>
#include <chipset_common/hwpower/buck_charge/buck_charge_ic_manager.h>
#include <chipset_common/hwpower/charger/charger_common_interface.h>
#include <chipset_common/hwpower/common_module/power_common_macro.h>
#include <chipset_common/hwpower/common_module/power_delay.h>
#include <chipset_common/hwpower/common_module/power_icon.h>
#include <chipset_common/hwpower/common_module/power_printk.h>
#include <chipset_common/hwpower/hvdcp_charge/hvdcp_charge.h>
#include <huawei_platform/hwpower/common_module/power_platform.h>
#include <huawei_platform/hwpower/common_module/power_platform_macro.h>
#include <huawei_platform/power/huawei_charger.h>
#include <huawei_platform/power/huawei_charger_context.h>
#include <huawei_platform/usb/hw_pd_dev.h>

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif
#define HWLOG_TAG huawei_charger_pd
HWLOG_REGIST();

DECLARE_COMPLETION(g_emark_detect_comp);

void emark_detect_complete(void)
{
	complete(&g_emark_detect_comp);
}

bool charge_pd_bat_vol_support_9v(void)
{
	int vbat = 0;

	if (charge_context(buck_9v_enter_th) <= 0)
		return true;

	power_supply_get_int_prop(POWER_PLATFORM_BAT_PSY_NAME,
		POWER_SUPPLY_PROP_VOLTAGE_NOW, &vbat, 0, POWER_UV_PER_MV);

	if (vbat < charge_context(buck_9v_enter_th))
		return false;

	return true;
}

static int charge_pd_disable_usbpd(bool disable)
{
	bool dfg_online = power_platform_check_online_status();

	(void)charge_set_vbus_vset(ADAPTER_5V);

	adapter_set_usbpd_enable(ADAPTER_PROTOCOL_SCP, !disable, dfg_online ? true : false);
	pd_dpm_disable_pd(disable);
	hwlog_info("%s\n", __func__);
	return 0;
}

#ifdef CONFIG_DIRECT_CHARGER
static void charge_pd_switch_type_to_standard(void)
{
	if (charge_get_monitor_work_flag() == CHARGE_MONITOR_WORK_NEED_STOP)
		return;

	charge_context(pd_charge_flag) = false;
	charge_context(pd_time) = 0;
	charge_set_charger_type(CHARGER_TYPE_STANDARD);
	charge_set_charger_source(POWER_SUPPLY_TYPE_MAINS);
	hwlog_info("%s CHARGER_TYPE_STANDARD\n", __func__);
}

static void _charge_pd_try_to_dc(void)
{
	int time = pd_dpm_support_cable_auth() ? EMARK_WITH_CABLE_AUTH : EAMRK_WAIT_TIME;

	if (direct_charge_in_charging_stage() == DC_IN_CHARGING_STAGE) {
		hwlog_info("ignore pre_check\n");
		charge_pd_switch_type_to_standard();
		charge_context(try_pd_to_dc_cnt) = 0;
	} else if (!direct_charge_pre_check()) {
		if (adapter_detect_get_runtime_protocol_type() == BIT(ADAPTER_PROTOCOL_UFCS)) {
			hw_pd_set_hv_en(false);
			time = (time > EMARK_WAIT_TIME_UFCS) ? time : EMARK_WAIT_TIME_UFCS;
		}
		if (pd_dpm_get_emark_detect_enable()) {
			reinit_completion(&g_emark_detect_comp);
			pd_dpm_detect_emark_cable();
			/* wait time to get cable vdo */
			if (!wait_for_completion_timeout(&g_emark_detect_comp,
				msecs_to_jiffies(time)))
				hwlog_info("emark timeout\n");
			reinit_completion(&g_emark_detect_comp);
			pd_dpm_detect_emark_cable_finish();
		}

		if ((direct_charge_in_charging_stage() == DC_NOT_IN_CHARGE_DONE_STAGE) &&
			(adapter_detect_get_runtime_protocol_type() == BIT(ADAPTER_PROTOCOL_SCP)))
			charge_pd_disable_usbpd(true);
		charge_pd_switch_type_to_standard();
		charge_context(try_pd_to_dc_cnt) = 0;
	} else {
		charge_context(try_pd_to_dc_cnt)--;
	}

	if (charge_context(try_pd_to_dc_cnt) < 0)
		charge_context(try_pd_to_dc_cnt) = 0;
}

static void _charge_pd_try_to_dc_new(void)
{
	if (!charge_context(pd_done)) {
		hwlog_info("%s pd not done, wait\n", __func__);
		return;
	}

	if (direct_charge_in_charging_stage() == DC_IN_CHARGING_STAGE) {
		hwlog_info("ignore pre_check\n");
		charge_pd_switch_type_to_standard();
		charge_context(try_pd_to_dc_cnt) = 0;
	} else if (!direct_charge_pre_check()) {
		if (direct_charge_in_charging_stage() == DC_NOT_IN_CHARGE_DONE_STAGE &&
			adapter_detect_get_runtime_protocol_type() == BIT(ADAPTER_PROTOCOL_SCP))
			charge_pd_disable_usbpd(true);
		charge_pd_switch_type_to_standard();
		charge_context(try_pd_to_dc_cnt) = 0;
	} else {
		charge_context(try_pd_to_dc_cnt)--;
		hwlog_info("%s try_pd_to_scp=%d\n", __func__, charge_context(try_pd_to_dc_cnt));
		if (!charge_context(try_pd_to_dc_cnt))
			charge_context(pd_adapter_support_dc) = false;
	}

	if (charge_context(try_pd_to_dc_cnt) < 0)
		charge_context(try_pd_to_dc_cnt) = 0;
}

void charge_pd_try_to_dc(void)
{
	if (!pd_dpm_get_ctc_cable_flag() || (charge_context(try_pd_to_dc_cnt) <= 0))
		return;

	charge_context(wait_pd_init) = false;

	if (charge_get_monitor_work_flag() == CHARGE_MONITOR_WORK_NEED_STOP) {
		charge_context(try_pd_to_dc_cnt) = 0;
		return;
	}

	hwlog_info("%s try_pd_to_scp\n", __func__);
	if (charge_context(support_new_pd_process))
		_charge_pd_try_to_dc_new();
	else
		_charge_pd_try_to_dc();
}
#else
void charge_pd_try_to_dc(void)
{
	charge_context(pd_adapter_support_dc) = false;
}
#endif /* CONFIG_DIRECT_CHARGER */

static void charge_pd_switch_type_to_fcp(void)
{
	if (charge_get_monitor_work_flag() == CHARGE_MONITOR_WORK_NEED_STOP)
		return;

	charge_context(pd_charge_flag) = false;
	charge_context(pd_time) = 0;
	charge_set_charger_type(CHARGER_TYPE_FCP);
	charge_set_charger_source(POWER_SUPPLY_TYPE_MAINS);
	hwlog_info("%s CHARGER_TYPE_FCP\n", __func__);
}

static void _charge_pd_try_to_fcp(void)
{
	hwlog_info("%s try_pd_to_fcp\n", __func__);
	if (hvdcp_get_charging_stage() == HVDCP_STAGE_SUCCESS) {
		hwlog_info("ignore check fcp\n");
		charge_pd_switch_type_to_fcp();
		charge_context(try_pd_to_fcp_cnt) = 0;
	} else if (hvdcp_detect_adapter()== ADAPTER_DETECT_SUCC) {
		if (pd_dpm_get_emark_detect_enable()) {
			pd_dpm_detect_emark_cable();
			/* wait 200ms to get cable vdo */
			if (!wait_for_completion_timeout(&g_emark_detect_comp, 
				msecs_to_jiffies(200)))
				hwlog_info("emark timeout\n");

			reinit_completion(&g_emark_detect_comp);
			pd_dpm_detect_emark_cable_finish();
		}

		charge_pd_disable_usbpd(true);
		msleep(800); /* Wait 800ms for BC1.2 complete */
		charge_pd_switch_type_to_fcp();
		charge_context(try_pd_to_fcp_cnt) = 0;
	} else {
		charge_context(try_pd_to_fcp_cnt)--;
	}

	if (charge_context(try_pd_to_fcp_cnt) < 0)
		charge_context(try_pd_to_fcp_cnt) = 0;
}

void charge_pd_try_to_fcp(void)
{
	if (!pd_dpm_get_ctc_cable_flag())
		return;

	if ((charge_context(try_pd_to_fcp_cnt) <= 0) || !power_platform_check_online_status() ||
		(charge_get_charger_type() == CHARGER_TYPE_STANDARD))
		return;

	charge_context(wait_pd_init) = false;

	if (charge_get_monitor_work_flag() == CHARGE_MONITOR_WORK_NEED_STOP) {
		charge_context(try_pd_to_fcp_cnt) = 0;
		return;
	}

	_charge_pd_try_to_fcp();
}

#ifdef CONFIG_TCPC_CLASS
static bool charge_pd_check_support_9v(void)
{
	if (!charge_context(pd_done)) {
		hwlog_info("%s pd not done, can not enter 9v\n", __func__);
		return false;
	}

	if (charge_context(pd_adapter_support_dc)) {
		hwlog_info("%s adapter support dc\n", __func__);
		return false;
	}

	if (!adapter_support_9v(ADAPTER_PROTOCOL_PD)) {
		hwlog_info("%s adapter not support 9v pd\n", __func__);
		return false;
	}

	if (charge_get_reset_adapter_source()) {
		hwlog_info("%s disable support 9v pd\n", __func__);
		return false;
	}

	if (charge_context(try_pd_to_9v_err_cnt) >= PD_TO_9V_ERR_MAX_COUNT) {
		hwlog_err("%s try pd to 9v exceed %d\n", __func__, charge_context(try_pd_to_9v_err_cnt));
		return false;
	}

	return true;
}

static void _charge_pd_check_new(void)
{
	int ret;

	if (!charge_pd_check_support_9v())
		return;

	charge_context(init_data).vbus = ADAPTER_9V;
	charge_context(init_data).charger_type = charge_get_charger_type();
	(void)charge_init_chip(&charge_context(init_data));

#ifdef CONFIG_PD_REV30_CERTIFICATION
	if (hw_pd_is_certification_hiz_en())
		charger_set_hiz(true);
#endif

	hwlog_info("%s set pd 9v start +++\n", __func__);
	ret = adapter_set_output_voltage(ADAPTER_PROTOCOL_PD, ADAPTER_9V * POWER_MV_PER_V);
	hwlog_info("%s set pd 9v end ---\n", __func__);
	if (ret)
		goto adapter_set_9v_err;
	do {
		ret = charge_set_vbus_vset(ADAPTER_9V);
		if (!ret)
			break;
		charge_context(try_pd_to_9v_err_cnt)++;
	} while (charge_context(try_pd_to_9v_err_cnt) < PD_TO_9V_ERR_MAX_COUNT);

	if (charge_context(try_pd_to_9v_err_cnt) < PD_TO_9V_ERR_MAX_COUNT) {
		charge_context(pd_charge_flag) = true;
		hwlog_info("%s: ok\n", __func__);
		return;
	}

	(void)adapter_set_output_voltage(ADAPTER_PROTOCOL_PD, ADAPTER_5V * POWER_MV_PER_V);
adapter_set_9v_err:
	charge_context(init_data).vbus = ADAPTER_5V;
	charge_context(init_data).charger_type = charge_get_charger_type();
	(void)charge_init_chip(&charge_context(init_data));
}

static void _charge_pd_check(void)
{
	if (pd_dpm_get_high_voltage_charging_status() == true) {
		charge_context(init_data).vbus = ADAPTER_9V;
		if (pd_judge_is_cover() == true)
			power_icon_notify(ICON_TYPE_WIRELESS_QUICK);
		else
			power_icon_notify(ICON_TYPE_QUICK);
	} else {
		charge_context(init_data).vbus = ADAPTER_5V;
	}
	charge_context(init_data).charger_type = charge_get_charger_type();
	(void)charge_init_chip(&charge_context(init_data));

#ifdef CONFIG_PD_REV30_CERTIFICATION
	if (hw_pd_is_certification_hiz_en())
		charger_set_hiz(true);
#endif

	if (pd_dpm_get_high_voltage_charging_status())
		(void)charge_set_vbus_vset(ADAPTER_9V);
	else
		(void)charge_set_vbus_vset(ADAPTER_5V);

	charge_context(pd_charge_flag) = true;
	hwlog_info("%s: ok\n", __func__);
}

void charge_pd_check(void)
{
	if (charge_context(pd_charge_flag))
		return;
	if (charge_get_monitor_work_flag() == CHARGE_MONITOR_WORK_NEED_STOP)
		return;
	if ((charge_get_charger_type() != CHARGER_TYPE_PD) || !power_platform_is_battery_exit())
		return;

	if (dc_adpt_get_inherent_info(ADAP_ANTIFAKE_RESULT) == ADAPTER_ANTIFAKE_FAIL) {
		hwlog_info("[%s] adapter is fake\n", __func__);
		return;
	}

	if (!charge_pd_bat_vol_support_9v()) {
		hwlog_info("can not enter pd 9v beacuse vbat is low\n");
		return;
	}

	if (charge_context(support_new_pd_process))
		_charge_pd_check_new();
	else
		_charge_pd_check();
}

static void _charge_pd_voltage_change_new(int vset)
{
	int ret;

	if (vset == charge_context(last_pd_vset)) {
		hwlog_info("%s pd voltage is same with last set, not need change\n", __func__);
		return;
	}

	if (charge_get_charger_type() != CHARGER_TYPE_PD) {
		hwlog_info("%s not pd charge type now, return\n", __func__);
		return;
	}

	if ((vset == ADAPTER_9V) && !charge_pd_bat_vol_support_9v()) {
		hwlog_info("can not switch to pd 9v beacuse vbat is low\n");
		return;
	}

	if ((vset == ADAPTER_5V) || ((vset == ADAPTER_9V) && charge_context(pd_charge_flag))) {
		ret = adapter_set_output_voltage(ADAPTER_PROTOCOL_PD, vset * POWER_MV_PER_V);
		if (ret)
			hwlog_err("%s set voltage failed\n", __func__);
		ret = charge_set_vbus_vset(vset);
		if (ret)
			hwlog_err("%s set vbus vset failed\n", __func__);
	}
}

static void _charge_pd_voltage_change(int vset)
{
	int ret;

	if (vset == charge_context(last_pd_vset)) {
		hwlog_info("%s pd voltage is same with last set, not need change\n", __func__);
		return;
	}

	ret = adapter_set_output_voltage(ADAPTER_PROTOCOL_PD, vset * POWER_MV_PER_V);
	if (ret)
		hwlog_err("[%s]set voltage failed\n", __func__);
}

static void charge_pd_voltage_change_work(struct work_struct *work)
{
	int vset = charge_get_reset_adapter_source() ? ADAPTER_5V : ADAPTER_9V;

	if (charge_context(support_new_pd_process))
		_charge_pd_voltage_change_new(vset);
	else
		_charge_pd_voltage_change(vset);
}
#else
void charge_pd_check(void)
{
}

static void charge_pd_voltage_change_work(struct work_struct *work)
{
}
#endif

void charge_pd_init(struct delayed_work *pd_voltage_change_work)
{
	if (!pd_voltage_change_work)
		return;

	INIT_DELAYED_WORK(pd_voltage_change_work, charge_pd_voltage_change_work);
}

void charge_pd_clear_data(void)
{
	charge_context(pd_charge_flag) = false;
	charge_context(pd_done) = false;
	charge_context(pd_adapter_support_dc) = true;
	charge_context(pd_to_dc_flag) = false;
	charge_context(try_pd_to_dc_cnt) = 0;
	charge_context(try_pd_to_fcp_cnt) = 0;
	charge_context(try_pd_to_9v_err_cnt) = 0;
	charge_set_reset_adapter_source(RESET_ADAPTER_CLEAR_MODE, RESET_ADAPTER_SOURCE_DISABLE_9V);
}
