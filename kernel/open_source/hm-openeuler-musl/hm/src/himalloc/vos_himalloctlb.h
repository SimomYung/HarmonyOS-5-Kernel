/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#ifndef VOS_HIMALLOCTLB_H
#define VOS_HIMALLOCTLB_H

#include "vos_himalloctype.h"
#include "vos_himallocassert.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

#if (__WORDSIZE == 32)
#define VOS_HIMALLOC_RTREE_LG_NLEAF 11UL
#else
#define VOS_HIMALLOC_RTREE_LG_NLEAF 14UL
#endif

#define VOS_HIMALLOC_RTREE_NLEAF (1UL << VOS_HIMALLOC_RTREE_LG_NLEAF)

#define VOS_HIMALLOC_TLB_NCACHE_MASK (VOS_HIMALLOC_TLB_NCACHE_MAX - 1UL)

#define VOS_HIMALLOC_TLB_SLOT_SHIFT (VOS_HIMALLOC_RTREE_LG_NLEAF + VOS_HIMALLOC_LG_VPAGE_SIZE)

extern HIDDEN VOS_HIMALLOC_BLOCK *VosHimallocTlbMatchHard(VOS_HIMALLOC_TLB_INFO_S *pstTlbInfo, size_t uvKey);

extern HIDDEN uint32_t VosHimallocTlbSetHard(VOS_HIMALLOC_TLB_INFO_S *pstTlbInfo, size_t uvKey, void *pObject);

extern HIDDEN uint32_t VosHimallocTlbModleInit(void);

static HIMALLOC_ALWAYS_INLINE size_t VosHimallocTlbPrevKeyGet(size_t uvKey)
{
    return uvKey >> VOS_HIMALLOC_TLB_SLOT_SHIFT;
}

static HIMALLOC_ALWAYS_INLINE size_t VosHimallocTlbSlotGet(size_t uvKey)
{
    return (uvKey >> VOS_HIMALLOC_TLB_SLOT_SHIFT) & VOS_HIMALLOC_TLB_NCACHE_MASK;
}

static HIMALLOC_ALWAYS_INLINE size_t VosHimallocTlbLeafKeyGet(size_t uvKey)
{
    return (uvKey >> VOS_HIMALLOC_LG_VPAGE_SIZE) & ((1UL << VOS_HIMALLOC_RTREE_LG_NLEAF) - 1UL);
}

static HIMALLOC_ALWAYS_INLINE bool VosHimallocTlbBlockMatch(size_t uvKey, const VOS_HIMALLOC_BLOCK *pstBlock)
{
    return ((uvKey >= ptr_to_ulong(pstBlock->pStart)) &&
        (uvKey < (ptr_to_ulong(pstBlock->pStart) + pstBlock->uvLength)));
}

static HIMALLOC_ALWAYS_INLINE VOS_HIMALLOC_BLOCK *VosHimallocTlbMatchImpl(VOS_HIMALLOC_TLB_INFO_S *pstTlbInfo,
    size_t uvKey)
{
    size_t uvLeafKey;
    size_t uvLeafPreKey;
    VOS_HIMALLOC_TLB_ITEM_S *pstL1Item;

    uvLeafPreKey = VosHimallocTlbPrevKeyGet(uvKey);
    pstL1Item = &(pstTlbInfo->stL1Cache);
    if (HIMALLOC_LIKELY(uvLeafPreKey == pstL1Item->uvPrevKey)) {
        uvLeafKey = VosHimallocTlbLeafKeyGet(uvKey);
        return pstL1Item->pstLeafNode->apstNode[uvLeafKey];
    }

    return VosHimallocTlbMatchHard(pstTlbInfo, uvKey);
}

static HIMALLOC_ALWAYS_INLINE VOS_HIMALLOC_BLOCK *VosHimallocTlbMatch(VOS_HIMALLOC_TLB_INFO_S *pstTlbInfo, size_t uvKey)
{
    VOS_HIMALLOC_BLOCK *pstMatchBlock;

    pstMatchBlock = VosHimallocTlbMatchImpl(pstTlbInfo, uvKey);
    if ((pstMatchBlock != HIMALLOC_NULL_PTR) && VosHimallocTlbBlockMatch(uvKey, pstMatchBlock)) {
        __hmattr_sideff_resolved("no relative side effect");
        return pstMatchBlock;
    }

    return HIMALLOC_NULL_PTR;
}

static HIMALLOC_ALWAYS_INLINE uint32_t VosHimallocTlbSet(VOS_HIMALLOC_TLB_INFO_S *pstTlbInfo, size_t uvKey,
    void *pObject)
{
    size_t uvSlot;
    size_t uvLeafKey;
    size_t uvLeafPreKey;
    VOS_HIMALLOC_TLB_ITEM_S *pstL1Item;
    VOS_HIMALLOC_TLB_ITEM_S *pstL2Item;
    uint32_t uiRet;

    uvLeafPreKey = VosHimallocTlbPrevKeyGet(uvKey);

    pstL1Item = &(pstTlbInfo->stL1Cache);
    if (HIMALLOC_LIKELY(uvLeafPreKey == pstL1Item->uvPrevKey)) {
        uvLeafKey = VosHimallocTlbLeafKeyGet(uvKey);
        VOS_HIMALLOC_ASSERT(pstL1Item->uvPrevKey == uvLeafPreKey);
        pstL1Item->pstLeafNode->apstNode[uvLeafKey] = pObject;
        return HIMALLOC_OK;
    }

    uvSlot = VosHimallocTlbSlotGet(uvKey);
    pstL2Item = &(pstTlbInfo->astL2Cache[uvSlot]);
    if (pstL2Item->uvPrevKey == uvLeafPreKey) {
        /* 更新L1缓存 */
        pstL1Item->uvPrevKey = uvLeafPreKey;
        pstL1Item->pstLeafNode = pstL2Item->pstLeafNode;

        /* 设置cache对象 */
        uvLeafKey = VosHimallocTlbLeafKeyGet(uvKey);
        pstL2Item->pstLeafNode->apstNode[uvLeafKey] = pObject;
        return HIMALLOC_OK;
    }

    uiRet = VosHimallocTlbSetHard(pstTlbInfo, uvKey, pObject);
    return uiRet;
}

extern HIDDEN VOS_HIMALLOC_BLOCK *VosHimallocTlbRtreeGet(size_t uvKey);

extern HIDDEN uint32_t VosHimallocTlbRtreeSet(size_t uvKey, void *pObject);

extern HIDDEN void VosHimallocTlbInit(VOS_HIMALLOC_TLB_INFO_S *pstTlbInfo);

extern HIDDEN VOS_RTREE_T VosHimallocRtreeIdGet(void);

#ifdef __cplusplus
}
#endif /* __cpluscplus */

#endif
