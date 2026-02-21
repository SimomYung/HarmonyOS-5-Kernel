/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 *
 * scharger_v800_regs.h
 *
 * SchargerV800 register config header.
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
#ifndef __SCHARGER_V800_REGS_H__
#define __SCHARGER_V800_REGS_H__

#include <soc_schargerV800_interface.h>

enum scharger_v800_sc_mode {
	CHG_BUCK_MODE = 0,
	CHG_OTG_MODE,
	CHG_FBPS_MODE,
	CHG_F21SC_MODE,
	CHG_F41SC_MODE,
	CHG_RBPS_MODE,
	CHG_R12SC_MODE,
	CHG_R14SC_MODE
};

enum scharger_v800_usb_ovp_mode {
	USBOVP_FORWARD_MODE = 0,
	USBOVP_FORCE_OPEN,
	USBOVP_FORCE_CLOSE,
	USBOVP_REVERSE_MODE
};

enum scharger_v800_psw_ovp_mode {
	PSWOVP_FORWARD_MODE = 0,
	PSWOVP_FORCE_OPEN,
	PSWOVP_FORCE_CLOSE,
	PSWOVP_REVERSE_MODE
};

enum q1q1_ss_buck_mode {
	AUTO_MODE = 0,
	FORCE_OPEN,
};

enum scharger_v800_chip_id {
	CHIP_ID_V800 = 0x8000,
	CHIP_ID_V801 = 0x0008,
	CHIP_ID_V802 = 0x0208,
	CHIP_ID_V803 = 0x0308,
	CHIP_ID_V810 = 0x1008,
};

#define CHG_ILIMIT_85                   85
#define CHG_ILIMIT_475                  475
#define CHG_ILIMIT_600                  600
#define CHG_ILIMIT_825                  825
#define CHG_ILIMIT_1000                 1000
#define CHG_ILIMIT_1100                 1100
#define CHG_ILIMIT_1200                 1200
#define CHG_ILIMIT_1400                 1400
#define CHG_ILIMIT_1500                 1500
#define CHG_ILIMIT_2500                 2500
#define CHG_LMT_NUM_V800                32

#define CHG_FAST_ICHG_100MA             100
#define CHG_FAST_ICHG_500MA             500
#define CHG_FAST_ICHG_2500MA            2500
#define CHG_FAST_ICHG_STEP_100          100
#define CHG_FAST_ICHG_00MA              0

#define VBUS_VSET_5V                    5
#define VBUS_VSET_9V                    9
#define VBUS_VSET_12V                   12

#define CHG_PRG_ICHG_200MA              200
#define CHG_PRG_ICHG_300MA              300
#define CHG_PRG_ICHG_400MA              400
#define CHG_PRG_ICHG_800MA              800
#define CHG_PRG_ICHG_MIN                CHG_PRG_ICHG_200MA
#define CHG_PRG_ICHG_MAX                CHG_PRG_ICHG_800MA

#define CHG_PRG_VCHG_2500               2500
#define CHG_PRG_VCHG_2800               2800
#define CHG_PRG_VCHG_3000               3000
#define CHG_PRG_VCHG_3200               3200

#define BOOST_LIM_500                   500
#define BOOST_LIM_1000                  1000
#define BOOST_LIM_1500                  1500
#define BOOST_LIM_2000                  2000
#define BOOST_LIM_MIN                   BOOST_LIM_500
#define BOOST_LIM_MAX                   BOOST_LIM_2000

#define R_UOHM_500                      500
#define R_UOHM_1000                     1000
#define R_UOHM_2000                     2000
#define R_UOHM_DEFAULT                  R_UOHM_1000 /* resisitance uohm */

/* BUCK/OTG mode only can be configured 6000/11000mv */
#define USB_OVP_6000MV                  6000
#define USB_OVP_6500MV                  6500
#define USB_OVP_7500MV                  7500
#define USB_OVP_11000MV                 11000
#define USB_OVP_13000MV                 13000
#define USB_OVP_14000MV                 14000
#define USB_OVP_20000MV                 20000
#define USB_OVP_23000MV                 23000
#define USB_OVP_24000MV                 24000

/* BUCK/OTG mode only can be configured 6000/11000mv */
#define PSW_OVP_6000MV                  6000
#define PSW_OVP_6500MV                  6500
#define PSW_OVP_7500MV                  7500
#define PSW_OVP_11000MV                 11000
#define PSW_OVP_13000MV                 13000
#define PSW_OVP_14000MV                 14000
#define PSW_OVP_20000MV                 20000
#define PSW_OVP_23000MV                 23000
#define PSW_OVP_24000MV                 24000

#define CHG_FASTCHG_TIMER_20H           3
#define CHG_RECHG_150                   0x00

#define ACL_RPT_EN                      1

#define CHG_FAST_VCHG_4430              4430

#define CHG_VBAT_LV_REG                 REG_VBAT_LV_REG_ADDR(REG_ANA_BASE)
#define CHG_VBAT_LV_SHIFT               REG_VBAT_LV_REG_vbat_lv_START
#define CHG_VBAT_LV_MSK                 (0x01 << CHG_VBAT_LV_SHIFT)
#define VBAT_LV_SET1                    1

#define CHG_CHARGER_CFG_REG_11_REG      REG_CHARGER_CFG_REG_11_ADDR(REG_ANA_BASE)
#define CHG_FAST_VCHG_SHIFT             REG_CHARGER_CFG_REG_11_da_chg_resvo_START
#define CHG_FAST_VCHG_MSK               (0x3f << CHG_FAST_VCHG_SHIFT)
#define CHG_FAST_VCHG_MIN               4152
#define CHG_FAST_VCHG_MAX               4656
#define CHG_FAST_VCHG_BASE_UV           (4152 * 1000)
#define CHG_FAST_VCHG_4100MV_SHIFT      6
#define CHG_FAST_VCHG_4100MV_MSK        (0x1 << CHG_FAST_VCHG_4100MV_SHIFT)
#define CHG_FAST_VCHG_STEP_8000UV       8000
#define UV_PER_MV                       1000
#define CHG_FAST_VCHG_4400              4400
#define CHG_CHARGER_CFG_REG_2_REG       REG_CHARGER_CFG_REG_2_ADDR(REG_ANA_BASE)
#define CHG_CV_ADJ_SHIFT                REG_CHARGER_CFG_REG_2_da_chg_cv_adj_START
#define CHG_CV_ADJ_MSK                  (0x7 << CHG_CV_ADJ_SHIFT)
#define CHG_EN_TERM_SHIFT               (REG_CHARGER_CFG_REG_2_da_chg_en_term_START)
#define CHG_EN_TERM_MSK                 (0x01 << CHG_EN_TERM_SHIFT)

#define CHG_IBAT_RES_SEL_REG            REG_IBAT_RES_SEL_ADDR(REG_ANA_BASE)
#define CHG_IBAT_RES_SEL_SHIFT          REG_IBAT_RES_SEL_da_bat_res_sel_START
#define CHG_IBAT_RES_SEL_MSK            (0x3 << CHG_IBAT_RES_SEL_SHIFT)

#define CHG_CHG_OTG_MODE_CFG_REG        REG_OTG_MODE_CFG_ADDR(REG_GLB_BASE)
#define CHG_CHG_OTG_MODE_CFG_SHIFT      REG_OTG_MODE_CFG_sc_chg_otg_mode_START
#define CHG_CHG_OTG_MODE_CFG_MSK        (0x3 << CHG_CHG_OTG_MODE_CFG_SHIFT)
#define DA_CHG_OTG_DIS                  (0x00)
#define DA_CHG_EN                       (0x01)
#define DA_OTG_EN                       (0x02)

#define CHG_SYS_DET_CFG_REG_6_REG       REG_SYS_DET_CFG_REG_6_ADDR(REG_ANA_BASE)
#define CHG_VBUS_OVP_SHIFT              REG_SYS_DET_CFG_REG_6_da_vbus_ov_bk_START
#define CHG_VBUS_OVP_MSK                (0x1 << CHG_VBUS_OVP_SHIFT)
#define CHG_VBUS_UVP_SHIFT              REG_SYS_DET_CFG_REG_6_da_vbus_uv_bk_START
#define CHG_VBUS_UVP_MSK                (0x1 << CHG_VBUS_UVP_SHIFT)
#define CHG_VUSB_SLEEP_SHIFT			REG_SYS_DET_CFG_REG_6_da_usb_sleep_block_START
#define CHG_VUSB_SLEEP_MSK				(0x1 << CHG_VUSB_SLEEP_SHIFT)

#define CHG_USB_OVP_CFG_REG_2_REG       REG_USB_OVP_CFG_REG_2_ADDR(REG_ANA_BASE)
#define CHG_USB_OVP_SET_SHIFT           REG_USB_OVP_CFG_REG_2_da_usb_ovp_set_START
#define CHG_USB_OVP_SET_MSK             (0x7 << CHG_USB_OVP_SET_SHIFT)

#define CHG_VBUS_BKVSET_REG             REG_VBUS_BKVSET_ADDR(REG_GLB_BASE)
#define CHG_VBUS_VSET_SHIFT             REG_VBUS_BKVSET_da_vbus_bkvset_START
#define CHG_VBUS_VSET_MSK               (0x1 << CHG_VBUS_VSET_SHIFT)

#define CHG_BUCK_OTG_TOP_CFG_REG_6_REG  REG_BUCK_OTG_TOP_CFG_REG_6_ADDR(REG_ANA_BASE)
#define CHG_DPM_AUTO_SHIFT              REG_BUCK_OTG_TOP_CFG_REG_6_da_buck_dpm_auto_START
#define CHG_DPM_AUTO_MSK                (0x1 << CHG_DPM_AUTO_SHIFT)
#define CHG_BUCK_OTG_TOP_CFG_REG_7_REG  REG_BUCK_OTG_TOP_CFG_REG_7_ADDR(REG_ANA_BASE)
#define CHG_DPM_SEL_SHIFT               REG_BUCK_OTG_TOP_CFG_REG_7_da_buck_dpm_sel_START
#define CHG_DPM_SEL_MSK                 (0xf << CHG_DPM_SEL_SHIFT)
#define CHG_DPM_MODE_AUTO               0x01
#define CHG_DPM_SEL_DEFAULT             0x06

#define CHG_BUCK_OTG_TOP_CFG_REG_1_REG  REG_BUCK_OTG_TOP_CFG_REG_1_ADDR(REG_ANA_BASE)
#define CHG_REVERBST_RST_SHIFT          3
#define CHG_REVERBST_RST_MSK            (0x1 << CHG_REVERBST_RST_SHIFT)
#define REVERBST_DELAY_ON               1000
#define ANTI_REVERBST_EN                1
#define ANTI_REVERBST_DIS               0

#define CHG_SC_BUCK_ENB_REG             REG_SC_BUCK_ENB_ADDR(REG_GLB_BASE)
#define CHG_HIZ_ENABLE_SHIFT            (REG_SC_BUCK_ENB_sc_buck_enb_START)
#define CHG_HIZ_ENABLE_MSK              (0x1 << CHG_HIZ_ENABLE_SHIFT)

#define CHG_CHARGER_CFG_REG_13_REG      REG_CHARGER_CFG_REG_13_ADDR(REG_ANA_BASE)
#define CHG_VRECHG_SHIFT                (REG_CHARGER_CFG_REG_13_da_chg_vrechg_hys_START)
#define CHG_VRECHG_MSK                  (0x03 << CHG_VRECHG_SHIFT)
#define CHG_CHG_VCLAMP_SHIFT            REG_CHARGER_CFG_REG_13_da_chg_vclamp_set_START
#define CHG_CHG_VCLAMP_MSK              (0x7 << CHG_CHG_VCLAMP_SHIFT)
#define CHG_IR_VCLAMP_STEP              50
#define CHG_IR_VCLAMP_MIN               0
#define CHG_IR_VCLAMP_MAX               350

#define CHG_CHARGER_CFG_REG_4_REG       (REG_CHARGER_CFG_REG_4_ADDR(REG_ANA_BASE))
#define CHG_FASTCHG_TIMER_SHIFT         (REG_CHARGER_CFG_REG_4_da_chg_fastchg_timer_START)
#define CHG_FASTCHG_TIMER_MSK           (0x03 << CHG_FASTCHG_TIMER_SHIFT)
#define CHG_FASTCHG_TIMER_20H           3

#define CHG_CHARGER_CFG_REG_5_REG       REG_CHARGER_CFG_REG_5_ADDR(REG_ANA_BASE)
#define CHG_IR_SET_SHIFT                REG_CHARGER_CFG_REG_5_da_chg_ir_set_START
#define CHG_IR_SET_MSK                  (0x0f << CHG_IR_SET_SHIFT)
#define CHG_IR_COMP_0MOHM               0
#define CHG_IR_COMP_150MOHM             150
#define BAT_COMP_MOHM_STEP              10
#define BAT_COMP_MOHM_MIN               CHG_IR_COMP_0MOHM
#define BAT_COMP_MOHM_MAX               CHG_IR_COMP_150MOHM

#define CHG_CHARGER_RO_REG_0_REG        REG_CHARGER_RO_REG_0_ADDR(REG_ANA_BASE)
#define CHG_IN_DPM_STATE_SHIFT          REG_CHARGER_RO_REG_0_ad_chg_dpm_state_START
#define CHG_IN_DPM_STATE_MSK            (0x1 << CHG_IN_DPM_STATE_SHIFT)
#define CHG_CHARGER_CFG_REG_0_REG       REG_CHARGER_CFG_REG_0_ADDR(REG_ANA_BASE)
#define CHG_ACL_RPT_EN_SHIFT            REG_CHARGER_CFG_REG_0_da_chg_acl_rpt_en_START
#define CHG_ACL_PRT_EN_MSK              (0x01 << CHG_ACL_RPT_EN_SHIFT)
#define CHG_IN_ACL_STATE_SHIFT          REG_CHARGER_RO_REG_0_ad_chg_acl_state_START
#define CHG_IN_ACL_STATE_MSK            (0x01 << CHG_IN_ACL_STATE_SHIFT)

#define CHG_CHARGER_RO_REG_1_REG        REG_CHARGER_RO_REG_1_ADDR(REG_ANA_BASE)
#define CHG_IN_THERM_STATE_SHIFT        REG_CHARGER_RO_REG_1_ad_chg_therm_state_START
#define CHG_IN_THERM_STATE_MSK          (0x1 << CHG_IN_THERM_STATE_SHIFT)

#define CHG_BUCK_ILIMIT_REG             REG_BUCK_ILIMIT_ADDR(REG_ANA_BASE)
#define CHG_ILIMIT_SHIFT                REG_BUCK_ILIMIT_da_buck_ilimit_START
#define CHG_ILIMIT_MSK                  (0x1f << CHG_ILIMIT_SHIFT)

#define CHG_CHARGER_CFG_REG_3_REG       REG_CHARGER_CFG_REG_3_ADDR(REG_ANA_BASE)
#define CHG_FAST_ICHG_SHIFT             (REG_CHARGER_CFG_REG_3_da_chg_fast_ichg_START)
#define CHG_FAST_ICHG_MSK               (0x1f << CHG_FAST_ICHG_SHIFT)

#define CHG_CHARGER_CFG_REG_12_REG      REG_CHARGER_CFG_REG_12_ADDR(REG_ANA_BASE)
#define CHG_TERM_ICHG_SHIFT             (REG_CHARGER_CFG_REG_12_da_chg_term_ichg_START)
#define CHG_TERM_ICHG_MSK               (0x03 << CHG_TERM_ICHG_SHIFT)
#define CHG_TERM_ICHG_200MA             200
#define CHG_TERM_ICHG_400MA             400
#define CHG_TERM_ICHG_800MA             800
#define CHG_TERM_ICHG_1200MA            1200

#define CHG_CHARGER_CFG_REG_7_REG       REG_CHARGER_CFG_REG_7_ADDR(REG_ANA_BASE)
#define CHG_PRE_ICHG_SHIFT              REG_CHARGER_CFG_REG_7_da_chg_pre_ichg_START
#define CHG_PRE_ICHG_MSK                (0x03 << CHG_PRE_ICHG_SHIFT)
#define CHG_PRE_VCHG_SHIFT              REG_CHARGER_CFG_REG_7_da_chg_pre_vchg_START
#define CHG_PRE_VCHG_MSK                (0x03 << CHG_PRE_VCHG_SHIFT)

#define CHG_CHARGER_BATFET_H_CTRL_REG   REG_CHARGER_BATFET_H_CTRL_ADDR(REG_ANA_BASE)
#define CHG_CHARGER_BATFET_H_CTRL_SHIFT REG_CHARGER_BATFET_H_CTRL_batfet_h_ctrl_START
#define CHG_CHARGER_BATFET_H_CTRL_MSK   (1 << CHG_CHARGER_BATFET_H_CTRL_SHIFT)
#define CHG_CHARGER_BATFET_L_CTRL_REG   REG_CHARGER_BATFET_L_CTRL_ADDR(REG_ANA_BASE)
#define CHG_CHARGER_BATFET_L_CTRL_SHIFT REG_CHARGER_BATFET_L_CTRL_batfet_l_ctrl_START
#define CHG_CHARGER_BATFET_L_CTRL_MSK   (1 << CHG_CHARGER_BATFET_L_CTRL_SHIFT)

#define CHG_BUCK_OTG_TOP_CFG_REG_38_REG REG_BUCK_OTG_TOP_CFG_REG_38_ADDR(REG_ANA_BASE)
#define CHG_OTG_SWITCH_SHIFT            REG_BUCK_OTG_TOP_CFG_REG_38_da_otg_switch_START
#define CHG_OTG_SWITCH_MASK             (1 << CHG_OTG_SWITCH_SHIFT)
#define CHG_PSEL_CFG_REG_0_REG          REG_PSEL_CFG_REG_0_ADDR(REG_ANA_BASE)
#define OTG_REGN_FORCE_CLOSE_EN_SHIFT   REG_PSEL_CFG_REG_0_da_psel_force_off_START
#define OTG_REGN_FORCE_CLOSE_EN_MSK     (0x1 << OTG_REGN_FORCE_CLOSE_EN_SHIFT)
#define OTG_REGN_EN_SHIFT               REG_BUCK_OTG_TOP_CFG_REG_38_da_otg_switch_regnoff_START
#define OTG_REGN_EN_MSK                 (0x1 << OTG_REGN_EN_SHIFT)

#define CHG_VERSION0_RO_REG_0_REG       REG_VERSION0_RO_REG_0_ADDR(REG_GLB_BASE)
#define CHG_VERSION4_RO_REG_0_REG       REG_VERSION4_RO_REG_0_ADDR(REG_GLB_BASE)

#define CHG_BUCK_OTG_TOP_RO_REG_45_REG  REG_BUCK_OTG_TOP_RO_REG_45_ADDR(REG_ANA_BASE)
#define CHG_BUCK_OK                     (0x01 << 1)
#define CHG_STAT_CHARGE_DONE            (0x03 << REG_CHARGER_RO_REG_0_ad_chg_chgstate_START)
#define CHG_WDT_TIMEOUT_REG             REG_WDT_TIMEOUT_ADDR(REG_GLB_BASE)
#define WATCHDOG_OK                     (0x01 << REG_WDT_TIMEOUT_wdt_time_out_n_START)

#define CHG_SHIP_MODE_ENB_REG           REG_SHIP_MODE_ENB_ADDR(REG_ANA_BASE)
#define CHG_SHIP_MODE_ENB_SHIFT         REG_SHIP_MODE_ENB_ship_mode_enb_START
#define CHG_SHIP_MODE_ENB_MSK           (0x1 << CHG_SHIP_MODE_ENB_SHIFT)

#define CHG_IRQ_FLAG_REG                REG_IRQ_FLAG_ADDR(REG_IRQ_BASE)
#define CHG_IRQ                         BIT(6)
#define SC_IRQ                          BIT(5)
#define FLASH_IRQ                       BIT(1)
#define CHG_IRQ_BULK_NUM                6
#define CHG_IRQ_FLAG_0_REG              REG_IRQ_FLAG_0_ADDR(REG_IRQ_BASE)
#define CHG_IRQ_FLAG_6_REG              REG_IRQ_FLAG_6_ADDR(REG_IRQ_BASE)
#define CHG_IRQ_FLAG_12_REG             REG_IRQ_FLAG_12_ADDR(REG_IRQ_BASE)
#define SC_IRQ_BULK_NUM                 3
#define FLASH_IRQ_BULK_NUM              2

#define CHG_WDT_SOFT_RST_REG            REG_WDT_SOFT_RST_ADDR(REG_GLB_BASE)
#define CHG_WDT_SOFT_RST_SHIFT          REG_SC_WDT_EN_sc_wdt_en_START
#define CHG_WDT_SOFT_RST_MSK            (0x1 << CHG_WDT_SOFT_RST_SHIFT)

#define CHG_SC_WDT_EN_REG               REG_SC_WDT_EN_ADDR(REG_GLB_BASE)
#define CHG_SC_WDT_EN_SHIFT             REG_WDT_SOFT_RST_wd_rst_n_START
#define CHG_SC_WDT_EN_MSK               (0x1 << CHG_WDT_SOFT_RST_SHIFT)

#define CHG_USB_PLUGIN_EN_SHIFT         REG_USB_OVP_CFG_REG_2_da_usb_plugin_en_START
#define CHG_USB_PLUGIN_EN_MSK           (0x01 << CHG_USB_PLUGIN_EN_SHIFT)
#define USB_PLUGIN_DIS                  0
#define USB_PLUGIN_EN                   1

#define CHG_WDT_CTRL_REG                REG_WDT_CTRL_ADDR(REG_GLB_BASE)
#define CHG_WDT_CTRL_SHIFT              REG_WDT_CTRL_sc_watchdog_timer_START
#define CHG_WDT_CTRL_MSK                (0x7 << CHG_WDT_CTRL_SHIFT)

#define CHG_BUCK_OTG_TOP_CFG_REG_32_REG REG_BUCK_OTG_TOP_CFG_REG_32_ADDR(REG_ANA_BASE)
#define CHG_OTG_LIM_SHIFT               REG_BUCK_OTG_TOP_CFG_REG_32_da_otg_lim_set_START
#define CHG_OTG_LIM_MSK                 (0x3 << CHG_OTG_LIM_SHIFT)

#define CHG_PSW_OVP_CFG_REG_2_REG       REG_PSW_OVP_CFG_REG_2_ADDR(REG_ANA_BASE)
#define CHG_PSW_OVP_SET_SHIFT           REG_PSW_OVP_CFG_REG_2_da_psw_ovp_set_START
#define CHG_PSW_OVP_SET_MSK             (0x7 << CHG_PSW_OVP_SET_SHIFT)

#define CHG_PSW_OVP_CFG_REG_0_REG       REG_PSW_OVP_CFG_REG_0_ADDR(REG_ANA_BASE)
#define CHG_PSW_OVP_PRO_EN_SHIFT        REG_PSW_OVP_CFG_REG_0_da_psw_ovp_pro_en_START
#define CHG_PSW_OVP_PRO_EN_MSK          (0x1 << CHG_PSW_OVP_PRO_EN_SHIFT)

#define CHG_SYS_DET_CFG_REG_4_REG       REG_SYS_DET_CFG_REG_4_ADDR(REG_ANA_BASE)
#define CHG_BAT_GD_SEL_SHIFT            3
#define CHG_BAT_GD_SEL_MSK              (0x3 << CHG_BAT_GD_SEL_SHIFT)
#define BAT_GD_2V8                      0
#define BAT_GD_2V6                      1
#define BAT_GD_2V5                      2
#define BAT_GD_3V                       3

#define CHG_REF_TOP_CFG_REG_3_REG       REG_REF_TOP_CFG_REG_3_ADDR(REG_ANA_BASE)
#define CHG_VBUS_UV_SET_SHIFT           REG_REF_TOP_CFG_REG_3_da_ref_resv1_START
#define CHG_VBUS_UV_SET_MSK             (0x7 << CHG_VBUS_UV_SET_SHIFT)
#define VBUS_UV_3V8                     0
#define VBUS_UV_7V4                     1
#define VBUS_UV_2V5                     2
#define VBUS_UV_3V                      3
#define VBUS_UV_5V                      4
#define VBUS_UV_6V                      5
#define VBUS_UV_10V                     6
#define VBUS_UV_12V                     7
#define CHG_FAST_DROPMIN_EN_SHIFT       3
#define CHG_FAST_DROPMIN_EN_MSK         (0x1 << CHG_FAST_DROPMIN_EN_SHIFT)

#define CHG_DA_SC_EN_REG                REG_DA_SC_EN_ADDR(REG_GLB_BASE)
#define CHG_SC_EN_SHIFT                 REG_DA_SC_EN_da_sc_en_START
#define CHG_SC_EN_MSK                   (0x1 << CHG_SC_EN_SHIFT)

#define CHG_SC_TOP_CFG_REG_7_REG        REG_SC_TOP_CFG_REG_7_ADDR(REG_ANA_BASE)
#define CHG_S1Q6_DRIVER_SHIFT           1
#define CHG_S1Q6_DRIVER_MSK             (0x3 << CHG_S1Q6_DRIVER_SHIFT)
#define S1Q6_DRIVER_DEFAULT             0
#define S1Q6_DRIVER_TRIPLE              1
#define S1Q6_DRIVER_WEAKEST             2
#define S1Q6_DRIVER_DOUBLE              3

#define CHG_BOOST_CFG_REG_5_REG         REG_BOOST_CFG_REG_5_ADDR(REG_ANA_BASE)
#define CHG_S1Q5S_EN_SHIFT              7
#define CHG_S1Q5S_EN_MSK                (0x1 << CHG_S1Q5S_EN_SHIFT)

#define CHG_BOOST_CFG_REG_6_REG         REG_BOOST_CFG_REG_6_ADDR(REG_ANA_BASE)
#define CHG_S1Q6_EN_SHIFT               7
#define CHG_S1Q6_EN_MSK                 (0x1 << CHG_S1Q6_EN_SHIFT)
#define CHG_S1Q5_EN_SHIFT               6
#define CHG_S1Q5_EN_MSK                 (0x1 << CHG_S1Q5_EN_SHIFT)
#define CHG_S1Q4S_EN_SHIFT              5
#define CHG_S1Q4S_EN_MSK                (0x1 << CHG_S1Q4S_EN_SHIFT)
#define CHG_S1Q4_EN_SHIFT               4
#define CHG_S1Q4_EN_MSK                 (0x1 << CHG_S1Q4_EN_SHIFT)

#define Q1Q1_SS_BUCK_REG                REG_SYS_LOGIC_CFG_REG_8_ADDR(REG_ANA_BASE)
#define DA_SYS_RESVO2_SHIFT             0
#define DA_SYS_RESVO2_MSK               (0x1 << DA_SYS_RESVO2_SHIFT)

#define CHG_REF_TOP_CFG_REG_3_REG       REG_REF_TOP_CFG_REG_3_ADDR(REG_ANA_BASE)
#define CHG_VBUS_UV_SET_SHIFT           REG_REF_TOP_CFG_REG_3_da_ref_resv1_START
#define CHG_VBUS_UV_SET_MSK             (0x7 << CHG_VBUS_UV_SET_SHIFT)

