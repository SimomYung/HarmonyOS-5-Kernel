/**
 * @file allocator_ex_inner.h
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * @brief 内存适配器 allocator 内部头文件。
 * @details
 * @author anonymous
 * @date 2022-10-10
 * @version v1.0.0
 * *******************************************************************************************
 * @par 修改日志：
 * <table>
 * <tr><th>Date        <th>Version  <th>Author    <th>Description
 * <tr><td>2022-10-10  <td>1.0.0    <td>anonymous <td>创建初始版本
 * </table>
 *
 * *******************************************************************************************
 */

#ifndef ALLOCATOR_EX_INNER_H
#define ALLOCATOR_EX_INNER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "allocator_ex.h"
#include "himalloc.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup allocator_ex_inner
 * @brief 最大handle个数，最大可取的handle值为0x8FFE
 */
#define HANDLE_MAX 0x8FFF
#define ALLOCATOR_RELEASE_DFX_HEAD_MAGIC 0xE1B0000C
#define ALLOCATOR_DFX_HEAD_MASK 0xFFF0000F
#define ALLOCATOR_RELEASE_DFX_GET_HANDLE_MASK 0x000FFFF0
extern bool g_himalloc_enable_mcheck;

extern HIDDEN mem_handle_use_info *get_handle_size_ex(void);

extern HIDDEN uint32_t update_handle_size_ex(void);

extern HIDDEN uint32_t max_handle_num_get_ex(void);
#ifdef __cplusplus
}
#endif

#endif
