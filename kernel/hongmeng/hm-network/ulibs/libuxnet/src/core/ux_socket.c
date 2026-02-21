/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: interface for network socket
 * Author: Huawei OS Kernel Lab
 * Create: Sun Apr 28 13:34:09 2019
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <security/security.h>
#include <libhmsrv_sys/hm_timer.h>

#include "osal.h"
#include "ux_socket.h"

#include "security/sec_hook.h"
#include "ux_domain.h"
#include "ux_eventpoll.h"
#include "ux_compat.h"
#include "ux_audit.h"
#include "ucopy.h"

#define MAX_MSG_SIZE 0xFFFF

int socket_create(ux_socket_t **sockp, int domain, int type, int protocol, ux_namespace_t *ns)
{
	int ret = E_HM_OK;
	ux_domain_t *domain_ops = NULL;
	ux_socket_t *sock = NULL;

	net_debug(HMNET_CORE, "%s: domain=%d, type=%d, protocol=%d\n",
			__func__, domain, type, protocol);

	domain_ops = ux_domain_get(domain);
	if (domain_ops == NULL) {
		net_error(HMNET_CORE, "domain %d not found\n", domain);
		return E_HM_AFNOSUPPORT;
	}

	ret = sec_chk_div_socket_create(domain, type, protocol);
	if (ret != E_HM_OK) {
		net_error(HMNET_CORE, "sec_chk_socket_create failed, ret:%s\n", hmstrerror(ret));
		return ret;
	}

	sock = ux_socket_alloc();
	if (sock == NULL) {
		net_error(HMNET_CORE, "failed to alloc new socket\n");
		return E_HM_NOMEM;
	}

	ret = domain_ops->create(sock, domain, type, protocol, ns);
	if (ret != E_HM_OK) {
		net_error(HMNET_CORE, "domain failed to create socket, ret=%d\n", ret);
		ux_socket_free(sock);
		return ret;
	}

	ret = sec_chk_div_socket_post_create(sock, domain, type, protocol);
	if (ret != E_HM_OK) {
		net_error(HMNET_CORE, "sec_chk_socket_post_create failed, ret:%s\n", hmstrerror(ret));
		ux_socket_free(sock);
		return ret;
	}

	*sockp = sock;
	return E_HM_OK;
}

int socket_socketpair(ux_socket_t *sock, ux_socket_t *sock2)
{
	if (sock->ops == NULL || sock->ops->socketpair == NULL) {
		return E_HM_OPNOTSUPP;
	}

	int ret = sec_hook_div_socket_socketpair(sock, sock2);
	if (ret != E_HM_OK) {
		return ret;
	}

	return sock->ops->socketpair(sock, sock2);
}

int socket_connect(ux_socket_t *sock, const struct sockaddr *addr, socklen_t addrlen)
{
	if (sock->ops == NULL || sock->ops->connect == NULL) {
		return E_HM_OPNOTSUPP;
	}

	return sock->ops->connect(sock, addr, addrlen);
}

int socket_bind(ux_socket_t *sock, const struct sockaddr *addr, socklen_t addrlen)
{
	if (sock->ops == NULL || sock->ops->bind == NULL) {
		return E_HM_OPNOTSUPP;
	}

	return sock->ops->bind(sock, addr, addrlen);
}

int socket_listen(ux_socket_t *sock, int backlog)
{
	if (sock->ops == NULL || sock->ops->listen == NULL) {
		return E_HM_OPNOTSUPP;
	}

	int ret = sec_chk_div_socket_listen(sock);
	if (ret != E_HM_OK) {
		return ret;
	}

	return sock->ops->listen(sock, backlog);
}

int socket_accept(ux_socket_t *sock, ux_socket_t *nsock,
		  struct sockaddr *addr, socklen_t *addrlen, int flags)
{
	int ret;

	if (sock->ops == NULL || sock->ops->accept == NULL) {
		return E_HM_OPNOTSUPP;
	}

	ret = sec_chk_div_socket_accept(sock);
	if (ret != E_HM_OK) {
		return ret;
	}

	ret = sock->ops->accept(sock, nsock, addr, addrlen, flags);
	if (ret != E_HM_OK) {
		return ret;
	}

	ret = sec_chk_div_socket_post_accept(sock, nsock);

	return ret;
}

int socket_shutdown(ux_socket_t *sock, int how)
{
	if (sock->ops == NULL || sock->ops->shutdown == NULL) {
		return E_HM_OPNOTSUPP;
	}

	int ret = sec_chk_div_socket_shutdown(sock);
	if (ret != E_HM_OK) {
		return ret;
	}

	return sock->ops->shutdown(sock, how);
}

int socket_fcntl(ux_socket_t *sock, int cmd, unsigned long arg)
{
	if (sock->ops == NULL || sock->ops->fcntl == NULL) {
		return E_HM_OPNOTSUPP;
	}

	int ret = sec_chk_div_socket_fcntl(sock, cmd);
	if (ret != E_HM_OK) {
		return ret;
	}

	return sock->ops->fcntl(sock, cmd, (int)(long)arg);
}

int socket_ioctl(ux_socket_t *sock, int cmd, uintptr_t args_u)
{
	int ret;

	if (sock->ops == NULL || sock->ops->ioctl == NULL) {
		return E_HM_OPNOTSUPP;
	}

	ret = sec_chk_div_socket_ioctl(sock, cmd);
	if (ret != E_HM_OK) {
		return ret;
	}

	ret = sock->ops->ioctl(sock, cmd, args_u);
	if (ret == E_HM_NOIOCTLCMD || ret == E_HM_NOSYS) {
		ret = E_HM_NOTTY;
	}

	return ret;
}

ssize_t socket_send(ux_socket_t *sock, const void *buf, size_t len, int flags)
{
	if (sock->ops == NULL || sock->ops->send == NULL) {
		return E_HM_OPNOTSUPP;
	}

	int ret = sec_chk_div_socket_sendmsg(sock);
	if (ret != E_HM_OK) {
		return ret;
	}

	return sock->ops->send(sock, buf, len, flags);
}

ssize_t socket_send_nosec(ux_socket_t *sock, const void *buf, size_t len, int flags)
{
	if (sock->ops == NULL || sock->ops->send == NULL) {
		return E_HM_OPNOTSUPP;
	}

	return sock->ops->send(sock, buf, len, flags);
}

ssize_t socket_recv(ux_socket_t *sock, void *buf, size_t len, int flags)
{
	if (sock->ops == NULL || sock->ops->recv == NULL) {
		return E_HM_OPNOTSUPP;
	}

	int ret = sec_chk_div_socket_recvmsg(sock);
	if (ret != E_HM_OK) {
		return ret;
	}

	return sock->ops->recv(sock, buf, len, flags);
}

int socket_sendmmsg(ux_socket_t *sock, const void *msgvec, size_t vlen, int flags)
{
	if (sock->ops == NULL || sock->ops->sendmmsg == NULL) {
		return E_HM_OPNOTSUPP;
	}

	int ret = sec_chk_div_socket_sendmsg(sock);
	if (ret != E_HM_OK) {
		return ret;
	}

	return sock->ops->sendmmsg(sock, msgvec, vlen, flags);
}

int socket_recvmmsg(ux_socket_t *sock, const void *msgvec, size_t vlen, int flags, void *timeout)
{
	if (sock->ops == NULL || sock->ops->recvmmsg == NULL) {
		return E_HM_OPNOTSUPP;
	}

	int ret = sec_chk_div_socket_recvmsg(sock);
	if (ret != E_HM_OK) {
		return ret;
	}

	return sock->ops->recvmmsg(sock, msgvec, vlen, flags, timeout);
}

ssize_t socket_sendmsg(ux_socket_t *sock, const struct msghdr *msg, int flags)
{
	if (sock->ops == NULL || sock->ops->sendmsg == NULL) {
		return E_HM_OPNOTSUPP;
	}

	int ret = sec_chk_div_socket_sendmsg(sock);
	if (ret != E_HM_OK) {
		return ret;
	}

	return sock->ops->sendmsg(sock, msg, flags);
}

ssize_t socket_recvmsg(ux_socket_t *sock, struct msghdr *msg, int flags)
{
	if (sock->ops == NULL || sock->ops->recvmsg == NULL) {
		return E_HM_OPNOTSUPP;
	}

	int ret = sec_chk_div_socket_recvmsg(sock);
	if (ret != E_HM_OK) {
		return ret;
	}

	return sock->ops->recvmsg(sock, msg, flags);
}

ssize_t socket_writev(ux_socket_t *sock, const struct iovec *iovs, int iovcnt, int flags)
{
	if (sock->ops == NULL || sock->ops->writev == NULL) {
		return E_HM_OPNOTSUPP;
	}

	int ret = sec_chk_div_socket_sendmsg(sock);
	if (ret != E_HM_OK) {
		return ret;
	}

	return sock->ops->writev(sock, iovs, iovcnt, flags);
}

ssize_t socket_readv(ux_socket_t *sock, const struct iovec *iov, int iovcnt)
{
	if (sock->ops == NULL || sock->ops->readv == NULL) {
		return E_HM_OPNOTSUPP;
	}

	int ret = sec_chk_div_socket_recvmsg(sock);
	if (ret != E_HM_OK) {
		return ret;
	}

	return sock->ops->readv(sock, iov, iovcnt);
}

int socket_fstat(ux_socket_t *sock, struct stat *buf)
{
	uid_t uid = 0;
	int ret = E_HM_OK;
	if (sock->ops == NULL) {
		return E_HM_OPNOTSUPP;
	}

	if (buf == NULL) {
		return E_HM_POSIX_FAULT;
	}

	ret = sec_chk_div_socket_fstat(sock);
	if (ret != E_HM_OK) {
		return ret;
	}

	buf->st_mode |= (S_IFSOCK);
	buf->st_ino = (unsigned long)ux_socket_ino(sock);

	if (sock->ops->fstat != NULL)
		ret = sock->ops->fstat(sock, &uid);

	if(ret != E_HM_OK)
		return ret;

	buf->st_uid = uid;

	return  E_HM_OK;
}

ssize_t socket_sendto(ux_socket_t *sock, const void *buf, size_t len, int flags,
		      const struct sockaddr *daddr, socklen_t addrlen)
{
	if (sock->ops == NULL || sock->ops->sendto == NULL) {
		return E_HM_OPNOTSUPP;
	}

	int ret = sec_chk_div_socket_sendmsg(sock);
	if (ret != E_HM_OK) {
		return ret;
	}

	return sock->ops->sendto(sock, buf, len, flags, daddr, addrlen);
}

ssize_t socket_recvfrom(ux_socket_t *sock, void *buf, size_t len, int flags,
			struct sockaddr *saddr, socklen_t *addrlen)
{
	if (sock->ops == NULL || sock->ops->recvfrom == NULL) {
		return E_HM_OPNOTSUPP;
	}

	int ret = sec_chk_div_socket_recvmsg(sock);
	if (ret != E_HM_OK) {
		return ret;
	}

	return sock->ops->recvfrom(sock, buf, len, flags, saddr, addrlen);
}

ssize_t socket_recvfrom_nosec(ux_socket_t *sock, void *buf, size_t len, int flags,
			      struct sockaddr *saddr, socklen_t *addrlen)
{
	if (sock->ops == NULL || sock->ops->recvfrom == NULL) {
		return E_HM_OPNOTSUPP;
	}

	return sock->ops->recvfrom(sock, buf, len, flags, saddr, addrlen);
}

int socket_getsockopt(ux_socket_t *sock, int level, int optname,
		      void *optval, socklen_t *optlen)
{
	if (sock->ops == NULL || sock->ops->getsockopt == NULL) {
		return E_HM_OPNOTSUPP;
	}

	int ret = sec_chk_div_socket_getsockopt(sock);
	if (ret != E_HM_OK) {
		return ret;
	}

	return sock->ops->getsockopt(sock, level, optname, optval, optlen);
}

int socket_fchown(ux_socket_t *sock, unsigned int uid, unsigned int gid)
{
	int ret;

	if (sock->ops == NULL || sock->ops->fchown == NULL) {
		return E_HM_OPNOTSUPP;
	}

	ret = sec_chk_div_socket_fchown(sock);
	if (ret != E_HM_OK) {
		return ret;
	}

	return sock->ops->fchown(sock, uid, gid);
}

int socket_setsockopt(ux_socket_t *sock, int level, int optname,
		      const void *optval, socklen_t optlen)
{
	if (sock->ops == NULL || sock->ops->setsockopt == NULL) {
		return E_HM_OPNOTSUPP;
	}

	int ret = sec_chk_div_socket_setsockopt(sock);
	if (ret != E_HM_OK) {
		return ret;
	}

	return sock->ops->setsockopt(sock, level, optname, optval, optlen);
}

int socket_getsockname(ux_socket_t *sock, struct sockaddr *addr,
		       socklen_t *addrlen)
{
	if (sock->ops == NULL || sock->ops->getsockname == NULL) {
		return E_HM_OPNOTSUPP;
	}

	int ret = sec_chk_div_socket_getsockname(sock);
	if (ret != E_HM_OK) {
		return ret;
	}

	return sock->ops->getsockname(sock, addr, addrlen);
}

int socket_getpeername(ux_socket_t *sock, struct sockaddr *addr,
		       socklen_t *addrlen)
{
	if (sock->ops == NULL || sock->ops->getpeername == NULL) {
		return E_HM_OPNOTSUPP;
	}

	int ret = sec_chk_div_socket_getpeername(sock);
	if (ret != E_HM_OK) {
		return ret;
	}

	return sock->ops->getpeername(sock, addr, addrlen);
}

int socket_getsockevent(ux_socket_t *sock, unsigned int *rcvev,
			unsigned int *sendev, unsigned int *errev, int *error)
{
	if (sock->ops == NULL || sock->ops->getsockevent == NULL) {
		return E_HM_OPNOTSUPP;
	}

	return sock->ops->getsockevent(sock, rcvev, sendev, errev, error);
}

int socket_check_permission(ux_socket_t *sock, ux_socket_t *peer_sock)
{
	if (sock == NULL || sock->ops == NULL ||
	    sock->ops->check_perm == NULL) {
		return E_HM_OPNOTSUPP;
	}

	return sock->ops->check_perm(sock, peer_sock);
}

int socket_mmap(ux_socket_t *sock, const struct net_mmap_info *info, uintptr_t *rvaddr)
{
	if (sock->ops == NULL || sock->ops->mmap == NULL) {
		return E_HM_NODEV;
	}

	return sock->ops->mmap(sock, info, rvaddr);
}

/* helper functions to copy from user */
static size_t acquire_malloc_length(size_t length)
{
	size_t ret;

	if (length == 0) {
		ret = 1;
	} else {
		ret = length;
	}

	return ret;
}

static int recvmsg_msghdr_check(const struct msghdr *msg)
{
	int iovlen;

	if (msg->msg_namelen > MAX_SOCK_ADDRLEN) {
		return E_HM_INVAL;
	}

	if (msg->msg_controllen > MAX_MSG_CTRL_LEN) {
		return E_HM_INVAL;
	}

	iovlen = msg->msg_iovlen;
	if (iovlen < 0 || iovlen > UIO_MAXIOV) {
		return E_HM_MSGSIZE;
	}

	return 0;
}

static int construct_client_msg_from_32bit_client(struct msghdr *cli_msg,
						  const void *msg)
{
	struct msghdr32 compat_msg = {0};
	struct iovec *iov = NULL;
	struct iovec32 *iov32 = NULL;
	int iovlen;
	size_t len;
	int i;
	int ret;

	ret = hm_actv_read_xact_vm(&compat_msg, msg, sizeof(struct msghdr32));
	if (ret != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	ux_msghdr32_to_msghdr(&compat_msg, cli_msg);

	ret = recvmsg_msghdr_check(cli_msg);
	if (ret < 0) {
		return ret;
	}

	iovlen = cli_msg->msg_iovlen;
	/*
	 * There may be a packet of size 0 in the network,
	 * but malloc(0)/calloc(0) is not allowed,
	 * so an extra byte needs to be allocated.
	 */
	len = acquire_malloc_length((size_t)(unsigned int)iovlen);

	iov32 = calloc(len, sizeof(struct iovec32));
	if (iov32 == NULL) {
		return E_HM_NOMEM;
	}

	ret = hm_actv_read_xact_vm(iov32, cli_msg->msg_iov,
				   (unsigned long)(unsigned int)iovlen * sizeof(struct iovec32));
	if (ret != E_HM_OK) {
		free(iov32);
		return E_HM_POSIX_FAULT;
	}

	iov = calloc(len, sizeof(struct iovec));
	if (iov == NULL) {
		free(iov32);
		return E_HM_NOMEM;
	}
	for (i = 0; i < iovlen; i++) {
		ux_iovec32_to_iovec(&iov32[i], &iov[i]);
	}

	free(iov32);
	cli_msg->msg_iov = iov;

	return 0;
}


static int construct_client_msg_from_64bit_client(struct msghdr *cli_msg,
						  const void *msg)
{
	struct iovec *iov = NULL;
	int iovlen;
	size_t len;
	int ret;

	ret = hm_actv_read_xact_vm(cli_msg, msg, sizeof(struct msghdr));
	if (ret != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	ret = recvmsg_msghdr_check(cli_msg);
	if (ret < 0) {
		return ret;
	}

	iovlen = cli_msg->msg_iovlen;
	len = acquire_malloc_length((size_t)(unsigned int)iovlen);
	iov = calloc(len, sizeof(struct iovec));
	if (iov == NULL) {
		return E_HM_NOMEM;
	}

	ret = hm_actv_read_xact_vm(iov, cli_msg->msg_iov,
				   (unsigned long)(unsigned int)iovlen * sizeof(struct iovec));
	if (ret != E_HM_OK) {
		free(iov);
		return E_HM_POSIX_FAULT;
	}

	cli_msg->msg_iov = iov;

	return 0;
}

static int construct_client_msg(struct msghdr *cli_msg, const void *msg)
{
	int ret;

	if (ux_is_compat_client()) {
		ret = construct_client_msg_from_32bit_client(cli_msg, msg);
	} else {
		ret = construct_client_msg_from_64bit_client(cli_msg, msg);
	}

	return ret;
}

static void destruct_client_msg(struct msghdr *cli_msg)
{
	free(cli_msg->msg_iov);
	cli_msg->msg_iov = NULL;
}

static void free_server_msg_iov(struct msghdr *srv_msg)
{
	free(srv_msg->msg_iov[0].iov_base);
	free(srv_msg->msg_iov);
	srv_msg->msg_iov = NULL;
}

static inline size_t msg_iov_totlen(const struct msghdr *msg)
{
	size_t totlen = 0;
	int i;

	for (i = 0; i < msg->msg_iovlen; i++) {
		totlen += msg->msg_iov[i].iov_len;
	}

	return totlen;
}

static int alloc_server_msg_iov(struct msghdr *srv_msg, const struct msghdr *cli_msg)
{
	size_t totlen;
	struct iovec *iov;
	size_t buflen;
	size_t len;
	void *buf;
	int ret;

	iov = calloc(1, sizeof(struct iovec));
	if (iov == NULL) {
		return E_HM_NOMEM;
	}

	/*
	 * limit the length of iov to defend against DoS attach
	 * caused by malicious large totlen
	 */
	totlen = msg_iov_totlen(cli_msg);
	buflen = totlen > MAX_MSG_SIZE ? MAX_MSG_SIZE : totlen;
	/*
	 * There may be a packet of size 0 in the network,
	 * but malloc(0)/calloc(0) is not allowed,
	 * so an extra byte needs to be allocated.
	 */
	len = acquire_malloc_length(buflen);

	buf = calloc(1, len);
	if (buf == NULL) {
		ret = E_HM_NOMEM;
		goto free_iov;
	}

	iov->iov_base = buf;
	iov->iov_len = buflen;
	srv_msg->msg_iovlen = 1;
	srv_msg->msg_iov = iov;

	return 0;

free_iov:
	free(iov);
	return ret;
}

static int construct_server_msg(struct msghdr *srv_msg, const struct msghdr *cli_msg)
{
	void *msg_name = NULL;
	void *msg_control = NULL;
	int ret = 0;

	srv_msg->msg_flags = cli_msg->msg_flags;

	srv_msg->msg_namelen = cli_msg->msg_namelen;
	if (srv_msg->msg_namelen == 0) {
		srv_msg->msg_name = NULL;
	} else {
		msg_name = calloc(1, srv_msg->msg_namelen);
		if (msg_name == NULL) {
			return E_HM_NOMEM;
		}
		srv_msg->msg_name = msg_name;
	}

	srv_msg->msg_controllen = cli_msg->msg_controllen;
	if (srv_msg->msg_controllen == 0) {
		srv_msg->msg_control = NULL;
	} else {
		/*
		 * When CONFIG_SCM_RIGHTS is enable, we should store scm
		 * info in the end of msg_control which will be processed
		 * by ulibs function.
		 * When client is 32 bit uapp, we should allocate larger
		 * space than spcified by msg_controllen to convert
		 * 64 bit structure to 32 bit.
		 * For simplicity, we just allocate enough space for
		 * both cases.
		 */
		msg_control = calloc(1, MAX_MSG_TOLCTRL_LEN);
		if (msg_control == NULL) {
			ret = E_HM_NOMEM;
			goto free_msg_name;
		}
		srv_msg->msg_control = msg_control;
	}

	ret = alloc_server_msg_iov(srv_msg, cli_msg);
	if (ret < 0) {
		goto free_msg_control;
	}

	return 0;

free_msg_control:
	free(msg_control);

free_msg_name:
	free(msg_name);

	return ret;
}

static void destruct_server_msg(struct msghdr *srv_msg)
{
	free_server_msg_iov(srv_msg);

	if (srv_msg->msg_control != NULL) {
		free(srv_msg->msg_control);
		srv_msg->msg_control = NULL;
	}

	if (srv_msg->msg_name != NULL) {
		free(srv_msg->msg_name);
		srv_msg->msg_name = NULL;
	}
}

static int set_client_msg_name(const struct msghdr *srv_msg, struct msghdr *cli_msg, void *msg)
{
	socklen_t msg_namelen;
	int ret;

	if (cli_msg->msg_name == NULL) {
		return 0;
	}

	msg_namelen = srv_msg->msg_namelen > cli_msg->msg_namelen ?
		      cli_msg->msg_namelen : srv_msg->msg_namelen;
	ret = hm_actv_write_xact_vm(cli_msg->msg_name, srv_msg->msg_name, msg_namelen);
	if (ret < 0) {
		return E_HM_POSIX_FAULT;
	}

	if (ux_is_compat_client()) {
		ret = hm_actv_write_xact_vm(&(((struct msghdr32 *)msg)->msg_namelen), &(srv_msg->msg_namelen), sizeof(socklen_t));
	} else {
		ret = hm_actv_write_xact_vm(&(((struct msghdr *)msg)->msg_namelen), &(srv_msg->msg_namelen), sizeof(socklen_t));
	}
	if (ret < 0) {
		return E_HM_POSIX_FAULT;
	}
	return 0;
}

static void msg_empty_buf_position(int *empty_position_num, size_t *empty_position_offset,
				   const struct msghdr *msg, size_t used_buf_len)
{
	size_t size = 0;
	int i;
	size_t used_len = used_buf_len;

	*empty_position_num = 0;
	*empty_position_offset = 0;
	for (i = 0; i < msg->msg_iovlen; i++) {
		size = msg->msg_iov[i].iov_len;
		if (size > used_len) {
			*empty_position_offset = used_len;
			*empty_position_num = i;
			break;
		} else {
			used_len = used_len - size;
		}
	}
}

static int set_client_msg_iov(const struct msghdr *srv_msg, struct msghdr *cli_msg, size_t recvd_len,
			      size_t used_buf_len)
{
	size_t offset = 0;
	int empty_position_num;
	size_t empty_position_offset;
	size_t size;
	int i;

	msg_empty_buf_position(&empty_position_num, &empty_position_offset, cli_msg, used_buf_len);
	for (i = empty_position_num; i < cli_msg->msg_iovlen && offset < recvd_len; i++) {
		size = cli_msg->msg_iov[i].iov_len - empty_position_offset;
		if (size > recvd_len - offset) {
			size = recvd_len - offset;
		}

		if (hm_actv_write_xact_vm((void *)((char *)cli_msg->msg_iov[i].iov_base + empty_position_offset),
					  (char *)srv_msg->msg_iov[0].iov_base + offset, size) < 0) {
			return E_HM_POSIX_FAULT;
		}
		offset += size;
		empty_position_offset = 0;
	}

	return 0;
}

static int preprocess_and_copy_msg_control(struct msghdr *srv_msg, struct msghdr *cli_msg)
{
	void *srv_addr = srv_msg->msg_control;
	void *cli_addr = cli_msg->msg_control;
	void *msg_control = NULL;
	socklen_t len;
	int ret;

	if (cli_msg->msg_controllen == 0) {
		return 0;
	}

	if (ux_is_compat_client()) {
		msg_control = calloc(1, MAX_MSG_TOLCTRL_LEN);
		if (msg_control == NULL) {
			return E_HM_NOMEM;
		}
		/* srv_msg->msg_controllen may be changed in ux_cmsghdr2compat_cmsghdr */
		ret = ux_cmsghdr2compat_cmsghdr(srv_msg, msg_control);
		if (ret != E_HM_OK) {
			/* returns error means that srv_msg is incomplete due to internal errors */
			free(msg_control);
			return ret;
		}
	} else {
		msg_control = srv_addr;
	}

	len = srv_msg->msg_controllen;
	if (cli_msg->msg_controllen < len) {
		srv_msg->msg_flags = (int)((unsigned int)srv_msg->msg_flags | MSG_CTRUNC);
	}

	if (cli_msg->msg_controllen < len) {
		len = cli_msg->msg_controllen;
	}
	ret = hm_actv_write_xact_vm(cli_addr, msg_control, len);
	if (ret < 0) {
		ret = E_HM_POSIX_FAULT;
	}

	if (msg_control != srv_addr) {
		free(msg_control);
	}
	return ret;
}

static int set_client_msg_control(struct msghdr *srv_msg, struct msghdr *cli_msg, void *msg)
{
	socklen_t len = 0;
	int ret;

	ret = preprocess_and_copy_msg_control(srv_msg, cli_msg);
	if (ret < 0) {
		return ret;
	}

	len = srv_msg->msg_controllen;
	if (len > cli_msg->msg_controllen) {
		len = cli_msg->msg_controllen;
	}
	if (ux_is_compat_client()) {
		ret = hm_actv_write_xact_vm(&(((struct msghdr32 *)msg)->msg_controllen), &len, sizeof(socklen_t));
	} else {
		ret = hm_actv_write_xact_vm(&(((struct msghdr *)msg)->msg_controllen), &len, sizeof(socklen_t));
	}
	if (ret < 0) {
		return E_HM_POSIX_FAULT;
	}

	return 0;
}

static int set_client_msg_flags(const struct msghdr *srv_msg, void *msg)
{
	int ret;

	if (ux_is_compat_client()) {
		ret = hm_actv_write_xact_vm(&(((struct msghdr32 *)msg)->msg_flags), &(srv_msg->msg_flags), sizeof(int));
	} else {
		ret = hm_actv_write_xact_vm(&(((struct msghdr *)msg)->msg_flags), &(srv_msg->msg_flags), sizeof(int));
	}

	if (ret < 0) {
		return E_HM_POSIX_FAULT;
	}
	return 0;
}

static int fill_client_msg(struct msghdr *srv_msg, struct msghdr *cli_msg,
		    void *msg, size_t recvd_len, size_t used_buf_len)
{
	int ret;

	ret = set_client_msg_name(srv_msg, cli_msg, msg);
	if (ret < 0) {
		return ret;
	}

	ret = set_client_msg_iov(srv_msg, cli_msg, recvd_len, used_buf_len);
	if (ret < 0) {
		return ret;
	}

	ret = set_client_msg_control(srv_msg, cli_msg, msg);
	if (ret < 0) {
		return ret;
	}

	ret = set_client_msg_flags(srv_msg, msg);
	if (ret < 0) {
		return ret;
	}

	return 0;
}
static int copy_msgname_from_client(struct msghdr *msg)
{
	void *msg_name = NULL;
	void *client_msg_name = msg->msg_name;
	int ret;

	msg->msg_name = NULL;
	if (msg->msg_namelen == 0) {
		return 0;
	}

	msg_name = calloc(1, msg->msg_namelen);
	if (msg_name == NULL) {
		return E_HM_NOMEM;
	}
	ret = hm_actv_read_xact_vm(msg_name, client_msg_name, msg->msg_namelen);
	if (ret != 0) {
		free(msg_name);
		return E_HM_POSIX_FAULT;
	}

	msg->msg_name = msg_name;
	return 0;
}

static void free_msgname(struct msghdr *msg)
{
	if (msg->msg_name != NULL) {
		free(msg->msg_name);
		msg->msg_name = NULL;
	}
}

static int copy_msgcontrol_from_client(struct msghdr *msg)
{
	void *msg_control = NULL;
	void *client_msg_control = msg->msg_control;
	socklen_t controllen = 0;
	int ret;

	msg->msg_control = NULL;
	if (msg->msg_controllen == 0) {
		return 0;
	}

	controllen = msg->msg_controllen;
	msg_control = calloc(1, controllen);
	if (msg_control == NULL) {
		return E_HM_NOMEM;
	}
	ret = hm_actv_read_xact_vm(msg_control, client_msg_control, controllen);
	if (ret != 0) {
		free(msg_control);
		return E_HM_POSIX_FAULT;
	}

	msg->msg_control = msg_control;
	return 0;
}

static void free_msgcontrol(struct msghdr *msg)
{
	if (msg->msg_control != NULL) {
		free(msg->msg_control);
		msg->msg_control = NULL;
	}
}

static int copy_msgiov_from_client(struct msghdr *msg)
{
	struct iovec *msg_iov = NULL;
	void *client_msg_iov = msg->msg_iov;
	int ret;
	size_t iovlen;

	/*
	 * There may be a packet of size 0 in the network,
	 * but malloc(0)/calloc(0) is not allowed,
	 * so an extra byte needs to be allocated.
	 */
	iovlen = acquire_malloc_length((size_t)(unsigned int)msg->msg_iovlen);

	msg_iov = calloc(iovlen, sizeof(struct iovec));
	if (msg_iov == NULL) {
		return E_HM_NOMEM;
	}

	if (ux_is_compat_client()) {
		ret = ux_get_compat_iov(client_msg_iov, (size_t)(unsigned int)msg->msg_iovlen, msg_iov);
	} else {
		ret = hm_actv_read_xact_vm(msg_iov, client_msg_iov, (size_t)(unsigned int)msg->msg_iovlen * sizeof(struct iovec));
	}

	if (ret < 0) {
		free(msg_iov);
		return ret;
	}

	msg->msg_iov = msg_iov;
	return 0;
}

static void free_msgiov(struct msghdr *msg)
{
	free(msg->msg_iov);
	msg->msg_iov = NULL;
}

static int do_copy_iovec_from_client(const struct msghdr *msg, void *buf,
				     size_t buflen)
{
	int i;
	size_t copy_len;
	size_t offset = 0;
	char *copy_src = NULL;
	int ret;

	if (buf == NULL) {
		return E_HM_POSIX_FAULT;
	}

	for (i = 0; i < msg->msg_iovlen && offset < buflen; i++) {
		copy_len = msg->msg_iov[i].iov_len;
		if (copy_len > buflen - offset) {
			copy_len = buflen - offset;
		}
		copy_src = (char *)buf + offset;
		if (copy_src == NULL) {
			return E_HM_POSIX_FAULT;
		}
		ret = hm_actv_read_xact_vm(copy_src, msg->msg_iov[i].iov_base, copy_len);
		if (ret < 0) {
			return E_HM_POSIX_FAULT;
		}
		offset += copy_len;
	}

	return E_HM_OK;
}

static int copy_iovec_from_client(struct msghdr *msg)
{
	struct iovec *iov;
	size_t totlen;
	size_t buflen;
	size_t len;
	void *buf;
	int ret;

	iov = (struct iovec *)malloc(sizeof(struct iovec));
	if (iov == NULL) {
		return E_HM_NOMEM;
	}

	totlen = msg_iov_totlen(msg);
	/*
	 * alloc MAX_MSG_SIZE bytes at most to avoid malicious user consuming
	 * too many memory and causing DoS
	 */
	buflen = totlen > MAX_MSG_SIZE ? MAX_MSG_SIZE : totlen;

	/*
	 * There may be a packet of size 0 in the network,
	 * but malloc(0)/calloc(0) is not allowed,
	 * so an extra byte needs to be allocated.
	 */
	len = acquire_malloc_length(buflen);

	buf = malloc(len);
	if (buf == NULL) {
		ret = E_HM_NOMEM;
		goto buf_alloc_fail;
	}

	ret = do_copy_iovec_from_client(msg, buf, buflen);
	if (ret < 0) {
		goto iovec_copy_fail;
	}

	free(msg->msg_iov);
	msg->msg_iovlen = 1;
	msg->msg_iov = iov;
	iov->iov_base = buf;
	iov->iov_len = buflen;
	return 0;

iovec_copy_fail:
	free(buf);

buf_alloc_fail:
	free(iov);

	return ret;
}

static void free_iovec(struct msghdr *msg)
{
	free(msg->msg_iov[0].iov_base);
	msg->msg_iov[0].iov_base = NULL;
}

static int copy_msgdata_from_client(struct msghdr *msg)
{
	int ret;

	ret = copy_msgname_from_client(msg);
	if (ret != 0) {
		return ret;
	}

	ret = copy_msgcontrol_from_client(msg);
	if (ret != 0) {
		goto copy_msgcontrol_err;
	}

	ret = copy_msgiov_from_client(msg);
	if (ret != 0) {
		goto copy_msgiov_err;
	}

	ret = copy_iovec_from_client(msg);
	if (ret != 0) {
		goto copy_iovec_err;
	}

	return 0;

copy_iovec_err:
	free_msgiov(msg);
copy_msgiov_err:
	free_msgcontrol(msg);
copy_msgcontrol_err:
	free_msgname(msg);
	return ret;
}

static void free_msgdata(struct msghdr *msg)
{
	free_iovec(msg);
	free_msgiov(msg);
	free_msgcontrol(msg);
	free_msgname(msg);
}

ssize_t socket_sendmsg_wrapper(ux_socket_t *sock, const struct msghdr *msg, int flags,
			   socket_sendmsg_handler handler)
{
	int ret = 0;
	struct msghdr srv_msg = {0};

	if (ux_is_compat_client()) {
		ret = ux_get_compat_msghdr(msg, &srv_msg);
	} else {
		ret = hm_actv_read_xact_vm(&srv_msg, msg, sizeof(struct msghdr));
	}

	if (ret != 0) {
		goto out;
	}

	if (srv_msg.msg_controllen > MAX_MSG_CTRL_LEN ||
	    /* udp or tcp can send packet with len=0 */
	    srv_msg.msg_iovlen < 0 || srv_msg.msg_iovlen > UIO_MAXIOV) {
		ret = E_HM_INVAL;
		goto out;
	}

	if (srv_msg.msg_namelen > MAX_SOCK_ADDRLEN) {
		srv_msg.msg_namelen = MAX_SOCK_ADDRLEN;
	}

	ret = copy_msgdata_from_client(&srv_msg);
	if (ret != 0) {
		goto out;
	}

	ux_audit_fill_sock_addr(srv_msg.msg_name, srv_msg.msg_namelen);

	if (ux_is_compat_client() && srv_msg.msg_control != NULL) {
		ret = ux_compat_cmsghdr2cmsghdr(&srv_msg);
		if (ret < 0) {
			goto cmsg_err;
		}
	}

	ret = (int)handler(sock, &srv_msg, flags);

cmsg_err:
	free_msgdata(&srv_msg);
out:
	return ret;
}

static int do_socket_recvmsg(ux_socket_t *sock, struct msghdr *srv_msg, int flags,
			     struct msghdr *cli_msg, void *msg, socket_recvmsg_handler handler)
{
	size_t unused_buf_len = msg_iov_totlen(cli_msg);
	size_t used_buf_len = 0;
	size_t size;
	ssize_t last_recvd_len;
	int ret;
	do {
		size = unused_buf_len > MAX_MSG_SIZE ? MAX_MSG_SIZE : unused_buf_len;
		srv_msg->msg_iov[0].iov_len = size;
		last_recvd_len = handler(sock, srv_msg, flags);
		if (last_recvd_len < 0) {
			break;
		} else if ((size_t)last_recvd_len < size) {
			ret = fill_client_msg(srv_msg, cli_msg, msg, (size_t)last_recvd_len, used_buf_len);
			if (ret < 0) {
				return ret;
			}
			break;
		} else if ((size_t)last_recvd_len > size) {
			ret = fill_client_msg(srv_msg, cli_msg, msg, size, used_buf_len);
			if (ret < 0) {
				return ret;
			}
			break;
		} else {
			ret = fill_client_msg(srv_msg, cli_msg, msg, (size_t)last_recvd_len, used_buf_len);
			if (ret < 0) {
				return ret;
			}
			used_buf_len += size;
			unused_buf_len -= size;
			last_recvd_len = 0;
		}
	} while ((unused_buf_len != 0) && (((unsigned int)flags & MSG_WAITALL) != 0));

	if (last_recvd_len < 0) {
		return (int)last_recvd_len;
	}

	return (int)((size_t)last_recvd_len + used_buf_len);
}

ssize_t socket_recvmsg_wrapper(ux_socket_t *sock, struct msghdr *msg, int flags,
			   socket_recvmsg_handler handler)
{
	int ret = 0;
	struct msghdr cli_msg = {0};
	struct msghdr srv_msg = {0};

	ret = construct_client_msg(&cli_msg, msg);
	if (ret < 0) {
		return ret;
	}

	ret = construct_server_msg(&srv_msg, &cli_msg);
	if (ret < 0) {
		goto destruct_client_msg;
	}

	ret = do_socket_recvmsg(sock, &srv_msg, flags, &cli_msg, msg, handler);
	if (ret < 0) {
		goto destruct_server_msg;
	}

	ux_audit_fill_sock_addr(srv_msg.msg_name, srv_msg.msg_namelen);

destruct_server_msg:
	destruct_server_msg(&srv_msg);

destruct_client_msg:
	destruct_client_msg(&cli_msg);
	return ret;
}

int common_sendmmsg(ux_socket_t *sock, const void *msgvec, size_t vlen, int flags)
{
	bool flag32 = ux_is_compat_client();
	struct netd_mmsghdr32 *mmsgvec32 = (struct netd_mmsghdr32 *)msgvec;
	struct netd_mmsghdr *mmsgvec = (struct netd_mmsghdr *)msgvec;
	int rc = 0;
	int datagrams = 0;
	size_t len = vlen;
	int flag = flags;

	if (len > UIO_MAXIOV) {
		len = UIO_MAXIOV;
	}

	flag |= MSG_BATCH;

	while (datagrams < (int)len) {
		if (datagrams == (int)len - 1) {
			flag = flags;
		}

		if (flag32) {
			rc = (int)socket_sendmsg(sock, (struct msghdr *)(void *)mmsgvec32, flag);
		} else {
			rc = (int)socket_sendmsg(sock, (struct msghdr *)mmsgvec, flag);
		}
		if (rc < 0) {
			break;
		}
		if (flag32) {
			rc = osal_write_xact_vm(&mmsgvec32->msg_len, &rc, sizeof(int));
			++mmsgvec32;
		} else {
			rc = osal_write_xact_vm(&mmsgvec->msg_len, &rc, sizeof(int));
			++mmsgvec;
		}

		if (rc != E_HM_OK) {
			break;
		}
		++datagrams;
	}

	if (datagrams != 0) {
		return datagrams;
	}
	return rc;
}

static int get_timespec(struct timespec *ts, void *uts)
{
	struct timespec kts;
	struct timespec32 kts32;
	int ret = E_HM_OK;

	if (ux_is_compat_client()) {
		if (osal_read_xact_vm(&kts32, uts, sizeof(kts32)) != E_HM_OK) {
			ret = E_HM_POSIX_FAULT;
		}
		ts->tv_sec = kts32.tv_sec;
		ts->tv_nsec = kts32.tv_nsec;
	} else {
		ret = osal_read_xact_vm(&kts, uts, sizeof(kts));
		if (ret != E_HM_OK) {
			ret = E_HM_POSIX_FAULT;
		}
		ts->tv_sec = kts.tv_sec;
		ts->tv_nsec = kts.tv_nsec;
	}

	return ret;
}

static int put_timespec(const struct timespec *ts, void *uts)
{
	int ret = E_HM_OK;

	if (ux_is_compat_client()) {
		struct timespec32 kts32 = {
			.tv_sec = (int)ts->tv_sec,
			.tv_nsec = (int)ts->tv_nsec
		};

		ret = osal_write_xact_vm(uts, &kts32, sizeof(kts32));
		if (ret != E_HM_OK) {
			ret = E_HM_POSIX_FAULT;
		}
	} else {
		struct timespec kts = {
			.tv_sec = ts->tv_sec,
			.tv_nsec = ts->tv_nsec
		};

		ret = osal_write_xact_vm(uts, &kts, sizeof(kts));
		if (ret != E_HM_OK) {
			ret = E_HM_POSIX_FAULT;
		}
	}

	return ret;
}

static int recvmmsg_check_timeout(const struct timespec *ts, const struct timespec *timeout)
{
	int rc = E_HM_OK;
	struct timespec now = {0};
	struct timespec consume = {0};

	rc = hm_clock_gettime(CLOCK_MONOTONIC, &now);
	if (rc != E_HM_OK) {
		return rc;
	}

	if (now.tv_nsec >= ts->tv_nsec) {
		consume.tv_sec = now.tv_sec - ts->tv_sec;
		consume.tv_nsec = now.tv_nsec - ts->tv_nsec;
	} else {
		consume.tv_sec = now.tv_sec - ts->tv_sec - 1;
		consume.tv_nsec = now.tv_nsec + NSEC_PER_SEC - ts->tv_nsec;
	}

	if (consume.tv_sec < timeout->tv_sec) {
		rc = E_HM_OK;
	} else if (consume.tv_sec > timeout->tv_sec) {
		rc = E_HM_TIMEDOUT;
	} else if (consume.tv_nsec < timeout->tv_nsec) {
		rc = E_HM_OK;
	} else {
		rc = E_HM_TIMEDOUT;
	}

	return rc;
}

static bool is_timespec_invalid(struct timespec *ts)
{
	if (ts->tv_sec < 0) {
		return true;
	}

	if ((unsigned long)ts->tv_nsec >= NSEC_PER_SEC) {
		return true;
	}

	return false;
}

static int do_sock_recvmmsg(ux_socket_t *sock, const void *msgvec,
			    size_t vlen, int flags, struct timespec *timeout)
{
	struct netd_mmsghdr *entry = (struct netd_mmsghdr *)msgvec;
	struct netd_mmsghdr32 *entry32 = (struct netd_mmsghdr32 *)msgvec;
	bool flag32 = ux_is_compat_client();
	int err = 0, datagrams = 0;
	long ret = 0;
	int flag = flags;
	struct timespec ts = {0};

	if (timeout != NULL) {
		if (is_timespec_invalid(timeout)) {
			return E_HM_INVAL;
		}

		ret = hm_clock_gettime(CLOCK_MONOTONIC, &ts);
		if (ret != E_HM_OK) {
			datagrams = (int)ret;
			goto out_put;
		}
	}

	while (datagrams < (int)vlen) {
		if (flag32) {
			err = (int)socket_recvmsg(sock, (struct msghdr *)(void *)entry32, flag & ~MSG_WAITFORONE);
			if (err < 0) {
				break;
			}
			err = osal_write_xact_vm(&entry32->msg_len, &err, sizeof(err));
			++entry32;
		} else {
			err = (int)socket_recvmsg(sock, (struct msghdr *)entry, flag & ~MSG_WAITFORONE);
			if (err < 0) {
				break;
			}
			err = osal_write_xact_vm(&entry->msg_len, &err, sizeof(err));
			++entry;
		}

		if (err != E_HM_OK) {
			break;
		}
		++datagrams;

		if (flag & MSG_WAITFORONE) {
			flag |= MSG_DONTWAIT;
		}

		if (timeout != NULL && recvmmsg_check_timeout(&ts, timeout)) {
			break;
		}
	}

	if (err == 0) {
		goto out_put;
	}

	if (datagrams == 0) {
		datagrams = err;
		goto out_put;
	}

	if (err != E_HM_AGAIN) {
		sock->sk->sk_err = err;
	}
out_put:
	return datagrams;
}

int common_recvmmsg(ux_socket_t *sock, const void *msgvec, size_t vlen, int flags, void *timeout)
{
	struct timespec timeout_sys;
	int datagrams = 0;

	if (timeout == NULL) {
		return do_sock_recvmmsg(sock, msgvec, vlen, flags, NULL);
	}

	if (get_timespec(&timeout_sys, timeout) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	datagrams = do_sock_recvmmsg(sock, msgvec, vlen, flags, &timeout_sys);
	if (datagrams <= 0) {
		return datagrams;
	}

	if ((timeout != NULL) && put_timespec(&timeout_sys, timeout)) {
		datagrams = E_HM_POSIX_FAULT;
	}
	return datagrams;
}

ssize_t socket_readv_wrapper(ux_socket_t *sock, const struct iovec *iov, int iovcnt,
			   socket_recvmsg_handler handler)
{
	int ret = 0;
	struct msghdr msg = {0};

	if (iovcnt < 0 || iovcnt > UIO_MAXIOV) {
		ret = E_HM_INVAL;
		goto out;
	}

	msg.msg_iov = (void *)iov;
	msg.msg_iovlen = iovcnt;

	ret = copy_msgiov_from_client(&msg);
	if (ret != 0) {
		goto out;
	}

	ret = copy_iovec_from_client(&msg);
	if (ret != 0) {
		goto copy_iovec_err;
	}

	ret = (int)handler(sock, &msg, 0);

copy_iovec_err:
	free_msgiov(&msg);
out:
	return ret;
}

ssize_t socket_writev_wrapper(ux_socket_t *sock, const struct iovec *iov, int iovcnt,
			   socket_sendmsg_handler handler)
{
	struct msghdr msg = {0};
	int ret = 0;

	if (iovcnt < 0 || iovcnt > UIO_MAXIOV) {
		ret = E_HM_INVAL;
		goto out;
	}

	msg.msg_iov = (void *)iov;
	msg.msg_iovlen = iovcnt;

	ret = copy_msgiov_from_client(&msg);
	if (ret != 0) {
		goto out;
	}

	ret = copy_iovec_from_client(&msg);
	if (ret != 0) {
		goto copy_iovec_err;
	}

	ret = (int)handler(sock, &msg, 0);

	free_iovec(&msg);
copy_iovec_err:
	free_msgiov(&msg);
out:
	return ret;
}

struct recvfrom_args {
	void *buf;
	void *sbuf;
	size_t len;
	void *addr;
	void *addrlen;
	socklen_t caddrlen;
	struct sockaddr_storage saddr;
	socklen_t saddrlen;
};

static int get_recvfrom_args(struct recvfrom_args *args, void *buf, size_t len, void *addr, void *addrlen)
{
	void *sbuf = NULL;
	size_t sbuflen = 0;
	size_t length;
	int ret;

	if (buf == NULL) {
		return E_HM_POSIX_FAULT;
	}

	if (addr != NULL) {
		ret = hm_actv_read_xact_vm(&args->caddrlen, addrlen, sizeof(socklen_t));
		if (ret < 0) {
			return E_HM_POSIX_FAULT;
		}
		if ((int)args->caddrlen < 0) {
			return E_HM_INVAL;
		}
	}

	sbuflen = len > MAX_MSG_SIZE ? MAX_MSG_SIZE : len;
	/*
	 * There may be a packet of size 0 in the network,
	 * but malloc(0)/calloc(0) is not allowed,
	 * so an extra byte needs to be allocated.
	 */
	length = acquire_malloc_length(sbuflen);

	sbuf = calloc(1, length);
	if (sbuf == NULL) {
		return E_HM_NOMEM;
	}

	args->buf = buf;
	args->sbuf = sbuf;
	args->len = len;
	args->addr = addr;
	args->addrlen = addrlen;
	args->saddrlen = sizeof(args->saddr);

	return 0;
}

static void free_recvfrom_args(struct recvfrom_args *args)
{
	free(args->sbuf);
	args->sbuf = NULL;
}

static int do_socket_recvfrom(ux_socket_t *sock, struct recvfrom_args *args, int flags,
			      socket_recvfrom_handler handler)
{
	void *sbuf;
	size_t len, sbuflen;
	ssize_t recvd_len;
	int err = 0, copied = 0;
	char *target = NULL;

	if (args->buf == NULL) {
		return E_HM_POSIX_FAULT;
	}

	sbuf = args->sbuf;
	len = args->len;

	do {
		sbuflen = len > MAX_MSG_SIZE ? MAX_MSG_SIZE : len;
		recvd_len = handler(sock, sbuf, sbuflen, flags,
					    (struct sockaddr *)(void *)&args->saddr,
					    &args->saddrlen);
		target = (char *)args->buf + copied;
		if (target == NULL) {
			err = E_HM_POSIX_FAULT;
			break;
		}
		/*
		 * The following situations may occur:
		 * 1. recvd_len < 0 means something error happen
		 * 2. recvd_len < sbuflen means that no enough data to read
		 * 3. recvd_len > sbuflen means that MSG_TRUNC is specified
		 *    and there is data discarded
		 * 4. normal case that enough data is read
		 */
		if (recvd_len < 0) {
			err = (int)recvd_len;
			break;
		} else if ((size_t)recvd_len < sbuflen) {
			err = hm_actv_write_xact_vm(target, sbuf, (unsigned long)recvd_len);
			if (err < 0) {
				break;
			}
			copied += (int)recvd_len;
			break;
		} else if ((size_t)recvd_len > sbuflen) {
			err = hm_actv_write_xact_vm(target, sbuf, (unsigned long)sbuflen);
			if (err < 0) {
				break;
			}
			copied += (int)recvd_len;
			break;
		} else {
			err = hm_actv_write_xact_vm(target, sbuf, (unsigned long)sbuflen);
			if (err < 0) {
				break;
			}
			copied += (int)recvd_len;
			len -= (size_t)recvd_len;
		}
	} while ((len > 0) && (((unsigned int)flags & MSG_WAITALL) != 0));

	return copied > 0 ? copied : err;
}

static int fill_client_sockaddr(struct recvfrom_args *args)
{
	void *addr = args->addr;
	void *saddr = &args->saddr;
	socklen_t saddrlen = args->saddrlen;
	socklen_t caddrlen = args->caddrlen;
	int ret;

	if (addr != NULL) {
		ret = hm_actv_write_xact_vm(addr, saddr, saddrlen > caddrlen ? caddrlen : saddrlen);
		if (ret < 0) {
			return E_HM_POSIX_FAULT;
		}

		ret = hm_actv_write_xact_vm(args->addrlen, &saddrlen, sizeof(socklen_t));
		if (ret < 0) {
			return E_HM_POSIX_FAULT;
		}
	}

	return 0;
}

ssize_t socket_recvfrom_wrapper(ux_socket_t *sock,
				void *buf, size_t len, int flags,
				struct sockaddr *saddr, socklen_t *addrlen,
				socket_recvfrom_handler handler)
{
	int err, ret = 0;
	struct recvfrom_args args = {0};

	ret = get_recvfrom_args(&args, buf, len, saddr, addrlen);
	if (ret < 0) {
		return ret;
	}

	ret = do_socket_recvfrom(sock, &args, flags, handler);
	if (ret < 0) {
		goto rcv_err;
	}

	ux_audit_fill_sock_addr((const char *)&args.saddr, args.saddrlen);

	err = fill_client_sockaddr(&args);
	if (err < 0) {
		ret = err;
	}

rcv_err:
	free_recvfrom_args(&args);
	return ret;
}

ssize_t socket_sendto_wrapper(ux_socket_t *sock,
			      const void *buf, size_t len, int flags,
			      const struct sockaddr *daddr, socklen_t addrlen,
			      socket_sendto_handler handler)
{
	int ret = 0;
	void *sbuf;
	struct sockaddr_storage saddr;
	size_t length;
	size_t datalen = len > MAX_MSG_SIZE ? MAX_MSG_SIZE : len;

	if (addrlen > sizeof(struct sockaddr_storage)) {
		return E_HM_INVAL;
	}

	/*
	 * There may be a packet of size 0 in the network,
	 * but malloc(0)/calloc(0) is not allowed,
	 * so an extra byte needs to be allocated.
	 */
	length = acquire_malloc_length(datalen);

	sbuf = calloc(1, length);
	if (sbuf == NULL) {
		return E_HM_NOMEM;
	}

	ret = hm_actv_read_xact_vm(sbuf, buf, datalen);
	if (ret < 0) {
		ret = E_HM_POSIX_FAULT;
		goto free_sbuf;
	}

	if (daddr == NULL && addrlen == 0) {
		ret = (int)handler(sock, sbuf, datalen, flags,
				   NULL, 0);
	} else {
		ret = hm_actv_read_xact_vm(&saddr, daddr, addrlen);
		if (ret < 0) {
			ret = E_HM_POSIX_FAULT;
			goto free_sbuf;
		}
		ux_audit_fill_sock_addr((const char *)&saddr, addrlen);

		ret = (int)handler(sock, sbuf, datalen, flags,
				   (const struct sockaddr *)(void *)&saddr, addrlen);
	}

free_sbuf:
	free(sbuf);
	return ret;
}

ssize_t socket_send_wrapper(ux_socket_t *sock,
			    const void *buf, size_t len, int flags,
			    socket_send_handler handler)
{
	int ret = 0;
	size_t length;
	void *sbuf = NULL;
	size_t datalen = len > MAX_MSG_SIZE ? MAX_MSG_SIZE : len;

	/*
	 * There may be a packet of size 0 in the network,
	 * but malloc(0)/calloc(0) is not allowed,
	 * so an extra byte needs to be allocated.
	 */
	length = acquire_malloc_length(datalen);

	sbuf = malloc(length);
	if (sbuf == NULL) {
		return E_HM_NOMEM;
	}

	if (hm_actv_read_xact_vm(sbuf, buf, datalen) != 0) {
		ret = E_HM_POSIX_FAULT;
		goto read_fail;
	}

	ret = (int)handler(sock, sbuf, datalen, flags);

read_fail:
	free(sbuf);
	return ret;
}

int socket_connect_wrapper(ux_socket_t *sock, const struct sockaddr *addr, socklen_t addrlen,
			   socket_connect_handler handler)
{
	struct sockaddr_storage saddr = {0};
	int ret;

	ret = ucopy_addr_read(&saddr, (const void *)addr, (int)addrlen);
	if (ret < 0) {
		return ret;
	}

	ux_audit_fill_sock_addr((const char *)&saddr, addrlen);

	ret = sec_chk_div_socket_connect(sock, (const struct sockaddr *)&saddr, addrlen);
	if (ret != E_HM_OK) {
		return ret;
	}

	return handler(sock, (const struct sockaddr *)&saddr, addrlen);
}

int socket_bind_wrapper(ux_socket_t *sock, const struct sockaddr *addr, socklen_t addrlen,
			   socket_bind_handler handler)
{
	struct sockaddr_storage saddr = {0};
	int ret;

	ret = ucopy_addr_read(&saddr, (const void *)addr, (int)addrlen);
	if (ret < 0) {
		return ret;
	}

	ux_audit_fill_sock_addr((const char *)&saddr, addrlen);

	ret = check_port_policy(&saddr);
	if (ret != E_HM_OK) {
		return ret;
	}

	ret = sec_chk_div_socket_bind(sock, (const struct sockaddr *)&saddr, addrlen);
	if (ret != E_HM_OK) {
		return ret;
	}

	return handler(sock, (const struct sockaddr *)&saddr, addrlen);
}

#ifdef CONFIG_NET_NS
void ux_namespace_fork(ux_sock_owner_t *src_owner, ux_sock_owner_t *dst_owner)
{
	if (src_owner != NULL && dst_owner != NULL) {
		if (src_owner->ns != NULL) {
			dst_owner->ns = src_owner->ns;
			ux_namespace_get(src_owner->ns);
		}
	}
}

void ux_namespace_revoke(ux_sock_owner_t *owner)
{
	if (owner!= NULL && owner->ns != NULL) {
		ux_namespace_put(owner->ns);
		owner->ns = NULL;
	}
}
#endif
