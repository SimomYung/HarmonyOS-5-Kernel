/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#include <assert.h>
#include "vos_bitmapgrp.h"
#include "osal_lock.h"
#include "vos_metaalloc.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

HIDDEN VOS_META_MEM_S g_stMetaMem;
HIDDEN uint32_t g_uiMetaAppendSize;
HIDDEN uint32_t g_uiMetaUnitNum;

static HIMALLOC_ALWAYS_INLINE uint32_t VosMetaBlockSizeToIdx(size_t uvSize)
{
    size_t uvNewSize = VOS_META_HALIGN(uvSize, VOS_META_BLOCK_MIN);
    uint32_t uiIdx;

    uiIdx = (uint32_t)(uvNewSize >> VOS_META_BLOCK_MIN_LG) - 1U;
    return (uiIdx < VOS_META_BLOCK_TYPE) ? uiIdx : (VOS_META_BLOCK_TYPE - 1U);
}

static HIMALLOC_ALWAYS_INLINE uint32_t VosMetaSliceSizeToIdx(size_t uvSize)
{
    uint32_t uiBitHigh = VosBitmapFLSImpl((uint32_t)uvSize);

    if ((uvSize & (size_t)((1UL << uiBitHigh) - 1U)) != 0UL) {
        uiBitHigh++;
    }

    return (uiBitHigh > VOS_META_SLICE_MIN_LG) ? (uiBitHigh - VOS_META_SLICE_MIN_LG) : 0U;
}

static HIMALLOC_ALWAYS_INLINE uint32_t VosMetaSliceIdxToSize(uint32_t uiIdx)
{
    return ((uint32_t)1 << (uiIdx + (uint32_t)VOS_META_SLICE_MIN_LG));
}

static HIMALLOC_ALWAYS_INLINE void VosMetaBitSet(uint32_t *puiBitmap, uint32_t uiBitGrpNum, uint16_t usIndex)
{
    uint16_t usGroupId = usIndex >> VOS_BITMAP_LG_BITS;

    if (HIMALLOC_UNLIKELY(usGroupId >= uiBitGrpNum)) {
        __hmattr_sideff_resolved("no relative side effect");
        return;
    }

    puiBitmap[usGroupId] |= (uint32_t)(1UL << (usIndex & VOS_BITMAP_NBITS_MASK));
    return;
}

static HIMALLOC_ALWAYS_INLINE void VosMetaBitClear(uint32_t *puiBitmap, uint32_t uiBitGrpNum, uint16_t usIndex)
{
    uint16_t usGroupId = usIndex >> VOS_BITMAP_LG_BITS;

    if (HIMALLOC_UNLIKELY(usGroupId >= uiBitGrpNum)) {
        __hmattr_sideff_resolved("no relative side effect");
        return;
    }

    puiBitmap[usGroupId] &= ~((uint32_t)1 << (usIndex & (uint32_t)VOS_BITMAP_NBITS_MASK));
    return;
}

static HIMALLOC_ALWAYS_INLINE VOS_META_PIECE_S *VosMetaPieceHeadGet(const void *pAddr)
{
    return (VOS_META_PIECE_S *)VOS_META_LALIGN(pAddr, g_uiMetaAppendSize);
}

static HIMALLOC_ALWAYS_INLINE void *VosMetaBlockStartInPieceGet(void *pPieceStart)
{
    uint32_t uiUnitNum = g_uiMetaUnitNum;
    uint32_t uiHeadLen = (uint32_t)VOS_META_HALIGN(sizeof(VOS_META_PIECE_S) + uiUnitNum * sizeof(VOS_META_NODE_S),
        (uint32_t)VOS_META_BLOCK_MIN);

    return ulong_to_ptr((ptr_to_ulong(pPieceStart) + uiHeadLen), void);
}

static HIMALLOC_ALWAYS_INLINE VOS_META_NODE_S *VosMetaUnitNodeGet(const void *pAddr)
{
    VOS_META_PIECE_S *pstPiece = VosMetaPieceHeadGet(pAddr);
    void *pFirstBlock = VosMetaBlockStartInPieceGet(pstPiece);
    VOS_META_NODE_S *pstMetaUnit = pstPiece->astMetaUnit;
    uint32_t uiUnitIdx;

    if (ptr_to_ulong(pAddr) < ptr_to_ulong(pFirstBlock)) {
        __hmattr_sideff_resolved("no relative side effect");
        return NULL;
    }

    uiUnitIdx = (uint32_t)((ptr_to_ulong(pAddr) - ptr_to_ulong(pFirstBlock)) >> 10);
    if (uiUnitIdx >= g_uiMetaUnitNum) {
        __hmattr_sideff_resolved("no relative side effect");
        return NULL;
    }

    return &(pstMetaUnit[uiUnitIdx]);
}

