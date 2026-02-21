/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#ifndef VOS_HIMALLOCSLICE_H
#define VOS_HIMALLOCSLICE_H

#include "vos_trickydiv.h"
#include "vos_bitmapgrp.h"
#include "vos_himalloctype.h"
#include "vos_himallocbase.h"
#include "vos_himallocsclass.h"
#include "vos_himalloc_errno.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

extern HIDDEN uint32_t g_uiHimallocSliceSize;

#define VOS_HIMALLOC_SLICE_SIZE_MAX g_uiHimallocSliceSize

/* slice 切片相关的信息 */
typedef struct tagHimallocSliceInfo {
    uint16_t usSliceSize;        /* 切割后的silce的长度 */
    uint16_t usSliceNum;         /* 单次切割可以获取的slice的数量 */
    uint32_t uiBlockSize;        /* 切割此slice需要的Block size */
    VOS_TRICKY_DIV_INFO stDivInfo; /* tricky division相关的信息 */
    VOS_BITMAP_INFO stBitmapInfo;  /* slice空闲与否的bitmap信息 */
} VOS_HIMALLOC_SLICE_INFO;

extern HIDDEN VOS_HIMALLOC_SLICE_INFO g_astHimallocSliceInfo[VOS_HIMALLOC_MAX_SMALL_INDEX];

static HIMALLOC_ALWAYS_INLINE VOS_HIMALLOC_SLICE_INFO *VosHimallocSliceInfoGet(uint32_t uiIndex)
{
    /* 调用接口可确保uiIndex的有效性 */
    return &(g_astHimallocSliceInfo[uiIndex]);
}

static HIMALLOC_ALWAYS_INLINE void *VosHimallocSliceAddrOffset(VOS_HIMALLOC_BLOCK *pstCurFreeBlock,
    uint32_t uiSliceIndex)
{
    size_t uvOffset;
    size_t uvSliceSize;

    uvSliceSize = VosHimallocIndex2Size(pstCurFreeBlock->ucSizeClass);

    /* 需强转为size_t后执行乘法防止溢出 */
    uvOffset = (size_t)uiSliceIndex * uvSliceSize;
    return ulong_to_ptr((ptr_to_ulong(pstCurFreeBlock->pStart) + uvOffset), void);
}

static HIMALLOC_ALWAYS_INLINE uint16_t VosHimallocSliceIndexGet(const VOS_HIMALLOC_BLOCK *pstBlock,
    VOS_TRICKY_DIV_INFO *pstDivInfo, const void *pAddr)
{
    uint32_t uiDiffLength;

    /* 获取slice的索引 */
    VOS_HIMALLOC_ASSERT(pAddr >= (pstBlock->pStart));
    uiDiffLength = (uint32_t)(ptr_to_ulong(pAddr) - ptr_to_ulong(pstBlock->pStart));
    return (uint16_t)VosTrickyDiv(uiDiffLength, pstDivInfo);
}

#ifdef HIMALLOC_TCACHE_POLICY_LIST // tcache方案采用链表组织缓存资源池的策略，底噪开销较小

static HIMALLOC_ALWAYS_INLINE void VosHimallocSliceTcacheSet(VOS_HIMALLOC_TCACHE_SLICE *pTcacheSlice, void *pAddr)
{
    ((VOS_HIMALLOC_TCACHE_FREE_SLICE_CB *)pAddr)->pstTcacheNextSlice = pTcacheSlice->pFreeStackList;
    pTcacheSlice->pFreeStackList = (VOS_HIMALLOC_TCACHE_FREE_SLICE_CB *)pAddr;
    pTcacheSlice->usNumCached += 1U;
}

static HIMALLOC_ALWAYS_INLINE void *VosHimallocSliceTcacheGet(VOS_HIMALLOC_TCACHE_SLICE *pTcacheSlice)
{
    void *pAddr = (void *)pTcacheSlice->pFreeStackList;

    pTcacheSlice->pFreeStackList = pTcacheSlice->pFreeStackList->pstTcacheNextSlice;
    pTcacheSlice->usNumCached -= 1U;
    return pAddr;
}

#else // tcache方案采用数组组织缓存资源池的策略，底噪开销较大

static HIMALLOC_ALWAYS_INLINE void VosHimallocSliceTcacheSet(VOS_HIMALLOC_TCACHE_SLICE *pTcacheSlice, void *pAddr)
{
    pTcacheSlice->usNumCached += 1U;
    *(void **)(pTcacheSlice->ppFreeStack - pTcacheSlice->usNumCached + 1) = pAddr;
}

static HIMALLOC_ALWAYS_INLINE void *VosHimallocSliceTcacheGet(VOS_HIMALLOC_TCACHE_SLICE *pTcacheSlice)
{
    pTcacheSlice->usNumCached -= 1U;
    return *(pTcacheSlice->ppFreeStack - pTcacheSlice->usNumCached);
}

#endif

extern HIDDEN void *VosHimallocSliceBinAlloc(VOS_HIMALLOC_ARENA_S *pstArena, uint16_t usBinIndex);

extern HIDDEN uint32_t VosHimallocSliceFree(VOS_HIMALLOC_ARENA_S *pstArena, VOS_HIMALLOC_BLOCK *pstBlock,
    const void *pAddr);

extern HIDDEN uint32_t VosHimallocSliceBinsLockInit(uint32_t uiNum, VOS_HIMALLOC_LOCK_S astBinsLock[]);

extern HIDDEN void VosHimallocSliceBinsLockDestory(uint32_t uiNum, VOS_HIMALLOC_LOCK_S astBinsLock[]);

extern HIDDEN void VosHimallocSliceBinsInit(VOS_HIMALLOC_SLICE_HEAP_S *pstHeap);

extern HIDDEN uint32_t VosHimallocSliceModuleInit(void);

extern HIDDEN int32_t VosHimallocUsedSliceTraverse(VOS_HIMALLOC_BLOCK *pstBlock,
    const HimallocMemParseFunc pfnParseHook, void *pResult);

extern HIDDEN uint32_t VosHimallocSliceBinFlush(VOS_HIMALLOC_ARENA_S *pstArena, void *pstTcacheSliceClass,
    VOS_HIMALLOC_TLB_INFO_S *pstThreadTlb, uint32_t uiTotalFlush, uint8_t ucSizeClass);

extern HIDDEN uint32_t VosHimallocSliceBinFill(VOS_HIMALLOC_ARENA_S *pstArena, void *pstTcacheSliceClass,
    uint32_t uiNumToFill, uint16_t usBinIndex);

extern HIDDEN int32_t VosHimallocUsedSliceBitmapTraverse(VOS_HIMALLOC_BLOCK *pstBlock,
    const HimallocMemParseFunc pfnParseHook, void *pResult);

#ifdef __cplusplus
}
#endif /* __cpluscplus */

#endif
