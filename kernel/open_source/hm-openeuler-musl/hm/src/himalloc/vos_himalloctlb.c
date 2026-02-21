/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#include "osal_lock.h"
#include "vos_radixtree.h"
#include "vos_himalloc_errno.h"
#include "himalloc_inner.h"
#include "vos_himalloctlb.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

/*
 * 定义基数树的level数目，以及各层level负责的bit数
 * 为方便扩展，最后一层level需要根据size_t推导计算
 */
#if (__WORDSIZE == 32)

#define VOS_HIMALLOC_RTREE_LEVEL_NUM 2

static const uint8_t g_aucHimallocRtreeBits[VOS_RTREE_LEVEL_MAX] = {
    (uint8_t)VOS_HIMALLOC_RTREE_LG_NLEAF, 10, 0, 0
};

#else

#define VOS_HIMALLOC_RTREE_LEVEL_NUM 3

static const uint8_t g_aucHimallocRtreeBits[VOS_RTREE_LEVEL_MAX] = {
    (uint8_t)VOS_HIMALLOC_RTREE_LG_NLEAF, 12, 11, 0
};

#endif

/* 定义基数树的ID */
static VOS_RTREE_T g_pstHimallocRtreeId = VOS_RTREE_IVALID_T;

HIDDEN VOS_RTREE_T VosHimallocRtreeIdGet(void)
{
    return g_pstHimallocRtreeId;
}

static uint32_t VosHimallocTlbRtreeInit(void)
{
    uint32_t uiIndex;
    uint32_t uiLevelAccum;
    VOS_RTREE_INFO_S stRTreeInfo;
    VOS_RTREE_LEVEL_S *pstLevelInfo = HIMALLOC_NULL_PTR;
    VOS_RTREE_ALLOC_S stAllocFree = { VosMetaAlloc, VosMetaFree };
    uint32_t uiRet = HIMALLOC_OK;

    (void)memset(&stRTreeInfo, 0, sizeof(VOS_RTREE_INFO_S));
    stRTreeInfo.uvLevelNum = VOS_HIMALLOC_RTREE_LEVEL_NUM;

    /* 非最后一级映射的BIT位由全局变量的配置值获取 */
    uiLevelAccum = VOS_HIMALLOC_LG_VPAGE_SIZE;
    for (uiIndex = 0; uiIndex < VOS_HIMALLOC_RTREE_LEVEL_NUM; ++uiIndex) {
        pstLevelInfo = &(stRTreeInfo.astLevelInfo[uiIndex]);
        pstLevelInfo->uiLevelBits = g_aucHimallocRtreeBits[uiIndex];
        uiLevelAccum += g_aucHimallocRtreeBits[uiIndex];
        pstLevelInfo->uiLevelAccum = uiLevelAccum;
        pstLevelInfo->uiLevelNo = uiIndex;
    }

    g_pstHimallocRtreeId = VosRtreeInit(&stRTreeInfo, &stAllocFree);
    if (g_pstHimallocRtreeId == VOS_RTREE_IVALID_T) {
        uiRet = HIMALLOC_ERRNO_INVAL;
    }

    return uiRet;
}

HIDDEN void VosHimallocTlbInit(VOS_HIMALLOC_TLB_INFO_S *pstTlbInfo)
{
    uint32_t uiIndex;

    (void)memset(pstTlbInfo, 0, sizeof(VOS_HIMALLOC_TLB_INFO_S));

    pstTlbInfo->stL1Cache.pstLeafNode = HIMALLOC_NULL_PTR;
    pstTlbInfo->stL1Cache.uvPrevKey = HIMALLOC_NULL_SIZE_T;

    for (uiIndex = 0; uiIndex < VOS_HIMALLOC_TLB_NCACHE_MAX; ++uiIndex) {
        pstTlbInfo->astL2Cache[uiIndex].pstLeafNode = HIMALLOC_NULL_PTR;
        pstTlbInfo->astL2Cache[uiIndex].uvPrevKey = HIMALLOC_NULL_SIZE_T;
    }
}

HIDDEN uint32_t VosHimallocTlbModleInit(void)
{
    return VosHimallocTlbRtreeInit();
}

static VOS_HIMALLOC_BLOCK **VosHimallocTlbItemSet(VOS_HIMALLOC_TLB_INFO_S *pstTlbInfo, size_t uvKey,
    VOS_RTREE_NODE_S *pstRtreeEntry)
{
    size_t uvSlot;
    size_t uvLeafKey;
    size_t uvLeafPreKey;
    VOS_HIMALLOC_TLB_ITEM_S *pstTlbItem;

    uvLeafPreKey = VosHimallocTlbPrevKeyGet(uvKey);
    uvSlot = VosHimallocTlbSlotGet(uvKey);

    pstTlbItem = &(pstTlbInfo->astL2Cache[uvSlot]);
    pstTlbItem->uvPrevKey = uvLeafPreKey;
    pstTlbItem->pstLeafNode = pstRtreeEntry;

    uvLeafKey = VosHimallocTlbLeafKeyGet(uvKey);
    return (ptr_cast((&(pstTlbItem->pstLeafNode->apstNode[uvLeafKey])), VOS_HIMALLOC_BLOCK *));
}

HIDDEN VOS_HIMALLOC_BLOCK *VosHimallocTlbHitMiss(VOS_HIMALLOC_TLB_INFO_S *pstTlbInfo, size_t uvKey)
{
    VOS_RTREE_NODE_S *pstRtreeEntry;
    VOS_HIMALLOC_BLOCK **ppstBlockItem;

    pstRtreeEntry = VosRtreeLevelNodeGet(g_pstHimallocRtreeId, uvKey, true);
    if (pstRtreeEntry == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }

    ppstBlockItem = VosHimallocTlbItemSet(pstTlbInfo, uvKey, pstRtreeEntry);
    return *ppstBlockItem;
}

HIDDEN uint32_t VosHimallocTlbSetHard(VOS_HIMALLOC_TLB_INFO_S *pstTlbInfo, size_t uvKey, void *pObject)
{
    VOS_RTREE_NODE_S *pstRtreeEntry;
    VOS_HIMALLOC_BLOCK **ppstBlockItem;

    pstRtreeEntry = VosRtreeLevelNodeGet(g_pstHimallocRtreeId, uvKey, true);
    if (pstRtreeEntry == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_NOMEM;
    }

    ppstBlockItem = VosHimallocTlbItemSet(pstTlbInfo, uvKey, pstRtreeEntry);
    *ppstBlockItem = pObject;
    return HIMALLOC_OK;
}

static HIMALLOC_ALWAYS_INLINE VOS_HIMALLOC_BLOCK **VosHimallocTlbRtreeEntryGet(size_t uvKey)
{
    size_t uvLeafKey;
    VOS_RTREE_NODE_S *pstRtreeEntry;

    pstRtreeEntry = VosRtreeLevelNodeGet(g_pstHimallocRtreeId, uvKey, true);
    if (pstRtreeEntry == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }

    uvLeafKey = VosHimallocTlbLeafKeyGet(uvKey);
    return (VOS_HIMALLOC_BLOCK **)(&(pstRtreeEntry->apstNode[uvLeafKey]));
}

VOS_HIMALLOC_BLOCK *VosHimallocTlbRtreeGet(size_t uvKey)
{
    VOS_HIMALLOC_BLOCK **ppstBlockItem;

    ppstBlockItem = VosHimallocTlbRtreeEntryGet(uvKey);
    if (ppstBlockItem == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }

    return (*ppstBlockItem);
}

uint32_t VosHimallocTlbRtreeSet(size_t uvKey, void *pObject)
{
    VOS_HIMALLOC_BLOCK **ppstBlockItem;

    ppstBlockItem = VosHimallocTlbRtreeEntryGet(uvKey);
    if (ppstBlockItem == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_NOMEM;
    }

    *ppstBlockItem = pObject;
    return HIMALLOC_OK;
}

HIDDEN bool VosHimallocTlbRtreeIsInstall(void)
{
    bool bRet = false;
    if (g_pstHimallocRtreeId != HIMALLOC_NULL_PTR) {
        bRet = true;
    }
    return bRet;
}

HIDDEN VOS_HIMALLOC_BLOCK *VosHimallocTlbMatchHard(VOS_HIMALLOC_TLB_INFO_S *pstTlbInfo, size_t uvKey)
{
    size_t uvSlot;
    size_t uvLeafKey;
    size_t uvLeafPreKey;
    VOS_HIMALLOC_TLB_ITEM_S *pstL1Item;
    VOS_HIMALLOC_TLB_ITEM_S *pstL2Item;

    uvLeafPreKey = VosHimallocTlbPrevKeyGet(uvKey);
    uvSlot = VosHimallocTlbSlotGet(uvKey);
    pstL2Item = &(pstTlbInfo->astL2Cache[uvSlot]);
    if (pstL2Item->uvPrevKey == uvLeafPreKey) {
        /* 更新L1缓存 */
        pstL1Item = &(pstTlbInfo->stL1Cache);
        pstL1Item->uvPrevKey = uvLeafPreKey;
        pstL1Item->pstLeafNode = pstL2Item->pstLeafNode;

        /* 返回cache对象 */
        uvLeafKey = VosHimallocTlbLeafKeyGet(uvKey);
        return pstL2Item->pstLeafNode->apstNode[uvLeafKey];
    }

    return VosHimallocTlbHitMiss(pstTlbInfo, uvKey);
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */
