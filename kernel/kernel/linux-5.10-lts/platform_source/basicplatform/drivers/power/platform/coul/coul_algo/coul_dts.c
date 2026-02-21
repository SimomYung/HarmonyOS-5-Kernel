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

#include <linux/of.h>
#include <securec.h>
#include <platform_include/basicplatform/linux/power/platform/coul/coul_private_interface.h>
#include "coul_nv.h"
#include "coul_temp.h"
#include "coul_dts.h"

#define ZERO_V_MAX      3200
#define ZERO_V_MIN      2800
#define DEFAULT_HKADC_BATT_TEMP         10
#define DEFAULT_HKADC_BATT_ID           11
#define DEFAULT_SOC_MONITOR_LIMIT       100
#define LOW_INT_VOL_COUNT       3
#define BASE_OF_SIMPLE_STRTOL   10
#define DEFAULT_DISCHG_OCV_SOC  5
#define DEFAULT_SOC_AT_TERM     100
#define WAKELOCK_LOW_BATT_SOC   3

/* get dts info */
static void get_current_full_enable(struct smartstar_coul_device *di,
	const struct device_node *np)
{
	int ret;
	unsigned int enable_current_full = 0;

	ret = of_property_read_u32(np, "current_full_enable",
		&enable_current_full);
	if (ret)
		coul_core_err("dts: can not get current_full_enable, use default : %u\n",
			enable_current_full);
	di->enable_current_full = enable_current_full;
	coul_core_info("dts:get enable_current_full = %u!\n",
		       di->enable_current_full);
}

static void get_cutoff_vol_mv(struct smartstar_coul_device *di,
	const struct device_node *np)
{
	int ret;

	ret = of_property_read_u32(np, "normal_cutoff_vol_mv", &di->v_cutoff);
	if (ret)
		di->v_cutoff = BATTERY_NORMAL_CUTOFF_VOL;
	ret = of_property_read_u32(np, "sleep_cutoff_vol_mv",
		&di->v_cutoff_sleep);
	if (ret)
		di->v_cutoff_sleep = BATTERY_VOL_2_PERCENT;
	ret = of_property_read_u32(np, "low_temp_cutoff_vol_mv",
		(unsigned int *)&di->v_cutoff_low_temp);
	if (ret)
		di->v_cutoff_low_temp = BATTERY_NORMAL_CUTOFF_VOL;
	coul_core_err("get_cutoff_vol_mv: cutoff = %u, cutoff_sleep = %u, low_temp_vol = %d\n",
		di->v_cutoff, di->v_cutoff_sleep, di->v_cutoff_low_temp);
}

static void get_dischg_ocv_soc(struct smartstar_coul_device *di,
	const struct device_node *np)
{
	int ret;

	ret = of_property_read_u32(np, "dischg_ocv_soc",
		(unsigned int *)&di->dischg_ocv_soc);
	if (ret)
		/* if not configured, default value is 5 percent */
		di->dischg_ocv_soc = DEFAULT_DISCHG_OCV_SOC;
	coul_core_info("dischg_ocv_soc: flag is %d\n", di->dischg_ocv_soc);
}

static void get_uuc_vol_dts(struct smartstar_coul_device *di,
	const struct device_node *np)
{
	int ret;

	ret = of_property_read_u32(np, "uuc_zero_vol", (u32 *)&di->uuc_zero_vol);
	if (ret)
		di->uuc_zero_vol = ZERO_V_MAX;

	if (di->uuc_zero_vol < ZERO_V_MIN)
		di->uuc_zero_vol = ZERO_V_MIN;

	coul_core_info("uuc_zero_vol:%d\n", di->uuc_zero_vol);
}

static void get_bci_dts_info(struct smartstar_coul_device *di)
{
	int ret;
	struct device_node *np = NULL;

	np = of_find_compatible_node(NULL, NULL, "hisilicon,bci_battery");
	if (np == NULL) {
		di->is_board_type = BAT_BOARD_SFT;
		return;
	}
	ret = of_property_read_u32(np, "battery_board_type", &di->is_board_type);
	if (ret) {
		di->is_board_type = BAT_BOARD_SFT;
		coul_core_err("dts:get board type fail\n");
	}
}

static void get_r_pcb_dts(struct smartstar_coul_device *di,
	const struct device_node *np)
{
	int ret;
	unsigned int r_pcb = DEFAULT_RPCB;

	ret = of_property_read_u32(np, "r_pcb", &r_pcb);
	if (ret)
		coul_core_err("error:get r_pcb value failed\n");
	di->r_pcb = (int)r_pcb;
	coul_core_info("dts:get r_pcb = %d uohm\n", di->r_pcb);
}

static void get_adc_info_dts(struct smartstar_coul_device *di,
	const struct device_node *np)
{
	int ret;

	ret = of_property_read_u32(np, "adc_batt_id", &di->adc_batt_id);
	if (ret) {
		di->adc_batt_id = DEFAULT_HKADC_BATT_ID;
		coul_core_err("dts:can not get batt id adc channel,use default channel: %u\n",
			di->adc_batt_id);
	}
	coul_core_info("dts:get batt id adc channel = %u\n", di->adc_batt_id);
}

static void get_soc_dts_info(struct smartstar_coul_device *di,
	const struct device_node *np)
{
	int ret;
	unsigned int soc_at_term = DEFAULT_SOC_AT_TERM;
	unsigned int soc_monitor_limit = DEFAULT_SOC_MONITOR_LIMIT;

	ret = of_property_read_u32(np, "soc_at_term", &soc_at_term);
	if (ret)
		coul_core_err("dts:can not get soc_at_term,use default : %u\n",
			soc_at_term);
	di->soc_at_term = (int)soc_at_term;
	coul_core_info("dts:get soc_at_term = %u\n", soc_at_term);

	ret = of_property_read_u32(np, "soc_monitor_limit", &soc_monitor_limit);
	if (ret)
		coul_core_err("dts:get soc_monitor_limit fail, use default limit value!\n");
	di->soc_monitor_limit = (int)soc_monitor_limit;
	coul_core_info("soc_monitor_limit = %d\n", di->soc_monitor_limit);
}

static void get_ntc_paras(struct smartstar_coul_device *di,
	const struct device_node *np)
{
	int i, ret, array_len, idata;
	const char *compensation_data_string = NULL;

	/* reset to 0 */
	memset(di->ntc_temp_compensation_para, 0,
		sizeof(di->ntc_temp_compensation_para));
	array_len = of_property_count_strings(np, "ntc_temp_compensation_para");
	if ((array_len <= 0) || (array_len % NTC_COMPENSATION_PARA_TOTAL != 0)) {
		coul_core_err("ntc_temp_compensation_para is invaild, please check ntc_temp_compensation_para number\n");
		return;
	}
	if (array_len > COMPENSATION_PARA_LEVEL * NTC_COMPENSATION_PARA_TOTAL) {
		array_len =
			COMPENSATION_PARA_LEVEL * NTC_COMPENSATION_PARA_TOTAL;
		coul_core_err("ntc_temp_compensation_para is too long, use only front %d paras\n",
			array_len);
		return;
	}

	for (i = 0; i < array_len; i++) {
		ret = of_property_read_string_index(np, "ntc_temp_compensation_para",
			i, &compensation_data_string);
		if (ret) {
			coul_core_err("get ntc_temp_compensation_para failed\n");
			return;
		}

		idata = 0;
		ret = kstrtoint(compensation_data_string,
			BASE_OF_SIMPLE_STRTOL, &idata);
		if (ret < 0)
			coul_core_err("compensation_data_string failed\n");
		switch (i % NTC_COMPENSATION_PARA_TOTAL) {
		case NTC_COMPENSATION_PARA_ICHG:
			di->ntc_temp_compensation_para[
				i / NTC_COMPENSATION_PARA_TOTAL].
					ntc_compensation_ichg = idata;
			break;
		case NTC_COMPENSATION_PARA_VALUE:
			di->ntc_temp_compensation_para[
				i / NTC_COMPENSATION_PARA_TOTAL].
					ntc_compensation_value = idata;
			break;
		default:
			coul_core_err("ntc_temp_compensation_para get failed\n");
		}
		coul_core_info("di->ntc_temp_compensation_para[%d][%d] = %d\n",
			i / (NTC_COMPENSATION_PARA_TOTAL),
			i % (NTC_COMPENSATION_PARA_TOTAL), idata);
	}
}

static void get_ntc_temp_compensation_para(
	struct smartstar_coul_device *di, const struct device_node *np)
{
	unsigned int ntc_compensation_is = 0;
	unsigned int external_compensation_is = 0;
	int ret;

	ret = of_property_read_u32(np, "external_compensation_is",
		&external_compensation_is);
	if (ret) {
		coul_core_info("get external_compensation_is default 0\n");
		external_compensation_is = 0;
	}
	di->external_compensation_is = (int)external_compensation_is;
	coul_core_info("external_compensation_is %d\n", external_compensation_is);

	ret = of_property_read_u32(np, "ntc_compensation_is",
		&ntc_compensation_is);
	if (ret) {
		di->ntc_compensation_is = 0;
		coul_core_info("get ntc_compensation_is failed\n");
	} else {
		di->ntc_compensation_is = (int)ntc_compensation_is;
		coul_core_info("ntc_compensation_is = %d\n",
			di->ntc_compensation_is);
		get_ntc_paras(di, np);
	}
}

static void get_low_vol_filter_cnt(struct smartstar_coul_device *di,
	const struct device_node *np)
{
	int ret;
	unsigned int low_vol_filter_cnt = LOW_INT_VOL_COUNT;

	ret = of_property_read_u32(np, "low_vol_filter_cnt",
		&low_vol_filter_cnt);
	if (ret)
		coul_core_err("dts:get low_vol_filter_cnt fail, use default limit value\n");
	di->low_vol_filter_cnt = low_vol_filter_cnt;
	coul_core_info("low_vol_filter_cnt = %u\n", di->low_vol_filter_cnt);
}

static void get_dts_temp_shielded(struct smartstar_coul_device *di,
	const struct device_node *np)
{
	int ret;

	ret = of_property_read_u32(np, "temp_shielded", &di->temp_shielded);
	if (ret) {
		di->temp_shielded = 0;
		coul_core_err("use default value 0\n");
	}
	coul_core_info("dts:get dts_temp_shielded=%d\n", di->temp_shielded);
}

static void get_wakelock_low_batt_soc(struct smartstar_coul_device *di,
	const struct device_node *np)
{
	int ret;

	ret = of_property_read_u32(np, "wakelock_low_batt_soc",
		&di->wakelock_low_batt_soc);
	if (ret) {
		di->wakelock_low_batt_soc = WAKELOCK_LOW_BATT_SOC;
		coul_core_err("dts: use default wakelock_low_batt_soc value\n");
	}

	if (di->wakelock_low_batt_soc > WAKELOCK_LOW_BATT_SOC)
		di->wakelock_low_batt_soc = WAKELOCK_LOW_BATT_SOC;

	coul_core_info("wakelock_low_batt_soc = %u\n", di->wakelock_low_batt_soc);
}

static void get_batt_index(struct smartstar_coul_device *di,
	const struct device_node *np)
{
	const char *batt_name_str = NULL;
	int ret;

	ret = of_property_read_u32(np, "batt_index", (unsigned int *)&di->batt_index);
	if (ret)
		coul_core_err("%s batt_index error %d\n", __func__, ret);

	ret = of_property_read_string(np, "batt_name", &batt_name_str);
	if (ret)
		(void)strcpy_s(di->batt_name, BATT_NAME_SIZE_MAX, "unknow");
	else
		(void)strcpy_s(di->batt_name, BATT_NAME_SIZE_MAX, batt_name_str);

	coul_core_info("%s batt_index %d, batt_name %s\n", __func__, di->batt_index, di->batt_name);
}

static void get_batt_indentify_fcc(struct smartstar_coul_device *di,
	const struct device_node *np)
{
	int ret;

	ret = of_property_read_u32(np, "batt_indentify_fcc", &di->batt_indentify_fcc);
	if (ret) {
		coul_core_err("%s batt_indentify_fcc not set\n", __func__);
		di->batt_indentify_fcc = 0;
		return ;
	}

	coul_core_info("%s batt_indentify_fcc %d\n", __func__, di->batt_indentify_fcc);
}

static void get_uuc_dts_info(struct smartstar_coul_device *di,
	const struct device_node *np)
{
	int ret;

	ret = of_property_read_u32(np, "emer_uuc_uah", &di->emer_uuc_uah);
	if (ret)
		di->emer_uuc_uah = 0;

	ret = of_property_read_u32(np, "off_find_uuc_uah", &di->off_find_uuc_uah);
	if (ret)
		di->off_find_uuc_uah = 0;

	ret = of_property_read_u32(np, "safety_uuc_uah", &di->safety_uuc_uah);
	if (ret)
		di->safety_uuc_uah = 0;

	/* whether to raise to voltage from basp zero ocv */
	ret = of_property_read_u32(np, "enable_basp_zero_ocv", &di->enable_basp_zero_ocv);
	if (ret)
		di->enable_basp_zero_ocv = 0;

	coul_core_info("%s emer_uuc_uah=%d, off_find_uuc_uah=%d, di->safety_uuc_uah=%d, enable_basp_zero_ocv=%d\n",
		__func__, di->emer_uuc_uah, di->off_find_uuc_uah, di->safety_uuc_uah, di->enable_basp_zero_ocv);
}

static void get_batt_nvme_manager_flag(struct smartstar_coul_device *di,
	const struct device_node *np)
{
	int ret;

	ret = of_property_read_u32(np, "use_nvme_manager", &di->use_nvme_manager);
	if (ret) {
		coul_core_err("%s use_nvme_manager not set\n", __func__);
		di->use_nvme_manager = 0;
	}

	coul_core_info("%s use_nvme_manager %d\n", __func__, di->use_nvme_manager);
}

static void get_limit_drc_dts_info(struct smartstar_coul_device *di,
	const struct device_node *np)
{
	int ret;

	ret = of_property_read_u32(np, "enable_charge_stop_limit_drc", &di->enable_charge_stop_limit_drc);
	if (ret)
		di->enable_charge_stop_limit_drc = 0;

	ret = of_property_read_u32(np, "charge_stop_limit_drc_soc", &di->charge_stop_limit_drc_soc);
	if (ret)
		di->charge_stop_limit_drc_soc = LIMIT_DRC_SOC;

	coul_core_info("%s enable_charge_stop_limit_drc %d, charge_stop_limit_drc_soc %d\n",
		__func__, di->enable_charge_stop_limit_drc, di->charge_stop_limit_drc_soc);
}

static void get_batt_aging_factor_info(struct smartstar_coul_device *di,
	const struct device_node *np)
{
	int ret;

	ret = of_property_read_u32(np, "enable_aging_factor", &di->enable_aging_factor);
	if (ret)
		di->enable_aging_factor = 0;

	coul_core_info("%s enable_aging_factor %d\n", __func__, di->enable_aging_factor);
}

static int get_uuc_smooth_paras_range(struct smartstar_coul_device *di, const struct device_node *np)
{
	int i, ret, array_len, idata;
	const char *uuc_smooth_temp_data_string = NULL;
	errno_t err;
 
	/* reset to 0 */
	err = memset_s(di->uuc_smooth_para_range, sizeof(di->uuc_smooth_para_range), 0,
		sizeof(di->uuc_smooth_para_range));
	if (err != EOK) {
		coul_core_err("%s menset fail!\n", __func__);
		return -1;
	}
	array_len = of_property_count_strings(np, "uuc_smooth_temp");
	if ((array_len <= 0) || (array_len % TEMPERATURE_PARA_TOTAL != 0) ||
		(array_len > UUC_SMOOTH_TEMP_LEVEL * TEMPERATURE_PARA_TOTAL)) {
		coul_core_err("uuc_smooth_temp is invaild, please check uuc_smooth_temp number\n");
		return -1;
	}
 
	for (i = 0; i < array_len; i++) {
		ret = of_property_read_string_index(np, "uuc_smooth_temp", i, &uuc_smooth_temp_data_string);
		if (ret) {
			coul_core_err("get uuc_smooth_temp failed\n");
			return -1;
		}
 
		idata = 0;
		ret = kstrtoint(uuc_smooth_temp_data_string, BASE_OF_SIMPLE_STRTOL, &idata);
		if (ret < 0) {
			coul_core_err("conver uuc_smooth_temp_data_string failed\n");
			return -1;
		}
	
		switch (i % TEMPERATURE_PARA_TOTAL) {
			case TEMPERATURE_PARA_MIN:
				di->uuc_smooth_para_range[i / TEMPERATURE_PARA_TOTAL].temp_min = idata;
				break;
			case TEMPERATURE_PARA_MAX:
				di->uuc_smooth_para_range[i / TEMPERATURE_PARA_TOTAL].temp_max = idata;
				break;
			case TEMPERATURE_PARA_SOC:
				di->uuc_smooth_para_range[i / TEMPERATURE_PARA_TOTAL].temp_soc = idata;
				break;
			default:
				coul_core_err("uuc_smooth_temp assign value failed\n");
		}
	
		coul_core_info("di->uuc_smooth_temp[%d][%d][%d] = %d\n",
			i / (TEMPERATURE_PARA_TOTAL), i % (TEMPERATURE_PARA_TOTAL), i % (TEMPERATURE_PARA_TOTAL), idata);
		}
 
	return 0;
}
 
static void get_uuc_smooth_para(struct smartstar_coul_device *di, const struct device_node *np)
{
	int ret;
	ret = get_uuc_smooth_paras_range(di, np);
	ret += of_property_read_u32(np, "uuc_smooth_vol_mv", &di->uuc_smooth_vol_mv);
	if (ret) {
		coul_core_err("%s get_uuc_smooth_para default 0!\n", __func__);
		di->uuc_smooth_vol_mv = 0;
		if (memset_s(di->uuc_smooth_para_range, sizeof(di->uuc_smooth_para_range), 0,
			sizeof(di->uuc_smooth_para_range)) != EOK)
			coul_core_err("get_uuc_smooth_para error!\n");
	} else {
		coul_core_info("%s uuc_smooth_vol_mv=%d\n", __func__, di->uuc_smooth_vol_mv);
	}
}
 
void coul_core_get_battery_dts(struct smartstar_coul_device *di, const struct device_node *np)
{
	get_batt_index(di, np);
	get_batt_indentify_fcc(di, np);
	get_cutoff_vol_mv(di, np);
	get_current_full_enable(di, np);
	get_dischg_ocv_soc(di, np);
	get_uuc_vol_dts(di, np);
	get_dts_temp_shielded(di, np);
	get_bci_dts_info(di);
	get_r_pcb_dts(di, np);
	get_adc_info_dts(di, np);
	get_soc_dts_info(di, np);
	get_ntc_temp_compensation_para(di, np);
	get_low_vol_filter_cnt(di, np);
	get_wakelock_low_batt_soc(di, np);
	get_uuc_dts_info(di, np);
	get_batt_nvme_manager_flag(di, np);
	get_limit_drc_dts_info(di, np);
	get_batt_aging_factor_info(di, np);
	get_uuc_smooth_para(di, np);
}