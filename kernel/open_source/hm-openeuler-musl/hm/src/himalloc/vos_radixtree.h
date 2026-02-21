/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#ifndef VOS_RADIXTREE_H
#define VOS_RADIXTREE_H

#include "osal_lock.h"
#include "vos_metaalloc.h"
#include "himalloc_inner.h"
#include "vos_himallocio.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

/* radixtree支持的最大level */
#define VOS_RTREE_LEVEL_MAX 4

/* radixtree支持的key的bit数 */
#define VOS_RTREE_SIZE_T_BITS (sizeof(size_t) << 3)

/* 0x22错误码 数学计算结果超出描述范围。 */
#define VOS_RTREE_ERRNO_RANGE 34 /* Math result not representable */

#define VOS_RTREE_ERRNO_NOMEM 12

/* 无效的radixtree句柄 */
#define VOS_RTREE_IVALID_T NULL

/* radixtree无效的key索引 */
#define VOS_RTREE_IVALID_KEY 1

/* radixtree Cache数的对数 */
#define VOS_RTEE_LG_NCACHE 4

/* radixtree Cache数的规格 */
#define VOS_RTEE_NCACHE_MAX (1UL << VOS_RTEE_LG_NCACHE)

/* radixtree申请内存的钩子原型 */
typedef void *(*VOS_RADIX_TREE_ALLOC_FUNC)(size_t uvSize);

/* radixtree释放内存的钩子原型 */
typedef void (*VOS_RADIX_TREE_FREE_FUNC)(void *pAddr);

typedef enum enRtreeLevel {
    VOS_RTREE_LEVEL_0_E = 0,
    VOS_RTREE_LEVEL_1_E,
    VOS_RTREE_LEVEL_2_E,
    VOS_RTREE_LEVEL_3_E,
    VOS_RTREE_LEVEL_BUTT,
} VOS_RTREE_LEVEL_E;

typedef struct tagRtreeEntry {
    void *pObject;
} VOS_RTREE_ENTRY_S;

typedef struct tagRtreeNode {
    void *apstNode[0];
} VOS_RTREE_NODE_S;

typedef struct tagRtreeLevel {
    uint32_t uiLevelAccum; /* 累积到当前level总的bit位 */
    uint32_t uiUpperAccum; /* 下游level总的bit位 */
    size_t uvAccumRmask;
    size_t uvLevelMask;
    uint32_t uiLevelNo;   /* 当前level的等级 */
    uint32_t uiLevelBits; /* 当前level映射的bit位 */
} VOS_RTREE_LEVEL_S;

typedef struct tagRtreeInfo {
    VOS_RTREE_LEVEL_S astLevelInfo[VOS_RTREE_LEVEL_MAX];
    size_t uvLevelNum;
} VOS_RTREE_INFO_S;

typedef struct tagRtreeCacheItem {
    size_t uvPreKey;
    VOS_RTREE_NODE_S *pstCacheNode;
} VOS_RTREE_CACHE_ITEM_S;

typedef struct tagRtreeCache {
    VOS_RTREE_CACHE_ITEM_S astRtreeCache[VOS_RTEE_NCACHE_MAX];
} VOS_RTREE_CACHE_S;

typedef struct tagRtreeStat {
    size_t uvTotalMem;
} VOS_RTREE_STAT_S;

typedef struct tagRtreeAlloc {
    VOS_RADIX_TREE_ALLOC_FUNC pfnAlloc;
    VOS_RADIX_TREE_FREE_FUNC pfnFree;
} VOS_RTREE_ALLOC_S;

typedef struct tagRtreeRoot {
    VOS_RTREE_INFO_S stTreeInfo;
    VOS_RTREE_CACHE_ITEM_S astRtreeCache[VOS_RTEE_NCACHE_MAX];
    OSAL_LOCK_T stLock;
    VOS_RTREE_STAT_S stTreeStat;
    VOS_RTREE_ALLOC_S stAllocHook;
    VOS_RTREE_NODE_S *pstTreeRoot;
} VOS_RTREE_ROOT_S;

/* radixtree的句柄 */
typedef VOS_RTREE_ROOT_S *VOS_RTREE_T;


extern HIDDEN VOS_RTREE_NODE_S *VosRtreeNodeCreate(VOS_RTREE_ROOT_S *pstRoot, VOS_RTREE_LEVEL_S *pstLevelInfo,
    VOS_RTREE_NODE_S *pstLevelNode, size_t uvSubKey);

extern HIDDEN VOS_RTREE_T VosRtreeInit(VOS_RTREE_INFO_S *pstTreeInfo, VOS_RTREE_ALLOC_S *pstAllocFun);

extern HIDDEN VOS_RTREE_NODE_S *VosRtreeLevelNodeGet(VOS_RTREE_ROOT_S *pstRoot, size_t uvKey, bool bInitMiss);

static HIMALLOC_ALWAYS_INLINE size_t VosRtreeSubKeyGet(const VOS_RTREE_LEVEL_S *pstLevelInfo, size_t uvKey)
{
    return (uvKey >> pstLevelInfo->uiUpperAccum) & pstLevelInfo->uvLevelMask;
}

static HIMALLOC_ALWAYS_INLINE VOS_RTREE_LEVEL_S *VosRtreeLevelInfoGet(VOS_RTREE_ROOT_S *pstRoot, size_t uvLevel)
{
    return &(((pstRoot->stTreeInfo).astLevelInfo)[uvLevel]);
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */

#endif