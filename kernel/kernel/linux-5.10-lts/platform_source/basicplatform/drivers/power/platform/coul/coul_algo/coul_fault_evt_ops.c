/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: fault event handler function for coul module
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
#include <platform_include/basicplatform/linux/power/platform/coul/coul_private_interface.h>
#ifdef CONFIG_HUAWEI_CHARGER_AP
#include <chipset_common/hwpower/common_module/power_event_ne.h>
#endif
#include "coul_ocv_ops.h"
#include "coul_temp.h"
#include "coul_fault_evt_ops.h"
#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
#include "coul_dsm.h"
#endif
#include <securec.h>
#include <linux/rtc.h>

#define LOW_INT_VOL_SLEEP_TIME  1000
#define LOW_INT_VOL_OFFSET      10 /* mv */
#define BATTERY_VOL_LOW_ERR     2900

void coul_core_lock_init(struct smartstar_coul_device *di)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 4, 0))
	di->g_coul_lock = wakeup_source_register(NULL, "coul_wakelock");
#else
	di->g_coul_lock = wakeup_source_register("coul_wakelock");
#endif
	if (!di->g_coul_lock) {
		coul_core_err("%s wakeup source register failed\n", __func__);
		return;
	}
}

void coul_core_lock_trash(struct smartstar_coul_device *di)
{
	wakeup_source_unregister(di->g_coul_lock);
}

/* apply coul wake_lock */
static void coul_wake_lock(struct smartstar_coul_device *di)
{
	if (!di->g_coul_lock->active) {
		__pm_stay_awake(di->g_coul_lock);
		coul_core_info("coul core wake lock\n");
	}
}

/* release coul wake_lock */
static void coul_wake_unlock(struct smartstar_coul_device *di)
{
	if (di->g_coul_lock->active) {
		__pm_relax(di->g_coul_lock);
		coul_core_info("coul core wake unlock\n");
	}
}

/* get cutoff interrupt vol mv */
static int coul_get_low_int_vol_mv(struct smartstar_coul_device *di)
{
	int vol;
	int delt = 0;

	if (di == NULL)
		return 0;

	if (di->v_low_int_value == LOW_INT_STATE_RUNNING)
		vol = (int)di->v_cutoff;
	else
		vol = (int)di->v_cutoff_sleep;

	if (di->batt_temp <= LOW_INT_TEMP_THRED)
		vol = (di->v_cutoff_low_temp < vol) ? di->v_cutoff_low_temp : vol;

	if (di->enable_basp_zero_ocv == 1) {
		delt = di->basp_zero_ocv - di->uuc_zero_vol;
		if (delt < 0)
			delt = 0;
		vol = min(vol + delt, OCV_BY_MAP_MAX);
	}

	coul_core_info("%s batt%d, v_cut =%u, v_sleep =%u, v_low_temp = %d, temp = %d, delt = %d, vol = %d\n",
		__func__, di->batt_index, di->v_cutoff, di->v_cutoff_sleep,
		di->v_cutoff_low_temp, di->batt_temp, delt, vol);
	return vol;
}

static void judge_dsm_report(struct smartstar_coul_device *di, int delta_soc,
	int ibat_ua, int vbat_uv)
{
	char buff[DSM_BUFF_SIZE_MAX] = {0};

	if ((delta_soc >= ABNORMAL_SHUTDOWN_SOC) &&
		(coul_get_temperature_stably(di, USER_COUL) >
			TEMP_OCV_ALLOW_CLEAR * TENTH)) {
		coul_core_info("delta_soc=%d, mark save ocv is invalid\n",
			delta_soc);
		/*
		 * dmd report: current information
		 * -- fcc, CC, cur_vol, cur_current, cur_temp, cur_soc, delta_soc
		 */
		snprintf_s(buff, DSM_BUFF_SIZE_MAX, DSM_BUFF_SIZE_MAX - 1,
			 "[LOW VOL] fcc:%dmAh, CC:%dmAh, cur_vol:%dmV, cur_current:%dmA, "
			 "cur_temp:%d, cur_soc:%d, delta_soc:%d, batt_soc_real=%d, ocv_interval:%d",
			 di->batt_fcc / UA_PER_MA,
			 di->coul_dev_ops->calculate_cc_uah(di->batt_index) / UA_PER_MA,
			 vbat_uv / UVOLT_PER_MVOLT, -(ibat_ua / UA_PER_MA),
			 di->batt_temp, di->batt_soc, delta_soc, di->batt_soc_real, di->get_cc_end_time);
#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
		coul_core_dsm_report_ocv_cali_info(di, ERROR_LOW_VOL_INT, buff);
#endif
		if (delta_soc > ABNORMAL_DELTA_SOC) {
			di->coul_dev_ops->clear_ocv(di->batt_index);
			di->last_ocv_level = INVALID_SAVE_OCV_LEVEL;
			di->coul_dev_ops->save_ocv_level(di->batt_index, di->last_ocv_level);
			coul_set_fcc_flag(di, NOT_UPDATE_FCC);
		}
	}
}

#ifdef CONFIG_HUAWEI_CHARGER_AP
static void coul_send_under_voltage_event(void)
{
	struct power_event_notify_data n_data;

	n_data.event = "BATTERY_UNDER_VOLTAGE=1";
	n_data.event_len = 23; /* length of BATTERY_UNDER_VOLTAGE=1 */
	power_event_report_uevent(&n_data);
	coul_core_err("battery under voltage, report uevent request shutdown\n");
}
#endif

static void handle_low_vol_int(struct smartstar_coul_device *di,
	int ibat_ua, int vbat_uv, int cut_off_vol)
{
	int delta_soc;
	/* low_vol_int count according to dts */
	int count = (int)di->low_vol_filter_cnt;
	struct blocking_notifier_head *notifier_list = NULL;

	get_notifier_list(&notifier_list);

	/* if battery vol is too low, it's false */
	if (vbat_uv / PERMILLAGE < BATTERY_VOL_LOW_ERR)
		coul_core_err("[batt%d]Battery vol too low\n", di->batt_index);

	if ((vbat_uv / PERMILLAGE - LOW_INT_VOL_OFFSET) > cut_off_vol) {
		/* false low vol int, next suspend don't cali */
		coul_core_err("[batt%d]false low_int, in sleep\n", di->batt_index);
		return;
	}
	while (count--) {
		/* sleep 1s */
		msleep(LOW_INT_VOL_SLEEP_TIME);
		coul_core_get_battery_voltage_and_current(di, &ibat_ua,
			&vbat_uv);
		coul_core_err("[batt%d] delay 1000ms and get vbat_uv is %duv\n",
			 di->batt_index, vbat_uv);
		di->coul_dev_ops->show_key_reg();
		if ((vbat_uv / PERMILLAGE - LOW_INT_VOL_OFFSET) > cut_off_vol) {
			/* false low vol int, next suspend don't cali */
			coul_core_err("fifo0 is false, it's got in 32k clk period\n");
			return;
		}
	}

	if (cut_off_vol >= BATTERY_VOL_2_PERCENT) {
		delta_soc = di->batt_soc - SOC_TWO;
		di->batt_soc = 1;
		if (di->soe_enable) {
			di->batt_soe = 1;
			di->batt_capacity_1000x = PERMILLAGE;
		}
	} else {
		delta_soc = di->batt_soc;
		di->batt_soc = 0;
		if (di->soe_enable) {
			di->batt_soe = 0;
			di->batt_capacity_1000x = 0;
		}
	}
	/* under voltage flag */
	di->batt_under_voltage_flag = 1;
#ifdef CONFIG_HUAWEI_CHARGER_AP
	coul_send_under_voltage_event();
#endif
	judge_dsm_report(di, delta_soc, ibat_ua, vbat_uv);
	blocking_notifier_call_chain(notifier_list, BATTERY_LOW_SHUTDOWN, NULL);
}

static void coul_low_vol_int_handle(struct smartstar_coul_device *di)
{
	int ibat_ua = 0;
	int vbat_uv = 0;
	int cut_off_vol;

	if (di == NULL) {
		coul_core_info("NULL point in %s\n", __func__);
		return;
	}

	if (di->is_board_type != BAT_BOARD_ASIC)
		return;

	cut_off_vol = coul_get_low_int_vol_mv(di);

	coul_core_get_battery_voltage_and_current(di, &ibat_ua, &vbat_uv);
	if ((vbat_uv / PERMILLAGE - LOW_INT_VOL_OFFSET) < cut_off_vol)
		coul_core_info("IRQ: batt%d low_vol_int current_vol is %d < %d, use fifo vol!\n",
			di->batt_index, vbat_uv / PERMILLAGE, cut_off_vol);

	coul_core_err("IRQ: batt%d, low_vol_int, cur_batt_soc=%d%%, cur_voltage=%dmv, cur_current=%dma, cur_temperature=%d\n",
		di->batt_index, di->batt_soc, vbat_uv / PERMILLAGE,
		-(ibat_ua / PERMILLAGE), di->batt_temp / TENTH);

	if (!di->batt_exist) {
		coul_core_err("IRQ: COUL_VBAT_LOW_INT, no battery, error\n");
		return;
	}

	if (strstr(saved_command_line, "ohos.boot.swtype=factory") || 
		strstr(saved_command_line, "androidboot.swtype=factory")) {
		coul_core_err("IRQ: COUL_VBAT_LOW_INT, factory_version, do nothing\n");
		return;
	}

	handle_low_vol_int(di, ibat_ua, vbat_uv, cut_off_vol);
}

/* update coulomb start value */
static void make_cc_no_overload(struct smartstar_coul_device *di)
{
	int cc;

	if (di == NULL) {
		coul_core_info("NULL point in %s\n", __func__);
		return;
	}
	cc = di->coul_dev_ops->calculate_cc_uah(di->batt_index);
	di->coul_dev_ops->save_cc_uah(di->batt_index, cc);
}

void report_coul_drop_dsm_second_work(struct work_struct *work)
{
	struct smartstar_coul_device *di =
		container_of(work, struct smartstar_coul_device, coul_drop_work);

	struct coul_drop_dsm coul_drop;
	struct rtc_time tm;
	int i;
	short j;
	char dsm_buf[DSM_BUFF_SIZE_MAX] = { 0 };

	/* second report */
	for (i = 0; i < FIFO_LEN; i++) {
		usleep_range(V_DROP_INTERNAL_MIN, V_DROP_INTERNAL_MAX);
		coul_drop.vol_mv_drop[FIFO_LEN - i - 1] = 
			di->coul_dev_ops->get_battery_vol_uv_drop_from_fifo(di->batt_index, 0) / PERMILLAGE;
	}

	for (j = 0 ; j < FIFO_LEN; j++)
		coul_drop.vol_mv_fifo[j] = 
			di->coul_dev_ops->get_battery_vol_uv_from_fifo(di->batt_index, j) / PERMILLAGE;
	for (j = 0 ; j < FIFO_LEN; j++)
		coul_drop.cur_ma_fifo[j] = 
			di->coul_dev_ops->get_battery_cur_ua_from_fifo(di->batt_index, j) / PERMILLAGE;

	coul_drop.charge_status = (di->charging_state == CHARGING_STATE_CHARGE_NOT_CHARGE) ? 0 : 1;
	coul_drop.cycle = (int)(di->batt_chargecycles / PERCENT);
	coul_drop.capfcc_mah = di->batt_fcc / PERMILLAGE;
	(void)strcpy_s(coul_drop.batvendor, BATT_NAME_SIZE_MAX, di->batt_data->batt_brand);
	coul_drop.soc = di->batt_soc;
	coul_drop.temp = di->batt_temp / TENTH;
	coul_drop.drop_thresthold = di->coul_dev_ops->get_coul_drop_th(di->batt_index);

	(void)memset_s(dsm_buf, DSM_BUFF_SIZE_MAX, 0, DSM_BUFF_SIZE_MAX);
	ktime_get_real_ts64(&coul_drop.ts);
	rtc_time_to_tm((unsigned long)(coul_drop.ts.tv_sec), &tm);

	(void)snprintf_s(dsm_buf, DSM_BUFF_SIZE_MAX, DSM_BUFF_SIZE_MAX - 1,
		"[coul_drop] index=%d, charge_status=%d, cycle=%d, capfcc=%d, batvender=%s, "
		"voltage_drop=[%d %d %d %d %d %d %d %d], "
		"voltage_fifo=[%d %d %d %d %d %d %d %d], "
		"current_drop=[%d %d %d %d %d %d %d %d], "
		"time_drop=%04d-%02d-%02d %02d:%02d:%02d:%03lu, "
		"soc=%d, temp=%d, cnt=1, drop_threshold=%d\n",
		di->batt_index, coul_drop.charge_status, coul_drop.cycle, coul_drop.capfcc_mah, coul_drop.batvendor,
		coul_drop.vol_mv_drop[0], coul_drop.vol_mv_drop[1], coul_drop.vol_mv_drop[2], coul_drop.vol_mv_drop[3],
		coul_drop.vol_mv_drop[4], coul_drop.vol_mv_drop[5], coul_drop.vol_mv_drop[6], coul_drop.vol_mv_drop[7],
		coul_drop.vol_mv_fifo[0], coul_drop.vol_mv_fifo[1], coul_drop.vol_mv_fifo[2], coul_drop.vol_mv_fifo[3],
		coul_drop.vol_mv_fifo[4], coul_drop.vol_mv_fifo[5], coul_drop.vol_mv_fifo[6], coul_drop.vol_mv_fifo[7],
		coul_drop.cur_ma_fifo[0], coul_drop.cur_ma_fifo[1], coul_drop.cur_ma_fifo[2], coul_drop.cur_ma_fifo[3],
		coul_drop.cur_ma_fifo[4], coul_drop.cur_ma_fifo[5], coul_drop.cur_ma_fifo[6], coul_drop.cur_ma_fifo[7],
		tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
		tm.tm_hour, tm.tm_min, tm.tm_sec, coul_drop.ts.tv_nsec / NSEC_TO_MSEC,
		coul_drop.soc, coul_drop.temp, coul_drop.drop_thresthold);

#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
	power_dsm_report_dmd(POWER_DSM_BATTERY, DSM_BATTERY_COUL_DROP_INT, dsm_buf);
#endif

	di->coul_dev_ops->coul_drop_enable(di->batt_index, EN);
}

static void report_coul_drop_dsm(struct smartstar_coul_device *di)
{
	struct coul_drop_dsm coul_drop;
	struct rtc_time tm;
	short j;
	char dsm_buf[DSM_BUFF_SIZE_MAX] = { 0 };

	/* first report */
	for (j = 0 ; j < V_DROP_LEN; j++)
		coul_drop.vol_mv_drop[j] =
			di->coul_dev_ops->get_battery_vol_uv_drop_from_fifo(di->batt_index, j) / PERMILLAGE;
	for (j = 0 ; j < FIFO_LEN; j++)
		coul_drop.vol_mv_fifo[j] = 
			di->coul_dev_ops->get_battery_vol_uv_from_fifo(di->batt_index, j) / PERMILLAGE;
	for (j = 0 ; j < FIFO_LEN; j++)
		coul_drop.cur_ma_fifo[j] = 
			di->coul_dev_ops->get_battery_cur_ua_from_fifo(di->batt_index, j) / PERMILLAGE;

	coul_drop.charge_status = (di->charging_state == CHARGING_STATE_CHARGE_NOT_CHARGE) ? 0 : 1;
	coul_drop.cycle = (int)(di->batt_chargecycles / PERCENT);
	coul_drop.capfcc_mah = di->batt_fcc / PERMILLAGE;
	(void)strcpy_s(coul_drop.batvendor, BATT_NAME_SIZE_MAX, di->batt_data->batt_brand);
	coul_drop.soc = di->batt_soc;
	coul_drop.temp = di->batt_temp / TENTH;
	coul_drop.drop_thresthold = di->coul_dev_ops->get_coul_drop_th(di->batt_index);

	(void)memset_s(dsm_buf, DSM_BUFF_SIZE_MAX, 0, DSM_BUFF_SIZE_MAX);
	ktime_get_real_ts64(&coul_drop.ts);
	rtc_time_to_tm((unsigned long)(coul_drop.ts.tv_sec), &tm);

	(void)snprintf_s(dsm_buf, DSM_BUFF_SIZE_MAX, DSM_BUFF_SIZE_MAX - 1,
		"[coul_drop] index=%d, charge_status=%d, cycle=%d, capfcc=%d, batvender=%s, "
		"voltage_drop=[%d %d %d], "
		"voltage_fifo=[%d %d %d %d %d %d %d %d], "
		"current_drop=[%d %d %d %d %d %d %d %d], "
		"time_drop=%04d-%02d-%02d %02d:%02d:%02d:%03lu, "
		"soc=%d, temp=%d, cnt=0, drop_threshold=%d\n",
		di->batt_index, coul_drop.charge_status, coul_drop.cycle, coul_drop.capfcc_mah, coul_drop.batvendor,
		coul_drop.vol_mv_drop[0], coul_drop.vol_mv_drop[1], coul_drop.vol_mv_drop[2],
		coul_drop.vol_mv_fifo[0], coul_drop.vol_mv_fifo[1], coul_drop.vol_mv_fifo[2], coul_drop.vol_mv_fifo[3],
		coul_drop.vol_mv_fifo[4], coul_drop.vol_mv_fifo[5], coul_drop.vol_mv_fifo[6], coul_drop.vol_mv_fifo[7],
		coul_drop.cur_ma_fifo[0], coul_drop.cur_ma_fifo[1], coul_drop.cur_ma_fifo[2], coul_drop.cur_ma_fifo[3],
		coul_drop.cur_ma_fifo[4], coul_drop.cur_ma_fifo[5], coul_drop.cur_ma_fifo[6], coul_drop.cur_ma_fifo[7],
		tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
		tm.tm_hour, tm.tm_min, tm.tm_sec, coul_drop.ts.tv_nsec / NSEC_TO_MSEC,
		coul_drop.soc, coul_drop.temp, coul_drop.drop_thresthold);

#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
	power_dsm_report_dmd(POWER_DSM_BATTERY, DSM_BATTERY_COUL_DROP_INT, dsm_buf);
#endif
	queue_work(system_power_efficient_wq, &di->coul_drop_work);
}

/* handler the fault events from coul driver Parameters: work:fault workqueue */
void coul_core_fault_work(struct work_struct *work)
{
	struct smartstar_coul_device *di =
		container_of(work, struct smartstar_coul_device, fault_work);

	switch (di->coul_fault) {
	case COUL_FAULT_LOW_VOL:
		coul_core_err("%s:batt %d, low vol int\n", __func__, di->batt_index);
		di->coul_dev_ops->irq_disable(di->batt_index);
		coul_wake_lock(di);
		coul_low_vol_int_handle(di);
		coul_wake_unlock(di);
		di->coul_dev_ops->irq_enable(di->batt_index);
		di->coul_fault = COUL_FAULT_NON;
		break;
	case COUL_FAULT_CL_INT:
		coul_core_err("%s:batt %d, cc capability compare int\n", __func__, di->batt_index);
		di->coul_fault = COUL_FAULT_NON;
		break;
	case COUL_FAULT_CL_IN:
		coul_core_err("%s:batt %d, cc in over 81 int\n", __func__, di->batt_index);
		di->coul_fault = COUL_FAULT_NON;
		make_cc_no_overload(di);
		break;
	case COUL_FAULT_CL_OUT:
		coul_core_err("%s:batt %d, cc out over 81 int!\n", __func__, di->batt_index);
		di->coul_fault = COUL_FAULT_NON;
		make_cc_no_overload(di);
		break;
	case COUL_FAULT_VBAT_DROP:
		coul_core_err("%s:batt %d, vbat drop!\n", __func__, di->batt_index);
		di->coul_fault = COUL_FAULT_NON;
		coul_batt_notify(di, BATT_COUL_DROP);
		report_coul_drop_dsm(di);
		break;
	default:
		coul_core_err("%s:batt %d, default deal with!\n", __func__, di->batt_index);
		break;
	}
}
