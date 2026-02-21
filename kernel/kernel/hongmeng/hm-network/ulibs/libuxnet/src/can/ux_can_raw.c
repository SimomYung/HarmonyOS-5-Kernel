/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Description: interface for network can raw protocol
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 30 09:49:09 2019
 */

#include <libstrict/strict.h>
#include <hongmeng/errno.h>
#include <net/if_arp.h>

#include "ux_netlog.h"
#include "ux_netdev.h"
#include "ux_pktbuf.h"
#include "ux_eventpoll.h"

#include "hm_can.h"
#include "ux_can_proto.h"
#include "ux_can_netdev.h"
#include "ux_can_raw.h"

static int ux_can_raw_bind(ux_socket_t *sock, const struct sockaddr *addr,
			   socklen_t addrlen)
{
	struct sockaddr_can *caddr = (struct sockaddr_can *)addr;
	ux_can_raw_sock_t *rsk = can_raw_sock(sock);
	ux_netdev_t *dev = NULL;
	ux_can_netdev_t *candev = NULL;
	int ret = 0;

	if (rsk == NULL) {
		return E_HM_INVAL;
	}

	if (addrlen < sizeof(struct sockaddr_can)) {
		return E_HM_INVAL;
	}

	if (caddr->can_family != AF_CAN) {
		return E_HM_INVAL;
	}

	if (caddr->can_ifindex) {
		dev = ux_netdev_get_by_index(caddr->can_ifindex);
		if (dev == NULL || dev->type != ARPHRD_CAN) {
			ret = E_HM_NODEV;
			goto out;
		}
		ux_netdev_put(dev);
	}

	candev = ux_netdev_priv(dev);
	raw_mutex_lock(&candev->raw_sock_list_lock);
	dlist_insert(&candev->raw_sock_list, &rsk->raw_sock_list_node);
	raw_mutex_unlock(&candev->raw_sock_list_lock);
	rsk->can_ifindex = caddr->can_ifindex;
	rsk->can_bind = 1;
out:
	return ret;
}

static int is_can_raw_size_invalid(ux_can_raw_sock_t *rsk, size_t len)
{
	if (len == CAN_MTU) {
		return 0;
	} else if (len != CANFD_MTU) {
		return 1;
	}

	return !rsk->can_fdcap;
}

static int ux_can_raw_send_common(ux_can_raw_sock_t *rsk, ux_netdev_t *dev,
				  const void *buf, size_t len, int flags)
{
	ux_pktbuf_t *pkt = NULL;
	int ret;
	void *data = NULL;

	if (is_can_raw_size_invalid(rsk, len)) {
		return E_HM_INVAL;
	}

	pkt = ux_pktbuf_alloc(len);
	if (pkt == NULL) {
		return E_HM_NOMEM;
	}
	data = ux_pktbuf_push(pkt, len);
	if (data == NULL) {
		return E_HM_NOMEM;
	}

	ret = memcpy_s(data, pkt->len, buf, len);
	if (ret != 0) {
		return E_HM_POSIX_FAULT;
	}

	ret = ux_can_netdev_xmit(dev, pkt, 0);
	if (ret) {
		ux_pktbuf_free(pkt);
		return ret;
	}

	return 0;
}

static ssize_t can_raw_sendmsg_handler(ux_socket_t *sock, const struct msghdr *msg, int flags)
{
	ux_can_raw_sock_t *rsk = can_raw_sock(sock);
	struct sockaddr_can *caddr = NULL;
	struct iovec *iov = NULL;
	ux_netdev_t *dev = NULL;
	int ret;
	int ifidx;

	ifidx = rsk->can_ifindex;
	if (msg->msg_name) {
		if (msg->msg_namelen < sizeof(struct sockaddr_can)) {
			return E_HM_INVAL;
		}

		caddr = (struct sockaddr_can *)msg->msg_name;
		if (caddr->can_family != AF_CAN) {
			return E_HM_INVAL;
		}
		ifidx = caddr->can_ifindex;
	}

	if (msg->msg_iovlen != 1 || msg->msg_iov == NULL) {
		return E_HM_INVAL;
	}

	dev = ux_netdev_get_by_index(ifidx);
	if (dev == NULL) {
		return E_HM_NOMEM;
	}

	iov = msg->msg_iov;
	ret = ux_can_raw_send_common(rsk, dev, iov->iov_base, iov->iov_len,
				     flags);
	if (ret) {
		ux_netdev_put(dev);
		return (ssize_t)ret;
	}

	ux_netdev_put(dev);

	return iov->iov_len;
}

static ssize_t ux_can_raw_sendmsg(ux_socket_t *sock, const struct msghdr *msg, int flags)
{
	return socket_sendmsg_wrapper(sock, msg, flags, can_raw_sendmsg_handler);
}

static ssize_t ux_can_raw_recv(ux_socket_t *sock, void *buf, size_t len, int flags);
static ssize_t can_raw_recvmsg_handler(ux_socket_t *sock, struct msghdr *msg, int flags)
{
	size_t tot_iov_len = 0;
	ssize_t buflen = 0;
	ssize_t ret;
	int i;

	if (!is_recv_flag_valid((unsigned int)flags)) {
		return E_HM_INVAL;
	}

	for (i = 0; i < msg->msg_iovlen; i++) {
		if ((msg->msg_iov[i].iov_base == NULL) || ((ssize_t)msg->msg_iov[i].iov_len <= 0) ||
		    ((size_t)(ssize_t)msg->msg_iov[i].iov_len != msg->msg_iov[i].iov_len) ||
		    ((ssize_t)(buflen + (ssize_t)msg->msg_iov[i].iov_len) <= 0)) {
			return -1;
		}

		tot_iov_len += msg->msg_iov[i].iov_len;
		ret = (ssize_t)ux_can_raw_recv(sock, msg->msg_iov[i].iov_base, msg->msg_iov[i].iov_len, flags);
		if (ret < 0) {
			return ret;
		}
		buflen += (ssize_t)ret;
	}

	if ((size_t)buflen > tot_iov_len) {
		msg->msg_flags = (int)((unsigned int)msg->msg_flags | MSG_TRUNC);
	}
	return buflen;
}

static ssize_t ux_can_raw_recvmsg(ux_socket_t *sock, struct msghdr *msg, int flags)
{
	return socket_recvmsg_wrapper(sock, msg, flags, can_raw_recvmsg_handler);
}

static ssize_t can_raw_send_handler(ux_socket_t *sock, const void *buf, size_t len, int flags)
{
	ux_can_raw_sock_t *rsk = can_raw_sock(sock);
	ux_netdev_t *dev = NULL;
	int ret;

	if (rsk == NULL || rsk->can_ifindex <= 0) {
		return E_HM_INVAL;
	}

	dev = ux_netdev_get_by_index(rsk->can_ifindex);
	if (dev == NULL) {
		return E_HM_NOMEM;
	}

	ret = ux_can_raw_send_common(rsk, dev, buf, len, 0);
	if (ret) {
		ux_netdev_put(dev);
		return (ssize_t)ret;
	}

	ux_netdev_put(dev);

	return len;
}

static ssize_t ux_can_raw_send(ux_socket_t *sock, const void *buf, size_t len, int flags)
{
	return socket_send_wrapper(sock, buf, len, flags, can_raw_send_handler);
}

static ssize_t ux_can_raw_recv(ux_socket_t *sock, void *buf, size_t len,
			       int flags)
{
	ux_can_raw_sock_t *rsk = can_raw_sock(sock);
	ux_buf_t *ux_buf = NULL;

	if (flags != 0) {
		return E_HM_OPNOTSUPP;
	}

	if (rsk == NULL || rsk->can_ifindex <= 0) {
		return E_HM_INVAL;
	}

	do {
		ux_buf_queue_lock(&rsk->recv_queue);
		ux_buf = ux_buf_queue_dqueue_first(&rsk->recv_queue);
		if (ux_buf == NULL) {
			if (raw_sem_init(&rsk->recvsem, 0) != 0) {
				ux_buf_queue_unlock(&rsk->recv_queue);
				return E_HM_INVAL;
			}

			if (flags & MSG_DONTWAIT) {
				ux_buf_queue_unlock(&rsk->recv_queue);
				return E_HM_AGAIN;
			}

			ux_buf_queue_unlock(&rsk->recv_queue);
			if (raw_sem_wait(&rsk->recvsem) == E_HM_INTR) {
				hm_info("raw_sem_wait interruppted\n");
				return E_HM_RESTARTSYS;
			}
		}
	} while (ux_buf == NULL);

	if (ux_buf_queue_len(&rsk->recv_queue) == 0) {
		ux_sock_poll_common(&rsk->sk, EPOLLIN << CLEAN_EVENT_SHIFT);
	}

	ux_buf_queue_unlock(&rsk->recv_queue);
	len =  len > ux_buf->data_len ? ux_buf->data_len : len;
	NOFAIL(memcpy_s(buf, len, ux_buf->data, len));

	return (ssize_t)len;
}

static ssize_t can_raw_sendto_handler(ux_socket_t *sock, const void *buf, size_t len,
				 int flags, const struct sockaddr *daddr,
				 socklen_t addrlen)
{
	ux_can_raw_sock_t *rsk = can_raw_sock(sock);
	struct sockaddr_can *caddr = NULL;
	ux_netdev_t *dev = NULL;
	int ret;

	if (addrlen < sizeof(struct sockaddr_can)) {
		return E_HM_INVAL;
	}

	caddr = (struct sockaddr_can *)daddr;
	if (caddr->can_family != AF_CAN) {
		return E_HM_INVAL;
	}

	dev = ux_netdev_get_by_index(caddr->can_ifindex);
	if (dev == NULL) {
		return E_HM_NOMEM;
	}

	ret = ux_can_raw_send_common(rsk, dev, buf, len, flags);
	if (ret) {
		ux_netdev_put(dev);
		return (ssize_t)ret;
	}

	ux_netdev_put(dev);

	return len;
}

static ssize_t ux_can_raw_sendto(ux_socket_t *sock, const void *buf, size_t len,
				 int flags, const struct sockaddr *daddr,
				 socklen_t addrlen)
{
	return socket_sendto_wrapper(sock, buf, len, flags, daddr, addrlen,
				     can_raw_sendto_handler);
}

static ssize_t ux_can_raw_recvfrom(ux_socket_t *sock, void *buf, size_t len,
				   int flags, struct sockaddr *saddr,
				   socklen_t *addrlen)
{
	return E_HM_OPNOTSUPP;
}

static ssize_t ux_can_raw_readv(ux_socket_t *sock, const struct iovec *iov, int iovcnt)
{
	return socket_readv_wrapper(sock, iov, iovcnt, can_raw_recvmsg_handler);
}
static ssize_t ux_can_raw_writev(ux_socket_t *sock, const struct iovec *iov, int iovcnt, int flags)
{
	UNUSED(flags);
	return socket_writev_wrapper(sock, iov, iovcnt, can_raw_sendmsg_handler);
}

static int ux_can_raw_getsockopt(ux_socket_t *sock, int level, int optname,
				 void *optval, socklen_t *optlen)
{
	return E_HM_OPNOTSUPP;
}

static int ux_can_raw_setsockopt(ux_socket_t *sock, int level, int optname,
				 const void *optval, socklen_t optlen)
{
	return E_HM_OPNOTSUPP;
}

static int ux_can_raw_getsockname(ux_socket_t *sock, struct sockaddr *addr,
				  socklen_t *addrlen)
{
	struct sockaddr_can *caddr = (struct sockaddr_can *)addr;
	ux_can_raw_sock_t *rsk = can_raw_sock(sock);

	NOFAIL(memset_s(caddr, sizeof(struct sockaddr_can), 0,
		 sizeof(struct sockaddr_can)));
	caddr->can_family = AF_CAN;
	caddr->can_ifindex = rsk->can_ifindex;
	*addrlen = sizeof(struct sockaddr_can);

	return 0;
}

static int ux_can_raw_close(ux_socket_t *sock)
{
	ux_sock_t *sk = ux_socket_get_sock(sock);

	ux_sock_put(sk);
	return 0;
}

static int ux_can_raw_ioctl(ux_socket_t *sock, int cmd, uintptr_t args_u)
{
	return 0;
}

static int ux_can_raw_getsockevent(ux_socket_t *sock,
				   unsigned int *rcvev, unsigned int *sendev,
				   unsigned int *errev, int *error)
{
	ux_can_raw_sock_t *rsk = can_raw_sock(sock);
	ux_sock_t *sk = &rsk->sk;

	ux_sock_get(sk);
	if (rcvev != NULL) {
		*rcvev = ux_buf_queue_len(&rsk->recv_queue) == 0 ? 0 : EPOLLIN;
	}

	if (errev != NULL) {
		*errev = sk->sk_err ? EPOLLERR : 0;
	}

	if (sendev != NULL) {
		*sendev = EPOLLOUT;
	}

	if (error != NULL) {
		*error = 0;
	}
	ux_sock_put(sk);

	return 0;
}

static ux_proto_ops_t can_raw_ops = {
	.close = ux_can_raw_close,
	.bind = ux_can_raw_bind,
	.send = ux_can_raw_send,
	.recv = ux_can_raw_recv,
	.sendmmsg = NULL,
	.recvmmsg = NULL,
	.sendmsg = ux_can_raw_sendmsg,
	.recvmsg = ux_can_raw_recvmsg,
	.sendto = ux_can_raw_sendto,
	.recvfrom = ux_can_raw_recvfrom,
	.readv = ux_can_raw_readv,
	.writev = ux_can_raw_writev,
	.getsockopt = ux_can_raw_getsockopt,
	.setsockopt = ux_can_raw_setsockopt,
	.getsockname = ux_can_raw_getsockname,
	.ioctl = ux_can_raw_ioctl,
	.getsockevent = ux_can_raw_getsockevent,
#ifdef CONFIG_LSYSCALL_ENABLED
	.poll = NULL,
	.remove_poll = NULL,
	.epoll_ctl = NULL,
	.epoll_remove = NULL,
	.epoll_recheck_revents = NULL,
#endif  /* !CONFIG_LSYSCALL_ENABLED */
	.fd_config = NULL,
	.mmap = NULL,
	.sync_ctrlmem_polllist = NULL,
};

static void ux_can_sock_free(ux_sock_t *sk)
{
	ux_can_raw_sock_t *rsk = container_of(sk, ux_can_raw_sock_t, sk);

	free(rsk);
	rsk = NULL;
}

static inline ux_can_raw_sock_t *can_sk(ux_sock_t *sk)
{
	return (ux_can_raw_sock_t *)(void *)sk;
}

static ux_can_raw_sock_t *ux_can_raw_socket_alloc(ux_socket_t *socket)
{
	ux_can_raw_sock_t *rsk = NULL;

	rsk = can_sk(ux_sock_alloc(socket, sizeof(ux_can_raw_sock_t)));
	if (rsk == NULL) {
		return NULL;
	}

	ux_sock_init(&rsk->sk, NULL);
	rsk->sk.release = ux_can_sock_free;
	rsk->can_loop = 1;
	dlist_init(&rsk->raw_sock_list_node);
	ux_buf_queue_init(&rsk->recv_queue);
	if (raw_sem_init(&rsk->recvsem, 0) != 0) {
		free(rsk);
		return NULL;
	}

	return rsk;
}

static int ux_can_raw_create(ux_socket_t *socket, int type, int protocol)
{
	ux_can_raw_sock_t *rsk = NULL;

	UNUSED(protocol);

	if (type != SOCK_RAW) {
		return E_HM_PROTOTYPE;
	}

	rsk = ux_can_raw_socket_alloc(socket);
	if (rsk == NULL) {
		return E_HM_NOMEM;
	}

	socket->state = SS_UNCONNECTED;

	socket->ops = &can_raw_ops;
	ux_socket_set_sock(socket, &rsk->sk);

	return 0;
}

static const ux_can_proto_t ux_can_proto_raw = {
	.type = SOCK_RAW,
	.proto = CAN_RAW,
	.create = ux_can_raw_create,
};

int ux_can_raw_socket_init(void)
{
	int ret;

	ret = ux_can_proto_register(&ux_can_proto_raw);
	if (ret) {
		net_error(HMNET_CAN, "register can raw proto failed\n");
		return ret;
	}

	return 0;
}

void ux_can_raw_socket_exit(void)
{
	ux_can_proto_unregister(&ux_can_proto_raw);
}
