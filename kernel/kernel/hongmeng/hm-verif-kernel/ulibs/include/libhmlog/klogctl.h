/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2022. All rights reserved.
 * Description: Definitions related to klogctl
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 14 21:59:54 2020
 */
#ifndef ULIBS_LIBHMLOG_KLOGCTL_H
#define ULIBS_LIBHMLOG_KLOGCTL_H

/*
 * Compatiable with action type defined in syslog(2).
 *
 * All commands except 3 and 10 require privilege.
 * 3 and 10 are allowed to unprivileged processes only if /proc/sys/kernel/dmesg_restrict
 * has the value 0. "privileged" means that the caller has the CAP_SYS_ADMIN
 * capability(deprecated), or the CAP_SYSLOG capability.
 */
enum klogctl_type {
	KLOGCTL_CLOSE			= 0, /* close the log */
	KLOGCTL_OPEN			= 1, /* open the log */
	KLOGCTL_WAIT_AND_READ		= 2, /* wait until kernel log buffer is nonempty then read */
	KLOGCTL_READ_ALL		= 3, /* read all kernel log in buffer */
	KLOGCTL_READ_AND_CLEAR		= 4, /* read all kernel log in buffer and clear them */
	KLOGCTL_CLEAR			= 5, /* clear kernel log in buffer */
	KLOGCTL_CONSOLE_OFF		= 6, /* disable klog to console */
	KLOGCTL_CONSOLE_ON		= 7, /* enable klog to console */
	KLOGCTL_SET_CONSOLE_LEVEL	= 8, /* set level of klog printed to console */
	KLOGCTL_GET_UNREAD_SIZE		= 9, /* bytes of logs currently available to be read in kernel buffer */
	KLOGCTL_GET_BUFFER_SIZE		= 10, /* user buffer size need malloc for all logs print */
	KLOGCTL_TYPE_MAX,
};

enum linux_log_level {
	LIBLINUX_LOGLEVEL_EMERG,
	LIBLINUX_LOGLEVEL_ALERT,
	LIBLINUX_LOGLEVEL_CRIT,
	LIBLINUX_LOGLEVEL_ERR,
	LIBLINUX_LOGLEVEL_WARNING,
	LIBLINUX_LOGLEVEL_NOTICE,
	LIBLINUX_LOGLEVEL_INFO,
	LIBLINUX_LOGLEVEL_DEBUG,
	LIBLINUX_LOGLEVEL_CONT = -3, /* KERN_CONT */
};

#endif /* !ULIBS_LIBHMLOG_KLOGCTL_H */
