/**
 * Copyright (c) @CompanyNameMagicTag 2024-2024. All rights reserved.
 *
 * Description:oal_pcie_chip_mp12.c header file
 * Author: @CompanyNameTag
 */

#ifndef PCIE_CHIP_MP12_H
#define PCIE_CHIP_MP12_H

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE
#include "pcie_host.h"

int32_t oal_pcie_chip_info_init_mp12(oal_pcie_res *pst_pci_res, int32_t device_id);
int32_t oal_pcie_board_init_mp12(linkup_fixup_ops *ops);
#endif
#endif
