/* SPDX-License-Identifier: GPL-2.0 */
/*
 * bk96285_chip.h
 *
 * bk96285 registers, chip info, etc.
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

#ifndef _BK96285_CHIP_H_
#define _BK96285_CHIP_H_

#define BK96285_GPIO_PWR_GOOD_VAL       1
#define BK96285_ALIGN_ADDR_MASK         0XFFFC
#define BK96285_ALIGN_ADDR_LEN          4
#define BK96285_READ_EXT_LEN            8
#define BK96285_TX_REG_NUM              0x0200
#define BK96285_REG_STEP                4

/* chip id register */
#define BK96285_CHIP_ID_ADDR            0x0000
#define BK96285_CHIP_ID_LEN             4

/* mtp ver register */
#define BK96285_MTP_VER_ADDR            0x0004
#define BK96285_MTP_VER_LEN             4

/* product type for fw */
#define BK96285_PRODUCT_TYPE_ADDR       0x0018
#define BK96285_PRODUCT_TYPE_DFLT       1

/* ask and fsk info */
#define BK96285_TRX_ASK_PKG_HEADER_ADDR 0x0130
#define BK96285_TRX_ASK_PKG_CMD_ADDR    0x0131
#define BK96285_TRX_ASK_PKG_DATA_ADDR   0x0132
#define BK96285_TRX_FSK_PKG_HEADER_ADDR 0x0150
#define BK96285_TRX_FSK_PKG_CMD_ADDR    0x0151
#define BK96285_TRX_FSK_PKG_DATA_ADDR   0x0152
#define BK96285_SNED_MSG_RETRY_CNT      2
#define BK96285_SEND_MSG_DATA_LEN       4
#define BK96285_RCVD_PKT_BUFF_LEN       8
#define BK96285_RCVD_PKT_STR_LEN        64
#define BK96285_RCVD_MSG_PKT_LEN        6
#define BK96285_WAIT_FOR_ACK_RETRY_CNT  5
#define BK96285_RCV_MSG_SLEEP_CNT       10
#define BK96285_RX_SEND_MSG_DATA_LEN    4
#define BK96285_QI_RETRY                0xCC

/* sys_mode register */
#define BK96285_SYS_MODE_ADDR           0x0108
#define BK96285_SYS_MODE_BP             BIT(0)
#define BK96285_SYS_MODE_RX             BIT(1)
#define BK96285_SYS_MODE_TX             BIT(2)
#define BK96285_TX_PING_PHASE           BIT(27)
#define BK96285_TX_PT_PHASE             BIT(29)

/*
 * rx mode
 */

/* cmd register */
#define BK96285_RX_CMD_ADDR             0x0100
#define BK96285_RX_CMD_EN               1
#define BK96285_RX_CMD_CLEAR_IRQ_MASK   BIT(0)
#define BK96285_RX_CMD_CLEAR_IRQ_SHIFT  0
#define BK96285_RX_CMD_RESET_MASK       BIT(1)
#define BK96285_RX_CMD_RESET_SHIFT      1
#define BK96285_RX_CMD_SEND_PPP_MASK    BIT(2)
#define BK96285_RX_CMD_SEND_PPP_SHIFT   2
#define BK96285_RX_CMD_KICK_WD_MASK     BIT(8)
#define BK96285_RX_CMD_KICK_WD_SHIFT    8
#define BK96285_RX_CMD_24BIT_RPP_MASK   BIT(10)
#define BK96285_RX_CMD_24BIT_RPP_SHIFT  10
#define BK96285_RX_CMD_BRG_SWITCH_MASK  BIT(11)
#define BK96285_RX_CMD_BRG_SWITCH_SHIFT 11
#define BK96285_RX_CMD_UPDATE_FOD_MASK  BIT(12)
#define BK96285_RX_CMD_UPDATE_FOD_SHIFT 12
#define BK96285_RX_CMD_SEND_EPT_MASK    BIT(16)
#define BK96285_RX_CMD_SEND_EPT_SHIFT   16
#define BK96285_RX_CMD_SET_VOUT_MASK    BIT(17)
#define BK96285_RX_CMD_SET_VOUT_SHIFT   17
#define BK96285_RX_CMD_DUMM_UPT_MASK    BIT(18)
#define BK96285_RX_CMD_DUMM_UPT_SHIFT   18
#define BK96285_RX_CMD_SEND_FC_MASK     BIT(19)
#define BK96285_RX_CMD_SEND_FC_SHIFT    19
#define BK96285_RX_CMD_MCU_HALT_MASK    BIT(31)
#define BK96285_RX_CMD_MCU_HALT_SHIFT   31

/* otp1 register */
#define BK96285_RX_OTP_THR_ADDR         0x0030
#define BK96285_RX_OTP_THR_DEFAULT      140

/* vtx and vrx register */
#define BK96285_RX_VOUT_ADDR            0x0034
#define BK96285_RX_VOUT_MAX             20000
#define BK96285_RX_VOUT_MIN             3500
#define BK96285_RX_VOUT_DLF             5500

/* rx_ldo_opp_vbst, in mV */
#define BK96285_RX_LDO_OPP_VBST_ADDR    0x0038
#define BK96285_RX_LDO_OPP_VBST_DFLT    0

/* rx_ldo_opp_cur_th, in mA */
#define BK96285_RX_LDO_OPP_ITHL_ADDR    0x003A
#define BK96285_RX_LDO_OPP_ITHL_DFLT    2000
#define BK96285_RX_LDO_OPP_ITHH_ADDR    0x003C
#define BK96285_RX_LDO_OPP_ITHH_DFLT    3000

/* set vfc */
#define BK96285_RX_FC_VRX_ADDR          0x0084
#define BK96285_RX_FC_VTX_ADDR          0x0086
#define BK96285_RX_FC_SUCC_THR_ADDR     0x0088
#define BK96285_RX_FC_SUCC_THR_DEFAULT  1500
#define BK96285_RX_FC_VPA_RETRY_CNT     3
#define BK96285_RX_FC_VPA_SLEEP_TIME    50
#define BK96285_RX_FC_VPA_TIMEOUT       1500
#define BK96285_RX_FC_VRX_TIMEOUT       3000
#define BK96285_RX_FC_VRX_SLEEP_TIME    50
#define BK96285_RX_BPCP_TIMEOUT         200
#define BK96285_RX_BPCP_SLEEP_TIME      50
#define BK96285_RX_FC_VOUT_ERR_LTH      500 /* lower threshold */
#define BK96285_RX_FC_VOUT_ERR_UTH      1000 /* upper threshold */

/* rx_ldo_cfg: ldo_drop0-1 && ldo_cur_thres0-1 */
#define BK96285_RX_LDO_CFG_ADDR         0x004c
#define BK96285_RX_LDO_CFG_LEN          12
#define BK96285_RX_DROP_MIN_ADDR        0x004C
#define BK96285_RX_DROP_MAX_ADDR        0x004E
#define BK96285_RX_V_OFFSET_ADDR        0x0054

/* rx_fod_coef register */
#define BK96285_RX_FOD_ADDR             0x0060
#define BK96285_RX_FOD_LEN              20
#define BK96285_RX_FOD_STR_LEN          96
#define BK96285_RX_FOD_TMP_STR_LEN      4
#define BK96285_RX_FOD_C0_ADDR          0x0069 /* 0-50mA */
#define BK96285_RX_FOD_C1_ADDR          0x006a/* 50-100mA */
#define BK96285_RX_FOD_C2_ADDR          0x006b /* 100-150mA */
#define BK96285_RX_FOD_C3_ADDR          0x006c /* 150-200mA */

