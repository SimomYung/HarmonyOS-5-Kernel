/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Formatted output of memory info
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jan 02 11:19:25 2020
 */
#ifndef ULIBS_INCLUDE_LIBMEM_MPRINT_H
#define ULIBS_INCLUDE_LIBMEM_MPRINT_H

#include <libhwsecurec/securec.h>
#include <hongmeng/syscall.h>

/* NOTE: There may be loop-call between hm_error and malloc, like malloc->mmap/brk->hm_error->malloc, in which case
 * a deadlock will happen. So use mprint_error rather than hm_error to print error message in ulibs of brk/mmap.
 * Remove mprint_error if the loop-call has been solved.
 */
#define MEM_ERR_BUFSZ 512U
#define mprint_error(fmt, args...) \
	do { \
		int __ret; \
		char __err_log[MEM_ERR_BUFSZ]; \
		__ret = snprintf_s(__err_log, (size_t)MEM_ERR_BUFSZ, (size_t)MEM_ERR_BUFSZ - 1U, \
				 "[%s:%d] "fmt, __func__, __LINE__, ##args); \
		/* if snprintf_s failed, just break and not print error message. */ \
		if (__ret > 0) { \
			(void)sysfast_syslog((unsigned int)HMLOG_ERROR, __err_log, (size_t)MEM_ERR_BUFSZ, NULL); \
		} \
	} while (1 != 1)
#define mprint_warn(fmt, args...) \
	do { \
		int __ret; \
		char __warn_log[MEM_ERR_BUFSZ]; \
		__ret = snprintf_s(__warn_log, (size_t)MEM_ERR_BUFSZ, (size_t)MEM_ERR_BUFSZ - 1U, \
				 "[%s:%d] "fmt, __func__, __LINE__, ##args); \
		/* if snprintf_s failed, just break and not print warn message. */ \
		if (__ret > 0) { \
			(void)sysfast_syslog((unsigned int)HMLOG_WARN, __warn_log, (size_t)MEM_ERR_BUFSZ, NULL); \
		} \
	} while (1 != 1)
#define mprint_debug(fmt, args...) \
	do { \
		int __ret; \
		char __debug_log[MEM_ERR_BUFSZ]; \
		__ret = snprintf_s(__debug_log, (size_t)MEM_ERR_BUFSZ, (size_t)MEM_ERR_BUFSZ - 1U, \
				 "[%s:%d] "fmt, __func__, __LINE__, ##args); \
		/* if snprintf_s failed, just break and not print debug message. */ \
		if (__ret > 0) { \
			(void)sysfast_syslog((unsigned int)HMLOG_DEBUG, __debug_log, (size_t)MEM_ERR_BUFSZ, NULL); \
		} \
	} while (1 != 1)
#endif
