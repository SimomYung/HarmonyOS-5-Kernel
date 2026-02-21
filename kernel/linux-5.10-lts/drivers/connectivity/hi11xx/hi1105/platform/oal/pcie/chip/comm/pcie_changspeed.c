/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description:pcie chip adaptation
 * Author: @CompanyNameTag
 */

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE
#define MPXX_LOG_MODULE_NAME "[PCIE_H]"
#define HISI_LOG_TAG           "[PCIE]"

#include "pcie_chip.h"
#include "pcie_linux.h"
#include "pcie_pm.h"
#include "board.h"
#include "mp16/host_ctrl_rb_regs.h"
#include "mp16/pcie_ctrl_rb_regs.h"
#include "mp16/pcie_pcs_rb_regs.h"
#include "pcie_pcs_trace.h"
#include "oam_ext_if.h"
#include "pcie_changspeed.h"

/* mac优化寄存器 */
#define PCI_EXP_LNKCTL2_SPEED_MASK  0xf
#define PCI_EXP_SPEED_CHANGE_MASK   0x20000

int32_t g_pcie_phy_changespeed_adapter_enable = 0x7FFFFFFF;
oal_debug_module_param(g_pcie_phy_changespeed_adapter_enable, int, S_IRUGO | S_IWUSR);
int32_t g_pcie_phy_ssi_adapter_enable = 0x7FFFFFFF;
oal_debug_module_param(g_pcie_phy_ssi_adapter_enable, int, S_IRUGO | S_IWUSR);

int32_t oal_pcie_get_phy_adapter_info_mp16c(pcie_phy_adapter **phy_adapter, uint32_t *phy_adapter_size);
int32_t oal_pcie_get_phy_adapter_info_mp17c(pcie_phy_adapter **phy_adapter, uint32_t *phy_adapter_size);
int32_t oal_pcie_get_phy_adapter_info_mp12(pcie_phy_adapter **phy_adapter, uint32_t *phy_adapter_size);
static board_phy_adapter_info_stru g_board_phy_adapter_info[] = {
    {BOARD_VERSION_MP16C, oal_pcie_get_phy_adapter_info_mp16c},
    {BOARD_VERSION_MP17C, oal_pcie_get_phy_adapter_info_mp17c},
    {BOARD_VERSION_MP12,  oal_pcie_get_phy_adapter_info_mp12},
};

int32_t oal_pcie_get_phy_adapter_info(pcie_phy_adapter **phy_adapter, uint32_t *phy_adapter_size)
{
    uint32_t i;
    int32_t chip_id = get_mpxx_subchip_type();
    uint32_t chip_num = (uint32_t)oal_array_size(g_board_phy_adapter_info);
    for (i = 0; i < chip_num; i++) {
        if (g_board_phy_adapter_info[i].chip_id == chip_id) {
            if (g_board_phy_adapter_info[i].func != NULL) {
                oal_print_mpxx_log(MPXX_LOG_INFO, "pcie board get phy adapter info 0x%x succ", chip_id);
                return g_board_phy_adapter_info[i].func(phy_adapter, phy_adapter_size);
            } else {
                oal_print_mpxx_log(MPXX_LOG_INFO, "pcie board phy adapter info 0x%x null func", chip_id);
                return OAL_SUCC;
            }
        }
    }
    oal_print_mpxx_log(MPXX_LOG_INFO, "not implement chip board init device_id=0x%x", chip_id);
    return -OAL_ENODEV;
}

void oal_pcie_device_phy_config_by_ssi(uint32_t base_addr)
{
    uint32_t count;
    pcs_reg_ops reg_ops = { 0 };
    pcie_phy_adapter *phy_adapter = NULL;
    uint32_t phy_adapter_size = 0;
    pcie_pcs_set_regs_ops(&reg_ops, PCIE_PCS_DUMP_BY_SSI);
    oal_pcie_get_phy_adapter_info(&phy_adapter, &phy_adapter_size);

    for (count = 0; count < phy_adapter_size; count++) {
        if ((uint32_t)g_pcie_phy_ssi_adapter_enable & (1 << count)) {
            if (phy_adapter[count].ini_name != NULL &&
                get_cust_conf_int32(INI_MODU_PLAT, phy_adapter[count].ini_name,
                &phy_adapter[count].value) == INI_FAILED) {
                pci_print_log(PCI_LOG_INFO, "pcie_phy_adapter[%d].ini_name not set", count);
                return;
            }
            pcie_pcs_set_bits32(&reg_ops, (base_addr + phy_adapter[count].reg), phy_adapter[count].pos,
                                phy_adapter[count].bit, phy_adapter[count].value);
            pci_print_log(PCI_LOG_INFO, "by ssi pcie_phy_adapter[%d] enable", count);
        }
    }
}

void oal_pcie_device_phy_config_by_changespeed(oal_pcie_res *pst_pci_res, uint32_t base_addr)
{
    uint32_t count;
    pcie_phy_adapter *phy_adapter = NULL;
    uint32_t phy_adapter_size = 0;
    oal_pcie_get_phy_adapter_info(&phy_adapter, &phy_adapter_size);

    for (count = 0; count < phy_adapter_size; count++) {
        if ((uint32_t)g_pcie_phy_changespeed_adapter_enable & (1 << count)) {
            if (phy_adapter[count].ini_name != NULL &&
                get_cust_conf_int32(INI_MODU_PLAT, phy_adapter[count].ini_name,
                &phy_adapter[count].value) == INI_FAILED) {
                pci_print_log(PCI_LOG_INFO, "pcie_phy_adapter[%d].ini_name not set", count);
                return;
            }
            oal_pcie_device_phy_config_reg(pst_pci_res, (base_addr + phy_adapter[count].reg),
                                           phy_adapter[count].pos, phy_adapter[count].bit,
                                           phy_adapter[count].value);
            pci_print_log(PCI_LOG_INFO, "by changespeed pcie_phy_adapter[%d] enable", count);
        }
    }
}

static void oal_pcie_change_speed_before_pcs(oal_pci_dev_stru *pst_pci_dev, oal_pcie_res *pst_pci_res,
                                             void *pcie_ctrl)
{
    pci_print_log(PCI_LOG_INFO,
                  "before: non_l1_rcvry_cnt:0x%x l1_rcvry_cnt:0x%x stat_monitor1:0x%x stat_monitor2:0x%x",
                  oal_readl(pcie_ctrl + PCIE_CTRL_RB_PCIE_STAT_CNT_RCVRY_OFF),
                  oal_readl(pcie_ctrl + PCIE_CTRL_RB_PCIE_STAT_CNT_L1_RCVRY_OFF),
                  oal_readl(pcie_ctrl + PCIE_CTRL_RB_PCIE_STAT_MONITOR1_OFF),
                  oal_readl(pcie_ctrl + PCIE_CTRL_RB_PCIE_STAT_MONITOR2_OFF));
    oal_writel(0xFFFFFFFF, (pcie_ctrl + PCIE_CTRL_RB_PCIE_STAT_MONITOR2_OFF));

    if (pcs_trace_speedchange_flag() != 0) {
        pcie_pcs_tracer_start(oal_pcie_get_pcs_paddr(pst_pci_dev, pst_pci_res, PCIE_PCS_DUMP_BY_MEMPORT),
                              PCS_TRACER_SPEED_CHANGE, 0, PCIE_PCS_DUMP_BY_MEMPORT);
    }
}

static void oal_pcie_change_speed_rc_mac_adapter(oal_pci_dev_stru *pst_rc_dev, int32_t *ep_l1ss_pm)
{
    uint32_t val;
    uint32_t lnkctl_val;
    /* debug mac reg before speed-change,bit[19:16]是软件切换后的链路速率 */
    oal_pci_read_config_dword(pst_rc_dev, *ep_l1ss_pm + PCI_EXP_LNKCTL, &lnkctl_val);
    pci_print_log(PCI_LOG_INFO, "before: rc mac addr:0x%x PCI_EXP_LNKCTL:0x%x", PCI_EXP_LNKCTL, lnkctl_val);

    /* 配置TARGET_LINK_SPEED,bit[3:0]是软件切换配置的目标速率,bit[20:17]对应EQ状态 */
    oal_pci_read_config_dword(pst_rc_dev, *ep_l1ss_pm + PCI_EXP_LNKCTL2, &val);
    pci_print_log(PCI_LOG_INFO, "before: rc mac addr:0x%x PCI_EXP_LNKCTL2:0x%x", PCI_EXP_LNKCTL2, val);
    val &= ~PCI_EXP_LNKCTL2_SPEED_MASK;
    val |= (uint32_t)(g_pcie_speed_change + 1);
    oal_pci_write_config_dword(pst_rc_dev, *ep_l1ss_pm + PCI_EXP_LNKCTL2, val);

#ifdef CONFIG_HIPCIE_CTRL
    lnkctl_val |= BIT5; /* 自研IP PCI_EXP_LNKCTL BIT5为retrain_link */
    oal_pci_write_config_dword(pst_rc_dev, *ep_l1ss_pm + PCI_EXP_LNKCTL, lnkctl_val);
    pci_print_log(PCI_LOG_INFO, "self pcie ip retrain_link");
#else
    /* 配置DIRECT_SPEED_CHANGE,bit[17]直接切速; 不用配置ep进行RETRAIN */
    oal_pci_read_config_dword(pst_rc_dev, *ep_l1ss_pm + PCI_GEN2_CTRL, &val);
    pci_print_log(PCI_LOG_INFO, "before: rc mac addr:0x%x PCI_GEN2_CTRL:0x%x", PCI_GEN2_CTRL, val);
    val &= ~PCI_EXP_SPEED_CHANGE_MASK;
    val |= PCI_EXP_SPEED_CHANGE_MASK;
    oal_pci_write_config_dword(pst_rc_dev, *ep_l1ss_pm + PCI_GEN2_CTRL, val);
#endif
}

static void oal_pcie_change_speed_check(oal_pci_dev_stru *pst_pci_dev, oal_pcie_res *pst_pci_res, const void *pcie_ctrl,
                                        uint8_t *speed_change_succ, uint32_t *link_speed, uint32_t *link_sts)
{
    msleep(1);

    oal_pcie_get_link_spend_sts(pst_pci_dev, pst_pci_res, link_speed, link_sts);
    if (*link_speed != g_pcie_speed_change) {
        oam_error_log1(0, OAM_SF_PWR, "speed change fail to %d[0:GEN1 1:GEN2 2:GEN3]", g_pcie_speed_change);
        pci_print_log(PCI_LOG_ERR, "speed change fail to %d[0:GEN1 1:GEN2 2:GEN3]", g_pcie_speed_change);
        declare_dft_trace_key_info("pcie_speed_change_fail", OAL_DFT_TRACE_FAIL);
        chr_exception_report(CHR_PLATFORM_EXCEPTION_EVENTID, CHR_SYSTEM_PLAT, CHR_LAYER_DRV,
                             CHR_PLT_DRV_EVENT_PCIE, CHR_PLAT_DRV_ERROR_PCIE_SPEED_CHANGE_FAIL);
#if defined(CONFIG_HUAWEI_OHOS_DSM) || defined(CONFIG_HUAWEI_DSM)
        hw_mpxx_dsm_client_notify(SYSTEM_TYPE_PLATFORM, DSM_WIFI_PCIE_LINKDOWN,
                                  "%s: pcie speed change fail\n", __FUNCTION__);
#endif
        if (pcs_trace_speedchange_flag() & 0x1) {
            pcie_pcs_tracer_dump(oal_pcie_get_pcs_paddr(pst_pci_dev, pst_pci_res, PCIE_PCS_DUMP_BY_SSI),
                                 0, PCIE_PCS_DUMP_BY_SSI);
        }
    } else {
        *speed_change_succ = 1;
        oam_warning_log1(0, OAM_SF_PWR, "speed change success to %d[0:GEN1 1:GEN2 2:GEN3]", g_pcie_speed_change);
        pci_print_log(PCI_LOG_INFO, "speed change success to %d[0:GEN1 1:GEN2 2:GEN3]", g_pcie_speed_change);
        declare_dft_trace_key_info("pcie_speed_change_succ", OAL_DFT_TRACE_SUCC);
        if (pcs_trace_speedchange_flag() & 0x2) {
            pcie_pcs_tracer_dump(oal_pcie_get_pcs_paddr(pst_pci_dev, pst_pci_res, PCIE_PCS_DUMP_BY_SSI),
                                 0, PCIE_PCS_DUMP_BY_SSI);
        }
    }
}

static void oal_pcie_change_speed_after_pcs(oal_pci_dev_stru *pst_rc_dev, void *pcie_ctrl, int32_t *ep_l1ss_pm,
                                            uint32_t *link_speed, uint32_t *link_sts)
{
    uint32_t val;
    pci_print_log(PCI_LOG_INFO,
                  "after: non_l1_rcvry_cnt:0x%x l1_rcvry_cnt:0x%x stat_monitor1:0x%x stat_monitor2:0x%x",
                  oal_readl(pcie_ctrl + PCIE_CTRL_RB_PCIE_STAT_CNT_RCVRY_OFF),
                  oal_readl(pcie_ctrl + PCIE_CTRL_RB_PCIE_STAT_CNT_L1_RCVRY_OFF),
                  oal_readl(pcie_ctrl + PCIE_CTRL_RB_PCIE_STAT_MONITOR1_OFF),
                  oal_readl(pcie_ctrl + PCIE_CTRL_RB_PCIE_STAT_MONITOR2_OFF));
    /* debug mac reg after speed-change */
    oal_pci_read_config_dword(pst_rc_dev, *ep_l1ss_pm + PCI_EXP_LNKCTL, &val);
    pci_print_log(PCI_LOG_INFO, "after: rc mac addr:0x%x PCI_EXP_LNKCTL:0x%x", PCI_EXP_LNKCTL, val);
    oal_pci_read_config_dword(pst_rc_dev, *ep_l1ss_pm + PCI_EXP_LNKCTL2, &val);
    pci_print_log(PCI_LOG_INFO, "after: rc mac addr:0x%x PCI_EXP_LNKCTL2:0x%x", PCI_EXP_LNKCTL2, val);
    oal_pci_read_config_dword(pst_rc_dev, *ep_l1ss_pm + PCI_GEN2_CTRL, &val);
    pci_print_log(PCI_LOG_INFO, "after: rc mac addr:0x%x PCI_GEN2_CTRL:0x%x", PCI_GEN2_CTRL, val);
    pci_print_log(PCI_LOG_INFO, "speed change result: link_sts = 0x%x, link_speed = 0x%x", *link_sts, *link_speed);
}

#define LINK_SPEED_CHANGE_MAX_ROUND 2
void oal_pcie_link_speed_change(oal_pci_dev_stru *pst_pci_dev, oal_pcie_res *pst_pci_res)
{
    void *pcie_ctrl = NULL;
    oal_pci_dev_stru *pst_rc_dev = NULL;
    int32_t ep_l1ss_pm = 0;
    uint8_t speed_change_round = 1;
    uint8_t speed_change_succ = 0;
    uint32_t link_speed, link_sts;

    pcie_ctrl = oal_pcie_get_ctrl_address(pst_pci_dev, pst_pci_res);
    if (pcie_ctrl == NULL) {
        pci_print_log(PCI_LOG_ERR, "pcie_ctrl virtual address is null");
        return;
    }

    while ((speed_change_succ == 0) && (speed_change_round <= LINK_SPEED_CHANGE_MAX_ROUND)) {
        pci_print_log(PCI_LOG_INFO, "pcie link speed change, round:%d", speed_change_round);
        speed_change_round++;

        /* Step1: 如果当前速率不等于GEN1, 或目标速率等于GEN1, 不切速 */
        if (oal_pcie_check_gen(pst_pci_dev, pst_pci_res) != OAL_SUCC) {
            oam_error_log0(0, OAM_SF_PWR, "pcie link speed change error, current speed is not GEN1");
            declare_dft_trace_key_info("pcie_speed_change_error", OAL_DFT_TRACE_FAIL);
            return;
        }

        if (g_pcie_speed_change == PCIE_GEN1) {
            pci_print_log(PCI_LOG_WARN, "pcie link speed change bypass, target speed is GEN1");
            continue;
        }

        /* Step2: 如果链路状态bit[14:9]非L0, 不切速 */
        if (oal_pcie_check_ltssm(pst_pci_dev, pst_pci_res) != OAL_SUCC) {
            pci_print_log(PCI_LOG_ERR, "pcie link speed change bypass, link stats is not L0");
            continue;
        }

        /* Step3: 链路状态机数采维测 */
        oal_pcie_change_speed_before_pcs(pst_pci_dev, pst_pci_res, pcie_ctrl);

        /* Step4: 配置rc侧mac寄存器 */
        pst_rc_dev = pci_upstream_bridge(pst_pci_dev);
        if (pst_rc_dev != NULL) {
            ep_l1ss_pm = oal_pci_pcie_cap(pst_rc_dev);
            if (!ep_l1ss_pm) {
                pci_print_log(PCI_LOG_ERR, "pcie link speed change fail, cannot get rc addr");
                continue;
            }
            oal_pcie_change_speed_rc_mac_adapter(pst_rc_dev, &ep_l1ss_pm);
        }

        /* Step5: 固定delay 1ms, EDA仿真不超过300us;并确认链路速度,bit[21:20] */
        oal_pcie_change_speed_check(pst_pci_dev, pst_pci_res, pcie_ctrl, &speed_change_succ, &link_speed, &link_sts);

        /* Step6: 链路状态机数采维测，预期non_l1_rcvry_cnt=0x3, l1_rcvry_cnt=0x0 */
        oal_pcie_change_speed_after_pcs(pst_rc_dev, pcie_ctrl, &ep_l1ss_pm, &link_speed, &link_sts);
    }
}

#endif
