/**
 * Copyright (c) @CompanyNameMagicTag 2024-2024. All rights reserved.
 *
 * Description:pcie driver
 * Author: @CompanyNameTag
 */

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE
#define MPXX_LOG_MODULE_NAME "[PCIE_H]"
#define HISI_LOG_TAG           "[PCIE]"
#include "pcie_host.h"
#include "pcie_chip.h"
#include "oam_ext_if.h"
#include "pcie_linux.h"
#include "pcie_reg_v151.h"
#include "securec.h"
#include "chip/comm/pcie_soc.h"
#include "plat_debug.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_OAL_PCIE_IATU_RECONFIG_C

#define PCIE_ATU_NUM             16
#define PCIE_ATU_BIT_SHIFT_32    32
#define PCIE_ATU_BIT_SHIFT_12    12

OAL_STATIC void oal_pcie_iatu_reg_dump_by_membar(oal_pcie_res *pci_res)
{
    void *inbound_addr = NULL;

    int32_t index;
    int32_t region_num;
    oal_pcie_region *region_base = NULL;

    if (pci_res->st_iatu_bar.st_region.vaddr == NULL) {
        pci_print_log(PCI_LOG_ERR, "iatu bar1 vaddr is null");
        return;
    }

    inbound_addr = pci_res->st_iatu_bar.st_region.vaddr;
    region_num = pci_res->regions.region_nums;
    region_base = pci_res->regions.pst_regions;
    for (index = 0; index < region_num; index++, region_base++) {
        if (index >= PCIE_ATU_NUM) { /* dump size为0x1000，一次偏移0x40，这里16代表dump了所有空间 */
            break;
        }

        pci_print_log(PCI_LOG_INFO, "INBOUND iatu index:%d 's register:\n", index);
        oal_pcie_print_config_reg_bar(pci_res,
                                      iob_rxatu_control_0_off(iob_rxatu_base_off(index)),
                                      "iob_rxatu_control_0_off");

        oal_pcie_print_config_reg_bar(pci_res,
                                      iob_rxatu_control_1_off(iob_rxatu_base_off(index)),
                                      "iob_rxatu_control_1_off");

        oal_pcie_print_config_reg_bar(pci_res,
                                      iob_rxatu_base_addr_l_off(iob_rxatu_base_off(index)),
                                      "iob_rxatu_base_addr_l_off");

        oal_pcie_print_config_reg_bar(pci_res,
                                      iob_rxatu_base_addr_h_off(iob_rxatu_base_off(index)),
                                      "iob_rxatu_base_addr_h_off");

        oal_pcie_print_config_reg_bar(pci_res,
                                      iob_rxatu_region_size_off(iob_rxatu_base_off(index)),
                                      "iob_rxatu_region_size_off");

        oal_pcie_print_config_reg_bar(pci_res,
                                      iob_rxatu_target_addr_l_off(iob_rxatu_base_off(index)),
                                      "iob_rxatu_target_addr_l_off");

        oal_pcie_print_config_reg_bar(pci_res,
                                      iob_rxatu_target_addr_h_off(iob_rxatu_base_off(index)),
                                      "iob_rxatu_target_addr_h_off");
    }
}

OAL_STATIC int32_t oal_pcie_set_inbound_iatu_region(oal_pcie_res *pci_res)
{
    void *inbound_addr = NULL;
    edma_paddr_t start, target;
    iob_rxatu_control_0 ctr0;
    iob_rxatu_control_1 ctr1;
    int32_t index;
    int32_t region_num;
    int32_t region_size;
    oal_pcie_region *region_base;
    region_num = pci_res->regions.region_nums;
    region_base = pci_res->regions.pst_regions;
    inbound_addr = pci_res->st_iatu_bar.st_region.vaddr;
    for (index = 0; index < region_num; index++, region_base++) {
        if (index >= PCIE_INBOUND_REGIONS_MAX) { /* 设置的大小为0x1000，一次偏移0x40，设置了所有空间 */
            pci_print_log(PCI_LOG_ERR, "iatu regions too many, index: %d, the max is %d",
                          index, PCIE_INBOUND_REGIONS_MAX);
            break;
        }

        /* Host侧64位地址的低32位地址 */
        start.addr = region_base->bus_addr;
        oal_writel(start.bits.low_addr,
                   inbound_addr + iob_rxatu_base_addr_l_off(iob_rxatu_base_off(index)));
        oal_writel(start.bits.high_addr,
                   inbound_addr + iob_rxatu_base_addr_h_off(iob_rxatu_base_off(index)));

        region_size = ((region_base->size >> PCIE_ATU_BIT_SHIFT_12) - 1) << PCIE_ATU_BIT_SHIFT_12;
        oal_writel(region_size,
                   inbound_addr + iob_rxatu_region_size_off(iob_rxatu_base_off(index)));

        /* Device侧对应的地址(PCI看到的地址) */
        target.addr = region_base->pci_start;
        oal_writel(target.bits.low_addr,
                   inbound_addr + iob_rxatu_target_addr_l_off(iob_rxatu_base_off(index)));
        oal_writel(target.bits.high_addr,
                   inbound_addr + iob_rxatu_target_addr_h_off(iob_rxatu_base_off(index)));

        ctr1.as_dword = 0;
        ctr1.bits.atu_reg_in_type = 0;
        ctr0.as_dword = 0;
        ctr0.bits.atu_reg_in_en = 1;

        oal_writel(ctr1.as_dword,
                   inbound_addr + iob_rxatu_control_1_off(iob_rxatu_base_off(index)));
        oal_writel(ctr0.as_dword,
                   inbound_addr + iob_rxatu_control_0_off(iob_rxatu_base_off(index)));
    }

    if (index) {
        /* 回读可以保证之前的IATU立刻生效 */
        uint32_t callback_read;
        callback_read = oal_readl(inbound_addr + iob_rxatu_control_0_off(iob_rxatu_base_off(0)));
        oal_reference(callback_read);
    }

    return OAL_SUCC;
}

OAL_STATIC int32_t oal_pcie_set_inbound_iatu_by_membar(oal_pcie_res *pci_res)
{
    int32_t ret;

    oal_pci_dev_stru *pci_dev = pcie_res_to_dev(pci_res);

    if (pci_res->st_iatu_bar.st_region.vaddr == NULL) {
        pci_print_log(PCI_LOG_ERR, "iatu bar1 vaddr is null");
        return -OAL_ENOMEM;
    }

    if (oal_pcie_set_inbound_iatu_region(pci_res) != OAL_SUCC) {
        return -OAL_EIO;
    }

    /* 配置命令寄存器                                                                         */
    /* BIT0 = 1(I/O Space Enable), BIT1 = 1(Memory Space Enable), BIT2 = 1(Bus Master Enable) */
    ret = oal_pci_write_config_word(pci_dev, 0x04, 0x7);
    if (ret) {
        pci_print_log(PCI_LOG_ERR, "pci write iatu config failed ret=%d\n", ret);
        return -OAL_EIO;
    }
    if (pci_dbg_condtion()) {
        oal_pcie_iatu_reg_dump_by_membar(pci_res);
    }

    return OAL_SUCC;
}

OAL_STATIC void oal_pcie_txatu_dump_by_membar(const void *outbound_addr, uint32_t index)
{
    if (outbound_addr == NULL) {
        pci_print_log(PCI_LOG_ERR, "atu bar1 vaddr is null");
        return;
    }

    pci_print_log(PCI_LOG_INFO, "txatu index:%d 's register:\n", index);
    pci_print_log(PCI_LOG_INFO, "%s : 0x%x", "iob_txatu_control_0_off",
                  oal_readl(outbound_addr + iob_txatu_control_0_off(iob_txatu_base_off(index))));
    pci_print_log(PCI_LOG_INFO, "%s : 0x%x", "iob_txatu_control_1_off",
                  oal_readl(outbound_addr + iob_txatu_control_1_off(iob_txatu_base_off(index))));
    pci_print_log(PCI_LOG_INFO, "%s : 0x%x", "iob_txatu_base_addr_l_off",
                  oal_readl(outbound_addr + iob_txatu_base_addr_l_off(iob_txatu_base_off(index))));
    pci_print_log(PCI_LOG_INFO, "%s : 0x%x", "iob_txatu_base_addr_h_off",
                  oal_readl(outbound_addr + iob_txatu_base_addr_h_off(iob_txatu_base_off(index))));
    pci_print_log(PCI_LOG_INFO, "%s : 0x%x", "iob_txatu_region_size_off",
                  oal_readl(outbound_addr + iob_txatu_region_size_off(iob_txatu_base_off(index))));
    pci_print_log(PCI_LOG_INFO, "%s : 0x%x", "iob_txatu_target_addr_l_off",
                  oal_readl(outbound_addr + iob_txatu_target_addr_l_off(iob_txatu_base_off(index))));
    pci_print_log(PCI_LOG_INFO, "%s : 0x%x", "iob_txatu_target_addr_h_off",
                  oal_readl(outbound_addr + iob_txatu_target_addr_h_off(iob_txatu_base_off(index))));
}

OAL_STATIC int32_t oal_pcie_set_outbound_iatu_by_membar(void* pst_bar1_vaddr, uint32_t index, uint64_t src_addr,
                                                        uint64_t dst_addr, uint64_t limit_size)
{
    /* IATU 对齐要求,开始结束地址按照4K对齐 */
    usp_txatu_control_0 ctr0;
    void *bound_addr = pst_bar1_vaddr;

    uint32_t src_addrl, src_addrh;
    uint32_t dst_addrl, dst_addrh;

    uint32_t region_size = ((limit_size >> PCIE_ATU_BIT_SHIFT_12) - 1) << PCIE_ATU_BIT_SHIFT_12;
    uint32_t ext_region_size = limit_size >> PCIE_ATU_BIT_SHIFT_32;
    uint32_t value;

    if (oal_warn_on(pst_bar1_vaddr == NULL)) {
        pci_print_log(PCI_LOG_ERR, "pst_bar1_vaddr is null");
        return -OAL_ENODEV;
    }

    src_addrl = (uint32_t)src_addr;
    src_addrh = (uint32_t)(src_addr >> PCIE_ATU_BIT_SHIFT_32); /* 32 high bits */
    dst_addrl = (uint32_t)dst_addr;
    dst_addrh = (uint32_t)(dst_addr >> PCIE_ATU_BIT_SHIFT_32); /* 32 high bits */

    oal_writel(src_addrl, bound_addr + iob_txatu_base_addr_l_off(iob_txatu_base_off(index)));
    oal_writel(src_addrh, bound_addr + iob_txatu_base_addr_h_off(iob_txatu_base_off(index)));

    oal_writel(region_size, bound_addr + iob_txatu_region_size_off(iob_txatu_base_off(index)));

    oal_writel(ext_region_size, bound_addr + iob_txatu_ext_region_size_off(iob_txatu_base_off(index)));

    oal_writel(dst_addrl, bound_addr + iob_txatu_target_addr_l_off(iob_txatu_base_off(index)));
    oal_writel(dst_addrh, bound_addr + iob_txatu_target_addr_h_off(iob_txatu_base_off(index)));

    ctr0.as_dword = 0;
    ctr0.bits.atu_reg_out_type = 0;
    ctr0.bits.atu_reg_out_en = 1;
    oal_writel(ctr0.as_dword, bound_addr + iob_txatu_control_0_off(iob_txatu_base_off(index)));
    value = oal_readl(bound_addr + iob_txatu_base_addr_l_off(iob_txatu_base_off(index)));
    if (value != src_addrl) {
        pci_print_log(PCI_LOG_ERR, "callback read 0x%x failed, src_addr=0x%llx, dst_addr=0x%llx region_size=0x%x, \
                      ext_region_size=0x%x, index=%u", value, src_addr, dst_addr, region_size, ext_region_size, index);
        oal_pcie_txatu_dump_by_membar(pst_bar1_vaddr, index);
        return -OAL_EFAIL;
    } else {
        pci_print_log(PCI_LOG_INFO, "outbound src_addr=0x%llx, dst_addr=0x%llx region_size=0x%x, ext_region_size=0x%x, \
                      index=%u", src_addr, dst_addr, region_size, ext_region_size, index);
        return OAL_SUCC;
    }
}

void oal_pcie_set_outbound_iatu_by_membar_v151(oal_pcie_iatu_bar* pst_iatu_bar)
{
    pst_iatu_bar->iatu_cb.set_outbound_iatu = oal_pcie_set_outbound_iatu_by_membar;
    pst_iatu_bar->iatu_cb.set_inbound_iatu = oal_pcie_set_inbound_iatu_by_membar;
}
#endif
