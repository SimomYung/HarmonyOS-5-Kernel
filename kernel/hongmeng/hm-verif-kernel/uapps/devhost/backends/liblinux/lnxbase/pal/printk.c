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
