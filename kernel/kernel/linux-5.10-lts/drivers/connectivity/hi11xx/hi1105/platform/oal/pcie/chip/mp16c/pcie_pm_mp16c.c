/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description:pcie driver,synophys edma
 * Author: @CompanyNameTag
 */

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE
#define MPXX_LOG_MODULE_NAME "[PCIE_H]"
#define HISI_LOG_TAG           "[PCIE]"
#include "pcie_host.h"
#include "pcie_linux.h"
#include "mp16c/pcie_pcs_rb_regs.h"
#include "mp16c/pcie_ctrl_rb_regs.h"
#include "pcie_changspeed.h"
#include "pcie_chip.h"
#include "pcie_chip_mp16c.h"
#include "oam_ext_if.h"

#define GLB_CTL_RB_BASE                     (0x40000000)
#define GLB_CTL_RB_PCIE_AUX_CLK_SEL_REG     (0x98)
#define GLB_CTL_RB_GLB_CTL_GP_REG15_REG     (0x1C0C)
#define AUX_CLK_SWITCH_DBG                  (GLB_CTL_RB_GLB_CTL_GP_REG15_REG)
#define AUX_CLK_SWITCH_RTC                  (0xCDCD)
#define AUX_CLK_SWITCH_TCXO                 (0xDEDE)

#define PCIE_XFER_PENDING_VAL               (0x80)
#define PCIE_INTX_XFER_PENDING_SEL          (0x100)
#define W_CTL_BASE_ADDR                     (0x40105000)
#define W_CTL_RB_BUS_PROTECT_EN_OFF         (0x1464)
#define W_CTL_RB_PCIE_AXI_PROTECT_EN_MASK   (0x4)
#define MSTR_TO_HOST_ERR_INT_STS            (0x1718)
#define MSTR_TO_HOST_ERR_INT_CLR            (0x1714)
#define MSTR_TO_HOST_ERR_CLR                (0x170C)
#define MSTR_ERR_AW_BASE_ADDR0              (0x15A4)
#define MSTR_ERR_AW_BASE_ADDR1              (0x15A8)
#define MSTR_ERR_AW_BASE_ADDR2              (0x15AC)
#define MSTR_ERR_AW_BASE_ADDR3              (0x15B0)
#define MSTR_ERR_AR_BASE_ADDR0              (0x15B4)
#define MSTR_ERR_AR_BASE_ADDR1              (0x15B8)
#define MSTR_ERR_AR_BASE_ADDR2              (0x15BC)
#define MSTR_ERR_AR_BASE_ADDR3              (0x15C0)
#define EACH_MSTR_IP_RIGION                 (0x28)
#define MSTR_TO_HOST_ERR_INT_NUM            (8)

OAL_STATIC uint32_t g_lp_cfg_value = 0x8;    /* 0x8: S_MAC PCIE_LOW_POWER_REG default value */

OAL_STATIC int32_t oal_pcie_device_phy_config_mp16c(oal_pcie_res *pst_pci_res)
{
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)oal_pci_get_drvdata(pcie_res_to_dev(pst_pci_res));
    if (oal_likely(pst_pci_lres == NULL)) {
        return -OAL_EINVAL;
    }
    /* pcie0 phy寄存器优化 */
    oal_pcie_device_phy_config_by_changespeed(pst_pci_res, MP16C_PCIE_PHY_BASE_ADDRESS);
    /* ASPM L1sub power_on_time bias */
    oal_pcie_config_l1ss_poweron_time(pst_pci_lres->pst_pcie_dev, 0x0);

    g_lp_cfg_value = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_CTRL_RB_LOW_POWER_CFG_OFF);

    /* GEN1->GENx link speed change */
    (void)oal_pcie_link_speed_change(pst_pci_lres->pst_pcie_dev, pst_pci_res);
    return OAL_SUCC;
}

int32_t oal_pcie_device_auxclk_init_mp16(oal_pcie_res *pst_pci_res);
int32_t oal_pcie_device_aspm_init_mp13(oal_pcie_res *pst_pci_res);
void oal_pcie_device_aspm_ctrl_mp13(oal_pcie_res *pst_pci_res, oal_bool_enum clear);

OAL_STATIC void oal_pcie_monior_enable(oal_pcie_res *pst_pci_res, oal_bool_enum enable)
{
    /*
     * 发送睡眠请求时打开数采
     * 唤醒时判断建链状态有异常，通过ssi访问数采结果
     */
    if (enable == OAL_TRUE) {
        // 拉高xfer_pending信号
        oal_writel(PCIE_XFER_PENDING_VAL | g_lp_cfg_value,
                   pst_pci_res->pst_pci_ctrl_base + PCIE_CTRL_RB_LOW_POWER_CFG_OFF);
        pci_print_log(PCI_LOG_INFO, "pcie monitor enable value 0x%x",
                      oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_CTRL_RB_MONITOR_CFG0_OFF));
        oal_writel(0x3, pst_pci_res->pst_pci_ctrl_base + PCIE_CTRL_RB_DELAY_MONITOR_CFG_OFF);
        oal_writel(0x0, pst_pci_res->pst_pci_ctrl_base + PCIE_CTRL_RB_MONITOR_CFG0_OFF);
        // 使能
        oal_writel(0x1, pst_pci_res->pst_pci_ctrl_base + PCIE_CTRL_RB_MONITOR_CFG0_OFF);
        // 拉低xfer_pending信号
        oal_writel((g_lp_cfg_value & (~(PCIE_XFER_PENDING_VAL))),
                   pst_pci_res->pst_pci_ctrl_base + PCIE_CTRL_RB_LOW_POWER_CFG_OFF);
    } else {
        // pcie monitor result and disable, 当前在dev完成
        return;
    }
}

OAL_STATIC void wcpu_mstr_to_host_err_check(oal_pcie_res *pst_pci_res)
{
    int32_t ret;
    int32_t bit;
    pci_addr_map addr_map;
    uint32_t mstr_to_host_err_int_sts;
    uint64_t access_addr;

    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, W_CTL_BASE_ADDR, &addr_map);
    if (ret != OAL_SUCC) {
        return;
    }

    mstr_to_host_err_int_sts = oal_readl(addr_map.va + MSTR_TO_HOST_ERR_INT_STS);
    if (mstr_to_host_err_int_sts == 0) {
        return;
    }
    pci_print_log(PCI_LOG_ERR, "err_access in deepsleep, ID_reg = 0x%x", mstr_to_host_err_int_sts);
    oal_writel(mstr_to_host_err_int_sts, addr_map.va + MSTR_TO_HOST_ERR_INT_CLR);
    for (bit = 0; bit <= MSTR_TO_HOST_ERR_INT_NUM; bit++) {
        if (mstr_to_host_err_int_sts & (1 << bit)) {
            access_addr = (((uint64_t)oal_readl(addr_map.va + MSTR_ERR_AW_BASE_ADDR3 +
                           (bit * EACH_MSTR_IP_RIGION)) << 0x30) |
                           ((uint64_t)oal_readl(addr_map.va + MSTR_ERR_AW_BASE_ADDR2 +
                           (bit * EACH_MSTR_IP_RIGION)) << 0x20) |
                           ((uint64_t)oal_readl(addr_map.va + MSTR_ERR_AW_BASE_ADDR1 +
                           (bit * EACH_MSTR_IP_RIGION)) << 0x10) |
                           ((uint64_t)oal_readl(addr_map.va + MSTR_ERR_AW_BASE_ADDR0 +
                           (bit * EACH_MSTR_IP_RIGION))));
            pci_print_log(PCI_LOG_ERR, "id:%d, err waddr = 0x%llx", bit, access_addr);
            access_addr = (((uint64_t)oal_readl(addr_map.va + MSTR_ERR_AR_BASE_ADDR3 +
                           (bit * EACH_MSTR_IP_RIGION)) << 0x30) |
                           ((uint64_t)oal_readl(addr_map.va + MSTR_ERR_AR_BASE_ADDR2 +
                           (bit * EACH_MSTR_IP_RIGION)) << 0x20) |
                           ((uint64_t)oal_readl(addr_map.va + MSTR_ERR_AR_BASE_ADDR1 +
                           (bit * EACH_MSTR_IP_RIGION)) << 0x10) |
                           ((uint64_t)oal_readl(addr_map.va + MSTR_ERR_AR_BASE_ADDR0 +
                           (bit * EACH_MSTR_IP_RIGION))));
            pci_print_log(PCI_LOG_ERR, "id:%d, err raddr = 0x%llx", bit, access_addr);
        }
    }
    oal_writel(mstr_to_host_err_int_sts, addr_map.va + MSTR_TO_HOST_ERR_CLR);
}

OAL_STATIC int32_t oal_pcie_device_forbid_access_host_mp16c(oal_pcie_res *pst_pci_res, oal_bool_enum is_forbid)
{
    int32_t ret;
    uint32_t val;
    pci_addr_map addr_map;
    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, W_CTL_BASE_ADDR, &addr_map);
    if (ret != OAL_SUCC) {
        return -OAL_EFAIL;
    }

    val = oal_readl(addr_map.va + W_CTL_RB_BUS_PROTECT_EN_OFF);
    if (val == PCIE_INVALID_VALUE) {
        pci_print_log(PCI_LOG_ERR, "read dev_slave_forbid_reg invalid");
        return -OAL_EFAIL;
    }

    if (is_forbid == OAL_TRUE) {
        /* 打开slave口拦截 */
        oal_writel((val | W_CTL_RB_PCIE_AXI_PROTECT_EN_MASK), addr_map.va + W_CTL_RB_BUS_PROTECT_EN_OFF);
    } else {
        /* 此分支预留， 当前关闭动作在dev侧完成 */
        oal_writel((val & (~W_CTL_RB_PCIE_AXI_PROTECT_EN_MASK)), addr_map.va + W_CTL_RB_BUS_PROTECT_EN_OFF);
        wcpu_mstr_to_host_err_check(pst_pci_res);
    }

    return OAL_SUCC;
}

OAL_STATIC void oal_pcie_forbid_intx_pullup_xferpending_mp16c(oal_pcie_res *pst_pci_res, oal_bool_enum is_forbid)
{
    uint32_t val;
    uint32_t host_intr_sts;
    uint32_t ete_intr_sts;
    uint32_t ete_dr_empty_sts;
    val = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_CTRL_RB_PCIE_CFG_OFF);
    if (is_forbid == OAL_TRUE) {
        /* 禁止dev侧intx中断拉高xfer_pending */
        oal_writel((val & (~PCIE_INTX_XFER_PENDING_SEL)), pst_pci_res->pst_pci_ctrl_base + PCIE_CTRL_RB_PCIE_CFG_OFF);
    } else {
        host_intr_sts = oal_readl(pst_pci_res->ete_info.reg.host_intr_sts_addr);
        ete_intr_sts = oal_readl(pst_pci_res->ete_info.reg.ete_intr_sts_addr);
        ete_dr_empty_sts = oal_readl(pst_pci_res->ete_info.reg.ete_dr_empty_sts_addr);
        if ((host_intr_sts != 0) || (ete_intr_sts != 0) || (ete_dr_empty_sts != 0)) {
            pci_print_log(PCI_LOG_INFO, "excp intx isr, host_intr_sts %x, ete_intr_sts %x, ete_dr_empty_sts %x",
                          host_intr_sts, ete_intr_sts, ete_dr_empty_sts);
        }
        /* 允许dev侧intx中断拉高xfer_pending */
        oal_writel((val | PCIE_INTX_XFER_PENDING_SEL), pst_pci_res->pst_pci_ctrl_base + PCIE_CTRL_RB_PCIE_CFG_OFF);
    }
}

OAL_STATIC void oal_pcie_check_link_state_read_wctrl(oal_pcie_res *pst_pci_res)
{
    int32_t ret;
    uint32_t val;
    pci_addr_map addr_map;
    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, W_CTL_BASE_ADDR, &addr_map);
    if (ret != OAL_SUCC) {
        return;
    }

    val = oal_readl(addr_map.va);
    if (val != 0x10D) {     // w_ctrl ID default value is 0x10D
        pci_print_log(PCI_LOG_ERR, "in wakeup_complete, read w_ctrl_id = 0x%x", val);
        return;
    }
}

OAL_STATIC int32_t oal_pcie_devcie_pm_debug_mp16c(oal_pcie_res *pst_pci_res, oal_bool_enum in_sleep)
{
    /* 唤醒等待dev拉高gpio超时，retry时检查link_state以做保护 */
    if (pst_pci_res->link_state <= PCI_WLAN_LINK_UP) {
        return OAL_SUCC;
    }

    if (in_sleep == OAL_TRUE) {
        if (oal_get_pcie_forbid_dev_access_host_flag() != 0) {
            /* 打开slave口拦截 */
            oal_pcie_device_forbid_access_host_mp16c(pst_pci_res, OAL_TRUE);
        }
        oal_pcie_forbid_intx_pullup_xferpending_mp16c(pst_pci_res, OAL_TRUE);
        oal_pcie_monior_enable(pst_pci_res, OAL_TRUE);
    } else {
        oal_pcie_check_link_state_read_wctrl(pst_pci_res);     // 用于唤醒 cpl_timeout 问题的维测
        /* 此分支预留， 当前monitor结果检查及关闭动作在dev完成 */
        oal_pcie_check_link_state(pst_pci_res);
        oal_pcie_monior_enable(pst_pci_res, OAL_FALSE);
        oal_pcie_forbid_intx_pullup_xferpending_mp16c(pst_pci_res, OAL_FALSE);
        if (oal_get_pcie_forbid_dev_access_host_flag() != 0) {
            /* 检测slave口拦截结果 */
            oal_pcie_device_forbid_access_host_mp16c(pst_pci_res, OAL_FALSE);
        }
    }

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_auxclk_switch_mp16c(oal_pcie_res *pst_pci_res, aux_clk_mode clk_mode)
{
    int32_t ret;
    pci_addr_map addr_map;
    uint32_t cpu_address = GLB_CTL_RB_BASE;

    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, cpu_address, &addr_map);
    if (ret != OAL_SUCC) {
        oal_io_print("oal_pcie_auxclk_switch[%d], 0x%8x unmap, read failed!\n", clk_mode, cpu_address);
        return -OAL_ENOMEM;
    }

    // 回读一下寄存器，再配置拉高xfer_pending信号，使链路维持在L0状态
    g_lp_cfg_value = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_CTRL_RB_LOW_POWER_CFG_OFF);
    oal_writel(PCIE_XFER_PENDING_VAL | g_lp_cfg_value,
               pst_pci_res->pst_pci_ctrl_base + PCIE_CTRL_RB_LOW_POWER_CFG_OFF);
    if (clk_mode == AUXCLK_TCXO) {
        oal_writel(0x0, (void *)addr_map.va + GLB_CTL_RB_PCIE_AUX_CLK_SEL_REG);
        oal_writel(AUX_CLK_SWITCH_TCXO, (void *)addr_map.va + AUX_CLK_SWITCH_DBG);
    } else {
        pci_print_log(PCI_LOG_INFO, "pcie n_l1_rcvy_cnt[%d], l1_rcvy_cnt[%d]",
                      oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_CTRL_RB_PCIE_STAT_CNT_RCVRY_OFF),
                      oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_CTRL_RB_PCIE_STAT_CNT_L1_RCVRY_OFF));
        oal_writel(0x3, (void *)addr_map.va + GLB_CTL_RB_PCIE_AUX_CLK_SEL_REG);
        oal_writel(AUX_CLK_SWITCH_RTC, (void *)addr_map.va + AUX_CLK_SWITCH_DBG);
    }
    // 拉低xfer_pending信号
    oal_writel((g_lp_cfg_value & (~(PCIE_XFER_PENDING_VAL))),
               pst_pci_res->pst_pci_ctrl_base + PCIE_CTRL_RB_LOW_POWER_CFG_OFF);
    return OAL_SUCC;
}

OAL_STATIC void oal_pcie_enable_master_mp16c(oal_pcie_res *pst_pci_res, oal_pci_dev_stru *pst_pcie_dev,
                                             oal_bool_enum enable)
{
    // mp16/mp16c切32k特性不配置set/clear master
}

OAL_STATIC int32_t oal_pcie_enable_aspm_mp16c(oal_pcie_res *pst_pci_res)
{
    uint32_t reg = oal_readl(pst_pci_res->pst_pci_dbi_base + PCIE_ACK_F_ASPM_CTRL_OFF);
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
    oal_writel(reg, pst_pci_res->pst_pci_dbi_base + PCIE_ACK_F_ASPM_CTRL_OFF);
    pci_print_log(PCI_LOG_INFO, "ack aspm ctrl val:0x%x", reg);
#ifdef _PRE_COMMENT_CODE_
    reg = oal_readl(pst_pci_res->pst_pci_dbi_base + PCIE_AUX_CLK_FREQ_OFF);
    reg &= ~(0x3ff);
    reg |= (0x2 << 0); /* aux_clk 2m,actual 1.92M  38.4M/20 */
    oal_writel(reg, pst_pci_res->pst_pci_dbi_base + PCIE_AUX_CLK_FREQ_OFF);
    pci_print_log(PCI_LOG_INFO, "aux_clk_freq val:0x%x", reg);
#endif
    return OAL_SUCC;
}

int32_t pcie_pm_chip_init_mp16c(oal_pcie_res *pst_pci_res, int32_t device_id)
{
    pst_pci_res->chip_info.cb.pm_cb.pcie_device_aspm_init = oal_pcie_device_aspm_init_mp13;
    pst_pci_res->chip_info.cb.pm_cb.pcie_device_auxclk_init = oal_pcie_device_auxclk_init_mp16;
    pst_pci_res->chip_info.cb.pm_cb.pcie_device_aspm_ctrl = oal_pcie_device_aspm_ctrl_mp13;
    pst_pci_res->chip_info.cb.pm_cb.pcie_device_phy_config = oal_pcie_device_phy_config_mp16c;
    pst_pci_res->chip_info.cb.pm_cb.pcie_device_pm_debug = oal_pcie_devcie_pm_debug_mp16c;
    pst_pci_res->chip_info.cb.pm_cb.pcie_device_auxclk_switch = oal_pcie_auxclk_switch_mp16c;
    pst_pci_res->chip_info.cb.pm_cb.pcie_enable_master = oal_pcie_enable_master_mp16c;
    pst_pci_res->chip_info.cb.pm_cb.pcie_enable_aspm = oal_pcie_enable_aspm_mp16c;
    return OAL_SUCC;
}
#endif

