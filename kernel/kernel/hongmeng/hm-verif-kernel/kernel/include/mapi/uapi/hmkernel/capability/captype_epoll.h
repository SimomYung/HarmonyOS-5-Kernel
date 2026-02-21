/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: Definition of Epoll captype
 * Author: Huawei OS Kernel Lab
 * Create: Thu Feb 23 14:40:01 2023
 */
/*
 * Must choose one GRANT type from ALL/NO/COARSE/FINE
 * Must choose one GRANTMOVE type from ALL/NO/COARSE
 */
CAPTYPE(Epoll, 22, CAPALLGRANT(Epoll)
		CAPNOGRANTMOVE(Epoll)
		CAPNODEFAULTCREF(Epoll)
		CAPRAWCREFRLOOKUP(Epoll)
		CAPDEFAULTSIZE(Epoll),
	CAPMETHOD(Epoll, Ctl)
	CAPMETHOD(Epoll, Wakeup)
	CAPMETHOD(Epoll, Cleanup)
	CAPMETHOD(Epoll, FetchReadyFds)
	CAPMETHOD(Epoll, FetchListenFds)
	CAPMETHOD(Epoll, FetchStatistics)
	CAPMETHOD(Epoll, SetDebugValue)
	CAPMETHOD(Epoll, GetDebugValue)
	)

#ifdef CAP_EXPORT_API
#include <hmasm/types.h>

enum {
	CRIGHT_EPOLL_RELEASE_BIT,
	CRIGHT_EPOLL_MAX
};
#define CRIGHT_EPOLL_RELEASE	(1U << CRIGHT_EPOLL_RELEASE_BIT)
#define __CAP_EPOLL_OBJ_SIZE	64U

struct __sysarg_epoll_info {
	int epfd;
	int flags;
	__u64 tock;
};

/* define the fd server type */
enum {
	__EP_SRV_FS_BIT = 1U,
	__EP_SRV_NET_BIT = 2U,
	__EP_SRV_KERNEL_BIT = 3U,

	__EP_SRV_MAX_BIT = 4U,
	__EP_SRV_ALWAYS_BIT = 5U,
};
#define FD_SERVER_FS		(1U << __EP_SRV_FS_BIT)
#define FD_SERVER_NET		(1U << __EP_SRV_NET_BIT)
#define FD_SERVER_KERNEL	(1U << __EP_SRV_KERNEL_BIT)
#define FD_SERVER_NONE		(1U << __EP_SRV_MAX_BIT)
#define FD_SERVER_ALWAYS	(1U << __EP_SRV_ALWAYS_BIT)
#define FD_SERVER_ALL		(FD_SERVER_FS | FD_SERVER_NET | FD_SERVER_KERNEL)

#define EVENTPOLL_KEY_KLOG	1U
#define EVENTPOLL_ENABLE_KLOG	1U

#define epoll_key(_key, _server)	((_key) << 4 | (_server))
struct __epoll_ctl_info_s {
	int fd;
	int op;
	__u64 key;
	__u64 data;
	__u32 events_waited;
	__u32 events_triggered;
	__u32 server;     /* FD_SERVER_FS or FD_SERVER_NET or FD_SERVER_KERNEL */
	__u32 cnode_idx;  /* cnode index of app */
};

struct __epoll_item_info_s {
	int fd;
	__u32 cnode;
	__u32 server;
	__u32 events;
};

#define __NR_MAX_ITEM	128
struct __epoll_fetch_info_s {
	int num;
	__u32 server;
	struct __epoll_item_info_s items[__NR_MAX_ITEM];
};

struct __epoll_statistics_info_s {
	__u64 last_notify_timestamp;
	__u32 last_notify_op;
	__u32 last_notify_events;
	__u32 events_waited;
	__u32 events_triggered;
	__u32 is_notify;
};

#define set_epoll_ctl_info(ctl_info, __fd, __op, __key, __events, __data) 	\
do {										\
	(ctl_info).fd = (__fd);							\
	(ctl_info).op = (__op);							\
	(ctl_info).key = (__key);						\
	(ctl_info).events_waited = (__events);					\
	(ctl_info).data = (__data);						\
} while (0)

#endif
