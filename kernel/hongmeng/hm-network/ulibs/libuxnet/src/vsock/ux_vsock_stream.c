/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: interface for network vsock socket
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 24 19:35:43 2021
 */

#include <libhmiov/iov_iter.h>
#include <libhmsrv_sys/hm_ctrlmem_kobjctnr.h>

#include "ux_netlog.h"
#include "ux_vsock_socket.h"
#include "ux_vsock_trans.h"
#include "ux_vsock_core.h"
#include "ux_vsock_template.h"

#define VSOCK_FIND_PORT_MAX_RETRY	64

#define min(a, b) \
	({ __typeof__ (a) _a = (a); \
	   __typeof__ (b) _b = (b); \
	   _a < _b ? _a : _b; })

DEFINE_CONTAINER_LIST(binded)

#define for_each_binded_vsock(vsock) \
	dlist_for_each_entry(vsock, &binded_vsock_list, ux_vsock_sock_t, node_for_binded)

DEFINE_CONTAINER_LIST(connected)

#define for_each_connected_vsock(vsock) \
	dlist_for_each_entry(vsock, &connected_vsock_list, ux_vsock_sock_t, node_for_connected)

static int check_vm_addr(const struct sockaddr *addr, socklen_t len)
{
	struct sockaddr_vm *vm_addr = NULL;
	int ret = E_HM_OK;

	if ((len < sizeof(struct sockaddr_vm)) || (addr == NULL)) {
		ret = E_HM_INVAL;
	} else {
		vm_addr = (struct sockaddr_vm*)addr;
		if (vm_addr->svm_family != AF_VSOCK) {
			ret = E_HM_AFNOSUPPORT;
		} else if (vm_addr->svm_zero[0] != 0) {
			ret = E_HM_INVAL;
		}
	}

	return ret;
}

static int vsock_wait(ux_vsock_sock_t *vsock, long sndtimeo, bool is_send)
{
	struct raw_sem *sem = is_send ? &vsock->sk.sendsem: &vsock->sk.recvsem;
	int ret = E_HM_OK;

	/* clear accumulated expired signals */
	if (raw_sem_init(sem, 0) != 0) {
		ret = E_HM_INVAL;
	} else {
		unlock_vsock(vsock);

		ret = ux_sem_cond_waiti(sem, sndtimeo);
		if (ret < 0) {
			ret = ((ret == E_HM_TIMEDOUT) ? E_HM_AGAIN : ret);
		} else {
			ret = E_HM_OK;
		}

		lock_vsock(vsock);
	}

	return ret;
}

static int vsock_send_wait(ux_vsock_sock_t *vsock, long sndtimeo)
{
	return vsock_wait(vsock, sndtimeo, true);
}

static int vsock_recv_wait(ux_vsock_sock_t *vsock, long sndtimeo)
{
	return vsock_wait(vsock, sndtimeo, false);
}

/**
 * @desc When peer shutdown or reset, or the guest vm is exited
 * 	 these are abnormal cases and we should wakeup all waiting
 * 	 senders and receivers, otherwise the blocked operation will
 * 	 block forever, for we can not receive response from peer anymore.
 */
static void vsock_wakeup_abnormal(ux_vsock_sock_t *vsock)
{
	ux_sem_signal(&vsock->sk.recvsem);
	ux_sem_signal(&vsock->sk.sendsem);
	ux_sock_poll_common(&vsock->sk, EPOLLERR);
}

static bool is_vm_addr_binded(const struct sockaddr_vm *addr)
{
	return addr->svm_port != VMADDR_PORT_ANY;
}

static bool is_port_binded_no_lock(unsigned int port)
{
	ux_vsock_sock_t *vsock = NULL;

	for_each_binded_vsock(vsock) {
		if (vsock->local_addr.svm_port == port) {
			return true;
		}
	}

	return false;
}

static int find_next_available_port(uint32_t *rport)
{
	static uint32_t port = LAST_RESERVED_PORT + 1;
	int i;

	for (i = 0; i < VSOCK_FIND_PORT_MAX_RETRY; ++i) {
		if (is_port_binded_no_lock(port)) {
			port ++;
			if (port < LAST_RESERVED_PORT) {
				port = LAST_RESERVED_PORT + 1;
			}
			continue;
		}

		*rport = port++;
		return E_HM_OK;
	}

	return E_HM_POSIX_NOENT;
}

/**
 * Before send connect, client need an local port.
 */
static int do_client_bind(ux_vsock_sock_t *vsock)
{
	uint32_t port = 0;
	int ret;

	lock_binded_vsock_list();

	ret = find_next_available_port(&port);
	if (ret == E_HM_OK) {
		make_vm_addr(&vsock->local_addr, vsock_local_cid(), (unsigned int)port);
		add_vsock_to_binded_list_nolock(vsock);
	}

	unlock_binded_vsock_list();

	return ret;
}

static void __vsock_connect_timeout(void *arg)
{
	ux_vsock_sock_t *vsock = (ux_vsock_sock_t *)arg;

	if (vsock != NULL) {
		lock_vsock(vsock);
		if (vsock_state(vsock) == SS_CONNECTING) {
			vsock->connect_timer_id = -1;
			vsock->sk.sk_err = E_HM_TIMEDOUT;
			vsock_set_unconnected(vsock);
			ux_sock_poll_common(&vsock->sk, EPOLLERR);
		}
		unlock_vsock(vsock);

		vsock_put(vsock);
	}
}

static int vsock_connect_normal(ux_vsock_sock_t *vsock, const struct sockaddr_vm *dst)
{
	int ret = E_HM_OK;

	copy_vm_addr(&vsock->remote_addr, dst);

	if (!is_vm_addr_binded(&vsock->local_addr)) {
		ret = do_client_bind(vsock);
	}

	if (ret == E_HM_OK) {
		ret = ux_trans_request(vsock, dst);
	}

	if (ret == E_HM_OK) {
		vsock_set_connecting(vsock);

		if (is_vsock_nonblock(vsock)) {
			vsock_get(vsock);
			ret = vsock_add_timer((int)(vsock->connect_timeout),
					      __vsock_connect_timeout, vsock);
			if (ret < 0) {
				vsock_set_unconnected(vsock);
				vsock_put(vsock);
			} else {
				vsock->connect_timer_id = ret;
				ret = E_HM_INPROGRESS;
			}
		} else {
			ret = vsock_recv_wait(vsock, vsock->connect_timeout);

			/**
			 * After @ux_sem_cond_waiti return before acquire the vsock lock in @vsock_wait
			 * the response packet may come in and be processed, we check the vsock state
			 * first here, to avoid lose this response.
			 */
			if (vsock_state(vsock) == SS_CONNECTED) {
				ret = E_HM_OK;
			}

			if (ret != E_HM_OK) {
				vsock_set_unconnected(vsock);
			} else if (vsock->sk.sk_err != E_HM_OK) {
				vsock_set_unconnected(vsock);
				ret = vsock->sk.sk_err;
			}
		}
	}

	return ret;
}

static int vsock_connect_isconn(ux_vsock_sock_t *vsock, const struct sockaddr_vm *dst)
{
	UNUSED(vsock, dst);
	return E_HM_ISCONN;
}

static int vsock_connect_invalid(ux_vsock_sock_t *vsock, const struct sockaddr_vm *dst)
{
	UNUSED(vsock, dst);
	return E_HM_INVAL;
}

static int vsock_connect_already(ux_vsock_sock_t *vsock, const struct sockaddr_vm *dst)
{
	UNUSED(vsock, dst);
	return E_HM_ALREADY;
}

static int vsock_connect_handler(ux_socket_t *sock, const struct sockaddr *addr, socklen_t addrlen)
{
	ux_vsock_sock_t *vsock = vsock_from_socket(sock);
	int ret;

	if (vsock == NULL) {
		ret = E_HM_INVAL;
	} else {
		ret = check_vm_addr(addr, addrlen);
	}

	if (ret == E_HM_OK) {
		lock_vsock(vsock);
		ret = vsock->pstate->connect(vsock, (struct sockaddr_vm*)addr);
		unlock_vsock(vsock);
	}

	return ret;
}

static int vsock_connect(ux_socket_t *sock, const struct sockaddr *addr, socklen_t addrlen)
{
	return socket_connect_wrapper(sock, addr, addrlen, vsock_connect_handler);
}

static int do_vsock_bind(ux_vsock_sock_t *vsock, struct sockaddr_vm *vm_addr)
{
	int ret = E_HM_OK;

	if (vm_addr->svm_cid == VMADDR_CID_ANY) {
		vm_addr->svm_cid = vsock_local_cid();
	}

	if (vm_addr->svm_cid != vsock_local_cid()) {
		ret = E_HM_ADDRNOTAVAIL;
	}

	lock_binded_vsock_list();

	if (ret == E_HM_OK) {
		if (vm_addr->svm_port == VMADDR_PORT_ANY) {
			unsigned int port = 0;

			ret = find_next_available_port(&port);
			if (ret == E_HM_OK) {
				vm_addr->svm_port = port;
			}
		} else {
			if (is_port_binded_no_lock(vm_addr->svm_port)) {
				ret = E_HM_ADDRINUSE;
			}
		}
	}

	if (ret == E_HM_OK) {
		copy_vm_addr(&vsock->local_addr, vm_addr);

		/*
		 * call bind multiple times is allowed,
		 * so we remove it from binded list first.
		 */
		rm_vsock_from_binded_list_nolock(vsock);
		add_vsock_to_binded_list_nolock(vsock);
	}

	unlock_binded_vsock_list();

	return ret;
}

static int vsock_bind_handler(ux_socket_t *sock,
		      const struct sockaddr *addr, socklen_t addrlen)
{
	ux_vsock_sock_t *vsock = vsock_from_socket(sock);
	int ret;

	if (vsock == NULL) {
		ret = E_HM_INVAL;
	} else {
		ret = check_vm_addr(addr, addrlen);
	}

	if (ret == E_HM_OK) {
		lock_vsock(vsock);
		ret = do_vsock_bind(vsock, (struct sockaddr_vm*)addr);
		unlock_vsock(vsock);
	}

	return ret;
}

static int vsock_bind(ux_socket_t *sock, const struct sockaddr *addr, socklen_t addrlen)
{
	return socket_bind_wrapper(sock, addr, addrlen, vsock_bind_handler);
}

static int do_vsock_listen(ux_vsock_sock_t *vsock, int backlog)
{
	int ret = E_HM_OK;

	if (vsock_state(vsock) != SS_UNCONNECTED) {
		ret = E_HM_INVAL;
	} else if (!is_vm_addr_binded(&vsock->local_addr)) {
		ret = E_HM_INVAL;
	} else {
		vsock_set_listening(vsock);
		if (((unsigned int)backlog) > SOMAXCONN) {
			vsock->backlog = SOMAXCONN;
		} else {
			vsock->backlog = backlog;
		}
	}

	return ret;
}

static int vsock_listen(ux_socket_t *sock, int backlog)
{
	ux_vsock_sock_t *vsock = vsock_from_socket(sock);
	int ret = E_HM_OK;

	if (vsock == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		lock_vsock(vsock);
		ret = do_vsock_listen(vsock, backlog);
		unlock_vsock(vsock);
	}

	return ret;
}

static int do_vsock_accept(ux_socket_t *sock, ux_socket_t *nsock,
			   struct sockaddr *addr, socklen_t *addrlen,
			   unsigned int flags)
{
	ux_vsock_sock_t *vsock = vsock_from_socket(sock);
	int ret = E_HM_OK;

	if (vsock == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		if (vsock_state(vsock) != SS_LISTENING) {
			ret = E_HM_INVAL;
		}
	}

	if (ret == E_HM_OK) {
		while (dlist_empty(&vsock->pending_child) && ret == E_HM_OK) {
			if ((flags & O_NONBLOCK) != 0U) {
				ret = E_HM_AGAIN;
				break;
			}

			ret = vsock_recv_wait(vsock, vsock->sk.sndtimeo);
		}
	}

	if (ret == E_HM_OK) {
		ux_vsock_sock_t *child = vsock_pop_child(vsock);

		if (child != NULL) {
			if (addr != NULL && addrlen != NULL) {
				copy_vm_addr((struct sockaddr_vm*)addr, &child->remote_addr);
				*addrlen = sizeof(struct sockaddr_vm);
			}

			nsock->ops = sock->ops;
			install_vsock_to_socket(nsock, child);

			if ((flags & SOCK_NONBLOCK) != 0U) {
				child->sk.noblock |= SOCK_NONBLOCK;
			}
		} else {
			ret = E_HM_CONNABORTED;
		}
	}

	if (ret == E_HM_OK) {
		ux_epoll_context_t *epoll_ctx = ux_socket_get_epoll_ctx(nsock);
		if (epoll_ctx != NULL) {
			raw_mutex_lock(&epoll_ctx->epi_mutex);
			ret = ux_spawn_polllist(nsock, epoll_ctx, 0);
			raw_mutex_unlock(&epoll_ctx->epi_mutex);
		}
	}

	return ret;
}

static int vsock_accept(ux_socket_t *sock, ux_socket_t *nsock,
			struct sockaddr *addr, socklen_t *addrlen, int flags)
{
	ux_vsock_sock_t *vsock = vsock_from_socket(sock);
	int ret;

	if (vsock == NULL) {
		ret = E_HM_INVAL;
	} else {
		lock_vsock(vsock);
		ret = do_vsock_accept(sock, nsock, addr, addrlen, (unsigned int)flags);
		unlock_vsock(vsock);
	}

	return ret;
}

static int do_vsock_shutdown(ux_vsock_sock_t *vsock, unsigned int mode)
{
	int ret = E_HM_OK;

	if (vsock_state(vsock) == SS_UNCONNECTED) {
		ret = E_HM_NOTCONN;
	} else if(mode != 0) {
		if (vsock_state(vsock) == SS_CONNECTED ||
		    vsock_state(vsock) == SS_DISCONNECTING) {
			ret = ux_trans_shutdown(vsock, mode);
		}

		if (ret == E_HM_OK) {
			vsock->shutdown |= mode;
		}
	}

	return ret;
}

static int vsock_shutdown(ux_socket_t *sock, int how)
{
	ux_vsock_sock_t *vsock = vsock_from_socket(sock);
	int ret = E_HM_OK;

	if (how != SHUT_RD && how != SHUT_WR && how != SHUT_RDWR) {
		ret = E_HM_INVAL;
	} else if (vsock == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		lock_vsock(vsock);
		/**
		 * SHUT_RD = 0, SHUT_WR = 1, SHUT_RDWR = 2,
		 * SHUTDOWN_RD = 1, SHUTDOWN_WR = 2, SHUTDOWN_MASK = 3;
		 */
		ret = do_vsock_shutdown(vsock, (unsigned int)(how + 1));
		unlock_vsock(vsock);
	}

	return ret;
}

static void __vsock_close_timeout(void *arg)
{
	ux_vsock_sock_t *vsock = (ux_vsock_sock_t *)arg;

	if (vsock != NULL) {
		lock_vsock(vsock);
		remove_vsock_from_lists(vsock);
		vsock->shutdown_timer_id = VSOCK_INVALID_TIMER_ID;
		unlock_vsock(vsock);

		vsock_put(vsock);
	}
}

static int do_vsock_close(ux_vsock_sock_t *vsock)
{
	int ret = E_HM_OK;

	if (vsock->peer_shutdown == SHUTDOWN_MASK) {
		ret = ux_trans_reset(vsock, NULL);
		if (ret < 0) {
			hm_error("close send reset failed ret = %d\n", ret);
		}

		remove_vsock_from_lists(vsock);
	} else {
		if (vsock->shutdown != SHUTDOWN_MASK) {
			ret = do_vsock_shutdown(vsock, SHUTDOWN_MASK);
		}

		if (ret == E_HM_OK) {
			vsock_get(vsock);
			ret = vsock_add_timer(VSOCK_DEFAULT_CLOSE_TIMEO,
					      __vsock_close_timeout, vsock);
			if (ret < 0) {
				vsock_put(vsock);
			} else {
				vsock->shutdown_timer_id = ret;
				ret = E_HM_OK;
			}
		}

		if (ret != E_HM_OK) {
			/**
			 * send shutdown or add timer failed, directly release the vsock
			 */
			remove_vsock_from_lists(vsock);
		}
	}

	return ret;
}

int ux_vsock_close(ux_vsock_sock_t *vsock)
{
	int ret = E_HM_OK;

	if (vsock == NULL) {
		ret = E_HM_INVAL;
	} else {
		lock_vsock(vsock);
		if (vsock_state(vsock) == SS_CONNECTED ||
		    vsock_state(vsock) == SS_DISCONNECTING) {
			ret = do_vsock_close(vsock);
		} else {
			remove_vsock_from_lists(vsock);
		}
		unlock_vsock(vsock);

		vsock_put(vsock);
	}

	return ret;
}

static int vsock_close(ux_socket_t *sock)
{
	ux_vsock_sock_t *vsock = vsock_from_socket(sock);
	int ret;

	ux_clear_eventpoll_list(sock);
	if (vsock == NULL) {
		ret = E_HM_INVAL;
	} else {
		ret = ux_vsock_close(vsock);
	}

	return ret;
}

void remove_vsock_from_lists(ux_vsock_sock_t *vsock)
{
	rm_vsock_from_connected_list(vsock);
	rm_vsock_from_binded_list(vsock);
}

static int try_wait_space(ux_vsock_sock_t *vsock, unsigned int flags)
{
	int ret;

	if (((flags & MSG_DONTWAIT) != 0U) || is_vsock_nonblock(vsock)) {
		ret = E_HM_AGAIN;
	} else {
		ret = vsock_send_wait(vsock, vsock->sk.sndtimeo);
	}

	return ret;
}

static ssize_t do_send_data(ux_vsock_sock_t *vsock, const struct iov_iter *iter)
{
	size_t len_to_send;
	char *ptr = iov_iter_curr_ptr(iter);

	uint32_t trans_space_left = ux_trans_space_left(vsock);
	size_t iov_space_left = iov_iter_curr_left(iter);

	len_to_send = min((size_t)trans_space_left, iov_space_left);
	len_to_send = min(len_to_send, VSOCK_DEFAULT_RX_BUF_SIZE);

	return ux_trans_write(vsock, ptr, len_to_send);
}

static ssize_t do_vsock_sendmsg(ux_vsock_sock_t *vsock, const struct msghdr *msg, int flags)
{
	struct iov_iter iter;
	ssize_t total_written = 0;
	int ret;

	if (msg->msg_namelen != 0) {
		return vsock_state(vsock) == SS_CONNECTED ? E_HM_ISCONN : E_HM_OPNOTSUPP;
	}

	if (((unsigned int)flags & MSG_OOB) != 0) {
		return E_HM_OPNOTSUPP;
	}

	if (is_write_shutdown(vsock)) {
		return E_HM_PIPE;
	}

	if (vsock_state(vsock) != SS_CONNECTED || !is_vm_addr_binded(&vsock->local_addr)) {
		return E_HM_NOTCONN;
	}

	ret = iov_iter_init(&iter, msg->msg_iov, (unsigned long)(unsigned int)(msg->msg_iovlen));
	if (ret != E_HM_OK) {
		return ret;
	}

	while (iov_iter_left(&iter) > 0) {
		ssize_t written;

		if (!ux_trans_has_space(vsock)) {
			ret = try_wait_space(vsock, (unsigned int)flags);
		}

		if (ret != E_HM_OK) {
			break;
		}

		if (vsock->sk.sk_err != E_HM_OK) {
			ret = vsock->sk.sk_err;
			break;
		}

		if (is_write_shutdown(vsock)) {
			ret = E_HM_PIPE;
			break;
		}

		written = do_send_data(vsock, &iter);
		if (written <= 0) {
			ret = (int)written;
			break;
		}

		iov_iter_forward(&iter, (size_t)written);
		vsock->trans_context.send_cnt += (unsigned int)written;
		total_written += written;
	}

	if (total_written > 0 && !ux_trans_has_space(vsock)) {
		vsock_clear_epoll_event(vsock, EPOLLOUT);
	}

	return total_written > 0 ? total_written : ret;
}

static ssize_t vsock_sendmsg_handler(ux_socket_t *sock, const struct msghdr *msg, int flags)
{
	ux_vsock_sock_t *vsock = vsock_from_socket(sock);
	int ret;

	if (vsock == NULL || msg == NULL) {
		ret = E_HM_INVAL;
	} else {
		lock_vsock(vsock);
		ret = (int)do_vsock_sendmsg(vsock, msg, flags);
		unlock_vsock(vsock);
	}

	return ret;
}

static ssize_t vsock_sendmsg(ux_socket_t *sock, const struct msghdr *msg, int flags)
{
	return socket_sendmsg_wrapper(sock, msg, flags, vsock_sendmsg_handler);
}

static int try_wait_data(ux_vsock_sock_t *vsock, unsigned int flags)
{
	int ret;

	if (((flags & MSG_DONTWAIT) != 0U) || is_vsock_nonblock(vsock)) {
		ret = E_HM_AGAIN;
	} else {
		ret = vsock_recv_wait(vsock, vsock->sk.rcvtimeo);
	}

	return ret;
}

static inline void vsock_pop_pkt(ux_vsock_sock_t *vsock, ux_vsock_buf_t *pkt)
{
	vsock->sk.rcv_buf_used -= (int)(pkt->len);
	dlist_delete(&pkt->node);
}

static ssize_t do_recv_data(ux_vsock_sock_t *vsock, struct iov_iter *iter)
{
	ux_vsock_buf_t *vbuf = NULL;
	bool need_update_credit = false;
	ssize_t copied = 0;

	while (iov_iter_left(iter) > 0 && is_vsock_data_ready(vsock)) {
		ssize_t curr_copied;
		size_t size_to_copy;
		size_t iov_left_size;
		vbuf = dlist_first_entry(&vsock->sk.rcv_queue, ux_vsock_buf_t, node);

		iov_left_size = iov_iter_left(iter);
		size_to_copy = min(iov_left_size, vbuf->len - vbuf->offset);
		curr_copied = copy_data_to_iter(iter, vbuf->ptr + vbuf->offset, size_to_copy);
		if (curr_copied < 0) {
			break;
		}

		copied += curr_copied;

		vbuf->offset += (size_t)curr_copied;
		if (vbuf->offset >= vbuf->len) {
			need_update_credit = true;
			vsock->trans_context.fwt_cnt += (uint32_t)(vbuf->len);
			vsock_pop_pkt(vsock, vbuf);

			free_vsock_buf(vbuf);
		}
	}

	if (need_update_credit) {
		if (ux_trans_update_credit(vsock) < 0) {
			net_warn(HMNET_VSOCK, "vsock update credit failed\n");
		}
	}

	return copied;
}

static ssize_t do_vsock_recvmsg(ux_vsock_sock_t *vsock, const struct msghdr *msg, int flags)
{
	struct iov_iter iter;
	ssize_t total_recv = 0;
	int ret = E_HM_OK;

	if (vsock_state(vsock) != SS_CONNECTED) {
		return E_HM_NOTCONN;
	}

	if ((((unsigned int)flags & MSG_OOB) != 0U) ||
	    (((unsigned int)flags & MSG_PEEK) != 0U)) {
		return E_HM_OPNOTSUPP;
	}

	if ((vsock->shutdown & SHUTDOWN_RD) != 0U) {
		return 0;
	}

	ret = iov_iter_init(&iter, msg->msg_iov, (unsigned long)(unsigned int)(msg->msg_iovlen));
	if (ret != E_HM_OK) {
		return ret;
	}

	while (iov_iter_left(&iter) > 0) {
		if (!is_vsock_data_ready(vsock)) {
			if (vsock->sk.sk_err != 0) {
				ret = vsock->sk.sk_err;
				break;
			}

			if (is_read_shutdown(vsock)) {
				ret = 0;
				break;
			}

			ret = try_wait_data(vsock, (unsigned int)flags);
		}

		if ((ret == E_HM_OK) && is_vsock_data_ready(vsock)) {
			ssize_t len = do_recv_data(vsock, &iter);
			if (len > 0) {
				total_recv += len;
			} else {
				ret = (int)len;
			}
		}

		if ((total_recv > 0) && (((unsigned int)flags & MSG_WAITALL) == 0)) {
			break;
		}

		if (ret != 0) {
			break;
		}
	}

	return total_recv > 0 ? total_recv : ret;
}

static ssize_t vsock_recvmsg_handler(ux_socket_t *sock, struct msghdr *msg, int flags)
{
	ux_vsock_sock_t *vsock = vsock_from_socket(sock);
	int ret;

	if (vsock == NULL || msg == NULL || !is_recv_flag_valid((unsigned int)flags)) {
		ret = E_HM_INVAL;
	} else {
		lock_vsock(vsock);
		ret = (int)do_vsock_recvmsg(vsock, msg, flags);
		if (ret > 0 && !is_vsock_data_ready(vsock)) {
			vsock_clear_epoll_event(vsock, EPOLLIN);
		}
		unlock_vsock(vsock);
	}

	return ret;
}

static ssize_t vsock_recvmsg(ux_socket_t *sock, struct msghdr *msg, int flags)
{
	return socket_recvmsg_wrapper(sock, msg, flags, vsock_recvmsg_handler);
}

static ssize_t vsock_send_handler(ux_socket_t *sock, const void *buf, size_t len, int flags)
{
	struct msghdr msg;
	struct iovec iov;

	NOFAIL(memset_s(&msg, sizeof(msg), 0, sizeof(msg)));
	NOFAIL(memset_s(&iov, sizeof(iov), 0, sizeof(iov)));
	iov.iov_base = (void *)buf;
	iov.iov_len = len;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;

	return vsock_sendmsg_handler(sock, &msg, flags);
}

static ssize_t vsock_send(ux_socket_t *sock, const void *buf, size_t len, int flags)
{
	return socket_send_wrapper(sock, buf, len, flags, vsock_send_handler);
}

static ssize_t vsock_sendto_handler(ux_socket_t *sock,
				    const void *buf, size_t len, int flags,
				    const struct sockaddr *daddr, socklen_t addrlen)
{
	struct msghdr msg;
	struct iovec iov;

	NOFAIL(memset_s(&msg, sizeof(msg), 0, sizeof(msg)));
	NOFAIL(memset_s(&iov, sizeof(iov), 0, sizeof(iov)));
	iov.iov_base = (void *)buf;
	iov.iov_len = len;
	msg.msg_iovlen = 1;
	msg.msg_iov = &iov;
	msg.msg_name = (struct sockaddr *)daddr;
	msg.msg_namelen = addrlen;

	return vsock_sendmsg_handler(sock, &msg, flags);
}

static ssize_t vsock_sendto(ux_socket_t *sock, const void *buf, size_t len, int flags,
			    const struct sockaddr *daddr, socklen_t addrlen)
{
	return socket_sendto_wrapper(sock, buf, len, flags, daddr, addrlen,
				     vsock_sendto_handler);
}

static ssize_t vsock_recvfrom_handler(ux_socket_t *sock, void *buf, size_t len, int flags,
				      struct sockaddr *saddr, socklen_t *addrlen)
{
	struct msghdr msg;
	struct iovec iov;

	NOFAIL(memset_s(&msg, sizeof(msg), 0, sizeof(msg)));
	NOFAIL(memset_s(&iov, sizeof(iov), 0, sizeof(iov)));
	iov.iov_base = (void *)buf;
	iov.iov_len = len;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	msg.msg_name = (struct sockaddr *)saddr;
	if (addrlen != NULL) {
		msg.msg_namelen = *addrlen;
	} else {
		msg.msg_namelen = 0;
	}

	return vsock_recvmsg_handler(sock, &msg, flags);
}

static ssize_t vsock_recvfrom(ux_socket_t *sock, void *buf, size_t len, int flags,
			      struct sockaddr *saddr, socklen_t *addrlen)
{
	return socket_recvfrom_wrapper(sock, buf, len, flags, saddr, addrlen,
				       vsock_recvfrom_handler);
}

static int vsock_getsockname(ux_socket_t *sock, struct sockaddr *addr,
			     socklen_t *addrlen)
{
	ux_vsock_sock_t *vsock = vsock_from_socket(sock);
	int ret = E_HM_OK;

	if (vsock == NULL || addr == NULL || addrlen == NULL) {
		ret = E_HM_INVAL;
	} else if (*addrlen < sizeof(vsock->local_addr)) {
		ret = E_HM_INVAL;
	} else {
		lock_vsock(vsock);
		copy_vm_addr((struct sockaddr_vm *)addr, &vsock->local_addr);
		*addrlen = sizeof(vsock->local_addr);
		unlock_vsock(vsock);
	}

	return ret;
}

static int vsock_getpeername(ux_socket_t *sock, struct sockaddr *addr,
			     socklen_t *addrlen)
{
	ux_vsock_sock_t *vsock = vsock_from_socket(sock);
	int ret = E_HM_OK;

	if (vsock == NULL || addr == NULL || addrlen == NULL ||
	    *addrlen < sizeof(struct sockaddr_vm)) {
		ret = E_HM_INVAL;
	} else {
		lock_vsock(vsock);

		if (vsock_state(vsock) != SS_CONNECTED) {
			ret = E_HM_NOTCONN;
		} else {
			copy_vm_addr((struct sockaddr_vm *)addr, &vsock->remote_addr);
			*addrlen = sizeof(vsock->remote_addr);
		}

		unlock_vsock(vsock);
	}

	return ret;
}

static int vsock_fcntl(ux_socket_t *sock, int cmd, int arg)
{
	ux_vsock_sock_t *vsock = vsock_from_socket(sock);
	unsigned int uarg = (unsigned int)arg;
	int ret = E_HM_OK;

	if (vsock == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		lock_vsock(vsock);

		switch (cmd) {
		case F_GETFL:
			ret = (int)vsock->sk.noblock;
			break;
		case F_SETFL:
			/* mask O_LARGEFILE */
			uarg &= ~O_LARGEFILE;
			if ((uarg & ~(O_NONBLOCK | O_RDWR)) == 0) {
				/* only O_NONBLOCK|O_RDWR, all other bits are zero */
				vsock->sk.noblock = uarg;
			}
			break;
		default:
			ret = E_HM_INVAL;
			break;
		}

		unlock_vsock(vsock);
	}

	return ret;
}

