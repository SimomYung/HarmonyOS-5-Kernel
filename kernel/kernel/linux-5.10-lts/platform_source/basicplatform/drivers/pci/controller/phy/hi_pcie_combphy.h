/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: HiPCIe kphy interface define.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#ifndef HI_PCIE_COMBPHY_H
#define HI_PCIE_COMBPHY_H

#include "hi_pcie_phy.h"
#include "../core/hi_pcie_core.h"

void pcie_combphy_load_firmware(struct hi_pcie_phy *phy,
	u32 type, u32 *firmware, u32 depth);
void pcie_combphy_core_freq_cfg(struct hi_pcie_phy * phy);
void pcie_combphy_setup(struct hi_pcie_phy *phy);
int pcie_combphy_check_lane(struct hi_pcie_phy *phy);
int pcie_combphy_dfx_register(struct hi_pcie *pcie);
void pcie_combphy_sel_clk(struct hi_pcie_phy *phy);
void pcie_combphy_set_fom_param(u32 fom_pattern);
#endif
