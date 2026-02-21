/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description:Test code of the device pcie driver chip
 * Author: @CompanyNameTag
 */

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE
#define MPXX_LOG_MODULE_NAME "[PCIE_H]"
#define HISI_LOG_TAG           "[PCIE]"
#include "oal_hcc_host_if.h"
#include "pcie_linux.h"
#include "mp17c/pcie_ctrl_rb_regs.h"
#include "mp17c/pcie_pcs_rb_regs.h"

int32_t oal_pcie_ip_l1pm_check(oal_pcie_linux_res *pst_pci_lres);
int32_t oal_pcie_unmask_device_link_erros(oal_pcie_res *pst_pci_res);
int32_t oal_pcie_device_mem_scanall(oal_pcie_res *pst_pci_res);
int32_t oal_pcie_check_device_link_errors(oal_pcie_res *pst_pci_res);
int32_t oal_pcie_gen_mode_check_mp16(oal_pcie_linux_res *pst_pci_lres);

// erroror：mp17c 切高频暂未配置
int32_t oal_pcie_device_changeto_high_cpufreq_mp17c(oal_pcie_res *pst_pci_res)
{
    int32_t ret;
    if (mpxx_is_asic()) {
        oal_udelay(30); /* 30us */
    }
    ret = oal_pcie_device_check_alive(pst_pci_res);
    if (ret) {
        oal_print_mpxx_log(MPXX_LOG_ERR, "change 640M failed");
        ssi_dump_device_regs(SSI_MODULE_MASK_ARM_REG);
        return ret;
    }

    oal_print_mpxx_log(MPXX_LOG_INFO, "change 640M done");
    return OAL_SUCC;
}

/* 获取phy基地址 */
static void* oal_pcie_get_ep_pcs_vaddr(oal_pcie_res *pst_pci_res)
{
    int32_t ret;
    pci_addr_map st_map;
    uint32_t phy_paddr = MP17C_PCIE_PHY_BASE_ADDRESS;
    ret = oal_pcie_inbound_ca_to_va(pst_pci_res, phy_paddr, &st_map);
    if (ret != OAL_SUCC) {
        pci_print_log(PCI_LOG_ERR, "get dev address 0x%x failed", phy_paddr);
        return NULL;
    }
    /* memport virtual address */
    return (void*)(uintptr_t)st_map.va;
}

/* 链路质量检测，获取误码率计数, 入参合法性由调用者保证，MP17C实现 */
static int32_t oal_pcie_get_link_quality_info_onchip(oal_pcie_res *pst_pci_res, uint32_t *rx_ber)
{
    int32_t ret;
    uint32_t version, dfx_reg, rx_symbol_err_cnt;

    /* pcie linkup, axi is ready */
    void* pcs = (void*)oal_pcie_get_ep_pcs_vaddr(pst_pci_res);
    if (pcs == NULL) {
        return -OAL_EFAUL;
    }

    /* 检查PCIE链路是否异常，若异常，拦截上报错误，误码率过高也可能直接导致直接断链
     * EP的误码率必须链路正常才读出，SSI接口大多数芯片可靠性不能保证 */
    ret = oal_pcie_check_link_state(pst_pci_res);
    if (ret != OAL_TRUE) {
        pci_print_log(PCI_LOG_ERR, "pcie link is abnormal! ret = %d", ret);
        return -OAL_ENODEV;
    }

    /* check phy chip_version */
    version = oal_readl(pcs + MP17C_PCIE_PHY_CHIP_VERSION_RO_OFF);
    if (version != MP17C_PCIE_PHY_CHIP_VERSION_RO_VAL) {
        pci_print_log(PCI_LOG_ERR, "phy chip_version invalid, chip_version = 0x%x, expect 0x%x",
                      version, MP17C_PCIE_PHY_CHIP_VERSION_RO_VAL);
        return -OAL_ENODEV;
    }
    dfx_reg = oal_readl(pcs + MP17C_PCIE_PHY_DFX_REG_OFF);
    rx_symbol_err_cnt = oal_get_bits(dfx_reg, 16, 16); // bit 16~31, refer to pcs bits field rx_symbol_err_cnt
    pci_print_log(PCI_LOG_INFO, "phy dfx_reg = 0x%x, rx_symbol_err_cnt = 0x%x",
                  dfx_reg, rx_symbol_err_cnt);
    if (rx_symbol_err_cnt > 0) {
        pci_print_log(PCI_LOG_ERR, "phy rx ber error, over limit");
    }
    *rx_ber = rx_symbol_err_cnt;
    return OAL_SUCC;
}

static int32_t oal_pcie_ip_factory_test_init(hcc_bus *pst_bus, oal_pcie_linux_res *pst_pci_lres)
{
    int32_t ret = oal_pcie_ip_init(pst_bus);
    if (ret) {
        return ret;
    }

    oal_msleep(5); /* 5ms wait pcie enter L1.2 */

    ret = oal_pcie_ip_l1pm_check(pst_pci_lres);
    if (ret) {
        return ret;
    }

    ret = oal_pcie_voltage_bias_init(pst_pci_lres->pst_pci_res);
    if (ret) {
        return ret;
    }

    ret = oal_pcie_device_changeto_high_cpufreq_mp17c(pst_pci_lres->pst_pci_res);
    if (ret) {
        return ret;
    }
    return OAL_SUCC;
}

int32_t oal_pcie_ip_factory_test_mp17c(hcc_bus *pst_bus, int32_t test_count)
{
    int32_t i;
    int32_t ret;
    oal_pcie_linux_res *pst_pci_lres;

    oal_print_mpxx_log(MPXX_LOG_INFO, "hello factory!!");

    pst_pci_lres = (oal_pcie_linux_res *)pst_bus->data;
    if (oal_warn_on(pst_pci_lres == NULL)) {
        return -OAL_ENODEV;
    }

    ret = oal_pcie_ip_factory_test_init(pst_bus, pst_pci_lres);
    if (ret != OAL_SUCC) {
        return ret;
    }

    ret = oal_pcie_gen_mode_check_mp16(pst_pci_lres);
    if (ret) {
        return ret;
    }

    ret = oal_pcie_unmask_device_link_erros(pst_pci_lres->pst_pci_res);
    if (ret) {
        oal_print_mpxx_log(MPXX_LOG_ERR, "unmask device link err failed = %d", ret);
        return ret;
    }

    for (i = 0; i < test_count; i++) {
        /* memcheck */
        ret = oal_pcie_device_mem_scanall(pst_pci_lres->pst_pci_res);
        if (ret) {
            return ret;
        }

        oal_print_mpxx_log(MPXX_LOG_INFO, "scan all mem done , test %d times", i + 1);

        ret = oal_pcie_check_device_link_errors(pst_pci_lres->pst_pci_res);
        if (ret) {
            return ret;
        }

        ret = oal_pcie_gen_mode_check_mp16(pst_pci_lres);
        if (ret) {
            return ret;
        }
    }

    oal_msleep(5); /* wait 5ms to enter pcie L1.2 */

    ret = oal_pcie_ip_l1pm_check(pst_pci_lres);
    if (ret) {
        return ret;
    }

    return OAL_SUCC;
}

int32_t oal_pcie_chiptest_init_mp17c(oal_pcie_res *pst_pci_res, int32_t device_id)
{
    pst_pci_res->chip_info.cb.pcie_ip_factory_test = oal_pcie_ip_factory_test_mp17c;
    pst_pci_res->chip_info.cb.pcie_get_link_quality_info = oal_pcie_get_link_quality_info_onchip;
    return OAL_SUCC;
}
#endif
