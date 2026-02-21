/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Implementation of LDK socket layer
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 16 13:34:09 2022
 */

#include <sys/types.h>
#include <sys/socket.h>

#include <libhmsrv_io/dev.h>
#include <libhwsecurec/securec.h>

#include "osal.h"
#include "ucopy.h"
#include "ux_netlog.h"
#include "ux_domain.h"
#include "ux_sockopt.h"
#include "ux_namespace.h"
#include "security/sec_hook.h"

#include "ux_libldk.h"
#include "ux_libldk_pub.h"
#include <libhmsrv_io/net_socket.h>
#include <libhmsrv_sys/hm_thread.h>
#include <libhmsrv_sys/hm_ctrlmem_kobjctnr.h>

static void usk_free(ux_ldk_sock_t *usk)
{
	if (usk == NULL) {
		hm_error("usk is NULL when free\n");
		return;
	}

	free(usk);
	return;
}

static void ux_ldk_sock_free(ux_sock_t *sk)
{
	ux_ldk_sock_t *usk = ldk_sk(sk);

	usk_free(usk);
}

static int ux_ldk_sock_alloc_and_init(ux_ldk_sock_t **uskp, int domain, int type, int protocol)
{
	ux_ldk_sock_t *ulsk = NULL;

	ulsk = ldk_sk(ux_sock_alloc(NULL, sizeof(ux_ldk_sock_t), 0));
	if (ulsk == NULL) {
		return E_HM_NOMEM;
	}

	ux_sock_init(&(ulsk->sk), NULL, domain, type, protocol);
	ulsk->sk.release = ux_ldk_sock_free;
	*uskp = ulsk;

	return E_HM_OK;
}

static ux_ldk_sock_t *uxsock_ldksock(ux_socket_t *sock)
{
	ux_sock_t *sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return NULL;
	}

	return ldk_sk(sk);
}

#define RECV_QUEUE_SIZE (11 * PAGE_SIZE)
static void socket_recvqueue_free(ux_socket_t *sock)
{
	if (sock->recv_queue != NULL) {
		munmap(sock->recv_queue, RECV_QUEUE_SIZE);
		sock->recv_queue = NULL;
	}
}

static int ldk_socket_close(ux_socket_t *sock)
{
	ux_ldk_sock_t *ulsk = NULL;
	int ret;

	ulsk = uxsock_ldksock(sock);
	if (ulsk == NULL) {
		return E_HM_INVAL;
	}

	ret = libldk_close(&(ulsk->ctx));

	ux_sock_put(&ulsk->sk);
	socket_recvqueue_free(sock);
	return ret;
}

static int ldk_socket_connect(ux_socket_t *sock, const struct sockaddr *addr, socklen_t addrlen)
{
	int ret;
	ux_ldk_sock_t *ulsk = NULL;
	struct sockaddr_storage saddr = {0};

	ret = ucopy_addr_read(&saddr, addr, addrlen);
	if (ret != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	ret = sec_chk_div_socket_connect(sock, (const struct sockaddr *)&saddr, addrlen);
	if (ret != E_HM_OK) {
		return ret;
	}

	ulsk = uxsock_ldksock(sock);
	if (ulsk == NULL) {
		return E_HM_INVAL;
	}

	return libldk_connect(&(ulsk->ctx), addr, addrlen);
}

static int ldk_socket_bind(ux_socket_t *sock, const struct sockaddr *addr, socklen_t addrlen)
{
	int ret;
	ux_ldk_sock_t *ulsk = NULL;
	struct sockaddr_storage saddr = {0};

	ret = ucopy_addr_read(&saddr, addr, addrlen);
	if (ret != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	ret = sec_chk_div_socket_bind(sock, (const struct sockaddr *)&saddr, addrlen);
	if (ret != E_HM_OK) {
		return ret;
	}

	ulsk = uxsock_ldksock(sock);
	if (ulsk == NULL) {
		return E_HM_INVAL;
	}

	return libldk_bind(&(ulsk->ctx), addr, addrlen);
}

static int ldk_socket_listen(ux_socket_t *sock, int backlog)
{
	ux_ldk_sock_t *ulsk = NULL;

	ulsk = uxsock_ldksock(sock);
	if (ulsk == NULL) {
		return E_HM_INVAL;
	}

	return libldk_listen(&(ulsk->ctx), backlog);
}

static int ldk_socket_accept(ux_socket_t *sock, ux_socket_t *nsock,
			     struct sockaddr *addr, socklen_t *addrlen, int flags)
{
	int rv;
	unsigned long ino;
	ux_ldk_sock_t *new_ulsk = NULL;
	ux_ldk_sock_t *ulsk = NULL;

	ulsk = uxsock_ldksock(sock);
	if (ulsk == NULL) {
		return E_HM_INVAL;
	}

	rv = ux_ldk_sock_alloc_and_init(&new_ulsk, ulsk->sk.socket_domain, ulsk->sk.type, ulsk->sk.socket_protocol);
	if (rv != 0) {
		return E_HM_NOMEM;
	}

	ino = (unsigned long)ux_socket_ino(nsock);
	rv = libldk_accept4(&(ulsk->ctx), addr, addrlen, flags, ino, &(new_ulsk->ctx));
	if (rv < 0) {
		ux_sock_put(&new_ulsk->sk);
		return rv;
	}

	ux_socket_set_sock(nsock, &(new_ulsk->sk));

	nsock->ops = sock->ops;

	return E_HM_OK;
}

static int ldk_socket_shutdown(ux_socket_t *sock, int how)
{
	ux_ldk_sock_t *ulsk = NULL;

	ulsk = uxsock_ldksock(sock);
	if (ulsk == NULL) {
		return E_HM_INVAL;
	}

	return libldk_shutdown(&(ulsk->ctx), how);
}

static int ldk_socket_ioctl(ux_socket_t *sock, int cmd, uintptr_t args_u)
{
	ux_ldk_sock_t *ulsk = NULL;

	ulsk = uxsock_ldksock(sock);
	if (ulsk == NULL) {
		return E_HM_INVAL;
	}

	return libldk_ioctl(&(ulsk->ctx), cmd, args_u);
}

static int ldk_socket_fcntl(ux_socket_t *sock, int cmd, int arg)
{
	ux_ldk_sock_t *ulsk = NULL;

	ulsk = uxsock_ldksock(sock);
	if (ulsk == NULL) {
		return E_HM_INVAL;
	}

	return libldk_fcntl(&(ulsk->ctx), cmd, arg);
}

/*
* shared with recv_queue
* bidirectional data transmission is not supported
*/
static inline int do_net_socket_sendqueue_init(ux_socket_t *sock, ux_ldk_sock_t *ulsk, int flags)
{
	int send_flags = flags_clr_recvqueue(flags);

	if (sock->recv_queue != NULL && ulsk->recv_queue == NULL) {
		ulsk->recv_queue = sock->recv_queue;
		(void)libldk_send(&(ulsk->ctx), sock->recv_queue, 0, flags_set_recvqueue(flags));
	}

	return send_flags;
}

static ssize_t ldk_socket_send(ux_socket_t *sock, const void *buf, size_t len, int flags)
{
	ux_ldk_sock_t *ulsk = NULL;
	int send_flags;

	ulsk = uxsock_ldksock(sock);
	if (ulsk == NULL) {
		return E_HM_INVAL;
	}

	send_flags = do_net_socket_sendqueue_init(sock, ulsk, flags);
	return libldk_send(&(ulsk->ctx), buf, len, send_flags);
}

static ssize_t ldk_socket_recv(ux_socket_t *sock, void *buf, size_t len, int flags)
{
	ux_ldk_sock_t *ulsk = NULL;

	ulsk = uxsock_ldksock(sock);
	if (ulsk == NULL) {
		return E_HM_INVAL;
	}

	return libldk_recv(&(ulsk->ctx), buf, len, flags);
}

static int ldk_socket_sendmmsg(ux_socket_t *sock, const void *msgvec, size_t vlen, int flags)
{
	ux_ldk_sock_t *ulsk = NULL;

	ulsk = uxsock_ldksock(sock);
	if (ulsk == NULL) {
		return E_HM_INVAL;
	}

	return libldk_sendmmsg(&(ulsk->ctx), msgvec, vlen, flags);
}

static int ldk_socket_recvmmsg(ux_socket_t *sock, const void *msgvec, size_t vlen, int flags, void *timeout)
{
	ux_ldk_sock_t *ulsk = NULL;

	ulsk = uxsock_ldksock(sock);
	if (ulsk == NULL) {
		return E_HM_INVAL;
	}

	return libldk_recvmmsg(&(ulsk->ctx), msgvec, vlen, flags, timeout);
}

static ssize_t ldk_socket_sendmsg(ux_socket_t *sock, const struct msghdr *msg, int flags)
{
	ux_ldk_sock_t *ulsk = NULL;

	ulsk = uxsock_ldksock(sock);
	if (ulsk == NULL) {
		return E_HM_INVAL;
	}

	return libldk_sendmsg(&(ulsk->ctx), msg, flags);
}

#define do_net_socket_recvqueue(sock, ulsk, flags)	({ \
	do { \
		flags = flags_clr_recvqueue(flags); \
		if ((sock)->recv_queue != NULL && (ulsk)->recv_queue == NULL) { \
			(ulsk)->recv_queue = (sock)->recv_queue; \
			(void)libldk_recvmsg(&((ulsk)->ctx), (sock)->recv_queue, flags_set_recvqueue(flags)); \
		} \
	} while(0); \
})

static ssize_t ldk_socket_recvmsg(ux_socket_t *sock, struct msghdr *msg, int flags)
{
	ux_ldk_sock_t *ulsk = NULL;

	ulsk = uxsock_ldksock(sock);
	if (ulsk == NULL) {
		return E_HM_INVAL;
	}

	do_net_socket_recvqueue(sock, ulsk, flags);
	return libldk_recvmsg(&(ulsk->ctx), msg, flags);
}

static ssize_t ldk_socket_sendto(ux_socket_t *sock, const void *buf, size_t len, int flags,
				 const struct sockaddr *daddr, socklen_t addrlen)
{
	ux_ldk_sock_t *ulsk = NULL;

	ulsk = uxsock_ldksock(sock);
	if (ulsk == NULL) {
		return E_HM_INVAL;
	}

	return libldk_sendto(&(ulsk->ctx), buf, len, flags, daddr, addrlen);
}

static ssize_t ldk_socket_recvfrom(ux_socket_t *sock, void *buf, size_t len, int flags,
				   struct sockaddr *saddr, socklen_t *addrlen)
{
	ux_ldk_sock_t *ulsk = NULL;

	ulsk = uxsock_ldksock(sock);
	if (ulsk == NULL) {
		return E_HM_INVAL;
	}

	return libldk_recvfrom(&(ulsk->ctx), buf, len, flags, saddr, addrlen);
}

static ssize_t ldk_socket_readv(ux_socket_t *sock, const struct iovec *iov, int iovcnt)
{
	ux_ldk_sock_t *ulsk = NULL;

	ulsk = uxsock_ldksock(sock);
	if (ulsk == NULL) {
		return E_HM_INVAL;
	}

	return libldk_readv(&(ulsk->ctx), iov, iovcnt);
}

static ssize_t ldk_socket_writev(ux_socket_t *sock, const struct iovec *iov, int iovcnt, int flags)
{
	ux_ldk_sock_t *ulsk = NULL;

	ulsk = uxsock_ldksock(sock);
	if (ulsk == NULL) {
		return E_HM_INVAL;
	}

	return libldk_writev(&(ulsk->ctx), iov, iovcnt, flags);
}

static int ldk_socket_getsockopt(ux_socket_t *sock, int level, int optname, void *optval, socklen_t *optlen)
{
	ux_ldk_sock_t *ulsk = NULL;
	int ret;

	ulsk = uxsock_ldksock(sock);
	if (ulsk == NULL) {
		return E_HM_INVAL;
	}

	if (level == SOL_SOCKET && optname == SO_TYPE) {
		ret = ux_common_getsockopt(sock, level, optname, optval, optlen);
	} else {
		ret = libldk_getsockopt(&(ulsk->ctx), level, optname, optval, optlen);
	}

	return ret;
}

static int ldk_socket_setsockopt(ux_socket_t *sock, int level, int optname, const void *optval, socklen_t optlen)
{
	ux_ldk_sock_t *ulsk = NULL;

	ulsk = uxsock_ldksock(sock);
	if (ulsk == NULL) {
		return E_HM_INVAL;
	}

	return libldk_setsockopt(&(ulsk->ctx), level, optname, optval, optlen);
}

static int ldk_socket_fchown(ux_socket_t *sock, unsigned int uid, unsigned int gid)
{
	ux_ldk_sock_t *ulsk = NULL;

	ulsk = uxsock_ldksock(sock);
	if (ulsk == NULL) {
		return E_HM_INVAL;
	}

	return libldk_fchown(&(ulsk->ctx), uid, gid);
}

static int ldk_socket_fstat(ux_socket_t *sock, uid_t *uid)
{
	ux_ldk_sock_t *ulsk = NULL;

	ulsk = uxsock_ldksock(sock);
	if (ulsk == NULL) {
		return E_HM_INVAL;
	}

	return libldk_fstat(&(ulsk->ctx), uid);
}

static int ldk_socket_mmap(ux_socket_t *sock, const struct net_mmap_info *info, uintptr_t *rvaddr)
{
	ux_ldk_sock_t *ulsk = NULL;

	ulsk = uxsock_ldksock(sock);
	if (ulsk == NULL) {
		return E_HM_INVAL;
	}

	return libldk_mmap(&(ulsk->ctx), info, rvaddr);
}

static int ldk_socket_getsockname(ux_socket_t *sock, struct sockaddr *addr,
				 socklen_t *addrlen)
{
	ux_ldk_sock_t *ulsk = NULL;

	ulsk = uxsock_ldksock(sock);
	if (ulsk == NULL) {
		return E_HM_INVAL;
	}

	return libldk_getsockname(&(ulsk->ctx), addr, addrlen);
}

static int ldk_socket_getpeername(ux_socket_t *sock, struct sockaddr *addr,
				 socklen_t *addrlen)
{
	ux_ldk_sock_t *ulsk = NULL;

	ulsk = uxsock_ldksock(sock);
	if (ulsk == NULL) {
		return E_HM_INVAL;
	}

	return libldk_getpeername(&(ulsk->ctx), addr, addrlen);
}

#ifdef CONFIG_LSYSCALL_ENABLED
static int ldk_socket_epoll_recheck_revents(ux_socket_t *sock, uint32_t events, uint32_t *revents)
{
	int ret = 0;
	ux_ldk_sock_t *ulsk = NULL;

	ulsk = uxsock_ldksock(sock);
	if (ulsk == NULL) {
		return E_HM_INVAL;
	}

	ret = hmsrv_io_dev_net_epoll_recheck(&ulsk->ctx, events, revents);
	if (ret != E_HM_OK) {
		net_error(HMNET_INET, "failed to recheck epoll revents, ret=%d\n", ret);
	}

	return ret;
}
#endif /* !CONFIG_LSYSCALL_ENABLED */

static void ldk_socket_sync_ctrlmem_polllist(ux_socket_t *sock)
{
	int ret = 0;
	ux_ldk_sock_t *ulsk = NULL;

	ulsk = uxsock_ldksock(sock);
	if (ulsk == NULL) {
		net_error(HMNET_INET, "accquire ulsk failed\n");
		return;
	}

	if (ulsk->ctx.kshm_key <= 0) {
		net_error(HMNET_INET, "sync_ctrlmem_polllist no support\n");
	}

	ret = hmsrv_io_dev_net_sync_ctrlmem_polllist(&ulsk->ctx);
	if (ret != E_HM_OK) {
		net_error(HMNET_INET, "failed to sync ctrlmem polllist, ret=%d\n", ret);
	}

	return;
}

static void set_kobj_to_fdconfig(ux_socket_t *sock, struct sysarg_fdtable_fd_install *config)
{
	void *addr = NULL;
	ux_ldk_sock_t *ulsk = NULL;
	struct hmsrv_io_ctx *ctx = NULL;

	ulsk = uxsock_ldksock(sock);
	if (ulsk == NULL) {
		net_error(HMNET_INET, "accquire ulsk failed\n");
		return;
	}

	ctx = &ulsk->ctx;
	if (ctx->kshm_key <= 0) {
		net_error(HMNET_INET, "kshm_key is invalid\n");
		return;
	}

	addr = ptr_to_void(ptr_to_ulong(ctx->kshm_key) + ctx->kobj_offset);
	config->common_file = addr;
	config->flags |= FDTABLE_FD_POLLABLE;
}

#ifdef CONFIG_OFFLOAD_RECV_QUEUE
#define RECV_QUEUE_THREAD_NAME "iperf"
static void set_recv_queue_to_fdconfig(ux_socket_t *sock,
		struct sysarg_fdtable_fd_install *config)
{
	int err;
	void *recv_queue = NULL;
	char name[HMCAP_THREAD_NAME_SIZE];

	if (sock->sk->socket_domain != AF_INET ||
	    (sock->sk->type & SOCK_TYPE_MASK) != SOCK_DGRAM) {
		return;
	}

	if (sock->recv_queue != NULL) {
		return;
	}

	err = hm_thread_main_thread_name_of_cnode_idx(osal_hm_actv_src_cnode(), name, HMCAP_THREAD_NAME_SIZE);
	if (err == E_HM_OK && (strncmp(name, RECV_QUEUE_THREAD_NAME, strlen(RECV_QUEUE_THREAD_NAME)) == 0)) {
		recv_queue = mmap(NULL, RECV_QUEUE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (recv_queue != NULL) {
			err = mlock(recv_queue, RECV_QUEUE_SIZE);
			if (err == E_HM_OK) {
				NOFAIL(memset_s(recv_queue, RECV_QUEUE_SIZE, 0, RECV_QUEUE_SIZE));
				config->priv = sock->recv_queue = recv_queue;
				config->flags |= FDTABLE_FD_FLAG_RECVQUEUE;
			} else {
				munmap(recv_queue, RECV_QUEUE_SIZE);
			}
		}
	}
}
#endif

static void ldk_fd_set_config(ux_socket_t *sock,
		struct sysarg_fdtable_fd_install *config)
{
#if defined(CONFIG_FD_FLAG_INSTALL) && defined(CONFIG_NET_FUSION)
	config->flags |= FDTABLE_FD_FLAG_FAST_COPY;
#endif
	set_kobj_to_fdconfig(sock, config);
#ifdef CONFIG_OFFLOAD_RECV_QUEUE
	set_recv_queue_to_fdconfig(sock, config);
#endif
}

ux_proto_ops_t ldk_socket_ops = {
	.socketpair = NULL,
	.close = ldk_socket_close,
	.connect = ldk_socket_connect,
	.bind = ldk_socket_bind,
	.listen = ldk_socket_listen,
	.accept = ldk_socket_accept,
	.shutdown = ldk_socket_shutdown,
	.ioctl = ldk_socket_ioctl,
	.fcntl = ldk_socket_fcntl,
	.send = ldk_socket_send,
	.recv = ldk_socket_recv,
	.sendmmsg = ldk_socket_sendmmsg,
	.recvmmsg = ldk_socket_recvmmsg,
	.sendmsg = ldk_socket_sendmsg,
	.recvmsg = ldk_socket_recvmsg,
	.sendto = ldk_socket_sendto,
	.recvfrom = ldk_socket_recvfrom,
	.readv = ldk_socket_readv,
	.writev = ldk_socket_writev,
	.getsockopt = ldk_socket_getsockopt,
	.setsockopt = ldk_socket_setsockopt,
	.getsockname = ldk_socket_getsockname,
	.getpeername = ldk_socket_getpeername,
	.getsockevent = NULL,
	.check_perm = NULL,
	.fchown = ldk_socket_fchown,
	.fstat = ldk_socket_fstat,
#ifdef CONFIG_LSYSCALL_ENABLED
	.epoll_recheck_revents = ldk_socket_epoll_recheck_revents,
#endif /* !CONFIG_LSYSCALL_ENABLED */
	.fd_config = ldk_fd_set_config,
	.mmap = ldk_socket_mmap,
	.sync_ctrlmem_polllist = ldk_socket_sync_ctrlmem_polllist,
};

static int ldk_socket_create(ux_socket_t *sock, int domain, int type, int protocol, ux_namespace_t *ns)
{
	UNUSED(ns);
	int rv;
	unsigned long ino;
	ux_domain_t *uxdomain = NULL;
	ux_ldk_domain_t *uldomain = NULL;
	ux_ldk_sock_t *ulsk = NULL;

	/* XXX: Remove the duplicate domain searching */
	uxdomain = ux_domain_get(domain);
	if (uxdomain == NULL) {
		return E_HM_AFNOSUPPORT;
	}

	uldomain = ldk_domain(uxdomain);
	if (uldomain == NULL) {
		return E_HM_INVAL;
	}

	rv = ux_ldk_sock_alloc_and_init(&ulsk, domain, type, protocol);
	if (rv != 0) {
		return E_HM_NOMEM;
	}

	ino = (unsigned long)ux_socket_ino(sock);
	rv = libldk_socket(&(uldomain->ctx), type, protocol, ino, &(ulsk->ctx));
	if (rv < 0) {
		goto free_out;
	}

	ux_socket_set_sock(sock, &(ulsk->sk));

	sock->ops = &ldk_socket_ops;
	sock->priv_ops = NULL;

	return 0;

free_out:
	if (ulsk != NULL) {
		ux_sock_put(&ulsk->sk);
	}

	return rv;
}

static ux_ldk_domain_t ldk_domain_table[] = {
#ifdef CONFIG_LDK_QIPCRTR
	{
		.domain.family = AF_QIPCRTR,
		.domain.create = ldk_socket_create,
	},
#endif
#ifdef CONFIG_LDK_INET
	{
		.domain.family = AF_INET,
		.domain.create = ldk_socket_create,
	},
	{
		.domain.family = AF_PACKET,
		.domain.create = ldk_socket_create,
	},
	{
		.domain.family = AF_INET6,
		.domain.create = ldk_socket_create,
	},
	{
		.domain.family = AF_NETLINK,
		.domain.create = ldk_socket_create,
	},
	{
		.domain.family = AF_XDP,
		.domain.create = ldk_socket_create,
	},
	{
		.domain.family = AF_KEY,
		.domain.create = ldk_socket_create,
	},
	{
		.domain.family = AF_ALG,
		.domain.create = ldk_socket_create,
	},
#ifdef CONFIG_UXNET_PPPOX
	{
		.domain.family = AF_PPPOX,
		.domain.create = ldk_socket_create,
	},
#endif
#endif
};

int ux_ldk_domain_register(int family, unsigned long long ap_rref)
{
	int ret;
	unsigned int i;

	ret = E_HM_AFNOSUPPORT;
	for (i = 0; i < ARRAY_SIZE(ldk_domain_table); i++) {
		if (ldk_domain_table[i].domain.family != family) {
			continue;
		}

		ret = ux_domain_register(&ldk_domain_table[i].domain);
		if (ret != E_HM_OK) {
			net_error(HMNET_INET, "failed to register LDK domain = %d\n",
					ldk_domain_table[i].domain.family);
			return ret;
		}

		ret = libldk_domain_init(&ldk_domain_table[i].ctx, family, ap_rref);
		break;
	}

	if (ret != E_HM_OK) {
		net_warn(HMNET_INET, "failed to register LDK domain(%d), hm-net not support.\n", family);
	}

	return ret;
}

bool ux_ldk_check_ops(struct ux_proto_ops *ops)
{
	return ops == &ldk_socket_ops;
}

int ux_ldk_get_file_id(ux_socket_t *sock, unsigned long long *file_id)
{
	ux_ldk_sock_t *ulsk = NULL;

	ulsk = uxsock_ldksock(sock);
	if (ulsk == NULL) {
		return E_HM_INVAL;
	}

	(void)memcpy_s((void *)file_id, sizeof(unsigned long long),
		       (void *)&ulsk->ctx.file_id, sizeof(unsigned long long));
	return E_HM_OK;
}

#ifdef CONFIG_NET_NS
static int ldk_ns_create(ux_namespace_t *ns)
{
	int rc;

	if (ns->nsid == UX_ROOT_NS_ID) {
		return E_HM_OK;
	}

	rc = hmsrv_io_socket_alloc_new_ns(&ldk_domain_table[0].ctx, ns->nsid);
	if (rc != E_HM_OK) {
		net_warn(HMNET_INET, "failed to alloc new net namespace\n");
	}

	return rc;
}

static int ldk_ns_destroy(ux_namespace_t *ns)
{
	int rc;

	if (ns->nsid == UX_ROOT_NS_ID) {
		return E_HM_OK;
	}

	rc = hmsrv_io_socket_destroy_ns(&ldk_domain_table[0].ctx, ns->nsid);
	if (rc != E_HM_OK) {
		net_warn(HMNET_INET, "failed to destroy LDK namespace\n");
	}

	return rc;
}

static ux_namespace_ops_t ldk_namespace_ops = {
	.ns_create = ldk_ns_create,
	.ns_destroy = ldk_ns_destroy,
};

int libldk_namespace_init(void)
{
	ux_register_namespace_ops(&ldk_namespace_ops);
	return 0;
}
#endif

int ux_ldk_bpf(int cmd, void *attr, unsigned int size)
{
	ux_domain_t *uxdomain = NULL;
	ux_ldk_domain_t *uldomain = NULL;

	/* XXX: Remove the duplicate domain searching */
	uxdomain = ux_domain_get(AF_INET);
	if (uxdomain == NULL) {
		return E_HM_AFNOSUPPORT;
	}

	uldomain = ldk_domain(uxdomain);
	if (uldomain == NULL) {
		return E_HM_INVAL;
	}

	return libldk_bpf(&(uldomain->ctx), cmd, attr, size);
}

int ux_ldk_dev_ioctl(int cmd, uintptr_t args_u)
{
	ux_domain_t *uxdomain = NULL;
	ux_ldk_domain_t *uldomain = NULL;

	uxdomain = ux_domain_get(AF_INET);
	if (uxdomain == NULL) {
		return E_HM_AFNOSUPPORT;
	}

	uldomain = ldk_domain(uxdomain);
	if (uldomain == NULL) {
		return E_HM_INVAL;
	}

	return libldk_dev_ioctl(&(uldomain->ctx), cmd, args_u);
}
