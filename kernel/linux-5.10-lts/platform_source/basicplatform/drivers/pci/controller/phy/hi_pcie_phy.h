/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: HiPCIe phy interface define.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#ifndef HI_PCIE_PHY_H
#define HI_PCIE_PHY_H

#include <linux/types.h>

struct hi_pcie_phy_change {
	u32 *change_param;
	u32 *change_param_addr;
	u32 change_param_size;
};

struct hi_pcie_phy {
	void __iomem *phy_base;
	u32 link_num;
	u32 lane_num;
	struct hi_pcie_phy_change phy_change;
};

struct hi_pcie_phy_ops {
	u32 (*phy_bit_read)(struct hi_pcie_phy *phy, u32 reg, u32 msb, u32 lsb);
	void (*phy_bit_write)(struct hi_pcie_phy *phy, u32 reg, u32 msb, u32 lsb,
				u32 val);
};

enum lpbk_mode {
	INNER_LOOPBACK = 0,
	MAC_TX2RX_LPBK,
	AD_BIST_INNER_LPBK,
	AD_BIST_DIG_INNER_LPBK,
	AD_BIST,
};

enum eyefom_mode {
	COMMON_EYEFOM = 0,
	DEFAULT_EYEFOM,
	DIG_EYEFORM,
};

struct hi_pcie_phy_dfx_ops {
	int (*phy_eye_pattern)(struct hi_pcie_phy *phy, enum eyefom_mode mode);
	int (*phy_param_dump)(struct hi_pcie_phy *phy);
	int (*phy_tx_detect_rx)(struct hi_pcie_phy *phy);
	int (*phy_param_change)(struct hi_pcie_phy *phy);
	int (*phy_trace_data_init)(struct hi_pcie_phy *phy, u32 tbd0, u32 tbd1, u32 tbd2, u32 tbd3);
	int (*phy_trace_data_save)(struct hi_pcie_phy *phy);
	int (*phy_trace_data_dump)(struct hi_pcie_phy *phy);
	int (*phy_enter_loopback)(struct hi_pcie_phy *phy, enum lpbk_mode mode);
	int (*phy_send_pattern)(struct hi_pcie_phy *phy, u32 patt0, u32 patt1, u32 patt2, u32 patt3);
};

#define drv_bit(n) ((u64)((u64)0x1 << (n)))
#define drv_32bit_mask(n1, n2) (drv_bit((u64)(n2) + 1) - drv_bit((u64)(n1)))
#define drv_32bit_read_field(v, n1, n2) ((u32)(((u64)(v) & drv_32bit_mask((n1), (n2))) >> (n1)))
#define drv_32bit_set_field(v1, n1, n2, v2) ((u32)(((u64)(v1) & (~drv_32bit_mask((n1), (n2)))) | \
		(((u64)(v2) << (u64)(n1)) & drv_32bit_mask((n1), (n2)))))
#endif
