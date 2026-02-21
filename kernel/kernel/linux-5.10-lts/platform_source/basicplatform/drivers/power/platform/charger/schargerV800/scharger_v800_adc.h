/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 *
 * scharger_v800_adc.h
 *
 * SchargerV800 adc header.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef __SCHARGER_V800_ADC_H__
#define __SCHARGER_V800_ADC_H__

#define CHG_ILIMIT_600MA_REG    0x06

#define CHG_ADC_CH_VUSB         0
#define CHG_ADC_CH_VPSW         1
#define CHG_ADC_CH_VBUS         2
#define CHG_ADC_CH_VBAT         3
#define CHG_ADC_CH_VSYSH        4
#define CHG_ADC_CH_IBAT         5
#define CHG_ADC_CH_IBUS_REF     6
#define CHG_ADC_CH_IBUS1        7
#define CHG_ADC_CH_L_MAX        8

#define CHG_ADC_CH_IBUS2        (0 + CHG_ADC_CH_L_MAX)
#define CHG_ADC_CH_TDIE         (1 + CHG_ADC_CH_L_MAX)
#define CHG_ADC_CH_TSBAT        (2 + CHG_ADC_CH_L_MAX)
#define CHG_ADC_CH_RESERVE1     (3 + CHG_ADC_CH_L_MAX)
#define CHG_ADC_CH_RESERVE2     (4 + CHG_ADC_CH_L_MAX)
#define CHG_ADC_CH_VDPDN        (5 + CHG_ADC_CH_L_MAX)
#define CHG_ADC_CH_SBU          (6 + CHG_ADC_CH_L_MAX)
#define CHG_ADC_CH_INTERNAL_CAL (7 + CHG_ADC_CH_L_MAX)

#define CHG_IBUS_DIV            5
#define MV_PER_V   1000
#define SCHARGER_100KOHOM 100
#define SCHARGER_1UA      1
#define SCHARGER_10UA     10

int scharger_v800_get_vusb(void);
int scharger_v800_get_vbus(void);
int scharger_v800_get_vbat(void);
int scharger_v800_get_ibus(void);
int scharger_v800_get_tdie(void);
int scharger_v800_get_sbu1_res(void);
int scharger_v800_get_sbu2_res(void);
int scharger_v800_get_dp_res(void);
int scharger_v800_get_vpsw(void);
int scharger_v800_get_ibat(void);
int scharger_v800_get_tsbat(void);
int scharger_v800_get_vsysh(void);
int scharger_v800_get_voltage_offset(void);
#endif