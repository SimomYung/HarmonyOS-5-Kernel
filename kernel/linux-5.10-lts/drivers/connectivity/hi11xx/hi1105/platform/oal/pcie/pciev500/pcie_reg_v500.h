/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description:pcie_reg.c header file
 * Author: @CompanyNameTag
 */

#ifndef PCIE_REG_V500_H
#define PCIE_REG_V500_H
#include "oal_types.h"

/* PCIe Config Register */
#define hi_pcie_iob_tx_base_off(i)  (0x0000 + ((i)*0x20))
#define hi_pcie_iob_rx_base_off(i)  (0x4020 + ((i)*0x20))

#define hi_pcie_iob_txatu_control_0_off_i(base)               (0x0 + (base))
#define hi_pcie_iob_rxatu_control_0_off_i(base)               (0x0 + (base))
#define hi_pcie_iob_txatu_control_2_off_i(base)               (0x8 + (base))
#define hi_pcie_iob_txatu_region_size_off_i(base)             (0xc + (base))
#define hi_pcie_iob_rxatu_region_size_off_i(base)             (0xc + (base))
#define hi_pcie_iob_txatu_base_addr_l_off_i(base)             (0x10 + (base))
#define hi_pcie_iob_rxatu_base_addr_l_off_i(base)             (0x10 + (base))
#define hi_pcie_iob_txatu_base_addr_h_off_i(base)             (0x14 + (base))
#define hi_pcie_iob_rxatu_base_addr_h_off_i(base)             (0x14 + (base))
#define hi_pcie_iob_txatu_tar_addr_l_off_i(base)              (0x18 + (base))
#define hi_pcie_iob_rxatu_tar_addr_l_off_i(base)              (0x18 + (base))
#define hi_pcie_iob_txatu_tar_addr_h_off_i(base)              (0x1c + (base))
#define hi_pcie_iob_rxatu_tar_addr_h_off_i(base)              (0x1c + (base))

typedef union {
    struct {
        // LSB
        uint32_t en               : 1;    // 0
        uint32_t work_mode        : 1;    // 1
        uint32_t type_trans_mode  : 1;    // 2
        uint32_t ep_remap_en      : 1;    // 3
        uint32_t ep_num           : 2;    // 4:5
        uint32_t reserved6        : 2;    // 7:6
        uint32_t pf_num           : 5;    // 8:12
        uint32_t cxl              : 1;    // 13
        uint32_t sec_en           : 1;    // 14
        uint32_t reserved15       : 17;   // 15:31
        // MSB
    } bits;
    uint32_t as_dword;
} iob_txatu_control_0_off;

typedef union {
    struct {
        // LSB
        uint32_t ext_region_size  : 20;    // 0
        uint32_t reserved15       : 12;   // 20:31
        // MSB
    } bits;
    uint32_t as_dword;
} iob_txatu_control_2_off;


typedef union {
    struct {
        // LSB
        uint32_t en          : 1;    // 0
        uint32_t func_mode   : 1;    // 1
        uint32_t reserved2   : 2;    // 2:3
        uint32_t cache       : 4;    // 4:7
        uint32_t snoop       : 1;    // 8
        uint32_t reserved9   : 23;   // 9:31
        // MSB
    } bits;
    uint32_t as_dword;
} iob_rxatu_control_0_off;

#endif

