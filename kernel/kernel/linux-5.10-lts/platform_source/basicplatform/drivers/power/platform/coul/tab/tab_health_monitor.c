/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description:  driver for battery
 *
 * This software is licensed under the terms of the GNU General Public
 * License, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#include <linux/mutex.h>
#include <linux/minmax.h>
#include <linux/power_supply.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of.h>
#include <linux/timekeeping.h>
#include <linux/rtc.h>
#include <platform_include/basicplatform/linux/power/platform/hisi_eis.h>
#include <platform_include/basicplatform/linux/power/platform/coul/coul_drv.h>
#include <platform_include/basicplatform/linux/power/platform/coul/coul_merge_drv.h>

#include "tab_health_monitor.h"

#define BATTERY_PSY_NAME			"Battery"

#define TAB_EVENT_EXTDMD_NO			930001027
#define TAB_EVENT_SINGLE_NAME			"STATISTICS_BBD_DET"
#define TAB_EVENT_SINGLE_DOMAIN			"BATTERY"

#ifdef CONFIG_HUAWEI_OHOS_DSM
#include <dfx/hiview_hisysevent.h>
#elif defined(CONFIG_HUAWEI_DSM)
#include <log/hiview_hievent.h>
#endif
#ifdef CONFIG_HUAWEI_CHARGER_AP
#include <chipset_common/hwpower/common_module/power_event_ne.h>
#include <chipset_common/hwpower/protocol/adapter_protocol.h>
#endif
#ifdef CONFIG_HUAWEI_OHOS_DSM
typedef struct hiview_hisysevent TAB_EVENT;
#define TAB_EVENT_CREATE_EVENT(...)		\
hisysevent_create(TAB_EVENT_SINGLE_DOMAIN, TAB_EVENT_SINGLE_NAME, STATISTIC)
#define TAB_EVENT_PUT_INTEGER(evt, key, val)	hisysevent_put_integer(evt, key, val)
#define TAB_EVENT_PUT_STRING(evt, key, val)  	hisysevent_put_string(evt, key, val)
#define TAB_EVENT_WRITE_EVENT(evt)		hisysevent_write(evt)
#define TAB_EVENT_DESTROY_EVENT(evt)		hisysevent_destroy(&evt)
#elif defined(CONFIG_HUAWEI_DSM)
typedef struct hiview_hievent TAB_EVENT;
#define TAB_EVENT_CREATE_EVENT(...)		\
hiview_hievent_create(TAB_EVENT_EXTDMD_NO)
#define TAB_EVENT_PUT_INTEGER(evt, key, val)	hiview_hievent_put_integral(evt, key, val)
#define TAB_EVENT_PUT_STRING(evt, key, val)  	hiview_hievent_put_string(evt, key, val)
#define TAB_EVENT_WRITE_EVENT(evt)		hiview_hievent_report(evt)
#define TAB_EVENT_DESTROY_EVENT(evt)		hiview_hievent_destroy(evt)
#else
typedef void TAB_EVENT;
#define TAB_EVENT_CREATE_EVENT(...) 		NULL
#define TAB_EVENT_PUT_INTEGER(evt, key, val)
#define TAB_EVENT_PUT_STRING(evt, key, val)
#define TAB_EVENT_WRITE_EVENT(evt) 		0
#define TAB_EVENT_DESTROY_EVENT(evt)
#endif

const int NEGATIVE = -1;

static time64_t tab_monitor_get_current_timestamp(void)
{
	struct timespec64 now = {0};
	ktime_get_coarse_real_ts64(&now);

	return now.tv_sec;
}

static int tab_monitor_dump_data_to_buf(struct charge_data *data, char *buf, int len, int mode, int ver)
{
    time64_t now = tab_monitor_get_current_timestamp();
	return snprintf_s(buf, len, len - 1,
		"BSOH_EVT=EVT_BBD@time:%lld,t0:%lld,r0:%d,r1:%d,cur0_t1:%d,cur1_t1:%d,temp0:%d,temp1:%d,brand0:%s,brand1:%s,"
		"cycle:%d,character:%d,mode:%d,uisoc_t0:%d,uisoc_t1:%d,ufsoc0_t0:%d,ufsoc1_t0:%d,ufsoc0_t1:%d,ufsoc1_t1:%d,"
		"cc0_t0:%d,cc0_t1:%d,cc1_t0:%d,cc1_t1:%d,vol0_t1:%d,vol1_t1:%d,fcc0:%d,fcc1:%d,version:%d\n",
		now, data->start_time, data->batt_0.resistance, data->batt_1.resistance,
		data->batt_0.current_end, data->batt_1.current_end,
		data->batt_0.temp, data->batt_1.temp,
		coul_merge_drv_battery_brand(BATT_0), coul_merge_drv_battery_brand(BATT_1),
		data->cycle, data->ratio, mode,
		data->uisoc_start, data->uisoc_end,
		data->batt_0.ufsoc_start, data->batt_1.ufsoc_start,
		data->batt_0.ufsoc_end, data->batt_1.ufsoc_end,
		data->batt_0.cc_start, data->batt_0.cc_end,
		data->batt_1.cc_start, data->batt_1.cc_end,
		data->batt_0.voltage_end, data->batt_1.voltage_end,
		data->batt_0.fcc, data->batt_1.fcc, ver);
}

#ifdef CONFIG_HUAWEI_CHARGER_AP
static int tab_send_uevent(struct charge_data *data, int mode, int ver)
{
    struct power_event_notify_data notify_data;
    char buf[POWER_EVENT_NOTIFY_SIZE] = { 0 };
	int len;

	len = tab_monitor_dump_data_to_buf(data, buf, POWER_EVENT_NOTIFY_SIZE, mode, ver);
    if (len < 0) {
        tab_monitor_err("dump uevent buf error\n");
        return -1;
    }
    tab_monitor_err("%s\n", buf);

	notify_data.event_len = len;
	notify_data.event = buf;
	power_event_report_uevent(&notify_data);
    return 0;
}

#else
static int tab_send_uevent(struct charge_data *data, int mode, int ver)
{
	return 0;
}
#endif

static int tab_bigdata_report(struct charge_data *data, int mode, int ver)
{
	int ret;
	TAB_EVENT *event;
	time64_t now = tab_monitor_get_current_timestamp();
	int *adp_type;

	adp_type = NULL;
    event = TAB_EVENT_CREATE_EVENT();
	if (event == NULL) {
		tab_monitor_err("hievent create fail");
		return -EPERM;
	}
    TAB_EVENT_PUT_INTEGER(event, "character", data->ratio);
	TAB_EVENT_PUT_INTEGER(event, "t0", data->start_time);
    TAB_EVENT_PUT_INTEGER(event, "t1", now);
    TAB_EVENT_PUT_INTEGER(event, "cc0_t0", data->batt_0.cc_start);
    TAB_EVENT_PUT_INTEGER(event, "cc1_t0", data->batt_1.cc_start);
    TAB_EVENT_PUT_INTEGER(event, "cc0_t1", data->batt_0.cc_end);
    TAB_EVENT_PUT_INTEGER(event, "cc1_t1", data->batt_1.cc_end);
    TAB_EVENT_PUT_INTEGER(event, "cur_0", data->batt_0.current_end);
    TAB_EVENT_PUT_INTEGER(event, "cur_1", data->batt_1.current_end);
    TAB_EVENT_PUT_INTEGER(event, "vol0_t1", data->batt_0.voltage_end);
    TAB_EVENT_PUT_INTEGER(event, "vol1_t1", data->batt_1.voltage_end);
    TAB_EVENT_PUT_INTEGER(event, "r0", data->batt_0.resistance);
    TAB_EVENT_PUT_INTEGER(event, "r1", data->batt_1.resistance);
#ifdef CONFIG_HUAWEI_CHARGER_AP
	adapter_get_adp_type(ADAPTER_PROTOCOL_SCP, adp_type);
	if (adp_type != NULL)
		TAB_EVENT_PUT_INTEGER(event, "charge_type", *adp_type);
#else
    TAB_EVENT_PUT_INTEGER(event, "charge_type", -1);
#endif
    TAB_EVENT_PUT_INTEGER(event, "cycle0", data->batt_0.cycle);
    TAB_EVENT_PUT_INTEGER(event, "cycle1", data->batt_1.cycle);
    TAB_EVENT_PUT_INTEGER(event, "ufsoc0_t0", data->batt_0.ufsoc_start);
    TAB_EVENT_PUT_INTEGER(event, "ufsoc1_t0", data->batt_1.ufsoc_start);
    TAB_EVENT_PUT_INTEGER(event, "ufsoc0_t1", data->batt_0.ufsoc_end);
    TAB_EVENT_PUT_INTEGER(event, "ufsoc1_t1", data->batt_1.ufsoc_end);
    TAB_EVENT_PUT_INTEGER(event, "fcc0", data->batt_0.fcc);
    TAB_EVENT_PUT_INTEGER(event, "fcc1", data->batt_1.fcc);
    TAB_EVENT_PUT_STRING(event, "batbrand0", coul_merge_drv_battery_brand(BATT_0));
    TAB_EVENT_PUT_STRING(event, "batbrand1", coul_merge_drv_battery_brand(BATT_1));
    TAB_EVENT_PUT_INTEGER(event, "mode", mode);
    TAB_EVENT_PUT_INTEGER(event, "version", ver);
    TAB_EVENT_PUT_INTEGER(event, "action_flag", 0); // currently useless
    TAB_EVENT_PUT_INTEGER(event, "failure_threshold", 0); // currently useless
    TAB_EVENT_PUT_INTEGER(event, "parameter1", data->uisoc_start);
    TAB_EVENT_PUT_INTEGER(event, "parameter2", data->uisoc_end);
    TAB_EVENT_PUT_INTEGER(event, "parameter3", data->temp);
    ret = TAB_EVENT_WRITE_EVENT(event);
    TAB_EVENT_DESTROY_EVENT(event);
    if (ret < 0) {
        tab_monitor_err("%s hievent report fail: %d\n", __func__, ret);
        return -EPERM;
    }
	return 0;
}

static int charge_done_assign_cur_vol(struct tab_health_monitor_device *di) 
{
	di->charge_done_data.batt_0.voltage_end = di->charge_series_data.batt_0.fifo_vol[FIFO_MAX-1];
	di->charge_done_data.batt_1.voltage_end = di->charge_series_data.batt_1.fifo_vol[FIFO_MAX-1];
	di->charge_done_data.batt_0.current_end = di->charge_series_data.batt_0.fifo_cur[FIFO_MAX-1];
	di->charge_done_data.batt_1.current_end = di->charge_series_data.batt_1.fifo_cur[FIFO_MAX-1];

	if (abs(di->charge_done_data.batt_1.current_end - di->charge_done_data.batt_0.current_end)
		> di->max_charge_done_current_diff_ma) {
		return -1;
	}

	if (di->charge_done_data.batt_0.current_end > di->max_charge_done_current_ma ||
		di->charge_done_data.batt_1.current_end > di->max_charge_done_current_ma) {
		return -1;
	}

	return 0;
}

static int series_check_vol_cur(struct tab_health_monitor_device *di)
{
	int i, min_0, max_0, min_1, max_1, sum_0, sum_1;

	sum_0 = 0;
	sum_1 = 0;
	min_0 = INT_MAX;
	max_0 = INT_MIN;
	min_1 = INT_MAX;
	max_1 = INT_MIN;

	for (i = 0; i < FIFO_MAX; i++) {
		min_0 = min(min_0, di->charge_series_data.batt_0.fifo_vol[i]);
		max_0 = max(max_0, di->charge_series_data.batt_0.fifo_vol[i]);
		min_1 = min(min_1, di->charge_series_data.batt_1.fifo_vol[i]);
		max_1 = max(max_1, di->charge_series_data.batt_1.fifo_vol[i]);
		sum_0 += di->charge_series_data.batt_0.fifo_vol[i];
		sum_1 += di->charge_series_data.batt_1.fifo_vol[i];
	}

	if (max_0 - min_0 > di->max_voltage_diff_mv || max_1 - min_1 > di->max_voltage_diff_mv) {
		tab_monitor_err("[%s] voltage difference too big, minmax_0: %d, minmax_1: %d\n"
			, __func__, max_0 - min_0, max_1 - min_1);
		return -1;
	}

	di->charge_series_data.batt_0.voltage_end = sum_0 / FIFO_MAX;
	di->charge_series_data.batt_1.voltage_end = sum_1 / FIFO_MAX;
	di->charge_done_data.batt_0.voltage_end = sum_0 / FIFO_MAX;
	di->charge_done_data.batt_1.voltage_end = sum_1 / FIFO_MAX;

	sum_0 = 0;
	sum_1 = 0;
	min_0 = INT_MAX;
	max_0 = INT_MIN;
	min_1 = INT_MAX;
	max_1 = INT_MIN;

	for (i = 0; i < FIFO_MAX; i++) {
		min_0 = min(min_0, di->charge_series_data.batt_0.fifo_cur[i]);
		max_0 = max(max_0, di->charge_series_data.batt_0.fifo_cur[i]);
		min_1 = min(min_1, di->charge_series_data.batt_1.fifo_cur[i]);
		max_1 = max(max_1, di->charge_series_data.batt_1.fifo_cur[i]);
		sum_0 += di->charge_series_data.batt_0.fifo_cur[i];
		sum_1 += di->charge_series_data.batt_1.fifo_cur[i];
	}

	if (max_0 - min_0 > di->max_current_diff_ma || max_1 - min_1 > di->max_current_diff_ma ||
		abs(sum_1 - sum_0) / FIFO_MAX > di->max_current_diff_batt_01_ma) {
		tab_monitor_err("[%s] current difference too big, minmax_0: %d, minmax_1: %d, sum: %d\n"
			, __func__, max_0 - min_0, max_1 - min_1, abs(sum_1 - sum_0) / FIFO_MAX);
		return -1;
	}

	di->charge_series_data.batt_0.current_end = sum_0 / FIFO_MAX;
	di->charge_series_data.batt_1.current_end = sum_1 / FIFO_MAX;
	di->charge_done_data.batt_0.current_end = sum_0 / FIFO_MAX;
	di->charge_done_data.batt_1.current_end = sum_1 / FIFO_MAX;

	return 0;
}

static void fifo_cur_vol_work(struct work_struct *work)
{
	struct tab_health_monitor_device *di =
		container_of(work, struct tab_health_monitor_device, get_cur_vol_delayed_work.work);
	
	int current_ma_0, current_ma_1, voltage_mv_0, voltage_mv_1, i;

	i = di->fifo_index;
	current_ma_0 = coul_merge_drv_battery_current(BATT_0);
	current_ma_1 = coul_merge_drv_battery_current(BATT_1);
	voltage_mv_0 = coul_merge_drv_battery_voltage_mv(BATT_0);
	voltage_mv_1 = coul_merge_drv_battery_voltage_mv(BATT_1);

	di->charge_series_data.batt_0.fifo_cur[i] = current_ma_0;
	di->charge_series_data.batt_1.fifo_cur[i] = current_ma_1;
	di->charge_series_data.batt_0.fifo_vol[i] = voltage_mv_0;
	di->charge_series_data.batt_1.fifo_vol[i] = voltage_mv_1;

	di->fifo_index++;
	if (di->fifo_index == 5) {
		di->fifo_index = 0;
	}

	queue_delayed_work(system_power_efficient_wq,
		&di->get_cur_vol_delayed_work, msecs_to_jiffies(DELAY_1_SECOND));
}

static int series_charge_handle(struct tab_health_monitor_device *di)
{
	int ratio;
	time64_t now = tab_monitor_get_current_timestamp();
	int temp0 = coul_merge_drv_battery_temperature(BATT_0);
	int temp1 = coul_merge_drv_battery_temperature(BATT_1);
	int cc0 = coul_merge_drv_battery_cc(BATT_0);
	int cc1 = coul_merge_drv_battery_cc(BATT_1);
	int ufsoc0 = coul_merge_drv_battery_unfiltered_capacity(BATT_0);
	int ufsoc1 = coul_merge_drv_battery_unfiltered_capacity(BATT_1);
	int resistance0 = -1;
	int resistance1 = -1;
	int cc0_diff, cc1_diff;

	tab_monitor_info("[%s] cc0start: %d, cc1start: %d, cc0end: %d, cc1end: %d\n",
		__func__, di->charge_series_data.batt_0.cc_start, di->charge_series_data.batt_1.cc_start, cc0, cc1);

	if (di->charge_series_data.start_time == 0) {
		tab_monitor_err("[%s] charge_series_data.start_time not init \n", __func__);
		return -1;
	}

	if (now - di->charge_series_data.start_time > di->max_series_charge_time_sec) {
		tab_monitor_err("[%s] charge time longer than 38mins \n", __func__);
		return -1;
	}

	if (temp0 <= di->min_valid_temperature || temp1 <= di->min_valid_temperature) {
		tab_monitor_err("[%s] temp lower than %d \n", __func__, di->min_valid_temperature);
		return -1;
	}

	if (di->current_charge_mode != BAT_SERIES_MODE) {
		tab_monitor_err("[%s] charge mode is parallel \n", __func__);
		return -1;
	}

	cc0_diff = abs(cc0 - di->charge_series_data.batt_0.cc_start);
	cc1_diff = abs(cc1 - di->charge_series_data.batt_1.cc_start);
	if (abs(cc0_diff - cc1_diff) > di->max_cc_diff_gap_mah ||
		cc0_diff < di->min_cc_diff_mah || cc1_diff < di->min_cc_diff_mah) {
		tab_monitor_err("[%s] cc difference too big \n", __func__);
		return -1;
	}

	if (series_check_vol_cur(di))
		return -1;

	if (resistance0 == -1 || resistance1 == -1) {
		tab_monitor_err("[%s] get resistance from eis fail \n", __func__);
		di->resistance_differnece = di->avg_resistance_diff_mohm;
	} else {
		di->resistance_differnece = resistance1 - resistance0;
	}

	// -1 * k * ( 1000 * (batt_0 V_end - batt_1 V_end) + batt_1 I_end * (batt_1 R_end - batt_0 R_end)) / abs(batt_1 CC_end - batt_1 CC_start)
	if (cc1_diff == 0) {
		tab_monitor_err("[%s] cc1_diff is 0, ratio division fail \n", __func__);
		return -1;
	}

	ratio = NEGATIVE * di->k_ratio_soc_percent_per_mv
		* (PERMILLAGE * (di->charge_series_data.batt_0.voltage_end - di->charge_series_data.batt_1.voltage_end)
		- di->charge_series_data.batt_1.current_end * di->resistance_differnece) / cc1_diff;

	di->charge_stop_time = now;
	di->charge_series_data.ratio = ratio;
	di->charge_series_data.batt_0.temp = temp0;
	di->charge_series_data.batt_1.temp = temp1;
	di->charge_series_data.temp = coul_drv_battery_temperature();
	di->charge_series_data.batt_0.cycle = coul_merge_drv_battery_cycle_count(BATT_0);
	di->charge_series_data.batt_1.cycle = coul_merge_drv_battery_cycle_count(BATT_1);
	di->charge_series_data.cycle = coul_drv_battery_cycle_count();
	di->charge_series_data.batt_0.cc_end = cc0;
	di->charge_series_data.batt_1.cc_end = cc1;
	di->charge_series_data.batt_0.ufsoc_end = ufsoc0;
	di->charge_series_data.batt_1.ufsoc_end = ufsoc1;
	di->charge_series_data.uisoc_end = di->batt_soc;
	di->charge_series_data.batt_0.resistance = resistance0;
	di->charge_series_data.batt_1.resistance = resistance1;
	di->charge_series_data.batt_0.fcc = coul_merge_drv_battery_fcc(BATT_0);
	di->charge_series_data.batt_1.fcc = coul_merge_drv_battery_fcc(BATT_1);

	tab_send_uevent(&(di->charge_series_data), BAT_SERIES_MODE, di->version);
	tab_bigdata_report(&(di->charge_series_data), BAT_SERIES_MODE, di->version);
	tab_monitor_info("[%s] Series mode, start time: %lld, end time: %lld, batt_0 voltage_end: %d, batt_1 voltage_end: %d,"
		"batt_0 current_end: %d, batt_1 current_end: %d, batt_0 temp: %d,"
		"batt_1 temp: %d, ratio: %d, uisoc start: %d, uisoc end: %d, batt_0 ufsoc start: %d, batt_1 ufsoc start: %d,"
		"batt_0 ufsoc end: %d, batt_1 ufsoc end: %d, batt_0 cc start: %d,"
		"batt_1 cc start: %d, batt_0 cc_end: %d, batt_1 cc_end: %d, batt_0 resistance: %d, batt_1 resistance: %d \n",
		__func__, di->charge_series_data.start_time, now, di->charge_series_data.batt_0.voltage_end,
		di->charge_series_data.batt_1.voltage_end,di->charge_series_data.batt_0.current_end,
		di->charge_series_data.batt_1.current_end, temp0, temp1, ratio, di->charge_series_data.uisoc_start,
		di->batt_soc, di->charge_series_data.batt_0.ufsoc_start,
		di->charge_series_data.batt_1.ufsoc_start, ufsoc0, ufsoc1,
		di->charge_series_data.batt_0.cc_start, di->charge_series_data.batt_1.cc_start, cc0, cc1, resistance0, resistance1);

	return 0;
}

static void tab_series_charge_handle_work(struct work_struct *work) {
	struct tab_health_monitor_device *di =
		container_of(work, struct tab_health_monitor_device, series_charge_handle_delayed_work.work);

	mutex_lock(&di->lock);
	series_charge_handle(di);
	di->charge_series_data.start_time = 0;
	mutex_unlock(&di->lock);
}

static int power_supply_callback(
	struct notifier_block *nb, unsigned long event, void *data)
{
	int ret;
	struct power_supply *psy = NULL;
	union power_supply_propval val = { 0 };
	struct tab_health_monitor_device *di =
		container_of(nb, struct tab_health_monitor_device, power_supply_nf);

	psy = power_supply_get_by_name(BATTERY_PSY_NAME);
	if (!psy) {
		tab_monitor_err("get power supply by name failed\n");
		return NOTIFY_DONE;
	}

	ret = power_supply_get_property(psy, POWER_SUPPLY_PROP_CAPACITY, &val);
	power_supply_put(psy);
	if (ret != 0) {
		tab_monitor_err("get capacity from power_supply failed: %d\n", ret);
		return NOTIFY_DONE;
	}

	if (di->batt_soc < di->series_handle_uisoc_th && val.intval >= di->series_handle_uisoc_th) {
		queue_delayed_work(system_power_efficient_wq,
			&di->series_charge_handle_delayed_work, msecs_to_jiffies(0));
	}
	di->batt_soc = val.intval;

	return NOTIFY_OK;
}

/* mode1, full charge -> calculate ratio */
static int charge_state_callback(
	struct notifier_block *nb, unsigned long event, void *data)
{
	int temp0, temp1, cc0, cc1, ufsoc0, ufsoc1, resistance0, resistance1;
	enum tab_charge_status_event new_event;
	time64_t now;
	struct tab_health_monitor_device *di =
		container_of(nb, struct tab_health_monitor_device, charge_state_nf);

	switch(event) {
		case VCHRG_START_CHARGING_EVENT:
		case VCHRG_START_AC_CHARGING_EVENT:
		case VCHRG_START_USB_CHARGING_EVENT:
			new_event = CHARGING;
			break;
		case VCHRG_CHARGE_DONE_EVENT:
			new_event = CHARGE_DONE;
			break;
		default:
			new_event = NOT_CHARGING;
			break;
	}

	if (di->current_charge_state == new_event) {
		return NOTIFY_OK;
	} 
	tab_monitor_info("[%s] current device event: %d, new event: %d \n", __func__, di->current_charge_state, new_event);
	di->current_charge_state = new_event;
	
	now = tab_monitor_get_current_timestamp();
	temp0 = coul_merge_drv_battery_temperature(BATT_0);
	temp1 = coul_merge_drv_battery_temperature(BATT_1);
	cc0 = coul_merge_drv_battery_cc(BATT_0);
	cc1 = coul_merge_drv_battery_cc(BATT_1);
	ufsoc0 = coul_merge_drv_battery_unfiltered_capacity(BATT_0);
	ufsoc1 = coul_merge_drv_battery_unfiltered_capacity(BATT_1);
	resistance0 = hisi_eis_get_resistance(BATT_0);
	resistance1 = hisi_eis_get_resistance(BATT_1);

	switch (di->current_charge_state) {
	case CHARGING:
		tab_monitor_info("Charging\n");
			
		/* start charge, delta ufsoc <= 2%, ufsoc <= 60, update CC */
		if (ufsoc0 > di->start_ufsoc_th || ufsoc1 > di->start_ufsoc_th ||
			abs(ufsoc0 - ufsoc1) > di->max_ufsoc_diff) {
			tab_monitor_err("[%s] check ufsoc at t0 \n", __func__);
			break;
		}

		di->charge_done_data.batt_0.cc_start = cc0;
		di->charge_done_data.batt_1.cc_start = cc1;
		di->charge_done_data.batt_0.ufsoc_start = ufsoc0;
		di->charge_done_data.batt_1.ufsoc_start = ufsoc1;
		di->charge_done_data.start_time = now;
		di->charge_done_data.uisoc_start = di->batt_soc;

		tab_monitor_info("[Charge Done]batt_0 start ufsoc: %d, batt_1 start ufsoc: %d, batt_0 start cc: %d, batt_1 start cc: %d, start time: %lld\n",
			di->charge_done_data.batt_0.ufsoc_start, di->charge_done_data.batt_1.ufsoc_start,
			di->charge_done_data.batt_0.cc_start, di->charge_done_data.batt_1.cc_start,
			di->charge_done_data.start_time);
		break;
	case CHARGE_DONE:
		tab_monitor_info("Charge full\n");

		/* full charge, calculate all struct members */
		if (di->charge_done_data.start_time == 0) {
			tab_monitor_err("[%s] charge_done_data.start_time not init \n", __func__);
			break;
		}

		if (temp0 <= di->min_valid_temperature || temp1 <= di->min_valid_temperature) {
			di->charge_done_data.start_time = 0;
			tab_monitor_err("[%s] temp lower than %d \n", __func__, di->min_valid_temperature);
			break;
		}

		if (now - di->charge_done_data.start_time > di->max_parallel_charge_time_sec) {
			di->charge_done_data.start_time = 0;
			tab_monitor_err("[%s] charge time longer than 8 hour \n", __func__);
			break;
		}

		if (charge_done_assign_cur_vol(di)) {
			di->charge_done_data.start_time = 0;
			tab_monitor_err("[%s] charge done current error \n", __func__);
			break;
		}

		di->charge_stop_time = now;
		di->charge_done_data.batt_0.cc_end = cc0;
		di->charge_done_data.batt_1.cc_end = cc1;
		di->charge_done_data.batt_0.ufsoc_end = ufsoc0;
		di->charge_done_data.batt_1.ufsoc_end = ufsoc1;
		di->charge_done_data.uisoc_end = di->batt_soc;
		di->charge_done_data.batt_0.temp = temp0;
		di->charge_done_data.batt_1.temp = temp1;
		di->charge_done_data.temp = coul_drv_battery_temperature();
		di->charge_done_data.batt_0.cycle = coul_merge_drv_battery_cycle_count(BATT_0);
		di->charge_done_data.batt_1.cycle = coul_merge_drv_battery_cycle_count(BATT_1);
		di->charge_done_data.cycle = coul_drv_battery_cycle_count();
		di->charge_done_data.batt_0.resistance = resistance0;
		di->charge_done_data.batt_1.resistance = resistance1;
		di->charge_done_data.batt_0.fcc = coul_merge_drv_battery_fcc(BATT_0);
		di->charge_done_data.batt_1.fcc = coul_merge_drv_battery_fcc(BATT_1);

		if (di->charge_done_data.batt_0.cc_end - di->charge_done_data.batt_0.cc_start == 0) {
			tab_monitor_err("[%s] cc0_diff is 0, ratio division fail \n", __func__);
			break;
		}
		di->charge_done_data.ratio =
			(di->charge_done_data.batt_1.cc_end - di->charge_done_data.batt_1.cc_start) * PERMILLAGE/
			(di->charge_done_data.batt_0.cc_end - di->charge_done_data.batt_0.cc_start);

		tab_monitor_info("[Charge Done]batt_0 end ufsoc: %d, batt_1 end ufsoc: %d, batt_0 end cc: %d, batt_1 end cc: %d,"
			"batt_0 fcc: %d, batt_1 fcc: %d, ratio: %d, end time: %lld\n",
			di->charge_done_data.batt_0.ufsoc_end, di->charge_done_data.batt_0.ufsoc_end,
			di->charge_done_data.batt_0.cc_end, di->charge_done_data.batt_1.cc_end,
			di->charge_done_data.batt_0.fcc, di->charge_done_data.batt_1.fcc,
			di->charge_done_data.ratio, di->charge_stop_time);

		tab_send_uevent(&(di->charge_done_data), BAT_PARALLEL_MODE, di->version);
		tab_bigdata_report(&(di->charge_done_data), BAT_PARALLEL_MODE, di->version);

		di->charge_done_data.start_time = 0;
		break;
	default:
		tab_monitor_info("[%s] Not charging event, mode0 start time = 0 \n", __func__);
		di->charge_done_data.start_time = 0;
		break;
	}
	
