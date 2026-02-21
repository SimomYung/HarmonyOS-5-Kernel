/**
 * @file mem_hm_inner.h
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * @brief mem_hm_inner 内存健康管理 内部头文件
 * @details
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

#ifndef MEM_HM_INNER_H
#define MEM_HM_INNER_H

#ifdef CONFIG_DEBUG_BUILD

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "mem_hm.h"

/* 由用户在编译mem_hm CBB时显示指定依赖的算法层CBB，当前仅记录himalloc一款算法，待扩展 */
#include "himalloc.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MEM_HM_DEBUG
    #define MEM_HM_STATIC
#else
    #define MEM_HM_STATIC static
#endif

/**
 * @ingroup mem_hm_inner
 * @brief 正确
 */
#define MEM_HM_OK 0

/**
 * @ingroup mem_hm_inner
 * @brief 出错
 */
#define MEM_HM_ERR 1

/**
 * @ingroup mem_hm_inner
 * @brief 适配鸿蒙OS的double free dfx机制，开关
 */
extern bool g_himalloc_enable_mcheck;

/**
 * @ingroup mem_hm_inner
 * @brief 调用栈结束魔术字、偏移信息赋值与获取时，移位操作的长度
 */
#define SHIFT_BIT 4

/**
 * @ingroup mem_hm_inner
 * @brief 调用栈层数
 */
#define STACK_LEVEL 5

/**
 * @ingroup mem_hm_inner
 * @brief Allocator CBB内存适配器调测头头魔术字
 */
#define ALLOCATOR_DFX_HEAD_MAGIC 0xAEEEEEEB

/**
 * @ingroup mem_hm_inner
 * @brief Allocator CBB内存适配器调测头调用栈结束魔术字
 */
#define ALLOCATOR_DFX_STACK_END_MAGIC 0xB000000D

/**
 * @ingroup mem_hm_inner
 * @brief Allocator CBB内存适配器调测尾尾魔术字
 */
#define ALLOCATOR_DFX_TAIL_MAGIC 0xCDCDCDCD

/**
 * @ingroup mem_hm_inner
 * @brief Allocator CBB内存适配器调测头偏移信息魔术字
 */
#define ALLOCATOR_DFX_OFFSET_MAGIC 0xF000000E

/**
 * @ingroup mem_hm_inner
 * @brief Allocator CBB内存适配器调测头头魔术字位置用来存储偏移量时的魔术字标志
 */
#define ALLOCATOR_DFX_HEAD_OFFSET_MAGIC 0xA000000B

/**
 * @ingroup mem_hm_inner
 * @brief Allocator CBB内存适配器用来检验各种魔术字的掩码
 */
#define ALLOCATOR_DFX_CHECK_MASK 0xF000000F

/**
 * @ingroup mem_hm_inner
 * @brief Allocator CBB内存适配器调测头长度（调试头头魔术字（uint32_t） + 调用栈结束魔术字（uint32_t）
 *  + 调用栈层数 * 调用栈（void *） + 用户空间长度（size_t））
 */
#define ALLOCATOR_DFX_HEAD_SIZE (2 * sizeof(uint32_t) + STACK_LEVEL * sizeof(void *) + sizeof(size_t))

/**
 * @ingroup mem_hm_inner
 * @brief 内存破坏检测功能当发生内存调测头的头魔术字被破坏打印前（大概率顺踩），向前偏移的长度
 */
#define ALLOCATOR_DFX_HEAD_DAMAGE_PRE_SIZE 0

/**
 * @ingroup mem_hm_inner
 * @brief 内存破坏检测功能当发生内存调测头的尾魔术字被破坏打印前（飞踩或向前写穿），向前偏移的长度
 */
#define ALLOCATOR_DFX_USER_DAMAGE_PRE_SIZE ALLOCATOR_DFX_HEAD_SIZE

/**
 * @ingroup mem_hm_inner
 * @brief 内存破坏检测功能当发生内存调测尾的尾魔术字被破坏打印前（内存写溢出），向前偏移的长度
 */
#define ALLOCATOR_DFX_TAIL_DAMAGE_PRE_SIZE (ALLOCATOR_DFX_HEAD_SIZE * 2)

/**
 * @ingroup mem_hm_inner
 * @brief 内存破坏检测功能打印内存镜像的4字节长度
 */
#define ALLOCATOR_DFX_DAMAGE_PRINT_SIZE (ALLOCATOR_DFX_HEAD_SIZE * 3)

/**
 * @ingroup mem_hm_inner
 * @brief 调测信息，从调测头到调用栈信息的长度
 */
#define ALLOCATOR_DFX_HEAD2RETADDR_SIZE (sizeof(uint32_t) + sizeof(size_t))

/**
 * @ingroup mem_hm_inner
 * @brief 日志记录时，打印输出的BUFFER长度
 */
#define ECHO_BUFFER_LEN 2048

/**
 * @ingroup mem_hm_inner
 * @brief 检测通过ALLOCATOR_ALLOC申请到的地址内存是否被破坏（内部函数）
 * @par 检测通过ALLOCATOR_ALLOC申请到的地址内存是否被破坏（内部函数）
 * @attention \n
 * 1.若发生内存破坏，将调用日志写接口进行被破坏的内存块镜像打印。
 * @param  addr   [IN]      被检测的Dfx内存起始地址。
 * @retval MEM_HM_OK 正常
 * @retval MEM_HM_ERR 被破坏。
 * @par 依赖：无。
 * @li mem_hm_inner.h：该接口声明所在的文件。
 * @see
 */
int32_t MemHMDfxMemDamageCheck(uint32_t *dfxAddr);

#endif

#ifdef __cplusplus
}
#endif

#endif
