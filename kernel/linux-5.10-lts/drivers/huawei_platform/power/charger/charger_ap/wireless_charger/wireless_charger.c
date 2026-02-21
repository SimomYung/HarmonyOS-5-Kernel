#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/time.h>
#include <huawei_platform/log/hw_log.h>
#include <chipset_common/hwpower/common_module/power_algorithm.h>
#include <chipset_common/hwpower/common_module/power_delay.h>
#include <chipset_common/hwpower/common_module/power_common_macro.h>
#include <chipset_common/hwpower/common_module/power_time.h>
#include <chipset_common/hwpower/common_module/power_wakeup.h>
#include <chipset_common/hwpower/hardware_ic/charge_pump.h>
#include <huawei_platform/power/huawei_charger.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_module.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_common.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_ic_intf.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_dts.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_status.h>
#include <chipset_common/hwpower/wireless_charge/wireless_buck_ictrl.h>
#include <huawei_platform/power/wireless/wireless_charger.h>
#include <huawei_platform/power/wireless/wireless_transmitter.h>
#include <chipset_common/hwpower/wireless_charge/wireless_dc_check.h>
#include <huawei_platform/power/wireless/wireless_direct_charger.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_alarm.h>
#include <huawei_platform/power/charging_core.h>
#include <chipset_common/hwpower/hardware_channel/wired_channel_switch.h>
#include <platform_include/basicplatform/linux/powerkey_event.h>
#include <chipset_common/hwpower/hardware_channel/vbus_channel.h>
#include <chipset_common/hwpower/hardware_ic/boost_5v.h>
#include <chipset_common/hwpower/wireless_charge/wireless_power_supply.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_sysfs.h>
#include <chipset_common/hwpower/wireless_charge/wireless_acc_types.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_acc.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_ui.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_pctrl.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_plim.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_interfere.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_pmode.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_fod.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_monitor.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_event.h>
#ifdef CONFIG_TCPC_CLASS
#include <huawei_platform/usb/hw_pd_dev.h>
#endif
#include <huawei_platform/power/wireless/wireless_keyboard.h>
#include <huawei_platform/hwpower/common_module/power_platform.h>
#include <huawei_platform/hwpower/wireless_charge/wireless_rx_platform.h>
#include <chipset_common/hwpower/hardware_channel/charger_channel.h>
#include <chipset_common/hwpower/hardware_channel/power_sw.h>
#include <chipset_common/hwpower/common_module/power_supply_interface.h>
#include <chipset_common/hwpower/wireless_charge/wireless_tx_pwr_src.h>
#include <chipset_common/hwpower/wireless_charge/wireless_test_wp.h>

#define HWLOG_TAG wireless_charger
HWLOG_REGIST();

static struct wlrx_dev_info *g_wlrx_di;
static struct wakeup_source *g_rx_con_wakelock;
static struct mutex g_rx_en_mutex;
static int g_fop_fixed_flag;
static int g_rx_vrect_restore_cnt;
static int g_rx_vout_err_cnt;
static int g_plimit_time_num;
static bool g_need_force_5v_vout;

int wireless_charge_get_rx_iout_limit(void)
{
	struct wlrx_dev_info *di = g_wlrx_di;

	if (!di) {
		hwlog_err("%s: di null\n", __func__);
		return 0;
	}

	return min(wlrx_pctrl_get_para(di->drv_type, WLRX_PCTRL_IRX), di->rx_iout_limit);
}

static void wlc_rx_chip_reset(struct wlrx_dev_info *di)
{
	if (di->wlc_err_rst_cnt >= WLC_RST_CNT_MAX)
		return;

	wlrx_ic_chip_reset(WLTRX_IC_MAIN);
	di->wlc_err_rst_cnt++;
	wlrx_set_discon_delay(WLTRX_DRV_MAIN, WL_RST_DISCONN_DELAY_MS);
}

static void wireless_charge_set_input_current(unsigned int drv_type)
{
	wlrx_buck_set_iin_for_rx(drv_type, wireless_charge_get_rx_iout_limit());
}

static int wireless_charge_fix_tx_fop(int fop)
{
	struct wlrx_dev_info *di = g_wlrx_di;

	if (!di) {
		hwlog_err("%s: di null\n", __func__);
		return -1;
	}
	if (wlrx_get_wireless_channel_state() == WIRELESS_CHANNEL_OFF) {
		hwlog_err("%s: not in wireless charging\n", __func__);
		return -1;
	}

	return wireless_fix_tx_fop(WLTRX_IC_MAIN, WIRELESS_PROTOCOL_QI, fop);
}

static int wireless_charge_unfix_tx_fop(void)
{
	struct wlrx_dev_info *di = g_wlrx_di;

	if (!di) {
		hwlog_err("%s: di null\n", __func__);
		return -1;
	}
	if (wlrx_get_wireless_channel_state() == WIRELESS_CHANNEL_OFF) {
		hwlog_err("%s: not in wireless charging\n", __func__);
		return -1;
	}

	return wireless_unfix_tx_fop(WLTRX_IC_MAIN, WIRELESS_PROTOCOL_QI);
}

static void wireless_charge_count_avg_iout(struct wlrx_dev_info *di)
{
	int cnt_max;
	int iavg = 0;
	int rx_low_iout = RX_LOW_IOUT;
	struct wlrx_dts *dts = wlrx_get_dts();

	if (di->monitor_interval <= 0)
		return;

	if (dts && (dts->rx_low_iout > 0))
		rx_low_iout = dts->rx_low_iout;

	cnt_max = RX_AVG_IOUT_TIME / di->monitor_interval;

	wlrx_ic_get_iavg(WLTRX_IC_MAIN, &iavg);
	if (wlrx_pmode_bst_rst_completed(di->drv_type) && (iavg < rx_low_iout)) {
		di->iout_low_cnt++;
		if (di->iout_low_cnt >= cnt_max) {
			di->iout_low_cnt = cnt_max;
			di->iout_high_cnt = 0;
		}
		return;
	}

	if ((iavg > RX_HIGH_IOUT) || wlrx_pmode_in_dc_mode(di->drv_type)) {
		di->iout_high_cnt++;
		if (di->iout_high_cnt >= cnt_max) {
			di->iout_high_cnt = cnt_max;
			di->iout_low_cnt = 0;
		}
		return;
	}
}

void wlc_ignore_vbus_only_event(bool ignore_flag)
{
	struct wlrx_dts *dts = wlrx_get_dts();

	if (!dts || !dts->bst5v_ignore_vbus_only)
		return;

#ifdef CONFIG_TCPC_CLASS
	if (ignore_flag)
		pd_dpm_ignore_vbus_only_event(true);
	else if (!boost_5v_status(BOOST_CTRL_WLC) && !boost_5v_status(BOOST_CTRL_WLDC))
		pd_dpm_ignore_vbus_only_event(false);
#endif
}

static void wlc_extra_power_supply(bool enable)
{
	int ret;
	static bool boost_5v_flag;
	struct wlrx_dev_info *di = g_wlrx_di;
	struct wlrx_dts *dts = wlrx_get_dts();

	if (!di || !dts || !dts->hvc_need_5vbst)
		return;

	if (enable && (di->supported_rx_vout > RX_HIGH_VOUT)) {
		ret = boost_5v_enable(BOOST_5V_ENABLE, BOOST_CTRL_WLC);
		if (ret) {
			hwlog_err("%s: boost_5v enable fail\n", __func__);
			di->extra_pwr_good_flag = 0;
			return;
		}
		wlc_ignore_vbus_only_event(true);
		boost_5v_flag = true;
	} else if (!enable && boost_5v_flag) {
		ret = boost_5v_enable(BOOST_5V_DISABLE, BOOST_CTRL_WLC);
		if (ret) {
			hwlog_err("%s: boost_5v disable fail\n", __func__);
			return;
		}
		wlc_ignore_vbus_only_event(false);
		boost_5v_flag = false;
	}
}

static void wireless_charge_set_ctrl_interval(struct wlrx_dev_info *di)
{
	if (wlrx_get_charge_stage() < WLRX_STAGE_REGULATION)
		di->ctrl_interval = CONTROL_INTERVAL_NORMAL;
	else
		di->ctrl_interval = CONTROL_INTERVAL_FAST;
}

void wlrx_set_iout_min(void)
{
	wlrx_pctrl_set_para(WLTRX_DRV_MAIN, WLRX_PCTRL_IRX_MIN, 0);
	wireless_charge_set_input_current(WLTRX_DRV_MAIN);
}

static void wlc_update_high_fop_para(struct wlrx_dev_info *di)
{
	int fop = 0;
	struct wlprot_acc_cap *acc_cap = wlrx_acc_get_cap(WLTRX_DRV_MAIN);

	if (!acc_cap || !acc_cap->support_fop_range)
		return;

	(void)wlrx_ic_get_fop(WLTRX_IC_MAIN, &fop);
	if (fop > WLRX_ACC_NORMAL_MAX_FOP) {
		wlrx_pctrl_set_para(di->drv_type, WLRX_PCTRL_VTX, VOUT_9V_STAGE_MAX);
		wlrx_pctrl_set_para(di->drv_type, WLRX_PCTRL_VRX, VOUT_9V_STAGE_MAX);
	}
}

static void wlc_update_iout_low_para(struct wlrx_dev_info *di, bool ignore_cnt_flag)
{
	struct wlprot_acc_cap *acc_cap = wlrx_acc_get_cap(WLTRX_DRV_MAIN);

	if (ignore_cnt_flag || (di->monitor_interval == 0))
		return;
	if (di->iout_low_cnt < RX_AVG_IOUT_TIME / di->monitor_interval)
		return;
	if (acc_cap && acc_cap->support_fop_range &&
		(wlrx_intfr_get_fixed_fop(WLTRX_DRV_MAIN) >= WLRX_ACC_NORMAL_MAX_FOP))
		return;
	wlrx_pctrl_set_para(di->drv_type, WLRX_PCTRL_VTX, TX_DEFAULT_VOUT);
	wlrx_pctrl_set_para(di->drv_type, WLRX_PCTRL_VRX, RX_DEFAULT_VOUT);
	wlrx_pctrl_set_para(di->drv_type, WLRX_PCTRL_IRX, RX_DEFAULT_IOUT);
}

static void wlc_update_tx_alarm_vmax(struct wlrx_dev_info *di)
{
	int vmax;

	vmax = wlrx_get_alarm_vlim();
	if (vmax <= 0)
		return;

	wlrx_pctrl_set_para(di->drv_type, WLRX_PCTRL_VTX, vmax);
	wlrx_pctrl_set_para(di->drv_type, WLRX_PCTRL_VRX, vmax);
}

void wireless_charge_update_max_vout_and_iout(bool ignore_cnt_flag)
{
	struct wlrx_dev_info *di = g_wlrx_di;

	if (!di)
		return;

	wlrx_pctrl_update_para(di->drv_type);

	if (wlrx_in_pwroff_reset(di->drv_type) || !di->extra_pwr_good_flag) {
		wlrx_pctrl_set_para(di->drv_type, WLRX_PCTRL_VTX, TX_DEFAULT_VOUT);
		wlrx_pctrl_set_para(di->drv_type, WLRX_PCTRL_VRX, RX_DEFAULT_VOUT);
		wlrx_pctrl_set_para(di->drv_type, WLRX_PCTRL_IRX, RX_DEFAULT_IOUT);
	}
	wlc_update_tx_alarm_vmax(di);
	wlc_update_high_fop_para(di);
	wlc_update_iout_low_para(di, ignore_cnt_flag);
}

void wlrx_wait_fop_fix_to_default(int vtx)
{
	int i;

	if ((vtx >= RX_HIGH_VOUT) || (g_fop_fixed_flag <= WLRX_ACC_NORMAL_MAX_FOP))
		return;

	hwlog_info("%s\n", __func__);
	g_need_force_5v_vout = true;
	/* delay 60*50 = 3000ms for direct charger check finish */
	for (i = 0; i < 60; i++) {
		if (g_fop_fixed_flag <= WLRX_ACC_NORMAL_MAX_FOP)
			break;
		if (!power_msleep(DT_MSLEEP_50MS, DT_MSLEEP_25MS,
			wlrx_msleep_exit))
			break;
	}
}

static void wlc_update_imax_by_tx_plimit(struct wlrx_dev_info *di)
{
	int vrx;
	int ilim;

	vrx = wlrx_pmode_get_curr_int_cfg(di->drv_type, WLRX_PMODE_VRX);
	ilim = wlrx_get_alarm_ilim(vrx);
	if (ilim <= 0)
		return;

	wlrx_pctrl_set_para(di->drv_type, WLRX_PCTRL_IRX, ilim);
}

static void wlc_update_ilim_by_low_vrect(struct wlrx_dev_info *di)
{
	static int rx_vrect_low_cnt;
	int cnt_max;
	int vrect = 0;
	int charger_iin_regval = wlrx_buck_get_iin_regval(WLTRX_DRV_MAIN);
	struct wlrx_dts *dts = wlrx_get_dts();
	int vrect_lth;

	if (!dts || (di->ctrl_interval <= 0))
		return;

	(void)wlrx_ic_get_vrect(WLTRX_IC_MAIN, &vrect);
	cnt_max = RX_VRECT_LOW_RESTORE_TIME / di->ctrl_interval;
	vrect_lth = wlrx_pmode_get_curr_int_cfg(di->drv_type, WLRX_PMODE_VRECT_TH);
	if (vrect < vrect_lth) {
		if (++rx_vrect_low_cnt >= RX_VRECT_LOW_CNT) {
			rx_vrect_low_cnt = RX_VRECT_LOW_CNT;
			hwlog_err("update_ilim_by_low_vrect: vrect:%d<lth:%d,decrease irx:%d\n",
				vrect, vrect_lth, dts->rx_istep);
			di->rx_iout_limit = max(RX_VRECT_LOW_IOUT_MIN,
				charger_iin_regval - dts->rx_istep);
			g_rx_vrect_restore_cnt = cnt_max;
		}
	} else if (g_rx_vrect_restore_cnt > 0) {
		rx_vrect_low_cnt = 0;
		g_rx_vrect_restore_cnt--;
		di->rx_iout_limit = charger_iin_regval;
	} else {
		rx_vrect_low_cnt = 0;
	}
}

static void wlc_update_iout_para(struct wlrx_dev_info *di)
{
	wlrx_buck_update_ictrl_para(WLTRX_DRV_MAIN, &di->rx_iout_limit);
	wlc_update_ilim_by_low_vrect(di);
	wlc_update_imax_by_tx_plimit(di);
}

static void wlc_iout_control(struct wlrx_dev_info *di)
{
	struct wlprot_acc_cap *acc_cap = wlrx_acc_get_cap(WLTRX_DRV_MAIN);

	if (!acc_cap || (wlrx_get_charge_stage() == WLRX_STAGE_REGULATION_DC) ||
		(wlrx_get_wireless_channel_state() == WIRELESS_CHANNEL_OFF))
		return;

	if (wlrx_in_pwroff_reset(di->drv_type))
		return;

	wlrx_pctrl_set_para(di->drv_type, WLRX_PCTRL_IRX,
		wlrx_pmode_get_curr_int_cfg(di->drv_type, WLRX_PMODE_IRX));
	wlrx_pctrl_set_para(di->drv_type, WLRX_PCTRL_IRX, acc_cap->imax);
	di->rx_iout_limit = wlrx_pctrl_get_para(di->drv_type, WLRX_PCTRL_IRX);

	if (wlrx_is_fac_tx(WLTRX_DRV_MAIN)) {
		if (wp_get_sample_type() <= WLRX_SAMPLE_WORK_IDLE)
			wp_start_sample_iout();
		if (wp_get_sample_type() == WLRX_SAMPLE_WORK_RUNNING) {
			di->rx_iout_limit = wlrx_pctrl_get_para(di->drv_type, WLRX_PCTRL_IRX);
			wireless_charge_set_input_current(WLTRX_DRV_MAIN);
			return;
		}
	}

	wlc_update_iout_para(di);
	wireless_charge_set_input_current(WLTRX_DRV_MAIN);
}

static int wlc_high_fop_vout_check(void)
{
	int rx_vout = 0;
	int vfc_reg = 0;

	(void)wlrx_ic_get_vout(WLTRX_IC_MAIN, &rx_vout);
	(void)wlrx_ic_get_vfc_reg(WLTRX_IC_MAIN, &vfc_reg);
	if ((rx_vout < VOUT_9V_STAGE_MIN) || (rx_vout > VOUT_9V_STAGE_MAX) ||
		(vfc_reg < VOUT_9V_STAGE_MIN) || (vfc_reg > VOUT_9V_STAGE_MAX))
		return -1;

	return 0;
}

static int wireless_charge_fop_fix_check(bool force_flag, int fixed_fop)
{
	if ((fixed_fop <= 0) || (g_fop_fixed_flag == fixed_fop))
		return 0;

	/* reset tx to 9V for high fop; else delay 40*100ms for limit iout */
	if (fixed_fop >= WLRX_ACC_NORMAL_MAX_FOP) {
		if (wlc_high_fop_vout_check())
			return -EINVAL;
	} else if (!force_flag && (g_plimit_time_num < 40)) {
		g_plimit_time_num++;
		return -EINVAL;
	}
	if (wireless_charge_fix_tx_fop(fixed_fop)) {
		hwlog_err("fop_fix_check: fixed failed\n");
		return -EINVAL;
	}

	g_fop_fixed_flag = fixed_fop;
	g_plimit_time_num = 0;
	return 0;
}

static void wireless_charge_fop_unfix_check(int fixed_fop)
{
	if (g_fop_fixed_flag <= 0)
		return;
	if ((fixed_fop > 0) && !g_need_force_5v_vout)
		return;
	if (wireless_charge_unfix_tx_fop()) {
		hwlog_err("fop_unfix_check: unfix failed\n");
		return;
	}

	g_fop_fixed_flag = 0;
	g_need_force_5v_vout = false;
}

static void wireless_charge_update_fop(bool force_flag)
{
	int fop = wlrx_pctrl_get_fix_fop(WLTRX_DRV_MAIN);

	if (!wlrx_is_std_tx(WLTRX_DRV_MAIN))
		return;
	if (!force_flag && (wlrx_get_charge_stage() <= WLRX_STAGE_CHARGING))
		return;
	if (!wlrx_acc_support_fix_fop(WLTRX_DRV_MAIN, fop))
		return;
	if (wireless_charge_fop_fix_check(force_flag, fop))
		return;
	wireless_charge_fop_unfix_check(fop);
}

static void wlc_update_charge_state(struct wlrx_dev_info *di)
{
	int ret;
	int soc;
	static int retry_cnt;

	if (!wlrx_is_std_tx(WLTRX_DRV_MAIN) || !wlrx_ic_is_tx_exist(WLTRX_IC_MAIN) ||
		(wlrx_get_wired_channel_state() == WIRED_CHANNEL_ON))
		return;

	if (wlrx_get_charge_stage() <= WLRX_STAGE_CHARGING) {
		retry_cnt = 0;
		return;
	}

	soc = power_platform_get_battery_ui_capacity();
	if (soc >= CAPACITY_FULL)
		di->stat_rcd.chrg_state_cur |= WIRELESS_STATE_CHRG_FULL;
	else
		di->stat_rcd.chrg_state_cur &= ~WIRELESS_STATE_CHRG_FULL;

	if (di->stat_rcd.chrg_state_cur != di->stat_rcd.chrg_state_last) {
		if (retry_cnt >= WLC_SEND_CHARGE_STATE_RETRY_CNT) {
			retry_cnt = 0;
			di->stat_rcd.chrg_state_last =
				di->stat_rcd.chrg_state_cur;
			return;
		}
		hwlog_info("[%s] charge_state=%d\n",
			__func__, di->stat_rcd.chrg_state_cur);
		ret = wireless_send_charge_state(WLTRX_IC_MAIN, WIRELESS_PROTOCOL_QI,
			di->stat_rcd.chrg_state_cur);
		if (ret) {
			hwlog_err("%s: send charge_state fail\n", __func__);
			retry_cnt++;
			return;
		}
		retry_cnt = 0;
		di->stat_rcd.chrg_state_last = di->stat_rcd.chrg_state_cur;
	}
}

static void wlc_check_voltage(struct wlrx_dev_info *di)
{
	int vout = 0;
	int vout_reg = 0;
	int vfc_reg = 0;
	int iavg = 0;
	int vbus = 0;
	int cnt_max = RX_VOUT_ERR_CHECK_TIME / di->monitor_interval;
	struct wlrx_dts *dts = wlrx_get_dts();

	if (!dts)
		return;

	(void)wlrx_ic_get_vout(WLTRX_IC_MAIN, &vout);
	if ((vout <= 0) || !wlrx_pmode_bst_rst_completed(di->drv_type) ||
		(wlrx_get_charge_stage() < WLRX_STAGE_HANDSHAKE))
		return;

	charge_get_vbus(&vbus);
	vout = (vout >= vbus) ? vout : vbus;
	(void)wlrx_ic_get_vout_reg(WLTRX_IC_MAIN, &vout_reg);
	if (vout >= vout_reg * dts->rx_vout_err_ratio / POWER_PERCENT) {
		g_rx_vout_err_cnt = 0;
		return;
	}

	wlrx_ic_get_iavg(WLTRX_IC_MAIN, &iavg);
	if (iavg >= RX_EPT_IGNORE_IOUT)
		return;

	hwlog_err("%s: abnormal vout=%dmV", __func__, vout);
	if (++g_rx_vout_err_cnt < cnt_max)
		return;

	g_rx_vout_err_cnt = cnt_max;
	(void)wlrx_ic_get_vfc_reg(WLTRX_IC_MAIN, &vfc_reg);
	if (!wlrx_ic_is_sleep_enable(WLTRX_IC_MAIN) &&
		(vfc_reg >= RX_HIGH_VOUT2)) {
		wlrx_plim_set_src(WLTRX_DRV_MAIN, WLRX_PLIM_SRC_VOUT_ERR);
		hwlog_err("%s: high vout err\n", __func__);
		return;
	}
	hwlog_info("[%s] vout lower than %d*%d%%mV for %dms, send EPT\n",
		__func__, vout_reg, dts->rx_vout_err_ratio,
		RX_VOUT_ERR_CHECK_TIME);
	if (wlrx_get_wireless_channel_state() == WIRELESS_CHANNEL_ON)
		wlrx_ic_send_ept(WLTRX_IC_MAIN, WIRELESS_EPT_ERR_VOUT);
}

static bool wlc_is_night_time(unsigned int drv_type)
{
	if (wlrx_sysfs_ignore_fan_ctrl(drv_type))
		return false;
	if (wlrx_in_high_pwr_test(drv_type))
		return false;
	if (wlrx_is_car_tx(drv_type))
		return false;

	/* night time: 21:00-7:00 */
	return power_is_within_time_interval(21, 0, 7, 0);
}

static void wlc_fan_control_handle(struct wlrx_dev_info *di,
	int *retry_cnt, u8 limit_val)
{
	int ret;

	if (*retry_cnt >= WLC_FAN_LIMIT_RETRY_CNT) {
		*retry_cnt = 0;
		di->stat_rcd.fan_last = di->stat_rcd.fan_cur;
		return;
	}

	hwlog_info("[%s] limit_val=0x%x\n", __func__, limit_val);
	ret = wireless_set_fan_speed_limit(WLTRX_IC_MAIN, WIRELESS_PROTOCOL_QI, limit_val);
	if (ret) {
		(*retry_cnt)++;
		return;
	}
	*retry_cnt = 0;
	di->stat_rcd.fan_last = di->stat_rcd.fan_cur;
}

static bool wlrx_need_fan_control(struct wlrx_dev_info *di)
{
	struct wlprot_acc_cap *acc_cap = wlrx_acc_get_cap(WLTRX_DRV_MAIN);

	if (!wlrx_is_std_tx(WLTRX_DRV_MAIN) || !acc_cap || !acc_cap->support_fan)
		return false;

	/* in charger mode, time zone is not available */
	if (power_cmdline_is_factory_mode() ||
		power_cmdline_is_powerdown_charging_mode())
		return false;

	return true;
}

static struct wlc_fun_speed {
	u8 fan_speed;
	u8 qi_fan_speed;
} const g_fun_speed[WLC_FAN_END] = {
	[WLC_FAN_SPEED_LEVEL1_MAX] = { WLC_FAN_SPEED_LEVEL1_MAX, 0x00 },
	[WLC_FAN_SPEED_LEVEL1] = { WLC_FAN_SPEED_LEVEL1, 0x20 },
	[WLC_FAN_SPEED_LEVEL2_MAX] = { WLC_FAN_SPEED_LEVEL2_MAX, 0x01 },
	[WLC_FAN_SPEED_LEVEL2] = { WLC_FAN_SPEED_LEVEL2, 0x30 },
	[WLC_FAN_SPEED_LEVEL3_MAX] = { WLC_FAN_SPEED_LEVEL3_MAX, 0x02 },
	[WLC_FAN_SPEED_LEVEL3] = { WLC_FAN_SPEED_LEVEL3, 0x40 },
	[WLC_FAN_SPEED_LEVEL4_MAX] = { WLC_FAN_SPEED_LEVEL4_MAX, 0x03 },
	[WLC_FAN_SPEED_LEVEL4] = { WLC_FAN_SPEED_LEVEL4, 0x50 },
	[WLC_FAN_SPEED_LEVEL5_MAX] = { WLC_FAN_SPEED_LEVEL5_MAX, 0x04 },
	[WLC_FAN_SPEED_LEVEL5] = { WLC_FAN_SPEED_LEVEL5, 0x60 },
};

static void wlc_update_fan_cur(struct wlrx_dev_info *di)
{
	u8 thermal_status = wlrx_sysfs_get_thermal_ctrl(WLTRX_DRV_MAIN) &
		WLRX_SYSFS_THERMAL_FORCE_FAN_FULL_SPEED;

	struct wlrx_acc_tx_fan_speed *tx_fan_speed = wlrx_acc_get_tx_fan_speed(di->drv_type);
	if (tx_fan_speed != NULL) {
		di->stat_rcd.fan_cur = (thermal_status == WLRX_SYSFS_THERMAL_FORCE_FAN_FULL_SPEED) ?
			tx_fan_speed->level : tx_fan_speed->level_max;
		return;
	}

	if (wldc_support_scx_mode())
		di->stat_rcd.fan_cur = (thermal_status == WLRX_SYSFS_THERMAL_FORCE_FAN_FULL_SPEED) ?
			WLC_FAN_SPEED_LEVEL4 : WLC_FAN_SPEED_LEVEL4_MAX;
	else
		di->stat_rcd.fan_cur = (thermal_status == WLRX_SYSFS_THERMAL_FORCE_FAN_FULL_SPEED) ?
			WLC_FAN_SPEED_LEVEL2 : WLC_FAN_SPEED_LEVEL2_MAX;
}

static void wlc_update_fan_control(struct wlrx_dev_info *di, bool force_flag)
{
	static int retry_cnt;
	int tx_pwr;
	int i;
	struct wlprot_acc_cap *acc_cap = NULL;

	if (!wlrx_need_fan_control(di))
		return;
	if (!wlrx_ic_is_tx_exist(WLTRX_IC_MAIN) ||
		(wlrx_get_wired_channel_state() == WIRED_CHANNEL_ON))
		return;
	if (!force_flag &&
		(wlrx_get_charge_stage() <= WLRX_STAGE_CHARGING)) {
		retry_cnt = 0;
		return;
	}

	acc_cap = wlrx_acc_get_cap(WLTRX_DRV_MAIN);
	if (!acc_cap)
		return;
	tx_pwr = acc_cap->vmax * acc_cap->imax;
	if (wlc_is_night_time(WLTRX_DRV_MAIN)) {
		di->stat_rcd.fan_cur = WLC_FAN_SPEED_LEVEL1_MAX;
		wlrx_plim_set_src(WLTRX_DRV_MAIN, WLRX_PLIM_SRC_FAN);
	} else if (tx_pwr <= WLC_FAN_CTRL_PWR) {
		di->stat_rcd.fan_cur = WLC_FAN_SPEED_LEVEL2_MAX;
		wlrx_plim_clear_src(WLTRX_DRV_MAIN, WLRX_PLIM_SRC_FAN);
	} else {
		wlc_update_fan_cur(di);
		wlrx_plim_clear_src(WLTRX_DRV_MAIN, WLRX_PLIM_SRC_FAN);
	}

	if (di->stat_rcd.fan_last != di->stat_rcd.fan_cur) {
		for (i = WLC_FAN_BEGIN; i < WLC_FAN_END; i++) {
			if (di->stat_rcd.fan_cur == g_fun_speed[i].fan_speed)
				wlc_fan_control_handle(di, &retry_cnt, g_fun_speed[i].qi_fan_speed);
		}
	}
}

static void wlc_update_scene_control(struct wlrx_dev_info *di)
{
	enum wlrx_scene scn_id = wlrx_get_scene();

	if ((scn_id == WLRX_SCN_LIGHTSTRAP) || (scn_id == WLRX_SCN_COOLINGCASE) ||
		(scn_id == WLRX_SCN_HALL_ONLY))
		wlrx_plim_set_src(WLTRX_DRV_MAIN, WLRX_PLIM_SRC_SCENE);
	else
		wlrx_plim_clear_src(WLTRX_DRV_MAIN, WLRX_PLIM_SRC_SCENE);
}

static void wireless_charge_update_status(struct wlrx_dev_info *di)
{
	wireless_charge_update_fop(false);
	wlc_update_charge_state(di);
	wlc_update_fan_control(di, false);
}

static void wireless_charge_power_mode_control(struct wlrx_dev_info *di)
{
	wlrx_pmode_select_opt(di->drv_type);
	wlc_iout_control(di);
}

static void wlc_recheck_cert_preprocess(struct wlrx_dev_info *di)
{
	/* vout may be 9v, so here reset 5V for cert stability */
	if (wlrx_pmode_set_vtrx(di->drv_type, TX_DEFAULT_VOUT, TX_DEFAULT_VOUT))
		hwlog_err("%s: set default vout failed\n", __func__);

	wlrx_set_iout_min();
	(void)power_msleep(WLRX_ILIM_DELAY, DT_MSLEEP_25MS, wlrx_msleep_exit);
	wlrx_set_charge_stage(WLRX_STAGE_AUTH);
}

static void wireless_charge_regulation(struct wlrx_dev_info *di)
{
	if ((wlrx_get_charge_stage() != WLRX_STAGE_REGULATION) ||
		(wlrx_get_wireless_channel_state() == WIRELESS_CHANNEL_OFF))
		return;

	if (wlrx_acc_auth_need_recheck(WLTRX_DRV_MAIN)) {
		wlc_recheck_cert_preprocess(di);
		return;
	}

	wireless_charge_update_max_vout_and_iout(false);
	wireless_charge_power_mode_control(di);
}

static void read_back_color_from_nv(char *back_color, unsigned int back_len)
{
	power_nv_read(POWER_NV_DEVCOLR, back_color, back_len);
}

static void wlc_check_pu_shell(struct wlrx_dev_info *di)
{
	static bool check_done;
	static char dev_color[BACK_DEVICE_COLOR_LEN];

	if (check_done)
		return;

	memset(dev_color, 0, sizeof(dev_color));
	read_back_color_from_nv(dev_color, BACK_DEVICE_COLOR_LEN - 1);
	hwlog_info("[%s] pu_color:%s\n", __func__, dev_color);
	check_done = true;

	if (!strncmp(dev_color, "puorange", strlen("puorange")) ||
		!strncmp(dev_color, "pugreen", strlen("pugreen")))
		wlrx_ic_set_pu_shell_flag(WLTRX_IC_MAIN, true);
	else
		wlrx_ic_set_pu_shell_flag(WLTRX_IC_MAIN, false);
}

static void wlc_set_ext_fod_flag(struct wlrx_dev_info *di)
{
	if (wlrx_ic_need_chk_pu_shell(WLTRX_IC_MAIN))
		wlc_check_pu_shell(di);
}

static void wireless_charge_start_charging(struct wlrx_dev_info *di)
{
	int vtx;

	if ((wlrx_get_charge_stage() != WLRX_STAGE_CHARGING) ||
		(wlrx_get_wireless_channel_state() == WIRELESS_CHANNEL_OFF))
		return;

	di->supported_rx_vout = wlrx_pmode_get_supported_max_vrx(di->drv_type);
	wlc_extra_power_supply(true);
	wlc_update_fan_control(di, true);
	wlc_update_scene_control(di);
	wireless_charge_update_fop(true);
	wlc_update_kb_control(WLC_START_CHARING);
	wireless_charge_update_max_vout_and_iout(true);
	wlrx_pmode_icon_display(di->drv_type, WLRX_PMODE_NORMAL_JUDGE);

	vtx = power_min_positive(wlrx_pctrl_get_para(di->drv_type, WLRX_PCTRL_VTX),
		wlrx_pmode_get_curr_int_cfg(di->drv_type, WLRX_PMODE_VTX));
	power_event_bnc_notify(POWER_BNT_WLC, POWER_NE_WLC_CHARGER_VBUS, &vtx);

	di->iout_low_cnt = 0;
	wireless_charge_power_mode_control(di);
	(void)wireless_send_charge_event(WLTRX_IC_MAIN, WIRELESS_PROTOCOL_QI,
		RX_STATR_CHARGING);

	if (wlrx_pmode_in_dc_mode(WLTRX_DRV_MAIN))
		return;

	wlrx_set_charge_stage(WLRX_STAGE_REGULATION);
}

static bool wlc_is_support_set_rpp_format(void)
{
	int ret;
	u8 tx_rpp = 0;
	struct wlrx_dts *dts = wlrx_get_dts();

	if (!dts || dts->pmax <= 0)
		return false;

	if (!wlrx_acc_auth_succ(WLTRX_DRV_MAIN))
		return false;

	ret = wireless_get_rpp_format(WLTRX_IC_MAIN, WIRELESS_PROTOCOL_QI, &tx_rpp);
	if (!ret && (tx_rpp == HWQI_ACK_RPP_FORMAT_24BIT))
		return true;

	return false;
}

static int wlc_set_rpp_format(struct wlrx_dev_info *di)
{
	int ret;
	int count = 0;
	struct wlrx_dts *dts = wlrx_get_dts();

	if (!dts)
		return -EINVAL;

	do {
		ret = wireless_set_rpp_format(WLTRX_IC_MAIN, WIRELESS_PROTOCOL_QI, dts->pmax);
		if (!ret) {
			hwlog_info("%s: succ\n", __func__);
			return 0;
		}
		(void)power_msleep(WLRX_SET_RPP_FORMAT_RETRY_DELAY,
			DT_MSLEEP_25MS, wlrx_msleep_exit);
		count++;
		hwlog_err("%s: failed, try next time\n", __func__);
	} while (count < WLC_SET_RPP_FORMAT_RETRY_CNT);

	if (count < WLC_SET_RPP_FORMAT_RETRY_CNT) {
		hwlog_info("[%s] succ\n", __func__);
		return 0;
	}

	return -EIO;
}

static void wlc_rpp_format_init(struct wlrx_dev_info *di)
{
	int ret;

	if (!wlrx_is_std_tx(WLTRX_DRV_MAIN) || wlrx_is_fac_tx(WLTRX_DRV_MAIN))
		return;

	if (!wlc_is_support_set_rpp_format()) {
		wlrx_plim_set_src(WLTRX_DRV_MAIN, WLRX_PLIM_SRC_RPP);
		return;
	}

	ret = wlc_set_rpp_format(di);
	if (!ret) {
		hwlog_info("[%s] succ\n", __func__);
		wlrx_plim_clear_src(WLTRX_DRV_MAIN, WLRX_PLIM_SRC_RPP);
		return;
	}
	wlrx_plim_set_src(WLTRX_DRV_MAIN, WLRX_PLIM_SRC_RPP);
}

static void wireless_charge_check_fwupdate(struct wlrx_dev_info *di)
{
	int ret;

	if ((wlrx_get_charge_stage() != WLRX_STAGE_FW_UPDATE) ||
		(wlrx_get_wireless_channel_state() == WIRELESS_CHANNEL_OFF))
		return;

	ret = wlrx_ic_fw_update(WLTRX_IC_MAIN);
	if (!ret)
		wlrx_chip_init(di->drv_type, WIRELESS_CHIP_INIT);

	charge_pump_chip_init(CP_TYPE_MAIN);
	charge_pump_chip_init(CP_TYPE_AUX);
	wlc_rpp_format_init(di);
	wlrx_set_charge_stage(WLRX_STAGE_CHARGING);
}

static void wireless_charge_check_certification(struct wlrx_dev_info *di)
{
	int ret;
	char dsm_buff[POWER_DSM_BUF_SIZE_0512] = { 0 };

	if ((wlrx_get_charge_stage() != WLRX_STAGE_AUTH) ||
		(wlrx_get_wireless_channel_state() == WIRELESS_CHANNEL_OFF))
		return;

	if (!wlrx_pmode_need_auth(di->drv_type)) {
		wlrx_set_charge_stage(WLRX_STAGE_FW_UPDATE);
		return;
	}

	wlrx_set_iout_min();
	ret = wlrx_acc_auth(WLTRX_DRV_MAIN);
	if (ret == WLRX_ACC_AUTH_SUCC) {
		wlrx_set_charge_stage(WLRX_STAGE_FW_UPDATE);
	} else if (ret == WLRX_ACC_AUTH_CNT_ERR) {
		if (di->certi_comm_err_cnt > 0) {
			wlc_rx_chip_reset(di);
		} else {
			wlrx_pmode_icon_display(di->drv_type, WLRX_PMODE_NORMAL_JUDGE);
			wlrx_dsm_report(WLTRX_DRV_MAIN, POWER_DSM_ERROR_WIRELESS_CERTI_COMM_FAIL,
				dsm_buff, sizeof(dsm_buff));
		}
		wlrx_set_charge_stage(WLRX_STAGE_FW_UPDATE);
	} else if ((ret == WLRX_ACC_AUTH_DEV_ERR) || (ret == WLRX_ACC_AUTH_SRV_NOT_READY)) {
		wlrx_set_charge_stage(WLRX_STAGE_FW_UPDATE);
	} else if (ret == WLRX_ACC_AUTH_CM_ERR) {
		++di->certi_comm_err_cnt;
	} else if (ret == WLRX_ACC_AUTH_SRV_ERR) {
		di->certi_comm_err_cnt = 0;
	}
}

static void wireless_charge_cable_detect(struct wlrx_dev_info *di)
{
	if ((wlrx_get_charge_stage() != WLRX_STAGE_CABLE_DET) ||
		(wlrx_get_wireless_channel_state() == WIRELESS_CHANNEL_OFF))
		return;

	di->cable_detect_succ_flag = WIRELESS_CHECK_SUCC;
	wlrx_set_charge_stage(WLRX_STAGE_AUTH);
}

static void wireless_charge_check_tx_ability(struct wlrx_dev_info *di)
{
	int ret;
	char dsm_buff[POWER_DSM_BUF_SIZE_0512] = {0};

	if ((wlrx_get_charge_stage() != WLRX_STAGE_GET_TX_CAP) ||
		(wlrx_get_wireless_channel_state() == WIRELESS_CHANNEL_OFF))
		return;

	wlrx_set_iout_min();
	ret = wlrx_acc_detect_cap(WLTRX_DRV_MAIN);
	if ((ret == WLRX_ACC_DET_CAP_SUCC) || (ret == WLRX_ACC_DET_CAP_DEV_ERR)) {
		wlrx_set_charge_stage(WLRX_STAGE_CABLE_DET);
	} else if (ret == WLRX_ACC_DET_CAP_CNT_ERR) {
		if (wlrx_is_std_tx(WLTRX_DRV_MAIN) &&
			(di->wlc_err_rst_cnt >= WLC_RST_CNT_MAX))
			wlrx_dsm_report(WLTRX_DRV_MAIN, POWER_DSM_ERROR_WIRELESS_CHECK_TX_ABILITY_FAIL,
				dsm_buff, sizeof(dsm_buff));
		wlc_rx_chip_reset(di);
		wlrx_set_charge_stage(WLRX_STAGE_CABLE_DET);
	}
}

static void wireless_charge_check_tx_id(struct wlrx_dev_info *di)
{
	int ret;

	if ((wlrx_get_charge_stage() != WLRX_STAGE_HANDSHAKE) ||
		(wlrx_get_wireless_channel_state() == WIRELESS_CHANNEL_OFF))
		return;

	wlrx_set_iout_min();
	ret = wlrx_acc_handshake(WLTRX_DRV_MAIN);
	if (ret == WLRX_ACC_HS_SUCC)
		wlrx_set_charge_stage(WLRX_STAGE_GET_TX_CAP);
	else if ((ret == WLRX_ACC_HS_DEV_ERR) || (ret == WLRX_ACC_HS_CNT_ERR) ||
		(ret == WLRX_ACC_HS_ID_ERR))
		wlrx_set_charge_stage(WLRX_STAGE_CABLE_DET);
}

static void wireless_charge_rx_stop_charing_config(struct wlrx_dev_info *di)
{
	wlrx_ic_stop_charging(WLTRX_IC_MAIN);
	wlrx_ic_set_turbo_charge_flag(WLTRX_IC_MAIN, false);
	wireless_reset_dev_info(WLTRX_IC_MAIN, WIRELESS_PROTOCOL_QI);
}

static void wlc_state_record_para_init(struct wlrx_dev_info *di)
{
	di->stat_rcd.chrg_state_cur = 0;
	di->stat_rcd.chrg_state_last = 0;
	di->stat_rcd.fan_cur = WLC_FAN_UNKNOWN_SPEED;
	di->stat_rcd.fan_last = WLC_FAN_UNKNOWN_SPEED;
}

static void wireless_charge_para_init(struct wlrx_dev_info *di)
{
	struct wlrx_dts *dts = wlrx_get_dts();

	if (!dts)
		return;

	di->monitor_interval = wlrx_get_monitor_interval();
	di->ctrl_interval = CONTROL_INTERVAL_NORMAL;
	wlrx_pctrl_set_para(di->drv_type, WLRX_PCTRL_INIT, 0);
	di->rx_iout_limit = wlrx_pctrl_get_para(di->drv_type, WLRX_PCTRL_IRX);
	di->certi_comm_err_cnt = 0;
	wlrx_sysfs_charge_para_init(WLTRX_DRV_MAIN);
	di->iout_high_cnt = 0;
	di->iout_low_cnt = 0;
	di->cable_detect_succ_flag = 0;
	di->curr_tx_type_index = 0;
	di->supported_rx_vout = RX_DEFAULT_VOUT;
	di->extra_pwr_good_flag = 1;
	wlrx_fod_reset_para(di->drv_type, WLTRX_RST_IN_INIT);
	g_rx_vrect_restore_cnt = 0;
	g_rx_vout_err_cnt = 0;
	wlrx_common_reset_para(di->drv_type, WLTRX_RST_IN_INIT);
	wlrx_pmode_reset_para(di->drv_type, WLTRX_RST_IN_INIT);
	wlrx_evt_clear_err_cnt(WLTRX_DRV_MAIN);
	wlrx_reset_fsk_alarm();
	wlrx_plim_reset_src(WLTRX_DRV_MAIN);
	wlc_state_record_para_init(di);
	wlrx_acc_reset_para(WLTRX_DRV_MAIN);
	wlrx_buck_set_iin_prop(WLTRX_DRV_MAIN, dts->rx_istep, WLRX_BUCK_IIN_DELAY, 0);
	wlrx_ic_set_turbo_charge_flag(WLTRX_IC_MAIN, false);
}

static void wireless_charge_control_work(struct work_struct *work)
{
	struct wlrx_dev_info *di = container_of(work,
		struct wlrx_dev_info, wireless_ctrl_work.work);

	if (!di) {
		hwlog_err("control_work: para null\n");
		return;
	}

	wireless_charge_check_tx_id(di);
	wireless_charge_check_tx_ability(di);
	wireless_charge_cable_detect(di);
	wireless_charge_check_certification(di);
	wireless_charge_check_fwupdate(di);
	wireless_charge_start_charging(di);
	wireless_charge_regulation(di);
	wireless_charge_set_ctrl_interval(di);

	if ((wlrx_get_wireless_channel_state() == WIRELESS_CHANNEL_ON) &&
		(wlrx_get_charge_stage() != WLRX_STAGE_REGULATION_DC))
		schedule_delayed_work(&di->wireless_ctrl_work,
			msecs_to_jiffies(di->ctrl_interval));
}

void wireless_charge_restart_charging(unsigned int stage_from)
{
	struct wlrx_dev_info *di = g_wlrx_di;

	if (!di) {
		hwlog_err("%s: di null\n", __func__);
		return;
	}
	if ((wlrx_get_wireless_channel_state() == WIRELESS_CHANNEL_ON) &&
		(wlrx_get_charge_stage() > WLRX_STAGE_CHARGING)) {
		wlrx_set_charge_stage(stage_from);
		schedule_delayed_work(&di->wireless_ctrl_work,
			msecs_to_jiffies(100)); /* 100ms for pmode stability */
	}
}

static void wireless_charge_switch_off(void)
{
	wlps_control(WLTRX_IC_MAIN, WLPS_SC_SW2, false);
	wlps_control(WLTRX_IC_MAIN, WLPS_RX_SW_AUX, false);
	wlrx_ic_sleep_enable(WLTRX_IC_MAIN, true);
	charge_pump_chip_enable(CP_TYPE_AUX, false);
	wlps_control(WLTRX_IC_MAIN, WLPS_RX_SW, false);
	charge_pump_chip_enable(CP_TYPE_MAIN, false);
	charger_select_channel(CHARGER_CH_USBIN);
}

static void wireless_charge_stop_charging(struct wlrx_dev_info *di)
{
	hwlog_info("%s ++\n", __func__);
	wlrx_ic_sleep_enable(WLTRX_IC_MAIN, true);
	wlc_extra_power_supply(false);
	pd_dpm_ignore_vbus_only_event(false);
	wlrx_set_charge_stage(WLRX_STAGE_DEFAULT);
	wlrx_buck_set_iin_prop(WLTRX_DRV_MAIN, 0, 0, 0);
	wireless_charge_rx_stop_charing_config(di);
	wlc_update_kb_control(WLC_STOP_CHARING);
	wlrx_ui_reset_icon_flag(WLTRX_DRV_MAIN, WLRX_UI_FAST_CHARGE);
	wlrx_ui_reset_icon_flag(WLTRX_DRV_MAIN, WLRX_UI_SUPER_CHARGE);
	g_fop_fixed_flag = 0;
	g_need_force_5v_vout = false;
	g_plimit_time_num = 0;
	cancel_delayed_work_sync(&di->wireless_ctrl_work);
	wlrx_fod_reset_para(di->drv_type, WLTRX_RST_IN_STOP);
	wlrx_ui_reset_icon_type(WLTRX_DRV_MAIN);
	wlrx_pmode_reset_para(di->drv_type, WLTRX_RST_IN_STOP);
	di->wlc_err_rst_cnt = 0;
	di->supported_rx_vout = RX_DEFAULT_VOUT;
	wlrx_acc_reset_para(WLTRX_DRV_MAIN);
	wireless_protocol_reset_para();
	wlrx_pctrl_count_dcdiscon(WLTRX_DRV_MAIN, false);
	wlrx_common_reset_para(di->drv_type, WLTRX_RST_IN_STOP);
	hwlog_info("%s --\n", __func__);
}

static void wireless_charge_wired_5votg_restore_work(struct work_struct *work)
{
	vbus_ch_close_switch(VBUS_CH_USER_WL_RX, VBUS_CH_TYPE_BOOST_GPIO, false);
}

static void wireless_charge_switch_on(struct wlrx_dev_info *di)
{
	int mode = 0;
	struct wlrx_dts *dts = wlrx_get_dts();

	vbus_ch_get_mode(VBUS_CH_USER_WL_RX, VBUS_CH_TYPE_BOOST_GPIO, &mode);
	if (!dts || !dts->wired_close_5votg ||
		(mode != VBUS_CH_IN_OTG_MODE)) {
		wlps_control(WLTRX_IC_MAIN, WLPS_RX_SW, true);
		return;
	}

	/* to fix a hardware issue, vpsw can not open when 5vbst is on */
	vbus_ch_close_switch(VBUS_CH_USER_WL_RX, VBUS_CH_TYPE_BOOST_GPIO, true);
	wlps_control(WLTRX_IC_MAIN, WLPS_RX_SW, true);
	cancel_delayed_work_sync(&di->wired_5votg_restore_work);
	schedule_delayed_work(&di->wired_5votg_restore_work,
		msecs_to_jiffies(dts->wired_5votg_restore_delay));
}

static void wlc_wireless_vbus_connect_handler(unsigned int stage_from)
{
	int max_vtx;
	struct wlrx_dev_info *di = g_wlrx_di;

	if (!di) {
		hwlog_err("wireless_vbus_connect_handler: para null\n");
		return;
	}

	if (wlrx_get_wired_channel_state() == WIRED_CHANNEL_ON) {
		hwlog_err("%s: wired vbus connect\n", __func__);
		return;
	}

	wlrx_set_wireless_channel_state(WIRELESS_CHANNEL_ON);
	wlrx_cut_off_wired_channel(WLTRX_DRV_MAIN);
	wireless_charge_switch_on(di);
	charge_pump_chip_enable(CP_TYPE_MAIN, true);
	wlrx_ic_sleep_enable(WLTRX_IC_MAIN, false);
	charger_select_channel(CHARGER_CH_WLSIN);
	wlrx_chip_init(di->drv_type, WIRELESS_CHIP_INIT);
	wlrx_pmode_reset_para(di->drv_type, WLTRX_RST_IN_PROC);

	if (wlrx_get_wireless_channel_state() != WIRELESS_CHANNEL_ON) {
		hwlog_err("%s: wireless vbus off\n", __func__);
		return;
	}

	wlrx_set_charge_stage(stage_from);
	wlrx_set_iout_min();
	mod_delayed_work(system_wq, &di->wireless_ctrl_work,
		msecs_to_jiffies(di->ctrl_interval));
	max_vtx = wlrx_pctrl_get_para(di->drv_type, WLRX_PCTRL_VTX);
	power_event_bnc_notify(POWER_BNT_WLC, POWER_NE_WLC_CHARGER_VBUS, &max_vtx);
	hwlog_info("%s --\n", __func__);
}

static void wlrx_wls_disconnect_set_wired_channel(void)
{
	if (!wdcm_dev_exist())
		return;

	wdcm_set_buck_channel_state(WDCM_CLIENT_WLS, WDCM_DEV_OFF);
}

static void wireless_charge_wireless_vbus_disconnect_handler(void)
{
	struct wlrx_dev_info *di = g_wlrx_di;

	if (!di) {
		hwlog_err("%s: di null\n", __func__);
		return;
	}
	if (wlrx_ic_is_tx_exist(WLTRX_IC_MAIN)) {
		hwlog_info("[%s] tx exist, ignore\n", __func__);
		power_event_bnc_notify(POWER_BNT_WLC, POWER_NE_WLC_VBUS_CONNECT, NULL);
		mod_delayed_work(system_wq,
			&di->wireless_monitor_work, msecs_to_jiffies(0));
		mod_delayed_work(di->wireless_kick_wtd_wq,
			&di->wireless_watchdog_work, msecs_to_jiffies(0));
		wldc_set_charge_stage(WLDC_STAGE_DEFAULT);
		wlc_wireless_vbus_connect_handler(WLRX_STAGE_REGULATION);
		return;
	}
	wlrx_set_wireless_channel_state(WIRELESS_CHANNEL_OFF);
	wireless_charge_switch_off();
	power_msleep(DT_MSLEEP_20MS, 0, NULL); /* delay 20ms for preventing volt backflow */
	wlrx_wls_disconnect_set_wired_channel();
	power_supply_set_int_property_value("charger_detection",
		POWER_SUPPLY_PROP_CHG_PLUGIN, POWER_SUPPLY_STOP_SINK_WIRELESS);
	wireless_charge_stop_charging(di);
}

static void wireless_charge_wireless_vbus_disconnect_work(
	struct work_struct *work)
{
	wireless_charge_wireless_vbus_disconnect_handler();
}

static void wireless_charge_pmic_vbus_work(
	struct work_struct *work)
{
	struct wlrx_dev_info *di = container_of(work,
		struct wlrx_dev_info, pmic_vbus_work.work);

	if (!di) {
		hwlog_err("pmic_vbus_connect_work: di null\n");
		return;
	}

	switch (di->pmic_vbus_state) {
	case VBUS_PLUGIN:
		wireless_charger_pmic_vbus_handler(true);
		break;
	case VBUS_PLUGOUT:
		wireless_charger_pmic_vbus_handler(false);
		break;
	default:
		break;
	}
}

static void wlrx_wired_connect_set_wired_channel(void)
{
	if (!wdcm_dev_exist()) {
		wired_chsw_set_wired_channel(WIRED_CHANNEL_BUCK, WIRED_CHANNEL_RESTORE);
		return;
	}

	wdcm_set_buck_channel_state(WDCM_CLIENT_WIRED, WDCM_DEV_ON);
}

static void wlrx_chip_enable(bool flag)
{
	mutex_lock(&g_rx_en_mutex);
	wlrx_ic_chip_enable(WLTRX_IC_MAIN, flag);
	mutex_unlock(&g_rx_en_mutex);
}

static void wireless_chip_en_enable_work(struct work_struct *work)
{
	if (wlrx_get_wired_channel_state() == WIRED_CHANNEL_ON) {
		hwlog_info("wired vbus connect, turn off wireless channel\n");
		return;
	}

	wlrx_chip_enable(true);
	hwlog_info("screen fold, turn on wireless channel\n");
}

static void wireless_chip_en_dis_work(struct work_struct *work)
{
	if (wireless_is_in_tx_mode())
		return;

	wlrx_chip_enable(false);
	hwlog_info("screen unfold, turn off wireless channel\n");
}

static void wireless_charge_wired_vbus_connect_work(struct work_struct *work)
{
	int i;
	int vout = 0;
	int vrect = 0;
	struct wlrx_dev_info *di = g_wlrx_di;

	if (!di) {
		hwlog_err("%s: di null\n", __func__);
		return;
	}
	mutex_lock(&g_rx_en_mutex);
	(void)wlrx_ic_get_vout(WLTRX_IC_MAIN, &vout);
	wireless_charge_rx_stop_charing_config(di);
	if (vout >= RX_HIGH_VOUT) {
		(void)wlrx_pmode_set_vtx(di->drv_type, TX_DEFAULT_VOUT);
		(void)wlrx_pmode_set_vrx(di->drv_type, TX_DEFAULT_VOUT);
		if (wlrx_get_wired_channel_state() == WIRED_CHANNEL_OFF) {
			hwlog_err("%s: wired vubs already off, reset rx\n", __func__);
			wlc_rx_chip_reset(di);
		}
		if (!wireless_is_in_tx_mode())
			wlrx_ic_chip_enable(WLTRX_IC_MAIN, false);
		wlrx_set_wireless_channel_state(WIRELESS_CHANNEL_OFF);
	} else {
		if (!wireless_is_in_tx_mode())
			wlrx_ic_chip_enable(WLTRX_IC_MAIN, false);
		wlrx_set_wireless_channel_state(WIRELESS_CHANNEL_OFF);
	}
	mutex_unlock(&g_rx_en_mutex);
	/* wait for 50ms*10=500ms at most, generally 300ms at most */
	for (i = 0; i < 10; i++) {
		(void)wlrx_ic_get_vrect(WLTRX_IC_MAIN, &vrect);
		(void)wlrx_ic_get_vout(WLTRX_IC_MAIN, &vout);
		hwlog_info("wired_vbus_connect_work, vout:%d vrect:%d\n", vout, vrect);
		if (wldc_is_stop_charging_complete()) {
			if (charge_get_charger_online())
				wlrx_wired_connect_set_wired_channel();
			break;
		}
		msleep(50);
	}
	hwlog_info("wired vbus connect, turn off wireless channel\n");
	wireless_charge_stop_charging(di);
}

static void wireless_charge_wired_vbus_disconnect_work(struct work_struct *work)
{
	if (wlrx_get_screen_stop_charging_state()) {
		hwlog_info("screen unfold, turn off wireless channel\n");
		return;
	}

	wlrx_chip_enable(true);
	hwlog_info("wired vbus disconnect, turn on wireless channel\n");
}

void wireless_charge_wired_vbus_connect_handler(void)
{
	struct wlrx_dev_info *di = g_wlrx_di;
	enum wlrx_ui_icon_type type = wlrx_ui_get_icon_type(WLTRX_DRV_MAIN);

	if (!di) {
		hwlog_err("%s: di null\n", __func__);
		wlrx_wired_connect_set_wired_channel();
		wireless_charge_switch_off();
		return;
	}
	if (wlrx_get_wired_channel_state() == WIRED_CHANNEL_ON) {
		hwlog_err("%s: already in sink_vbus state, ignore\n", __func__);
		return;
	}
	hwlog_info("[%s] wired vbus connect\n", __func__);
	power_event_bnc_notify(POWER_BNT_WLC, POWER_NE_WLC_WIRED_VBUS_CONNECT, NULL);
	wlrx_ui_reset_icon_flag(WLTRX_DRV_MAIN, WLRX_UI_SUPER_CHARGE);
	wlrx_set_wired_channel_state(WIRED_CHANNEL_ON);
	wldc_tx_disconnect_handler();
	wireless_charge_switch_off();

	if (wltx_get_cur_pwr_src() == PWR_SRC_RVS_SC4_CP)
		charge_pump_reverse_cp4_chip_init(CP_TYPE_MAIN, false);

	if (type == WLRX_UI_NORMAL_CHARGE)
		wlrx_wired_connect_set_wired_channel();
	schedule_work(&di->wired_vbus_connect_work);
}

static void wlrx_wired_disconnect_set_wired_channel(void)
{
	if (!wdcm_dev_exist()) {
		wired_chsw_set_wired_channel(WIRED_CHANNEL_ALL, WIRED_CHANNEL_CUTOFF);
		return;
	}

	wdcm_set_buck_channel_state(WDCM_CLIENT_WIRED, WDCM_DEV_OFF);
}

void wireless_charge_wired_vbus_disconnect_handler(void)
{
	struct wlrx_dev_info *di = g_wlrx_di;
	static bool first_in = true;

	if (!di) {
		hwlog_err("%s: di null\n", __func__);
		wlrx_wired_disconnect_set_wired_channel();
		return;
	}
	if (!first_in && (wlrx_get_wired_channel_state() == WIRED_CHANNEL_OFF)) {
		hwlog_err("%s: not in sink_vbus state, ignore\n", __func__);
		return;
	}
	first_in = false;
	hwlog_info("[%s] wired vbus disconnect\n", __func__);
	wlrx_set_wired_channel_state(WIRED_CHANNEL_OFF);
	wlrx_wired_disconnect_set_wired_channel();
	power_event_bnc_notify(POWER_BNT_WLC, POWER_NE_WLC_WIRED_VBUS_DISCONNECT, NULL);
	schedule_work(&di->wired_vbus_disconnect_work);
}

#ifdef CONFIG_DIRECT_CHARGER
void direct_charger_disconnect_event(void)
{
	struct wlrx_dev_info *di = g_wlrx_di;

	if (!di) {
		hwlog_err("%s: di null\n", __func__);
		return;
	}
	hwlog_info("wired vbus disconnect in scp charging mode\n");
	wlrx_set_wired_channel_state(WIRED_CHANNEL_OFF);
	wired_chsw_set_wired_channel(WIRED_CHANNEL_ALL, WIRED_CHANNEL_CUTOFF);
	schedule_work(&di->wired_vbus_disconnect_work);
}
#endif

void wireless_charger_pmic_vbus_handler(bool vbus_state)
{
	int vfc_reg = 0;
	bool charger_ovp_exist;
	struct wlrx_dev_info *di = g_wlrx_di;

	if (!di)
		return;

	if (wireless_tx_get_tx_open_flag())
		return;
	charger_ovp_exist = power_sw_exist_by_label(POWER_SW_CHARGER_OVP);
	(void)wlrx_ic_get_vfc_reg(WLTRX_IC_MAIN, &vfc_reg);
	if (!vbus_state && !charger_ovp_exist &&(vfc_reg > TX_REG_VOUT) &&
		wlrx_acc_auth_succ(WLTRX_DRV_MAIN))
		wlrx_ic_sleep_enable(WLTRX_IC_MAIN, true);

	wlrx_ic_pmic_vbus_handler(WLTRX_IC_MAIN, vbus_state);
}

static int wireless_charge_check_tx_disconnect(struct wlrx_dev_info *di)
{
	int discon_delay;
	int vtx = TX_DEFAULT_VOUT;

	if (wlrx_ic_is_tx_exist(WLTRX_IC_MAIN))
		return 0;

	g_fop_fixed_flag = 0;
	g_need_force_5v_vout = false;
	g_plimit_time_num = 0;
	wldc_tx_disconnect_handler();
	wlrx_ic_sleep_enable(WLTRX_IC_MAIN, true);
	power_event_bnc_notify(POWER_BNT_WLC, POWER_NE_WLC_CHARGER_VBUS, &vtx);
	wlrx_set_wireless_channel_state(WIRELESS_CHANNEL_OFF);
	power_event_bnc_notify(POWER_BNT_WLC, POWER_NE_WLC_VBUS_DISCONNECT, NULL);
	wireless_charge_rx_stop_charing_config(di);
	cancel_delayed_work_sync(&di->wireless_ctrl_work);
	cancel_delayed_work_sync(&di->wireless_vbus_disconnect_work);
	discon_delay = wlrx_get_discon_delay(WLTRX_DRV_MAIN);
	schedule_delayed_work(&di->wireless_vbus_disconnect_work,
		msecs_to_jiffies(discon_delay));
	hwlog_err("%s: tx not exist, delay %dms to report disconnect event\n",
		__func__, discon_delay);

	return -1;
}

void wlc_reset_wireless_charge(void)
{
	struct wlrx_dev_info *di = g_wlrx_di;

	if (!di)
		return;

	if (delayed_work_pending(&di->wireless_vbus_disconnect_work))
		mod_delayed_work(system_wq, &di->wireless_vbus_disconnect_work,
			msecs_to_jiffies(0));
}

static void wireless_charge_monitor_work(struct work_struct *work)
{
	int ret;
	struct wlrx_dev_info *di = g_wlrx_di;

	if (!di) {
		hwlog_err("%s: di null\n", __func__);
		return;
	}

	ret = wireless_charge_check_tx_disconnect(di);
	if (ret) {
		hwlog_info("[%s] tx disconnect, stop monitor work\n", __func__);
		return;
	}
	wireless_charge_count_avg_iout(di);
	wlc_check_voltage(di);
	wireless_charge_update_status(di);
	wlrx_mon_show_info(WLTRX_DRV_MAIN);

	schedule_delayed_work(&di->wireless_monitor_work,
		msecs_to_jiffies(di->monitor_interval));
}

static void wireless_charge_watchdog_work(struct work_struct *work)
{
	int64_t delta_time;
	static struct timespec64 last_work_time;
	struct timespec64 time_now;
	struct wlrx_dev_info *di = container_of(work,
		struct wlrx_dev_info, wireless_watchdog_work.work);

	if (last_work_time.tv_sec <= 0)
		last_work_time = power_get_current_kernel_time64();

	time_now = power_get_current_kernel_time64();
	delta_time = (time_now.tv_sec - last_work_time.tv_sec) * NSEC_PER_SEC +
		(time_now.tv_nsec - last_work_time.tv_nsec);
	delta_time = delta_time / NSEC_PER_MSEC;
	last_work_time = time_now;
	/* print if kick watchdog real interval exceeds 500ms */
	if (delta_time >= 500)
		hwlog_err("%s: kick watchdog interval %ldms\n", __func__, delta_time);

	if (!di || (wlrx_get_wireless_channel_state() == WIRELESS_CHANNEL_OFF)) {
		last_work_time.tv_sec = 0;
		hwlog_err("%s: return\n", __func__);
		return;
	}

	if (wlrx_ic_kick_watchdog(WLTRX_IC_MAIN))
		hwlog_err("%s: fail\n", __func__);

	/* kick watchdog at an interval of 300ms */
	mod_delayed_work(di->wireless_kick_wtd_wq, &di->wireless_watchdog_work,
		msecs_to_jiffies(300));
}

static void wireless_charge_pwroff_reset_work(struct work_struct *work)
{
	struct wlrx_dev_info *di = container_of(work,
		struct wlrx_dev_info, wireless_pwroff_reset_work);

	if (!di) {
		hwlog_err("%s: di null\n", __func__);
		power_wakeup_unlock(g_rx_con_wakelock, false);
		return;
	}
	if (wlrx_in_pwroff_reset(di->drv_type)) {
		msleep(60); /* test result, about 60ms */
		wlrx_ic_chip_reset(WLTRX_IC_MAIN);
		wlrx_pmode_set_vtx(di->drv_type, TX_DEFAULT_VOUT);
		wlrx_pmode_set_vrx(di->drv_type, RX_DEFAULT_VOUT);
	}
	power_wakeup_unlock(g_rx_con_wakelock, false);
}

static void wlc_rx_power_on_ready_handler(struct wlrx_dev_info *di)
{
	wldc_set_charge_stage(WLDC_STAGE_DEFAULT);
	wlrx_set_charge_stage(WLRX_STAGE_DEFAULT);
	wireless_charge_para_init(di);

	if (((di->rx_evt.type == POWER_NE_WLRX_PWR_ON) &&
		wlrx_evt_is_pwr_good(di->drv_type)) ||
		(di->rx_evt.type == POWER_NE_WLRX_READY)) {
		wltx_reset_reverse_charging();
		wlps_control(WLTRX_IC_MAIN, WLPS_TX_SW, false);
		power_event_bnc_notify(POWER_BNT_WLC, POWER_NE_WLC_VBUS_CONNECT, NULL);
		power_supply_set_int_property_value("charger_detection",
			POWER_SUPPLY_PROP_CHG_PLUGIN, POWER_SUPPLY_START_SINK_WIRELESS);
	}
	pd_dpm_ignore_vbus_only_event(true);
	mod_delayed_work(system_wq, &di->wireless_monitor_work,
		msecs_to_jiffies(0));
	if (delayed_work_pending(&di->wireless_vbus_disconnect_work))
		cancel_delayed_work_sync(&di->wireless_vbus_disconnect_work);
	if (di->rx_evt.type == POWER_NE_WLRX_READY) {
		if (!di->wlc_err_rst_cnt)
			wlrx_ui_reset_icon_flag(WLTRX_DRV_MAIN, WLRX_UI_FAST_CHARGE);
		wlc_set_ext_fod_flag(di);
		wlrx_set_discon_delay(WLTRX_DRV_MAIN, 0);
		power_event_bnc_notify(POWER_BNT_WLC, POWER_NE_WLC_READY, NULL);
		wlc_wireless_vbus_connect_handler(WLRX_STAGE_HANDSHAKE);
		mod_delayed_work(di->wireless_kick_wtd_wq, &di->wireless_watchdog_work,
			msecs_to_jiffies(0));
	}
}

static void wlc_handle_tx_bst_err_evt(void)
{
	hwlog_info("handle_tx_bst_err_evt\n");
	wlrx_plim_set_src(WLTRX_DRV_MAIN, WLRX_PLIM_SRC_TX_BST_ERR);
	wireless_charge_update_max_vout_and_iout(true);
}

static void wlc_handle_rx_ldo_off_evt(struct wlrx_dev_info *di)
{
	hwlog_info("handle_rx_ldo_off_evt\n");
	power_supply_set_int_property_value("charger_detection",
		POWER_SUPPLY_PROP_CHG_PLUGIN, POWER_SUPPLY_STOP_SINK_WIRELESS);
	cancel_delayed_work_sync(&di->wireless_ctrl_work);
	cancel_delayed_work_sync(&di->wireless_monitor_work);
	cancel_delayed_work_sync(&di->wireless_watchdog_work);
}

static void wlc_rx_event_work(struct work_struct *work)
{
	struct wlrx_dev_info *di = container_of(work,
		struct wlrx_dev_info, rx_event_work.work);

	if (!di) {
		power_wakeup_unlock(g_rx_con_wakelock, false);
		return;
	}

	switch (di->rx_evt.type) {
	case POWER_NE_WLRX_PWR_ON:
		hwlog_info("[%s] RX power on\n", __func__);
		wlc_rx_power_on_ready_handler(di);
		break;
	case POWER_NE_WLRX_READY:
		hwlog_info("[%s] RX ready\n", __func__);
		wlc_rx_power_on_ready_handler(di);
		break;
	case POWER_NE_WLRX_LDO_OFF:
		wlc_handle_rx_ldo_off_evt(di);
		break;
	case POWER_NE_WLRX_TX_ALARM:
		wlrx_handle_fsk_alarm(&di->rx_evt.tx_alarm);
		break;
	case POWER_NE_WLRX_TX_BST_ERR:
		wlc_handle_tx_bst_err_evt();
		break;
	default:
		break;
	}
	power_wakeup_unlock(g_rx_con_wakelock, false);
}

static void wlc_save_rx_evt_data(struct wlrx_dev_info *di,
	unsigned long event, void *data)
{
	if (!data)
		return;

	switch (di->rx_evt.type) {
	case POWER_NE_WLRX_TX_ALARM:
		memcpy(&di->rx_evt.tx_alarm, data,
			sizeof(struct wireless_protocol_tx_alarm));
		break;
	default:
		break;
	}
}

static int wireless_charge_rx_event_notifier_call(struct notifier_block *rx_event_nb,
	unsigned long event, void *data)
{
	struct wlrx_dev_info *di = container_of(rx_event_nb,
		struct wlrx_dev_info, rx_event_nb);

	if (!di)
		return NOTIFY_OK;

	switch (event) {
	case POWER_NE_WLRX_PWR_ON:
	case POWER_NE_WLRX_READY:
	case POWER_NE_WLRX_LDO_OFF:
	case POWER_NE_WLRX_TX_ALARM:
	case POWER_NE_WLRX_TX_BST_ERR:
		break;
	default:
		return NOTIFY_OK;
	}

	power_wakeup_lock(g_rx_con_wakelock, false);
	di->rx_evt.type = (int)event;
	wlc_save_rx_evt_data(di, event, data);

	cancel_delayed_work_sync(&di->rx_event_work);
	mod_delayed_work(system_wq, &di->rx_event_work,
		msecs_to_jiffies(0));

	return NOTIFY_OK;
}

static int wireless_charge_pwrkey_event_notifier_call(struct notifier_block *pwrkey_event_nb,
	unsigned long event, void *data)
{
	struct wlrx_dev_info *di = g_wlrx_di;

	if (!di) {
		hwlog_err("%s: di is NULL\n", __func__);
		return NOTIFY_OK;
	}

	if (wlrx_get_wireless_channel_state() == WIRELESS_CHANNEL_OFF) {
		hwlog_err("%s: not in wireless charging\n", __func__);
		return NOTIFY_OK;
	}

	switch (event) {
	case PRESS_KEY_6S:
		power_wakeup_lock(g_rx_con_wakelock, false);
		hwlog_err("%s: response long press 6s interrupt, reset tx vout\n", __func__);
		wlrx_set_pwroff_reset_flag(di->drv_type, true);
		schedule_work(&di->wireless_pwroff_reset_work);
		break;
	case PRESS_KEY_UP:
		wlrx_set_pwroff_reset_flag(di->drv_type, false);
		break;
	default:
		break;
	}
	return NOTIFY_OK;
}

static int wireless_charge_chrg_event_notifier_call(struct notifier_block *chrg_event_nb,
	unsigned long event, void *data)
{
	struct wlrx_dev_info *di = container_of(chrg_event_nb,
		struct wlrx_dev_info, chrg_event_nb);

	if (!di)
		return NOTIFY_OK;

	switch (event) {
	case POWER_NE_CHG_CHARGING_DONE:
		hwlog_debug("[%s] charge done\n", __func__);
		di->stat_rcd.chrg_state_cur |= WIRELESS_STATE_CHRG_DONE;
		break;
	default:
		break;
	}

	return NOTIFY_OK;
}

static int wlrx_power_hall_event_notifier_call(struct notifier_block *power_hall_nb,
	unsigned long event, void *data)
{
	struct wlrx_dev_info *di = container_of(power_hall_nb,
		struct wlrx_dev_info, power_hall_nb);
	struct wlrx_dts *dts = wlrx_get_dts();

	if (!di || !dts) {
		hwlog_err("%s: di is NULL\n", __func__);
		return NOTIFY_OK;
	}

	switch (event) {
	case POWER_NE_SCREEN_FOLD:
		if (!dts->support_fold_status_check)
			return NOTIFY_OK;
		if (!wlrx_get_screen_stop_charging_state())
			break;
		wlrx_set_screen_stop_charging_state(0);
		schedule_work(&di->wireless_chip_en_work);
		break;
	case POWER_NE_SCREEN_UNFOLD:
		if (!dts->support_fold_status_check)
			return NOTIFY_OK;
		if (wlrx_get_screen_stop_charging_state())
			break;
		wlrx_set_screen_stop_charging_state(1);
		schedule_work(&di->wireless_chip_dis_work);
		break;
	case POWER_NE_WINDOW_SHELL_FOLD:
		wlrx_plim_set_src(di->drv_type, WLRX_PLIM_SRC_WINDOW_SHELL);
		break;
	case POWER_NE_WINDOW_SHELL_UNFOLD:
		if (wlrx_plim_get_src(WLTRX_DRV_MAIN) & BIT(WLRX_PLIM_SRC_WINDOW_SHELL))
			wlrx_plim_clear_src(di->drv_type, WLRX_PLIM_SRC_WINDOW_SHELL);
		break;
	default:
		break;
	}

	return NOTIFY_OK;
}

static int wireless_charge_pmic_vbus_notifier_call(
	struct notifier_block *pmic_vbus_nb, unsigned long event, void *data)
{
	struct wlrx_dev_info *di = container_of(pmic_vbus_nb,
		struct wlrx_dev_info, pmic_vbus_nb);

	if (!di)
		return NOTIFY_OK;

	di->pmic_vbus_state = (enum vbus_irq_type)event;
	schedule_delayed_work(&di->pmic_vbus_work, msecs_to_jiffies(0));
	return NOTIFY_OK;
}

