/**
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 *
 * Description:pcie_pcs_deye_pattern.c
 * Author: @CompanyNameTag
 */

#ifdef _PRE_PLAT_FEATURE_HI110X_PCIE
#include "pcie_pcs_deye_pattern.h"
#include "pcie_pcs.h"
#include "oal_util.h"
#include "pcie_pcs_regs.h"
#include "pcie_pcs_regs_ver02.h"
#include "pcie_host_mpxx.h"

#define QUADRANT_1 0x21
#define QUADRANT_2 0x02
#define QUADRANT_3 0x14
#define QUADRANT_4 0x38
#define EYE_PATTERN_ARRY_SIZE 32
#define EYE_PATTERN_QUADRANT_SIZE 16
#define EYE_PATTERN_VALUE 255
#define EYE_PATTERN_INVALID_VALUE 0xFFFFFFFFU

typedef struct _pcie_para_type1 {
    char *name;
    uint32_t off;
    uint32_t pos;
    uint32_t bits;
} pcie_para_type1;

typedef struct _pcie_para_type2 {
    char *name;
    uint32_t write_val;
} pcie_para_type2;

pcie_para_type1 g_pcs_phy_para_part1[] =  {
    {"band_gap",                0xAC8,      0,      4   },  // pos0, bits4, 回读BandGap基准修调结果
    {"ad_pll_lock",             0xB70,      0,      1   },  // pos0, bits1，模拟pll lock detect结果
    {"dig_pll_lock",            0xB70,      2,      1   },  // pos2, bits1, 数字pll lock detect结果
    {"pll_kband_r1",            0xB30,      0,      10  },  // pos0, bits10, GEN2 PLL粗调档位
    {"kband_cplt_synzd",        0xB74,      0,      1   },  // pos0, bits1, PLL-kBand校准结束标志位
    {"da_pll_kband_ctrl_sts",   0xF88,      8,      10  },  // pos8, bits8, PLL0-Kband校准值回读
    {"pll_lock_status",         0xA00,      8,      1   },  // pos8, bits1, PLL-模拟Lock-Status回读
    {"tx_termp",                0x154C,     16,     4   },  // pos16, bits4, TXP 阻抗校准回读
    {"tx_termn",                0x154C,     12,     4   },  // pos12, bits4, TXN 阻抗校准回读
    {"rx_term",                 0x154C,     0,      5   },  // pos0, bits5, RX阻抗校准回读
    {"pipe_txdeemph_ctrl_sts",  0x1B84,     7,      18  },  // pos7, bits18, 当前去加重
    {"rate_status",             0x1440,     4,      2   },  // pos4, bits2, 当前速率
    {"power_status",            0x1440,     0,      4   },  // pos0, bits4, 当前功耗模式
    {"rx_ctrl_gsa_vref",        0x1530,     23,     4   }   // pos23, bits4, rxctrl_gsa_vref
};

pcie_para_type2 g_pcs_phy_para_part2[] = {
    {"da_eq_os",        0x1 },
    {"da_sa0_os",       0x2 },
    {"da_sa1_os",       0x3 },
    {"da_sa2_os",       0x4 },
    {"da_sa3_os",       0x5 },
    {"da_gs0_os",       0x6 },
    {"da_gs1_os",       0x7 },
    {"da_rx_att",       0x8 },
    {"da_sel_r",        0x9 },
    {"da_sel_gain",     0xA },
    {"da_sel_c",        0xB },
    {"ctle2_sel_r",     0x1C},
    {"ctle2_sel_c",     0x1D},
    {"da_dfe_eq_vref1", 0xD },
    {"da_dfe_eq_vref0", 0xE },
    {"eq_adapt_fsm",    0x11},
    {"from_mea_fsm",    0x12},
    {"gs_adapt_fsm",    0x13},
    {"adapt_0_fsm",     0x14},
    {"adapt_1_fsm",     0x15},
    {"da_dfe_eq_tap1",  0x18},
    {"da_dfe_eq_tap2",  0x19},
    {"da_dfe_eq_tap3",  0x1A},
};

pcie_para_type1 g_soc_para_mp17c[] = {
    {"PCIE_STAT_MONITOR1",  0x3B8, 0, 32},
    {"PCIE_STAT_MONITOR2",  0x3BC, 0, 32},
    {"PCIE_PHY_STATUS",     0x43c, 0, 32},
    {"PCIE_PHY_RESET_STS1", 0x61C, 0, 32},
    {"PCIE_PHY_RESET_STS2", 0x644, 0, 32},
    {"PCIE_MAC_STATUS",     0x750, 0, 32}
};

OAL_STATIC uint32_t pcie_pcs_deye_cali_debug_read(pcs_reg_ops *reg_ops, uintptr_t base_addr, uint32_t result_sel)
{
    /* 初始化 */
    uint32_t result;

    /* bit[6:2] 5bit reg_rx_dig_info_sel */
    pcie_pcs_set_bits32(reg_ops, base_addr + 0x158C, 2, 5, result_sel);

    /* bit[20:16] 5bit ad_rx_dig_info */
    result = pcie_pcs_get_bits32(reg_ops, base_addr + 0x1594, 16, 5);
    return result;
}

OAL_STATIC int32_t pcie_pcs_deye_cali_phy_para(pcs_reg_ops *reg_ops, uintptr_t base_addr, uint8_t *buff, int32_t size)
{
    uint32_t value;
    int32_t ret = 0;
    int32_t offset = 0;
    int32_t i;

    ret = snprintf_s((char *)(buff + offset), size - offset, size - offset - 1,
        "********  PCIe idx:0x%x ctle eq cali result show  ********" NEWLINE, base_addr);
    offset += ret;
    if (ret <= 0) {
        return offset;
    }

    for (i = 0; i < sizeof(g_pcs_phy_para_part1) / sizeof(g_pcs_phy_para_part1[0]); i++) {
        value = pcie_pcs_get_bits32(reg_ops, base_addr + g_pcs_phy_para_part1[i].off,
                                    g_pcs_phy_para_part1[i].pos, g_pcs_phy_para_part1[i].bits);
        ret = snprintf_s((char *)(buff + offset), size - offset, size - offset - 1, "%s = 0x%x" NEWLINE,
                         g_pcs_phy_para_part1[i].name, value);
        offset += ret;
        if (ret <= 0) {
            return offset;
        }
    }

    /* PVSENSOR回读结果 */
    pcie_pcs_set_bits32(reg_ops, base_addr + 0x9B0, 0, 0, 0x1);
    value = pcie_pcs_get_bits32(reg_ops, base_addr + 0xE00, 0x8, 0x8);
    ret = snprintf_s((char *)(buff + offset), size - offset, size - offset - 1, "pvsensor = 0x%x" NEWLINE, value);
    offset += ret;
    if (ret <= 0) {
        return offset;
    }

    for (i = 0; i < sizeof(g_pcs_phy_para_part2) / sizeof(g_pcs_phy_para_part2[0]); i++) {
        value = pcie_pcs_deye_cali_debug_read(reg_ops, base_addr, g_pcs_phy_para_part2[i].write_val);
        ret = snprintf_s((char *)(buff + offset), size - offset, size - offset - 1, "%s = 0x%x" NEWLINE,
                         g_pcs_phy_para_part2[i].name, value);
        offset += ret;
        if (ret <= 0) {
            return offset;
        }
    }

    return offset;
}

OAL_STATIC int32_t pcie_pcs_deye_cali_soc_reg(pcs_reg_ops *reg_ops, uintptr_t base_addr, uint8_t *buff, int32_t size)
{
    uint32_t value;
    int32_t ret;
    int32_t offset = 0;
    int32_t i;

    for (i = 0; i < sizeof(g_soc_para_mp17c) / sizeof(g_soc_para_mp17c[0]); i++) {
        value = pcie_pcs_get_bits32(reg_ops, base_addr + g_soc_para_mp17c[i].off,
                                    g_soc_para_mp17c[i].pos, g_soc_para_mp17c[i].bits);
        ret = snprintf_s((char *)(buff + offset), size - offset, size - offset - 1, "%s = 0x%x" NEWLINE,
                         g_soc_para_mp17c[i].name, value);
        offset += ret;
        if (ret <= 0) {
            return offset;
        }
    }

    return offset;
}

uint32_t pcie_pcs_deye_para_result_printv2(pcs_reg_ops *reg_ops, uintptr_t base_addr, uint8_t *buff, int32_t size)
{
    int32_t ret;
    int32_t offset = 0;
    oal_pcie_res *pst_pci_res = oal_get_default_pcie_handler();

    ret = pcie_pcs_deye_cali_phy_para(reg_ops, base_addr, buff + offset, size - offset);
    if (ret <= 0) {
        return -OAL_ENOMEM;
    }
    offset += ret;
    ret = pcie_pcs_deye_cali_soc_reg(reg_ops, (uintptr_t)pst_pci_res->pst_pci_ctrl_base, buff + offset, size - offset);
    if (ret <= 0) {
        return -OAL_ENOMEM;
    }
    offset += ret;

    return offset;
}

uint32_t pcie_pcs_deye_pattern_testv2(pcs_reg_ops *reg_ops, uintptr_t base_addr, uint32_t lane, uint32_t quadrant,
    uint32_t pi, uint32_t vref)
{
    /* 初始化 */
    uint32_t counts;

    /* bit[11] set 0，关闭硬件FOM开关 */
    pcie_pcs_clr_bit(reg_ops, base_addr + pcie_pclk_lane_offset(lane, 0x152C), 11);
    /* bit[6] set 0，Record inside eye events */
    pcie_pcs_clr_bit(reg_ops, base_addr + pcie_pclk_lane_offset(lane, 0x153C), 6);

    /* bit[7:0] 8bit 设置象限 */
    pcie_pcs_set_bits32(reg_ops, base_addr + pcie_pclk_lane_offset(lane, 0x153C), 0, 8, quadrant);

    /* bit[15:12] 4bit 设置横纵坐标偏移 */
    pcie_pcs_set_bits32(reg_ops, base_addr + pcie_pclk_lane_offset(lane, 0x153C), 12, 4, vref);
    /* bit[11:8] 4bit FOM PI CKF step */
    pcie_pcs_set_bits32(reg_ops, base_addr + pcie_pclk_lane_offset(lane, 0x153C), 8, 4, pi);

    /* 使能 */
    pcie_pcs_set_bit(reg_ops, base_addr + pcie_pclk_lane_offset(lane, 0x1538), 1);
    pcie_pcs_udelay(1);
    /* bit[2] set 1 FOM Enable */
    pcie_pcs_set_bit(reg_ops, base_addr + pcie_pclk_lane_offset(lane, 0x1538), 2);
    /* bit[5] must be set 1 to enable HW updata function */
    pcie_pcs_set_bit(reg_ops, base_addr + pcie_pclk_lane_offset(lane, 0x152C), 5);
    /* bit[13] must be set 1 to enable pma clk lane */
    pcie_pcs_set_bit(reg_ops, base_addr + pcie_pclk_lane_offset(lane, 0x1300), 13);
    pcie_pcs_udelay(1);

    pcie_pcs_set_bit(reg_ops, base_addr + pcie_pclk_lane_offset(lane, 0x1538), 0);
    pcie_pcs_udelay(100); // delay 100us

    /* bit[18] fom complete interupt,check status */
    if (pcie_pcs_deye_test_status_polling(reg_ops, base_addr + pcie_pclk_lane_offset(lane, 0x1000), 18, 1, 0x1) !=
        0x1) {
        pcie_pcs_print("eye_pattern_test_status_polling fail quadrant[%d], pi[%d], vref[%d]\n", quadrant, pi, vref);
        return EYE_PATTERN_INVALID_VALUE;
    }

    /* bit[15:8] FOM Counter, 返回值 */
    counts = pcie_pcs_get_bits32(reg_ops, base_addr + pcie_pclk_lane_offset(lane, 0x1528), 8, 8);
    /* bit[2:1] set 3 FOM disable */
    pcie_pcs_set_bits32(reg_ops, base_addr + pcie_pclk_lane_offset(lane, 0x1538), 0, 3, 0x0);
    /* bit[5] set 0 used as the state requested to update to CDR_RXG_EN */
    pcie_pcs_clr_bit(reg_ops, base_addr + pcie_pclk_lane_offset(lane, 0x152C), 5);

    return counts;
}

#endif
