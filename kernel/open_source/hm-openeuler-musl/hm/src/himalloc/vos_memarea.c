/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#include <stdlib.h>
#include "osal_lock.h"
#include "v_legacyavl.h"
#include "osal_lock.h"
#include "vos_metaalloc.h"
#include "vos_memarea.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

typedef struct tagMemAreaNode {
    VOS_MEM_AREA_REGION_S stRegion;
    AVL_NODE_S stNode;
} VOS_MEM_AREA_NODE_S;

#define VOS_MEM_AREA_ENTRY(item, type, member) ((type *)((uintptr_t)(item) - (uintptr_t)(&((type *)0UL)->member)))

HIDDEN VOS_MEM_AREA_T VosMemAreaInit(void)
{
    uint32_t uiRet;
    VOS_MEM_AREA_S *pstArea;

    pstArea = (VOS_MEM_AREA_S *)VosMetaAlloc(sizeof(VOS_MEM_AREA_S));
    if (pstArea == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return VOS_MEM_INVALID_T;
    }
    pstArea->pstRoot = HIMALLOC_NULL_PTR;

    uiRet = OSAL_LockCreate(&(pstArea->stLock));
    if (uiRet != HIMALLOC_OK) {
        VosMetaFree(pstArea);
        __hmattr_sideff_resolved("no relative side effect");
        return VOS_MEM_INVALID_T;
    }

    return (VOS_MEM_AREA_T)pstArea;
}

static __pure int32_t VosMemAreaNodeCmp(VOS_AVL_NODE *pNode1, VOS_AVL_NODE *pNode2)
{
    VOS_MEM_AREA_NODE_S *pstAreaNode1;
    VOS_MEM_AREA_NODE_S *pstAreaNode2;
    VOS_MEM_AREA_REGION_S *pstRegion1;
    VOS_MEM_AREA_REGION_S *pstRegion2;

    pstAreaNode1 = (VOS_MEM_AREA_NODE_S *)VOS_MEM_AREA_ENTRY(pNode1, VOS_MEM_AREA_NODE_S, stNode);
    pstRegion1 = &(pstAreaNode1->stRegion);

    pstAreaNode2 = (VOS_MEM_AREA_NODE_S *)VOS_MEM_AREA_ENTRY(pNode2, VOS_MEM_AREA_NODE_S, stNode);
    pstRegion2 = &(pstAreaNode2->stRegion);

    if ((pstRegion2->uvStart + pstRegion2->uvLen) <= pstRegion1->uvStart) {
        return 1;
    }
    if ((pstRegion1->uvStart + pstRegion1->uvLen) <= pstRegion2->uvStart) {
        return -1;
    }

    return 0;
}

static VOS_MEM_AREA_NODE_S *VosMemAreaNodeCreate(uintptr_t uvStart, uintptr_t uvLen, uint32_t uiArena,
    VosMemAreaRegionTypeE enAllocType)
{
    VOS_MEM_AREA_NODE_S *pstNode;
    VOS_MEM_AREA_REGION_S *pstDstRegion;

    pstNode = (VOS_MEM_AREA_NODE_S *)VosMetaAlloc(sizeof(VOS_MEM_AREA_NODE_S));
    if (pstNode == HIMALLOC_NULL_PTR) {
        VosHimallocLogWrite(HIMALLOC_ERRNO_NOMEM, "[HIMALLOC-%s]system mem is not enough, malloc size(%lu)",
            __FUNCTION__, sizeof(VOS_MEM_AREA_NODE_S));
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }
    (void)memset(pstNode, 0, sizeof(VOS_MEM_AREA_NODE_S));

    pstDstRegion = &(pstNode->stRegion);
    pstDstRegion->enAllocType = enAllocType;
    pstDstRegion->uiArena = uiArena;
    pstDstRegion->uvStart = uvStart;
    pstDstRegion->uvLen = uvLen;

    return pstNode;
}

static VOS_MEM_AREA_NODE_S *VosMemAreaOverlapGet(VOS_MEM_AREA_S *pstArea, VOS_MEM_AREA_NODE_S *pstNode)
{
    AVL_NODE_S *pstAvlNode;
    VOS_MEM_AREA_NODE_S *pstFindNode = HIMALLOC_NULL_PTR;

    pstAvlNode = VOS_AVLFind(pstArea->pstRoot, &(pstNode->stNode), VosMemAreaNodeCmp);
    if (pstAvlNode != HIMALLOC_NULL_PTR) {
        pstFindNode = (VOS_MEM_AREA_NODE_S *)VOS_MEM_AREA_ENTRY(pstAvlNode, VOS_MEM_AREA_NODE_S, stNode);
    }
    return pstFindNode;
}

static bool VosMemAreaMergePermit(const VOS_MEM_AREA_REGION_S *pstPrevRegion, const VOS_MEM_AREA_REGION_S *pstRegion)
{
    uintptr_t uvPrevEnd;

    uvPrevEnd = pstPrevRegion->uvStart + pstPrevRegion->uvLen;
    if ((uvPrevEnd == pstRegion->uvStart) && (pstPrevRegion->uiArena == pstRegion->uiArena) &&
        (pstPrevRegion->enAllocType == MEM_AREA_REGION_MMAP) && (pstRegion->enAllocType == MEM_AREA_REGION_MMAP)) {
        /* 处理人质问题时，需要允许扩展块合并 */
        return false;
    }

    return false;
}

/* 调用者需确保pstSourceNode的线性区不会tree中的线性区重叠 */
static uint32_t VosMemAreaAddImpl(VOS_MEM_AREA_S *pstArea, uintptr_t uvStart, uintptr_t uvLen, uint32_t uiArena,
    VosMemAreaRegionTypeE enAllocType)
{
    VOS_MEM_AREA_NODE_S stKeyNode;
    VOS_MEM_AREA_REGION_S *pstKeyRegion;
    VOS_MEM_AREA_NODE_S *pstPrevNode;
    VOS_MEM_AREA_NODE_S *pstNextNode;
    VOS_MEM_AREA_NODE_S *pstAddNode;

    pstAddNode = VosMemAreaNodeCreate(uvStart, uvLen, uiArena, enAllocType);
    if (pstAddNode == HIMALLOC_NULL_PTR) {
        /* VosMemAreaNodeCreate内部已经记录日志，此处不再记录 */
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_NOMEM;
    }

    /* 尝试与前向节点合并 */
    pstKeyRegion = &(stKeyNode.stRegion);
    pstKeyRegion->uvStart = uvStart - 1U;
    pstKeyRegion->uvLen = 1U;
    pstPrevNode = VosMemAreaOverlapGet(pstArea, &stKeyNode);
    if ((pstPrevNode != HIMALLOC_NULL_PTR) &&
        VosMemAreaMergePermit(&(pstPrevNode->stRegion), &(pstAddNode->stRegion))) {
        pstArea->pstRoot = VOS_AVLDel(pstArea->pstRoot, &(pstPrevNode->stNode), VosMemAreaNodeCmp);

        pstPrevNode->stRegion.uvLen += pstAddNode->stRegion.uvLen;
        VosMetaFree(pstAddNode);
        pstAddNode = pstPrevNode;
    }

    /* 尝试与后向节点合并 */
    pstKeyRegion->uvStart = uvStart + uvLen;
    pstKeyRegion->uvLen = 1U;
    pstNextNode = VosMemAreaOverlapGet(pstArea, &stKeyNode);
    if ((pstNextNode != HIMALLOC_NULL_PTR) &&
        VosMemAreaMergePermit(&(pstAddNode->stRegion), &(pstNextNode->stRegion))) {
        pstArea->pstRoot = VOS_AVLDel(pstArea->pstRoot, &(pstNextNode->stNode), VosMemAreaNodeCmp);

        pstAddNode->stRegion.uvLen += pstNextNode->stRegion.uvLen;
        VosMetaFree(pstNextNode);
        pstNextNode = HIMALLOC_NULL_PTR;
    }

    pstArea->pstRoot = VOS_AVLAdd(pstArea->pstRoot, &(pstAddNode->stNode), VosMemAreaNodeCmp);

    return HIMALLOC_OK;
}

HIDDEN uint32_t VosMemAreaRegionAdd(VOS_MEM_AREA_T area, uintptr_t uvStart, uintptr_t uvLen, uint32_t uiArena,
    VosMemAreaRegionTypeE enAllocType)
{
    uint32_t uiLockValue;
    VOS_MEM_AREA_S *pstArea;
    VOS_MEM_AREA_NODE_S stKeyNode;
    VOS_MEM_AREA_REGION_S *pstKeyRegion;
    VOS_MEM_AREA_NODE_S *pstOverlapNode;
    uint32_t uiRet;

    pstArea = (VOS_MEM_AREA_S *)area;
    if (pstArea == HIMALLOC_NULL_PTR) {
        VosHimallocLogWrite(HIMALLOC_ERRNO_INVAL, "[HIMALLOC-%s]area is null", __FUNCTION__);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_INVAL;
    }

    pstKeyRegion = &(stKeyNode.stRegion);
    pstKeyRegion->uvStart = uvStart;
    pstKeyRegion->uvLen = uvLen;

    uiLockValue = OSAL_LockLock(&(pstArea->stLock));

    /* 地址重叠检查 */
    pstOverlapNode = VosMemAreaOverlapGet(pstArea, &stKeyNode);
    if (pstOverlapNode != HIMALLOC_NULL_PTR) {
        OSAL_LockUnLock(&(pstArea->stLock), uiLockValue);
        VosHimallocLogWrite(HIMALLOC_ERRNO_INVAL, "[HIMALLOC-%s]area mem added overlap",
            __FUNCTION__);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_ADDRNOTAVAIL;
    }

    uiRet = VosMemAreaAddImpl(pstArea, uvStart, uvLen, uiArena, enAllocType);
    OSAL_LockUnLock(&(pstArea->stLock), uiLockValue);

    return uiRet;
}


static uint32_t VosMemAreaDelImpl(VOS_MEM_AREA_S *pstArea, uintptr_t uvStartAddr, uintptr_t uvLen)
{
    size_t uvSplitEnd;
    VOS_MEM_AREA_NODE_S stKeyNode;
    VOS_MEM_AREA_REGION_S *pstKeyRegion;
    VOS_MEM_AREA_NODE_S *pstSplitNode;
    VOS_MEM_AREA_REGION_S *pstSplitRegion;
    uint32_t uiRet;
    uintptr_t uvLenTmp = uvLen;
    uintptr_t uvStartAddrTmp = uvStartAddr;

    pstKeyRegion = &(stKeyNode.stRegion);
    do {
        pstKeyRegion->uvStart = uvStartAddrTmp;
        /* 从低地址开始找重叠的扩展块 */
        pstKeyRegion->uvLen = 1;
        pstSplitNode = VosMemAreaOverlapGet(pstArea, &stKeyNode);
        if ((pstSplitNode == HIMALLOC_NULL_PTR) || (uvStartAddrTmp < pstSplitNode->stRegion.uvStart)) {
            /* [uvStartAddr, uvStartAddr + uvLen) 部分区段不在线性区 */
            __hmattr_sideff_resolved("no relative side effect");
            return HIMALLOC_ERRNO_INVAL;
        }

        pstArea->pstRoot = VOS_AVLDel(pstArea->pstRoot, &(pstSplitNode->stNode), VosMemAreaNodeCmp);

        /* 首部存在剩余的线性区，重新添加到树中 */
        pstSplitRegion = &(pstSplitNode->stRegion);
        if (pstSplitRegion->uvStart < uvStartAddrTmp) {
            uiRet = VosMemAreaAddImpl(pstArea, pstSplitRegion->uvStart, uvStartAddrTmp - pstSplitRegion->uvStart,
                pstSplitRegion->uiArena, pstSplitRegion->enAllocType);
            if (uiRet != HIMALLOC_OK) {
                __hmattr_sideff_resolved("no relative side effect");
                return uiRet;
            }
        }

        /* 尾部存在剩余的线性区，重新添加到树中 */
        uvSplitEnd = pstSplitRegion->uvStart + pstSplitRegion->uvLen;
        if (uvSplitEnd > uvStartAddrTmp + uvLenTmp) {
            uiRet = VosMemAreaAddImpl(pstArea, uvStartAddrTmp + uvLenTmp, uvSplitEnd - uvStartAddrTmp - uvLenTmp,
                pstSplitRegion->uiArena, pstSplitRegion->enAllocType);
            if (uiRet != HIMALLOC_OK) {
                __hmattr_sideff_resolved("no relative side effect");
                return uiRet;
            }
        }

        VosMetaFree(pstSplitNode);
        pstSplitNode = HIMALLOC_NULL_PTR;

        if (uvSplitEnd >= uvStartAddrTmp + uvLenTmp) {
            __hmattr_sideff_resolved("no relative side effect");
            return HIMALLOC_OK;
        } else {
            uvLenTmp = uvStartAddrTmp + uvLenTmp - uvSplitEnd;
            uvStartAddrTmp = uvSplitEnd;
        }
    } while (uvLenTmp > 0);

    return HIMALLOC_OK;
}

HIDDEN uint32_t VosMemAreaRegionDel(VOS_MEM_AREA_T area, uintptr_t uvStartAddr, uintptr_t uvLen)
{
    uint32_t uiRet;
    uint32_t uiLockValue;
    VOS_MEM_AREA_S *pstArea;

    pstArea = (VOS_MEM_AREA_S *)area;
    if (pstArea == HIMALLOC_NULL_PTR || uvLen == 0) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_INVAL;
    }

    uiLockValue = OSAL_LockLock(&(pstArea->stLock));
    uiRet = VosMemAreaDelImpl(pstArea, uvStartAddr, uvLen);
    OSAL_LockUnLock(&(pstArea->stLock), uiLockValue);

    return uiRet;
}

HIDDEN uint32_t VosMemAreaRegionGet(VOS_MEM_AREA_T area, uintptr_t uvAddr, VOS_MEM_AREA_REGION_S *pstRegion)
{
    uint32_t uiLockValue;
    VOS_MEM_AREA_NODE_S stKeyNode;
    VOS_MEM_AREA_S *pstArea;
    VOS_MEM_AREA_REGION_S *pstKeyRegion;
    VOS_MEM_AREA_NODE_S *pstFindNode;
    VOS_MEM_AREA_REGION_S *pstFindRegion;

    pstArea = (VOS_MEM_AREA_S *)area;
    if (pstArea == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_INVAL;
    }

    pstKeyRegion = &(stKeyNode.stRegion);
    pstKeyRegion->uvStart = uvAddr;
    pstKeyRegion->uvLen = 1;

    uiLockValue = OSAL_LockLock(&(pstArea->stLock));

    /* 地址重叠检查 */
    pstFindNode = VosMemAreaOverlapGet(pstArea, &stKeyNode);
    if (pstFindNode == HIMALLOC_NULL_PTR) {
        OSAL_LockUnLock(&(pstArea->stLock), uiLockValue);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_ADDRNOTAVAIL;
    }

    pstFindRegion = &(pstFindNode->stRegion);
    pstRegion->uvStart = pstFindRegion->uvStart;
    pstRegion->uvLen = pstFindRegion->uvLen;
    pstRegion->uiArena = pstFindRegion->uiArena;
    pstRegion->enAllocType = pstFindRegion->enAllocType;

    OSAL_LockUnLock(&(pstArea->stLock), uiLockValue);

    return HIMALLOC_OK;
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */
