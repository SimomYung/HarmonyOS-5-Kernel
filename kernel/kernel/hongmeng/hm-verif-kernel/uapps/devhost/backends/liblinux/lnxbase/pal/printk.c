/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: liblinux PAL implement for log output
 * Author: Huawei OS Kernel Lab
 * Create: Wed Dec 18 10:45:12 2019
 */
#include <liblinux/pal.h>

#include <sys/types.h>
#include <hongmeng/panic.h>
#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_security.h>
#include <libsec/posix_cap.h>

static enum hmlog_level linux_log_level_map[] = {
	[LIBLINUX_LOGLEVEL_EMERG] = HMLOG_ERROR,
	[LIBLINUX_LOGLEVEL_ALERT] = HMLOG_ERROR,
	[LIBLINUX_LOGLEVEL_CRIT] = HMLOG_ERROR,
	[LIBLINUX_LOGLEVEL_ERR] = HMLOG_ERROR,
	[LIBLINUX_LOGLEVEL_WARNING] = HMLOG_WARN,
	[LIBLINUX_LOGLEVEL_NOTICE] = HMLOG_INFO,
	[LIBLINUX_LOGLEVEL_INFO] = HMLOG_INFO,
	[LIBLINUX_LOGLEVEL_DEBUG] = HMLOG_DEBUG,
};

static __thread char last_contains_newline = 1;
static __thread enum hmlog_level last_log_lvl = HMLOG_INFO;

void liblinux_pal_log_store(int level, const char *log, int log_len)
{
	enum hmlog_level log_level = last_log_lvl;

	/* treat all invlid `level` as `KERN_CONT` now */
	if ((level >= (int)LIBLINUX_LOGLEVEL_EMERG) && (level <= (int)LIBLINUX_LOGLEVEL_DEBUG)) {
		if (last_contains_newline == 0) {
			hmlog_output(HMLOG_OUTPUT_KBOX, log_level,
				     "\n", sizeof("\n"));
		}
		log_level = linux_log_level_map[level];
		last_log_lvl = log_level;
	}
	last_contains_newline = 0;
	/* When the log contains more than two characters and the last valid
	 * character is a newline character, the next log msg should be
	 * displayed in a newline.
	 * 20231213: hmlog_output print every log as a newline, unless KERN_CONT log. */
	if (((log_len >= 2) && (log[log_len - 2] == '\n')) || level != LIBLINUX_LOGLEVEL_CONT) {
		last_contains_newline = 1;
	}
	hmlog_output(HMLOG_OUTPUT_KBOX, log_level, log, (size_t)(unsigned int)log_len);
}

int liblinux_pal_kptr_restrict(void)
{
	if (hm_sec_shconf() == NULL) {
		/**
		 * If hm_sec_shconf not init, kptr_restrict return 1 by default.
		 * It is equal to the value inited in sysmgr, which is set by init
		 * process. This value will control %pK print address selectively,
		 * by checking current whether have CAP_SYSLOG ability.
		 */
		return HM_SEC_SHCONF_KPTR_RESTRICT_ENABLE;
	} else {
		return (int)*hm_sec_shconf_kptr_restrict();
	}
}

bool liblinux_pal_kptr_show_value(void)
{
	switch (liblinux_pal_kptr_restrict()) {
	case HM_SEC_SHCONF_KPTR_RESTRICT_DISABLE:
		return true;
	case HM_SEC_SHCONF_KPTR_RESTRICT_ENABLE:
		return (bool)liblinux_pal_capable(POSIX_CAP_SYSLOG, LIBLINUX_CAP_OPT_NOAUDIT);
	case HM_SEC_SHCONF_KPTR_RESTRICT_FORCE:
	default:
		return false;
	}
}
