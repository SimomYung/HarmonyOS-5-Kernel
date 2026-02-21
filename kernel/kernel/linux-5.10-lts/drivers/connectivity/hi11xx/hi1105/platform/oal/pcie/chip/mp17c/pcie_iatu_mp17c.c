/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description:pcie driver,synophys edma
 * Author: @CompanyNameTag
 */

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE
#define MPXX_LOG_MODULE_NAME "[PCIE_H]"
#define HISI_LOG_TAG           "[PCIE]"
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/err.h>
#include <linux/workqueue.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include "oam_ext_if.h"
#include "pcie_reg.h"
#include "oal_hcc_host_if.h"
#include "plat_firmware.h"
#include "plat_pm_wlan.h"
#include "board.h"
#include "plat_pm.h"

#include "chip/mp17c/pcie_soc_mp17c.h"

OAL_STATIC oal_pcie_region g_pcie_regions[] = {
    {   .pci_start = 0x00200000,
        .pci_end   = 0x0031FFFF,
        .cpu_start = 0x00200000,
        .cpu_end   = 0x0031FFFF, /* 1152KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "MP17C_ITCM"
    },
    {   .pci_start = 0x00400000,
        .pci_end   = 0x0048FFFF,
        .cpu_start = 0x00400000,
        .cpu_end   = 0x0048FFFF, /* 576KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "MP17C_DTCM_S2"
    },
    {   .pci_start = 0x01200000,
        .pci_end   = 0x0132FFFF,
        .cpu_start = 0x01200000,
        .cpu_end   = 0x0132FFFF, /* 1216KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "MP17C_BITCM"
    },
    {   .pci_start = 0x01400000,
        .pci_end   = 0x01451FFF,
        .cpu_start = 0x01400000,
        .cpu_end   = 0x01451FFF, /* 328KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "MP17C_BDTCM"
    },
    {   .pci_start = 0x02200000,
        .pci_end   = 0x022BFFFF,
        .cpu_start = 0x02200000,
        .cpu_end   = 0x022BFFFF, /* 768KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "MP17C_GFITCM"
    },
    {   .pci_start = 0x02400000,
        .pci_end   = 0x024C7FFF,
        .cpu_start = 0x02400000,
        .cpu_end   = 0x024C7FFF, /* 800KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "MP17C_GFDTCM"
    },
    {   .pci_start = 0x03200000,
        .pci_end   = 0x03217FFF,
        .cpu_start = 0x03200000,
        .cpu_end   = 0x03217FFF, /* 96KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "MP17C_COMRAM"
    },
    {   .pci_start = 0x04000000,
        .pci_end   = 0x041E3FFF,
        .cpu_start = 0x04000000,
        .cpu_end   = 0x041E3FFF, /* 1936KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "MP17C_AON_WAPB"
    },
    {   .pci_start = 0x04900000,
        .pci_end   = 0x04906FFF,
        .cpu_start = 0x04900000,
        .cpu_end   = 0x04906FFF, /* 28KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "MP17C_PCIE_WAPB"
    },
    {   .pci_start = 0x08000000,
        .pci_end   = 0x0808FFFF,
        .cpu_start = 0x08000000,
        .cpu_end   = 0x0808FFFF, /* 576KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "MP17C_SHARE_MEM_S5"
    },
};

OAL_STATIC oal_pcie_region g_hisi_pcie_regions[] = {
    {   .pci_start = 0x00200000,
        .pci_end   = 0x0031FFFF,
        .cpu_start = 0x00200000,
        .cpu_end   = 0x0031FFFF, /* 1152KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "MP17C_ITCM"
    },
    {   .pci_start = 0x00400000,
        .pci_end   = 0x0048FFFF,
        .cpu_start = 0x00400000,
        .cpu_end   = 0x0048FFFF, /* 576KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "MP17C_DTCM_S2"
    },
    {   .pci_start = 0x01200000,
        .pci_end   = 0x0132FFFF,
        .cpu_start = 0x01200000,
        .cpu_end   = 0x0132FFFF, /* 1216KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "MP17C_BITCM"
    },
    {   .pci_start = 0x01400000,
        .pci_end   = 0x01451FFF,
        .cpu_start = 0x01400000,
        .cpu_end   = 0x01451FFF, /* 328KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "MP17C_BDTCM"
    },
    {   .pci_start = 0x02200000,
        .pci_end   = 0x022BFFFF,
        .cpu_start = 0x02200000,
        .cpu_end   = 0x022BFFFF, /* 768KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "MP17C_GFITCM"
    },
    {   .pci_start = 0x02400000,
        .pci_end   = 0x024C7FFF,
        .cpu_start = 0x02400000,
        .cpu_end   = 0x024C7FFF, /* 800KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "MP17C_GFDTCM"
    },
    {   .pci_start = 0x03200000,
        .pci_end   = 0x03217FFF,
        .cpu_start = 0x03200000,
        .cpu_end   = 0x03217FFF, /* 96KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "MP17C_COMRAM"
    },
    {   .pci_start = 0x04000000,
        .pci_end   = 0x041E3FFF,
        .cpu_start = 0x04000000,
        .cpu_end   = 0x041E3FFF, /* 1936KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "MP17C_AON_WAPB"
    },
    {   .pci_start = 0x04900000,
        .pci_end   = 0x04985FFF,
        .cpu_start = 0x04900000,
        .cpu_end   = 0x04985FFF, /* 536KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "MP17C_PCIE_WAPB"
    },
    {   .pci_start = 0x08000000,
        .pci_end   = 0x0808FFFF,
        .cpu_start = 0x08000000,
        .cpu_end   = 0x0808FFFF, /* 576KB */
        .flag      = OAL_IORESOURCE_REG,
        .name      = "MP17C_SHARE_MEM_S5"
    },
    {   .pci_start = 0x0061F000,
        .pci_end   = 0x00621000,
        .cpu_start = 0x0061F000,
        .cpu_end   = 0x00621000,
        .flag      = OAL_IORESOURCE_REG,
        .name      = "MP17C_DTCM_S1"
    },
};

int32_t oal_pcie_get_bar_region_info_mp17c(oal_pcie_res *pst_pci_res,
                                           oal_pcie_region **region_base, uint32_t *region_num)
{
    oal_print_mpxx_log(MPXX_LOG_INFO, "mp17c %s region map", mpxx_is_asic() ? "asic" : "fpga");

    if (pst_pci_res->revision == PCIE_REVISION_A) {
        *region_num = (uint32_t)oal_array_size(g_pcie_regions);
        *region_base = &g_pcie_regions[0];
    } else if (pst_pci_res->revision == PCIE_REVISION_5_01A) {
        *region_num = (uint32_t)oal_array_size(g_hisi_pcie_regions);
        *region_base = &g_hisi_pcie_regions[0];
    } else {
        pci_print_log(PCI_LOG_ERR, "unkown pcie ip revision :0x%x\n", pst_pci_res->revision);
        return -OAL_ENODEV;
    }
    return OAL_SUCC;
}

int32_t oal_pcie_set_outbound_membar_mp17c(oal_pcie_res *pst_pci_res, oal_pcie_iatu_bar* pst_iatu_bar)
{
    int32_t ret;
    ret = pst_pci_res->st_iatu_bar.iatu_cb.set_outbound_iatu(pst_iatu_bar->st_region.vaddr,
                                                             0, HISI_PCIE_SLAVE_START_ADDRESS,
                                                             HISI_PCIE_MASTER_START_ADDRESS,
                                                             HISI_PCIE_IP_REGION_SIZE);
    if (ret) {
        return ret;
    }
    return OAL_SUCC;
}

int32_t oal_pcie_set_inbound_membar_mp17c(oal_pcie_res *pst_pci_res)
{
    int32_t ret;
    ret = pst_pci_res->st_iatu_bar.iatu_cb.set_inbound_iatu(pst_pci_res);
    if (ret) {
        return ret;
    }
    return OAL_SUCC;
}
#endif