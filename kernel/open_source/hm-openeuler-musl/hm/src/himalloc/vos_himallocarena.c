/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#include <string.h>
#include <pthread.h>
#include "himalloc_inner.h"
#include "osal_lock.h"
#include "vos_himallocbase.h"
#include "vos_metaalloc.h"
#include "vos_memdecay.h"
#include "vos_himallocarena.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

HIDDEN VOS_HIMALLOC_ARENA_CENTER_S g_stHimallocArenaCenter;

static int32_t g_himModuleInitState = HIM_MODULE_INIT_STATE_UNINIT;

HIDDEN uint32_t VosHimallocArenaModuleInit(void)
{
    uint32_t uiRet = HIMALLOC_OK;
    (void)memset(&g_stHimallocArenaCenter, 0, sizeof(VOS_HIMALLOC_ARENA_CENTER_S));
    CSTL_ListInit(&(g_stHimallocArenaCenter.stDeleted));
    uiRet = OSAL_LockCreate(&(g_stHimallocArenaCenter.stLock));
    return uiRet;
}

/* 该接口申请新的Arena区域后只对相应的锁进行初始化，其余结构体保持为未初始化状态，防止重复初始化 */
static VOS_HIMALLOC_ARENA_S *VosHimallocArenaCtlCreate(void)
{
    uint32_t uiRet;
    VOS_HIMALLOC_ARENA_S *pstArena;

    /* 申请资源块 */
    pstArena = (VOS_HIMALLOC_ARENA_S *)VosMetaAlloc(sizeof(VOS_HIMALLOC_ARENA_S));
    if (pstArena == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }
    (void)memset(pstArena, 0, sizeof(VOS_HIMALLOC_ARENA_S));

    /* 初始化基础数据 */
    CSTL_ListInit(&(pstArena->stLinkTcache));
    CSTL_ListInit(&(pstArena->stLinkArenaCtl));

    /* 初始化Tcache锁 */
    uiRet = OSAL_LockCreate(&(pstArena->stTcacheLock));
    if (uiRet != HIMALLOC_OK) {
        (void)VosMetaFree(pstArena);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }

    uiRet = VosHimallocSliceBinsLockInit(VOS_HIMALLOC_SLICE_BINS_LOCK_NUM, pstArena->stSliceHeap.astBinsLock);
    if (uiRet != HIMALLOC_OK) {
        (void)OSAL_LockDestroy(&(pstArena->stTcacheLock));
        (void)VosMetaFree(pstArena);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }

    uiRet = VosHimallocLockCreate(&(pstArena->stBlockHeap.stTreeBinLock));
    if (uiRet != HIMALLOC_OK) {
        (void)VosHimallocSliceBinsLockDestory(VOS_HIMALLOC_SLICE_BINS_LOCK_NUM, pstArena->stSliceHeap.astBinsLock);
        (void)OSAL_LockDestroy(&(pstArena->stTcacheLock));
        (void)VosMetaFree(pstArena);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }

    return pstArena;
}

static VOS_HIMALLOC_ARENA_S *VosHimallocArenaCtlGet(void)
{
    CSTL_LIST_HEAD_S *pstHead;
    VOS_HIMALLOC_ARENA_S *pstArena;
    CSTL_LIST_HEAD_S *pstNodeDele;

    /* 如果存在arena资源则直接复用 */
    pstHead = &(g_stHimallocArenaCenter.stDeleted);
    if (CSTL_ListIsEmpty(pstHead) != true) {
        pstArena = (VOS_HIMALLOC_ARENA_S *)CSTL_LIST_ENTRY(pstHead->next, VOS_HIMALLOC_ARENA_S, stLinkArenaCtl);
        pstNodeDele = pstHead->next;
        CSTL_ListRemove(pstNodeDele);
    } else {
        /* 否则申请新的arena资源块 */
        pstArena = VosHimallocArenaCtlCreate();
    }
    return pstArena;
}

static uint32_t VosHimallocArenaInitIner(uint8_t ucArenaId, VOS_HIMALLOC_ARENA_S *pstArena)
{
    VOS_MEM_ZONE_DECAY_S stDecayTime = { VOS_MEM_ZONE_DECAY_COUNT, VOS_MEM_ZONE_DECAY_TIME_MS };

    /* 初始化基础数据 */
    pstArena->ucArenaId = ucArenaId;
    CSTL_ListInit(&(pstArena->stLinkTcache));

    /*  初始化缓存使能类型 */
    pstArena->ucTcacheType = (uint8_t)VOS_HIMALLOC_TCACHE_CLOSED;

    /* 初始化物理页老化算法管理结构 */
    pstArena->pstDecayInfo = VosMemDecayInfoInit();
    if (pstArena->pstDecayInfo == VOS_MEM_DECAY_INFO_INVALID_T) {
        return HIMALLOC_ERRNO_NOMEM;
    }

    pstArena->ucSaveHostage = true;
    pstArena->siTick = (int16_t)VOS_MEM_DECAY_INITIAL_TICKS;

    /* 初始化Block控制块的zone */
    pstArena->zoneBlock = VosMemZoneInit((uint32_t)sizeof(VOS_HIMALLOC_BLOCK), &stDecayTime);
    if (pstArena->zoneBlock == VOS_MEM_ZONE_INVALID_T) {
        __hmattr_sideff_resolved("no relative side effect");
        VosMemDecayInfoDestory(pstArena->pstDecayInfo);
        pstArena->pstDecayInfo = VOS_MEM_DECAY_INFO_INVALID_T;
        pstArena->ucSaveHostage = false;
        pstArena->siTick = 0;
        return HIMALLOC_ERRNO_NOMEM;
    }

    /* 初始化Slice堆 */
    VosHimallocSliceBinsInit(&(pstArena->stSliceHeap));

    /* 初始化Block堆 */
    VosHimallocBlockHeapCtlInit(&(pstArena->stBlockHeap));

    VosHimallocTlbInit(&(pstArena->stTlb));

    return HIMALLOC_OK;
}

static uint32_t VosHimallocArenaPreInit(void)
{
    uint32_t uiRet;
    int32_t siOldState;
    bool bSucceed = false;

    do {
        siOldState = __sync_fetch_and_add(&g_himModuleInitState, 0);
        if (siOldState == (int32_t)HIM_MODULE_INIT_STATE_INITED) {
            __hmattr_sideff_resolved("no relative side effect");
            return HIMALLOC_OK;
        }
        if (siOldState == (int32_t)HIM_MODULE_INIT_STATE_ININIT) {
            bSucceed = false;
            (void)sched_yield();
            continue;
        }

        bSucceed = __sync_bool_compare_and_swap(&g_himModuleInitState, siOldState,
            (int32_t)HIM_MODULE_INIT_STATE_ININIT);
    } while (!bSucceed);

    uiRet = VosHimallocModuleInit();
    if (uiRet != HIMALLOC_OK) {
        __hmattr_sideff_resolved("no relative side effect");
        return uiRet;
    }

    do {
        siOldState = __sync_fetch_and_add(&g_himModuleInitState, 0);
    } while ((__sync_val_compare_and_swap(&g_himModuleInitState, siOldState,
        (int32_t)HIM_MODULE_INIT_STATE_INITED)) != siOldState);

    return HIMALLOC_OK;
}

HIDDEN uint32_t VosHimallocArenaInit(uint8_t ucArenaId)
{
    uint32_t uiRet;
    uint32_t uiLockValue;
    VOS_HIMALLOC_ARENA_S *pstNewArena;

    if (HIMALLOC_UNLIKELY(g_himModuleInitState != (int32_t)HIM_MODULE_INIT_STATE_INITED)) {
        uiRet = VosHimallocArenaPreInit();
        if (uiRet != HIMALLOC_OK) {
            __hmattr_sideff_resolved("no relative side effect");
            return uiRet;
        }
    }

    uiLockValue = OSAL_LockLock(&(g_stHimallocArenaCenter.stLock));
    if (g_stHimallocArenaCenter.astArenas[ucArenaId] != HIMALLOC_NULL_PTR) {
        OSAL_LockUnLock(&(g_stHimallocArenaCenter.stLock), uiLockValue);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_INVAL;
    }

    pstNewArena = VosHimallocArenaCtlGet();
    if (pstNewArena == HIMALLOC_NULL_PTR) {
        OSAL_LockUnLock(&(g_stHimallocArenaCenter.stLock), uiLockValue);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_NOMEM;
    }

    uiRet = VosHimallocArenaInitIner(ucArenaId, pstNewArena);
    if (uiRet != HIMALLOC_OK) {
        CSTL_ListAdd(&(pstNewArena->stLinkArenaCtl), &(g_stHimallocArenaCenter.stDeleted));
        OSAL_LockUnLock(&(g_stHimallocArenaCenter.stLock), uiLockValue);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_NOMEM;
    }

    g_stHimallocArenaCenter.astArenas[ucArenaId] = pstNewArena;

    (void)VosHimallocSliceMaxSizeSet(ucArenaId, &g_uiHimallocSliceSize);

    OSAL_LockUnLock(&(g_stHimallocArenaCenter.stLock), uiLockValue);

    return HIMALLOC_OK;
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */
