/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2022. All rights reserved.
 * Description: Klog
 * Author: Huawei OS Kernel Lab
 * Create: Sat Jun 9 07:55:53 2018
 */
#ifndef KLIBS_KLOG_H
#define KLIBS_KLOG_H

#include <stdarg.h>
#include <hmkernel/time.h>
#include <hmkernel/types.h>
#include <hmkernel/compiler.h>
#include <hmkernel/klog_level.h>

extern void
klog_set_read_time(struct __time_real_t (*func)(void));

extern void
klog_set_read_time_nolock(struct __time_real_t (*func)(void));

extern void
klog_log_time_enable(bool enable);

extern __u64 klog_read_time(void);

/* Force output with klog level */
extern void __printf(2, 3) __cold
klog_printf(enum klog_level level, const char *fmt, ...);

/*
 * nolock == false: klog print with lock, include timekeeplock and kconsolelock
 * nolock == true: klog print without lock, and print to kev only
 */
extern void __printf(2, 3) __cold
klog_printf_nolock(enum klog_level level, const char *fmt, ...);

extern void __printf(3, 0) __cold
__klog_vprintf(bool nolock, enum klog_level level, const char *fmt, va_list args);

void klog_enter_lastword(void);
extern void klog_enable_multi_buffer(void);

#define __klog(lv, fmt, ...)		klog_printf(lv, "[%s:%d] "fmt, __func__, __LINE__, ##__VA_ARGS__)

#ifdef CONFIG_KLOG_PURE_NOLOCK
#define __klog_nolock(lv, fmt, ...)	klog_printf_nolock(lv, fmt, ##__VA_ARGS__)
#else
#define __klog_nolock(lv, fmt, ...)	klog_printf_nolock(lv, "[%s:%d] "fmt, __func__, __LINE__, ##__VA_ARGS__)
#endif /* CONFIG_KLOG_PURE_NOLOCK */

#ifndef __hmbuild__
# ifdef CONFIG_DEBUG_BUILD
#  define __hmfeature_debug__
# else
#  define __hmfeature_no_debug__
# endif
#endif

#ifdef __hmfeature_debug__
#define klog_debug(fmt...)		__klog(KLOG_DEBUG, fmt)
#define klog_debug_nolock(fmt...)	__klog_nolock(KLOG_DEBUG, fmt)
#else
static inline void __printf(1, 2)
__no_klog(const char *fmt, ...)
{
	UNUSED(fmt);
}
#define klog_debug(fmt...)		__no_klog(fmt)
#define klog_debug_nolock(fmt...)	__no_klog(fmt)
#endif /* __hmfeature_debug__ */

#define klog_info(fmt...)		__klog(KLOG_INFO, fmt)
#define klog_warn(fmt...)		__klog(KLOG_WARN, fmt)
#define klog_error(fmt...)		__klog(KLOG_ERROR, fmt)
#define __klog_panic(fmt...)		__klog(KLOG_PANIC, fmt)
#define __klogv_panic(fmt, va)		__klog_vprintf(false, KLOG_PANIC, fmt, va)
#define __klogv_warn(fmt, va)		__klog_vprintf(false, KLOG_WARN, fmt, va)

/* nolock version */
#define klog_info_nolock(fmt...)	__klog_nolock(KLOG_INFO, fmt)
#define klog_warn_nolock(fmt...)	__klog_nolock(KLOG_WARN, fmt)
#define klog_error_nolock(fmt...)	__klog_nolock(KLOG_ERROR, fmt)
#define klog_panic_nolock(fmt...)	__klog_nolock(KLOG_PANIC, fmt)

#define klog_info_panic(is_panic, fmt...) \
	do { \
		if (is_panic) { \
			__klog(KLOG_PANIC, fmt); \
		} else { \
			__klog(KLOG_INFO, fmt); \
		} \
	} while (__false)

#define klog_info_panic_nolock(is_panic, fmt...) \
	do { \
		if (is_panic) { \
			__klog_nolock(KLOG_PANIC, fmt); \
		} else { \
			__klog_nolock(KLOG_INFO, fmt); \
		} \
	} while (__false)

/* not accurately print once when called concurrently */
#define klog_warn_once(fmt...) \
	do { \
		static bool __warn = true; \
		if (__warn) { \
			__warn = false; \
			klog_warn(fmt); \
		} \
	} while (0)
#endif

#define klog_print(nolock, is_panic, fmt...)			\
	do {							\
		if (nolock) {					\
			klog_info_panic_nolock(is_panic, fmt);	\
		} else {					\
			klog_info_panic(is_panic, fmt);		\
		}						\
	} while (0)
