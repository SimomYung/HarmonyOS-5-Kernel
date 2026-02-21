/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: enhance log records for fsck-tools
 * Create: 2024-3-4
*/
#ifndef __F2FS_DFX_COMMON_H__
#define __F2FS_DFX_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) x
#elif defined(__cplusplus)
# define UNUSED(x)
#else
# define UNUSED(x) x
#endif

/*
 * for now, only fsck records log. If you want to add slog/klog to other
 * components, you should:
 *   1. add new log type and tag to logType and log_tag;
 *   2. add case in init_log_info();
 *   3. add SlogInit/SlogExit to corresponding main().
 */
enum LogType {
    LOG_TYP_NONE,
    LOG_TYP_FSCK,
    LOG_TYP_DUMP,
    LOG_TYP_DEFRAG,
    LOG_TYP_RESIZE,
    LOG_TYP_MKFS,
    LOG_TYP_MAX,
};

#ifdef __cplusplus
}
#endif
#endif
