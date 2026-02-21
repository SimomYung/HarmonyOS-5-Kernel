/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020-2021. All rights reserved.
 * Description: State epoll server common interface and data structure
 * Author: Huawei OS Kernel Lab
 * Create: Wed Feb 12 21:00:44 2020
 */
#ifndef ULIBS_INCLUDE_LIBHMSRV_SYS_HM_SELECT_COMMON_H
#define ULIBS_INCLUDE_LIBHMSRV_SYS_HM_SELECT_COMMON_H

#include <lib/dlist.h>
#include <libalgo/rbtree.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_scopedptr.h>
#include <sys/epoll.h>
#include <sys/select.h>
#include <time.h>

#define EPOLLIN_CACHE_EVENT	0x00000800U
#define EPOLL_RECHECK		0x00020000U
#define DEFAULT_POLLMASK	(EPOLLIN | EPOLLOUT | EPOLLRDNORM | EPOLLWRNORM)
#define EPOLLINCACHE		EPOLLIN_CACHE_EVENT
#define EPOLLRECHECK		EPOLL_RECHECK
#define EPOLL_DEFAULT_EVENTS	(EPOLLHUP | EPOLLERR)

/* same with epoll.h in kernel */
static inline uint32_t retrive_valid_events(uint32_t events, uint32_t revents)
{
	uint32_t rev = revents;
	if ((revents & EPOLLINCACHE) != 0U) {
		rev |= EPOLLIN;
	}
	return (uint32_t)((events | EPOLL_DEFAULT_EVENTS | EPOLLRECHECK) & rev);
}

#define EVENTPOLL_TYPE_EPOLL	0
#define EVENTPOLL_TYPE_PPOLL	1
#define eventpoll_ops_exec(name, ops, ...)		\
do {							\
	if ((ops) != NULL && (ops)->name != NULL) {	\
		(ops)->name(__VA_ARGS__);		\
	}						\
} while (0)

struct eventpoll_ops_t;
struct eventpoll_node_s {
	struct dlist_node link;
	uint64_t key;
	uint32_t events;
	int fd;
	struct eventpoll_ops_t *ops;
};

struct eventpoll_ops_t {
	void (*wakeup)(struct eventpoll_node_s *node, uint32_t revents);
	void (*cleanup)(struct eventpoll_node_s *node, uint32_t revents);
	void (*close)(struct eventpoll_node_s *node, uint32_t revents);
	void (*remove)(struct eventpoll_node_s *node, uint64_t private, void **mem);
	void (*add_event)(struct eventpoll_node_s *node, uint32_t revents);
};

struct ppoll_info_args {
	uint64_t tcb_key;
	rref_t fdtable_rref;
	uint32_t events;
	int idx;
	int fd;
};

struct ppoll_info_s {
	/* use tcb cref as key to find ppoll_s in kernel */
	uint64_t tcb_key;
	rref_t fdtable_rref;
	uint32_t revents;
	unsigned int cnode;
	int idx;
};

#define ppoll_info_init(_poll, _tcb, _rref, _revents, _cnode, _idx)	\
do {									\
	(_poll)->tcb_key = (uint64_t)(_tcb);				\
	(_poll)->fdtable_rref = (rref_t)(_rref);			\
	(_poll)->revents = (uint32_t)(_revents);			\
	(_poll)->cnode =  (unsigned int)(_cnode);			\
	(_poll)->idx =  (int)(_idx);					\
} while (0)

struct ppoll_node {
	struct eventpoll_node_s node;
	struct ppoll_info_s info;
};

/* for epoll sink */
struct epoll_cap_grant_s {
	__u64 tock;
	cref_t cref;
	/* ep_rref is used for syscap. vfs does not require authorization. Therefore, ep_rref = ep_cref. */
	rref_t rref;
};

struct epoll_node {
	struct eventpoll_node_s node;
	struct epoll_cap_grant_s cap;
};

struct epoll_cap_info_s {
	struct epoll_cap_grant_s cap;
	int epfd;
	unsigned int ap_cnode;
};

struct __epoll_ctl_info_s;

struct epoll_info_arg {
	/* The remotefd command is used in signal epoll. */
	int remote_fd;
	struct epoll_cap_info_s cap_info;
	struct __epoll_ctl_info_s ctl_info;
};

#define set_epoll_cap_info(cap_info, __tock, __cref, __rref) 			\
do {										\
	(cap_info).cap.tock = (__tock);						\
	(cap_info).cap.cref = (__cref);						\
	(cap_info).cap.rref = (__rref);						\
} while (0)

/* The interfaces provided by the new epoll are as follows: */
int eventpoll_ctl_common(struct dlist_node *epoll_list, struct epoll_info_arg *arg, uint32_t revents,
			 struct epoll_node **ep_info);
int eventpoll_cap_epoll_grant(int epfd, unsigned int ap_cnode, struct epoll_cap_grant_s *ep);

int eventpoll_add(struct dlist_node *eventpoll_list,
		  struct ppoll_info_s *info,
		  uint32_t events, int fd, uint64_t key,
		  struct ppoll_node **node);

/* event comes */
void eventpoll_wakeup_sink(struct dlist_node *eventpoll_list, uint32_t revents);
/* clear event bits */
void eventpoll_cleanup_sink(struct dlist_node *eventpoll_list, uint32_t revents,
			    uint64_t key);
/* set event bits */
void eventpoll_add_event_sink(struct dlist_node *eventpoll_list, uint32_t revents,
				  uint64_t key);
/* report events and delete from eventpoll_list, as file closes */
void eventpoll_close_sink(struct dlist_node *eventpoll_list, uint32_t revents,
			  uint64_t key);

/* remove node form list */
void eventpoll_remove_sink(struct dlist_node *eventpoll_list, uint64_t key, uint64_t private, void **mem);

/* for select */
#define POLLIN_SET	(EPOLLRDNORM | EPOLLRDBAND | EPOLLIN | EPOLLHUP | EPOLLERR | \
			 EPOLLIN_CACHE_EVENT | EPOLLNVAL)
#define POLLOUT_SET	(EPOLLWRBAND | EPOLLWRNORM | EPOLLOUT | EPOLLERR | EPOLLNVAL)
#define POLLEX_SET	(EPOLLPRI | EPOLLNVAL)

/* compatiable with the define in select.h */
#define SELECT_NONE		0U
#define SELECT_VFS_READ		1U
#define SELECT_VFS_WRITE	2U
#define SELECT_VFS_EXP		4U
#define SELECT_NET_READ		0x10U
#define SELECT_NET_WRITE	0x20U
#define SELECT_NET_EXP		0x40U
#define SELECT_BADFD		0x80U
#define SELECT_ONE_SHOT		0x100U
#define SELECT_COMPAT		0x200U
#define SELECT_NET		(SELECT_NET_READ | SELECT_NET_WRITE | SELECT_NET_EXP)
#define SELECT_VFS		(SELECT_VFS_READ | SELECT_VFS_WRITE | SELECT_VFS_EXP)

#define NFDS_TO_SIZE(n)		((((n) + 63) / 64) * 8)
#define FDSET_SIZE(n)		(size_t)(NFDS_TO_SIZE(n) > sizeof(fd_set) ? sizeof(fd_set) : NFDS_TO_SIZE(n))

/* the fdset type of select/pselect */
enum {
	FDSET_IDX_READ = 0,
	FDSET_IDX_WRITE,
	FDSET_IDX_EXP,
	FDSET_IDX_MAX,
};

struct select_ctl_info_s {
	void *fdsets[FDSET_IDX_MAX]; /* the pointer of fd_set in uapp */
	fd_set tmpfds[FDSET_IDX_MAX];
	cref_t tcb_cref;
	uint32_t type;
	uint32_t nfds;
	uint32_t cnode_idx;
};
typedef int (*select_handler)(struct select_ctl_info_s *select, rref_t fdtable, uint32_t cnode);

int eventpoll_select_common(struct select_ctl_info_s *select, rref_t fdtable, select_handler func);
uint32_t select_waited_events(int fd, const fd_set tmpfds[]);
int select_triggered_events(int fd, uint32_t revents, fd_set tmpfds[]);

#endif /* ULIBS_INCLUDE_LIBHMSRV_SYS_HM_SELECT_COMMON_H */
