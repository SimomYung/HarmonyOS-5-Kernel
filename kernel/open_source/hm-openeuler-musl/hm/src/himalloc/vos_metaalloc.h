/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#ifndef VOS_METAALLOC_H
#define VOS_METAALLOC_H

#include <unistd.h>
#include <sys/mman.h>
#include "himalloc_inner.h"
#include "vos_himalloc_errno.h"
#include "cstl_list.h"
#include "osal_lock.h"
#include "vos_bitmapgrp.h"
#include "vos_himallocio.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

#define VOS_META_PAGE_MIN_LG 12U /* 最小页大小4k, piece首地址4k对齐 */
#define VOS_META_PAGE_SIZE ((size_t)sysconf(_SC_PAGESIZE))

/* block的最小单位为1k */
#define VOS_META_BLOCK_MIN_LG 10U /* block的最小单位为1k */
#define VOS_META_BLOCK_MIN (1UL << VOS_META_BLOCK_MIN_LG)

#define VOS_META_BLOCK_TYPE 256U /* 256个哈希节点，代表1k到256k的内存大小 */
#define VOS_META_BLOCK_TYPE_LG 8U
#define VOS_META_BLOCK_BIT_GROUPS (VOS_META_BLOCK_TYPE >> VOS_BITMAP_LG_BITS)

#define VOS_META_ALLOC_SIZE_MAX_LG (VOS_META_BLOCK_TYPE_LG + VOS_META_BLOCK_MIN_LG)
#define VOS_META_APPEND_MAX_SIZE (1UL << VOS_META_ALLOC_SIZE_MAX_LG)
#define VOS_META_HEAD_LEN (1UL << 10)
#define VOS_META_ALLOC_SIZE_MAX (VOS_META_APPEND_MAX_SIZE - VOS_META_HEAD_LEN)
/* piece控制头1K对齐需要占用1k的内存，bitmap占用512字节的内存不会引起额外的对齐开销 */
#define VOS_META_PIECE_BIT_MAX (512UL << 3)
#define VOS_META_PIECE_BIT_GROUPS (VOS_META_PIECE_BIT_MAX >> VOS_BITMAP_LG_BITS)

#define VOS_META_SLICE_MIN_LG (VOS_META_ALLOC_SIZE_MAX_LG - (VOS_META_PAGE_MIN_LG))
#define VOS_META_SLICE_MAX (1UL << (VOS_META_BLOCK_MIN_LG - 1U))
/* slice大小按照2的幂次方分布 */
#define VOS_META_SLICE_TYPE (VOS_META_BLOCK_MIN_LG - VOS_META_SLICE_MIN_LG)

/* 内存头handle的后11位存放内存大小，第12位开始存放piece首地址，piece首地址4k对齐 */
#define VOS_META_PIECE_ADDR_IN_HEAD_MASK (~(uintptr_t)((1UL << VOS_META_PAGE_MIN_LG) - 1U))
#define VOS_META_SIZE_IN_HEAD_MASK ((1UL << VOS_META_PAGE_MIN_LG) - 1U)
#define VOS_META_PIECE_ADDR_IN_HEAD_GET(uvHandle) ((uvHandle)&VOS_META_PIECE_ADDR_IN_HEAD_MASK)
#define VOS_META_SIZE_IN_HEAD_GET(uvHandle) (((uvHandle)&VOS_META_SIZE_IN_HEAD_MASK) << VOS_META_SLICE_MIN_LG)
#define VOS_META_HANDLE_IN_HEAD_SET(pstPiece, uvLen) ((uintptr_t)(pstPiece) | ((uvLen) >> VOS_META_SLICE_MIN_LG))

#define VOS_META_HALIGN(uvValue, uvAlign) (((size_t)(uvValue) + (size_t)(uvAlign) - 1UL) & (~((size_t)(uvAlign) - 1UL)))
#define VOS_META_LALIGN(uvValue, uvAlign) ((size_t)(uvValue) & (~((size_t)(uvAlign) - 1UL)))

#define VOS_META_BIT_TO_ADDR(pStart, uiBit) ((void *)((uintptr_t)(pStart) + ((uiBit) << VOS_META_BLOCK_MIN_LG)))

#define VOS_META_PIECE_SIZE_ADJUST_AND_ALIGN(uvSize) \
    VOS_META_HALIGN((size_t)(uvSize) + sizeof(VOS_META_PIECE_S), VOS_META_BLOCK_MIN)

#define VOS_META_PIECE_ADDR_OFFSET_AND_ALIGN(pAddr, uiHeadLen) \
    VOS_META_HALIGN((uintptr_t)(pAddr) + (uiHeadLen), VOS_META_BLOCK_MIN)

#define VOS_META_ADDR_TO_BIT(pStart, pAddr, uvSize, uiBitStart, uiBitEnd)    \
    do {                                                                     \
        /* 每一个bit代表1k的内存范围 */                            \
        (uiBitStart) = ((uintptr_t)(pAddr) - (uintptr_t)(pStart)) >> 10; \
        (uiBitEnd) = (uiBitStart) + ((uvSize) >> 10);                        \
    } while (0)

#define VOS_META_SLICE_POP(pstSliceCb, pAddr)                 \
    do {                                                      \
        (pAddr) = (pstSliceCb)->pstFree;                      \
        (pstSliceCb)->pstFree = (pstSliceCb)->pstFree->pNext; \
        (pstSliceCb)->uiFreeNum--;                            \
    } while (0)

typedef enum tagMetaBlockPos {
    META_BLOCK_POS_HEAD = 0, /* 位于连续的block内存的头 */
    META_BLOCK_POS_MEDIUM = 1, /* 位于连续的block内存的中间 */
    META_BLOCK_POS_TAIL = 2, /* 位于连续的block内存的尾部 */
} VOS_META_BLOCK_POS_E;

#pragma pack(1)
typedef struct tagVosMetaNodeInfo {
    uint32_t uiAlloced : 1;                /* 内存是否空闲的标志位 */
    uint32_t uiSliced : 1;                 /* 区分内存是block内存还是slice内存的标志位 */
    uint32_t uiReserve : 6;                /* 空闲遗留比特位 */
    union {
        uint32_t uiSliceType : 24;         /* 为slice内存时，表示slice内存在astSliceCb对应的下标 */
        uint32_t uiBlockUnitNum : 24;      /* 为block内存时，表示block内存包含多少个连续的单元，1单元为1k */
    } unVosMetaMemLen;
} VOS_META_NODE_S;
#pragma pack()

typedef struct tagVosMetaPiece {
    uint32_t uiUsedSize;
    VOS_META_NODE_S astMetaUnit[0];
} VOS_META_PIECE_S;

typedef struct tagVosMetaBlockNode {
    CSTL_LIST_HEAD_S stLinkNode;
    size_t uvLen;
} VOS_META_BLOCK_NODE;

typedef struct tagVosMetaSliceNode {
    struct tagVosMetaSliceNode *pNext;
} VOS_META_SLICE_NODE;

typedef struct tagVosMetaSliceCb {
    CSTL_LIST_HEAD_S stLinkNode;
    uint32_t uiFreeNum;
    VOS_META_SLICE_NODE *pstFree;
    VOS_META_PIECE_S *pstPiece;
} VOS_META_SLICE_CB;

typedef struct tagVosMetaSlice {
    VOS_META_SLICE_CB *pstCurBlock;
    size_t uvBlockRmask;
    uint16_t usUnitLen;
    uint16_t usBlockLen;
    uint16_t usNumPerBlock;
    uint16_t usReserved;
    CSTL_LIST_HEAD_S stEmptyBlock; /* 空闲Block，无可分配的Slice */
    CSTL_LIST_HEAD_S stBusyBlock;  /* 忙碌Block，内含可分配的Slice */
} VOS_META_SLICE_S;

typedef struct tagVosMetaMemStat {
    size_t uvTotalSize;
    size_t uvFreeSize;
} VOS_META_MEM_STAT_S;

typedef struct tagVosMetaMem {
    uint32_t auiFreeMap[VOS_META_BLOCK_BIT_GROUPS];      /* block内存的map表，1代表对应的大小的block内存有空闲 */
    VOS_META_BLOCK_NODE *apstBlock[VOS_META_BLOCK_TYPE]; /* 各类block内存 */
    VOS_META_SLICE_S astSliceCb[VOS_META_SLICE_TYPE];    /* 各类slice内存 */
    VOS_META_MEM_STAT_S stStat;
    OSAL_LOCK_T stLock;
} VOS_META_MEM_S;

typedef enum tagVosMetaOperType {
    VOS_META_OPER_ALLOC,
    VOS_META_OPER_FREE,
    VOS_META_OPER_SLICE,
} VOS_META_OPER_TYPE_E;

extern HIDDEN void *VosMetaAlloc(size_t uvSize);
extern HIDDEN void *VosMetaAllocAlign(size_t uvSize, size_t uvAlign);
extern HIDDEN void VosMetaFree(void *pAddr);
extern HIDDEN uint32_t VosMetaInit(void);
extern HIDDEN VOS_META_MEM_S g_stMetaMem;

#ifdef __cplusplus
}
#endif /* __cpluscplus */

#endif
