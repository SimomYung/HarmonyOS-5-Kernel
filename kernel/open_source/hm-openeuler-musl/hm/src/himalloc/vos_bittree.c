/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#include "vos_bittree.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

#define VosBitTreeBitReverse(uiBitmap, usIndex)     \
    do {                                            \
        ((uiBitmap) ^= ((uint32_t)1 << (usIndex))); \
    } while (0)

static HIMALLOC_ALWAYS_INLINE uint8_t VosBitTreeNextAvaiable(uint32_t uiBitMap, uint8_t ucLastIndex)
{
    uint32_t uiTmpMask;

    if (ucLastIndex >= (VOS_BIT_TREE_IDX_MAX - 1U)) {
        __hmattr_sideff_resolved("no relative side effect");
        return VOS_BIT_TREE_IDX_MAX;
    }

    uiTmpMask = (uint32_t)(~((1UL << (ucLastIndex + 1U)) - 1U));
    uiTmpMask = uiBitMap & uiTmpMask;
    if (uiTmpMask != 0U) {
        __hmattr_sideff_resolved("no relative side effect");
        return (uint8_t)VosBitmapFFSImpl(uiTmpMask);
    }
    return VOS_BIT_TREE_IDX_MAX;
}

HIDDEN void VosBitTreeInit(VOS_BIT_TREE_BIN_S *pstTreeBin, uintptr_t uvPrivateData)
{
    uint32_t uiIndex;

    /* 初始化Bitmap信息 */
    pstTreeBin->uvPrivateData = uvPrivateData;
    pstTreeBin->uiTreeAvaiableMap = 0U;
    pstTreeBin->uiReserve = 0U;
    pstTreeBin->pstSplitChunk = HIMALLOC_NULL_PTR;
    pstTreeBin->uvSplitSize = 0UL;

    /* 初始化Bit tree信息 */
    for (uiIndex = 0U; uiIndex < VOS_BIT_TREE_IDX_MAX; uiIndex++) {
        pstTreeBin->apstFreeTrees[uiIndex] = HIMALLOC_NULL_PTR;
    }

    return;
}

static HIMALLOC_ALWAYS_INLINE void VosBitTreeListRemove(VOS_BIT_TREE_NODE_S *pstItemp)
{
    pstItemp->pstPrev->pstNext = pstItemp->pstNext;
    pstItemp->pstNext->pstPrev = pstItemp->pstPrev;

    return;
}

static HIMALLOC_ALWAYS_INLINE uint8_t VosBitTreeBitGet(uint8_t ucIndex)
{
    uint8_t ucTreeBit;
    /* 最后一个箱子和前31个箱子不同，要根据最高位来判断是左孩子还是右孩子 */
    if (ucIndex == ((1U << VOS_BIT_TREE_LG_IDX) - 1U)) {
        ucTreeBit = ((uint8_t)(sizeof(size_t)) << 3) - 1U;
    } else {
        /*
         * 最开始的内存范围为128，为2的7次方，查找位为bit0到bit6  ，-1是因为查找位从0开始
         * 0-1 2-3 4-5 6-7 8-9 10-11... 两两都有相同的bit查找位
         */
        ucTreeBit = (uint8_t)((ucIndex >> 1) + 7U - 1U);
    }
    return ucTreeBit;
}

static HIMALLOC_ALWAYS_INLINE void VosBitTreeLinkAdd(VOS_BIT_TREE_NODE_S *pstItemp, VOS_BIT_TREE_NODE_S *pstWhere)
{
    pstItemp->pstNext = pstWhere->pstNext;
    pstItemp->pstPrev = pstWhere;
    pstWhere->pstNext = pstItemp;
    pstItemp->pstNext->pstPrev = pstItemp;

    return;
}

static HIMALLOC_ALWAYS_INLINE void VosBitTreeNodeInit(VOS_BIT_TREE_NODE_S *pstNode, size_t uvKey, uint8_t ucIdx)
{
    pstNode->pstNext = pstNode;
    pstNode->pstPrev = pstNode;
    pstNode->pstParent = HIMALLOC_NULL_PTR;
    pstNode->apstChilds[0] = HIMALLOC_NULL_PTR;
    pstNode->apstChilds[1] = HIMALLOC_NULL_PTR;
    pstNode->uvHandle = (uvKey | ucIdx);

    return;
}

static HIMALLOC_ALWAYS_INLINE __pure uint8_t VosBitTreeIndexGet(size_t uvSize)
{
    size_t uvIndex;
    uint32_t uiResult;

    uvIndex = uvSize >> VOS_BIT_TREE_LG_MIN_SIZE;
    if (uvIndex == 0UL) {
        return 0U;
    } else if (uvIndex < VOS_BIT_TREE_IDX_MASK_MAX) {
        uiResult = VosBitmapFLSImpl((uint32_t)uvIndex);
        return (uint8_t)(((uint64_t)uiResult << 1) + (uvSize >> (uiResult + VOS_BIT_TREE_LG_MIN_SIZE - 1U) & 1U));
    } else {
        return (VOS_BIT_TREE_IDX_MAX - 1U);
    }
}

/* pstOldNode为主节点，pstNewNode非主节点   */
static void VosBitTreeEntryInherit(VOS_BIT_TREE_BIN_S *pstTreeBin, VOS_BIT_TREE_NODE_S *pstOldNode,
    VOS_BIT_TREE_NODE_S *pstNewNode)
{
    uint8_t ucIndex;
    uint8_t ucChild;
    VOS_BIT_TREE_NODE_S *pstDelNodeParent;

    /* 继承被删除节点的父节点信息 */
    pstDelNodeParent = pstOldNode->pstParent;
    if ((uintptr_t)pstDelNodeParent == VOS_BIT_TREE_ROOT_PARENT) {
        ucIndex = (uint8_t)VOS_BIT_TREE_IDX_GET(pstOldNode->uvHandle);
        pstTreeBin->apstFreeTrees[ucIndex] = pstNewNode;
    } else {
        if (pstDelNodeParent->apstChilds[0] == pstOldNode) {
            pstDelNodeParent->apstChilds[0] = pstNewNode;
        } else {
            pstDelNodeParent->apstChilds[1] = pstNewNode;
        }
    }
    pstNewNode->pstParent = pstDelNodeParent;

    /* 继承儿子信息 */
    for (ucChild = 0; ucChild < 2; ucChild++) { /* 2在本处通俗易懂 */
        /* 继承节点就是当前节点的儿子节点 */
        if (pstOldNode->apstChilds[ucChild] == pstNewNode) {
            continue;
        }

        if (pstOldNode->apstChilds[ucChild] != HIMALLOC_NULL_PTR) {
            pstNewNode->apstChilds[ucChild] = pstOldNode->apstChilds[ucChild];
            pstNewNode->apstChilds[ucChild]->pstParent = pstNewNode;
        } else {
            pstNewNode->apstChilds[ucChild] = HIMALLOC_NULL_PTR;
        }
    }

    return;
}

static void VosBitTreeBrotherEntryRemove(VOS_BIT_TREE_BIN_S *pstTreeBin, VOS_BIT_TREE_NODE_S *pstDelNode)
{
    /* 当删除的节点是主节点(主节点有父节点，附属节点没有)，第一附属节点上升为主节点 */
    if (pstDelNode->pstParent != HIMALLOC_NULL_PTR) {
        /* 使用兄弟节点替换 */
        VosBitTreeEntryInherit(pstTreeBin, pstDelNode, pstDelNode->pstNext);
    }

    /* 把这个节点从链表上摘除 */
    VosBitTreeListRemove(pstDelNode);

    return;
}

static HIMALLOC_ALWAYS_INLINE VOS_BIT_TREE_NODE_S *VosBitTreeRightMostHeirGet(VOS_BIT_TREE_NODE_S *pstNode)
{
    VOS_BIT_TREE_NODE_S **ppstRstorage = HIMALLOC_NULL_PTR;
    VOS_BIT_TREE_NODE_S *tmpNode = pstNode;

    while ((tmpNode->apstChilds[1] != NULL) || (tmpNode->apstChilds[0] != NULL)) {
        if (tmpNode->apstChilds[1] != NULL) {
            ppstRstorage = &(tmpNode->apstChilds[1]);
        } else {
            ppstRstorage = &(tmpNode->apstChilds[0]);
        }
        tmpNode = *ppstRstorage;
    }

    if (ppstRstorage != HIMALLOC_NULL_PTR) {
        *ppstRstorage = HIMALLOC_NULL_PTR;
    }

    return tmpNode;
}

