/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Livepatch log define
 * Author: Huawei OS Kernel Lab
 * Create: Mon Jun  6 09:02:47 2022
 */

#ifndef KLIBS_LIVEPATCH_LOG_H
#define KLIBS_LIVEPATCH_LOG_H

#include <lib/livepatch/livepatch_loader.h>
#include <hmkernel/klog_level.h>

#define __livepatch_log(lv, fmt, ...) \
	do { \
		struct livepatch_loader_ops_s *__lp_ops = livepatch_loader_ops(); \
		if (__lp_ops != NULL && __lp_ops->livepatch_printf != NULL) { \
			__lp_ops->livepatch_printf(lv, "[%s:%d] "fmt, __func__, __LINE__, ##__VA_ARGS__); \
		} \
	} while (__false)

#define livepatch_info(fmt...)		__livepatch_log(KLOG_INFO, fmt)
#define livepatch_warn(fmt...)		__livepatch_log(KLOG_WARN, fmt)
#define livepatch_error(fmt...)		__livepatch_log(KLOG_ERROR, fmt)

#ifdef __hmfeature_debug__
#define livepatch_debug(fmt...)		__livepatch_log(KLOG_DEBUG, fmt)
#else
static inline void __attribute__((format(printf, 1, 2)))
__livepatch_no_log(const char *fmt, ...)
{
	(void)fmt;
}
#define livepatch_debug(fmt...)		__livepatch_no_log(fmt)
#endif /* __hmfeature_debug__ */

#endif /* KLIBS_LIVEPATCH_LOG_H */
