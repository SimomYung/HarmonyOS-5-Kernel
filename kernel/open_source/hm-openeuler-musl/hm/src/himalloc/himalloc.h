/**
 * @file himalloc.h
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * @brief himalloc 内存分配算法对外头文件（给鸿蒙或其他CBB使用）。
 * @details \n
 * himalloc分配算法通过管理内存与用户内存分离、元数据加密、内存随机化、释放内存污化等手段，切断攻击者通过劫持分配算法劫持进程的目的，确保安全性的领先。本头文件提供himalloc的对外接口。
 * @author anonymous
 * @date 2021-03-05
 * @version v1.0.0
 * *******************************************************************************************
 * @par 修改日志：
 * <table>
 * <tr><th>Date        <th>Version  <th>Author    <th>Description
 * <tr><td>2021-03-05  <td>1.0.0    <td>anonymous <td>创建初始版本
 * </table>
 *
 * *******************************************************************************************
 */

/**
 * @defgroup himalloc 算法对外头文件
 * @ingroup himalloc
 */

#ifndef HIMALLOC_H
#define HIMALLOC_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef HIDDEN
#ifdef OS_HONGMENG
#define HIDDEN __attribute__((__visibility__("hidden")))
#else
#define HIDDEN
#endif
#endif

/**
 * @ingroup himalloc
 * @brief Allocator适配层调用算法层申请内存的函数宏
 * @see VosHimallocAlloc
 */
#define ALLOCATOR_ALLOC VosHimallocAlloc

/**
 * @ingroup himalloc
 * @brief Allocator适配层调用算法层申请内存地址对齐的内存的函数宏
 * @see VosHimallocAllocAlign
 */
#define ALLOCATOR_ALLOC_ALIGN VosHimallocAllocAlign

/**
 * @ingroup himalloc
 * @brief Allocator适配层调用算法层释放内存的函数宏
 * @see VosHimallocFree
 */
#define ALLOCATOR_FREE VosHimallocFree

/**
 * @ingroup himalloc
 * @brief Allocator适配层调用算法层获取当前内存块长度的函数宏
 * @see VosHimallocAllocUsableSizeGet
 */
#define ALLOCATOR_ALLOC_USABLE_SIZE_GET VosHimallocAllocUsableSizeGet

/**
 * @ingroup himalloc
 * @brief Allocator适配层调用算法层申请内部内存的函数宏
 * @attention 该接口仅用于鸿蒙DFX版本
 * @see VosMetaAlloc
 */
#define ALLOCATOR_ALLOC_INNER VosMetaAlloc

/**
 * @ingroup himalloc
 * @brief Allocator适配层调用算法层释放内部内存的函数宏
 * @attention 该接口仅用于鸿蒙DFX版本
 * @see VosMetaFree
 */
#define ALLOCATOR_FREE_INNER VosMetaFree

/**
 * @ingroup himalloc
 * @brief Allocator适配层调用算法层遍历所有内存块的函数宏
 * @attention 该接口仅用于鸿蒙DFX版本
 * @see VosHimallocAllMemWalk
 */
#define ALLOCATOR_ALL_MEM_TRAVERSE VosHimallocAllMemWalk

/**
 * @ingroup himalloc
 * @brief himalloc算法申请内存。
 * @par 描述：申请一块指定长度的内存，并返回指向内存起始地址的指针。
 * @attention \n
 * 1.该接口主要开放给Allocator适配层使用，申请时直接从0号arena里面分配，DOPRA侧请勿使用。
 * @param  size   [IN]  申请内存空间的大小。
 * @retval 指向内存起始地址的指针，NULL表示申请失败。
 * @par 依赖：无。
 * @li himalloc.h：该接口声明所在的文件。
 * @see VosHimallocFree
 */
HIDDEN void *VosHimallocAlloc(size_t size);

/**
 * @ingroup himalloc
 * @brief himalloc算法申请内存地址对齐的内存。
 * @par 描述：申请一块按照uvAlign对齐的指定长度的内存，并返回指向内存起始地址的指针。
 * @attention \n
 * 1.该接口主要开放给Allocator适配层使用，申请时直接从0号arena里面分配，DOPRA侧请勿使用。
 * @param  uvSize   [IN]  申请内存空间的大小。
 * @param  uvAlign  [IN]  申请时起始地址对齐的长度。
 * @retval 指向内存起始地址的指针，NULL表示申请失败。
 * @par 依赖：无。
 * @li himalloc.h：该接口声明所在的文件。
 * @see VosHimallocFree
 */
HIDDEN void *VosHimallocAllocAlign(size_t uvSize, size_t uvAlign);

/**
 * @ingroup himalloc
 * @brief himalloc算法释放内存。
 * @par 描述：释放himalloc算法申请的内存块，并返回释放结果。
 * @attention \n
 * 1.该接口配合VosHimallocAlloc接口进行使用，同理DOPRA侧请勿使用。
 * @param  mem   [IN]  待释放的内存块起始地址。
 * @par 依赖：无。
 * @li himalloc.h：该接口声明所在的文件。
 * @see VosHimallocAlloc
 */
HIDDEN void VosHimallocFree(void *mem);

/**
 * @ingroup himalloc
 * @brief himalloc算法获取当前内存块长度。
 * @par
 * 描述：通过VosHimallocAlloc申请的内存，实际返回的内存大小会比用户的入参要大（向上对齐长度，减少内存碎片），该接口用于获取当前内存块真实长度。
 * @attention \n
 * 1.该接口配合VosHimallocAlloc接口进行使用，同理DOPRA侧请勿使用。
 * @param  pAddr   [IN]  待获取真实长度的内存块起始地址。
 * @param  puvSize [OUT] 存储真实长度的指针。
 * @retval HIMALLOC_OK 0 获取成功 HIMALLOC_ERR 1 获取失败。
 * @par 依赖：无。
 * @li himalloc.h：该接口声明所在的文件。
 * @see VosHimallocAlloc
 */
HIDDEN uint32_t VosHimallocAllocUsableSizeGet(void *pAddr, size_t *puvSize);

/**
 * @ingroup himalloc
 * @brief himalloc算法申请内部资源池内存。
 * @par 描述：从算法内部资源池申请一块指定长度的内存，并返回指向内存起始地址的指针。
 * @attention \n
 * 1.该接口主要开放给mem_hm CBB使用，用于内存统计功能申请信息存储空间，DOPRA侧请勿使用。\n
 * 2.该接口申请的内存不会被VosHimallocAllMemWalk接口扫描出来。\n
 * 3.算法内部资源池空间有限，请勿大量调用该接口。
 * @param  uvSize   [IN]  申请内存空间的大小。
 * @retval 指向内存起始地址的指针，NULL表示申请失败。
 * @par 依赖：无。
 * @li himalloc.h：该接口声明所在的文件。
 * @see VosMetaFree
 */
HIDDEN void *VosMetaAlloc(size_t uvSize);

/**
 * @ingroup himalloc
 * @brief himalloc算法释放内部资源池内存。
 * @par 描述：释放从算法内部资源池申请的内存。
 * @attention \n
 * 1.该接口主要开放给mem_hm CBB，用于释放内存统计功能申请的信息存储空间，配合VosMetaAlloc接口使用，DOPRA侧请勿使用。
 * @param  pAddr   [IN]  待释放的内存块起始地址。
 * @retval 无
 * @par 依赖：无。
 * @li himalloc.h：该接口声明所在的文件。
 * @see VosMetaAlloc
 */
HIDDEN void VosMetaFree(void *pAddr);

/**
 * @ingroup himalloc
 * @brief 该函数根据输入的pAddr更新统计信息pResult。
 * @param pAddr      [IN]     内存块地址
 * @param pResult    [IN/OUT] 内存统计信息结构体指针
 * @retval HIMALLOC_OK 0 获取成功 HIMALLOC_ERR 1 获取失败。
 */
typedef int32_t (*HimallocMemParseFunc)(void *pAddr, size_t size, void *pResult);

/**
 * @ingroup himalloc
 * @brief himalloc算法内存遍历处理函数。
 * @par 描述：内存遍历处理函数，函数内将遍历每一块内存，对每块内存调用入参pfnParseHook函数钩子，刷新入参pPara数据。
 * @attention \n
 * 1.该接口主要开放给mem_hm CBB，用于遍历每一块通过VosHimallocAlloc接口申请的内存。DOPRA侧请勿使用。\n
 * 2.该接口将对每块内存调用入参pfnParseHook函数钩子，在钩子函数里面将刷新入参pPara数据。
 * @param  pPara          [IN/OUT]  记录内存处理信息的结构体指针。
 * @param  pfnParseHook   [IN]      内存处理函数。
 * @param  bIsSliceBitmap [IN]      选择使用遍历slice bitmap还是tcache。
 * @retval HIMALLOC_OK 0 获取成功 HIMALLOC_ERR 1 获取失败。
 * @par 依赖：无。
 * @li himalloc.h：该接口声明所在的文件。
 * @see HimallocMemParseFunc
 */
HIDDEN int32_t VosHimallocAllMemWalk(void *pPara, HimallocMemParseFunc pfnParseHook, bool bIsSliceBitmap);

#ifdef __cplusplus
}
#endif

#endif
