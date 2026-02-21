/**
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef __DP_PHY_CONFIG_INTERFACE_H__
#define __DP_PHY_CONFIG_INTERFACE_H__

#include <linux/kernel.h>

struct dp_ctrl;

struct phylane_mapping {
	int plug_type;
	int lane;
	int phylane;
};

struct usb31phy_mapping {
	uint32_t sw_level;
	uint32_t pe_level;
	bool need_config;
	uint32_t tx_vboot_addr_val;
	uint32_t u161n02_addr_val;
	uint32_t u161n03_addr_val;
};

struct combophy_rate {
	uint32_t rate_type;
	uint32_t ckgctrl;
	uint32_t sscginital;
	uint32_t sscgcnt;
	uint32_t sscgcnt2;
};

struct combophy_rate_csv2 {
	uint32_t rate_type;
	uint32_t ckplldiv;
	uint32_t divsel;
	uint32_t sscginital;
	uint32_t sscgstepsize1;
	uint32_t sscgstepsize2;
	uint32_t sscgstepnum;
};

struct dptx_phy_rate {
	uint32_t rate_type;
	uint32_t pll1_cofe0;
	uint32_t pll1_cofe1;
	uint32_t pll1_cofe2;
	uint32_t test_data1;
};

void dptx_phy_layer_init(struct dp_ctrl *dptx);

#endif /* DP_PHY_CONFIG_INTERFACE_H */
