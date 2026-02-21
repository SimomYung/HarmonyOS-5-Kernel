/* SPDX-License-Identifier: GPL-2.0 */
/*
 * power_dsm.h
 *
 * dsm for power module
 *
 * Copyright (c) 2020-2020 Huawei Technologies Co., Ltd.
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

#ifndef _POWER_DSM_H_
#define _POWER_DSM_H_

#include <dsm/dsm_pub.h>
#include <linux/list.h>

/* define dsm new id */
#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
#define POWER_DSM_ERROR_BATT_ACR_OVER_THRESHOLD           ERROR_BATT_ACR_OVER_THRESHOLD
#define POWER_DSM_ERROR_LOW_VOL_INT                       ERROR_LOW_VOL_INT
#define POWER_DSM_BATTERY_COUL_DROP_INT                   DSM_BATTERY_COUL_DROP_INT
#define POWER_DSM_ERROR_NO_WATER_CHECK_BASE               ERROR_NO_WATER_CHECK_BASE
#define POWER_DSM_ERROR_NO_TYPEC_4480_OVP                 ERROR_NO_TYPEC_4480_OVP
#define POWER_DSM_ERROR_SWITCH_ATTACH                     ERROR_SWITCH_ATTACH
#define POWER_DSM_ERROR_FCP_OUTPUT                        ERROR_FCP_OUTPUT
#define POWER_DSM_ERROR_FCP_DETECT                        ERROR_FCP_DETECT
#define POWER_DSM_ERROR_BATT_TEMP_LOW                     ERROR_BATT_TEMP_LOW
#define POWER_DSM_ERROR_BATT_SOC_CHANGE_FAST              ERROR_BATT_SOC_CHANGE_FAST
#define POWER_DSM_ERROR_NO_TYPEC_CC_OVP                   ERROR_NO_TYPEC_CC_OVP
#define POWER_DSM_ERROR_WIRELESS_TX_STATISTICS            ERROR_WIRELESS_TX_STATISTICS
#define POWER_DSM_ERROR_NO_SMPL                           ERROR_NO_SMPL
#define POWER_DSM_ERROR_NON_STANDARD_CHARGER_PLUGGED      ERROR_NON_STANDARD_CHARGER_PLUGGED
#define POWER_DSM_ERROR_NO_WATER_CHECK_IN_USB             ERROR_NO_WATER_CHECK_IN_USB
#define POWER_DSM_ERROR_BATTERY_POLAR_ISHORT              ERROR_BATTERY_POLAR_ISHORT
#define POWER_DSM_ERROR_NO_CPU_BUCK_BASE                  ERROR_NO_CPU_BUCK_BASE
#define POWER_DSM_ERROR_REFRESH_FCC_OUTSIDE               ERROR_REFRESH_FCC_OUTSIDE
#define POWER_DSM_ERROR_BATT_TERMINATE_TOO_EARLY          ERROR_BATT_TERMINATE_TOO_EARLY
#define POWER_DSM_ERROR_CHARGE_I2C_RW                     ERROR_CHARGE_I2C_RW
#define POWER_DSM_ERROR_WEAKSOURCE_STOP_CHARGE            ERROR_WEAKSOURCE_STOP_CHARGE
#define POWER_DSM_ERROR_BOOST_OCP                         ERROR_BOOST_OCP
#define POWER_DSM_ERROR_NO_USB_SHORT_PROTECT_NTC          ERROR_NO_USB_SHORT_PROTECT_NTC
#define POWER_DSM_ERROR_NO_USB_SHORT_PROTECT              ERROR_NO_USB_SHORT_PROTECT
#define POWER_DSM_ERROR_NO_USB_SHORT_PROTECT_HIZ          ERROR_NO_USB_SHORT_PROTECT_HIZ
#define POWER_DSM_ERROR_WIRELESS_BOOSTING_FAIL            ERROR_WIRELESS_BOOSTING_FAIL
#define POWER_DSM_ERROR_WIRELESS_CERTI_COMM_FAIL          ERROR_WIRELESS_CERTI_COMM_FAIL
#define POWER_DSM_ERROR_WIRELESS_CHECK_TX_ABILITY_FAIL    ERROR_WIRELESS_CHECK_TX_ABILITY_FAIL
#define POWER_DSM_ERROR_WIRELESS_RX_OCP                   ERROR_WIRELESS_RX_OCP
#define POWER_DSM_ERROR_WIRELESS_RX_OVP                   ERROR_WIRELESS_RX_OVP
#define POWER_DSM_ERROR_WIRELESS_RX_OTP                   ERROR_WIRELESS_RX_OTP
#define POWER_DSM_ERROR_WIRELESS_RX_OPP                   ERROR_WIRELESS_RX_OPP
#define POWER_DSM_ERROR_WIRELESS_TX_POWER_SUPPLY_FAIL     ERROR_WIRELESS_TX_POWER_SUPPLY_FAIL
#define POWER_DSM_ERROR_WIRELESS_TX_BATTERY_OVERHEAT      ERROR_WIRELESS_TX_BATTERY_OVERHEAT
#define POWER_DSM_ERROR_WIRELESS_TX_FOD_PING_TIMEOUT      ERROR_WIRELESS_TX_FOD_PING_TIMEOUT
#define POWER_DSM_ERROR_WIRELESS_ERROR                    ERROR_WIRELESS_ERROR
#define POWER_DSM_ERROR_ADAPTER_OVLT                      ERROR_ADAPTER_OVLT
#define POWER_DSM_ERROR_ADAPTER_OCCURRENT                 ERROR_ADAPTER_OCCURRENT
#define POWER_DSM_ERROR_ADAPTER_OTEMP                     ERROR_ADAPTER_OTEMP
#define POWER_DSM_DIRECT_CHARGE_ADAPTER_OTP               DSM_DIRECT_CHARGE_ADAPTER_OTP
#define POWER_DSM_DIRECT_CHARGE_VOL_ACCURACY              DSM_DIRECT_CHARGE_VOL_ACCURACY
#define POWER_DSM_DIRECT_CHARGE_USB_PORT_LEAKAGE_CURRENT  DSM_DIRECT_CHARGE_USB_PORT_LEAKAGE_CURRENT
#define POWER_DSM_DIRECT_CHARGE_FULL_PATH_RESISTANCE      DSM_DIRECT_CHARGE_FULL_PATH_RESISTANCE
#define POWER_DSM_DIRECT_CHARGE_VBUS_OVP                  DSM_DIRECT_CHARGE_VBUS_OVP
#define POWER_DSM_DIRECT_CHARGE_REVERSE_OCP               DSM_DIRECT_CHARGE_REVERSE_OCP
#define POWER_DSM_DIRECT_CHARGE_OTP                       DSM_DIRECT_CHARGE_OTP
#define POWER_DSM_DIRECT_CHARGE_INPUT_OCP                 DSM_DIRECT_CHARGE_INPUT_OCP
#define POWER_DSM_DIRECT_CHARGE_SC_FAULT_VBUS_OVP         DSM_DIRECT_CHARGE_SC_FAULT_VBUS_OVP
#define POWER_DSM_DIRECT_CHARGE_SC_FAULT_TSBAT_OTP        DSM_DIRECT_CHARGE_SC_FAULT_TSBAT_OTP
#define POWER_DSM_DIRECT_CHARGE_SC_FAULT_TSBUS_OTP        DSM_DIRECT_CHARGE_SC_FAULT_TSBUS_OTP
#define POWER_DSM_DIRECT_CHARGE_SC_FAULT_TDIE_OTP         DSM_DIRECT_CHARGE_SC_FAULT_TDIE_OTP
#define POWER_DSM_DIRECT_CHARGE_SC_FAULT_AC_OVP           DSM_DIRECT_CHARGE_SC_FAULT_AC_OVP
#define POWER_DSM_DIRECT_CHARGE_SC_FAULT_VBAT_OVP         DSM_DIRECT_CHARGE_SC_FAULT_VBAT_OVP
#define POWER_DSM_DIRECT_CHARGE_SC_FAULT_IBAT_OCP         DSM_DIRECT_CHARGE_SC_FAULT_IBAT_OCP
#define POWER_DSM_DIRECT_CHARGE_SC_FAULT_IBUS_OCP         DSM_DIRECT_CHARGE_SC_FAULT_IBUS_OCP
#define POWER_DSM_DIRECT_CHARGE_SC_FAULT_CONV_OCP         DSM_DIRECT_CHARGE_SC_FAULT_CONV_OCP
#define POWER_DSM_DIRECT_CHARGE_SC_FAULT_CFLY_SHORT       DSM_DIRECT_CHARGE_SC_FAULT_CFLY_SHORT
#define POWER_DSM_DIRECT_CHARGE_FULL_PATH_RESISTANCE_2ND  DSM_DIRECT_CHARGE_FULL_PATH_RESISTANCE_2ND
#define POWER_DSM_SOFT_RECHARGE_NO                        DSM_SOFT_RECHARGE_NO
#define POWER_DSM_BATTERY_HEATING                         DSM_BATTERY_HEATING
#define POWER_DSM_POWER_DEVICES_INFO                      DSM_POWER_DEVICES_INFO
#define POWER_DSM_BATTERY_DETECT_ERROR_NO                 DSM_BATTERY_DETECT_ERROR_NO
#define POWER_DSM_OTG_VBUS_SHORT                          DSM_OTG_VBUS_SHORT
#define POWER_DSM_LIGHTSTRAP_STATUS                       DSM_LIGHTSTRAP_STATUS
#define POWER_DSM_PD_TYPEC_VBUS_VALID                     DSM_PD_TYPEC_VBUS_VALID
#define POWER_DSM_PD_TYPEC_VBUS_STILL_VALID               DSM_PD_TYPEC_VBUS_STILL_VALID
#define POWER_DSM_DA_BATTERY_ACR_ERROR_NO                 DA_BATTERY_ACR_ERROR_NO
#define POWER_DSM_BATTERY_ROM_ID_CERTIFICATION_FAIL       DSM_BATTERY_ROM_ID_CERTIFICATION_FAIL
#define POWER_DSM_BATTERY_IC_EEPROM_STATE_ERROR           DSM_BATTERY_IC_EEPROM_STATE_ERROR
#define POWER_DSM_BATTERY_IC_KEY_CERTIFICATION_FAIL       DSM_BATTERY_IC_KEY_CERTIFICATION_FAIL
#define POWER_DSM_OLD_BOARD_AND_OLD_BATTERY_UNMATCH       DSM_OLD_BOARD_AND_OLD_BATTERY_UNMATCH
#define POWER_DSM_OLD_BOARD_AND_NEW_BATTERY_UNMATCH       DSM_OLD_BOARD_AND_NEW_BATTERY_UNMATCH
#define POWER_DSM_NEW_BOARD_AND_OLD_BATTERY_UNMATCH       DSM_NEW_BOARD_AND_OLD_BATTERY_UNMATCH
#define POWER_DSM_BATTERY_NV_DATA_READ_FAIL               DSM_BATTERY_NV_DATA_READ_FAIL
#define POWER_DSM_CERTIFICATION_SERVICE_IS_NOT_RESPONDING DSM_CERTIFICATION_SERVICE_IS_NOT_RESPONDING
#define POWER_DSM_UNMATCH_BATTERYS                        DSM_UNMATCH_BATTERYS
#define POWER_DSM_MULTI_CHARGE_CURRENT_RATIO_INFO         DSM_MULTI_CHARGE_CURRENT_RATIO_INFO
#define POWER_DSM_MULTI_CHARGE_CURRENT_RATIO_WARNING      DSM_MULTI_CHARGE_CURRENT_RATIO_WARNING
#define POWER_DSM_MULTI_CHARGE_CURRENT_RATIO_ERROR        DSM_MULTI_CHARGE_CURRENT_RATIO_ERROR
#define POWER_DSM_DIRECT_CHARGE_ERR_WITH_STD_CABLE        DSM_DIRECT_CHARGE_ERR_WITH_STD_CABLE
#define POWER_DSM_DIRECT_CHARGE_ERR_WITH_NONSTD_CABLE     DSM_DIRECT_CHARGE_ERR_WITH_NONSTD_CABLE
#define POWER_DSM_DUAL_BATTERY_CURRENT_BIAS_DETECT        DSM_DUAL_BATTERY_CURRENT_BIAS_DETECT
#define POWER_DSM_DUAL_BATTERY_OUT_OF_POSITION_DETECTION  DSM_DUAL_BATTERY_OUT_OF_POSITION_DETECTION
#define POWER_DSM_DUAL_BATTERY_CAPACITY_DIFFERENT_DETECT  DSM_DUAL_BATTERY_CAPACITY_DIFFERENT_DETECT
#define POWER_DSM_WLPS_GPIO_TXSW                          DSM_WLPS_GPIO_TXSW
#define POWER_DSM_PMU_BMS_BATTERY_VOL_SOC_DISMATCH        DSM_PMU_BMS_BATTERY_VOL_SOC_DISMATCH
#define POWER_DSM_BYASS_BOOST_FAULT                       ERROR_NO_BYPASS_BOOST_FAULT
#define POWER_DSM_BYPASS_BOOST_I2C_TRANS_FAIL             ERROR_NO_BYPASS_BOOST_I2C_TRANS_FAIL
#define POWER_DSM_CHARGE_SCHARGER_USB_PORT_VDROPMIN_WARNING               DSM_CHARGE_SCHARGER_USB_PORT_VDROPMIN_WARNING
#define POWER_DSM_CHARGE_SCHARGER_SERIAL_PARALLEL_SWITCH_QSW2_ERROR       DSM_CHARGE_SCHARGER_SERIAL_PARALLEL_SWITCH_QSW2_ERROR
#define POWER_DSM_CHARGE_SCHARGER_SERIAL_PARALLEL_SWITCH_QSW1_ERROR       DSM_CHARGE_SCHARGER_SERIAL_PARALLEL_SWITCH_QSW1_ERROR
#define POWER_DSM_CHARGE_SCHARGER_SERIAL_CHARGE_QSW2_RCP_ERROR            DSM_CHARGE_SCHARGER_SERIAL_CHARGE_QSW2_RCP_ERROR
#define POWER_DSM_CHARGE_SCHARGER_PARALLEL_ONE_BATTERY_DISCHARGE_WARNING  DSM_CHARGE_SCHARGER_PARALLEL_ONE_BATTERY_DISCHARGE_WARNING
#define POWER_DSM_CHARGE_SCHARGER_SERIAL_CHARGE_ONE_BATTERY_ERR_WARNING   DSM_CHARGE_SCHARGER_SERIAL_CHARGE_ONE_BATTERY_ERR_WARNING
#define POWER_DSM_CHARGE_SCHARGER_LVC_PATH_WARNING                        DSM_CHARGE_SCHARGER_LVC_PATH_WARNING
#define POWER_DSM_CHARGE_SCHARGER_FUEL_GAUGE_PRECISION_WARNING            DSM_CHARGE_SCHARGER_FUEL_GAUGE_PRECISION_WARNING
#define POWER_DSM_CHARGE_DYNAMIC_CV_TRIM_MAX                              DSM_CHARGE_DYNAMIC_CV_TRIM_MAX
#define POWER_DSM_CHARGE_DYNAMIC_CV_TRIM_INFO                             DSM_CHARGE_DYNAMIC_CV_TRIM_INFO
#define POWER_DSM_CHARGE_DYNAMIC_CV_CHARGER_DONE_VBAT_INFO                DSM_CHARGE_DYNAMIC_CV_CHARGER_DONE_VBAT_INFO
#define POWER_DSM_ASW_SCENE_H3                            920001203
#define POWER_DSM_ASW_SCENE_H4                            920001204
#define POWER_DSM_ASW_SCENE_H5                            920001205
#define POWER_DSM_ASW_SCENE_H6                            920001206
#define POWER_DSM_CVT_ABNORMAL                            920001105
#define POWER_DSM_FASTFCC_SLOPE_OVER_THRESHOLD            920001106
#define POWER_DSM_ACR_SLOPE_OVER_THRESHOLD                920001107
#define POWER_DSM_ACR_ERROR_NO                            924005003
#define POWER_DSM_RCHG_TYPE_SUPER                         DSM_RCHG_TYPE_SUPER
#define POWER_DSM_RCHG_TYPE_NORMAL                        DSM_RCHG_TYPE_NORMAL
#define POWER_DSM_BATTERY_TEMP_OUT_RANGE                  DSM_BATTERY_TEMP_OUT_RANGE
#define POWER_DSM_BAT_FAKE_COUL_USING                     DSM_BAT_FAKE_COUL_USING
#else
#define POWER_DSM_ERROR_BATT_ACR_OVER_THRESHOLD           0
#define POWER_DSM_ERROR_LOW_VOL_INT                       0
#define POWER_DSM_ERROR_NO_WATER_CHECK_BASE               0
#define POWER_DSM_ERROR_NO_TYPEC_4480_OVP                 0
#define POWER_DSM_ERROR_SWITCH_ATTACH                     0
#define POWER_DSM_ERROR_FCP_OUTPUT                        0
#define POWER_DSM_ERROR_FCP_DETECT                        0
#define POWER_DSM_ERROR_BATT_TEMP_LOW                     0
#define POWER_DSM_ERROR_BATT_SOC_CHANGE_FAST              0
#define POWER_DSM_ERROR_NO_TYPEC_CC_OVP                   0
#define POWER_DSM_ERROR_WIRELESS_TX_STATISTICS            0
#define POWER_DSM_ERROR_NO_SMPL                           0
#define POWER_DSM_ERROR_NON_STANDARD_CHARGER_PLUGGED      0
#define POWER_DSM_ERROR_NO_WATER_CHECK_IN_USB             0
#define POWER_DSM_ERROR_BATTERY_POLAR_ISHORT              0
#define POWER_DSM_ERROR_NO_CPU_BUCK_BASE                  0
#define POWER_DSM_ERROR_REFRESH_FCC_OUTSIDE               0
#define POWER_DSM_ERROR_BATT_TERMINATE_TOO_EARLY          0
#define POWER_DSM_ERROR_CHARGE_I2C_RW                     0
#define POWER_DSM_ERROR_WEAKSOURCE_STOP_CHARGE            0
#define POWER_DSM_ERROR_BOOST_OCP                         0
#define POWER_DSM_ERROR_NO_USB_SHORT_PROTECT_NTC          0
#define POWER_DSM_ERROR_NO_USB_SHORT_PROTECT              1
#define POWER_DSM_ERROR_NO_USB_SHORT_PROTECT_HIZ          2
#define POWER_DSM_ERROR_WIRELESS_BOOSTING_FAIL            0
#define POWER_DSM_ERROR_WIRELESS_CERTI_COMM_FAIL          0
#define POWER_DSM_ERROR_WIRELESS_CHECK_TX_ABILITY_FAIL    0
#define POWER_DSM_ERROR_WIRELESS_RX_OCP                   0
#define POWER_DSM_ERROR_WIRELESS_RX_OVP                   0
#define POWER_DSM_ERROR_WIRELESS_RX_OTP                   0
#define POWER_DSM_ERROR_WIRELESS_RX_OPP                   0
#define POWER_DSM_ERROR_WIRELESS_TX_POWER_SUPPLY_FAIL     0
#define POWER_DSM_ERROR_WIRELESS_TX_BATTERY_OVERHEAT      0
#define POWER_DSM_ERROR_WIRELESS_TX_FOD_PING_TIMEOUT      0
#define POWER_DSM_ERROR_WIRELESS_ERROR                    0
#define POWER_DSM_ERROR_ADAPTER_OVLT                      0
#define POWER_DSM_ERROR_ADAPTER_OCCURRENT                 0
#define POWER_DSM_ERROR_ADAPTER_OTEMP                     0
#define POWER_DSM_DIRECT_CHARGE_ADAPTER_OTP               0
#define POWER_DSM_DIRECT_CHARGE_VOL_ACCURACY              0
#define POWER_DSM_DIRECT_CHARGE_USB_PORT_LEAKAGE_CURRENT  0
#define POWER_DSM_DIRECT_CHARGE_FULL_PATH_RESISTANCE      0
#define POWER_DSM_DIRECT_CHARGE_VBUS_OVP                  0
#define POWER_DSM_DIRECT_CHARGE_REVERSE_OCP               0
#define POWER_DSM_DIRECT_CHARGE_OTP                       0
#define POWER_DSM_DIRECT_CHARGE_INPUT_OCP                 0
#define POWER_DSM_DIRECT_CHARGE_SC_FAULT_VBUS_OVP         0
#define POWER_DSM_DIRECT_CHARGE_SC_FAULT_TSBAT_OTP        0
#define POWER_DSM_DIRECT_CHARGE_SC_FAULT_TSBUS_OTP        0
#define POWER_DSM_DIRECT_CHARGE_SC_FAULT_TDIE_OTP         0
#define POWER_DSM_DIRECT_CHARGE_SC_FAULT_AC_OVP           0
#define POWER_DSM_DIRECT_CHARGE_SC_FAULT_VBAT_OVP         0
#define POWER_DSM_DIRECT_CHARGE_SC_FAULT_IBAT_OCP         0
#define POWER_DSM_DIRECT_CHARGE_SC_FAULT_IBUS_OCP         0
#define POWER_DSM_DIRECT_CHARGE_SC_FAULT_CONV_OCP         0
#define POWER_DSM_DIRECT_CHARGE_SC_FAULT_CFLY_SHORT       0
#define POWER_DSM_DIRECT_CHARGE_FULL_PATH_RESISTANCE_2ND  0
#define POWER_DSM_SOFT_RECHARGE_NO                        0
#define POWER_DSM_BATTERY_HEATING                         0
#define POWER_DSM_POWER_DEVICES_INFO                      0
#define POWER_DSM_BATTERY_DETECT_ERROR_NO                 0
#define POWER_DSM_OTG_VBUS_SHORT                          0
#define POWER_DSM_LIGHTSTRAP_STATUS                       0
#define POWER_DSM_PD_TYPEC_VBUS_VALID                     0
#define POWER_DSM_PD_TYPEC_VBUS_STILL_VALID               0
#define POWER_DSM_DA_BATTERY_ACR_ERROR_NO                 0
#define POWER_DSM_BATTERY_ROM_ID_CERTIFICATION_FAIL       0
#define POWER_DSM_BATTERY_IC_EEPROM_STATE_ERROR           0
#define POWER_DSM_BATTERY_IC_KEY_CERTIFICATION_FAIL       0
#define POWER_DSM_OLD_BOARD_AND_OLD_BATTERY_UNMATCH       0
#define POWER_DSM_OLD_BOARD_AND_NEW_BATTERY_UNMATCH       0
#define POWER_DSM_NEW_BOARD_AND_OLD_BATTERY_UNMATCH       0
#define POWER_DSM_BATTERY_NV_DATA_READ_FAIL               0
#define POWER_DSM_CERTIFICATION_SERVICE_IS_NOT_RESPONDING 0
#define POWER_DSM_UNMATCH_BATTERYS                        0
#define POWER_DSM_MULTI_CHARGE_CURRENT_RATIO_INFO         0
#define POWER_DSM_MULTI_CHARGE_CURRENT_RATIO_WARNING      0
#define POWER_DSM_MULTI_CHARGE_CURRENT_RATIO_ERROR        0
#define POWER_DSM_DIRECT_CHARGE_ERR_WITH_STD_CABLE        0
#define POWER_DSM_DIRECT_CHARGE_ERR_WITH_NONSTD_CABLE     0
#define POWER_DSM_DUAL_BATTERY_CURRENT_BIAS_DETECT        0
#define POWER_DSM_DUAL_BATTERY_OUT_OF_POSITION_DETECTION  0
#define POWER_DSM_DUAL_BATTERY_CAPACITY_DIFFERENT_DETECT  0
#define POWER_DSM_WLPS_GPIO_TXSW                          0
#define POWER_DSM_PMU_BMS_BATTERY_VOL_SOC_DISMATCH        0
#define POWER_DSM_BYASS_BOOST_FAULT                       0
#define POWER_DSM_BYPASS_BOOST_I2C_TRANS_FAIL             0
#define POWER_DSM_CHARGE_SCHARGER_USB_PORT_VDROPMIN_WARNING               0
#define POWER_DSM_CHARGE_SCHARGER_SERIAL_PARALLEL_SWITCH_QSW2_ERROR       0
#define POWER_DSM_CHARGE_SCHARGER_SERIAL_PARALLEL_SWITCH_QSW1_ERROR       0
#define POWER_DSM_CHARGE_SCHARGER_SERIAL_CHARGE_QSW2_RCP_ERROR            0
#define POWER_DSM_CHARGE_SCHARGER_PARALLEL_ONE_BATTERY_DISCHARGE_WARNING  0
#define POWER_DSM_CHARGE_SCHARGER_SERIAL_CHARGE_ONE_BATTERY_ERR_WARNING   0
#define POWER_DSM_CHARGE_SCHARGER_LVC_PATH_WARNING                        0
#define POWER_DSM_CHARGE_SCHARGER_FUEL_GAUGE_PRECISION_WARNING            0
#define POWER_DSM_CHARGE_DYNAMIC_CV_TRIM_MAX                              0
#define POWER_DSM_CHARGE_DYNAMIC_CV_TRIM_INFO                             0
#define POWER_DSM_CHARGE_DYNAMIC_CV_CHARGER_DONE_VBAT_INFO                0
#define POWER_DSM_RCHG_TYPE_SUPER                         0
#define POWER_DSM_RCHG_TYPE_NORMAL                        0
#define POWER_DSM_BATTERY_TEMP_OUT_RANGE                  0
#define POWER_DSM_BAT_FAKE_COUL_USING                     0
#endif /* CONFIG_HUAWEI_OHOS_DSM || CONFIG_HUAWEI_DSM */

/* define dsm buffer size */
#define POWER_DSM_BUF_SIZE_0016                           16
#define POWER_DSM_BUF_SIZE_0032                           32
#define POWER_DSM_BUF_SIZE_0048                           48
#define POWER_DSM_BUF_SIZE_0128                           128
#define POWER_DSM_BUF_SIZE_0256                           256
#define POWER_DSM_BUF_SIZE_0512                           512
#define POWER_DSM_BUF_SIZE_1024                           1024
#define POWER_DSM_BUF_SIZE_2048                           2048
#define POWER_DSM_RESERVED_SIZE                           16

#ifdef CONFIG_HUAWEI_OHOS_DSM
#define POWER_DSM_ERR             (-1)
#define POWER_DSM_OK              0

#endif

enum power_dsm_type {
	POWER_DSM_TYPE_BEGIN = 0,
	POWER_DSM_CPU_BUCK = POWER_DSM_TYPE_BEGIN,
	POWER_DSM_USB,
	POWER_DSM_BATTERY_DETECT,
	POWER_DSM_BATTERY,
	POWER_DSM_CHARGE_MONITOR,
	POWER_DSM_SUPERSWITCH,
	POWER_DSM_SMPL,
	POWER_DSM_PD_RICHTEK,
	POWER_DSM_PD,
	POWER_DSM_USCP,
	POWER_DSM_PMU_OCP,
	POWER_DSM_PMU_BMS,
	POWER_DSM_PMU_IRQ,
	POWER_DSM_DIRECT_CHARGE_SC,
	POWER_DSM_FCP_CHARGE,
	POWER_DSM_MTK_SWITCH_CHARGE2,
	POWER_DSM_LIGHTSTRAP,
	POWER_DSM_TYPE_END,
};

#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
struct power_dsm_client {
	unsigned int type;
	const char *name;
	struct dsm_client *client;
	struct dsm_dev *dev;
};
#endif /* CONFIG_HUAWEI_OHOS_DSM || CONFIG_HUAWEI_DSM */

struct hiview_info {
	const char *key;
	char *value;
};

struct power_dsm_dump {
	unsigned int type;
	int error_no;
	bool dump_enable;
	bool dump_always;
	const char *pre_buf;
	bool (*support)(void);
	void (*dump)(char *buf, unsigned int buf_len);
	bool (*check_error)(char *buf, unsigned int buf_len);
};

#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
struct dsm_client *power_dsm_get_dclient(unsigned int type);
int power_dsm_report_dmd(unsigned int type, int err_no, const char *buf);

#ifdef CONFIG_HUAWEI_DATA_ACQUISITION
int power_dsm_report_fmd(unsigned int type, int err_no,
	const void *msg);
#else
static inline int power_dsm_report_fmd(unsigned int type,
	int err_no, const void *msg)
{
	return 0;
}
#endif /* CONFIG_HUAWEI_DATA_ACQUISITION */

#ifndef CONFIG_HUAWEI_OHOS_DSM
#define power_dsm_report_format_dmd(type, err_no, fmt, args...) do { \
	if (power_dsm_get_dclient(type)) { \
		if (!dsm_client_ocuppy(power_dsm_get_dclient(type))) { \
			dsm_client_record(power_dsm_get_dclient(type), fmt, ##args); \
			dsm_client_notify(power_dsm_get_dclient(type), err_no); \
			pr_info("report type:%d, err_no:%d\n", type, err_no); \
		} else { \
			pr_err("power dsm client is busy\n"); \
		} \
	} \
} while (0)
#endif /* CONFIG_HUAWEI_OHOS_DSM */

void power_dsm_reset_dump_enable(struct power_dsm_dump *data, unsigned int size);
void power_dsm_dump_data(struct power_dsm_dump *data, unsigned int size);
void power_dsm_dump_data_with_error_no(struct power_dsm_dump *data,
	unsigned int size, int err_no);
#else
static inline struct dsm_client *power_dsm_get_dclient(unsigned int type)
{
	return NULL;
}

static inline int power_dsm_report_dmd(unsigned int type,
	int err_no, const char *buf)
{
	return 0;
}

static inline int power_dsm_report_fmd(unsigned int type,
	int err_no, const void *msg)
{
	return 0;
}

#ifndef CONFIG_HUAWEI_OHOS_DSM
#define power_dsm_report_format_dmd(type, err_no, fmt, args...)
#endif /* CONFIG_HUAWEI_OHOS_DSM */

static inline void power_dsm_reset_dump_enable(
	struct power_dsm_dump *data, unsigned int size)
{
}

static inline void power_dsm_dump_data(
	struct power_dsm_dump *data, unsigned int size)
{
}

static inline void power_dsm_dump_data_with_error_no(
	struct power_dsm_dump *data, unsigned int size, int err_no)
{
}
#endif /* CONFIG_HUAWEI_OHOS_DSM || CONFIG_HUAWEI_DSM */

#ifdef CONFIG_HUAWEI_OHOS_DSM
/* allow no more than 4096 events in one domain */
#define POWER_HISYS_DSM_MAX_EVENT_NUM  512
#define POWER_HISYSENVENT_BUF_MAX      2176
#define POWER_HISYSENVENT_DECIMAL      10
#define POWER_HISYSENVENT_PARAM_NUM    5
#define POWER_HISYSENVENT_DOMAIN_NUM   3

struct hisysevent_error_no_map {
	int error_no;
	int level;
	char domain[POWER_DSM_BUF_SIZE_0016];
	char name[POWER_DSM_BUF_SIZE_0032];
	char key[POWER_DSM_BUF_SIZE_0048];
	int running_test_switch;
};

struct power_dsm_err_map {
	int event_num;
	struct list_head map_node;
	struct hisysevent_error_no_map map[POWER_HISYS_DSM_MAX_EVENT_NUM];
};

enum power_dsm_level {
	POWER_DSM_LEVEL_FAULT = 1,
	POWER_DSM_LEVEL_STATISTIC,
	POWER_DSM_LEVEL_SECURITY,
	POERT_DSM_LEVEL_BEHAVIOR,
	POERT_DSM_LEVEL_MAX
};


struct power_dsm_err_map *power_dsm_single_map_register(const char *domain);
int power_dsm_report_format_dmd(unsigned int type, int err_no, const char *fmt, ...);
#endif /* CONFIG_HUAWEI_OHOS_DSM */

#if (defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_LOG_EXCEPTION))
int power_dsm_report_hiview(unsigned int err_no, const char *key, const char *value);
int power_dsm_report_multi_hiview(unsigned int err_no, struct hiview_info *info, unsigned int num);
#else
static inline int power_dsm_report_hiview(unsigned int err_no, const char *key, const char *value)
{
	return 0;
}

static inline int power_dsm_report_multi_hiview(unsigned int err_no,
	struct hiview_info *info, unsigned int num)
{
	return 0;
}
#endif /* CONFIG_HUAWEI_OHOS_DSM || CONFIG_LOG_EXCEPTION */

#endif /* _POWER_DSM_H_ */
