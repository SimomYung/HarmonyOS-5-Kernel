/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Netlink table operations
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan  3 17:10:16 2019
 */

#include "ux_netlink_proto.h"
#include "ux_netlink_table.h"

static ux_netlink_table_t netlink_tables[NETLINK_PROTO_MAX];

static ux_netlink_sock_t *__ux_netlink_lookup(const ux_netlink_table_t *table,
					      uint32_t port)
{
	ux_netlink_sock_t *nsk = NULL;

	dlist_for_each_entry(nsk, &table->head, ux_netlink_sock_t, head) {
		if (nsk->port == port) {
			return nsk;
		}
	}

	return NULL;
}

int ux_netlink_table_insert(ux_netlink_sock_t *nsk, uint32_t port)
{
	ux_netlink_table_t *table = NULL;
	ux_netlink_sock_t *tmp = NULL;
	int ret = 0;

	table = &netlink_tables[nsk->proto->protocol];

	(void)raw_rwlock_wrlock(&table->lock);
	ux_sock_lock(&nsk->sk);

	if (nsk->bound == 0) {
		tmp = __ux_netlink_lookup(table, port);
		if (tmp != NULL) {
			ret = E_HM_POSIX_EXIST;
		} else {
			dlist_insert_tail(&table->head, &nsk->head);
			nsk->port = port;
			nsk->bound = 1;
		}
	} else if (nsk->port != port) {
		ret = E_HM_BUSY;
	}

	ux_sock_unlock(&nsk->sk);
	(void)raw_rwlock_unlock(&table->lock);
	return ret;
}

void ux_netlink_table_remove(ux_netlink_sock_t *nsk)
{
	ux_netlink_table_t *table = &netlink_tables[nsk->proto->protocol];

	(void)raw_rwlock_wrlock(&table->lock);
	dlist_delete(&nsk->head);
	(void)raw_rwlock_unlock(&table->lock);
}

ux_netlink_sock_t *ux_netlink_table_lookup(int proto, uint32_t port)
{
	ux_netlink_table_t *table = &netlink_tables[proto];
	ux_netlink_sock_t *nsk = NULL;

	(void)raw_rwlock_rdlock(&table->lock);
	nsk = __ux_netlink_lookup(table, port);
	if (nsk != NULL) {
		ux_sock_get(&nsk->sk);
	}
	(void)raw_rwlock_unlock(&table->lock);

	return nsk;
}

int ux_netlink_table_init(void)
{
	ux_netlink_table_t *table = NULL;
	int i;

	for (i = 0; i < NETLINK_PROTO_MAX; i++) {
		table = &netlink_tables[i];

		dlist_init(&table->head);
		raw_rwlock_init(&table->lock, NULL);
	}

	return 0;
}

void ux_netlink_table_exit(void)
{
}

int ux_netlink_unicast(int proto, unsigned int dst_port,
		       const char *msg, const size_t len, bool nonblock)
{
	ux_netlink_sock_t *nsk = NULL;
	ux_pktbuf_t *pktbuf = NULL;
	char *ptr = NULL;
	int ret = E_HM_OK;

	if (msg == NULL || len == 0 || len > NETLINK_PAYLOAD_MAX) {
		return E_HM_INVAL;
	}

	if (proto < 0 || proto >= NETLINK_PROTO_MAX) {
		return E_HM_INVAL;
	}

	pktbuf = ux_pktbuf_alloc(len);
	if (pktbuf == NULL) {
		return E_HM_NOMEM;
	}

	ptr = ux_pktbuf_push(pktbuf, (unsigned int)len);
	if (ptr == NULL) {
		ux_pktbuf_free(pktbuf);
		return E_HM_NOMEM;
	}

	ret = memcpy_s(ptr, len, msg, len);
	if (ret != 0) {
		ux_pktbuf_free(pktbuf);
		return E_HM_POSIX_FAULT;
	}

	nsk = ux_netlink_table_lookup(proto, dst_port);
	if (nsk == NULL) {
		ux_pktbuf_free(pktbuf);
		return E_HM_INVAL;
	}
	ret = ux_netlink_recv_data(&nsk->sk, pktbuf, nonblock);
	if (ret != 0) {
		ux_sock_put(&nsk->sk);
		ux_pktbuf_free(pktbuf);
		return ret;
	}
	ux_sem_signal(&nsk->sk.recvsem);
	ux_sock_put(&nsk->sk);

	return ret;
}

static int do_netlink_broadcast(int proto, unsigned int dst_group, ux_pktbuf_t *pktbuf)
{
	ux_netlink_sock_t *nsk = NULL;
	ux_netlink_table_t *table = NULL;
	ux_pktbuf_t *new_buff = NULL;
	int ret = E_HM_OK;

	table = &netlink_tables[proto];
	(void)raw_rwlock_rdlock(&table->lock);
	dlist_for_each_entry(nsk, &table->head, ux_netlink_sock_t, head) {
		if ((nsk->group & (1U << (dst_group - 1))) == 0) {
			continue;
		}

		new_buff = ux_pktbuf_clone(pktbuf);
		if (new_buff == NULL) {
			net_debug(HMNET_NETLINK, "No memory when receive uevent cloning pkt\n");
			continue;
		}

		ret = ux_netlink_recv_data(&nsk->sk, new_buff, false);
		if (ret != 0) {
			ux_pktbuf_free(new_buff);
			(void)raw_rwlock_unlock(&table->lock);
			return ret;
		}
		ux_sem_signal(&nsk->sk.recvsem);
	}
	(void)raw_rwlock_unlock(&table->lock);

	return ret;
}

int ux_netlink_broadcast(int proto, unsigned int dst_group,
			 const char *msg, const size_t len)
{
	ux_pktbuf_t *pktbuf = NULL;
	char *ptr = NULL;
	int ret = E_HM_OK;

	if (msg == NULL || len == 0) {
		return E_HM_INVAL;
	}

	if (proto < 0 || proto >= NETLINK_PROTO_MAX) {
		return E_HM_INVAL;
	}

	pktbuf = ux_pktbuf_alloc(len);
	if (pktbuf == NULL) {
		return E_HM_NOMEM;
	}

	ptr = ux_pktbuf_push(pktbuf, (unsigned int)len);
	if (ptr == NULL) {
		ux_pktbuf_free(pktbuf);
		net_error(HMNET_NETLINK, "uevent push pkt buff failed\n");
		return E_HM_NOMEM;
	}

	ret = memcpy_s(ptr, len, msg, len);
	if (ret != 0) {
		net_warn(HMNET_NETLINK, "copy uevent message to pkt buffer failed err = %d\n", ret);
		ux_pktbuf_free(pktbuf);
		return E_HM_POSIX_FAULT;
	}

	ret = do_netlink_broadcast(proto, dst_group, pktbuf);
	ux_pktbuf_free(pktbuf);

	return ret;
}
