/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description:pcie driver
 * Author: @CompanyNameTag
 */

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE
/* HOST未适配RC Interface或不需要适配的, 将接口定义成空函数 */
#if (!(defined _PRE_CONFIG_HERTZ_PCIE) \
     && !(defined CONFIG_ARCH_KIRIN_PCIE) \
     && !(defined _PRE_CONFIG_ARCH_HI1620S_KUNPENG_PCIE))

#include "pcie_rc_port.h"

void oal_pcie_wlan_pm_vote(hcc_bus *hi_bus, uint8_t uc_allow)
{
}

void oal_pcie_deregister_event(oal_pcie_linux_res *pst_pci_lres)
{
}

void oal_pcie_register_event(oal_pci_dev_stru *pst_pci_dev, oal_pcie_linux_res *pst_pci_lres)
{
}

int32_t oal_pcie_pm_control(struct pci_dev *pdev, u32 rc_idx, int power_option)
{
    pci_print_log(PCI_LOG_WARN, "oal_pcie_pm_control not implement\n");
    return OAL_SUCC;
}

int32_t oal_pcie_power_notifiy_register(struct pci_dev *pdev, u32 rc_idx, int (*poweron)(void *data),
                                        int (*poweroff)(void *data), void *data)
{
    pci_print_log(PCI_LOG_WARN, "pci_power_notify_register not implement\n");
    return OAL_SUCC;
}

int32_t oal_pcie_host_aspm_init(oal_pcie_linux_res *pst_pci_lres)
{
    return OAL_SUCC;
}

int32_t oal_pcie_get_mps(oal_pci_dev_stru *pst_pcie_dev)
{
    return OAL_SUCC;
}

int32_t oal_pcie_mps_init(oal_pcie_linux_res *pst_pci_lres)
{
    return OAL_SUCC;
}

int32_t oal_pcie_enumerate(int32_t rc_idx)
{
    return OAL_SUCC;
}
#endif
#endif // _PRE_PLAT_FEATURE_HI110X_PCIE
