// SPDX-License-Identifier: GPL-2.0
/*
 * huawei_charger_check.c
 *
 * huawei charger check driver
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
#include <chipset_common/hwpower/common_module/power_printk.h>
#include <chipset_common/hwpower/hvdcp_charge/hvdcp_charge.h>
#include <huawei_platform/hwpower/common_module/power_platform_macro.h>
#include <huawei_platform/power/huawei_charger.h>
#include <huawei_platform/power/huawei_charger_context.h>
#include <huawei_platform/usb/hw_pd_dev.h>

#ifdef HWLOG_TAG
#undef HWLOG_TAG
#endif
#define HWLOG_TAG huawei_charger_check
HWLOG_REGIST();

#define VBUS_CHECK_END                1
#define VBUS_CHECK_CONTINUE           0
#define PD_VBUS_ABNORMAL_TIME_TH      30
#define VBUS_ABNORMAL_TIME_TH         60
#define VBUS_REPORT_NUM               4

static int charge_check_get_vbus_voltage(unsigned int *vbus)
{
	int vusb;

	if (!charge_need_vusb_replace_vbus())
		return charge_get_vbus(vbus);

	vusb = charge_get_vusb();
	*vbus = (vusb < 0) ? 0 : vusb;
	hwlog_info("%s vusb=%d\n", __func__, *vbus);
	return 0;
}

static int charge_check_vbus_ovp(void)
{
	int i, ret;
	unsigned int vbus_vol = 0;
	unsigned int vbus_ovp_cnt = 0;
	unsigned int state = CHAGRE_STATE_NORMAL;

	ret = charge_get_charging_state(&state);
	if (ret < 0) {
		hwlog_err("get_charge_state fail, ret=%d\n", ret);
		return VBUS_CHECK_END;
	}

	for (i = 0; i < VBUS_VOL_READ_CNT; ++i) {
		charge_check_get_vbus_voltage(&vbus_vol);
		if (vbus_vol > VBUS_VOLTAGE_13400_MV) {
			if (!(state & CHAGRE_STATE_NOT_PG))
				vbus_ovp_cnt++; /* if power ok, then count plus one */
			/* Wait for chargerIC to be in stable state */
			(void)power_msleep(DT_MSLEEP_25MS, 0, NULL);
		} else {
			break;
		}
	}
	if (vbus_ovp_cnt == VBUS_VOL_READ_CNT) {
		hwlog_err("[%s]vbus_vol = %u\n", __func__, vbus_vol);
		charge_state_dsm_report(ERROR_VBUS_VOL_OVER_13400MV);
	}

	return VBUS_CHECK_CONTINUE;
}

/* 
 * when the PD_UFCS adapter exits UFCS, the adapter voltage will be increased to 9V by PD,
 * which will trigger vbus OVP and should be reset to 5V.
 */
int charge_check_pd_ufcs_adapter_vbus(void)
{
	unsigned int vbus_vol = 0;
	struct charge_device_info *di = huawei_charge_get_di();

	if (!di)
		return VBUS_CHECK_END;

	if (charge_context(support_new_pd_process))
		return VBUS_CHECK_CONTINUE;

	charge_check_get_vbus_voltage(&vbus_vol);
	if (vbus_vol > VBUS_VOLTAGE_7000_MV) {
		if (adapter_set_output_voltage(ADAPTER_PROTOCOL_PD, ADAPTER_5V * POWER_MV_PER_V))
			(void)charge_set_vbus_vset(ADAPTER_9V);
		else
			(void)charge_set_vbus_vset(ADAPTER_5V);
		charge_set_input_current(di->input_current);
		hwlog_info("[%s]vbus_vol=%d mV, input_current=%d\n",
			__func__, vbus_vol, di->input_current);
		return VBUS_CHECK_END;
	}

	return VBUS_CHECK_CONTINUE;
}

#ifdef CONFIG_TCPC_CLASS
static void charge_check_pd_vbus_vset(unsigned int vbus)
{
	int vbus_vset = 0;

	if (!charge_context(support_new_pd_process) || charge_get_vbus_vset(&vbus_vset))
		return;

	switch (vbus_vset) {
	case ADAPTER_5V:
		if (vbus > VBUS_VOLTAGE_7000_MV)
			charge_set_vbus_vset(ADAPTER_9V);
		break;
	case ADAPTER_9V:
		if (vbus < VBUS_VOLTAGE_7000_MV)
			charge_set_vbus_vset(ADAPTER_5V);
		break;
	default:
		break;
	}
}

static int charge_check_pd_adapter_vbus(void)
{
	unsigned int vbus_vol = 0;
	time64_t delta_time;

	if (charge_get_charger_type() != CHARGER_TYPE_PD)
		return VBUS_CHECK_CONTINUE;

	if (!charge_context(pd_charge_flag))
		return VBUS_CHECK_END;

	charge_check_get_vbus_voltage(&vbus_vol);
	charge_check_pd_vbus_vset(vbus_vol);
	if (pd_dpm_get_high_voltage_charging_status() && (vbus_vol > VBUS_VOLTAGE_7000_MV)) {
		if (!charge_get_reset_adapter_source()) {
			charge_context(last_pd_vset) = ADAPTER_9V;
			hwlog_info("%s last_pd_vset=%d\n", __func__, charge_context(last_pd_vset));
		} else {
			if (adapter_set_output_voltage(ADAPTER_PROTOCOL_PD,
				ADAPTER_5V * POWER_MV_PER_V))
				hwlog_err("[%s]set voltage 5v failed\n", __func__);
			if (charge_context(support_new_pd_process))
				(void)charge_set_vbus_vset(ADAPTER_5V);
		}
		charge_context(pd_time) = 0;
		return VBUS_CHECK_END;
	}
	if (!pd_dpm_get_high_voltage_charging_status() && (vbus_vol < VBUS_VOLTAGE_7000_MV)) {
		if (!pd_dpm_get_optional_max_power_status() || charge_get_reset_adapter_source()) {
			charge_context(last_pd_vset) = ADAPTER_5V;
			hwlog_info("%s last_pd_vset=%d\n", __func__, charge_context(last_pd_vset));
		} else {
			if (adapter_set_output_voltage(ADAPTER_PROTOCOL_PD,
				ADAPTER_9V * POWER_MV_PER_V))
				hwlog_err("[%s]set voltage 9v failed\n", __func__);
			if (charge_context(support_new_pd_process))
				(void)charge_set_vbus_vset(ADAPTER_9V);
		}
		charge_context(pd_time) = 0;
		return VBUS_CHECK_END;
	}

	if (!charge_context(pd_time)) {
		charge_context(pd_time) = power_get_monotonic_boottime();
		hwlog_err("[%s]pd vbus abnormal, time=%lld\n", __func__, charge_context(pd_time));
	} else {
		delta_time = power_get_monotonic_boottime() - charge_context(pd_time);
		if (delta_time >= PD_VBUS_ABNORMAL_TIME_TH) {
			hwlog_err("[%s]pd vbus abnormal over 30s, hard_reset\n", __func__);
			adapter_hard_reset_master(ADAPTER_PROTOCOL_PD);
		}
	}

	return VBUS_CHECK_END;
}
#else
static bool charge_check_pd_adapter_vbus(void)
{
	return VBUS_CHECK_CONTINUE;
}
#endif /* CONFIG_TCPC_CLASS */

