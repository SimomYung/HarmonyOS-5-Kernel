/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#ifndef VOS_MEMAREA_H
#define VOS_MEMAREA_H

#include "himalloc_algo.h"
#include "himalloc_inner.h"
#include "vos_himallocio.h"
#include "osal_lock.h"
#include "v_legacyavl.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

typedef void *VOS_MEM_AREA_T;

typedef struct tagMemArea {
    AVL_NODE_S *pstRoot;
    OSAL_LOCK_T stLock;
} VOS_MEM_AREA_S;

#define VOS_MEM_INVALID_T HIMALLOC_NULL_PTR

/**
 * @ingroup himalloc
 * 0x62 申请地址请求失败。
 */
#define HIMALLOC_ERRNO_ADDRNOTAVAIL 98 /* Cannot assign requested address */

typedef struct tagMemAreaRegion {
    uintptr_t uvStart;
    uintptr_t uvLen;
    uint32_t uiArena;
    VosMemAreaRegionTypeE enAllocType;
} VOS_MEM_AREA_REGION_S;

extern HIDDEN VOS_MEM_AREA_T VosMemAreaInit(void);

extern HIDDEN uint32_t VosMemAreaRegionAdd(VOS_MEM_AREA_T area, uintptr_t uvStart, uintptr_t uvLen,
    uint32_t uiArena, VosMemAreaRegionTypeE enAllocType);

extern HIDDEN uint32_t VosMemAreaRegionDel(VOS_MEM_AREA_T area, uintptr_t uvStartAddr, uintptr_t uvLen);

extern HIDDEN uint32_t VosMemAreaRegionGet(VOS_MEM_AREA_T area, uintptr_t uvAddr, VOS_MEM_AREA_REGION_S *pstRegion);

#ifdef __cplusplus
}
#endif /* __cpluscplus */

#endif
