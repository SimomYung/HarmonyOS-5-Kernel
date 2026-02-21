/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: Definition of sec_mac_user_conf and some note for user
 * Author: Huawei OS Kernel Lab
 * Create: Wed Apr 29 12:45:05 2020
 */
/*
 * User and Label info will be save in /etc/hmmac/userconf.
 * Use libsecconf to READ/WRITE this file.
 *
 * Format:
 *	name1:label1
 *	name2:label2
 * 
 * Note:
 * Usernames may only be up to 32 characters long, and Label must be up
 * to 64 characters long.
 * The constraints are that usernames and label must neither start with
 * a dash ('-') nor plus ('+') nor tilde ('~') nor contain a colon (':'),
 * a comma (','), or a whitespace (space: ' ', end of line: '\n',
 * tabulation: '\t', slash: '/', etc.). 
 * It is usually recommended to only use usernames that begin with a
 * lower case letter or an underscore, followed by lower case letters,
 * digits, underscores, or dashes. They can end with a dollar sign.
 * In regular expression terms: [a-z_][a-z0-9_-]*[$]?
 * */
#ifndef ULIBS_LIBSECCONF_H
#define ULIBS_LIBSECCONF_H

#ifndef __HOST_LLT__
#define SEC_MAC_USER_CONF_PATH "/etc/hmmac/userconf"
#else
#define SEC_MAC_USER_CONF_PATH "userconf"
#endif /* __HOST_LLT__ */

/*
 * Check use-after-free:
 * There are two situations in the design, one is to use the memory and
 * variables created by the caller, and the other is to use internal
 * global variables.
 *
 * In the first case, libsecconf will not perform release operation to
 * caller's memory, and the memory should be managed by caller. So the
 * UAF will not happen.
 *
 * In the second case, only when getline failed, libsecconf will release 
 * buffer created by getline, and the user_conf.name and user_conf.label
 * will not be accessed again. So the UAF will not happen too.
 * */
struct sec_mac_user_conf {
	char *name;  /* user name */
	char *label; /* user label */
};

#endif /* ifndef ULIBS_LIBSECCONF_H */
