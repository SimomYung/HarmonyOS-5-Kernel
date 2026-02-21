/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description:pcie driver
 * Author: @CompanyNameTag
 */

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE
#ifdef CONFIG_ARCH_KIRIN_PCIE

#include "pcie_rc_port.h"
#include "external/lpcpu_feature.h"
#include "oam_ext_if.h"
#include "plat_pm.h"
#include "plat_exception_rst.h"

OAL_STATIC int32_t g_pcie_aspm_enable = 1;
oal_debug_module_param(g_pcie_aspm_enable, int, S_IRUGO | S_IWUSR);
OAL_STATIC int32_t g_pcie_performance_mode = 0;
oal_debug_module_param(g_pcie_performance_mode, int, S_IRUGO | S_IWUSR);

void oal_pcie_wlan_pm_vote(hcc_bus *hi_bus, uint8_t uc_allow)
{
#if defined(CONFIG_KIRIN_PCIE_L1SS_IDLE_SLEEP) || defined(CONFIG_PCIE_KPORT_IDLE) || defined(CONFIG_HIPCIE_IDLE)
    /* PCIe 32K feature, control rc fnpll clock */
    /* 0 means allow to fnpll gating */
    /* 1 means disallow to fnpll gating */
    kirin_pcie_refclk_device_vote(PCIE_DEVICE_WLAN, g_pcie_rc_idx, (uc_allow == ALLOW_IDLESLEEP) ? 0 : 1);
#else
    pci_print_log(PCI_LOG_INFO, "RC_refclk_device_vote interface not set");
#endif
}

#ifndef _PRE_HI375X_PCIE
void oal_pcie_linkdown_callback(struct kirin_pcie_notify *noti)
{
    oal_pcie_linux_res *pst_pci_lres = NULL;
    oal_pci_dev_stru *pst_pci_dev = (oal_pci_dev_stru *)noti->user;
    if (pst_pci_dev == NULL) {
        oal_io_print("pcie linkdown, pci dev is null!!\n");
        return;
    }

    pst_pci_lres = (oal_pcie_linux_res *)oal_pci_get_drvdata(pst_pci_dev);
    if (pst_pci_lres == NULL) {
        oal_io_print("pcie linkdown, lres is null\n");
        return;
    }

    if (pst_pci_lres->pst_bus == NULL) {
        oal_io_print("pcie linkdown, pst_bus is null\n");
        return;
    }

    oal_io_print("pcie dev[%s] [%d:%d] linkdown\n",
                 dev_name(&pst_pci_dev->dev), pst_pci_dev->vendor, pst_pci_dev->device);
    declare_dft_trace_key_info("pcie_link_down", OAL_DFT_TRACE_EXCEP);

    oal_pcie_change_link_state(pst_pci_lres->pst_pci_res, PCI_WLAN_LINK_DOWN);

    hcc_bus_exception_submit(pst_pci_lres->pst_bus, HCC_BUS_TRANS_FAIL);
#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
    hw_mpxx_dsm_client_notify(SYSTEM_TYPE_PLATFORM, DSM_WIFI_PCIE_LINKDOWN, "%s: pcie linkdown\n", __FUNCTION__);
#endif
    chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                         CHR_PLT_DRV_EVENT_PCIE, CHR_PLAT_DRV_ERROR_WKUP_GPIO_PCIE_LINK_DOWN);
}
#endif

void oal_pcie_deregister_event(oal_pcie_linux_res *pst_pci_lres)
{
#ifndef _PRE_HI375X_PCIE
    kirin_pcie_deregister_event(&pst_pci_lres->pcie_event);
#endif
    return;
}

void oal_pcie_register_event(oal_pci_dev_stru *pst_pci_dev, oal_pcie_linux_res *pst_pci_lres)
{
#ifndef _PRE_HI375X_PCIE
    pst_pci_lres->pcie_event.events = KIRIN_PCIE_EVENT_LINKDOWN;
    pst_pci_lres->pcie_event.user = pst_pci_dev;
    pst_pci_lres->pcie_event.mode = KIRIN_PCIE_TRIGGER_CALLBACK;
    pst_pci_lres->pcie_event.callback = oal_pcie_linkdown_callback;
    kirin_pcie_register_event(&pst_pci_lres->pcie_event);
#endif
    return;
}

int32_t oal_pcie_pm_control(struct pci_dev *pdev, u32 rc_idx, int power_option)
{
    int32_t ret;

    if (power_option == PCIE_POWER_ON) {
        wlan_pm_idle_sleep_vote(DISALLOW_IDLESLEEP);
    }

    ret = kirin_pcie_pm_control(power_option, rc_idx);
    pci_print_log(PCI_LOG_INFO, "host_pcie_pm_control ret=%d\n", ret);
    if ((power_option == PCIE_POWER_OFF_WITH_L3MSG) || (power_option == PCIE_POWER_OFF_WITHOUT_L3MSG)) {
        wlan_pm_idle_sleep_vote(ALLOW_IDLESLEEP);
    }
    return ret;
}

int32_t oal_pcie_power_notifiy_register(struct pci_dev *pdev, u32 rc_idx, int (*poweron)(void *data),
                                        int (*poweroff)(void *data), void *data)
{
    int32_t ret;

    ret = kirin_pcie_power_notifiy_register(rc_idx, poweron, poweroff, data);
    pci_print_log(PCI_LOG_INFO, "host_pcie_power_notifiy_register ret=%d\n", ret);
    return ret;
}

int32_t oal_pcie_host_aspm_init(oal_pcie_linux_res *pst_pci_lres)
{
    uint16_t val = 0;
    uint32_t reg;
    oal_pci_dev_stru *pst_rc_dev;
    pst_rc_dev = pci_upstream_bridge(pst_pci_lres->pst_pcie_dev);

    /* 使能/去使能ASPM，RC & EP */
    kirin_pcie_lp_ctrl(g_pcie_rc_idx, 0);

    if (pst_pci_lres->pst_pci_res != NULL) {
        oal_pcie_device_phy_config(pst_pci_lres->pst_pci_res);
    }

    if (g_pcie_aspm_enable) {
        /* L1SS config */
        if ((pst_pci_lres->pst_pci_res != NULL) && (pst_pci_lres->pst_pci_res->pst_pci_dbi_base != NULL)) {
            reg = oal_readl(pst_pci_lres->pst_pci_res->pst_pci_dbi_base + PCIE_ACK_F_ASPM_CTRL_OFF);
#ifdef _PRE_COMMENT_CODE_
            /* bit[26:24]->0x0, bit[29:27]->0x0, 保留除L0s, L1外的其他位 */
            reg &= ~((0x7 << 24) | (0x7 << 27));
#endif
            reg &= ~((0x7 << 27)); /* bit[29:27]->0x0 保留L1外的其他位 */
#ifdef _PRE_COMMENT_CODE_
            /* L0s bit[26:24] set 0x7->7us entry,L1 bit[29:27] set 0x7->64us entery, tx & rx */
            reg |= (0x7 << 24) | (0x7 << 27);
#endif
            reg |= (0x3 << 27); /* bit[29:27] set 0x3-> L1 8us entry */
            oal_writel(reg, pst_pci_lres->pst_pci_res->pst_pci_dbi_base + PCIE_ACK_F_ASPM_CTRL_OFF);
            pci_print_log(PCI_LOG_INFO, "ack aspm ctrl val:0x%x", reg);
#ifdef _PRE_COMMENT_CODE_
            reg = oal_readl(pst_pci_lres->pst_pci_res->pst_pci_dbi_base + PCIE_AUX_CLK_FREQ_OFF);
            reg &= ~(0x3ff);
            reg |= (0x2 << 0); /* aux_clk 2m,actual 1.92M  38.4M/20 */
            oal_writel(reg, pst_pci_lres->pst_pci_res->pst_pci_dbi_base + PCIE_AUX_CLK_FREQ_OFF);
            pci_print_log(PCI_LOG_INFO, "aux_clk_freq val:0x%x", reg);
#endif
        } else {
            pci_print_log(PCI_LOG_ERR, "pci res null or ctrl_base is null");
        }
        kirin_pcie_lp_ctrl(g_pcie_rc_idx, 1);
    }
    pci_print_log(PCI_LOG_INFO, "g_pcie_aspm_enable:%d", g_pcie_aspm_enable);

    /* rc noc protect */
    if (pst_rc_dev != NULL) {
        oal_pci_read_config_word(pst_rc_dev, oal_pci_pcie_cap(pst_rc_dev) + PCI_EXP_DEVCTL2, &val);
#ifdef _PRE_COMMENT_CODE_
        /* 调整rc pcie rsp 超时时间，对kirin noc有影响 */
        val |= (0xe << 0);
        oal_pci_write_config_word(pst_rc_dev, oal_pci_pcie_cap(pst_rc_dev) + PCI_EXP_DEVCTL2, val);
#endif
        pci_print_log(PCI_LOG_INFO, "devctrl:0x%x", val);
    }

    return OAL_SUCC;
}

int32_t oal_pcie_get_mps(oal_pci_dev_stru *pst_pcie_dev)
{
    int32_t ret;
    int32_t pos;
    uint16_t reg16 = 0;

    pos = pci_find_capability(pst_pcie_dev, PCI_CAP_ID_EXP);
    if (!pos) {
        return -1;
    }

    ret = oal_pci_read_config_word(pst_pcie_dev, pos + PCI_EXP_DEVCAP, &reg16);
    if (ret) {
        pci_print_log(PCI_LOG_ERR, "pci read devcap failed ret=%d", ret);
        return -1;
    }

    return (int32_t)(reg16 & PCI_EXP_DEVCAP_PAYLOAD);
}

/* Max Payload Size Supported,  must config beofre pcie access */
int32_t oal_pcie_mps_init(oal_pcie_linux_res *pst_pci_lres)
{
    int32_t rc_mps, ep_mps;
    uint32_t mps, mrq;
    oal_pci_dev_stru *pst_rc_dev = NULL;
    oal_pci_dev_stru *pst_ep_dev = pst_pci_lres->pst_pcie_dev;
    /* 默认128, 开启后可协商256, 一次Burst多一倍对峰值性能有收益 */
    if (!g_pcie_performance_mode) {
        return OAL_SUCC;
    }

    pst_rc_dev = pci_upstream_bridge(pst_ep_dev);
    if (pst_rc_dev == NULL) {
        pci_print_log(PCI_LOG_ERR, "no upstream dev");
        return -OAL_ENODEV;
    }

    rc_mps = oal_pcie_get_mps(pst_rc_dev);
    ep_mps = oal_pcie_get_mps(pst_ep_dev);
    if (rc_mps < 0 || ep_mps < 0) {
        pci_print_log(PCI_LOG_ERR, "mps get failed, rc_mps:%d  , ep_mps:%d", rc_mps, ep_mps);
        return -OAL_EFAIL;
    }

    mps = (uint32_t)oal_min(rc_mps, ep_mps);
    mrq = mps;
    pci_print_log(PCI_LOG_INFO, "rc/ep max payload size, rc_mps:%d  , ep_mps:%d, select mps:%d bytes",
                  rc_mps, ep_mps, OAL_PCIE_MIN_MPS << mps);
    mps <<= 5;  /* PCI_EXP_DEVCTL_PAYLOAD mps << 5 = 0b1110 0000 */
    mrq <<= 12; /* PCI_EXP_DEVCTL_READRQ  mrq << 12 = 0b0111 0000 0000 0000 */

    pcie_capability_clear_and_set_word(pst_ep_dev, PCI_EXP_DEVCTL, PCI_EXP_DEVCTL_READRQ, mrq);
    pcie_capability_clear_and_set_word(pst_ep_dev, PCI_EXP_DEVCTL, PCI_EXP_DEVCTL_PAYLOAD, mps);
    pcie_capability_clear_and_set_word(pst_rc_dev, PCI_EXP_DEVCTL, PCI_EXP_DEVCTL_PAYLOAD, mps);
    pcie_capability_clear_and_set_word(pst_rc_dev, PCI_EXP_DEVCTL, PCI_EXP_DEVCTL_READRQ, mrq);
    return OAL_SUCC;
}

int32_t oal_pcie_enumerate(int32_t rc_idx)
{
    int32_t ret = -OAL_ENODEV;
    oal_io_print("notify host to scan rc:%d\n", rc_idx);
    ret = kirin_pcie_enumerate(rc_idx);

    return ret;
}

#endif // CONFIG_ARCH_KIRIN_PCIE
#endif // _PRE_PLAT_FEATURE_HI110X_PCIE
