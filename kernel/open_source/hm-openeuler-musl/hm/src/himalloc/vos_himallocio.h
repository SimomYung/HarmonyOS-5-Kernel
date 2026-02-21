/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#ifndef VOS_HIMALLOCIO_H
#define VOS_HIMALLOCIO_H

#include "himalloc_algo.h"
#include "himalloc_inner.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

extern HIDDEN VosHimallocLogFunc g_pfnHimallocLogHook;

extern HIDDEN void VosHimallocLogDefaultFunc(uint32_t uiLevel, uint32_t uiIntCode, const char *pscFmt, ...);

#define VosHimallocLogWrite(uiError, args...)                                                                  \
    do {                                                                                                       \
        if (g_pfnHimallocLogHook != NULL) {                                                                    \
            g_pfnHimallocLogHook((uint32_t)VOS_HIMALLOC_LOG_LEVEL_ERROR, uiError, ##args); \
        }                                                                                                      \
    } while (0)

#ifdef __cplusplus
}
#endif /* __cpluscplus */

#endif