static void VosBitTreeLonelyEntryRemove(VOS_BIT_TREE_BIN_S *pstTreeBin, VOS_BIT_TREE_NODE_S *pstDelNode)
{
    uint8_t ucIndex;
    VOS_BIT_TREE_NODE_S *pstDelNodeParent;

    pstDelNodeParent = pstDelNode->pstParent;
    if ((uintptr_t)pstDelNodeParent == VOS_BIT_TREE_ROOT_PARENT) {
        ucIndex = (uint8_t)VOS_BIT_TREE_IDX_GET(pstDelNode->uvHandle);
        pstTreeBin->apstFreeTrees[ucIndex] = HIMALLOC_NULL_PTR;
        VosBitTreeBitReverse(pstTreeBin->uiTreeAvaiableMap, (uint32_t)ucIndex);
    } else {
        if (pstDelNodeParent->apstChilds[0] == pstDelNode) {
            pstDelNodeParent->apstChilds[0] = HIMALLOC_NULL_PTR;
        } else {
            pstDelNodeParent->apstChilds[1] = HIMALLOC_NULL_PTR;
        }
    }

    return;
}

/* 调用处保证传入的节点在树上，否则进程运行会发生异常 */
HIDDEN void VosBitTreeEntryRemove(VOS_BIT_TREE_BIN_S *pstTreeBin, VOS_BIT_TREE_NODE_S *pstDelNode)
{
    VOS_BIT_TREE_NODE_S *pstRightMostHeir;

    if (pstTreeBin->pstSplitChunk == pstDelNode) {
        pstTreeBin->pstSplitChunk = HIMALLOC_NULL_PTR;
        pstTreeBin->uvSplitSize = 0;
        __hmattr_sideff_resolved("no relative side effect");
        return;
    }

    /* 存在兄弟节点使用兄弟节点替换 */
    if (pstDelNode->pstNext != pstDelNode) {
        VosBitTreeBrotherEntryRemove(pstTreeBin, pstDelNode);
        __hmattr_sideff_resolved("no relative side effect");
        return;
    }

    pstRightMostHeir = VosBitTreeRightMostHeirGet(pstDelNode);
    if (pstRightMostHeir == pstDelNode) {
        /* 无子嗣也无兄弟节点删除节点 */
        VosBitTreeLonelyEntryRemove(pstTreeBin, pstDelNode);
        __hmattr_sideff_resolved("no relative side effect");
        return;
    }

    /* 使用最右子嗣替换当前节点 */
    VosBitTreeEntryInherit(pstTreeBin, pstDelNode, pstRightMostHeir);

    return;
}

static HIMALLOC_ALWAYS_INLINE VOS_BIT_TREE_NODE_S *VosBitTreeAllocByIndex(VOS_BIT_TREE_BIN_S *pstTreeBin,
    size_t uvApplyKey, uint8_t ucTreeIndex, VOS_BIT_TREE_NODE_S **ppstRootNode, size_t *puvFitNodeKey)
{
    uint8_t ucChildIndex;
    uint8_t ucSearchBit;
    size_t uvKeyValue;
    size_t uvFitNodeKey = HIMALLOC_NULL_SIZE_T;
    VOS_BIT_TREE_NODE_S *pstFitNode = HIMALLOC_NULL_PTR;
    VOS_BIT_TREE_NODE_S *pstRootNode = HIMALLOC_NULL_PTR;
    VOS_BIT_TREE_NODE_S *pstTempNode = HIMALLOC_NULL_PTR;

    pstTempNode = pstTreeBin->apstFreeTrees[ucTreeIndex];
    ucSearchBit = VosBitTreeBitGet(ucTreeIndex);
    while (pstTempNode != HIMALLOC_NULL_PTR) {
        uvKeyValue = VOS_BIT_TREE_KEY_GET(pstTempNode->uvHandle);
        if ((uvKeyValue >= uvApplyKey) && (uvKeyValue < uvFitNodeKey)) {
            pstFitNode = pstTempNode;
            uvFitNodeKey = uvKeyValue;
            pstRootNode = HIMALLOC_NULL_PTR;

            /* 已经找到完全匹配的最适大小，不需要再继续查找 */
            if (uvFitNodeKey == uvApplyKey) {
                break;
            }
        }

        ucChildIndex = (uint8_t)((uvApplyKey >> ucSearchBit) & 0x1U);
        if ((ucChildIndex == 0) && (pstTempNode->apstChilds[1] != HIMALLOC_NULL_PTR)) {
            pstRootNode = pstTempNode->apstChilds[1];
        }

        pstTempNode = pstTempNode->apstChilds[ucChildIndex];
        ucSearchBit--;
    }

    *ppstRootNode = pstRootNode;
    *puvFitNodeKey = uvFitNodeKey;
    return pstFitNode;
}

HIDDEN VOS_BIT_TREE_NODE_S *VosBitTreeEntryExtract(VOS_BIT_TREE_BIN_S *pstTreeBin, size_t uvApplyKey)
{
    uint8_t ucTreeIndex;
    size_t uvFitNodeKey;
    VOS_BIT_TREE_NODE_S *pstRootNode;
    VOS_BIT_TREE_NODE_S *pstFitNode;

    ucTreeIndex = VosBitTreeIndexGet(uvApplyKey);

    pstFitNode = VosBitTreeAllocByIndex(pstTreeBin, uvApplyKey, ucTreeIndex, &pstRootNode, &uvFitNodeKey);
    if ((pstFitNode == HIMALLOC_NULL_PTR) && (pstRootNode == HIMALLOC_NULL_PTR)) {
        ucTreeIndex = VosBitTreeNextAvaiable(pstTreeBin->uiTreeAvaiableMap, ucTreeIndex);
        if (ucTreeIndex < VOS_BIT_TREE_IDX_MAX) {
            pstRootNode = pstTreeBin->apstFreeTrees[ucTreeIndex];
        }
    }

    /* pstRootNode能够确保任意节点均满足要求，获取最小节点即可 */
    while (pstRootNode != HIMALLOC_NULL_PTR) {
        /*
         * 为提升性能，此处无需获取真实的keyvalue，
         * 使用携带index的uvhandle进行比较，结果一致
         */
        if (uvFitNodeKey > pstRootNode->uvHandle) {
            pstFitNode = pstRootNode;
            uvFitNodeKey = pstRootNode->uvHandle;
        }

        pstRootNode = ((pstRootNode->apstChilds[0] != HIMALLOC_NULL_PTR) ? pstRootNode->apstChilds[0] :
                                                                           pstRootNode->apstChilds[1]);
    }

    if (pstTreeBin->uvSplitSize >= uvApplyKey && pstTreeBin->uvSplitSize <= uvFitNodeKey) {
        pstFitNode = pstTreeBin->pstSplitChunk;
        pstTreeBin->pstSplitChunk = HIMALLOC_NULL_PTR;
        pstTreeBin->uvSplitSize = 0UL;
        __hmattr_sideff_resolved("no relative side effect");
        return pstFitNode;
    }

    if (pstFitNode != HIMALLOC_NULL_PTR) {
        VosBitTreeEntryRemove(pstTreeBin, pstFitNode);
    }

    return pstFitNode;
}

HIDDEN void VosBitTreeEntryInsert(VOS_BIT_TREE_BIN_S *pstTreeBin, VOS_BIT_TREE_NODE_S *pstFreeNode, size_t uvKey,
    bool bSplitNode)
{
    uint8_t ucTreeIndex;
    uint8_t ucSearchBit;
    uint8_t ucChildIndex = 0U;
    size_t uvTempKey;
    VOS_BIT_TREE_NODE_S *pstTmpNode;
    VOS_BIT_TREE_NODE_S *pstTempNode;
    VOS_BIT_TREE_NODE_S *pstParentNode = HIMALLOC_NULL_PTR;
    VOS_BIT_TREE_NODE_S *pstFreeNodeTmp = pstFreeNode;
    size_t uvKeyTmp = uvKey;

    if (pstTreeBin->pstSplitChunk == HIMALLOC_NULL_PTR) {
        /* ucTreeIndex的计算延迟到插入树前 */
        pstTreeBin->pstSplitChunk = pstFreeNodeTmp;
        pstTreeBin->uvSplitSize = uvKeyTmp;
        __hmattr_sideff_resolved("no relative side effect");
        return;
    }

    if (bSplitNode == true) {
        /* 将旧的split节点置换出来并插入树中 */
        pstTempNode = pstTreeBin->pstSplitChunk;
        uvTempKey = pstTreeBin->uvSplitSize;

        pstTreeBin->pstSplitChunk = pstFreeNodeTmp;
        pstTreeBin->uvSplitSize = uvKeyTmp;

        pstFreeNodeTmp = pstTempNode;
        uvKeyTmp = uvTempKey;
    }

    ucTreeIndex = VosBitTreeIndexGet(uvKeyTmp);
    VosBitTreeNodeInit(pstFreeNodeTmp, uvKeyTmp, ucTreeIndex);

    pstTmpNode = pstTreeBin->apstFreeTrees[ucTreeIndex];
    if (pstTmpNode == HIMALLOC_NULL_PTR) {
        pstFreeNodeTmp->pstParent = (VOS_BIT_TREE_NODE_S *)VOS_BIT_TREE_ROOT_PARENT;
        pstTreeBin->apstFreeTrees[ucTreeIndex] = pstFreeNodeTmp;
        VosBitTreeBitReverse(pstTreeBin->uiTreeAvaiableMap, (uint32_t)ucTreeIndex);
        __hmattr_sideff_resolved("no relative side effect");
        return;
    }

    ucSearchBit = VosBitTreeBitGet(ucTreeIndex);
    while (pstTmpNode != HIMALLOC_NULL_PTR) {
        if (uvKeyTmp == VOS_BIT_TREE_KEY_GET(pstTmpNode->uvHandle)) {
            VosBitTreeLinkAdd(pstFreeNodeTmp, pstTmpNode);
            __hmattr_sideff_resolved("no relative side effect");
            return;
        }

        pstParentNode = pstTmpNode;
        ucChildIndex = (uint8_t)((uvKeyTmp >> ucSearchBit) & 0x1U);
        pstTmpNode = pstTmpNode->apstChilds[ucChildIndex];
        ucSearchBit--;
    }

    pstParentNode->apstChilds[ucChildIndex] = pstFreeNodeTmp;
    pstFreeNodeTmp->pstParent = pstParentNode;
    return;
}

static VOS_BIT_TREE_NODE_S *VosBitTreeMaxKeyNodeGet(VOS_BIT_TREE_BIN_S *pstTreeBin)
{
    size_t uvKeyTmp;
    size_t uvKeyMax = 0;
    uint32_t uiMaxBinIndex;
    VOS_BIT_TREE_NODE_S *pstTmpNode = NULL;
    VOS_BIT_TREE_NODE_S *pstMaxKeyNode = NULL;

    /* 找到存在根节点的最大分箱 */
    if (pstTreeBin->uiTreeAvaiableMap != 0) {
        uiMaxBinIndex = VosBitmapFLSImpl(pstTreeBin->uiTreeAvaiableMap);
        pstTmpNode = pstTreeBin->apstFreeTrees[uiMaxBinIndex];
    }

    while (pstTmpNode != NULL) {
        uvKeyTmp = VOS_BIT_TREE_KEY_GET(pstTmpNode->uvHandle);
        if (uvKeyTmp > uvKeyMax) {
            uvKeyMax = uvKeyTmp;
            pstMaxKeyNode = pstTmpNode;
        }
        /* 右子树的key必定大于左子树 */
        if (pstTmpNode->apstChilds[1] != NULL) {
            pstTmpNode = pstTmpNode->apstChilds[1];
        } else {
            pstTmpNode = pstTmpNode->apstChilds[0];
        }
    }

    uvKeyTmp = pstTreeBin->uvSplitSize;
    if (uvKeyTmp > uvKeyMax) {
        pstMaxKeyNode = pstTreeBin->pstSplitChunk;
    }

    return pstMaxKeyNode;
}

HIDDEN VOS_BIT_TREE_NODE_S *VosBitTreeMaxKeyNodeExtract(VOS_BIT_TREE_BIN_S *pstTreeBin)
{
    VOS_BIT_TREE_NODE_S *pstMaxKeyNode;

    pstMaxKeyNode = VosBitTreeMaxKeyNodeGet(pstTreeBin);
    if (pstMaxKeyNode == NULL) {
        return NULL;
    }

    VosBitTreeEntryRemove(pstTreeBin, pstMaxKeyNode);

    return pstMaxKeyNode;
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */
