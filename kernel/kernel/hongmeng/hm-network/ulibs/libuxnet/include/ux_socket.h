/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description:  Header file for network socket
 * Author: Huawei OS Kernel Lab
 * Create: Sun Apr 28 13:34:09 2019
 */

#ifndef __LIBUXNET_UX_SOCKET_
#define __LIBUXNET_UX_SOCKET_

#include <sys/types.h>
#include <sys/socket.h>
#include <poll.h>
#include <limits.h>
#include <sys/epoll.h>
#include <ifaddrs.h>

#include <lib/dlist.h>
#include <libmem/umem.h>
#include <libalgo/rbtree.h>
#include <libhmsync/atomic.h>
#include <libhmsync/raw_mutex.h>
#include <libhmsync/raw_thread_cond.h>
#include <libhmsrv_sys/hm_mem.h>
#include <libhmsrv_sys/hm_ashm.h>
#include <libhmsync/raw_scopedptr.h>
#include <libhmsync/raw_compounded_refcnt.h>
#include <libhmsrv_sys/hm_select_common.h>
#include <libhmiov/iov_iter.h>
#include <libhmsrv_io/net_socket.h>
#include <hmkernel/ctrlmem/kobjs/polllist.h>

#include "bpf/ux_bpf_common.h"

#include <ux_event.h>
#include "ux_namespace.h"
#include "ux_fusion_ops.h"

#include "ux_netlog.h"
#include "ux_pktbuf.h"
#include "ux_utils.h"
#include "net_pac.h"

#define TV_HZ 1000
/* max sockets per process */
#define MAX_SOCKET_NUMS CONFIG_SOCKET_LIMIT_PER_PROC
#define SOCK_MAX (SOCK_PACKET + 1)
#define SOCK_TYPE_MASK	0xF

/*
 * Kernel use the socket pointer address to tell us whether to go fastcopy or not.
 * We need to correct the pointer address back.
 */
#define SOCKET_IS_ALIGNED(addr) (((addr) & 0x3) == 0)
#define SOCKET_CORRECT_FLAG(addr) \
	do { \
		addr = (long long)(uintptr_t)((char *)(uintptr_t)(addr) - 1); \
		BUG_ON(!SOCKET_IS_ALIGNED(addr)); \
	} while (0)

/*
 * 0x100000 is an internal flag in Linux for sendpage.
 * It shouldn't come from user input.
 * We reuse this bit to mark whether to go fastcopy or not.
 * If user does use this flag, we should ignore it and go normal path.
 */
#define MSG_FASTCOPY 0x100000
static inline int flags_set_fastcopy(int flags)
{
	return (int)((unsigned int)flags | MSG_FASTCOPY);
}

static inline int flags_clr_fastcopy(int flags)
{
	return (int)((unsigned int)flags & (~MSG_FASTCOPY));
}

static inline bool flags_need_fastcopy(int flags)
{
	return (((unsigned int)flags & MSG_FASTCOPY) != 0);
}

/*
 * We use some bits of flags to identify different features,
 * provided that these bits are not used for external standards
 * and for internal use only.
 * If these bits are passed in externally, we just ignore them.
 */
static inline int ignore_flags(int flags)
{
	return flags_clr_fastcopy(flags);
}

/*
 * 0x80000 is an internal flag in Linux for sendpage.
 */
#define MSG_RECVQUEUE 0x80000
static inline int flags_set_recvqueue(int flags)
{
	return (int)((unsigned int)flags | MSG_RECVQUEUE);
}

static inline int flags_clr_recvqueue(int flags)
{
	return (int)((unsigned int)flags & (~MSG_RECVQUEUE));
}

/* file or socket pointer upcall passed by the kernel */
DECLARE_UPCALL_ACTVHDLR(handle_common_file_close);

enum ux_sock_flags {
	SOCK_DEAD,
	SOCK_LINGER,
	SOCK_KEEPALIVE,
};

typedef struct {
	uint32_t send_cidx;
	uint32_t recv_cidx;
	int vfsfd_idx;
	char fssrv_name[FSSRV_PATHNAME_LEN];
	void *sock;
	void *scm_sock;
	int fstype;
} ux_sock_trans_info_t;

struct ux_socket;
typedef struct ux_sock_owner {
	struct rbt_node node;
	struct raw_mutex lock;
	struct raw_refcnt_nr refcnt;

	struct raw_mutex sslock;
	ux_sock_trans_info_t *ssocks;
	int ssocks_count;

	uint32_t key;
	int count;

	unsigned int max_sockets;
	struct ux_socket **sockets;

	rref_t fdtable_rref;
#ifdef CONFIG_NET_NS
	struct ux_namespace *ns;
#endif
} ux_sock_owner_t;

struct ux_proto_ops;
struct ux_priv_ops;
struct ux_domain;

typedef struct ux_epoll_context {
	struct raw_mutex epi_mutex;
	struct __ctrlmem_kobj_polllist_s *polllist;
	struct polllist_s *kobj_polllist;
} ux_epoll_context_t;

typedef struct ux_sock {
	struct raw_refcnt_nr refcnt;
	struct raw_mutex lock;
	unsigned int noblock;
	struct ux_conn_filter *filter;
	u32_t flags;
	int type;
#ifdef CONFIG_NET_NS
	struct ux_namespace *ns;
#endif

	struct raw_sem recvsem;
	struct raw_sem sendsem;

	struct ux_wq_head recv_wq;
	struct ux_wq_head send_wq;

	struct dlist_node rcv_queue;
	int rcv_buf_used;
	int socket_domain;
	int socket_protocol;

	ux_epoll_context_t epoll_ctx;

	long sndtimeo;
	long rcvtimeo;
	unsigned long lingertime;

	unsigned int reuse;
	int passcred;

	int rcvbuf;
	int sndbuf;
	int sk_err;

#ifdef CONFIG_SEHARMONY
	__sec_data(net, ptr) void *sk_security;
#endif
	struct ux_socket *socket;

	void (*release)(struct ux_sock *sk);
	void (*set_write_space)(struct ux_sock *sk, int val);
	void (*set_read_space)(struct ux_sock *sk, int val);
	int (*getsockevent)(struct ux_sock *sk, unsigned int *rcvev, unsigned int *sendev, unsigned int *errev);
} ux_sock_t;

#define IGNORED_SETFL_MASK (O_LARGEFILE | O_WRONLY | O_RDONLY | O_RDWR)
#define SETFL_MASK (O_NONBLOCK)
#define sock_enable_supported_fflags(origin, flags) (((flags) & SETFL_MASK) | ((origin) & ~(SETFL_MASK)))
#define sock_remove_ignored_fflags(flags) ((flags) & ~(IGNORED_SETFL_MASK))
#define sock_if_fflags_supported(flags) (((flags) & ~(SETFL_MASK)) == 0)

enum {
	SS_UNCONNECTED = 1,
	SS_CONNECTING,
	SS_CONNECTED,
	SS_DISCONNECTING,
	SS_LISTENING
};

#define __SO_ACCEPTCON (1 << 16) /* performed a listen */

#define UX_SOCK_TIMESTAMP 1

typedef struct ux_socket {
	struct raw_compounded_refcnt refcnt;

	struct raw_mutex lock;
	struct ux_proto_ops *ops;
	struct ux_priv_ops *priv_ops;

	ux_sock_t *sk;

	int state;

	unsigned int mark;

	/**
	 * HM socket do not have sockfs, and so do not have any inode concept;
	 * but we have to compatible with linux for the netstat use inode to
	 * calculate the pid and process name of tcp/udp/unix socket. @see netstat source
	 *
	 * Here we use the cookie as a pseudo inode.
	 *
	 * The inode in /proc/[pid]/fd must be consistent with /proc/net/unix,
	 * /proc/net/tcp and /proc/net/udp.
	 */
	unsigned long long cookie;

	/* store sockopt values */
	unsigned long flags;

	struct timeval ts;

	/*  for storing data received in batches */
	void *recv_queue;
} ux_socket_t;

struct netd_msghdr {
	void *msg_name;
	socklen_t msg_namelen;
	struct iovec *msg_iov;
	size_t msg_iovlen;
	void *msg_control;
	size_t msg_controllen;
	int msg_flags;

	/* iterator to access iovec */
	struct iov_iter msg_iter;
	bool is_user_iov;
};

struct netd_mmsghdr {
	struct msghdr msg_hdr;
	unsigned int  msg_len;
};

static inline unsigned long long ux_socket_ino(const ux_socket_t *socket)
{
	return socket->cookie;
}

static inline void ux_socket_set_sock(ux_socket_t *s, ux_sock_t *sock)
{
	BUG_ON(s == NULL);

	s->sk = sock;
}

static inline ux_sock_t *ux_socket_get_sock(const ux_socket_t *s)
{
	if (s->sk != NULL) {
		BUG_ON(raw_refcnt_nr_counter(&s->sk->refcnt) <= 0,
		       "illegal sk refcnt 0x%x\n", raw_refcnt_nr_counter(&s->sk->refcnt));
	}
	return s->sk;
}

static inline ux_epoll_context_t *ux_socket_get_epoll_ctx(const ux_socket_t *s)
{
	ux_sock_t *sk = ux_socket_get_sock(s);

	if (sk != NULL) {
		BUG_ON(raw_refcnt_nr_counter(&sk->refcnt) <= 0);
		return &sk->epoll_ctx;
	} else {
		return NULL;
	}
}

static inline ssize_t ux_copy_data_from_netd_msghdr(struct netd_msghdr *msg, char *data, size_t size)
{
	if (msg->is_user_iov) {
#if defined(CONFIG_NETFS_FUSION) && defined(__aarch64__)
		return copy_data_from_iter_customize(&msg->msg_iter, data, size, hm_copy_from_user);
#else
		if (is_fusion_ops_enable()) {
			return copy_data_from_iter_customize(&msg->msg_iter, data, size, fusion_copy_from_user);
		}
		return copy_data_from_caller_iter(&msg->msg_iter, data, size);
#endif
	} else {
		return copy_data_from_iter(&msg->msg_iter, data, size);
	}
}

static inline ssize_t ux_copy_data_to_netd_msghdr(struct netd_msghdr *msg, char *data, size_t size)
{
	if (msg->is_user_iov) {
#if defined(CONFIG_NETFS_FUSION) && defined(__aarch64__)
		return copy_data_to_iter_customize(&msg->msg_iter, data, size, hm_copy_to_user);
#else
		if (is_fusion_ops_enable()) {
			return copy_data_to_iter_customize(&msg->msg_iter, data, size, fusion_copy_to_user);
		}
		return copy_data_to_caller_iter(&msg->msg_iter, data, size);
#endif
	} else {
		return copy_data_to_iter(&msg->msg_iter, data, size);
	}
}

static inline void ux_sock_set_socket(ux_sock_t *sk, ux_socket_t *sock)
{
	BUG_ON(sk == NULL);
	sk->socket = sock;
}

/*
 * Currently, sk->socket is only accessed when netstat, and there is a list lock
 * to ensure the validity of the obtained socket.
 * In other scenarios, ensure the validity of the socket.
 */
static inline ux_socket_t *ux_sock_get_socket(ux_sock_t *sk)
{
	BUG_ON(sk == NULL);
	return sk->socket;
}

typedef struct ux_proto_ops {
	int (*close)(ux_socket_t *sock);
	int (*shutdown)(ux_socket_t *sock, int how);

	int (*socketpair)(ux_socket_t *sock, ux_socket_t *sock2);

	int (*connect)(ux_socket_t *sock, const struct sockaddr *addr,
		       socklen_t addrlen);
	int (*bind)(ux_socket_t *sock, const struct sockaddr *addr, socklen_t addrlen);
	int (*listen)(ux_socket_t *sock, int backlog);
	int (*accept)(ux_socket_t *sock, ux_socket_t *nsock,
		      struct sockaddr *addr, socklen_t *addrlen, int flags);

	int (*fcntl)(ux_socket_t *sock, int cmd, int arg);
	int (*ioctl)(ux_socket_t *sock, int cmd, uintptr_t arg);

	ssize_t (*send)(ux_socket_t *sock, const void *buf, size_t len, int flags);
	ssize_t (*recv)(ux_socket_t *sock, void *buf, size_t len, int flags);

	int (*sendmmsg)(ux_socket_t *sock, const void *msgvec, size_t vlen, int flags);
	int (*recvmmsg)(ux_socket_t *sock, const void *msgvec, size_t vlen, int flags, void *timeout);

	ssize_t (*sendmsg)(ux_socket_t *sock, const struct msghdr *msg, int flags);
	ssize_t (*recvmsg)(ux_socket_t *sock, struct msghdr *msg, int flags);

	ssize_t (*sendto)(ux_socket_t *sock, const void *buf, size_t len, int flags,
			  const struct sockaddr *daddr, socklen_t addrlen);
	ssize_t (*recvfrom)(ux_socket_t *sock, void *buf, size_t len, int flags,
			    struct sockaddr *saddr, socklen_t *addrlen);

	ssize_t (*writev)(ux_socket_t *sock, const struct iovec *iov, int iovcnt, int flags);
	ssize_t (*readv)(ux_socket_t *sock, const struct iovec *iov, int iovcnt);

	int (*getsockopt)(ux_socket_t *sock, int level, int optname,
			  void *optval, socklen_t *optlen);
	int (*setsockopt)(ux_socket_t *sock, int level, int optname,
			  const void *optval, socklen_t optlen);

	int (*getsockname)(ux_socket_t *sock, struct sockaddr *addr,
			   socklen_t *addrlen);
	int (*getpeername)(ux_socket_t *sock, struct sockaddr *addr,
			   socklen_t *addrlen);
	int (*check_perm)(ux_socket_t *sock, ux_socket_t *peer_sock);
	int (*fchown)(ux_socket_t *sock, unsigned int uid, unsigned int gid);
	int (*fstat)(ux_socket_t *sock, uid_t *uid);
	int (*mmap)(ux_socket_t *sock, const struct net_mmap_info *info, uintptr_t *rvaddr);
#ifdef CONFIG_LSYSCALL_ENABLED
	int (*epoll_recheck_revents)(ux_socket_t *sock, uint32_t events, uint32_t *revents);
	void (*sync_ctrlmem_polllist)(ux_socket_t *sock);
#endif
	void (*fd_config)(ux_socket_t *sock, struct sysarg_fdtable_fd_install *config);
} ux_proto_ops_t;

typedef struct ux_priv_ops {
	int (*getpeerinfo)(ux_socket_t *sock, int *peer_pid, unsigned long long *peer_cookie);
} ux_priv_ops_t;

int ux_sockmgr_init(void);

int ux_sock_owner_foreach(int (*iter)(ux_sock_owner_t *owner, void *arg), void *arg);
ux_sock_owner_t *ux_sock_owner_current(void);
ux_sock_owner_t *ux_sock_owner_get(uint32_t cnode, int create);
void ux_sock_owner_put(ux_sock_owner_t *owner);
ux_sock_owner_t *ux_sock_owner_get_fast(uint32_t cnode, int create);
void ux_sock_owner_put_fast(ux_sock_owner_t *owner);

void ux_sock_owner_detach(ux_sock_owner_t *owner);

int ux_sock_owner_fork(ux_sock_owner_t *src_owner, uint32_t dst_cnode_idx);
int ux_sock_owner_resize_sock_table(ux_sock_owner_t *owner, unsigned int new_count);

int ux_socket_insert(ux_sock_owner_t *owner, ux_socket_t *sock, int fd);

ux_socket_t *ux_socket_alloc(void);
void ux_socket_free(ux_socket_t *sock);

int ux_socket_attach(ux_sock_owner_t *owner, ux_socket_t *sock, int fd);
int ux_socket_detach(ux_sock_owner_t *owner, ux_socket_t *sock, int fd);
int ux_socket_detach_fd(ux_sock_owner_t *owner, ux_socket_t *sock, int fd, bool balance);
int ux_socket_get_timestamp(ux_socket_t *sock, struct timeval *arg);
int common_sendmmsg(ux_socket_t *sock, const void *msgvec, size_t vlen, int flags);
int common_recvmmsg(ux_socket_t *sock, const void *msgvec, size_t vlen, int flags, void *timeout);

static inline void ux_clear_sock_flags(ux_sock_t *sk, enum ux_sock_flags flag)
{
	sk->flags &= ~(u32_t)((u32_t)1 << (unsigned int)flag);
}

static inline void ux_set_sock_flags(ux_sock_t *sk, enum ux_sock_flags flag)
{
	sk->flags |= (u32_t)((u32_t)1 << (unsigned int)flag);
}

static inline u32_t ux_check_sock_flags(const ux_sock_t *sk, enum ux_sock_flags flag)
{
	return sk->flags & (u32_t)((u32_t)1 << (unsigned int)flag);
}

static inline bool sk_is_noblock(const ux_sock_t *sk)
{
	if ((sk->noblock & SOCK_NONBLOCK) == 0U) {
		return false;
	}

	return true;
}

#ifdef CONFIG_NET_NS
static inline ux_namespace_t *ux_sock_ns(const ux_sock_t *sock)
{
	return sock->ns;
}

static inline void ux_sock_ns_atttach(ux_sock_t *sock, ux_namespace_t *ns)
{
	if (ns != NULL) {
		ux_namespace_get(ns);
	}

	sock->ns = ns;

	return;
}

static inline void ux_sock_ns_detach(ux_sock_t *sock)
{
	if (sock->ns != NULL) {
		ux_namespace_put(sock->ns);
		sock->ns = NULL;
	}

	return;
}
void ux_namespace_fork(ux_sock_owner_t *src_owner, ux_sock_owner_t *dst_owner);
void ux_namespace_revoke(ux_sock_owner_t *owner);
#else
static inline ux_namespace_t *ux_sock_ns(const ux_sock_t *sock)
{
	UNUSED(sock);
	return NULL;
}

static inline void ux_sock_ns_atttach(ux_sock_t *sock, ux_namespace_t *ns)
{
	UNUSED(sock, ns);
	return;
}

static inline void ux_sock_ns_detach(ux_sock_t *sock)
{
	UNUSED(sock);
	return;
}

static inline void ux_namespace_fork(ux_sock_owner_t *src_owner, ux_sock_owner_t *dst_owner)
{
	UNUSED(src_owner, dst_owner);
	return;
}

static inline void ux_namespace_revoke(ux_sock_owner_t *owner)
{
	UNUSED(owner);
	return;
}
#endif

ux_socket_t *ux_socket_get_slow(uint32_t cnode, int fd);
ux_socket_t *ux_socket_get(uint32_t cnode, int fd);
void ux_socket_put(ux_socket_t *sock);
void ux_socket_rpccnt_put(ux_socket_t *sock);

void ux_socket_lock(ux_socket_t *sock);
void ux_socket_unlock(ux_socket_t *sock);

ux_sock_t *ux_sock_alloc(ux_socket_t *sock, size_t size, int use_polllist);
void ux_sock_init(ux_sock_t *sk, ux_namespace_t *ns, int domain, int type, int protocol);

void ux_sock_get(ux_sock_t *sk);
void ux_sock_put(ux_sock_t *sk);

void ux_sock_lock(ux_sock_t *sk);
void ux_sock_unlock(ux_sock_t *sk);

void ux_sock_poll_common(ux_sock_t *sk, uint32_t event);
void ux_sock_recv_data(ux_sock_t *sk, ux_pktbuf_t *pkt);

/* XXX: use ERR_PTR() to eliminate the extra out parameter */
int socket_create(ux_socket_t **sockp, int domain, int type, int protocol, ux_namespace_t *ns);
void socket_release(ux_socket_t *sock);

int socket_close(const ux_socket_t *sock, uint32_t cnode, int fd);

int socket_socketpair(ux_socket_t *sock, ux_socket_t *sock2);

int socket_connect(ux_socket_t *sock, const struct sockaddr *addr, socklen_t addrlen);
int socket_bind(ux_socket_t *sock, const struct sockaddr *addr, socklen_t addrlen);
int socket_listen(ux_socket_t *sock, int backlog);
int socket_accept(ux_socket_t *sock, ux_socket_t *nsock,
		  struct sockaddr *addr, socklen_t *addrlen, int flags);
int socket_shutdown(ux_socket_t *sock, int how);

int socket_fcntl(ux_socket_t *sock, int cmd, unsigned long arg);
int socket_ioctl(ux_socket_t *sock, int cmd, uintptr_t args_u);

ssize_t socket_send(ux_socket_t *sock, const void *buf, size_t len, int flags);
ssize_t socket_send_nosec(ux_socket_t *sock, const void *buf, size_t len, int flags);
ssize_t socket_recv(ux_socket_t *sock, void *buf, size_t len, int flags);

int socket_sendmmsg(ux_socket_t *sock, const void *msgvec, size_t vlen, int flags);
int socket_recvmmsg(ux_socket_t *sock, const void *msgvec, size_t vlen, int flags, void *timeout);

ssize_t socket_sendmsg(ux_socket_t *sock, const struct msghdr *msg, int flags);
ssize_t socket_recvmsg(ux_socket_t *sock, struct msghdr *msg, int flags);

ssize_t socket_writev(ux_socket_t *sock, const struct iovec *iov, int iovcnt, int flags);
ssize_t socket_readv(ux_socket_t *sock, const struct iovec *iov, int iovcnt);

ssize_t socket_sendto(ux_socket_t *sock, const void *buf, size_t len, int flags,
		      const struct sockaddr *daddr, socklen_t addrlen);
ssize_t socket_recvfrom(ux_socket_t *sock, void *buf, size_t len, int flags,
			struct sockaddr *saddr, socklen_t *addrlen);
ssize_t socket_recvfrom_nosec(ux_socket_t *sock, void *buf, size_t len, int flags,
			      struct sockaddr *saddr, socklen_t *addrlen);

int socket_getsockopt(ux_socket_t *sock, int level, int optname,
		      void *optval, socklen_t *optlen);
int socket_setsockopt(ux_socket_t *sock, int level, int optname,
		      const void *optval, socklen_t optlen);
int socket_getsockname(ux_socket_t *sock, struct sockaddr *addr,
		       socklen_t *addrlen);
int socket_getpeername(ux_socket_t *sock, struct sockaddr *addr,
		       socklen_t *addrlen);
int socket_getrecvto(ux_socket_t *sock);

int socket_fchown(ux_socket_t *sock, unsigned int uid, unsigned int gid);

int socket_fstat(ux_socket_t *sock, struct stat *buf);

int socket_mmap(ux_socket_t *sock, const struct net_mmap_info *info, uintptr_t *rvaddr);

void socket_clear_ssocks(ux_sock_owner_t *owner);
int socket_check_permission(ux_socket_t *sock, ux_socket_t *peer_sock);

#ifdef CONFIG_UXNET_UNIX
int get_netstat_unix(char **pstring, int *len, int type);
void release_netstat_unix(char *msg, int type);
#else
static inline int get_netstat_unix(char **pstring, int *len, int type)
{
	UNUSED(pstring, len, type);
	return 0;
}

static inline void release_netstat_unix(char *msg, int type)
{
	UNUSED(msg, type);
}
#endif

void do_put_send_fd(int fs_type, ux_socket_t *src_sock, int vfs_fd,
		    uint32_t send_cidx, const char *fssrv_name);

DECLARE_UPCALL_ACTVHDLR(handle_close_on_exec);

#ifdef CONFIG_VIRTIO_VSOCK
int ux_vsock_stream_stat(char *buf, size_t count);
#else
static inline int ux_vsock_stream_stat(const char *buf, size_t count)
{
	UNUSED(buf);
	UNUSED(count);
	return 0;
}
#endif

typedef ssize_t (*socket_send_handler)(ux_socket_t *sock,
				       const void *buf, size_t len, int flags);
ssize_t socket_send_wrapper(ux_socket_t *sock,
			    const void *buf, size_t len, int flags,
			    socket_send_handler handler);

typedef ssize_t (*socket_sendmsg_handler)(ux_socket_t *sock,
					  const struct msghdr *msg, int flags);
ssize_t socket_sendmsg_wrapper(ux_socket_t *sock,
			       const struct msghdr *msg, int flags,
			       socket_sendmsg_handler handler);

typedef ssize_t (*socket_recvmsg_handler)(ux_socket_t *sock,
					  struct msghdr *msg, int flags);
ssize_t socket_recvmsg_wrapper(ux_socket_t *sock,
			       struct msghdr *msg, int flags,
			       socket_recvmsg_handler handler);

ssize_t socket_readv_wrapper(ux_socket_t *sock,
			      const struct iovec *iov, int iovcnt,
			      socket_recvmsg_handler handler);
ssize_t socket_writev_wrapper(ux_socket_t *sock,
			      const struct iovec *iov, int iovcnt,
			      socket_sendmsg_handler handler);

typedef ssize_t (*socket_sendto_handler)(ux_socket_t *sock,
					 const void *buf, size_t len, int flags,
					 const struct sockaddr *daddr, socklen_t addrlen);
ssize_t socket_sendto_wrapper(ux_socket_t *sock,
			      const void *buf, size_t len, int flags,
			      const struct sockaddr *daddr, socklen_t addrlen,
			      socket_sendto_handler handler);

typedef ssize_t (*socket_recvfrom_handler)(ux_socket_t *sock,
					  void *buf, size_t len, int flags,
					  struct sockaddr *saddr, socklen_t *addrlen);
ssize_t socket_recvfrom_wrapper(ux_socket_t *sock,
				void *buf, size_t len, int flags,
				struct sockaddr *saddr, socklen_t *addrlen,
				socket_recvfrom_handler handler);

typedef int (*socket_connect_handler)(ux_socket_t *sock, const struct sockaddr *addr, socklen_t addrlen);
int socket_connect_wrapper(ux_socket_t *sock, const struct sockaddr *addr, socklen_t addrlen,
			   socket_connect_handler handler);

typedef int (*socket_bind_handler)(ux_socket_t *sock, const struct sockaddr *addr, socklen_t addrlen);
int socket_bind_wrapper(ux_socket_t *sock, const struct sockaddr *addr, socklen_t addrlen,
			   socket_bind_handler handler);
DECLARE_UPCALL_ACTVHDLR(handle_rpccnt_put);
#endif
