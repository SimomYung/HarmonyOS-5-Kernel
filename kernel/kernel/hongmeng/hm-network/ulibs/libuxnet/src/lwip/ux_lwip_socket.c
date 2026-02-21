/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: interface for network lwip socket
 * Author: Huawei OS Kernel Lab
 * Create: Sun Apr 28 13:34:09 2019
 */

#include <sys/ioctl.h>
#include <net/if_arp.h>
#include <netinet/if_ether.h>

#include <lwip/opt.h>

#include <lwip/sockets_api.h>
#include <lwip/sys.h>
#include <lwip/timeouts.h>
#include <lwip/debug.h>
#include <lwip/stats.h>
#include <lwip/snmp.h>
#include <lwip/init.h>
#include <lwip/tcpip.h>
#include <lwip/netif.h>
#include <lwip/netifapi.h>
#include <lwip/api.h>
#include <lwip/err.h>

#include <lwip/tcp.h>
#include <lwip/udp.h>

/* lwIP netif includes */
#include <lwip/etharp.h>
#include <lwip/ethip6.h>
#include <netif/ethernet.h>

#include <libhmlog/hmlog.h>

#include "security/sec_hook.h"
#include "ux_compat.h"
#include "ux_domain.h"
#include "ux_sockopt.h"
#include "ux_eventpoll.h"
#include "ux_ioctl.h"
#include "ux_liblwip.h"

#ifdef CONFIG_UXNET_NETFILTER
#include "netfilter/ux_iptables.h"
#endif

void lwip_register_uxsock_epoll_hook(uintptr_t fun);

struct lwip_sockaddr {
	u8_t        sa_len;
	u8_t        sa_family; /* sa_family_t */
	char        sa_data[14];
};

static void sockaddr2lwipsockaddr(struct sockaddr *addr, socklen_t addr_len)
{
	u8_t family;
	struct lwip_sockaddr *lwipaddr = (struct lwip_sockaddr *)addr;

	/* delegate to lwip's validation */
	if (addr == NULL || (int)addr_len < (int)sizeof(struct lwip_sockaddr)) {
		return;
	}

	family = (u8_t)addr->sa_family;
	lwipaddr->sa_family = family;
	lwipaddr->sa_len = (u8_t)addr_len;
}

static void lwipsockaddr2sockaddr(
	struct lwip_sockaddr *lwipaddr)
{
	struct sockaddr *addr = NULL;
	unsigned short sa_family;

	if (lwipaddr == NULL) {
		return;
	}

	sa_family = (unsigned short)lwipaddr->sa_family;
	addr = (struct sockaddr *)lwipaddr;
	addr->sa_family = sa_family;
}

static int isavailaddr(const struct sockaddr *addr)
{
	struct netif *netif = NULL;
	in_addr_t inaddr = {0};

	inaddr = ((const struct sockaddr_in*)(const void*)(addr))->sin_addr.s_addr;

	if (inaddr == INADDR_ANY)
		return 1;

	LOCK_TCPIP_CORE();
	NETIF_FOREACH(netif) {
		if (inaddr == netif->ip_addr.u_addr.ip4.addr) {
			UNLOCK_TCPIP_CORE();
			return 1;
		}
	}
	UNLOCK_TCPIP_CORE();
	return 0;
}


static int lwip_inet_getsockevent(ux_sock_t *sk, unsigned int *rcvev, unsigned int *sendev, unsigned int *errev)
{
	ux_lwip_sock_t *lwip_sock = container_of(sk, ux_lwip_sock_t, sk);
	int ret;

	ret = lwip_getsockevent(lwip_sock->fd, rcvev, sendev, errev, NULL);
	return ret;
}

static void lwip_sock_release(struct ux_sock *sk)
{
	ux_lwip_sock_t *lwip_sock = container_of(sk, ux_lwip_sock_t, sk);

	free(lwip_sock);
}

static inline ux_lwip_sock_t *lwip_sk(ux_sock_t *sk)
{
	return (ux_lwip_sock_t *)(void *)sk;
}

static ux_lwip_sock_t *lwip_sock_alloc_and_init(ux_socket_t *socket, int domain, int type, int protocol)
{
	ux_lwip_sock_t *sock = lwip_sk(ux_sock_alloc(socket, sizeof(ux_lwip_sock_t), 0));

	if (sock != NULL) {
		ux_sock_init(&sock->sk, NULL, type, domain, protocol);

		sock->sk.release = lwip_sock_release;
		sock->sk.getsockevent = lwip_inet_getsockevent;

		sock->fd = -1;
	}

	return sock;
}

static ux_lwip_sock_t *lwip_sock_from_socket(const ux_socket_t *sock)
{
	ux_sock_t *sk = ux_socket_get_sock(sock);

	return container_of(sk, ux_lwip_sock_t, sk);
}

static int lwip_inet_close(ux_socket_t *sock)
{
	ux_lwip_sock_t *lwip_sock = lwip_sock_from_socket(sock);
	int ret;

	ret = lwip_close(lwip_sock->fd);
	if (ret == -1) {
		ret = posix2hmerrno(errno);
	}

	ux_sock_put(&lwip_sock->sk);

	return ret;
}

static int lwip_inet_connect_handler(ux_socket_t *sock, const struct sockaddr *addr, socklen_t addrlen)
{
	ux_lwip_sock_t *lwip_sock = lwip_sock_from_socket(sock);
	int ret;

	sockaddr2lwipsockaddr((struct sockaddr *)addr, addrlen);
	ret = lwip_connect(lwip_sock->fd, addr, addrlen);
	if (ret == -1) {
		return posix2hmerrno(errno);
	}

	return ret;
}

static int lwip_inet_connect(ux_socket_t *sock, const struct sockaddr *addr, socklen_t addrlen)
{
	return socket_connect_wrapper(sock, addr, addrlen, lwip_inet_connect_handler);
}

static int lwip_inet_bind_handler(ux_socket_t *sock, const struct sockaddr *addr,
			  socklen_t addrlen)
{
	ux_lwip_sock_t *lwip_sock = lwip_sock_from_socket(sock);
	int ret;

	if (addr == NULL || addrlen < sizeof(struct sockaddr_in)) {
		return E_HM_INVAL;
	}

	sockaddr2lwipsockaddr((struct sockaddr *)addr, addrlen);
	if (!isavailaddr(addr)) {
		return E_HM_ADDRNOTAVAIL;
	}

	ret = lwip_bind(lwip_sock->fd, addr, addrlen);
	if (ret == -1) {
		return posix2hmerrno(errno);
	}

	return ret;
}

static int lwip_inet_bind(ux_socket_t *sock, const struct sockaddr *addr, socklen_t addrlen)
{
	return socket_bind_wrapper(sock, addr, addrlen, lwip_inet_bind_handler);
}

static int lwip_inet_listen(ux_socket_t *sock, int backlog)
{
	ux_lwip_sock_t *lwip_sock = lwip_sock_from_socket(sock);
	int ret;

	ret = lwip_listen(lwip_sock->fd, backlog);
	if (ret == -1) {
		return posix2hmerrno(errno);
	}

	return ret;
}

static int lwip_inet_accept(ux_socket_t *sock, ux_socket_t *nsock,
			    struct sockaddr *addr, socklen_t *addrlen,
			    int flags)
{
	ux_lwip_sock_t *s_lwip_sock = lwip_sock_from_socket(sock);
	ux_lwip_sock_t *c_lwip_sock = lwip_sock_alloc_and_init(nsock, s_lwip_sock->sk.socket_domain, s_lwip_sock->sk.type,
									s_lwip_sock->sk.socket_protocol);
	int ret = E_HM_OK;
	int cfd = -1;

	if (c_lwip_sock == NULL) {
		ret = E_HM_NOMEM;
	} else {
		cfd = lwip_accept(s_lwip_sock->fd, addr, addrlen);
		if (cfd < 0) {
			if (errno == ERESTARTSYS) {
				ret = E_HM_RESTARTSYS;
			} else {
				ret = posix2hmerrno(errno);
			}
		}
	}

	if (ret == E_HM_OK) {
		ret = lwip_set_uxsock_info(get_socket(cfd), &c_lwip_sock->sk.epoll_ctx);
	}

	if (ret == E_HM_OK) {
		lwipsockaddr2sockaddr((struct lwip_sockaddr *)addr);

		c_lwip_sock->fd = cfd;
		nsock->ops = sock->ops;

		ux_socket_set_sock(nsock, &c_lwip_sock->sk);

		if (((unsigned int)flags & SOCK_NONBLOCK) != 0U) {
			(void)lwip_fcntl(cfd, F_SETFL, O_NONBLOCK);
		}
	} else {
		if (c_lwip_sock != NULL) {
			free(c_lwip_sock);
		}

		if (cfd >= 0) {
			(void)lwip_close(cfd);
		}
	}

	return ret;
}

static int lwip_inet_shutdown(ux_socket_t *sock, int how)
{
	ux_lwip_sock_t *lwip_sock = lwip_sock_from_socket(sock);
	int ret;

	ret = lwip_shutdown(lwip_sock->fd, how);
	if (ret == -1) {
		return posix2hmerrno(errno);
	}

	return ret;
}

static int lwip_arp_ioctl(int cmd, struct arpreq *req)
{
	int err = (int)ERR_OK;
	uint32_t ip = ((struct sockaddr_in *)&req->arp_pa)->sin_addr.s_addr;
	const ip4_addr_t *ret_ip;
	struct eth_addr *ret_eth;

	if (req->arp_pa.sa_family != AF_INET) {
		err = (int)ERR_NOSUPPORT;
		goto errout;
	}

	switch (cmd) {
	case SIOCDARP:
		LOCK_TCPIP_CORE();
		err = etharp_remove_static_entry((ip4_addr_t *)&ip);
		UNLOCK_TCPIP_CORE();
		break;
	case SIOCGARP:
		LOCK_TCPIP_CORE();
		err = (int)etharp_find_addr(NULL, (ip4_addr_t *)&ip, &ret_eth, &ret_ip);
		if (err == -1) {
			UNLOCK_TCPIP_CORE();
			err = (int)ERR_VAL;
			break;
		}
		UNLOCK_TCPIP_CORE();
		err = memcpy_s((void *)req->arp_ha.sa_data, ETH_ALEN, ret_eth, ETH_ALEN);
		if (err != 0) {
			err = (int)ERR_MEM;
		} else {
			err = (int)ERR_OK;
		}
		break;
	case SIOCSARP:
		LOCK_TCPIP_CORE();
		err = etharp_add_static_entry((ip4_addr_t *)&ip,
					      (struct eth_addr *)req->arp_ha.sa_data);
		UNLOCK_TCPIP_CORE();
		break;
	default:
		err = (int)ERR_NOSUPPORT;
		break;
	}

errout:
	if (err != (int)ERR_OK) {
		err = err_to_errno((int8_t)err);
		errno = err;
		return -1;
	}

	return 0;
}

static int lwip_inet_ioctl(ux_socket_t *sock, int cmd, uintptr_t args_u)
{
	ux_lwip_sock_t *lwip_sock = lwip_sock_from_socket(sock);
	void *arg_buf = NULL;
	int cmdlen = 0;
	int ret;

	cmdlen = ux_ioctl_cmdlen(cmd);
	if (cmdlen < 0) {
		return cmdlen;
	}

	arg_buf = calloc((size_t)(unsigned int)cmdlen + 1, 1);
	if (arg_buf == NULL) {
		return E_HM_NOMEM;
	}

	if (hm_actv_read_xact_vm(arg_buf, (void *)args_u,
				 (unsigned long)(unsigned int)cmdlen) != E_HM_OK) {
		ret = E_HM_POSIX_FAULT;
		goto out;
	}

	switch (cmd) {
	case SIOCDARP:
	case SIOCGARP:
	case SIOCSARP:
		ret = lwip_arp_ioctl(cmd, (struct arpreq *)arg_buf);
		break;
	default:
		ret = lwip_ioctl(lwip_sock->fd, cmd, arg_buf);
	}

	if (ret == -1) {
		ret = posix2hmerrno(errno);
		goto out;
	}

	if (hm_actv_write_xact_vm((void *)args_u, arg_buf,
				  (unsigned long)(unsigned int)cmdlen) != E_HM_OK) {
		ret = E_HM_POSIX_FAULT;
		goto out;
	}
out:
	free(arg_buf);
	return ret;
}

static int lwip_inet_fcntl(ux_socket_t *sock, int cmd, int arg)
{
	ux_lwip_sock_t *lwip_sock = lwip_sock_from_socket(sock);
	int ret;

	ret = lwip_fcntl(lwip_sock->fd, cmd, arg);
	if (ret == -1) {
		return posix2hmerrno(errno);
	}

	return ret;
}

static ssize_t lwip_send_handler(ux_socket_t *sock, const void *buf, size_t len, int flags)
{
	ux_lwip_sock_t *lwip_sock = lwip_sock_from_socket(sock);
	ssize_t ret;

	ret = lwip_send(lwip_sock->fd, buf, len, flags);
	if (ret == -1) {
		return posix2hmerrno(errno);
	}

	return ret;
}

static ssize_t lwip_inet_send(ux_socket_t *sock, const void *buf, size_t len, int flags)
{
	return socket_send_wrapper(sock, buf, len, flags, lwip_send_handler);
}

static ssize_t lwip_inet_recv(ux_socket_t *sock, void *buf, size_t len,
			      int flags)
{
	ux_lwip_sock_t *lwip_sock = lwip_sock_from_socket(sock);
	ssize_t ret;

	ret = lwip_recv(lwip_sock->fd, buf, len, flags);
	if (ret == -1) {
		if (errno == ERESTARTSYS) {
			return E_HM_RESTARTSYS;
		}
		return posix2hmerrno(errno);
	}

	return ret;
}

static ssize_t lwip_sendmsg_handler(ux_socket_t *sock, const struct msghdr *msg, int flags)
{
	ux_lwip_sock_t *lwip_sock = lwip_sock_from_socket(sock);
	ssize_t ret;

	if (msg->msg_name) {
		sockaddr2lwipsockaddr((struct sockaddr *)msg->msg_name, msg->msg_namelen);
	}

	ret = lwip_sendmsg(lwip_sock->fd, msg, flags);
	if (ret == -1) {
		return posix2hmerrno(errno);
	}

	return ret;
}

static ssize_t lwip_inet_sendmsg(ux_socket_t *sock, const struct msghdr *msg, int flags)
{
	return socket_sendmsg_wrapper(sock, msg, flags, lwip_sendmsg_handler);
}

static ssize_t lwip_recvmsg_handler(ux_socket_t *sock, struct msghdr *msg, int flags)
{
	ux_lwip_sock_t *lwip_sock = lwip_sock_from_socket(sock);
	ssize_t ret;

	if (!is_recv_flag_valid((unsigned int)flags)) {
		return E_HM_INVAL;
	}

	ret = lwip_recvmsg(lwip_sock->fd, msg, flags);
	if (ret == -1) {
		if (errno == ERESTARTSYS) {
			return E_HM_RESTARTSYS;
		}
		return posix2hmerrno(errno);
	}

	lwipsockaddr2sockaddr((struct lwip_sockaddr *)(msg->msg_name));

	return ret;
}

static ssize_t lwip_inet_recvmsg(ux_socket_t *sock, struct msghdr *msg, int flags)
{
	return socket_recvmsg_wrapper(sock, msg, flags, lwip_recvmsg_handler);
}

static ssize_t lwip_sendto_handler(ux_socket_t *sock, const void *buf, size_t len,
			   int flags, const struct sockaddr *daddr,
			   socklen_t addrlen)
{
	ux_lwip_sock_t *lwip_sock = lwip_sock_from_socket(sock);
	ssize_t ret;

	sockaddr2lwipsockaddr((struct sockaddr *)daddr, addrlen);
	ret = lwip_sendto(lwip_sock->fd, buf, len, flags, daddr, addrlen);
	if (ret == -1) {
		return posix2hmerrno(errno);
	}

	return ret;
}

static ssize_t lwip_inet_sendto(ux_socket_t *sock, const void *buf, size_t len,
			   int flags, const struct sockaddr *daddr,
			   socklen_t addrlen)
{
	return socket_sendto_wrapper(sock, buf, len, flags, daddr, addrlen,
				     lwip_sendto_handler);
}

static ssize_t lwip_recvfrom_handler(ux_socket_t *sock, void *buf, size_t len, int flags,
			     struct sockaddr *saddr, socklen_t *addrlen)
{
	ux_lwip_sock_t *lwip_sock = lwip_sock_from_socket(sock);
	ssize_t ret;

	ret = lwip_recvfrom(lwip_sock->fd, buf, len, flags, saddr, addrlen);
	if (ret == -1) {
		if (errno == ERESTARTSYS) {
			return E_HM_RESTARTSYS;
		}
		return posix2hmerrno(errno);
	}
	lwipsockaddr2sockaddr((struct lwip_sockaddr *)saddr);

	return ret;
}

static ssize_t lwip_inet_recvfrom(ux_socket_t *sock, void *buf, size_t len, int flags,
			     struct sockaddr *saddr, socklen_t *addrlen)
{
	return socket_recvfrom_wrapper(sock, buf, len, flags, saddr, addrlen,
				       lwip_recvfrom_handler);
}

static ssize_t lwip_inet_readv(ux_socket_t *sock, const struct iovec *iov, int iovcnt)
{
	return socket_readv_wrapper(sock, iov, iovcnt, lwip_recvmsg_handler);
}

static ssize_t lwip_inet_writev(ux_socket_t *sock, const struct iovec *iov, int iovcnt, int flags)
{
	UNUSED(flags);
	return socket_writev_wrapper(sock, iov, iovcnt, lwip_sendmsg_handler);
}

static int lwip_sockopt_get_optlen(const socklen_t *optlen, socklen_t *len)
{
	if (hm_actv_read_xact_vm(len, optlen, sizeof(socklen_t)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	if (*len > UX_MAX_SOCKOPT_LEN) {
		return E_HM_INVAL;
	}

	return 0;
}

static int lwip_inet_getsockopt_common(const ux_socket_t *sock, int level, int optname,
				       void *optval, socklen_t *optlen)
{
	ux_lwip_sock_t *lwip_sock = lwip_sock_from_socket(sock);
	int ret = 0;

	ret = lwip_getsockopt(lwip_sock->fd, level, optname, optval, optlen);
	if (ret == -1) {
		return posix2hmerrno(errno);
	}

	return ret;
}

static int lwip_inet_getsockopt_compat(const ux_socket_t *sock, int level, int optname,
				       void *optval, socklen_t *optlen)
{
	ux_lwip_sock_t *lwip_sock = lwip_sock_from_socket(sock);
	int optname_tmp = optname;
	int ret = 0;

	if (optname == SO_RCVTIMEO_NEW || optname == SO_SNDTIMEO_NEW) {
		optname_tmp = optname - (SO_RCVTIMEO_NEW - SO_RCVTIMEO_OLD);
		*optlen = sizeof(struct timeval);
	}

	ret = lwip_getsockopt(lwip_sock->fd, level, optname_tmp, optval, optlen);
	if (ret == -1) {
		return posix2hmerrno(errno);
	}

	if (optname == SO_RCVTIMEO_NEW || optname == SO_SNDTIMEO_NEW) {
		struct timeval tv = *(struct timeval *)optval;
		struct timeval32 *tv32 = optval;

		tv32->tv_sec = (int)tv.tv_sec;
		tv32->tv_usec = (int)tv.tv_usec;
		*optlen = sizeof(struct timeval32);
	}

	return 0;
}

static int lwip_inet_getsockopt(ux_socket_t *sock, int level, int optname,
				void *optval, socklen_t *optlen)
{
	char buf[UX_MAX_SOCKOPT_LEN] = {0};
	socklen_t len = 0;
	int ret = 0;

	if (optval == NULL) {
		return E_HM_POSIX_FAULT;
	}

#ifdef CONFIG_UXNET_NETFILTER
	if (level == SOL_IP) {
		if (optname == UX_IPT_SO_GET_INFO || optname == UX_IPT_SO_GET_ENTRIES ||
		    optname == UX_IPT_SO_GET_REVISION_MATCH || optname == UX_IPT_SO_GET_REVISION_TARGET) {
			ret = do_ipt_get_ctl(optname, optval, (int *)optlen);
			return ret;
		}
	}
#endif

	ret = lwip_sockopt_get_optlen(optlen, &len);
	if (ret != E_HM_OK) {
		return ret;
	}

	if (level == SOL_SOCKET && (optname == SO_MARK || optname == SO_COOKIE)) {
		return ux_common_getsockopt(sock, level, optname, optval, optlen);
	}

	if (ux_is_compat_client()) {
		ret = lwip_inet_getsockopt_compat(sock, level, optname, buf, &len);
	} else {
		ret = lwip_inet_getsockopt_common(sock, level, optname, buf, &len);
	}
	if (ret != E_HM_OK) {
		return ret;
	}

	if (hm_actv_write_xact_vm(optlen, &len, sizeof(socklen_t)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	if (hm_actv_write_xact_vm(optval, buf, len) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	return 0;
}

static int lwip_inet_setsockopt_compat(const ux_socket_t *sock, int level, int optname,
				       void *optval, socklen_t opt_len)
{
	ux_lwip_sock_t *lwip_sock = lwip_sock_from_socket(sock);
	int ret = 0;

	socklen_t optlen = opt_len;
	if (optname == SO_RCVTIMEO_NEW || optname == SO_SNDTIMEO_NEW) {
		struct timeval32 tv32 = *(struct timeval32 *)optval;
		struct timeval *tv = optval;

		tv->tv_sec = tv32.tv_sec;
		tv->tv_usec = tv32.tv_usec;
		optlen = sizeof(struct timeval);
		optname = optname - (SO_RCVTIMEO_NEW - SO_RCVTIMEO_OLD);
	}

	ret = lwip_setsockopt(lwip_sock->fd, level, optname, optval, optlen);
	if (ret == -1) {
		return posix2hmerrno(errno);
	}

	return ret;
}

static int lwip_inet_setsockopt_common(const ux_socket_t *sock, int level, int optname,
				const void *optval, socklen_t optlen)
{
	ux_lwip_sock_t *lwip_sock = lwip_sock_from_socket(sock);
	int ret = 0;

	ret = lwip_setsockopt(lwip_sock->fd, level, optname, optval, optlen);
	if (ret == -1) {
		return posix2hmerrno(errno);
	}

	return ret;
}

static int lwip_inet_setsockopt(ux_socket_t *sock, int level, int optname,
				const void *optval, socklen_t optlen)
{
	char buf[UX_MAX_SOCKOPT_LEN] = {0};
	int ret = 0;

#ifdef CONFIG_UXNET_NETFILTER
	if (level == SOL_IP) {
		if (optname == UX_IPT_SO_SET_REPLACE || optname == UX_IPT_SO_SET_ADD_COUNTERS) {
			ret = do_ipt_set_ctl(optname, (char *)optval, optlen);
			return ret;
		}
	}
#endif

	if (optlen > UX_MAX_SOCKOPT_LEN) {
		return E_HM_INVAL;
	}

	if (level == SOL_SOCKET && optname == SO_MARK) {
		return ux_common_setsockopt(sock, level, optname, optval, optlen);
	}

	if (hm_actv_read_xact_vm(buf, optval, optlen) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	if (ux_is_compat_client()) {
		ret = lwip_inet_setsockopt_compat(sock, level, optname, buf, optlen);
	} else {
		ret = lwip_inet_setsockopt_common(sock, level, optname, buf, optlen);
	}

	return ret;
}

static int lwip_inet_getsockname(ux_socket_t *sock, struct sockaddr *addr,
				 socklen_t *addrlen)
{
	ux_lwip_sock_t *lwip_sock = lwip_sock_from_socket(sock);
	int ret;

	ret = lwip_getsockname(lwip_sock->fd, addr, addrlen);
	if (ret == 0) {
		lwipsockaddr2sockaddr((struct lwip_sockaddr *)addr);
	} else {
		return posix2hmerrno(errno);
	}

	return ret;
}

static int lwip_inet_getpeername(ux_socket_t *sock, struct sockaddr *addr,
				 socklen_t *addrlen)
{
	ux_lwip_sock_t *lwip_sock = lwip_sock_from_socket(sock);
	int ret;

	ret = lwip_getpeername(lwip_sock->fd, addr, addrlen);
	if (ret == 0) {
		lwipsockaddr2sockaddr((struct lwip_sockaddr *)addr);
	} else {
		return posix2hmerrno(errno);
	}

	return ret;
}

static ux_proto_ops_t lwip_inet_ops = {
	.socketpair = NULL,
	.close = lwip_inet_close,
	.connect = lwip_inet_connect,
	.bind = lwip_inet_bind,
	.listen = lwip_inet_listen,
	.accept = lwip_inet_accept,
	.shutdown = lwip_inet_shutdown,
	.ioctl = lwip_inet_ioctl,
	.fcntl = lwip_inet_fcntl,
	.send = lwip_inet_send,
	.recv = lwip_inet_recv,
	.sendmmsg = common_sendmmsg,
	.recvmmsg = common_recvmmsg,
	.sendmsg = lwip_inet_sendmsg,
	.recvmsg = lwip_inet_recvmsg,
	.sendto = lwip_inet_sendto,
	.recvfrom = lwip_inet_recvfrom,
	.readv = lwip_inet_readv,
	.writev = lwip_inet_writev,
	.getsockopt = lwip_inet_getsockopt,
	.setsockopt = lwip_inet_setsockopt,
	.getsockname = lwip_inet_getsockname,
	.getpeername = lwip_inet_getpeername,
	/* .getrecvto = inet_getrecvto, */
	.check_perm = NULL,
#ifdef CONFIG_LSYSCALL_ENABLED
	.epoll_recheck_revents = NULL,
#endif  /* !CONFIG_LSYSCALL_ENABLED */
	.fd_config = NULL,
	.mmap = NULL,
	.sync_ctrlmem_polllist = NULL,
};

static void ux_epoll_wake_epoll_wait(const void *ctx, uint32_t event)
{
	ux_epoll_wakeup_clean((ux_epoll_context_t *)ctx, event);
}

static int lwip_inet_socket_create(ux_socket_t *sock, int domain, int type,
				   int protocol, ux_namespace_t *ns)
{
	UNUSED(ns);
	ux_lwip_sock_t *lwip_sock = lwip_sock_alloc_and_init(sock, domain, type, protocol);
	unsigned int flags = 0;
	int ret = E_HM_OK;
	int fd = -1;

	flags = (unsigned int)type & ~SOCK_TYPE_MASK;
	type = (int)((unsigned int)type & SOCK_TYPE_MASK);
	if (type == SOCK_RAW && sec_chk_div_raw_socket() != 0) {
		ret = E_HM_PERM;
	} else if (lwip_sock == NULL) {
		ret = E_HM_NOMEM;
	} else {
		fd = lwip_socket(domain, type, protocol);
		if (fd == -1) {
			ret = posix2hmerrno(errno);
		}
	}

	if (ret == E_HM_OK) {
		ret = lwip_set_uxsock_info(get_socket(fd), &lwip_sock->sk.epoll_ctx);
	}

	if (ret == E_HM_OK) {
		lwip_sock->fd = fd;
		sock->ops = &lwip_inet_ops;
		sock->priv_ops = NULL;

		ux_socket_set_sock(sock, &lwip_sock->sk);

		if ((flags & SOCK_NONBLOCK) != 0) {
			(void)socket_fcntl(sock, F_SETFL, O_NONBLOCK);
		}
	} else {
		if (fd >= 0) {
			(void)lwip_close(fd);
		}

		if (lwip_sock != NULL) {
			free(lwip_sock);
		}
	}

	return ret;
}

static ux_domain_t lwip_inet_domain = {
	.family = AF_INET,
	.create = lwip_inet_socket_create,
};

int lwip_socket_init(void)
{
	int ret;

	lwip_register_uxsock_epoll_hook((uintptr_t)ux_epoll_wake_epoll_wait);

	ret = ux_domain_register(&lwip_inet_domain);
	if (ret != E_HM_OK) {
		net_error(HMNET_INET, "lwip register inet failed\n");
		return ret;
	}

	return 0;
}
