/* SPDX-License-Identifier: GPL-2.0 */
/*
 * adapter_protocol_ufcs_constants.h
 *
 * ufcs protocol driver
 *
 * Copyright (c) 2025-2025 Huawei Technologies Co., Ltd.
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

#ifndef _ADAPTER_PROTOCOL_UFCS_CONSTANTS_H_
#define _ADAPTER_PROTOCOL_UFCS_CONSTANTS_H_

#define HWUFCS_POWER_READY_RETRY              14
#define HWUFCS_SEND_MSG_MAX_RETRY             3
#define HWUFCS_POWER_CURVE_SIZE               32

/*
 * message header data as below:
 * bit0~2: message type
 * bit3~8: protocol version
 * bit9~12: message number
 * bit13~15: device address
 */
#define HWUFCS_MSG_PROT_VERSION               0x1
#define HWUFCS_MSG_MAX_COUNTER                16
#define HWUFCS_MSG_MAX_BUFFER_SIZE            256
#define HWUFCS_MSG_VERSION_ID                 0x0001
#define HWUFCS_ADP_TYPE_FCR_66W               0x1

#define HWUFCS_HDR_MASK_MSG_TYPE              0x7
#define HWUFCS_HDR_MASK_PROT_VERSION          0x3f
#define HWUFCS_HDR_MASK_MSG_NUMBER            0xf
#define HWUFCS_HDR_MASK_DEV_ADDRESS           0x7

#define HWUFCS_HDR_HEADER_LEN                 2
#define HWUFCS_HDR_HEADER_H_OFFSET            0
#define HWUFCS_HDR_HEADER_L_OFFSET            1

#define HWUFCS_MSG_CMD_LEN                    1
#define HWUFCS_MSG_LENGTH_LEN                 1
#define HWUFCS_MSG_VER_ID_LEN                 2

/* control/data msg body */
#define HWUFCS_CTRL_DATA_MSG_MAX_BUF_SIZE     254
#define HWUFCS_CTRL_DATA_MSG_CMD_OFFSET       2
#define HWUFCS_CTRL_DATA_MSG_LEN_OFFSET       3
#define HWUFCS_CTRL_DATA_MSG_DATA_OFFSET      4

/* vendor defined msg body */
#define HWUFCS_VD_MSG_MAX_BUF_SIZE            251
#define HWUFCS_VD_MSG_VER_ID_H_OFFSET         2
#define HWUFCS_VD_MSG_VER_ID_L_OFFSET         3
#define HWUFCS_VD_MSG_MSG_LEN_OFFSET          4
#define HWUFCS_VD_MSG_CMD_H_OFFSET            5
#define HWUFCS_VD_MSG_CMD_L_OFFSET            6
#define HWUFCS_VD_MSG_DATA_OFFSET             5

#define HWUFCS_VDR_MASK_MSG_CMD               0x1fff
#define HWUFCS_VDR_MASK_MSG_CMD_TYPE          0x7

#define HWUFCS_VDR_HEADER_LEN                 2

/*
 * capabilities data message as below:
 * bit0~7: min output current
 * bit8~23: max output current
 * bit24~39: min output voltage
 * bit40~55: max output voltage
 * bit56: voltage adjust step
 * bit57~59: current adjust step
 * bit60~63: output mode
 */
#define HWUFCS_CAP_MASK_MIN_CURR              0xff
#define HWUFCS_CAP_MASK_MAX_CURR              0xffff
#define HWUFCS_CAP_MASK_MIN_VOLT              0xffff
#define HWUFCS_CAP_MASK_MAX_VOLT              0xffff
#define HWUFCS_CAP_MASK_VOLT_STEP             0x1
#define HWUFCS_CAP_MASK_CURR_STEP             0x7
#define HWUFCS_CAP_MASK_OUTPUT_MODE           0xf

#define HWUFCS_CAP_UNIT_CURR                  10 /* ma */
#define HWUFCS_CAP_UNIT_VOLT                  10 /* mv */

#define HWUFCS_CAP_MAX_OUTPUT_MODE            15

/*
 * output power range rules as follows:
 * 5V: 3.4V~5.5V
 * 10V: 5.5V~11V
 * 20V: 11V~21V
 * 30V: 21V~36V
 */
#define HWUFCS_5V_VOLT_MIN                    3400 /* mv */
#define HWUFCS_5V_VOLT_MAX                    5500 /* mv */
#define HWUFCS_10V_VOLT_MIN                   5500 /* mv */
#define HWUFCS_10V_VOLT_MAX                   11000 /* mv */
#define HWUFCS_20V_VOLT_MIN                   11000 /* mv */
#define HWUFCS_20V_VOLT_MAX                   21000 /* mv */
#define HWUFCS_30V_VOLT_MIN                   21000 /* mv */
#define HWUFCS_30V_VOLT_MAX                   36000 /* mv */

/*
 * request data message as below:
 * bit0~15: request output current
 * bit16~31: request output voltage
 * bit32~59: reserved
 * bit60~63: output mode
 */
#define HWUFCS_REQ_MASK_OUTPUT_CURR           0xffff
#define HWUFCS_REQ_MASK_OUTPUT_VOLT           0xffff
#define HWUFCS_REQ_MASK_OUTPUT_MODE           0xf

#define HWUFCS_REQ_BASE_OUTPUT_MODE           1
#define HWUFCS_REQ_MIN_OUTPUT_MODE            1
#define HWUFCS_REQ_MAX_OUTPUT_MODE            15

#define HWUFCS_REQ_UNIT_OUTPUT_CURR           10 /* ma */
#define HWUFCS_REQ_UNIT_OUTPUT_VOLT           10 /* mv */
#define HWUFCS_REQ_LVC_OUTPUT_VOLT            5000 /* mv */
#define HWUFCS_REQ_SC_OUTPUT_VOLT             10000 /* mv */
#define HWUFCS_REQ_SC4_OUTPUT_VOLT            20000 /* mv */

/*
 * source information data message as below:
 * bit0~15: current output current
 * bit16~31: current output voltage
 * bit32~39: current usb port temp
 * bit40~47: current device temp
 */
#define HWUFCS_SOURCE_INFO_MASK_OUTPUT_CURR   0xffff
#define HWUFCS_SOURCE_INFO_MASK_OUTPUT_VOLT   0xffff
#define HWUFCS_SOURCE_INFO_MASK_PORT_TEMP     0xff
#define HWUFCS_SOURCE_INFO_MASK_DEV_TEMP      0xff

#define HWUFCS_SOURCE_INFO_UNIT_CURR          10 /* ma */
#define HWUFCS_SOURCE_INFO_UNIT_VOLT          10 /* mv */

#define HWUFCS_SOURCE_INFO_TEMP_BASE          50 /* centigrade */

/*
 * sink information data message as below:
 * bit0~15: current charging current
 * bit16~31: current battery voltage
 * bit32~39: current usb temp
 * bit40~47: current battery temp
 */
#define HWUFCS_SINK_INFO_MASK_BAT_CURR        0xffff
#define HWUFCS_SINK_INFO_MASK_BAT_VOLT        0xffff
#define HWUFCS_SINK_INFO_MASK_USB_TEMP        0xff
#define HWUFCS_SINK_INFO_MASK_BAT_TEMP        0xff

#define HWUFCS_SINK_INFO_UNIT_CURR            10 /* ma */
#define HWUFCS_SINK_INFO_UNIT_VOLT            10 /* mv */

#define HWUFCS_SINK_INFO_TEMP_BASE            50 /* centigrade */

/*
 * cable information data message as below:
 * bit0~7: max loading current
 * bit8~15: max loading voltage
 * bit16~31: cable resistance
 * bit32~47: cable electronic lable vendor id
 * bit48~63: cable vendor id
 */
#define HWUFCS_CABLE_INFO_MASK_MAX_CURR       0xff
#define HWUFCS_CABLE_INFO_MASK_MAX_VOLT       0xff
#define HWUFCS_CABLE_INFO_MASK_RESISTANCE     0xffff
#define HWUFCS_CABLE_INFO_MASK_RESERVED_1     0xffff
#define HWUFCS_CABLE_INFO_MASK_RESERVED_2     0xffff

#define HWUFCS_CABLE_INFO_UNIT_CURR           1000 /* ma */
#define HWUFCS_CABLE_INFO_UNIT_VOLT           1000 /* mv */
#define HWUFCS_CABLE_INFO_UNIT_RESISTANCE     1000 /* mo */

/*
 * device information data message as below:
 * bit0~15: software version
 * bit16~31: hardware version
 * bit32~47: protocol chip vendor id
 * bit48~63: manufacture vendor id
 */
#define HWUFCS_DEV_INFO_MASK_SW_VER           0xffff
#define HWUFCS_DEV_INFO_MASK_HW_VER           0xffff
#define HWUFCS_DEV_INFO_MASK_RESERVED_1       0xffff
#define HWUFCS_DEV_INFO_MASK_RESERVED_2       0xffff

/*
 * error information data message as below:
 * bit0: output ovp
 * bit1: output uvp
 * bit2: output ocp
 * bit3: output scp
 * bit4: usb otp
 * bit5: device otp
 * bit6: cc ovp
 * bit7: d- ovp
 * bit8: d+ ovp
 * bit9: input ovp
 * bit10: input uvp
 * bit11: over leakage current
 * bit12: input drop
 * bit13: crc error
 * bit14: watchdog overflow
 */
#define HWUFCS_ERROR_INFO_MASK_OUTPUT_OVP     0x1
#define HWUFCS_ERROR_INFO_MASK_OUTPUT_UVP     0x1
#define HWUFCS_ERROR_INFO_MASK_OUTPUT_OCP     0x1
#define HWUFCS_ERROR_INFO_MASK_OUTPUT_SCP     0x1
#define HWUFCS_ERROR_INFO_MASK_USB_OTP        0x1
#define HWUFCS_ERROR_INFO_MASK_DEVICE_OTP     0x1
#define HWUFCS_ERROR_INFO_MASK_CC_OVP         0x1
#define HWUFCS_ERROR_INFO_MASK_DMINUS_OVP     0x1
#define HWUFCS_ERROR_INFO_MASK_DPLUS_OVP      0x1
#define HWUFCS_ERROR_INFO_MASK_INPUT_OVP      0x1
#define HWUFCS_ERROR_INFO_MASK_INPUT_UVP      0x1
#define HWUFCS_ERROR_INFO_MASK_OVER_LEAKAGE   0x1
#define HWUFCS_ERROR_INFO_MASK_INPUT_DROP     0x1
#define HWUFCS_ERROR_INFO_MASK_CRC_ERROR      0x1
#define HWUFCS_ERROR_INFO_MASK_WTG_OVERFLOW   0x1

#define HWUFCS_ERROR_INFO_NORMAL              0
#define HWUFCS_ERROR_INFO_ABNORMAL            1

/*
 * config watchdog data message as below:
 * bit0~15: watchdog overflow time
 */
#define HWUFCS_WTG_MASK_TIME                  0xffff

#define HWUFCS_WTG_UNIT_TIME                  1000 /* ms */

/*
 * refuse data message as below:
 * bit0~7: reason
 * bit8~15: command number
 * bit16~18: message type
 * bit24~27: message number
 */
#define HWUFCS_REFUSE_MASK_REASON             0xff
#define HWUFCS_REFUSE_MASK_CMD_NUMBER         0xff
#define HWUFCS_REFUSE_MASK_MSG_TYPE           0x7
#define HWUFCS_REFUSE_MASK_MSG_NUMBER         0xf

#define HWUFCS_REFUSE_REASON_NOT_IDENTIFY     0x1
#define HWUFCS_REFUSE_REASON_NOT_SUPPORT      0x2
#define HWUFCS_REFUSE_REASON_DEVICE_BUSY      0x3
#define HWUFCS_REFUSE_REASON_OVER_RANGE       0x4
#define HWUFCS_REFUSE_REASON_OTHER            0x5

/*
 * power change data message as below:
 * bit0~15: max output current
 * bit16~19: reserve byte
 * bit20~23: output mode
 */
#define HWUFCS_POWER_CHANGE_MASK_MAX_CURR     0xffff
#define HWUFCS_POWER_CHANGE_MASK_OUTPUT_MODE  0xf
#define HWUFCS_POWER_CHANGE_MAX_OUTPUT_MODE   7
#define HWUFCS_POWER_CHANGE_DATA_SIZE         3
#define HWUFCS_POWER_CHANGE_UNIT_CURR         10 /* ma */

/*
 * test request data message as below:
 * bit0~7: message cmd
 * bit8~10: message type
 * bit11~13: device address
 * bit14: voltage accuracy test mode
 */
#define HWUFCS_TEST_REQ_MASK_MSG_CMD          0xff
#define HWUFCS_TEST_REQ_MASK_MSG_TYPE         0x7
#define HWUFCS_TEST_REQ_MASK_DEV_ADDRESS      0x7
#define HWUFCS_TEST_REQ_MASK_VOLT_TEST_MODE   0x1
#define HWUFCS_TEST_REQ_MASK_EN_TEST_MODE     0x1

/*
 * vdm scpb power data message as below:
 * bit0~7: 5V power or min curr
 * bit8~23: 10V power or max curr
 * bit24~39: 15V power or min vol
 * bit40~55: 20V power or max vol
 * bit56~59: output mode number
 * bit60~63: power type:0:power gear,1:power curve
 */
#define HWUFCS_SCPB_MAX_PWR_MODE              15

#define HWUFCS_VDM_SCPB_PWR_MASK_PWR_5V       0xff
#define HWUFCS_VDM_SCPB_PWR_MASK_PWR_10V      0xffff
#define HWUFCS_VDM_SCPB_PWR_MASK_PWR_15V      0xffff
#define HWUFCS_VDM_SCPB_PWR_MASK_PWR_20V      0xffff

#define HWUFCS_VDM_SCPB_PWR_STEP              2 /* 0.5mw */

#define HWUFCS_VDM_SCPB_PWR_MASK_MIN_CURR     0xff
#define HWUFCS_VDM_SCPB_PWR_MASK_MAX_CURR     0xffff
#define HWUFCS_VDM_SCPB_PWR_MASK_MIN_VOLT     0xffff
#define HWUFCS_VDM_SCPB_PWR_MASK_MAX_VOLT     0xffff

#define HWUFCS_VDM_SCPB_PWR_UNIT_CURR         10 /* ma */
#define HWUFCS_VDM_SCPB_PWR_UNIT_VOLT         10 /* mv */

#define HWUFCS_VDM_SCPB_PWR_MASK_OUTPUT_MODE  0xf
#define HWUFCS_VDM_SCPB_PWR_MASK_PWR_TYPE     0xf

#endif /* _ADAPTER_PROTOCOL_UFCS_CONSTANTS_H_ */
