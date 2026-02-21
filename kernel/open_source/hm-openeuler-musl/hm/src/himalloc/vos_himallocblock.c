/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#include <sys/mman.h>
#include "vos_himallocgrow.h"
#include "vos_himallocarena.h"
#include "vos_memdecay.h"
#include "vos_himallocblock.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

typedef uint32_t (*VOS_HIMALLOC_STAT_IDX_GET_HOOK)(size_t *puvSizeMap, uint32_t uiTypeNum, size_t uvSize);

static bool g_isBlockShouldRecycle = true;

static HIMALLOC_ALWAYS_INLINE void VosHimallocBlockAddPropertySet(VOS_HIMALLOC_BLOCK *pstBlock)
{
    pstBlock->ucProperty = (uint8_t)VOS_HIMALLOC_BLOCK_PROPRERTY_INTEGRITY;
}

static HIMALLOC_ALWAYS_INLINE void VosHimallocBlockSplitPropertySet(VOS_HIMALLOC_BLOCK *pstBlockHead,
    VOS_HIMALLOC_BLOCK *pstBlockTail)
{
    pstBlockTail->ucProperty = pstBlockHead->ucProperty & (uint8_t)VOS_HIMALLOC_BLOCK_PROPRERTY_TAIL;
    pstBlockHead->ucProperty &= VOS_HIMALLOC_BLOCK_PROPRERTY_HEAD;
}

static HIMALLOC_ALWAYS_INLINE void VosHimallocBlockMergePropertySet(VOS_HIMALLOC_BLOCK *pstBlockHead,
    const VOS_HIMALLOC_BLOCK *pstBlockTail)
{
    pstBlockHead->ucProperty |= pstBlockTail->ucProperty;
}

static HIMALLOC_ALWAYS_INLINE bool VosHimallocIsBlockIntegrity(const VOS_HIMALLOC_BLOCK *pstBlock)
{
    return (pstBlock->ucProperty == VOS_HIMALLOC_BLOCK_PROPRERTY_INTEGRITY);
}

HIDDEN void VosHimallocBlockHeapCtlInit(VOS_HIMALLOC_BLOCK_HEAP_S *pstHeap)
{
    VosBitTreeInit(&(pstHeap->stBlockTreeBinNormal), (uintptr_t)VOS_HIMALLOC_BLOCK_STAT_FREE_NORMAL);

    VosBitTreeInit(&(pstHeap->stBlockTreeBinClean), (uintptr_t)VOS_HIMALLOC_BLOCK_STAT_FREE_CLEAN);

    (void)memset(&(pstHeap->stHeapStats), 0, sizeof(VOS_HIMALLOC_BLOCK_HEAP_STAT_S));

    return;
}

static HIMALLOC_ALWAYS_INLINE void VosHimallocBlockNodeInit(VOS_HIMALLOC_BLOCK *pstBlock, void *pStart,
    uintptr_t uvLength, uint8_t ucBlockState, VosMemAreaRegionTypeE enAllocType)
{
    pstBlock->pStart = pStart;
    pstBlock->ucSliced = 0U;
    pstBlock->enAllocType = (uint8_t)enAllocType;
    pstBlock->ucBlockState = ucBlockState;
#if (__WORDSIZE == 64)
    pstBlock->uiReserve = 0U;
#endif
    pstBlock->uvLength = uvLength;

    return;
}

static HIMALLOC_ALWAYS_INLINE void *VosHimallocBlockEndAddr(VOS_HIMALLOC_BLOCK *pstBlock)
{
    return ulong_to_ptr((ptr_to_ulong(pstBlock->pStart) + pstBlock->uvLength), void);
}

static HIMALLOC_ALWAYS_INLINE uintptr_t VosHimallocBlockPrev(VOS_HIMALLOC_BLOCK *pstBlock)
{
    return ptr_to_ulong(pstBlock->pStart) - (VOS_HIMALLOC_VPAGE_SIZE_HALF + 1UL);
}

static HIMALLOC_ALWAYS_INLINE uintptr_t VosHimallocBlockNext(VOS_HIMALLOC_BLOCK *pstBlock)
{
    uintptr_t uvEndAddr = ptr_to_ulong(VosHimallocBlockEndAddr(pstBlock));
    return (uvEndAddr + (VOS_HIMALLOC_VPAGE_SIZE_HALF - 1UL));
}

static HIMALLOC_ALWAYS_INLINE uint32_t VosHimallocBlockEndRegister(VOS_HIMALLOC_TLB_INFO_S *pstTlbInfo, void *pAddr,
    void *pEntry)
{
    return VosHimallocTlbSet(pstTlbInfo, VosHimallocBlockEndRegGet(ptr_to_ulong(pAddr)), pEntry);
}

static HIMALLOC_ALWAYS_INLINE uint32_t VosHimallocBlockStartRegister(VOS_HIMALLOC_TLB_INFO_S *pstTlbInfo, void *pAddr,
    void *pEntry)
{
    return VosHimallocTlbSet(pstTlbInfo, VosHimallocBlockStartRegGet(ptr_to_ulong(pAddr)), pEntry);
}

static void VosHimallocBlockUnRegister(VOS_HIMALLOC_TLB_INFO_S *pstTlbInfo, VOS_HIMALLOC_BLOCK *pstBlock)
{
    (void)VosHimallocBlockStartRegister(pstTlbInfo, pstBlock->pStart, HIMALLOC_NULL_PTR);
    (void)VosHimallocBlockEndRegister(pstTlbInfo, VosHimallocBlockEndAddr(pstBlock), HIMALLOC_NULL_PTR);
    return;
}

static HIMALLOC_ALWAYS_INLINE uint32_t VosHimallocBlockRegister(VOS_HIMALLOC_TLB_INFO_S *pstTlbInfo,
    VOS_HIMALLOC_BLOCK *pstBlock)
{
    uint32_t uiRet;

    uiRet = VosHimallocBlockStartRegister(pstTlbInfo, pstBlock->pStart, pstBlock);
    if (uiRet == HIMALLOC_OK) {
        uiRet = VosHimallocBlockEndRegister(pstTlbInfo, VosHimallocBlockEndAddr(pstBlock), pstBlock);
        if (uiRet != HIMALLOC_OK) {
            /* 使用函数代替内敛函数回退提升性能 */
            VosHimallocBlockUnRegister(pstTlbInfo, pstBlock);
        }
    }

    return uiRet;
}

