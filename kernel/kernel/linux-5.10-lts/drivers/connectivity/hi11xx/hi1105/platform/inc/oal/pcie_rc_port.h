/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description:pcie_interface.h header file
 * Author: @CompanyNameTag
 */

#ifndef PCIE_RC_PORT_H
#define PCIE_RC_PORT_H

#include "oal_types.h"
#include "oal_hardware.h"
#include "pcie_linux_mpxx.h"
#include "pcie_host_mpxx.h"

void oal_pcie_wlan_pm_vote(hcc_bus *hi_bus, uint8_t uc_allow);
void oal_pcie_deregister_event(oal_pcie_linux_res *pst_pci_lres);
void oal_pcie_register_event(oal_pci_dev_stru *pst_pci_dev, oal_pcie_linux_res *pst_pci_lres);
int32_t oal_pcie_pm_control(struct pci_dev *pdev, u32 rc_idx, int power_option);
int32_t oal_pcie_power_notifiy_register(struct pci_dev *pdev, u32 rc_idx, int (*poweron)(void *data),
                                        int (*poweroff)(void *data), void *data);
int32_t oal_pcie_host_aspm_init(oal_pcie_linux_res *pst_pci_lres);
int32_t oal_pcie_get_mps(oal_pci_dev_stru *pst_pcie_dev);
int32_t oal_pcie_mps_init(oal_pcie_linux_res *pst_pci_lres);
int32_t oal_pcie_enumerate(int32_t rc_idx);
int32_t oal_pcie_get_rc_link_quality_info(oal_pcie_linux_res *pst_pci_lres, uint32_t *d2h_ber);

#endif /* end for PCIE_RC_PORT_H */

