/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#ifndef VOS_HIMALLOCBLOCK_H
#define VOS_HIMALLOCBLOCK_H

#include <stdint.h>
#include "himalloc_inner.h"
#include "vos_himalloctype.h"
#include "vos_himalloctlb.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

/* 用于标示BLOCK的状态 */
typedef enum tagHimallocBlockState {
    VOS_HIMALLOC_BLOCK_STAT_ALLOCAED = 0,  /* 此Block已被分配 */
    VOS_HIMALLOC_BLOCK_STAT_FREE_NORMAL,   /* 此Block是空闲的，且已映射物理内存 */
    VOS_HIMALLOC_BLOCK_STAT_FREE_CLEAN,    /* 此Block是空闲的，且未映射物理内存 */
} VOS_HIMALLOC_BLOCK_STATE_E;

/* 111表示完整的扩展块，110表示扩展块的头部，011表示扩展块的尾部 */
#define VOS_HIMALLOC_BLOCK_PROPRERTY_INTEGRITY 0X7U
#define VOS_HIMALLOC_BLOCK_PROPRERTY_HEAD 0x6U
#define VOS_HIMALLOC_BLOCK_PROPRERTY_TAIL 0x3U
#define VOS_HIMALLOC_BLOCK_PRIVATE_DATA_MASK 0xFU

/* 向上按照物理页大小对齐 */
#define VOS_HIMALLOC_BLOCK_ADDR_ALIGNUP(pvAddr) \
    (((uintptr_t)(pvAddr) + VOS_HIMALLOC_PAGE_SIZE - 1) & (~(uintptr_t)(VOS_HIMALLOC_PAGE_SIZE - 1)))

/* 向下按照物理页大小对齐 */
#define VOS_HIMALLOC_BLOCK_ADDR_ALIGNDOWN(pvAddr) \
    ((uintptr_t)(pvAddr) & (~(uintptr_t)(VOS_HIMALLOC_PAGE_SIZE - 1)))

extern HIDDEN VOS_HIMALLOC_BLOCK *VosHimallocBlockAlloc(VOS_HIMALLOC_ARENA_S *pstArena, size_t uvSize);

extern HIDDEN void *VosHimallocBlockAllocAlign(VOS_HIMALLOC_ARENA_S *pstArena, size_t uvSize, size_t uvAlign);

extern HIDDEN uint32_t VosHimallocBlockFree(VOS_HIMALLOC_ARENA_S *pstArena, VOS_HIMALLOC_BLOCK *pstBlock);

extern HIDDEN void VosHimallocBlockHeapCtlInit(VOS_HIMALLOC_BLOCK_HEAP_S *pstHeap);

static HIMALLOC_ALWAYS_INLINE uintptr_t VosHimallocBlockStartRegGet(uintptr_t uvStart)
{
    return uvStart + VOS_HIMALLOC_VPAGE_SIZE_HALF - 1UL;
}

static HIMALLOC_ALWAYS_INLINE uintptr_t VosHimallocBlockEndRegGet(uintptr_t uvEndAddr)
{
    return uvEndAddr - (VOS_HIMALLOC_VPAGE_SIZE_HALF + 1UL);
}

static HIMALLOC_ALWAYS_INLINE VOS_HIMALLOC_BLOCK *VosHimallocBlockGetImp(
    VOS_HIMALLOC_TLB_INFO_S *pstTlbInfo, void *pAddr)
{
    return VosHimallocTlbMatchImpl(pstTlbInfo, (size_t)((uintptr_t)pAddr));
}

static HIMALLOC_ALWAYS_INLINE VOS_HIMALLOC_BLOCK_STATE_E VosHimallocBlockTreeStateGet(VOS_BIT_TREE_BIN_S *pstTreeBin)
{
    uintptr_t uvTreeState;

    uvTreeState = VosBitTreeDataGet(pstTreeBin);

    return (VOS_HIMALLOC_BLOCK_STATE_E)(uvTreeState & VOS_HIMALLOC_BLOCK_PRIVATE_DATA_MASK);
}

extern HIDDEN uint32_t VosHimallocBlockDelImpl(VOS_HIMALLOC_ARENA_S *pstArena, VOS_HIMALLOC_BLOCK *pstBlock);

extern HIDDEN void VosHimallocBlockHeapCleanSetUp(VOS_HIMALLOC_ARENA_S *pstArena, bool bDecayMode);

static HIMALLOC_ALWAYS_INLINE size_t VosHimallocBlockTreeFreeSizeGet(VOS_BIT_TREE_BIN_S *pstTreeBin)
{
    size_t uvFreeSize;
    size_t uvTreeState;

    uvFreeSize = (size_t)VosBitTreeDataGet(pstTreeBin);
    uvTreeState = uvFreeSize & VOS_HIMALLOC_BLOCK_PRIVATE_DATA_MASK;

    return (uvFreeSize - uvTreeState);
}

static HIMALLOC_ALWAYS_INLINE void VosHimallocBlockTreeFreeSizeAdd(uintptr_t *puvTreeFreeSize, size_t uvAddSize)
{
    (*puvTreeFreeSize) += uvAddSize;
}

static HIMALLOC_ALWAYS_INLINE void VosHimallocBlockTreeFreeSizeSub(uintptr_t *puvTreeFreeSize, size_t uvSubSize)
{
    (*puvTreeFreeSize) -= uvSubSize;
}

static HIMALLOC_ALWAYS_INLINE bool VosHimallocCleanTickTok(int16_t *psiTick, uint16_t usTicks)
{
    int16_t siDeclineTicks = *psiTick;

    siDeclineTicks -= (int16_t)usTicks;
    if (HIMALLOC_UNLIKELY(siDeclineTicks <= 0)) {
        /* Tick计数满足，先重置Tick值使得其余线程可以正常开始计数 */
        VosMemDecayTickReset(psiTick);
        return true;
    }

    *psiTick = siDeclineTicks;
    return false;
}

static HIMALLOC_ALWAYS_INLINE void VosHimallocBlockHeapCleanMaybe(VOS_HIMALLOC_ARENA_S *pstArena)
{
    if (HIMALLOC_UNLIKELY(VosHimallocCleanTickTok(&pstArena->siTick, 1))) {
        VosHimallocBlockHeapCleanSetUp(pstArena, true);
    }
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */

#endif
