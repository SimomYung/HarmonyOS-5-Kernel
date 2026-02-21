/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 *
 * scharger_v800_buck.h
 *
 * SchargerV800 buck header.
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
#ifndef __SCHARGER_V800_BUCK_H__
#define __SCHARGER_V800_BUCK_H__

#include <linux/workqueue.h>
#include <linux/platform_device.h>
#include <pmic_interface.h>
#include "scharger_v800_regs.h"
#include <chipset_common/hwpower/hardware_channel/power_sw.h>

#define scharger_err(fmt, args...)      pr_err("[schargerV800]" fmt, ## args)
#define scharger_evt(fmt, args...)      pr_warn("[schargerV800]" fmt, ## args)
#define scharger_inf(fmt, args...)      pr_info("[schargerV800]" fmt, ## args)
#define scharger_dbg(fmt, args...)      pr_debug("[schargerV800]" fmt, ## args)

#define CHG_ENABLE                      1
#define CHG_DISABLE                     0
#define CHARGE_IC_GOOD                  0
#define CHARGE_IC_BAD                   1
#define CHG_STAT_ENABLE                 1
#define CHG_STAT_DISABLE                0
#define BATTERY_H                       1
#define BATTERY_L                       0
#define CHG_BATFET_DIS                  0
#define CHG_BATFET_EN                   1
#define CHARGE_OTG_DISABLE              0
#define CHG_TERM_EN                     1
#define CHG_TERM_DIS                    0

#define ILIMIT_RBOOST_CNT               4
#define WEAKSOURCE_FLAG_REG             PMIC_HRST_REG12_ADDR(0)
#define WAEKSOURCE_FLAG                 BIT(2)

#define CHG_STATE_NOT_PG                (0x02)
#define CHG_STATE_CHRG_DONE             (0x10)
#define CHG_STATE_WDT_FAULT             (0x04)

#define BAT_COMP_DEFAULT_VAL            80
#define VCLAMP_DEFAULT_VAL              224

#define CHG_VBATT_SOFT_OVP_CNT          3
#define CHG_VBATT_SOFT_OVP_MAX          4600
#define chg_vbatt_cv_103(x)             ((x) * 103 / 100)

#define VBAT_3000MV                     3000
#define VBUS_6000MV                     6000
#define VBUS_8100MV                     8100
#define ADJ_VOL_DELAY                   100

#define CV_TRIM_PROTECT                 52
#define CV_TRIM_MAX                     45

#define REVERBST_RETRY                  300

#define CHG_IRQ0                        0
#define CHG_IRQ1                        1
#define CHG_IRQ2                        2
#define CHG_IRQ3                        3
#define CHG_IRQ4                        4
#define CHG_IRQ5                        5
#define FAULT_CHG_DONE                  0x08
#define FAULT_OTG_SCP                   0x04
#define FAULT_REVERSBST                 0x01
#define FAULT_BUCK_VBAT_OVP             0x08
#define FAULT_TDIE_OTP                  0x10
#define FAULT_CHG_VBAT_LV               0x02
#define FAULT_VBUS_OVP                  0x20
#define FAULT_OTG_OCP                   0x01
#define FAULT_CHG_FAULT                 0x01
#define FAULT_RECHG                     0x04
#define FAULT_VUSB_OVP                  0x08
#define FAULT_REGN_OCP                  0x20
#define FAULT_VPSW_OVP                  0x40
#define FAULT_TBAT_OTP                  0x40
#define FAULT_WDT_TOT                   0x80
#define FAULT_GATE_VDROP_OVP            0x01
#define IRQ_PSW_PLUGOUT                 0x02
#define IRQ_OVP_VUSB_OVP                0x08
#define IRQ_OVP_VPSW_OVP                0x40
#define IRQ_VSYS_H_SCP                  0x80
#define IRQ_VSYS_H_OVP                  0x40
#define IRQ_BUCK_OCP                    0x02

#define SC_IRQ0                         0
#define SC_IRQ1                         1
#define SC_IRQ2                         2
#define FAULT_IBUS_OCP                  0x80
#define FAULT_VDROP_OVP                 0x01
#define FAULT_IBAT_OCP                  0x02
#define FAULT_REVERSE_OCP               0x20
#define FAULT_VBAT_OVP                  0x10

#define FAULT_OVP_VDROP_MIN             0x04
#define FAULT_PSW_VDROP_MIN             0x02
#define FAULT_IBUS_UCP                  0x40
#define FAULT_VOUT_OVP                  0x10
#define FAULT_VOUT_UVLO                 0x08
#define FAULT_CFLY1_SCP                 0x01
#define FAULT_CFLY2_SCP                 0x80
#define FAULT_CFLY3_SCP                 0x40
#define FAULT_SC1_Q4_OCP_PEAK           0x20
#define FAULT_SC1_Q5_OCP_PEAK           0x10
#define FAULT_SC1_Q10_OCP_PEAK          0x08
#define FAULT_SC2_Q4_OCP_PEAK           0x04
#define FAULT_SC2_Q5_OCP_PEAK           0x02
#define FAULT_SC2_Q10_OCP_PEAK          0x01

#define FLASH_IRQ0                      0
#define FLASH_IRQ1                      1
#define IRQ_LED1_SCP                    0x40
#define IRQ_LED1_OPEN                   0x20
#define IRQ_LED2_SCP                    0x10
#define IRQ_LED2_OPEN                   0x08

#define reg_cfg(r, m, s, v)  { .reg = (r), .mask = (m), .shift = (s), .val = (v),}

#define SCHARGER_DIEID_LENGTH           8
#define SCHARGER_DIEID_STRING_LENGTH    34
#define SCHARGER_DIEID_BYTE7            63
#define SCHARGER_DIEID_BYTE6            62
#define SCHARGER_DIEID_BYTE5            61
#define SCHARGER_DIEID_BYTE4            60
#define SCHARGER_DIEID_BYTE3            59
#define SCHARGER_DIEID_BYTE2            58
#define SCHARGER_DIEID_BYTE1            57
#define SCHARGER_DIEID_BYTE0            56
#define SCHARGER_ADC_OFFSET_EFUSE_ADDR  52

#define BUCK_INIT_5V_WORK_FIRST_DELAY   200
#define BUCK_INIT_5V_WORK_DELAY         50
#define BUCK_INIT_9V_WORK_DELAY         50

#define BUCK_IRQ_USB_PSW_OVP_WOEK_DELAY 100

#define VBUS_CHECK_CNT                  3
#define OTG_PORT_VOL_DETECT_CNT         3

#define BUCK_OCP_TRIM_WORK_DELAY        60000
#define EFUSE_ADDR_43                   43
#define EFUSE_BUCK_OCP_SHIFT            3
#define EFUSE_BUCK_OCP_MSK              (0xf << EFUSE_BUCK_OCP_SHIFT)
#define EFUSE_BUCK_OCP_TRIM_VAL         0xf

#define FLASH_BUCK5V_IACL_COEFFICIENT   8
#define FLASH_BUCK9V_IACL_COEFFICIENT   4
#define CHG_FLASH_ILIMIT_2200           2200
#define CHG_FLASH_ILIMIT_1200           1200

struct param {
	unsigned int bat_comp;
	unsigned int vclamp;
	unsigned int r_charger_mode;
	unsigned int r_charger_uohm;
	unsigned int auto_shipmode_en;
	unsigned int auto_shipmode_thre;
	unsigned int default_vterm;
};

enum charger_mode {
	UNKNOWN = 0,
	BUCK5V,
	BUCK9V,
	SC11,
	SC21,
	SC41,
	RSC11,
	RSC12,
	RSC14,
	OTG5V,
	OTG9V,
	WDRC12
};

enum cable {
	WIRED,
	WIRELSEE
};

struct current_charger_mode {
	enum charger_mode chg_mode;
	enum cable cable_flag;
};

enum chg_mode_state {
	NONE = 0,
	BUCK,
	LVC,
	SC,
	SC4,
	OTG_MODE,
	SOH_MODE,
	REV_SC,
	REV_SC4
};

enum otg_mode {
	OTG_DISABLE = 0,
	OTG_5V,
	OTG_9V,
};

struct scharger_buck {
	struct device *dev;
	struct regmap *regmap;
	struct param param_dts;
	unsigned int term_vol_mv;
	unsigned int buck_vbus_set;
	int input_current;
	int charge_current;
	unsigned int batt_ovp_cnt_30s;
	unsigned int is_board_type;
	int chg_irq;
	int sc_irq;
	struct work_struct irq_work;
	struct mutex mode_lock;
	struct mutex efuse_lock;
	struct wakeup_source *efuse_write_wake_lock;
	spinlock_t efuse_write_lock;

	enum chg_mode_state chg_mode;
	unsigned int scharger_version;
	unsigned int chip_id;
	int reverbst_cnt;
	struct delayed_work reverbst_work;
	struct delayed_work buck_init_5v_work;
	struct delayed_work buck_init_9v_work;
	struct delayed_work buck_irq_usb_ovp_work;
	struct delayed_work buck_irq_psw_ovp_work;
	struct delayed_work buck_ocp_trim_work;
	struct notifier_block usb_nb;
	struct power_sw_attr usbovp_sw_attr;
	struct power_sw_attr pswovp_sw_attr;
	struct power_sw_attr usbovp_sw_flag_attr;
	struct power_sw_attr pswovp_sw_flag_attr;
	struct current_charger_mode current_charge_mode;
	bool dc_charging;
};

struct opt_regs {
	u16 reg;
	u8 mask;
	u8 shift;
	u32 val;
};

struct cv_info {
	int cv_ori_val;
	int cv_new_set_val;
	int cv_trim_flag;
};

int scharger_v800_chg_irq_init(struct scharger_buck *buck);
int scharger_v800_sc_irq_init(struct scharger_buck *buck);
int scharger_v800_set_charge_enable(int enable);
int scharger_v800_set_buck_mode_enable(u32 enable);
int scharger_v800_set_term_enable(int enable);
void scharger_v800_irq_work(struct work_struct *work);
int scharger_v800_get_register_head(char *buf, int size, void *dev_data);
int scharger_v800_dump_register(char *buf, int size, void *dev_data);
int scharger_v800_set_sc_mode(enum scharger_v800_sc_mode mode);
int scharger_v800_set_usb_ovp_mode(enum scharger_v800_usb_ovp_mode mode);
int scharger_v800_set_psw_ovp_mode(enum scharger_v800_psw_ovp_mode mode);
int scharger_v800_dc_switch_to_buck_mode(void);
void scharger_v800_set_boot_weaksource_flag(void);
void scharger_v800_set_anti_reverbst_reset(struct scharger_buck *buck);
int scharger_v800_set_vpsw_ovp(enum charger_mode mode);
int scharger_v800_set_vusb_ovp(enum charger_mode mode);
int scharger_set_vusb_vpsw_ovp(enum charger_mode mode);
unsigned int scharger_v800_device_version(void);
void scharger_v800_vbat_lv_handle(struct scharger_buck *buck);
unsigned int scharger_v800_get_sc_mode(void);
int scharger_check_input_dpm_state(void);
int scharger_v800_gate1_enable(bool enable);
int scharger_efuse_read(u8 addr, u8 *value);
void scharger_v800_mode_lock(void);
void scharger_v800_mode_unlock(void);
struct current_charger_mode* scharger_v800_get_current_charge_mode(void);
int is_current_charge_mode(enum charger_mode mode);
int scharger_v800_set_current_charge_mode(enum charger_mode mode);
void scharger_v800_set_dc_charging(bool dc_charging);
int buck_init(u32 value);
int scharger_get_vusb_vpsw(void);
int scharger_set_q1q1_ss_buck_mode(enum q1q1_ss_buck_mode mode);
unsigned int scharger_v800_get_usb_ovp_mode(void);
unsigned int scharger_v800_get_psw_ovp_mode(void);
void scharger_v800_buck_irq_usb_ovp_work(struct work_struct *work);
void scharger_v800_buck_irq_psw_ovp_work(struct work_struct *work);
int __set_input_current(int cin_limit);
bool scharger_chip_version_is_v80x(void);
int scharger_get_flash_limit_input_current(void);
int scharger_flash_restore_input_current(void);
#endif