/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Author: songqiubin
 *
 * mtp debug macro define
 */

#ifndef __MINTP_LOG_H__
#define __MINTP_LOG_H__

extern int sysctl_mtp_debug;

#ifdef MTP_DEBUG_FTRACE
#define mtp_printk(fmt, lvl, ...) trace_printk("[nstack_mtp]"fmt, ##__VA_ARGS__)
#define mtp_sk_printk(sk, fmt, lvl, ...) trace_printk("[nstack_mtp]%d:%d "fmt, (sk)->src_port, (sk)->dst_port, \
                                                      ##__VA_ARGS__)
#else
#define mtp_printk(fmt, lvl, ...) printk(lvl pr_fmt(fmt), ##__VA_ARGS__)
#define mtp_sk_printk(sk, fmt, lvl, ...)  printk(lvl pr_sk_fmt(sk, fmt), ##__VA_ARGS__)
#endif

/* mtp_debug also use KERN_INFO for kenrel is not print level KERN_DEBUG */
#define mtp_debug(fmt, ...)	\
	do {	\
		if (unlikely(sysctl_mtp_debug >= LOGLEVEL_DEBUG))	\
			mtp_printk(fmt, KERN_INFO, ##__VA_ARGS__); \
	} while (0)

#define mtp_info(fmt, ...)	\
	do {	\
		if (likely(sysctl_mtp_debug >= LOGLEVEL_INFO))		\
			mtp_printk(fmt, KERN_INFO, ##__VA_ARGS__); \
	} while (0)

#define mtp_warn(fmt, ...)	\
	do {	\
		if (likely(sysctl_mtp_debug >= LOGLEVEL_WARNING))	\
			mtp_printk(fmt, KERN_WARNING, ##__VA_ARGS__); \
	} while (0)

#define mtp_err(fmt, ...)	\
	do {	\
		if (likely(sysctl_mtp_debug >= LOGLEVEL_ERR))		\
			mtp_printk(fmt, KERN_ERR, ##__VA_ARGS__); \
	} while (0)

#define mtp_sk_debug(sk, fmt, ...)	\
	do {	\
		if (unlikely(sysctl_mtp_debug >= LOGLEVEL_DEBUG))	\
			mtp_sk_printk(sk, fmt, KERN_INFO, ##__VA_ARGS__); \
	} while (0)

#define mtp_sk_info(sk, fmt, ...)	\
	do {	\
		if (likely(sysctl_mtp_debug >= LOGLEVEL_INFO))		\
			mtp_sk_printk(sk, fmt, KERN_INFO, ##__VA_ARGS__); \
	} while (0)

#define mtp_sk_warn(sk, fmt, ...)	\
	do {	\
		if (likely(sysctl_mtp_debug >= LOGLEVEL_WARNING))	\
			mtp_sk_printk(sk, fmt, KERN_WARNING, ##__VA_ARGS__); \
	} while (0)

#define mtp_sk_err(sk, fmt, ...)	\
	do {	\
		if (likely(sysctl_mtp_debug >= LOGLEVEL_ERR))		\
			mtp_sk_printk(sk, fmt, KERN_ERR, ##__VA_ARGS__); \
	} while (0)

#define MACFMT "%02x:%02x:%02x:%02x:**:**"
#define MACDATA(mac) (mac)[0], (mac)[1], (mac)[2], (mac)[3]

#endif /* __MINTP_LOG_H__ */
