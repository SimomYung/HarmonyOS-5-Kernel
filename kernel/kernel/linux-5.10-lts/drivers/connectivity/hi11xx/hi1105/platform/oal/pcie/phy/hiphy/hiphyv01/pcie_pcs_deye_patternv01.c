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
#include "pcie_pcs_regs_ver01.h"
#include "pcie_host_mpxx.h"

#define QUADRANT_1 0x21
#define QUADRANT_2 0x02
#define QUADRANT_3 0x14
#define QUADRANT_4 0x38
#define EYE_PATTERN_ARRY_SIZE 32
#define EYE_PATTERN_QUADRANT_SIZE 16
#define EYE_PATTERN_VALUE 255
#define EYE_PATTERN_INVALID_VALUE 0xFFFFFFFFU

uint32_t pcie_pcs_deye_pattern_testv1(pcs_reg_ops *reg_ops, uintptr_t base_addr, uint32_t lane, uint32_t quadrant,
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
    /* bit[6] set 1 enable software controllable function */
    pcie_pcs_set_bit(reg_ops, base_addr + pcie_pclk_lane_offset(lane, 0x1B14), 6);
    /* bit[6] set 1 enable software controllable value configure register */
    pcie_pcs_set_bit(reg_ops, base_addr + pcie_pclk_lane_offset(lane, 0x1B58), 6);
    pcie_pcs_udelay(1);
    /* bit[2] set 1 FOM Enable */
    pcie_pcs_set_bit(reg_ops, base_addr + pcie_pclk_lane_offset(lane, 0x1538), 2);
    /* bit[5] must be set 1 to enable HW updata function  */
    pcie_pcs_set_bit(reg_ops, base_addr + pcie_pclk_lane_offset(lane, 0x152C), 5);
    pcie_pcs_udelay(1);

    pcie_pcs_set_bit(reg_ops, base_addr + pcie_pclk_lane_offset(lane, 0x1538), 0);
    pcie_pcs_udelay(2); // delay 2us

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
    /* bit[6] set 1 enable software controllable function */
    pcie_pcs_set_bit(reg_ops, base_addr + pcie_pclk_lane_offset(lane, 0x1B14), 6);
    /* bit[6] set 0 disable software controllable value configure register */
    pcie_pcs_clr_bit(reg_ops, base_addr + pcie_pclk_lane_offset(lane, 0x1B58), 6);

    /* bit[11] set 1 打开硬件FOM开关 */
    pcie_pcs_set_bit(reg_ops, base_addr + pcie_pclk_lane_offset(lane, 0x152C), 11);

    return counts;
}

#endif
