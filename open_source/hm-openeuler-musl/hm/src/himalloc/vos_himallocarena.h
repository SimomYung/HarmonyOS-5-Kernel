/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#ifndef VOS_HIMALLOCARENA_H
#define VOS_HIMALLOCARENA_H

#include "osal_lock.h"
#include "vos_memzone.h"
#include "vos_himalloctype.h"
#include "vos_himalloctcache.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

#define VOS_HIMALLOC_ARENA_NUM_MAX 1U

typedef struct tagHimallocArenaCenter {
    VOS_HIMALLOC_ARENA_S *astArenas[VOS_HIMALLOC_ARENA_NUM_MAX];
    CSTL_LIST_HEAD_S stDeleted;
    OSAL_LOCK_T stLock;
} VOS_HIMALLOC_ARENA_CENTER_S;

extern HIDDEN VOS_HIMALLOC_ARENA_CENTER_S g_stHimallocArenaCenter;

HIDDEN uint32_t VosHimallocArenaModuleInit(void);

HIDDEN uint32_t VosHimallocArenaInit(uint8_t ucArenaId);

static HIMALLOC_ALWAYS_INLINE VOS_HIMALLOC_ARENA_S *VosHimallocArenaGet(uint8_t ucArenaId)
{
    return (g_stHimallocArenaCenter.astArenas)[ucArenaId];
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */

#endif
