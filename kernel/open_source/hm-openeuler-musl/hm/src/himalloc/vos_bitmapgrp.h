/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#ifndef VOS_BITMAPGRP_H
#define VOS_BITMAPGRP_H

#include <stdbool.h>
#include <stdint.h>
#include "himalloc_inner.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 单个group的bit数的对数 */
#define VOS_BITMAP_LG_BITS 5

/* 单个group的bit数的数量 */
#define VOS_BITMAP_NBITS (1U << VOS_BITMAP_LG_BITS)

/* 单个group的bit数的掩码 */
#define VOS_BITMAP_NBITS_MASK (VOS_BITMAP_NBITS - 1U)

#define VOS_BITMAP_IS_BIT_SET(auiBitmap, uiBitGrpNum, usIndex) \
    (((auiBitmap)[(usIndex) >> VOS_BITMAP_LG_BITS] & (1UL << ((usIndex) & VOS_BITMAP_NBITS_MASK))) != 0U)

/* bitmap 相关的信息 */
typedef struct tagBitmapInfo {
    uint16_t usGroupNum;
    uint16_t usTotalBit;
} VOS_BITMAP_INFO;

static HIMALLOC_ALWAYS_INLINE uint32_t VosBitmapFFSBinarySearch(uint32_t uiSize)
{
    uint32_t uiIndex = 0U;
    uint32_t uiSizeTmp = uiSize;

    if ((uiSizeTmp & 0xFFFFU) == 0U) {
        uiSizeTmp >>= 16U;
        uiIndex += 16U;
    }
    if ((uiSizeTmp & 0xFFU) == 0U) {
        uiSizeTmp >>= 8U;
        uiIndex += 8U;
    }
    if ((uiSizeTmp & 0xFU) == 0U) {
        uiSizeTmp >>= 4U;
        uiIndex += 4U;
    }
    if ((uiSizeTmp & 0x3U) == 0U) {
        uiSizeTmp >>= 2U;
        uiIndex += 2U;
    }
    if ((uiSizeTmp & 0x1U) == 0U) {
        uiIndex += 1U;
    }
    return uiIndex;
}

/* 查找最低置1的比特所在的位，最低比特位0 */
static HIMALLOC_ALWAYS_INLINE uint32_t VosBitmapFFSImpl(uint32_t uiSize)
{
    /* gcc编译器可使用自带库函数 */
#if (__WORDSIZE == 64)

    return (uint32_t)__builtin_ctz(uiSize);
#else
    /* 非gcc编译器使用二分位移寻找msb */
    return VosBitmapFFSBinarySearch(uiSize);
#endif
}

static HIMALLOC_ALWAYS_INLINE uint32_t VosBitmapFLSBinarySearch(uint32_t uiSize)
{
    uint32_t uiIndex = 1U;
    uint32_t uiSizeTmp = uiSize;

    if ((uiSizeTmp >> 16U) == 0U) {
        uiIndex += 16U;
        uiSizeTmp <<= 16U;
    }

    if ((uiSizeTmp >> 24U) == 0U) {
        uiIndex += 8U;
        uiSizeTmp <<= 8U;
    }

    if ((uiSizeTmp >> 28U) == 0U) {
        uiIndex += 4U;
        uiSizeTmp <<= 4U;
    }

    if ((uiSizeTmp >> 30U) == 0U) {
        uiIndex += 2U;
        uiSizeTmp <<= 2U;
    }

    uiIndex -= (uiSizeTmp >> 31U);
    return 31U - uiIndex;
}

/* 查找最高置1的比特所在的位，最低比特位0 */
static HIMALLOC_ALWAYS_INLINE uint32_t VosBitmapFLSImpl(uint32_t uiSize)
{
#if (__WORDSIZE == 64)
    return (uint32_t)VOS_BITMAP_NBITS - 1U - (uint32_t)__builtin_clz(uiSize);
#else
    return VosBitmapFLSBinarySearch(uiSize);
#endif
}

/* BIT数组的初始化工作 */
static HIMALLOC_ALWAYS_INLINE void VosBitmapInit(uint32_t *auiBitmap, uint32_t uiBitMapSize,
    VOS_BITMAP_INFO *pstBitmapInfo)
{
    uint32_t uiIndex;
    uint32_t uiTotalBit;
    uint32_t uiDropBit;
    uint16_t usGroupNum;

    usGroupNum = pstBitmapInfo->usGroupNum;

    /* 超过bitmap支持的最大位数则将所有位置1，多出的位丢弃 */
    if (usGroupNum > uiBitMapSize) {
        usGroupNum = (uint16_t)uiBitMapSize;
        pstBitmapInfo->usGroupNum = (uint16_t)uiBitMapSize;
        pstBitmapInfo->usTotalBit = (uint16_t)(uiBitMapSize << VOS_BITMAP_LG_BITS);
    }

    for (uiIndex = 0U; uiIndex < usGroupNum; uiIndex++) {
        auiBitmap[uiIndex] = HIMALLOC_NULL_DWORD;
    }

    uiTotalBit = (uint32_t)usGroupNum << VOS_BITMAP_LG_BITS;
    uiDropBit = uiTotalBit - pstBitmapInfo->usTotalBit;
    auiBitmap[usGroupNum - 1U] >>= uiDropBit;

    return;
}

/* 测试并置相应的BIT位 */
static HIMALLOC_ALWAYS_INLINE bool VosBitmapCheckSet(uint32_t *auiBitmap, uint32_t uiBitGrpNum, uint16_t usIndex)
{
    uint16_t usGroupId = usIndex >> VOS_BITMAP_LG_BITS;
    bool bRet = true;

    (void)uiBitGrpNum;

    /* 已设置BIT位返回false */
    if ((auiBitmap[usGroupId] & ((uint32_t)1 << (usIndex & VOS_BITMAP_NBITS_MASK))) != 0U) {
        bRet = false;
    } else {
        /* 否则设置相应的BIT位 */
        auiBitmap[usGroupId] |= ((uint32_t)1 << (usIndex & VOS_BITMAP_NBITS_MASK));
    }
    return bRet;
}

/* 清除第一个置1的比特 */
static HIMALLOC_ALWAYS_INLINE uint32_t VosBitmapCFS(uint32_t *auiBitmap, uint32_t uiBitGrpNum)
{
    uint32_t uiGroupId;
    uint32_t uiFirstSet;
    uint32_t uiBitMap = HIMALLOC_NULL_DWORD;

    for (uiGroupId = 0U; uiGroupId < uiBitGrpNum; ++uiGroupId) {
        if (auiBitmap[uiGroupId] != 0U) {
            uiFirstSet = VosBitmapFFSImpl(auiBitmap[uiGroupId]);
            auiBitmap[uiGroupId] &= (~((uint32_t)1 << uiFirstSet));
            uiBitMap = ((uiGroupId << VOS_BITMAP_LG_BITS) + uiFirstSet);
            break;
        }
    }

    return uiBitMap;
}

/* 清除最高一个置1的比特 */
static HIMALLOC_ALWAYS_INLINE uint32_t VosBitmapCLS(uint32_t *auiBitmap, uint32_t uiBitGrpNum)
{
    uint32_t uiGroupId;
    uint32_t uiLastSet;
    uint32_t uiBitMap = HIMALLOC_NULL_DWORD;

    /*
     * 由于有符号的扩展性能比较差，因此GoupId不再逆序获取，
     * 即GoupId不再从高到低获取，避免uiGroupId声明为int32_t
     */
    for (uiGroupId = 0U; uiGroupId < uiBitGrpNum; ++uiGroupId) {
        if (HIMALLOC_LIKELY(auiBitmap[uiGroupId] != 0U)) {
            uiLastSet = VosBitmapFLSImpl(auiBitmap[uiGroupId]);
            auiBitmap[uiGroupId] &= (~((uint32_t)1 << uiLastSet));
            uiBitMap = ((uiGroupId << VOS_BITMAP_LG_BITS) + uiLastSet);
            break;
        }
    }

    return uiBitMap;
}

#ifdef __cplusplus
}
#endif

#endif
