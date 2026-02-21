/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: vpipe header file
 * Author: Huawei OS Kernel Lab
 * Create: Sat May 08 15:10:22 2021
 */

#ifndef LIBVNOTIFY_VPIPE_H
#define LIBVNOTIFY_VPIPE_H

#include <fd.h>
#include <vfs_epoll.h>
#include <hongmeng/errno.h>
#include <libvpipe/vnotify.h>

void hm_vpipe_fork_reinit(void);
void hm_vpipe_lock_parent(void);
void hm_vpipe_unlock_parent(void);
int hm_vpipe_notify(int vpfd);
int hm_vpipe_fast_notify(int vpfd);
int hm_vpipe_receive(int vpfd, const struct timespec *ts,
		     unsigned int flags);
int hm_vpipe_fast_receive(int vpfd, const struct timespec *ts, unsigned int flags);
int hm_vpipe_inject_event(int vpfd, unsigned int event);
int hm_vpipe_chnid_of(int vpfd);
int hm_vpipe_attach_chn(const char *name, unsigned int flags);
int hm_vpipe_detach_chn(int vpfd);
int hm_vpipe_update_futex(int vpfd, int *futex, bool shared);
void post_epoll_ctl_handler(int fd, int op, int fd2, const struct epoll_event *ev);

#endif
