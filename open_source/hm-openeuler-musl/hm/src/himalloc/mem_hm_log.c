/**
 * @file mem_hm_log.c
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * @brief mem_hm Memory health management common module, which is responsible for enabling and disabling the memory
 * damage, leakage, and statistics collection submodules and printing logs.
 * @details
 * @author anonymous
 * @date 2021-03-11
 * @version v1.0.0
 * *******************************************************************************************
 * @par Modify log:
 * <table>
 * <tr><th>Date <th>Version <th>Author <th>Description
 * <tr><td>2021-03-11 <td>1.0.0 <td>anonymous <td> initial version
 * </table>
 *
 * *******************************************************************************************
 */
#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "mem_hm_log_inner.h"
#include "mem_log.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

#define MEM_HM_LOG_MAX_LENGTH 1024
#define ERROR_INFO_LEN 18

/* 内存健康管理CBB默认的日志打印钩子函数是MemHMDefaultLogWrite */
static MemHMLogFunc g_memHMLogFunc;

/**
 * @ingroup mem_hm
 * @brief 内存健康管理CBB默认的日志打印函数，输出到屏幕。
 * @par 描述：内存健康管理CBB默认的日志打印函数，输出到屏幕。
 * @attention
 * @param level      [IN] 日志等级
 * @param errorCode  [IN] 错误码
 * @param fmt        [IN] 字符串信息
 * @retval 无
 * @par 依赖：无。
 * @see MemHMLogFuncReg
 */
static void MemHMDefaultLogWrite(int32_t level, int32_t errorCode,
    const char *fmt, ...)
{
    va_list ap;
    int32_t ret;
    char output[MEM_HM_LOG_MAX_LENGTH];

	va_start(ap, fmt);
	ret = vsnprintf(output, MEM_HM_LOG_MAX_LENGTH, fmt, ap);
	va_end(ap);

	if (ret < 0) {
		(void)write(STDERR_FILENO, "vsnprintf fail.\n", ERROR_INFO_LEN);
		return;
	}

	(void)write(STDOUT_FILENO, output, (size_t)ret);
	return;
}

void MemHMLogFuncReg(MemHMLogFunc logFunc)
{
    g_memHMLogFunc = logFunc;
    return;
}

MemHMLogFunc MemHMLogFuncGet(void)
{
    return g_memHMLogFunc;
}

void __attribute__((constructor)) MemHMLogInit(void)
{
    MemHMLogFuncReg(MemHMDefaultLogWrite);
}

#ifdef __cplusplus
}
#endif /* __cpluscplus */
