/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Netlink core methods
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan  3 17:10:16 2019
 */

#include <sys/socket.h>

#include <libhmsync/raw_rwlock.h>
#include <libstrict/strict.h>
#include <libsec/cred.h>

#include "hm_netlink.h"

#include "ux_domain.h"
#include "ux_sockopt.h"

#include "ux_netlink_core.h"
#include "ux_netlink_table.h"
#include "ux_netlink_route.h"
#include "ux_netlink_generic.h"
#include "ux_netlink_audit.h"
#include "modules/ux_netlink_modules.h"

#include "ux_eventpoll.h"

#define MAX_AUTOBIMD_PORT 0xfffff000
#define BITS_PER_LONG 32

static ssize_t ux_netlink_recv(ux_socket_t *sock, void *buf, size_t len,
			       int flags);
static ssize_t ux_netlink_send(ux_socket_t *sock, const void *buf, size_t len,
			       int flags);

int ux_netlink_attr_get(uintptr_t data, size_t len, const struct rtattr *attr)
{
	size_t payload_len = (size_t)(unsigned int)RTA_PAYLOAD(attr);
	if (len < payload_len) {
		return E_HM_INVAL;
	}

	if (payload_len == 0UL) {
		return 0;
	}

	NOFAIL(memcpy_s((void *)data, len, RTA_DATA(attr), payload_len));

	return 0;
}

bool ux_netlink_attr_put(ux_pktbuf_t *pkt, uint16_t attr, const void *data, size_t len)
{
	struct nlattr *nla;

	nla = ux_pktbuf_push(pkt, (unsigned int)NLA_ALIGN(NLA_HDRLEN + len));
	if (nla == NULL) {
		return true;
	}

	nla->nla_type = attr;
	nla->nla_len = NLA_HDRLEN + (uint16_t)len;

	if (len == 0UL) {
		return 0;
	}

	NOFAIL(memcpy_s((void *)((char *)nla + NLA_HDRLEN), len, data, len));

	return false;
}

void ux_netlink_error(ux_sock_t *sk, const struct nlmsghdr *nlh, int err)
{
	ux_netlink_sock_t *nsk = netlink_sock(sk);
	ux_pktbuf_t *pkt = NULL;
	struct nlmsghdr *enlh = NULL;
	struct nlmsgerr *emsg = NULL;
	int ret;

	/* get a vaild family */
	pkt = ux_pktbuf_alloc(NETLINK_PAYLOAD_MAX);
	if (pkt == NULL) {
		return;
	}

	enlh = ux_pktbuf_push(pkt, NLMSG_SPACE(sizeof(struct nlmsgerr)));
	if (enlh == NULL) {
		goto err;
	}

	enlh->nlmsg_type = NLMSG_ERROR;
	enlh->nlmsg_len = NLMSG_SPACE(sizeof(struct nlmsgerr));
	enlh->nlmsg_flags = NLM_F_CAPPED;
	enlh->nlmsg_pid = nsk->port;
	enlh->nlmsg_seq = nlh->nlmsg_seq;

	emsg = ux_nlmsg_data(enlh);
	emsg->error = err;
	NOFAIL(memcpy_s((void *)&emsg->msg, sizeof(emsg->msg),
			nlh, sizeof(struct nlmsghdr)));
	ret = ux_netlink_recv_data(sk, pkt, false);
	if (ret != 0) {
		net_debug(HMNET_NETLINK, "ux_netlink_recv_data return %d.\n", ret);
		ux_pktbuf_free(pkt);
	}

	return;

err:
	ux_pktbuf_free(pkt);
}

/*
 * XXX: We will introduce an OSAL(OS Abstraction Layer) in the future,
 * to split all the os-related operations out of the business logic,
 * thus convert netd core os independent.
 */
static int osal_get_pid(pid_t *pid)
{
	int ret;
	struct sec_chk_cred_xact xact = {0};
	struct sec_chk_cred *cred;
	struct sec_cred_basic *basic_cred;

	ret = sec_chk_cred_xact_init(&xact);
	if (ret != E_HM_OK) {
		return ret;
	}
	cred = sec_chk_src_cred(&xact);
	if (cred == NULL) {
		return E_HM_INVAL;
	}
	basic_cred = basic_cred_of_sec_cred(cred);
	*pid = basic_cred->pid;
	return 0;
}

