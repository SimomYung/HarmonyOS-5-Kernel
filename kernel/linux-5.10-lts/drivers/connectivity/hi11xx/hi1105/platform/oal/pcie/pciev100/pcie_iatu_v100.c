/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
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
#include "pcie_reg_v100.h"
#include "securec.h"
#include "chip/comm/pcie_soc.h"

#undef THIS_FILE_ID
#define THIS_FILE_ID OAM_FILE_ID_OAL_PCIE_IATU_V100_C

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
        if (index >= 16) { /* dump size为0x2000，一次偏移0x200，这里16代表dump了所有空间 */
            break;
        }

        pci_print_log(PCI_LOG_INFO, "INBOUND iatu index:%d 's register:\n", index);

        oal_pcie_print_config_reg_bar(pci_res,
                                      hi_pci_iatu_region_ctrl_1_off_inbound_i(hi_pci_iatu_inbound_base_off(index)),
                                      "hi_pci_iatu_region_ctrl_1_off_inbound_i");

        oal_pcie_print_config_reg_bar(pci_res,
                                      hi_pci_iatu_region_ctrl_2_off_inbound_i(hi_pci_iatu_inbound_base_off(index)),
                                      "hi_pci_iatu_region_ctrl_2_off_inbound_i");

        oal_pcie_print_config_reg_bar(pci_res,
                                      hi_pci_iatu_lwr_base_addr_off_inbound_i(hi_pci_iatu_inbound_base_off(index)),
                                      "hi_pci_iatu_lwr_base_addr_off_inbound_i");

        oal_pcie_print_config_reg_bar(pci_res,
                                      hi_pci_iatu_upper_base_addr_off_inbound_i(hi_pci_iatu_inbound_base_off(index)),
                                      "hi_pci_iatu_upper_base_addr_off_inbound_i");

        oal_pcie_print_config_reg_bar(pci_res,
                                      hi_pci_iatu_limit_addr_off_inbound_i(hi_pci_iatu_inbound_base_off(index)),
                                      "hi_pci_iatu_limit_addr_off_inbound_i");

        oal_pcie_print_config_reg_bar(pci_res,
                                      hi_pci_iatu_lwr_target_addr_off_inbound_i(hi_pci_iatu_inbound_base_off(index)),
                                      "hi_pci_iatu_lwr_target_addr_off_inbound_i");

        oal_pcie_print_config_reg_bar(pci_res,
                                      hi_pci_iatu_upper_target_addr_off_inbound_i(hi_pci_iatu_inbound_base_off(index)),
                                      "hi_pci_iatu_upper_target_addr_off_inbound_i");
    }
}

OAL_STATIC int32_t oal_pcie_set_inbound_iatu_region(oal_pcie_res *pci_res)
{
    void *inbound_addr = NULL;
    edma_paddr_t start, target, end;
    iatu_region_ctrl_2_off ctr2;
    int32_t index;
    int32_t region_num;
    oal_pcie_region *region_base;
    region_num = pci_res->regions.region_nums;
    region_base = pci_res->regions.pst_regions;
    inbound_addr = pci_res->st_iatu_bar.st_region.vaddr;

    for (index = 0; index < region_num; index++, region_base++) {
        if (index >= PCIE_INBOUND_REGIONS_MAX) { /* 设置的大小为0x2000，一次偏移0x200，设置了所有空间 */
            pci_print_log(PCI_LOG_ERR, "iatu regions too many, index: %d, the max is %d",
                          index, PCIE_INBOUND_REGIONS_MAX);
            break;
        }

        oal_writel(0x0, inbound_addr + hi_pci_iatu_region_ctrl_1_off_inbound_i(hi_pci_iatu_inbound_base_off(index)));

        ctr2.as_dword = 0;
        ctr2.bits.region_en = 1;
        ctr2.bits.bar_num = region_base->bar_info->bar_idx;
        oal_writel(ctr2.as_dword,
                   inbound_addr + hi_pci_iatu_region_ctrl_2_off_inbound_i(hi_pci_iatu_inbound_base_off(index)));

        /* Host侧64位地址的低32位地址 */
        start.addr = region_base->bus_addr;
        oal_writel(start.bits.low_addr,
                   inbound_addr + hi_pci_iatu_lwr_base_addr_off_inbound_i(hi_pci_iatu_inbound_base_off(index)));
        oal_writel(start.bits.high_addr,
                   inbound_addr + hi_pci_iatu_upper_base_addr_off_inbound_i(hi_pci_iatu_inbound_base_off(index)));

        end.addr = start.addr + region_base->size - 1;
        if (start.bits.high_addr != end.bits.high_addr) {
            /* 如果跨了4G地址应该多配置一个iatu表项，待增加 */
            pci_print_log(PCI_LOG_ERR, "iatu high 32 bits must same!");
            return -OAL_EIO;
        }
        oal_writel(end.bits.low_addr,
                   inbound_addr + hi_pci_iatu_limit_addr_off_inbound_i(hi_pci_iatu_inbound_base_off(index)));

        /* Device侧对应的地址(PCI看到的地址) */
        target.addr = region_base->pci_start;
        oal_writel(target.bits.low_addr,
                   inbound_addr + hi_pci_iatu_lwr_target_addr_off_inbound_i(hi_pci_iatu_inbound_base_off(index)));
        oal_writel(target.bits.high_addr,
                   inbound_addr + hi_pci_iatu_upper_target_addr_off_inbound_i(hi_pci_iatu_inbound_base_off(index)));
    }

    if (index) {
        /* 回读可以保证之前的IATU立刻生效 */
        uint32_t callback_read;
        callback_read = oal_readl(inbound_addr +
                                  hi_pci_iatu_region_ctrl_1_off_inbound_i(hi_pci_iatu_inbound_base_off(0)));
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

OAL_STATIC void oal_pcie_iatu_outbound_dump_by_membar(const void *outbound_addr, uint32_t index)
{
    if (outbound_addr == NULL) {
        pci_print_log(PCI_LOG_ERR, "iatu bar1 vaddr is null");
        return;
    }

    pci_print_log(PCI_LOG_INFO, "OUTBOUND iatu index:%d 's register:\n", index);
    pci_print_log(PCI_LOG_INFO, "%s : 0x%x", "hi_pci_iatu_region_ctrl_1_off_outbound_i",
                  oal_readl(outbound_addr
                  + hi_pci_iatu_region_ctrl_1_off_outbound_i(hi_pci_iatu_outbound_base_off(index))));
    pci_print_log(PCI_LOG_INFO, "%s : 0x%x", "hi_pci_iatu_region_ctrl_2_off_outbound_i",
                  oal_readl(outbound_addr + hi_pci_iatu_region_ctrl_2_off_outbound_i(
                      hi_pci_iatu_outbound_base_off(index))));
    pci_print_log(PCI_LOG_INFO, "%s : 0x%x", "hi_pci_iatu_lwr_base_addr_off_outbound_i",
                  hi_pci_iatu_lwr_base_addr_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));
    pci_print_log(PCI_LOG_INFO, "%s : 0x%x", "hi_pci_iatu_upper_base_addr_off_outbound_i",
                  hi_pci_iatu_upper_base_addr_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));
    pci_print_log(PCI_LOG_INFO, "%s : 0x%x", "hi_pci_iatu_limit_addr_off_outbound_i",
                  hi_pci_iatu_limit_addr_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));
    pci_print_log(PCI_LOG_INFO, "%s : 0x%x", "hi_pci_iatu_lwr_target_addr_off_outbound_i",
                  hi_pci_iatu_lwr_target_addr_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));
    pci_print_log(PCI_LOG_INFO, "%s : 0x%x", "hi_pci_iatu_upper_target_addr_off_outbound_i",
                  hi_pci_iatu_upper_target_addr_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));
}

OAL_STATIC int32_t oal_pcie_set_outbound_iatu_by_membar(void* pst_bar1_vaddr, uint32_t index, uint64_t src_addr,
                                                        uint64_t dst_addr, uint64_t limit_size)
{
    /* IATU 对齐要求,开始结束地址按照4K对齐 */
    iatu_region_ctrl_1_off ctr1;
    iatu_region_ctrl_2_off ctr2;
    void *bound_addr = pst_bar1_vaddr;

    uint32_t src_addrl, src_addrh, value;
    uint32_t dst_addrl, dst_addrh;
    uint64_t limit_addr = src_addr + limit_size - 1;

    if (oal_warn_on(pst_bar1_vaddr == NULL)) {
        pci_print_log(PCI_LOG_ERR, "pst_bar1_vaddr is null");
        return -OAL_ENODEV;
    }

    src_addrl = (uint32_t)src_addr;
    src_addrh = (uint32_t)(src_addr >> 32); /* 32 high bits */
    dst_addrl = (uint32_t)dst_addr;
    dst_addrh = (uint32_t)(dst_addr >> 32); /* 32 high bits */

    ctr1.as_dword = 0;

    if (limit_addr >> 32) { // 32 hight bits
        ctr1.bits.inc_region_size = 1; /* more than 4G */
    }

    ctr2.as_dword = 0;
    ctr2.bits.region_en = 1;
    ctr2.bits.bar_num = 0x0;

    oal_writel(ctr1.as_dword, bound_addr +
               hi_pci_iatu_region_ctrl_1_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));
    oal_writel(ctr2.as_dword, bound_addr +
               hi_pci_iatu_region_ctrl_2_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));

    oal_writel(src_addrl, bound_addr +
               hi_pci_iatu_lwr_base_addr_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));
    oal_writel(src_addrh, bound_addr +
               hi_pci_iatu_upper_base_addr_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));

    oal_writel((uint32_t)limit_addr, bound_addr +
               hi_pci_iatu_limit_addr_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));
    // 32 hight bits
    oal_writel((uint32_t)(limit_addr >> 32), bound_addr +
               hi_pci_iatu_uppr_limit_addr_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));

    oal_writel(dst_addrl, bound_addr +
               hi_pci_iatu_lwr_target_addr_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));
    oal_writel(dst_addrh,
               bound_addr + hi_pci_iatu_upper_target_addr_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));

    value = oal_readl(bound_addr + hi_pci_iatu_lwr_base_addr_off_outbound_i(hi_pci_iatu_outbound_base_off(index)));
    if (value != src_addrl) {
        pci_print_log(PCI_LOG_ERR, "callback read 0x%x failed src_addr=0x%llx, dst_addr=0x%llx limit=0x%llx, index=%u",
                      value, src_addr, dst_addr, limit_addr, index);
        oal_pcie_iatu_outbound_dump_by_membar(pst_bar1_vaddr, index);
        return -OAL_EFAIL;
    } else {
        pci_print_log(PCI_LOG_INFO, "outbound src_addr=0x%llx, dst_addr=0x%llx limit=0x%llx, index = %u",
                      src_addr, dst_addr, limit_addr, index);
        return OAL_SUCC;
    }
}

void oal_pcie_set_outbound_iatu_by_membar_v100(oal_pcie_iatu_bar* pst_iatu_bar)
{
    pst_iatu_bar->iatu_cb.set_outbound_iatu = oal_pcie_set_outbound_iatu_by_membar;
    pst_iatu_bar->iatu_cb.set_inbound_iatu = oal_pcie_set_inbound_iatu_by_membar;
}

#endif
