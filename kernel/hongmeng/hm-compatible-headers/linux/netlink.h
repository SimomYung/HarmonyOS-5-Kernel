/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Add linux/netlink.h to compat linux interface
 * Author: Huawei OS Kernel Lab
 * Create: Fri Sep 24 16:07:39 2021
 */

#ifndef __LINUX_NETLINK_H_
#define __LINUX_NETLINK_H_

#include <linux/types.h>
#include <linux/socket.h>

#define NETLINK_ROUTE		0
#define NETLINK_USERSOCK	2
#define NETLINK_FIREWALL	3
#define NETLINK_AUDIT		9
#define NETLINK_IP6_FW		13
#define NETLINK_KOBJECT_UEVENT  15

#define NLMSG_LENGTH(len) ((len) + NLMSG_HDRLEN)

struct sockaddr_nl {
	__kernel_sa_family_t    nl_family;
	unsigned short  nl_pad;
	__u32       nl_pid;
	__u32       nl_groups;
};

struct nlmsghdr {
	__u32		nlmsg_len;
	__u16		nlmsg_type;
	__u16		nlmsg_flags;
	__u32		nlmsg_seq;
	__u32		nlmsg_pid;
};

#define NLM_F_REQUEST		0x01
#define NLM_F_ACK		0x04
#define NLM_F_DUMP_INTR		0x10

#define NLM_F_ROOT		0x100
#define NLM_F_MATCH		0x200
#define NLM_F_DUMP		(NLM_F_ROOT|NLM_F_MATCH)

#define NLM_F_EXCL		0x200
#define NLM_F_CREATE		0x400

#define NLMSG_ALIGNTO		4U
#define NLMSG_ALIGN(len)	( ((len)+NLMSG_ALIGNTO-1) & ~(NLMSG_ALIGNTO-1)  )
#define NLMSG_HDRLEN		((int) NLMSG_ALIGN(sizeof(struct nlmsghdr)))
#define NLMSG_LENGTH(len)	((len) + NLMSG_HDRLEN)
#define NLMSG_SPACE(len)	NLMSG_ALIGN(NLMSG_LENGTH(len))
#define NLMSG_DATA(nlh)		((void*)(((char*)nlh) + NLMSG_LENGTH(0)))
#define NLMSG_NEXT(nlh,len)	((len) -= NLMSG_ALIGN((nlh)->nlmsg_len), \
				(struct nlmsghdr*)(((char*)(nlh)) + NLMSG_ALIGN((nlh)->nlmsg_len)))
#define NLMSG_OK(nlh,len)	((len) >= (int)sizeof(struct nlmsghdr) && \
				(nlh)->nlmsg_len >= sizeof(struct nlmsghdr) && \
				(nlh)->nlmsg_len <= (len))
#define NLMSG_PAYLOAD(nlh,len)	((nlh)->nlmsg_len - NLMSG_SPACE((len)))

#define NLMSG_NOOP		0x1
#define NLMSG_ERROR		0x2
#define NLMSG_DONE		0x3

struct nlmsgerr {
	int		error;
	struct		nlmsghdr msg;
};

#define NLA_F_NESTED		(1 << 15)

struct nlattr {
	__u16           nla_len;
	__u16           nla_type;
};

#endif
