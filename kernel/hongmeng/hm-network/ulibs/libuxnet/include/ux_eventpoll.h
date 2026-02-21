/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description:  Header file for eventpoll
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 30 14:35:43 2019
 */

#ifndef __LIBUXNET_UX_EVENTPOLL_H__
#define __LIBUXNET_UX_EVENTPOLL_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <limits.h>
#include <ifaddrs.h>
#include <sys/epoll.h>

#include <lib/dlist.h>
#include <libalgo/rbtree.h>
#include <libhmsync/atomic.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_thread_cond.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsync/raw_scopedptr.h>
#include <libhmsrv_sys/hm_select_common.h>

#include "ux_socket.h"

/* event callback wakeup mask */
#define WAKEUP_MASK 0x0000ffff

/* event callback clean mask */
#define CLEAN_MASK 0xffff0000

#define CLEAN_EVENT_SHIFT 16
#define SHARE_POLLLIST  1

int ux_spawn_polllist(ux_socket_t *sock, ux_epoll_context_t *epoll_ctx, int use_polllist);
void ux_epoll_wakeup_clean(ux_epoll_context_t *ctx, uint32_t events);

int ux_epoll_copy_from_client(void *dst, const void *src, size_t bytes);

unsigned int do_epoll_getevents(ux_socket_t *sock);
int ux_epoll_recheck(ux_socket_t *sock, uint32_t events, uint32_t *revents);
void ux_clear_eventpoll_list(ux_socket_t *sock);
void ux_sync_polllist_revents(ux_socket_t *sock);
void ux_set_fd_config(ux_socket_t *sock, struct sysarg_fdtable_fd_install *config);

#endif
