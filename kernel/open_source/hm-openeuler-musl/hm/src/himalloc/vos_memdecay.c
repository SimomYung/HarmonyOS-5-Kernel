/*
 * @file vos_memdecay.c
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: DOPRA SmoothStep Decay algorithm
 * Author: Anonym
 * Create: 2021/2/25
 */
#include <time.h>
#include <sys/mman.h>
#include "vos_metaalloc.h"
#include "vos_memdecay.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

#define VOS_MEM_DECOMMIT_CHECK_SIZE 4096

/* 内存老化模块，默认打开 */
/* 内存老化回收周期(单位为纳秒) */
HIDDEN uint64_t g_ullVosDecayPeriodicTimeNs;

/* 建立SmootherFactorTabel,查表获取每个步长对应的权重因子而不是每次都去计算 */
HIDDEN uint32_t *g_auiVosSmoothFactorTable;

/* Smoother公式如下所示
 * float smoothstep(float edge0, float edge1, float x) {
 *      x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
 *     return x * x * (3 - 2 * x);
 * }
 * 为了规避浮点数运算，对x进行扩大一定倍数，令edge0=0 edge=N.其中N为步长
 * 则有Smoothstep_y = (x * x * (3* N - 2)) / (N * N * N),设置步长要求为2的幂次假设N = 2 ^ k
 * 则有Smoothstep_y = (x * x * (3* 2 ^ k - 2)) / (2 ^ (3 * k))
 * 令StepHeight = (x * x * (3* 2 ^ k - 2))以及wightFactor = StepHeight / (2 ^ k) = StepHeight >> k
 * 则有wightFactor = StepHeight / (2 ^ k) = Smoothstep_y * (2 ^ (3 * k)) / (2 ^ k) = Smoothstep_y * (2 ^ (2 * k))
 * 则计算加权和时x * Smoothstep_y = x * wightFactor / (2 ^ (2 * k)) = (x * wightFactor) >> (2 * k)
 */
HIDDEN uint32_t VosMemDecayStepHeightCal(uint32_t uiStepNum, uint32_t uiDecaySteps)
{
    uint32_t uiStepHeight;

    /* 3和2是SmoothStep参数，具体含义见上放 */
    uiStepHeight = uiStepNum * uiStepNum * (3 * uiDecaySteps - 2 * uiStepNum);
    uiStepHeight >>= VOS_MEM_DECAY_STEPS_LG;

    return uiStepHeight;
}

HIDDEN uint32_t VosMemDecayFactorTableCreate(void)
{
    uint32_t uiStepNum;
    uint32_t uiStepHeight;
    const uint32_t uiDecaySteps = VOS_MEM_DECAY_STEPS;

    g_auiVosSmoothFactorTable = (uint32_t *)VosMetaAlloc(uiDecaySteps * sizeof(uint32_t));
    if (g_auiVosSmoothFactorTable == NULL) {
        return HIMALLOC_ERRNO_NOMEM;
    }

    for (uiStepNum = 1; uiStepNum <= uiDecaySteps; uiStepNum++) {
        uiStepHeight = VosMemDecayStepHeightCal(uiStepNum, uiDecaySteps);
        g_auiVosSmoothFactorTable[(uiStepNum - 1)] = uiStepHeight;
    }

    return HIMALLOC_OK;
}

/* 利用smootherstep计算此次老化回收epoch中需要保留的数量 */
HIDDEN size_t VosMemDecayReserveCal(VOS_MEM_DECAY_INFO_S *pstDecayInfo)
{
    uint32_t uiInd = 0;
    uint64_t ullSum = 0;
    uint32_t uiCursor = pstDecayInfo->uiCursor;
    const uint32_t uiDecaySteps = VOS_MEM_DECAY_STEPS;
    size_t *auvRetainArray = &(pstDecayInfo->uvRetainArray[0]);

    while (uiCursor < uiDecaySteps) {
        ullSum += auvRetainArray[uiCursor] * (uint64_t)g_auiVosSmoothFactorTable[uiInd];
        uiCursor += 1;
        uiInd += 1;
    }

    uiCursor = 0;
    while (uiInd < uiDecaySteps) {
        ullSum += auvRetainArray[uiCursor] * (uint64_t)g_auiVosSmoothFactorTable[uiInd];
        uiCursor += 1;
        uiInd += 1;
    }

    return (size_t)((ullSum) >> (VOS_MEM_DECAY_STEPS_LG << 1));
}

HIDDEN void VosMemDecayArrayUpdate(VOS_MEM_DECAY_INFO_S *pstDecayInfo, size_t uvCurrent, uint32_t uiAdvanced)
{
    size_t uvLatest;
    uint32_t uiOldCursor;
    uint32_t uiNewCursor;
    const uint32_t uiDecaySteps = VOS_MEM_DECAY_STEPS;
    size_t *auvRetainArray = &(pstDecayInfo->uvRetainArray[0]);

    /* 距离上个回收的epoch已经过去一整个周期了，所以完全重置RetainArray */
    if (uiAdvanced >= uiDecaySteps) {
        uiNewCursor = 0;
        (void)memset(&(auvRetainArray[0]), 0, (sizeof(size_t) * uiDecaySteps));
    } else {
        /* 距离上个回收的epoch已经过去了Advanced个epoch，窗口向左滑动Advanced个位置，即游标向右滑动Advanced个位置 */
        uiOldCursor = pstDecayInfo->uiCursor;
        uiNewCursor = VOS_MEM_DECAY_STEPS_MODULUS((uiOldCursor + uiAdvanced));
        if (uiNewCursor > uiOldCursor) {
            (void)memset(&(auvRetainArray[uiOldCursor]), 0, (sizeof(size_t) * uiAdvanced));
        } else {
            (void)memset(&(auvRetainArray[0]), 0, (sizeof(size_t) * uiNewCursor));
            (void)memset(&(auvRetainArray[uiOldCursor]), 0, (sizeof(size_t) * (uiDecaySteps - uiOldCursor)));
        }
    }

    pstDecayInfo->uiCursor = uiNewCursor;

    uvLatest = (uvCurrent > pstDecayInfo->uvNonDecay) ? (uvCurrent - pstDecayInfo->uvNonDecay) : 0;
    if (uiNewCursor == 0) {
        auvRetainArray[(uiDecaySteps - 1)] = uvLatest;
    } else {
        auvRetainArray[(uiNewCursor - 1)] = uvLatest;
    }
}

/* 获取ns精度的时间 */
static HIMALLOC_ALWAYS_INLINE uint64_t VosMemDecayCurTimeNsGet(void)
{
    struct timespec stTimeSpec;

    (void)clock_gettime(CLOCK_MONOTONIC, &stTimeSpec);
    return (uint64_t)stTimeSpec.tv_sec *
        (uint64_t)VOS_MEM_DECAY_SECONDE_TO_NANOSECOND + (uint64_t)stTimeSpec.tv_nsec;
}

HIDDEN void VosMemDecayEpochTimeNsInit(VOS_MEM_DECAY_INFO_S *pstDecayInfo)
{
    pstDecayInfo->ullEpochStart = VosMemDecayCurTimeNsGet();
    pstDecayInfo->ullEpochEnd = pstDecayInfo->ullEpochStart + pstDecayInfo->ullInterval;
}

HIDDEN bool VosMemDecayNextEpochReach(VOS_MEM_DECAY_INFO_S *pstDecayInfo, uint64_t *pullCurTimeNs)
{
    uint64_t ullCurTimeNs = VosMemDecayCurTimeNsGet();
    /* 获取当前时间与上一次Epoch的起始时间对比，发现出现时间回退，由于获取的是单调时间，所以是s转换为ns时发生了翻转
       需要重新设置EpochStart时间,放弃这次Epoch流程 */
    if (HIMALLOC_UNLIKELY(ullCurTimeNs <= pstDecayInfo->ullEpochStart)) {
        VosMemDecayEpochTimeNsInit(pstDecayInfo);
        return false;
    }

    if (ullCurTimeNs > (pstDecayInfo->ullEpochEnd)) {
        *pullCurTimeNs = ullCurTimeNs;
        return true;
    }

    return false;
}

HIDDEN size_t VosMemDecaySizeGet(VOS_MEM_DECAY_INFO_S *pstDecayInfo, size_t uvCurrentSize)
{
    uint64_t ullAdvanced;
    uint64_t ullDelta;
    uint64_t ullCurTimeNs = 0;
    size_t uvReserve;
    uint32_t uiLockRet;

    uiLockRet = OSAL_LockLock(&(pstDecayInfo->stDecayLock));

    /* 间隔太短未进入下一次回收epoch */
    if (VosMemDecayNextEpochReach(pstDecayInfo, &ullCurTimeNs) == false) {
        OSAL_LockUnLock(&(pstDecayInfo->stDecayLock), uiLockRet);
        return 0;
    }

    ullDelta = ullCurTimeNs - pstDecayInfo->ullEpochStart;
    ullAdvanced = ullDelta / (pstDecayInfo->ullInterval);

    /* 更新这次 epoch 流程的起始时间和终止时间，下次epoch流程的开始必须在此次终止时间之后 */
    pstDecayInfo->ullEpochStart += (ullAdvanced * pstDecayInfo->ullInterval);
    pstDecayInfo->ullEpochEnd = pstDecayInfo->ullEpochStart + pstDecayInfo->ullInterval;

    VosMemDecayArrayUpdate(pstDecayInfo, uvCurrentSize, (uint32_t)ullAdvanced);
    uvReserve = VosMemDecayReserveCal(pstDecayInfo);
    pstDecayInfo->uvNonDecay = uvReserve;

    OSAL_LockUnLock(&(pstDecayInfo->stDecayLock), uiLockRet);

    return (uvCurrentSize > uvReserve) ? (uvCurrentSize - uvReserve) : 0;
}

HIDDEN VOS_MEM_DECAY_INFO_S *VosMemDecayInfoInit(void)
{
    uint32_t uiRet;
    VOS_MEM_DECAY_INFO_S *pstDecayInfo;

    pstDecayInfo = (VOS_MEM_DECAY_INFO_S *)VosMetaAlloc(sizeof(VOS_MEM_DECAY_INFO_S));
    if (pstDecayInfo == NULL) {
        return NULL;
    }
    (void)memset(pstDecayInfo, 0, sizeof(VOS_MEM_DECAY_INFO_S));

    pstDecayInfo->ullInterval = g_ullVosDecayPeriodicTimeNs >> VOS_MEM_DECAY_STEPS_LG;

    uiRet = OSAL_LockCreate(&(pstDecayInfo->stDecayLock));
    if (uiRet != HIMALLOC_OK) {
        (void)VosMetaFree(pstDecayInfo);
        return NULL;
    }

    VosMemDecayEpochTimeNsInit(pstDecayInfo);

    return pstDecayInfo;
}

HIDDEN void VosMemDecayInfoDestory(VOS_MEM_DECAY_INFO_S *pstDecayInfo)
{
    if (pstDecayInfo != NULL) {
        OSAL_LockDestroy(&(pstDecayInfo->stDecayLock));
        (void)VosMetaFree(pstDecayInfo);
    }
}

HIDDEN uint32_t VosMemDecayModuleInit(void)
{
    /* 设置老化回收间隔不能小于1024ms同时不能大于60min */
    const uint64_t ullTimeMs = VOS_MEM_DECAY_DEFAULT_PERIODIC_TIME_MS; /* 此处暂时使用默认值，实际可配 */
    g_ullVosDecayPeriodicTimeNs = ullTimeMs * (uint64_t)VOS_MEM_DECAY_MICROSECONDE_TO_NANOSECOND;
    return VosMemDecayFactorTableCreate();
}

/* 将内存与物理内存页解映射 */
HIDDEN void VosMemDecayClean(void *pAddr, size_t uvSize)
{
    (void)madvise(pAddr, uvSize, MADV_DONTNEED);
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */
