/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include "himalloc.h"
#include "allocator.h"
#include "himalloc_inner.h"
#include "vos_himallocsclass.h"
#include "vos_himallocblock.h"
#include "vos_himallocarena.h"
#include "vos_himalloctcache.h"
#include "vos_himallocgrow.h"
#include "vos_memdecay.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

static int32_t g_siHimInitState = HIM_MODULE_INIT_STATE_UNINIT;

#define HIMALLOC_SYS_PARTITION 0

/* 参考posix中的ptrdiff_t'类型 */
#if defined(__WORDSIZE) && (__WORDSIZE == 64)
#define HIMALLOC_PTRDIFF_MAX_SIZE (0x7FFFFFFFFFFFFFFFu)
#else
#define HIMALLOC_PTRDIFF_MAX_SIZE (0x7FFFFFFFu)
#endif

HIDDEN uint32_t VosHimallocModuleInit(void)
{
    uint32_t uiRet = HIMALLOC_OK;

    uiRet = VosMetaInit();
    if (uiRet == HIMALLOC_OK) {
        uiRet = VosHimallocBaseInit();
    }

    if (uiRet == HIMALLOC_OK) {
        uiRet = VosHimallocGrowModuleInit();
    }

    if (uiRet == HIMALLOC_OK) {
        uiRet = VosHimallocSliceModuleInit();
    }

    if (uiRet == HIMALLOC_OK) {
        uiRet = VosHimallocTlbModleInit();
    }

    if (uiRet == HIMALLOC_OK) {
        uiRet = VosHimallocArenaModuleInit();
    }

    if (uiRet == HIMALLOC_OK) {
        uiRet = VosHimallocTcacheModuleInit();
    }

    if (uiRet == HIMALLOC_OK) {
        uiRet = VosMemDecayModuleInit();
    }

    return uiRet;
}

static HIMALLOC_ALWAYS_INLINE void *VosHimallocLargeAlloc(VOS_HIMALLOC_ARENA_S *pstArena, size_t *puvSize)
{
    VOS_HIMALLOC_BLOCK *pstBlock;

    pstBlock = VosHimallocBlockAlloc(pstArena, *puvSize);
    if (pstBlock == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }

    *puvSize = pstBlock->uvLength;
    return pstBlock->pStart;
}

static HIMALLOC_ALWAYS_INLINE void *VosHimallocSliceAlloc(VOS_HIMALLOC_ARENA_S *pstArena, size_t *puvSize)
{
    uint16_t usBinIndex;
    void *pAllocedAddr;

    usBinIndex = VosHimallocSize2Index(*puvSize);
    VOS_HIMALLOC_ASSERT(usBinIndex < VOS_HIMALLOC_MAX_SMALL_INDEX);

    pAllocedAddr = VosHimallocSliceBinAlloc(pstArena, usBinIndex);
    if (pAllocedAddr != HIMALLOC_NULL_PTR) {
        *puvSize = VosHimallocIndex2Size(usBinIndex);
    }
    return pAllocedAddr;
}

static HIMALLOC_ALWAYS_INLINE void *VosHimallocAllocHardImpl(VOS_HIMALLOC_ARENA_S *pstArena, size_t *puvSize)
{
    void *pAllocedAddr;
    if (*puvSize <= pstArena->usMaxSliceSize) {
        pAllocedAddr = VosHimallocSliceAlloc(pstArena, puvSize);
    } else {
        pAllocedAddr = VosHimallocLargeAlloc(pstArena, puvSize);
    }
    return pAllocedAddr;
}

static HIMALLOC_ALWAYS_INLINE uint32_t VosHimallocFreeHardImpl(
    VOS_HIMALLOC_ARENA_S *pstArena, VOS_HIMALLOC_BLOCK *pstBlock, const void *pAddr, size_t *puvSize)
{
    uint32_t uiResult;

    if (pstArena->ucArenaId != pstBlock->ucArenaId) {
        __hmattr_sideff_resolved("no relative side effect");
        return (uint32_t)VOS_HIMALLOC_FREE_FAILED_E;
    }

    if (pstBlock->ucSliced == true) {
        *puvSize = VosHimallocIndex2Size(pstBlock->ucSizeClass);
        uiResult = VosHimallocSliceFree(pstArena, pstBlock, pAddr);
    } else {
        if (HIMALLOC_UNLIKELY(g_himalloc_enable_mcheck)) {
            if (pstBlock->pStart != pAddr) {
                abort(); /* 该地址跟对应的block起始地址不同，认为是非法地址，在MALLOC_CHECK_打开的情况下abort */
            }
        }

        *puvSize = pstBlock->uvLength;
        uiResult = VosHimallocBlockFree(pstArena, pstBlock);
    }

    return uiResult;
}

HIDDEN uint32_t VosHimallocAddrSizeGet(void *pAddr, uint32_t *puiType, size_t *puvSize)
{
    uint32_t uiType;
    size_t uvSize;
    VOS_HIMALLOC_BLOCK *pstBlock = HIMALLOC_NULL_PTR;
    VOS_HIMALLOC_BLOCK_HEAP_S *pstBlockHeap;
    VOS_HIMALLOC_ARENA_S *pstArena;
    VOS_MEM_AREA_REGION_S stRegion;

    /* 获取地址所在的扩展块，确定地址所在的分区, region添加时可以保证分区号合法 */
    if (VosMemAreaRegionGet(g_HimallocAreaId, ptr_to_ulong(pAddr), &stRegion) != HIMALLOC_OK) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERR;
    }

    /* 允许存在手动扩展的内存未收缩时分区删除，因此在可以获取到扩展块的情况下，不能保证分区存在 */
    pstArena = VosHimallocArenaGet((uint8_t)stRegion.uiArena);
    if (pstArena == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_INVAL;
    }

    pstBlockHeap = &(pstArena->stBlockHeap);
    (void)VosHimallocLock(&(pstBlockHeap->stTreeBinLock));

    pstBlock = VosHimallocBlockGetImp(&(pstArena->stTlb), pAddr);
    if (pstBlock == HIMALLOC_NULL_PTR) {
        VosHimallocUnLock(&(pstBlockHeap->stTreeBinLock));
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_INVAL;
    }

    if (pstBlock->ucSliced == true) {
        uiType = pstBlock->ucSizeClass;
        uvSize = VosHimallocIndex2Size((uint16_t)uiType);
        if (uvSize == 0) {
            VosHimallocUnLock(&(pstBlockHeap->stTreeBinLock));
            __hmattr_sideff_resolved("no relative side effect");
            return HIMALLOC_ERRNO_INVAL;
        }
        if ((ptr_to_ulong(pAddr) - (uintptr_t)pstBlock->pStart) % uvSize != 0) {
            VosHimallocUnLock(&(pstBlockHeap->stTreeBinLock));
            __hmattr_sideff_resolved("no relative side effect");
            return HIMALLOC_ERRNO_INVAL;
        }
        *puiType = uiType;
        *puvSize = uvSize;
    } else {
        if (pAddr != pstBlock->pStart) {
            VosHimallocUnLock(&(pstBlockHeap->stTreeBinLock));
            __hmattr_sideff_resolved("no relative side effect");
            return HIMALLOC_ERRNO_INVAL;
        }
        *puiType = VOS_HIMALLOC_MAX_SMALL_INDEX;
        *puvSize = pstBlock->uvLength;
    }

    VosHimallocUnLock(&(pstBlockHeap->stTreeBinLock));
    return HIMALLOC_OK;
}

HIDDEN uint32_t VosHimallocAllocUsableSizeGet(void *pAddr, size_t *puvSize)
{
    uint32_t tmpType;
    uint32_t uiRet = HIMALLOC_OK;
    int oldState, oldType;

    if ((pAddr == NULL) || (puvSize == NULL)) {
        return HIMALLOC_ERR;
    }

    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &oldType);
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldState);

    if (VosHimallocAddrSizeGet(pAddr, &tmpType, puvSize) != HIMALLOC_OK) {
        *puvSize = 0;
        uiRet = HIMALLOC_ERR;
    }

    pthread_setcancelstate(oldState, NULL);
    pthread_setcanceltype(oldType, NULL);

    return uiRet;
}

static HimallocMemParseFunc g_pfnHimallocParseHook = HIMALLOC_NULL_PTR;

static HIMALLOC_ALWAYS_INLINE void VosHimallocMemWalkHookReg(HimallocMemParseFunc pfnParseHookToReg)
{
    g_pfnHimallocParseHook = pfnParseHookToReg;
}

static HIMALLOC_ALWAYS_INLINE void VosHimallocMemWalkHookUnReg(void)
{
    g_pfnHimallocParseHook = HIMALLOC_NULL_PTR;
}

static bool g_bHimallocSliceBitmapWalk = false;

static uint32_t VosHimallocMemWalkHook(const VOS_MEM_ZONE_T pZone, void *pPara, void *pSlice)
{
    int32_t siRet = HIMALLOC_OK;
    VOS_HIMALLOC_BLOCK *pstBlock = (VOS_HIMALLOC_BLOCK *)pSlice;
    HimallocMemParseFunc pfnHimallocParseHook = g_pfnHimallocParseHook;

    (void)pZone;

    if (pstBlock->ucSliced == 0U) {
        if (pstBlock->ucBlockState == (uint8_t)VOS_HIMALLOC_BLOCK_STAT_ALLOCAED) {
            siRet = pfnHimallocParseHook(pstBlock->pStart, (size_t)pstBlock->uvLength, pPara);
        }
    } else {
        if (g_bHimallocSliceBitmapWalk == false) {
            siRet = VosHimallocUsedSliceTraverse(pstBlock, pfnHimallocParseHook, pPara);
        } else {
            siRet = VosHimallocUsedSliceBitmapTraverse(pstBlock, pfnHimallocParseHook, pPara);
        }
    }

    return (uint32_t)siRet;
}

static HIMALLOC_ALWAYS_INLINE void VosHimallocSliceBitmapWalkReg(bool bIsSliceBitmap)
{
	g_bHimallocSliceBitmapWalk = bIsSliceBitmap;
}

static HIMALLOC_ALWAYS_INLINE void VosHimallocSliceBitmapWalkUnReg(void)
{
	g_bHimallocSliceBitmapWalk = false;
}

/* 性能较差待优化 */
HIDDEN int32_t VosHimallocAllMemWalk(void *pPara, HimallocMemParseFunc pfnParseHook, bool bIsSliceBitmap)
{
    VOS_HIMALLOC_ARENA_S *pstArena;
    uint32_t uiRet;

    pstArena = VosHimallocArenaGet(HIMALLOC_SYS_PARTITION);
    if ((pstArena == HIMALLOC_NULL_PTR) || (pfnParseHook == HIMALLOC_NULL_PTR)) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_INVAL;
    }
    /* 暂时不做加锁，不支持并发调用 */
    VosHimallocMemWalkHookReg(pfnParseHook);
    VosHimallocSliceBitmapWalkReg(bIsSliceBitmap);

    uiRet = VosMemZoneWalk(pstArena->zoneBlock, pPara, (VOS_MEM_ZONE_WALK_HOOK)VosHimallocMemWalkHook);

    VosHimallocSliceBitmapWalkUnReg();
    VosHimallocMemWalkHookUnReg();
    if (uiRet != HIMALLOC_OK) {
        VosHimallocLogWrite(uiRet, "[HIMALLOC-%s]:Mem walk failed", __FUNCTION__);
    }

    return (int32_t)uiRet;
}

