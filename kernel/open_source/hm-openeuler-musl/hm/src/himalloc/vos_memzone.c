/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include "vos_metaalloc.h"
#include "vos_memzone.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

HIDDEN VOS_MEM_ZONE_T VosMemZoneInit(uint32_t uiUnitLen, const VOS_MEM_ZONE_DECAY_S *pstDecayTime)
{
    VOS_MEM_ZONE_S *pstZone;

    if ((pstDecayTime == HIMALLOC_NULL_PTR) || (uiUnitLen < sizeof(VOS_MEM_ZONE_SLICE_S)) ||
        (uiUnitLen > VOS_MEM_ZONE_MAX_UNIT)) {
        __hmattr_sideff_resolved("no relative side effect");
        return VOS_MEM_ZONE_INVALID_T;
    }

    pstZone = (VOS_MEM_ZONE_S *)VosMetaAlloc(sizeof(VOS_MEM_ZONE_S));
    if (pstZone == HIMALLOC_NULL_PTR) {
        VosHimallocLogWrite(HIMALLOC_ERRNO_NOMEM, "[HIMALLOC-%s]system memory is not enough, malloc size(%lu)",
            __FUNCTION__, sizeof(VOS_MEM_ZONE_S));
        __hmattr_sideff_resolved("no relative side effect");
        return VOS_MEM_ZONE_INVALID_T;
    }

    /* 初始化成员变量 */
    (void)memset(pstZone, 0, sizeof(VOS_MEM_ZONE_S));
    pstZone->uiBlockLen = (uint32_t)VOS_MEM_ZONE_BLOCK_LEN;
    pstZone->usUnitLen = (uint16_t)uiUnitLen;
    pstZone->usNumPerBlock = (uint16_t)((pstZone->uiBlockLen - sizeof(VOS_MEM_ZONE_BLOCK_S)) / uiUnitLen);
    pstZone->uvBlockRmask = (~(((uintptr_t)pstZone->uiBlockLen) - 1U));
    pstZone->iNextCheckNum = 0;
    pstZone->uiReserve = 0;

    pstZone->stDecayTime.uiDecayCount = pstDecayTime->uiDecayCount;
    pstZone->stDecayTime.uiDecayTimeMs = pstDecayTime->uiDecayTimeMs;

    CSTL_ListInit(&(pstZone->stBusyBlock));
    CSTL_ListInit(&(pstZone->stEmptyBlock));
    CSTL_ListInit(&(pstZone->stReturnBlock));

    return (VOS_MEM_ZONE_T)pstZone;
}

static void VosMemZoneShrink(VOS_MEM_ZONE_S *pstZone, VOS_MEM_ZONE_BLOCK_S *pstBlock)
{
    (void)munmap(pstBlock, pstZone->uiBlockLen);
    return;
}

static void VosMemZoneSlicer(const VOS_MEM_ZONE_S *pstZone, VOS_MEM_ZONE_BLOCK_S *pstBlock)
{
    uint16_t usUnitLen;
    uintptr_t uvCutAddr;
    uintptr_t uvEndAddr;
    VOS_MEM_ZONE_SLICE_S *pstLastSlice;
    uint32_t uiSlicedBlockLen = pstBlock->uiSlicedBlockLen;
    uintptr_t uvSlicedEndAddr;
    uint32_t uiNum = 0;

    /* zone初始化的时候可以确保至少可以切割一个slice */
    uvEndAddr = (uintptr_t)pstBlock + pstZone->uiBlockLen;
    pstLastSlice = (VOS_MEM_ZONE_SLICE_S *)((uintptr_t)pstBlock + uiSlicedBlockLen);
    pstBlock->pstFree = pstLastSlice;

    uvSlicedEndAddr = (uintptr_t)pstBlock + uiSlicedBlockLen + (pstZone->uiBlockLen / VOS_MEM_ZONE_BLOCK_SLICE_TIME);
    uvSlicedEndAddr = (uvSlicedEndAddr < uvEndAddr) ? uvSlicedEndAddr : uvEndAddr;

    /* 将Block切割成slice */
    usUnitLen = pstZone->usUnitLen;
    while ((uvCutAddr = (uintptr_t)pstLastSlice + usUnitLen) <= uvSlicedEndAddr) {
        pstLastSlice->pNext = (VOS_MEM_ZONE_SLICE_S *)uvCutAddr;
        pstLastSlice = (VOS_MEM_ZONE_SLICE_S *)uvCutAddr;
        uiSlicedBlockLen += usUnitLen;
        uiNum++;
    }
    pstLastSlice = (VOS_MEM_ZONE_SLICE_S *)((uintptr_t)pstLastSlice - usUnitLen);
    pstLastSlice->pNext = HIMALLOC_NULL_PTR;

    pstBlock->uiFreeNum += uiNum;
    pstBlock->uiSlicedBlockLen = uiSlicedBlockLen;

    return;
}

static uintptr_t VosMemZoneTrim(uintptr_t uvStart, uintptr_t uvEnd, size_t uvSize, size_t uvAlign)
{
    uintptr_t uvStartAligned;
    uintptr_t uvEndAligned;

    if ((uvSize + uvAlign) > (uvEnd - uvStart)) {
        __hmattr_sideff_resolved("no relative side effect");
        return 0UL;
    }

    uvStartAligned = (uvStart + uvAlign - 1U) & ~((uintptr_t)uvAlign - 1U);
    /* 头部向后偏移小于保护页大小，取下一个对齐的地址 */
    if (uvStart > uvStartAligned) {
        uvStartAligned += uvAlign;
    }

    /* 释放地址对齐后，前后多余的内存 */
    if (uvStartAligned > uvStart) {
        (void)munmap(ulong_to_ptr(uvStart, void), uvStartAligned - uvStart);
    }
    uvEndAligned = uvStartAligned + uvSize;
    if (uvEnd > uvEndAligned) {
        (void)munmap(ulong_to_ptr(uvEndAligned, void), uvEnd - uvEndAligned);
    }

    return uvStartAligned;
}

static void *VosMemZoneMmapGrow(uint32_t uiAlign, uint32_t uiSize)
{
    uintptr_t uvAddrTmp;
    uintptr_t uvAddr;
    uintptr_t uvEndTmp;

    uvAddrTmp = ptr_to_ulong(mmap(HIMALLOC_NULL_PTR, (size_t)uiSize + uiAlign,
        (int32_t)((uint32_t)PROT_READ | (uint32_t)PROT_WRITE),
        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
    if (ulong_to_ptr(uvAddrTmp, void) == MAP_FAILED) {
        VosHimallocLogWrite(HIMALLOC_ERRNO_NOMEM, "[HIMALLOC-%s]system memory is not enough, size(%u)",
            __FUNCTION__, uiSize + uiAlign);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }
    uvEndTmp = uvAddrTmp + uiSize + uiAlign;

    uvAddr = VosMemZoneTrim(uvAddrTmp, uvEndTmp, uiSize, uiAlign);

    return ulong_to_ptr(uvAddr, void);
}

static void *VosMemZoneGrowAlign(uint32_t uiAlign, uint32_t uiSize)
{
    return VosMemZoneMmapGrow(uiAlign, uiSize);
}

static VOS_MEM_ZONE_BLOCK_S *VosMemZoneGrow(VOS_MEM_ZONE_S *pstZone)
{
    VOS_MEM_ZONE_BLOCK_S *pstBlock;

    pstBlock = (VOS_MEM_ZONE_BLOCK_S *)VosMemZoneGrowAlign(pstZone->uiBlockLen, pstZone->uiBlockLen);
    if (pstBlock == HIMALLOC_NULL_PTR) {
        VosHimallocLogWrite(HIMALLOC_ERRNO_NOMEM, "[HIMALLOC-%s]system memory is not enough, size(%u)",
            __FUNCTION__, pstZone->uiBlockLen);
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }

    /* 初始化成员变量 */
    pstBlock->uiFreeNum = 0; /* 切割slice时会重新初始化 */
    pstBlock->pstFree = HIMALLOC_NULL_PTR;
    CSTL_ListInit(&(pstBlock->stLinkNode));
    pstBlock->uiSlicedBlockLen = (uint32_t)sizeof(VOS_MEM_ZONE_BLOCK_S);

    /* 将Block切割Slice */
    VosMemZoneSlicer(pstZone, pstBlock);

    return pstBlock;
}

static VOS_MEM_ZONE_BLOCK_S *VosMemZoneBlockGet(VOS_MEM_ZONE_S *pstZone)
{
    VOS_MEM_ZONE_BLOCK_S *pstFreeBlock;

    if (!CSTL_ListIsEmpty(&(pstZone->stBusyBlock))) {
        pstFreeBlock = CSTL_LIST_ENTRY(pstZone->stBusyBlock.next, VOS_MEM_ZONE_BLOCK_S, stLinkNode);
        CSTL_ListRemove(&(pstFreeBlock->stLinkNode));
        __hmattr_sideff_resolved("no relative side effect");
        return pstFreeBlock;
    }

    if (!CSTL_ListIsEmpty(&(pstZone->stReturnBlock))) {
        /* 取尾部节点提升首部节点释放的概率 */
        pstFreeBlock = CSTL_LIST_ENTRY(pstZone->stReturnBlock.prev, VOS_MEM_ZONE_BLOCK_S, stLinkNode);
        CSTL_ListRemove(&(pstFreeBlock->stLinkNode));

        if (CSTL_ListIsEmpty(&(pstZone->stReturnBlock))) {
            pstZone->iNextCheckNum = 0;
        }
        __hmattr_sideff_resolved("no relative side effect");
        return pstFreeBlock;
    }

    return VosMemZoneGrow(pstZone);
}

HIDDEN void *VosMemZoneAlloc(VOS_MEM_ZONE_T zone)
{
    VOS_MEM_ZONE_S *pstZone;
    VOS_MEM_ZONE_BLOCK_S *pstCurBlock;
    VOS_MEM_ZONE_SLICE_S *pstCurSlice;

    pstZone = (VOS_MEM_ZONE_S *)zone;
    if (pstZone == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_NULL_PTR;
    }

    pstCurBlock = pstZone->pstCurBlock;
    if (pstCurBlock == HIMALLOC_NULL_PTR) {
        pstCurBlock = VosMemZoneBlockGet(pstZone);
        if (pstCurBlock == HIMALLOC_NULL_PTR) {
            __hmattr_sideff_resolved("no relative side effect");
            return HIMALLOC_NULL_PTR;
        }
        pstZone->pstCurBlock = pstCurBlock;
    }

    pstCurSlice = pstCurBlock->pstFree;
    pstCurBlock->pstFree = pstCurSlice->pNext;
    pstCurBlock->uiFreeNum--;

    if (pstCurBlock->pstFree == HIMALLOC_NULL_PTR) {
        if (pstCurBlock->uiSlicedBlockLen + (uint32_t)pstZone->usUnitLen < pstZone->uiBlockLen) {
            VosMemZoneSlicer(pstZone, pstCurBlock);
        } else {
            pstZone->pstCurBlock = HIMALLOC_NULL_PTR;
            CSTL_ListAdd(&(pstCurBlock->stLinkNode), &(pstZone->stEmptyBlock));
        }
    }

    return pstCurSlice;
}

static HIMALLOC_ALWAYS_INLINE bool VosMemZoneTsCompare(const struct timespec *pstTs1, const struct timespec *pstTs2,
    uint32_t uiDecayTimeMs)
{
    bool bRet;
    if (pstTs1->tv_nsec >= pstTs2->tv_nsec) {
        bRet = (((pstTs1->tv_sec - pstTs2->tv_sec) * SECONDE_TO_MILLISECOND +
            ((pstTs1->tv_nsec - pstTs2->tv_nsec) / MILLISECOND_TO_NANOSECOND)) >= (int32_t)uiDecayTimeMs);
    } else {
        bRet = (((pstTs1->tv_sec - pstTs2->tv_sec - 1) * SECONDE_TO_MILLISECOND +
            ((SECONDE_TO_NANOSECOND + pstTs1->tv_nsec - pstTs2->tv_nsec) / MILLISECOND_TO_NANOSECOND)) >=
            (int32_t)uiDecayTimeMs);
    }
    return bRet;
}

/* clock_gettime作为posix接口不存在os适配问题,而
   CLOCK_MONOTONIC_COARSE宏是linux特有的且仅支持内核版本大于2.6.3 */
static HIMALLOC_ALWAYS_INLINE void VosMemZoneReturnTimeGet(struct timespec *pstReturnTime)
{
#if defined(CLOCK_MONOTONIC_COARSE)
    (void)clock_gettime(CLOCK_MONOTONIC_COARSE, pstReturnTime);
#elif defined(CLOCK_MONOTONIC)
    (void)clock_gettime(CLOCK_MONOTONIC, pstReturnTime);
#else
    (void)clock_gettime(CLOCK_REALTIME, pstReturnTime);
#endif
    return;
}

static void VosMemZoneDecay(VOS_MEM_ZONE_S *pstZone)
{
    VOS_MEM_ZONE_DECAY_S *pstDecayTime;
    CSTL_LIST_HEAD_S *pstNode;
    CSTL_LIST_HEAD_S *pstTempNode;
    VOS_MEM_ZONE_BLOCK_S *pstBlock;

    struct timespec stCurrentTime;

    pstDecayTime = &(pstZone->stDecayTime);
    pstZone->iNextCheckNum = (int32_t)pstDecayTime->uiDecayCount;

    /* 没有待归还的Block直接返回 */
    if (CSTL_ListIsEmpty(&(pstZone->stReturnBlock))) {
        pstZone->iNextCheckNum = 0;
        __hmattr_sideff_resolved("no relative side effect");
        return;
    }

    /* 获取当前时间并比较 */
    VosMemZoneReturnTimeGet(&stCurrentTime);

    CSTL_LIST_FOR_EACH_ITEM_SAFE(pstNode, pstTempNode, &(pstZone->stReturnBlock))
    {
        pstBlock = CSTL_LIST_ENTRY(pstNode, VOS_MEM_ZONE_BLOCK_S, stLinkNode);
        if (VosMemZoneTsCompare(&stCurrentTime, &(pstBlock->stReturnTime), pstDecayTime->uiDecayTimeMs) == false) {
            /* stReturnBlock按照老化时间排序 */
            __hmattr_sideff_resolved("no relative side effect");
            return;
        }

        /* 达到老化时间归还给操作系统 */
        CSTL_ListRemove(&(pstBlock->stLinkNode));
        VosMemZoneShrink(pstZone, pstBlock);
    }

    return;
}


static HIMALLOC_ALWAYS_INLINE void VosMemZoneReturnBlockAdd(VOS_MEM_ZONE_S *pstZone, VOS_MEM_ZONE_BLOCK_S *pstBlock)
{
    if (pstBlock == pstZone->pstCurBlock) {
        pstZone->pstCurBlock = HIMALLOC_NULL_PTR;
    } else {
        CSTL_ListRemove(&(pstBlock->stLinkNode));
    }

    if (pstZone->iNextCheckNum == 0) {
        pstZone->iNextCheckNum = (int32_t)((pstZone->stDecayTime).uiDecayCount);
    }

    if (pstZone->iNextCheckNum != 0) {
        /* 根据归还时间由近及远排序 */
        VosMemZoneReturnTimeGet(&(pstBlock->stReturnTime));
        CSTL_ListAddBefore(&(pstBlock->stLinkNode), &(pstZone->stReturnBlock));
    } else {
        /* 老化时间为0表示立即释放 */
        VosMemZoneShrink(pstZone, pstBlock);
    }

    return;
}

HIDDEN uint32_t VosMemZoneFree(VOS_MEM_ZONE_T zone, void *pAddr)
{
    VOS_MEM_ZONE_S *pstZone;
    VOS_MEM_ZONE_BLOCK_S *pstBlock;
    VOS_MEM_ZONE_SLICE_S *pstSlice;

    pstZone = (VOS_MEM_ZONE_S *)zone;

    pstBlock = (VOS_MEM_ZONE_BLOCK_S *)((ptr_to_ulong(pAddr)) & (pstZone->uvBlockRmask));
    pstSlice = (VOS_MEM_ZONE_SLICE_S *)pAddr;
    pstSlice->pNext = pstBlock->pstFree;
    pstBlock->pstFree = pstSlice;
    pstBlock->uiFreeNum++;

    if (pstZone->iNextCheckNum != 0) {
        if (--pstZone->iNextCheckNum == 0) {
            VosMemZoneDecay(pstZone);
        }
    }

    if (pstBlock->uiFreeNum == 1) {
        CSTL_ListRemove(&(pstBlock->stLinkNode));
        CSTL_ListAdd(&(pstBlock->stLinkNode), &(pstZone->stBusyBlock));
    } else if (pstBlock->uiFreeNum == pstZone->usNumPerBlock) {
        VosMemZoneReturnBlockAdd(pstZone, pstBlock);
    } else {
        /* Do nothing */
    }

    return HIMALLOC_OK;
}

static uint32_t VosMemZoneFreeFlagInit(VOS_MEM_ZONE_BLOCK_S *pstBlock, uint16_t usUnitLen, uint8_t aucFreeFlag[],
    uint32_t uiArrayLen)
{
    uintptr_t uvIndex;
    uintptr_t uvStartAddr;
    VOS_MEM_ZONE_SLICE_S *pstFreeSlice;

    if (usUnitLen == 0) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_INVAL;
    }

    /* 完成出参的初始化 */
    (void)memset(aucFreeFlag, 0, uiArrayLen);

    /* 根据序号设置flag */
    uvStartAddr = ((uintptr_t)pstBlock + sizeof(VOS_MEM_ZONE_BLOCK_S));
    pstFreeSlice = pstBlock->pstFree;
    while (pstFreeSlice != HIMALLOC_NULL_PTR) {
        uvIndex = ((uintptr_t)pstFreeSlice - uvStartAddr) / usUnitLen;
        if (uvIndex >= uiArrayLen) {
            __hmattr_sideff_resolved("no relative side effect");
            return HIMALLOC_ERRNO_INVAL;
        }

        aucFreeFlag[uvIndex] = true;
        pstFreeSlice = pstFreeSlice->pNext;
    }

    return HIMALLOC_OK;
}

static uint32_t VosMemZoneBlockWalk(VOS_MEM_ZONE_S *pstZone, VOS_MEM_ZONE_BLOCK_S *pstZoneBlock, void *pPara,
    const VOS_MEM_ZONE_WALK_HOOK pfnHook)
{
    uint16_t usUnitLen;
    uint32_t uiRet;
    uintptr_t uvEndAddr;
    uintptr_t uvIndex;
    uintptr_t uvLastSliceAddr;
    uint8_t *pucFreeFlag = HIMALLOC_NULL_PTR;

    usUnitLen = pstZone->usUnitLen;
    pucFreeFlag = (uint8_t *)VosMetaAlloc(VOS_MEM_ZONE_MAX_NUM_PER_BLOCK);
    if (pucFreeFlag == HIMALLOC_NULL_PTR) {
        return HIMALLOC_ERRNO_INVAL;
    }
    uiRet = VosMemZoneFreeFlagInit(pstZoneBlock, usUnitLen, pucFreeFlag, (uint32_t)VOS_MEM_ZONE_MAX_NUM_PER_BLOCK);
    if (uiRet != HIMALLOC_OK) {
        __hmattr_sideff_resolved("no relative side effect");
        VosMetaFree(pucFreeFlag);
        return uiRet;
    }

    uvEndAddr = (uintptr_t)pstZoneBlock + pstZoneBlock->uiSlicedBlockLen;
    uvLastSliceAddr = (uintptr_t)pstZoneBlock + sizeof(VOS_MEM_ZONE_BLOCK_S);

    uvIndex = 0;
    while ((uvLastSliceAddr + usUnitLen) <= uvEndAddr) {
        /* 说明是空闲的slice，不需要调用回调处理 */
        if (pucFreeFlag[uvIndex] == true) {
            uvIndex++;
            uvLastSliceAddr = uvLastSliceAddr + usUnitLen;
            continue;
        }

        uiRet = pfnHook(pstZone, pPara, (VOS_MEM_ZONE_SLICE_S *)uvLastSliceAddr);
        if (uiRet != HIMALLOC_OK) {
            break;
        }

        uvIndex++;
        uvLastSliceAddr = uvLastSliceAddr + usUnitLen;
    }

    VosMetaFree(pucFreeFlag);
    return uiRet;
}

static uint32_t VosMemZoneListWalk(VOS_MEM_ZONE_S *pstZone, void *pPara, const VOS_MEM_ZONE_WALK_HOOK pfnHook,
    CSTL_LIST_HEAD_S *pstListHead)
{
    uint32_t uiRet = HIMALLOC_OK;
    CSTL_LIST_HEAD_S *pstNode = HIMALLOC_NULL_PTR;
    CSTL_LIST_HEAD_S *pstTemp = HIMALLOC_NULL_PTR;
    VOS_MEM_ZONE_BLOCK_S *pstZoneBlock = HIMALLOC_NULL_PTR;

    CSTL_LIST_FOR_EACH_ITEM_SAFE(pstNode, pstTemp, pstListHead)
    {
        pstZoneBlock = CSTL_LIST_ENTRY(pstNode, VOS_MEM_ZONE_BLOCK_S, stLinkNode);

        uiRet = VosMemZoneBlockWalk(pstZone, pstZoneBlock, pPara, pfnHook);
        if (uiRet != HIMALLOC_OK) {
            break;
        }
    }

    return uiRet;
}

HIDDEN uint32_t VosMemZoneWalk(VOS_MEM_ZONE_T zone, void *pPara, VOS_MEM_ZONE_WALK_HOOK pfnHook)
{
    uint32_t uiRet;
    VOS_MEM_ZONE_S *pstZone;

    pstZone = (VOS_MEM_ZONE_S *)zone;
    if (pstZone == HIMALLOC_NULL_PTR) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_INVAL;
    }

    if (pstZone->pstCurBlock != HIMALLOC_NULL_PTR) {
        uiRet = VosMemZoneBlockWalk(pstZone, pstZone->pstCurBlock, pPara, pfnHook);
        if (uiRet != HIMALLOC_OK) {
            __hmattr_sideff_resolved("no relative side effect");
            return uiRet;
        }
    }

    uiRet = VosMemZoneListWalk(pstZone, pPara, pfnHook, &(pstZone->stBusyBlock));
    if (uiRet != HIMALLOC_OK) {
        __hmattr_sideff_resolved("no relative side effect");
        return uiRet;
    }

    uiRet = VosMemZoneListWalk(pstZone, pPara, pfnHook, &(pstZone->stEmptyBlock));
    return uiRet;
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */
