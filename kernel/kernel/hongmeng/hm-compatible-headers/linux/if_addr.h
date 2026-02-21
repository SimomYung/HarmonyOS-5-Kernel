/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Add linux/if_addr.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 24 15:15:31 2021
 */

#ifndef __LINUX_IF_ADDR_H
#define __LINUX_IF_ADDR_H

#include <linux/types.h>

struct ifaddrmsg {
	__u8        ifa_family;
	__u8        ifa_prefixlen;
	__u8        ifa_flags;
	__u8        ifa_scope;
	__u32       ifa_index;
};

enum {
	IFA_UNSPEC,
	IFA_ADDRESS,
	IFA_LOCAL,
	IFA_LABEL,
	IFA_BROADCAST,
	IFA_ANYCAST,
	IFA_CACHEINFO,
	IFA_MULTICAST,
	IFA_FLAGS,
	IFA_RT_PRIORITY,
	IFA_TARGET_NETNSID,
	__IFA_MAX,
};

#define IFA_F_SECONDARY     0x01
#define IFA_F_TEMPORARY     IFA_F_SECONDARY

#define IFA_F_DEPRECATED    0x20
#define IFA_F_TENTATIVE	    0x40

struct ifa_cacheinfo {
	__u32   ifa_prefered;
	__u32   ifa_valid;
	__u32   cstamp;
	__u32   tstamp;
};

#define IFA_RTA(r)  ((struct rtattr*)(((char*)(r)) + NLMSG_ALIGN(sizeof(struct ifaddrmsg))))
#define IFA_PAYLOAD(n) NLMSG_PAYLOAD(n,sizeof(struct ifaddrmsg))

#endif
