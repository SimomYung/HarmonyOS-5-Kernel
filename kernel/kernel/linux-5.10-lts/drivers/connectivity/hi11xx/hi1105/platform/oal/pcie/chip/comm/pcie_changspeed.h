/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description:pcie_pm.c header file
 * Author: @CompanyNameTag
 */

#ifndef PCIE_CHANGESPEED_HISI_H
#define PCIE_CHANGESPEED_HISI_H

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE
#include "pcie_host.h"

typedef struct _pcie_phy_adapter_ {
    char *ini_name;
    uint32_t reg;
    uint8_t pos;
    uint8_t bit;
    int32_t value;
} pcie_phy_adapter;

typedef int32_t (*pcie_phy_adapter_info)(pcie_phy_adapter **, uint32_t *);
typedef struct _board_phy_adapter_info_stru_ {
    int32_t chip_id;
    pcie_phy_adapter_info func;
} board_phy_adapter_info_stru;

void oal_pcie_link_speed_change(oal_pci_dev_stru *pst_pci_dev, oal_pcie_res *pst_pci_res);
void oal_pcie_device_phy_config_by_changespeed(oal_pcie_res *pst_pci_res, uint32_t base_addr);
#endif

#endif
