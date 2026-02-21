/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#ifndef VOS_HIMALLOCTYPE_H
#define VOS_HIMALLOCTYPE_H

#include "cstl_list.h"
#include "vos_bittree.h"
#include "vos_memzone.h"
#include "vos_himallocbase.h"
#include "vos_himallocsclass.h"
#include "vos_radixtree.h"
#include "vos_memdecay.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

#define VOS_HIMALLOC_SLICE_LG_BINS_LOCK_NUM 0U

#define VOS_HIMALLOC_SLICE_BINS_LOCK_NUM (1U << VOS_HIMALLOC_SLICE_LG_BINS_LOCK_NUM)

#define VOS_HIMALLOC_TLB_LG_NCACHE 2UL

#define VOS_HIMALLOC_TLB_NCACHE_MAX (1UL << VOS_HIMALLOC_TLB_LG_NCACHE)

typedef struct tagHimallocTlbItem {
    uintptr_t uvPrevKey;
    VOS_RTREE_NODE_S *pstLeafNode;
} VOS_HIMALLOC_TLB_ITEM_S;

typedef struct tagHimallocTlbInfo {
    VOS_HIMALLOC_TLB_ITEM_S stL1Cache;
    VOS_HIMALLOC_TLB_ITEM_S astL2Cache[VOS_HIMALLOC_TLB_NCACHE_MAX];
} VOS_HIMALLOC_TLB_INFO_S;

#ifdef HIMALLOC_TCACHE_POLICY_LIST // tcache方案采用链表组织缓存资源池的策略，底噪开销较小

/* 指定slice缓存的空闲内存链表串节点头 */
typedef struct tagHiMallocFreeSliceNode {
    struct tagHiMallocFreeSliceNode *pstTcacheNextSlice;
} VOS_HIMALLOC_TCACHE_FREE_SLICE_CB;

#endif

/* 指定slice缓存的内存 */
typedef struct tagHiMallocTcacheSlice {
    uint16_t usLowWater;                                        /* 自上次GC回收以来缓存的下限 */
    uint16_t usNumCached;                                       /* 当前SIZE类缓存的内存数量 */
    uint16_t usMaxCached;                                       /* 当前SIZE类缓存的最大内存数量 */
    uint16_t uiReserved;                                        /* 对齐保留字段 */
#ifdef HIMALLOC_TCACHE_POLICY_LIST // tcache方案采用链表组织缓存资源池的策略，底噪开销较小
    VOS_HIMALLOC_TCACHE_FREE_SLICE_CB *pFreeStackList;          /* 缓存的可分配的内存地址链表串 */
#else // tcache方案采用数组组织缓存资源池的策略，底噪开销较大
    void **ppFreeStack;                                         /* 缓存的可分配的内存地址 */
#endif
} VOS_HIMALLOC_TCACHE_SLICE;

typedef struct tagHimallocSliceBinStat {
    uint64_t ullTotalNMalloc;
    uint64_t ullTotalNDalloc;
    size_t uvNBinBlocks;
} VOS_HIMALLOC_SLICE_BIN_STAT_S;

typedef struct tagHimallocSliceBin {
    VOS_HIMALLOC_LOCK_S *pstLock;
    void *pCurFreeBlock;                 /* 当前正在分配的空闲的Block */
    CSTL_LIST_HEAD_S stFreeBlockHead;         /* 此链表中的Block存在空闲的slice */
    CSTL_LIST_HEAD_S stNotFreeBlockHead;      /* 此链表中的Block不再有空闲的slice */
    VOS_HIMALLOC_SLICE_BIN_STAT_S stBinStat; /* bin的统计信息 */
    size_t uvBinIndex;
} VOS_HIMALLOC_SLICE_BIN_S;

typedef struct tagHimallocSliceHeap {
    VOS_HIMALLOC_SLICE_BIN_S astSliceBins[VOS_HIMALLOC_MAX_SMALL_INDEX]; /* 管理可分配的slices */

    /*
     * bins锁数量的对数, 如果每个bin拥有独立的锁，可以有效地提升bin之间的并发率
     * 但是会导致arena的管理开销增大，因此折中考虑所有bins通过hash共享下面锁的数量
     * 如果后续需要对所有的bins加锁，需要优先加序号低的锁再加序号高的锁，以此类推
     */
    VOS_HIMALLOC_LOCK_S astBinsLock[VOS_HIMALLOC_SLICE_BINS_LOCK_NUM]; /* 管理slice bins的锁 */
} VOS_HIMALLOC_SLICE_HEAP_S;

typedef struct tagHimallocBlockHeapStat {
    uint64_t ullTotalNMalloc;
    uint64_t ullTotalNDalloc;
    size_t uvNormalSize;
    size_t uvDontNeedSize;
} VOS_HIMALLOC_BLOCK_HEAP_STAT_S;

typedef struct tagHimallocBlockHeap {
    VOS_BIT_TREE_BIN_S stBlockTreeBinNormal;    /* 此树中的Block是空闲的，且已映射物理内存 */
    VOS_BIT_TREE_BIN_S stBlockTreeBinClean;     /* 此树中的Block是空闲的，且未映射物理内存 */
    VOS_HIMALLOC_BLOCK_HEAP_STAT_S stHeapStats; /* Block heap的统计信息 */
    VOS_HIMALLOC_LOCK_S stTreeBinLock;          /* Tree bin的专属锁 */
} VOS_HIMALLOC_BLOCK_HEAP_S;

/* VOS_HIMALLOC_ARENA_S结构体8字节对齐，且涉及到申请释放流程的关键字段都尽可能地置于同一个Cacheline中 */
typedef struct tagHimallocArena {
    VOS_HIMALLOC_SLICE_HEAP_S stSliceHeap;                            /* 可分配的Slice内存 */
    VOS_HIMALLOC_BLOCK_HEAP_S stBlockHeap;                            /* 可分配的Block内存 */
    VOS_MEM_ZONE_T zoneBlock;                                         /* 分配Block控制块的zone */
    VOS_HIMALLOC_TLB_INFO_S stTlb;                                    /* 记录缓存信息的tlb */
    uint8_t ucArenaId;                                                /* 从arena的ID */
    bool ucSaveHostage;                                               /* 内存人质开关 */
    uint8_t ucTcacheType;                                             /* 从arena的tcahche使能开关 */
    uint8_t ucReserve;                                                /* 预留字段保证8字节对齐 */
    uint16_t usMaxSliceSize;                                          /* 该arena的Slice内存size上限 */
    int16_t siTick;                                                   /* 计数功能，默认达到1000次进行物理页老化流程 */
    VOS_MEM_DECAY_INFO_S *pstDecayInfo;                               /* 物理页老化控制结构 */
    OSAL_LOCK_T stTcacheLock;                                         /* stLinkTcache的专属锁 */
    CSTL_LIST_HEAD_S stLinkTcache;                                    /* 此arena关联的tcache */
    CSTL_LIST_HEAD_S stLinkArenaCtl;                                  /* 链接到Link的控制节点 */
} VOS_HIMALLOC_ARENA_S;

/* Block内存的控制块 */
typedef struct tagHiMallocBlock {
    void *pStart;                           /* Block内存以页为单位的启始地址 */
    uintptr_t uvLength;                     /* Block维护的用户内存的长度 */
    uint8_t ucFreeSlice;                    /* 切割为slice后，空闲slice的数量 */
    uint8_t ucSizeClass;                    /* 如果切割为slice则为slice的size class */
    uint8_t ucSliced : 1;                   /* 是否是Slice的Block */
                                            /* Block属性，110代表在扩展块头，010表示在扩展块中间，011表示在扩展块末尾 */
    uint8_t ucProperty : 3;
    VosMemAreaRegionTypeE enAllocType : 2;  /* 内存类型 */
    uint8_t ucBlockState : 2;               /* Block的状态，见VOS_HIMALLOC_BLOCK_STATE_E */
    uint8_t ucArenaId;                      /* Block内存所在的arena */
#if (__WORDSIZE == 64)
    uint32_t uiReserve;                     /* 对齐预留字段，防止性能裂化 */
#endif

    union {
        VOS_BIT_TREE_NODE_S stTreeNode;     /* 当Block大于256K空闲时链接至BITMAP树 */
        VOS_HIMALLOC_HYBRID_S stHybrid;     /* 非树上的Slice节点或者Block的节点 */
    } unMetaDataNode;
} VOS_HIMALLOC_BLOCK;

typedef struct tagHimallocMemUseInfo {
    size_t uvTotalSize;
    size_t uvUsedSize;
    size_t uvTotalNum;
    size_t uvUsedNum;
} VOS_HIM_MEM_USE_INFO_S;

#define VOS_HIM_MEM_STAT_TYPE_MAX 64

typedef struct tagHimallocFreeMemStatInfo {
    uint32_t auiMemCnt[VOS_HIM_MEM_STAT_TYPE_MAX];
    size_t auvMemSize[VOS_HIM_MEM_STAT_TYPE_MAX];
    size_t *puvSizeMap;
    uint32_t uiMapTypeNum;
} VOS_HIM_FREE_MEM_STAT_INFO_S;

#ifdef __cplusplus
}
#endif /* __cpluscplus */

#endif
