/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#include "himalloc_inner.h"
#include "vos_himallocassert.h"
#include "vos_trickydiv.h"
#include "vos_himalloctype.h"
#include "vos_himallocarena.h"
#include "vos_himallocslice.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

HIDDEN VOS_HIMALLOC_SLICE_INFO g_astHimallocSliceInfo[VOS_HIMALLOC_MAX_SMALL_INDEX];

HIDDEN uint32_t g_uiHimallocSliceSize = ((uint32_t)1 << VOS_HIMALLOC_LG_VPAGE_SIZE);

HIDDEN void VosHimallocSliceBinsLockDestory(uint32_t uiNum, VOS_HIMALLOC_LOCK_S astBinsLock[])
{
    uint32_t uiIndex;

    for (uiIndex = 0; uiIndex < uiNum; ++uiIndex) {
        (void)VosHimallocLockDestory(&(astBinsLock[uiIndex]));
    }

    return;
}

HIDDEN uint32_t VosHimallocSliceBinsLockInit(uint32_t uiNum, VOS_HIMALLOC_LOCK_S astBinsLock[])
{
    uint32_t uiRet = HIMALLOC_OK;
    uint32_t uiIndex;

    for (uiIndex = 0; uiIndex < uiNum; ++uiIndex) {
        uiRet = VosHimallocLockCreate(&(astBinsLock[uiIndex]));
        if (uiRet != HIMALLOC_OK) {
            VosHimallocSliceBinsLockDestory(uiIndex, astBinsLock);
            break;
        }
    }

    return uiRet;
}

HIDDEN void VosHimallocSliceBinsInit(VOS_HIMALLOC_SLICE_HEAP_S *pstHeap)
{
    uint32_t uiIndex;
    uint32_t uiLockIndex;
    VOS_HIMALLOC_SLICE_BIN_S *pstSliceBin;

    for (uiIndex = 0; uiIndex < VOS_HIMALLOC_MAX_SMALL_INDEX; ++uiIndex) {
        pstSliceBin = &((pstHeap->astSliceBins)[uiIndex]);
        (void)memset(pstSliceBin, 0, sizeof(VOS_HIMALLOC_SLICE_BIN_S));

        uiLockIndex = uiIndex % VOS_HIMALLOC_SLICE_BINS_LOCK_NUM;
        pstSliceBin->pstLock = &((pstHeap->astBinsLock)[uiLockIndex]);
        pstSliceBin->pCurFreeBlock = HIMALLOC_NULL_PTR;
        CSTL_ListInit(&(pstSliceBin->stFreeBlockHead));
        CSTL_ListInit(&(pstSliceBin->stNotFreeBlockHead));
        pstSliceBin->uvBinIndex = uiIndex;
    }

    return;
}

static uint32_t VosHimallocSliceCutPagesGet(uint16_t usSliceSize)
{
    uint32_t uiNumPages = 1;
    uint32_t uiBlockSize;
    uint32_t uiCurRemainSize;
    uint32_t uiMaxRemainSize;
    uint32_t uiBestBlockSize;
    uint32_t uiBestBlockRemain;
    const uint32_t uiMaxBlockNumPages = 32;

    uiBestBlockRemain = HIMALLOC_NULL_DWORD;
    uiBestBlockSize = (uint32_t)VOS_HIMALLOC_LG_ALIGN(usSliceSize, VOS_HIMALLOC_LG_VPAGE_SIZE);
    while (uiNumPages <= uiMaxBlockNumPages) {
        /* 计算切割slice的block的大小，确保切割后浪费的内存开销小于12.5% */
        uiBlockSize = uiNumPages << VOS_HIMALLOC_LG_VPAGE_SIZE;
        uiNumPages++;
        if (uiBlockSize < usSliceSize) {
            /* 至少需要确保能够切割成单个slice */
            continue;
        }

        uiCurRemainSize = uiBlockSize % usSliceSize;
        uiMaxRemainSize = uiBlockSize >> 3; /* 3在本处通俗易懂 */
        if (uiCurRemainSize <= uiMaxRemainSize) {
            uiBestBlockSize = uiBlockSize;
            break;
        } else if (uiCurRemainSize < uiBestBlockRemain) {
            uiBestBlockRemain = uiCurRemainSize;
            uiBestBlockSize = uiBlockSize;
        } else {
            continue;
        }
    }

    return uiBestBlockSize >> VOS_HIMALLOC_LG_VPAGE_SIZE;
}

static void VosHimallocSliceInfoInit(void)
{
    uint32_t uiIndex;
    uint32_t uiSliceSize;
    uint32_t uiBlockSize;
    uint32_t uiCurNumPages;
    uint32_t uiLastNumPages;
    uint32_t uiSliceNum;
    VOS_HIMALLOC_SLICE_INFO *pstSliceInfo;

    uiLastNumPages = 0;
    for (uiIndex = 0; uiIndex < VOS_HIMALLOC_MAX_SMALL_INDEX; uiIndex++) {
        uiSliceSize = (uint32_t)VosHimallocIndex2Size((uint16_t)uiIndex);
        uiCurNumPages = VosHimallocSliceCutPagesGet((uint16_t)uiSliceSize);
        /* 切割slice使用的block不小于上个slice切割使用的block */
        if (uiLastNumPages != 0 && uiCurNumPages < uiLastNumPages) {
            uiCurNumPages = uiLastNumPages;
        }

        /* 初始化index对应的slice信息 */
        pstSliceInfo = &(g_astHimallocSliceInfo[uiIndex]);
        uiBlockSize = uiCurNumPages << VOS_HIMALLOC_LG_VPAGE_SIZE;
        pstSliceInfo->usSliceSize = (uint16_t)uiSliceSize;
        pstSliceInfo->uiBlockSize = uiBlockSize;
        uiSliceNum = uiBlockSize / uiSliceSize;
        pstSliceInfo->usSliceNum = (uint16_t)uiSliceNum;

        /* 初始化bitmap信息 */
        pstSliceInfo->stBitmapInfo.usGroupNum = (uint16_t)((uiSliceNum + VOS_BITMAP_NBITS - 1U) / VOS_BITMAP_NBITS);
        pstSliceInfo->stBitmapInfo.usTotalBit = (uint16_t)uiSliceNum;

        /* 初始化tricky division信息 */
        (void)VosTrickyDivInit(uiSliceSize, &(pstSliceInfo->stDivInfo));

        /* 更新最后一次切割slice使用的pages */
        uiLastNumPages = uiCurNumPages;
    }

    return;
}

static uint32_t VosHimallocSliceSizeInit(void)
{
    uint8_t ucSizeClass;

    /* 后续人质问题开发过程中可能会引入Block内存按4KPage对齐，如果slice内存的上限配置太小可能会导致内碎片增大，
       启动内存开销上升，所以将slice内存的上限提供配置项 */
    /* uiMaxSliceSize需要大于VOS_HIMALLOC_MAX_SLICE_SIZE并且小于VOS_HIMALLOC_VPAGE_SIZE */
    const uint32_t uiMaxSliceSize = (uint32_t)VOS_HIMALLOC_MAX_SLICE_SIZE;

    ucSizeClass = (uint8_t)VosHimallocSize2Index(uiMaxSliceSize);
    g_uiHimallocSliceSize = (uint32_t)VosHimallocIndex2Size(ucSizeClass);

    return HIMALLOC_OK;
}

HIDDEN uint32_t VosHimallocSliceModuleInit(void)
{
    uint32_t uiRet;

    VosHimallocSliceInfoInit();

    uiRet = VosHimallocSliceSizeInit();

    return uiRet;
}

static HIMALLOC_ALWAYS_INLINE void *VosHimallocSliceBinAllocDirect(VOS_HIMALLOC_BLOCK *pstCurFreeBlock)
{
    uint32_t uiSliceIndex;

    VOS_HIMALLOC_ASSERT(pstCurFreeBlock->ucFreeSlice != 0);

    /* 随机分配Block首部或尾部的slice提升防攻击能力 */
    pstCurFreeBlock->ucFreeSlice--;
    uiSliceIndex = VosBitmapCFS((pstCurFreeBlock)->unMetaDataNode.stHybrid.auiFreeBitmap,
        (uint32_t)VOS_HIMALLOC_MAX_BITMAP_GROUPS);

    VOS_HIMALLOC_ASSERT(uiSliceIndex != HIMALLOC_NULL_DWORD);

    return VosHimallocSliceAddrOffset(pstCurFreeBlock, uiSliceIndex);
}

static void *VosHimallocSliceAllocFromList(VOS_HIMALLOC_SLICE_BIN_S *pstBinCtl)
{
    CSTL_LIST_HEAD_S *pstListNode;
    VOS_HIMALLOC_BLOCK *pstFreeBlock;

    VOS_HIMALLOC_ASSERT(!CSTL_ListIsEmpty(&(pstBinCtl->stFreeBlockHead)));

    pstListNode = pstBinCtl->stFreeBlockHead.next;

    pstFreeBlock = VOS_HIMALLOC_HYBRID_LIST_ENTRY(pstListNode, VOS_HIMALLOC_BLOCK, stHybrid);
    VosHimallocHybridRemove(&((pstFreeBlock)->unMetaDataNode.stHybrid));
    pstBinCtl->pCurFreeBlock = (void *)pstFreeBlock;

    return VosHimallocSliceBinAllocDirect(pstFreeBlock);
}

/* 此接口无须确保puvStart一定小于等于puvEnd */
static void VosHimallocSliceRegSpaceGet(VOS_HIMALLOC_BLOCK *pstBlock, size_t *puvStart, size_t *puvEnd)
{
    size_t uvStartAddr;
    size_t uvEndAddr;

    /* 获取Block注册的启始地址与终止地址 */
    uvStartAddr = VosHimallocBlockStartRegGet(ptr_to_ulong(pstBlock->pStart));

    uvEndAddr = VosHimallocBlockEndRegGet(ptr_to_ulong(pstBlock->pStart) + pstBlock->uvLength);

    uvEndAddr = VOS_HIMALLOC_LG_ALIGN(uvEndAddr, VOS_HIMALLOC_LG_VPAGE_SIZE);

    /*
     * 计算映射的首部与尾部
     * Block分配时已经映射了首部与尾部，因此这两个VPAGE可以不映射
     */
    if (uvStartAddr < HIMALLOC_NULL_SIZE_T - VOS_HIMALLOC_VPAGE_SIZE) {
        uvStartAddr = uvStartAddr + VOS_HIMALLOC_VPAGE_SIZE;
    }

    if (uvEndAddr > VOS_HIMALLOC_VPAGE_SIZE) {
        uvEndAddr = uvEndAddr - VOS_HIMALLOC_VPAGE_SIZE;
    }

    *puvStart = (size_t)uvStartAddr;
    *puvEnd = (size_t)uvEndAddr;

    return;
}

/* 如果获取不到thread local的tlb结构则直接注册进入radix tree中 */
static void VosHimallocSliceSlowRegister(VOS_HIMALLOC_BLOCK *pstBlock, void *pObject)
{
    size_t uvStart;
    size_t uvEnd;

    VosHimallocSliceRegSpaceGet(pstBlock, &uvStart, &uvEnd);
    for (; uvStart < uvEnd; uvStart = uvStart + VOS_HIMALLOC_VPAGE_SIZE) {
        (void)VosHimallocTlbRtreeSet(uvStart, pObject);
    }
}

/* 传入pObject为空表示Deregister */
static void VosHimallocSliceFastRegister(
    VOS_HIMALLOC_TLB_INFO_S *pstTlbInfo, VOS_HIMALLOC_BLOCK *pstBlock, void *pObject)
{
    size_t uvStart;
    size_t uvEnd;

    VosHimallocSliceRegSpaceGet(pstBlock, &uvStart, &uvEnd);
    /* 地址对齐由radix tree处理,Block不可能跨两个以上rtree node节点，因此下述操作不会失败 */
    for (; uvStart < uvEnd; uvStart = uvStart + VOS_HIMALLOC_VPAGE_SIZE) {
        (void)VosHimallocTlbSet(pstTlbInfo, uvStart, pObject);
    }

    return;
}
static void VosHimallocSliceBlockInit(VOS_HIMALLOC_ARENA_S *pstArena,
    VOS_HIMALLOC_BLOCK *pstBlock, uint16_t usBinIndex)
{
    VOS_HIMALLOC_SLICE_INFO *pstSliceInfo;
    VOS_HIMALLOC_TLB_INFO_S *pstTlbInfo;

    VOS_HIMALLOC_ASSERT(usBinIndex < VOS_HIMALLOC_MAX_SMALL_INDEX);

    /* 更新Block的slice信息 */
    pstSliceInfo = &(g_astHimallocSliceInfo[usBinIndex]);
    VosBitmapInit((pstBlock)->unMetaDataNode.stHybrid.auiFreeBitmap, (uint32_t)VOS_HIMALLOC_MAX_BITMAP_GROUPS,
        &(pstSliceInfo->stBitmapInfo));
    pstBlock->ucSizeClass = (uint8_t)usBinIndex;
    pstBlock->ucFreeSlice = (uint8_t)pstSliceInfo->usSliceNum;
    pstBlock->ucSliced = 1;

    pstTlbInfo = VosHimallocTcacheTlbGet(pstArena);
    if (HIMALLOC_UNLIKELY(pstTlbInfo == HIMALLOC_NULL_PTR)) {
        VosHimallocSliceSlowRegister(pstBlock, pstBlock);
    } else {
        VosHimallocSliceFastRegister(pstTlbInfo, pstBlock, pstBlock);
    }

    return;
}

static void *VosHimallocSliceBlockAlloc(VOS_HIMALLOC_ARENA_S *pstArena, VOS_HIMALLOC_SLICE_BIN_S *pstBinCtl,
    uint16_t usBinIndex)
{
    VOS_HIMALLOC_BLOCK *pstFreeBlock;
    VOS_HIMALLOC_BLOCK *pstOldCurBlock;
    VOS_HIMALLOC_SLICE_INFO *pstSliceInfo;

    VosHimallocUnLock(pstBinCtl->pstLock);

    pstSliceInfo = &(g_astHimallocSliceInfo[usBinIndex]);
    pstFreeBlock = VosHimallocBlockAlloc(pstArena, pstSliceInfo->uiBlockSize);
    if (pstFreeBlock == HIMALLOC_NULL_PTR) {
        VosHimallocLogWrite(HIMALLOC_ERRNO_NOBLOCK,
            "[HIMALLOC-%s]block alloc failed, arena id(%u), block size(%u)", __FUNCTION__, pstArena->ucArenaId,
            pstSliceInfo->uiBlockSize);
        (void)VosHimallocLock(pstBinCtl->pstLock);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }

    VosHimallocSliceBlockInit(pstArena, pstFreeBlock, usBinIndex);

    (void)VosHimallocLock(pstBinCtl->pstLock);

    if (VosHimallocStatsEnable() == true) {
        pstBinCtl->stBinStat.uvNBinBlocks++;
    }

    pstOldCurBlock = (VOS_HIMALLOC_BLOCK *)(pstBinCtl->pCurFreeBlock);
    if (pstOldCurBlock == HIMALLOC_NULL_PTR) {
        /* 防止解锁后其他进程已初始化pstCurFreeBlock */
        pstBinCtl->pCurFreeBlock = (void *)pstFreeBlock;
    } else if (pstOldCurBlock->ucFreeSlice == 0) {
        VosHimallocHybridAdd(&((pstOldCurBlock)->unMetaDataNode.stHybrid), &(pstBinCtl->stNotFreeBlockHead));
        pstBinCtl->pCurFreeBlock = (void *)pstFreeBlock;
    } else {
        VosHimallocHybridAdd(&((pstFreeBlock)->unMetaDataNode.stHybrid), &(pstBinCtl->stFreeBlockHead));
    }

    return VosHimallocSliceBinAllocDirect(pstFreeBlock);
}

static void *VosHimallocSliceBinAllocHard(VOS_HIMALLOC_ARENA_S *pstArena, VOS_HIMALLOC_SLICE_BIN_S *pstBinCtl,
    uint16_t usBinIndex)
{
    VOS_HIMALLOC_BLOCK *pstCurFreeBlock;
    void *pAllocAddr;

    pstCurFreeBlock = (VOS_HIMALLOC_BLOCK *)(pstBinCtl->pCurFreeBlock);
    if (pstCurFreeBlock != HIMALLOC_NULL_PTR) {
        pstBinCtl->pCurFreeBlock = HIMALLOC_NULL_PTR;
        VosHimallocHybridAdd(&((pstCurFreeBlock)->unMetaDataNode.stHybrid), &(pstBinCtl->stNotFreeBlockHead));
    }

    if (!CSTL_ListIsEmpty(&(pstBinCtl->stFreeBlockHead))) {
        pAllocAddr = VosHimallocSliceAllocFromList(pstBinCtl);
    } else {
        pAllocAddr = VosHimallocSliceBlockAlloc(pstArena, pstBinCtl, usBinIndex);
    }

    return pAllocAddr;
}

static HIMALLOC_ALWAYS_INLINE VOS_HIMALLOC_SLICE_BIN_S *VosHimallocSliceBinGet(VOS_HIMALLOC_ARENA_S *pstArena,
    uint16_t usBinIndex)
{
    VOS_HIMALLOC_ASSERT(usBinIndex < VOS_HIMALLOC_MAX_SMALL_INDEX);
    return &(((pstArena->stSliceHeap).astSliceBins)[usBinIndex]);
}

HIDDEN void *VosHimallocSliceBinAlloc(VOS_HIMALLOC_ARENA_S *pstArena, uint16_t usBinIndex)
{
    void *pAllocedAddr;
    VOS_HIMALLOC_BLOCK *pstCurFreeBlock;
    VOS_HIMALLOC_SLICE_BIN_S *pstBinCtl;

    pstBinCtl = VosHimallocSliceBinGet(pstArena, usBinIndex);

    VOS_HIMALLOC_ASSERT(pstBinCtl->pstLock != HIMALLOC_NULL_PTR);
    (void)VosHimallocLock(pstBinCtl->pstLock);

    pstCurFreeBlock = (VOS_HIMALLOC_BLOCK *)(pstBinCtl->pCurFreeBlock);
    if (HIMALLOC_LIKELY(pstCurFreeBlock != HIMALLOC_NULL_PTR) && HIMALLOC_LIKELY(pstCurFreeBlock->ucFreeSlice != 0U)) {
        pAllocedAddr = VosHimallocSliceBinAllocDirect(pstCurFreeBlock);
    } else {
        pAllocedAddr = VosHimallocSliceBinAllocHard(pstArena, pstBinCtl, usBinIndex);
    }

    if (VosHimallocStatsEnable() == true) {
        if (pAllocedAddr != HIMALLOC_NULL_PTR) {
            pstBinCtl->stBinStat.ullTotalNMalloc++;
        }
    }

    VosHimallocUnLock(pstBinCtl->pstLock);

    return pAllocedAddr;
}

HIDDEN uint32_t VosHimallocSliceBinFill(VOS_HIMALLOC_ARENA_S *pstArena, void *pstTcacheSliceClass, uint32_t uiNumToFill,
    uint16_t usBinIndex)
{
    uint32_t uiLockRet;
    uint32_t uiFilledNum;
    void *pAllocedAddr;
    VOS_HIMALLOC_BLOCK *pstCurFreeBlock;
    VOS_HIMALLOC_SLICE_BIN_S *pstBinCtl;
    VOS_HIMALLOC_TCACHE_SLICE *pstTcacheSlice = (VOS_HIMALLOC_TCACHE_SLICE *)pstTcacheSliceClass;

    pstBinCtl = VosHimallocSliceBinGet(pstArena, usBinIndex);

    VOS_HIMALLOC_ASSERT(pstBinCtl->pstLock != HIMALLOC_NULL_PTR);
    uiLockRet = OSAL_LockLock(&(pstBinCtl->pstLock->stLock));
    for (uiFilledNum = 0; uiFilledNum < uiNumToFill; ++uiFilledNum) {
        pstCurFreeBlock = (VOS_HIMALLOC_BLOCK *)(pstBinCtl->pCurFreeBlock);
        if (HIMALLOC_LIKELY((pstCurFreeBlock != HIMALLOC_NULL_PTR) && (pstCurFreeBlock->ucFreeSlice != 0U))) {
            pAllocedAddr = VosHimallocSliceBinAllocDirect(pstCurFreeBlock);
        } else {
            pAllocedAddr = VosHimallocSliceBinAllocHard(pstArena, pstBinCtl, usBinIndex);
        }
        if (pAllocedAddr == HIMALLOC_NULL_PTR) {
            break;
        }

        VosHimallocSliceTcacheSet(pstTcacheSlice, pAllocedAddr);
    }

    if (VosHimallocStatsEnable() == true) {
        pstBinCtl->stBinStat.ullTotalNMalloc += uiFilledNum;
    }

    if (HIMALLOC_UNLIKELY(VosHimallocCleanTickTok(&pstArena->siTick, 1))) {
        OSAL_LockUnLock(&(pstBinCtl->pstLock->stLock), uiLockRet);
        VosHimallocBlockHeapCleanSetUp(pstArena, true);
    } else {
        OSAL_LockUnLock(&(pstBinCtl->pstLock->stLock), uiLockRet);
    }

    return uiFilledNum;
}

static HIMALLOC_ALWAYS_INLINE uint32_t VosHimallocSliceInsert(VOS_HIMALLOC_BLOCK *pstBlock,
    VOS_HIMALLOC_SLICE_INFO *pstSliceInfo, const void *pAddr)
{
    bool bSucceed;
    uint16_t usSliceIndex;
    uint32_t uiRet = HIMALLOC_OK;

    usSliceIndex = VosHimallocSliceIndexGet(pstBlock, &(pstSliceInfo->stDivInfo), pAddr);
    if (HIMALLOC_UNLIKELY(g_himalloc_enable_mcheck)) {
        if (VosHimallocSliceAddrOffset(pstBlock, usSliceIndex) != pAddr) {
            abort(); /* 该地址与slice分割出来的地址不同，认为是非法地址，在MALLOC_CHECK_打开的情况下abort */
        }
    }

    bSucceed = VosBitmapCheckSet(
        (pstBlock)->unMetaDataNode.stHybrid.auiFreeBitmap, (uint32_t)VOS_HIMALLOC_MAX_BITMAP_GROUPS, usSliceIndex);
    if (bSucceed != true) {
        if (g_himalloc_enable_mcheck) {
            abort(); /* 引用计数为0，认为是double free，在MALLOC_CHECK_打开的情况下abort */
        }

        uiRet = (uint32_t)VOS_HIMALLOC_FREE_REF_ZERO_E; // 引用计数为0，double free
    } else {
        pstBlock->ucFreeSlice++;
    }

    return uiRet;
}

static HIMALLOC_ALWAYS_INLINE void VosHimallocSliceBlockFinit(VOS_HIMALLOC_ARENA_S *pstArena,
    VOS_HIMALLOC_BLOCK *pstBlock)
{
    VOS_HIMALLOC_TLB_INFO_S *pstTlbInfo;

    pstBlock->ucFreeSlice = 0;
    pstBlock->ucSliced = 0;
    pstBlock->ucSizeClass = VOS_HIMALLOC_MAX_SMALL_INDEX;

    pstTlbInfo = &(pstArena->stTlb);
    if (HIMALLOC_UNLIKELY(pstTlbInfo == HIMALLOC_NULL_PTR)) {
        VosHimallocSliceSlowRegister(pstBlock, HIMALLOC_NULL_PTR);
    } else {
        VosHimallocSliceFastRegister(pstTlbInfo, pstBlock, HIMALLOC_NULL_PTR);
    }

    return;
}

static void VosHimallocSliceBlockFree(VOS_HIMALLOC_ARENA_S *pstArena, VOS_HIMALLOC_SLICE_BIN_S *pstBinCtl,
    VOS_HIMALLOC_BLOCK *pstBlock)
{
    if (pstBlock == pstBinCtl->pCurFreeBlock) {
        pstBinCtl->pCurFreeBlock = HIMALLOC_NULL_PTR;
    } else {
        VosHimallocHybridRemove(&((pstBlock)->unMetaDataNode.stHybrid));
    }

    VosHimallocUnLock(pstBinCtl->pstLock);

    VosHimallocLock((&(&(pstArena->stBlockHeap))->stTreeBinLock));

    VosHimallocSliceBlockFinit(pstArena, pstBlock);

    (void)VosHimallocBlockFree(pstArena, pstBlock);

    VosHimallocUnLock((&(&(pstArena->stBlockHeap))->stTreeBinLock));

    (void)VosHimallocLock(pstBinCtl->pstLock);

    if (VosHimallocStatsEnable() == true) {
        pstBinCtl->stBinStat.uvNBinBlocks--;
    }

    return;
}

static HIMALLOC_ALWAYS_INLINE uint32_t VosHimallocSliceFreeImpl(VOS_HIMALLOC_ARENA_S *pstArena,
    VOS_HIMALLOC_SLICE_BIN_S *pstBinCtl, VOS_HIMALLOC_BLOCK *pstBlock, const void *pAddr)
{
    uint32_t uiRet;
    VOS_HIMALLOC_SLICE_INFO *pstSliceInfo;

    pstSliceInfo = VosHimallocSliceInfoGet(pstBlock->ucSizeClass);
    VOS_HIMALLOC_ASSERT(pstBlock->ucFreeSlice < pstSliceInfo->usSliceNum);

    uiRet = VosHimallocSliceInsert(pstBlock, pstSliceInfo, pAddr);
    if (uiRet != HIMALLOC_OK) {
        __hmattr_sideff_resolved("no relative side effect");
        return uiRet;
    }

    if (VosHimallocStatsEnable() == true) {
        pstBinCtl->stBinStat.ullTotalNDalloc++;
    }

    if ((pstBlock->ucFreeSlice == 1) && ((void *)pstBlock != pstBinCtl->pCurFreeBlock)) {
        VosHimallocHybridRemove(&((pstBlock)->unMetaDataNode.stHybrid));
        VosHimallocHybridAddBefore(&((pstBlock)->unMetaDataNode.stHybrid), &(pstBinCtl->stFreeBlockHead));
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_OK;
    }

    if (pstBlock->ucFreeSlice == pstSliceInfo->usSliceNum) {
        VosHimallocSliceBlockFree(pstArena, pstBinCtl, pstBlock);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_OK;
    }

    return HIMALLOC_OK;
}

HIDDEN uint32_t VosHimallocSliceFree(VOS_HIMALLOC_ARENA_S *pstArena, VOS_HIMALLOC_BLOCK *pstBlock, const void *pAddr)
{
    uint32_t uiRet;
    VOS_HIMALLOC_SLICE_BIN_S *pstBinCtl;

    pstBinCtl = VosHimallocSliceBinGet(pstArena, pstBlock->ucSizeClass);

    (void)VosHimallocLock(pstBinCtl->pstLock);
    uiRet = VosHimallocSliceFreeImpl(pstArena, pstBinCtl, pstBlock, pAddr);
    VosHimallocUnLock(pstBinCtl->pstLock);

    return uiRet;
}

HIDDEN uint32_t VosHimallocSliceBinFlush(VOS_HIMALLOC_ARENA_S *pstArena, void *pstTcacheSliceClass,
    VOS_HIMALLOC_TLB_INFO_S *pstThreadTlb, uint32_t uiTotalFlush, uint8_t ucSizeClass)
{
    uint32_t uiLock;
    uint32_t uiFlushNum;
    void *freeTcacheSlice;
    uint32_t uiRet = HIMALLOC_OK;
    VOS_HIMALLOC_BLOCK *pstBlock;
    VOS_HIMALLOC_SLICE_BIN_S *pstBinCtl;
    VOS_HIMALLOC_TCACHE_SLICE *pstTcacheSlice = (VOS_HIMALLOC_TCACHE_SLICE *)pstTcacheSliceClass;

    VOS_HIMALLOC_ASSERT(pstTcacheSlice->usNumCached >= uiTotalFlush);

    pstBinCtl = VosHimallocSliceBinGet(pstArena, ucSizeClass);
    uiLock = OSAL_LockLock(&(pstBinCtl->pstLock->stLock));

    for (uiFlushNum = 0; uiFlushNum < uiTotalFlush; ++uiFlushNum) {
        freeTcacheSlice = VosHimallocSliceTcacheGet(pstTcacheSlice);
        pstBlock = VosHimallocBlockGetImp(pstThreadTlb, freeTcacheSlice);
        if (pstBlock == HIMALLOC_NULL_PTR) {
            VosHimallocSliceTcacheSet(pstTcacheSlice, freeTcacheSlice); // 释放异常，需要再放回去，防止泄露
            uiRet = (uint32_t)VOS_HIMALLOC_FREE_FAILED_E;
            break;
        }

        uiRet = VosHimallocSliceFreeImpl(pstArena, pstBinCtl, pstBlock, freeTcacheSlice);
        if (uiRet != HIMALLOC_OK) {
            VosHimallocSliceTcacheSet(pstTcacheSlice, freeTcacheSlice); // 释放异常，需要再放回去，防止泄露
            break;
        }
    }

    if (HIMALLOC_UNLIKELY(VosHimallocCleanTickTok(&pstArena->siTick, 1))) {
        OSAL_LockUnLock(&(pstBinCtl->pstLock->stLock), uiLock);
        VosHimallocBlockHeapCleanSetUp(pstArena, true);
    } else {
        OSAL_LockUnLock(&(pstBinCtl->pstLock->stLock), uiLock);
    }

    return uiRet;
}

/* pstStats的初始化由调用者保证 */
static void VosHimallocSliceListStat(CSTL_LIST_HEAD_S *pstListHead, const VOS_HIMALLOC_SLICE_INFO *pstSliceInfo,
    VosHimallocSliceRtimeStatS *pstStats)
{
    CSTL_LIST_HEAD_S *pstNode = HIMALLOC_NULL_PTR;
    VOS_HIMALLOC_BLOCK *pstSliceBlock = HIMALLOC_NULL_PTR;

    CSTL_LIST_FOR_EACH_ITEM(pstNode, pstListHead)
    {
        pstSliceBlock = VOS_HIMALLOC_HYBRID_LIST_ENTRY(pstNode, VOS_HIMALLOC_BLOCK, stHybrid);

        VOS_HIMALLOC_ASSERT(pstSliceBlock->ucSliced == true);
        VOS_HIMALLOC_ASSERT(pstSliceBlock->ucFreeSlice <= pstSliceInfo->usSliceNum);
        pstStats->uvTotalNum += pstSliceInfo->usSliceNum;
        pstStats->uvUsedNum += (size_t)pstSliceInfo->usSliceNum - (size_t)pstSliceBlock->ucFreeSlice;
        pstStats->uvNBinBlocks++;
    }

    return;
}

HIDDEN uint32_t VosHimallocSliceRtimeStat(uint8_t ucArenaId, uint32_t uiType, VosHimallocSliceRtimeStatS *pstStats)
{
    VOS_HIMALLOC_ARENA_S *pstArena;
    VOS_HIMALLOC_SLICE_BIN_S *pstBinCtl;
    VOS_HIMALLOC_SLICE_INFO *pstSliceInfo;
    uint16_t usSliceNum;
    uint16_t usFreeNum;
    uint16_t usCachedNum;

    if ((pstStats == HIMALLOC_NULL_PTR) || (uiType >= VOS_HIMALLOC_MAX_SMALL_INDEX)) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_INVAL;
    }

    pstArena = VosHimallocArenaGet(ucArenaId);
    if (pstArena == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_INVAL;
    }

    (void)memset(pstStats, 0, sizeof(VosHimallocSliceRtimeStatS));

    pstSliceInfo = VosHimallocSliceInfoGet(uiType);
    pstBinCtl = VosHimallocSliceBinGet(pstArena, (uint16_t)uiType);

    (void)VosHimallocLock(pstBinCtl->pstLock);

    if (pstBinCtl->pCurFreeBlock != HIMALLOC_NULL_PTR) {
        usSliceNum = pstSliceInfo->usSliceNum;
        usFreeNum = ((VOS_HIMALLOC_BLOCK *)pstBinCtl->pCurFreeBlock)->ucFreeSlice;
        pstStats->uvTotalNum += usSliceNum;
        pstStats->uvUsedNum += (size_t)usSliceNum - (size_t)usFreeNum;
        pstStats->uvNBinBlocks++;
    }

    VosHimallocSliceListStat(&(pstBinCtl->stFreeBlockHead), pstSliceInfo, pstStats);

    VosHimallocSliceListStat(&(pstBinCtl->stNotFreeBlockHead), pstSliceInfo, pstStats);

    if (VosHimallocTcacheIsEnable(ucArenaId)) {
        usCachedNum = VosHimallocTcacheLeftCachedNumGet(pstArena, uiType);
        pstStats->uvUsedNum -= usCachedNum;
    }

    VosHimallocUnLock(pstBinCtl->pstLock);

    return HIMALLOC_OK;
}

HIDDEN void VosHimallocSliceSpecGet(VosHimallocSliceSpecS *pstSliceSpec)
{
    pstSliceSpec->uiMaxSliceClass = VOS_HIMALLOC_MAX_SMALL_INDEX;
    pstSliceSpec->uiMaxSliceSize = (uint32_t)VOS_HIMALLOC_MAX_SMALL_SIZE;

    pstSliceSpec->uiSpecSliceClass = VosHimallocSize2Index(VOS_HIMALLOC_SLICE_SIZE_MAX);
    pstSliceSpec->uiSpecSliceSize = VOS_HIMALLOC_SLICE_SIZE_MAX;
}

HIDDEN uint32_t VosHimallocSliceVisitInfoGet(uint8_t ucArenaId, uint8_t ucClass, uint64_t *pullAllocTimes,
    uint64_t *pullFreeTimes)
{
    VOS_HIMALLOC_ARENA_S *pstArena = HIMALLOC_NULL_PTR;
    VOS_HIMALLOC_SLICE_BIN_STAT_S *pstSliceStat = HIMALLOC_NULL_PTR;

    pstArena = VosHimallocArenaGet(ucArenaId);
    if (pstArena == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_INVAL;
    }

    pstSliceStat = &(pstArena->stSliceHeap.astSliceBins[ucClass].stBinStat);
    *pullAllocTimes = pstSliceStat->ullTotalNMalloc;
    *pullFreeTimes = pstSliceStat->ullTotalNDalloc;
    return HIMALLOC_OK;
}

HIDDEN uint32_t VosHimallocSliceMaxSizeSet(uint8_t ucArenaId, uint32_t *puiMaxSliceSize)
{
    size_t uvIndex;
    VOS_HIMALLOC_ARENA_S *pstArena;

    pstArena = VosHimallocArenaGet(ucArenaId);
    if (pstArena == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_INVAL;
    }

    uvIndex = VosHimallocSize2Index(*puiMaxSliceSize);
    *puiMaxSliceSize = (uint32_t)g_auvHimallocIndex2size[uvIndex];
    pstArena->usMaxSliceSize = (uint16_t)*puiMaxSliceSize;

    return HIMALLOC_OK;
}

HIDDEN int32_t VosHimallocUsedSliceTraverse(VOS_HIMALLOC_BLOCK *pstBlock, const HimallocMemParseFunc pfnParseHook,
    void *pResult)
{
    bool isBitSet = false;
    bool isCached = false;
    uintptr_t uvStartTmp = ptr_to_ulong(pstBlock->pStart);
    uintptr_t uvEnd = ptr_to_ulong(pstBlock->pStart) + pstBlock->uvLength;
    size_t uvSliceSize = VosHimallocIndex2Size(pstBlock->ucSizeClass);
    uint32_t uiSliceIndex = 0;
    int32_t uiRet = HIMALLOC_OK;

    while (uvStartTmp + uvSliceSize <= uvEnd) {
        isBitSet = VOS_BITMAP_IS_BIT_SET((pstBlock)->unMetaDataNode.stHybrid.auiFreeBitmap,
            VOS_HIMALLOC_MAX_BITMAP_GROUPS, uiSliceIndex);
        isCached =
            VosHimallocTcacheAddrIsCached(ulong_to_ptr(uvStartTmp, void), pstBlock->ucSizeClass, pstBlock->ucArenaId);
        if (isBitSet || isCached) {
            uvStartTmp += uvSliceSize;
            uiSliceIndex++;
            continue;
        }
        uiRet = pfnParseHook(ulong_to_ptr(uvStartTmp, void), uvSliceSize, pResult);
        if (uiRet != HIMALLOC_OK) {
            break;
        }
        uvStartTmp += uvSliceSize;
        uiSliceIndex++;
    }
    return uiRet;
}

HIDDEN int32_t VosHimallocUsedSliceBitmapTraverse(VOS_HIMALLOC_BLOCK *pstBlock, const HimallocMemParseFunc pfnParseHook,
    void *pResult)
{
    uintptr_t uvStartTmp = ptr_to_ulong(pstBlock->pStart);
    uintptr_t uvEnd = ptr_to_ulong(pstBlock->pStart) + pstBlock->uvLength;
    size_t uvSliceSize = VosHimallocIndex2Size(pstBlock->ucSizeClass);
    uint32_t uiSliceIndex = 0;
    int32_t uiRet = HIMALLOC_OK;

    while (uvStartTmp + uvSliceSize <= uvEnd) {
        if (VOS_BITMAP_IS_BIT_SET((pstBlock)->unMetaDataNode.stHybrid.auiFreeBitmap,
            VOS_HIMALLOC_MAX_BITMAP_GROUPS, uiSliceIndex)) {
            uvStartTmp += uvSliceSize;
            uiSliceIndex++;
            continue;
        }
        uiRet = pfnParseHook(ulong_to_ptr(uvStartTmp, void), uvSliceSize, pResult);
        if (uiRet != HIMALLOC_OK) {
            break;
        }
        uvStartTmp += uvSliceSize;
        uiSliceIndex++;
    }
    return uiRet;
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */
