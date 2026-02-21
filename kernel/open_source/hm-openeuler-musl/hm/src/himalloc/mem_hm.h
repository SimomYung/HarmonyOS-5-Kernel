/**
 * @file mem_hm.h
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * @brief memory health management。内存健康管理，主要负责内存统计、内存泄漏检查、内存破坏机制。
 * @details \n
 * 内存健康管理是在Allocator CBB的HM版本基础上进行开发设计的，主要负责内存破坏检测、泄露检测和统计等DFX功能。
 * @author anonymous
 * @date 2021-03-11
 * @version v1.0.0
 * *******************************************************************************************
 * @par 修改日志：
 * <table>
 * <tr><th>Date        <th>Version  <th>Author    <th>Description
 * <tr><td>2021-03-11  <td>1.0.0    <td>anonymous <td>创建初始版本
 * </table>
 *
 * *******************************************************************************************
 */

/**
 * @defgroup mem_hm 内存健康管理
 * @ingroup mem_hm
 */

#ifndef MEM_HM_H
#define MEM_HM_H

#ifdef CONFIG_DEBUG_BUILD

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup mem_hm
 * @brief 调试开关选项，开启内存破坏检测功能
 * @see MemHMSwitchSet
 */
#define MEM_HM_OPTION_WITH_DAMAGE_CHECK 0x1

/**
 * @ingroup mem_hm
 * @brief 调测开关选项，开启内存统计功能
 * @see MemHMSwitchSet
 */
#define MEM_HM_OPTION_WITH_STATISTIC 0x2

/**
 * @ingroup mem_hm
 * @brief 调测开关选项，开启内存泄漏检测功能
 * @see MemHMSwitchSet
 */
#define MEM_HM_OPTION_WITH_LEAK_CHECK 0x4

/**
 * @ingroup mem_hm
 * @brief 错误码：内部内存申请时发生内存不足错误
 */
#define MEM_HM_ERRNO_NOMEM (12)

/**
 * @ingroup mem_hm
 * @brief 错误码：内部调用系统函数发生错误
 */
#define MEM_HM_ERRNO_INVAL (22)

/**
 * @ingroup mem_hm
 * @brief 错误码：忙碌，内存DFX功能正在进行中
 */
#define MEM_HM_ERRNO_BUSY (16)

/**
 * @ingroup mem_hm
 * @brief 内存健康管理各个自模块功能开关。
 * @par 描述：通过该接口，为内存健康管理CBB组合内存破坏检查、泄露检测和统计三个子模块的开关状态。
 * @attention \n
 * 1.内存破坏检测功能开关选项宏为 MEM_HM_OPTION_WITH_DAMAGE_CHECK(下面称为宏1)，值为0x1。\n
 * 2.内存统计功能开关选项宏为 MEM_HM_OPTION_WITH_STATISTIC(下面称为宏2)，值为0x2。\n
 * 3.内存泄露检测功能开关选项宏为 MEM_HM_OPTION_WITH_LEAK_CHECK(下面称为宏3)，值为0x4。\n
 * 4.MemHMSwitchSet(0)为关闭所有模块功能。\n
 * 5.MemHMSwitchSet(宏1)为仅开启内存破坏检测功能。\n
 * 6.MemHMSwitchSet(宏2)为仅开启内存统计功能。\n
 * 7.MemHMSwitchSet(宏3)为仅开启内存泄露检测功能。\n
 * 8.MemHMSwitchSet(宏1|宏2)为开启内存破坏检测和内存统计功能。\n
 * 9.MemHMSwitchSet(宏1|宏3)为开启内存破坏检测和内存泄露检测功能。\n
 * 10.MemHMSwitchSet(宏2|宏3)为开启内存统计和内存泄露检测功能。\n
 * 11.MemHMSwitchSet(宏1|宏2|宏3)为开启内存破坏检测、内存统计和内存泄露检测功能。\n
 * 12.内存调测模块启动时，默认支持上述上述三种DFX调测能力。\n
 * @param  option   [IN]      内存健康管理功能开关选项。
 * @retval 无
 * @par 依赖：无。
 * @li mem_hm.h：该接口声明所在的文件。
 * @see MemHMSwitchGet、MEM_HM_OPTION_WITH_DAMAGE_CHECK、MEM_HM_OPTION_WITH_STATISTIC
 */