static HIMALLOC_ALWAYS_INLINE void VosHimallocBlockInsert(VOS_BIT_TREE_BIN_S *pstTreeBin, VOS_HIMALLOC_BLOCK *pstBlock,
    bool bSplitBlock)
{
    pstBlock->ucBlockState = (uint8_t)VosHimallocBlockTreeStateGet(pstTreeBin);

    /* 将剩余节点归还BIT树 */
    (void)VosBitTreeEntryInsert(pstTreeBin, &((pstBlock)->unMetaDataNode.stTreeNode), pstBlock->uvLength, bSplitBlock);

    return;
}

static uint32_t VosHimallocBlockMemAlign(void *pAreaAddr, size_t uvAreaSize, void **ppBlockAddr,
    size_t *puvBlockSize)
{
    size_t uvSizeAlign;
    void *pBlockAddrAlign;

    /* 与dlm扩展行为保持一致，地址和大小不进行页对齐 */
    if (ptr_to_ulong(pAreaAddr) > ptr_to_ulong(pAreaAddr) + uvAreaSize) {
        VosHimallocLogWrite(HIMALLOC_ERRNO_OVERFLOW, "[HIMALLOC-%s]area addr and size(%lu) overflow", __FUNCTION__,
            uvAreaSize);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_OVERFLOW;
    }

    pBlockAddrAlign = ulong_to_ptr(((ptr_to_ulong(pAreaAddr) + sizeof(size_t) - 1UL) & ~(sizeof(size_t) - 1UL)), void);
    uvSizeAlign = ptr_to_ulong(pAreaAddr) + uvAreaSize - ptr_to_ulong(pBlockAddrAlign);
    uvSizeAlign = ((uvSizeAlign >> VOS_HIMALLOC_LG_BLOCK_SIZE) << VOS_HIMALLOC_LG_BLOCK_SIZE);

    *puvBlockSize = uvSizeAlign;
    *ppBlockAddr = pBlockAddrAlign;
    return HIMALLOC_OK;
}

static VOS_HIMALLOC_BLOCK *VosHimallocBlockPrepare(
    VOS_HIMALLOC_ARENA_S *pstArena, void *pAddr, size_t uvAllocSize, VosMemAreaRegionTypeE enAllocType)
{
    uint32_t uiRet;
    void *pBlockAddr;
    size_t uvBlockSize;
    VOS_HIMALLOC_BLOCK *pstBlock = HIMALLOC_NULL_PTR;
    VOS_HIMALLOC_BLOCK_STATE_E enBlockState;

    if (VosHimallocBlockMemAlign(pAddr, uvAllocSize, &pBlockAddr, &uvBlockSize) != HIMALLOC_OK) {
        return HIMALLOC_NULL_PTR;
    }

    if (uvBlockSize < 2 * VOS_HIMALLOC_VPAGE_SIZE) { /* 2倍的VOS_HIMALLOC_VPAGE_SIZE */
        return HIMALLOC_NULL_PTR;
    }

    pstBlock = VosMemZoneAlloc(pstArena->zoneBlock);
    if (pstBlock == HIMALLOC_NULL_PTR) {
        return HIMALLOC_NULL_PTR;
    }

    uiRet = VosMemAreaRegionAdd(g_HimallocAreaId, ptr_to_ulong(pAddr), uvAllocSize, pstArena->ucArenaId, enAllocType);
    if (uiRet != HIMALLOC_OK) {
        (void)VosMemZoneFree(pstArena->zoneBlock, pstBlock);
        return HIMALLOC_NULL_PTR;
    }

#if (VOS_HIMALLOC_VPAGE_SIZE != (1 << VOS_HIMALLOC_LG_BLOCK_SIZE))
    /* 在人质解救开关打开的场景下，mmap类型的扩展块尾部不再预留VPAGE的空间允许不同扩展块合并 */
    uvBlockSize -= VOS_HIMALLOC_VPAGE_SIZE;
#endif

    /* 只有mmap属性的内存支持人质回收 */
    if (enAllocType == MEM_AREA_REGION_MMAP) {
        enBlockState = VOS_HIMALLOC_BLOCK_STAT_FREE_CLEAN;
    } else {
        enBlockState = VOS_HIMALLOC_BLOCK_STAT_FREE_NORMAL;
    }

    VosHimallocBlockNodeInit(pstBlock, pBlockAddr, uvBlockSize, enBlockState, enAllocType);
    pstBlock->ucArenaId = pstArena->ucArenaId;
    VosHimallocBlockAddPropertySet(pstBlock);

    uiRet = VosHimallocBlockRegister(&pstArena->stTlb, pstBlock);
    if (uiRet != HIMALLOC_OK) {
        (void)VosMemAreaRegionDel(g_HimallocAreaId, ptr_to_ulong(pAddr), uvAllocSize);
        (void)VosMemZoneFree(pstArena->zoneBlock, pstBlock);
        return HIMALLOC_NULL_PTR;
    }

    return pstBlock;
}

static VOS_HIMALLOC_BLOCK *VosHimallocBlockAllocCore(VOS_HIMALLOC_ARENA_S *pstArena, size_t uvSize)
{
    void *pAddr;
    size_t uvAllocSize = uvSize;
    VOS_HIMALLOC_BLOCK *pstBlock;
    VosMemAreaRegionTypeE enAllocType = MEM_AREA_REGION_MMAP;

    pAddr = VosHimallocGrowAlloc(pstArena, &uvAllocSize, &enAllocType);
    if (pAddr == HIMALLOC_NULL_PTR) {
        VosHimallocLogWrite(HIMALLOC_ERRNO_NOMEM, "[HIMALLOC-%s]system memory is not enough, size(%lu)",
            __FUNCTION__, uvAllocSize);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }

    pstBlock = VosHimallocBlockPrepare(pstArena, pAddr, uvAllocSize, enAllocType);
    if (pstBlock == HIMALLOC_NULL_PTR) {
        (void)VosHimallocGrowFree(pstArena, pAddr, uvAllocSize, enAllocType);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }

    return pstBlock;
}

static void VosHimallocBlockRollBack(VOS_HIMALLOC_TLB_INFO_S *pstTlbInfo, VOS_HIMALLOC_BLOCK *pstBlock,
    size_t uvNewSize)
{
    /* 由于解映射时不需要申请内存，因此去注册不会失败 */
    VosHimallocBlockUnRegister(pstTlbInfo, pstBlock);

    pstBlock->uvLength = uvNewSize;
    (void)VosHimallocBlockRegister(pstTlbInfo, pstBlock);

    return;
}

static VOS_HIMALLOC_BLOCK *VosHimallocBlockSplit(VOS_HIMALLOC_ARENA_S *pstArena, VOS_BIT_TREE_BIN_S *pstTreeBin,
    VOS_HIMALLOC_BLOCK *pstSplitBlock, size_t uvApplySize, bool bPrev)
{
    uint32_t uiResult;
    size_t uvRemainSize;
    size_t uvRegisterAddr;
    VOS_HIMALLOC_BLOCK *pstNewBlock;

    /*
     * 小于VPAGE的内存，vpagemap映射时存在冲突，不再单独成Block
     * 后续想复用，可以在后向Block空闲时，合并这部分空闲内存
     */
    if (pstSplitBlock->uvLength < (uvApplySize + VOS_HIMALLOC_VPAGE_SIZE)) {
        __hmattr_sideff_resolved("no relative side effect");
        return pstSplitBlock;
    }

    uvRemainSize = pstSplitBlock->uvLength - uvApplySize;

    pstNewBlock = VosMemZoneAlloc(pstArena->zoneBlock);
    if (HIMALLOC_UNLIKELY(pstNewBlock == HIMALLOC_NULL_PTR)) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }

    /* 初始化新Block */
    VosHimallocBlockNodeInit(pstNewBlock, ulong_to_ptr((ptr_to_ulong(pstSplitBlock->pStart) + uvApplySize), void),
        uvRemainSize, pstSplitBlock->ucBlockState, (VosMemAreaRegionTypeE)pstSplitBlock->enAllocType);
    pstNewBlock->ucArenaId = pstSplitBlock->ucArenaId;
    VosHimallocBlockSplitPropertySet(pstSplitBlock, pstNewBlock);

    /*
     * pstSplitBlock的start已注册，因此仅需注册end即可
     * pstSplitBlock的end就是pstNewBlock的start，使用pstNewBlock减少一次计算
     */
    pstSplitBlock->uvLength = uvApplySize;
    uvRegisterAddr = VosHimallocBlockEndRegGet(ptr_to_ulong(pstNewBlock->pStart));
    uiResult = VosHimallocTlbSet(&(pstArena->stTlb), uvRegisterAddr, pstSplitBlock);
    if (HIMALLOC_UNLIKELY(uiResult != HIMALLOC_OK)) {
        pstSplitBlock->uvLength = uvApplySize + uvRemainSize;
        (void)VosMemZoneFree(pstArena->zoneBlock, pstNewBlock);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }

    /* 注册新的Block, 新Block的end先前已注册一次，不会失败 */
    uvRegisterAddr = VOS_HIMALLOC_VPAGE_NEXT(uvRegisterAddr);
    uiResult = VosHimallocTlbSet(&(pstArena->stTlb), uvRegisterAddr, pstNewBlock);
    if (HIMALLOC_UNLIKELY(uiResult != HIMALLOC_OK)) {
        VosHimallocBlockRollBack(&(pstArena->stTlb), pstSplitBlock, uvApplySize + uvRemainSize);
        (void)VosMemZoneFree(pstArena->zoneBlock, pstNewBlock);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }
    (void)VosHimallocBlockEndRegister(&(pstArena->stTlb), VosHimallocBlockEndAddr(pstNewBlock), pstNewBlock);

    /* 将剩余节点归还BIT树 */
    if (bPrev == true) {
        VosHimallocBlockInsert(pstTreeBin, pstNewBlock, true);
        __hmattr_sideff_resolved("no relative side effect");
        return pstSplitBlock;
    } else {
        VosHimallocBlockInsert(pstTreeBin, pstSplitBlock, true);
        __hmattr_sideff_resolved("no relative side effect");
        return pstNewBlock;
    }
}

static VOS_HIMALLOC_BLOCK *VosHimallocBlockExtract(VOS_HIMALLOC_ARENA_S *pstArena, VOS_BIT_TREE_BIN_S *pstTreeBin,
    size_t uvApplySize)
{
    VOS_BIT_TREE_NODE_S *pstTreeNode;
    VOS_HIMALLOC_BLOCK *pstPrevBlock;
    VOS_HIMALLOC_BLOCK *pstAllocBlock;

    pstTreeNode = VosBitTreeEntryExtract(pstTreeBin, uvApplySize);
    if (pstTreeNode == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }

    pstAllocBlock = VOS_BIT_TREE_ENTRY(pstTreeNode, VOS_HIMALLOC_BLOCK, unMetaDataNode);
    pstAllocBlock->ucBlockState = (uint8_t)VOS_HIMALLOC_BLOCK_STAT_ALLOCAED;

    pstPrevBlock = VosHimallocBlockSplit(pstArena, pstTreeBin, pstAllocBlock, uvApplySize, true);
    if (pstPrevBlock == HIMALLOC_NULL_PTR) {
        VosHimallocBlockInsert(pstTreeBin, pstAllocBlock, false);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }

    /* 成功从BitTree中申请一块Block,Tick--并判断是否达到Clean回收条件 */
    VosHimallocBlockTreeFreeSizeSub(&(pstTreeBin->uvPrivateData), pstPrevBlock->uvLength);
    VosHimallocBlockHeapCleanMaybe(pstArena);

    return pstPrevBlock;
}

/* 扩展失败，最后把所有的NormalTree上的内存回收并入CleanTree再从CleanTree中尝试申请 */
static VOS_HIMALLOC_BLOCK *VosHimallocBlockAllocFinalTry(VOS_HIMALLOC_ARENA_S *pstArena, size_t uvSize)
{
    VOS_HIMALLOC_BLOCK *pstBlock;

    VosHimallocBlockHeapCleanSetUp(pstArena, false);

    pstBlock = VosHimallocBlockExtract(pstArena, &((pstArena->stBlockHeap).stBlockTreeBinClean), uvSize);
    if (pstBlock == HIMALLOC_NULL_PTR) {
        pstBlock = VosHimallocBlockAllocCore(pstArena, uvSize);
    }

    return pstBlock;
}

static VOS_HIMALLOC_BLOCK *VosHimallocBlockAllocHard(VOS_HIMALLOC_ARENA_S *pstArena, size_t uvSize)
{
    size_t uvGrowSize;
    VOS_HIMALLOC_BLOCK *pstBlock;
    VOS_HIMALLOC_BLOCK *pstPrevBlock;
    VOS_BIT_TREE_BIN_S *pstTreeBin;

    pstBlock = VosHimallocBlockAllocCore(pstArena, uvSize);
    if (pstBlock == HIMALLOC_NULL_PTR) {
        pstBlock = VosHimallocBlockAllocFinalTry(pstArena, uvSize);
        if (pstBlock == HIMALLOC_NULL_PTR) {
            __hmattr_sideff_resolved("no relative side effect");
            return HIMALLOC_NULL_PTR;
        }
    }
    uvGrowSize = pstBlock->uvLength;

    if (VosHimallocStatsEnable() == true) {
        pstArena->stBlockHeap.stHeapStats.uvNormalSize += uvSize;
    }

    /* 如果启用了人质回收功能，每次扩展先将获取的整块扩展块Block进行Decomit操作并且置入CleanTree中，
       并将切割得到的前一块申请所需要的内存Block进行Commit操作，最终直接会返回出去进行分配。 */
    if (pstBlock->ucBlockState == VOS_HIMALLOC_BLOCK_STAT_FREE_CLEAN) {
        pstTreeBin = &((pstArena->stBlockHeap).stBlockTreeBinClean);
    } else {
        pstTreeBin = &((pstArena->stBlockHeap).stBlockTreeBinNormal);
    }
    pstPrevBlock = VosHimallocBlockSplit(pstArena, pstTreeBin, pstBlock, uvSize, true);
    if (pstPrevBlock == HIMALLOC_NULL_PTR) {
        VosHimallocBlockInsert(pstTreeBin, pstBlock, false);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }

    VosHimallocBlockTreeFreeSizeAdd(&(pstTreeBin->uvPrivateData), (uvGrowSize - pstPrevBlock->uvLength));

    pstPrevBlock->ucBlockState = VOS_HIMALLOC_BLOCK_STAT_ALLOCAED;

    return pstPrevBlock;
}

HIDDEN VOS_HIMALLOC_BLOCK *VosHimallocBlockAlloc(VOS_HIMALLOC_ARENA_S *pstArena, size_t uvSize)
{
    size_t uvUserSize;
    VOS_HIMALLOC_BLOCK *pstBlock;
    VOS_HIMALLOC_BLOCK_HEAP_S *pstBlockHeap;

    uvUserSize = VOS_HIMALLOC_LG_ALIGN(uvSize, VOS_HIMALLOC_LG_BLOCK_SIZE);

    pstBlockHeap = &(pstArena->stBlockHeap);
    (void)VosHimallocLock(&(pstBlockHeap->stTreeBinLock));

    pstBlock = VosHimallocBlockExtract(pstArena, &(pstBlockHeap->stBlockTreeBinNormal), uvUserSize);
    if (pstBlock == HIMALLOC_NULL_PTR) {
        /* 即使未开启人质解救开关，仍然进入CleanTree里找一次，不会影响性能，因为仅在自动扩展的这次申请中会进入该分支 */
        pstBlock = VosHimallocBlockExtract(pstArena, &(pstBlockHeap->stBlockTreeBinClean), uvUserSize);
        if (pstBlock == HIMALLOC_NULL_PTR) {
            /* 内存不足，需要进行扩展 */
            pstBlock = VosHimallocBlockAllocHard(pstArena, uvUserSize);
        }
    }

    if (VosHimallocStatsEnable() == true) {
        if (pstBlock != HIMALLOC_NULL_PTR) {
            pstBlockHeap->stHeapStats.ullTotalNMalloc++;
        }
    }

    VosHimallocUnLock(&(pstBlockHeap->stTreeBinLock));

    return pstBlock;
}

HIDDEN void *VosHimallocBlockAllocAlign(VOS_HIMALLOC_ARENA_S *pstArena, size_t uvSize, size_t uvAlign)
{
    VOS_HIMALLOC_BLOCK *pstBlock;
    size_t uvTotalSize;
    uintptr_t uvBegin;
    uintptr_t uvEnd;
    uintptr_t uvAddr;
    VOS_BIT_TREE_BIN_S *pstTreeBin = &((pstArena->stBlockHeap).stBlockTreeBinNormal);
    VOS_HIMALLOC_BLOCK_HEAP_S *pstBlockHeap = &(pstArena->stBlockHeap);
    VOS_HIMALLOC_BLOCK *pstSplitBlock;
    size_t uvAlignTmp = uvAlign;
    size_t uvAlignSize;

    if (uvAlignTmp <= VOS_HIMALLOC_VPAGE_SIZE) {
        pstBlock = VosHimallocBlockAlloc(pstArena, uvSize);
        if (pstBlock == NULL) {
                return HIMALLOC_NULL_PTR;
        }
        return pstBlock->pStart;
    }

    /* 多申请uvAlign的内存 */
    uvAlignSize = (uvSize + VOS_HIMALLOC_VPAGE_SIZE - 1U) & (~(size_t)(VOS_HIMALLOC_VPAGE_SIZE - 1U));
    uvTotalSize = uvAlignSize + uvAlignTmp;
    pstBlock = VosHimallocBlockAlloc(pstArena, uvTotalSize);
    if (pstBlock == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }

    uvBegin = ptr_to_ulong(pstBlock->pStart);
    uvEnd = uvBegin + uvTotalSize;
    uvAddr = (uvBegin + uvAlignTmp - 1U) & ~(uvAlignTmp - 1U);
    pstSplitBlock = pstBlock;

    /* 归还首尾多余空间 */
    (void)VosHimallocLock(&(pstBlockHeap->stTreeBinLock));
    if (uvBegin < uvAddr) {
        pstSplitBlock = VosHimallocBlockSplit(pstArena, pstTreeBin, pstSplitBlock, uvAddr - uvBegin, false);
    }

    if (uvAddr + uvAlignSize < uvEnd) {
        pstSplitBlock = VosHimallocBlockSplit(pstArena, pstTreeBin, pstSplitBlock, uvAlignSize, true);
    }
    VosHimallocUnLock(&(pstBlockHeap->stTreeBinLock));

    if (pstSplitBlock == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }

    return pstSplitBlock->pStart;
}

/* 头部或整体不合并 */
static HIMALLOC_ALWAYS_INLINE bool VosHimallocBlockCoalescePrevPermit(VOS_HIMALLOC_BLOCK *pstPrevBlock,
    const VOS_HIMALLOC_BLOCK *pstNextBlock)
{
    bool bRet = true;
    if ((pstPrevBlock->ucBlockState != pstNextBlock->ucBlockState) ||
        (VosHimallocBlockEndAddr(pstPrevBlock) != pstNextBlock->pStart) ||
        (pstPrevBlock->ucArenaId != pstNextBlock->ucArenaId) ||
        (pstPrevBlock->enAllocType != pstNextBlock->enAllocType) ||
        (pstNextBlock->ucProperty == VOS_HIMALLOC_BLOCK_PROPRERTY_HEAD) ||
        (pstNextBlock->ucProperty == VOS_HIMALLOC_BLOCK_PROPRERTY_INTEGRITY)) {
        bRet = false;
    }

    return bRet;
}

/* 尾部或整体不合并 */
static HIMALLOC_ALWAYS_INLINE bool VosHimallocBlockCoalesceNextPermit(VOS_HIMALLOC_BLOCK *pstPrevBlock,
    const VOS_HIMALLOC_BLOCK *pstNextBlock)
{
	bool bRet = true;
    if ((pstPrevBlock->ucBlockState != pstNextBlock->ucBlockState) ||
        (VosHimallocBlockEndAddr(pstPrevBlock) != pstNextBlock->pStart) ||
        (pstPrevBlock->ucArenaId != pstNextBlock->ucArenaId) ||
        (pstPrevBlock->enAllocType != pstNextBlock->enAllocType) ||
        (pstPrevBlock->ucProperty == VOS_HIMALLOC_BLOCK_PROPRERTY_TAIL) ||
        (pstPrevBlock->ucProperty == VOS_HIMALLOC_BLOCK_PROPRERTY_INTEGRITY)) {
        bRet = false;
    }

    return bRet;
}

static VOS_HIMALLOC_BLOCK *VosHimallocBlockMerge(VOS_HIMALLOC_ARENA_S *pstArena, VOS_HIMALLOC_BLOCK *pstPrevBlock,
    VOS_HIMALLOC_BLOCK *pstNextBlock)
{
    uintptr_t uvCancelKey;

    VOS_HIMALLOC_ASSERT(VosHimallocBlockEndAddr(pstPrevBlock) == pstNextBlock->pStart);

    /* prev与next相邻，prev的尾节点就是Next的启始节点，循环取消前向节点的终止节点与后向节点的启始节点 */
    uvCancelKey = VosHimallocBlockEndRegGet(ptr_to_ulong(pstNextBlock->pStart));
    (void)VosHimallocTlbSet(&(pstArena->stTlb), uvCancelKey, HIMALLOC_NULL_PTR);
    (void)VosHimallocTlbSet(&(pstArena->stTlb), VOS_HIMALLOC_VPAGE_NEXT(uvCancelKey), HIMALLOC_NULL_PTR);

    /* 新Block长度更新后需要重新映射首部与尾部，前面uvCancelKey可能会导致首部被解映射，因此首尾须重新映射 */
    pstPrevBlock->uvLength += pstNextBlock->uvLength;
    VosHimallocBlockMergePropertySet(pstPrevBlock, pstNextBlock);

    (void)VosHimallocBlockStartRegister(&(pstArena->stTlb), pstPrevBlock->pStart, pstPrevBlock);
    (void)VosHimallocBlockEndRegister(&(pstArena->stTlb), VosHimallocBlockEndAddr(pstPrevBlock), pstPrevBlock);
    (void)VosMemZoneFree(pstArena->zoneBlock, pstNextBlock);
    return pstPrevBlock;
}

static HIMALLOC_ALWAYS_INLINE VOS_HIMALLOC_BLOCK *VosHimallocBlockCoalesceInsert(VOS_HIMALLOC_ARENA_S *pstArena,
    VOS_BIT_TREE_BIN_S *pstTreeBin, VOS_HIMALLOC_BLOCK *pstBlock)
{
    VOS_HIMALLOC_BLOCK *pstNextBlock;
    VOS_HIMALLOC_BLOCK *pstPrevBlock;
    VOS_HIMALLOC_BLOCK *pstBlockTmp = pstBlock;

    /* 尝试与前向节点合并 */
    pstPrevBlock = VosHimallocTlbMatchImpl(&(pstArena->stTlb), (size_t)VosHimallocBlockPrev(pstBlockTmp));
    if (pstPrevBlock != HIMALLOC_NULL_PTR) {
        if (VosHimallocBlockCoalescePrevPermit(pstPrevBlock, pstBlockTmp) == true) {
            /* 从树上摘除前向节点 */
            VosBitTreeEntryRemove(pstTreeBin, &((pstPrevBlock)->unMetaDataNode.stTreeNode));

            /* 与前向节点合并 */
            pstBlockTmp = VosHimallocBlockMerge(pstArena, pstPrevBlock, pstBlockTmp);
        }
    }

    /* 尝试与后向节点合并 */
    pstNextBlock = VosHimallocTlbMatchImpl(&(pstArena->stTlb), (size_t)VosHimallocBlockNext(pstBlockTmp));
    if (pstNextBlock != HIMALLOC_NULL_PTR) {
        if (VosHimallocBlockCoalesceNextPermit(pstBlockTmp, pstNextBlock) == true) {
            /* 从树上摘除后向节点 */
            VosBitTreeEntryRemove(pstTreeBin, &((pstNextBlock)->unMetaDataNode.stTreeNode));

            /* 与后向节点合并 */
            pstBlockTmp = VosHimallocBlockMerge(pstArena, pstBlockTmp, pstNextBlock);
        }
    }

    VosBitTreeEntryInsert(pstTreeBin, &((pstBlockTmp)->unMetaDataNode.stTreeNode), pstBlockTmp->uvLength, false);

    return pstBlockTmp;
}

static void VosHimallocBlockTreeRemove(VOS_HIMALLOC_ARENA_S *pstArena, VOS_HIMALLOC_BLOCK *pstBlock)
{
    VOS_HIMALLOC_BLOCK_HEAP_S *pstBlockHeap;
    VOS_BIT_TREE_BIN_S *pstTreeBin;

    /* 树上摘除节点 */
    pstBlockHeap = &(pstArena->stBlockHeap);
    if (pstBlock->ucBlockState == (uint8_t)VOS_HIMALLOC_BLOCK_STAT_FREE_NORMAL) {
        pstTreeBin = &(pstBlockHeap->stBlockTreeBinNormal);
    } else {
        pstTreeBin = &(pstBlockHeap->stBlockTreeBinClean);
    }
    VosBitTreeEntryRemove(pstTreeBin, &((pstBlock)->unMetaDataNode.stTreeNode));

    VosHimallocBlockTreeFreeSizeSub(&(pstTreeBin->uvPrivateData), pstBlock->uvLength);

    return;
}

static HIMALLOC_ALWAYS_INLINE bool VosHimallocBlockShouldShrink(const VOS_HIMALLOC_ARENA_S *pstArena,
    const VOS_HIMALLOC_BLOCK *pstBlock)
{
    (void)pstArena;
    (void)pstBlock;
    return g_isBlockShouldRecycle;
}

static bool VosHimallocBlockShrinkJudge(VOS_HIMALLOC_ARENA_S *pstArena, VOS_HIMALLOC_BLOCK *pstBlock)
{
    /* 非用户手动传入内存，一旦识别到扩展块被整块释放，则直接从树上摘除节点防止后续被申请走 */
    if (pstBlock->enAllocType != MEM_AREA_REGION_CUSTOM_MANUAL) {
        VosHimallocBlockTreeRemove(pstArena, pstBlock);
    }

    if (VosHimallocBlockShouldShrink(pstArena, pstBlock) == true) {
        (void)VosHimallocBlockDelImpl(pstArena, pstBlock);
        (void)VosMemZoneFree(pstArena->zoneBlock, pstBlock);
        return true;
    }

    return false;
}

HIDDEN uint32_t VosHimallocBlockFree(VOS_HIMALLOC_ARENA_S *pstArena, VOS_HIMALLOC_BLOCK *pstBlock)
{
    VOS_HIMALLOC_BLOCK_HEAP_S *pstBlockHeap;
    VOS_BIT_TREE_BIN_S *pstTreeBin;
    VOS_HIMALLOC_BLOCK *pstBlockTmp = pstBlock;

    /* 防止重复释放 */
    if (HIMALLOC_UNLIKELY(pstBlock->ucBlockState != (uint8_t)VOS_HIMALLOC_BLOCK_STAT_ALLOCAED)) {
        __hmattr_sideff_resolved("no relative side effect");
        if (g_himalloc_enable_mcheck) {
            abort(); /* 该地址获取的pstBlock未被申请，认为是double free，在MALLOC_CHECK_打开的情况下abort */
        }

        return (uint32_t)VOS_HIMALLOC_FREE_REF_ZERO_E;
    }

    pstBlockHeap = &(pstArena->stBlockHeap);
    pstTreeBin = &(pstBlockHeap->stBlockTreeBinNormal);
    (void)VosHimallocLock(&(pstBlockHeap->stTreeBinLock));

    /* 回收应该在此次Block内存释放前，防止这次回收恰好将该Block内存回收了 */
    VosHimallocBlockTreeFreeSizeAdd(&(pstTreeBin->uvPrivateData), pstBlock->uvLength);
    VosHimallocBlockHeapCleanMaybe(pstArena);

    /* 需要先更新状态，后续Coalesce时会根据状态判断是否允许合并 */
    pstBlockTmp->ucBlockState = (uint8_t)VOS_HIMALLOC_BLOCK_STAT_FREE_NORMAL;
    pstBlockTmp = VosHimallocBlockCoalesceInsert(pstArena, pstTreeBin, pstBlockTmp);
    if (VosHimallocIsBlockIntegrity(pstBlockTmp) == true) {
        (void)VosHimallocBlockShrinkJudge(pstArena, pstBlockTmp);
    }

    if (VosHimallocStatsEnable() == true) {
        pstBlockHeap->stHeapStats.ullTotalNDalloc++;
    }
    VosHimallocUnLock(&(pstBlockHeap->stTreeBinLock));

    return HIMALLOC_OK;
}

static uint32_t VosRegionDelRangeCheckAndSet(VOS_HIMALLOC_BLOCK *pstBlock, uintptr_t *puvStart, uintptr_t *puvEnd)
{
    VOS_MEM_AREA_REGION_S stAreaRegion;
    uintptr_t uvStartTmp = ptr_to_ulong(pstBlock->pStart);
    uintptr_t uvEndTmp = ptr_to_ulong(pstBlock->pStart) + (uintptr_t)(pstBlock->uvLength);
    uintptr_t uvBlockEnd;
    void *pBlockStart;
    size_t uvBlockSize;

    if (VosMemAreaRegionGet(g_HimallocAreaId, ptr_to_ulong(pstBlock->pStart), &stAreaRegion) != HIMALLOC_OK) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_INVAL;
    }

    /* 获取region中block的内存范围 */
    if (VosHimallocBlockMemAlign(ulong_to_ptr(stAreaRegion.uvStart, void), stAreaRegion.uvLen, &pBlockStart,
        &uvBlockSize) != HIMALLOC_OK) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_INVAL;
    }

    /*
     * 为支持自动回收，所有类型的扩展块尾部都预留VPAGE的空间防止不同扩展块合并
     * 后续需要解决人质问题，MEM_AREA_REGION_MMAP类型的扩展块不需要预留VPAGE
     */
    uvBlockEnd = ptr_to_ulong(pBlockStart) + uvBlockSize - VOS_HIMALLOC_VPAGE_SIZE_BACKUP;
    if (stAreaRegion.enAllocType != MEM_AREA_REGION_MMAP) {
        /* 除了map出来的内存，其他不允许部分回收 */
        if ((uvStartTmp != ptr_to_ulong(pBlockStart)) || (uvEndTmp != uvBlockEnd)) {
            __hmattr_sideff_resolved("no relative side effect");
            return HIMALLOC_ERRNO_INVAL;
        }
    }

    /* 如果位于扩展块的头部，需要将头部页对齐偏移的空间一起删除 */
    if (uvStartTmp == ptr_to_ulong(pBlockStart)) {
        uvStartTmp = stAreaRegion.uvStart;
    }

    /* 如果位于扩展块的尾部，需要将尾部页对齐偏移的空间一起删除 */
    if (uvEndTmp == uvBlockEnd) {
        uvEndTmp = stAreaRegion.uvStart + stAreaRegion.uvLen;
    }

    *puvStart = uvStartTmp;
    *puvEnd = uvEndTmp;

    return HIMALLOC_OK;
}

HIDDEN uint32_t VosHimallocBlockDelImpl(VOS_HIMALLOC_ARENA_S *pstArena, VOS_HIMALLOC_BLOCK *pstBlock)
{
    uintptr_t uvStart = ptr_to_ulong(pstBlock->pStart);
    uintptr_t uvEnd = ptr_to_ulong(pstBlock->pStart) + pstBlock->uvLength;
    if (VosRegionDelRangeCheckAndSet(pstBlock, &uvStart, &uvEnd) != HIMALLOC_OK) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_INVAL;
    }

    /* 删除线性区 */
    if (VosMemAreaRegionDel(g_HimallocAreaId, uvStart, uvEnd - uvStart) != HIMALLOC_OK) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_INVAL;
    }

    /* 解除映射关系 */
    VosHimallocBlockUnRegister(&pstArena->stTlb, pstBlock);

    /* 用户手动传入内存不支持自动回收，需要在该接口内进行节点摘除 */
    if (pstBlock->enAllocType == MEM_AREA_REGION_CUSTOM_MANUAL) {
        VosHimallocBlockTreeRemove(pstArena, pstBlock);
    }

    /* 内存块归还OS */
    VosHimallocGrowFree(pstArena, ulong_to_ptr(uvStart, void), uvEnd - uvStart,
        (VosMemAreaRegionTypeE)pstBlock->enAllocType);

    return HIMALLOC_OK;
}

HIDDEN uint32_t VosHimallocBlockVisitInfoGet(uint8_t ucArenaId, uint64_t *pullAllocTimes, uint64_t *pullFreeTimes)
{
    VOS_HIMALLOC_ARENA_S *pstArena = HIMALLOC_NULL_PTR;
    VOS_HIMALLOC_BLOCK_HEAP_STAT_S *pstHeapStat = HIMALLOC_NULL_PTR;

    pstArena = VosHimallocArenaGet(ucArenaId);
    if (pstArena == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_INVAL;
    }
    pstHeapStat = &(pstArena->stBlockHeap.stHeapStats);

    *pullAllocTimes = pstHeapStat->ullTotalNMalloc;
    *pullFreeTimes = pstHeapStat->ullTotalNDalloc;
    return HIMALLOC_OK;
}

static void VosHimallocCleanBlockAlign(const VOS_HIMALLOC_BLOCK *pstBlock, uintptr_t *puvBeginAddr,
    uintptr_t *puvEndAddr)
{
    uintptr_t uvTmpBeginAddr = *puvBeginAddr;
    uintptr_t uvTmpEndAddr = *puvEndAddr;

    if ((uintptr_t)pstBlock->pStart <= (uintptr_t)VOS_HIMALLOC_BLOCK_ADDR_ALIGNDOWN(uvTmpBeginAddr)) {
        uvTmpBeginAddr = VOS_HIMALLOC_BLOCK_ADDR_ALIGNDOWN(uvTmpBeginAddr);
    } else {
        uvTmpBeginAddr = VOS_HIMALLOC_BLOCK_ADDR_ALIGNUP(uvTmpBeginAddr);
    }

    if (((uintptr_t)pstBlock->pStart + pstBlock->uvLength) >= VOS_HIMALLOC_BLOCK_ADDR_ALIGNUP(uvTmpEndAddr)) {
        uvTmpEndAddr = VOS_HIMALLOC_BLOCK_ADDR_ALIGNUP(uvTmpEndAddr);
    } else {
        uvTmpEndAddr = VOS_HIMALLOC_BLOCK_ADDR_ALIGNDOWN(uvTmpEndAddr);
    }

    *puvBeginAddr = uvTmpBeginAddr;
    *puvEndAddr = uvTmpEndAddr;
}

HIDDEN size_t VosHimallocBlockClean(VOS_HIMALLOC_ARENA_S *pstArena, VOS_HIMALLOC_BLOCK *pstBlock)
{
    uintptr_t uvBeginAddr = (uintptr_t)pstBlock->pStart;
    uintptr_t uvEndAddr = (uintptr_t)pstBlock->pStart + pstBlock->uvLength;
    VOS_BIT_TREE_BIN_S *pstTreeBin = &((pstArena->stBlockHeap).stBlockTreeBinClean);
    VOS_HIMALLOC_BLOCK *pstMergeBlock;

    VosHimallocBlockTreeFreeSizeAdd(&(pstTreeBin->uvPrivateData), pstBlock->uvLength);
    pstBlock->ucBlockState = VOS_HIMALLOC_BLOCK_STAT_FREE_CLEAN;

    pstMergeBlock = VosHimallocBlockCoalesceInsert(pstArena, pstTreeBin, pstBlock);
    VosHimallocCleanBlockAlign(pstMergeBlock, &uvBeginAddr, &uvEndAddr);
    if (VosHimallocIsBlockIntegrity(pstMergeBlock) == true) {
        /* 如果在人质回收过程中发现该内存可以被整块收缩，则不进行Clean操作，直接返回交由OS进行收缩 */
        if (VosHimallocBlockShrinkJudge(pstArena, pstMergeBlock) == true) {
            return uvEndAddr - uvBeginAddr;
        }
    }

    if (uvEndAddr > uvBeginAddr) {
        VOS_HIMALLOC_ASSERT(uvEndAddr - uvBeginAddr >= VOS_HIMALLOC_PAGE_SIZE);
        VosMemDecayClean((void *)uvBeginAddr, uvEndAddr - uvBeginAddr);
    }

    return uvEndAddr - uvBeginAddr;
}

/* 从BitTree上从大到小遍历内存块进行回收，直到回收的内存大小超过uvDecaySize */
HIDDEN uint32_t VosHimallocBlockHeapTraverseToClean(VOS_HIMALLOC_ARENA_S *pstArena, size_t uvDecaySize)
{
    size_t uvSizeToClean = uvDecaySize;
    size_t uvCleanSize;
    VOS_BIT_TREE_NODE_S *pstMaxKeyNode;
    VOS_BIT_TREE_BIN_S *pstTreeBin;
    VOS_HIMALLOC_BLOCK *pstMaxSizeBlock;

    pstTreeBin = &((pstArena->stBlockHeap).stBlockTreeBinNormal);
    while (uvSizeToClean > 0) {
        pstMaxKeyNode = VosBitTreeMaxKeyNodeExtract(pstTreeBin);
        if (pstMaxKeyNode == HIMALLOC_NULL_PTR) {
            break;
        }
        pstMaxSizeBlock = VOS_BIT_TREE_ENTRY(pstMaxKeyNode, VOS_HIMALLOC_BLOCK, unMetaDataNode);
        VosHimallocBlockTreeFreeSizeSub(&(pstTreeBin->uvPrivateData), pstMaxSizeBlock->uvLength);
        (void)VosHimallocBlockClean(pstArena, pstMaxSizeBlock);
        uvCleanSize = pstMaxSizeBlock->uvLength;
        if (uvCleanSize >= uvSizeToClean) {
            uvSizeToClean = 0;
        } else {
            uvSizeToClean -= uvCleanSize;
        }
    }

    return HIMALLOC_OK;
}

HIDDEN void VosHimallocBlockHeapCleanEpoch(VOS_HIMALLOC_ARENA_S *pstArena, size_t uvDecaySize)
{
    size_t uvSizeToClean = uvDecaySize;
    VOS_BIT_TREE_NODE_S *pstBestFitNode;
    VOS_HIMALLOC_BLOCK *pstBestFitBlock;
    VOS_HIMALLOC_BLOCK_HEAP_S *pstBlockHeap;
    VOS_BIT_TREE_BIN_S *pstTreeBin;

    pstBlockHeap = &(pstArena->stBlockHeap);
    pstTreeBin = &(pstBlockHeap->stBlockTreeBinNormal);

    VosHimallocLock(&(pstBlockHeap->stTreeBinLock));
    pstBestFitNode = VosBitTreeEntryExtract(pstTreeBin, uvSizeToClean);
    if (pstBestFitNode == HIMALLOC_NULL_PTR) {
        /* Best-Fit寻找失败，说明需要回收的Size大于该分区中最大的内存块Size */
        (void)VosHimallocBlockHeapTraverseToClean(pstArena, uvSizeToClean);
        VosHimallocUnLock(&(pstBlockHeap->stTreeBinLock));
        return;
    }

    pstBestFitBlock = VOS_BIT_TREE_ENTRY(pstBestFitNode, VOS_HIMALLOC_BLOCK, unMetaDataNode);
    VosHimallocBlockTreeFreeSizeSub(&(pstTreeBin->uvPrivateData), pstBestFitBlock->uvLength);

    /* 直接全部回收，即使Best-Fit的Size比需要回收的Size大得多 */
    (void)VosHimallocBlockClean(pstArena, pstBestFitBlock);
    VosHimallocUnLock(&(pstBlockHeap->stTreeBinLock));

    return;
}

HIDDEN void VosHimallocBlockHeapDirectClean(VOS_HIMALLOC_ARENA_S *pstArena)
{
    size_t uvCurFreeSize;

    /* 传入0xFFFFFFFF把NormalTree上的所有内存都回收掉 */
    uvCurFreeSize = (size_t)(-1);
    VosHimallocBlockHeapCleanEpoch(pstArena, uvCurFreeSize);
}


/* 该接口为物理内存回收流程启动接口，在此接口中会判断是否要通过老化进行物理内存回收，不启用老化则会直接把NormalTree中的内存一次性全部回收
   启用老化则会计算此次Epoch中需要回收的内存大小，并从NormalTree中选取合适的内存块进行回收，最后该接口还会重置Tick数 */
HIDDEN void VosHimallocBlockHeapCleanSetUp(VOS_HIMALLOC_ARENA_S *pstArena, bool bDecayMode)
{
    size_t uvCurFreeSize;
    size_t uvDecaySize;
    VOS_HIMALLOC_BLOCK_HEAP_S *pstBlockHeap = &(pstArena->stBlockHeap);
    VOS_MEM_DECAY_INFO_S *pstDecayInfo = pstArena->pstDecayInfo;

    if (HIMALLOC_UNLIKELY(bDecayMode == false)) {
        VosHimallocBlockHeapDirectClean(pstArena);
        return;
    }

    /* 不进行加锁获取，使用瞬时值也没有问题 */
    uvCurFreeSize = VosHimallocBlockTreeFreeSizeGet(&(pstBlockHeap->stBlockTreeBinNormal));
    uvDecaySize = VosMemDecaySizeGet(pstDecayInfo, uvCurFreeSize);
    if (uvDecaySize != 0) {
        /* 从此处开始进入新的一个回收的epoch流程 */
        uvDecaySize = VOS_HIMALLOC_LG_ALIGN(uvDecaySize, VOS_HIMALLOC_LG_PAGE_SIZE);
        VosHimallocBlockHeapCleanEpoch(pstArena, uvDecaySize);
    }
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */
