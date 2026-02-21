/**
 * @file himalloc_algo.h
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * @brief himalloc 内存分配算法仅对DOPRA分区开放的头文件。
 * @details \n
 * himalloc分配算法通过管理内存与用户内存分离、元数据加密、内存随机化、释放内存污化等手段，切断攻击者通过劫持分配算法劫持进程的目的，确保安全性的领先。本头文件提供himalloc仅对DOPRA分区适配而开放的接口。
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
 * @defgroup himalloc_algo 算法适配DOPRA分区概念的头文件
 * @ingroup himalloc
 */

#ifndef HIMALLOC_ALGO_H
#define HIMALLOC_ALGO_H

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
 * @ingroup himalloc_algo
 * @brief himalloc算法在指定的ArenaId分区下面申请内存。
 * @par 描述：从指定的ArenaId分区下，申请一块指定长度的内存，并返回指向内存起始地址的指针。
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。
 * 2.参数puvSize既是入参也是出参，*puvSize的长度将在算法分配时，按class模块向上对齐，返回新的长度。
 * @param  ucArenaId  [IN]      分区ID。
 * @param  puvSize    [IN/OUT]  存储内存块大小的指针地址。
 * @retval 指向内存起始地址的指针，NULL表示申请失败。
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see VosHimallocFreeImpl
 */
HIDDEN void *VosHimallocAllocImpl(uint8_t ucArenaId, size_t *puvSize);

/**
 * @ingroup himalloc_algo
 * @brief himalloc算法释放在指定的ArenaId分区下面申请到的内存。
 * @par 描述：释放himalloc算法在指定的ArenaId分区下面申请到的内存块，并返回释放结果。
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。
 * @param  ucArenaId  [IN]      分区ID。
 * @param  pAddr      [IN]      待释放的内存块地址
 * @param  puvSize    [OUT]     存储内存块大小的指针地址。
 * @retval HIMALLOC_OK 0 释放成功； HIMALLOC_ERR 1 释放失败。
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see VosHimallocAllocImpl
 */
HIDDEN uint32_t VosHimallocFreeImpl(uint8_t ucArenaId, void *pAddr, size_t *puvSize);

/**
 * @ingroup himalloc_algo
 * @brief himalloc算法获取当前内存块sizeClass类型和长度信息。
 * @par
 * 描述：通过himalloc申请的内存，实际返回的内存大小会比用户的入参要大（根据sizeClass类型向上对齐长度，减少内存碎片），该接口
 * \n 用于获取当前内存块sizeClass类型和真实长度。
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。
 * @param  pAddr   [IN]  待获取信息的内存块起始地址。
 * @param  puiType [OUT] 存储sizeClass类型的指针。
 * @param  puvSize [OUT] 存储真实长度的指针。
 * @retval HIMALLOC_OK 0 获取成功 HIMALLOC_ERR 1 获取失败。
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see VosHimallocAllocImpl
 */
HIDDEN uint32_t VosHimallocAddrSizeGet(void *pAddr, uint32_t *puiType, size_t *puvSize);

/**
 * @ingroup himalloc_algo
 * @enum VosMemAreaRegionTypeE
 * @brief 定义隔离域内存块来源类型
 */
typedef enum TagMemAreaRegionType {
    MEM_AREA_REGION_MALLOC = 0,        /**< malloc申请               */
    MEM_AREA_REGION_MMAP = 1,          /**< mmap申请                 */
    MEM_AREA_REGION_CUSTOM_AUTO = 2,   /**< 自动扩展时从用户钩子中获取 */
    MEM_AREA_REGION_CUSTOM_MANUAL = 3, /**< 用户手动传入              */
} VosMemAreaRegionTypeE;

/**
 * @ingroup himalloc_algo
 * @brief 隔离域扩展时的内存申请钩子
 * @param ucArenaId     [IN]     分区ID
 * @param puvAllocSize  [IN]     扩展内存大小
 * @param penAllocType  [IN/OUT] 来源类型
 * @retval 返回新扩展的内存块起始地址，NULL即为扩展时申请内存块失败。
 */
typedef void *(*VosHimallocAllocFunc)(uint8_t ucArenaId, size_t *puvAllocSize, VosMemAreaRegionTypeE *penAllocType);

/**
 * @ingroup himalloc_algo
 * @brief 隔离域收缩时的内存申请钩子
 * @param ucArenaId    [IN] 分区ID
 * @param pAddr        [IN] 待收缩的隔离域内存块起始地址
 * @param uvSize       [IN] 待收缩的隔离域内存块大小
 * @param enAllocType  [IN] 来源类型
 * @retval 无。
 */
typedef void (*VosHimallocFreeFunc)(uint8_t ucArenaId, void *pAddr, size_t uvSize, VosMemAreaRegionTypeE enAllocType);

/**
 * @ingroup himalloc_algo
 * @brief 隔离域适配DOPRA内存分区的回收钩子。
 * @param ucPtNo         [IN] 分区号
 * @param pPieceAddr     [IN] 分片起始地址
 * @param uvPieceSize    [IN] 分片长度
 * @param enMemAllocType [IN] 内存块的来源类型
 * @retval true 回收成功， false 回收失败。
 */
typedef bool (*VosHimallocRecycledFunc)(uint8_t ucArenaId, void *pPieceAddr, size_t uvPieceSize,
    VosMemAreaRegionTypeE enMemAllocType);

/**
 * @ingroup himalloc_algo
 * @struct VosHimallocGrowFuncsS
 * @brief 隔离域扩展回收钩子
 */
typedef struct TagHimallocGrowFuncs {
    VosHimallocAllocFunc pfnAlloc;
    VosHimallocFreeFunc pfnFree;
    VosHimallocRecycledFunc pfnRecycled;
} VosHimallocGrowFuncsS;


/**
 * @ingroup himalloc_algo
 * @brief himalloc分区销毁。
 * @par 描述：himalloc分区销毁。
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。
 * @param  ucArenaId  [IN]  分区ID。
 * @retval HIMALLOC_OK 0 销毁成功； 其他 销毁失败。
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see VosHimallocArenaInit
 */
HIDDEN uint32_t VosHimallocArenaFinit(uint8_t ucArenaId);

/**
 * @ingroup himalloc_algo
 * @brief himalloc分区扩展。
 * @par 描述：himalloc分区扩展。
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。
 * @param  ucArenaId   [IN]  分区ID。
 * @param  pAddr       [IN]  扩展的内存块起始地址
 * @param  uvSize      [IN]  扩展的内存块长度
 * @param  enAllocType [IN]  扩展的内存块的来源类型
 * @retval HIMALLOC_OK 0 扩展成功； 其他 扩展失败。
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see VosHimallocArenaShrink
 */
HIDDEN uint32_t VosHimallocArenaAppend(uint8_t ucArenaId, void *pAddr, size_t uvSize,
    VosMemAreaRegionTypeE enAllocType);

/**
 * @ingroup himalloc_algo
 * @brief himalloc分区收缩。
 * @par 描述：himalloc分区收缩。
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。
 * @param  ucArenaId   [IN]      分区ID。
 * @param  pAddr       [IN]      收缩的内存块起始地址
 * @param  puvSize     [IN/OUT]  收缩的内存块长度指针
 * @retval HIMALLOC_OK 0 收缩成功； 其他 收缩失败。
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see VosHimallocArenaAppend
 */
HIDDEN uint32_t VosHimallocArenaShrink(uint8_t ucArenaId, void *pAddr, size_t *puvSize);

/**
 * @ingroup himalloc_algo
 * @brief 获取对应的himalloc分区是否已初始化完成。
 * @par 描述：获取对应的himalloc分区是否已初始化完成。
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。
 * @param  ucArenaId   [IN]      分区ID。
 * @retval true 已初始化，false 未初始化。
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see VosHimallocArenaInit
 */
HIDDEN bool VosHimallocIsArenaInstall(uint8_t ucArenaId);

/**
 * @ingroup himalloc_algo
 * @brief 获取radixTree模块是否已初始化完成。
 * @par 描述：获取radixTree模块是否已初始化完成。
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。
 * @param  无
 * @retval true 已初始化，false 未初始化。
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see
 */
HIDDEN bool VosHimallocTlbRtreeIsInstall(void);

/**
 * @ingroup himalloc_algo
 * @brief 获取himalloc分区最大数量。
 * @par 描述：获取himalloc分区最大数量。
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。
 * @param  无
 * @retval 隔离域最大数量。
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see VosHimallocArenaInit
 */
HIDDEN uint8_t VosHimallocArenaMaxNumGet(void);

/**
 * @ingroup himalloc_algo
 * @brief 获取当前此块内存是否已经被申请使用。
 * @par 描述：获取当前此块内存是否已经被申请使用。
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。
 * @param  pAddr          [IN]     内存地址
 * @param  pbIsMemAlloced [IN/OUT] 存储当前内存块的状态的指针
 * @retval HIMALLOC_OK 0 获取成功； 其他 获取失败。
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see
 */
HIDDEN uint32_t VosHimallocMemStateGet(void *pAddr, bool *pbIsMemAlloced);

/**
 * @ingroup himalloc_algo
 * @brief 获取指定隔离域的最大可用内存大小。
 * @par 描述：获取指定隔离域的最大可用内存大小。
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。
 * @param  ucArenaId   [IN]   分区ID
 * @retval 最大可用内存大小
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see
 */
HIDDEN size_t VosHimallocMaxUsableGet(uint8_t ucArenaId);

/**
 * @ingroup himalloc_algo
 * @brief 获取指定扩展块类型的扩展内存空间大小。
 * @par 描述：获取指定扩展块类型的扩展内存空间大小。
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。
 * @param  enMemType   [IN]   扩展块类型
 * @retval 扩展内存空间大小
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see
 */
HIDDEN size_t VosHimallocInitialUsedSizeGet(VosMemAreaRegionTypeE enMemType);

/**
 * @ingroup himalloc_algo
 * @brief 获取himalloc算法的sizeClass类型最大数量。
 * @par 描述：获取himalloc算法的sizeClass类型最大数量。
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。
 * @param  无
 * @retval sizeClass类型最大数量。
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see
 */
HIDDEN uint8_t VosHimallocMaxClassGet(void);

/**
 * @ingroup himalloc_algo
 * @brief 根据sizeClass类型序号，获取对应序号的size大小。
 * @par 描述：根据sizeClass类型序号，获取对应序号的size大小。
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。
 * @param  usIndex  [IN] sizeClass类型数组的序号
 * @retval size大小
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see VosHimallocSize2Index
 */
HIDDEN size_t VosHimallocIndex2Size(uint16_t usIndex);

/**
 * @ingroup himalloc_algo
 * @brief 根据size大小，获取到对应的sizeClass类型序号。
 * @par 描述：根据size大小，获取到对应的sizeClass类型序号。
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。
 * @param  uvSize  [IN] size大小
 * @retval 对应的sizeClass类型数组的序号
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see VosHimallocIndex2Size
 */
HIDDEN uint16_t VosHimallocSize2Index(size_t uvSize);

/**
 * @ingroup himalloc_algo
 * @brief 获取由sizeClass类型序号转化为size大小的数组信息。
 * @par 获取由sizeClass类型序号转化为size大小的数组信息。
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。
 * @param  无
 * @retval 由sizeClass类型序号转化为size大小的数组指针
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see VosHimallocSize2IdxTableGet
 */
HIDDEN const size_t *VosHimallocIdx2SizeTableGet(void);

/**
 * @ingroup himalloc_algo
 * @brief 获取由size大小转化为sizeClass类型序号的数组信息。
 * @par 获取由size大小转化为sizeClass类型序号的数组信息。
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。
 * @param  无
 * @retval 由size大小转化为sizeClass类型序号的数组指针
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see VosHimallocIdx2SizeTableGet
 */
HIDDEN const uint8_t *VosHimallocSize2IdxTableGet(void);

/**
 * @ingroup himalloc_algo
 * @brief 为指定分区ID的隔离域配置最大的slice块size。
 * @par 为指定分区ID的隔离域配置最大的slice块size。
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。
 * @param  ucArenaId       [IN]     分区ID
 * @param  puiMaxSliceSize [IN/OUT] slice块上限大小
 * @retval HIMALLOC_OK 0 配置成功； 其他 配置失败。
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see
 */
HIDDEN uint32_t VosHimallocSliceMaxSizeSet(uint8_t ucArenaId, uint32_t *puiMaxSliceSize);

/**
 * @ingroup himalloc_algo
 * @enum VosHimallocTcacheTypeE
 * @brief 定义隔离域内存块来源类型
 */
typedef enum tagVosHimallocTcacheType {
    VOS_HIMALLOC_TCACHE_CLOSED = 0,                    /* 针对独立发布接口，tlb要求每个线程一个，没有cache缓存 */
    VOS_HIMALLOC_TCACHE_OPEN,                          /* 针对独立发布接口，tlb要求每个线程一个，有cache缓存 */
    VOS_HIMALLOC_ACACHE_CLOSED,                        /* 针对主线代码，tlb要求每个arena一个，没有cache缓存 */
    VOS_HIMALLOC_ACACHE_OPEN,                          /* 针对主线代码，tlb要求每个arena一个，有cache缓存 */
    VOS_HIMALLOC_TCACHE_BUTT
} VosHimallocTcacheTypeE;

/**
 * @ingroup himalloc_algo
 * @brief 为指定分区ID的隔离域配置缓存功能。
 * @par 为指定分区ID的隔离域配置缓存功能。
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。\n
 * 2.该接口默认是关闭的，通过调用并传第二参数值为VOS_HIMALLOC_TCACHE_THREAD可以打开，才支持并发功能。
 * @param  ucArenaId       [IN]   分区ID
 * @param  ucTcacheType    [IN]   缓存功能选项
 * @retval HIMALLOC_OK 0 配置成功； 其他 配置失败。
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see
 */
HIDDEN uint32_t VosHimallocTcacheEnable(uint8_t ucArenaId);

/**
 * @ingroup himalloc_algo
 * @brief 获取tiny slice 的最大内存。
 * @par 获取tiny slice 的最大内存。
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。
 * @param  无
 * @retval tiny slice 的最大内存
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see
 */
HIDDEN uint32_t VosHimallocMaxTinySizeGet(void);

/**
 * @ingroup himalloc_algo
 * @brief 获取tiny slice对齐长度的对数。
 * @par 获取tiny slice对齐长度的对数。
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。
 * @param  无
 * @retval tiny slice对齐长度的对数。
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see
 */
HIDDEN uint32_t VosHimallocTinySearchStepGet(void);

/**
 * @ingroup himalloc_algo
 * @brief 获取small slice对齐长度的对数。
 * @par 获取small slice对齐长度的对数。
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。
 * @param  无
 * @retval small slice对齐长度的对数。
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see
 */
HIDDEN uint32_t VosHimallocLargeSearchStepGet(void);

/**
 * @ingroup himalloc_algo
 * @struct VosHimallocSliceSpecS
 * @brief himalloc的slice内存类别信息
 */
typedef struct TagVosHimallocSliceSpec {
    uint32_t uiMaxSliceClass;  /**< 算法能够支持的最大的Slice内存类型 */
    uint32_t uiSpecSliceClass; /**< 算法当前被设置的最大的Slice内存类型 */
    uint32_t uiMaxSliceSize;   /**< 算法能够支持的最大的Slice内存大小 */
    uint32_t uiSpecSliceSize;  /**< 算法当前被设置的最大的Slice内存大小 */
} VosHimallocSliceSpecS;

/**
 * @ingroup himalloc_algo
 * @brief himalloc获取算法中关于Slice内存的类别信息
 * @par himalloc获取算法中关于Slice内存的类别信息
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。
 * @param  pstSliceSpec   [IN/OUT]  存储slice内存类别信息的结构体指针
 * @retval 无
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see VosHimallocSliceSpecS
 */
HIDDEN void VosHimallocSliceSpecGet(VosHimallocSliceSpecS *pstSliceSpec);

/**
 * @ingroup himalloc_algo
 * @brief 传入分区号已经扩展块的地址以及扩展的大小，将该扩展块从分区管理中删除。
 * @par 传入分区号已经扩展块的地址以及扩展的大小，将该扩展块从分区管理中删除。
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。\n
 * 2.pAddr必须为有效值不能为null。
 * @param  ucArenaId    [IN]     分区ID
 * @param  pAddr        [IN]     扩展块地址
 * @param  uvSize       [IN]     扩展块大小
 * @retval HIMALLOC_OK 0 删除成功； 其他 删除失败。
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see VosHimallocBlockScans
 */
HIDDEN uint32_t VosHimallocBlockShrink(uint8_t ucArenaId, void *pAddr, size_t uvSize);

/* *****************************以下接口为himalloc算法的调测和统计接口***************************** */

/**
 * @ingroup himalloc_algo
 * @brief himalloc调测功能开关函数
 * @par himalloc调测功能开关函数
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。
 * @param  option       [IN]     开关
 * @retval 无
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see
 */
HIDDEN void VosHimallocStatsSwitchSet(bool option);

/**
 * @ingroup himalloc_algo
 * @brief 获取Block可视化信息，申请次数和释放次数。
 * @par 获取Block可视化信息，申请次数和释放次数。
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。
 * @param  ucArenaId       [IN]     分区ID
 * @param  pullAllocTimes  [IN/OUT] 申请次数
 * @param  pullFreeTimes   [IN/OUT] 释放次数
 * @retval HIMALLOC_OK 0 获取成功； 其他 获取失败。
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see VosHimallocSliceVisitInfoGet
 */
HIDDEN uint32_t VosHimallocBlockVisitInfoGet(uint8_t ucArenaId, uint64_t *pullAllocTimes, uint64_t *pullFreeTimes);

/**
 * @ingroup himalloc_algo
 * @brief 获取指定sizeClass类型的Slice可视化信息，申请次数和释放次数。
 * @par 获取指定sizeClass类型的Slice可视化信息，申请次数和释放次数。
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。
 * @param  ucArenaId       [IN]     分区ID
 * @param  ucClass         [IN]     sizeClass类型
 * @param  pullAllocTimes  [IN/OUT] 申请次数
 * @param  pullFreeTimes   [IN/OUT] 释放次数
 * @retval HIMALLOC_OK 0 获取成功； 其他 获取失败。
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see VosHimallocBlockVisitInfoGet
 */
HIDDEN uint32_t VosHimallocSliceVisitInfoGet(uint8_t ucArenaId, uint8_t ucClass, uint64_t *pullAllocTimes,
    uint64_t *pullFreeTimes);

/**
 * @ingroup himalloc_algo
 * @brief 该函数提供从扩展块获取信息的功能。
 * @param ucPt          [IN]        分区号
 * @param pAddr         [IN]        扩展块地址
 * @param uvTotalSize   [IN/OUT]    扩展块总大小
 * @param uvUsedSize    [IN/OUT]    扩展块已使用大小
 * @param bCommitted    [IN]        暂时默认为true表示该扩展块是由mmap申请出来的
 * @param bAlloced      [IN]        表示该扩展块是由malloc申请出来的
 * @param pData         [IN/OUT]    表示要获取的信息
 * @retval HIMALLOC_OK 0 获取成功； 其他 获取失败。
 */
typedef uint32_t (*VosMemPieceCallbackFunc)(uint8_t ucPt, void *pAddr, size_t uvTotalSize, size_t uvUsedSize,
    bool bCommitted, bool bAlloced, void *pData);

/**
 * @ingroup himalloc_algo
 * @brief himalloc适配DOPRA分区，遍历从pStart地址之后的分区的所有扩展块并依据钩子获取相应的信息。
 * @par himalloc适配DOPRA分区，遍历从pStart地址之后的分区的所有扩展块并依据钩子获取相应的信息。
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。
 * @param  ucArenaId    [IN]     分区ID
 * @param  pStart       [IN]     扩展块首地址
 * @param  pfnWalkFunc  [IN]     处理函数钩子
 * @param  pData        [IN/OUT] 所需要的数据
 * @retval HIMALLOC_OK 0 获取成功； 其他 获取失败。
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see VosMemPieceCallbackFunc
 */
HIDDEN uint32_t VosHimallocBlockScans(uint8_t ucArenaId, const void *pStart,
    VosMemPieceCallbackFunc pfnWalkFunc, void *pData);

/**
 * @ingroup himalloc_algo
 * @brief 该函数根据输入的pAddr更新统计信息pResult。
 * @param pAddr      [IN]     内存块地址
 * @param pResult    [IN/OUT] 内存统计信息结构体指针
 * @retval HIMALLOC_OK 0 获取成功 HIMALLOC_ERR 1 获取失败。
 */
typedef int32_t (*HimallocMemParseFunc)(void *pAddr, size_t size, void *pResult);

/**
 * @ingroup himalloc_algo
 * @brief himalloc适配DOPRA分区，遍历pStart所指向的扩展块，根据钩子获取相应信息。
 * @par himalloc适配DOPRA分区，遍历pStart所指向的扩展块，根据钩子获取相应信息。
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。\n
 * 2.该接口不支持并发调用，DOPRA内存分区层自己保证。
 * @param  ucArenaId    [IN]     分区ID
 * @param  pStart       [IN]     扩展块首地址
 * @param  uvSize       [IN]     扩展块长度
 * @param  pfnParseHook [IN]     处理函数钩子
 * @param  pResult      [IN/OUT] 所需要的数据
 * @retval HIMALLOC_OK 0 获取成功； 其他 获取失败。
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see HimallocMemParseFunc
 */
HIDDEN int32_t VosHimallocUsedBlockTraverse(uint8_t ucArenaId, const void *pStart,
    size_t uvSize, const HimallocMemParseFunc pfnParseHook, void *pResult);

/**
 * @ingroup himalloc_algo
 * @struct VosHimallocBlockHeapRtimeStatS
 * @brief himalloc的block大块内存信息
 */
typedef struct TagHimallocBlockHeapRtimeStat {
    size_t uvUsedNum; /**< 大块内存使用的块数  */
} VosHimallocBlockHeapRtimeStatS;

/**
 * @ingroup himalloc_algo
 * @brief 传入分区号以及结构体指针，获取大块内存使用信息。
 * @par 传入分区号以及结构体指针，获取大块内存使用信息。
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。
 * @param  ucArenaId    [IN]     分区ID
 * @param  pstHeapStats [IN/OUT] 信息结构体指针
 * @retval HIMALLOC_OK 0 获取成功； 其他 获取失败。
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see VosHimallocBlockHeapRtimeStatS
 */
HIDDEN uint32_t VosHimallocBlockHeapRtimeStats(uint8_t ucArenaId, VosHimallocBlockHeapRtimeStatS *pstHeapStats);

/**
 * @ingroup himalloc_algo
 * @struct VosHimallocSliceRtimeStatS
 * @brief himalloc的某个slice类型下的内存使用信息
 */
typedef struct TagHimallocSliceRtimeStat {
    size_t uvUsedNum;    /**< 已经被使用的Slice内存数量 */
    size_t uvTotalNum;   /**< 总的Slice内存数量 */
    size_t uvNBinBlocks; /**< 所占用的大内存数量(切割该类小内存的大块内存) */
} VosHimallocSliceRtimeStatS;

/**
 * @ingroup himalloc_algo
 * @brief 传入分区号以及slice内存类型，获取对应slice内存的信息。
 * @par 传入分区号以及slice内存类型，获取对应slice内存的信息。
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。
 * @param  ucArenaId    [IN]     分区ID
 * @param  uiType       [IN]     slice内存类型
 * @param  pstStats     [IN/OUT] 内存信息结构体指针
 * @retval HIMALLOC_OK 0 获取成功； 其他 获取失败。
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see VosHimallocSliceRtimeStatS
 */
HIDDEN uint32_t VosHimallocSliceRtimeStat(uint8_t ucArenaId, uint32_t uiType, VosHimallocSliceRtimeStatS *pstStats);

/**
 * @ingroup himalloc_algo
 * @brief 该函数提供日志记录功能。
 * @param uiLevel     [IN] 日志等级
 * @param usIntCode   [IN] 错误码
 * @param pscFmt      [IN] 字符串信息
 * @retval 无。
 */
typedef void (*VosHimallocLogFunc)(uint32_t uiLevel, uint32_t usIntCode, const char *pscFmt, ...);

/**
 * @ingroup himalloc_algo
 * @brief himalloc日志钩子注册函数
 * @par himalloc日志钩子注册函数
 * @attention \n
 * 1.该接口主要开放给DOPRA内存分区配置申请算法使用，其他CBB请勿使用。
 * @param  pfnHook       [IN]     日志钩子
 * @retval 无
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see
 */
HIDDEN void VosHimallocLogFuncReg(VosHimallocLogFunc pfnHook);

/**
 * @ingroup himalloc_algo
 * @brief himalloc大块内存的释放是否交还给系统内存的策略设置
 * @par himalloc大块内存的释放是否交还给系统内存的策略设置
 * @attention \n
 * 1.为适配鸿蒙测，默认值为true
 * 2.而如果使用himalloc_algo模块，即使用多分区的himalloc算法，需要先调用该接口设置为false
 * 3.而当为true时，大块内存释放后将交还给操作系统
 * 4.当为false时，大块内存释放后将还由himalloc进行管理
 * @param  recycleStrategy       [IN]     策略true or false
 * @retval 无
 * @par 依赖：无。
 * @li himalloc_algo.h：该接口声明所在的文件。
 * @see
 */
HIDDEN void VosHimallocBlockRecycleSet(bool recycleStrategy);

#ifdef __cplusplus
}
#endif

#endif
