/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 *
 * scharger_v800_dc.h
 *
 * SchargerV800 direct charger header.
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

#ifndef __SCHARGER_V800_DC_H__
#define __SCHARGER_V800_DC_H__

#include <chipset_common/hwpower/common_module/power_event_ne.h>

#define ADAPTER_VENDOR_ID      0x82
#define ADAPTER_VENDOR_ID_SIZE 2
#define IWATT_ADAPTER_ID       0x80A2
#define ADAPTER_TYPE           0x8d
#define ADAPTER_CURRENT        0xc9
#define ADAPTER_CURRENT_UNIT   50

#define UCP_EN_DELAY 1000
#define IBUS_500MA   500
#define UCP_EN_COUNT 4

#define AFTER_REV_SC_DELAY 200

#ifndef unused
#define unused(x) ((void)(x))
#endif

struct scharger_dc {
	struct device *dev;
	struct regmap *regmap;
	struct notifier_block nb;
	enum chg_mode_state dc_chg_mode;
	bool iwatt_lvc_adap;
	bool dc_init_flag;
	unsigned int scharger_version;
	struct delayed_work ucp_en_work;
	int ucp_en_cnt;
	struct delayed_work after_rev_sc_work;
	unsigned int lvc_mos_support;
	unsigned int wired_rvs_support;
};

void sc_charge_disable_for_irq(void);
#endif