static uint32_t VosHimallocInit(void)
{
    uint32_t uiRet;
    int32_t siOldState;
    bool bSucceed = false;

    do {
        siOldState = __sync_fetch_and_add(&g_siHimInitState, 0);
        if (siOldState == (int32_t)HIM_MODULE_INIT_STATE_INITED) {
            __hmattr_sideff_resolved("no relative side effect");
            return HIMALLOC_OK;
        }
        if (siOldState == (int32_t)HIM_MODULE_INIT_STATE_ININIT) {
            bSucceed = false;
            (void)sched_yield();
            continue;
        }

        bSucceed = __sync_bool_compare_and_swap(&g_siHimInitState, siOldState, (int32_t)HIM_MODULE_INIT_STATE_ININIT);
    } while (!bSucceed);

    uiRet = VosHimallocArenaInit(HIMALLOC_SYS_PARTITION);
    if (uiRet != HIMALLOC_OK) {
        __hmattr_sideff_resolved("no relative side effect");
        return uiRet;
    }
    (void)VosHimallocSliceMaxSizeSet(HIMALLOC_SYS_PARTITION, &g_uiHimallocSliceSize);

    do {
        siOldState = __sync_fetch_and_add(&g_siHimInitState, 0);
    } while ((__sync_val_compare_and_swap(&g_siHimInitState, siOldState,
        (int32_t)HIM_MODULE_INIT_STATE_INITED)) != siOldState);

    return HIMALLOC_OK;
}

HIDDEN void *VosHimallocAllocHard(VOS_HIMALLOC_ARENA_S *pstArena, size_t uvSize)
{
    /* 作用一，防止size翻转，可能对VosHimallocAlloc接口传入负数；
       作用二，遵循posix原则，内存分配器不应该申请出size大于PTRDIFF_MAX的内存，防止出现point subtraction错误 */
    if (uvSize > HIMALLOC_PTRDIFF_MAX_SIZE) {
        errno = ENOMEM;
        return HIMALLOC_NULL_PTR;
    }

    return VosHimallocAllocHardImpl(pstArena, &uvSize);
}

HIDDEN void *VosHimallocAlloc(size_t size)
{
    size_t tmpSize = size;
    VOS_HIMALLOC_ARENA_S *pstArena;

    void* ret = NULL;
    int oldState, oldType;

    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &oldType);
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldState);

    if (HIMALLOC_UNLIKELY(g_siHimInitState != (int32_t)HIM_MODULE_INIT_STATE_INITED)) {
        (void)VosHimallocInit();
    }

    if (HIMALLOC_UNLIKELY(tmpSize == 0)) {
        tmpSize = 1;
    }

    pstArena = VosHimallocArenaGet(HIMALLOC_SYS_PARTITION);
    if ((pstArena->ucTcacheType != (uint8_t)VOS_HIMALLOC_TCACHE_CLOSED) && (tmpSize <= pstArena->usMaxSliceSize)) {
        ret = VosHimallocTcacheAllocImpl(HIMALLOC_SYS_PARTITION, pstArena, tmpSize);
    } else {
        ret = VosHimallocAllocHard(pstArena, tmpSize);
    }

    pthread_setcancelstate(oldState, NULL);
    pthread_setcanceltype(oldType, NULL);

    return ret;
}

static HIMALLOC_ALWAYS_INLINE VOS_HIMALLOC_BLOCK *VosHimallocBlockGet(VOS_HIMALLOC_ARENA_S *pstArena, void *pAddr)
{
    VOS_HIMALLOC_TLB_INFO_S *pstTlbInfo;
    VOS_HIMALLOC_BLOCK *pstBlock;

    pstTlbInfo = VosHimallocTcacheTlbGet(pstArena);
    /* 如果获取不到thread local的tlb结构则直接从radix tree中获取 */
    if (HIMALLOC_UNLIKELY(pstTlbInfo == HIMALLOC_NULL_PTR)) {
        pstBlock = VosHimallocTlbRtreeGet((size_t)((uintptr_t)pAddr));
    } else {
        pstBlock = VosHimallocBlockGetImp(pstTlbInfo, pAddr);
    }

    return pstBlock;
}

HIDDEN uint32_t VosHimallocFreeHard(VOS_HIMALLOC_ARENA_S *pstArena, void *pAddr)
{
    size_t uvSize = 0;
    VOS_HIMALLOC_BLOCK *pstBlock;

    /* 在FreeHardImpl接口中进行空指针判断 */
    pstBlock = VosHimallocBlockGet(pstArena, pAddr);
    if (pstBlock == HIMALLOC_NULL_PTR) {
        if (g_himalloc_enable_mcheck) {
            abort(); /* 该地址获取的pstBlock为空，认为是double free了，在MALLOC_CHECK_打开的情况下abort */
        }

        return (uint32_t)VOS_HIMALLOC_FREE_REF_ZERO_E;
    }

    return VosHimallocFreeHardImpl(pstArena, pstBlock, pAddr, &uvSize);
}

HIDDEN void VosHimallocFree(void *mem)
{
    VOS_HIMALLOC_ARENA_S *pstArena;
    int oldState, oldType;

    if (HIMALLOC_UNLIKELY(mem == NULL)) {
        return;
    }

    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &oldType);
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldState);

    pstArena = VosHimallocArenaGet(HIMALLOC_SYS_PARTITION);
    if (pstArena->ucTcacheType != (uint8_t)VOS_HIMALLOC_TCACHE_CLOSED) {
        (void)VosHimallocTcacheFreeImpl(HIMALLOC_SYS_PARTITION, pstArena, mem);
    } else {
        (void)VosHimallocFreeHard(pstArena, mem);
    }

    pthread_setcancelstate(oldState, NULL);
    pthread_setcanceltype(oldType, NULL);

    return;
}
HIDDEN void *VosHimallocAllocAlign(size_t uvSize, size_t uvAlign)
{
    VOS_HIMALLOC_ARENA_S *pstArena;
    uint16_t usBinIndex;
    size_t uvBinSize;
    size_t uvAlignTmp = uvAlign;
    void* ret = NULL;
    int oldState, oldType;

    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &oldType);
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldState);

    if (HIMALLOC_UNLIKELY(g_siHimInitState != (int32_t)HIM_MODULE_INIT_STATE_INITED)) {
        (void)VosHimallocInit();
    }

    pstArena = VosHimallocArenaGet(HIMALLOC_SYS_PARTITION);

    if (uvSize <= VOS_HIMALLOC_VPAGE_SIZE_HALF) {
        usBinIndex = VosHimallocSize2Index(uvSize);
        uvBinSize = VosHimallocIndex2Size(usBinIndex);
        /* 接口外部调用时保证uvAlign为2的指数次方 */
        if (uvAlignTmp < VOS_HIMALLOC_MIN_SIZE) {
            uvAlignTmp = VOS_HIMALLOC_MIN_SIZE;
        }
        if (uvBinSize % uvAlignTmp == 0) {
            __hmattr_sideff_resolved("no relative side effect");
            ret = VosHimallocSliceAlloc(pstArena, &uvBinSize);

            pthread_setcancelstate(oldState, NULL);
            pthread_setcanceltype(oldType, NULL);

            return ret;
        }
        if (uvAlignTmp <= VOS_HIMALLOC_VPAGE_SIZE_HALF) {
            while (uvAlignTmp < uvBinSize) {
                uvAlignTmp = (size_t)((uint64_t)uvAlignTmp << 1);
            }
            __hmattr_sideff_resolved("no relative side effect");
            ret = VosHimallocSliceAlloc(pstArena, &uvAlignTmp);

            pthread_setcancelstate(oldState, NULL);
            pthread_setcanceltype(oldType, NULL);

            return ret;
        }
    }

    ret = VosHimallocBlockAllocAlign(pstArena, uvSize, uvAlign);

    pthread_setcancelstate(oldState, NULL);
    pthread_setcanceltype(oldType, NULL);

    return ret;
}

int VosHimallocMallocTrim(size_t uvSize)
{
	VOS_HIMALLOC_ARENA_S *pstArena = VosHimallocArenaGet(HIMALLOC_SYS_PARTITION);

	(void)uvSize;
	VosHimallocBlockHeapCleanSetUp(pstArena, false);
	return 1;
}

/* release 版本 */
#ifndef CONFIG_DEBUG_BUILD

/* 正常版本下，为VosHimallocAlloc函数起个别名为malloc */
WEAK_ALIAS(VosHimallocAlloc, malloc);

/* 正常版本下，为MemFree函数起个别名为free */
WEAK_ALIAS(VosHimallocFree, free);

#endif

/* Alias a another name for malloc_trim */
WEAK_ALIAS(VosHimallocMallocTrim, malloc_trim);

#ifdef __cplusplus
}
#endif /* __cpluscplus */
