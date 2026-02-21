/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: ocv select and table monitor functions for coul module.
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
#include "coul_ocv_monitor.h"
#include <platform_include/see/ffa/ffa_plat_drv.h>
#include <platform_include/basicplatform/linux/power/platform/coul/coul_private_interface.h>
#include <coul_ocv_manager.h>

#define coul_debug(fmt, args...) pr_debug("[coul_ocv]" fmt, ## args)
#define coul_info(fmt, args...) pr_info("[coul_ocv]" fmt, ## args)
#define coul_warn(fmt, args...) pr_warn("[coul_ocv]" fmt, ## args)
#define coul_err(fmt, args...) pr_err("[coul_ocv]" fmt, ## args)

#define CAPACITY_DENSE_AREA_3200        3200000
#define CAPACITY_DENSE_AREA_3670        3670000
#define CAPACITY_DENSE_AREA_3700        3700000
#define CAPACITY_DENSE_AREA_3720        3720000
#define CAPACITY_DENSE_AREA_3810        3810000
#define CAPACITY_DENSE_AREA_3900        3900000
#define CAPACITY_DENSE_AREA_4000        4000000
#define DEFAULT_CHG_HIGHCAP             60
#define DEFAULT_DCHG_HIGHCAP_THRED      3
#define DEFAULT_CHG_THRED               9
#define DEFAULT_DCHG_UP_THRED           20
#define DEFAULT_DCHG_DOWN_THRED         5
#define DEFAULT_DISCHG_OCV_SOC          5

#define FID_POWER_COUL_OCD_SYNC_SEND        0xc3004A01u

static struct ocv_chg_data *g_ocv_chg_data[BATT_MAX];

#ifdef CONFIG_PLATDRV_SUPPORT_FFA
static int tzsp_process(struct coul_shmem_to_tzsp *tzsp_shmem, unsigned int cmd)
{
	int ret;
	struct ffa_send_direct_data args = {
		.data0 = FID_POWER_COUL_OCD_SYNC_SEND,
		.data1 = (unsigned long)cmd,
		.data2 = 0,
		.data3 = 0,
		.data4 = sizeof(*tzsp_shmem),
	};

	if (tzsp_shmem == NULL) {
		coul_err("NULL point in [%s]\n", __func__);
		return -1;
	}

	ret = ffa_platdrv_send_msg_with_shmem(&args, (void *)tzsp_shmem, SHMEM_INOUT);
	if (ret || args.data2 != POWER_SVC_SUCC || args.data4 != sizeof(*tzsp_shmem)) {
		coul_err("%s, ret %d\n", __func__, ret);
		return -1;
	}

	return ret;
}
#else
static int tzsp_process(struct coul_shmem_to_tzsp *tzsp_shmem, unsigned int cmd)
{
	return -1;
}
#endif

/**
 * coul_ocv_check_dense_area
 *
 * Return: TRUE - not in dense area, can update
 *         FALSE - in dense area, can not update
 */
bool coul_ocv_check_dense_area(int batt_index, int ocv_uv)
{
	int i;
	struct cap_dense_area *areas = NULL;

	if (batt_index < 0 || batt_index >= BATT_MAX || g_ocv_chg_data[batt_index] == NULL)
		return FALSE;

	areas = g_ocv_chg_data[batt_index]->batt_data->cap_dense_area;

	/* whtether to use cap dense area data from batt raw data excel */
	if (areas && areas->rows > 0) {
		for (i = 0; i < areas->rows; i++) {
			if ((ocv_uv > areas->start[i] * PERMILLAGE) &&
				(ocv_uv < areas->end[i] * PERMILLAGE))
				return TRUE;
		}
	} else {
		if (ocv_uv < CAPACITY_DENSE_AREA_3200 ||
			(ocv_uv > CAPACITY_DENSE_AREA_3670 &&
			ocv_uv < CAPACITY_DENSE_AREA_3700) ||
			(ocv_uv > CAPACITY_DENSE_AREA_3720 &&
			ocv_uv < CAPACITY_DENSE_AREA_3810))
			return TRUE;
	}
	return FALSE;
}

static enum ocv_table_type select_ocv_type(int batt_index, int soc)
{
	int ret;
	enum ocv_table_type ocv_type;
	struct coul_shmem_to_tzsp tzsp_shmem;
	struct ocv_chg_data *ocd = NULL;

	ocd = g_ocv_chg_data[batt_index];
	memcpy_s(&tzsp_shmem.ocd, sizeof(tzsp_shmem.ocd), ocd, sizeof(*ocd));
	tzsp_shmem.ocd.batt_data = NULL;
	tzsp_shmem.soc = soc;

	ret = tzsp_process(&tzsp_shmem, COUL_SEL_OCV_TYPE);
	if (ret) {
		coul_err("%s: tzsp process error %d\n", __func__, ret);
		return OCV_MIX;
	}

	ocv_type = tzsp_shmem.result;
	return ocv_type;
}

/**
 * 
 * On the premise of sub_dense_area voltage, judge whether battery in discharge state.
 *
 * Return: TRUE: discharge state, can update dischg ocv
 *         FALSE: not discharge state, can not update
 */
bool coul_ocv_is_dischg_state(int batt_index, int soc_10x)
{
	struct ocv_chg_data *ocd = NULL;

	if (batt_index < 0 || batt_index >= BATT_MAX || g_ocv_chg_data[batt_index] == NULL)
		return FALSE;

	ocd = g_ocv_chg_data[batt_index];

	if (ocd->chg_stop_soc - soc_10x / TENTH > ocd->dischg_ocv_soc)
		return TRUE;
	return FALSE;
}

/**
 * select ocv type, or whether this ocv can be update.
 * this function can only be called during discharge or change-done.
 *
 * @ocv_uv: the new ocv
 * @soc_10x: current soc
 * Return: ocv_table_type, OCV_MIX: do not update ocv.
 */
enum ocv_table_type coul_ocv_select_type(int batt_index, int ocv_uv, int soc_10x)
{
	struct ocv_chg_data *ocd = NULL;

	if (batt_index < 0 || batt_index >= BATT_MAX || g_ocv_chg_data[batt_index] == NULL)
		return OCV_MIX;

	ocd = g_ocv_chg_data[batt_index];
	coul_core_info("%s batt_index=%d, ocv_uv=%d, soc_10x=%d, start_soc=%d, stop_soc=%d\n",
		__func__, batt_index, ocv_uv, soc_10x, ocd->chg_start_soc, ocd->chg_stop_soc);
	if (coul_ocv_check_dense_area(batt_index, ocv_uv))
		return OCV_MIX;

	if (ocd->enable) {
		return select_ocv_type(batt_index, soc_10x / TENTH);
	} else {
		if (ocv_uv < CAPACITY_DENSE_AREA_4000) {
			if (coul_ocv_is_dischg_state(batt_index, soc_10x))
				return OCV_DISCHG;
			else
				return OCV_MIX;
		}
		return OCV_DISCHG;
	}
}

/**
 * look up pc_temp_ocv table，ocv->pc.
 *
 * @ocv_uv: input ocv
 * @ocv_type: @ocv_uv's ocv type,
 *            if @ocv_uv is not the last updated ocv, should be ocv_table_type::OCV_DCHG.
 * Return: percent of soc, 0.1 percent.
 */
int coul_ocv_cal_tbl_pc(int batt_index, int ocv_uv, enum ocv_table_type ocv_type, int temp_10x)
{
	int pc;
	struct ocv_chg_data *ocd = NULL;

	if (batt_index < 0 || batt_index >= BATT_MAX || g_ocv_chg_data[batt_index] == NULL)
		return 0;

	ocd = g_ocv_chg_data[batt_index];
	if (!ocd->enable) { /* default we use dischg */
		pc = coul_interpolate_pc(ocd->batt_data->pc_temp_ocv_lut,
			temp_10x, ocv_uv / PERMILLAGE);
	} else {
		switch(ocv_type) {
		case OCV_CHG:
			pc = coul_interpolate_pc(ocd->batt_data->pc_temp_ocv_chg_lut,
				temp_10x, ocv_uv / PERMILLAGE);
			break;
		case OCV_DISCHG:
		default: /* no support mix type */
			pc = coul_interpolate_pc(ocd->batt_data->pc_temp_ocv_lut,
				temp_10x, ocv_uv / PERMILLAGE);
		}
	}
	return pc;
}

static int coul_ocv_sel_ocv_by_type(int batt_index, enum ocv_table_type ocv_type, int ocv_chg, int ocv_dchg, int soc_10x) {
	int ret, ocv_ret;
	struct coul_shmem_to_tzsp tzsp_shmem;
	struct ocv_chg_data *ocd = NULL;

	ocd = g_ocv_chg_data[batt_index];
	memcpy_s(&tzsp_shmem.ocd, sizeof(tzsp_shmem.ocd), ocd, sizeof(*ocd));
	tzsp_shmem.ocd.batt_data = NULL;
	tzsp_shmem.soc = soc_10x;
	tzsp_shmem.ocv_type = ocv_type;
	tzsp_shmem.ocv_chg = ocv_chg;
	tzsp_shmem.ocv_dchg = ocv_dchg;

	ret = tzsp_process(&tzsp_shmem, COUL_SEL_OCV_BY_TYPE);
	if (ret) {
		coul_err("%s: tzsp process error %d\n", __func__, ret);
		return ocv_dchg;
	}

	ocv_ret = tzsp_shmem.result;
	coul_info("%s: soc_10x = %d, ocv_type = %d, ocv_chg = %d, ocv_dchg = %d, ocv_ret = %d\n",
		__func__, soc_10x, ocv_type, ocv_chg, ocv_dchg, ocv_ret);
	return ocv_ret;
}

/**
 * anti look up pc_temp_ocv table，pc->ocv.
 * this function can only be called during discharge or change-done.
 *
 * @soc_10x: input soc
 * @ocv_type: ocv type used for anti look up table,
 *            if @soc_10x is not the current soc, should be ocv_table_type::OCV_DCHG.
 * Return: ocv_mv
 */
int coul_ocv_cal_tbl_ocv(int batt_index, int soc_10x, enum ocv_table_type ocv_type, int temp_10x)
{
	int ocv_chg;
	int ocv_dchg;
	struct ocv_chg_data *ocd = NULL;

	if (batt_index < 0 || batt_index >= BATT_MAX || g_ocv_chg_data[batt_index] == NULL)
		return 0;

	ocd = g_ocv_chg_data[batt_index];

	ocv_dchg = coul_interpolate_ocv(ocd->batt_data->pc_temp_ocv_lut, temp_10x / TENTH, soc_10x);
	if (!ocd->enable)
		return ocv_dchg;
	ocv_chg = coul_interpolate_ocv(ocd->batt_data->pc_temp_ocv_chg_lut, temp_10x / TENTH, soc_10x);

	return coul_ocv_sel_ocv_by_type(batt_index, ocv_type, ocv_chg, ocv_dchg, soc_10x);
}

void coul_ocv_chg_event_process(int batt_index, unsigned int evt)
{
	int soc;
	static int last_type[BATT_MAX] = { 0 }; /* 0: discharging. 1: charging */
	int charge_type[BATT_MAX] = { 0 };

	if (batt_index < 0 || batt_index >= BATT_MAX || g_ocv_chg_data[batt_index] == NULL)
		return;

	switch (evt) {
	case VCHRG_START_USB_CHARGING_EVENT:
	case VCHRG_START_AC_CHARGING_EVENT:
	case VCHRG_START_CHARGING_EVENT:
		charge_type[batt_index] = 1;
		break;
	case VCHRG_STOP_CHARGING_EVENT:
	case VCHRG_CURRENT_FULL_EVENT:
	case VCHRG_CHARGE_DONE_EVENT:
		charge_type[batt_index] = 0;
		break;
	default:
		return;
	}

	if (charge_type[batt_index] == 1) {
		if (last_type[batt_index] == 1)
			return;
		else
			last_type[batt_index] = 1;
	}

	if (charge_type[batt_index] == 0) {
		if (last_type[batt_index] == 0)
			return;
		else
			last_type[batt_index] = 0;
	}

	soc = coul_merge_drv_battery_unfiltered_capacity(batt_index);
	if (charge_type[batt_index] == 1)
		g_ocv_chg_data[batt_index]->chg_start_soc = soc;
	else
		g_ocv_chg_data[batt_index]->chg_stop_soc = soc;
	
	coul_info("%s changed, soc=%d, start_soc=%d, stop_soc=%d\n", __func__, soc,
		g_ocv_chg_data[batt_index]->chg_start_soc, g_ocv_chg_data[batt_index]->chg_stop_soc);
}

static void get_ocv_dts(struct ocv_chg_data *ocd, const struct device_node *np)
{
	int ret = 0;

	ret = of_property_read_u32(np, "dischg_ocv_soc", (unsigned int *)&ocd->dischg_ocv_soc);
	if (ret)
		ocd->dischg_ocv_soc = DEFAULT_DISCHG_OCV_SOC;

	ret = of_property_read_u32(np, "ocv_chg_enable", (unsigned int *)&ocd->enable);
	if (ret)
		ocd->enable = 0;

	ret = of_property_read_u32(np, "ocv_chg_highcap", (unsigned int *)&ocd->chg_highcap);
	if (ret)
		ocd->chg_highcap = DEFAULT_CHG_HIGHCAP;
	ret = of_property_read_u32(np, "ocv_dchg_highcap_thred", (unsigned int *)&ocd->dchg_highcap_thred);
	if (ret)
		ocd->dchg_highcap_thred = DEFAULT_DCHG_HIGHCAP_THRED;
	ret = of_property_read_u32(np, "ocv_chg_thred", (unsigned int *)&ocd->chg_thred);
	if (ret)
		ocd->chg_thred = DEFAULT_CHG_THRED;
	ret = of_property_read_u32(np, "ocv_dchg_up_thred", (unsigned int *)&ocd->dchg_up_thred);
	if (ret)
		ocd->dchg_up_thred = DEFAULT_DCHG_UP_THRED;
	ret = of_property_read_u32(np, "ocv_dchg_down_thred", (unsigned int *)&ocd->dchg_down_thred);
	if (ret)
		ocd->dchg_down_thred = DEFAULT_DCHG_DOWN_THRED;

	coul_core_info("%s onoff:%d, chg_highcap=%d, dchg_highcap_thred=%d, chg_thred=%d,"
		"dchg_up_thred=%d, dchg_down_thred=%d, dischg_ocv_soc=%d\n",
		__func__, ocd->enable, ocd->chg_highcap, ocd->dchg_highcap_thred, ocd->chg_thred,
		ocd->dchg_up_thred, ocd->dchg_down_thred, ocd->dischg_ocv_soc);
}

int coul_ocv_register(int batt_index, struct coul_battery_data *batt_data,
	const struct device_node *np)
{
	struct ocv_chg_data *ocd = NULL;

	if (batt_index < 0 || batt_index >= BATT_MAX || np == NULL || batt_data == NULL)
		return -1;
	ocd = (struct ocv_chg_data *)kzalloc(sizeof(*ocd), GFP_KERNEL);
	if (ocd == NULL) {
		coul_core_err("%s failed to alloc ocv_chg_data\n", __func__);
		return -ENOMEM;
	}

	get_ocv_dts(ocd, np);

	ocd->batt_data = batt_data;
	if (batt_data == NULL || batt_data->pc_temp_ocv_chg_lut == NULL) {
		coul_core_err("%s NULL chg-ocv data, disable chg ocv!\n", __func__);
		ocd->enable = 0;
	}

	g_ocv_chg_data[batt_index] = ocd;
	return 0;
}