static void wlrx_init_probe_para(struct wlrx_dev_info *di)
{
	wlrx_set_discon_delay(di->drv_type, 0);
	wlrx_acc_reset_para(di->drv_type);
}

static void wireless_charge_shutdown(struct platform_device *pdev)
{
	int ret;
	struct wlrx_dev_info *di = platform_get_drvdata(pdev);

	hwlog_info("%s ++\n", __func__);
	if (!di) {
		hwlog_err("%s: di is null\n", __func__);
		return;
	}
	if (wlrx_get_wireless_channel_state() == WIRELESS_CHANNEL_ON) {
		wlrx_set_pwroff_reset_flag(di->drv_type, true);
		wireless_charge_switch_off();
		wlps_control(WLTRX_IC_MAIN, WLPS_RX_EXT_PWR, false);
		msleep(50); /* delay 50ms for power off */
		ret = wlrx_pmode_set_vtx(di->drv_type, WLTRX_VOLT_5V);
		if (ret)
			hwlog_err("%s: wlc sw control fail\n", __func__);
	}
	wlrx_ic_sleep_enable(WLTRX_IC_MAIN, true);
	cancel_delayed_work(&di->wireless_ctrl_work);
	cancel_delayed_work(&di->wireless_monitor_work);
	cancel_delayed_work(&di->wireless_watchdog_work);
	wlrx_fod_reset_para(di->drv_type, WLTRX_RST_IN_SHUTDWON);
	hwlog_info("%s --\n", __func__);
}

static int wireless_charge_remove(struct platform_device *pdev)
{
	struct wlrx_dev_info *di = platform_get_drvdata(pdev);

	hwlog_info("%s ++\n", __func__);
	if (!di) {
		hwlog_err("%s: di is null\n", __func__);
		return 0;
	}

	destroy_workqueue(di->wireless_kick_wtd_wq);
	power_event_bnc_unregister(POWER_BNT_CHG, &di->chrg_event_nb);
	power_event_bnc_unregister(POWER_BNT_WLRX, &di->rx_event_nb);
	power_event_bnc_unregister(POWER_BNT_HALL_EVENT, &di->power_hall_nb);
	wlrx_kfree_dts();
	power_wakeup_source_unregister(g_rx_con_wakelock);

	hwlog_info("%s --\n", __func__);

	return 0;
}

static void wlrx_module_deinit(unsigned int drv_type)
{
	wlrx_common_deinit(drv_type);
	wlrx_sysfs_deinit(drv_type);
	wlrx_evt_deinit(drv_type);
	wlrx_acc_deinit(drv_type);
	wlrx_fod_deinit(drv_type);
	wlrx_pctrl_deinit(drv_type);
	wlrx_pmode_deinit(drv_type);
	wlrx_buck_ictrl_deinit(drv_type);
	wlrx_ui_deinit(drv_type);
}

static int wlrx_module_init(unsigned int drv_type, struct device *dev)
{
	int ret;

	ret = wlrx_common_init(drv_type, dev);
	if (ret)
		goto exit;
	ret = wlrx_sysfs_init(drv_type, dev);
	if (ret)
		goto exit;
	ret = wlrx_evt_init(drv_type, dev);
	if (ret)
		goto exit;
	ret = wlrx_acc_init(drv_type, dev);
	if (ret)
		goto exit;
	ret = wlrx_fod_init(drv_type, dev);
	if (ret)
		goto exit;
	ret = wlrx_pctrl_init(drv_type, dev);
	if (ret)
		goto exit;
	ret = wlrx_pmode_init(drv_type, dev);
	if (ret)
		goto exit;
	ret = wlrx_buck_ictrl_init(drv_type, dev);
	if (ret)
		goto exit;
	ret = wlrx_ui_init(drv_type, dev);
	if (ret)
		goto exit;

	return 0;

exit:
	wlrx_module_deinit(drv_type);
	return -ENODEV;
}

static void wlrx_work_init(struct wlrx_dev_info *di)
{
	if (!di) {
		hwlog_err("%s: di is NULL\n", __func__);
		return;
	}

	di->wireless_kick_wtd_wq = create_singlethread_workqueue("wireless_wtd_wq");
	INIT_WORK(&di->wired_vbus_connect_work, wireless_charge_wired_vbus_connect_work);
	INIT_WORK(&di->wired_vbus_disconnect_work, wireless_charge_wired_vbus_disconnect_work);
	INIT_WORK(&di->wireless_chip_dis_work, wireless_chip_en_dis_work);
	INIT_WORK(&di->wireless_chip_en_work, wireless_chip_en_enable_work);
	INIT_WORK(&di->wireless_pwroff_reset_work, wireless_charge_pwroff_reset_work);
	INIT_DELAYED_WORK(&di->rx_event_work, wlc_rx_event_work);
	INIT_DELAYED_WORK(&di->wireless_ctrl_work, wireless_charge_control_work);
	INIT_DELAYED_WORK(&di->wireless_monitor_work, wireless_charge_monitor_work);
	INIT_DELAYED_WORK(&di->wireless_watchdog_work, wireless_charge_watchdog_work);
	INIT_DELAYED_WORK(&di->wireless_vbus_disconnect_work,
		wireless_charge_wireless_vbus_disconnect_work);
	INIT_DELAYED_WORK(&di->pmic_vbus_work, wireless_charge_pmic_vbus_work);
	INIT_DELAYED_WORK(&di->wired_5votg_restore_work, wireless_charge_wired_5votg_restore_work);
}

static int wlrx_event_register(struct wlrx_dev_info *di)
{
	int ret;
	struct wlrx_dts *dts = wlrx_get_dts();

	if (!di || !dts) {
		hwlog_err("%s: di or dts is NULL\n", __func__);
		return -EINVAL;
	}

	di->rx_event_nb.notifier_call = wireless_charge_rx_event_notifier_call;
	ret = power_event_bnc_register(POWER_BNT_WLRX, &di->rx_event_nb);
	if (ret < 0) {
		hwlog_err("register rx_connect notifier failed\n");
		goto  wireless_charge_event_register_fail_0;
	}

	di->chrg_event_nb.notifier_call = wireless_charge_chrg_event_notifier_call;
	ret = power_event_bnc_register(POWER_BNT_CHG, &di->chrg_event_nb);
	if (ret < 0) {
		hwlog_err("register charger_event notifier failed\n");
		goto  wireless_charge_event_register_fail_1;
	}

	di->pwrkey_nb.notifier_call = wireless_charge_pwrkey_event_notifier_call;
	ret = powerkey_register_notifier(&di->pwrkey_nb);
	if (ret < 0) {
		hwlog_err("register power_key notifier failed\n");
		goto  wireless_charge_event_register_fail_2;
	}

	di->power_hall_nb.notifier_call = wlrx_power_hall_event_notifier_call;
	ret = power_event_bnc_register(POWER_BNT_HALL_EVENT, &di->power_hall_nb);
	if (ret < 0)  {
		hwlog_err("register power_hall notifier failed\n");
		goto  wireless_charge_event_register_fail_3;
	}

	di->pmic_vbus_nb.notifier_call = wireless_charge_pmic_vbus_notifier_call;
	(void)pmic_vbus_register_notifier(&di->pmic_vbus_nb);

	return 0;

wireless_charge_event_register_fail_3:
	powerkey_unregister_notifier(&di->pwrkey_nb);
wireless_charge_event_register_fail_2:
	power_event_bnc_unregister(POWER_BNT_CHG, &di->chrg_event_nb);
wireless_charge_event_register_fail_1:
	power_event_bnc_unregister(POWER_BNT_WLRX, &di->rx_event_nb);
wireless_charge_event_register_fail_0:
	return ret;
}

static int wireless_charge_probe(struct platform_device *pdev)
{
	struct wlrx_dev_info *di = NULL;
	struct device_node *np = NULL;

	if (!wlrx_ic_is_ops_registered(WLTRX_IC_MAIN))
		return -EPROBE_DEFER;

	di = kzalloc(sizeof(*di), GFP_KERNEL);
	if (!di) {
		hwlog_err("alloc di failed\n");
		return -ENOMEM;
	}

	g_wlrx_di = di;
	di->dev = &pdev->dev;
	np = di->dev->of_node;
	di->drv_type = WLTRX_DRV_MAIN;
	platform_set_drvdata(pdev, di);
	g_rx_con_wakelock = power_wakeup_source_register(di->dev, "rx_con_wakelock");

	if (wlrx_parse_dts(np))
		goto wireless_charge_fail_0;

	if (wlrx_module_init(WLTRX_DRV_MAIN, di->dev))
		goto module_init_fail;

	wlrx_init_probe_para(di);
	mutex_init(&g_rx_en_mutex);
	wlrx_work_init(di);
	if (wlrx_event_register(di))
		goto wlrx_event_register_fail;

	if (wlrx_ic_is_tx_exist(WLTRX_IC_MAIN)) {
		wireless_charge_para_init(di);
		power_supply_set_int_property_value("charger_detection",
			POWER_SUPPLY_PROP_CHG_PLUGIN, POWER_SUPPLY_START_SINK_WIRELESS);
		pd_dpm_ignore_vbus_only_event(true);
		wlc_wireless_vbus_connect_handler(WLRX_STAGE_HANDSHAKE);
		schedule_delayed_work(&di->wireless_monitor_work, msecs_to_jiffies(0));
		queue_delayed_work(di->wireless_kick_wtd_wq, &di->wireless_watchdog_work,
			msecs_to_jiffies(0));
	} else {
		wireless_charge_switch_off();
		wlrx_wls_disconnect_set_wired_channel();
	}
	hwlog_info("wireless_charger probe ok\n");
	return 0;

wlrx_event_register_fail:
	wlrx_module_deinit(WLTRX_DRV_MAIN);
module_init_fail:
	wlrx_kfree_dts();
wireless_charge_fail_0:
	power_wakeup_source_unregister(g_rx_con_wakelock);
	kfree(di);
	g_wlrx_di = NULL;
	platform_set_drvdata(pdev, NULL);
	return -1;
}

static struct of_device_id wireless_charge_match_table[] = {
	{
		.compatible = "huawei,wireless_charger",
		.data = NULL,
	},
	{},
};

static struct platform_driver wireless_charge_driver = {
	.probe = wireless_charge_probe,
	.remove = wireless_charge_remove,
	.shutdown = wireless_charge_shutdown,
	.driver = {
		.name = "huawei,wireless_charger",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(wireless_charge_match_table),
	},
};

static int __init wireless_charge_init(void)
{
	hwlog_info("wireless_charger init ok\n");

	return platform_driver_register(&wireless_charge_driver);
}

static void __exit wireless_charge_exit(void)
{
	platform_driver_unregister(&wireless_charge_driver);
}

device_initcall_sync(wireless_charge_init);
module_exit(wireless_charge_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("wireless charge module driver");
MODULE_AUTHOR("HUAWEI Inc");
