/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: interface for network vsock socket
 * Author: Huawei OS Kernel Lab
 * Create: Mon June 17 19:35:43 2024
 */

#include <net/if_arp.h>
#include <libhmiov/iov_iter.h>
#include <libhmsrv_sys/hm_ctrlmem_kobjctnr.h>

#include "ux_vsock_host.h"
#include "osal.h"
#include "ucopy.h"
#include "ux_netlog.h"
#include "ux_vsock_socket.h"
#include "ux_vsock_trans.h"
#include "ux_vsock_core.h"
#include "ux_vsock_template.h"
#include "vsock/ux_vsock_common.h"
#include "vsock/ux_vsock_netdev.h"
#include "hmnet/hm_vsock.h"

struct vsock_host_netdev {
	struct vsock_netdev dev;
	ux_host_vsock_sock_t *host_vsock;
};

static struct vm_msg *dup_vm_msg_pkt(const struct vm_msg *src)
{
	struct vm_msg *new_data = (struct vm_msg *)malloc(sizeof(struct vm_msg) + src->len);
	if (new_data == NULL) {
		return NULL;
	}

	NOFAIL(memcpy_s(new_data, sizeof(struct vm_msg), src, sizeof(struct vm_msg)));
	if (src->len > 0) {
		new_data->data = (char *)(new_data + 1);
		NOFAIL(memcpy_s(new_data->data, src->len, src->data, src->len));
	}

	return new_data;
}

static int vsock_host_xmit(ux_netdev_t *dev, const void *data, int len, int flags)
{
	struct vsock_host_netdev *host_netdev = NULL;
	struct vm_msg *msg = NULL;
	struct vm_msg *new_msg = NULL;
	ux_host_vsock_sock_t *host_vsock = NULL;

	if (flags != 0) {
		return E_HM_INVAL;
	}

	if (dev == NULL || data == NULL) {
		return E_HM_INVAL;
	}

	host_netdev = ux_netdev_priv(dev);
	msg = (struct vm_msg *)data;
	if (host_netdev == NULL || len != (int)(sizeof(msg->hdr) + msg->hdr.len)) {
		return E_HM_INVAL;
	}
	host_vsock = host_netdev->host_vsock;

	new_msg = dup_vm_msg_pkt(msg);
	if (new_msg == NULL) {
		return E_HM_NOMEM;
	}

	lock_host_vsock(host_vsock);
	dlist_insert_tail(&host_vsock->sk.rcv_queue, &new_msg->node);
	ux_sock_poll_common(&host_vsock->sk, EPOLLIN);
	unlock_host_vsock(host_vsock);

	return E_HM_OK;
}

static ux_netdev_ops_t vsock_host_netdev_ops = {
	.ndo_init = NULL,
	.ndo_set_mtu = NULL,
	.ndo_ioctl = NULL,
	.ndo_xmit = vsock_host_xmit,
	.ndo_dump_stats = NULL,
	.ndo_get_master = NULL,
	.ndo_set_mac = NULL,
	.ndo_dellink = NULL,
	.ndo_add_slave = NULL,
	.ndo_del_slave = NULL,
};

static int vsock_register_host_netdev(ux_host_vsock_sock_t *vsock, unsigned int cid, uint32_t cnode_idx)
{
	ux_netdev_t *vsock_dev = NULL;
	struct vsock_host_netdev *host_netdev;
	int ret;

	if (!vsock_is_cid_valid(cid)) {
		return E_HM_INVAL;
	}

	ret = vsock_alloc_netdev(cid, &vsock_dev, sizeof(struct vsock_host_netdev));
	if (ret != E_HM_OK) {
		return ret;
	}

	host_netdev = ux_netdev_priv(vsock_dev);

	vsock_dev->ops = &vsock_host_netdev_ops;
	vsock_dev->type = ARPHRD_VSOCKMON;

	vsock->host_netdev = vsock_dev;
	vsock->cid = cid;

	ux_sock_get(&vsock->sk);
	host_netdev->host_vsock = vsock;
	host_netdev->dev.cid = cid;
	host_netdev->dev.uvmm_cnode_idx = cnode_idx;

	ret = ux_netdev_register(vsock_dev);
	if (ret != E_HM_OK) {
		ux_netdev_free(vsock_dev);
		vsock->host_netdev = NULL;
		ux_sock_put(&vsock->sk);
		return ret;
	}

	return E_HM_OK;
}

static int vsock_unregister_host_netdev(ux_host_vsock_sock_t *vsock, uint32_t cnode_idx)
{
	ux_netdev_t *vsock_dev = vsock->host_netdev;
	struct vsock_host_netdev *host_netdev;
	ux_host_vsock_sock_t *host_vsock;

	if (vsock_dev == NULL) {
		net_warn(HMNET_VSOCK, "can not find vsock_dev related to this vsock\n");
		return E_HM_INVAL;
	}

	if (vsock_dev->type != ARPHRD_VSOCKMON) {
		net_warn(HMNET_VSOCK, "vsock_dev->type = %u\n, which is not supported", vsock_dev->type);
		return E_HM_OPNOTSUPP;
	}

	host_netdev = ux_netdev_priv(vsock_dev);
	if (host_netdev->dev.uvmm_cnode_idx == cnode_idx) {
		host_vsock = host_netdev->host_vsock;

		ux_netdev_unregister(vsock_dev);
		host_netdev->host_vsock = NULL;
		ux_sock_put(&host_vsock->sk);
	}

	return E_HM_OK;
}

static int __do_host_vsock_register_vm_cid(ux_host_vsock_sock_t *vsock, const void *optval, socklen_t optlen)
{
	uint32_t cnode_idx = osal_hm_actv_src_cnode();
	int cid = 0;
	int rc = E_HM_OK;

	if (vsock->host_netdev != NULL) {
		return E_HM_EXIST;
	}

	if (optlen != sizeof(cid)) {
		return E_HM_INVAL;
	}

	rc = osal_copy_from_user(&cid, optval, optlen);
	if (rc != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	return vsock_register_host_netdev(vsock, cid, cnode_idx);
}

static int __do_host_vsock_unregister_vm_cid(ux_host_vsock_sock_t *vsock, const void *optval, socklen_t optlen)
{
	UNUSED(optval, optlen);
	uint32_t cnode_idx = osal_hm_actv_src_cnode();
	int rc;

	rc = vsock_unregister_host_netdev(vsock, cnode_idx);
	if (rc == E_HM_OK) {
		vsock->cid = 0;
		vsock->host_netdev = NULL;
	}

	return rc;
}

static int __do_host_vsock_setsockopt(ux_host_vsock_sock_t *vsock, int optname,
				      const void *optval, socklen_t optlen)
{
	int rc = E_HM_OPNOTSUPP;

	switch (optname) {
	case SO_VSOCK_REGISTER_NETDEV:
		rc = __do_host_vsock_register_vm_cid(vsock, optval, optlen);
		break;
	case SO_VSOCK_UNREGISTER_NETDEV:
		rc = __do_host_vsock_unregister_vm_cid(vsock, optval, optlen);
		break;
	default:
		net_warn(HMNET_VSOCK, "optname is %d, which is not supported\n", optname);
		break;
	}

	return rc;
}

static int host_vsock_setsockopt(ux_socket_t *sock, int level, int optname,
				 const void *optval, socklen_t optlen)
{
	int ret = E_HM_OK;
	ux_host_vsock_sock_t *vsock = host_vsock_from_socket(sock);
	if (vsock == NULL) {
		return E_HM_INVAL;
	}

	if (level != SOL_SOCKET) {
		return E_HM_OPNOTSUPP;
	}

	lock_host_vsock(vsock);
	ret = __do_host_vsock_setsockopt(vsock, optname, optval, optlen);
	unlock_host_vsock(vsock);

	return ret;
}

static int host_vsock_close(ux_socket_t *sock)
{
	ux_host_vsock_sock_t *vsock = host_vsock_from_socket(sock);
	if (vsock == NULL) {
		return E_HM_INVAL;
	}

	ux_clear_eventpoll_list(sock);
	(void)__do_host_vsock_unregister_vm_cid(vsock, NULL, 0);
	ux_sock_put(&vsock->sk);

	return E_HM_OK;
}

static inline bool chk_msg_netd_has_space(const struct netd_msghdr *msg_netd, const struct vm_msg *vm_msg)
{
	return (iov_iter_left(&msg_netd->msg_iter) >= sizeof(vm_msg->hdr) + vm_msg->len);
}

static int recv_single_node(ux_host_vsock_sock_t *vsock, struct netd_msghdr *msg_netd)
{
	struct dlist_node *node = dlist_pop_first(&vsock->sk.rcv_queue);
	struct vm_msg *vm_msg = (struct vm_msg *)node;
	int ret = E_HM_OK;
	ssize_t len;

	if (!chk_msg_netd_has_space(msg_netd, vm_msg)) {
		net_warn(HMNET_CORE, "iov_iter_left not enough, iov_iter_left = %d, len = %d\n",
				 (int)iov_iter_left(&msg_netd->msg_iter), (int)(sizeof(vm_msg->hdr) + vm_msg->len));
		dlist_insert_head(&vsock->sk.rcv_queue, node);
		return E_HM_INVAL;
	}

	/* copy hdr */
	ret = copy_data_to_caller_iter(&msg_netd->msg_iter, (char *)&vm_msg->hdr, sizeof(vm_msg->hdr));
	if (ret != (ssize_t)sizeof(vm_msg->hdr)) {
		net_warn(HMNET_CORE, "hdr copy fail, ret = %d, sizeof(vm_msg->hdr) = %d\n",
				(int)ret, (int)sizeof(vm_msg->hdr));
		dlist_insert_head(&vsock->sk.rcv_queue, node);
		return E_HM_FAULT;
	}

	/* copy data */
	len = copy_data_to_caller_iter(&msg_netd->msg_iter, vm_msg->data, vm_msg->len);
	if (len != (ssize_t)vm_msg->len) {
		net_warn(HMNET_CORE, "len = %d, vm_msg->len = %d\n", (int)len, (int)vm_msg->len);
		dlist_insert_head(&vsock->sk.rcv_queue, node);
		return E_HM_FAULT;
	}

	ret += len;
	free(node);

	return ret;
}

static ssize_t host_vsock_recvmsg(ux_socket_t *sock, struct msghdr *msg, int flags)
{
	ux_host_vsock_sock_t *vsock = host_vsock_from_socket(sock);
	ssize_t ret = E_HM_OK;
	struct sockaddr *uaddr;
	struct netd_msghdr msg_netd = {0};
	struct iovec iovstack[UIO_FASTIOV];
	struct iovec *iov = iovstack;

	if (flags != 0) {
		return E_HM_INVAL;
	}

	ret = ucopy_recvmsg_msghdr_read(&msg_netd, msg, &uaddr, &iov);
	if (ret != E_HM_OK) {
		net_warn(HMNET_CORE, "failed to copy msghdr, ret=%d\n", (int)ret);
		if (iov != iovstack) {
			free(iov);
		}
		return ret;
	}

	lock_host_vsock(vsock);

	if (!dlist_empty(&vsock->sk.rcv_queue)) {
		ret = recv_single_node(vsock, &msg_netd);
		if (ret > 0 && dlist_empty(&vsock->sk.rcv_queue)) {
			ux_sock_poll_common(&vsock->sk, EPOLLIN << CLEAN_EVENT_SHIFT);
		}
	} else {
		ret = E_HM_AGAIN;
	}

	unlock_host_vsock(vsock);

	if (iov != iovstack) {
		free(iov);
	}

	return ret;
}

static ssize_t host_vsock_sendmsg(ux_socket_t *sock, const struct msghdr *msg, int flags)
{
	ux_host_vsock_sock_t *vsock = host_vsock_from_socket(sock);
	ux_netdev_t *vsock_dev = vsock->host_netdev;
	struct vm_msg vm_msg = { 0 };
	int rc = E_HM_OK;
	struct netd_msghdr msg_netd = {0};
	struct iovec iovstack[UIO_FASTIOV];
	struct iovec *iov = iovstack;

	if (vsock_dev == NULL) {
		net_warn(HMNET_VSOCK, "vsock_dev is NULL\n");
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK && flags != 0) {
		rc = E_HM_OPNOTSUPP;
	}

	if (rc == E_HM_OK) {
		rc = ucopy_sendmsg_msghdr_read(&msg_netd, msg, &iov);
	}

	if (rc == E_HM_OK) {
		rc = vsock_copy_iov_to_vm_msg(iov, msg_netd.msg_iovlen,
						&vm_msg, copy_data_from_caller_iter);
	}

	if (rc == E_HM_OK && vm_msg.hdr.src_cid != vsock->cid) {
		net_warn(HMNET_VSOCK, "invalid src_cid = %u\n", (unsigned int)(vm_msg.hdr.src_cid));
		rc = E_HM_INVAL;
	}

	if (rc == E_HM_OK) {
		rc = ux_netdev_receive_zero_copy(vsock_dev, &vm_msg,
						  (int)(sizeof(vm_msg.hdr) + vm_msg.len), 0, UXDEV_PKT_STRUCT);
	}

	/**
	 * if msg.data is not null, it means processing error, so we must free the data here.
	 */
	if (vm_msg.data != NULL) {
		free(vm_msg.data);
	}

	if (iov != iovstack) {
		free(iov);
	}

	return rc == E_HM_OK ? (int)(sizeof(vm_msg.hdr) + vm_msg.hdr.len): rc;
}

static inline bool is_host_vsock_data_ready(const ux_host_vsock_sock_t *vsock)
{
	return dlist_empty(&vsock->sk.rcv_queue) == false;
}

static int host_vsock_getsockevent(ux_socket_t *sock, unsigned int *rcvev,
				   unsigned int *sendev, unsigned int *errev, int *error)
{
	ux_host_vsock_sock_t *vsock = host_vsock_from_socket(sock);
	if (vsock == NULL) {
		return E_HM_INVAL;
	}

	if (rcvev != NULL) {
		*rcvev = is_host_vsock_data_ready(vsock) ? EPOLLIN : 0;
	}

	if (sendev != NULL) {
		*sendev = EPOLLOUT;
	}

	if (errev != NULL) {
		*errev = 0;
	}

	if (error != NULL) {
		*error = 0;
	}

	return E_HM_OK;
}

static struct ux_proto_ops host_vsock_ops = {
	.connect = NULL,
	.bind = NULL,
	.listen = NULL,
	.accept = NULL,
	.shutdown = NULL,
	.close = host_vsock_close,

	.sendmmsg = NULL,
	.recvmmsg = NULL,
	.sendmsg = host_vsock_sendmsg,
	.recvmsg = host_vsock_recvmsg,

	.send = NULL,

	.sendto = NULL,
	.recvfrom = NULL,

	.getsockname = NULL,
	.getpeername = NULL,

	.fcntl = NULL,
	.ioctl = NULL,

	.getsockopt = NULL,
	.setsockopt = host_vsock_setsockopt,

	.getsockevent = host_vsock_getsockevent,
	.check_perm = NULL,

	.writev = NULL,
	.readv = NULL,

#ifdef CONFIG_LSYSCALL_ENABLED
	.epoll_recheck_revents = NULL,
#endif  /* !CONFIG_LSYSCALL_ENABLED */

	.fd_config = NULL,
	.sync_ctrlmem_polllist = NULL,
};

static void host_vsock_release(struct ux_sock *sk)
{
	free(host_vsock_sk(sk));
}

static void host_vsock_set_write_space(ux_sock_t *sk, int sndbuf)
{
	UNUSED(sndbuf);
	if (sk != NULL) {
		ux_sock_poll_common(sk, EPOLLOUT);
	}
}

static ux_host_vsock_sock_t *alloc_and_init_host_vsock(ux_socket_t *socket, int domain, int type, int protocol)
{
	ux_sock_t *sock;
	ux_host_vsock_sock_t *host_vsock;

	sock = ux_sock_alloc(socket, sizeof(ux_host_vsock_sock_t), 0);
	if (sock == NULL) {
		return NULL;
	}

	host_vsock = host_vsock_sk(sock);
	ux_sock_init(&host_vsock->sk, NULL, domain, type, protocol);

	host_vsock->cid = 0;
	host_vsock->host_netdev = NULL;

	host_vsock->sk.release = host_vsock_release;
	host_vsock->sk.set_write_space = host_vsock_set_write_space;

	return host_vsock;
}

int ux_host_vsock_create(ux_socket_t *socket, int domain, int type, int protocol, int flags)
{
	ux_host_vsock_sock_t *host_vsock = alloc_and_init_host_vsock(socket, domain, type, protocol);
	if (host_vsock == NULL) {
		return E_HM_NOMEM;
	}

	socket->ops = &host_vsock_ops;
	ux_socket_set_sock(socket, &host_vsock->sk);
	if ((flags & SOCK_NONBLOCK) != 0) {
		(void)socket_fcntl(socket, F_SETFL, O_NONBLOCK);
	}

	return E_HM_OK;
}
