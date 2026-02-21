/*
 * virtual_network.h
 *
 * virtual network kernel module implementation
 *
 * Copyright (c) 2024-2024 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */

#ifndef _VIRTUAL_NETWORK_SESSION_H_
#define _VIRTUAL_NETWORK_SESSION_H_

#include "virtual_network_device.h"

#include <linux/types.h>
#include <linux/kref.h>
#include <linux/atomic.h>
#include <linux/rwlock.h>
#include <linux/rbtree.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_core.h>
#include <net/sock.h>

#define VNET_RX_INTERVAL_STEP 25 // ms
#define VNET_RX_INTERVAL_NUM 41

typedef struct vnet_uid_session {
    struct kref ref;
    spinlock_t lock;
    struct rb_node node;
    u32 uid;

    u32 tcp_rx_packet;
    u64 tcp_rx_traffic;
    u32 tcp_tx_packet;
    u64 tcp_tx_traffic;
    u32 udp_rx_packet;
    u64 udp_rx_traffic;
    u32 udp_tx_packet;
    u64 udp_tx_traffic;
} vnet_uid_session_t;

struct vnet_session;

typedef struct vnet_session_statistics {
    u64 packet_tx;
    u64 traffic_tx;
    u64 timestamp_tx;

    u64 packet_tx_primary;
    u64 traffic_tx_primary;

    u64 packet_tx_secondary;
    u64 traffic_tx_secondary;

    u64 packet_tx_wifi;
    u64 traffic_tx_wifi;

    u64 packet_rx;
    u64 traffic_rx;
    u64 timestamp_rx;

    u64 packet_rx_primary;
    u64 traffic_rx_primary;
    u64 timestamp_rx_primary;

    u64 packet_rx_secondary;
    u64 traffic_rx_secondary;
    u64 timestamp_rx_secondary;

    u64 packet_rx_wifi;
    u64 traffic_rx_wifi;
    u64 timestamp_rx_wifi;

    u32 rx_interval[VNET_RX_INTERVAL_NUM];
    u32 rx_interval_primary[VNET_RX_INTERVAL_NUM];
    u32 rx_interval_secondary[VNET_RX_INTERVAL_NUM];
    u32 rx_interval_wifi[VNET_RX_INTERVAL_NUM];
} vnet_session_statistics_t;

typedef struct vnet_branch {
    struct hlist_nulls_node ss_hnnode;  // link to vnet_session_t.branches
    struct hlist_nulls_node hnnode;  // link to g_branch_htable
    struct nf_conntrack_tuple tuple;
    unsigned int hash;

    struct vnet_session *ss;
    int role_id;
    __be16 pkt_id;
} vnet_branch_t;

typedef struct vnet_session {
    struct kref ref;
    rwlock_t lock;
    u32 uid;
    vnet_uid_session_t *uid_ss;
    vnet_branch_t master; // master trunk to tx packets through vnic device
    struct hlist_nulls_head branches; // sub-branches to tx/rx packets through physical net device
    vnet_session_statistics_t statistics;
} vnet_session_t;

int vnet_session_mgr_init(void);
void vnet_session_mgr_exit(void);

bool vnet_has_sessions(void);

vnet_session_t *vnet_get_session_for_tx(struct sk_buff *skb);
vnet_session_t *vnet_get_session_for_rx(struct sk_buff *skb, int role_id);

bool vnet_session_maybe_add_new_branch(vnet_session_t *ss, const struct sk_buff *skb, int role_id);

void vnet_session_get(vnet_session_t *ss);
void vnet_session_put(vnet_session_t *ss);

void vnet_session_update_tx(vnet_session_t *ss, const struct sk_buff *skb);
void vnet_session_update_rx(vnet_session_t *ss, const struct sk_buff *skb);

void vnet_session_update_path_tx(vnet_session_t *ss, const struct sk_buff *skb, vnet_device_role_id_t role_id);
void vnet_session_update_path_rx(vnet_session_t *ss, const struct sk_buff *skb, vnet_device_role_id_t role_id);

void vnet_count_tcp_tx_packet(u32 uid, const size_t len);
void vnet_count_tcp_rx_packet(u32 uid, const size_t len);
void vnet_count_udp_tx_packet(u32 uid, const size_t len);
void vnet_count_udp_rx_packet(u32 uid, const size_t len);

void vnet_session_mgr_start_statistics_print(void);
void vnet_session_mgr_stop_statistics_print(void);

#endif /* _VIRTUAL_NETWORK_SESSION_H_ */
