/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: UDK routines for printing
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 14 18:58:58 2019
 */
#ifndef __UDK_LOG_H__
#define __UDK_LOG_H__

#ifndef __hmbuild__
#  ifdef CONFIG_DEBUG_BUILD
#    ifndef __hmfeature_debug__
#      define __hmfeature_debug__
#    endif
#  endif
#endif

enum udklog_level {
	UDKLOG_DEBUG,
	UDKLOG_INFO,
	UDKLOG_WARN,
	UDKLOG_ERROR,
	UDKLOG_PANIC,
};

extern void __attribute__((format(printf, 5, 6)))
udk_printf(enum udklog_level, const char *file, const char *func,
	   int line, const char *fmt, ...);

#ifdef __hmfeature_debug__
#define udk_log(lv, fmt...)   udk_printf(lv, __FILE__, __func__, __LINE__, fmt)
#define udk_debug(fmt, ...)   udk_log(UDKLOG_DEBUG, fmt, ##__VA_ARGS__)
#else
#define udk_log(lv, fmt...)   udk_printf(lv, NULL, __func__, __LINE__, fmt)

static inline void __attribute__((format(printf, 1, 2))) __no_log(const char *fmt, ...)
{
	(void)fmt;
}
#define udk_debug(fmt, ...)   __no_log(fmt, ##__VA_ARGS__)
#endif /* __hmfeature_debug__ */
#define udk_info(fmt, ...)    udk_log(UDKLOG_INFO, fmt, ##__VA_ARGS__)
#define udk_warn(fmt, ...)    udk_log(UDKLOG_WARN, fmt, ##__VA_ARGS__)
#define udk_error(fmt, ...)   udk_log(UDKLOG_ERROR, fmt, ##__VA_ARGS__)
#define udk_fatal(fmt, ...)   udk_log(UDKLOG_PANIC, fmt, ##__VA_ARGS__)

#endif /* __UDK_LOG_H__ */
