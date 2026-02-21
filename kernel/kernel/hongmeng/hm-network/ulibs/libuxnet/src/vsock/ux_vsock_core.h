/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description:  Header file for network vsock core
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 24 19:35:43 2021
 */

#ifndef __UX_VSOCK_CORE_H_
#define __UX_VSOCK_CORE_H_

#include <stdlib.h>
#include <lib/dlist.h>
#include <lib/dlist_ext.h>

#include <hm_vsock.h>
#include "ux_socket.h"
#include "ux_eventpoll.h"
#include "ux_vsock_template.h"
#include "vsock/ux_vsock_common.h"

#define	VSOCK_BUFFER_SIZE		(1024U * 256)
#define VSOCK_MIN_BUFFER_SIZE		(128U)
#define VSOCK_MAX_BUFFER_SIZE		(1024U * 256)
#define VSOCK_MAX_BUF_SIZE_LIMIT	0xFFFFFFFFUL

#define VSOCK_DEFAULT_CONNECT_TIMEO	2000 /* ms */
#define VSOCK_DEFAULT_CLOSE_TIMEO	4000 /* ms */

/**
 * Context info for vsock transport layer
 */
struct vsock_trans {
	uint32_t peer_buf_alloc;    /* peer recv buffer size in bytes */
	uint32_t peer_fwt_cnt;      /* how many bytes peer has forward to uapp */
	uint32_t send_cnt;          /* how many bytes we have send to peer */

	uint32_t buf_alloc;         /* our buffer size */
	uint32_t fwt_cnt;           /* how many bytes we have forwarded to uapp */
};

struct vsock_state;

typedef struct ux_vsock_sock {
	ux_sock_t sk;

	struct dlist_node node_for_binded;
	struct dlist_node node_for_connected;
	struct dlist_node node_for_child;

	int backlog;
	struct dlist_node pending_child;
	unsigned int pending_child_count;

	struct vsock_state *pstate;

	struct sockaddr_vm local_addr;
	struct sockaddr_vm remote_addr;

	long connect_timeout;     /* in ms */
	int connect_timer_id;

	int shutdown_timer_id;
	uint32_t shutdown;        /* shutdown flags we have send */
	uint32_t peer_shutdown;   /* shutdown flags peer send to us */

	struct vsock_trans trans_context;

	uint64_t max_buffer_size; /* upper limit of buffer size */
	uint64_t min_buffer_size; /* lower limit of buffer size */
} ux_vsock_sock_t;

struct vsock_state {
	char *state_name;    /* name of current state, mainly for debug */
	int state;

	int (*connect)(ux_vsock_sock_t *vsock, const struct sockaddr_vm *addr);
	int (*recv_msg)(struct ux_vsock_sock *vsock, struct vm_msg *msg);
};

/**
 * A vsock buffer to avoid multiple copy.
 */
typedef struct ux_vsock_buf {
	struct dlist_node node;

	char *ptr;
	size_t len;
	size_t offset;		 /* current read offset */

	void (*release)(struct ux_vsock_buf *vbuf);
} ux_vsock_buf_t;

static inline void vsock_get(ux_vsock_sock_t *vsock)
{
	ux_sock_get(&vsock->sk);
}

static inline void vsock_put(ux_vsock_sock_t *vsock)
{
	ux_sock_put(&vsock->sk);
}

static inline ux_vsock_sock_t *vsock_from_socket(ux_socket_t *s)
{
	return (ux_vsock_sock_t *)(void *)ux_socket_get_sock(s);
}

static inline void lock_vsock(ux_vsock_sock_t *vsock)
{
	raw_mutex_lock(&vsock->sk.lock);
}

static inline void unlock_vsock(ux_vsock_sock_t *vsock)
{
	raw_mutex_unlock(&vsock->sk.lock);
}

static inline bool is_vsock_data_ready(const ux_vsock_sock_t *vsock)
{
	return (dlist_empty(&vsock->sk.rcv_queue) == __false) ||
	       (dlist_empty(&vsock->pending_child) == __false);
}

static inline bool is_vsock_nonblock(const ux_vsock_sock_t *vsock)
{
	return ((vsock->sk.noblock & O_NONBLOCK) != 0);
}

static inline bool is_read_shutdown(const ux_vsock_sock_t *vsock)
{
	return ((vsock->shutdown & SHUTDOWN_RD) != 0U) ||
	       ((vsock->peer_shutdown & SHUTDOWN_WR) != 0U);
}

static inline bool is_write_shutdown(const ux_vsock_sock_t *vsock)
{
	return ((vsock->shutdown & SHUTDOWN_WR) != 0U) ||
	       ((vsock->peer_shutdown & SHUTDOWN_RD) != 0U);
}

static inline void vsock_clear_epoll_event(ux_vsock_sock_t *vsock, uint32_t events)
{
	ux_sock_poll_common(&vsock->sk, events << CLEAN_EVENT_SHIFT);
}

/**
 * HM is always the host
 */
static inline unsigned int vsock_local_cid(void)
{
	return VMADDR_CID_HOST;
}

/**
 * 0,1,2 are reserved cids, and can not be used by guest
 */
static inline bool vsock_is_cid_valid(unsigned int cid)
{
	return cid > VMADDR_CID_HOST;
}

static inline void make_vm_addr(struct sockaddr_vm *addr,
				unsigned int cid, unsigned int port)
{
	mem_zero_s(*addr);

	addr->svm_family = AF_VSOCK;
	addr->svm_cid = cid;
	addr->svm_port = port;
}

static inline void copy_vm_addr(struct sockaddr_vm *to, const struct sockaddr_vm *from)
{
	to->svm_family = from->svm_family;
	to->svm_cid = from->svm_cid;
	to->svm_port = from->svm_port;
}

static inline bool is_vm_addr_equal(const struct sockaddr_vm *a, const struct sockaddr_vm *b)
{
	return (a->svm_family == b->svm_family) &&
	       (a->svm_cid == b->svm_cid) &&
	       (a->svm_port == b->svm_port);
}

static inline bool allow_new_child(const ux_vsock_sock_t *vsock)
{
	return (unsigned int)vsock->backlog >= vsock->pending_child_count;
}

static inline int vsock_state(const ux_vsock_sock_t *vsock)
{
	BUG_ON(vsock == NULL);
	BUG_ON(vsock->pstate == NULL);
	return vsock->pstate->state;
}

static inline char *vsock_state_name(const ux_vsock_sock_t *vsock)
{
	BUG_ON(vsock == NULL);
	BUG_ON(vsock->pstate == NULL);
	return vsock->pstate->state_name;
}

static inline void vsock_push_child(ux_vsock_sock_t *vsock, ux_vsock_sock_t *child)
{
	vsock_get(child);
	dlist_insert(&vsock->pending_child, &child->node_for_child);
	++vsock->pending_child_count;
	ux_sock_poll_common(&vsock->sk, EPOLLIN);
}

static inline ux_vsock_sock_t *vsock_pop_child(ux_vsock_sock_t *vsock)
{
	ux_vsock_sock_t *child = NULL;
	if (vsock != NULL && !dlist_empty(&vsock->pending_child)) {
		child = container_of(dlist_pop_first(&vsock->pending_child), ux_vsock_sock_t, node_for_child);

		vsock_put(child);
		--vsock->pending_child_count;
		if (vsock->pending_child_count == 0) {
			vsock_clear_epoll_event(vsock, EPOLLIN);
		}
	}

	return child;
}

static inline uint64_t ux_vsock_get_max_buffer_size(const ux_vsock_sock_t *vsock)
{
	return vsock->max_buffer_size;
}

static inline uint64_t ux_vsock_get_min_buffer_size(const ux_vsock_sock_t *vsock)
{
	return vsock->min_buffer_size;
}

int ux_vsock_stream_init(ux_socket_t *s, ux_vsock_sock_t *sock,
			 int type, int protocol);
int ux_vsock_close(ux_vsock_sock_t *vsock);
void remove_vsock_from_lists(ux_vsock_sock_t *vsock);

ux_vsock_sock_t *vsock_find_connected(const struct sockaddr_vm *local,
				      const struct sockaddr_vm *remote);
ux_vsock_sock_t *vsock_find_binded(const struct sockaddr_vm *local);

int vsock_recv_msg(ux_vsock_sock_t *vsock, struct vm_msg *msg);
ssize_t vsock_vm_send_msg(unsigned int cid, struct virtio_vsock_hdr *hdr,
			  const char *buf, size_t len);
ssize_t vsock_send_msg(unsigned int cid, const struct virtio_vsock_hdr *hdr,
		       const char *buf, size_t len);

#define VSOCK_INVALID_TIMER_ID		(-1)
typedef void (*vsock_timer_handler_t)(void *arg);
int vsock_add_timer(int interval_ms, vsock_timer_handler_t handler, void *arg);
int vsock_cancel_timer(int timer_id);

DECLARE_VSOCK_STATE(unconnected);
DECLARE_VSOCK_STATE(connecting);
DECLARE_VSOCK_STATE(connected);
DECLARE_VSOCK_STATE(listening);
DECLARE_VSOCK_STATE(disconnecting);

#endif