static HIMALLOC_ALWAYS_INLINE void VosMetaUnitSetFree(const void *pAddr, size_t uvSize)
{
    VOS_META_PIECE_S *pstPiece = VosMetaPieceHeadGet(pAddr);
    VOS_META_NODE_S *pstHeadNode = VosMetaUnitNodeGet(pAddr);
    VOS_META_NODE_S *pstTailNode = VosMetaUnitNodeGet(ulong_to_ptr((ptr_to_ulong(pAddr) + uvSize - VOS_META_BLOCK_MIN),
        void));
    uint32_t uiBlockIdx = VosMetaBlockSizeToIdx(uvSize);

    if (pstHeadNode == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return;
    }
    pstHeadNode->uiAlloced = 0U;
    pstHeadNode->uiSliced = 0U;
    pstHeadNode->unVosMetaMemLen.uiBlockUnitNum = uiBlockIdx + 1U;

    if (pstTailNode == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return;
    }
    pstTailNode->uiAlloced = 0U;
    pstTailNode->uiSliced = 0U;
    pstTailNode->unVosMetaMemLen.uiBlockUnitNum = uiBlockIdx + 1U;

    pstPiece->uiUsedSize -= (uint32_t)uvSize;
    return;
}

static HIMALLOC_ALWAYS_INLINE void VosMetaUnitSetAlloced(const void *pAddr, size_t uvSize)
{
    VOS_META_NODE_S *pstHeadNode = VosMetaUnitNodeGet(pAddr);
    VOS_META_NODE_S *pstTailNode =
        VosMetaUnitNodeGet(ulong_to_ptr((ptr_to_ulong(pAddr) + uvSize - VOS_META_BLOCK_MIN), void));
    uint32_t uiBlockIdx = VosMetaBlockSizeToIdx(uvSize);

    if (pstHeadNode == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return;
    }
    pstHeadNode->uiAlloced = 1U;
    pstHeadNode->uiSliced = 0U;
    pstHeadNode->unVosMetaMemLen.uiBlockUnitNum = uiBlockIdx + 1U;

    if (pstTailNode == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return;
    }
    pstTailNode->uiAlloced = 1U;
    pstTailNode->uiSliced = 0U;
    pstTailNode->unVosMetaMemLen.uiBlockUnitNum = uiBlockIdx + 1U;
}

static HIMALLOC_ALWAYS_INLINE void VosMetaUnitSetSliced(const void *pAddr, uint32_t uiSliceType)
{
    VOS_META_NODE_S *pstHeadNode = VosMetaUnitNodeGet(pAddr);
    if (pstHeadNode == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return;
    }

    pstHeadNode->uiAlloced = 1U;
    pstHeadNode->uiSliced = 1U;
    pstHeadNode->unVosMetaMemLen.uiSliceType = uiSliceType;
}

HIDDEN uint32_t VosMetaInit(void)
{
    VOS_META_SLICE_S *pstSliceInfo;
    uint32_t uiRet;
    uint32_t i;
    uint32_t uiMaxUnitNum;

    (void)memset(&g_stMetaMem, 0, sizeof(VOS_META_MEM_S));
    pstSliceInfo = g_stMetaMem.astSliceCb;

    for (i = 0U; i < VOS_META_SLICE_TYPE; i++) {
        pstSliceInfo->usBlockLen = (uint16_t)VOS_META_BLOCK_MIN;
        pstSliceInfo->usUnitLen = (uint16_t)VosMetaSliceIdxToSize(i);
        pstSliceInfo->usNumPerBlock =
            (uint16_t)((VOS_META_BLOCK_MIN - sizeof(VOS_META_SLICE_CB)) / pstSliceInfo->usUnitLen);
        pstSliceInfo->uvBlockRmask = (size_t)(~(VOS_META_BLOCK_MIN - 1U));
        CSTL_ListInit(&pstSliceInfo->stEmptyBlock);
        CSTL_ListInit(&pstSliceInfo->stBusyBlock);
        pstSliceInfo++;
    }

    uiRet = OSAL_LockCreate(&(g_stMetaMem.stLock));
    if (uiRet != HIMALLOC_OK) {
        __hmattr_sideff_resolved("no relative side effect");
        return uiRet;
    }

    g_uiMetaAppendSize = (VOS_META_PAGE_SIZE > VOS_META_APPEND_MAX_SIZE) ?
        (uint32_t)VOS_META_PAGE_SIZE : (uint32_t)VOS_META_APPEND_MAX_SIZE;
    /* head最少占用1k且要求1k对齐, 4字节的usedsize可以复用记录0号单元的管理结构, 10表示B到KB的转换 */
    uiMaxUnitNum = (uint32_t)((VOS_META_HEAD_LEN - sizeof(VOS_META_PIECE_S)) / sizeof(VOS_META_NODE_S));
    g_uiMetaUnitNum = (uint32_t)(g_uiMetaAppendSize - VOS_META_HEAD_LEN) >> 10; /* 10: B转换成KB */
    g_uiMetaUnitNum = (g_uiMetaUnitNum < uiMaxUnitNum) ? g_uiMetaUnitNum : uiMaxUnitNum;

    return HIMALLOC_OK;
}

static HIMALLOC_ALWAYS_INLINE void VosMetaBlockNodeAdd(void *pAddr, size_t uvSize)
{
    VOS_META_BLOCK_NODE *pstNode = (VOS_META_BLOCK_NODE *)pAddr;
    uint32_t uiIdx = VosMetaBlockSizeToIdx(uvSize);
    VOS_META_BLOCK_NODE *pstBlockHash = g_stMetaMem.apstBlock[uiIdx];

    pstNode->uvLen = uvSize;
    CSTL_ListInit(&(pstNode->stLinkNode));

    if (g_stMetaMem.apstBlock[uiIdx] == HIMALLOC_NULL_PTR) {
        g_stMetaMem.apstBlock[uiIdx] = pstNode;
        VosMetaBitSet(g_stMetaMem.auiFreeMap, VOS_META_BLOCK_BIT_GROUPS, (uint16_t)uiIdx);
    } else {
        CSTL_ListAdd(&pstNode->stLinkNode, &(pstBlockHash->stLinkNode));
    }

    VosMetaUnitSetFree(pAddr, uvSize);
}

static HIMALLOC_ALWAYS_INLINE void VosMetaBlockNodeRemove(VOS_META_BLOCK_NODE *pstNode)
{
    uint32_t uiIdx = VosMetaBlockSizeToIdx(pstNode->uvLen);
    VOS_META_PIECE_S *pstPiece = VosMetaPieceHeadGet((const void *)pstNode);

    if (CSTL_ListIsEmpty(&pstNode->stLinkNode)) {
        g_stMetaMem.apstBlock[uiIdx] = HIMALLOC_NULL_PTR;
        VosMetaBitClear(g_stMetaMem.auiFreeMap, VOS_META_BLOCK_BIT_GROUPS, (uint16_t)uiIdx);
    } else {
        g_stMetaMem.apstBlock[uiIdx] = CSTL_LIST_ENTRY(pstNode->stLinkNode.next, VOS_META_BLOCK_NODE, stLinkNode);
        CSTL_ListRemove(&pstNode->stLinkNode);
    }

    VosMetaUnitSetAlloced(pstNode, pstNode->uvLen);
    pstPiece->uiUsedSize += (uint32_t)pstNode->uvLen;
}

static HIMALLOC_ALWAYS_INLINE VOS_META_BLOCK_NODE *VosMetaBlockExtract(uint32_t uiHashIdx)
{
    VOS_META_BLOCK_NODE *pstNode = g_stMetaMem.apstBlock[uiHashIdx];
    VOS_META_PIECE_S *pstPiece = VosMetaPieceHeadGet((const void *)pstNode);

    if (CSTL_ListIsEmpty(&pstNode->stLinkNode)) {
        g_stMetaMem.apstBlock[uiHashIdx] = HIMALLOC_NULL_PTR;
        VosMetaBitClear(g_stMetaMem.auiFreeMap, VOS_META_BLOCK_BIT_GROUPS, (uint16_t)uiHashIdx);
    } else {
        g_stMetaMem.apstBlock[uiHashIdx] = CSTL_LIST_ENTRY(pstNode->stLinkNode.next, VOS_META_BLOCK_NODE, stLinkNode);
        CSTL_ListRemove(&pstNode->stLinkNode);
    }

    VosMetaUnitSetAlloced(pstNode, pstNode->uvLen);
    pstPiece->uiUsedSize += (uint32_t)pstNode->uvLen;

    return pstNode;
}

