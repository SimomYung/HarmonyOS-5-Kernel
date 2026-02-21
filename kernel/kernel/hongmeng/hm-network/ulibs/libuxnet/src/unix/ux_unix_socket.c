/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Interface for network unix
 * Author: Huawei OS Kernel Lab
 * Create: Sun Apr 28 13:34:09 2019
 */

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/un.h>
#include <sys/epoll.h>
#include <lib/utils.h>
#include <libstrict/strict.h>
#include <libhmsync/raw_rwlock.h>
#include <libhmsrv_sys/hm_security.h>
#include <libsec/cred.h>
#include <libsec/uid_gid.h>
#include <bits/ioctl.h>
#include <libhmiov/iov_iter.h>
#include <libhmsrv_sys/hm_ctrlmem_kobjctnr.h>

#include <hmkernel/extensions/lsyscall.h>

#include "osal.h"
#include "ucopy.h"
#include "ux_audit.h"
#include "ux_domain.h"
#include "ux_sockopt.h"
#include "ux_socket.h"
#include "ux_compat.h"
#include "ux_eventpoll.h"
#include "ux_procfs.h"
#include "ux_netlog.h"
#include "ux_mempool.h"
#include "ux_unix_scm.h"
#include "ux_unix_util.h"
#include "ux_unix_stat.h"
#include "ux_unix_procfs.h"
#include "ux_unix_socket.h"
#include "ux_unix_fastpath.h"
#include "security/sec_hook.h"

static void ux_unix_push_child(ux_unix_sock_t *usk, ux_unix_sock_t *conn)
{
	ux_sock_get(&conn->sk);
	dlist_insert_tail(&usk->queue, &conn->queuenode);
	usk->queued++;

	ux_wq_wakeup(&usk->acceptwq);
	ux_sock_poll_common(&usk->sk, EPOLLIN);
}

static ux_unix_sock_t *ux_unix_pop_child(ux_unix_sock_t *usk)
{
	ux_unix_sock_t *child = NULL;

	child = dlist_first_entry(&usk->queue, ux_unix_sock_t, queuenode);
	if (child != NULL) {
		dlist_delete(&child->queuenode);
		usk->queued--;
		ux_sock_put(&child->sk);
	}

	if (usk->queued == 0) {
		ux_sock_poll_common(&usk->sk, EPOLLIN << CLEAN_EVENT_SHIFT);
	}
	ux_wq_wakeup(&usk->connwq);

	return child;
}

static int af_unix_socketpair(ux_socket_t *sock, ux_socket_t *sock2);
static int __unix_close(ux_unix_sock_t *usk);

static void __unix_clear_recv_queue(ux_unix_sock_t *usk)
{
	ux_buf_t *tmpbuf = NULL;

	while (ux_buf_queue_len(&usk->recv_queue) > 0) {
		tmpbuf = (ux_buf_t *)ux_buf_queue_dqueue_first(&usk->recv_queue);
		if (tmpbuf != NULL) {
			ux_unix_buf_t *tmp_unix_buf = container_of(tmpbuf, ux_unix_buf_t, ux_buf);
			ux_unix_buff_put(tmp_unix_buf);
		} else {
			net_info(HMNET_UNIX, "clear recv queue may left %u\n", ux_buf_queue_len(&usk->recv_queue));
			break;
		}
	}
}

static void __unix_clear_accept_queue(ux_unix_sock_t *usk)
{
	ux_unix_sock_t *child = NULL;

	while (!dlist_empty(&usk->queue)) {
		child = ux_unix_pop_child(usk);
		if (child != NULL) {
			int err = __unix_close(child);
			if (err != E_HM_OK) {
				net_error(HMNET_UNIX, "close child socket failed ret = %s\n", hmstrerror(err));
			}
		} else {
			break;
		}
	}
}

static int __unix_close(ux_unix_sock_t *usk)
{
	int state = 0;
	ux_unix_sock_t *conn = NULL;

	ux_unix_sock_lock(usk);

	state = usk->state;
	usk->state = SS_UNCONNECTED;
	usk->shutdown = (SEND_SHUTDOWN | RCV_SHUTDOWN);
	ux_set_sock_flags(&usk->sk, SOCK_DEAD);
	conn = usk->peer;
	usk->peer = NULL;
	ux_wq_wakeup_all(&usk->sendwq);
	ux_wq_wakeup_all(&usk->recvwq);

	if (state == SS_LISTENING) {
		__unix_clear_accept_queue(usk);
	}

	if (usk->isbind) {
		bind_path_release(usk);
		usk->isbind = false;
	}
	delete_unixlist(usk);

	ux_unix_fastpath_disable(usk);

	ux_unix_sock_unlock(usk);

	if (conn != NULL) {
		if (usk->type == SOCK_STREAM || usk->type == SOCK_SEQPACKET) {
			ux_unix_sock_lock(conn);
			conn->shutdown = (SEND_SHUTDOWN | RCV_SHUTDOWN);
			notify_peer(usk, conn, 0, true);
			ux_unix_sock_unlock(conn);
		}

		ux_sock_put(&conn->sk);
	}

	/* must clear recv queue last, the recv queue length may be used by notify peer */
	__unix_clear_recv_queue(usk);

	ux_sock_put(&usk->sk);
	return 0;
}

static int af_unix_close(ux_socket_t *sock)
{
	ux_unix_sock_t *usk = unix_sock_from_socket(sock);
	if (usk != NULL) {
		return __unix_close(usk);
	} else {
		return E_HM_INVAL;
	}
}

static int connect_find_peer_by_addr(ux_unix_sock_t *usk, const struct sockaddr *addr, socklen_t addrlen,
				     ux_unix_sock_t **peer)
{
	int ret = E_HM_OK;

	ret = find_peer_by_addr(addr, addrlen, ux_sock_ns(&usk->sk), peer);
	if (ret == E_HM_OK) {
		if ((*peer)->type != usk->type) {
			ret = E_HM_PROTOTYPE;
		}
	}

	if (ret != E_HM_OK && *peer != NULL) {
		ux_sock_put(&(*peer)->sk);
	}

	return ret;
}

static int wait_for_backlog(ux_unix_sock_t *peer, long timeout, bool nonblock)
{
	struct ux_wq_entry waiter;
	int ret = E_HM_OK;

	if (timeout == 0 || nonblock) {
		return E_HM_AGAIN;
	}

	ux_wq_prepare_wait(&peer->connwq, &waiter);
	ux_unix_sock_unlock(peer);

	if (ux_is_accept_queue_full(peer)) {
		ret = ux_wq_do_wait(&waiter, timeout);
	}

	ux_unix_sock_lock(peer);
	ux_wq_finish_wait(&peer->connwq, &waiter);

	if (ret < 0) {
		ret = (ret == E_HM_TIMEDOUT) ? E_HM_AGAIN : ret;
	}

	return ret;
}

static int do_stream_connect_nolock(ux_unix_sock_t *usk, ux_unix_sock_t *peer)
{
	ux_unix_sock_t *conn = NULL;
	int ret = E_HM_OK;

	conn = ux_unix_sock_alloc_and_init(NULL, peer->type, peer->sk.socket_protocol,
				  &peer->cred, ux_sock_ns(&usk->sk));
	if (conn == NULL) {
		ret = E_HM_NOMEM;
	} else {
		ret = sec_hook_div_unix_stream_connect(&usk->sk, &peer->sk, &conn->sk);
		if (ret == E_HM_OK) {
			ret = sec_chk_multi_user_iso(usk->cred.uid, peer->cred.uid);
		}
	}

	if (ret == E_HM_OK) {
		ret = ux_unix_init_root_ns_pid(usk);
	}

	if (ret == E_HM_OK) {
		ret = ux_unix_copy_root_ns_pid(peer, conn);
	}

	if (ret == E_HM_OK) {
		ux_sock_get(&conn->sk);
		usk->peer = conn;

		ux_sock_get(&usk->sk);
		conn->peer = usk;

		usk->state = SS_CONNECTED;
		conn->state = SS_CONNECTED;
		conn->sk.noblock = peer->sk.noblock;

		conn->pathlen = peer->pathlen;
		NOFAIL(memcpy_s(conn->path, USK_PATH_LOCAL_MAX, peer->path, (size_t)peer->pathlen));

		ux_sock_poll_common(&usk->sk, EPOLLHUP << CLEAN_EVENT_SHIFT);
		ux_unix_push_child(peer, conn);
		insert_unixlist(conn);
	}

	if (ret != E_HM_OK) {
		if (conn != NULL) {
			ux_sock_put(&conn->sk);
		}
	}

	return ret;
}

static int unix_stream_connect(ux_unix_sock_t *usk, const struct sockaddr *addr, socklen_t addrlen)
{
	long timeout = usk->sk.sndtimeo;
	bool nonblock = sk_is_noblock(&usk->sk);

	ux_unix_sock_t *peer = NULL;
	int state = -1;    /* -1 is invalid state */
	int ret = E_HM_OK;

	ret = connect_find_peer_by_addr(usk, addr, addrlen, &peer);
	if (ret != 0) {
		return ret;
	}

	ux_unix_sock_lock(peer);

	if (ux_check_sock_flags(&peer->sk, SOCK_DEAD) || ux_is_rd_shutdown(peer)) {
		ret = E_HM_CONNREFUSED;
	} else if (peer->state != SS_LISTENING) {
		ret = E_HM_CONNREFUSED;
	} else {
		while (ux_is_accept_queue_full(peer)) {
			ret = wait_for_backlog(peer, timeout, nonblock);
			if (ret != E_HM_OK) {
				break;
			}

			if (ux_check_sock_flags(&peer->sk, SOCK_DEAD) || ux_is_rd_shutdown(peer)) {
				ret = E_HM_CONNREFUSED;
				break;
			}
		}
	}

	if (ret == E_HM_OK) {
		state = usk->state;
		if (state == SS_UNCONNECTED) {
			ret = E_HM_OK;
		} else if (state == SS_CONNECTED) {
			ret = E_HM_ISCONN;
		} else if (state == SS_CONNECTING) {
			ret = E_HM_ALREADY;
		} else {
			ret = E_HM_INVAL;
		}
	}

	if (ret == E_HM_OK) {
		ux_unix_sock_lock(usk);

		if (usk->state != state) {
			ret = E_HM_INVAL;
		} else {
			ret = do_stream_connect_nolock(usk, peer);
		}

		ux_unix_sock_unlock(usk);
	}

	ux_unix_sock_unlock(peer);
	ux_sock_put(&peer->sk);
	return ret;
}

/*
 * When the peer is connected to us, if one of the bidirectional dgram unix is disconnected,
 * we signal an error and the message is lost.
 */
static void __do_dgram_disconnect_peer(ux_unix_sock_t *usk, ux_unix_sock_t *peer)
{
	ux_unix_sock_pair_lock(usk, peer);
	if (ux_buf_queue_len(&usk->recv_queue) > 0) {
		__unix_clear_recv_queue(usk);
		ux_sock_poll_common(&usk->sk, EPOLLIN << CLEAN_EVENT_SHIFT);

		ux_wq_wakeup_all(&peer->sendwq);
		ux_wq_wakeup_all(&peer->recvwq);
		if (!ux_check_sock_flags(&peer->sk, SOCK_DEAD) && usk_peer(peer) == usk) {
			peer->sk.sk_err = E_HM_CONNRESET;
			ux_sock_poll_common(&peer->sk, EPOLLERR);
		}
	}
	ux_unix_sock_pair_unlock(usk, peer);
}

static int __do_dgram_disconnect(ux_unix_sock_t *usk)
{
	ux_unix_sock_t *old_peer = NULL;
	int ret = E_HM_OK;

	ux_unix_sock_lock(usk);
	if (usk_peer(usk) != NULL) {
		old_peer = usk_peer(usk);
		usk->peer = NULL;
	}
	ux_unix_sock_unlock(usk);

	if (old_peer != NULL) {
		__do_dgram_disconnect_peer(usk, old_peer);
		ux_sock_put(&old_peer->sk);
	}

	return ret;
}

static int __do_dgram_connect(ux_unix_sock_t *usk, const struct sockaddr *addr, socklen_t addrlen)
{
	ux_unix_sock_t *old_peer = NULL;
	ux_unix_sock_t *peer = NULL;
	int ret;

	ret = connect_find_peer_by_addr(usk, addr, addrlen, &peer);
	if (ret != 0) {
		return ret;
	}

	ux_unix_sock_pair_lock(usk, peer);

	if (ux_check_sock_flags(&peer->sk, SOCK_DEAD)) {
		ret = E_HM_CONNREFUSED;
	} else if (peer->peer && peer->peer != usk) {
		ret = E_HM_PERM;
	} else {
		ret = sec_hook_div_unix_may_send(&usk->sk, &peer->sk);
		if (ret == E_HM_OK) {
			ret = sec_chk_multi_user_iso(usk->cred.uid, peer->cred.uid);
		}
	}

	if (ret == E_HM_OK) {
		ret = ux_unix_try_client_fastpath(usk, peer->fp_type);
	}

	if (ret == E_HM_OK) {
		old_peer = usk->peer;
		ux_sock_get(&peer->sk);
		usk->peer = peer;
	}

	ux_unix_sock_pair_unlock(usk, peer);

	if (old_peer != NULL) {
		/*
		 * If the usk has been connected to the old_peer, we should disconnect
		 * old_peer when we connect to another peer through dgram connect.
		 */
		if (peer != old_peer) {
			__do_dgram_disconnect_peer(usk, old_peer);
		}
		ux_sock_put(&old_peer->sk);
	}

	ux_sock_put(&peer->sk);

	return ret;
}

static int unix_dgram_connect(ux_unix_sock_t *usk, const struct sockaddr *addr, socklen_t addrlen)
{
	if (addrlen < offset_of(struct sockaddr_un, sun_path[0])) {
		return E_HM_INVAL;
	} else if (addr->sa_family == AF_UNSPEC) {
		return __do_dgram_disconnect(usk);
	} else {
		return __do_dgram_connect(usk, addr, addrlen);
	}
}

static int unix_connect_handler(ux_socket_t *sock, const struct sockaddr *addr, socklen_t addrlen)
{
	ux_unix_sock_t *usk = unix_sock_from_socket(sock);
	if (usk == NULL) {
		return E_HM_INVAL;
	}

	if (usk->type != SOCK_DGRAM) {
		return unix_stream_connect(usk, addr, addrlen);
	} else {
		return unix_dgram_connect(usk, addr, addrlen);
	}
}

static int af_unix_connect(ux_socket_t *sock, const struct sockaddr *addr, socklen_t addrlen)
{
	return socket_connect_wrapper(sock, addr, addrlen, unix_connect_handler);
}

static int unix_bind_handler(ux_socket_t *sock, const struct sockaddr *addr, socklen_t addrlen)
{
	const struct sockaddr_un *addr_un = (const struct sockaddr_un *)(uintptr_t)addr;
	ux_unix_sock_t *usk = unix_sock_from_socket(sock);

	if (usk == NULL) {
		return E_HM_INVAL;
	}

	return __unix_bind(usk, addr_un, addrlen);
}

static int af_unix_bind(ux_socket_t *sock, const struct sockaddr *addr, socklen_t addrlen)
{
	return socket_bind_wrapper(sock, addr, addrlen, unix_bind_handler);
}

static int af_unix_listen(ux_socket_t *sock, int backlog)
{
	ux_unix_sock_t *usk = unix_sock_from_socket(sock);
	int tmp_backlog = backlog;
	int ret = E_HM_OK;

	if (usk == NULL) {
		return E_HM_INVAL;
	}

	if ((unsigned int)tmp_backlog >= NR_USK) {
		tmp_backlog = NR_USK;
	}

	ux_unix_sock_lock(usk);

	if (usk->type == SOCK_DGRAM) {
		ret = E_HM_OPNOTSUPP;
	} else if (usk->state != SS_UNCONNECTED && usk->state != SS_LISTENING) {
		ret = E_HM_INVAL;
	} else if (!usk->isbind) {
		ret = E_HM_INVAL; /* bind must be done before call listen */
	} else {
		ret = ux_unix_init_root_ns_pid(usk);
	}

	if (ret == E_HM_OK) {
		if ((unsigned short)tmp_backlog > usk->backlog) {
			ux_wq_wakeup(&usk->connwq);
		}
		usk->state = SS_LISTENING;
		usk->backlog = (unsigned short)tmp_backlog;
		ux_sock_poll_common(&usk->sk, (EPOLLOUT | EPOLLHUP) << CLEAN_EVENT_SHIFT);
	}

	ux_unix_sock_unlock(usk);
	return ret;
}

static int __do_wait_for_connect(ux_unix_sock_t *usk, bool nonblock, long timeout)
{
	int ret = E_HM_OK;

	if (ux_is_rd_shutdown(usk)) {
		ret = E_HM_CONNABORTED;
	} else if (nonblock) {
		ret = E_HM_AGAIN;
	} else {
		struct ux_wq_entry waiter;

		ux_wq_prepare_wait(&usk->acceptwq, &waiter);
		ux_unix_sock_unlock(usk);

		if (ux_is_accept_queue_empty(usk)) {
			ret = ux_wq_do_wait(&waiter, timeout);
		}
		ux_wq_finish_wait(&usk->acceptwq, &waiter);

		ux_unix_sock_lock(usk);
		if (ret < 0) {
			ret = (ret == E_HM_TIMEDOUT) ? E_HM_AGAIN : ret;
		}
	}

	return ret;
}

static int __do_unix_accept(ux_unix_sock_t *usk, ux_socket_t *sock, ux_socket_t *nsock,
			    struct sockaddr *addr, socklen_t *addrlen, int flags)
{
	ux_unix_sock_t *pusk = NULL;
	ux_unix_sock_t *nusk = NULL;
	ux_epoll_context_t *epoll_ctx = NULL;
	int ret = E_HM_OK;

	if (dlist_empty(&usk->queue) || usk->queued == 0) {
		ret = E_HM_CONNABORTED;
	} else {
		nusk = ux_unix_pop_child(usk);
		if (nusk == NULL) {
			ret = E_HM_CONNABORTED;
		}
	}

	if (ret == E_HM_OK) {
		pusk = usk_peer(nusk);
		ux_unix_sock_lock(pusk);
		if (addr != NULL && addrlen != NULL) {
			ux_unix_make_addr(pusk->path, pusk->pathlen,
					  (struct sockaddr_un *)(void *)addr, addrlen);
		}
		ux_unix_sock_unlock(pusk);

		if (((unsigned int)flags & SOCK_NONBLOCK) != 0) {
			nusk->sk.noblock |= SOCK_NONBLOCK;
		}
		ux_socket_set_sock(nsock, &nusk->sk);
		nsock->ops = sock->ops;
		nusk->sk.passcred = usk->sk.passcred;    /* inherit flags */
		ux_sock_set_socket(nsock->sk, nsock);

		epoll_ctx = ux_socket_get_epoll_ctx(nsock);
		if (epoll_ctx != NULL) {
			raw_mutex_lock(&epoll_ctx->epi_mutex);
			ret = ux_spawn_polllist(nsock, epoll_ctx, SHARE_POLLLIST);
			raw_mutex_unlock(&epoll_ctx->epi_mutex);
		}
	}

	return ret;
}

static int af_unix_accept(ux_socket_t *sock, ux_socket_t *nsock,
			  struct sockaddr *addr, socklen_t *addrlen, int flags)
{
	ux_unix_sock_t *usk = unix_sock_from_socket(sock);
	bool nonblock = false;
	int ret = E_HM_OK;

	if (usk == NULL) {
		return E_HM_INVAL;
	}

	ux_unix_sock_lock(usk);

	if (usk->type == SOCK_DGRAM) {
		ret = E_HM_OPNOTSUPP;
	} else if (usk->state != SS_LISTENING) {
		ret = E_HM_INVAL;
	} else {
		nonblock = (((unsigned int)flags & SOCK_NONBLOCK) != 0U) ||
			   ((usk->sk.noblock & SOCK_NONBLOCK) != 0U);
		while (ux_is_accept_queue_empty(usk)) {
			ret = __do_wait_for_connect(usk, nonblock, usk->sk.rcvtimeo);
			if (ret != E_HM_OK) {
				break;
			}
		}
	}

	if (ret == E_HM_OK) {
		ret = __do_unix_accept(usk, sock, nsock, addr, addrlen, flags);
	}

	ux_sync_polllist_revents(nsock);
	ux_unix_sock_unlock(usk);
	return ret;
}

static int af_unix_fcntl(ux_socket_t *sock, int cmd, int arg)
{
	ux_sock_t *sk = NULL;
	unsigned int uarg = (unsigned int)arg;
	int ret = 0;

	sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	switch (cmd) {
	case F_GETFL:
		ret = (int)sk->noblock;
		return ret;
	case F_SETFL:
		/* noblock work when O_NONBLOCK == SOCK_NONBLOCK */
		uarg = sock_remove_ignored_fflags(uarg);
		if (sock_if_fflags_supported(uarg)) {
			sk->noblock = sock_enable_supported_fflags(sk->noblock, uarg);
			return 0;
		}
		break;
	default:
		break;
	}

	return E_HM_INVAL;
}

static int af_unix_ioctl_handle_read(ux_unix_sock_t *usk, uintptr_t arg_u)
{
	int ret = 0;
	int val = 0;

	ux_unix_sock_lock(usk);
	val = (int)ux_buf_len(&usk->recv_queue);
	ux_unix_sock_unlock(usk);
	if (hm_actv_write_xact_vm((void *)arg_u, &val, sizeof(int)) != 0) {
		ret = E_HM_POSIX_FAULT;
	}

	return ret;
}

static int af_unix_ioctl(ux_socket_t *sock, int cmd, uintptr_t arg_u)
{
	ux_unix_sock_t *usk = unix_sock_from_socket(sock);
	int val = 0;
	int ret = 0;

	if (usk == NULL) {
		return E_HM_INVAL;
	}

	if (hm_actv_read_xact_vm(&val, (void *)arg_u, sizeof(int)) != E_HM_OK) {
		return E_HM_POSIX_FAULT;
	}

	switch (cmd) {
	case FIONBIO:
		if (val != 0) {
			usk->sk.noblock |= O_NONBLOCK;
		} else {
			usk->sk.noblock &= ~O_NONBLOCK;
		}
		break;
	case TIOCOUTQ:
		ux_unix_sock_lock(usk);
		val = ux_unix_send_buffer_used(usk);
		ux_unix_sock_unlock(usk);

		if (hm_actv_write_xact_vm((void *)arg_u, &val, sizeof(int)) != E_HM_OK) {
			ret = E_HM_POSIX_FAULT;
		}
		break;
	case FIONREAD:
		ret = af_unix_ioctl_handle_read(usk, arg_u);
		break;
	default:
		ret = E_HM_NOIOCTLCMD;
		break;
	}

	return ret;
}

static ssize_t af_unix_send(ux_socket_t *sock, const void *buf, size_t len, int flags)
{
	struct netd_msghdr msg = {0};
	struct iovec iov;
	int msg_flags = flags;
	int ret;

	msg.is_user_iov = true;
	if (flags_need_fastcopy(msg_flags)) {
		msg_flags = flags_clr_fastcopy(msg_flags);
		msg.is_user_iov = false;
	}

	iov.iov_base = (void *)buf;
	iov.iov_len = len;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	ret = iov_iter_init(&msg.msg_iter, &iov, 1);
	if (ret != E_HM_OK) {
		return ret;
	}

	return common_unix_sendmsg(sock, &msg, msg_flags);
}

static ssize_t af_unix_recv(ux_socket_t *sock, void *buf, size_t len, int flags)
{
	struct netd_msghdr msghdr = {0};
	struct iovec iov;
	int ret;

	iov.iov_base = (void *)buf;
	iov.iov_len = len;
	msghdr.msg_iov = &iov;
	msghdr.msg_iovlen = 1;
	msghdr.is_user_iov = true;
	ret = iov_iter_init(&msghdr.msg_iter, &iov, 1);
	if (ret != E_HM_OK) {
		return ret;
	}

	return common_unix_recvmsg(sock, &msghdr, NULL, flags);
}

static ssize_t af_unix_sendmsg(ux_socket_t *sock, const struct msghdr *msg, int flags)
{
	ssize_t ret;
	struct sockaddr_storage address;
	struct netd_msghdr msg_netd = {0};
	/* use stack areas to avoid memory allocation in most cases */
	struct iovec iovstack[UIO_FASTIOV], *iov = iovstack;
	char cmsgstack[sizeof(struct cmsghdr) + 20], *cmsg = cmsgstack;

	msg_netd.msg_name = (void *)&address;
	ret = ucopy_sendmsg_msghdr_read(&msg_netd, msg, &iov);
	if (ret != E_HM_OK) {
		net_warn(HMNET_UNIX, "failed to copy msghdr, ret=%d\n", (int)ret);
		return ret;
	}

	ret = ucopy_cmsg_read(&msg_netd, &cmsg, sizeof(cmsgstack));
	if (ret != E_HM_OK) {
		net_warn(HMNET_UNIX, "failed to copy cmsg, ret=%d\n", (int)ret);
		goto free_iov;
	}

	msg_netd.is_user_iov = true;
	ret = common_unix_sendmsg(sock, &msg_netd, flags);

	/* free cmsg if needed */
	if (cmsg != cmsgstack) {
		osal_free(cmsg);
	}

free_iov: /* free iov if needed */
	if (iov != iovstack) {
		osal_free(iov);
	}
	return ret;
}

static ssize_t __unix_recvmsg(ux_socket_t *sock, struct netd_msghdr *msg, int flags)
{
	ssize_t ret;
	ux_scm_cookie_t scm = {0};

	ret = common_unix_recvmsg(sock, msg, &scm, flags);
	if (ret >= 0) {
		unix_scm_recv((struct msghdr *)msg, &scm, flags);
	}

	unix_scm_destroy(&scm);
	return ret;
}

static ssize_t unix_recvmsg(ux_socket_t *sock, struct netd_msghdr *msg, struct msghdr *umsg,
			    struct sockaddr *uaddr, int flags)
{
	int err;
	ssize_t ret;
	socklen_t *uaddr_len;
	struct sockaddr_storage addr;
	uintptr_t cmsg_ptr = (uintptr_t)msg->msg_control;
	struct msghdr32 *umsg_compat = (struct msghdr32 *)(void *)umsg;

	/* userspace msg_namelen address */
	uaddr_len = &umsg->msg_namelen;
	if (ux_is_compat_client()) {
		uaddr_len = &umsg_compat->msg_namelen;
	}

	msg->msg_name = &addr;
	msg->msg_namelen = sizeof(struct sockaddr_storage);
	msg->msg_flags = (int)((unsigned int)flags & MSG_CMSG_CLOEXEC);
	ret = __unix_recvmsg(sock, msg, flags);
	if (ret < 0) {
		return ret;
	}

	if (uaddr != NULL) {
		err = ucopy_addr_write(&addr, (int)msg->msg_namelen, uaddr, uaddr_len);
		if (err != E_HM_OK) {
			return E_HM_POSIX_FAULT;
		}
	}

	if (msg->msg_flags != 0) {
		err = osal_copy_to_user(&umsg->msg_flags,
					&msg->msg_flags, sizeof(msg->msg_flags));
		if (err != E_HM_OK) {
			return E_HM_POSIX_FAULT;
		}
	}
	/* update msg_controllen */
	if (msg->msg_controllen != 0 || (uintptr_t)msg->msg_control != cmsg_ptr) {
		socklen_t controllen = (socklen_t)((uintptr_t)msg->msg_control - cmsg_ptr);
		if (ux_is_compat_client()) {
			err = osal_copy_to_user(&umsg_compat->msg_controllen,
						&controllen, sizeof(controllen));
		} else {
			err = osal_copy_to_user(&umsg->msg_controllen,
						&controllen, sizeof(controllen));
		}

		if (err != E_HM_OK) {
			return E_HM_POSIX_FAULT;
		}
	}

	return ret;
}

static ssize_t af_unix_recvmsg(ux_socket_t *sock, struct msghdr *msg, int flags)
{
	ssize_t ret;
	/* userspace address */
	struct sockaddr *uaddr;
	struct netd_msghdr msg_netd = {0};
	struct iovec iovstack[UIO_FASTIOV], *iov = iovstack;

	ret = ucopy_recvmsg_msghdr_read(&msg_netd, msg, &uaddr, &iov);
	if (ret != E_HM_OK) {
		net_warn(HMNET_UNIX, "failed to copy msghdr, ret=%d\n", (int)ret);
		return ret;
	}

	msg_netd.is_user_iov = true;
	ret = unix_recvmsg(sock, &msg_netd, msg, uaddr, flags);

	/* free iov if needed */
	if (iov != iovstack) {
		osal_free(iov);
	}

	return ret;
}

static ssize_t af_unix_sendto(ux_socket_t *sock, const void *buf, size_t len,
			      int flags, const struct sockaddr *daddr,
			      socklen_t addrlen)
{
	struct sockaddr_storage saddr = {0};
	struct netd_msghdr msg = {0};
	struct iovec iov;
	int msg_flags = flags;
	int ret;

	msg.is_user_iov = true;
	if (flags_need_fastcopy(msg_flags)) {
		msg_flags = flags_clr_fastcopy(msg_flags);
		ux_audit_fill_sock_addr((const char *)daddr, addrlen);
		msg.is_user_iov = false;
		msg.msg_name = (struct sockaddr *)daddr;
	} else {
		if (daddr != NULL) {
			ret = ucopy_addr_read(&saddr, daddr, (int)addrlen);
			if (ret != E_HM_OK) {
				return ret;
			}
			ux_audit_fill_sock_addr((const char *)&saddr, addrlen);
			msg.msg_name = (struct sockaddr *)&saddr;
		}
	}

	iov.iov_base = (void *)buf;
	iov.iov_len = len;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_namelen = addrlen;
	ret = iov_iter_init(&msg.msg_iter, &iov, 1);
	if (ret != E_HM_OK) {
		return ret;
	}

	return common_unix_sendmsg(sock, &msg, msg_flags);
}

static ssize_t af_unix_recvfrom(ux_socket_t *sock, void *buf, size_t len, int flags,
				struct sockaddr *uaddr, socklen_t *uaddrlen)
{
	struct sockaddr_storage addr = { 0 };
	struct netd_msghdr msg = { 0 };
	int msg_flags = flags;
	struct iovec iov;
	ssize_t ret;

	if (buf == NULL) {
		return E_HM_POSIX_FAULT;
	}

	iov.iov_base = (void *)buf;
	iov.iov_len = len;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.is_user_iov = true;
	if (flags_need_fastcopy(msg_flags)) {
		msg_flags = flags_clr_fastcopy(msg_flags);
		msg.is_user_iov = false;
	}

	ret = iov_iter_init(&msg.msg_iter, &iov, 1);
	if (ret != E_HM_OK) {
		return ret;
	}

	if (uaddr != NULL && uaddrlen != NULL) {
		msg.msg_name = (struct sockaddr *)&addr;
		msg.msg_namelen = sizeof(addr);
	}

	ret = common_unix_recvmsg(sock, &msg, NULL, msg_flags);
	if (ret > 0) {
		if (uaddr != NULL) {
			int err = ucopy_addr_write(&addr, (int)msg.msg_namelen, uaddr, uaddrlen);
			if (err != E_HM_OK) {
				return err;
			}
		}
	}

	return ret;
}

static ssize_t af_unix_readv(ux_socket_t *sock, const struct iovec *iovs, int iovcnt)
{
	ssize_t ret;
	struct netd_msghdr msg_netd = {0};
	/* use stack areas to avoid memory allocation in most cases */
	struct iovec iovstack[UIO_FASTIOV], *iov = iovstack;

	if ((iovcnt < 0) || (iovcnt > UIO_MAXIOV)) {
		return E_HM_MSGSIZE;
	}

	ret = ucopy_import_iovec((struct iovec *)iovs, (unsigned int)iovcnt,
				 UIO_FASTIOV, &iov, &msg_netd.msg_iter);
	if (ret < 0) {
		net_warn(HMNET_UNIX, "failed to copy msg_iov, ret=%d\n", (int)ret);
		return ret;
	}

	if (ret > 0) {
		msg_netd.is_user_iov = true;
		ret = common_unix_recvmsg(sock, &msg_netd, NULL, 0);
	}

	if (iov != iovstack) {
		osal_free(iov);
	}

	return ret;
}

static ssize_t unix_writev_fast(ux_socket_t *sock, const struct iovec *iovs, size_t len)
{
	struct netd_msghdr msg_netd = {0};
	struct iovec iov;
	ssize_t ret;

	iov.iov_base = (void *)iovs;    /* iovs is the real data, see the kernel copy logic */
	iov.iov_len = len;
	msg_netd.msg_iov = &iov;
	msg_netd.msg_iovlen = 1;
	msg_netd.is_user_iov = false;

	ret = iov_iter_init(&msg_netd.msg_iter, &iov, 1);
	if (ret != E_HM_OK) {
		return ret;
	}

	return common_unix_sendmsg(sock, &msg_netd, 0);
}

static ssize_t unix_writev_slow(ux_socket_t *sock, const struct iovec *iovs, int iovcnt)
{
	ssize_t ret;
	struct netd_msghdr msg_netd = {0};
	/* use stack areas to avoid memory allocation in most cases */
	struct iovec iovstack[UIO_FASTIOV], *iov = iovstack;

	if ((iovcnt > UIO_MAXIOV) || (iovcnt < 0)) {
		return E_HM_MSGSIZE;
	}

	msg_netd.is_user_iov = true;

	ret = ucopy_import_iovec((struct iovec *)iovs, (unsigned int)iovcnt,
				 UIO_FASTIOV, &iov, &msg_netd.msg_iter);
	if (ret < 0) {
		net_warn(HMNET_UNIX, "failed to copy msg_iov, ret=%d\n", (int)ret);
		return ret;
	}

	if (ret > 0) {
		ret = common_unix_sendmsg(sock, &msg_netd, 0);
	}

	if (iov != iovstack) {
		osal_free(iov);
	}
	return ret;
}

static ssize_t af_unix_writev(ux_socket_t *sock, const struct iovec *iovs, int iovcnt, int flags)
{
	ssize_t ret;

	/*
	 * NOTE: if fastcopy, iovcnt means iov len, max is 4k
	 * iovs has processed to be srv buf.
	 */
	if (flags_need_fastcopy(flags)) {
		ret = unix_writev_fast(sock, iovs, (size_t)(unsigned)iovcnt);
	} else {
		ret = unix_writev_slow(sock, iovs, iovcnt);
	}

	return ret;
}

static int af_unix_getsockopt(ux_socket_t *sock, int level, int optname,
			      void *optval, socklen_t *optlen)
{
	ux_unix_sock_t *usk = unix_sock_from_socket(sock);
	int ret = 0;

	if (usk == NULL) {
		return E_HM_INVAL;
	}

	if (level == SOL_SOCKET && optname == SO_PEERCRED) {
		ret = getsockopt_peercred(usk, optval, optlen);
	} else {
		ret = ux_common_getsockopt(sock, level, optname,
					   optval, optlen);
	}

	return ret;
}

static int af_unix_setsockopt(ux_socket_t *sock, int level, int optname,
			      const void *optval, socklen_t optlen)
{
	ux_sock_t *sk = NULL;
	int ret = 0;

	sk = ux_socket_get_sock(sock);
	if (sk == NULL) {
		return E_HM_INVAL;
	}

	ret = ux_common_setsockopt(sock, level, optname, optval, optlen);

	return ret;
}

static int af_unix_shutdown(ux_socket_t *sock, int how)
{
	ux_unix_sock_t *usk = unix_sock_from_socket(sock);
	ux_unix_sock_t *conn = NULL;
	unsigned int uhow = (unsigned int)how;
	uint32_t event;

	if (how < SHUT_RD || how > SHUT_RDWR) {
		return E_HM_INVAL;
	}

	if (usk == NULL) {
		return E_HM_INVAL;
	}

	++uhow;
	ux_unix_sock_lock(usk);
	usk->shutdown |= (unsigned char)uhow;
	conn = usk_peer(usk);
	if (conn != NULL) {
		ux_sock_get(&conn->sk);
	}
	ux_unix_sock_unlock(usk);
	ux_wq_wakeup_all(&usk->acceptwq);
	ux_wq_wakeup_all(&usk->connwq);
	ux_wq_wakeup_all(&usk->sendwq);
	ux_wq_wakeup_all(&usk->recvwq);
	event = epoll_get_shutdown_events(usk->shutdown);
	ux_sock_poll_common(&usk->sk, event);

	if (conn != NULL && (conn->type == SOCK_STREAM || conn->type == SOCK_SEQPACKET)) {
		unsigned int mask = 0;
		if ((uhow & RCV_SHUTDOWN) != 0U) {
			mask |= SEND_SHUTDOWN;
		}
		if ((uhow & SEND_SHUTDOWN) != 0U) {
			mask |= RCV_SHUTDOWN;
		}

		ux_unix_sock_lock(conn);
		conn->shutdown |= (unsigned char)mask;
		ux_wq_wakeup(&conn->sendwq);
		ux_wq_wakeup(&conn->recvwq);
		ux_wq_wakeup(&conn->connwq);
		ux_wq_wakeup(&conn->acceptwq);
		ux_unix_sock_unlock(conn);

		event = epoll_get_shutdown_events(conn->shutdown);
		ux_sock_poll_common(&conn->sk, event);
	}

	if (conn != NULL) {
		ux_sock_put(&conn->sk);
	}
	return 0;
}

static int af_unix_getsockname(ux_socket_t *sock, struct sockaddr *addr,
			       socklen_t *addrlen)
{
	ux_unix_sock_t *usk = unix_sock_from_socket(sock);

	if (usk == NULL) {
		return E_HM_INVAL;
	}

	if (addr == NULL || addrlen == NULL) {
		return E_HM_POSIX_FAULT;
	}

	ux_unix_make_addr(usk->path, usk->pathlen, (struct sockaddr_un *)(void *)addr, addrlen);
	return 0;
}

static int af_unix_getpeername(ux_socket_t *sock, struct sockaddr *addr,
			       socklen_t *addrlen)
{
	ux_unix_sock_t *usk = unix_sock_from_socket(sock);
	ux_unix_sock_t *peer = NULL;

	if (addr == NULL || addrlen == NULL) {
		return E_HM_POSIX_FAULT;
	}

	if (usk == NULL) {
		return E_HM_INVAL;
	}

	ux_unix_sock_lock(usk);

	peer = usk_peer(usk);
	if (peer == NULL || usk->state == SS_CONNECTING || usk->state == SS_LISTENING) {
		ux_unix_sock_unlock(usk);
		return E_HM_NOTCONN;
	}

	ux_unix_make_addr(peer->path, peer->pathlen, (struct sockaddr_un *)(void *)addr, addrlen);

	ux_unix_sock_unlock(usk);

	return 0;
}

static int af_unix_check_perm(ux_socket_t *sock, ux_socket_t *peer_sock)
{
	ux_unix_sock_t *peer_usk = unix_sock_from_socket(peer_sock);
	ux_unix_sock_t *usk = unix_sock_from_socket(sock);
	ux_unix_sock_t *peer = NULL;

	if (usk == NULL || peer_usk == NULL) {
		return E_HM_INVAL;
	}

	ux_unix_sock_lock(usk);
	peer = usk_peer(usk);
	if (peer == NULL || peer != peer_usk) {
		ux_unix_sock_unlock(usk);
		return E_HM_INVAL;
	}
	ux_unix_sock_unlock(usk);

	return 0;
}

/* domain socket custom operations of fd can be added here */
static void af_unix_fd_config(ux_socket_t *sock, struct sysarg_fdtable_fd_install *config)
{
	/* flag can be customed, i.e. FDTABLE_FD_FLAG_FAST_COPY */
	ux_epoll_context_t *epoll_ctx = ux_socket_get_epoll_ctx(sock);
	if (config != NULL) {
		if(epoll_ctx != NULL && epoll_ctx->kobj_polllist != NULL) {
			config->common_file = epoll_ctx->kobj_polllist->polllist;
		}
#ifdef CONFIG_FD_FLAG_INSTALL
		config->flags |= FDTABLE_FD_FLAG_FAST_COPY;
#endif
		config->flags |= FDTABLE_FD_POLLABLE;
	}
}

static int af_unix_getpeerinfo(ux_socket_t *sock, int *peer_pid, unsigned long long *peer_cookie)
{
	ux_unix_sock_t *usk = unix_sock_from_socket(sock);
	if (usk == NULL) {
		return E_HM_INVAL;
	}
	ux_unix_getpeerinfo(usk, peer_pid, peer_cookie);
	return E_HM_OK;
}

static ux_proto_ops_t usk_ops = {
	.close = af_unix_close,
	.shutdown = af_unix_shutdown,
	.socketpair = af_unix_socketpair,
	.connect = af_unix_connect,
	.bind = af_unix_bind,
	.listen = af_unix_listen,
	.accept = af_unix_accept,
	.fcntl = af_unix_fcntl,
	.ioctl = af_unix_ioctl,
	.send = af_unix_send,
	.recv = af_unix_recv,
	.sendmmsg = common_sendmmsg,
	.recvmmsg = common_recvmmsg,
	.sendmsg = af_unix_sendmsg,
	.recvmsg = af_unix_recvmsg,
	.sendto = af_unix_sendto,
	.recvfrom = af_unix_recvfrom,
	.writev = af_unix_writev,
	.readv = af_unix_readv,
	.getsockopt = af_unix_getsockopt,
	.setsockopt = af_unix_setsockopt,
	.getsockname = af_unix_getsockname,
	.getpeername = af_unix_getpeername,
	.check_perm = af_unix_check_perm,
#ifdef CONFIG_LSYSCALL_ENABLED
	.epoll_recheck_revents = NULL,
#endif  /* !CONFIG_LSYSCALL_ENABLED */
	.fd_config = af_unix_fd_config,
	.mmap = NULL,
	.sync_ctrlmem_polllist = NULL,
};

static ux_priv_ops_t usk_priv_ops = {
	.getpeerinfo = af_unix_getpeerinfo,
};

static int unix_socket_create(ux_socket_t *sock, int domain, int type, int protocol, ux_namespace_t *ns)
{
	int ret;
	struct ucred cred = {0};
	unsigned int flags = 0;
	ux_unix_sock_t *usk = NULL;

	UNUSED(domain);
	flags = (unsigned int)type & ~SOCK_TYPE_MASK;
	type = (int)((unsigned int)type & SOCK_TYPE_MASK);

	if ((protocol != 0) && (protocol != PF_UNIX)) {
		net_error(HMNET_UNIX, "invalid protocol, protocol: %d \n", protocol);
		return E_HM_PROTONOSUPPORT;
	}

	switch (type) {
	case SOCK_STREAM:
	case SOCK_SEQPACKET:
	case SOCK_DGRAM:
		sock->ops = &usk_ops;
		sock->priv_ops = &usk_priv_ops;
		break;
	default:
		return E_HM_SOCKTNOSUPPORT;
	}

	ret = osal_get_ucred(&cred);
	if (ret != E_HM_OK) {
		net_error(HMNET_UNIX, "failed to get ucred, ret: %s \n", hmstrerror(ret));
		return ret;
	}

	usk = ux_unix_sock_alloc_and_init(sock, type, protocol, &cred, ns);
	if (usk == NULL) {
		net_error(HMNET_UNIX, "failed to alloc sock \n");
		return E_HM_NOMEM;
	}

	usk->backlog = (unsigned short)ux_max_dgram_qlen();
	ux_socket_set_sock(sock, &usk->sk);
	if ((flags & SOCK_NONBLOCK) != 0) {
		(void)socket_fcntl(sock, F_SETFL, O_NONBLOCK);
	}

	insert_unixlist(usk);
	ux_sync_polllist_revents(sock);
	return 0;
}

static int af_unix_socketpair(ux_socket_t *sock, ux_socket_t *sock2)
{
	ux_unix_sock_t *usk = unix_sock_from_socket(sock);
	ux_unix_sock_t *usk2 = unix_sock_from_socket(sock2);

	if (usk == NULL || usk2 == NULL) {
		return E_HM_INVAL;
	}

	if (usk->type == SOCK_DGRAM) {
		ux_sock_get(&usk->sk);
		usk2->peer = usk;

		ux_sock_get(&usk2->sk);
		usk->peer = usk2;
	} else {
		ux_sock_get(&usk->sk);
		ux_sock_get(&usk2->sk);
		usk->peer = usk2;
		usk->state = SS_CONNECTED;
		usk2->peer = usk;
		usk2->state = SS_CONNECTED;
		ux_sock_poll_common(&usk->sk, EPOLLHUP << CLEAN_EVENT_SHIFT);
		ux_sock_poll_common(&usk2->sk, EPOLLHUP << CLEAN_EVENT_SHIFT);
	}

	ux_unix_init_root_ns_pid(usk);
	ux_unix_init_root_ns_pid(usk2);

	return 0;
}

static ux_domain_t unix_domain = {
	.family = AF_UNIX,
	.create = unix_socket_create,
};

int af_unix_init(void)
{
	int ret;
	struct sec_cred_cfg cred_cfg;

	ux_unix_fastpath_init(&usk_ops);

	ux_mempool_init();

	ux_unix_procfs_init();

	ret = ux_domain_register(&unix_domain);
	if (ret != E_HM_OK) {
		net_error(HMNET_UNIX, "fail to register unix domain\n");
		return ret;
	}

	ret = bindtable_init();
	if (ret != E_HM_OK) {
		net_error(HMNET_UNIX, "fail to init unix bindtable\n");
		return ret;
	}

	ret = hm_sec_cred_cfg(&cred_cfg);
	if (ret != E_HM_OK) {
		net_warn(HMNET_UNIX, "fail to get cred cfg.\n");
		return ret;
	}
	set_unix_dac_enabled(cred_cfg.cfg_mask & SEC_SUPPORT_DAC_BIT);

	return 0;
}
