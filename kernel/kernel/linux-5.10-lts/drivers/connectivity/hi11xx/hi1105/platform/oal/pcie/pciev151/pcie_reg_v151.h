/**
 * Copyright (c) @CompanyNameMagicTag 2024-2024. All rights reserved.
 *
 * Description:pcie_reg.c header file
 * Author: @CompanyNameTag
 */

#ifndef PCIE_REG_V151_H
#define PCIE_REG_V151_H
#include "oal_types.h"

/* PCIe Config Register */
#define iob_rxatu_base_off(i)  (0x800 + ((i) * 0x40)) /* base address 0x04901000 size:0x800 */
#define iob_txatu_base_off(i)  (0x000 + ((i) * 0x40)) /* base address 0x04901800 size:0x800 */

#define iob_txatu_control_0_off(base)         (0x0 + (base))
#define iob_rxatu_control_0_off(base)         (0x0 + (base)) /* same as outbound, use viewport select */
#define iob_rxatu_control_1_off(base)         (0x4 + (base))
#define iob_txatu_control_1_off(base)         (0x4 + (base))
#define iob_rxatu_base_addr_l_off(base)       (0x10 + (base))
#define iob_txatu_base_addr_l_off(base)       (0x10 + (base))
#define iob_txatu_base_addr_h_off(base)       (0x14 + (base))
#define iob_rxatu_base_addr_h_off(base)       (0x14 + (base))
#define iob_txatu_region_size_off(base)       (0xC + (base))
#define iob_txatu_ext_region_size_off(base)   (0x8 + (base))
#define iob_rxatu_region_size_off(base)       (0xC + (base))
#define iob_txatu_target_addr_l_off(base)     (0x18 + (base))
#define iob_rxatu_target_addr_l_off(base)     (0x18 + (base))
#define iob_txatu_target_addr_h_off(base)     (0x1C + (base))
#define iob_rxatu_target_addr_h_off(base)     (0x1C + (base))

typedef union {
    struct {
        uint32_t atu_reg_out_en : 1;                  // 0
        uint32_t Reserved0 : 6;                       // 1:6
        uint32_t atu_reg_out_shift : 1;               // 7
        uint32_t atu_reg_out_func_num : 3;            // 8:10
        uint32_t Reserved1 : 2;                       // 11:12
        uint32_t atu_reg_out_tc : 3;                  // 13:15
        uint32_t atu_reg_out_type : 5;                // 16:20
        uint32_t atu_reg_out_attr : 2;                // 21:22
        uint32_t Reserved2 : 9;                       // 23:31
    } bits;
    uint32_t as_dword;
} usp_txatu_control_0;

typedef union {
    struct {
        uint32_t atu_reg_out_incr_rgn_size_val : 1;         // 0
        uint32_t atu_reg_out_incr_rgn_size_override : 1;    // 1
        uint32_t atu_reg_out_incr_rgn_size_mode : 1;        // 2
        uint32_t Reserved0 : 29;                            // 3:31
    } bits;
    uint32_t as_dword;
} usp_txatu_control_3;

typedef union {
    struct {
        uint32_t atu_reg_in_en : 1;                       // 0
        uint32_t atu_reg_in_bar_match : 1;                // 1
        uint32_t reserved0 : 6;                           // 2:7
        uint32_t atu_reg_in_func_match_en : 1;            // 8
        uint32_t reserved1 : 4;                           // 9:12
        uint32_t atu_reg_in_tc_match_en : 1;              // 13
        uint32_t reserved2 : 7;                           // 14:20
        uint32_t atu_reg_in_attr_match_en : 1;            // 21
        uint32_t reserved3 : 10;                          // 22:31
    } bits;
    uint32_t as_dword;
} iob_rxatu_control_0;

typedef union {
    struct {
        uint32_t atu_reg_in_bar_num : 3;                  // 0:2
        uint32_t reserved0 : 5;                           // 3:7
        uint32_t atu_reg_in_func_num : 3;                 // 8:10
        uint32_t reserved1 : 2;                           // 11:12
        uint32_t atu_reg_in_tc : 3;                       // 13:15
        uint32_t atu_reg_in_type : 5;                     // 16:20
        uint32_t atu_reg_in_attr : 2;                     // 21:22
        uint32_t reserved2 : 9;                           // 23:31
    } bits;
    uint32_t as_dword;
} iob_rxatu_control_1;
#endif

