/*
 * wireless_charger.h
 *
 * wireless charger driver
 *
 * Copyright (c) 2017-2020 Huawei Technologies Co., Ltd.
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

#ifndef _WIRELESS_CHARGER_
#define _WIRELESS_CHARGER_

#include <linux/workqueue.h>
#include <linux/notifier.h>
#include <chipset_common/hwpower/protocol/wireless_protocol.h>
#include <chipset_common/hwpower/protocol/wireless_protocol_qi.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_common.h>
#include <chipset_common/hwpower/common_module/power_common_macro.h>
#include <platform_include/basicplatform/linux/mfd/pmic_vbus.h>

#define RX_EN_ENABLE                1
#define RX_EN_DISABLE               0
#define RX_SLEEP_EN_ENABLE          1
#define RX_SLEEP_EN_DISABLE         0

#define RX_IOUT_MID                 500
#define TX_BOOST_VOUT               12000
#define RX_HIGH_VOUT                7000
#define RX_HIGH_VOUT2               12000
#define TX_REG_VOUT                 9000
#define RX_DEFAULT_IOUT             1000
#define CHANNEL_SW_TIME             50
#define CHANNEL_SW_TIME_2           200

#define RX_HIGH_IOUT                750
#define RX_LOW_IOUT                 300
#define RX_EPT_IGNORE_IOUT          500
#define RX_AVG_IOUT_TIME            30000 /* 30s */
#define RX_VRECT_LOW_RESTORE_TIME   10000
#define RX_VRECT_LOW_IOUT_MIN       300
#define RX_VRECT_LOW_CNT            3
#define RX_VOUT_ERR_CHECK_TIME      1000

#define WLRX_ILIM_DELAY             300 /* delay for ibus stability */

#define TX_ID_HW                    0x8866

#define CONTROL_INTERVAL_NORMAL     300
#define CONTROL_INTERVAL_FAST       100
#define WL_DISCONN_DELAY_MS         1600
#define WL_RST_DISCONN_DELAY_MS     3000

#define WIRELESS_STAGE_STR_LEN      32
#define WIRELESS_TMP_STR_LEN        16

#define CERTI_ERR_CNT_MAX           3
/* Qi: if reset more than twice, tx will end power transfer */
#define WLC_RST_CNT_MAX             2

/* rx charge state */
#define WIRELESS_STATE_CHRG_FULL            BIT(0)
#define WIRELESS_STATE_CHRG_DONE            BIT(1)

#define WIRELESS_INT_CNT_TH                 10
#define WIRELESS_INT_TIMEOUT_TH             15000 /* 15 * 1000ms */

#define WIRELESS_CHECK_UNKNOWN              (-1)
#define WIRELESS_CHECK_FAIL                 0
#define WIRELESS_CHECK_SUCC                 1

/* back device color nv info */
#define BACK_DEVICE_COLOR_LEN               16

/* cmd: send charge state */
#define WLC_SEND_CHARGE_STATE_RETRY_CNT     3

#define WLC_FAN_LIMIT_RETRY_CNT             3
#define WLC_FAN_CTRL_PWR                    10000000 /* 10w */

/* cmd: set tx rpp format */
#define WLC_SET_RPP_FORMAT_RETRY_CNT        3
#define WLRX_SET_RPP_FORMAT_RETRY_DELAY     100

enum wireless_mode {
	WIRELESS_RX_MODE = 0,
	WIRELESS_TX_MODE,
};

enum wireless_etp_type {
	WIRELESS_EPT_UNKOWN         = 0x00,
	WIRELESS_EPT_CHRG_COMPLETE  = 0x01,
	WIRELESS_EPT_INTERNAL_FAULT = 0x02,
	WIRELESS_EPT_OTP            = 0x03,
	WIRELESS_EPT_OVP            = 0x04,
	WIRELESS_EPT_OCP            = 0x05,
	WIRELESS_EPT_BATT_FAILURE   = 0x06,
	WIRELESS_EPT_RESERVED       = 0x07,
	WIRELESS_EPT_NO_RESPONSE    = 0x08,
	WIRELESS_EPT_ERR_VRECT      = 0xA0,
	WIRELESS_EPT_ERR_VOUT       = 0xA1,
};

