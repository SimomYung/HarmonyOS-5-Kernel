/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#include "vos_himallocarena.h"
#include "vos_himallocassert.h"
#include "vos_metaalloc.h"
#include "vos_himalloctcache.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

HIDDEN VOS_HIMALLOC_TCACHE_SLICE_INFO g_astHimallocTcacheSliceInfo[VOS_HIMALLOC_MAX_SMALL_INDEX];

/* tcache管理结构内存申请释放钩子 */
HIDDEN VOS_HIMALLOC_TCACHE_MANAGE_HOOK g_stHimallocTcacheManageHook;
static HIMALLOC_ALWAYS_INLINE void VosHimallocTcacheManageHookReg(void)
{
    g_stHimallocTcacheManageHook.pfnAlloc = (VOS_HIAMALLOC_TCACHE_MANAGE_ALLOC_HOOK)VosMetaAlloc;
    g_stHimallocTcacheManageHook.pfnFree = (VOS_HIAMALLOC_TCACHE_MANAGE_FREE_HOOK)VosMetaFree;
}

static void VosHimallocTcacheCachedLimitGet(const char *pscKey, uint32_t *puiCachedMin, uint32_t *puiCachedMax)
{
    *puiCachedMin = VOS_HIMALLOC_TCACHE_DEFAULT_MIN;
    *puiCachedMax = VOS_HIMALLOC_TCACHE_DEFAULT_MAX;

    (void)pscKey;

    return;
}

/* 初始化Tcache slice info */
static void VosHimallocTcacheSliceInfoInit(const char *pscKey)
{
    uint16_t usIndex;
    uint32_t uiCachedNum;
    uint32_t uiCachedMin;
    uint32_t uiCachedMax;
    VOS_HIMALLOC_SLICE_INFO *pstSliceInfo;

    VosHimallocTcacheCachedLimitGet(pscKey, &uiCachedMin, &uiCachedMax);

    /* 初始化各个size class最大缓存 */
    for (usIndex = 0U; usIndex < VOS_HIMALLOC_MAX_SMALL_INDEX; usIndex++) {
        pstSliceInfo = VosHimallocSliceInfoGet(usIndex);
        uiCachedNum = (uint32_t)pstSliceInfo->usSliceNum;
        if (uiCachedNum > uiCachedMax) {
            uiCachedNum = uiCachedMax;
        }
        if (uiCachedNum <= uiCachedMin) {
            uiCachedNum = uiCachedMin;
        }

        g_astHimallocTcacheSliceInfo[usIndex].usMaxCached = (uint16_t)uiCachedNum;
    }

    return;
}

static HIMALLOC_ALWAYS_INLINE VOS_HIMALLOC_TCACHE_NODE_S *VosHimallocTcacheNodeGetFromCache(
    VOS_HIMALLOC_TCACHE_S *pstTcache, uint8_t ucArenaId)
{
    return pstTcache->apstTcacheNodeArray[ucArenaId];
}

static void VosHimallocTcacheClear(VOS_HIMALLOC_ARENA_S *pstArena, VOS_HIMALLOC_TCACHE_NODE_S *pstCacheNode)
{
    uint8_t ucSizeClass;
    uint8_t ucMaxSliceClass;
    VOS_HIMALLOC_TCACHE_SLICE *pstTcacheSlice;

    ucMaxSliceClass = (uint8_t)VosHimallocSize2Index(pstArena->usMaxSliceSize);
    for (ucSizeClass = 0U; ucSizeClass <= ucMaxSliceClass; ucSizeClass++) {
        pstTcacheSlice = &(pstCacheNode->astMemCached[ucSizeClass]);
        if (pstTcacheSlice->usNumCached == 0U) {
            continue;
        }

        (void)VosHimallocSliceBinFlush(pstArena, pstTcacheSlice, pstCacheNode->pstTlb, pstTcacheSlice->usNumCached,
            ucSizeClass);
    }

    return;
}

static void VosHimallocTcacheScavenge(uint8_t ucArenaId, VOS_HIMALLOC_TCACHE_NODE_S *pstCacheNode)
{
    uint32_t uiRetValue;
    VOS_HIMALLOC_ARENA_S *pstArena;

    pstArena = VosHimallocArenaGet(ucArenaId);
    VosHimallocTcacheClear(pstArena, pstCacheNode);

    uiRetValue = OSAL_LockLock(&(pstArena->stTcacheLock));
    CSTL_ListRemove(&(pstCacheNode->stLinkNode));
    OSAL_LockUnLock(&(pstArena->stTcacheLock), uiRetValue);

    return;
}

static void VosHimallocTcacheCleanup(VOS_HIMALLOC_TCACHE_S *pstTcache)
{
    uint8_t ucArenaId;
    VOS_HIMALLOC_ARENA_S *pstArena;
    VOS_HIMALLOC_TCACHE_NODE_S *pstCacheNode;
    VOS_HIAMALLOC_TCACHE_MANAGE_FREE_HOOK pfnManageFree = g_stHimallocTcacheManageHook.pfnFree;

    for (ucArenaId = 0U; ucArenaId < VOS_HIMALLOC_TCACHE_ARRAY_LENGTH; ucArenaId++) {
        pstArena = VosHimallocArenaGet(ucArenaId);
        if ((pstArena == HIMALLOC_NULL_PTR) || (pstArena->ucTcacheType != (uint8_t)VOS_HIMALLOC_TCACHE_OPEN)) {
            continue;
        }
        pstCacheNode = VosHimallocTcacheNodeGetFromCache(pstTcache, ucArenaId);
        if (pstCacheNode == HIMALLOC_NULL_PTR) {
            /* 当前线程在这个arena中没有缓存过任何内存 */
            continue;
        }

        VosHimallocTcacheScavenge(ucArenaId, pstCacheNode);
        pfnManageFree(pstCacheNode);
        pstTcache->apstTcacheNodeArray[ucArenaId] = HIMALLOC_NULL_PTR;
    }

    return;
}

HIDDEN void VosHimallocThreadDestroy(long long tcacheAddr)
{
    VOS_HIMALLOC_TCACHE_S *pstLocalTcache;
    VOS_HIAMALLOC_TCACHE_MANAGE_FREE_HOOK pfnManageFree = g_stHimallocTcacheManageHook.pfnFree;

    if (tcacheAddr <= HIMALLOC_OPEN_TCACHE_THRESHOLD) {
        /* 未创建线程私有变量，直接退出即可 */
        __pthread_self()->himalloc_tcache_addr = 0LL;
        return;
    }

    pstLocalTcache = ulong_to_ptr(tcacheAddr, VOS_HIMALLOC_TCACHE_S);
    /* 当前线程使用了线程缓存 */
    VosHimallocTcacheCleanup(pstLocalTcache);
    (void)pfnManageFree(pstLocalTcache);
    pstLocalTcache = HIMALLOC_NULL_PTR;
    __pthread_self()->himalloc_tcache_addr = 0LL;

    return;
}

HIDDEN uint32_t VosHimallocTcacheModuleInit(void)
{
    VosHimallocTcacheManageHookReg();
    VosHimallocTcacheSliceInfoInit(HIMALLOC_NULL_PTR);

    return HIMALLOC_OK;
}

#ifdef HIMALLOC_TCACHE_POLICY_LIST // tcache方案采用链表组织缓存资源池的策略，底噪开销较小

static HIMALLOC_ALWAYS_INLINE size_t VosHimallocTcacheNodeSizeCompute(uint8_t ucMaxSliceClass)
{
    return (sizeof(VOS_HIMALLOC_TCACHE_NODE_S) + sizeof(VOS_HIMALLOC_TCACHE_SLICE) * (ucMaxSliceClass + 1));
}

#else // tcache方案采用数组组织缓存资源池的策略，底噪开销较大

static HIMALLOC_ALWAYS_INLINE size_t VosHimallocTcacheNodeSizeCompute(uint8_t ucMaxSliceClass)
{
    uint8_t ucSizeClass;
    size_t uvNodeSize;
    size_t uvTcacheBinSize = sizeof(VOS_HIMALLOC_TCACHE_SLICE);

    uvNodeSize = sizeof(VOS_HIMALLOC_TCACHE_NODE_S);

    for (ucSizeClass = 0U; ucSizeClass <= ucMaxSliceClass; ucSizeClass++) {
        uvNodeSize += uvTcacheBinSize;
        uvNodeSize += g_astHimallocTcacheSliceInfo[ucSizeClass].usMaxCached * sizeof(void *);
    }

    return uvNodeSize;
}

#endif

static HIMALLOC_ALWAYS_INLINE void VosHimallocTcaheCacheNodeInit(VOS_HIMALLOC_TCACHE_NODE_S *pstCacheNode,
    uint8_t ucMaxSliceClass)
{
    uint8_t ucSizeClass;
    VOS_HIMALLOC_TCACHE_SLICE *pstTcacheSlice = HIMALLOC_NULL_PTR;
#ifdef HIMALLOC_TCACHE_POLICY_ARRAY // 其他场景下，采用数组缓存策略，空间较大
    uintptr_t uvStackStart;
    /* 地址偏移到tcache slice控制头的末尾 */
    uvStackStart = (uintptr_t)(pstCacheNode->astMemCached) + sizeof(VOS_HIMALLOC_TCACHE_SLICE) * (ucMaxSliceClass + 1U);
#endif

    for (ucSizeClass = 0U; ucSizeClass <= ucMaxSliceClass; ucSizeClass++) {
        pstTcacheSlice = (VOS_HIMALLOC_TCACHE_SLICE *)(&(pstCacheNode->astMemCached[ucSizeClass]));
        /* 缓存刚创建时是满规格的1/4 */
        pstCacheNode->ucFillDividendLg[ucSizeClass] = 2U;
		/* 为了提高扩展效率，起始的下水位设置为0，即pstTcacheSlice->usLowWater = 0U，但是前面memset为0过，因此无需再写 */
        /* 设置size class的缓存上限 */
        pstTcacheSlice->usMaxCached = g_astHimallocTcacheSliceInfo[ucSizeClass].usMaxCached;
#ifdef HIMALLOC_TCACHE_POLICY_ARRAY // 其他场景下，采用数组缓存策略，空间较大
        /* 下一个class范围的起点 */
        uvStackStart += sizeof(void *) * pstTcacheSlice->usMaxCached;
        /* 当前class的最后一个指针的首地址 */
        pstTcacheSlice->ppFreeStack = (void **)(uvStackStart - sizeof(void *));
#endif
    }

    CSTL_ListInit(&(pstCacheNode->stLinkNode));

    return;
}

HIDDEN VOS_HIMALLOC_TCACHE_NODE_S *VosHimallocTcacheCacheNodeCreat(VOS_HIMALLOC_TCACHE_S *pstHimTcache,
    uint8_t ucArenaId)
{
    uint8_t ucMaxSliceClass;
    uint32_t uiRetValue;
    size_t uvCacheNodeSize;
    VOS_HIMALLOC_ARENA_S *pstArena;
    VOS_HIMALLOC_TCACHE_NODE_S *pstCacheNode;
    VOS_HIAMALLOC_TCACHE_MANAGE_ALLOC_HOOK pfnManageAlloc;

    /* 内部接口，外层已防护，不需要判断有效性 */
    pstArena = VosHimallocArenaGet(ucArenaId);
    ucMaxSliceClass = (uint8_t)VosHimallocSize2Index(pstArena->usMaxSliceSize);

    uvCacheNodeSize = VosHimallocTcacheNodeSizeCompute(ucMaxSliceClass);
    pfnManageAlloc = g_stHimallocTcacheManageHook.pfnAlloc;
    pstCacheNode = (VOS_HIMALLOC_TCACHE_NODE_S *)pfnManageAlloc(uvCacheNodeSize);
    if (pstCacheNode == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }
    (void)memset(pstCacheNode, 0, uvCacheNodeSize);
    pstHimTcache->apstTcacheNodeArray[ucArenaId] = pstCacheNode;
    pstCacheNode->pstTlb = &(pstHimTcache->stThreadTlb);

    VosHimallocTcaheCacheNodeInit(pstCacheNode, ucMaxSliceClass);

    /* 只有在开启线程缓存时才需要StatNode */
    if (pstArena->ucTcacheType == (uint8_t)VOS_HIMALLOC_TCACHE_OPEN) {
        uiRetValue = OSAL_LockLock(&(pstArena->stTcacheLock));
        CSTL_ListAdd(&(pstCacheNode->stLinkNode), &(pstArena->stLinkTcache));
        OSAL_LockUnLock(&(pstArena->stTcacheLock), uiRetValue);
    }

    return pstCacheNode;
}

HIDDEN VOS_HIMALLOC_TCACHE_S *VosHimallocTcacheLocalHeapCreate(uint8_t ucTcacheType)
{
    size_t uvThreadHeapSize;
    VOS_HIMALLOC_TCACHE_S *pstLocalTcache = HIMALLOC_NULL_PTR;
    VOS_HIAMALLOC_TCACHE_MANAGE_ALLOC_HOOK pfnManageAlloc;

    uvThreadHeapSize = sizeof(VOS_HIMALLOC_TCACHE_S);
    if (ucTcacheType == VOS_HIMALLOC_TCACHE_OPEN) {
        /* 鸿蒙OS用不到多个分区，只用0号分区，因此此处只申请一个节点 */
        uvThreadHeapSize += sizeof(VOS_HIMALLOC_TCACHE_NODE_S *);
    }

    pfnManageAlloc = g_stHimallocTcacheManageHook.pfnAlloc;
    pstLocalTcache = pfnManageAlloc(uvThreadHeapSize);
    if (pstLocalTcache == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }

    (void)memset(pstLocalTcache, 0, uvThreadHeapSize);

    __pthread_self()->himalloc_tcache_addr = (long long)(uint64_t)pstLocalTcache;

    /* 初始化线程私有的tlb结构 */
    (void)VosHimallocTlbInit(&(pstLocalTcache->stThreadTlb));

    return pstLocalTcache;
}

HIDDEN void VosHimallocTcacheGcFluch(VOS_HIMALLOC_ARENA_S *pstArena, VOS_HIMALLOC_TCACHE_NODE_S *pstCacheNode)
{
    VOS_HIMALLOC_TCACHE_SLICE *pstTcacheSlice;
    uint16_t usLowWater;
    uint32_t uiShrinkNum;
    uint8_t ucTypeNum;
    uint8_t i;

    ucTypeNum = (uint8_t)(VosHimallocSize2Index(pstArena->usMaxSliceSize) + 1U);

    for (i = 0U; i < ucTypeNum; i++) {
        pstTcacheSlice = &(pstCacheNode->astMemCached)[i];
        usLowWater = pstTcacheSlice->usLowWater;
        /* 回收周期内从来没有使用的内存的一半 */
        uiShrinkNum = (uint32_t)usLowWater >> 1;
        if (uiShrinkNum != 0U) {
            (void)VosHimallocSliceBinFlush(pstArena, pstTcacheSlice, pstCacheNode->pstTlb, uiShrinkNum, i);
        }

        /* 如果上一个GC周期对应class的缓存用完了，下一次两倍扩展 */
        if (usLowWater == 0U && pstCacheNode->ucFillDividendLg[i] > 1) {
            pstCacheNode->ucFillDividendLg[i]--;
        }
        /* 如果上一个GC周期对应class的缓存没有用完，下一次折半扩展,但是每次扩展的数量下限不能小于1 */
        if ((usLowWater != 0U) && ((pstTcacheSlice->usMaxCached >> pstCacheNode->ucFillDividendLg[i]) > 1)) {
            pstCacheNode->ucFillDividendLg[i]++;
        }
        /* 开始新一个GC周期，下水位更新至当前的slice */
        pstTcacheSlice->usLowWater = pstTcacheSlice->usNumCached;
    }

    pstCacheNode->uiGcCount = 0U;

    return;
}

HIDDEN uint32_t VosHimallocTcacheEnable(uint8_t ucArenaId)
{
    uint32_t uiRet;
    VOS_HIMALLOC_ARENA_S *pstArena;

    pstArena = VosHimallocArenaGet(ucArenaId);
    if (pstArena == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_INVAL;
    }

    uiRet = OSAL_LockCreate(&(pstArena->stTcacheLock));
    if (uiRet != HIMALLOC_OK) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_NOMEM;
    }
    CSTL_ListInit(&(pstArena->stLinkTcache));
    pstArena->ucTcacheType = (uint8_t)VOS_HIMALLOC_TCACHE_OPEN;

    return HIMALLOC_OK;
}

HIDDEN bool VosHimallocTcacheIsEnable(uint8_t ucArenaId)
{
    VOS_HIMALLOC_ARENA_S *pstArena;

    pstArena = VosHimallocArenaGet(ucArenaId);
    if (pstArena == HIMALLOC_NULL_PTR) {
        return false;
    }

    return (pstArena->ucTcacheType == (uint8_t)VOS_HIMALLOC_TCACHE_OPEN);
}

HIDDEN __pure uint16_t VosHimallocTcacheLeftCachedNumGet(VOS_HIMALLOC_ARENA_S *pstArena, uint32_t uiSizeClass)
{
    uint8_t ucMaxSliceClass;
    uint16_t usCachedNum = 0;
    uint32_t uiRetValue;
    VOS_HIMALLOC_TCACHE_NODE_S *pstCacheNode;
    CSTL_LIST_HEAD_S *pstListHead = HIMALLOC_NULL_PTR;
    CSTL_LIST_HEAD_S *pstListNode = HIMALLOC_NULL_PTR;

    ucMaxSliceClass = (uint8_t)VosHimallocSize2Index(pstArena->usMaxSliceSize);
    if (uiSizeClass > ucMaxSliceClass) {
        return 0;
    }

    if (pstArena->ucTcacheType != (uint8_t)VOS_HIMALLOC_TCACHE_OPEN) {
        return 0;
    }

    uiRetValue = OSAL_LockLock(&(pstArena->stTcacheLock));
    pstListHead = &(pstArena->stLinkTcache);
    if (CSTL_ListIsEmpty(pstListHead)) {
        OSAL_LockUnLock(&(pstArena->stTcacheLock), uiRetValue);
        return 0;
    }
    CSTL_LIST_FOR_EACH_ITEM(pstListNode, pstListHead) {
        pstCacheNode = CSTL_LIST_ENTRY(pstListNode, VOS_HIMALLOC_TCACHE_NODE_S, stLinkNode);
        usCachedNum += (pstCacheNode->astMemCached[uiSizeClass]).usNumCached;
    }
    OSAL_LockUnLock(&(pstArena->stTcacheLock), uiRetValue);

    return usCachedNum;
}

#ifdef HIMALLOC_TCACHE_POLICY_LIST // tcache方案采用链表组织缓存资源池的策略，底噪开销较小
static HIMALLOC_ALWAYS_INLINE bool VosHimallocTcacheSliceFind(VOS_HIMALLOC_TCACHE_SLICE *pTcacheSlice,
    const void *pAddr)
{
    VOS_HIMALLOC_TCACHE_FREE_SLICE_CB *freeSliceList = pTcacheSlice->pFreeStackList;

    while (freeSliceList != NULL) {
        if (pAddr == freeSliceList) {
            return true;
        }
        freeSliceList = freeSliceList->pstTcacheNextSlice;
    }

    return false;
}
#else // tcache方案采用数组组织缓存资源池的策略，底噪开销较大
static HIMALLOC_ALWAYS_INLINE bool VosHimallocTcacheSliceFind(VOS_HIMALLOC_TCACHE_SLICE *pTcacheSlice,
    const void *pAddr)
{
    uint16_t usIndex;
    uint16_t usNumCached = pTcacheSlice->usNumCached;
    void **ppFreeStackTmp = pTcacheSlice->ppFreeStack;

    for (usIndex = 0; usIndex < usNumCached; usIndex++) {
        if (pAddr == *(void **)(ppFreeStackTmp - usIndex)) {
            return true;
        }
    }

    return false;
}
#endif

HIDDEN bool VosHimallocTcacheAddrIsCached(const void *pAddr, uint8_t ucSizeClass, uint8_t ucArenaId)
{
    uint32_t uiRetValue;
    VOS_HIMALLOC_ARENA_S *pstArena;
    VOS_HIMALLOC_TCACHE_NODE_S *pstCacheNode;
    VOS_HIMALLOC_TCACHE_SLICE *pstTcacheSlice;
    CSTL_LIST_HEAD_S *pstListHead = NULL;
    CSTL_LIST_HEAD_S *pstListNode = NULL;

    pstArena = VosHimallocArenaGet(ucArenaId);
    if (pstArena == NULL) {
        return false;
    }

    if (pstArena->ucTcacheType != VOS_HIMALLOC_TCACHE_OPEN) {
        return false;
    }

    uiRetValue = OSAL_LockLock(&(pstArena->stTcacheLock));
    pstListHead = &(pstArena->stLinkTcache);
    if (CSTL_ListIsEmpty(pstListHead)) {
        OSAL_LockUnLock(&(pstArena->stTcacheLock), uiRetValue);
        return false;
    }

    CSTL_LIST_FOR_EACH_ITEM(pstListNode, pstListHead) {
        pstCacheNode = CSTL_LIST_ENTRY(pstListNode, VOS_HIMALLOC_TCACHE_NODE_S, stLinkNode);
        pstTcacheSlice = &(pstCacheNode->astMemCached[ucSizeClass]);
        if (VosHimallocTcacheSliceFind(pstTcacheSlice, pAddr)) {
            OSAL_LockUnLock(&(pstArena->stTcacheLock), uiRetValue);
            return true;
        }
    }
    OSAL_LockUnLock(&(pstArena->stTcacheLock), uiRetValue);

    return false;
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */
