/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#ifndef VOS_MEMZONE_H
#define VOS_MEMZONE_H

#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include "cstl_list.h"
#include "himalloc_inner.h"
#include "vos_himallocio.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

#define VOS_MEM_ZONE_MAX_UNIT 256

#if (__WORDSIZE == 32)
#define VOS_MEM_ZONE_MIN_UNIT 4
#else
#define VOS_MEM_ZONE_MIN_UNIT 8
#endif

#define VOS_MEM_ZONE_INVALID_T HIMALLOC_NULL_PTR

/* MemZone老化策略 */
#define VOS_MEM_ZONE_DECAY_COUNT 128
#define VOS_MEM_ZONE_DECAY_TIME_MS 180000

/* 秒，微秒，纳秒间的转换倍数 */
#define SECONDE_TO_MILLISECOND 1000
#define MILLISECOND_TO_NANOSECOND 1000000
#define SECONDE_TO_NANOSECOND 1000000000

#define VOS_MEM_ZONE_BLOCK_SLICE_TIME 16U
#define VOS_MEM_ZONE_BLOCK_LEN_DEFAULT (64UL << 10)
#define VOS_MEM_ZONE_BLOCK_LEN                                                       \
    (((size_t)VOS_MEM_ZONE_BLOCK_LEN_DEFAULT >= (size_t)sysconf(_SC_PAGESIZE)) ?     \
        (size_t)VOS_MEM_ZONE_BLOCK_LEN_DEFAULT :                                     \
        (size_t)sysconf(_SC_PAGESIZE))
#define VOS_MEM_ZONE_MAX_NUM_PER_BLOCK                         \
    ((VOS_MEM_ZONE_BLOCK_LEN - sizeof(VOS_MEM_ZONE_BLOCK_S)) / \
        sizeof(VOS_MEM_ZONE_SLICE_S))

typedef struct tagMemZoneSlice {
    struct tagMemZoneSlice *pNext;
} VOS_MEM_ZONE_SLICE_S;

typedef struct tagMemZoneBlock {
    uint32_t uiFreeNum;
    uint32_t uiSlicedBlockLen;       /* 已切割Block的大小 */
    CSTL_LIST_HEAD_S stLinkNode;
    VOS_MEM_ZONE_SLICE_S *pstFree;
    struct timespec stReturnTime;
} VOS_MEM_ZONE_BLOCK_S;

typedef struct tagMemZoneDecay {
    uint32_t uiDecayCount;  /* 释放多少个Block检测一次老化 */
    uint32_t uiDecayTimeMs; /* Block归还操作系统的老化时间ms */
} VOS_MEM_ZONE_DECAY_S;

typedef struct tagMemZone {
    VOS_MEM_ZONE_BLOCK_S *pstCurBlock; /* 当前正在分配slice的Block */
    size_t uvBlockRmask;           /* Block长度的反向掩码 */
    uint16_t usNumPerBlock;
    uint16_t usUnitLen;
    uint32_t uiBlockLen;   /* Block的大小，必须是2幂次方 */
    int32_t iNextCheckNum; /* 释放多少个Block后检测老化时间 */
    uint32_t uiReserve;
    VOS_MEM_ZONE_DECAY_S stDecayTime; /* 释放Block的老化机制 */
    CSTL_LIST_HEAD_S stEmptyBlock;     /* 空闲Block，无可分配的Slice */
    CSTL_LIST_HEAD_S stBusyBlock;      /* 忙碌Block，内含可分配的Slice */
    CSTL_LIST_HEAD_S stReturnBlock;    /* 根据归还时间排序的ReturnBlock */
} VOS_MEM_ZONE_S;

typedef struct tagMemZoneUseInfo {
    size_t uvTotalSize;
    size_t uvUsedSize;
    size_t uvTotalNum;
    size_t uvUsedNum;
} VOS_MEM_ZONE_USE_INFO_S;

typedef void *VOS_MEM_ZONE_T;

typedef uint32_t (*VOS_MEM_ZONE_WALK_HOOK)(const VOS_MEM_ZONE_T zone, void *pPara, void *pSlice);

extern HIDDEN VOS_MEM_ZONE_T VosMemZoneInit(uint32_t uiUnitLen, const VOS_MEM_ZONE_DECAY_S *pstDecayTime);

/* 单个zone的并发性由调用者保证 */
extern HIDDEN void *VosMemZoneAlloc(VOS_MEM_ZONE_T zone);

/* 单个zone的并发性由调用者保证 */
extern HIDDEN uint32_t VosMemZoneFree(VOS_MEM_ZONE_T zone, void *pAddr);

extern HIDDEN uint32_t VosMemZoneWalk(VOS_MEM_ZONE_T zone, void *pPara, VOS_MEM_ZONE_WALK_HOOK pfnHook);

#ifdef __cplusplus
}
#endif /* __cpluscplus */

#endif
