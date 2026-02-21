/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description:pcie driver
 * Author: @CompanyNameTag
 */

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE
#ifdef _PRE_CONFIG_ARCH_HI1620S_KUNPENG_PCIE

#include "pcie_rc_port.h"
#include "external/lpcpu_feature.h"
#include "oam_ext_if.h"
#include "plat_pm.h"
#include "plat_exception_rst.h"

int pci_power_notify_register(struct pci_dev *pdev, int (*poweron)(void* data),
                              int (*poweroff)(void* data), void* data);
int pci_rp_power_ctrl(struct pci_dev *pdev, int power_option);
int pci_dev_re_enumerate(struct pci_dev *pdev);

void oal_pcie_wlan_pm_vote(hcc_bus *hi_bus, uint8_t uc_allow)
{
}

void oal_pcie_deregister_event(oal_pcie_linux_res *pst_pci_lres)
{
    return;
}

void oal_pcie_register_event(oal_pci_dev_stru *pst_pci_dev, oal_pcie_linux_res *pst_pci_lres)
{
    return;
}

int32_t oal_pcie_pm_control(struct pci_dev *pdev, u32 rc_idx, int power_option)
{
    int32_t ret;

    if (pdev == NULL) {
        return -OAL_EFAIL;
    }

    ret = pci_rp_power_ctrl(pdev, power_option);
    pci_print_log(PCI_LOG_INFO, "pci_rp_power_ctrl ret=%d\n", ret);
    return ret;
}

int32_t oal_pcie_power_notifiy_register(struct pci_dev *pdev, u32 rc_idx, int (*poweron)(void *data),
                                        int (*poweroff)(void *data), void *data)
{
    int32_t ret;

    if (pdev == NULL) {
        return -OAL_EFAIL;
    }

    ret = pci_power_notify_register(pdev, poweron, poweroff, data);
    pci_print_log(PCI_LOG_INFO, "pci_power_notify_register ret=%d\n", ret);
    return ret;
}

int32_t oal_pcie_host_aspm_init(oal_pcie_linux_res *pst_pci_lres)
{
    if (pst_pci_lres->pst_pci_res != NULL) {
        oal_pcie_device_phy_config(pst_pci_lres->pst_pci_res);
    }
    return OAL_SUCC;
}

int32_t oal_pcie_get_mps(oal_pci_dev_stru *pst_pcie_dev)
{
    return OAL_SUCC;
}

/* Max Payload Size Supported,  must config beofre pcie access */
int32_t oal_pcie_mps_init(oal_pcie_linux_res *pst_pci_lres)
{
    return OAL_SUCC;
}

int32_t oal_pcie_enumerate(int32_t rc_idx)
{
    return OAL_SUCC;
}

int32_t oal_pcie_get_rc_link_quality_info(oal_pcie_linux_res *pst_pci_lres, uint32_t *d2h_ber)
{
    *d2h_ber = 0;
    return OAL_SUCC;
}
#endif // _PRE_CONFIG_ARCH_HI1620S_KUNPENG_PCIE
#endif // _PRE_PLAT_FEATURE_HI110X_PCIE
