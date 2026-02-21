/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#ifndef VOS_HIMALLOCBASE_H
#define VOS_HIMALLOCBASE_H

#include <stdbool.h>
#include <time.h>
#include "himalloc_inner.h"
#include "cstl_list.h"
#include "osal_lock.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

/* 管理内存与虚拟内存映射的虚拟页的对数大小 */
#define VOS_HIMALLOC_LG_VPAGE_SIZE 11U

/* 管理内存与虚拟内存映射的虚拟页的大小 */
#define VOS_HIMALLOC_VPAGE_SIZE (1UL << VOS_HIMALLOC_LG_VPAGE_SIZE)

/* 管理内存与虚拟内存映射的虚拟页的折半 */
#define VOS_HIMALLOC_VPAGE_SIZE_HALF (1UL << (VOS_HIMALLOC_LG_VPAGE_SIZE - 1U))

/* 管理内存与虚拟内存映射的跨页大小 */
#define VOS_HIMALLOC_VPAGE_ONE_MORE (VOS_HIMALLOC_VPAGE_SIZE + VOS_HIMALLOC_VPAGE_SIZE_HALF)

/* 管理内存与虚拟内存映射的虚拟页的掩码 */
#define VOS_HIMALLOC_VPAGE_SIZE_MASK (VOS_HIMALLOC_VPAGE_SIZE - 1U)

/* 根据地址计算后一个VPAGE */
#define VOS_HIMALLOC_VPAGE_NEXT(uvAddr) (((size_t)(uvAddr) + VOS_HIMALLOC_VPAGE_SIZE))

/* 根据地址计算前一个VPAGE */
#define VOS_HIMALLOC_VPAGE_PREV(uvAddr) (((size_t)(uvAddr)-VOS_HIMALLOC_VPAGE_SIZE))

/*
 * himalloc页大小的对数，Block以此为单位管理
 * 此页大小有可能不同于操作系统内存的页大小
 */
#define VOS_HIMALLOC_LG_PAGE_SIZE 12

/* himalloc的页大小 */
#define VOS_HIMALLOC_PAGE_SIZE (1UL << VOS_HIMALLOC_LG_PAGE_SIZE)

/* himalloc页大小的掩码 */
#define VOS_HIMALLOC_PAGE_SIZE_MASK (VOS_HIMALLOC_PAGE_SIZE - 1U)

/* 单个Block可以切割的最大slice的对数 */
#define VOS_HIMALLOC_LG_MAX_SLICES (VOS_HIMALLOC_LG_PAGE_SIZE - VOS_HIMALLOC_LG_MIN_SIZE)

/* 单个Block可以切割的最大slice数量 */
#define VOS_HIMALLOC_MAX_SLICES (1UL << VOS_HIMALLOC_LG_MAX_SLICES)

/* slice分配上限的大小 */
#define VOS_HIMALLOC_MAX_SLICE_SIZE (1UL << 11)

/* 单个Group的bit数量 */
#define VOS_HIMALLOC_BITNUM_IN_GROUP 32U

/* 单个Block需要的最大的bitmap组 */
#define VOS_HIMALLOC_MAX_BITMAP_GROUPS \
    ((VOS_HIMALLOC_MAX_SLICES + VOS_HIMALLOC_BITNUM_IN_GROUP - 1U) / VOS_HIMALLOC_BITNUM_IN_GROUP)

/* 最小分配内存对数, 此值决定了block控制块大小 */
#define VOS_HIMALLOC_LG_MIN_SIZE 5

/* 最小分配内存的大小 */
#define VOS_HIMALLOC_MIN_SIZE (1UL << VOS_HIMALLOC_LG_MIN_SIZE)

/*
 * Block内存对齐长度的对数
 * 适当增加对齐长度可以减少bittree的高度
 */
#define VOS_HIMALLOC_LG_BLOCK_SIZE VOS_HIMALLOC_LG_VPAGE_SIZE

/*
 * 管理内存与虚拟内存映射的虚拟页的大小影响的预留长度，当2K对齐时，长度为0
 */
#if (VOS_HIMALLOC_VPAGE_SIZE != (1 << VOS_HIMALLOC_LG_BLOCK_SIZE))
#define VOS_HIMALLOC_VPAGE_SIZE_BACKUP VOS_HIMALLOC_VPAGE_SIZE
#else
#define VOS_HIMALLOC_VPAGE_SIZE_BACKUP 0
#endif

/* 将uvSize按照2的幂次方对齐 */
#define VOS_HIMALLOC_LG_ALIGN(uvSize, uiAlign) ((((uvSize) + ((1UL << (uiAlign)) - 1U)) >> (uiAlign)) << (uiAlign))

/* 根据hybrid节点的位置偏移至type的头部 */
#define VOS_HIMALLOC_HYBRID_LIST_ENTRY(item, type, hybrid) \
    ((type *)((uintptr_t)(item) - (uintptr_t)(&(((type *)0UL)->unMetaDataNode.hybrid).stLinkNode)))

/* size_t的bit位掩码 */
#define VOS_HIMALLOC_SIZE_BIT_MASK (sizeof(size_t) * 8U - 1U)

typedef struct tagHimallocHybrid {
    uint32_t auiFreeBitmap[VOS_HIMALLOC_MAX_BITMAP_GROUPS]; /* 负责维护Block内Slice的空闲 */
    CSTL_LIST_HEAD_S stLinkNode;                            /* 此链表节点必须放置在首部 */
} VOS_HIMALLOC_HYBRID_S;

typedef struct tagHimallocLock {
    OSAL_LOCK_T stLock;
    size_t uvRet;
} VOS_HIMALLOC_LOCK_S;

typedef struct tagHimallocRandom {
    size_t uvSeed;
    size_t uvCnt;
} VOS_HIMALLOC_RANDOM_S;

static HIMALLOC_ALWAYS_INLINE uint32_t VosHimallocLock(VOS_HIMALLOC_LOCK_S *pstLock)
{
    return OSAL_LockLock(&(pstLock->stLock));
}

static HIMALLOC_ALWAYS_INLINE void VosHimallocUnLock(VOS_HIMALLOC_LOCK_S *pstLock)
{
    OSAL_LockUnLock(&(pstLock->stLock), (uint32_t)(pstLock->uvRet));
    return;
}

static HIMALLOC_ALWAYS_INLINE uint32_t VosHimallocLockCreate(VOS_HIMALLOC_LOCK_S *pstLock)
{
    return OSAL_LockCreate(&(pstLock->stLock));
}

static HIMALLOC_ALWAYS_INLINE uint32_t VosHimallocLockDestory(VOS_HIMALLOC_LOCK_S *pstLock)
{
    return OSAL_LockDestroy(&(pstLock->stLock));
}

static HIMALLOC_ALWAYS_INLINE void VosHimallocHybridAdd(VOS_HIMALLOC_HYBRID_S *pstHybrid,
    CSTL_LIST_HEAD_S *pstInsertPoint)
{
    CSTL_ListAdd(&(pstHybrid->stLinkNode), pstInsertPoint);
    return;
}

static HIMALLOC_ALWAYS_INLINE void VosHimallocHybridAddBefore(VOS_HIMALLOC_HYBRID_S *pstHybrid,
    CSTL_LIST_HEAD_S *pstInsertPoint)
{
    CSTL_ListAdd(&(pstHybrid->stLinkNode), pstInsertPoint->prev);
    return;
}

static HIMALLOC_ALWAYS_INLINE void VosHimallocHybridRemove(VOS_HIMALLOC_HYBRID_S *pstHybrid)
{
    CSTL_ListRemove(&(pstHybrid->stLinkNode));
    return;
}

static HIMALLOC_ALWAYS_INLINE void VosHimallocHybridLinkInit(VOS_HIMALLOC_HYBRID_S *pstHybrid)
{
    CSTL_ListInit(&(pstHybrid->stLinkNode));
    return;
}

HIDDEN bool VosHimallocStatsEnable(void);

HIDDEN uint32_t VosHimallocBaseInit(void);

#ifdef __cplusplus
}
#endif /* __cpluscplus */

#endif
