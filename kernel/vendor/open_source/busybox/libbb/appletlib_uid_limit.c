/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023.
 * Description: busybox appletlib user limit
 * Author: Huawei IoT Dept 2
 *
 * Licensed under GPLv2, see file LICENSE in this source tree.
 */

//config:config FEATURE_APPLETLIB_UID_LIMIT
//config:	bool "busybox appletlib uid limit"
//config:	default n
//config:	help
//config:	Check the trustlist before running the command.
//config:
//config:config DEFAULT_APPLETLIB_UID_LIMIT_FILE
//config:	string "busybox exec whitelist file path"
//config:	default "/etc/busybox_limit"
//config:	depends on FEATURE_APPLETLIB_UID_LIMIT
//config:	help
//config:	File example:
//config:		mv:usr1,usr2
//config:		cmd2:usr1

//kbuild:lib-$(CONFIG_FEATURE_APPLETLIB_UID_LIMIT) += appletlib_uid_limit.o

#include "busybox.h"
#include "NUM_APPLETS.h"
#if NUM_APPLETS > 0
#define MAX_LIMIT_CONFIG_LINE 256
#define MAX_LEN_PRE_LINE 1024

static int check_one_applet_limit(const char *allow_users, uid_t uid)
{
	const char *start = NULL;
	char *found = NULL;
	unsigned int len;
	struct passwd *user = NULL;

	user = getpwuid(uid);
	if (user == NULL || user->pw_name == NULL)
		return CHECK_UID_DENY;

	len = strlen(user->pw_name);
	start = allow_users;
	do {
		found = strstr(start, user->pw_name);
		if (found == NULL)
			return CHECK_UID_DENY;
        // Ensure that the character string found is the beginning of a single user name.
        // Avoid this scenario: "FakeAllowUser,User2".
		if (found != allow_users && *(found - 1) != ',') {
			start = found + len;
			continue;
		}
        // Make sure the string found is the end of the user name.
		if (found[len] == '\0' || found[len] == ',' || found[len] == '\r' || found[len] == '\n')
			return CHECK_UID_ALLOW;

		start = found + len;
	} while (*start != '\0');

	return CHECK_UID_DENY;
}

int check_applet_uid_limit(const char *name)
{
	int ret;
	unsigned int len;
	char line[MAX_LEN_PRE_LINE];
	FILE *fp = NULL;
	int i;

    // The root user can use all commands.
	uid_t uid = getuid();
	if (uid == 0)
		return CHECK_UID_ALLOW;

	fp = fopen(CONFIG_DEFAULT_APPLETLIB_UID_LIMIT_FILE, "r");
	if (fp == NULL)
		return CHECK_UID_DENY;

	len = strlen(name);
	for (i = 0; i < MAX_LIMIT_CONFIG_LINE; i++) {
		if (fgets(line, sizeof(line), fp) == NULL)
			break;

		// Find the applet name in config file.
		if (strncmp(line, name, len) == 0 && line[len] == ':') {
			ret = check_one_applet_limit(&line[len + 1], uid);
			fclose(fp);
			return ret;
		}
	}

	fclose(fp);
	return CHECK_UID_DENY;
}
#endif

