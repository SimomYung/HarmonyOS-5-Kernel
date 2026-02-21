/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Sat Feb 26 15:11:46 2022
 */

#include <string.h>
#include <paths.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <utmp.h>

void logwtmp(const char *line, const char *name, const char *host)
{
	struct utmp ut;
	(void)memset(&ut, 0, sizeof(ut));
	struct timeval time_val;
	ut.ut_tv.tv_sec = 0L;
	ut.ut_tv.tv_usec = 0L;
	if (gettimeofday(&time_val, NULL) == 0) {
		ut.ut_tv.tv_sec = time_val.tv_sec;
		ut.ut_tv.tv_usec = time_val.tv_usec;
	}
	ut.ut_pid = getpid();
	if (name[0] != '\0') {
		ut.ut_type = USER_PROCESS;
	} else {
		ut.ut_type = DEAD_PROCESS;
	}
	(void)strncpy(ut.ut_line, line, sizeof(ut.ut_line));
	ut.ut_line[sizeof(ut.ut_line) - 1UL] = '\0';
	(void)strncpy(ut.ut_name, name, sizeof(ut.ut_name));
	ut.ut_name[sizeof(ut.ut_name) - 1UL] = '\0';
	(void)strncpy(ut.ut_host, host, sizeof(ut.ut_host));
	ut.ut_host[sizeof(ut.ut_host) - 1UL] = '\0';

	updwtmp(_PATH_WTMP, &ut);
}