void MemHMSwitchSet(uint32_t option);

/**
 * @ingroup mem_hm
 * @brief 获取内存健康管理下面各模块的开关状态。
 * @par 描述：获取内存健康管理下面各模块的开关状态。
 * @attention \n
 * 1.内存破坏检测功能开关选项宏为 MEM_HM_OPTION_WITH_DAMAGE_CHECK(下面称为宏1)，值为0x1。\n
 * 2.内存统计功能开关选项宏为 MEM_HM_OPTION_WITH_STATISTIC(下面称为宏2)，值为0x2。\n
 * 3.内存泄露检测功能开关选项宏为 MEM_HM_OPTION_WITH_LEAK_CHECK(下面称为宏3)，值为0x4。\n
 * 4.MemHMSwitchSet(0)为关闭所有模块功能。\n
 * 5.MemHMSwitchSet(宏1)为仅开启内存破坏检测功能。\n
 * 6.MemHMSwitchSet(宏2)为仅开启内存统计功能。\n
 * 7.MemHMSwitchSet(宏3)为仅开启内存泄露检测功能。\n
 * 8.MemHMSwitchSet(宏1|宏2)为开启内存破坏检测和内存统计功能。\n
 * 9.MemHMSwitchSet(宏1|宏3)为开启内存破坏检测和内存泄露检测功能。\n
 * 10.MemHMSwitchSet(宏2|宏3)为开启内存统计和内存泄露检测功能。\n
 * 11.MemHMSwitchSet(宏1|宏2|宏3)为开启内存破坏检测、内存统计和内存泄露检测功能。\n
 * 12.内存调测模块启动时，默认支持上述上述三种DFX调测能力。\n
 * @param  无
 * @retval 全部功能模块开关状态的组合option。
 * @par 依赖：无。
 * @li mem_hm.h：该接口声明所在的文件。
 * @see MemHMSwitchSet
 */
uint32_t MemHMSwitchGet(void);

/**
 * @ingroup mem_hm
 * @brief 内存统计功能的信息展示接口。
 * @par 描述：根据入参选项，展示相关的内存统计信息。
 * @attention \n
 * 1.option为0时，将显示内存信息展示接口的帮助信息。
 * 2.option为1时，将显示所有内存调用点的统计信息。
 * 3.option为2时，将显示所有内存调用点的详细信息，逐个输出。
 * @param  option   [IN]      信息展示功能选项。
 * @retval 无。
 * @par 依赖：无。
 * @li mem_hm.h：该接口声明所在的文件。
 * @see
 */
void MemHMStatisticsShow(uint32_t option);

/**
 * @ingroup mem_hm
 * @brief 检测用户申请到的地址内存是否被破坏
 * @par 描述：检测用户申请到的地址内存是否被破坏。
 * @attention \n
 * 1.若发生内存破坏，将调用日志写接口进行被破坏的内存块镜像打印。
 * @param  userAddr   [IN]      被检测的用户内存起始地址。
 * @retval MEM_HM_OK 正常
 * @retval MEM_HM_ERR 被破坏。
 * @par 依赖：无。
 * @li mem_hm.h：该接口声明所在的文件。
 * @see MemHMBatchMemDamageCheck
 */
int32_t MemHMUserMemDamageCheck(void *userAddr);

/**
 * @ingroup mem_hm
 * @brief 全量扫描所有用户通过Allocator内存适配器申请的内存，检测是否发生内存破坏。
 * @par 描述：全量扫描所有用户通过Allocator内存适配器申请的内存，检测是否发生内存破坏。
 * @attention \n
 * 1.若发生内存破坏，将调用日志写接口进行被破坏的内存块镜像打印。
 * @param  无
 * @retval MEM_HM_OK 正常
 * @retval MEM_HM_ERR 有内存被破坏。
 * @par 依赖：无。
 * @li mem_hm.h：该接口声明所在的文件。
 * @see
 */
int32_t MemHMBatchMemDamageCheck(void);

/**
 * @ingroup mem_hm
 * @brief 对内存泄漏检测功能的资源进行手动清理
 * @par 描述：
 * 对内存泄漏检测功能的资源进行手动清理，清空上一次泄漏检测结果数据，释放资源。
 * @attention\n
 * 调用该接口后，内存泄漏扫描的结果数据将被清空\n
 * 该接口请勿与MemHMLeakScan、MemHMLeakInfoGet接口并发调用，否则会发生异常\n
 * 该接口不支持并发调用
 * @retval 无
 * @par 依赖：无
 * @li mem_hm.h 该接口声明所在的文件
 * @see MemHMLeakScan
 */
void MemHMLeakInfoClean(void);

/**
 * @ingroup mem_hm
 * @brief 进行垃圾回收方式内存泄露检测
 * @par 描述：
 * 进行垃圾回收方式内存泄露检测。检测结束后，可以调用接口MemHmLeakInfoGet 获取和显示检测结果
 * @attention\n
 * 该特性支持HongMeng libc的内存调测版本\n
 * 使用垃圾回收方式执行内存泄露扫描时，禁止对动态库进行卸载，否则会访问已释放的非法地址导致段错误\n
 * 操作系统（rtos、euler）修改后，用户态获取不到esp寄存器信息，不会扫描堆栈内存，会导致误报率提高\n
 * 如果使用动态链接库，需要在链接可执行文件时，加上选项：-rdynamic\n
 * 检测前最好先做一下内存破坏检测，如果遇到被破坏的内存块，会报日志，并跳过该内存块\n
 * 检测过程中如果有频繁的创建和删除任务，可能会发生严重错误\n
 * 由于该内存泄露检测算法的局限性，该接口存在误报、漏报，因此仅作为提高产品定位能力的辅助手段\n
 * 请勿多任务调用该函数\n
 * 检查所需的时间由所检查的内存的大小以及cpu的占用率等决定，建议用户在业务不繁忙的时候调用
 * @retval #MEM_HM_OK           0       该函数操作成功
 * @retval #MEM_HM_ERRNO_INVAL  22      获取系统调用栈和数据段操作失败
 * @retval #MEM_HM_ERRNO_NOMEM  12      内存不足
 * @par 依赖：无
 * @li mem_hm.h 该接口声明所在的文件
 * @see MemHmLeakInfoGet
 */
int32_t MemHMLeakScan(void);

/**
 * @ingroup mem_hm
 * @brief 获取垃圾法内存泄露检测的结果
 * @par 描述：
 * 获取垃圾法内存泄露检测的结果，包括泄露的总大小、泄露的块数以及调用用户注册的打印钩子进行泄露内容打印
 * @attention\n
 * 仅打印最近一次调用MemHMLeakScan成功后的扫描结果
 * @param memNum    [OUT] 该参数表示内存泄露的块数
 * @param memSize   [OUT] 该参数表示内存泄露的大小
 * @retval #MEM_HM_OK           0       该函数操作成功
 * @retval #MEM_HM_ERRNO_INVAL  22      还没有执行过内存泄露检测
 * @retval #MEM_HM_ERRNO_BUSY   16      内存泄露检测正在进行中
 * @par 依赖：无
 * @li mem_hm.h 该接口声明所在的文件
 * @see MemHmLeakScan
 */
int32_t MemHMLeakInfoGet(int32_t *memNum, size_t *memSize);

#endif
#ifdef __cplusplus
}
#endif

#endif
