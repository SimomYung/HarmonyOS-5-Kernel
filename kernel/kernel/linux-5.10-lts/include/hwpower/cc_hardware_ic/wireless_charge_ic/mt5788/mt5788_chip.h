/* SPDX-License-Identifier: GPL-2.0 */
/*
 * mt5788_chip.h
 *
 * mt5788 registers, chip info, etc.
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

#ifndef _MT5788_CHIP_H_
#define _MT5788_CHIP_H_

#define MT5788_GPIO_PWR_GOOD_VAL             1
#define MT5788_ADDR_LEN                      2

/* chip id register */
#define MT5788_CHIP_ID_ADDR                  0x0000
#define MT5788_CHIP_ID_LEN                   2
#define MT5788_CHIP_ID                       0x5788

/* mtp ver register */
#define MT5788_MTP_VER_ADDR                  0x0008
#define MT5788_MTP_VER_LEN                   4

/*
 * rx mode
 */

/* sys mode register */
#define MT5788_RX_SYS_MODE_ADDR              0x0018
#define MT5788_RX_SYS_MODE_LEN               4
#define MT5788_RX_SYS_MODE_RX                BIT(1)
#define MT5788_RX_SYS_MODE_BRIDGE_MASK       BIT(18)
#define MT5788_RX_SYS_MODE_BRIDGE_SHIFT      18
#define MT5788_RX_SYS_MODE_FC_SUCC_MASK      BIT(17)
#define MT5788_RX_SYS_MODE_FC_SUCC_SHIFT     17
#define MT5788_RX_SYS_MODE_FC_SUCC_RESET     1

/* interrupt flag register */
#define MT5788_RX_IRQ_ADDR                   0X0020
#define MT5788_RX_IRQ_LEN                    4
#define MT5788_RX_IRQ_READY                  BIT(9)
#define MT5788_RX_IRQ_POWER_ON               BIT(8)
#define MT5788_RX_IRQ_FSK_PKT                BIT(14)
#define MT5788_RX_IRQ_OCP                    BIT(17)
#define MT5788_RX_IRQ_OVP                    BIT(18)
#define MT5788_RX_IRQ_OTP                    BIT(19)
#define MT5788_RX_IRQ_OPP_H                  BIT(26)

/* rx_irq_clr register */
#define MT5788_RX_IRQ_CLR_ADDR               0X0024
#define MT5788_RX_IRQ_CLR_LEN                4
#define MT5788_RX_IRQ_CLR_ALL                0xFFFFFFFF

/* rx_ctrl register */
#define MT5788_RX_CTRL_ADDR                  0x0028
#define MT5788_RX_CTRL_LEN                   4
#define MT5788_RX_CTRL_EXT_POWER_MASK        BIT(0)
#define MT5788_RX_CTRL_EXT_POWER_SHIFT       0
#define MT5788_RX_CTRL_24BIT_RPP_MASK        BIT(1)
#define MT5788_RX_CTRL_24BIT_RPP_SHIFT       1
#define MT5788_RX_CTRL_EN                    1
#define MT5788_RX_CTRL_DIS                   0

/* rx_cmd register */
#define MT5788_RX_CMD_ADDR                   0x002c
#define MT5788_RX_CMD_LEN                    4
#define MT5788_RX_CMD_VAL                    1
#define MT5788_RX_CMD_SYS_RST_MASK           BIT(3)
#define MT5788_RX_CMD_SYS_RST_SHIFT          3
#define MT5788_RX_CMD_CLEAR_INT_MASK         BIT(4)
#define MT5788_RX_CMD_CLEAR_INT_SHIFT        4
#define MT5788_RX_CMD_SEND_PPP_MASK          BIT(8)
#define MT5788_RX_CMD_SEND_PPP_SHIFT         8
#define MT5788_RX_CMD_SEND_EPT_MASK          BIT(9)
#define MT5788_RX_CMD_SEND_EPT_SHIFT         9
#define MT5788_RX_CMD_SEND_FC_MASK           BIT(10)
#define MT5788_RX_CMD_SEND_FC_SHIFT          10
#define MT5788_RX_CMD_VOUT_MASK              BIT(11)
#define MT5788_RX_CMD_VOUT_SHIFT             11
#define MT5788_RX_CMD_SET_CALI_PARAM_MASK    BIT(12)
#define MT5788_RX_CMD_SET_CALI_PARAM_SHIFT   12
#define MT5788_RX_CMD_FOD_UPDATE_MASK        BIT(18)
#define MT5788_RX_CMD_FOD_UPDATE_SHIFT       18
#define MT5788_RX_CMD_SWITCH_BRG_MASK        BIT(20)
#define MT5788_RX_CMD_SWITCH_BRG_SHIFT       20

