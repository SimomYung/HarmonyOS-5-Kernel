/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#ifndef VOS_BITTREE_H
#define VOS_BITTREE_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "himalloc_inner.h"
#include "vos_bitmapgrp.h"

#ifdef __cplusplus
extern "C" {
#endif

/* bitmap的group的数量 */
#define VOS_BIT_TREE_GRPNUM 1

/* handle中index的对数 */
#define VOS_BIT_TREE_LG_IDX 5UL

/* handle中key的对数 */
#define VOS_BIT_TREE_LG_KEY ((sizeof(size_t) << 3) - VOS_BIT_TREE_LG_IDX)

/* handle中index的总数 */
#define VOS_BIT_TREE_IDX_MAX (1U << VOS_BIT_TREE_LG_IDX)

/* handle中KEY的掩码 */
#define VOS_BIT_TREE_KEY_MASK (HIMALLOC_NULL_SIZE_T << VOS_BIT_TREE_LG_IDX)

/* handle中index的掩码 */
#define VOS_BIT_TREE_IDX_MASK ((1UL << VOS_BIT_TREE_LG_IDX) - 1U)

/* 获取handle中index的值 */
#define VOS_BIT_TREE_IDX_GET(handle) ((handle) & VOS_BIT_TREE_IDX_MASK)

/* 设置handle中index的值 */
#define VOS_BIT_TREE_IDX_SET(pstNode, index) ((((pstNode)->uvHandle) & (~VOS_BIT_TREE_IDX_MASK)) | (index))

/* 获取handle中key的值 */
#define VOS_BIT_TREE_KEY_GET(handle) ((handle) & VOS_BIT_TREE_KEY_MASK)

/* 设置handle中KEY的值 */
#define VOS_BIT_TREE_KEY_SET(pstNode, length) ((((pstNode)->uvHandle) & (~VOS_BIT_TREE_KEY_MASK)) | (length))

/* bit tree存储最小值的对数 */
#define VOS_BIT_TREE_LG_MIN_SIZE 8U

/* bit tree当中index掩码的最大值 */
#define VOS_BIT_TREE_IDX_MASK_MAX 0xFFFF

#define VOS_BIT_TREE_ENTRY(item, type, member) ((type *)((uintptr_t)(item) - (uintptr_t)(&((type *)0UL)->member)))

/* 父节点是ROOT节点的标记 */
#define VOS_BIT_TREE_ROOT_PARENT 1UL

/* 用于管理大块内存的BITMAP树的节点 */
typedef struct tagBitTreeNode {
    struct tagBitTreeNode *pstParent; /* bit查找树的父节点 */
    struct tagBitTreeNode *pstNext; /* chunk在插入到bit查找树后，用以链接有相同大小的chunk，双向链表 */
    struct tagBitTreeNode *pstPrev;
    struct tagBitTreeNode *apstChilds[2]; /* 0表示左子树,1表示右子树 */
    uintptr_t uvHandle;
} VOS_BIT_TREE_NODE_S;

typedef struct tagHimallocTreeBin {
    uint32_t uiTreeAvaiableMap;         /* 每位分别表示对应的树是否为空，0为空树，1为非空 */
    uint32_t uiReserve;                 /* 对齐保留字段，防止64位性能劣化 */
    uintptr_t uvPrivateData;            /* 用户强相关的私有数据 */
    size_t uvSplitSize;                 /* split chunk的大小 */
    VOS_BIT_TREE_NODE_S *pstSplitChunk; /* 最后一次被拆分chunk的剩余chunk */
    VOS_BIT_TREE_NODE_S *apstFreeTrees[VOS_BIT_TREE_IDX_MAX]; /* 分别指向32棵树的根节点 */
} VOS_BIT_TREE_BIN_S;

static HIMALLOC_ALWAYS_INLINE uintptr_t VosBitTreeDataGet(VOS_BIT_TREE_BIN_S *pstTreeBin)
{
    return pstTreeBin->uvPrivateData;
}

extern HIDDEN void VosBitTreeInit(VOS_BIT_TREE_BIN_S *pstTreeBin, uintptr_t uvPrivateData);

extern HIDDEN VOS_BIT_TREE_NODE_S *VosBitTreeEntryExtract(VOS_BIT_TREE_BIN_S *pstTreeBin, size_t uvApplyKey);

extern HIDDEN void VosBitTreeEntryInsert(VOS_BIT_TREE_BIN_S *pstTreeBin, VOS_BIT_TREE_NODE_S *pstFreeNode, size_t uvKey,
    bool bSplitNode);

extern HIDDEN void VosBitTreeEntryRemove(VOS_BIT_TREE_BIN_S *pstTreeBin, VOS_BIT_TREE_NODE_S *pstDelNode);

extern HIDDEN VOS_BIT_TREE_NODE_S *VosBitTreeMaxKeyNodeExtract(VOS_BIT_TREE_BIN_S *pstTreeBin);

#ifdef __cplusplus
}
#endif

#endif