static void *VosMetaAllocCoreAlign(size_t uvSize, size_t uvAlign)
{
    uint32_t uvNewSize = (uint32_t)uvSize;
    uintptr_t uvAddrTmp;
    uintptr_t uvEnd;
    uintptr_t uvStartAligned;
    uintptr_t uvEndAligned;

    uvNewSize = (uint32_t)VOS_META_HALIGN(uvNewSize, VOS_META_PAGE_SIZE);
    uvAddrTmp = ptr_to_ulong(mmap(NULL, uvNewSize + uvAlign,
        (int32_t)((uint32_t)PROT_READ | (uint32_t)PROT_WRITE), MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
    if (ulong_to_ptr(uvAddrTmp, void) == ulong_to_ptr(MAP_FAILED, void)) {
        VosHimallocLogWrite(HIMALLOC_ERRNO_NOMEM, "[HIMALLOC-%s]system memory is not enough, size(%u)",
            __FUNCTION__, uvNewSize + uvAlign);
        __hmattr_sideff_resolved("no relative side effect");
        return NULL;
    }
    g_stMetaMem.stStat.uvTotalSize += uvNewSize;

    uvEnd = uvAddrTmp + uvNewSize + uvAlign;
    uvStartAligned = (uvAddrTmp + uvAlign - 1UL) & ~((uintptr_t)uvAlign - 1UL);
    /* 头部向后偏移小于保护页大小，取下一个对齐的地址 */
    if (uvAddrTmp > uvStartAligned) {
        uvStartAligned += uvAlign;
    }

    /* 释放地址对齐后，前后多余的内存 */
    if (uvStartAligned > uvAddrTmp) {
        (void)munmap(ulong_to_ptr(uvAddrTmp, void), uvStartAligned - uvAddrTmp);
    }
    uvEndAligned = uvStartAligned + uvNewSize;
    if (uvEnd > uvEndAligned) {
        (void)munmap(ulong_to_ptr(uvEndAligned, void), uvEnd - uvEndAligned);
    }

    return ulong_to_ptr(uvStartAligned, void);
}

static void VosMetaFreeCore(void *pAddr, size_t uvSize)
{
    (void)munmap(pAddr, uvSize);
    g_stMetaMem.stStat.uvTotalSize -= uvSize;
}

static void VosMetaPieceAdd(void *pPieceStart)
{
    VOS_META_PIECE_S *pstPieceHead = pPieceStart;
    VOS_META_NODE_S *pstMetaUnit = pstPieceHead->astMetaUnit;
    uint32_t uiUnitNum = g_uiMetaUnitNum;
    size_t uvBlockSize = (size_t)uiUnitNum << VOS_META_BLOCK_MIN_LG;
    void *pBlockStart = VosMetaBlockStartInPieceGet(pPieceStart);
    uint32_t uiIdx;

    /* 设置内存首尾节点的信息 */
    pstMetaUnit[0].uiAlloced = 0U;
    pstMetaUnit[uiUnitNum - 1U].uiAlloced = 0U;
    pstMetaUnit[0].uiSliced = 0U;
    pstMetaUnit[uiUnitNum - 1U].uiSliced = 0U;

    uiIdx = VosMetaBlockSizeToIdx(uvBlockSize);
    pstMetaUnit[0].unVosMetaMemLen.uiBlockUnitNum = uiIdx + 1U;
    pstMetaUnit[uiUnitNum - 1U].unVosMetaMemLen.uiBlockUnitNum = uiIdx + 1U;

    /* 将扩展块添加到空闲block的hash表中 */
    VosMetaBlockNodeAdd(pBlockStart, uvBlockSize);
    g_stMetaMem.stStat.uvFreeSize += uvBlockSize;
    pstPieceHead->uiUsedSize = 0U;
}

static uint32_t VosMetaAppend(void)
{
    size_t uvAppendSize = g_uiMetaAppendSize;
    VOS_META_PIECE_S *pstPieceHead;

    pstPieceHead = VosMetaAllocCoreAlign(uvAppendSize, uvAppendSize);
    if (pstPieceHead == NULL) {
        VosHimallocLogWrite(HIMALLOC_ERRNO_NOMEM,
            "[VOS_METAALLOC-%s]system memory is not enough, alloc size(%lu)", __FUNCTION__, uvAppendSize);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_NOMEM;
    }

    VosMetaPieceAdd(pstPieceHead);

    return HIMALLOC_OK;
}

static void VosMetaShink(VOS_META_PIECE_S *pstPiece)
{
    size_t uvSize = g_uiMetaAppendSize;
    VOS_META_BLOCK_NODE *pstBlock = (VOS_META_BLOCK_NODE *)VosMetaBlockStartInPieceGet(pstPiece);

    VosMetaBlockNodeRemove(pstBlock);
    g_stMetaMem.stStat.uvFreeSize -= pstBlock->uvLen;
    (void)VosMetaFreeCore((void *)pstPiece, uvSize);
}

static uint32_t VosMetaApplyIdxGet(size_t uvSize)
{
    uint32_t *puiFreeMap = g_stMetaMem.auiFreeMap;
    uint32_t uiBit = VosMetaBlockSizeToIdx(uvSize);
    uint32_t uiBitRest = uiBit & VOS_BITMAP_NBITS_MASK;
    uint32_t uiGrpIdxStart = uiBit >> VOS_BITMAP_LG_BITS;
    uint32_t uiGrpIdxTmp;
    uint32_t uiValueTmp;
    uint32_t uiMetaApplyId = HIMALLOC_NULL_DWORD;

    uiGrpIdxTmp = uiGrpIdxStart;
    while (uiGrpIdxTmp < VOS_META_BLOCK_BIT_GROUPS) {
        if (uiGrpIdxTmp == uiGrpIdxStart) {
            uiValueTmp = puiFreeMap[uiGrpIdxTmp] & (~(uint32_t)((1UL << uiBitRest) - 1U));
        } else {
            uiValueTmp = puiFreeMap[uiGrpIdxTmp];
        }

        if (uiValueTmp != 0U) {
            uiMetaApplyId = (uiGrpIdxTmp << VOS_BITMAP_LG_BITS) + VosBitmapFFSImpl(uiValueTmp);
            break;
        }
        uiGrpIdxTmp++;
    }

    return uiMetaApplyId;
}

static void *VosMetaBlockAllocDirect(size_t uvSize)
{
    uint32_t uiIdx;
    VOS_META_BLOCK_NODE *pstAllocNode;
    void *pAddr;
    size_t uvReturnLen;

    /* 取出最适节点 */
    uiIdx = VosMetaApplyIdxGet(uvSize);
    if (uiIdx == HIMALLOC_NULL_DWORD) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }
    pstAllocNode = VosMetaBlockExtract(uiIdx);
    pAddr = (void *)pstAllocNode;

    /* 切割后归还剩余内存 */
    uvReturnLen = pstAllocNode->uvLen - uvSize;
    if (uvReturnLen != 0UL) {
        VosMetaBlockNodeAdd(ulong_to_ptr((ptr_to_ulong(pAddr) + uvSize), void), uvReturnLen);
    }

    /* 更新已分配的内存的管理结构 */
    VosMetaUnitSetAlloced(pAddr, uvSize);

    return pAddr;
}

static void *VosMetaBlockAlloc(size_t *puvSize)
{
    size_t uvSize = VOS_META_HALIGN(*puvSize, VOS_META_BLOCK_MIN);
    void *pAddr;
    uint32_t uiRet;

    pAddr = VosMetaBlockAllocDirect(uvSize);
    if (HIMALLOC_UNLIKELY(pAddr == HIMALLOC_NULL_PTR)) {
        uiRet = VosMetaAppend();
        if (HIMALLOC_LIKELY(uiRet == HIMALLOC_OK)) {
            pAddr = VosMetaBlockAllocDirect(uvSize);
        }
    }

    *puvSize = uvSize;
    return pAddr;
}

static void *VosMetaSliceAllocDirect(uint32_t uiIdx)
{
    VOS_META_SLICE_S *pstSliceInfo = &g_stMetaMem.astSliceCb[uiIdx];
    VOS_META_SLICE_CB *pstSliceCb;
    void *pAddr = HIMALLOC_NULL_PTR;

    pstSliceCb = pstSliceInfo->pstCurBlock;
    if (pstSliceCb != HIMALLOC_NULL_PTR) {
        VOS_META_SLICE_POP(pstSliceCb, pAddr);
        if (pstSliceCb->uiFreeNum == 0U) {
            pstSliceInfo->pstCurBlock = HIMALLOC_NULL_PTR;
            CSTL_ListAdd(&pstSliceCb->stLinkNode, &pstSliceInfo->stEmptyBlock);
        }
        __hmattr_sideff_resolved("no relative side effect");
        return pAddr;
    }

    if (!CSTL_ListIsEmpty(&pstSliceInfo->stBusyBlock)) {
        pstSliceCb = ptr_cast((pstSliceInfo->stBusyBlock.next), VOS_META_SLICE_CB);
        CSTL_ListRemove(&pstSliceCb->stLinkNode);
        pstSliceInfo->pstCurBlock = pstSliceCb;
    }

    if (pstSliceInfo->pstCurBlock != HIMALLOC_NULL_PTR) {
        VOS_META_SLICE_POP(pstSliceCb, pAddr);
        if (pstSliceCb->uiFreeNum == 0U) {
            pstSliceInfo->pstCurBlock = HIMALLOC_NULL_PTR;
            CSTL_ListAdd(&pstSliceCb->stLinkNode, &pstSliceInfo->stEmptyBlock);
        }
    }

    return pAddr;
}

static void VosMetaBlockSlicer(const VOS_META_SLICE_S *pstSliceInfo, VOS_META_SLICE_CB *pstBlock)
{
    uint16_t usUnitLen = pstSliceInfo->usUnitLen;
    uintptr_t uvCutAddr;
    uintptr_t uvEndAddr;
    VOS_META_SLICE_NODE *pstLastSlice;

    uvEndAddr = (uintptr_t)pstBlock + pstSliceInfo->usBlockLen;
    pstLastSlice = (VOS_META_SLICE_NODE *)((uintptr_t)pstBlock + sizeof(VOS_META_SLICE_CB));
    pstBlock->pstFree = pstLastSlice;

    /* 将Block切割成slice */
    while ((uvCutAddr = (uintptr_t)pstLastSlice + usUnitLen) <= uvEndAddr) {
        pstLastSlice->pNext = (VOS_META_SLICE_NODE *)uvCutAddr;
        pstLastSlice = (VOS_META_SLICE_NODE *)uvCutAddr;
    }
    pstLastSlice = (VOS_META_SLICE_NODE *)((uintptr_t)pstLastSlice - usUnitLen);
    pstLastSlice->pNext = HIMALLOC_NULL_PTR;

    pstBlock->uiFreeNum = pstSliceInfo->usNumPerBlock;
    CSTL_ListInit(&(pstBlock->stLinkNode));

    return;
}

static uint32_t VosMetaSliceGrow(uint32_t uiIdx)
{
    VOS_META_SLICE_S *pstSliceInfo = &g_stMetaMem.astSliceCb[uiIdx];
    size_t uvSize = pstSliceInfo->usBlockLen;
    VOS_META_SLICE_CB *pstBlock = VosMetaBlockAlloc(&uvSize);

    if (pstBlock == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_NOMEM;
    }

    VosMetaBlockSlicer(pstSliceInfo, pstBlock);
    VosMetaUnitSetSliced(pstBlock, uiIdx);
    pstSliceInfo->pstCurBlock = pstBlock;
    return HIMALLOC_OK;
}

static void *VosMetaSliceAlloc(size_t *puvSize)
{
    uint32_t uiIdx;
    void *pAddr;
    uint32_t uiRet;
    size_t uvNewSize;

    uiIdx = VosMetaSliceSizeToIdx(*puvSize);
    uvNewSize = VosMetaSliceIdxToSize(uiIdx);

    pAddr = VosMetaSliceAllocDirect(uiIdx);
    if (HIMALLOC_UNLIKELY(pAddr == HIMALLOC_NULL_PTR)) {
        uiRet = VosMetaSliceGrow(uiIdx);
        if (HIMALLOC_LIKELY(uiRet == HIMALLOC_OK)) {
            pAddr = VosMetaSliceAllocDirect(uiIdx);
        }
    }

    *puvSize = uvNewSize;
    return pAddr;
}

HIDDEN void *VosMetaAlloc(size_t uvSize)
{
    void *pAddr;
    size_t uvNewSize = uvSize;
    uint32_t uiLockValue;

    if (HIMALLOC_UNLIKELY(uvNewSize > VOS_META_ALLOC_SIZE_MAX)) {
        VosHimallocLogWrite(HIMALLOC_ERRNO_INVAL, "[VOS_METAALLOC-%s]alloc size is too large, size(%lu), max size(%u)",
            __FUNCTION__, uvNewSize, (uint32_t)VOS_META_ALLOC_SIZE_MAX);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }

    uiLockValue = OSAL_LockLock(&(g_stMetaMem.stLock));

    if (uvNewSize <= VOS_META_SLICE_MAX) {
        pAddr = VosMetaSliceAlloc(&uvNewSize);
    } else {
        pAddr = VosMetaBlockAlloc(&uvNewSize);
    }

    if (HIMALLOC_UNLIKELY(pAddr == HIMALLOC_NULL_PTR)) {
        OSAL_LockUnLock(&(g_stMetaMem.stLock), uiLockValue);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }

    g_stMetaMem.stStat.uvFreeSize -= uvNewSize;

    OSAL_LockUnLock(&(g_stMetaMem.stLock), uiLockValue);

    return pAddr;
}

static void *VosMetaBlockAllocDirectAlign(size_t uvSize, size_t uvAlign)
{
    uint32_t uiIdx;
    VOS_META_BLOCK_NODE *pstAllocNode;
    uintptr_t uvAddr;
    uintptr_t uvStartAligned;
    size_t uvTotalLen;

    /* 取出最适节点 */
    uiIdx = VosMetaApplyIdxGet(uvSize + uvAlign);
    if (uiIdx == HIMALLOC_NULL_DWORD) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }
    pstAllocNode = VosMetaBlockExtract(uiIdx);
    uvAddr = (uintptr_t)pstAllocNode;
    uvTotalLen = pstAllocNode->uvLen;
    uvStartAligned = (uintptr_t)VOS_META_HALIGN(uvAddr, uvAlign);
    if (uvStartAligned > uvAddr) {
        VosMetaBlockNodeAdd(ulong_to_ptr(uvAddr, void), (size_t)(uvStartAligned - uvAddr));
    }

    if (uvAddr + uvTotalLen > uvStartAligned + uvSize) {
        VosMetaBlockNodeAdd(ulong_to_ptr(uvStartAligned + uvSize, void),
            uvAddr + uvTotalLen - uvStartAligned - uvSize);
    }
    VosMetaUnitSetAlloced(ulong_to_ptr(uvStartAligned, void), uvSize);

    return ulong_to_ptr(uvStartAligned, void);
}

/* 对齐长度必须为2的幂次方，否则申请失败 */
HIDDEN void *VosMetaAllocAlign(size_t uvSize, size_t uvAlign)
{
    void *pAddr;
    size_t uvNewSize = VOS_META_HALIGN(uvSize, VOS_META_BLOCK_MIN);
    uint32_t lockValue;
    uint32_t uiRet;

    if ((uvAlign & (uvAlign - (size_t)1)) != 0) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }

    if (uvAlign <= VOS_META_BLOCK_MIN) {
        __hmattr_sideff_resolved("no relative side effect");
        return VosMetaAlloc(uvSize);
    }

    if (HIMALLOC_UNLIKELY(uvNewSize > VOS_META_ALLOC_SIZE_MAX - uvAlign)) {
        VosHimallocLogWrite(HIMALLOC_ERRNO_INVAL,
            "[VOS_METAALLOC-%s]alloc size is too large, size(%lu), max size(%u)",
            __FUNCTION__, uvSize, (uint32_t)(VOS_META_ALLOC_SIZE_MAX - uvAlign));
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }

    lockValue = OSAL_LockLock(&(g_stMetaMem.stLock));

    pAddr = VosMetaBlockAllocDirectAlign(uvNewSize, uvAlign);
    if (HIMALLOC_UNLIKELY(pAddr == HIMALLOC_NULL_PTR)) {
        uiRet = VosMetaAppend();
        if (HIMALLOC_LIKELY(uiRet == HIMALLOC_OK)) {
            pAddr = VosMetaBlockAllocDirectAlign(uvNewSize, uvAlign);
        }
    }

    if (HIMALLOC_UNLIKELY(pAddr == HIMALLOC_NULL_PTR)) {
        OSAL_LockUnLock(&(g_stMetaMem.stLock), lockValue);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }

    g_stMetaMem.stStat.uvFreeSize -= uvNewSize;

    OSAL_LockUnLock(&(g_stMetaMem.stLock), lockValue);

    return pAddr;
}

static uint32_t VosMetaBlockFree(void *pAddr)
{
    void *pStartMerged = pAddr;
    size_t uvSizeMerged;
    size_t uvSize;
    VOS_META_PIECE_S *pstPiece = VosMetaPieceHeadGet(pAddr);
    VOS_META_NODE_S *pstPreTail;
    VOS_META_NODE_S *pstCurNode;
    VOS_META_NODE_S *pstNextHead;
    size_t uvPrevLen;
    size_t uvNextLen;

    pstCurNode = VosMetaUnitNodeGet(pAddr);
    if (pstCurNode == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERR;
    }
    uvSize = (pstCurNode->uiSliced == 1U) ? VOS_META_BLOCK_MIN :
		 ((size_t)pstCurNode->unVosMetaMemLen.uiBlockUnitNum << VOS_META_BLOCK_MIN_LG);
    uvSizeMerged = uvSize;

	/* 前向合并 */
    pstPreTail = VosMetaUnitNodeGet(ulong_to_ptr((ptr_to_ulong(pAddr) - VOS_META_BLOCK_MIN), void));
    if (pstPreTail != NULL && pstPreTail->uiAlloced == 0U) {
        uvPrevLen = (pstPreTail->uiSliced == 1U) ? VOS_META_BLOCK_MIN :
            ((size_t)pstPreTail->unVosMetaMemLen.uiBlockUnitNum << VOS_META_BLOCK_MIN_LG);
        VosMetaBlockNodeRemove((VOS_META_BLOCK_NODE *)(ptr_to_ulong(pAddr) - uvPrevLen));
        uvSizeMerged += uvPrevLen;
        pStartMerged = ulong_to_ptr((ptr_to_ulong(pAddr) - uvPrevLen), void);
    }

    /* 后向合并 */
    pstNextHead = VosMetaUnitNodeGet(ulong_to_ptr((ptr_to_ulong(pAddr) + uvSize), VOS_META_NODE_S));
    if (pstNextHead != NULL && pstNextHead->uiAlloced == 0U) {
        uvNextLen = (pstNextHead->uiSliced == 1U) ? VOS_META_BLOCK_MIN :
            ((size_t)pstNextHead->unVosMetaMemLen.uiBlockUnitNum << VOS_META_BLOCK_MIN_LG);
        VosMetaBlockNodeRemove(ulong_to_ptr((ptr_to_ulong(pAddr) + uvSize), VOS_META_BLOCK_NODE));
        uvSizeMerged += uvNextLen;
    }

    VosMetaBlockNodeAdd(pStartMerged, uvSizeMerged);
    if (pstPiece->uiUsedSize == 0U) {
        VosMetaShink(pstPiece);
    }

    return HIMALLOC_OK;
}

static void VosMetaSliceFree(void *pAddr)
{
    VOS_META_NODE_S *pstBlockNode = VosMetaUnitNodeGet(pAddr);
    if (pstBlockNode == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return;
    }

    uint32_t uiIdx = pstBlockNode->unVosMetaMemLen.uiSliceType;
    VOS_META_SLICE_S *pstSliceInfo = &g_stMetaMem.astSliceCb[uiIdx];
    VOS_META_SLICE_CB *pstSliceCb = ulong_to_ptr(((ptr_to_ulong(pAddr)) & (pstSliceInfo->uvBlockRmask)),
        VOS_META_SLICE_CB);
    VOS_META_SLICE_NODE *pstSliceHead = (VOS_META_SLICE_NODE *)pAddr;

    /* 如果block的slice被全部申请完，将block从empty链表移到busy链表 */
    if (pstSliceCb->uiFreeNum == 0U) {
        CSTL_ListRemove(&(pstSliceCb->stLinkNode));
        CSTL_ListAdd(&(pstSliceCb->stLinkNode), &(pstSliceInfo->stBusyBlock));
    }

    pstSliceCb->uiFreeNum++;

    /* 如果block的slice已经全部释放，将block归还给block哈希表 */
    if (pstSliceCb->uiFreeNum == pstSliceInfo->usNumPerBlock) {
        if (pstSliceCb == pstSliceInfo->pstCurBlock) {
            pstSliceInfo->pstCurBlock = HIMALLOC_NULL_PTR;
        } else {
            CSTL_ListRemove(&(pstSliceCb->stLinkNode));
        }

        (void)VosMetaBlockFree(pstSliceCb);
    } else {
        pstSliceHead->pNext = pstSliceCb->pstFree;
        pstSliceCb->pstFree = pstSliceHead;
    }
    return;
}

/* 支持部分释放，但是size需要1k对齐 */
HIDDEN void VosMetaFree(void *pAddr)
{
    uint32_t uiLockValue;
    VOS_META_NODE_S *pstBlockInfo = VosMetaUnitNodeGet(pAddr);
    bool bIsSlice;
    size_t uvFreeSize;

    if (pstBlockInfo == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return;
    }
    bIsSlice = (bool)pstBlockInfo->uiSliced;

    uiLockValue = OSAL_LockLock(&(g_stMetaMem.stLock));
    if (bIsSlice) {
        uvFreeSize = VosMetaSliceIdxToSize(pstBlockInfo->unVosMetaMemLen.uiSliceType);
        VosMetaSliceFree(pAddr);
    } else {
        uvFreeSize = (size_t)pstBlockInfo->unVosMetaMemLen.uiBlockUnitNum << VOS_META_BLOCK_MIN_LG;
        (void)VosMetaBlockFree(pAddr);
    }
    g_stMetaMem.stStat.uvFreeSize += uvFreeSize;
    OSAL_LockUnLock(&(g_stMetaMem.stLock), uiLockValue);
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */
