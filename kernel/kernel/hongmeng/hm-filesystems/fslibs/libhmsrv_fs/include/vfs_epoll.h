/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: State epoll ulib interface and data structure
 * Author: Huawei OS Kernel Lab
 * Create: Fri Feb 28 16:12:42 2020
 */
#ifndef LIBHMSRV_FS_VFS_EPOLL_H
#define LIBHMSRV_FS_VFS_EPOLL_H

#include <vfs.h>
#include <poll.h>
#include <libhmsrv_sys/hm_select_common.h>
#include <libhmsync/raw_mutex.h>

#ifndef EPOLL_CLIENT_H
#define EPOLL_CLIENT_H
struct epoll_sigset {
	sigset_t *sigmask;
	size_t sigsize;
};
#endif /* EPOLL_CLIENT_H */

struct vfs_epoll_pwait_info {
	int epfd;
	struct epoll_event *ev;
	int maxevents;
	int timeout;
	struct epoll_sigset *sigset;
};

int vfs_epoll_rref_grant(struct server_info *si, int epfd, __u32 cnode_idx, rref_t in_rref, rref_t *out_rref);
int vfs_epoll_rref_acquire(struct server_info *si, int epfd, __u32 cnode_idx, struct epoll_cap_grant_s *ep);
#endif /* LIBHMSRV_FS_VFS_EPOLL_H */