#define CHG_USBOVP_CTRL_REG             REG_USBOVP_CTRL_ADDR(REG_GLB_BASE)
#define CHG_USBOVP_CTRL_SHIFT           REG_USBOVP_CTRL_da_usbovp_ctrl_START
#define CHG_USBOVP_CTRL_MSK             (0x3 << CHG_USBOVP_CTRL_SHIFT)

#define CHG_PSWOVP_CTRL_REG             REG_PSWOVP_CTRL_ADDR(REG_GLB_BASE)
#define CHG_PSWOVP_CTRL_SHIFT           REG_PSWOVP_CTRL_da_pswovp_ctrl_START
#define CHG_PSWOVP_CTRL_MSK             (0x3 << CHG_PSWOVP_CTRL_SHIFT)

#define CHG_PD_PWR_CTRL_REG             REG_PD_PWR_CTRL_ADDR(REG_PD_BASE)
#define CHG_BLEED_DISCH_EN_SHIFT        REG_PD_PWR_CTRL_pd_bleed_disch_en_START
#define CHG_BLEED_DISCH_EN_MSK          (0x1 << CHG_BLEED_DISCH_EN_SHIFT)
#define CHG_FORCE_DISCH_EN_SHIFT        REG_PD_PWR_CTRL_pd_force_disch_en_START
#define CHG_FORCE_DISCH_EN_MSK          (0x1 << CHG_FORCE_DISCH_EN_SHIFT)
#define DISCH_EN                        1
#define DISCH_DIS                       0

#define CHG_BAT_RES_MODE_REG            REG_BAT_RES_MODE_ADDR(REG_ANA_BASE)
#define CHG_BAT_RES_MODE_SHIFT          REG_BAT_RES_MODE_sc_bat_res_mode_START
#define CHG_BAT_RES_MODE_MSK            (0x1 << CHG_BAT_RES_MODE_SHIFT)

#define CHG_SC_PRO_CFG_REG_5_REG        REG_SC_PRO_CFG_REG_5_ADDR(REG_ANA_BASE)
#define CHG_IBAT_OCP_SET_SHIFT          REG_SC_PRO_CFG_REG_5_da_sc_ibat_ocp_set_START
#define CHG_IBAT_OCP_SET_MSK            (0xf << CHG_IBAT_OCP_SET_SHIFT)

#define CHG_USB_OVP_CFG_REG_3_REG       REG_USB_OVP_CFG_REG_3_ADDR(REG_ANA_BASE)
#define CHG_GATE1_EN_SHIFT              0
#define CHG_GATE1_EN_MSK                (0x1 << CHG_GATE1_EN_SHIFT)

/* When the ibat gear is 24A, 0xa indicates the ibat ocp 20A gear. */
#define IBAT_OCP_20A                    0xa

#define CHG_COUL_TOP_CFG_REG_8_REG      REG_COUL_TOP_CFG_REG_8_ADDR(REG_COUL_NOPWR0_BASE)
#define CHG_SHIP_MODE_DELAY_SHIFT       1
#define CHG_SHIP_MODE_DELAY_MSK         (0x1 << CHG_SHIP_MODE_DELAY_SHIFT)

#define CHG_SYS_DET_CFG_REG_4_REG       REG_SYS_DET_CFG_REG_4_ADDR(REG_ANA_BASE)
#define CHG_BAT_ECO_SET_SHIFT           REG_SYS_DET_CFG_REG_4_da_sys_det_reserve_START
#define CHG_BAT_ECO_SET_MSK             (0x7 << CHG_BAT_ECO_SET_SHIFT)
#define BAT_ECO_3300MV                  3300
#define BAT_ECO_3200MV                  3200
#define BAT_ECO_3100MV                  3100
#define BAT_ECO_3000MV                  3000
#define BAT_ECO_2900MV                  2900
#define BAT_ECO_2750MV                  2750
#define BAT_ECO_2600MV                  2600
#define BAT_ECO_2500MV                  2500

#define CHG_AUTO_SHIP_MODE_REG          REG_AUTO_SHIP_MODE_ADDR(REG_ANA_BASE)
#define CHG_AUTO_SHIP_MODE_SHIFT        REG_AUTO_SHIP_MODE_auto_ship_mode_START
#define CHG_AUTO_SHIP_MODE_MSK          (0x1 << CHG_AUTO_SHIP_MODE_SHIFT)

#define CHG_SYS_DET_CFG_REG_5_REG       REG_SYS_DET_CFG_REG_5_ADDR(REG_ANA_BASE)
#define CHG_DROP_OVP_EN_SHIFT           2
#define CHG_DROP_OVP_EN_MSK             (0x1 << CHG_DROP_OVP_EN_SHIFT)

#define CHG_SC_PRO_CFG_REG_1_REG    REG_SC_PRO_CFG_REG_1_ADDR(REG_ANA_BASE)
#define CHG_S1Q1_UCP_EN_SHIFT       REG_SC_PRO_CFG_REG_1_da_s1q1_ucp_en_START
#define CHG_S1Q1_UCP_EN_MSK         (0x1 << CHG_S1Q1_UCP_EN_SHIFT)

#define CHG_S1Q1_RCP_EN_SHIFT       REG_SC_PRO_CFG_REG_1_da_s1q1_rcp_en_START
#define CHG_S1Q1_RCP_EN_MSK         (0x1 << CHG_S1Q1_RCP_EN_SHIFT)

#define CHG_SC_PRO_CFG_REG_3_REG    REG_SC_PRO_CFG_REG_3_ADDR(REG_ANA_BASE)
#define CHG_S2Q1_UCP_EN_SHIFT       REG_SC_PRO_CFG_REG_3_da_s2q1_ucp_en_START
#define CHG_S2Q1_UCP_EN_MSK         (0x1 << CHG_S2Q1_UCP_EN_SHIFT)

#define CHG_S2Q1_RCP_EN_SHIFT       REG_SC_PRO_CFG_REG_3_da_s2q1_rcp_en_START
#define CHG_S2Q1_RCP_EN_MSK         (0x1 << CHG_S2Q1_RCP_EN_SHIFT)

#define CHG_USB_OVP_CFG_REG_4_REG       REG_USB_OVP_CFG_REG_4_ADDR(REG_ANA_BASE)
#define CHG_SCP_PRO_EN_SHIFT            4
#define CHG_SCP_PRO_EN_MSK              (0x1 << CHG_SCP_PRO_EN_SHIFT)

#define CHG_SYS_DET_CFG_REG_1_REG       REG_SYS_DET_CFG_REG_1_ADDR(REG_ANA_BASE)
#define CHG_PSW_DROPMIN_DBT_SHIFT       REG_SYS_DET_CFG_REG_1_da_sc_psw_dropmin_dbt_START
#define CHG_PSW_DROPMIN_DBT_MSK         (0x1 << CHG_PSW_DROPMIN_DBT_SHIFT)
#define CHG_PSW_DROPMIN_SET_SHIFT       REG_SYS_DET_CFG_REG_1_da_sc_psw_dropmin_set_START
#define CHG_PSW_DROPMIN_SET_MSK         (0x3 << CHG_PSW_DROPMIN_SET_SHIFT)
#define DBT_TIME_1MS                    1
#define DBT_TIME_8US                    0
#define DROPMIN_SET_200                 1
#define DROPMIN_SET_400                 3

#define CHG_SYS_DET_CFG_REG_0_REG       REG_SYS_DET_CFG_REG_0_ADDR(REG_ANA_BASE)
#define CHG_BAT_GD_SHIELD_SHIFT         REG_SYS_DET_CFG_REG_0_da_bat_gd_shield_START
#define CHG_BAT_GD_SHIELD_MSK           (0x1 << CHG_BAT_GD_SHIELD_SHIFT)
#define FORCE_BAT_GD_HIGH				1

#define CHG_BOOST_CFG_REG_6_REG         REG_BOOST_CFG_REG_6_ADDR(REG_ANA_BASE)
#define CHG_SEL_R13W_SHIFT              3
#define CHG_SEL_R13W_MSK                (0x1 << CHG_SEL_R13W_SHIFT)
#define SEL_R13W_2MS                    1
#define SEL_R13W_1MS                    0

#define CHG_BOOST_CFG_REG_8_REG         REG_BOOST_CFG_REG_8_ADDR(REG_ANA_BASE)
#define CHG_SEL_R14W_SHIFT              6
#define CHG_SEL_R14W_MSK                (0x1 << CHG_SEL_R14W_SHIFT)
#define SEL_R14W_4MS                    1
#define SEL_R14W_2MS                    0

#define CHG_S1Q6S_EN_SHIFT              5
#define CHG_S1Q6S_EN_MSK                (0x1 << CHG_S1Q6S_EN_SHIFT)

#define CHG_DEBUG_WRITE_PRO_REG REG_DEBUG_WRITE_PRO_ADDR(REG_COUL_NOPWR0_BASE)
#define WRITE_PRO_ENABLE    0x5a
#define WRITE_PRO_DISABLE   0xac

#define CHG_SYS_LOGIC_CFG_REG_4_REG     REG_SYS_LOGIC_CFG_REG_4_ADDR(REG_ANA_BASE)
#define CHG_WDT_FORCE_MASK_SHIFT        2
#define CHG_WDT_FORCE_MASK_MSK          (0x1 << CHG_WDT_FORCE_MASK_SHIFT)
#define CHG_LDO33_EN_SHIFT              0
#define CHG_LDO33_EN_MSK                (0x1 << CHG_LDO33_EN_SHIFT)

#define CHG_IRQ_STATUS_2_REG            REG_IRQ_STATUS_2_ADDR(REG_IRQ_BASE)
#define CHG_VBAT_OVP_MSK                (0x1 << REG_IRQ_STATUS_2_st_irq_vbat_ovp_START)

#define CHG_IRQ_FLAG_2_REG              REG_IRQ_FLAG_2_ADDR(REG_IRQ_BASE)
#define CHG_IRQ_BATFET_OCP_SHIFT        REG_IRQ_FLAG_2_irq_batfet_ocp_START
#define CHG_IRQ_BATFET_OCP_MSK          (0x1 << CHG_IRQ_BATFET_OCP_SHIFT)

#define CHG_USB_OVP_CFG_REG_3_REG       REG_USB_OVP_CFG_REG_3_ADDR(REG_ANA_BASE)
#define CHG_GATE1_EN_SHIFT              0
#define CHG_GATE1_EN_MSK                (0x1 << CHG_GATE1_EN_SHIFT)

#define CHG_OVP_GATE1_VDROPOVP_EN_SHIFT 7
#define CHG_OVP_GATE1_VDROPOVP_EN_MSK   (0x1 << CHG_OVP_GATE1_VDROPOVP_EN_SHIFT)

#define CHG_USB_OVP_CFG_REG_1_REG       REG_USB_OVP_CFG_REG_1_ADDR(REG_ANA_BASE)
#define CHG_GATE1_VDROPOVP_SET_SHIFT    6
#define CHG_GATE1_VDROPOVP_SET_MSK      (0x1 << CHG_GATE1_VDROPOVP_SET_SHIFT)
#define CHG_USB_UV_SET_SHIFT            4
#define CHG_USB_UV_SET_MSK              (0x1 << CHG_USB_UV_SET_SHIFT)

#define CHG_PSW_OVP_CFG_REG_1_REG       REG_PSW_OVP_CFG_REG_1_ADDR(REG_ANA_BASE)
#define CHG_GATE2_VDROPOVP_SET_SHIFT    6
#define CHG_GATE2_VDROPOVP_SET_MSK      (0x1 << CHG_GATE2_VDROPOVP_SET_SHIFT)
#define CHG_SYS_LOGIC_CFG_REG_9_REG     REG_SYS_LOGIC_CFG_REG_9_ADDR(REG_ANA_BASE)
#define CHG_OVP_GATE2_VDROPOVP_EN_SHIFT 7
#define CHG_OVP_GATE2_VDROPOVP_EN_MSK   (0x1 << CHG_OVP_GATE2_VDROPOVP_EN_SHIFT)
#define CHG_GATE2_EN_SHIFT              0
#define CHG_GATE2_EN_MSK                (0x1 << CHG_GATE2_EN_SHIFT)

#define EFUSE_TESTBUS_CFG_REG           (REG_EFUSE_TESTBUS_CFG_ADDR(REG_GLB_BASE))
#define EFUSE_TESTBUS_EN_SHIFT          (REG_EFUSE_TESTBUS_CFG_sc_efuse_testbus_en_START)
#define EFUSE_TESTBUS_EN_MSK            (0x1 << EFUSE_TESTBUS_EN_SHIFT)
#define EFUSE_TESTBUS_SEL_SHIFT         (REG_EFUSE_TESTBUS_CFG_sc_efuse1_testbus_sel_START)
#define EFUSE_TESTBUS_SEL_MSK           (0x7f << EFUSE_TESTBUS_SEL_SHIFT)
#define EFUSE_TESTBUS_RDATA_REG         (REG_EFUSE_TESTBUS_RDATA_ADDR(REG_GLB_BASE))
#define EFUSE_RD_ENABLE                 1
#define EFUSE_RD_DISABLE                0

#define EFUSE_FSM_RESET_REG             REG_EFUSE_FSM_RESET_ADDR(REG_GLB_BASE)

#define EFUSE_PDOB_SEL_REG              (REG_EFUSE_PDOB_SEL_ADDR(REG_GLB_BASE))
#define EFUSE_PDOB_SEL_SHIFT            (REG_EFUSE_PDOB_SEL_sc_efuse_pdob_sel_START)
#define EFUSE_PDOB_SEL_MSK              (0x01 << EFUSE_PDOB_SEL_SHIFT)
#define EFUSE_PDOB_PRE_ADDR_WE_REG      (REG_EFUSE_PDOB_PRE_ADDR_WE_ADDR(REG_GLB_BASE))
#define EFUSE_PDOB_PRE_ADDR_SHIFT       (REG_EFUSE_PDOB_PRE_ADDR_WE_sc_efuse_pdob_pre_addr_START)
#define EFUSE_PDOB_PRE_ADDR_MSK         (0x3f << EFUSE_PDOB_PRE_ADDR_SHIFT)
#define EFUSE_PDOB_PRE_WR_EN_SHIFT      (REG_EFUSE_PDOB_PRE_ADDR_WE_sc_efuse_pdob_pre_we_START)
#define EFUSE_PDOB_PRE_WR_EN_MSK        (0x1 << EFUSE_PDOB_PRE_WR_EN_SHIFT)

#define EFUSE_PDOB_PRE_WDATA_REG        (REG_EFUSE_PDOB_PRE_WDATA_ADDR(REG_GLB_BASE))
#define EFUSE_PDOB_PRE_WDATA_SHIFT      (REG_EFUSE_PDOB_PRE_WDATA_sc_efuse_pdob_pre_wdata_START)
#define EFUSE_PDOB_PRE_WDATA_MSK        (0xff << EFUSE_PDOB_PRE_WDATA_SHIFT)
#define EFUSE_WR_ENABLE                 0x01
#define EFUSE_WR_DISABLE                0x00
#define EFUSE_WR_START                  0x01
#define EFUSE_WR_END                    0x00
#define MASK_RESET_N_EFUSE_ADDR         0x07
#define DISABLE_MASK_RESET_N            (0x01 << 7)
#define DISABLE_MASK_RESET_N_MSK        (0x01 << 7)

#define EFUSE_MODE_REG                  REG_EFUSE_MODE_ADDR(REG_GLB_BASE)
#define EFUSE_MODE_WR_MODE              0x05
#define EFUSE_MODE_WR_MODE_EXIT         0x00
#define EFUSE_RDEN_REG                  REG_EFUSE_RDEN_ADDR(REG_GLB_BASE)
#define EFUSE_PGMEN_REG                 REG_EFUSE_PGMEN_ADDR(REG_GLB_BASE)
#define EFUSE_ADDR_REG                  REG_EFUSE_ADDR_ADDR(REG_GLB_BASE)
#define EFUSE_ADDR_H_REG                REG_EFUSE_ADDR_H_ADDR(REG_GLB_BASE)
#define EFUSE_AEN_REG                   REG_EFUSE_AEN_ADDR(REG_GLB_BASE)
#define EFUSE_RD_CTRL_REG               REG_EFUSE_RD_CTRL_ADDR(REG_GLB_BASE)
#define EFUSE_ADDR_CAL_SEL_REG          REG_EFUSE_ADDR_CAL_SEL_ADDR(REG_GLB_BASE)

#define REG_BIT_NUM                     8
#define BIT_ADDR_H_MSK                  0xff00
#define BIT_ADDR_L_MSK                  0x00ff

#define CHG_AD_STATUS_RSV3_REG          REG_AD_STATUS_RSV3_ADDR(REG_ANA_BASE)
#define CHG_SC_SS_OK_SHIFT              6
#define CHG_SC_SS_OK_MSK                (0x1 << CHG_SC_SS_OK_SHIFT)

#define CHG_VUSB_UVLO_MSK               (0x1 << 7)
#define CHG_VPSW_UVLO_MSK               (0x1 << 2)

#define CHG_USB_OVP_CFG_REG_0_REG       REG_USB_OVP_CFG_REG_0_ADDR(REG_ANA_BASE)
#define CHG_FAST_USB_UV_EN_SHIFT        REG_USB_OVP_CFG_REG_0_da_usb_ovp_alm_en_START
#define CHG_FAST_USB_UV_EN_MSK          (0x1 << CHG_FAST_USB_UV_EN_SHIFT)

/* adc regs */
#define CHG_HKADC_CTRL1_REG             REG_HKADC_CTRL1_ADDR(REG_ADC_BASE)
#define CHG_HKADC_SEQ_LOOP_SHIFT        REG_HKADC_CTRL1_sc_hkadc_seq_loop_START
#define CHG_HKADC_SEQ_LOOP_MSK          (0x1 << CHG_HKADC_SEQ_LOOP_SHIFT)

#define CHG_HKADC_EN_REG                REG_HKADC_EN_ADDR(REG_ADC_BASE)
#define CHG_HKADC_EN_SHIFT              REG_HKADC_EN_sc_hkadc_en_START
#define CHG_HKADC_EN_MSK                (0x1 << CHG_HKADC_EN_SHIFT)

#define CHG_HKADC_SEQ_CH_H_REG          REG_HKADC_SEQ_CH_H_ADDR(REG_ADC_BASE)
#define CHG_HKADC_SEQ_CH_L_REG          REG_HKADC_SEQ_CH_L_ADDR(REG_ADC_BASE)

#define CHG_HKADC_START_REG             REG_HKADC_START_ADDR(REG_ADC_BASE)
#define CHG_HKADC_START_SHIFT           REG_HKADC_START_sc_hkadc_start_START
#define CHG_HKADC_START_MSK             (0x1 << CHG_HKADC_START_SHIFT)

#define CHG_HKADC_RD_SEQ_REG            REG_HKADC_RD_SEQ_ADDR(REG_ADC_BASE)
#define CHG_HKADC_RD_SEQ_SHIFT          REG_HKADC_RD_SEQ_sc_hkadc_rd_req_START
#define CHG_HKADC_RD_SEQ_MSK            (0x1 << CHG_HKADC_RD_SEQ_SHIFT)

#define CHG_CH0_ADC_L_REG               REG_CH0_ADC_L_ADDR(REG_ADC_BASE)
#define REG_ADC_DATA_BASE               CHG_CH0_ADC_L_REG
#define ADC_DATA_LEN                    2

#define CHG_HKADC_DATA_VALID_REG        REG_HKADC_DATA_VALID_ADDR(REG_ADC_BASE)
#define CHG_HKADC_DATA_VALID_SHIFT      REG_HKADC_DATA_VALID_hkadc_data_valid_START
#define CHG_HKADC_DATA_VALID_MSK        (0x1 << CHG_HKADC_DATA_VALID_SHIFT)

#define CHG_TYPE_CFG_REG_4_REG          REG_TYPE_CFG_REG_4_ADDR(REG_ANA_BASE)
#define SBU1_10UA_DET                   (BIT(2)|BIT(1))
#define SBU1_1UA_DET                    (BIT(1))
#define SBU2_10UA_DET                   (BIT(4)|BIT(2)|BIT(0))
#define SBU2_1UA_DET                    (BIT(4)|BIT(0))
#define SBU_DET_DIS                     (0)

#define CHG_FCP_TOP_CFG_REG_REG         REG_FCP_TOP_CFG_REG_ADDR(REG_ANA_BASE)
#define DP_10UA_DET                     (BIT(2)|BIT(1))
#define DP_1UA_DET                      (BIT(2))
#define DP_DET_DIS                      (0)

#define CHG_DA_SC_MODE_REG              REG_DA_SC_MODE_ADDR(REG_GLB_BASE)
#define CHG_DA_SC_MODE_SHIFT            REG_DA_SC_MODE_da_sc_mode_START
#define CHG_DA_SC_MODE_MSK              (0x7 << CHG_DA_SC_MODE_SHIFT)

#define IBUS_FORWARD_COEFFICIENT        ((10 * 130 * 40) / 6)
#define IBUS_REVERSE_COEFFICIENT        ((11 * 130 * 20) / 3)

#define CHG_IRQ_STATUS_0_REG            REG_IRQ_STATUS_0_ADDR(REG_IRQ_BASE)
#define CHG_IRQ_ST_VUSB_OVP_SHIFT       REG_IRQ_STATUS_0_st_irq_ovp_vusb_ovp_START
#define CHG_IRQ_ST_VUSB_OVP_MSK         (0x1 << REG_IRQ_STATUS_0_st_irq_ovp_vusb_ovp_START)

/* fcp regs */
#define FCP_VUSB_PLUGOUT_REG            REG_IRQ_STATUS_0_ADDR(REG_IRQ_BASE)
#define FCP_VUSB_PLUGOUT_SHIFT          REG_IRQ_STATUS_0_st_irq_ovp_vusb_plugout_START
#define FCP_VUSB_PLUGOUT_MSK            (0x1 << FCP_VUSB_PLUGOUT_SHIFT)
#define FCP_ISR1_REG                    REG_ISR1_ADDR(REG_FCP_BASE)
#define FCP_ISR2_REG                    REG_ISR2_ADDR(REG_FCP_BASE)
#define FCP_CMD_REG                     REG_CMD_ADDR(REG_FCP_BASE)
#define FCP_ADDR_REG                    REG_ADDR_ADDR(REG_FCP_BASE)
#define FCP_CNTL_REG                    REG_CNTL_ADDR(REG_FCP_BASE)
#define FCP_FCP_IRQ3_REG                REG_FCP_IRQ3_ADDR(REG_FCP_BASE)
#define FCP_FCP_IRQ4_REG                REG_FCP_IRQ4_ADDR(REG_FCP_BASE)
#define FCP_FCP_RDATA0_REG              REG_FCP_RDATA0_ADDR(REG_FCP_BASE)
#define FCP_STATUIS_REG                 REG_STATUIS_ADDR(REG_FCP_BASE)
#define FCP_FCP_CTRL_REG                REG_FCP_CTRL_ADDR(REG_FCP_BASE)
#define FCP_DATA0_REG                   REG_DATA0_ADDR(REG_FCP_BASE)
#define FCP_FCP_ADAP_CTRL_REG           REG_FCP_ADAP_CTRL_ADDR(REG_FCP_BASE)
#define FCP_FCP_SOFT_RST_CTRL_REG       REG_FCP_SOFT_RST_CTRL_ADDR(REG_FCP_BASE)
#define FCP_FCP_LENGTH_REG              REG_FCP_LENGTH_ADDR(REG_FCP_BASE)

#define BUCK_VBUS_UVP_IRQ_REG           REG_IRQ_MASK_5_ADDR(REG_IRQ_BASE)
#define BUCK_VBUS_UVP_SHIFT             REG_IRQ_MASK_5_irq_reserved_2_mask_START
#define BUCK_VBUS_UVP_MASK              (0x1 << BUCK_VBUS_UVP_SHIFT)

#define CHG_FLASH_LED1_CODE_REG         REG_FLASH_LED1_CODE_ADDR(REG_FLASH_BASE)
#define CHG_FLASH_LED2_CODE_REG         REG_FLASH_LED2_CODE_ADDR(REG_FLASH_BASE)

#define CHG_FLASH_DIG_CFG_0_REG         REG_FLASH_DIG_CFG_0_ADDR(REG_FLASH_BASE)
#define CHG_FLASH_BB_EN_SHIFT           REG_FLASH_DIG_CFG_0_sc_flash_bb_en_START
#define CHG_FLASH_BB_EN_MSK             (0x1 << CHG_FLASH_BB_EN_SHIFT)
#define FLASH_BB_ENABLE                 1

#define CHG_FLASH_LED_EN_REG            REG_FLASH_LED_EN_ADDR(REG_FLASH_BASE)
#define CHG_LED_EN_SHIFT                REG_FLASH_LED_EN_sc_flash_led_en_START
#define CHG_LED_EN_MSK                  (0x7 << CHG_LED_EN_SHIFT)
#define LED1_EN                         0x1
#define LED1_DIS                        0x6
#define LED2_EN                         (0x1 << 1)
#define LED2_DIS                        0x5

#define CHG_BUCK_OTG_TOP_CFG_REG_15_REG REG_BUCK_OTG_TOP_CFG_REG_15_ADDR(REG_ANA_BASE)
#define CHG_CC_EN_SHIFT                 4
#define CHG_CC_EN_MSK                   (0x1 << CHG_CC_EN_SHIFT)

#define CHG_SC_CRG_CLK_EN_CTRL_1_REG    REG_SC_CRG_CLK_EN_CTRL_1_ADDR(REG_GLB_BASE)
#define CHG_FLASH_CLK_EN_SHIFT          REG_SC_CRG_CLK_EN_CTRL_1_sc_cg_flash_clk_en_START
#define CHG_FLASH_CLK_EN_MSK            (0x1 << CHG_FLASH_CLK_EN_SHIFT)

#define CHG_FLASH_LED_MD_REG            REG_FLASH_LED_MD_ADDR(REG_FLASH_BASE)
#define CHG_FLASH_LED_MD_SHIFT          REG_FLASH_LED_MD_sc_flash_led_md_START
#define CHG_FLASH_LED_MD_MSK            (0x7 << CHG_FLASH_LED_MD_SHIFT)
#define LED1_TORCH_MODE                 0x0
#define LED2_TORCH_MODE                 0x0

#define CHG_FLASH_RAMP_STEP_REG         REG_FLASH_RAMP_STEP_ADDR(REG_FLASH_BASE)
#define SET_CURRENT_RAMP                0x22

#define CHG_AD_STATUS_RSV0_REG          REG_AD_STATUS_RSV0_ADDR(REG_ANA_BASE)
#define CHG_BOOST_EN                    (0x1 << 1)

#define CHG_TCPC_CFG_REG_3_REG          REG_TCPC_CFG_REG_3_ADDR(REG_ANA_BASE)
#define CHG_SBU_OVP_EN_SHIFT            3
#define CHG_SBU_OVP_EN_MSK              (0x1 << CHG_SBU_OVP_EN_SHIFT)

#endif