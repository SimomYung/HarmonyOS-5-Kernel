/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#ifndef VOS_HIMALLOCTCACHE_H
#define VOS_HIMALLOCTCACHE_H

#include <pthread.h>
#include <stdlib.h>
#include "pthread_impl.h"
#include "himalloc_inner.h"
#include "vos_himalloctype.h"
#include "vos_himallocblock.h"
#include "vos_himallocslice.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

/* 线程缓存开启阈值，小块内存申请达此阈值才开启线程缓存功能 */
#define HIMALLOC_OPEN_TCACHE_THRESHOLD 256

/* 触发GC回收的计数阈值 */
#define HIMALLOC_GC_CHECK_CYCLE 8192

/* Tcache在特定size class缓存的最小值 */
#define VOS_HIMALLOC_TCACHE_DEFAULT_MIN 4

/* Tcache在特定size class缓存的最大值 */
#define VOS_HIMALLOC_TCACHE_DEFAULT_MAX 16

#define VOS_HIMALLOC_TCACHE_ARRAY_LENGTH 1

/* Cache管理内存申请释放的函数原型 */
typedef void *(*VOS_HIAMALLOC_TCACHE_MANAGE_ALLOC_HOOK)(size_t uvSize);
typedef void (*VOS_HIAMALLOC_TCACHE_MANAGE_FREE_HOOK)(void *pAddr);

/* 指定slice缓存的内存信息 */
typedef struct tagHiMallocTcacheSliceInfo {
    uint16_t usMaxCached; /* 当前SIZE类最大缓存的内存数量 */
} VOS_HIMALLOC_TCACHE_SLICE_INFO;

/* 线程缓存的缓存控制块 */
typedef struct tagHiMallocCacheNode {
    uint32_t uiGcCount;
    uint32_t uiReserved;
    VOS_HIMALLOC_TLB_INFO_S *pstTlb;                                   /* 指向Thread Local结构的tlb */
    uint8_t ucFillDividendLg[VOS_HIMALLOC_MAX_SMALL_INDEX_LG_ALIGN]; /* 向中心区填充的被除数系数 */
    CSTL_LIST_HEAD_S stLinkNode;                                     /* 用于挂解到arena的tcahce控制块 */
    VOS_HIMALLOC_TCACHE_SLICE astMemCached[0];                       /* 指定SIZE缓存的内存 */
} VOS_HIMALLOC_TCACHE_NODE_S;

typedef struct tagHiMallocThreadCache {
    VOS_HIMALLOC_TLB_INFO_S stThreadTlb;
    VOS_HIMALLOC_TCACHE_NODE_S *apstTcacheNodeArray[0];
} VOS_HIMALLOC_TCACHE_S;

typedef struct tagVosMemTcacheHook {
    VOS_HIAMALLOC_TCACHE_MANAGE_ALLOC_HOOK pfnAlloc;
    VOS_HIAMALLOC_TCACHE_MANAGE_FREE_HOOK pfnFree;
} VOS_HIMALLOC_TCACHE_MANAGE_HOOK;

extern HIDDEN void *VosHimallocAllocHard(VOS_HIMALLOC_ARENA_S *pstArena, size_t uvSize);

extern HIDDEN uint32_t VosHimallocFreeHard(VOS_HIMALLOC_ARENA_S *pstArena, void *pAddr);

extern HIDDEN VOS_HIMALLOC_TCACHE_S *VosHimallocTcacheLocalHeapCreate(uint8_t ucTcacheType);

extern HIDDEN void VosHimallocTcacheGcFluch(VOS_HIMALLOC_ARENA_S *pstArena, VOS_HIMALLOC_TCACHE_NODE_S *pstCacheNode);

extern HIDDEN VOS_HIMALLOC_TCACHE_NODE_S *VosHimallocTcacheCacheNodeCreat(VOS_HIMALLOC_TCACHE_S *pstHimTcache,
    uint8_t ucArenaId);

extern HIDDEN uint32_t VosHimallocTcacheEnable(uint8_t ucArenaId);

extern HIDDEN bool VosHimallocTcacheIsEnable(uint8_t ucArenaId);

extern HIDDEN uint16_t VosHimallocTcacheLeftCachedNumGet(VOS_HIMALLOC_ARENA_S *pstArena, uint32_t uiSizeClass);

extern HIDDEN uint32_t VosHimallocTcacheModuleInit(void);

extern HIDDEN bool VosHimallocTcacheAddrIsCached(const void *pAddr, uint8_t ucSizeClass, uint8_t ucArenaId);

extern HIDDEN void VosHimallocThreadDestroy(long long tcacheAddr);

static HIMALLOC_ALWAYS_INLINE VOS_HIMALLOC_TCACHE_NODE_S *VosHimallocTcacheNodeGetImp(
    VOS_HIMALLOC_TCACHE_S *pstTcache, uint8_t ucArenaId)
{
    VOS_HIMALLOC_TCACHE_NODE_S *pstCacheNode;

    pstCacheNode = pstTcache->apstTcacheNodeArray[ucArenaId];
    if (HIMALLOC_UNLIKELY(pstCacheNode == HIMALLOC_NULL_PTR)) {
        pstCacheNode = VosHimallocTcacheCacheNodeCreat(pstTcache, ucArenaId);
    }

    return pstCacheNode;
}

static HIMALLOC_ALWAYS_INLINE VOS_HIMALLOC_TCACHE_S *VosHimallocTcacheGet(uint8_t ucTcacheType)
{
    uintptr_t uiCount;

    uiCount = (uintptr_t)__pthread_self()->himalloc_tcache_addr;
    if (uiCount <= HIMALLOC_OPEN_TCACHE_THRESHOLD) {
        return NULL;
    }

    return ulong_to_ptr(uiCount, VOS_HIMALLOC_TCACHE_S);
}

static HIMALLOC_ALWAYS_INLINE VOS_HIMALLOC_TCACHE_S *VosHimallocTcacheGetInAlloc(uint8_t ucTcacheType)
{
    uintptr_t uiCount;

    uiCount = (uintptr_t)__pthread_self()->himalloc_tcache_addr;
    if (uiCount < HIMALLOC_OPEN_TCACHE_THRESHOLD) {
        __pthread_self()->himalloc_tcache_addr += 1LL;
        return NULL;
    }

    if (HIMALLOC_UNLIKELY(uiCount == HIMALLOC_OPEN_TCACHE_THRESHOLD)) {
        return VosHimallocTcacheLocalHeapCreate(ucTcacheType);
    }

    return ulong_to_ptr(uiCount, VOS_HIMALLOC_TCACHE_S);
}

static HIMALLOC_ALWAYS_INLINE VOS_HIMALLOC_TCACHE_NODE_S *VosHimallocTcacheNodeGet(uint8_t ucArenaId)
{
    VOS_HIMALLOC_TCACHE_S *pstTcache;

    pstTcache = VosHimallocTcacheGet(VOS_HIMALLOC_TCACHE_OPEN);
    if (HIMALLOC_UNLIKELY(pstTcache == HIMALLOC_NULL_PTR)) {
        return HIMALLOC_NULL_PTR;
    }

    return VosHimallocTcacheNodeGetImp(pstTcache, ucArenaId);
}

static HIMALLOC_ALWAYS_INLINE VOS_HIMALLOC_TCACHE_NODE_S *VosHimallocTcacheNodeGetInAlloc(uint8_t ucArenaId)
{
    VOS_HIMALLOC_TCACHE_S *pstTcache;

    pstTcache = VosHimallocTcacheGetInAlloc(VOS_HIMALLOC_TCACHE_OPEN);
    if (HIMALLOC_UNLIKELY(pstTcache == HIMALLOC_NULL_PTR)) {
        return HIMALLOC_NULL_PTR;
    }

    return VosHimallocTcacheNodeGetImp(pstTcache, ucArenaId);
}

static HIMALLOC_ALWAYS_INLINE VOS_HIMALLOC_TLB_INFO_S *VosHimallocTcacheTlbGet(VOS_HIMALLOC_ARENA_S *pstArena)
{
    uint8_t ucTcacheType = pstArena->ucTcacheType;
    VOS_HIMALLOC_TCACHE_S *pstTcache;

    /* 无论分区缓存是否开启，tlb都是每个arena一个，所以直接从arena成员中获取即可 */
    if (ucTcacheType == (uint8_t)VOS_HIMALLOC_ACACHE_CLOSED) {
        return &(pstArena->stTlb);
    } else {
        pstTcache = VosHimallocTcacheGet(ucTcacheType);
        if (HIMALLOC_UNLIKELY(pstTcache == HIMALLOC_NULL_PTR)) {
            return HIMALLOC_NULL_PTR;
        }
    }

    /* 无论线程缓存是否开启，tlb都是每个线程一个，所以需要获取到thread local结构并拿到其中tlb成员即可 */
    return &(pstTcache->stThreadTlb);
}

/* FreeStack指针指向栈底，栈底位于高地址，并且往低地址grow,每次弹出低地址的栈顶指针
   Remark:每个bin的FreeStack始终是固定的，回收和填充操作只改变栈顶位置 */
static HIMALLOC_ALWAYS_INLINE void *VosHimallocTcacheFreeStackPop(VOS_HIMALLOC_TCACHE_SLICE *pstTcacheSlice)
{
    void *pAddr = VosHimallocSliceTcacheGet(pstTcacheSlice);
    VOS_HIMALLOC_ASSERT(pAddr != HIMALLOC_NULL_PTR);

    if (pstTcacheSlice->usNumCached < pstTcacheSlice->usLowWater) {
        pstTcacheSlice->usLowWater = pstTcacheSlice->usNumCached;
    }

    return pAddr;
}

static HIMALLOC_ALWAYS_INLINE void VosHimallocTcacheDoubleFreeCheck(VOS_HIMALLOC_TCACHE_SLICE *pstTcacheSlice,
    const void *pAddr)
{
#ifdef HIMALLOC_TCACHE_POLICY_LIST // 鸿蒙数通IOT场景下，采用链表缓存策略，空间较小
    VOS_HIMALLOC_TCACHE_FREE_SLICE_CB *pTcacheSlice = pstTcacheSlice->pFreeStackList;

    while (pTcacheSlice != NULL) {
        if (pAddr == (void *)pTcacheSlice) {
            abort(); /* 缓存池中已存在该地址，认为是double free，在MALLOC_CHECK_功能打开的情况下abort */
        }
        pTcacheSlice = pTcacheSlice->pstTcacheNextSlice;
    }
#else
    uint16_t numCached = pstTcacheSlice->usNumCached;
    void **freeStack = pstTcacheSlice->ppFreeStack;

    for (uint16_t index = 0; index < numCached; index++) {
        if (*(void **)(freeStack - index) == pAddr) {
            abort(); /* 缓存池中已存在该地址，认为是double free，在MALLOC_CHECK_功能打开的情况下abort */
        }
    }
#endif
}

static HIMALLOC_ALWAYS_INLINE void VosHimallocTcacheFreeStackPush(VOS_HIMALLOC_TCACHE_SLICE *pstTcacheSlice,
    void *pAddr)
{
    VosHimallocSliceTcacheSet(pstTcacheSlice, pAddr);
}

static HIMALLOC_ALWAYS_INLINE void *VosHimallocTcacheAllocFromCache(VOS_HIMALLOC_ARENA_S *pstArena, uint8_t ucSizeClass,
    VOS_HIMALLOC_TCACHE_NODE_S *pstCacheNode)
{
    uint16_t usNumCached;
    uint32_t uiFillNum;
    void *pFreeTcacheSlice;
    VOS_HIMALLOC_TCACHE_SLICE *pstSliceClassCache;

    pstSliceClassCache = &(pstCacheNode->astMemCached[ucSizeClass]);
    /* 当且仅当bin中没有任何缓存slice时会进行填充操作并且填充数量由全局默认缓存最大值与自动调节的缓冲系数决定  */
    if (pstSliceClassCache->usNumCached == 0) {
        uiFillNum = (uint32_t)(pstSliceClassCache->usMaxCached) >> pstCacheNode->ucFillDividendLg[ucSizeClass];
        usNumCached = (uint16_t)VosHimallocSliceBinFill(pstArena, pstSliceClassCache, uiFillNum, ucSizeClass);
        if (usNumCached == 0) {
            __hmattr_sideff_resolved("no relative side effect");
            return HIMALLOC_NULL_PTR;
        }
    }

    pFreeTcacheSlice = VosHimallocTcacheFreeStackPop(pstSliceClassCache);

    pstCacheNode->uiGcCount++;
    if (pstCacheNode->uiGcCount >= HIMALLOC_GC_CHECK_CYCLE) {
        VosHimallocTcacheGcFluch(pstArena, pstCacheNode);
    }

    return (void *)pFreeTcacheSlice;
}

static HIMALLOC_ALWAYS_INLINE void *VosHimallocTcacheAllocImpl(
    uint8_t ucArenaId, VOS_HIMALLOC_ARENA_S *pstArena, size_t uvSize)
{
    uint8_t ucSizeClass;
    VOS_HIMALLOC_TCACHE_NODE_S *pstCacheNode;

    pstCacheNode = VosHimallocTcacheNodeGetInAlloc(ucArenaId);
    if (pstCacheNode != HIMALLOC_NULL_PTR) {
        ucSizeClass = (uint8_t)VosHimallocSize2Index(uvSize);
        return VosHimallocTcacheAllocFromCache(pstArena, ucSizeClass, pstCacheNode);
    }

    return  VosHimallocAllocHard(pstArena, uvSize);
}

static HIMALLOC_ALWAYS_INLINE void VosHimallocTcacheFreetoCache(VOS_HIMALLOC_ARENA_S *pstArena,
    VOS_HIMALLOC_TCACHE_NODE_S *pstCacheNode, void *pAddr, uint8_t ucSizeClass)
{
    uint32_t uiShrinkNum;
    VOS_HIMALLOC_TCACHE_SLICE *pstTcacheSlice;

    pstTcacheSlice = &(pstCacheNode->astMemCached[ucSizeClass]);

    if (HIMALLOC_UNLIKELY(g_himalloc_enable_mcheck)) {
        VosHimallocTcacheDoubleFreeCheck(pstTcacheSlice, pAddr);
    }

    /* tcache内存释放 */
    VosHimallocTcacheFreeStackPush(pstTcacheSlice, pAddr);

    /* 回收策略大于对应bin中设置缓存数量最大值，则对本分箱进行主动回收 */
    if (pstTcacheSlice->usNumCached >= pstTcacheSlice->usMaxCached) {
        /* 回收数量为缓存上限的一半 */
        uiShrinkNum = (uint32_t)(pstTcacheSlice->usMaxCached) >> 1;
        /* 传入的内存已经push成功，flush是否成功都不影响free的结果 */
        (void)VosHimallocSliceBinFlush(pstArena, pstTcacheSlice, pstCacheNode->pstTlb, uiShrinkNum, ucSizeClass);
    }

    /* GC刷新 */
    pstCacheNode->uiGcCount++;
    if (pstCacheNode->uiGcCount >= HIMALLOC_GC_CHECK_CYCLE) {
        VosHimallocTcacheGcFluch(pstArena, pstCacheNode);
    }
}

static HIMALLOC_ALWAYS_INLINE void VosHimallocInvalidFreeCheck(VOS_HIMALLOC_BLOCK *pstBlock, const void *pAddr)
{
    uint16_t usSliceIndex;
    VOS_HIMALLOC_SLICE_INFO *pstSliceInfo;

    pstSliceInfo = VosHimallocSliceInfoGet(pstBlock->ucSizeClass);
    usSliceIndex = VosHimallocSliceIndexGet(pstBlock, &(pstSliceInfo->stDivInfo), pAddr);

    if (HIMALLOC_UNLIKELY(VosHimallocSliceAddrOffset(pstBlock, usSliceIndex) != pAddr)) {
        abort(); /* 该地址与slice分割的地址不同，认为是非法地址，在MALLOC_CHECK_打开的情况下abort */
    }
}


static HIMALLOC_ALWAYS_INLINE uint32_t VosHimallocTcacheFreeImpl(
    uint8_t ucArenaId, VOS_HIMALLOC_ARENA_S *pstArena, void *pAddr)
{
    VOS_HIMALLOC_BLOCK *pstBlock;
    VOS_HIMALLOC_TCACHE_NODE_S *pstCacheNode;

    pstCacheNode = VosHimallocTcacheNodeGet(ucArenaId);
    if (pstCacheNode != HIMALLOC_NULL_PTR) {
        pstBlock = VosHimallocBlockGetImp(pstCacheNode->pstTlb, pAddr);
        if (pstBlock == HIMALLOC_NULL_PTR) {
            __hmattr_sideff_resolved("no relative side effect");
            if (g_himalloc_enable_mcheck) {
                abort(); /* 获取pstBlock为空，认为是double free，在MALLOC_CHECK_打开的情况下abort */
            }

            return (uint32_t)VOS_HIMALLOC_FREE_FAILED_E;
        }

        if (pstBlock->ucSliced == true) {
            if (HIMALLOC_UNLIKELY(g_himalloc_enable_mcheck)) {
                VosHimallocInvalidFreeCheck(pstBlock, pAddr);
            }

            VosHimallocTcacheFreetoCache(pstArena, pstCacheNode, pAddr, pstBlock->ucSizeClass);
            return HIMALLOC_OK;
        } else {
            if (HIMALLOC_UNLIKELY(g_himalloc_enable_mcheck)) {
                if (pstBlock->pStart != pAddr) {
                    abort(); /* 该地址跟对应的block起始地址不同，认为是非法地址，在MALLOC_CHECK_打开的情况下abort */
                }
            }

            return VosHimallocBlockFree(pstArena, pstBlock);
        }
    }

    return VosHimallocFreeHard(pstArena, pAddr);
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */

#endif
