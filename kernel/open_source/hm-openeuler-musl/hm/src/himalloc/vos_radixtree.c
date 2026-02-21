/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#include <stdlib.h>
#include <sys/mman.h>
#include "osal_lock.h"
#include "vos_radixtree.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

static uint32_t VosRtreeInfoCheck(VOS_RTREE_INFO_S *pstTreeInfo)
{
    size_t uvLevelIndex;
    size_t uvLevelNum;
    uint32_t uiTotalAccum;
    VOS_RTREE_LEVEL_S *pstLevelInfo;

    if (pstTreeInfo == HIMALLOC_NULL_PTR) {
        VosHimallocLogWrite(HIMALLOC_ERRNO_INVAL, "[HIMALLOC-%s]radix tree info is null", __FUNCTION__);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_INVAL;
    }

    uvLevelNum = pstTreeInfo->uvLevelNum;
    if ((uvLevelNum == 0) || (uvLevelNum > VOS_RTREE_LEVEL_MAX)) {
        VosHimallocLogWrite(HIMALLOC_ERRNO_INVAL, "[HIMALLOC-%s]level num(%lu) is invalid", __FUNCTION__, uvLevelNum);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_INVAL;
    }

    pstLevelInfo = &((pstTreeInfo->astLevelInfo)[0]);
    if (pstLevelInfo->uiLevelAccum < pstLevelInfo->uiLevelBits) {
        VosHimallocLogWrite(HIMALLOC_ERRNO_INVAL, "[HIMALLOC-%s]level info is invalid, leval accum(%u), level bit(%u)",
            __FUNCTION__, pstLevelInfo->uiLevelAccum, pstLevelInfo->uiLevelBits);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_INVAL;
    }
    uiTotalAccum = pstLevelInfo->uiLevelAccum - pstLevelInfo->uiLevelBits;

    for (uvLevelIndex = 0; uvLevelIndex < uvLevelNum; ++uvLevelIndex) {
        pstLevelInfo = &((pstTreeInfo->astLevelInfo)[uvLevelIndex]);
        if (pstLevelInfo->uiLevelNo != uvLevelIndex) {
            VosHimallocLogWrite(HIMALLOC_ERRNO_INVAL,
                "[HIMALLOC-%s]level info is invalid, leval no(%u), level index(%lu)", __FUNCTION__,
                pstLevelInfo->uiLevelNo, uvLevelIndex);
            __hmattr_sideff_resolved("no relative side effect");
            return HIMALLOC_ERRNO_INVAL;
        }

        if ((pstLevelInfo->uiLevelAccum) != (uiTotalAccum + pstLevelInfo->uiLevelBits)) {
            VosHimallocLogWrite(HIMALLOC_ERRNO_INVAL,
                "[HIMALLOC-%s]level info is invalid, leval accum(%u), level bit(%u), total accum(%u)",
                __FUNCTION__, pstLevelInfo->uiLevelAccum, pstLevelInfo->uiLevelBits, uiTotalAccum);
            __hmattr_sideff_resolved("no relative side effect");
            return HIMALLOC_ERRNO_INVAL;
        }

        uiTotalAccum += pstLevelInfo->uiLevelBits;
        if (uiTotalAccum > VOS_RTREE_SIZE_T_BITS) {
            VosHimallocLogWrite(VOS_RTREE_ERRNO_RANGE, "[HIMALLOC-%s]level info is invalid, uiTotalAccum(%u)",
                __FUNCTION__, uiTotalAccum);
            __hmattr_sideff_resolved("no relative side effect");
            return VOS_RTREE_ERRNO_RANGE;
        }
    }

    return HIMALLOC_OK;
}

static void VosRtreeNodeTrim(const void *pStart, size_t uvSize)
{
    uintptr_t uvStart = VOS_META_HALIGN(ptr_to_ulong(pStart), VOS_META_PAGE_SIZE);
    uintptr_t uvEnd = VOS_META_LALIGN(ptr_to_ulong(pStart) + uvSize, VOS_META_PAGE_SIZE);
    int32_t iRet;

    if (uvEnd > uvStart) {
        iRet = madvise(ulong_to_ptr(uvStart, void), uvEnd - uvStart, MADV_DONTNEED);
        if (iRet != 0) {
            VosHimallocLogWrite((uint32_t)iRet, "[HIMALLOC-%s]madvise fail: errno=%d\n", __FUNCTION__, iRet);
        }
    }

    return;
}

static VOS_RTREE_NODE_S *VosRtreeNodeAlloc(VOS_RTREE_ROOT_S *pstRoot, const VOS_RTREE_LEVEL_S *pstLevelInfo)
{
    size_t uvSize;
    VOS_RTREE_NODE_S *pstNode;

    uvSize = sizeof(void *) << pstLevelInfo->uiLevelBits;

    if (uvSize >= VOS_META_PAGE_SIZE) {
        pstNode = (VOS_RTREE_NODE_S *)mmap(NULL, uvSize, (int32_t)((uint32_t)PROT_READ | (uint32_t)PROT_WRITE),
            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    } else {
        pstNode = (VOS_RTREE_NODE_S *)VosMetaAlloc(uvSize);
    }
    if (pstNode == HIMALLOC_NULL_PTR || pstNode == MAP_FAILED) {
        VosHimallocLogWrite(VOS_RTREE_ERRNO_NOMEM, "[HIMALLOC-%s]system memory is not enough, size(%lu)",
            __FUNCTION__, uvSize);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }

    pstRoot->stTreeStat.uvTotalMem += uvSize;

    (void)memset(pstNode, 0, uvSize);
    VosRtreeNodeTrim(pstNode, uvSize);
    return pstNode;
}

static uint32_t VosRtreeRootInit(VOS_RTREE_ROOT_S *pstRoot)
{
    VOS_RTREE_LEVEL_S *pstLevelInfo;
    uint32_t uiRet = HIMALLOC_OK;

    pstLevelInfo = VosRtreeLevelInfoGet(pstRoot, (pstRoot->stTreeInfo).uvLevelNum - 1UL);
    pstRoot->pstTreeRoot = VosRtreeNodeAlloc(pstRoot, pstLevelInfo);
    if (pstRoot->pstTreeRoot == HIMALLOC_NULL_PTR) {
        uiRet = VOS_RTREE_ERRNO_NOMEM;
    }

    return uiRet;
}

static uint32_t VosRtreeInfoInit(VOS_RTREE_INFO_S *pstSrcTreeInfo, VOS_RTREE_ROOT_S *pstRoot)
{
    uint32_t uiRet;
    size_t uvLevelIndex;
    size_t uvLevelNum;
    VOS_RTREE_LEVEL_S *pstSrcLevelInfo;
    VOS_RTREE_LEVEL_S *pstDstLevelInfo;
    VOS_RTREE_INFO_S *pstDstTreeInfo;

    uiRet = VosRtreeInfoCheck(pstSrcTreeInfo);
    if (uiRet != HIMALLOC_OK) {
        __hmattr_sideff_resolved("no relative side effect");
        return uiRet;
    }

    pstDstTreeInfo = &(pstRoot->stTreeInfo);
    uvLevelNum = pstSrcTreeInfo->uvLevelNum;
    pstDstTreeInfo->uvLevelNum = uvLevelNum;
    for (uvLevelIndex = 0UL; uvLevelIndex < uvLevelNum; ++uvLevelIndex) {
        pstSrcLevelInfo = &((pstSrcTreeInfo->astLevelInfo)[uvLevelIndex]);
        pstDstLevelInfo = &((pstDstTreeInfo->astLevelInfo)[uvLevelIndex]);
        pstDstLevelInfo->uiLevelAccum = pstSrcLevelInfo->uiLevelAccum;
        pstDstLevelInfo->uvAccumRmask = ~((1UL << pstSrcLevelInfo->uiLevelAccum) - 1UL);
        pstDstLevelInfo->uiLevelBits = pstSrcLevelInfo->uiLevelBits;
        pstDstLevelInfo->uiLevelNo = pstSrcLevelInfo->uiLevelNo;
        pstDstLevelInfo->uvLevelMask = ((1UL << pstDstLevelInfo->uiLevelBits) - 1UL);
        pstDstLevelInfo->uiUpperAccum = pstDstLevelInfo->uiLevelAccum - pstDstLevelInfo->uiLevelBits;
    }

    return HIMALLOC_OK;
}

static uint32_t VosRtreeAllocInit(VOS_RTREE_ALLOC_S *pstAllocFun, VOS_RTREE_ROOT_S *pstRoot)
{
    /* Radix tree提供默认的申请释放函数 */
    if (pstAllocFun == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_OK;
    }

    if (pstAllocFun->pfnAlloc == HIMALLOC_NULL_PTR || pstAllocFun->pfnFree == HIMALLOC_NULL_PTR) {
        VosHimallocLogWrite(HIMALLOC_ERRNO_INVAL, "[HIMALLOC-%s]alloc or free func is invalid",
            __FUNCTION__);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_INVAL;
    }

    pstRoot->stAllocHook.pfnAlloc = pstAllocFun->pfnAlloc;
    pstRoot->stAllocHook.pfnFree = pstAllocFun->pfnFree;

    return HIMALLOC_OK;
}

static void VosRtreeCacheInit(VOS_RTREE_CACHE_ITEM_S astRtreeCache[], uint32_t uiNCache)
{
    uint32_t uiIndex;

    for (uiIndex = 0U; uiIndex < uiNCache; ++uiIndex) {
        astRtreeCache[uiIndex].pstCacheNode = HIMALLOC_NULL_PTR;
        astRtreeCache[uiIndex].uvPreKey = VOS_RTREE_IVALID_KEY;
    }

    return;
}

HIDDEN VOS_RTREE_T VosRtreeInit(VOS_RTREE_INFO_S *pstTreeInfo, VOS_RTREE_ALLOC_S *pstAllocFun)
{
    uint32_t uiRet;
    VOS_RTREE_ROOT_S *pstRoot;

    pstRoot = (VOS_RTREE_ROOT_S *)pstAllocFun->pfnAlloc(sizeof(VOS_RTREE_ROOT_S));
    if (pstRoot == HIMALLOC_NULL_PTR) {
        VosHimallocLogWrite(VOS_RTREE_ERRNO_NOMEM, "[HIMALLOC-%s]system memory is not enough, malloc size(%lu)",
            __FUNCTION__, sizeof(VOS_RTREE_ROOT_S));
        __hmattr_sideff_resolved("no relative side effect");
        return VOS_RTREE_IVALID_T;
    }
    (void)memset(pstRoot, 0, sizeof(VOS_RTREE_ROOT_S));

    /* 初始化radix tree的锁 */
    uiRet = OSAL_LockCreate(&(pstRoot->stLock));
    if (uiRet != HIMALLOC_OK) {
        VosHimallocLogWrite(uiRet, "[HIMALLOC-%s]create lock fail", __FUNCTION__);
        pstAllocFun->pfnFree(pstRoot);
        __hmattr_sideff_resolved("no relative side effect");
        return VOS_RTREE_IVALID_T;
    }

    /* 初始化申请释放钩子 */
    uiRet = VosRtreeAllocInit(pstAllocFun, pstRoot);
    if (uiRet != HIMALLOC_OK) {
        (void)OSAL_LockDestroy(&(pstRoot->stLock));
        pstAllocFun->pfnFree(pstRoot);
        __hmattr_sideff_resolved("no relative side effect");
        return VOS_RTREE_IVALID_T;
    }

    /* 初始化tree info信息 */
    uiRet = VosRtreeInfoInit(pstTreeInfo, pstRoot);
    if (uiRet != HIMALLOC_OK) {
        (void)OSAL_LockDestroy(&(pstRoot->stLock));
        pstAllocFun->pfnFree(pstRoot);
        __hmattr_sideff_resolved("no relative side effect");
        return VOS_RTREE_IVALID_T;
    }

    /* 初始化Tree的根节点 */
    uiRet = VosRtreeRootInit(pstRoot);
    if (uiRet != HIMALLOC_OK) {
        (void)OSAL_LockDestroy(&(pstRoot->stLock));
        pstAllocFun->pfnFree(pstRoot);
        __hmattr_sideff_resolved("no relative side effect");
        return VOS_RTREE_IVALID_T;
    }

    /* 初始化Cache信息 */
    VosRtreeCacheInit(pstRoot->astRtreeCache, (uint32_t)VOS_RTEE_NCACHE_MAX);

    return (VOS_RTREE_T)pstRoot;
}

HIDDEN VOS_RTREE_NODE_S *VosRtreeNodeCreate(VOS_RTREE_ROOT_S *pstRoot, VOS_RTREE_LEVEL_S *pstLevelInfo,
    VOS_RTREE_NODE_S *pstLevelNode, size_t uvSubKey)
{
    uint32_t uiLockValue;
    VOS_RTREE_NODE_S *pstSubNode;
    VOS_RTREE_LEVEL_S *pstSubLevelInfo;

    uiLockValue = OSAL_LockLock(&(pstRoot->stLock));

    /* 防止并发情况下，已有线程完成初始化 */
    if (pstLevelNode->apstNode[uvSubKey] != HIMALLOC_NULL_PTR) {
        OSAL_LockUnLock(&(pstRoot->stLock), uiLockValue);
        __hmattr_sideff_resolved("no relative side effect");
        return pstLevelNode->apstNode[uvSubKey];
    }

    /* 申请新的节点 */
    pstSubLevelInfo = pstLevelInfo - 1;
    pstSubNode = VosRtreeNodeAlloc(pstRoot, pstSubLevelInfo);
    pstLevelNode->apstNode[uvSubKey] = pstSubNode;
    OSAL_LockUnLock(&(pstRoot->stLock), uiLockValue);

    return pstSubNode;
}

HIDDEN VOS_RTREE_NODE_S *VosRtreeLevelNodeGet(VOS_RTREE_ROOT_S *pstRoot, size_t uvKey, bool bInitMiss)
{
    size_t uvIndex;
    size_t uvNLevel;
    size_t uvSubKey;
    VOS_RTREE_LEVEL_S *pstLevelInfo;
    VOS_RTREE_NODE_S *pstLevelNode;
    VOS_RTREE_NODE_S *pstSubNode;

    if (pstRoot == HIMALLOC_NULL_PTR) {
        VosHimallocLogWrite(HIMALLOC_ERRNO_INVAL, "[HIMALLOC-%s]radix tree is null", __FUNCTION__);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }

    pstLevelNode = pstRoot->pstTreeRoot;
    uvNLevel = pstRoot->stTreeInfo.uvLevelNum - 1UL;

    for (uvIndex = uvNLevel; uvIndex > 0; uvIndex--) {
        pstLevelInfo = VosRtreeLevelInfoGet(pstRoot, uvIndex);
        uvSubKey = VosRtreeSubKeyGet(pstLevelInfo, uvKey);
        pstSubNode = pstLevelNode->apstNode[uvSubKey];
        if ((pstSubNode == HIMALLOC_NULL_PTR) && (bInitMiss == true)) {
            pstSubNode = VosRtreeNodeCreate(pstRoot, pstLevelInfo, pstLevelNode, uvSubKey);
        }

        if (pstSubNode == HIMALLOC_NULL_PTR) {
            __hmattr_sideff_resolved("no relative side effect");
            return HIMALLOC_NULL_PTR;
        }
        pstLevelNode = pstSubNode;
    }

    return pstLevelNode;
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */
