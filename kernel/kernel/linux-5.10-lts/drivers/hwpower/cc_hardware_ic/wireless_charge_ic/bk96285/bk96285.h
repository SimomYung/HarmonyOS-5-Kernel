/* SPDX-License-Identifier: GPL-2.0 */
/*
 * bk96285.h
 *
 * bk96285 macro, addr etc.
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

#ifndef _BK96285_H_
#define _BK96285_H_

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/workqueue.h>
#include <linux/bitops.h>
#include <linux/jiffies.h>
#include <chipset_common/hwpower/common_module/power_delay.h>
#include <chipset_common/hwpower/hardware_ic/boost_5v.h>
#include <chipset_common/hwpower/hardware_ic/charge_pump.h>
#include <chipset_common/hwpower/hardware_ic/wireless_ic_fod.h>
#include <chipset_common/hwpower/hardware_ic/wireless_ic_iout.h>
#include <chipset_common/hwpower/hardware_ic/wireless_ic_debug.h>
#include <chipset_common/hwpower/wireless_charge/wireless_firmware.h>
#include <chipset_common/hwpower/wireless_charge/wireless_trx_intf.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_ic_intf.h>
#include <chipset_common/hwpower/wireless_charge/wireless_tx_ic_intf.h>
#include <chipset_common/hwpower/wireless_charge/wireless_power_supply.h>
#include <chipset_common/hwpower/wireless_charge/wireless_rx_status.h>
#include <huawei_platform/power/wireless/wireless_charger.h>
#include <huawei_platform/power/wireless/wireless_direct_charger.h>
#include <huawei_platform/power/wireless/wireless_transmitter.h>
#include <chipset_common/hwpower/wireless_charge/wireless_test_hw.h>
#include <chipset_common/hwpower/hardware_channel/wired_channel_switch.h>
#include <chipset_common/hwpower/common_module/power_gpio.h>
#include <chipset_common/hwpower/common_module/power_i2c.h>
#include <chipset_common/hwpower/common_module/power_printk.h>
#include <chipset_common/hwpower/common_module/power_common_macro.h>
#include <chipset_common/hwpower/common_module/power_time.h>
#include <chipset_common/hwpower/common_module/power_wakeup.h>
#include <chipset_common/hwpower/common_module/power_cmdline.h>
#include <chipset_common/hwpower/common_module/power_devices_info.h>
#include <chipset_common/hwpower/common_module/power_pinctrl.h>
#include "bk96285_chip.h"

/* for coil test */
#define BK96285_COIL_TEST_PING_INTERVAL       0
#define BK96285_COIL_TEST_PING_FREQ           115
/* for battery heating */
#define BK96285_BAT_HEATING_PING_INTERVAL     1
#define BK96285_BAT_HEATING_PING_FREQ         100

struct bk96285_vfc_map {
	int vpa;
	int vmldo;
};

/* for rx dummy iload */
enum bk96285_dummy_iload {
	BK96285_DUMMY_ILOAD_BEGIN = 0,
	BK96285_DUMMY_ILOAD_5V_NO_MOD = BK96285_DUMMY_ILOAD_BEGIN,
	BK96285_DUMMY_ILOAD_5V_MOD,
	BK96285_DUMMY_ILOAD_9V_NO_MOD,
	BK96285_DUMMY_ILOAD_9V_MOD,
	BK96285_DUMMY_ILOAD_SC_NO_MOD,
	BK96285_DUMMY_ILOAD_SC_MOD,
	BK96285_DUMMY_ILOAD_FODCHK_NO_MOD,
	BK96285_DUMMY_ILOAD_FODCHK_MOD,
	BK96285_DUMMY_ILOAD_12V_NO_MOD,
	BK96285_DUMMY_ILOAD_12V_MOD,
	BK96285_DUMMY_ILOAD_END,
};

struct bk96285_chip_info {
	u32 chip_id;
	u16 mtp_ver;
};

struct bk96285_fw_info {
	u16 mtp_ver;
	u16 mtp_crc;
	u16 mtp_len;
};

struct bk96285_mtp_check_delay {
	u32 user;
	u32 fac;
};

struct bk96285_rx_cm_intfr_cfg {
	u8 type;
	u8 cm;
	u8 polar;
};

struct bk96285_global_val {
	bool mtp_latest;
	bool mtp_chk_complete;
	bool irq_abnormal;
	bool rx_stop_chrg;
	bool tx_stop_chrg;
	bool tx_open_flag;
	bool vfc_set_complete;
	struct hwqi_handle *qi_hdl;
};

struct bk96285_rx_cm_cfg {
	u8 l_volt;
	u8 h_volt;
	u8 fac_h_volt;
};

struct bk96285_rx_polar_cfg {
	u8 l_volt;
	u8 h_volt;
	u8 fac_h_volt;
};

struct bk96285_rx_mod_cfg {
	struct bk96285_rx_cm_cfg cm;
	struct bk96285_rx_polar_cfg polar;
	bool ctrl_by_ap;
};

struct bk96285_rx_ldo_cfg {
	u8 l_volt[BK96285_RX_LDO_CFG_LEN]; /* 5V buck */
	u8 m_volt[BK96285_RX_LDO_CFG_LEN]; /* 9V buck */
	u8 sc[BK96285_RX_LDO_CFG_LEN];
};

struct bk96285_tx_init_para {
	u16 ping_interval;
	u16 ping_freq;
	u16 ping_duty;
	u32 ping_ocp_th;
	u32 ocp_th;
};

struct bk96285_tx_fod_para {
	u32 ploss_th0;
	u32 ploss_th1;
	u32 ploss_th2;
	u32 ploss_th3;
	u32 hp_ploss_th0;
	u32 hp_ploss_th1;
	u32 hp_ploss_th2;
	u32 hp_cur_th0;
	u32 hp_cur_th1;
	u32 ploss_cnt;
	u32 q_en;
	u32 q_coil_th;
	u32 q_th;
};

struct bk96285_rx_ldo_opp_vbst {
	u32 vbst;
	u32 iout_lth;
	u32 iout_hth;
};

struct bk96285_tx_debug_param {
	bool print_log;
};

struct bk96285_dev_info {
	struct i2c_client *client;
	struct device *dev;
	struct mutex mutex_irq;
	struct wakeup_source *wakelock;
	struct work_struct irq_work;
	struct delayed_work mtp_check_work;
	struct bk96285_fw_info fw_mtp;
	struct bk96285_global_val g_val;
	struct bk96285_rx_mod_cfg rx_mod_cfg;
	struct bk96285_rx_cm_intfr_cfg *cm_intfr_cfg;
	struct bk96285_rx_ldo_cfg rx_ldo_cfg;
	struct bk96285_tx_init_para tx_init_para;
	struct bk96285_tx_fod_para tx_fod;
	struct bk96285_mtp_check_delay mtp_check_delay;
	struct bk96285_rx_ldo_opp_vbst ldo_opp_vbst;
	struct wakeup_source *fw_wakelock;
	struct bk96285_tx_debug_param dbg_param;
	u32 dummy_iload[BK96285_DUMMY_ILOAD_END];
	u32 product_type;
	bool is_scx_mode;
	bool fw_program;
	unsigned int ic_type;
	int rx_ss_good_lth;
	int otp_th;
	int duty_max;
	int duty_min;
	int gpio_en;
	int gpio_en_valid_val;
	int gpio_sleep_en;
	int gpio_int;
	int gpio_pwr_good;
	int irq_int;
	int irq_cnt;
	u32 irq_val;
	u32 tx_ept_type;
	bool need_ignore_irq;
	bool irq_active;
	bool irq_awake;
	u32 tx_ping_freq;
	u32 tx_ping_duty;
	u32 ping_ocp_th;
	u32 tx_pt_ocp_th;
	u32 rx_vfc_diff;
	u32 rx_fod_offset;
	u32 fodchk_ask_cap;
	u32 coil_test_ping_freq;
	int cali_a;
	int cali_b;
	u32 chip_id;
	u32 tx_fod_fmt;
	u32 rx_ss_low_thr;
	int sysfs_cm_type;
	int cm_intfr_cfg_level;
	bool fc_succ;
	bool can_boost;
	bool ldo_on;
	int coil_test_ping_duty;
	int need_ctrl_brg;
	u32 prevfod_gain_unit;
	u32 rx_ldo_opp_en;
	int full_bridge_ith;
	int dummy_len;
	int tx_fodchk_duty_max;
	int tx_fodchk_duty_min;
};

/* bk96285 i2c */
int bk96285_read_byte(struct bk96285_dev_info *di, u16 reg, u8 *data);
int bk96285_write_byte(struct bk96285_dev_info *di, u16 reg, u8 data);
int bk96285_read_byte_mask(struct bk96285_dev_info *di, u16 reg, u8 mask, u8 shift, u8 *data);
int bk96285_write_byte_mask(struct bk96285_dev_info *di, u16 reg, u8 mask, u8 shift, u8 data);
int bk96285_read_word(struct bk96285_dev_info *di, u16 reg, u16 *data);
int bk96285_write_word(struct bk96285_dev_info *di, u16 reg, u16 data);
int bk96285_write_word_mask(struct bk96285_dev_info *di, u16 reg, u16 mask, u16 shift, u16 data);
int bk96285_read_dword(struct bk96285_dev_info *di, u16 reg, u32 *data);
int bk96285_write_dword(struct bk96285_dev_info *di, u16 reg, u32 data);
int bk96285_read_dword_mask(struct bk96285_dev_info *di, u16 reg, u32 mask, u32 shift, u32 *data);
int bk96285_write_dword_mask(struct bk96285_dev_info *di, u16 reg, u32 mask, u32 shift, u32 data);
int bk96285_read_block(struct bk96285_dev_info *di, u16 reg, u8 *data, u8 len);
int bk96285_write_block(struct bk96285_dev_info *di, u16 reg, u8 *data, u8 data_len);

/* bk96285 common */
int bk96285_get_chip_info(struct bk96285_dev_info *di, struct bk96285_chip_info *info);
int bk96285_get_chip_info_str(char *info_str, int len, void *dev_data);
bool bk96285_is_pwr_good(struct bk96285_dev_info *di);
bool bk96285_rx_is_tx_exist(void *dev_data);
void bk96285_enable_irq(struct bk96285_dev_info *di);
void bk96285_disable_irq_nosync(struct bk96285_dev_info *di);
void bk96285_rx_abnormal_irq_handler(struct bk96285_dev_info *di);
void bk96285_rx_mode_irq_handler(struct bk96285_dev_info *di);
void bk96285_enable_irq_wake(struct bk96285_dev_info *di);
void bk96285_disable_irq_wake(struct bk96285_dev_info *di);
bool bk96285_rx_is_tx_exist(void *dev_data);
void bk96285_rx_shutdown_handler(struct bk96285_dev_info *di);
void bk96285_chip_enable(bool enable, void *dev_data);
bool bk96285_is_chip_enable(void *dev_data);
int bk96285_get_mode(struct bk96285_dev_info *di, u32 *mode);
int bk96285_fw_sram_update(void *dev_data);
int bk96285_read_register(struct bk96285_dev_info *di, u16 reg, u8 *pData, u8 len);
int bk96285_write_register(struct bk96285_dev_info *di, u16 reg, u8 *pData, u8 len);
struct device_node *bk96285_dts_dev_node(void *dev_data);
int bk96285_set_product_type(struct bk96285_dev_info *di, u32 product_type);

/* bk96285 dts */
int bk96285_parse_dts(struct device_node *np, struct bk96285_dev_info *di);

/* bk96285 fw */
void bk96285_fw_mtp_check(struct bk96285_dev_info *di);
int bk96285_fw_ops_register(struct wltrx_ic_ops *ops, struct bk96285_dev_info *di);
int bk96285_fw_get_mtp_ver(struct bk96285_dev_info *di, u16 *mtp_ver);

/* bk96285 rx */
int bk96285_rx_ops_register(struct wltrx_ic_ops *ops, struct bk96285_dev_info *di);

/* bk96285 qi */
int bk96285_qi_ops_register(struct wltrx_ic_ops *ops, struct bk96285_dev_info *di);

/* bk96285 hw_test */
int bk96285_hw_test_ops_register(struct bk96285_dev_info *di);

/* bk96285 tx */
int bk96285_tx_ops_register(struct wltrx_ic_ops *ops, struct bk96285_dev_info *di);
void bk96285_tx_mode_irq_handler(struct bk96285_dev_info *di);
#endif /* _BK96285_H_ */
