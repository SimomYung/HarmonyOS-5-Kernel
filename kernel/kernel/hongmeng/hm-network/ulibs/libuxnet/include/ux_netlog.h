/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description:  Header file for network log
 * Author: Huawei OS Kernel Lab
 * Create: Sat Feb 22 10:49:45 2020
 */

#ifndef __UX_HMNET_LOG_H__
#define __UX_HMNET_LOG_H__

#include <stdint.h>
#include <sys/types.h>
#include <libhmlog/hmlog.h>

/*
 * bitmap, MSB
 * 11 for log level, 00 for log mask
 * 1 for hmnetd log level, 0 for inet stack log level
 */
#define HMNET_LOG_SETTING_MASK		0x3fffffff
#define HMNET_LOG_SETTING_LABEL		0x40000000
#define HMNET_LOG_SETTING_AF_LABEL	0x80000000

#define HMNET_IPC		0x1
#define HMNET_UTIL		0x2
#define HMNET_CORE		0x4
#define HMNET_INET		0x8
#define HMNET_UNIX		0x10
#define HMNET_NETLINK		0x20
#define HMNET_BPF		0x40
#define HMNET_PACKET		0x80
#define HMNET_CAN		0x100
#define HMNET_NETFILTER		0x200
#define HMNET_TUN		0x400
#define HMNET_PPP		0x800
#define HMNET_VHOST		0x1000
#define HMNET_VSOCK		0x2000

#define HMNET_LOG_DEFAULT_MASK (HMNET_CORE | HMNET_INET | HMNET_UNIX)

bool ux_is_module_enabled(int module);
enum hmlog_level ux_get_log_level(void);

#define hmnetlog_fmt(fmt)	fmt

#ifdef CONFIG_DEBUG_BUILD
#define net_debug(module, fmt, ...) \
	do { \
		if (ux_is_module_enabled(module) && ux_get_log_level() == HMLOG_DEBUG) {\
			__hmlog(HMLOG_DEBUG, hmnetlog_fmt(fmt), ##__VA_ARGS__); \
		} \
	} while (0)
#else
/* non-debug version should not print debug info */
#define net_debug(module, fmt, ...)
#endif

#define net_info(module, fmt, ...) \
	do { \
		if (ux_is_module_enabled(module) && ux_get_log_level() <= HMLOG_INFO) {\
			hm_info(hmnetlog_fmt(fmt), ##__VA_ARGS__); \
		} \
	} while (0)
#define net_warn(module, fmt, ...) \
	hm_warn(hmnetlog_fmt(fmt), ##__VA_ARGS__)
#define net_error(module, fmt, ...) \
	hm_error(hmnetlog_fmt(fmt), ##__VA_ARGS__)
#define net_panic(module, fmt, ...) \
	hm_panic(hmnetlog_fmt(fmt), ##__VA_ARGS__)

void ux_hmnetd_log_level(int level);
void ux_hmnetd_log_mask(uint32_t mask);
void ux_hmnetd_log_default(void);
void ux_hmnetd_log_setting(uint32_t logsetting);
void ux_register_tcpip_set_log_level_hook(void (*hook)(int));

#endif /* __UX_HMNET_LOG_H__ */
