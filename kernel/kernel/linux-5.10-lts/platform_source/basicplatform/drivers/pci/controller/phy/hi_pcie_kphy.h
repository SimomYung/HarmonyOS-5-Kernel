/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: HiPCIe kphy interface define.
 *
 * This software is distributed under the terms of the GNU General
 * Public License ("GPL") as published by the Free Software Foundation,
 * either version 2 of that License or (at your option) any later version.
 */

#ifndef HI_PCIE_KPHY_H
#define HI_PCIE_KPHY_H

#include "hi_pcie_phy.h"
#include "../core/hi_pcie_core.h"

void pcie_kphy_dump_status(struct hi_pcie_phy *phy);
void pcie_kphy_setup(struct hi_pcie_phy *phy);
void pcie_kphy_setup_chs(struct hi_pcie_phy *phy);
int pcie_kphy_check_status(struct hi_pcie_phy *phy);
int pcie_kphy_cfg_ffe(struct hi_pcie_phy *phy);
int pcie_kphy_cfg_ffe_chs(struct hi_pcie_phy *phy);
int pcie_kphy_dfx_register(struct hi_pcie *pcie);
void pcie_kphy_pcs_clk_sel(struct hi_pcie_phy *phy);
void pcie_kphy_set_fom_param(u32 fom_pattern);
#endif
