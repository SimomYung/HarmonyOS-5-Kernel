/**
 * @file allocator.h
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * @brief 内存适配器 allocator 对外头文件。
 * @details \n
 * 本头文件是在各种内存分配算法基础上，借鉴musl库对外提供的内存接口，加上DFX调试功能（DEBUG版本），\n
 * 从方便用户使用的角度出发，抽象提炼的内存适配器接口。
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

/**
 * @defgroup allocator 内存适配器
 * @ingroup allocator
 */

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @ingroup allocator
 * @brief 申请一块内存
 * @par 描述：分配所需的内存空间，并返回一个指向它的指针。
 * @attention \n
 * 1.该接口是release版本接口。
 * @param size      [IN] 申请内存空间的大小
 * @retval 指向用户内存起始地址的指针 申请成功
 * @retval NULL 申请失败
 * @par 依赖：无。
 * @li allocator.h：该接口声明所在的文件。
 * @see free
 */
void *malloc(size_t size);

/**
 * @ingroup allocator
 * @brief 申请n个长度为size的连续内存
 * @par 描述：在内存的动态存储区中分配n个长度为size的连续空间，并将内存初始化为0。
 * @attention \n
 * 1.该接口是release版本接口。
 * @param n         [IN] 内存空间的个数
 * @param size      [IN] 单个内存空间的大小
 * @retval 指向用户内存起始地址的指针 申请成功
 * @retval NULL 申请失败
 * @par 依赖：无。
 * @li allocator.h：该接口声明所在的文件。
 * @see free
 */
void *calloc(size_t n, size_t size);

/**
 * @ingroup allocator
 * @brief 动态内存调整函数
 * @par 描述：对用户指定的内存空间重新调整大小，返回一块拥有原内存内容信息的新内存地址。
 * @attention \n
 * 1.如果期望调整后的内存大小比原内存可用空间大小要小，将申请一块新的内存，拷贝原内存信息到新的内存，并\n
 * 将原内存地址free，返回新内存地址，可能导致数据丢失。\n
 * 2.如果期望调整后的内存大小比原内存可用空间大小要大，则申请一块新的内存，拷贝原内存信息到新的内存，并\n
 * 将原内存地址free，返回新内存地址。\n
 * 3.realloc生成的内存，当内存不再使用时，应使用free函数将内存块释放。
 * 4.该接口是release版本接口
 * @param oldMem         [IN] 指向需要被调整的内存的指针
 * @param newSize        [IN] 期望调整后新内存的大小
 * @retval 指向用户内存起始地址的指针 调整成功
 * @retval NULL 调整失败
 * @par 依赖：无。
 * @li allocator.h：该接口声明所在的文件。
 * @see malloc
 */
void *realloc(void *oldMem, size_t newSize);

/**
 * @ingroup allocator
 * @brief 按页大小作为对齐长度去申请一块长度为size的内存
 * @par 描述：按页大小作为对齐长度去申请一块长度为size的内存。
 * @attention \n
 * 1.该接口是release版本接口。
 * @param size       [IN] 申请内存空间的大小
 * @retval 指向用户内存起始地址的指针 申请成功
 * @retval NULL 申请失败
 * @par 依赖：无。
 * @li allocator.h：该接口声明所在的文件。
 * @see memalign
 */
void *valloc(size_t size);

/**
 * @ingroup allocator
 * @brief 按align作为对齐长度去申请一块长度为size的内存
 * @par 描述：分配一个由size指定大小，地址是按align的2的幂次对齐的内存。
 * @attention \n
 * 1.该接口是release版本接口。\n
 * 2.该接口的入参align最小值为32，即使输入比32小的对齐数，也会按32对齐。\n
 * 3.由于DFX结构限制，当前该接口align支持的最大大小是2的23次方。
 * @param align      [IN] 地址对齐的大小，必须是2的幂次
 * @param size       [IN] 申请内存空间的大小
 * @retval 指向用户内存起始地址的指针 申请成功
 * @retval NULL 申请失败
 * @par 依赖：无。
 * @li allocator.h：该接口声明所在的文件。
 * @see valloc
 */
void *memalign(size_t align, size_t size);

/**
 * @ingroup allocator
 * @brief 释放内存
 * @par 描述：释放内存。
 * @attention \n
 * 1.该接口是release版本接口。
 * @param mem       [IN] 想要释放的内存指针
 * @retval 无
 * @par 依赖：无。
 * @li allocator.h：该接口声明所在的文件。
 * @see malloc
 */
void free(void *mem);

/**
 * @ingroup allocator
 * @brief 获取当前内存地址的内存可用大小
 * @par 描述：获取当前地址mem指向的内存空间的可用大小。
 * @attention \n
 * 1.该接口是release版本接口。
 * @param mem      [IN] 需要获取可用大小的内存块地址
 * @retval 可用大小
 * @retval 0 获取失败
 * @par 依赖：无。
 * @li allocator.h：该接口声明所在的文件。
 * @see malloc
 */
size_t malloc_usable_size(void *mem);

#ifdef __cplusplus
}
#endif

#endif
