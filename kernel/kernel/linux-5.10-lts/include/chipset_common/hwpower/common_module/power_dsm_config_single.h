/* SPDX-License-Identifier: GPL-2.0 */
/*
 * power_dsm_config_single.h
 *
 * dsm for power module
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


#define SMARTCHARGE_ILLEGAL_BAT_REUSE			920002011
#define SMARTCHARGE_DCR_ABNORMAL_WHEN_BOOTUP	920002016
#define SMARTCHARGE_DCR_ABNORMAL_NOW			920002017
#define SMARTCHARGE_BAT_NO_FAULT_WHEN_BOOTUP	920002018
#define SMARTCHARGE_BAT_VOL_TEMP	            930001002
#define SMARTCHARGE_CCA_BAT_PROTECT             930001003
#define SMARTCHARGE_ACR_DET                     930001004
#define SMARTCHARGE_DCR_DET0                    930001005
#define SMARTCHARGE_BAT_VOL_TEMP_SHORT          930001006
#define SMARTCHARGE_BAT_AGING_SAFE          	930001007
#define SMARTCHARGE_QNS_BAT_PROTECTION      	930001008
#define SMARTCHARGE_CV_T_DET	                930001009
#define SMARTCHARGE_BAT_ANTI_BIG_DATA           930001010
#define SMARTCHARGE_FAST_FCC_RAW_DATA	        930001011
#define SMARTCHARGE_SAFETY_DET_PARAS_IN_BAT     930001012
#define SMARTCHARGE_DCR_DET1                    930001016
#define SMARTCHARGE_REVERSE_SUPER_FAST_CHG      930001017
#define SMARTCHARGE_REVERSE_SUPER_FAST_CHG_FAIL 930001018
#define SMARTCHARGE_BAT_HEALTH_RAW_DATA         930009001
#define SMARTCHARGE_Q_OFFSET_WLC                930009002
#define SMARTCHARGE_NON_STD_CHARGER_PLUG_IN		920008033
#define BMSBEHAVIOR_NOT_CHARGING_IBUS_ZERO		920008000
#define BMSBEHAVIOR_NOT_CHARGING_TEMP_OVER		920008001
#define BMSBEHAVIOR_CHARGING_SLOW_TEMP_OVER		920008002
#define BMSBEHAVIOR_CHARGING_SLOW_IBUS_LOW		920008004
#define BMSBEHAVIOR_CHARGING_OVER_RELEASE		920008006
#define BMSBEHAVIOR_BATTERY_NOT_PRESENT			920008008


int power_dsm_single_battery_register(void);
int power_dsm_single_domain_check(const char *domain);