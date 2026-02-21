/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2022. All rights reserved.
 * Description: Definitions of hongmeng kernel log api
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 19 03:43:55 2018
 */
#ifndef ULIBS_INCLUDE_LIBHMLOG_HMLOG_H
#define ULIBS_INCLUDE_LIBHMLOG_HMLOG_H

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <hmasm/panic_trap_lib.h>
#include <hongmeng/types.h>
#include <hmkernel/compiler.h>
#include <hmkernel/klog_level.h>

#ifdef __HOST_LLT__
char *get_hmlog_buffer(void);
void clear_hmlog_buffer(void);
#endif

#ifndef __hmbuild__
# ifdef CONFIG_DEBUG_BUILD
#  define __hmfeature_debug__
# else
#  define __hmfeature_no_debug__
# endif
#endif

/* Only show addresses on the debug build */
#ifdef __hmfeature_debug__
#define vaddr_of(ptr) (unsigned long)(uintptr_t)(ptr)
#else
#define vaddr_of(ptr) ({uintptr_t __p; __p = (ptr) == NULL ? 0UL : UINTPTR_MAX; (unsigned long)__p;})
#endif

#define HMLOG_LEVEL_NR KLOG_LEVEL_NR
enum hmlog_level {
	HMLOG_DEBUG =	KLOG_DEBUG,
	HMLOG_INFO =	KLOG_INFO,
	HMLOG_WARN =	KLOG_WARN,
	HMLOG_ERROR =	KLOG_ERROR,
	HMLOG_PANIC =	KLOG_PANIC,
	HMLOG_NOLOCK =	KLOG_NOLOCK,
	HMLOG_INVALID =	KLOG_INVALID,
};

#ifdef __HOST_LLT__
enum hmlog_output_type {
	HMLOG_OUTPUT_PRINTF,
	HMLOG_OUTPUT_BUFFER,
	HMLOG_OUTPUT_CONSOLE,
	HMLOG_OUTPUT_INVAL,
};
#define HMLOG_DEFAULT_OUTPUT HMLOG_OUTPUT_PRINTF
#else
enum hmlog_output_type {
	HMLOG_OUTPUT_KBOX,
	HMLOG_OUTPUT_CONSOLE,
	HMLOG_OUTPUT_INVAL,
};
#define HMLOG_DEFAULT_OUTPUT HMLOG_OUTPUT_KBOX
#endif

void __attribute__((noreturn)) __cold
__panic_impl(const char *file, const char *func, int line);

void __attribute__((noreturn)) __cold
__panic_impl_with_reason(const char *file, const char *func, int line,
			 unsigned int reason);

void __attribute__((format(printf, 5, 6)))
__hmlog_printf(enum hmlog_level level, const char *file, const char *func,
	       int line, const char *fmt, ...);

void __attribute__((format(printf, 4, 0)))
__hmlog_vprintf_stkchk(enum hmlog_level level, const char *func,
		       int line, const char *fmt, va_list args);

void __attribute__((__noinline__)) __attribute__((format(printf, 4, 0)))
__hmlog_vprintf(enum hmlog_level level, const char *func,
		int line, const char *fmt, va_list args);

void __attribute__((format(printf, 5, 6)))
__hmlog_printf_syslog(enum hmlog_level level, const char *file, const char *func,
		      int line, const char *fmt, ...);

/* wait to be deleted */
#define hmlog_setlevel(level)

void hmlog_setoutput(enum hmlog_output_type output);

void hmlog_output(enum hmlog_output_type output_type, enum hmlog_level level,
		  const char *__log, size_t log_size);

const char *hmlog_name_of_level(unsigned int level);
unsigned int hmlog_level_of_name(const char *name);
unsigned int linux_log_level_of_name(const char *name);

void hmlog_enable_stk_chk(void);
void hmlog_disable_stk_chk(void);
bool hmlog_stk_chk_of(void);

#ifndef log_fmt
#define log_fmt(fmt) fmt
#endif /* log_fmt */

#define __hmlogv(lv, fmt, va) \
	__hmlog_vprintf(lv, __func__, __LINE__, fmt, va)

#ifdef __hmfeature_debug__
#define __hmlog(lv, fmt, args...) \
	__hmlog_printf(lv, __FILE__, __func__, __LINE__, fmt, ##args)

#define __hmlog_syslog(lv, fmt, args...) \
	__hmlog_printf_syslog(lv, __FILE__, __func__, __LINE__, fmt, ##args)
#else
/* non-debug version should not include FILE PATH in binary */
#define __hmlog(lv, fmt, args...) \
	__hmlog_printf(lv, NULL, __func__, __LINE__, fmt, ##args)

#define __hmlog_syslog(lv, fmt, args...) \
	__hmlog_printf_syslog(lv, NULL, __func__, __LINE__, fmt, ##args)
#endif /* __hmfeature_debug__ */


#define hm_info(fmt, ...)	__hmlog(HMLOG_INFO, log_fmt(fmt), ##__VA_ARGS__)
#define hm_warn(fmt, ...)	__hmlog(HMLOG_WARN, log_fmt(fmt), ##__VA_ARGS__)
#define hm_error(fmt, ...)	__hmlog(HMLOG_ERROR, log_fmt(fmt), ##__VA_ARGS__)


#ifdef __hmfeature_debug__
#define hm_debug(fmt...)	__hmlog(HMLOG_DEBUG, fmt)
#define hm_panic(fmt, ...)	\
	do {	\
		arch_panic_trap_preproc(); \
		__hmlog(HMLOG_PANIC, log_fmt(fmt), ##__VA_ARGS__);	\
		__panic_impl(__FILE__, __FUNCTION__, __LINE__);	\
	} while (__false)
#define hm_panic_with_reason(reason, fmt, ...)	\
	do {	\
		arch_panic_trap_preproc(); \
		__hmlog(HMLOG_PANIC, log_fmt(fmt), ##__VA_ARGS__);	\
		__panic_impl_with_reason(__FILE__, __FUNCTION__, __LINE__, reason);	\
	} while (__false)
#else
/* non-debug version should not print debug info */
static inline void __attribute__((format(printf, 1, 2)))
__no_hmlog(const char *fmt, ...)
{
	(void)fmt;
}
#define hm_debug(fmt...)	__no_hmlog(fmt)
#define hm_panic(fmt, ...)	\
	do {	\
		arch_panic_trap_preproc(); \
		__hmlog(HMLOG_PANIC, log_fmt(fmt), ##__VA_ARGS__);	\
		__panic_impl(NULL, __FUNCTION__, __LINE__);	\
	} while (__false)
#define hm_panic_with_reason(reason, fmt, ...)	\
	do {	\
		arch_panic_trap_preproc(); \
		__hmlog(HMLOG_PANIC, log_fmt(fmt), ##__VA_ARGS__);	\
		__panic_impl_with_reason(NULL, __FUNCTION__, __LINE__, reason);	\
	} while (__false)
#endif /* __hmfeature_debug__ */

#define __hmlogv_panic(fmt, va) __hmlog_vprintf(HMLOG_PANIC, __FUNCTION__, __LINE__, fmt, va)
#define __hmlogv_warn(fmt, va) __hmlog_vprintf(HMLOG_WARN, __FUNCTION__, __LINE__, fmt, va)

/* support full version of nolock for all above log level in the future */
#define __hmlog_nolock(fmt, ...)	__hmlog_syslog(HMLOG_NOLOCK, log_fmt(fmt), ##__VA_ARGS__)

#endif /* ULIBS_INCLUDE_LIBHMLOG_HMLOG_H */
