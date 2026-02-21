/*
 * @file vos_memdecay.h
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: DOPRA SmoothStep Decay algorithm
 * Author: Anonym
 * Create: 2021/2/25
 */
#ifndef VOS_MEMDECAY_H
#define VOS_MEMDECAY_H

#include <stddef.h>
#include "osal_lock.h"
#include "himalloc_inner.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

#define VOS_MEM_DECAY_STEPS 64                      // Decay步长要求是2的幂次
#define VOS_MEM_DECAY_STEPS_LG 6                    // 2 ^ VOS_MEM_DECAY_STEPS_LG = VOS_MEM_DECAY_STEPS
#define VOS_MEM_DECAY_STEPS_MODULUS(uiNum) ((uiNum) & (VOS_MEM_DECAY_STEPS - 1))

#define VOS_MEM_DECAY_INITIAL_TICKS 1000
#define VOS_MEM_DECAY_DEFAULT_PERIODIC_TIME_MS 20000
#define VOS_MEM_DECAY_SECONDE_TO_NANOSECOND 1000000000
#define VOS_MEM_DECAY_MICROSECONDE_TO_NANOSECOND 1000000

#define VOS_MEM_DECAY_INFO_INVALID_T NULL

typedef struct TagMemDecayInfo {
    uint64_t ullEpochStart;                           /* 当前epoch开始时间 */
    uint64_t ullEpochEnd;                             /* 当前epoch结束时间 */
    uint64_t ullInterval;                             /* 每个epoch的间隔即PERIODIC_TIME_MS >> VOS_MEM_DECAY_STEPS_LG */
    size_t uvNonDecay;                                /* 上一个epoch中没有回收的数量 */
    OSAL_LOCK_T stDecayLock;                          /* 老化锁，对该结构体内所有成员进行防护 */
    uint32_t uiCursor;
    /* 循环数组RetainArray的游标，该位置的对应数量是需要在此次Epoch中完全回收的而uiCursor-1位置对应的数量是最新产生的脏页数量 */
    size_t uvRetainArray[VOS_MEM_DECAY_STEPS];        /* SmootherStep算法辅助数组 */
} VOS_MEM_DECAY_INFO_S;

extern HIDDEN VOS_MEM_DECAY_INFO_S *VosMemDecayInfoInit(void);

extern HIDDEN void VosMemDecayInfoDestory(VOS_MEM_DECAY_INFO_S *pstDecayInfo);

extern HIDDEN uint32_t VosMemDecayModuleInit(void);

extern HIDDEN size_t VosMemDecaySizeGet(VOS_MEM_DECAY_INFO_S *pstDecayInfo, size_t uvCurrentSize);

extern HIDDEN void VosMemDecayClean(void *pAddr, size_t uvSize);

static HIMALLOC_ALWAYS_INLINE void VosMemDecayTickReset(int16_t *psiTick)
{
    *psiTick = (int16_t)VOS_MEM_DECAY_INITIAL_TICKS;
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */

#endif /* VOS_MEMDECAY_H */
