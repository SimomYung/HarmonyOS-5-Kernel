/**
 * Copyright (c) @CompanyNameMagicTag 2025-2025. All rights reserved.
 *
 * Description: oal_hcc_test.c header file
 * Author: @CompanyNameTag
 */

#ifndef OAL_HCC_TEST_H
#define OAL_HCC_TEST_H

/* 其他头文件包含 */
#include "oal_types.h"

#define TEST_LOOP_CALIC_INTERVAL   100 // 吞吐率计算间隔，建议不低于100ms, jiffies精度只有ms级别
#if defined(_PRE_PLAT_FEATURE_HI110X_PCIE)
/**
 * @brief 测试高吞吐流量PCIE负载，用于产线等场景
 * @param interval_ms 测试间隔时间（单位：毫秒）
 * @param trans_timeout 用例执行时间（单位：毫秒）
 * @param throught 指向存储实际吞吐率的指针（单位：Mbps）
 * @return int32_t 返回状态码，OAL_SUCC表示成功，其他值表示失败
 * @note
 * - 该函数用于测试高吞吐流量的PCIE负载，适用于产线等需要控制执行时间和流量的场景。
 * - 需要外部上电WiFi芯片，确保设备已正确初始化。
 * - throught参数不能为空，否则会导致程序异常。
 * - 函数内部会修改全局变量g_hcc_test_event->interval_ms，测试完成后会恢复原值。
 */
int32_t hcc_test_loop_pcie_chan(uint32_t interval_ms, unsigned long trans_timeout, uint64_t *throught);
/**
 * @brief 终止PCIe环回测试
 * @return void 无返回值
 * @note 该函数用于终止PCIe环回测试（仅作用于runtime模式）
 */
void hcc_test_loop_pcie_chan_abort(void);
#else
static inline int32_t hcc_test_loop_pcie_chan(uint32_t interval_ms, unsigned long trans_timeout, uint64_t *throught)
{
    return -OAL_ENODEV;
}
static inline void hcc_test_loop_pcie_chan_abort(void)
{
    return;
}
#endif
#endif