/* rx_fod_gain_unit regiester */
#define BK96285_RX_GAIN_UNIT_ADDR       0x0074
#define BK96285_RX_GAIN_UNIT_DEFAULT    4

/* rx_fod_offset regiester */
#define BK96285_RX_FOD_OFFSET_ADDR      0x0075
#define BK96285_RX_FOD_OFFSET_DEFAULT   50

/* fc timeout */
#define BK96285_RX_FC_TIMEOUT_ADDR      0x008a
#define BK96285_RX_FC_TIMEOUT_DFT       1500 /* 1.5s */

/* modulation capacitance */
#define BK96285_RX_CM_CFG_ADDR          0x0094
#define BK96285_RX_CM_SHIFT             2
#define BK96285_RX_CM_POLY_SHIFT        1
#define BK96285_RX_CM_CTRL_BY_AP        0x1
#define BK96285_RX_CM_POSITIVE          0
#define BK96285_RX_CMA_EN_VAL           0x1

/* no modulation dummy load size setting */
#define BK96285_RX_DUMMY_LOAD_NO_MOD_ADDR  0x009A
#define BK96285_RX_DUMMY_LOAD_NO_MOD_VAL   50

/* modulation dummy load size setting */
#define BK96285_RX_DUMMY_LOAD_MOD_ADDR     0x009E
#define BK96285_RX_DUMMY_LOAD_MOD_VAL      30

/* ctrl register */
#define BK96285_RX_CTRL_ADDR            0x0104
#define BK96285_RX_CTRL_EN              1
#define BK96285_RX_CTRL_DISABLE         0
#define BK96285_RX_CTRL_VDD_EN_MASK     BIT(1)
#define BK96285_RX_CTRL_VDD_EN_SHIFT    1
#define BK96285_RX_MLDO_OPP_EN_MASK     BIT(3)
#define BK96285_RX_MLDO_OPP_SHIFT       3
#define BK96285_RX_FASTRPP_EN_MASK      BIT(4)
#define BK96285_RX_FASTRPP_EN_SHIFT     4

/* intr flag */
#define BK96285_RX_INTR_FLAG_ADDR       0x0110
#define BK96285_RX_IRQ_POWER_ON         BIT(0)
#define BK96285_RX_IRQ_OTP              BIT(4)
#define BK96285_RX_IRQ_RECV_ACK         BIT(6)
#define BK96285_RX_IRQ_FSK_PKG_RECV     BIT(7)
#define BK96285_RX_IRQ_OCP              BIT(8)
#define BK96285_RX_IRQ_MLDO_OVP         BIT(9)
#define BK96285_RX_IRQ_OPP_H            BIT(18)

#define BK96285_RX_IRQ_FC_SUCC          BIT(19)
#define BK96285_RX_IRQ_READY            BIT(26)
#define BK96285_RX_IRQ_LDO_ON           BIT(27)
#define BK96285_RX_IRQ_LDO_OFF          BIT(28)
#define BK96285_RX_IRQ_VRECT_OVP        BIT(30)

/* intr clear */
#define BK96285_RX_INTR_CLR_ADDR        0X0114
#define BK96285_RX_INTR_CLR_ALL         0xffffffff
#define BK96285_RX_WDG_TIME_ADDR        0x011c
#define BK96285_RX_WDG_TIMES            2000
#define BK96285_RX_WDG_TIME_STEP        100

/* Actual vout/iout/vrect voltage */
#define BK96285_RX_VOUT_ADC_ADDR        0x0160
#define BK96285_RX_IOUT_ADDR            0x0164
#define BK96285_RX_VRECT_ADDR           0x0168

/* rx ping fop */
#define BK96285_RX_PING_FOP_VAL_ADDR    0x0176
#define BK96285_RX_PING_FOP_VAL_UNIT    10

/* rx ping vrect */
#define BK96285_RX_PING_VRECT_VAL_ADDR  0x016c

/* fop */
#define BK96285_RX_FOP_ADDR             0x0174
#define BK96285_RX_FOP_STEP             10

/* chip temp */
#define BK96285_RX_CHIP_TEMP_ADDR       0x0178
#define BK96285_RX_CHIP_TEMP_STEP       10

/* bridge ctrl */
#define BK96285_RX_BRIDGE_CTRL_ADDR     0x0180
#define BK96285_RX_FULL_BRIDGE          0x1
#define BK96285_RX_HALF_BRIDGE          0x3
#define BK96285_RX_NEED_CTRL_BRG        0

/* ept reason */
#define BK96285_RX_EPT_MSG_ADDR         0x0190

/* ss val */
#define BK96285_RX_SS_VAL_ADDR          0x01a0
#define BK96285_RX_SS_MAX               255

/* cep val */
#define BK96285_RX_CEP_VAL_ADDR         0x01b4

/* power max */
#define BK96285_RX_RP_PMAX_ADDR         0x01c0
#define BK96285_RX_RP_VAL_UNIT          1

/*
 * tx mode
 */
/* tx_fop register */
#define BK96285_TX_MIN_FOP_ADDR           0x00A0
#define BK96285_TX_MAX_FOP_ADDR           0x00A2
#define BK96285_TX_MIN_FOP                113
#define BK96285_TX_MAX_FOP                145
#define BK96285_TX_FOP_UNIT               10

/* tx_ilim register*/
#define BK96285_TX_ILIM_ADDR              0x00A6
#define BK96285_TX_ILIM_MIN               100
#define BK96285_TX_ILIM_MAX               2000

/* tx_ping_ocp register */
#define BK96285_TX_PING_OCP_TH_ADDR       0x00A8
#define BK96285_TX_PING_OCP_TH_DEFAULT    1000

/* tx_pt_ocp register */
#define BK96285_TX_PT_OCP_TH_ADDR         0x00AA
#define BK96285_TX_PT_OCP_TH              2000
#define BK96285_TX_BAT_HEATING_OCP_TH     1000

/* tx_pt_ovp register */
#define BK96285_TX_PT_OVP_TH_ADDR         0x00AE
#define BK96285_TX_PT_OVP_TH              20000

/* tx_ping_interval, in ms */
#define BK96285_TX_PING_INTERVAL_ADDR     0x00BC
#define BK96285_TX_PING_INTERVAL_LEN      2
#define BK96285_TX_PING_INTERVAL_MIN      0
#define BK96285_TX_PING_INTERVAL_MAX      1000
#define BK96285_TX_PING_INTERVAL          500

/* tx_ping_freq register */
#define BK96285_TX_PING_FREQ_ADDR         0x00C0
#define BK96285_TX_PING_FREQ_MIN          100
#define BK96285_TX_PING_FREQ_MAX          180
#define BK96285_TX_PING_FREQ              115
#define BK96285_TX_PING_FREQ_UNIT         10

/* tx_duty register */
#define BK96285_TX_PING_DUTY_ADDR         0x00C2
#define BK96285_TX_PING_DUTY_MIN          10
#define BK96285_TX_PING_DUTY_MAX          50
#define BK96285_TX_PING_DUTY_UNIT         255
#define BK96285_TX_PING_DUTY              30

/* tx_fsk_delay register */
#define BK96285_TX_FSK_DELAY_ADDR         0x00C8
#define BK96285_TX_FSK_DELAY              0

/* duty cycle */
#define BK96285_TX_PT_DUTY_MIN_ADDR       0x00D0
#define BK96285_TX_PT_DUTY_MIN_DEFAULT    10
#define BK96285_TX_PT_DUTY_MAX_ADDR       0x00D2
#define BK96285_TX_PT_DUTY_MAX_DEFAULT    50
#define BK96285_TX_PT_DUTY_MAX            50
#define BK96285_TX_PT_DUTY_UNIT           255

/* tx_brg_ctrl register*/
#define BK96285_TX_BRG_CTRL_ADDR          0x00D4
#define BK96285_TX_FULL_BRG               0
#define BK96285_TX_HALF_BRG               1
#define BK96285_TX_PING_BRG_MASK          BIT(0)
#define BK96285_TX_PING_BRG_SHIFT         0
#define BK96285_TX_PT_BRG_MASK            BIT(1)
#define BK96285_TX_PT_BRG_SHIFT           1
#define BK96285_TX_SWITCH_BRI_CTL_VTH     12
#define BK96285_TX_FULL_BRI_ITH           200 /* mA */

/* FOD triger counter */
#define BK96285_TX_PLOSS_CNT_ADDR         0x00E1

/* tx_fod_ploss_thres register, in mW */
#define BK96285_TX_FOD_ADDR               0x00E4
#define BK96285_TX_FOD_LEN                12
#define BK96285_TX_PLOSS_TH0_ADDR         0x00E4 /* 5v full bridge */
#define BK96285_TX_PLOSS_TH0_VAL          4000
#define BK96285_TX_PLOSS_TH1_ADDR         0x00E6 /* 10v half bridge */
#define BK96285_TX_PLOSS_TH1_VAL          4000
#define BK96285_TX_PLOSS_TH2_ADDR         0x00E8 /* 10v full bridge low vol */
#define BK96285_TX_PLOSS_TH2_VAL          4000
#define BK96285_TX_PLOSS_TH3_ADDR         0x00EA /* 10v full bridge high vol */
#define BK96285_TX_PLOSS_TH3_VAL          4000
#define BK96285_TX_PLOSS_CNT_VAL          3

/* tx_ploss register*/
#define BK96285_TX_PLOSS_ADDR             0x00F4

/* tx_cmd register*/
#define BK96285_TX_CMD_ADDR               0x0100
#define BK96285_TX_CMD_VAL                1
#define BK96285_TX_CMD_DIS                0
#define BK96285_TX_CMD_CLR_IRQ            BIT(0)
#define BK96285_TX_CMD_CLR_IRQ_SHIFT      1
#define BK96285_TX_CMD_SYS_RST            BIT(1)
#define BK96285_TX_CMD_SYS_RST_SHIFT      1
#define BK96285_TX_CMD_SEND_FSK           BIT(2)
#define BK96285_TX_CMD_OVP_CHANGE         BIT(4)
#define BK96285_TX_CMD_OVP_CHANGE_SHIFT   4
#define BK96285_TX_CMD_BRG_CHANGE         BIT(11)
#define BK96285_TX_CMD_BRG_CHANGE_SHIFT   11
#define BK96285_TX_CMD_FOD_UPDATE         BIT(12)
#define BK96285_TX_CMD_ILIM_CHANGE        BIT(13)
#define BK96285_TX_CMD_ILIM_CHANGE_SHIFT  13
#define BK96285_TX_CMD_EN_TX              BIT(14)
#define BK96285_TX_CMD_EN_TX_SHIFT        14
#define BK96285_TX_CMD_START_PING         BIT(16)
#define BK96285_TX_CMD_START_PING_SHIFT   16
#define BK96285_TX_CMD_STOP_PING          BIT(17)
#define BK96285_TX_CMD_STOP_PING_SHIFT    17
#define BK96285_TX_CMD_BRG_AUTO           BIT(19)
#define BK96285_TX_CMD_BRG_AUTO_SHIFT     19
#define BK96285_TX_CMD_BRG_MANUAL         BIT(20)
#define BK96285_TX_CMD_BRG_MANUAL_SHIFT   20
#define BK96285_TX_CMD_FOD_ENABLE         BIT(21)
#define BK96285_TX_CMD_FOD_DISABLE        BIT(22)
#define BK96285_TX_CMD_PWM_CHANGE         BIT(25)
#define BK96285_TX_CMD_PWM_CHANGE_SHIFT   25

/* tx ctrl register */
#define BK96285_TX_CTRL_ADDR               0x0104
#define BK96285_TX_CTRL_OPENLOOP_MASK      BIT(5)
#define BK96285_TX_CTRL_OPENLOOP_SHIFT     5
#define BK96285_TX_OPEN_LOOP_EN            1

/* tx_irq_en register*/
#define BK96285_TX_IRQ_EN_ADDR            0x010C
#define BK96285_TX_IRQ_VAL                0xFFFFFFFF

/* tx irq register*/
#define BK96285_TX_IRQ_ADDR               0x0110
#define BK96285_TX_IRQ_SS_PKG_RCVD        BIT(1)
#define BK96285_TX_IRQ_ID_PKT_RCVD        BIT(2)
#define BK96285_TX_IRQ_CFG_PKT_RCVD       BIT(3)
#define BK96285_TX_IRQ_ASK_PKT_RCVD       BIT(6)
#define BK96285_TX_IRQ_RX_PKT_RCVD        BIT(7)
#define BK96285_TX_IRQ_AC_DET             BIT(12)
#define BK96285_TX_IRQ_EPT_PKT_RCVD       BIT(17)
#define BK96285_TX_IRQ_REMOVE_POWER       BIT(20)
#define BK96285_TX_IRQ_START_PING         BIT(22)
#define BK96285_TX_IRQ_RPP_TIMEOUT        BIT(24)

/* tx_irq_clr register*/
#define BK96285_TX_IRQ_CLR_ADDR           0x0114
#define BK96285_TX_IRQ_CLR_ALL            0xFFFFFFFF

/* tx_vin register*/
#define BK96285_TX_VIN_ADDR               0x0160

/* tx_iin register*/
#define BK96285_TX_IIN_ADDR               0x0164

/* tx_vrect register*/
#define BK96285_TX_VRECT_ADDR             0x0168

/* tx_prx register*/
#define BK96285_RX_POWER_ADDR             0x0170

/* tx_ptx register*/
#define BK96285_TX_POWER_ADDR             0x0172

/* tx_fop register*/
#define BK96285_TX_FOP_FREQ_ADDR          0x0174
#define BK96285_TX_FOP_UNIT               10

/* tx_chip_temp register*/
#define BK96285_TX_CHIP_TEMP_ADDR         0x0178
#define BK96285_TX_TEMP_UNIT              10

/* tx_curfod ping iin */
#define BK96285_TX_CURFOD_IIN_ADDR        0x017C

/* tx_pwm_duty register*/
#define BK96285_TX_CURR_DUTY_ADDR         0x0184

/* tx_cep register*/
#define BK96285_TX_CEP_ADDR               0x01B4

/* tx_ept_type register */
#define BK96285_TX_EPT_SRC_ADDR           0x0194
#define BK96285_TX_EPT_SRC_CLR            0
#define BK96285_TX_EPT_SRC_FOD            BIT(1)
#define BK96285_TX_EPT_SRC_RX_EPT         BIT(2)
#define BK96285_TX_EPT_SRC_CEP_TIMEOUT    BIT(3)

/* tx_cfg register */
#define BK96285_TX_CFG_ADDR               0x01a9
#define BK96285_TX_CFG                    0X30

/* tx_fsk_cfg register*/
/*
 * firmware register
 */
#define BK96285_ADDR_LEN                2
#define BK96285_ADDR_HIGH_BASE          0xFFE0
#define BK96285_HIGH_8BIT_ADDR          8
#define BK96285_HIGH_16BIT_ADDR         16
#define BK96285_LOW_8BIT_ADDR           0xFF
#define BK96285_LOW_16BIT_ADDR          0xFFFF
#define BK96285_READ_LEN                4
#define BK96285_WRITE_LEN               4
#define BK96285_SHIFT_1BIT              1
#define BK96285_SHIFT_31BIT             31
#define BK96285_UINT_MAX                0xFFFFFFFF
#define BK96285_U16_MAX                 0xFFFF
#define BK96285_U8_MAX                  0xFF
#define BK96285_MSG_BIT                 1
#define BK96285_PGM_TYPE_LEN            1
#define BK96285_XOR_LEN                 2
#define BK96285_RETRY_TIMES             1000
#define BK96285_BYTE_LEN                4
#define BK96285_SHIFT_8BIT              8
#define BK96285_32BIT                   32
#define BK96285_16BIT                   16
#define BK96285_FW_CHECKSUM             0x442712e0
#define BK96285_SRAM_ADDR_H             (0x20000000 >> 16)
#define BK96285_SRAM_ADDR_L             (0x20000000 & 0xFFFF)

#define BK96285_CHECKSUM_ADDR	        0x000C
#define BK96285_PGM_HEADER_SIZE	        8
#define BK96285_PGM_MSG_SIZE            0x100
#define BK96285_PGM_MSG_ADDR            0x0008
#define BK96285_PGM_WORD                4
#define BK96285_PGM_INFO_SIZE           64
#define BK96285_PGM_XOR_LEN             6
#define BK96285_PGM_XOR_INDEX           2

#define BK96285_PGM_FRIMWARE_SIZE       256
#define BK96285_PGM_PKG_SIZE            16

#define BK96285_REG_ATE	                0x60000000
#define BK96285_REG_RST	                0xFFE4
#define BK96285_REG_IDLE                0xFFE7

#define BK96285_RST_PW0	                0x96
#define BK96285_RST_PW1	                0x00

#define BK96285_IDLE_PW0                0x5AA5
#define BK96285_IDLE_ADDR_HIGH          0x4000
#define BK96285_IDLE_ADDR_LOW1          0x7008
#define BK96285_IDLE_ADDR_LOW2          0x7004
#define BK96285_IDLE_VAL                0x5F4E585F

/*  The PGM will update the Status in the SRAM as follows: */
#define BK96285_PGM_STATE_LEN           4
#define BK96285_PGM_STATE_NONE          0x00
#define BK96285_PGM_STATE_TIMEOUT       0x7f
#define BK96285_PGM_STATE_DONE          0xa5
#define BK96285_PGM_STATE_BUSY          0xbb

#define BK96285_PGM_CMD_INFO            0xa0
#define BK96285_PGM_CMD_MARGIN          0xb0
#define BK96285_PGM_CMD_VERIFY          0xc0
#define BK96285_PGM_CMD_WRITE_CODE      0xc2
#define BK96285_PGM_CMD_CHIP_ERASE      0xce

#define BK96285_PGM_STATE_ADDR          0x0000

#define BK96285_SRAM_BASE               0x20000000

#define BK96285_CRC_ADDR                0x0000
#define BK96285_CRC_SIZE                4
#define BK96285_CRC_CHECK_MARGIN0       0
#define BK96285_CRC_CHECK_MARGIN7       3
#define BK96285_CRC_POLY                0x04c11db7

#define BK96285_CHIP_SIZE               (32 * 1024)

#define BK96285_ATE_BOOT_BIT            3

#define BK96285_FOUR_BYTE_ALIGN         4
#define BK96285_HIGH_ADDR_NUM           2
#define BK96285_HIGH_ADDR0              0
#define BK96285_HIGH_ADDR1              1

#pragma pack(1)
typedef struct {
	u8 state;
	u8 cmd;
	u16 addr;
	u16 len;
	u16 xor;
	u8 msg[BK96285_PGM_MSG_SIZE];
} _type;

typedef union {
	u8 value[BK96285_PGM_MSG_SIZE + BK96285_PGM_HEADER_SIZE];
	_type type;
} bk96285_pgm_pkt_type;
#pragma pack()

#endif /* _BK96285_CHIP_H_ */
