/**
 * @file mem_log.h
 * @copyright Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * @brief Memory adapter allocator_byhandle external header file.
 * @details \n
 * This header file is based on various memory allocation algorithms, the memory interface provided by the musl library,
 * and the DFX debugging function (DEBUG version).\n To facilitate user use, abstract and abstract memory adapter
 * interfaces. Compared with allocator, the allocator provides more extended interfaces.
 * @author anonymous
 * @date 2022-11-09
 * @version v1.0.0
 * *******************************************************************************************
 * @par Modify log:
 * <table>
 * <tr><th>Date <th>Version <th>Author <th>Description
 * <tr><td>2022-11-09 <td>1.0.0 <td>anonymous <td> initial version
 * </table>
 *
 * *******************************************************************************************
 */

/**
 * @defgroup Memory log
 * @ingroup Memory log
 */

#ifndef MEM_LOG_H
#define MEM_LOG_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup mem_log
 * @enum mem_hm_log_level_e
 * @brief Log Level Enumeration
 */
typedef enum tag_mem_hm_log_level {
	MEM_HM_LOG_LEVEL_INFO = 0,
	MEM_HM_LOG_LEVEL_WARNING = 1,
	MEM_HM_LOG_LEVEL_ERROR = 2,
	MEM_HM_LOG_LEVEL_EXCEPT = 3,
	MEM_HM_LOG_LEVEL_BUTT
} mem_hm_log_level_e;

/**
 * @ingroup mem_log
 * @brief No error occurs when the log is recorded.
 */
#define NORMAL_NO (0)

/**
 * @ingroup mem_log
 * @brief Error code information during log recording. The memory is damaged.
 */
#define ERRNO_DAMAGE_HAPPEN (-1)

/**
 * @ingroup mem_log
 * @brief Error code information during log recording, indicating that the memory fails to be applied for.
 */
#define ERRNO_ALLOC_WRONG (-2)

/**
 * @ingroup mem_log
 * @brief Error code information during log recording. The security function fails to be run.
 */
#define ERRNO_SECUREC_WRONG (-3)

/**
 * @ingroup mem_log
 * @brief Error code information recorded in logs. Traversing the interface fails.
 */
#define ERRNO_TRAVERSE_WRONG (-4)

typedef void (*MemHMLogFunc)(int32_t level, int32_t errorCode,
			     const char *fmt, ...);

void MemHMLogFuncReg(MemHMLogFunc logFunc);

#ifdef __cplusplus
}
#endif

#endif