	return NOTIFY_OK;
}

static int series_parallel_callback(
	struct notifier_block *nb, unsigned long event, void *data)
{
	struct tab_health_monitor_device *di =
		container_of(nb, struct tab_health_monitor_device, series_parallel_mode_nf);

	/* batt0 or batt1 */
	time64_t now = tab_monitor_get_current_timestamp();
	int ufsoc0 = coul_merge_drv_battery_unfiltered_capacity(BATT_0);
	int ufsoc1 = coul_merge_drv_battery_unfiltered_capacity(BATT_1);
	int cc0 = coul_merge_drv_battery_cc(BATT_0);
	int cc1 = coul_merge_drv_battery_cc(BATT_1);

	tab_monitor_info("[%s] current event: %d, new event: %lu\n", __func__, di->current_charge_mode ,event);

	if (event == BAT_SERIES_MODE && di->current_charge_mode == BAT_PARALLEL_MODE) {
		if (ufsoc0 <= di->start_ufsoc_th && ufsoc1 <= di->start_ufsoc_th &&
			now - di->charge_stop_time > di->min_parallel_dischg_time_sec) {
			di->charge_series_data.batt_0.cc_start = cc0;
			di->charge_series_data.batt_1.cc_start = cc1;
			di->charge_series_data.batt_0.ufsoc_start = ufsoc0;
			di->charge_series_data.batt_1.ufsoc_start = ufsoc1;
			di->charge_series_data.start_time = now;
			di->charge_series_data.uisoc_start = di->batt_soc;

			tab_monitor_info("[Charge Series]batt_0 start ufsoc: %d, batt_1 start ufsoc: %d, batt_0 start cc: %d, batt_1 start cc: %d, start time: %lld\n",
				di->charge_series_data.batt_0.ufsoc_start, di->charge_series_data.batt_1.ufsoc_start,
				di->charge_series_data.batt_0.cc_start, di->charge_series_data.batt_1.cc_start,
				di->charge_series_data.start_time);
		}
	} else if (event == BAT_PARALLEL_MODE) {
		di->charge_series_data.start_time = 0;
		tab_monitor_info("[%s] parallel event, mode1 start time = 0 \n", __func__);
	}
	di->current_charge_mode = event;

	return NOTIFY_OK;
}

static int tab_health_monitor_parse_dts(struct tab_health_monitor_device *di)
{
	struct device_node *np = di->dev->of_node;
	int ret;

	ret = of_property_read_u32(np, "max_charge_done_current_ma", &di->max_charge_done_current_ma);
	if (ret) {
		tab_monitor_err("cannot get max_charge_done_current_ma\n");
		return -1;
	}

	ret = of_property_read_u32(np, "max_charge_done_current_diff_ma", &di->max_charge_done_current_diff_ma);
	if (ret) {
		tab_monitor_err("cannot get max_charge_done_current_diff_ma\n");
		return -1;
	}

	ret = of_property_read_u32(np, "max_current_diff_batt_01_ma", &di->max_current_diff_batt_01_ma);
	if (ret) {
		tab_monitor_err("cannot get max_current_diff_batt_01_ma\n");
		return -1;
	}
	ret = of_property_read_u32(np, "max_current_diff_ma", &di->max_current_diff_ma);
	if (ret) {
		tab_monitor_err("cannot get max_current_diff_ma\n");
		return -1;
	}
	ret = of_property_read_u32(np, "max_voltage_diff_mv", &di->max_voltage_diff_mv);
	if (ret) {
		tab_monitor_err("cannot get max_voltage_diff_mv\n");
		return -1;
	}
	ret = of_property_read_u32(np, "max_series_charge_time_sec", &di->max_series_charge_time_sec);
	if (ret) {
		tab_monitor_err("cannot get max_series_charge_time_sec\n");
		return -1;
	}
	ret = of_property_read_u32(np, "max_parallel_charge_time_sec", &di->max_parallel_charge_time_sec);
	if (ret) {
		tab_monitor_err("cannot get max_parallel_charge_time_sec\n");
		return -1;
	}
	ret = of_property_read_u32(np, "min_parallel_dischg_time_sec", &di->min_parallel_dischg_time_sec);
	if (ret) {
		tab_monitor_err("cannot get min_parallel_dischg_time_sec\n");
		return -1;
	}
	ret = of_property_read_u32(np, "min_valid_temperature", &di->min_valid_temperature);
	if (ret) {
		tab_monitor_err("cannot get min_valid_temperature\n");
		return -1;
	}
	ret = of_property_read_u32(np, "max_cc_diff_gap_mah", &di->max_cc_diff_gap_mah);
	if (ret) {
		tab_monitor_err("cannot get max_cc_diff_gap_mah\n");
		return -1;
	}
	ret = of_property_read_u32(np, "min_cc_diff_mah", &di->min_cc_diff_mah);
	if (ret) {
		tab_monitor_err("cannot get min_cc_diff_mah\n");
		return -1;
	}
	ret = of_property_read_u32(np, "avg_resistance_diff_mohm", &di->avg_resistance_diff_mohm);
	if (ret) {
		tab_monitor_err("cannot get avg_resistance_diff_mohm\n");
		return -1;
	}
	ret = of_property_read_u32(np, "series_handle_uisoc_th", &di->series_handle_uisoc_th);
	if (ret) {
		tab_monitor_err("cannot get series_handle_uisoc_th\n");
		return -1;
	}
	ret = of_property_read_u32(np, "start_ufsoc_th", &di->start_ufsoc_th);
	if (ret) {
		tab_monitor_err("cannot get start_ufsoc_th\n");
		return -1;
	}
	ret = of_property_read_u32(np, "max_ufsoc_diff", &di->max_ufsoc_diff);
	if (ret) {
		tab_monitor_err("cannot get max_ufsoc_diff\n");
		return -1;
	}
	ret = of_property_read_u32(np, "k_ratio_soc_percent_per_mv", &di->k_ratio_soc_percent_per_mv);
	if (ret) {
		tab_monitor_err("cannot get k_ratio_soc_percent_per_mv\n");
		return -1;
	}
	ret = of_property_read_u32(np, "version", &di->version);
	if (ret) {
		tab_monitor_err("cannot get version\n");
		return -1;
	}

	return 0;
}

static int tab_health_monitor_probe(struct platform_device *pdev)
{
	struct tab_health_monitor_device *di = NULL;
	int ret;

	if (!pdev || !pdev->dev.of_node)
		return -ENODEV;

	di = devm_kzalloc(&pdev->dev, sizeof(*di), GFP_KERNEL);
	if (!di)
		return -ENOMEM;

	di->dev = &pdev->dev;
	ret = tab_health_monitor_parse_dts(di);
	if(ret) {
		tab_monitor_err("[%s] parse fail, return \n", __func__);
		return 0;
	}
	di->batt_soc = 0;
	di->charge_stop_time = 0;
	di->fifo_index = 0;
	di->valid_fifo_count = 0;
	di->current_charge_mode = BAT_PARALLEL_MODE;
	di->current_charge_state = NOT_CHARGING;
	mutex_init(&di->lock);

	di->charge_state_nf.notifier_call = charge_state_callback;
	bci_register_notifier(&di->charge_state_nf, 1);
	di->power_supply_nf.notifier_call = power_supply_callback;
	power_supply_reg_notifier(&di->power_supply_nf);
	di->series_parallel_mode_nf.notifier_call = series_parallel_callback;
	batt_mode_register_notifier(&di->series_parallel_mode_nf);
	INIT_DELAYED_WORK(&di->series_charge_handle_delayed_work, tab_series_charge_handle_work);
	INIT_DELAYED_WORK(&di->get_cur_vol_delayed_work, fifo_cur_vol_work);
	queue_delayed_work(system_power_efficient_wq,
		&di->get_cur_vol_delayed_work, msecs_to_jiffies(0));

	return 0;
}

static int tab_health_monitor_remove(struct platform_device *pdev)
{
	struct tab_health_monitor_device *di = dev_get_drvdata(&pdev->dev);

	bci_unregister_notifier(&di->charge_state_nf, 1);
	power_supply_unreg_notifier(&di->power_supply_nf);
	batt_mode_unregister_notifier(&di->series_parallel_mode_nf);
	di = NULL;

	return 0;
}

static const struct of_device_id tab_health_monitor_table[] = {
	{
		.compatible = "hisilicon,tab_health_monitor",
		.data = NULL,
	},
	{},
};

static struct platform_driver tab_health_monitor_driver = {
	.probe = tab_health_monitor_probe,
	.remove = tab_health_monitor_remove,
	.driver = {
		.name = "hisilicon,tab_health_monitor",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(tab_health_monitor_table),
	},
};

static int __init tab_health_monitor_init(void)
{
	return platform_driver_register(&tab_health_monitor_driver);
}

static void __exit tab_health_monitor_exit(void)
{
	platform_driver_unregister(&tab_health_monitor_driver);
}

late_initcall(tab_health_monitor_init);
module_exit(tab_health_monitor_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("battery tab health monitor driver");