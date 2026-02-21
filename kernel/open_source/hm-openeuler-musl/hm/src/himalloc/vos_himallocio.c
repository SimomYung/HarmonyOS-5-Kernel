/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 */
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include "vos_himallocio.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

#ifdef CONFIG_DEBUG_BUILD
HIDDEN VosHimallocLogFunc g_pfnHimallocLogHook = VosHimallocLogDefaultFunc;
#else
HIDDEN VosHimallocLogFunc g_pfnHimallocLogHook = NULL;
#endif

HIDDEN void VosHimallocLogDefaultFunc(uint32_t uiLevel, uint32_t uiIntCode,
    const char *pscFmt, ...)
{
    char buf[256]; /* 256 代表字符缓存池长度 */
    va_list ap;
    int ret;

    (void)uiLevel;
    (void)uiIntCode;

    va_start(ap, pscFmt);
    ret = vsnprintf(buf, 256, pscFmt, ap); /* 256 代表字符缓存池长度 */
    va_end(ap);
    if (ret > 0) {
        (void)write(STDOUT_FILENO, buf, (size_t)ret);
    } else {
        (void)write(STDOUT_FILENO, "vsnprintf failed\n", 17); /* 17 代表失败信息字符长度 */
    }
}

HIDDEN void VosHimallocLogFuncReg(VosHimallocLogFunc pfnHook)
{
    g_pfnHimallocLogHook = pfnHook;
    return;
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */
