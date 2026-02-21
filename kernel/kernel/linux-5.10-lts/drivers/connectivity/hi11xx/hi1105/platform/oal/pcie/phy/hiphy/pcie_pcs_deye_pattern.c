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
    uint32_t pi, uint32_t vref);
uint32_t pcie_pcs_deye_pattern_testv2(pcs_reg_ops *reg_ops, uintptr_t base_addr, uint32_t lane, uint32_t quadrant,
    uint32_t pi, uint32_t vref);
uint32_t pcie_pcs_deye_para_result_printv1(pcs_reg_ops *reg_ops, uintptr_t base_addr, uint8_t *buff, int32_t size);
uint32_t pcie_pcs_deye_para_result_printv2(pcs_reg_ops *reg_ops, uintptr_t base_addr, uint8_t *buff, int32_t size);
/* 获取完成状态 */
uint32_t pcie_pcs_deye_test_status_polling(pcs_reg_ops *reg_ops, uintptr_t addr, uint32_t pos,
    uint32_t bits, uint32_t check_val)
{
    uint32_t polling_cnt = 0;
    uint32_t read_val = pcie_pcs_get_bits32(reg_ops, addr, pos, bits);

    while (read_val != check_val) {
        pcie_pcs_udelay(1);
        read_val = pcie_pcs_get_bits32(reg_ops, addr, pos, bits);
        polling_cnt++;
        /* wait 20us */
        if (polling_cnt > 20) {
            return read_val;
        }
    }

    return read_val;
}

uint32_t pcie_pcs_deye_pattern_test(pcs_reg_ops *reg_ops, uintptr_t base_addr, uint32_t lane, uint32_t quadrant,
    uint32_t pi, uint32_t vref)
{
    oal_pcie_res *pst_pci_res = oal_get_default_pcie_handler();
    if (pst_pci_res == NULL) {
        return OAL_EINVAL;
    }
    if (pst_pci_res->chip_info.phy_version == PCIE_PHY_VERSION2) {
        return pcie_pcs_deye_pattern_testv2(reg_ops, base_addr, lane, quadrant, pi, vref);
    } else {
        return pcie_pcs_deye_pattern_testv1(reg_ops, base_addr, lane, quadrant, pi, vref);
    }
}

int32_t pcie_pcs_deye_para_result_print(pcs_reg_ops *reg_ops, uintptr_t base_addr, uint8_t *buff, int32_t size)
{
    oal_pcie_res *pst_pci_res = oal_get_default_pcie_handler();
    if (pst_pci_res == NULL) {
        return OAL_EINVAL;
    }
    if (pst_pci_res->chip_info.phy_version == PCIE_PHY_VERSION2) {
        return pcie_pcs_deye_para_result_printv2(reg_ops, base_addr, buff, size);
    } else {
        return pcie_pcs_deye_para_result_printv1(reg_ops, base_addr, buff, size);
    }
}

static uint32_t g_deye_pattern_result[EYE_PATTERN_ARRY_SIZE][EYE_PATTERN_ARRY_SIZE];
int32_t pcie_pcs_deye_pattern_draw(pcs_reg_ops *reg_ops, uintptr_t base_addr, uint32_t lane, uint8_t *buff,
    int32_t size)
{
    uint32_t pi, vref;
    int i, j, k;
    int32_t ret;
    int32_t offset = 0;

    (void)memset_s((void*)g_deye_pattern_result, sizeof(g_deye_pattern_result), 0, sizeof(g_deye_pattern_result));

    /*
     * 电子眼图4个象限
     * 每个象限16*16, 整个map是32*32
     * 纵坐标是vref电压, 横坐标是pi时间
     * 每次进入FOM模式, 读取一个值，代表某时间点的电压
     */
    for (vref = 0; vref < EYE_PATTERN_QUADRANT_SIZE; vref++) {
        for (pi = 0; pi < EYE_PATTERN_QUADRANT_SIZE; pi++) {
            /* 从中心向四周扩展 */
            g_deye_pattern_result[EYE_PATTERN_QUADRANT_SIZE -1 - vref][EYE_PATTERN_QUADRANT_SIZE -1 - pi] =
                pcie_pcs_deye_pattern_test(reg_ops, base_addr, lane, QUADRANT_1, pi, vref);
            g_deye_pattern_result[EYE_PATTERN_QUADRANT_SIZE + vref][EYE_PATTERN_QUADRANT_SIZE -1 - pi] =
                pcie_pcs_deye_pattern_test(reg_ops, base_addr, lane, QUADRANT_2, pi, vref);
            g_deye_pattern_result[EYE_PATTERN_QUADRANT_SIZE + vref][EYE_PATTERN_QUADRANT_SIZE + pi] =
                pcie_pcs_deye_pattern_test(reg_ops, base_addr, lane, QUADRANT_3, pi, vref);
            g_deye_pattern_result[EYE_PATTERN_QUADRANT_SIZE -1 - vref][EYE_PATTERN_QUADRANT_SIZE + pi] =
                pcie_pcs_deye_pattern_test(reg_ops, base_addr, lane, QUADRANT_4, pi, vref);
        }
    }

    /* 保证非pcie aspm状态，且dbg_uart打印正常 */
    ret = snprintf_s((char *)(buff + offset), size - offset, size - offset - 1,
        "********  PCIe idx:0x%x lane:%u Eye Pattern  ********" NEWLINE, (uint32_t)base_addr, lane);
    if (ret <= 0) {
        return -OAL_ENOMEM;
    }
    offset += ret;

    /* 打印横坐标 */
    ret = snprintf_s((char *)(buff + offset), size - offset, size - offset - 1,
        "  F E D C B A 9 8 7 6 5 4 3 2 1 0 0 1 2 3 4 5 6 7 8 9 A B C D E F" NEWLINE);
    if (ret <= 0) {
        return -OAL_ENOMEM;
    }
    offset += ret;

    for (i = 0; i < EYE_PATTERN_ARRY_SIZE; i++) {
        /* 打印纵坐标 */
        k = 0xF - i;
        if (k >= 0) {
        } else {
            k = -k - 1;
        }
        ret = snprintf_s((char *)(buff + offset), size - offset, size - offset - 1, "%x ", k);
        if (ret <= 0) {
            return -OAL_ENOMEM;
        }
        offset += ret;

        /* 打印电压值 */
        for (j = 0; j < EYE_PATTERN_ARRY_SIZE; j++) {
            if (g_deye_pattern_result[i][j] > 0 && g_deye_pattern_result[i][j] < EYE_PATTERN_VALUE) {
                ret = snprintf_s((char *)(buff + offset), size - offset, size - offset - 1, "+ "); // 眼皮
            } else if (g_deye_pattern_result[i][j] == 0) {
                ret = snprintf_s((char *)(buff + offset), size - offset, size - offset - 1, "- "); // 眼外
            } else if (g_deye_pattern_result[i][j] == EYE_PATTERN_VALUE) {
                ret = snprintf_s((char *)(buff + offset), size - offset, size - offset - 1, "  "); // 眼内
            } else if (g_deye_pattern_result[i][j] == EYE_PATTERN_INVALID_VALUE) {
                ret = snprintf_s((char *)(buff + offset), size - offset, size - offset - 1, "@"); // 超时标记
            } else {
                ret = snprintf_s((char *)(buff + offset), size - offset, size - offset - 1, "#"); // 非法值
            }
            if (ret <= 0) {
                return -OAL_ENOMEM;
            }
            offset += ret;
        }
        ret = snprintf_s((char *)(buff + offset), size - offset, size - offset - 1, NEWLINE);
        if (ret <= 0) {
            return -OAL_ENOMEM;
        }
        offset += ret;
    }
    ret = snprintf_s((char *)(buff + offset), size - offset, size - offset - 1, NEWLINE);
    if (ret <= 0) {
        return -OAL_ENOMEM;
    }
    offset += ret;

    /* 打印电子眼图 rx校准结果 */
    ret = pcie_pcs_deye_para_result_print(reg_ops, base_addr, buff + offset, size - offset);
    if (ret <= 0) {
        return -OAL_ENOMEM;
    }
    offset += ret;

    return OAL_SUCC;
}

#endif