static int autobind(const ux_socket_t *sock)
{
	ux_netlink_sock_t *nsk = NULL;
	pid_t pid = 0;
	uint32_t max_port = MAX_AUTOBIMD_PORT;
	uint32_t portid;
	int ret;

	nsk = netlink_sock(ux_socket_get_sock(sock));
	if (nsk == NULL) {
		return E_HM_INVAL;
	}

	ret = osal_get_pid(&pid);
	if (ret != 0) {
		net_debug(HMNET_NETLINK, "%s: failed to get current process's pid\n", __func__);
		return ret;
	}

	portid = (uint32_t)pid;
retry:
	ret = ux_netlink_table_insert(nsk, portid);
	if (ret == E_HM_EXIST && nsk->bound == 0) {
		portid = max_port--;
		if (portid == 0) {
			return ret;
		}
		goto retry;
	}
	if (ret == 0) {
		nsk->port = portid;
	}
	return ret;
}

static int netlink_bind_handler(ux_socket_t *sock, const struct sockaddr *addr,
			   socklen_t addrlen)
{
	struct sockaddr_nl *nl = (struct sockaddr_nl *)addr;
	ux_netlink_sock_t *nsk = NULL;
	int ret = 0;

	nsk = netlink_sock(ux_socket_get_sock(sock));
	if (nsk == NULL) {
		return E_HM_INVAL;
	}

	/* check whether addrlen is invalid */
	if (addrlen < sizeof(struct sockaddr_nl)) {
		return E_HM_INVAL;
	}

	/* check address is AF_NETLINK */
	if (nl->nl_family != AF_NETLINK) {
		return E_HM_INVAL;
	}

	if (nsk->bound != 0) {
		if (nsk->port != nl->nl_pid) {
			return E_HM_INVAL;
		} else {
			return 0;
		}
	}

	if (nl->nl_pid != 0) {
		ret = ux_netlink_table_insert(nsk, nl->nl_pid);
		nsk->port = nl->nl_pid;
	} else {
		ret = autobind(sock);
	}
	if (ret != 0) {
		return ret;
	}

	nsk->group = nl->nl_groups;
	return 0;
}

static int ux_netlink_bind(ux_socket_t *sock, const struct sockaddr *addr, socklen_t addrlen)
{
	return socket_bind_wrapper(sock, addr, addrlen, netlink_bind_handler);
}

static int netlink_connect_handler(ux_socket_t *sock, const struct sockaddr *addr, socklen_t addrlen)
{
	struct sockaddr_nl *nl = (struct sockaddr_nl *)addr;
	ux_netlink_sock_t *nsk = NULL;
	int ret;

	nsk = netlink_sock(ux_socket_get_sock(sock));
	if (nsk == NULL) {
		return E_HM_INVAL;
	}

	if (addrlen < sizeof(nl->nl_family)) {
		return E_HM_INVAL;
	}

	if (nl->nl_family == AF_UNSPEC) {
		/* AF_UNSPEC means disconnect */
		sock->state = SS_UNCONNECTED;
		nsk->dst_port = 0;
		nsk->dst_group = 0;
		return 0;
	}

	if (nl->nl_family != AF_NETLINK) {
		/* invalid address family */
		return E_HM_INVAL;
	}

	if (nsk->bound == 0) {
		ret = autobind(sock);
		if (ret != 0) {
			return ret;
		}
	}

	nsk->dst_port = nl->nl_pid;
	nsk->dst_group = nl->nl_groups;
	return 0;
}

static int ux_netlink_connect(ux_socket_t *sock, const struct sockaddr *addr, socklen_t addrlen)
{
	return socket_connect_wrapper(sock, addr, addrlen, netlink_connect_handler);
}

static int ux_netlink_send_unicast(ux_netlink_sock_t *nsk, uint32_t dst_port,
				   ux_pktbuf_t *pkt)
{
	ux_netlink_sock_t *ssk = NULL;
	int ret;

	ssk = ux_netlink_table_lookup(nsk->protocol, dst_port);
	if (ssk == NULL) {
		ux_pktbuf_free(pkt);
		return E_HM_INVAL;
	}

	if (ux_netlink_is_service(ssk) == 0) {
		ux_pktbuf_free(pkt);
		ux_sock_put(&ssk->sk);
		return E_HM_INVAL;
	}

	if (ssk->proto == NULL || ssk->proto->rcv == NULL) {
		ux_pktbuf_free(pkt);
		ux_sock_put(&ssk->sk);
		return E_HM_INVAL;
	}

	ret = ssk->proto->rcv(&nsk->sk, pkt);
	ux_sock_put(&ssk->sk);

	ux_pktbuf_free(pkt);

	return ret;
}

