/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description:  Header file for network packet
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 30 09:03:44 2019
 */

#ifndef __HM_NET_UX_PACKET_H
#define __HM_NET_UX_PACKET_H
#include <libhmsync/raw_mutex_guard.h>
#include <netinet/if_ether.h>
#include "ux_socket.h"
#include "ux_netdev.h"
#include "ux_buf_queue.h"
#include "ux_utils.h"


#define SSL_ADDR_LEN 8

struct ux_packet_stats {
	uint32_t packets;
	uint32_t drops;
};

typedef struct ux_packet_mc {
	struct dlist_node node;
	int ifindex;
	int count;
	unsigned short type;
	unsigned short alen;
	unsigned char addr[32];
} ux_packet_mc_t;

typedef struct ux_packet_mclist {
	struct dlist_node node;
	struct raw_mutex lock;
} ux_packet_mclist_t;

typedef struct ux_packet_sock {
	ux_sock_t sk;
	int ifindex;
	int type;
	void *epoll_info;
	uint16_t protocol_num;
	bool running;
	struct raw_sem recvsem;
	struct ux_packet_stats stats;
	struct ux_packet_type prot_hook;
	struct ux_buf_head psk_receive_queue;
	int auxdata;
	struct ux_packet_mclist ml;
	struct dlist_node head;
} ux_packet_sock_t;

typedef struct ux_packet_buf {
	uint8_t pkttype;
	int devindex;
	uint16_t devtype;
	uint8_t dev_halen;
	uint8_t dev_addr[SSL_ADDR_LEN];
	uint32_t len;
	uint32_t snaplen;
	uint32_t nethdr_offset;
	bool vlan_present;
	uint16_t vlan_tci;
	uint16_t vlan_tpid;
	ux_buf_t ux_buf;
} ux_packet_buf_t;

static ux_packet_sock_t *ux_packet_sk(ux_sock_t *sk)
{
	return (ux_packet_sock_t *)sk;
}

static inline void ux_packet_mclist_init(ux_packet_mclist_t *psk_mclist)
{
	dlist_init(&psk_mclist->node);
	raw_mutex_init(&psk_mclist->lock);
}

static inline void ux_packet_mclist_lock(ux_packet_mclist_t *psk_mclist)
{
	raw_mutex_lock(&psk_mclist->lock);
}

static inline void ux_packet_mclist_unlock(ux_packet_mclist_t *psk_mclist)
{
	raw_mutex_unlock(&psk_mclist->lock);
}

void ux_packet_mclist_free(ux_packet_mclist_t *psk_mclist);

int af_packet_init(void);

#endif
