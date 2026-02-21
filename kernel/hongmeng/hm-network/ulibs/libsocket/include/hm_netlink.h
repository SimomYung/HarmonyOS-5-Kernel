/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Header file for netlink
 * Author: Huawei OS Kernel Lab
 * Create: Sat Dec 28 09:50:26 2019
 */

/*
 * This header file defines structures, enums and macros for network
 * netlink subsystem. In order to maintain compatibility with existing
 * user applications, we make these definitions keep consistent with
 * LINUX netlink.h, which could be found at /usr/include/linux/ in linux
 * operating system.
 */
#ifndef __HM_NETLINK_H_
#define __HM_NETLINK_H_

#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef IFNAMSIZ
#define IFNAMSIZ 16
#endif

/* nsk setsockopts */
#define NETLINK_ADD_MEMBERSHIP 1
#define NETLINK_DROP_MEMBERSHIP 2
#define NETLINK_CAP_ACK 10
#define NETLINK_EXT_ACK 11

/* nsk flags */
#define NETLINK_F_CAP_ACK 0x20
#define NETLINK_F_EXT_ACK 0x40

struct sockaddr_nl {
	sa_family_t nl_family;  /* AF_NETLINK */
	unsigned short nl_pad;  /* zero */
	uint32_t nl_pid;  /* port ID */
	uint32_t nl_groups;  /* multicast groups mask */
};

struct nlmsghdr {
	uint32_t nlmsg_len;
	uint16_t nlmsg_type;
	uint16_t nlmsg_flags;
	uint32_t nlmsg_seq;
	uint32_t nlmsg_pid;
};

#define NLM_F_REQUEST		0x01
#define NLM_F_MULTI		0x02
#define NLM_F_ACK		0x04
#define NLM_F_ECHO		0x08
#define NLM_F_DUMP_INTR		0x10
#define NLM_F_DUMP_FILTERED	0x20
#define NLM_F_ROOT	0x100
#define NLM_F_MATCH	0x200
#define NLM_F_ATOMIC	0x400
#define NLM_F_DUMP	(NLM_F_ROOT | NLM_F_MATCH)

#define NLM_F_REPLACE	0x100
#define NLM_F_EXCL	0x200
#define NLM_F_CREATE	0x400
#define NLM_F_APPEND	0x800

#define NLM_F_NONREC	0x100

#define NLM_F_CAPPED	0x100
#define NLM_F_ACK_TLVS	0x200

#define NLMSG_NOOP	0x1
#define NLMSG_ERROR	0x2
#define NLMSG_DONE	0x3
#define NLMSG_OVERRUN	0x4

struct nlmsgerr {
	int		error;
	struct nlmsghdr msg;
};

#define NLMSG_ALIGNTO		4U
#define NLMSG_ALIGN(len)	(((len) + NLMSG_ALIGNTO - 1U) & ~(NLMSG_ALIGNTO - 1U))
#define NLMSG_HDRLEN		((unsigned int) NLMSG_ALIGN((unsigned int)sizeof(struct nlmsghdr)))
#define NLMSG_LENGTH(len)	((len) + NLMSG_HDRLEN)
#define NLMSG_SPACE(len)	NLMSG_ALIGN(NLMSG_LENGTH(len))
#define NLMSG_DATA(nlh)		((void*)(((char*)(nlh)) + NLMSG_HDRLEN))
#define NLMSG_NEXT(nlh,len)	((len) -= NLMSG_ALIGN((nlh)->nlmsg_len), \
				(struct nlmsghdr*)(((char*)(nlh)) + NLMSG_ALIGN((nlh)->nlmsg_len)))
#define NLMSG_OK(nlh,len)	((len) >= (int)sizeof(struct nlmsghdr) && \
				 (nlh)->nlmsg_len >= sizeof(struct nlmsghdr) && \
				 (int)(nlh)->nlmsg_len <= (len))
#define NLMSG_PAYLOAD(nlh, len)	((nlh)->nlmsg_len - NLMSG_SPACE((len)))
#define NLMSG_TAIL(nlh)	((struct rtattr *) (((char *) (nlh)) + NLMSG_ALIGN((nlh)->nlmsg_len)))

#define NLMSG_MIN_TYPE		0x10

#define NETLINK_ROUTE		0 /* Routing/device hook				*/
#define NETLINK_UNUSED		1 /* Unused number				*/
#define NETLINK_USERSOCK	2 /* Reserved for user mode socket protocols 	*/
#define NETLINK_FIREWALL	3 /* Unused number, formerly ip_queue		*/
#define NETLINK_SOCK_DIAG	4 /* socket monitoring				*/
#define NETLINK_NFLOG		5 /* netfilter/iptables ULOG */
#define NETLINK_XFRM		6 /* ipsec */
#define NETLINK_SELINUX		7 /* SELINUX event notifications */
#define NETLINK_ISCSI		8 /* Open-iSCSI */
#define NETLINK_AUDIT		9 /* auditing */
#define NETLINK_FIB_LOOKUP	10
#define NETLINK_CONNECTOR	11
#define NETLINK_NETFILTER	12 /* netfilter subsystem */
#define NETLINK_IP6_FW		13
#define NETLINK_DNRTMSG		14 /* DECnet routing messages */
#define NETLINK_KOBJECT_UEVENT	15 /* Kernel messages to userspace */
#define NETLINK_GENERIC		16
/* leave room for NETLINK_DM (DM Events) */
#define NETLINK_SCSITRANSPORT	18 /* SCSI Transports */
#define NETLINK_ECRYPTFS	19
#define NETLINK_RDMA		20
#define NETLINK_CRYPTO		21 /* Crypto layer */
#define NETLINK_SMC			22 /* SMC monitoring */

enum rt_scope_t {
	RT_SCOPE_UNIVERSE = 0,

	RT_SCOPE_SITE = 200,
	RT_SCOPE_LINK = 253,
	RT_SCOPE_HOST = 254,
	RT_SCOPE_NOWHERE = 255
};

#define RTM_F_NOTIFY		0x100
#define RTM_F_CLONED		0x200
#define RTM_F_EQUALIZE		0x400
#define RTM_F_PREFIX		0x800
#define RTM_F_LOOKUP_TABLE	0x1000
#define RTM_F_FIB_MATCH		0x2000


enum rt_class_t {
	RT_TABLE_UNSPEC = 0,

	RT_TABLE_COMPAT = 252,
	RT_TABLE_DEFAULT = 253,
	RT_TABLE_MAIN = 254,
	RT_TABLE_LOCAL = 255,
	RT_TABLE_MAX = 0xFFFFFFFF
};

enum rtattr_type_t {
	RTA_UNSPEC,
	RTA_DST,
	RTA_SRC,
	RTA_IIF,
	RTA_OIF,
	RTA_GATEWAY,
	RTA_PRIORITY,
	RTA_PREFSRC,
	RTA_METRICS,
	RTA_MULTIPATH,
	RTA_PROTOINFO,
	RTA_FLOW,
	RTA_CACHEINFO,
	RTA_SESSION,
	RTA_MP_ALGO,
	RTA_TABLE,
	RTA_MARK,
	RTA_MFC_STATS,
	RTA_VIA,
	RTA_NEWDST,
	RTA_PREF,
	RTA_ENCAP_TYPE,
	RTA_ENCAP,
	RTA_EXPIRES,
	RTA_PAD,
	RTA_UID,
	RTA_TTL_PROPAGATE,
	RTA_IP_PROTO,
	RTA_SPORT,
	RTA_DPORT,
	__RTA_MAX
};

#define RTA_MAX (__RTA_MAX - 1)
#define RTM_RTA(r)  ((struct rtattr*)(((char*)(r)) + NLMSG_ALIGN(sizeof(struct rtmsg))))
#define RTM_PAYLOAD(n) NLMSG_PAYLOAD(n, sizeof(struct rtmsg))

struct rtnexthop {
	unsigned short		rtnh_len;
	unsigned char		rtnh_flags;
	unsigned char		rtnh_hops;
	int			rtnh_ifindex;
};

#define RTNH_F_DEAD		1
#define RTNH_F_PERVASIVE	2
#define RTNH_F_ONLINK		4
#define RTNH_F_OFFLOAD		8
#define RTNH_F_LINKDOWN		16
#define RTNH_F_UNRESOLVED	32

#define RTNH_COMPARE_MASK	(RTNH_F_DEAD | RTNH_F_LINKDOWN | RTNH_F_OFFLOAD)

#define RTNH_ALIGNTO	4
#define RTNH_ALIGN(len) (((len) + RTNH_ALIGNTO - 1) & ~(RTNH_ALIGNTO - 1))
#define RTNH_OK(rtnh,len) ((rtnh)->rtnh_len >= sizeof(struct rtnexthop) && \
			   ((int)(rtnh)->rtnh_len) <= (len))
#define RTNH_NEXT(rtnh)	((struct rtnexthop*)(((char*)(rtnh)) + RTNH_ALIGN((rtnh)->rtnh_len)))
#define RTNH_LENGTH(len) (RTNH_ALIGN(sizeof(struct rtnexthop)) + (len))
#define RTNH_SPACE(len)	RTNH_ALIGN(RTNH_LENGTH(len))
#define RTNH_DATA(rtnh)   ((struct rtattr*)(((char*)(rtnh)) + RTNH_ALIGN(sizeof(struct rtnexthop))))

struct rta_cacheinfo {
	uint32_t	rta_clntref;
	uint32_t	rta_lastuse;
	int		rta_expires;
	uint32_t	rta_error;
	uint32_t	rta_used;

#define RTNETLINK_HAVE_PEERINFO 1
	uint32_t	rta_id;
	uint32_t	rta_ts;
	uint32_t	rta_tsage;
};

enum {
	RTAX_UNSPEC,
	RTAX_LOCK,
	RTAX_MTU,
	RTAX_WINDOW,
	RTAX_RTT,
	RTAX_RTTVAR,
	RTAX_SSTHRESH,
	RTAX_CWND,
	RTAX_ADVMSS,
	RTAX_REORDERING,
	RTAX_HOPLIMIT,
	RTAX_INITCWND,
	RTAX_FEATURES,
	RTAX_RTO_MIN,
	RTAX_INITRWND,
	RTAX_QUICKACK,
	RTAX_CC_ALGO,
	RTAX_FASTOPEN_NO_COOKIE,
	__RTAX_MAX
};

#define RTAX_MAX (__RTAX_MAX - 1)

#define RTAX_FEATURE_ECN	(1 << 0)
#define RTAX_FEATURE_SACK	(1 << 1)
#define RTAX_FEATURE_TIMESTAMP	(1 << 2)
#define RTAX_FEATURE_ALLFRAG	(1 << 3)

#define RTAX_FEATURE_MASK	(RTAX_FEATURE_ECN | RTAX_FEATURE_SACK | \
				 RTAX_FEATURE_TIMESTAMP | RTAX_FEATURE_ALLFRAG)

enum {
	RTM_NEWLINK	= 16,
#define RTM_BASE RTM_NEWLINK
	RTM_DELLINK,
	RTM_GETLINK,
	RTM_SETLINK,
	RTM_NEWADDR	= 20,
	RTM_DELADDR,
	RTM_GETADDR,
	RTM_NEWROUTE	= 24,
	RTM_DELROUTE,
	RTM_GETROUTE,
	RTM_NEWNEIGH	= 28,
	RTM_DELNEIGH,
	RTM_GETNEIGH,
};

struct rtgenmsg {
	unsigned char rtgen_family;
};

struct ifinfomsg {
	unsigned char ifi_family;
	unsigned char __ifi_pad;
	unsigned short ifi_type;
	int ifi_index;
	unsigned ifi_flags;
	unsigned ifi_change;
};

struct nlattr {
	uint16_t nla_len;
	uint16_t nla_type;
};

struct ifaddrmsg {
	uint8_t		ifa_family;
	uint8_t		ifa_prefixlen;
	uint8_t		ifa_flags;
	uint8_t		ifa_scope;
	uint32_t	ifa_index;
};

#define NLA_ALIGNTO		4
#define NLA_ALIGN(len)		(((len) + NLA_ALIGNTO - 1) & ~(NLA_ALIGNTO - 1))
#define NLA_HDRLEN		((unsigned int) NLA_ALIGN(sizeof(struct nlattr)))

/* Macros to handle rtattributes */

#define RTA_ALIGNTO	4U
#define RTA_ALIGN(len) (((len) + RTA_ALIGNTO - 1) & ~(RTA_ALIGNTO - 1))
#define RTA_OK(rta,len) ((len) >= (int)sizeof(struct rtattr) && \
			 (rta)->rta_len >= sizeof(struct rtattr) && \
			 (rta)->rta_len <= (len))
#define RTA_NEXT(rta,attrlen)	((attrlen) -= (int)RTA_ALIGN((rta)->rta_len), \
				 (struct rtattr*)(uintptr_t)(((char*)(rta)) + RTA_ALIGN((rta)->rta_len)))

#define RTA_LENGTH(len)	(RTA_ALIGN(sizeof(struct rtattr)) + (len))
#define RTA_SPACE(len)	RTA_ALIGN(RTA_LENGTH(len))
#define RTA_DATA(rta)   ((void*)(((char*)(rta)) + RTA_ALIGN(sizeof(struct rtattr))))
#define RTA_PAYLOAD(rta) ((unsigned int)((rta)->rta_len) - RTA_ALIGN(sizeof(struct rtattr)))

struct rtmsg {
	unsigned char		rtm_family;
	unsigned char		rtm_dst_len;
	unsigned char		rtm_src_len;
	unsigned char		rtm_tos;

	unsigned char		rtm_table;
	unsigned char		rtm_protocol;
	unsigned char		rtm_scope;
	unsigned char		rtm_type;

	unsigned		rtm_flags;
};

enum {
	RTN_UNSPEC,
	RTN_UNICAST,
	RTN_LOCAL,
	RTN_BROADCAST,
	RTN_ANYCAST,
	RTN_MULTICAST,
	RTN_BLACKHOLE,
	RTN_UNREACHABLE,
	RTN_PROHIBIT,
	RTN_THROW,
	RTN_NAT,
	RTN_XRESOLVE,
	__RTN_MAX
};
#define RTN_MAX (__RTN_MAX - 1)

#define RTPROT_UNSPEC	0
#define RTPROT_REDIRECT	1
#define RTPROT_KERNEL	2
#define RTPROT_BOOT	3
#define RTPROT_STATIC	4

enum {
	IFLA_UNSPEC,
	IFLA_ADDRESS,
	IFLA_BROADCAST,
	IFLA_IFNAME,
	IFLA_MTU,
	IFLA_LINK,
	IFLA_QDISC,
	IFLA_MASTER,
	IFLA_OPERSTATE,
	IFLA_LINKINFO = 18,
#define IFLA_LINKINFO IFLA_LINKINFO

	__IFLA_MAX
};
#define IFLA_MAX (__IFLA_MAX - 1)

enum {
	IFLA_INFO_UNSPEC,
	IFLA_INFO_KIND,
	IFLA_INFO_DATA,
	IFLA_INFO_XSTATS,
	IFLA_INFO_SLAVE_KIND,
	IFLA_INFO_SLAVE_DATA,
	__IFLA_INFO_MAX,
};
#define IFLA_INFO_MAX	(__IFLA_INFO_MAX - 1)
#define MAX_TYPE_LEN (64 - sizeof(unsigned long))
#define IFLA_RTA(r)  ((struct rtattr*)(((char*)(r)) + NLMSG_ALIGN(sizeof(struct ifinfomsg))))
#define IFLA_PAYLOAD(n) NLMSG_PAYLOAD(n, sizeof(struct ifinfomsg))

/* ifa_flags */
#define IFA_F_SECONDARY		0x01
#define IFA_F_TEMPORARY		IFA_F_SECONDARY
#define	IFA_F_NODAD		0x02
#define IFA_F_OPTIMISTIC	0x04
#define IFA_F_DADFAILED		0x08
#define	IFA_F_HOMEADDRESS	0x10
#define IFA_F_DEPRECATED	0x20
#define IFA_F_TENTATIVE		0x40
#define IFA_F_PERMANENT		0x80
#define IFA_F_MANAGETEMPADDR	0x100
#define IFA_F_NOPREFIXROUTE	0x200
#define IFA_F_MCAUTOJOIN	0x400
#define IFA_F_STABLE_PRIVACY	0x800

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
	__IFA_MAX,
};
#define IFA_MAX (__IFA_MAX - 1)

struct ifa_cacheinfo {
	uint32_t	ifa_prefered;
	uint32_t	ifa_valid;
	uint32_t	cstamp;
	uint32_t	tstamp;
};

#define IFA_RTA(r)  ((struct rtattr*)(((char*)(r)) + NLMSG_ALIGN(sizeof(struct ifaddrmsg))))
#define IFA_PAYLOAD(n) NLMSG_PAYLOAD(n, sizeof(struct ifaddrmsg))

typedef struct rt_entry {
	uint32_t dst;  // dst ip addr
	uint8_t dst_len;  // dst mask len, default 32
	uint32_t src;  // src ip addr
	uint8_t src_len;  // src mask len, default 32
	uint32_t flags;
	uint32_t type;
	uint32_t scope;
	char oifname[IFNAMSIZ]; // oif dev->name
	char iifname[IFNAMSIZ]; // iif dev->name
	uint32_t gw;
	uint32_t table;
	uint32_t presrc;
	uint32_t metric;
}rt_entry_t;

struct rtattr {
	unsigned short	rta_len;
	unsigned short	rta_type;
};

int hm_netlink_unicast(int protocol, unsigned int src_portid,
			 unsigned int dst_port, const char *msg,
			 const size_t len);

int hm_netlink_unicast_nowait(int protocol, unsigned int src_portid,
			      unsigned int dst_port, const char *msg,
			      const size_t len);

int hm_netlink_broadcast(int protocol, unsigned int src_portid,
			 unsigned int dst_group, const char *msg,
			 const size_t len);

int hm_netlink_create_service(int protocol);
int hm_netlink_release_service(int protocol);

#ifdef __cplusplus
}
#endif
#endif
