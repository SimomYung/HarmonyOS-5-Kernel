/* SPDX-License-Identifier: GPL-2.0 */
/*
 * sc8510.h
 *
 * sc8510 macro, addr etc.
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

#ifndef _SC8510_H_
#define _SC8510_H_

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/of.h>
#include <linux/of_device.h>

/* device id register */
#define SC8510_DEVICE_ID_REG                0x14
#define SC8510_DEVICE_ID                    0x10
#define SC8510_FWD_OCP_MAX                  0x0f
#define SC8510_AUDIO_EN                     1

#define SC8510_OCP_REG                      0x03
#define SC8510_FWD_OCP_MASK                 (BIT(0) | BIT(1) | BIT(2) | BIT(3))
#define SC8510_FWD_OCP_SHIFT                0

#define SC8510_AUDIO_MODE_REG               0x06
#define SC8510_AUDIO_MODE_MASK              BIT(2)
#define SC8510_AUDIO_MODE_SHIFT             2

/* fsw ctrl register */
#define SC8510_FSW_CTRL_REG                 0x07
#define SC8510_FIX_FREQ_EN_MASK             BIT(6)
#define SC8510_FIX_FREQ_EN_SHIFT            6

#define SGM41603A_DEVICE_ID                 0x09
#define SGM41603A_SCC_CFG1_REG              0x06
#define SGM41603A_FIX_FREQ_EN_MASK          BIT(0)
#define SGM41603A_FIX_FREQ_EN_SHIFT         0
#define SGM41603A_V2X_OVP_REG               0x08
#define SGM41603A_V2X_OVP_MASK              (BIT(3) | BIT(4) | BIT(5) | BIT(6) | BIT(7))
#define SGM41603A_V2X_OVP_SHIFT             3
#define SGM41603A_V2X_OVP_10500MV           0x16   /* 10.5v = 5v + 0.25v * 22(0x16) */

#define SGM41603A_MISC2_REG                 0x11
#define SGM41603A_BATFET_DET_DIS_MASK       BIT(7)
#define SGM41603A_BATFET_DET_DIS_SHIFT      7
#define SGM41603A_BATFET_DET_DIS            1

struct sc8510_dev_info {
	struct i2c_client *client;
	struct device *dev;
	struct notifier_block event_nb;
	u32 audio_mode_en;
	u32 fwd_ocp_max_en;
	u8 device_id;
};

#endif /* _SC8510_H_ */
