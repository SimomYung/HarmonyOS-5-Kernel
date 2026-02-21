/**
 * @file mem_hm_log_inner.h
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * @brief mem_hm_log header file
 * @details mem_hm_log header file
 * @author Anonymous
 * @date 2022-11-07
 * @version v0.0.1
 * *******************************************************************************************
 * @par Modify log:
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022-11-07  <td>0.0.1    <td>Anonymous   <td>Initial Version
 * </table>
 * *******************************************************************************************
 */
#ifndef MEM_HM_LOG_INNER_H
#define MEM_HM_LOG_INNER_H

#include <stdio.h>
#include "mem_log.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cpluscplus */

/**
 * @ingroup mem_hm_log
 * @brief 日志输出函数宏
 * @see g_memHMLogFunc
 */
#define MemHMLogWrite(level, errorCode, args...)                   \
    do {                                                           \
        MemHMLogFunc logFunc = MemHMLogFuncGet();                  \
        if (logFunc != NULL) {                                     \
            logFunc(level, errorCode, ##args); \
        }                                                          \
    } while (0)

/**
 * @ingroup mem_hm_inner
 * @brief 获取mem_hm日志钩子函数
 * @par 获取mem_hm日志钩子函数
 * @attention
 * @param  无
 * @retval MemHMLogFunc钩子函数指针
 * @par 依赖：无。
 * @li mem_hm_inner.h：该接口声明所在的文件。
 * @see MemHMLogFunc、MemHMLogFuncReg
 */
MemHMLogFunc MemHMLogFuncGet(void);

#ifdef __cplusplus
}
#endif /* __cpluscplus */

#endif