static int construct_pktbuf_from_msghdr(const ux_netlink_sock_t *nsk,
					const struct msghdr *msg,
					size_t *plen, ux_pktbuf_t **ppkt)
{
	size_t len = 0;
	ux_pktbuf_t *pkt = NULL;
	size_t iov_len;
	void *ptr = NULL;
	int i;

	for (i = 0; i < msg->msg_iovlen; i++) {
		len += msg->msg_iov[i].iov_len;
	}

	if (len == 0) {
		goto out;
	}

	if (len > (size_t)(uint32_t)nsk->sk.sndbuf) {
		return E_HM_MSGSIZE;
	}

	pkt = ux_pktbuf_alloc(len);
	if (pkt == NULL) {
		return E_HM_NOMEM;
	}

	for (i = 0; i < msg->msg_iovlen; i++) {
		iov_len = msg->msg_iov[i].iov_len;
		ptr = ux_pktbuf_push(pkt, (unsigned int)iov_len);
		BUG_ON(ptr == NULL);
		NOFAIL(memcpy_s(ptr, (pkt->tot_len - pkt->len) + iov_len, msg->msg_iov[i].iov_base, iov_len));
	}

out:
	*plen = len;
	*ppkt = pkt;
	return 0;
}

static ssize_t netlink_sendmsg_handler(ux_socket_t *sock,
				       const struct msghdr *msg, int flags)
{
	ux_netlink_sock_t *nsk = NULL;
	ux_pktbuf_t *pkt = NULL;
	size_t len = 0;
	int ret = 0;
	uint32_t dst_port = 0;
	struct sockaddr_nl *nl = NULL;

	UNUSED(flags);

	nsk = netlink_sock(ux_socket_get_sock(sock));
	if (nsk == NULL) {
		return E_HM_INVAL;
	}

	if (nsk->bound == 0) {
		ret = autobind(sock);
		if (ret != E_HM_OK) {
			return ret;
		}
	}

	ret = construct_pktbuf_from_msghdr(nsk, msg, &len, &pkt);
	if (ret < 0) {
		return ret;
	}

	if (len == 0) {
		return 0;
	}

	/*
	 * if user sendmsg set msgname, get dst_port from msgname
	 * if msgname is null, get dst_port from usk
	 * send()   get dst_port from nsk
	 * sendto() get dst_port from daddr
	 * The msg_name field is used on an unconnected socket to specify
	 * the target address for a datagram.  It points to a buffer
	 * containing the address; the msg_namelen field should be set to
	 * the size of the address.  For a connected socket, these fields
	 * should be specified as NULL and 0, respectively.
	 */
	if (msg->msg_namelen != 0) {
		if (msg->msg_namelen < sizeof(struct sockaddr_nl)) {
			ux_pktbuf_free(pkt);
			return E_HM_INVAL;
		}
		nl = (struct sockaddr_nl *)msg->msg_name;
		if (nl->nl_family != AF_NETLINK) {
			ux_pktbuf_free(pkt);
			return E_HM_INVAL;
		}
		dst_port = nl->nl_pid;
	} else {
		dst_port = nsk->dst_port;
	}

	ret = ux_netlink_send_unicast(nsk, dst_port, pkt);
	if (ret != E_HM_OK) {
		return (ssize_t)ret;
	}
	ux_sem_signal(&nsk->sk.recvsem);

	return (ssize_t)len;
}

static ssize_t ux_netlink_sendmsg(ux_socket_t *sock, const struct msghdr *msg, int flags)
{
	return socket_sendmsg_wrapper(sock, msg, flags, netlink_sendmsg_handler);
}

static int do_netlink_getpeername(const ux_socket_t *sock,
				  struct sockaddr_nl *nl,
				  socklen_t *addrlen)
{
	ux_netlink_sock_t *nsk = NULL;

	nsk = netlink_sock(ux_socket_get_sock(sock));
	if (nsk == NULL) {
		return E_HM_INVAL;
	}

	if (*addrlen < sizeof(struct sockaddr_nl)) {
		return E_HM_INVAL;
	}

	NOFAIL(memset_s(nl, sizeof(struct sockaddr_nl), 0,
			sizeof(struct sockaddr_nl)));

	nl->nl_family = AF_NETLINK;
	nl->nl_pid = nsk->dst_port;
	nl->nl_groups = nsk->group;
	*addrlen = sizeof(struct sockaddr_nl);

	return E_HM_OK;
}

static ssize_t netlink_recvmsg_handler(ux_socket_t *sock, struct msghdr *msg, int flags)
{
	struct sockaddr_nl *nl = (struct sockaddr_nl *)msg->msg_name;
	size_t tot_iov_len = 0;
	ssize_t buflen = 0;
	ssize_t ret;
	int i;

	if (!is_recv_flag_valid((unsigned int)flags)) {
		return E_HM_INVAL;
	}

	if (nl != NULL) {
		ret = do_netlink_getpeername(sock, nl, &msg->msg_namelen);
		if (ret < 0) {
			return ret;
		}
	}

	for (i = 0; i < msg->msg_iovlen; i++) {
		if ((msg->msg_iov[i].iov_base == NULL) || ((ssize_t)msg->msg_iov[i].iov_len < 0) ||
		    ((size_t)(ssize_t)msg->msg_iov[i].iov_len != msg->msg_iov[i].iov_len) ||
		    ((ssize_t)(buflen + (ssize_t)msg->msg_iov[i].iov_len) < 0)) {
			return -1;
		}

		tot_iov_len += msg->msg_iov[i].iov_len;
		ret = (ssize_t)ux_netlink_recv(sock, msg->msg_iov[i].iov_base, msg->msg_iov[i].iov_len, flags);
		if (ret < 0) {
			return ret;
		}
		buflen = (ssize_t)(ret + buflen);
	}

	if ((size_t)buflen > tot_iov_len) {
		msg->msg_flags = (int)((unsigned int)msg->msg_flags | MSG_TRUNC);
	}
	return buflen;
}

static ssize_t ux_netlink_recvmsg(ux_socket_t *sock, struct msghdr *msg, int flags)
{
	return socket_recvmsg_wrapper(sock, msg, flags, netlink_recvmsg_handler);
}

static ssize_t netlink_send_handler(ux_socket_t *sock, const void *buf, size_t len, int flags)
{
	ux_netlink_sock_t *nsk = NULL;
	ux_pktbuf_t *pkt = NULL;
	ux_sock_t *sk = ux_socket_get_sock(sock);
	int ret;

	UNUSED(flags);

	if (len == 0UL) {
		return 0;
	}

	if (sk == NULL) {
		return E_HM_INVAL;
	}

	nsk = netlink_sock(sk);
	if (nsk->bound == 0) {
		ret = autobind(sock);
		if (ret != E_HM_OK) {
			return ret;
		}
	}

	if (len > (size_t)(uint32_t)nsk->sk.sndbuf) {
		return E_HM_MSGSIZE;
	}

	pkt = ux_pktbuf_alloc(len);
	if (pkt == NULL) {
		return E_HM_NOMEM;
	}

	(void)ux_pktbuf_push(pkt, (unsigned int)len);
	NOFAIL(memcpy_s(pkt->data, pkt->len, buf, len));

	ret = ux_netlink_send_unicast(nsk, nsk->dst_port, pkt);
	if (ret != E_HM_OK) {
		return (ssize_t)ret;
	}
	ux_sem_signal(&nsk->sk.recvsem);

	return (ssize_t)len;
}

static ssize_t ux_netlink_send(ux_socket_t *sock, const void *buf, size_t len, int flags)
{
	return socket_send_wrapper(sock, buf, len, flags, netlink_send_handler);
}

static int ux_netlink_check_empty_recv(ux_sock_t *sk, int flags)
{
	int ret = 0;

	if (dlist_empty(&sk->rcv_queue)) {
		/* Clear accumulated expired signals */
		if ((((unsigned int)flags & MSG_DONTWAIT) != 0U) || (raw_sem_init(&sk->recvsem, 0) != 0)) {
			ux_sock_unlock(sk);
			ux_sock_put(sk);
			return E_HM_AGAIN;
		}
		ux_sock_unlock(sk);
		ret = ux_sem_cond_waiti(&sk->recvsem, sk->rcvtimeo);
		if (ret < 0) {
			ux_sock_put(sk);
			return ret == E_HM_TIMEDOUT ? E_HM_AGAIN : ret;
		}
		ux_sock_lock(sk);
	}
	return 0;
}

static ssize_t ux_netlink_recv(ux_socket_t *sock, void *buf, size_t len, int flags)
{
	ux_netlink_sock_t *nsk = netlink_sock(ux_socket_get_sock(sock));
	ux_sock_t *sk = &nsk->sk;
	ux_pktbuf_t *pkt = NULL;
	bool peek = (((unsigned int)flags & MSG_PEEK) != 0U);
	size_t copy = 0UL;
	int ret;

	ux_sock_get(sk);
	ux_sock_lock(sk);

	ret = ux_netlink_check_empty_recv(sk, flags);
	if (ret != E_HM_OK) {
		return (ssize_t)ret;
	}

	if (!dlist_empty(&sk->rcv_queue)) {
		pkt = dlist_first_entry(&sk->rcv_queue, ux_pktbuf_t, head);
		/* if flag is not MSG_PEEK, pkt should not be NULL since list not empty */
		if (peek == 0) {
			sk->rcv_buf_used -= (int)pkt->len;
			dlist_delete(&pkt->head);
			ux_sem_signal(&sk->sendsem);
		}
	}
	if ((peek == 0) && dlist_empty(&sk->rcv_queue)) {
		ux_sock_poll_common(sk, EPOLLIN << CLEAN_EVENT_SHIFT);
	}
	ux_sock_unlock(sk);

	/* should block util packet recv */
	if (pkt == NULL) {
		ux_sock_put(sk);
		return E_HM_AGAIN;
	}

	if (len != 0UL) {
		copy = len > pkt->len ? pkt->len : len;
		NOFAIL(memcpy_s((void *)buf, len, pkt->data, copy));
	}

	copy = (((unsigned int)flags & MSG_TRUNC) != 0U) ? pkt->len : copy;
	if (!peek) {
		ux_pktbuf_free(pkt);
	}

	ux_sock_put(sk);

	return (int)copy;
}

static ssize_t netlink_sendto_handler(ux_socket_t *sock, const void *buf, size_t len,
				 int flags, const struct sockaddr *daddr,
				 socklen_t addrlen)
{
	struct sockaddr_nl *nl = (struct sockaddr_nl *)(void *)daddr;
	ux_netlink_sock_t *nsk = NULL;
	ux_pktbuf_t *pkt = NULL;
	uint32_t dst_port;
	int ret;

	UNUSED(flags);

	if (len == 0UL) {
		return 0;
	}

	nsk = netlink_sock(ux_socket_get_sock(sock));
	if (nsk == NULL) {
		return E_HM_INVAL;
	}

	if (daddr != NULL) {
		if (addrlen < sizeof(struct sockaddr_nl)) {
			return E_HM_INVAL;
		}

		if (nl->nl_family != AF_NETLINK) {
			return E_HM_INVAL;
		}

		dst_port = nl->nl_pid;
	} else {
		dst_port = nsk->dst_port;
	}

	if (len > (size_t)(uint32_t)nsk->sk.sndbuf) {
		return E_HM_MSGSIZE;
	}

	if (nsk->bound == 0) {
		ret = autobind(sock);
		if (ret != E_HM_OK) {
			return ret;
		}
	}

	pkt = ux_pktbuf_alloc(len);
	if (pkt == NULL) {
		return E_HM_NOMEM;
	}

	(void)ux_pktbuf_push(pkt, (unsigned int)len);
	NOFAIL(memcpy_s(pkt->payload, len, buf, len));

	ret = ux_netlink_send_unicast(nsk, dst_port, pkt);
	if (ret != E_HM_OK) {
		return (ssize_t)ret;
	}

	return (ssize_t)len;
}

static ssize_t ux_netlink_sendto(ux_socket_t *sock, const void *buf, size_t len,
				 int flags, const struct sockaddr *daddr,
				 socklen_t addrlen)
{
	return socket_sendto_wrapper(sock, buf, len, flags, daddr, addrlen,
				     netlink_sendto_handler);
}

static ssize_t netlink_recvfrom_handler(ux_socket_t *sock, void *buf, size_t len,
				   int flags, struct sockaddr *saddr,
				   socklen_t *addrlen)
{
	struct sockaddr_nl *nl = (struct sockaddr_nl *)(void *)saddr;
	int ret;

	if (nl != NULL) {
		if (addrlen == NULL) {
			return (ssize_t)E_HM_INVAL;
		}

		ret = do_netlink_getpeername(sock, nl, addrlen);
		if (ret < 0) {
			return (ssize_t)ret;
		}
	}

	return ux_netlink_recv(sock, buf, len, flags);
}

static ssize_t ux_netlink_recvfrom(ux_socket_t *sock, void *buf, size_t len,
				   int flags, struct sockaddr *saddr,
				   socklen_t *addrlen)
{
	return socket_recvfrom_wrapper(sock, buf, len, flags, saddr, addrlen,
				       netlink_recvfrom_handler);
}

static ssize_t ux_netlink_readv(ux_socket_t *sock, const struct iovec *iov, int iovcnt)
{
	return socket_readv_wrapper(sock, iov, iovcnt, netlink_recvmsg_handler);
}

static ssize_t ux_netlink_writev(ux_socket_t *sock, const struct iovec *iov, int iovcnt, int flags)
{
	UNUSED(flags);
	return socket_writev_wrapper(sock, iov, iovcnt, netlink_sendmsg_handler);
}

static int ux_netlink_getsockopt(ux_socket_t *sock, int level, int optname,
				 void *optval, socklen_t *optlen)
{
	return ux_common_getsockopt(sock, level, optname, optval, optlen);
}

static int __netlink_setsockopt(ux_netlink_sock_t *nsk, int optname,
				const void *optval, socklen_t optlen)
{
	int val = 0;

	if (optlen < sizeof(int)) {
		return E_HM_INVAL;
	}

	if (hm_actv_read_xact_vm(&val, optval, sizeof(int)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	switch (optname) {
	case NETLINK_ADD_MEMBERSHIP:
	case NETLINK_DROP_MEMBERSHIP:
		if ((val <= 0) || ((val - 1) >= BITS_PER_LONG)) {
			return E_HM_INVAL;
		}
		if (optname == NETLINK_ADD_MEMBERSHIP) {
			nsk->group |= (1U << (unsigned int)(val - 1));
		} else {
			nsk->group &= ~(1U << (unsigned int)(val - 1));
		}
		break;
	case NETLINK_CAP_ACK:
		if (val != 0) {
			nsk->flags = (int)((unsigned int)nsk->flags | NETLINK_F_CAP_ACK);
		} else {
			nsk->flags = (int)((unsigned int)nsk->flags & (~NETLINK_F_CAP_ACK));
		}
		break;
	case NETLINK_EXT_ACK:
		if (val != 0) {
			nsk->flags = (int)((unsigned int)nsk->flags | NETLINK_F_EXT_ACK);
		} else {
			nsk->flags = (int)((unsigned int)nsk->flags & (~NETLINK_F_EXT_ACK));
		}
		break;
	default:
		return E_HM_NOPROTOOPT;
	}
	return 0;
}

static int ux_netlink_setsockopt(ux_socket_t *sock, int level, int optname,
				 const void *optval, socklen_t optlen)
{
	ux_netlink_sock_t *nsk = NULL;
	int ret = 0;

	nsk = netlink_sock(ux_socket_get_sock(sock));
	if (nsk == NULL) {
		return E_HM_INVAL;
	}

	if (level == SOL_NETLINK) {
		ret = __netlink_setsockopt(nsk, optname, optval, optlen);
	} else if (level == SOL_SOCKET) {
		ret = ux_common_setsockopt(sock, level, optname,
					   optval, optlen);
	} else {
		ret = E_HM_NOPROTOOPT;
	}

	return ret;
}

static int ux_netlink_getsockname(ux_socket_t *sock, struct sockaddr *addr,
				  socklen_t *addrlen)
{
	struct sockaddr_nl *nl = (struct sockaddr_nl *)addr;
	ux_netlink_sock_t *nsk = NULL;

	nsk = netlink_sock(ux_socket_get_sock(sock));
	if (nsk == NULL) {
		return E_HM_INVAL;
	}

	if (addr == NULL || addrlen == NULL) {
		return E_HM_INVAL;
	}

	if ((int)*addrlen < (int)sizeof(struct sockaddr_nl)) {
		return E_HM_INVAL;
	}

	NOFAIL(memset_s(nl, sizeof(struct sockaddr_nl), 0,
			sizeof(struct sockaddr_nl)));
	nl->nl_family = AF_NETLINK;
	nl->nl_pid = nsk->port;
	*addrlen = sizeof(struct sockaddr_nl);

	return 0;
}

static int ux_netlink_getpeername(ux_socket_t *sock, struct sockaddr *addr,
				  socklen_t *addrlen)
{
	struct sockaddr_nl *nl = (struct sockaddr_nl *)addr;

	if (addr == NULL || addrlen == NULL) {
		return E_HM_INVAL;
	}

	return do_netlink_getpeername(sock, nl, addrlen);
}

static int ux_netlink_getsockevent(ux_socket_t *sock,
				   unsigned int *rcvev, unsigned int *sendev,
				   unsigned int *errev, int *error)
{
	ux_netlink_sock_t *nsk = NULL;
	ux_sock_t *sk = NULL;

	nsk = netlink_sock(ux_socket_get_sock(sock));
	if (nsk == NULL) {
		return E_HM_INVAL;
	}

	sk = &nsk->sk;
	ux_sock_get(sk);

	if (rcvev != NULL) {
		*rcvev = dlist_empty(&sk->rcv_queue) ? 0 : EPOLLIN;
	}
	if (sendev != NULL) {
		*sendev = EPOLLOUT;
	}
	if (errev != NULL) {
		*errev = (sk->sk_err != 0) ? EPOLLERR : 0;
	}
	if (error != NULL) {
		*error = 0;
	}

	ux_sock_put(sk);

	return 0;
}

static int ux_netlink_close(ux_socket_t *sock)
{
	ux_netlink_sock_t *nsk = netlink_sock(ux_socket_get_sock(sock));
	ux_sock_t *sk = &nsk->sk;

	ux_clear_eventpoll_list(sock);
	ux_set_sock_flags(sk, SOCK_DEAD);

	ux_sem_signal(&sk->sendsem);
	ux_sem_signal(&sk->recvsem);
	ux_sock_poll_common(sk, POLLHUP);

	/*
	 * This function uses sk's mutex and table's mutex.
	 * Prevent ABBA deadlock with do_netlink_broadcast.
	 */
	ux_netlink_table_remove(nsk);

	ux_sock_put(sk);
	return 0;
}

static int ux_netlink_ioctl(ux_socket_t *sock, int cmd, uintptr_t args_u)
{
	UNUSED(sock, cmd, args_u);
	return E_HM_OPNOTSUPP;
}

static ux_proto_ops_t netlink_proto_ops = {
	.close = ux_netlink_close,
	.shutdown = NULL,
	.socketpair = NULL,
	.bind = ux_netlink_bind,
	.connect = ux_netlink_connect,
	.listen = NULL,
	.accept = NULL,
	.fcntl = NULL,
	.send = ux_netlink_send,
	.recv = ux_netlink_recv,
	.sendmmsg = NULL,
	.recvmmsg = NULL,
	.sendmsg = ux_netlink_sendmsg,
	.recvmsg = ux_netlink_recvmsg,
	.sendto = ux_netlink_sendto,
	.recvfrom = ux_netlink_recvfrom,
	.readv = ux_netlink_readv,
	.writev = ux_netlink_writev,
	.getsockopt = ux_netlink_getsockopt,
	.setsockopt = ux_netlink_setsockopt,
	.getsockname = ux_netlink_getsockname,
	.getpeername = ux_netlink_getpeername,
	.getsockevent = ux_netlink_getsockevent,
	.ioctl = ux_netlink_ioctl,
	.check_perm = NULL,
#ifdef CONFIG_LSYSCALL_ENABLED
	.epoll_recheck_revents = NULL,
#endif  /* !CONFIG_LSYSCALL_ENABLED */
	.fd_config = NULL,
	.mmap = NULL,
	.sync_ctrlmem_polllist = NULL,
};

static void ux_netlink_sock_free(ux_sock_t *sk)
{
	ux_pktbuf_t *pkt = NULL;
	ux_netlink_sock_t *nsk = netlink_sock(sk);

	ux_sock_lock(sk);
	while (!dlist_empty(&sk->rcv_queue)) {
		pkt = dlist_first_entry(&sk->rcv_queue, ux_pktbuf_t, head);
		dlist_delete(&pkt->head);
		ux_pktbuf_free(pkt);
		pkt = NULL;
	}

	ux_sock_unlock(sk);

	free(nsk);
}

static void af_netlink_set_read_space(ux_sock_t *sk, int sndbuf)
{
	UNUSED(sndbuf);
	if (sk != NULL) {
		ux_sock_lock(sk);
		ux_sem_signal(&sk->sendsem);
		ux_sock_unlock(sk);
	}
}

static ux_netlink_sock_t *ux_netlink_sock_alloc_and_init(ux_socket_t *sock, int domain, int type, int protocol)
{
	ux_netlink_sock_t *nsk = NULL;

	nsk = netlink_sock(ux_sock_alloc(sock, sizeof(ux_netlink_sock_t), 0));
	if (nsk == NULL) {
		return NULL;
	}

	ux_sock_init(&nsk->sk, NULL, domain, type, protocol);
	dlist_init(&nsk->head);
	nsk->sk.release = ux_netlink_sock_free;
	nsk->sk.set_read_space = af_netlink_set_read_space;

	return nsk;
}

static int ux_netlink_create(ux_socket_t *sock, int domain, int type,
			     int protocol, ux_namespace_t *ns)
{
	UNUSED(ns);
	ux_netlink_proto_t *proto = NULL;
	ux_netlink_sock_t *nsk = NULL;

	type = (int)((unsigned int)type & SOCK_TYPE_MASK);
	if (type != SOCK_RAW && type != SOCK_DGRAM) {
		return E_HM_SOCKTNOSUPPORT;
	}

	proto = ux_netlink_proto_lookup(protocol);
	if (proto == NULL) {
		return E_HM_PROTONOSUPPORT;
	}

	nsk = ux_netlink_sock_alloc_and_init(sock, domain, type, protocol);
	if (nsk == NULL) {
		return E_HM_NOMEM;
	}

	nsk->proto = proto;
	nsk->protocol = protocol;

	ux_socket_set_sock(sock, &nsk->sk);
	sock->ops = &netlink_proto_ops;
	sock->priv_ops = NULL;
	sock->state = SS_UNCONNECTED;

	return 0;
}

int ux_netlink_create_service(ux_netlink_proto_t *proto)
{
	ux_netlink_sock_t *nsk = NULL;
	int ret;

	nsk = ux_netlink_sock_alloc_and_init(NULL, 0, 0, 0);
	if (nsk == NULL) {
		return E_HM_NOMEM;
	}

	nsk->proto = proto;
	nsk->flags = NETLINK_SERVICE;
	nsk->protocol = proto->protocol;

	ret = ux_netlink_table_insert(nsk, 0);
	if (ret != E_HM_OK) {
		net_error(HM_NETLINK, "netlink create service failed\n");
		free(nsk);
		return ret;
	}

	return 0;
}

void ux_netlink_release_service(ux_netlink_proto_t *proto)
{
	ux_netlink_sock_t *nsk = NULL;

	nsk = ux_netlink_table_lookup(proto->protocol, 0);

	ux_netlink_table_remove(nsk);

	/* put lookup */
	ux_sock_put(&nsk->sk);

	/* put alloc */
	ux_sock_put(&nsk->sk);
}

int ux_netlink_dump_done(ux_sock_t *sk, uint32_t seq)
{
	ux_netlink_sock_t *nsk = netlink_sock(sk);
	struct nlmsghdr *nlh = NULL;
	ux_pktbuf_t *pkt = NULL;
	int ret;

	pkt = ux_pktbuf_alloc(NETLINK_PAYLOAD_MAX);
	if (pkt == NULL) {
		return E_HM_NOMEM;
	}

	nlh = ux_pktbuf_push(pkt, NLMSG_SPACE(0));
	if (nlh == NULL) {
		ux_pktbuf_free(pkt);
		return E_HM_NOMEM;
	}

	nlh->nlmsg_type = NLMSG_DONE;
	nlh->nlmsg_len = NLMSG_SPACE(sizeof(struct nlmsghdr));
	nlh->nlmsg_flags = NLM_F_MULTI;
	nlh->nlmsg_pid = nsk->port;
	nlh->nlmsg_seq = seq;
	nlh->nlmsg_len = (uint32_t)((char *)pkt->tail - (char *)nlh);
	ret = ux_netlink_recv_data(sk, pkt, false);
	if (ret != 0) {
		ux_pktbuf_free(pkt);
		return ret;
	}

	return 0;
}

static ux_domain_t netlink_domain = {
	.family = AF_NETLINK,
	.create = ux_netlink_create,
};

int af_netlink_init(void)
{
	int ret;

	ret = ux_netlink_table_init();
	if (ret != E_HM_OK) {
		net_error(HMNET_NETLINK, "register netlink table failed\n");
		return ret;
	}

	ret = ux_netlink_route_init();
	if (ret != E_HM_OK) {
		net_error(HMNET_NETLINK, "register netlink route failed\n");
		goto err_route;
	}
	ret = ux_netlink_audit_init();
	if (ret != E_HM_OK) {
		net_error(HMNET_NETLINK, "register netlink audit failed\n");
		goto err_audit;
	}

	ret = ux_netlink_generic_init();
	if (ret != E_HM_OK) {
		net_error(HMNET_NETLINK, "register netlink generic failed\n");
		goto err_generic;
	}

	ret = ux_netlink_modules_init();
	if (ret != E_HM_OK) {
		net_error(HMNET_NETLINK, "register netlink modules failed\n");
		goto err_modules;
	}

	ret = ux_domain_register(&netlink_domain);
	if (ret != E_HM_OK) {
		net_error(HMNET_NETLINK, "register AF_NETLINK domain failed\n");
		goto err_domain;
	}

	return 0;

err_domain:
	ux_netlink_modules_exit();
err_modules:
	ux_netlink_generic_exit();
err_generic:
	ux_netlink_route_exit();
err_audit:
	ux_netlink_audit_exit();
err_route:
	ux_netlink_table_exit();

	return ret;
}

int ux_netlink_recv_data(ux_sock_t *sk, ux_pktbuf_t *pkt, bool nonblock)
{
	int ret;

	ux_sock_lock(sk);
	while (pkt->len > (size_t)(uint32_t)(sk->rcvbuf - sk->rcv_buf_used)) {
		if (ux_check_sock_flags(sk, SOCK_DEAD) != 0) {
			/* socket is dead, we should return an errno, otherwise the @pkt may leak */
			ret = E_HM_CONNREFUSED;
			goto err;
		}

		if (sk_is_noblock(sk) || nonblock) {
			ret = E_HM_AGAIN;
			goto err;
		} else {
			if (raw_sem_init(&sk->sendsem, 0) != 0) {
				ret = E_HM_INVAL;
				goto err;
			}
			ux_sock_unlock(sk);
			/* Wait with lock released */
			ret = ux_sem_cond_waiti(&sk->sendsem, (time_t)sk->sndtimeo);
			if (ret != 0) {
				return ret;
			}
			ux_sock_lock(sk);
		}
	}
	ux_sock_recv_data(sk, pkt);
	ux_sock_unlock(sk);

	return 0;

err:
	ux_sock_unlock(sk);
	return ret;
}
