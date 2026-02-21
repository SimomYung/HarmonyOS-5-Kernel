/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: interface for network sock
 * Author: Huawei OS Kernel Lab
 * Create: Wed Oct  9 11:50:28 2019
 */

#include "security/sec_hook.h"
#include "ux_socket.h"
#include "ux_eventpoll.h"
#include "ux_sockopt.h"
#include "bpf/ux_bpf_filter.h"

static void ux_sock_release(ux_sock_t *sk)
{
#ifdef CONFIG_UXNET_BPF
	ux_detach_filter(sk);
#endif
	ux_sock_ns_detach(sk);

	int ret = sec_hook_div_sk_free_security(sk);
	if (ret != E_HM_OK) {
		net_warn(HMNET_CORE, "sec_hook_sk_free_security failed, ret:%s\n", hmstrerror(ret));
	}

	ux_clear_eventpoll_list(sk);
	if (sk->release != NULL) {
		sk->release(sk);
	}
}

ux_sock_t *ux_sock_alloc(ux_socket_t *sock, size_t size, int use_polllist)
{
	int rc = E_HM_OK;
	ux_sock_t *sk = NULL;

	sk = calloc(1, size);
	if (sk == NULL) {
		net_warn(HMNET_CORE, "ux_sock_alloc failed\n");
		rc = E_HM_NOMEM;
	}

	if (rc == E_HM_OK) {
		rc = sec_hook_div_sk_alloc_security(sk);
		if (rc != E_HM_OK) {
			net_warn(HMNET_CORE, "sec_hook_sk_alloc_security failed, ret:%s\n", hmstrerror(rc));
			free(sk);
			sk = NULL;
		}
	}

	if (rc == E_HM_OK && sock != NULL) {
		ux_sock_set_socket(sk, sock);
		rc = ux_spawn_polllist(sock, &sk->epoll_ctx, use_polllist);
		if (rc < 0) {
			net_error(HMNET_CORE, "accquire kobjpool_spawn_polllist failed\n");
			sec_hook_div_sk_free_security(sk);
			free(sk);
			sk = NULL;
		}
	}

	return sk;
}

void ux_sock_init(ux_sock_t *sk, ux_namespace_t *ns, int domain, int type, int protocol)
{
	raw_refcnt_nr_init(&sk->refcnt, 1);
	raw_mutex_init(&sk->lock);
	dlist_init(&sk->rcv_queue);
	(void)ux_sem_init(&sk->recvsem, 0);
	(void)ux_sem_init(&sk->sendsem, 0);
	ux_wq_head_init(&sk->recv_wq);
	ux_wq_head_init(&sk->send_wq);

	raw_mutex_init(&sk->epoll_ctx.epi_mutex);

	sk->rcv_buf_used = 0;
	sk->noblock = O_RDWR;
	sk->release = NULL;
	sk->rcvbuf = UX_SOCK_DEFAULT_RCVBUF;
	sk->sndbuf = UX_SOCK_DEFAULT_SNDBUF;
	sk->rcvtimeo = LONG_MAX;
	sk->sndtimeo = LONG_MAX;
	sk->flags = 0;
	sk->type = type;
	sk->lingertime = 0;
	sk->socket_domain = domain;
	sk->socket_protocol = protocol;

	ux_sock_ns_atttach(sk, ns);
}

void ux_sock_get(ux_sock_t *sk)
{
	int ret;

	ret = raw_refcnt_nr_get(&sk->refcnt);
	BUG_ON(ret <= 0, "sk refcnt fail, refcnt %d, ret %d(%s)\n",
	       raw_refcnt_nr_counter(&sk->refcnt), ret, hmstrerror(ret));
}

void ux_sock_put(ux_sock_t *sk)
{
	BUG_ON(raw_refcnt_nr_counter(&sk->refcnt) <= 0);

	if (raw_refcnt_nr_put(&sk->refcnt) == 0) {
		ux_sock_release(sk);
	}
}

void ux_sock_lock(ux_sock_t *sk)
{
	raw_mutex_lock(&sk->lock);
}

void ux_sock_unlock(ux_sock_t *sk)
{
	raw_mutex_unlock(&sk->lock);
}

void ux_sock_poll_common(ux_sock_t *sk, uint32_t event)
{
	ux_epoll_wakeup_clean(&sk->epoll_ctx, event);
}

/*
 * Please ensure that sk lock is held already.
 */
void ux_sock_recv_data(ux_sock_t *sk, ux_pktbuf_t *pkt)
{
	dlist_insert_tail(&sk->rcv_queue, &pkt->head);
	sk->rcv_buf_used += (int)pkt->len;
	ux_sock_poll_common(sk, EPOLLIN);
}