/* rx ping fop */
#define MT5788_RX_PING_FOP_VAL_ADDR          0x0050
#define MT5788_RX_PING_FOP_VAL_LEN           2
#define MT5788_RX_PING_FOP_VAL_UNIT          10

/* rx_op_freq register, in kHZ */
#define MT5788_RX_FOP_ADDR                   0x0052
#define MT5788_RX_FOP_LEN                    2

/* rx_iout register */
#define MT5788_RX_IOUT_ADDR                  0x0054
#define MT5788_RX_IOUT_LEN                   2

/* rx_vout register */
#define MT5788_RX_VOUT_ADDR                  0x0056
#define MT5788_RX_VOUT_LEN                   2

/* rx_vrect register */
#define MT5788_RX_VRECT_ADDR                 0x0058
#define MT5788_RX_VRECT_LEN                  2

/* rx_chip_temp register, in degC */
#define MT5788_RX_CHIP_TEMP_ADDR             0x005a
#define MT5788_RX_CHIP_TEMP_LEN              2

/* rx ping vrect */
#define MT5788_RX_PING_VRECT_VAL_ADDR        0x005e
#define MT5788_RX_PING_VRECT_VAL_LEN         2

/* signal strength value */
#define MT5788_RX_SS_ADDR                    0x0072
#define MT5788_RX_SS_LEN                     1
#define MT5788_RX_SS_MAX                     255

/* rx_cep_val register */
#define MT5788_RX_CEP_VAL_ADDR               0x0073
#define MT5788_RX_CEP_VAL_LEN                1

/* cep cnt */
#define MT5788_RX_RPP_INTV_ADDR              0x0084
#define MT5788_RX_RPP_INTV_LEN               1
#define MT5788_RX_RPPDLY_CNT_DFLT            15

/* iout cali */
#define MT5788_RX_IOUT_CALI_ADDR             0x0086
#define MT5788_RX_IOUT_CALI_LEN              3

/* rx otp register */
#define MT5788_RX_OTP_ADDR                   0x0090
#define MT5788_RX_OTP_LEN                    2
#define MT5788_RX_OTP_TH_DEFAULT             140

/* dummy_iload_ask_th regiester */
#define MT5788_RX_ILOAD_ASK_TH_ADDR          0x00a0
#define MT5788_RX_ILOAD_ASK_TH_LEN           2
#define MT5788_RX_ILOAD_ASK_TH_DEFAULT       100
/* dummy_iload_ask regiester */
#define MT5788_RX_ILOAD_ASK_ADDR             0x00a2
#define MT5788_RX_ILOAD_ASK_LEN              2
#define MT5788_RX_ILOAD_ASK_DEFAULT          60

/* dummy_iload_th regiester */
#define MT5788_RX_ILOAD_NON_ASK_TH_ADDR      0x00a4
#define MT5788_RX_ILOAD_NON_ASK_TH_LEN       2
#define MT5788_RX_ILOAD_NON_ASK_TH_DEFAULT   100

/* dummy_iload regiester */
#define MT5788_RX_ILOAD_ADDR                 0x00a6
#define MT5788_RX_ILOAD_LEN                  2
#define MT5788_RX_ILOAD_DEFAULT              60