static void charge_check_fcp_uvp(struct charge_device_info *di)
{
	unsigned int vbus_vol = 0;
	time64_t delta_time = 0;

	charge_check_get_vbus_voltage(&vbus_vol);
	if (vbus_vol < VBUS_VOLTAGE_7000_MV) {
		if (!charge_context(fcp_time)) {
			charge_context(fcp_time) = power_get_monotonic_boottime();
			hwlog_err("[%s]fcp vbus abnormal, time=%lld\n", __func__,
				charge_context(fcp_time));
		} else {
			delta_time = power_get_monotonic_boottime() - charge_context(fcp_time);
		}
	} else {
		charge_context(fcp_time) = 0;
	}

	if (delta_time >= VBUS_ABNORMAL_TIME_TH) {
		charge_context(fcp_vbus_low_cnt)++;
		hvdcp_check_adapter_status();
		hvdcp_set_charging_stage(HVDCP_STAGE_DEFAUTL);
		charge_set_charger_type(CHARGER_TYPE_STANDARD);
		if (hvdcp_reset_adapter())
			hwlog_err("adapter reset failed\n");
		(void)charge_set_vbus_vset(ADAPTER_5V);
		hwlog_err("[%s]fcp vbus abnormal over 60s, reset adapter to 5v\n", __func__);
	}

	if (charge_context(fcp_vbus_low_cnt) >= VBUS_REPORT_NUM) {
		charge_context(fcp_vbus_low_cnt) = 0;
		charge_state_dsm_report(ERROR_FCP_VOL_OVER_HIGH);
	}

	charge_context(nonfcp_time) = 0;
}

static void charge_check_nonfcp_ovp(struct charge_device_info *di)
{
	unsigned int vbus_vol = 0;
	time64_t delta_time = 0;
	int ret;

	charge_check_get_vbus_voltage(&vbus_vol);
	if (vbus_vol > VBUS_VOLTAGE_6500_MV) {
		if (!charge_context(nonfcp_time)) {
			charge_context(nonfcp_time) = power_get_monotonic_boottime();
			hwlog_err("[%s]nonfcp vbus abnormal, time=%lld\n", __func__,
				charge_context(nonfcp_time));
		} else {
			delta_time = power_get_monotonic_boottime() - charge_context(nonfcp_time);
		}
	} else {
		charge_context(nonfcp_time) = 0;
	}

	if (delta_time >= VBUS_ABNORMAL_TIME_TH) {
		di->charge_enable = FALSE;
		charge_state_dsm_report(ERROR_FCP_VOL_OVER_HIGH);
		ret = adapter_is_accp_charger_type(ADAPTER_PROTOCOL_FCP);
		if (ret) {
			if (hvdcp_reset_adapter())
				hwlog_err("adapter reset failed\n");
			hwlog_info("[%s]is fcp adapter\n", __func__);
		} else {
			hwlog_info("[%s]is not fcp adapter\n", __func__);
		}
		hwlog_err("[%s]nonfcp vbus abnormal over 60s, need disable charge\n", __func__);
	}

	charge_context(fcp_time) = 0;
}

static void charge_check_adapter_vbus(struct charge_device_info *di)
{
	if (hvdcp_get_charging_stage() == HVDCP_STAGE_SUCCESS)
		charge_check_fcp_uvp(di);
	else
		charge_check_nonfcp_ovp(di);
}

void charge_check_vbus_voltage(void)
{
	struct charge_device_info *di = huawei_charge_get_di();

	if (!di || !charge_ops_valid(get_vbus))
		return;

	if (charge_ops_valid(set_covn_start) && charge_set_covn_start(true)) {
		hwlog_err("[%s]set covn start fail\n", __func__);
		return;
	}

	if (charge_check_vbus_ovp())
		return;

	if (((adapter_detect_get_runtime_protocol_type() == BIT(ADAPTER_PROTOCOL_UFCS)) ||
		(adapter_detect_get_runtime_protocol_type() == BIT(ADAPTER_PROTOCOL_PPS))) &&
		pd_dpm_get_ctc_cable_flag() && (charge_get_charger_type() == CHARGER_TYPE_STANDARD)) {
		if (charge_check_pd_ufcs_adapter_vbus())
			return;
	}

	if (charge_check_pd_adapter_vbus())
		return;

#ifdef CONFIG_WIRELESS_CHARGER
	if ((charge_get_charger_type() == CHARGER_TYPE_WIRELESS) ||
		(charge_get_reset_adapter_source() & BIT(RESET_ADAPTER_SOURCE_WLTX)))
		return;
#endif

	charge_check_adapter_vbus(di);
}

void charge_check_reset_vbus_check_info(void)
{
	charge_context(fcp_vbus_low_cnt) = 0;
	charge_context(nonfcp_time) = 0;
	charge_context(fcp_time) = 0;
	charge_context(pd_time) = 0;
}
