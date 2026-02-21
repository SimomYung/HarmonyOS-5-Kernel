/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Header file for ux event
 * Author: Huawei OS Kernel Lab
 * Create: Tue Sep 24 12:48:06 2019
 */

#ifndef __LIBEVENT_UX_EVENT_
#define __LIBEVENT_UX_EVENT_

#include <stdbool.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <hmkernel/errno.h>

#ifdef CONFIG_LSYSCALL_ENABLED
#include <libsysif/utils.h>

DECLARE_UPCALL_ACTVHDLR(handle_eventpoll_polllist_sync);
DECLARE_UPCALL_ACTVHDLR(handle_eventpoll_ppoll);
DECLARE_UPCALL_ACTVHDLR(handle_epoll_recheck);
DECLARE_UPCALL_ACTVHDLR(handle_select);
DECLARE_UPCALL_ACTVHDLR(handle_select_clear);
DECLARE_UPCALL_ACTVHDLR(handle_eventpoll_recheck);
#endif /* !CONFIG_LSYSCALL_ENABLED */

#define HMNET_CHANNEL	"hmnetd_ch"

#define SOCKET_SHM_SIZE 0xffff /* 64k */
#define SOCKET_CTL_SHM_SIZE 0x1000 /* 4k */
#define HM_NET_SHMEM_SEND_OFFSET	0x11000 /* SOCKET_SHM_SIZE + SOCKET_CTL_SHM_SIZE */
#define HM_PER_NETDEVICE_SIZE 0x28 /* 40 Byte */
#define HM_MAX_NETDEVICE_NUM 5012 /* 4096 vlan device + 1951 netdevice(8) + lo device(1) + space left(7) */
/* We have to make HM_NET_SHMEM_SIZE page aligned, so we add an offset 0xE0, so HM_NET_SHMEM_SIZE is 0x42000 */
#define HM_NET_SHMEM_SIZE (HM_PER_NETDEVICE_SIZE * HM_MAX_NETDEVICE_NUM + HM_NET_SHMEM_SEND_OFFSET + 0xE0)

#define SIOCSIFVLAN	0x8983 /* Set 802.1Q VLAN options */
#define SIOCETHTOOL     0x8946          /* Ethtool interface            */

#define SCM_MAX_FD 32U
#define FSSRV_PATHNAME_LEN 32 /* Consistent with SRVNAME_MAX_LEN */
struct ux_sock_owner;
typedef struct ux_scm_fd {
	int fds[SCM_MAX_FD];
	int type[SCM_MAX_FD]; /* fstype is SCM_FS_NET or SCM_FS_VFS */
	int file_type[SCM_MAX_FD]; /* fd->file_type */
	char fssrv_name[SCM_MAX_FD][FSSRV_PATHNAME_LEN];
	uint32_t src_cnode_idx;
	struct ux_sock_owner *send_owner;
	uint8_t count;
} ux_scm_fd_t;

typedef struct ux_scm_creds {
	pid_t pid;
	uid_t uid;
	gid_t gid;
	uint8_t passcd;
} ux_scm_creds_t;

typedef struct ux_scm_cookie {
	ux_scm_fd_t fds;
	ux_scm_creds_t creds;
} ux_scm_cookie_t;

#define MAX_MSG_SCMCOOKIES_LEN	sizeof(ux_scm_cookie_t)
#define MAX_SOCK_ADDRLEN	sizeof(struct sockaddr_storage)
#define MAX_MSG_CTRL_LEN	8192
#define MAX_MSG_TOLCTRL_LEN	(MAX_MSG_CTRL_LEN + MAX_MSG_SCMCOOKIES_LEN)

typedef struct ux_binder_args {
	int send_fd;
	uint32_t send_cidx;
	int recv_fd;
	uint32_t recv_cidx;
	/* scm use */
	int sock_fd;
	int fstype;
	int file_type;
	char fssrv_name[FSSRV_PATHNAME_LEN];
	bool is_scm;
} ux_binder_args_t;

#define SCM_FS_NET		1
#define SCM_FS_VFS		2
#define SCM_FS_SOCKFS		3
#define SCM_FS_INVALID		0

static inline bool is_recv_flag_valid(unsigned int flags)
{
	if ((flags & ~(MSG_PEEK | MSG_DONTWAIT | MSG_NOSIGNAL |
	     MSG_CMSG_CLOEXEC | MSG_TRUNC | MSG_WAITALL | MSG_CTRUNC)) != 0) {
		return false;
	}

	return true;
}

#endif
