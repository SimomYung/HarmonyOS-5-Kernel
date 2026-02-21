/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#ifndef VOS_HIMALLOCGROW_H
#define VOS_HIMALLOCGROW_H

#include "vos_memarea.h"
#include "vos_himalloctype.h"
#include "vos_himalloc_errno.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

extern HIDDEN VOS_MEM_AREA_T g_HimallocAreaId;

extern HIDDEN uint32_t VosHimallocGrowModuleInit(void);

extern HIDDEN void *VosHimallocGrowAlloc(VOS_HIMALLOC_ARENA_S *pstArena, size_t *puvSize,
    VosMemAreaRegionTypeE *penAllocType);

extern HIDDEN void VosHimallocGrowFree(VOS_HIMALLOC_ARENA_S *pstArena, void *pAddr, size_t uvSize,
    VosMemAreaRegionTypeE enAllocType);

#ifdef __cplusplus
}
#endif /* __cpluscplus */

#endif
