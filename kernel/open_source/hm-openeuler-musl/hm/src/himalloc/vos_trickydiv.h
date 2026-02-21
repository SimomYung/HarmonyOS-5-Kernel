/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#ifndef VOS_TRICKYDIV_H
#define VOS_TRICKYDIV_H

#include <stdint.h>
#include "himalloc_inner.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

typedef struct tagTrickyDivInfo {
    uint32_t uiMagicNum;
} VOS_TRICKY_DIV_INFO;

extern HIDDEN uint32_t VosTrickyDivInit(uint32_t uiDivisor, VOS_TRICKY_DIV_INFO *pstDivInfo);

static HIMALLOC_ALWAYS_INLINE uint32_t VosTrickyDiv(uint32_t uiDividend, const VOS_TRICKY_DIV_INFO *pstDivInfo)
{
    uint64_t ret = ((uint64_t)uiDividend * (uint64_t)pstDivInfo->uiMagicNum) >> 32;
    return (uint32_t)ret;
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */

#endif
