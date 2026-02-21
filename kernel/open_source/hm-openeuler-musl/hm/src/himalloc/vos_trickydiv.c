/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#include "vos_trickydiv.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

HIDDEN uint32_t VosTrickyDivInit(uint32_t uiDivisor, VOS_TRICKY_DIV_INFO *pstDivInfo)
{
    uint64_t ullTwoToK;
    uint32_t uiMagicNum;

    if (uiDivisor == 0) {
        __hmattr_sideff_resolved("no relative side effect");
        return HIMALLOC_ERRNO_INVAL;
    }

    ullTwoToK = (uint64_t)(1ULL << 32); /* 32在本处通俗易懂 */
    uiMagicNum = (uint32_t)(ullTwoToK / uiDivisor);
    if (ullTwoToK % uiDivisor != 0) {
        uiMagicNum++;
    }
    pstDivInfo->uiMagicNum = uiMagicNum;

    return HIMALLOC_OK;
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */
