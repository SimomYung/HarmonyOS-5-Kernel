/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: interface for network vsock socket
 * Author: Huawei OS Kernel Lab
 * Create: Mon May 24 19:35:43 2021
 */

#include <stdlib.h>

#include "ux_socket.h"
#include "ux_domain.h"

#include "ux_vsock_socket.h"
#include "ux_vsock_host.h"

#include "security/sec_hook.h"
#include "ux_vsock_trans.h"
#include "ux_vsock_core.h"
#include "ux_revoke_handler.h"
#include "vsock/ux_vsock_netdev.h"
#include "vsock/ux_vsock_actv_netdev.h"

#include <hmnet/hm_vsock.h>

static void __clear_vsock_pkts(const ux_vsock_sock_t *vsock)
{
	ux_vsock_buf_t *curr = NULL;
	ux_vsock_buf_t *next = NULL;

	dlist_for_each_entry_safe(curr, next, &vsock->sk.rcv_queue,
				  ux_vsock_buf_t, node) {
		dlist_delete(&curr->node);
		free_vsock_buf(curr);
	}
}

static void __clear_pending_childs(ux_vsock_sock_t *vsock)
{
	ux_vsock_sock_t *child = NULL;

	while ((child = vsock_pop_child(vsock)) != NULL) {
		(void)ux_vsock_close(child);
	}
}

static void vsock_release(struct ux_sock *sk)
{
	ux_vsock_sock_t *vsock = container_of(sk, ux_vsock_sock_t, sk);
	lock_vsock(vsock);
	__clear_vsock_pkts(vsock);
	__clear_pending_childs(vsock);
	unlock_vsock(vsock);
	free(vsock);
}

static void vsock_set_write_space(ux_sock_t *sk, int sndbuf)
{
	UNUSED(sndbuf);

	if (sk != NULL) {
		ux_sem_signal(&sk->sendsem);
		ux_sock_poll_common(sk, EPOLLOUT);
	}
}

static inline ux_vsock_sock_t *vsock_sk(ux_sock_t *sk)
{
	return (ux_vsock_sock_t *)(void *)sk;
}

ux_vsock_sock_t *vsock_alloc_and_init(ux_socket_t *sock, int domain, int type, int protocol)
{
	ux_vsock_sock_t *vsock = vsock_sk(ux_sock_alloc(sock, sizeof(ux_vsock_sock_t), 0));

	if (vsock != NULL) {
		ux_sock_init(&vsock->sk, NULL, domain, type, protocol);

		dlist_init(&vsock->node_for_binded);
		dlist_init(&vsock->node_for_connected);

		dlist_init(&vsock->node_for_child);
		dlist_init(&vsock->pending_child);

		make_vm_addr(&vsock->local_addr, VMADDR_CID_ANY, VMADDR_PORT_ANY);
		make_vm_addr(&vsock->remote_addr, VMADDR_CID_ANY, VMADDR_PORT_ANY);

		vsock->connect_timeout = VSOCK_DEFAULT_CONNECT_TIMEO;
		vsock->connect_timer_id = VSOCK_INVALID_TIMER_ID;

		vsock->shutdown_timer_id = VSOCK_INVALID_TIMER_ID;

		ux_trans_alloc(vsock);
		vsock->sk.release = vsock_release;
		vsock->sk.set_write_space = vsock_set_write_space;
	}

	return vsock;
}

static void __release_vsock_buf(struct ux_vsock_buf *vbuf)
{
	if (vbuf != NULL) {
		free(vbuf->ptr);
		free(vbuf);
	}
}

ux_vsock_buf_t *alloc_vsock_buf(const char *buf, size_t len)
{
	ux_vsock_buf_t *vbuf = NULL;
	size_t size = sizeof(ux_vsock_buf_t);

	vbuf = malloc(size);
	if (vbuf != NULL) {
		dlist_init(&vbuf->node);
		vbuf->ptr = (char *)buf;
		vbuf->len = len;
		vbuf->offset = 0;
		vbuf->release = __release_vsock_buf;
	}

	return vbuf;
}

void free_vsock_buf(ux_vsock_buf_t *buf)
{
	if (buf->release != NULL) {
		buf->release(buf);
	}
}

void install_vsock_to_socket(ux_socket_t *sock, ux_vsock_sock_t *vsock)
{
	ux_socket_set_sock(sock, &vsock->sk);
}

static int do_vsock_create(ux_socket_t *sock, int domain, int type, int protocol, unsigned int flags)
{
	int ret = E_HM_OK;
	ux_vsock_sock_t *vsock = NULL;

	vsock = vsock_alloc_and_init(sock, domain, type, protocol);
	if (vsock == NULL) {
		ret = E_HM_NOMEM;
	}

	if (ret == E_HM_OK) {
		switch (type) {
		case SOCK_STREAM:
			ret = ux_vsock_stream_init(sock, vsock, type, protocol);
			break;
		default:
			ret = E_HM_SOCKTNOSUPPORT;
			break;
		}
	}

	if (ret == E_HM_OK) {
		install_vsock_to_socket(sock, vsock);

		if ((flags & SOCK_NONBLOCK) != 0) {
			(void)socket_fcntl(sock, F_SETFL, O_NONBLOCK);
		}
	} else {
		if (vsock != NULL) {
			free(vsock);
		}
	}

	return ret;
}

static int ux_vsock_create(ux_socket_t *sock, int domain, int type, int protocol, ux_namespace_t *ns)
{
	UNUSED(ns);
	int ret = E_HM_OK;

	unsigned int flags = (unsigned int)type & ~SOCK_TYPE_MASK;
	int masked_type = (int)((unsigned int)type & SOCK_TYPE_MASK);

	if (sock == NULL) {
		return E_HM_INVAL;
	}

	ret = sec_chk_div_vsock_create();
	if (ret != E_HM_OK) {
		return ret;
	}

	switch (protocol) {
#ifdef CONFIG_VSOCK_HOST
	case PF_VSOCK_HOST:
		ret = ux_host_vsock_create(sock, domain, masked_type, protocol, flags);
		break;
#endif
	case PF_VSOCK:
	case PF_VSOCK_DEFAULT:
		ret = do_vsock_create(sock, domain, masked_type, protocol, flags);
		break;
	default:
		ret = E_HM_PROTONOSUPPORT;
	}

	return ret;
}

static ux_domain_t vsock_domain = {
	.family = AF_VSOCK,
	.create = ux_vsock_create,
};

int af_vsock_init(void)
{
	int ret;

	ret = ux_domain_register(&vsock_domain);
	if (ret != E_HM_OK) {
		net_warn(HMNET_VSOCK, "register af_vsock domain failed\n");
		return ret;
	}

	ret = vsock_netdev_init(vsock_handle_cid_revoke);
	if (ret != E_HM_OK) {
		net_warn(HMNET_VSOCK, "vsock netdev init failed\n");
		return ret;
	}

	ret = ux_register_revoke_handler("vsock", vsock_actv_netdev_revoke);
	if (ret != E_HM_OK) {
		net_warn(HMNET_VSOCK, "vsock revoke handler init failed\n");
		return ret;
	}

	return 0;
}