enum tx_cap_info {
	TX_CAP_TYPE = 1,
	TX_CAP_VOUT_MAX,
	TX_CAP_IOUT_MAX,
	TX_CAP_ATTR,
	TX_CAP_TOTAL,
};

enum wlc_fan_type {
	WLC_FAN_BEGIN = 0,
	WLC_FAN_UNKNOWN_SPEED = WLC_FAN_BEGIN,
	WLC_FAN_SPEED_LEVEL1_MAX, /* 1800 */
	WLC_FAN_SPEED_LEVEL1,
	WLC_FAN_SPEED_LEVEL2_MAX, /* 3600 */
	WLC_FAN_SPEED_LEVEL2,
	WLC_FAN_SPEED_LEVEL3_MAX, /* 4000 */
	WLC_FAN_SPEED_LEVEL3,
	WLC_FAN_SPEED_LEVEL4_MAX, /* 4200 */
	WLC_FAN_SPEED_LEVEL4,
	WLC_FAN_SPEED_LEVEL5_MAX, /* 4400 */
	WLC_FAN_SPEED_LEVEL5,
	WLC_FAN_END,
};

struct wlc_state_record {
	u8 fan_cur;
	u8 fan_last;
	u8 chrg_state_cur;
	u8 chrg_state_last;
};

struct wlc_rx_evt {
	int type;
	struct wireless_protocol_tx_alarm tx_alarm;
};

struct wlrx_pctrl;

struct wlrx_dev_info {
	struct device *dev;
	struct notifier_block rx_event_nb;
	struct notifier_block chrg_event_nb;
	struct notifier_block pwrkey_nb;
	struct notifier_block power_hall_nb;
	struct notifier_block pmic_vbus_nb;
	struct work_struct wired_vbus_connect_work;
	struct work_struct wired_vbus_disconnect_work;
	struct work_struct wireless_chip_en_work;
	struct work_struct wireless_chip_dis_work;
	struct delayed_work rx_event_work;
	struct work_struct wireless_pwroff_reset_work;
	struct delayed_work wireless_vbus_disconnect_work;
	struct delayed_work wireless_ctrl_work;
	struct delayed_work wireless_monitor_work;
	struct delayed_work wireless_watchdog_work;
	struct delayed_work pmic_vbus_work;
	struct delayed_work wired_5votg_restore_work;
	struct workqueue_struct *wireless_kick_wtd_wq;
	struct wlc_state_record stat_rcd;
	struct wlc_rx_evt rx_evt;
	unsigned int drv_type;
	int supported_rx_vout;
	int ctrl_interval;
	int monitor_interval;
	int certi_comm_err_cnt;
	int wlc_err_rst_cnt;
	int rx_iout_limit;
	int iout_high_cnt;
	int iout_low_cnt;
	int cable_detect_succ_flag;
	int curr_tx_type_index;
	int extra_pwr_good_flag;
	enum vbus_irq_type pmic_vbus_state;
};

enum wltx_stage wireless_tx_get_stage(void);
void wireless_charge_wired_vbus_connect_handler(void);
void wireless_charge_wired_vbus_disconnect_handler(void);
void direct_charger_disconnect_event(void);
int wireless_charge_get_rx_iout_limit(void);
void wireless_charger_pmic_vbus_handler(bool vbus_state);

void wireless_charge_restart_charging(unsigned int stage_from);
void wireless_charge_update_max_vout_and_iout(bool ignore_cnt_flag);

void wlc_ignore_vbus_only_event(bool ignore_flag);

#ifdef CONFIG_WIRELESS_CHARGER
void wlc_reset_wireless_charge(void);
#else
static inline void wlc_reset_wireless_charge(void)
{
}

#endif /* CONFIG_WIRELESS_CHARGER */

#endif /* _WIRELESS_CHARGER_ */
