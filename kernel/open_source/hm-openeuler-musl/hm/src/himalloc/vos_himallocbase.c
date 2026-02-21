/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#include <time.h>
#ifndef OS_HONGMENG
#include <unistd.h>
#else
#include <stdlib.h>
#endif
#include "himalloc_algo.h"
#include "vos_himallocbase.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

#define VOS_HIMALLOC_HIGH16_BIT 16

HIDDEN VOS_HIMALLOC_RANDOM_S g_uiHimallocRandom;

/* 定义内存调测相关的统计开关 */
#if defined(VOS_YES) && ((defined(VOS_BUILD_DEBUG) && (VOS_YES == VOS_BUILD_DEBUG)))
static bool g_himallocStatsSwitch = true;
#else
static bool g_himallocStatsSwitch = false;
#endif

/* 鸿蒙启动时不支持部分系统调用，又实际鸿蒙侧用不到随机数模块，直接赋值seed为0 */
#ifndef OS_HONGMENG
static HIMALLOC_ALWAYS_INLINE uint32_t VosHimallocRandomSeedGenerate(const struct timespec *pstTimeSpec)
{
    uint32_t uiNsec = (uint32_t)(pstTimeSpec->tv_nsec);
    uint32_t uiSec = (uint32_t)(pstTimeSpec->tv_sec);
    return ((uint32_t)getpid() << VOS_HIMALLOC_HIGH16_BIT) ^ uiNsec ^ uiSec;
}
#endif

static void VosHimallocRandomInit(void)
{
#ifndef OS_HONGMENG
    size_t uvSeed;
    struct timespec stCurrentTime;

    clock_gettime(CLOCK_REALTIME, &stCurrentTime);
    uvSeed = (size_t)VosHimallocRandomSeedGenerate(&stCurrentTime);

#if (__WORDSIZE == 64)
    uvSeed = ((uvSeed << 32) | uvSeed); /* 32在本处通俗易懂 */
#endif

    g_uiHimallocRandom.uvSeed = uvSeed;
#else
    g_uiHimallocRandom.uvSeed = (size_t)0; /* 鸿蒙OS侧特殊处理 */
#endif
    g_uiHimallocRandom.uvCnt = 0;
}

HIDDEN uint32_t VosHimallocBaseInit(void)
{
    VosHimallocRandomInit();
    return HIMALLOC_OK;
}

HIDDEN void VosHimallocStatsSwitchSet(bool option)
{
    g_himallocStatsSwitch = option;
}

HIDDEN bool VosHimallocStatsEnable(void)
{
    return g_himallocStatsSwitch;
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */
