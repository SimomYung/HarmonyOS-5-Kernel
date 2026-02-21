/**
 * Copyright (c) @CompanyNameMagicTag 2025-2025. All rights reserved.
 *
 * Description: oal_hcc_test_enhance.c header file
 * Author: @CompanyNameTag
 */

#ifndef OAL_HCC_TEST_ENHANCE_H
#define OAL_HCC_TEST_ENHANCE_H

/* 其他头文件包含 */
#include "oal_types.h"

/**
 * @brief 增强测试结果结构体
 * @details 该结构体用于存储增强测试的结果，包括实际运行时间、发送误码率和接收误码率。
 * @struct hcc_test_enhance_result
 */
typedef struct {
    /**
     * @brief 用例实际运行时间（单位：微妙）
     */
    uint64_t    real_runtime_us;
    
    /**
     * @brief 发送误码率计数
     */
    uint32_t    tx_ber;
    
    /**
     * @brief 接收误码率计数
     */
    uint32_t    rx_ber;

    /**
     * @brief 吞吐率（单位：Mbps）
     */
    uint32_t    throuput;

    /**
     * @brief 用例执行返回值（单位：0表示成功，非0表示失败）
     */
    int32_t errno_run;

    /**
     * @brief 获取链路状态返回值（单位：0表示成功，非0表示失败）
     */
    int32_t errno_check;
} hcc_test_enhance_result;

#if (defined(_PRE_PLAT_FEATURE_HI110X_PCIE) || defined(CONFIG_MMC)) && (_PRE_PRODUCT_ID != _PRE_PRODUCT_ID_GF61_HOST)
#define CONFIG_MPXX_HCC_TEST_ENHANCE
#else
#undef CONFIG_MPXX_HCC_TEST_ENHANCE
#endif

#ifdef CONFIG_MPXX_HCC_TEST_ENHANCE
/**
 * @brief 启动增强的PCIe循环测试
 * @param timeout_ms 测试用例预期执行时间，超过则停止（单位：毫秒）
 * @param interval_ms 控制流量，100毫秒内空闲的毫秒数, 比如传入90, 则100毫秒内只发送10ms,90ms空闲（单位：毫秒）
 * @return int32_t 返回状态码，-OAL_EIO表示操作失败
 * @note 该函数用于启动一个增强的PCIe循环测试，测试会根据给定的超时时间和间隔时间进行循环操作。
 *       适用于需要对PCIe性能进行测试的场景。函数执行期间，WiFi芯片需要处于正常工作状态。
 *       用例执行时会强制关闭WiFi低功耗，防止PCIE被下电，执行完后不会恢复，重新上下电WiFi子系统可恢复。
 */
int32_t conn_test_pcie_loop_enhance_start(uint32_t timeout_ms, uint32_t interval_ms);

/**
 * @brief 停止增强的PCIe循环测试
 * @param result 测试结果的存储指针
 * @return int32_t 返回状态码，-OAL_EIO表示函数本身的错误码，用例执行情况通过result参数返回
 * @note 该函数用于停止一个增强的PCIe循环测试，并将测试结果存储在传入的`result`指针中。
 *       适用于需要结束PCIe循环测试并获取结果的场景。函数执行期间，WiFi芯片需要处于正常工作状态。
 */
int32_t conn_test_pcie_loop_enhance_stop(hcc_test_enhance_result *result);

/**
 * @brief 初始化增强的测试环境
 * @return void 无返回值
 * @note 该函数用于初始化增强的测试环境，为后续的测试操作做准备。
 *       适用于在进行测试前需要初始化资源的场景。不对外部组件开放
 */
void hcc_test_enhance_init(void);

/**
 * @brief 反初始化增强的测试环境
 * @return void 无返回值
 * @note 该函数用于反初始化增强的测试环境，释放测试过程中使用的资源。
 *       适用于在测试结束后需要清理资源的场景。不对外部组件开放
 */
void hcc_test_enhance_deinit(void);
#else
static inline int32_t conn_test_pcie_loop_enhance_start(uint32_t timeout_ms, uint32_t interval_ms)
{
    return -OAL_EIO;
}
static inline int32_t conn_test_pcie_loop_enhance_stop(hcc_test_enhance_result *result)
{
    return -OAL_EIO;
}
static inline int32_t hcc_test_enhance_init(void)
{
    return OAL_SUCC;
}
static inline void hcc_test_enhance_deinit(void)
{
    return;
}
#endif
#endif
