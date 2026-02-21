/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: For ILP32 compat
 * Author: Huawei OS Kernel Lab
 * Create: Tue Aug 25 10:35:04 2020
 */

#ifndef __UX_NET_COMPAT_H_
#define __UX_NET_COMPAT_H_

#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/route.h>
#include <libhmactv/actv.h>
#include <hongmeng/panic.h>

#include "ux_sock_filter.h"

#define SO_RCVTIMEO_OLD		20
#define SO_SNDTIMEO_OLD		21
#define SO_RCVTIMEO_NEW		66
#define SO_SNDTIMEO_NEW		67

#define SIOCGSTAMP_OLD		0x8906
#define SIOCGSTAMP_NEW		_IOR(0x89, 6, char[16])

#if defined(CONFIG_COMPAT)
#define MSG_CMSG_COMPAT		0x80000000
#else
#define MSG_CMSG_COMPAT		0
#endif

typedef uint32_t uintptr32_t;

/* for ILP32 */
struct timespec32 {
	int	tv_sec;
	int	tv_nsec;
};

struct timeval32 {
	int	tv_sec;
	int	tv_usec;
};

struct compat_itimerspec {
	struct timespec32 it_interval;
	struct timespec32 it_value;
};

struct compat_stat {
	unsigned long long st_dev;
	int __st_dev_padding;
	int __st_ino_truncated;
	unsigned int st_mode;
	unsigned int st_nlink;
	unsigned int st_uid;
	unsigned int st_gid;
	unsigned long long st_rdev;
	int __st_rdev_padding;
	long long st_size;
	int st_blksize;
	long long st_blocks;
	struct timespec32 st_atim;
	struct timespec32 st_mtim;
	struct timespec32 st_ctim;
	unsigned long long st_ino;
};

struct msghdr32 {
	uintptr32_t msg_name;
	socklen_t msg_namelen;
	uintptr32_t msg_iov;
	int msg_iovlen;
	uintptr32_t msg_control;
	socklen_t msg_controllen;
	int msg_flags;
};

struct netd_mmsghdr32 {
	struct msghdr32 msg_hdr;
	unsigned int msg_len;
};

struct iovec32 {
	uintptr32_t iov_base;
	unsigned int iov_len;
};

struct cmsghdr32 {
	unsigned int cmsg_len;
	int	cmsg_level;
	int	cmsg_type;
};

#define CMSG32_ALIGN(len) (((unsigned long)(len) + 3UL) & 0xFFFFFFFCUL)
#define CMSG32_DATA(cmsg) \
	((char *)(void *)(cmsg) + sizeof(struct cmsghdr32))
#define CMSG32_LEN(len) \
	(sizeof(struct cmsghdr32) + (len))
#define CMSG32_SPACE(len) \
	(sizeof(struct cmsghdr32) + CMSG32_ALIGN(len))

#define CMSG32_FIRSTHDR(mhdr) \
	((size_t) (mhdr)->msg_controllen >= sizeof (struct cmsghdr32) ? \
	(struct cmsghdr32 *) (mhdr)->msg_control : (struct cmsghdr32 *) NULL)

#define CMSG32_OK(cmlen, cmsg, head, msg_ctl_len)	\
	(((cmlen) >= (unsigned int)sizeof(struct cmsghdr32)) &&		\
	 ((cmlen) <= ((unsigned int)(msg_ctl_len) - (unsigned int)((char *)(cmsg) - (char *)(head)))))

static inline struct cmsghdr32 *cmsg32_nxthdr(const struct cmsghdr32 *head,
					      socklen_t msg_ctl_len,
					      struct cmsghdr32 *cmsg, unsigned int cmsg_len)
{
	char *ptr = (char *)cmsg + CMSG32_ALIGN(cmsg_len);
	if ((unsigned long)(ptr + 1 - (char *)head) > msg_ctl_len)
		return NULL;
	return (struct cmsghdr32 *)(uintptr_t)ptr;
}

unsigned int ux_get_compat_cmsglen(const struct msghdr *msg);

struct ifmap32 {
	unsigned int mem_start;
	unsigned int mem_end;
	unsigned short int base_addr;
	unsigned char irq;
	unsigned char dma;
	unsigned char port;
};

struct ifreq32 {
	union {
		char ifrn_name[IFNAMSIZ];
	} ifr_ifrn;
	union {
		struct sockaddr ifru_addr;
		struct sockaddr ifru_dstaddr;
		struct sockaddr ifru_broadaddr;
		struct sockaddr ifru_netmask;
		struct sockaddr ifru_hwaddr;
		short int ifru_flags;
		int ifru_ivalue;
		int ifru_mtu;
		struct ifmap32 ifru_map;
		char ifru_slave[IFNAMSIZ];
		char ifru_newname[IFNAMSIZ];
		unsigned int ifru_data;
	} ifr_ifru;
};

struct ifconf32 {
	int ifc_len;
	union {
		unsigned int ifcu_buf;
		unsigned int ifcu_req;
	} ifc_ifcu;
};

/* do not support ifru_map and ifru_data */
static inline void ux_ifreq2compat_ifreq_imperfectly(const struct ifreq *ifr,
						     struct ifreq32 *ifr32)
{
	*ifr32 = *(struct ifreq32*)ifr;
}

/* do not support ifru_map and ifru_data */
static inline void ux_compat_ifreq2ifreq_imperfectly(const struct ifreq32 *ifr32,
						     struct ifreq *ifr)
{
	*(struct ifreq32*)ifr = *ifr32;
}

struct rtentry32 {
	unsigned int rt_pad1;
	struct sockaddr rt_dst;
	struct sockaddr rt_gateway;
	struct sockaddr rt_genmask;
	unsigned short int rt_flags;
	short int rt_pad2;
	unsigned int rt_pad3;
	unsigned char rt_tos;
	unsigned char rt_class;
	/* 2 refer to musl struct rtentry */
	short int rt_pad4[sizeof(int) / 2 - 1];
	short int rt_metric;
	unsigned int rt_dev; /* this for char * */
	unsigned int rt_mtu;
	unsigned int rt_window;
	unsigned short int rt_irtt;
};

static inline void *compat_ptr(uintptr32_t uptr)
{
	return (void *)(uintptr_t)uptr;
}

static inline bool ux_is_compat_client(void)
{
	return hm_actv_local_is_compat();
}

struct ux_compat_sock_fprog {
	uint16_t len;
	unsigned int filter; /* unsigend int eq void * in ILP32 */
};

int ux_get_compat_fprog(const void *optval, struct ux_sock_fprog *fprog);

int ux_get_compat_timeval(const void *optval, struct timeval *tv);
int ux_set_compat_timeval(const struct timeval *tv, void *optval);

int ux_compat_cmsghdr2cmsghdr(struct msghdr *msg);
int ux_cmsghdr2compat_cmsghdr(struct msghdr *msg, char *msg_control);

void ux_rtentry2trentry32(const struct rtentry *rt, struct rtentry32 *rt32);
void ux_rtentry322trentry(const struct rtentry32 *rt32, struct rtentry *rt);

int ux_get_compat_msghdr(const void *msg, struct msghdr *srv_msg);
int ux_get_compat_iov(const void *iov, size_t iovlen, struct iovec *srv_iov);

void ux_msghdr32_to_msghdr(const struct msghdr32 *msg32, struct msghdr *msg);

void ux_iovec32_to_iovec(const struct iovec32 *iov32, struct iovec *iov);

void ux_compat_fixup_stat(struct compat_stat *dst, const struct stat *src);

#endif
