/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: get dts info for coul core method
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
#include <securec.h>
#include <platform_include/basicplatform/linux/power/platform/coul/coul_private_interface.h>
#include <chipset_common/hwpower/common_module/power_dsm.h>
#include "coul_dsm.h"
#include "../coul_ic/pmic_hrst.h"
#include "coul_ocv_monitor.h"

void coul_dsm_info_init(struct smartstar_coul_device *di)
{
	if (!di)
		return;
	di->soc_change_check.check_flag = WAIT_SOC_CHANGE_CHECK;
	di->adjust_soc_check_flag = 1;
}

static void record_batt_dsm_info(struct smartstar_coul_device *di,
	struct coul_batt_dsm_info *batt_info)
{
	if (!batt_info)
		return;
	batt_info->batt_capacity = DIV_ROUND_CLOSEST(di->batt_capacity_1000x, PERMILLAGE);
	batt_info->batt_soc_real = di->batt_soc_real / TENTH;
	batt_info->batt_soc_est = di->batt_soc_est;
	batt_info->batt_ocv = di->batt_ocv / PERMILLAGE;
	batt_info->batt_ocv_level = di->last_ocv_level;
	batt_info->batt_uuc = di->batt_uuc / PERMILLAGE;
	batt_info->batt_drc = di->batt_delta_rc / PERMILLAGE;
	batt_info->batt_iavg = di->last_fifo_iavg_ma;
	batt_info->batt_vavg = di->last_fifo_vavg_mv;
	batt_info->batt_temp = di->batt_temp;
}

static int check_soc_change_fast(struct smartstar_coul_device *di, char *buf)
{
	int ibat_ua, vbat_uv;
	int delta_soc_real, delta_soc;
	struct soc_change_check_info *info;

	info = &di->soc_change_check;
	coul_core_get_battery_voltage_and_current(di, &ibat_ua, &vbat_uv);
	if ((ibat_ua < -CHARGING_CURRENT_OFFSET) || (di->batt_capacity_1000x > TENTH * PERMILLAGE)) {
		info->check_flag = WAIT_SOC_CHANGE_CHECK;
		return 0;
	}

	if ((di->batt_capacity_1000x == TENTH * PERMILLAGE) && !info->check_flag) {
		info->check_flag = SOC_CHANGE_CHECKING;
		record_batt_dsm_info(di, &info->start_info);
		coul_core_info("%s record info0\n", __func__);
		return 0;
	}

	if (di->batt_capacity_1000x == SOC_ONE * PERMILLAGE && info->check_flag) {
		info->check_flag = WAIT_SOC_CHANGE_CHECK;
		record_batt_dsm_info(di, &info->end_info);
		delta_soc_real = info->start_info.batt_soc_real - info->end_info.batt_soc_real;
		delta_soc = info->start_info.batt_capacity - info->end_info.batt_capacity;
		info->ratio = delta_soc_real * PERCENT / delta_soc;
		return 1;
	}

	return 0;
}

static void soc_change_fast_dump(struct smartstar_coul_device *di, char *buf)
{
	int ret;
	struct coul_batt_dsm_info *info0, *info1;

	info0 = &di->soc_change_check.start_info;
	info1 = &di->soc_change_check.end_info;
	ret = snprintf_s(buf, DSM_BUFF_SIZE_MAX, DSM_BUFF_SIZE_MAX - 1,
		"[soc_change_fast]batt%d, cycle = %d, fcc = %d, capacity0 = %d, soc_real0 = %d, ocv0 = %d, "
		"ocv_lv0 = %d, uuc0 = %dmAh, drc0 = %dmAh, iavg0 = %dmA, vavg0 = %dmV, temp0 = %d, "
		"capacity1 = %d, soc_real1 = %d, ocv1 = %d, ocv_lv1 = %d, uuc1 = %dmAh, "
		"drc1 = %dmAh, iavg1 = %dmA, vavg1 = %dmV, temp1 = %d, ratio = %d, ocv_interval = %d\n",
		di->batt_index, di->batt_chargecycles, di->fcc_real_mah, info0->batt_capacity, info0->batt_soc_real,
		info0->batt_ocv, info0->batt_ocv_level, info0->batt_uuc, info0->batt_drc, info0->batt_iavg,
		info0->batt_vavg, info0->batt_temp, info1->batt_capacity, info1->batt_soc_real,
		info1->batt_ocv, info1->batt_ocv_level, info1->batt_uuc, info1->batt_drc,
		info1->batt_iavg, info1->batt_vavg, info1->batt_temp,
		di->soc_change_check.ratio, di->get_cc_end_time);
	if (ret < 0)
		coul_core_err("%s snprintf_s failed\n", __func__);
}

static int record_coul_config_para(struct smartstar_coul_device *di, char *buff, int buff_size)
{
	int tmp_len, i, ret, fcc_sf_cols;

	if (!di || !di->batt_data || !di->batt_data->fcc_sf_lut) {
		coul_core_err("%s di is null\n", __func__);
		return -EINVAL;
	}
	fcc_sf_cols = di->batt_data->fcc_sf_lut->cols;
	tmp_len = snprintf_s(buff, buff_size, buff_size - 1,
		"drc_enable:%d, drc_limit_soc:%d, soc_at_term:%d, indentify_fcc:%d, "
		"uuc_zero_v:%d, nor_cutoff_v:%d, sleep_cutoff_v:%d, low_temp_cutoff_v:%d, low_vol_filter_cnt:%d, "
		"enable_basp_zero_v:%d, basp_zero_v:%d, soe_enable:%d,hold_full_soc_enable:%d, emer_uuc:%d, "
		"off_find_uuc:%d, safety_uuc:%d, nvme:%d, uuc_smooth_vol:%d, ",
		di->enable_charge_stop_limit_drc, di->charge_stop_limit_drc_soc, di->soc_at_term, di->batt_indentify_fcc,
		di->uuc_zero_vol, di->v_cutoff, di->v_cutoff_sleep, di->v_cutoff_low_temp, di->low_vol_filter_cnt,
		di->enable_basp_zero_ocv, di->basp_zero_ocv, di->soe_enable, di->hold_full_soc_enable,
		di->emer_uuc_uah / PERMILLAGE, di->off_find_uuc_uah / PERMILLAGE, di->safety_uuc_uah / PERMILLAGE,
		di->use_nvme_manager, di->uuc_smooth_vol_mv);
	if (tmp_len < 0)
		return tmp_len;
	/* record uuc_smooth_range */
	for (i = 0; i < UUC_SMOOTH_TEMP_LEVEL; i++) {
		ret = snprintf_s(buff + tmp_len, buff_size - tmp_len, buff_size - tmp_len - 1,
			"uuc_temp_min[%d]:%d, uuc_temp_max[%d]:%d, uuc_temp_soc[%d]:%d, ",
			i, di->uuc_smooth_para_range[i].temp_min,
			i, di->uuc_smooth_para_range[i].temp_max,
			i, di->uuc_smooth_para_range[i].temp_soc);
		if (ret < 0)
			return ret;
		tmp_len += ret;
	}
	/* record fcc_sf_y */
	for (i = 0; i < fcc_sf_cols; i++) {
		ret = snprintf_s(buff + tmp_len, buff_size - tmp_len, buff_size - tmp_len - 1,
			"fcc_sf[%d]:%d, ",
			di->batt_data->fcc_sf_lut->x[i], di->batt_data->fcc_sf_lut->y[i]);
		if (ret < 0)
			return ret;
		tmp_len += ret;
	}
	ret = snprintf_s(buff + tmp_len, buff_size - tmp_len, buff_size - tmp_len - 1, "\n");
	return (ret < 0) ? ret : tmp_len + ret;
}

void coul_para_report(struct smartstar_coul_device *di)
{
	int ret, i, tmp_len, volt, cur;
	short last_soc;
	char dsm_buf[DSM_BUFF_SIZE_MAX] = { 0 };

	if (!di) {
		coul_core_err("%s di is null\n", __func__);
		return;
	}

	coul_hardware_get_last_soc(&last_soc);
	coul_core_get_battery_voltage_and_current(di, &cur, &volt);
	di->coul_error_soc_type = ERROR_COUL_REPORT_PARA;
	/* record battery info and nv info */
	tmp_len = snprintf_s(dsm_buf, DSM_BUFF_SIZE_MAX, DSM_BUFF_SIZE_MAX - 1,
		"[coul_para_report]error_type = %d, batt%d, cyc:%u, v:%d, i:%d, temp:%d, ufsoc:%d, soc:%d, soe:%d, cap:%d, "
		"last_soc:%d, fcc:%d, last_fcc_cyc:%u, limitfcc:%d, rm:%d, uuc:%d, drc:%d, cc:%d, ctime:%u, ocv:%d, "
		"ocv_temp:%d, ocv_lv:%d, ocv_type:%d, ocv_flag:%d, v_a:%d, v_b:%d, c_a:%d, c_b:%d, r_pcb:%d, ",
		di->coul_error_soc_type, di->batt_index, di->batt_chargecycles / PERCENT, volt / PERMILLAGE,
		cur / PERMILLAGE, di->batt_temp, di->batt_soc_real, di->batt_soc, di->batt_soe,
		di->batt_capacity_1000x / PERMILLAGE, last_soc, di->batt_fcc / PERMILLAGE,
		(di->last_fcc_cycle & LAST_FCC_CYCLE_MASK), di->batt_limit_fcc / PERMILLAGE, di->batt_ruc / PERMILLAGE,
		di->batt_uuc / PERMILLAGE, di->batt_delta_rc / PERMILLAGE, di->cc_end_value / PERMILLAGE,
		di->get_cc_end_time, di->batt_ocv / PERMILLAGE, di->batt_ocv_temp, di->last_ocv_level,
		di->batt_ocv_dsm_type, di->batt_ocv_valid_to_refresh_fcc,
		di->g_coul_cali_params.v_offset_a, di->g_coul_cali_params.v_offset_b,
		di->g_coul_cali_params.c_offset_a, di->g_coul_cali_params.c_offset_b, di->r_pcb);
	if (tmp_len < 0) {
		coul_core_err("%s record batt info snprintf_s failed\n", __func__);
		return;
	}

	ret = record_coul_config_para(di, dsm_buf + tmp_len, DSM_BUFF_SIZE_MAX - tmp_len);
	if (ret < 0) {
		coul_core_err("%s record config para snprintf_s failed\n", __func__);
		return;
	}

	power_dsm_report_dmd(POWER_DSM_BATTERY, ERROR_BATT_SOC_PARAM, dsm_buf);
}

static bool is_unfilter_soc_valid(int soc)
{
	return (soc >= UNFILTER_SOC_MIN) && (soc <= UNFILTER_SOC_MAX);
}

static int check_unfiltered_soc(struct smartstar_coul_device *di, char *buf)
{
	int err_est;

	if (di->batt_soc_est == -EINVAL)
		err_est = 0;
	else
		err_est = abs(di->batt_soc_real / TENTH - di->batt_soc_est);

	if (!is_unfilter_soc_valid(di->batt_soc_real / TENTH) || err_est > SOC_EST_ERROR_MAX) {
		di->coul_error_soc_type = ERROR_COUL_UNFILTERED_SOC;
		coul_core_err("%s soc_real = %d, soc_est = %d, err_est = %d\n", __func__,
			di->batt_soc_real / TENTH, di->batt_soc_est, err_est);
		return 1;
	}

	return 0;
}

static int check_delta_rc_final(struct smartstar_coul_device *di, char *buf)
{
	int abnormal_delta_rc = di->batt_fcc * ABNORMAL_DRC_PERCENT / PERCENT;

	if (di->batt_delta_rc > abnormal_delta_rc) {
		di->coul_error_soc_type = ERROR_COUL_DELTA_RC;
		coul_core_err("%s batt_delta_rc = %d, abnormal_delta_rc = %d\n",
			__func__, di->batt_delta_rc, abnormal_delta_rc);
		return 1;
	}

	return 0;
}

static int check_unusable_capacity(struct smartstar_coul_device *di, char *buf)
{
	int abnormal_uuc = di->batt_fcc * ABNORMAL_UUC_PERCENT / PERCENT;

	if ((di->batt_temp / TENTH >= UUC_TEMP_THRESHOLD) &&
		(di->batt_uuc > abnormal_uuc)) {
		di->coul_error_soc_type = ERROR_COUL_UUC;
		coul_core_err("%s batt_temp = %d, abnormal_uuc == %d,  batt_uuc = %d\n",
			__func__, di->batt_temp, abnormal_uuc, di->batt_uuc);
		return 1;
	}
	return 0;
}

static int check_limit_fcc(struct smartstar_coul_device *di, char *buf)
{
	int limit_fcc_max, limit_fcc_min;

	if (di->batt_limit_fcc == 0)
		return 0;

	limit_fcc_max = di->batt_fcc * ABNORMAL_LIMIT_FCC_MAX / PERCENT;
	limit_fcc_min = di->batt_fcc * ABNORMAL_LIMIT_FCC_MIN / PERCENT;
	if ((di->batt_limit_fcc > limit_fcc_max) || (di->batt_limit_fcc < limit_fcc_min)) {
		di->coul_error_soc_type = ERROR_COUL_LIMIT_FCC;
		coul_core_err("%s abnormal limit_fcc = %d\n", __func__, di->batt_limit_fcc);
		return 1;
	}
	return 0;
}

static void coul_dsm_info_dump(struct smartstar_coul_device *di, char *buf)
{
	int i;
	int tmp_len = 0;

	tmp_len += snprintf_s(buf, DSM_BUFF_SIZE_MAX, DSM_BUFF_SIZE_MAX- 1,
		"[coul_dsm_info]error_type = %d, battName:%s, batt%d, cycle = %d, fcc = %d, "
		"soc = %d, soc_real = %d, soc_est = %d, ocv = %d, ocv_lv = %d, uuc = %dmAh, "
		"drc = %dmAh, Iavg = %dmA, Vavg = %dmV, limit_fcc = %d, ocv_interval = %d\n",
		di->coul_error_soc_type, di->batt_data->batt_brand, di->batt_index,
		di->batt_chargecycles / PERCENT, di->batt_fcc / PERMILLAGE, di->batt_soc,
		di->batt_soc_real / TENTH, di->batt_soc_est, di->batt_ocv, di->last_ocv_level,
		di->batt_uuc / PERMILLAGE, di->batt_delta_rc / PERMILLAGE, di->last_fifo_iavg_ma,
		di->last_fifo_vavg_mv, di->batt_limit_fcc / PERMILLAGE, di->get_cc_end_time);

	for (i = 0; i < INDEX_MAX; i++) {
		tmp_len += snprintf_s(buf + tmp_len, DSM_BUFF_SIZE_MAX - tmp_len,
			DSM_BUFF_SIZE_MAX - tmp_len - 1, "[OCV calibration]%s ",
			di->g_coul_ocv_cali_info[i].cali_timestamp);
		tmp_len += snprintf_s(buf + tmp_len, DSM_BUFF_SIZE_MAX - tmp_len,
			DSM_BUFF_SIZE_MAX - tmp_len - 1, "OCV:%duV ",
			di->g_coul_ocv_cali_info[i].cali_ocv_uv);
		tmp_len += snprintf_s(buf + tmp_len, DSM_BUFF_SIZE_MAX - tmp_len,
			DSM_BUFF_SIZE_MAX - tmp_len - 1, "temp:%d ",
			di->g_coul_ocv_cali_info[i].cali_ocv_temp);
		tmp_len += snprintf_s(buf + tmp_len, DSM_BUFF_SIZE_MAX - tmp_len,
			DSM_BUFF_SIZE_MAX - tmp_len - 1, "CC:%duAh ",
			(int)(di->g_coul_ocv_cali_info[i].cali_cc_uah));
		tmp_len += snprintf_s(buf + tmp_len, DSM_BUFF_SIZE_MAX - tmp_len,
			DSM_BUFF_SIZE_MAX - tmp_len - 1, "rbatt:%d ",
			di->g_coul_ocv_cali_info[i].cali_rbatt);
		tmp_len += snprintf_s(buf + tmp_len, DSM_BUFF_SIZE_MAX - tmp_len,
			DSM_BUFF_SIZE_MAX - tmp_len - 1, "ocv_cali_level:%u\n",
			di->g_coul_ocv_cali_info[i].cali_ocv_level);
	}
}

struct coul_dsm_check dsm_check_array[] = {
	{ERROR_BATT_SOC_DROP_FAST, true,
		.dump = soc_change_fast_dump, check_soc_change_fast},
	{ERROR_BATT_SOC_PARAM, true,
		.dump = coul_dsm_info_dump, check_unfiltered_soc},
	{ERROR_BATT_SOC_PARAM, true,
		.dump = coul_dsm_info_dump, check_delta_rc_final},
	{ERROR_BATT_SOC_PARAM, true,
		.dump = coul_dsm_info_dump, check_unusable_capacity},
	{ERROR_BATT_SOC_PARAM, true,
		.dump = coul_dsm_info_dump, check_limit_fcc},
};

static int check_dsm_error_info(struct smartstar_coul_device *di,
	char *buf, int i)
{
	unsigned int err;

	if (di && dsm_check_array[i].check_error(di, buf)) {
		err = (unsigned int)dsm_check_array[i].error_no;
		buf[DSM_BUFF_SIZE_MAX - 1] = 0;
		if (dsm_check_array[i].dump)
			dsm_check_array[i].dump(di, buf);
		power_dsm_report_dmd(POWER_DSM_BATTERY, err, buf);
		dsm_check_array[i].notify_enable = false;
		coul_core_err("%s err_no=%d\n", __func__, err);
		return 1;
	}
	return 0;
}

void coul_algo_dsm_check(struct smartstar_coul_device *di)
{
	int i;
	char *buf = NULL;

	if (!di)
		return;
	buf = (char *)kzalloc(DSM_BUFF_SIZE_MAX, GFP_KERNEL);
	if (!buf) {
		coul_core_err("%s call kzalloc failed\n", __func__);
		return;
	}

	for (i = 0; i < (int)(sizeof(dsm_check_array) / sizeof(struct coul_dsm_check));
		++i) {
		if (dsm_check_array[i].notify_enable) {
			if (check_dsm_error_info(di, buf, i))
				break;
		}
	}

	kfree(buf);
}

void coul_algo_dsm_flag_enable(void)
{
	int i;

	for (i = 0; i < (int)(sizeof(dsm_check_array) / sizeof(struct coul_dsm_check));
		++i) {
		dsm_check_array[i].notify_enable = true;
	}
}

void coul_report_ocv_cali_info(struct smartstar_coul_device *di)
{
	int ret, ocv_cali_index, ocv_time, pc, ruc_now_uah;
	struct coul_ocv_cali_info *info;
	char dsm_buf[DSM_BUFF_SIZE_MAX] = { 0 };
	int delta_rm_uah = 0;

	coul_core_info("%s ocv cali dmd report begin\n", __func__);
	if (!di || (di->coul_dev_ops == NULL) ||
		(di->coul_dev_ops->get_coul_time == NULL))
		return;

	ocv_time = (int)di->coul_dev_ops->get_coul_time(di->batt_index);
	if (di->batt_ocv_dsm_type == POWER_ON_SAVED_OCV) {
		coul_core_err("%s ocv type is POWER_ON_SAVED_OCV", __func__);
		return;
	}
	if (((di->batt_ocv_dsm_type == NORMAL_SR_OCV) ||
		(di->batt_ocv_dsm_type == CHARGE_DONE_RESUME_OCV)) &&
		(ocv_time < SR_OCV_REPORT_TIME_SEC)) {
		coul_core_err("%s sr ocv report time is too short, time = %d\n",
			__func__, ocv_time);
		return;
	}

	if ((di->batt_ocv_dsm_type != POWER_ON_CALC_OCV) && (di->batt_ocv_dsm_type != POWER_ON_PMU_OCV)) {
		pc = coul_ocv_cal_tbl_pc(di->batt_index, di->batt_ocv, di->batt_ocv_type, di->batt_ocv_temp);
		ruc_now_uah = (di->batt_fcc / PERMILLAGE) * pc;
		delta_rm_uah = di->batt_ruc - ruc_now_uah;
	}

	ocv_cali_index = (di->g_ocv_cali_index >= INDEX_MAX) ? INDEX_MAX - 1 : di->g_ocv_cali_index;
	info = &di->g_coul_ocv_cali_info[ocv_cali_index];
	ret = snprintf_s(dsm_buf, DSM_BUFF_SIZE_MAX, DSM_BUFF_SIZE_MAX - 1,
		"[ocv_cali_info] batt%d, %s, ocv = %duV, temp = %d, cc = %duAh, rbatt = %d, "
		"ocv_cali_level = %d, ocv_type = %d, ocv_interval = %d, delta_rm_uah = %d, cc_mA_tenth = %d\n",
		di->batt_index, info->cali_timestamp, info->cali_ocv_uv, info->cali_ocv_temp,
		info->cali_cc_uah, info->cali_rbatt, info->cali_ocv_level, info->cali_ocv_type,
		di->get_cc_end_time, delta_rm_uah, abs(delta_rm_uah) / PERCENT * SEC_PER_HOUR / di->get_cc_end_time);
	if (ret < 0) {
		coul_core_err("%s snprintf_s failed\n", __func__);
		return;
	}

	power_dsm_report_dmd(POWER_DSM_BATTERY, DSM_BATT_COUL_OCV_CALI, dsm_buf);
	coul_core_info("%s ocv cali dmd report success\n", __func__);
}

void coul_report_adjust_soc_fast_info(struct smartstar_coul_device *di,
	int soc_est, int soc)
{
	char dsm_buf[DSM_BUFF_SIZE_MAX] = { 0 };

	if (!di)
		return;
	if ((di->adjust_soc_check_flag && (soc_est == 0)) &&
			(soc - soc_est >= ADJUST_SOC_FAST_THRESHOLD)) {
		di->coul_error_soc_type = ERROR_COUL_ADJUST_SOC_FAST;
		di->adjust_soc_check_flag = 0;
		coul_dsm_info_dump(di, dsm_buf);
		power_dsm_report_dmd(POWER_DSM_BATTERY, ERROR_BATT_SOC_PARAM, dsm_buf);
	}
	coul_core_info("%s soc = %d, soc_est = %d\n", __func__, soc, soc_est);
}