static int vsock_ioctl(ux_socket_t *sock, int cmd, uintptr_t arg)
{
	UNUSED(sock, cmd, arg);
	return E_HM_OPNOTSUPP;
}

static int copy_buffer_size_to_usr(uint32_t *optval, socklen_t *optlen, uint32_t value)
{
	socklen_t len;
	int ret;

	ret = hm_actv_read_caller_vm(&len, optlen, sizeof(socklen_t));
	if (ret == E_HM_OK) {
		if (len < sizeof(uint32_t)) {
			ret = E_HM_INVAL;
		}
	}

	if (ret == E_HM_OK) {
		ret = hm_actv_write_caller_vm(optval, &value, sizeof(uint32_t));
	}

	if (ret == E_HM_OK) {
		len = sizeof(uint32_t);
		ret = hm_actv_write_caller_vm(optlen, &len, sizeof(socklen_t));
	}

	return ret;
}

static int do_getsockopt(const ux_vsock_sock_t *vsock, int optname,
			 void *optval, socklen_t *optlen)
{
	int ret = E_HM_OK;

	switch (optname) {
	case VSOCK_OPT_BUFFER_SIZE: {
		uint32_t buffer_size = ux_trans_get_buffer_size(vsock);

		ret = copy_buffer_size_to_usr(optval, optlen, buffer_size);
		break;
	}
	case VSOCK_OPT_MIN_BUFFER_SIZE: {
		ret = copy_buffer_size_to_usr(optval, optlen, VSOCK_MIN_BUFFER_SIZE);
		break;
	}
	case VSOCK_OPT_MAX_BUFFER_SIZE: {
		ret = copy_buffer_size_to_usr(optval, optlen, VSOCK_MAX_BUFFER_SIZE);
		break;
	}
	default:
		ret = E_HM_NOPROTOOPT;
		break;
	}

	return ret;
}

static int vsock_getsockopt(ux_socket_t *sock, int level, int optname,
			    void *optval, socklen_t *optlen)
{
	ux_vsock_sock_t *vsock = vsock_from_socket(sock);
	int ret = E_HM_OK;

	UNUSED(level);

	if (vsock == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		lock_vsock(vsock);
		ret = do_getsockopt(vsock, optname, optval, optlen);
		unlock_vsock(vsock);
	}

	return ret;
}

static int do_setsockopt(ux_vsock_sock_t *vsock, int optname,
			 const void *optval, socklen_t optlen)
{
	int ret = E_HM_OK;

	switch (optname) {
	case VSOCK_OPT_BUFFER_SIZE: {
		uint32_t buffer_size;

		if (optlen < sizeof(uint32_t)) {
			ret = E_HM_INVAL;
			break;
		}

		ret = hm_actv_read_caller_vm(&buffer_size, optval,
					     sizeof(uint32_t));
		if (ret != E_HM_OK) {
			break;
		}

		if (buffer_size < VSOCK_MIN_BUFFER_SIZE ||
		    buffer_size > VSOCK_MAX_BUFFER_SIZE) {
			ret = E_HM_INVAL;
			break;
		}

		ux_trans_set_buffer_size(vsock, buffer_size);
		break;
	}
	default:
		ret = E_HM_NOPROTOOPT;
		break;
	}

	return ret;
}

static int vsock_setsockopt(ux_socket_t *sock, int level, int optname,
			    const void *optval, socklen_t optlen)
{
	ux_vsock_sock_t *vsock = vsock_from_socket(sock);
	int ret = E_HM_OK;

	UNUSED(level);

	if (vsock == NULL) {
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		lock_vsock(vsock);
		ret = do_setsockopt(vsock, optname, optval, optlen);
		unlock_vsock(vsock);
	}

	return ret;
}

static unsigned int vsock_get_error_event(const ux_vsock_sock_t *vsock)
{
	unsigned int ret = 0;

	if (vsock_state(vsock) == SS_UNCONNECTED || vsock_state(vsock) == SS_DISCONNECTING) {
		ret = EPOLLHUP;
	} else if (((vsock->shutdown & SHUTDOWN_RD) != 0U) ||
		   ((vsock->peer_shutdown & SHUTDOWN_WR) != 0U)) {
		ret = EPOLLIN;
	} else if ((vsock->shutdown == SHUTDOWN_MASK) || (vsock->peer_shutdown == SHUTDOWN_MASK)) {
		ret = EPOLLIN | EPOLLHUP;
	}

	return ret;
}

static int vsock_getsockevent(ux_socket_t *sock, unsigned int *rcvev,
			      unsigned int *sendev, unsigned int *errev, int *error)
{
	ux_vsock_sock_t *vsock = vsock_from_socket(sock);
	int ret = E_HM_OK;

	if (vsock == NULL) {
		ret = E_HM_INVAL;
	} else {
		if (rcvev != NULL) {
			*rcvev = is_vsock_data_ready(vsock) ? EPOLLIN : 0;
		}

		if (sendev != NULL) {
			*sendev = ux_trans_has_space(vsock) ? EPOLLOUT : 0;
		}

		if (errev != NULL) {
			*errev = vsock_get_error_event(vsock);
		}

		if (error != NULL) {
			*error = 0;
		}
	}

	return ret;
}

static int vsock_check_perm(ux_socket_t *sock, ux_socket_t *peer_sock)
{
	UNUSED(sock, peer_sock);
	return E_HM_OK;
}

static ssize_t vsock_writev(ux_socket_t *sock, const struct iovec *iov, int iovcnt, int flags)
{
	UNUSED(flags);
	return socket_writev_wrapper(sock, iov, iovcnt, vsock_sendmsg_handler);
}

static ssize_t vsock_readv(ux_socket_t *sock, const struct iovec *iov, int iovcnt)
{
	return socket_readv_wrapper(sock, iov, iovcnt, vsock_recvmsg_handler);
}

ux_vsock_sock_t *vsock_find_connected(const struct sockaddr_vm *local,
				      const struct sockaddr_vm *remote)
{
	ux_vsock_sock_t *vsock_iter = NULL;
	ux_vsock_sock_t *vsock_result = NULL;

	lock_connected_vsock_list();
	for_each_connected_vsock(vsock_iter) {
		if (local->svm_port == vsock_iter->local_addr.svm_port &&
		    is_vm_addr_equal(remote, &vsock_iter->remote_addr)) {
			vsock_result = vsock_iter;
			vsock_get(vsock_result);
			break;
		}
	}
	unlock_connected_vsock_list();

	return vsock_result;
}

ux_vsock_sock_t *vsock_find_binded(const struct sockaddr_vm *local)
{
	ux_vsock_sock_t *vsock_iter = NULL;
	ux_vsock_sock_t *vsock_result = NULL;

	lock_binded_vsock_list();
	for_each_binded_vsock(vsock_iter) {
		if (local->svm_port == vsock_iter->local_addr.svm_port) {
			vsock_result = vsock_iter;
			vsock_get(vsock_result);
			break;
		}
	}
	unlock_binded_vsock_list();

	return vsock_result;
}

static int vsock_recv_data_unconnected(ux_vsock_sock_t *vsock, struct vm_msg *msg)
{
	/* recv data when just before listen() and after bind, just reset the request */
	net_warn(HMNET_VSOCK, "unconnected vsock should not recv any data\n");
	return ux_trans_reset(vsock, &msg->hdr);
}

static int vsock_recv_data_connecting(ux_vsock_sock_t *vsock, struct vm_msg *msg)
{
	struct virtio_vsock_hdr *hdr = &(msg->hdr);
	int ret = E_HM_OK;

	if (hdr->op == (uint16_t)VIRTIO_VSOCK_OP_RESPONSE) {
		vsock->sk.sk_err = E_HM_OK;
		vsock_set_connected(vsock);
		add_vsock_to_connected_list(vsock);
		ux_sock_poll_common(&vsock->sk, EPOLLOUT);
	} else if (hdr->op == (uint16_t)VIRTIO_VSOCK_OP_RST) {
		vsock_set_unconnected(vsock);
		vsock->sk.sk_err = E_HM_CONNRESET;
		ux_sock_poll_common(&vsock->sk, EPOLLERR);
	} else {
		net_warn(HMNET_VSOCK, "invalid msg op = %u\n", (uint32_t)hdr->op);
		ret = E_HM_INVAL;
	}

	if (ret == E_HM_OK) {
		ux_sem_signal(&vsock->sk.recvsem);
	}

	return ret;
}

static void vsock_push_pkt(ux_sock_t *sk, ux_vsock_buf_t *pkt)
{
	dlist_insert_tail(&sk->rcv_queue, &pkt->node);
	sk->rcv_buf_used += (int)pkt->len;
	ux_sock_poll_common(sk, EPOLLIN);
}

static inline bool is_recv_buffer_full(const ux_vsock_sock_t *vsock, size_t extra)
{
	const size_t used = (size_t)(unsigned int)(vsock->sk.rcv_buf_used);
	return (uint32_t)(used + extra) > vsock->trans_context.buf_alloc;
}

static int vsock_recv_data_connected(ux_vsock_sock_t *vsock, struct vm_msg *msg)
{
	int ret = E_HM_OK;

	switch (msg->hdr.op) {
	case VIRTIO_VSOCK_OP_RW: {
		ux_vsock_buf_t *pkt = NULL;

		/**
		 * Check if our local buffer is used up, if so the peer should
		 * not send packet anymore, here we just drop this packet.
		 */
		if (is_recv_buffer_full(vsock, msg->len)) {
			net_warn(HMNET_VSOCK, "recv buffer full, peer should not send any data\n");
			ret = E_HM_INVAL;
			break;
		}

		pkt = alloc_vsock_buf(msg->data, msg->len);
		if (pkt == NULL) {
			ret = E_HM_NOMEM;
			break;
		}

		/*
		 * msg->data is move to pkt->ptr, and will be freed
		 * when pkt is all forward to user
		 */
		msg->data = NULL;
		vsock_push_pkt(&vsock->sk, pkt);
		ux_sem_signal(&vsock->sk.recvsem);
		break;
	}
	case VIRTIO_VSOCK_OP_SHUTDOWN: {
		if ((msg->hdr.flags & VIRTIO_VSOCK_SHUTDOWN_RCV) != 0U) {
			vsock->peer_shutdown |= SHUTDOWN_RD;
		}

		if ((msg->hdr.flags & VIRTIO_VSOCK_SHUTDOWN_SEND) != 0U) {
			vsock->peer_shutdown |= SHUTDOWN_WR;
		}

		if (vsock->peer_shutdown == SHUTDOWN_MASK &&
		    !is_vsock_data_ready(vsock)) {
			vsock_set_disconnecting(vsock);
		}

		vsock_wakeup_abnormal(vsock);
		break;
	}
	case VIRTIO_VSOCK_OP_RST: {
		vsock->peer_shutdown = SHUTDOWN_MASK;
		if (!is_vsock_data_ready(vsock)) {
			vsock_set_disconnecting(vsock);
		}

		vsock_wakeup_abnormal(vsock);
		remove_vsock_from_lists(vsock);
		break;
	}
	default:
		break;
	}

	return ret;
}

static int vsock_recv_data_listening(ux_vsock_sock_t *vsock, struct vm_msg *msg)
{
	struct virtio_vsock_hdr *hdr = &(msg->hdr);
	ux_vsock_sock_t *child = NULL;
	int ret = E_HM_OK;

	if (hdr->op != (uint16_t)VIRTIO_VSOCK_OP_REQUEST) {
		net_warn(HMNET_VSOCK, "invalid msg op = %u\n", (uint32_t)hdr->op);
		ret = E_HM_INVAL;
	} else if (!allow_new_child(vsock)) {
		ret = E_HM_NOMEM;
	} else {
		child = vsock_alloc_and_init(NULL, vsock->sk.socket_domain, vsock->sk.type, vsock->sk.socket_protocol);
		if (child == NULL) {
			ret = E_HM_NOMEM;
		}
	}

	if (ret == E_HM_OK) {
		int err;

		vsock_set_connected(child);

		make_vm_addr(&child->local_addr, vsock_local_cid(), vsock->local_addr.svm_port);
		make_vm_addr(&child->remote_addr, (unsigned int)(hdr->src_cid), hdr->src_port);

		vsock_push_child(vsock, child);
		add_vsock_to_connected_list(child);

		update_trans_info(child, hdr);
		err = ux_trans_response(child, hdr);
		if (err != E_HM_OK) {
			hm_error("trans response failed\n");
		}

		/**
		 * wakeup waiting parent, we reuse the sk.recvsem
		 * when doing accept and recv
		 */
		ux_sem_signal(&vsock->sk.recvsem);
	} else {
		(void)ux_trans_reset(vsock, hdr);
	}

	return ret;
}

static int vsock_recv_data_disconnecting(ux_vsock_sock_t *vsock, struct vm_msg *msg)
{
	struct virtio_vsock_hdr *hdr = &(msg->hdr);

	if (hdr->op == (uint16_t)VIRTIO_VSOCK_OP_RST) {
		vsock_wakeup_abnormal(vsock);
		remove_vsock_from_lists(vsock);
	}

	return E_HM_OK;
}

int vsock_recv_msg(ux_vsock_sock_t *vsock, struct vm_msg *msg)
{
	int ret;

	lock_vsock(vsock);

	BUG_ON(vsock->pstate == NULL);
	BUG_ON(vsock->pstate->recv_msg == NULL);

	ret = vsock->pstate->recv_msg(vsock, msg);

	unlock_vsock(vsock);

	return ret;
}

DEFINE_VSOCK_STATE(unconnected, SS_UNCONNECTED,
		   vsock_connect_normal, vsock_recv_data_unconnected)

DEFINE_VSOCK_STATE(connecting, SS_CONNECTING,
		   vsock_connect_already, vsock_recv_data_connecting)

DEFINE_VSOCK_STATE(connected, SS_CONNECTED,
		   vsock_connect_isconn, vsock_recv_data_connected)

DEFINE_VSOCK_STATE(listening, SS_LISTENING,
		   vsock_connect_invalid, vsock_recv_data_listening)

DEFINE_VSOCK_STATE(disconnecting, SS_DISCONNECTING,
		   vsock_connect_invalid, vsock_recv_data_disconnecting)

static struct ux_proto_ops vsock_stream_ops = {
	.connect = vsock_connect,
	.bind = vsock_bind,
	.listen = vsock_listen,
	.accept = vsock_accept,
	.shutdown = vsock_shutdown,
	.close = vsock_close,

	.sendmmsg = NULL,
	.recvmmsg = NULL,
	.sendmsg = vsock_sendmsg,
	.recvmsg = vsock_recvmsg,

	.send = vsock_send,

	.sendto = vsock_sendto,
	.recvfrom = vsock_recvfrom,

	.getsockname = vsock_getsockname,
	.getpeername = vsock_getpeername,

	.fcntl = vsock_fcntl,
	.ioctl = vsock_ioctl,

	.getsockopt = vsock_getsockopt,
	.setsockopt = vsock_setsockopt,

	.getsockevent = vsock_getsockevent,
	.check_perm = vsock_check_perm,

	.writev = vsock_writev,
	.readv = vsock_readv,

#ifdef CONFIG_LSYSCALL_ENABLED
	.epoll_recheck_revents = NULL,
#endif  /* !CONFIG_LSYSCALL_ENABLED */

	.fd_config = NULL,
	.sync_ctrlmem_polllist = NULL,
};

int ux_vsock_stream_init(ux_socket_t *s, ux_vsock_sock_t *sock,
			 int type, int protocol)
{
	UNUSED(type, protocol);
	s->ops = &vsock_stream_ops;
	vsock_set_unconnected(sock);
	return E_HM_OK;
}

void vsock_handle_cid_revoke(unsigned int cid)
{
	ux_vsock_sock_t *iter = NULL;

	lock_connected_vsock_list();
	for_each_connected_vsock(iter) {
		if (iter->remote_addr.svm_cid == cid) {
			iter->sk.sk_err = E_HM_NODEV;
			vsock_wakeup_abnormal(iter);
		}
	}
	unlock_connected_vsock_list();
}

static int __dump_stream_vsock(const ux_vsock_sock_t *vsock, char *buf, size_t count)
{
	int n = 0;

	n = sprintf_s(buf, count, "%08X:%08X\t%08X:%08X\t%s\t%s\t%08d\t%08u\t%08u\t%08u\t%08u\t%08u\n",
		      vsock->local_addr.svm_cid, vsock->local_addr.svm_port,
		      vsock->remote_addr.svm_cid, vsock->remote_addr.svm_port,
		      "stream", vsock_state_name(vsock), vsock->sk.rcv_buf_used,
		      vsock->trans_context.buf_alloc, vsock->trans_context.fwt_cnt,
		      vsock->trans_context.send_cnt, vsock->trans_context.peer_buf_alloc,
		      vsock->trans_context.peer_fwt_cnt);
	if (n < 0) {
		return E_HM_POSIX_FAULT;
	}

	return n;
}

int ux_vsock_stream_stat(char *buf, size_t count)
{
	int ret;
	int used = 0;
	ux_vsock_sock_t *vs = NULL;

	ret = sprintf_s(buf, count, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n",
			"Local Addr", "Remote Addr", "Type", "State", "Recv Q",
			"Local Buff", "Local Fwd", "Local Send", "Peer Buff", "Peer Fwd");
	if (ret < 0) {
		return E_HM_POSIX_FAULT;
	}

	used += ret;
	/* NOTE: non-binded sockets not concerned now */
	lock_binded_vsock_list();
	for_each_binded_vsock(vs) {
		ret = __dump_stream_vsock(vs, buf + used, count - (size_t)(unsigned int)used);
		if (ret < 0) {
			break;
		}
		used += ret;
	}
	unlock_binded_vsock_list();

	return (ret >= 0) ? used : ret;
}