/* Vrect curve parameter */
#define MT5788_RX_LDO_CFG_ADDR               0x00b0
#define MT5788_RX_LDO_CFG_LEN                8
#define MT5788_RX_LDO_CFG_V2_ADDR            0x00b2
#define MT5788_RX_LDO_CFG_V1_ADDR            0x00b0

/* rx_ldo_opp_cur_th, in mA */
#define MT5788_RX_LDO_OPP_ITHL_ADDR          0x00b8
#define MT5788_RX_LDO_OPP_ITHL_LEN           2
#define MT5788_RX_LDO_OPP_ITHL_DFLT          2000
#define MT5788_RX_LDO_OPP_ITHH_ADDR          0x00ba
#define MT5788_RX_LDO_OPP_ITHH_LEN           2
#define MT5788_RX_LDO_OPP_ITHH_DFLT          4000

/* rx_ldo_opp_vbst, in mV */
#define MT5788_RX_LDO_OPP_VBST_ADDR          0x00bc
#define MT5788_RX_LDO_OPP_VBST_LEN           2
#define MT5788_RX_LDO_OPP_VBST_DFLT          0

/* rx_vout_set register, 3500-20000mV */
#define MT5788_RX_VOUT_SET_ADDR              0x00be
#define MT5788_RX_VOUT_SET_LEN               2
#define MT5788_RX_VOUT_MAX                   20000
#define MT5788_RX_VOUT_MIN                   3500

/* ask modulation cap */
#define MT5788_RX_SWITCH_BRIDGE_MODE_ADDR    0x00c0
#define MT5788_RX_SWITCH_BRIDGE_MODE_LEN     1
#define MT5788_RX_FULL_BRIDGE                1
#define MT5788_RX_HALF_BRIDGE                2
#define MT5788_RX_NEED_CTRL_BRG              1

/* rx_fc_vmldo register */
#define MT5788_RX_FC_VMLDO_ADDR              0x00c2
#define MT5788_RX_FC_VMLDO_LEN               2
#define MT5788_RX_SET_VMLDO_SLEEP_TIMEOUT    3000
#define MT5788_RX_SET_VMLDO_SLEEP_TIME       50

/* rx_fc_volt register */
#define MT5788_RX_FC_VPA_ADDR                0x00c4
#define MT5788_RX_FC_VPA_LEN                 2
#define MT5788_RX_FC_VPA_RETRY_CNT           3
#define MT5788_RX_BPCP_SLEEP_TIME            50
#define MT5788_RX_BPCP_TIMEOUT               200
#define MT5788_RX_FC_VOUT_ERR_LTH            500 /* lower threshold */
#define MT5788_RX_FC_VOUT_ERR_UTH            1000 /* upper threshold */
#define MT5788_RX_FC_VOUT_TIMEOUT            1500
#define MT5788_RX_FC_VOUT_SLEEP_TIME         50

/* rx_vfc_diff, in mV */
#define MT5788_RX_VFC_DIFF_ADDR              0x00c6
#define MT5788_RX_VFC_DIFF_LEN               2
#define MT5788_RX_VFC_DIFF_DEFAULT           1800

/* rx watchdog register */
#define MT5788_RX_WDT_ADDR                   0x00c8
#define MT5788_RX_WDT_LEN                    2
#define MT5788_RX_KICK_WDT_VAL               0

/* watchdog overflow threshold */
#define MT5788_RX_WDT_TIMEOUT_ADDR           0x00ca
#define MT5788_RX_WDT_TIMEOUT_LEN            2
#define MT5788_RX_WDT_TIMEOUT                1000

/* ask modulation cap */
#define MT5788_RX_ASK_CAP_ADDR               0x00d1
#define MT5788_RX_ASK_CAP_LEN                1
#define MT5788_RX_ASK_CAP_CM_MASK            (BIT(0) | BIT(1) | BIT(2))
#define MT5788_RX_ASK_CAP_POLARITY_SHIFT     3
#define MT5788_RX_CM_POSITIVE                0
#define MT5788_RX_CM_NEGTIVE                 1
#define MT5788_RX_CMALL_EN_VAL               0x3
#define MT5788_RX_CAP0_POSITIVE              0x01

/* Parameter of  Set_Max_Power CMD */
#define MT5788_RX_MAX_POWER_ADDR             0x00d2
#define MT5788_RX_MAX_POWER_LEN              1
#define MT5788_RX_RP_VAL_UNIT                2

/* EPT type */
#define MT5788_RX_EPT_MSG_ADDR               0x00d3
#define MT5788_RX_EPT_MSG_LEN                1

/* ASK header register */
#define MT5788_RX_SEND_MSG_HEADER_ADDR       0x00d8
#define MT5788_RX_SEND_MSG_HEADER_LEN        1

/* ask msg cmd */
#define MT5788_RX_SEND_MSG_CMD_ADDR          0x00d9
#define MT5788_RX_SEND_MSG_CMD_LEN           1

/* ask msg data */
#define MT5788_RX_SEND_MSG_DATA_ADDR         0x00da
#define MT5788_RX_SEND_MSG_DATA_LEN          4

/* FSK Proprietary packet header */
#define MT5788_RX_FSK_HEADER_ADDR            0x00ee
#define MT5788_RX_FSK_HEADER_LEN             1

/* FSK Proprietary packet message0 */
#define MT5788_RX_FSK_CMD_ADDR               0x00ef
#define MT5788_RX_FSK_CMD_LEN                1

/* rx_ldo opp off register */
#define MT5788_RX_LDO_OPP_OFF_ADDR           0x00fa
#define MT5788_RX_LDO_OFF_OFF_EN             1
/* rx_fod_coef register */
#define MT5788_RX_FOD_ADDR                   0x0100
#define MT5788_RX_FOD_LEN                    20
#define MT5788_RX_FOD_TMP_STR_LEN            4
#define MT5788_RX_FOD_OFFSET0_ADDR           0x0109

/* rx_fod_offset regiester */
#define MT5788_RX_FOD_OFFSET_ADDR            0x0115
#define MT5788_RX_FOD_OFFSET_LEN             1
#define MT5788_RX_FOD_OFFSET_DEFAULT         48

/* rxmode rx power */
#define MT5788_RX_PRX_ADDR                   0x0060
#define MT5788_RX_PRX_LEN                    2

/*
 * tx mode
 */

/* sys mode register */
#define MT5788_TX_SYS_MODE_ADDR              0x0018
#define MT5788_TX_SYS_MODE_LEN               4
#define MT5788_TX_SYS_MODE_RX                BIT(1)
#define MT5788_TX_SYS_MODE_TX                BIT(2)

/* enable int register */
#define MT5788_TX_INT_EN_ADDR                0x001c
#define MT5788_TX_INT_EN_LEN                 4
#define MT5788_TX_INT_EN_ALL                 0XFFFFFFFF

/* interrupt flag register */
#define MT5788_TX_INT_FLAG_ADDR              0x0020
#define MT5788_TX_INT_FLAG_LEN               4
#define MT5788_TX_INT_FSK_PP_ACK             BIT(5)
#define MT5788_TX_INT_SS_PKG_RCVD            BIT(8)
#define MT5788_TX_INT_ID_PKT_RCVD            BIT(9)
#define MT5788_TX_INT_CFG_PKT_RCVD           BIT(10)
#define MT5788_TX_INT_ASK_PKT_RCVD           BIT(15)
#define MT5788_TX_INT_AC_DET                 BIT(16)
#define MT5788_TX_INT_EPT                    BIT(17)
#define MT5788_TX_INT_START_PING             BIT(18)
#define MT5788_TX_INT_RPP_TIMEOUT            BIT(20)
#define MT5788_TX_INT_TX_PING_OCP            BIT(27)
#define MT5788_TX_INT_TX_FOD                 BIT(30)

/* clear interrrupt flag */
#define MT5788_TX_INT_CLEAR_ADDR             0x0024
#define MT5788_TX_INT_CLEAR_LEN              4
#define MT5788_TX_CLEAR_ALL_INT              0xFFFFFFFF

/* ctrl register */
#define MT5788_TX_CTRL_ADDR                  0x0028
#define MT5788_TX_CTRL_LEN                   4
#define MT5788_TX_CTRL_CEP_MASK              BIT(5)
#define MT5788_TX_CTRL_CEP_SHIFT             5
#define MT5788_TX_CTRL_FOD_EN_MASK           BIT(6)
#define MT5788_TX_CTRL_FOD_EN_SHIFT          6
#define MT5788_TX_CTRL_OPEN_LOOP_MASK        BIT(7)
#define MT5788_TX_CTRL_OPEN_LOOP_SHIFT       7
#define MT5788_TX_CTRL_EN                    1
#define MT5788_TX_CTRL_DIS                   0

/* cmd register */
#define MT5788_TX_CMD_ADDR                   0x002c
#define MT5788_TX_CMD_LEN                    4
#define MT5788_TX_CMD_VALUE                  1
#define MT5788_TX_CMD_ENTER_TX_MASK          BIT(1)
#define MT5788_TX_CMD_ENTER_TX_SHIFT         1
#define MT5788_TX_CMD_CHIP_RST_MASK          BIT(3)
#define MT5788_TX_CMD_CHIP_RST_SHIFT         3
#define MT5788_TX_CMD_CLEAR_INT_MASK         BIT(4)
#define MT5788_TX_CMD_CLEAR_INT_SHIFT        4
#define MT5788_TX_CMD_START_WORK_MASK        BIT(8)
#define MT5788_TX_CMD_START_WORK_SHIFT       8
#define MT5788_TX_CMD_STOP_WORK_MASK         BIT(9)
#define MT5788_TX_CMD_STOP_WORK_SHIFT        9
#define MT5788_TX_CMD_SEND_PPP_MASK          BIT(10)
#define MT5788_TX_CMD_SEND_PPP_SHIFT         10
#define MT5788_TX_TMD_SET_CALI_PARAM_MASK    BIT(11)
#define MT5788_TX_CMD_SET_CALI_PARAM_SHIFT   11
#define MT5788_TX_CMD_BRG_SET_MASK           BIT(18)
#define MT5788_TX_CMD_BRG_SET_SHIFT          18
#define MT5788_TX_CMD_FOD_EN_MASK            BIT(19)
#define MT5788_TX_CMD_FOD_EN_SHIFT           19
#define MT5788_TX_CMD_24BIT_RPP_MASK         BIT(22)
#define MT5788_TX_CMD_24BIT_RPP_SHIFT        22

/* fop register */
#define MT5788_TX_FOP_ADDR                   0x0052
#define MT5788_TX_FOP_LEN                    2

/* input current register */
#define MT5788_TX_IIN_ADDR                   0x0054
#define MT5788_TX_IIN_LEN                    2

/* input voltage register */
#define MT5788_TX_VIN_ADDR                   0x0056
#define MT5788_TX_VIN_LEN                    2

/* bridge voltage register */
#define MT5788_TX_VBRG_ADDR                  0X0058
#define MT5788_TX_VBRG_LEN                   2

/* temp register */
#define MT5788_TX_TEMP_ADDR                  0x005a
#define MT5788_TX_TEMP_LEN                   2

/* tx_curfod ping iin */
#define MT5788_TX_CURFOD_IIN_ADDR            0x0062
#define MT5788_TX_CURFOD_IIN_LEN             2

/* control error packet value */
#define MT5788_TX_CEP_VAL_ADDR               0x0073
#define MT5788_TX_CEP_VAL_LEN                1
#define MT5788_TX_CEP_VAL_STEP               10

/* duty cycle */
#define MT5788_TX_DUTY_READ_ADDR             0x0076
#define MT5788_TX_DUTY_READ_LEN              2

/* rx receive power */
#define MT5788_TX_PRX_ADDR                   0x007c
#define MT5788_TX_PRX_LEN                    4

/* input power */
#define MT5788_TX_PTX_ADDR                   0x0080
#define MT5788_TX_PTX_LEN                    4

/* Power loss */
#define MT5788_TX_PLOSS_ADDR                 0x0084
#define MT5788_TX_PLOSS_LEN                  4

/* end power transfer type */
#define MT5788_TX_EPT_TYPE_ADDR              0x0088
#define MT5788_TX_EPT_TYPE_LEN               4
#define MT5788_TX_EPT_CLEAR                  0
#define MT5788_TX_EPT_PT_OCP                 BIT(1)
#define MT5788_TX_EPT_RX                     BIT(5)
#define MT5788_TX_EPT_CEP_TIMEOUT            BIT(6)
#define MT5788_TX_EPT_RX_RST                 BIT(8)
#define MT5788_TX_EPT_FOD                    BIT(3)
#define MT5788_TX_EPT_SS                     BIT(11)

/* max input voltage threshold */
#define MT5788_TX_PT_OVP_ADDR                0x0090
#define MT5788_TX_PT_OVP_LEN                 2
#define MT5788_TX_PT_OVP_TH                  20000

/* max input current */
#define MT5788_TX_PT_OCP_ADDR                0x0094
#define MT5788_TX_PT_OCP_LEN                 2
#define MT5788_TX_PT_OCP_TH                  1800
#define MT5788_TX_BAT_HEATING_OCP1_TH        1000

/* tx_ping_ocp_thres register, in mA */
#define MT5788_TX_PING_OCP_TH_ADDR           0x0096
#define MT5788_TX_PING_OCP_TH_LEN            2
#define MT5788_TX_PING_OCP_TH_DEFAULT        2000

/* over current register */
#define MT5788_TX_ILIM_ADDR                  0x0098
#define MT5788_TX_ILIM_LEN                   2
#define MT5788_TX_ILIM_MIN                   200
#define MT5788_TX_ILIM_MAX                   3000

/* min input voltage threshold */
#define MT5788_TX_LVP_ADDR                   0x009a
#define MT5788_TX_LVP_LEN                    2
#define MT5788_TX_LVP_TH                     3500

/* Max Transmit Power */
#define MT5788_TX_MAX_POWER_ADDR             0x009e
#define MT5788_TX_MAX_POWER_LEN              1
#define MT5788_TX_RP_VAL_UNIT                2

/* TX Iin chip */
#define MT5788_TX_IIN_CHIP_ADDR              0x009f
#define MT5788_TX_IIN_CHIP_LEN               1
#define MT5788_TX_IIN_CHIP_DFLT              12

/* bridge ctrl */
#define MT5788_TX_BRIDGE_ACESS_ADDR          0x00b2
#define MT5788_TX_BRIDGE_ACESS_LEN           1
#define MT5788_TX_BRIDGE_ACESS_AP_CTRL       1
#define MT5788_TX_BRIDGE_ACESS_CHIP_CTRL     0
#define MT5788_TX_BRIDGE_MODE_ADDR           0x00b3
#define MT5788_TX_BRIDGE_MODE_LEN            1
#define MT5788_TX_PING_BRIDGE_MASK           (BIT(0) | BIT(1) | BIT(2) | BIT(3))
#define MT5788_TX_PING_BRIDGE_SHIFT          0
#define MT5788_TX_PING_HALF_BRIDGE           0x3
#define MT5788_TX_PING_FULL_BRIDGE           0xc
#define MT5788_TX_PT_BRIDGE_MASK             (BIT(4) | BIT(5)|BIT(6) | BIT(7))
#define MT5788_TX_PT_BRIDGE_SHIFT            4
#define MT5788_TX_PT_HALF_BRIDGE             0x3
#define MT5788_TX_PT_FULL_BRIDGE             0xc
#define MT5788_TX_SWITCH_BRI_CTL_VTH         7500 /* mV */

/* bridge duty */
#define MT5788_TX_BRIDGE_DUTY_ADDR           0x00b4
#define MT5788_TX_BRIDGE_DUTY_LEN            2
#define MT5788_TX_BRIDGE_DUTY_DEFAULT        30

/* bridge freq */
#define MT5788_TX_BRIDGE_FREQ_ADDR           0x00b6
#define MT5788_TX_BRIDGE_FREQ_LEN            2
#define MT5788_TX_BRIDGE_FREQ_DEFAULT        200

/* Digital Ping Interval */
#define MT5788_TX_PING_INTERVAL_ADDR         0x00ba
#define MT5788_TX_PING_INTERVAL_LEN          2
#define MT5788_TX_PING_INTERVAL_MIN          0
#define MT5788_TX_PING_INTERVAL_MAX          1000
#define MT5788_TX_PING_INTERVAL              500
#define MT5788_TX_PING_INTERVAL_COIL_TEST    1
#define MT5788_TX_PING_INTERVAL_BAT_HEATING  1

/* ping freq register */
#define MT5788_TX_PING_FREQ_ADDR             0X00bc
#define MT5788_TX_PING_FREQ_LEN              2
#define MT5788_TX_PING_FREQ_STEP             10
#define MT5788_TX_PING_FREQ_MIN              100
#define MT5788_TX_PING_FREQ_MAX              180
#define MT5788_TX_PING_FREQ                  115
#define MT5788_TX_PING_FREQ_COIL_TEST        115
#define MT5788_TX_PING_DUTY_COIL_TEST        30
#define MT5788_TX_PING_FREQ_BAT_HEATING      100

/* tx ping duty */
#define MT5788_TX_PING_DUTY_ADDR             0x00be
#define MT5788_TX_PING_DUTY_LEN              2
#define MT5788_TX_PING_DUTY                  30
#define MT5788_TX_PING_DUTY_MIN              20
#define MT5788_TX_PING_DUTY_MAX              50

/* pt duty */
#define MT5788_TX_PT_DUTY_MAX_ADDR           0x00c0
#define MT5788_TX_PT_DUTY_MAX_LEN            2
#define MT5788_TX_PT_DUTY_MAX_DEFAULT        50
#define MT5788_TX_PT_DUTY_MIN_ADDR           0x00c2
#define MT5788_TX_PT_DUTY_MIN_LEN            2
#define MT5788_TX_PT_DUTY_MIN_DEFAULT        10

/* max operation freq register */
#define MT5788_TX_MAX_FOP_FREQ_ADDR          0x00c4
#define MT5788_TX_MAX_FOP_FREQ_LEN           2

/* min operation freq register */
#define MT5788_TX_MIN_FOP_FREQ_ADDR          0x00c6
#define MT5788_TX_MIN_FOP_FREQ_LEN           2
#define MT5788_TX_MIN_FOP                    113
#define MT5788_TX_MAX_FOP                    145
#define MT5788_TX_FOP_STEP                   10

/* tx iin cali */
#define MT5788_TX_IIN_CALI_ADDR             0x00d6
#define MT5788_TX_IIN_CALI_LEN              3

/* ask packet header */
#define MT5788_TX_RCVD_MSG_HEADER_ADDR       0x00e0
#define MT5788_TX_RCVD_MSG_HEADER_LEN        1

/* FSK Header */
#define MT5788_TX_SEND_MSG_HEADER_ADDR       0x0100
#define MT5788_TX_SEND_MSG_HEADER_LEN        1

/* FSK Message0 */
#define MT5788_TX_SEND_MSG_CMD_ADDR          0x0101
#define MT5788_TX_SEND_MSG_CMD_LEN           1

/* FSK Message1 */
#define MT5788_TX_SEND_MSG_DATA_ADDR         0x0102
#define MT5788_TX_SEND_MSG_DATA_LEN          1

/* tx_fod_ploss_cnt regiester */
#define MT5788_TX_PLOSS_CNT_ADDR             0x0110
#define MT5788_TX_PLOSS_CNT_VAL              3
#define MT5788_TX_PLOSS_CNT_LEN              2

/* tx_fod_th_id register */
#define MT5788_TX_FOD_TH_ID_ADDR             0x0112
#define MT5788_TX_FOD_TH_ID_LEN              2

/* tx_fod_ploss_thres regierster, in mW */
#define MT5788_TX_PLOSS_TH0_ADDR             0x0124 /* 5v full bridge */
#define MT5788_TX_PLOSS_TH0_VAL              4000
#define MT5788_TX_PLOSS_TH0_LEN              2
#define MT5788_TX_PLOSS_TH1_ADDR             0x126 /* 10v half bridge */
#define MT5788_TX_PLOSS_TH1_VAL              4000
#define MT5788_TX_PLOSS_TH1_LEN              2
#define MT5788_TX_PLOSS_TH2_ADDR             0x128 /* 10v full bridge low vol */
#define MT5788_TX_PLOSS_TH2_VAL              4000
#define MT5788_TX_PLOSS_TH2_LEN              2
#define MT5788_TX_PLOSS_TH3_ADDR             0x12a /* 10v full bridge high vol */
#define MT5788_TX_PLOSS_TH3_VAL              4000
#define MT5788_TX_PLOSS_TH3_LEN              2
#define MT5788_TX_FOD_ADDR                   0x0124
#define MT5788_TX_FOD_LEN                    12

/*
 * firmware register
 */

/* chip id */
#define MT5788_MTP_CHIPID_ADDR               0x1008
#define MT5788_MTP_CHIPID_LEN                2
#define MT5788_MTP_CHIPID_VALUE              0x5788

/* program cmd */
#define MT5788_MTP_PGM_CMD_ADDR              0x1000
#define MT5788_MTP_PGM_CMD_LEN               2
#define MT5788_MTP_PGM_CMD                   0x01
#define MT5788_MTP_PGM_SUCESS                0x02

/* program mtp */
#define MT5788_MTP_PGM_ADDR_ADDR             0x1002
#define MT5788_MTP_PGM_ADDR_LEN              2

/* program mtp len */
#define MT5788_MTP_PGM_LEN_ADDR              0x1004
#define MT5788_MTP_PGM_LEN_LEN               2

/* check sum */
#define MT5788_MTP_PGM_CHKSUM_ADDR           0x1006
#define MT5788_MTP_PGM_CHKSUM_LEN            2

/* mtp data */
#define MT5788_MTP_PGM_DATA_ADDR             0x1008
#define MT5788_MTP_BUFF_SIZE                 256

/* crc address */
#define MT5788_MTP_CRC_ADDR                  0x0010
#define MT5788_MTP_CRC_LEN                   2

/* total mtp length */
#define MT5788_MTP_TOTAL_LENGTH_ADDR         0x0012
#define MT5788_MTP_TOTAL_LENGTH_LEN          2
#define MT5788_MTP_CRC_CHECK                 0x0014
#define MT5788_MTP_CRC_CHECK_LEN             1
#define MT5788_MTP_CRC_CHECK_VAL             0xaa
#define MT5788_MTP_CRC_RESULT                0x0015
#define MT5788_MTP_CRC_RESULT_LEN            1
#define MT5788_MTP_CRC_CHK_SUCC              0x02
#define MT5788_MTP_CRC_CHK_FAIL              0x03

/* load bootloader */
#define MT5788_BTLOADER_ADDR                 0x0000

/* fw crc calculation */
#define MT5788_MTP_CRC_SEED                  0x1021
#define MT5788_MTP_CRC_INIT                  0xffff
#define MT5788_MTP_CRC_HIGHEST_BIT           0x8000

/* watchdog */
#define MT5788_MTP_WDT_ADDR                  0x5808
#define MT5788_MTP_WDT_ADDR_LEN              1
#define MT5788_MTP_WDT_CLOSE_RETRY           24
#define MT5788_MTP_WDT_CLOSE_FLAG            0
#define MT5788_MTP_WDT_CLOSE_EN              0x95

/* erase mtp */
#define MT5788_MTP_ERASE_ADDR                0x1000
#define MT5788_MTP_ERASE_ADDR_LEN            2
#define MT5788_MTP_ERASE_EN                  0x8000
#define MT5788_MTP_ERASE_SUCC                0x02
#define MT5788_MTP_ERASE_CNT                 50
#define MT5788_MTP_ERASE_DEALY               10000 /* 10ms */

#endif /* _MT5788_CHIP_H_ */
