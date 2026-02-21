/* SPDX-License-Identifier: GPL-2.0 */
/*
 * hl7603.h
 *
 * hl7603 header
 *
 * Copyright (c) 2023-2023 Huawei Technologies Co., Ltd.
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

#ifndef _HL7603_H_
#define _HL7603_H_

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/of_device.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <chipset_common/hwpower/hardware_ic/buck_boost.h>

/* define ic state of bypass boost */
#define BYPASS_BST_IC_GOOD              0
#define BYPASS_BST_IC_BAD               1

#define HL7603_VOUT_SEL_DEFAULT         3450

/* register devid */
#define HL7603_DEVID_ADDR               0x00
#define HL7603_DEVID_MASK               0xf0
#define HL7603_DEVID_SHIFT              4

/* register config */
#define HL7603_CONFIG_ADDR              0x01
#define HL7603_DEV_EN_MASK              0x40
#define HL7603_DEV_EN_SHIFT             6
#define HL7603_MODE_MASK                0x30
#define HL7603_MODE_SHIFT               4
#define HL7603_CONFIG_DFLT              0x68

/* register vout_sel */
#define HL7603_VOUT_SEL_ADDR            0x02
#define HL7603_VOUT_PLIM_OFF_MASK       0x80
#define HL7603_VOUT_PLIM_OFF_SHIFT      7
#define HL7603_VOUT_LOOP_MODE_MASK      0x40
#define HL7603_VOUT_LOOP_MODE_SHIFT     6
#define HL7603_VOUT_SEL_MASK            0x3f
#define HL7603_VOUT_SEL_SHIFT           0
#define HL7603_VOUT_SEL_BASE            2850
#define HL7603_VOUT_SEL_STEP            50
#define HL7603_VOUT_SEL_MAX             5000
#define HL7603_VOUT_SEL_MIN             2850

/* register ilim_sel */
#define HL7603_ILIM_SEL_ADDR            0x03
#define HL7603_ILIM_SEL_MASK            0x0f
#define HL7603_ILIM_SEL_SHIFT           0
#define HL7603_ILIM_SEL_BASE            4000
#define HL7603_ILIM_SEL_STEP            500
#define HL7603_ILIM_SEL_MIN             4000
#define HL7603_ILIM_SEL_MAX             8500
#define HL7603_ILIM_SEL_DFLT            0x16
#define HL7603_ILIM_SEL_SBW1            0x96
#define HL7603_ILIM_SEL_HL_DFLT         0x06

#define HL7603_CONFIG2_ADDR_HL          0x04
#define HL7603_CONFIG2_HL_DFLT          0x0a

/* register state */
#define HL7603_STATE_ADDR_HL            0x05
#define HL7603_STATE_ADDR_JWT           0x04
#define HL7603_ABNORMAL_STATE_MASK      (BIT(7) | BIT(2) | BIT(1))
#define HL7603_ABNORMAL_STATE_MASK_JWT  (BIT(7) | BIT(2))
#define HL7603_STATE_POWER_GOOD_MASK    BIT(0)

#define HL7603_SHUTDOWN_CFG_GROUP       16
#define HL7603_SHUTDOWN_CFG_LEN         3
 
enum {
	HL7603_DEVID,
	HL7603_SHUTDOWN_MODE,
	HL7603_SHUTDOWN_VOLTAGE,
};

enum {
	HL7603_DEVID_HL = 0xb,
	HL7603_DEVID_JWT = 0x5,
	HL7603_DEVID_HLR = 0xc,
	HL7603_DEVID_SBW = 0x2,
	HL7603_DEVID_SBW1 = 0x3,
};

enum {
	HL7603_MODE_BEGIN = 0,
	HL7603_MODE_LOW_IQ = HL7603_MODE_BEGIN,
	HL7603_MODE_FORCE_BYPASS,
	HL7603_MODE_AUTO_BYPASS,
	HL7603_MODE_END,
};

struct hl7603_device_info {
	struct i2c_client *client;
	struct device *dev;
	struct buck_boost_ops bbst_ops;
	u32 const_vout;
	u32 init_mode;
	u8 dev_id;
	int ic_role;
	int gpio_no;
	int need_ctrl_mode;
	unsigned int shutdown_mode;
	u32 shutdown_voltage;
	int status;
	int adc_channel;
	int adc_coef;
	int ilimit;
	u32 init_reset_en;
	u32 bypass_ilim2_off;
};

#endif /* _HL7603_H_ */
