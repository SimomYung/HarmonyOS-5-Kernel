/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Log information denied by security framework
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 23 23:20:59 2020
 */
#ifndef ULIBS_LIBSEC_LOG_H
#define ULIBS_LIBSEC_LOG_H
#include <stdarg.h>
#include <stdint.h>
#include <sys/types.h>
#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_security.h>
// security
#include "cred.h"

static inline bool sec_log_is_enabled(unsigned int mode, uint32_t flags)
{
	return mode == HM_SEC_SHCONF_SEC_DEBUG_ENABLE ||
	       (mode == HM_SEC_SHCONF_SEC_DEBUG_RESTRICT &&
		sec_cred_is_flag_set(flags, SEC_BASIC_LOG_ENABLE));
}

static inline bool sec_log_trace_is_enabled(unsigned int mode, uint32_t flags)
{
	return mode == HM_SEC_SHCONF_SEC_DEBUG_ENABLE ||
	       (mode == HM_SEC_SHCONF_SEC_DEBUG_RESTRICT &&
		sec_cred_is_flag_set(flags, SEC_BASIC_LOG_TRACE));
}

/*
 * Print sec denied logs directly, called by mac check methods(mac, ability, vfs_mac)
 */
#define sec_deny_log(fmt...) __hmlog(HMLOG_INFO, fmt)

struct sec_deny_log_info {
	const struct sec_chk_cred *cred;
	const struct sec_cred_basic *basic;
	const char *fmt;
	bool is_log_enabled;    // print logs directly if true
	bool is_trace_disabled; // do not print backtrace if true
	enum hmlog_level level;
	const char *func;
	int line;
};

#define sec_deny_log_info_init(name, __cred, __basic, __fmt,	\
			       __is_log_enabled,		\
			       __is_trace_disabled)		\
	struct sec_deny_log_info name = {			\
		.cred = (__cred),				\
		.basic = (__basic),				\
		.fmt = (__fmt),					\
		.is_log_enabled = (__is_log_enabled),		\
		.is_trace_disabled = (__is_trace_disabled),	\
		.level = HMLOG_INFO,				\
		.func = __func__,				\
		.line = __LINE__,				\
	}

/*
 * NOTE: This function should not be called directly, use sec_deny_log_debug instead.
 */
void __attribute__((format(printf, 2, 3)))
sec_log_debug(const struct sec_deny_log_info *info, const char *fmt, ...);

/*
 * Print sec denied logs or backtrace, called by dac/posix methods
 * and security check framework.
 */
#define sec_deny_log_debug(info, para...) \
do { \
	sec_log_debug((info), (info)->fmt, para); \
} while(0)

#define sec_hook_deny_log(cred, fmt, para...) do { \
	sec_deny_log_info_init(__info, cred, NULL, fmt, false, false); \
	sec_deny_log_debug(&__info, para); \
} while (0)

/* used before sysif exit */
void sec_dump_stack(void);

void sec_chk_show_obj_name(const struct sec_chk_cred *cred, void *obj);
typedef int (*sec_deny_log_hook_func_t)(uint32_t audit_type, uint32_t flag,
					const char *fmt, va_list args);

typedef void (*sec_chk_show_extra)(const struct sec_chk_cred *cred, void *arg);
/* Extra log handler usually logging object information such as process name and etc.. */
struct sec_log_extra_info {
	sec_chk_show_extra func;
	void *arg;
};

#define sec_log_extra_info_init(name, infunc, inarg) struct sec_log_extra_info name = {	\
	.func = (infunc),								\
	.arg = (inarg),									\
}

#ifdef CONFIG_AUDIT
void sec_set_deny_log_hook(const sec_deny_log_hook_func_t func);
void sec_deny_log_hook(const char *fmt, ...);
#else
static inline void sec_set_deny_log_hook(const sec_deny_log_hook_func_t func)
{
	(void)func;
	return;
}

static inline void sec_deny_log_hook(const char *fmt, ...)
{
	(void)fmt;
	return;
}
#endif

#endif /* ifndef ULIBS_LIBSEC_LOG_H */
