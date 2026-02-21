/**
 * @file allocator_inner.h
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * @brief 内存适配器 allocator 内部头文件。
 * @details
 * @author anonymous
 * @date 2021-03-10
 * @version v1.0.0
 * *******************************************************************************************
 * @par 修改日志：
 * <table>
 * <tr><th>Date        <th>Version  <th>Author    <th>Description
 * <tr><td>2021-03-10  <td>1.0.0    <td>anonymous <td>创建初始版本
 * </table>
 *
 * *******************************************************************************************
 */

#ifndef ALLOCATOR_INNER_H
#define ALLOCATOR_INNER_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup allocator_inner
 * @brief 调用栈结束魔术字、偏移信息赋值与获取时，移位操作的长度
 */
#define SHIFT_BIT 4

/**
 * @ingroup allocator_inner
 * @brief 调用栈层数
 */
#define STACK_LEVEL 5

/**
 * @ingroup allocator_inner
 * @brief 正确
 */
#define ALLOCATOR_OK 0

/**
 * @ingroup allocator_inner
 * @brief 错误
 */
#define ALLOCATOR_ERR (-1)

/**
 * @ingroup allocator_inner
 * @brief 内存适配器memalign系列函数，align参数要求最小的值
 */
#ifdef CONFIG_DEBUG_BUILD
#define ALLOCATOR_MIN_ALIGN 64 /* 由于debug版本修改成5层调用栈模式，因此最小的align为64，防止对齐溢出 */
#else
#define ALLOCATOR_MIN_ALIGN 32
#endif

/**
 * @ingroup allocator_inner
 * @brief 用来检验各种魔术字的掩码，release版本通用
 */
#define ALLOCATOR_DFX_CHECK_MASK 0xF000000F

/**
 * @ingroup allocator_inner
 * @brief release dfx标志头
 */
#define ALLOCATOR_RELEASE_DFX_HEAD_FLAG 0xA000000B

#ifdef CONFIG_DEBUG_BUILD
/* HM（内存健康管理）版本内存适配器需要用到的宏定义 */

/**
 * @ingroup allocator_inner
 * @brief HM（内存健康管理）版本下，内存适配器增加的调试头长度（调试头头魔术字（uint32_t）+用户空间长度（size_t）
 * +调用栈层数 * 调用栈（uintptr_t）+调用栈结束魔术字（uint32_t））
 */
#define ALLOCATOR_DFX_HEAD_SIZE (2 * sizeof(uint32_t) + STACK_LEVEL * sizeof(uintptr_t) + sizeof(size_t))

/**
 * @ingroup allocator_inner
 * @brief \n
 * 调试头长度 + 调试尾尾魔术字（uint32_t）长度
 */
#define ALLOCATOR_DFX_SIZE (ALLOCATOR_DFX_HEAD_SIZE + sizeof(uint32_t))

/**
 * @ingroup allocator_inner
 * @brief HM（内存健康管理）版本下，内存适配器需增加的调试头的头魔术字
 */
#define ALLOCATOR_DFX_HEAD_MAGIC 0xAEEEEEEB

/**
 * @ingroup allocator_inner
 * @brief HM（内存健康管理）版本下，内存适配器需增加的调试头的调用栈结束魔术字
 */
#define ALLOCATOR_DFX_STACK_END_MAGIC 0xB000000D

/**
 * @ingroup allocator_inner
 * @brief HM（内存健康管理）版本下，内存适配器需增加的调试尾的尾魔术字
 */
#define ALLOCATOR_DFX_TAIL_MAGIC 0xCDCDCDCD

/**
 * @ingroup allocator_inner
 * @brief HM（内存健康管理）版本下，内存适配器需增加的调试头的偏移信息魔术字
 */
#define ALLOCATOR_DFX_OFFSET_MAGIC 0xF000000E

/**
 * @ingroup allocator_inner
 * @brief HM（内存健康管理）版本下，内存适配器需增加的调试头头魔术字位置用来存储偏移量时的魔术字标志
 */
#define ALLOCATOR_DFX_HEAD_OFFSET_MAGIC 0xA000000B

/**
 * @ingroup allocator_inner
 * @brief HM（内存健康管理）版本下，realloc函数不重新申请内存的最大差异长度
 */
#define ALLOCATOR_DFX_DONT_CARE 16

#endif

#ifdef __cplusplus
}
#endif

#endif
