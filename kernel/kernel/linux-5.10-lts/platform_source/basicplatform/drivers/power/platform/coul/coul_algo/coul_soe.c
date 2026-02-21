/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: coul_soe.c
 *
 * smartstar coulometer functions
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

#include <platform_include/basicplatform/linux/power/platform/coul/coul_merge_drv.h>
#include <platform_include/basicplatform/linux/power/platform/battery_data.h>
#include <platform_include/basicplatform/linux/power/platform/coul/coul_core.h>
#include "coul_soe.h"

#define coul_soe_info(fmt, args...) pr_info("[coul_soe]" fmt, ## args)
#define coul_soe_err(fmt, args...) pr_err("[coul_soe]" fmt, ## args)

static int coul_check_charge_status_change(struct soe_information *soe_info)
{
    if (soe_info->soe_para.current_ua <= 0)
        soe_info->curr_charge_status = SOE_CHARGE;
    else if (soe_info->soe_para.current_ua > 0 && soe_info->soe_para.charging_state == CHARGING_STATE_CHARGE_UNKNOW)
        soe_info->curr_charge_status = SOE_START;
    else
        soe_info->curr_charge_status = SOE_DISCHARGE;

    if (soe_info->curr_charge_status != soe_info->last_charge_status) {
        soe_info->last_charge_status = soe_info->curr_charge_status;
        return 1;
    }
    return 0;
}

/* calculate capcity when soe 2 soc */
static void coul_cal_soe_capacity_charging(struct soe_information *soe_info, int delta_cap)
{
    if (!soe_info->soe_adjust_flag) {
        soe_info->soe_para.capacity_1000x = soe_info->soe_para.soc * PERMILLAGE;
        return;
    } else
        soe_info->soe_para.capacity_1000x += delta_cap;

    if (DIV_ROUND_CLOSEST(soe_info->soe_para.capacity_1000x, PERMILLAGE) >= soe_info->soe_para.soc) {
        soe_info->soe_adjust_flag = 0;
        soe_info->soe_para.capacity_1000x = soe_info->soe_para.soc * PERMILLAGE;
    }
}

/* calculate capcity when soc 2 soe */
static void coul_calc_capacity_discharging(struct soe_information *soe_info, int delta_cap)
{
    if (!soe_info->soe_adjust_flag) {
        soe_info->soe_para.capacity_1000x = soe_info->soe_para.soe * PERMILLAGE;
        return;
    } else
        soe_info->soe_para.capacity_1000x -= delta_cap;

    if (DIV_ROUND_CLOSEST(soe_info->soe_para.capacity_1000x, PERMILLAGE) <= soe_info->soe_para.soe) {
        soe_info->soe_adjust_flag = 0;
        soe_info->soe_para.capacity_1000x = soe_info->soe_para.soe * PERMILLAGE;
    }
}

static int coul_calculate_delta_cap(struct soe_information *soe_info, int delta_time)
{
    int count, time_to_smooth, cap_to_smooth_real, delta_cap;
    int cap_to_smooth_est = soe_info->soe_para.fcc_uah * DELTA_SOE_SMOOTH / PERCENT;

    if ((soe_info->soe_para.current_ua == 0) || (soe_info->soe_para.fcc_uah - soe_info->soe_para.rm_uah < 0) ||
        (soe_info->soe_para.rm_uah - soe_info->soe_para.uuc_uah < 0))
        return DELTA_SOE_PC_DEFAULT;

    if (soe_info->curr_charge_status == SOE_CHARGE) {
        cap_to_smooth_real = (soe_info->soe_para.fcc_uah - soe_info->soe_para.rm_uah > cap_to_smooth_est)
                               ? cap_to_smooth_est : (soe_info->soe_para.fcc_uah - soe_info->soe_para.rm_uah);
        time_to_smooth = (int)((s64)cap_to_smooth_real * SEC_PER_HOUR /
                      (s64)abs(soe_info->soe_para.current_ua));
        count = time_to_smooth / (delta_time * CHARGE_COUNT_COE);
    } else {
        cap_to_smooth_real = (soe_info->soe_para.rm_uah - soe_info->soe_para.uuc_uah)
                               ? cap_to_smooth_est : (soe_info->soe_para.rm_uah - soe_info->soe_para.uuc_uah);
        time_to_smooth = (int)((s64)cap_to_smooth_real * SEC_PER_HOUR /
                      (s64)abs(soe_info->soe_para.current_ua));
        count = time_to_smooth / (delta_time * DISCHARGE_COUNT_COE);
    }
    if (count == 0)
        return DELTA_SOE_PC_DEFAULT;
    delta_cap = abs(soe_info->soe_para.soc - soe_info->soe_para.soe) * DELTA_SOE_GAIN / count;
    delta_cap = (delta_cap > PERMILLAGE) ? PERMILLAGE : delta_cap;
    coul_soe_info("batt_index is %d, delta_cap is %d\n", soe_info->soe_para.index, delta_cap);
    return delta_cap;
}

static int coul_limit_capacity(struct soe_information *soe_info)
{
    soe_info->soe_para.capacity_1000x = max(soe_info->soe_para.soe * PERMILLAGE, soe_info->soe_para.capacity_1000x);
    soe_info->soe_para.capacity_1000x = min(soe_info->soe_para.soc * PERMILLAGE, soe_info->soe_para.capacity_1000x);
    soe_info->soe_para.capacity_1000x = max(0, soe_info->soe_para.capacity_1000x);
    soe_info->soe_para.capacity_1000x = min(SOC_FULL * PERMILLAGE, soe_info->soe_para.capacity_1000x);
    coul_soe_info("batt is %d, soe_adjust_flag is %d, curr_charge_status is %d, last_charge_status is %d\n",
        soe_info->soe_para.index, soe_info->soe_adjust_flag,
        soe_info->curr_charge_status, soe_info->last_charge_status);
    return soe_info->soe_para.capacity_1000x;
}

static int coul_get_batt_capacity(struct soe_information *soe_info)
{
    int delta_cap;
    u64 time_now = dfx_getcurtime() / NSEC_PER_SEC;
    int delta_time = (int)(time_now - soe_info->last_calc_time);

    soe_info->last_calc_time = time_now;
    if (soe_info->first_in)
        soe_info->soe_para.capacity_1000x = soe_info->soe_para.ui_soc * PERMILLAGE;

    soe_info->first_in = FALSE;

    if (delta_time <= 0)
        return soe_info->soe_para.capacity_1000x;

    if (coul_check_charge_status_change(soe_info))
        soe_info->soe_adjust_flag = 1;

    delta_cap = coul_calculate_delta_cap(soe_info, delta_time);
    switch (soe_info->curr_charge_status) {
        case SOE_CHARGE:
            coul_cal_soe_capacity_charging(soe_info, delta_cap);
            break;
        case SOE_START:
        case SOE_DISCHARGE:
            coul_calc_capacity_discharging(soe_info, delta_cap);
            break;
        default:
            break;
    }
    return coul_limit_capacity(soe_info);
}

int cou_calc_soe_cap(struct soe_params *soe_params, struct soe_information *soe_info, int *soe_value)
{
    s64 soe;

    soe = (s64)soe_info->soe_para.soc;
    soe = (s64)soe_params->a * soe * soe * soe + (s64)soe_params->b * soe * soe +
          (s64)soe_params->c * soe + (s64)soe_params->d;
    *soe_value = (int)(soe / SOE_PARAMS_GAIN);
    soe_info->soe_para.soe = *soe_value;
    return coul_get_batt_capacity(soe_info);
}

static void coul_soe_info_init(struct soe_information *soe_info)
{
    soe_info->first_in = TRUE;
    soe_info->soe_adjust_flag = 1;
    soe_info->last_calc_time = dfx_getcurtime() / NSEC_PER_SEC;
    soe_info->curr_charge_status = SOE_UNKNOW;
    soe_info->last_charge_status = SOE_UNKNOW;
}

void coul_soe_initial(int batt_index, int soe_enable, struct soe_params *soe_param, struct soe_information *soe_info)
{
    if (soe_enable == 0)
        return;
    
    coul_soe_info_init(soe_info);
    if (soe_param == NULL || batt_index < 0 || batt_index >= BATT_MAX)
        goto default_params;
    if (soe_param->a < 0 || soe_param->a >= SOE_PARAMS_GAIN)
        goto default_params;
    if (soe_param->b <= -SOE_PARAMS_GAIN || soe_param->b >= SOE_PARAMS_GAIN)
        goto default_params;
    if (soe_param->c <= -SOE_PARAMS_GAIN || soe_param->c >= SOE_PARAMS_GAIN)
        goto default_params;
    if (soe_param->d <= -SOE_PARAMS_GAIN || soe_param->d >= SOE_PARAMS_GAIN)
        goto default_params;

    coul_soe_info("%s a:%d, b:%d, c:%d, d:%d\n",
        __func__, soe_param->a, soe_param->b, soe_param->c, soe_param->d);
    return;
default_params:
    soe_param->a = 0;
    soe_param->b = 0;
    soe_param->c = SOE_PARAMS_GAIN;
    soe_param->d = 0;
    coul_soe_info("%s use default params!\n", __func__);
}
