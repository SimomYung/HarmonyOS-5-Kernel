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
#include "pcie_chip.h"
#include "pcie_chip_mp16.h"
#include "pcie_linux.h"
#include "mp16/pcie_ctrl_rb_regs.h"
#include "mp16/pcie_pcs_rb_regs.h"
#include "pcie_pcs_host.h"
#include "pcie_changspeed.h"

#define GLB_CTL_RB_BASE                     (0x40000000)
#define GLB_CTL_RB_PCIE_AUX_CLK_SEL_REG     (0xB4)

#define MP16_PCIE_PHY_PLLTMR0_OFFSET   0x0A24
#define MP16_PCIE_PHY_PLLTMR1_OFFSET   0x0A28
#define MP16_PCIE_PHY_TXDRVTMR_OFFSET  0x0A40
#define MP16_PCIE_PHY_RXEQTMR_OFFSET   0x0A50
#define MP16_PCIE_PHY_BG_CTRL_REG_0_OFFSET   0x0AC8
#define MP16_PCIE_MAC_PCI_ERR_UNCOR_STATUS 0x104 /* PCI_ERR_UNCOR_STATUS_CAP_ID: */
#define MP16_PCIE_MAC_PCI_ERR_COR_STATUS   0x110 /* PCI_ERR_COR_STATUS_CAP_ID: */

#define PCIE_XFER_PENDING_VAL               (0x80)

OAL_STATIC uint32_t g_lp_cfg_value = 0x8;    /* 0x8: S_MAC PCIE_LOW_POWER_REG default value */

typedef struct _pcie_auxclk_timer_config {
    uint32_t offset;
    uint32_t pos;
    uint32_t bits;
    uint32_t val;
} pcie_auxclk_timer_config;

OAL_STATIC pcie_auxclk_timer_config g_pcie_auxclk_switch_rtc[] = {
    { MP16_PCIE_PHY_PLLTMR0_OFFSET, 16, 13, 0x1 },      /* pll_ldo_etmr     [pos:16] [bits:13] */
    { MP16_PCIE_PHY_PLLTMR0_OFFSET, 0, 13, 0x1 },       /* pll_bias_etmr    [pos: 0] [bits:13] */
    { MP16_PCIE_PHY_PLLTMR1_OFFSET, 26, 6, 0x1 },       /* pll_clk_etmr     [pos:26] [bits: 6] */
    { MP16_PCIE_PHY_PLLTMR1_OFFSET, 12, 13, 0x2 },      /* pll_etmr         [pos:12] [bits:13] */
    { MP16_PCIE_PHY_PLLTMR1_OFFSET, 0, 9, 0x1 },        /* pll_vco_etmr     [pos: 0] [bits: 9] */
    { MP16_PCIE_PHY_TXDRVTMR_OFFSET, 20, 5, 0x1 },      /* txvcm_etmr       [pos:20] [bits: 5] */
    { MP16_PCIE_PHY_TXDRVTMR_OFFSET, 0, 18, 0x3 },      /* txhold_etmr      [pos: 0] [bits:18] */
    { MP16_PCIE_PHY_RXEQTMR_OFFSET, 0, 14, 0x1 },       /* rx_eq_tmr        [pos: 0] [bits:14] */
    { MP16_PCIE_PHY_BG_CTRL_REG_0_OFFSET, 0, 9, 0x1 }   /* bg_bias_en_etmr  [pos: 0] [bits: 9] */
};

OAL_STATIC pcie_auxclk_timer_config g_pcie_auxclk_switch_tcxo[] = {
    { MP16_PCIE_PHY_PLLTMR0_OFFSET, 16, 13, 0xC4 },     /* pll_ldo_etmr     [pos:16] [bits:13] */
    { MP16_PCIE_PHY_PLLTMR0_OFFSET, 0, 13, 0xC4 },      /* pll_bias_etmr    [pos: 0] [bits:13] */
    { MP16_PCIE_PHY_PLLTMR1_OFFSET, 26, 6, 0x2 },       /* pll_clk_etmr     [pos:26] [bits: 6] */
    { MP16_PCIE_PHY_PLLTMR1_OFFSET, 12, 13, 0x3C0 },    /* pll_etmr         [pos:12] [bits:13] */
    { MP16_PCIE_PHY_PLLTMR1_OFFSET, 0, 9, 0x18 },       /* pll_vco_etmr     [pos: 0] [bits: 9] */
    { MP16_PCIE_PHY_TXDRVTMR_OFFSET, 20, 5, 0x2 },      /* txvcm_etmr       [pos:20] [bits: 5] */
    { MP16_PCIE_PHY_TXDRVTMR_OFFSET, 0, 18, 0x783 },    /* txhold_etmr      [pos: 0] [bits:18] */
    { MP16_PCIE_PHY_RXEQTMR_OFFSET, 0, 14, 0xC0 },      /* rx_eq_tmr        [pos: 0] [bits:14] */
    { MP16_PCIE_PHY_BG_CTRL_REG_0_OFFSET, 0, 9, 0x61 }  /* bg_bias_en_etmr  [pos: 0] [bits: 9] */
};

int32_t oal_pcie_device_aspm_init_mp13(oal_pcie_res *pst_pci_res);
void oal_pcie_device_aspm_ctrl_mp13(oal_pcie_res *pst_pci_res, oal_bool_enum clear);

int32_t oal_pcie_device_phy_config_mp16(oal_pcie_res *pst_pci_res)
{
    oal_pcie_linux_res *pst_pci_lres = (oal_pcie_linux_res *)oal_pci_get_drvdata(pcie_res_to_dev(pst_pci_res));
    if (oal_likely(pst_pci_lres == NULL)) {
        return -OAL_EINVAL;
    }

    /* pcie0 phy寄存器优化 */
    oal_pcie_device_phy_config_single_lane(pst_pci_res, MP16_PA_PCIE0_PHY_BASE_ADDRESS);

    /* ASPM L1sub power_on_time bias */
    oal_pcie_config_l1ss_poweron_time(pst_pci_lres->pst_pcie_dev, 0x0);

    g_lp_cfg_value = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_CTRL_RB_LOW_POWER_CFG_OFF);

    /* GEN1->GENx link speed change */
    (void)oal_pcie_link_speed_change(pst_pci_lres->pst_pcie_dev, pst_pci_res);

    oal_pcie_device_phy_disable_l1ss_rekey(pst_pci_res, MP16_PA_PCIE0_PHY_BASE_ADDRESS);

#ifdef _PRE_PLAT_FEATURE_HI110X_DUAL_PCIE
    /* dual pcie */
    if (pst_pci_lres->pst_pcie_dev_dual != NULL) {
        /* pcie1 phy寄存器优化 */
        oal_pcie_device_phy_config_single_lane(pst_pci_res, MP16_PA_PCIE1_PHY_BASE_ADDRESS);

        /* ASPM L1sub power_on_time bias */
        oal_pcie_config_l1ss_poweron_time(pst_pci_lres->pst_pcie_dev_dual, 0x0);

        /* GEN1->GENx link speed change */
        (void)oal_pcie_link_speed_change(pst_pci_lres->pst_pcie_dev_dual, pst_pci_res);

        oal_pcie_device_phy_disable_l1ss_rekey(pst_pci_res, MP16_PA_PCIE1_PHY_BASE_ADDRESS);
    }
#endif

    return OAL_SUCC;
}

/* 时钟分频要在低功耗关闭下配置 */
int32_t oal_pcie_device_auxclk_init_mp16(oal_pcie_res *pst_pci_res)
{
    int32_t ret;
    uint32_t value;
    pci_addr_map st_map;

    value = oal_readl(pst_pci_res->pst_pci_dbi_base + PCIE_AUX_CLK_FREQ_OFF);
    /* 保留value bit[10]以上, aux_clk 1M, synophys set */
    value &= (~((1 << 10) - 1));
    value |= 0x1;
    oal_writel(value, pst_pci_res->pst_pci_dbi_base + PCIE_AUX_CLK_FREQ_OFF);

    /* tcxo 38.4M 39分频 = 0.98M 接近1M */
    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, (0x40000000 + 0x2c), &st_map);
    if (ret != OAL_SUCC) {
        oal_print_mpxx_log(MPXX_LOG_ERR, "wcpu address  convert failed, ret=%d", ret);
        return ret;
    }

    value = oal_readl((void *)st_map.va);
    value &= (~(((1 << 6) - 1) << 8)); // 6, 8, bit pos
    value |= (0x27 << 8); // 8, bit pos
    oal_writel(value, (void *)st_map.va);

    oal_print_mpxx_log(MPXX_LOG_INFO, "clk_freq reg:0x%x, freq_div reg:0x%x",
                       oal_readl(pst_pci_res->pst_pci_dbi_base + PCIE_AUX_CLK_FREQ_OFF),
                       oal_readl((void *)st_map.va));
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_auxclk_switch_phy_timer_adapter(oal_pcie_res *pst_pci_res, uint32_t clk_mode)
{
    int32_t ret;
    int32_t cnt;
    uint32_t reg;
    uint32_t base_addr = MP16_PA_PCIE0_PHY_BASE_ADDRESS;
    pci_addr_map addr_map;

    /* phy addr */
    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, base_addr, &addr_map);
    if (ret != OAL_SUCC) {
        return -OAL_EFAIL;
    }
    if (clk_mode == AUXCLK_TCXO) {
        for (cnt = 0; cnt < oal_array_size(g_pcie_auxclk_switch_tcxo); cnt++) {
            reg = oal_readl(addr_map.va + g_pcie_auxclk_switch_tcxo[cnt].offset);
            oal_setl_bits((void*)(addr_map.va + g_pcie_auxclk_switch_tcxo[cnt].offset),
                          g_pcie_auxclk_switch_tcxo[cnt].pos, g_pcie_auxclk_switch_tcxo[cnt].bits,
                          g_pcie_auxclk_switch_tcxo[cnt].val);
            pci_print_log(PCI_LOG_INFO, "PCIe phy reg config: change from 0x%x to 0x%x",
                          reg, oal_readl(addr_map.va + g_pcie_auxclk_switch_tcxo[cnt].offset));
        }
    } else {
        for (cnt = 0; cnt < oal_array_size(g_pcie_auxclk_switch_rtc); cnt++) {
            reg = oal_readl(addr_map.va + g_pcie_auxclk_switch_rtc[cnt].offset);
            oal_setl_bits((void*)(addr_map.va + g_pcie_auxclk_switch_rtc[cnt].offset),
                          g_pcie_auxclk_switch_rtc[cnt].pos, g_pcie_auxclk_switch_rtc[cnt].bits,
                          g_pcie_auxclk_switch_rtc[cnt].val);
            pci_print_log(PCI_LOG_INFO, "PCIe phy reg config: change from 0x%x to 0x%x",
                          reg, oal_readl(addr_map.va + g_pcie_auxclk_switch_rtc[cnt].offset));
        }
    }
    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_auxclk_switch_mp16(oal_pcie_res *pst_pci_res, aux_clk_mode clk_mode)
{
    int32_t ret;
    pci_addr_map addr_map;
    uint32_t cpu_address = GLB_CTL_RB_BASE;

    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, cpu_address, &addr_map);
    if (ret != OAL_SUCC) {
        oal_io_print("oal_pcie_auxclk_switch[%d], 0x%8x unmap, read failed!\n", clk_mode, cpu_address);
        return -OAL_ENOMEM;
    }

    if (clk_mode == AUXCLK_TCXO) {
        // rtc->tcxo, 不回读，使用保存的值，配置拉高xfer_pending信号，使链路维持在L0状态
        oal_writel(PCIE_XFER_PENDING_VAL | g_lp_cfg_value,
                   pst_pci_res->pst_pci_ctrl_base + PCIE_CTRL_RB_LOW_POWER_CFG_OFF);
        oal_pcie_auxclk_switch_phy_timer_adapter(pst_pci_res, AUXCLK_TCXO);
        oal_writel(0x0, (void *)addr_map.va + GLB_CTL_RB_PCIE_AUX_CLK_SEL_REG);
    } else {
        // tcxo->rtc,回读一下寄存器，再配置拉高xfer_pending信号，使链路维持在L0状态
        g_lp_cfg_value = oal_readl(pst_pci_res->pst_pci_ctrl_base + PCIE_CTRL_RB_LOW_POWER_CFG_OFF);
        oal_writel(PCIE_XFER_PENDING_VAL | g_lp_cfg_value,
                   pst_pci_res->pst_pci_ctrl_base + PCIE_CTRL_RB_LOW_POWER_CFG_OFF);
        oal_pcie_auxclk_switch_phy_timer_adapter(pst_pci_res, AUXCLK_RTC);
        oal_writel(0x3, (void *)addr_map.va + GLB_CTL_RB_PCIE_AUX_CLK_SEL_REG);
    }
    // 拉低xfer_pending信号
    oal_writel((g_lp_cfg_value & (~(PCIE_XFER_PENDING_VAL))),
               pst_pci_res->pst_pci_ctrl_base + PCIE_CTRL_RB_LOW_POWER_CFG_OFF);
    return OAL_SUCC;
}

OAL_STATIC void oal_pcie_enable_master_mp16(oal_pcie_res *pst_pci_res, oal_pci_dev_stru *pst_pcie_dev,
                                            oal_bool_enum enable)
{
    // mp16/mp16c切32k特性不配置set/clear master
}

int32_t pcie_pm_chip_init_mp16(oal_pcie_res *pst_pci_res, int32_t device_id)
{
    pst_pci_res->chip_info.cb.pm_cb.pcie_device_aspm_init = oal_pcie_device_aspm_init_mp13;
    pst_pci_res->chip_info.cb.pm_cb.pcie_device_auxclk_init = oal_pcie_device_auxclk_init_mp16;
    pst_pci_res->chip_info.cb.pm_cb.pcie_device_auxclk_switch = oal_pcie_auxclk_switch_mp16;
    pst_pci_res->chip_info.cb.pm_cb.pcie_device_aspm_ctrl = oal_pcie_device_aspm_ctrl_mp13;
    pst_pci_res->chip_info.cb.pm_cb.pcie_device_phy_config = oal_pcie_device_phy_config_mp16;
    pst_pci_res->chip_info.cb.pm_cb.pcie_enable_master = oal_pcie_enable_master_mp16;
    return OAL_SUCC;
}
#endif
