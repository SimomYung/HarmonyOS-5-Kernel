/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Define all asan option struct
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 07 14:13:58 2022
 */
/*
 * Notes:
 * This file is used to describe asan_options and provide related definitions
 * and check functions.
 * And all options init operation will be done in asan_env.c
 */

// libc
#include <string.h>

// hongmeng
#include <vfs.h>
#include <hongmeng/errno.h>
#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_procmgr.h>

// security kasan
#include "asan_env.h"
#include "asan_test.h"

/*
 * =====================================================================================
 * log_path option:
 * Specifying the path of the log file. If you do not specify it will be printed to the screen.
 * =====================================================================================
 */
INNER int log_path_check(void);
INNER int log_path_prepare(void);

struct asan_option_s log_path_option = {
	LOG_PATH, "log_path",
	NULL,
	log_path_check,
	log_path_prepare,
};

INNER int log_path_check(void)
{
	char *str = log_path_option.str;
	if (str == NULL || str[0] == '\0') {
		hm_debug("log path option is empty\n");
		return -1;
	}

	size_t len = strlen(str);
	// path cannot be too long, because we will add pid suffix.
	if (len >= DEFAULT_PATH_SIZE - PID_STR_MAX_LEN) {
		hm_info("log_path's lenth is too long, it should not be longer than %u",
			 (DEFAULT_PATH_SIZE - PID_STR_MAX_LEN));
		return -1;
	}

	// path's tail is '/', file do not have prefix
	if (str[len - 1] == '/') {
		hm_info("log_path's tail char should not be /");
		return -1;
	}

	return 0;
}

INNER int log_path_prepare(void)
{
#define LOG_FILE_MODE 0666
	char *str = log_path_option.str;
	pid_t pid = hm_getpid();
	int ret = snprintf_s(asan_env_gs()->full_path, DEFAULT_PATH_SIZE, DEFAULT_PATH_SIZE -1,
		   "%s.%d", str, pid);
	if (ret < 0) {
		hm_error("failed to get log file full path\n");
		return -1;
	}

	int fd = vfs_open(asan_env_gs()->full_path,
			  (unsigned int)O_CREAT | (unsigned int)O_RDWR | (unsigned int)O_APPEND, LOG_FILE_MODE);
	if (fd < 0) {
		hm_error("failed to open %s, err=%s\n", asan_env_gs()->full_path, hmstrerror(fd));
		return -1;
	}
	asan_env_gs()->log_file_fd = fd;
	hm_debug("create log file success, filename: %s, fileid: %d\n", asan_env_gs()->full_path, fd);

	return 0;
}

/*
 * =====================================================================================
 * halt_on_error option:
 * This option is used to determine whether to continue after a memory error is detected.
 * If the value is "0", continue. If the value is "1", coredump is triggered.
 * =====================================================================================
 */
INNER int halt_on_error_check(void);

struct asan_option_s halt_on_error_option = {
	HALT_ON_ERROR, "halt_on_error",
	NULL,
	halt_on_error_check,
	NULL,
};

INNER int halt_on_error_check(void)
{
	char *str = halt_on_error_option.str;
	if (str == NULL || str[0] == '\0') {
		hm_debug("halt_on_error option is empty\n");
		return -1;
	}

	// halt_on_error must be "0" or "1"
	if (strcmp(str, "0") != 0 && strcmp(str, "1") != 0) {
		hm_error("halt_on_err option should be 0 or 1, option is %s now\n", str);
		return -1;
	}

	return 0;
}

void register_all_asan_opts(void)
{
	asan_env_register(&log_path_option);
	asan_env_register(&halt_on_error_option);
}
