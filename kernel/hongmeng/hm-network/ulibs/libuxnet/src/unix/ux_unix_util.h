/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Util function for network unix
 * Author: Huawei OS Kernel Lab
 * Create: Mon Oct 12 10:21:00 2020
 */

#ifndef __UX_UNIX_UTIL_H_
#define __UX_UNIX_UTIL_H_

#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <vfs.h>

#include "ux_socket.h"
#include "ux_buf_queue.h"
#include "ux_eventpoll.h"
#include "ux_unix_fastpath.h"

#include <libhmsync/atomic.h>
#include <libhmsync/raw_rwlock.h>
#include <libhmsync/raw_scopedptr.h>
#include <libalgo/rb_htable.h>

#define SYS_EOF       0x8ff
#define USK_PATH_PADDING_SIZE	4
#define USK_PATH_USER_MAX	108
#define USK_PATH_LOCAL_MAX	(USK_PATH_USER_MAX + USK_PATH_PADDING_SIZE)
#define RCV_SHUTDOWN    1
#define SEND_SHUTDOWN   2
#define BIND_FILE_PERM	0644

#define RECVQUE_SHIFT   2

#define usk_is_connecting(usk)      \
	((usk)->state == SS_CONNECTING && (usk)->peer != NULL)
#define NR_USK	4096
#define USKPROTO	0

#ifndef OPEN_MAX
#define OPEN_MAX	64
#endif

#define min(a, b) \
	({ __typeof__ (a) _a = (a); \
	   __typeof__ (b) _b = (b); \
	   _a < _b ? _a : _b; })

typedef enum {
	BINDTYPE_AUTO_BIND,
	BINDTYPE_BSD,
	BINDTYPE_ABSTRACT,
	BINDTYPE_INVALID,
} unix_socket_bindtype_t;

#define ux_unix_pkt_truesize(x) ((int)((x) + sizeof(ux_unix_buf_t)))
struct ux_unix_sock;
typedef struct ux_unix_sock {
	ux_sock_t sk;

	int type;
	int state;

	struct ux_unix_sock *peer;

	raw_atomic_int_t send_buffer_used;

	unsigned short queued;     /* current nr of sockets on queue */
	unsigned short backlog;    /* maximum nr of connecting sockets */

	unsigned char shutdown;
	bool isbind;
	ux_buf_head_t recv_queue;
	union {
		struct dlist_node bindlnode; /* for bsd path */
		struct rbt_node bindrnode; /* for abstract path */
	} u;
	struct dlist_node head;

	struct ux_wq_head recvwq;
	struct ux_wq_head sendwq;
	struct ux_wq_head connwq;
	struct ux_wq_head acceptwq;

	unsigned char pathlen;     /* socket file path length (w/o nul) */
	char path[USK_PATH_LOCAL_MAX];  /* socket file path (null terminated) */

	vfs_unix_node_t unode;

	struct dlist_node queue;      /* queue of conned sockets */
	struct dlist_node queuenode;

	struct raw_mutex queuelock;

	enum unix_fastpath_type fp_type;
	void *rbuffer;

	struct ucred cred;
	pid_t root_ns_pid;
} ux_unix_sock_t;

typedef struct ux_unix_buf {
	struct raw_refcnt ref;

	ux_scm_creds_t creds;
	ux_scm_fd_t scm_fd;

	size_t real_size;
	ux_unix_sock_t *allocator;

	ux_buf_t ux_buf;    /* must be the last member */
} ux_unix_buf_t;

struct unix_path_key {
	const char *path;
	ux_namespace_t *ns;
	unsigned char pathlen;
};

ux_unix_buf_t *ux_unix_alloc_buffer(ux_unix_sock_t *allocator, size_t size);
int ux_unix_buff_get(ux_unix_buf_t *buf);
void ux_unix_buff_put(ux_unix_buf_t *buf);

static inline int ux_unix_send_buffer_used(ux_unix_sock_t *usk)
{
	return raw_atomic_int_read(&usk->send_buffer_used);
}

static inline int ux_unix_send_buffer_add(ux_unix_sock_t *usk, int val)
{
	return raw_atomic_int_add(&usk->send_buffer_used, val);
}

static inline int ux_unix_send_buffer_sub(ux_unix_sock_t *usk, int val)
{
	return raw_atomic_int_sub(&usk->send_buffer_used, val);
}

static inline ux_unix_sock_t *usk_peer(ux_unix_sock_t *usk)
{
	return usk->peer;
}

static inline bool is_connect_established(ux_unix_sock_t *usk)
{
	return ((usk->state == SS_CONNECTED) || usk_is_connecting(usk));
}

static inline size_t ux_unix_buf_truesize(const ux_buf_head_t *buf_queue)
{
	return buf_queue->len + buf_queue->qlen * sizeof(ux_unix_buf_t);
}

static inline bool is_sndbuf_exhausted(const ux_unix_sock_t *usk, ux_unix_sock_t *pusk, size_t len)
{
	if ((int)ux_unix_buf_truesize(&pusk->recv_queue) + ux_unix_pkt_truesize(len) > usk->sk.sndbuf) {
		return true;
	}

	return false;
}

void set_unix_dac_enabled(bool is_enabled);

static inline ux_unix_sock_t *unix_sk(ux_sock_t *sk)
{
	return (ux_unix_sock_t *)(void *)sk;
}

static inline ux_unix_sock_t *unix_sock_from_socket(ux_socket_t *socket)
{
	ux_sock_t *sk = ux_socket_get_sock(socket);

	if (sk == NULL) {
		return NULL;
	} else {
		return unix_sk(sk);
	}
}

static inline void ux_unix_sock_lock(ux_unix_sock_t *usk)
{
	ux_sock_lock(&usk->sk);
}

static inline void ux_unix_sock_unlock(ux_unix_sock_t *usk)
{
	ux_sock_unlock(&usk->sk);
}

static inline void ux_unix_sock_pair_lock(ux_unix_sock_t *usk1, ux_unix_sock_t *usk2)
{
	if (usk1 == usk2 || usk2 == NULL) {
		ux_sock_lock(&usk1->sk);
	} else {
		(void)raw_mutex_lock_pair(&usk1->sk.lock, &usk2->sk.lock);
	}
}

static inline void ux_unix_sock_pair_unlock(ux_unix_sock_t *usk1, ux_unix_sock_t *usk2)
{
	if (usk1 == usk2 || usk2 == NULL) {
		ux_sock_unlock(&usk1->sk);
	} else {
		(void)raw_mutex_unlock_pair(&usk1->sk.lock, &usk2->sk.lock);
	}
}

static inline bool ux_is_accept_queue_full(const ux_unix_sock_t *usk)
{
	return usk->queued > usk->backlog;
}

static inline bool ux_is_accept_queue_empty(const ux_unix_sock_t *usk)
{
	return usk->queued == 0;
}

static inline bool ux_is_rd_shutdown(const ux_unix_sock_t *usk)
{
	return (usk->shutdown & RCV_SHUTDOWN) != 0;
}

static inline bool ux_is_wr_shutdown(const ux_unix_sock_t *usk)
{
	return (usk->shutdown & SEND_SHUTDOWN) != 0;
}

static inline bool ux_is_rdwr_shutdown(const ux_unix_sock_t *usk)
{
	return ux_is_rd_shutdown(usk) && ux_is_wr_shutdown(usk);
}

static inline bool if_unix_sock_bind_bsd(ux_unix_sock_t *usk)
{
	return usk->path[0] != '\0';
}

static inline bool if_unix_sock_bind_abstract(ux_unix_sock_t *usk)
{
	return usk->pathlen > 0 && usk->path[0] == '\0';
}

int ux_unix_init_root_ns_pid(ux_unix_sock_t *usk);
int ux_unix_copy_root_ns_pid(ux_unix_sock_t *from, ux_unix_sock_t *to);
void ux_unix_release_root_ns_pid(ux_unix_sock_t *usk);

int osal_get_ucred(struct ucred *ucred);
ux_unix_sock_t *ux_unix_sock_alloc_and_init(ux_socket_t *sock, int type, int protocol,
				   struct ucred *cred, ux_namespace_t *ns);

void ux_unix_make_addr(const char *path, unsigned char len, struct sockaddr_un *addr,
		       socklen_t *addr_len);
unsigned int epoll_get_shutdown_events(unsigned int uhow);
void notify_peer(ux_unix_sock_t *usk, ux_unix_sock_t *conn, unsigned int uhow, bool is_close);

int bindtable_init(void);

int unix_get_pusk(ux_unix_sock_t *usk, const struct netd_msghdr *msg, ux_unix_sock_t **peerp);
void ux_unix_get_peercred(ux_unix_sock_t *usk, struct ucred *optval);
void ux_unix_getpeerinfo(ux_unix_sock_t *usk, int *peer_pid, unsigned long long *peer_cookie);
int getsockopt_peercred(ux_unix_sock_t *usk, void *optval, socklen_t *optlen);
void ux_unix_clear_queue(ux_unix_sock_t *usk, const ux_unix_sock_t *except);
void usk_free(ux_unix_sock_t *usk);
void ux_unix_sock_free(ux_sock_t *sk);

ssize_t common_unix_sendmsg(ux_socket_t *sock, struct netd_msghdr *msg, int flags);
ssize_t common_unix_recvmsg(ux_socket_t *sock, struct netd_msghdr *msg, ux_scm_cookie_t *scm, int flags);

int check_un_addr(const struct sockaddr_un *addr, socklen_t addrlen);

void bind_path_release(ux_unix_sock_t *usk);
int __unix_bind(ux_unix_sock_t *usk, const struct sockaddr_un *addr_un, socklen_t addrlen);
int find_peer_by_addr(const struct sockaddr *addr, socklen_t addrlen, ux_namespace_t *ns, ux_unix_sock_t **peer);
#endif
