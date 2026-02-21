/*
 * virtual_network.c
 *
 * virtual network module implementation
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

#include "virtual_network_session.h"
#include "virtual_network_utils.h"
#include "virtual_network.h"
#include "virtual_network_device.h"

#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/random.h>
#include <linux/list.h>
#include <linux/rculist.h>
#include <linux/rculist_nulls.h>
#include <linux/rwlock.h>
#include <linux/mutex.h>

#include <net/ip.h>
#include <net/udp.h>
#include <net/tcp.h>
#include <net/icmp.h>

#define VNET_SESSION_PCPU_LISTS_NULLS_VAL ((1 << 6) + 0)

#define VNET_SESSION_TBL_SIZE 2048
#define VNET_SESSION_COUNT_MAX 2048

#define VNET_STATISTICS_PRINT_MS 5000
#define SESSION_PRINT_PACKET_TX_THRES 128
#define SESSION_PRINT_PACKET_RX_THRES 128

#define VNET_TUPLE_PFORMAT "{p %hhu, s %u:%hu, d %u:%hu}"
#define VNET_TUPLE_PRINT(_tuple) \
    (_tuple)->dst.protonum, (_tuple)->src.u3.ip, (_tuple)->src.u.all, (_tuple)->dst.u3.ip, (_tuple)->dst.u.all

// global variables that related to ui_session
static struct rb_root g_uid_session_data;
static seqlock_t g_uid_session_data_lock;
static atomic_t g_uid_session_count;
static __read_mostly struct kmem_cache *g_uid_session_cache = NULL;

static __read_mostly unsigned int g_hash_seed = 0;

// global variables that related to session
static DEFINE_MUTEX(g_clear_session_mutex);

static atomic_t g_all_session_count;
static __read_mostly struct kmem_cache *g_session_cache = NULL;
static __read_mostly struct kmem_cache *g_branch_cache = NULL;

static __read_mostly unsigned int g_session_htable_size;
static __read_mostly struct hlist_nulls_head *g_session_htable = NULL;
static __cacheline_aligned_in_smp rwlock_t g_session_htable_locks[VNET_SESSION_TBL_SIZE];
static atomic_t g_session_htable_count;

static __read_mostly struct hlist_nulls_head *g_branch_htable = NULL;
static __cacheline_aligned_in_smp rwlock_t g_branch_htable_locks[VNET_SESSION_TBL_SIZE];

static struct hrtimer g_statistics_print_timer;
static struct work_struct g_statistics_print_work;

static int vnet_init_uid_session_data(void)
{
    g_uid_session_data = RB_ROOT;
    seqlock_init(&g_uid_session_data_lock);
    atomic_set(&g_uid_session_count, 0);
    g_uid_session_cache = kmem_cache_create("virtual_network_uid_session_cache", sizeof(vnet_uid_session_t),
        NFCT_INFOMASK + 1, SLAB_TYPESAFE_BY_RCU | SLAB_HWCACHE_ALIGN, NULL);
    if (unlikely(!g_uid_session_cache)) {
        vnet_log_err("uid_session cache create failed, error");
        return ERROR;
    }

    return SUCCESS;
}

static void vnet_uninit_uid_session_data(void)
{
    if (likely(g_uid_session_cache != NULL)) {
        kmem_cache_destroy(g_uid_session_cache);
        g_uid_session_cache = NULL;
        vnet_log_info("uid_session cache free");
    }
    return;
}

static vnet_uid_session_t *vnet_alloc_uid_session(const u32 uid)
{
    vnet_uid_session_t *uid_ss = NULL;

    if (unlikely(!g_uid_session_cache)) {
        vnet_log_err("g_uid_session_cache is NULL, error");
        return NULL;
    }

    uid_ss = kmem_cache_alloc(g_uid_session_cache, GFP_ATOMIC);
    if (unlikely(!uid_ss)) {
        vnet_log_err("kmem_cache_alloc failed, error");
        return NULL;
    }

    kref_init(&uid_ss->ref);
    spin_lock_init(&(uid_ss->lock));
    uid_ss->uid = uid;

    uid_ss->tcp_rx_packet = 0;
    uid_ss->tcp_rx_traffic = 0;
    uid_ss->tcp_tx_packet = 0;
    uid_ss->tcp_tx_traffic = 0;
    uid_ss->udp_rx_packet = 0;
    uid_ss->udp_rx_traffic = 0;
    uid_ss->udp_tx_packet = 0;
    uid_ss->udp_tx_traffic = 0;

    atomic_inc(&g_uid_session_count);
    vnet_log_info("uid_session alloc, uid:%u uid_session_count %d", uid_ss->uid, atomic_read(&g_uid_session_count));
    return uid_ss;
}

static void vnet_free_uid_session(vnet_uid_session_t *uid_ss)
{
    if (unlikely(uid_ss == NULL)) {
        return;
    }

    if (unlikely(g_uid_session_cache == NULL)) {
        return;
    }

    atomic_dec(&g_uid_session_count);
    vnet_log_info("uid_session free, uid:%u uid_session_count %d", uid_ss->uid, atomic_read(&g_uid_session_count));
    kmem_cache_free(g_uid_session_cache, uid_ss);
    return;
}

static void vnet_uid_session_release(struct kref *kref)
{
    vnet_uid_session_t *uid_ss = container_of(kref, vnet_uid_session_t, ref);
    vnet_free_uid_session(uid_ss);
}

// Incremental reference count on a vnet_uid_session
static void vnet_uid_session_get(vnet_uid_session_t *uid_ss)
{
    kref_get(&uid_ss->ref);
}

// decrement reference count on a vnet_uid_session
static void vnet_uid_session_put(vnet_uid_session_t *uid_ss)
{
    kref_put(&uid_ss->ref, vnet_uid_session_release);
}

static void vnet_clear_all_uid_session(void)
{
    struct rb_root *root = &g_uid_session_data;
    struct rb_node *p = NULL;

    vnet_log_info("clear all uid_session, uid_session_count %d", atomic_read(&g_uid_session_count));
    write_seqlock_bh(&g_uid_session_data_lock);
    if (RB_EMPTY_ROOT(root)) {
        write_sequnlock_bh(&g_uid_session_data_lock);
        return;
    }

    while (true) {
        vnet_uid_session_t *uid_ss;
        p = rb_first(root);
        if (p == NULL) {
            break;
        }
        uid_ss = rb_entry(p, vnet_uid_session_t, node);
        vnet_log_info("clear uid_session %u", uid_ss->uid);
        rb_erase(&uid_ss->node, root);
        vnet_uid_session_put(uid_ss);
    }
    g_uid_session_data = RB_ROOT;
    write_sequnlock_bh(&g_uid_session_data_lock);
    return;
}

// Called with rcu_read_lock() or g_uid_session_data_lock held
static vnet_uid_session_t *vnet_lookup_uid_session(struct rb_root *root, unsigned int read_seq, unsigned int seq,
    int *invalid, struct rb_node **parent_p, struct rb_node ***pp_p, const u32 uid)
{
    struct rb_node **pp = NULL;
    struct rb_node *parent = NULL;
    struct rb_node *next = NULL;
    vnet_uid_session_t *p = NULL;

    pp = &(root->rb_node);
    while (1) {
        next = rcu_dereference_raw(*pp);
        if (!next) {
            break;
        }
        parent = next;
        p = rb_entry(parent, vnet_uid_session_t, node);
        if (uid == p->uid) {
            *invalid = 0;
            return p;
        }
        if (read_seq != 0) {
            if (unlikely(read_seqretry(&g_uid_session_data_lock, seq)))
                break;
        }
        if (uid < p->uid) {
            pp = &next->rb_left;
        } else {
            pp = &next->rb_right;
        }
    }
    *invalid = 1;
    *parent_p = parent;
    *pp_p = pp;
    return NULL;
}

static vnet_uid_session_t *vnet_search_uid_session(const u32 uid)
{
    struct rb_root *root = &g_uid_session_data;
    vnet_uid_session_t *p = NULL;
    struct rb_node **pp = NULL;
    struct rb_node *parent = NULL;
    unsigned int seq;
    int invalid = 0;

    /* Attempt a lockless lookup first.
     * Because of a concurrent writer, we might not find an existing entry.
     */
    rcu_read_lock();
    seq = read_seqbegin(&g_uid_session_data_lock);
    p = vnet_lookup_uid_session(root, 1, seq, &invalid, &parent, &pp, uid);
    rcu_read_unlock();

    /* If find the uid vnet_session, return the uid vnet_session */
    if (p) {
        return p;
    }

    /* If no writer did a change and not create, we can return early. */
    if (invalid == 0) {
        return NULL;
    }

    /* retry an exact lookup, taking the lock before.
     * At least, nodes should be hot in our cache.
     */
    parent = NULL;
    write_seqlock_bh(&g_uid_session_data_lock);
    p = vnet_lookup_uid_session(root, 0, 0, &invalid, &parent, &pp, uid);
    write_sequnlock_bh(&g_uid_session_data_lock);
    return p;
}

// need Existence check by vnet_search_uid_session function before called
static vnet_uid_session_t *vnet_insert_uid_session(const u32 uid)
{
    struct rb_root *root = &g_uid_session_data;
    vnet_uid_session_t *cursor = NULL;
    vnet_uid_session_t *p = NULL;
    struct rb_node **pp = NULL;
    struct rb_node *parent = NULL;

    write_seqlock_bh(&g_uid_session_data_lock);
    pp = &(root->rb_node);
    parent = NULL;
    while (*pp) {
        parent = *pp;
        cursor = rb_entry(parent, vnet_uid_session_t, node);
        if (uid < cursor->uid) {
            pp = &(*pp)->rb_left;
        } else if (uid > cursor->uid) {
            pp = &(*pp)->rb_right;
        } else {
            write_sequnlock_bh(&g_uid_session_data_lock);
            return cursor;
        }
    }
    p = vnet_alloc_uid_session(uid);
    if (!p) {
        write_sequnlock_bh(&g_uid_session_data_lock);
        return NULL;
    }
    rb_link_node_rcu(&p->node, parent, pp);
    rb_insert_color(&p->node, root);

    write_sequnlock_bh(&g_uid_session_data_lock);
    vnet_log_info("insert uid_session %d", uid);
    return p;
}

static void vnet_remove_uid_session(const u32 uid)
{
    struct rb_root *root = &g_uid_session_data;
    vnet_uid_session_t *uid_ss = NULL;
    struct rb_node **cur = NULL;

    write_seqlock_bh(&g_uid_session_data_lock);
    cur = &(root->rb_node);
    while (*cur != NULL) {
        uid_ss = rb_entry(*cur, vnet_uid_session_t, node);
        if (uid < uid_ss->uid) {
            cur = &((*cur)->rb_left);
        } else if (uid > uid_ss->uid) {
            cur = &((*cur)->rb_right);
        } else {
            rb_erase(&(uid_ss->node), root);
            vnet_uid_session_put(uid_ss);
            write_sequnlock_bh(&g_uid_session_data_lock);
            vnet_log_info("remove uid_session %d", uid);
            return;
        }
    }
    write_sequnlock_bh(&g_uid_session_data_lock);
    vnet_log_warn("not find uid_session %d", uid);
    return;
}

void vnet_count_tcp_tx_packet(u32 uid, const size_t len)
{
    vnet_uid_session_t *uid_ss = NULL;
    if (!vnet_has_sessions())
        return;

    uid_ss = vnet_search_uid_session(uid);
    if (uid_ss == NULL) {
        return;
    }

    spin_lock_bh(&uid_ss->lock);
    uid_ss->tcp_tx_packet++;
    uid_ss->tcp_tx_traffic += len;
    spin_unlock_bh(&uid_ss->lock);
    return;
}

void vnet_count_tcp_rx_packet(u32 uid, const size_t len)
{
    vnet_uid_session_t *uid_ss = NULL;
    if (!vnet_has_sessions())
        return;

    uid_ss = vnet_search_uid_session(uid);
    if (uid_ss == NULL) {
        return;
    }

    spin_lock_bh(&uid_ss->lock);
    uid_ss->tcp_rx_packet++;
    uid_ss->tcp_rx_traffic += len;
    spin_unlock_bh(&uid_ss->lock);
    return;
}

void vnet_count_udp_tx_packet(u32 uid, const size_t len)
{
    vnet_uid_session_t *uid_ss = NULL;
    if (!vnet_has_sessions()) {
        return;
    }

    uid_ss = vnet_search_uid_session(uid);
    if (uid_ss == NULL) {
        return;
    }

    spin_lock_bh(&uid_ss->lock);
    uid_ss->udp_tx_packet++;
    uid_ss->udp_tx_traffic += len;
    spin_unlock_bh(&uid_ss->lock);
    return;
}

void vnet_count_udp_rx_packet(u32 uid, const size_t len)
{
    vnet_uid_session_t *uid_ss = NULL;
    if (!vnet_has_sessions()) {
        return;
    }

    uid_ss = vnet_search_uid_session(uid);
    if (uid_ss == NULL) {
        return;
    }

    spin_lock_bh(&uid_ss->lock);
    uid_ss->udp_rx_packet++;
    uid_ss->udp_rx_traffic += len;
    spin_unlock_bh(&uid_ss->lock);
    return;
}

static void vnet_print_uid_session_statistics(vnet_uid_session_t *uid_ss)
{
    spin_lock_bh(&uid_ss->lock);
    vnet_log_info("uid:%u tcp {tx %u traffic %llu; rx %u traffic %llu}"
        " udp {tx %u traffic %llu; rx %u traffic %llu}",
        uid_ss->uid, uid_ss->tcp_tx_packet, uid_ss->tcp_tx_traffic, uid_ss->tcp_rx_packet, uid_ss->tcp_rx_traffic,
        uid_ss->udp_tx_packet, uid_ss->udp_tx_traffic, uid_ss->udp_rx_packet, uid_ss->udp_rx_traffic);
    spin_unlock_bh(&uid_ss->lock);
    return;
}

static void vnet_print_all_uid_session_statistics(void)
{
    struct rb_root *root = &g_uid_session_data;
    vnet_uid_session_t *pos = NULL;
    vnet_uid_session_t *n = NULL;

    write_seqlock_bh(&g_uid_session_data_lock);
    if (RB_EMPTY_ROOT(root)) {
        write_sequnlock_bh(&g_uid_session_data_lock);
        return;
    }

    rbtree_postorder_for_each_entry_safe(pos, n, root, node)
    {
        vnet_print_uid_session_statistics(pos);
    }

    write_sequnlock_bh(&g_uid_session_data_lock);
    return;
}

static u32 vnet_cal_tuple_hash(const struct nf_conntrack_tuple *tuple, u32 net_hash)
{
    unsigned int n;
    u32 seed;
    u32 val;

    get_random_once(&g_hash_seed, sizeof(g_hash_seed));

    seed = g_hash_seed ^ net_hash;
    n = (sizeof(tuple->src) + sizeof(tuple->dst.u3)) / sizeof(u32);
    val = jhash2((u32 *)tuple, n, seed ^ (((__force __u16)tuple->dst.u.all << 16) | tuple->dst.protonum));
    return reciprocal_scale(val, g_session_htable_size);
}

static u_int8_t vnet_get_l3_protonum(const struct sk_buff *skb)
{
    u_int16_t l3num;

    switch (skb->protocol) {
        case htons(ETH_P_IP):
            l3num = NFPROTO_IPV4;
            break;
        case htons(ETH_P_IPV6):
            l3num = NFPROTO_IPV6;
            break;
        default:
            l3num = NFPROTO_UNSPEC;
            break;
    }

    return l3num;
}

static bool vnet_get_tuple_of_tx_skb(struct nf_conntrack_tuple *tuple, const struct sk_buff *skb)
{
    u_int16_t l3num = vnet_get_l3_protonum(skb);
    if (l3num == NFPROTO_UNSPEC) {
        return false;
    }

    if (!nf_ct_get_tuplepr(skb, skb_network_offset(skb), l3num, dev_net(skb->dev), tuple)) {
        struct iphdr *iph = ip_hdr(skb);
        vnet_log_err("nf_ct_get_tuplepr failed, l3num %hu, ip_protocol:%hhu", l3num, iph->protocol);
        return false;
    }

    return true;
}

static bool vnet_get_tuple_of_rx_skb(struct nf_conntrack_tuple *tuple, const struct sk_buff *skb)
{
    struct nf_conntrack_tuple inverse;
    u_int16_t l3num = vnet_get_l3_protonum(skb);
    if (l3num == NFPROTO_UNSPEC) {
        return false;
    }

    if (!nf_ct_get_tuplepr(skb, skb_network_offset(skb), l3num, dev_net(skb->dev), &inverse)) {
        struct iphdr *iph = ip_hdr(skb);
        vnet_log_err("nf_ct_get_tuplepr failed, l3num %hu, ip_protocol:%hhu", l3num, iph->protocol);
        return false;
    }

    if (!nf_ct_invert_tuple(tuple, &inverse)) {
        struct iphdr *iph = ip_hdr(skb);
        vnet_log_err("nf_ct_invert_tuple failed, ip_protocol:%hhu src.l3num:%hu, dst.protonum:%hu dst.u.icmp.type:%hhu",
            iph->protocol, inverse.src.l3num, inverse.dst.protonum, inverse.dst.u.icmp.type);
        return false;
    }

    return true;
}

static int vnet_init_session_data(void)
{
    int i;

    atomic_set(&g_session_htable_count, 0);
    g_session_htable_size = VNET_SESSION_TBL_SIZE;
    g_session_htable = nf_ct_alloc_hashtable(&g_session_htable_size, 1);
    if (unlikely(!g_session_htable)) {
        vnet_log_err("session hash table alloc failed, error");
        return ERROR;
    }

    for (i = 0; i < VNET_SESSION_TBL_SIZE; i++) {
        rwlock_init(&g_session_htable_locks[i]);
    }

    g_branch_htable = nf_ct_alloc_hashtable(&g_session_htable_size, 1);
    if (unlikely(!g_branch_htable)) {
        vnet_log_err("session branch hash table alloc failed, error");
        goto branch_htable_fail;
    }

    for (i = 0; i < VNET_SESSION_TBL_SIZE; i++) {
        rwlock_init(&g_branch_htable_locks[i]);
    }

    atomic_set(&g_all_session_count, 0);
    g_session_cache = kmem_cache_create("virtual_network_session_cache", sizeof(vnet_session_t), NFCT_INFOMASK + 1,
        SLAB_TYPESAFE_BY_RCU | SLAB_HWCACHE_ALIGN, NULL);
    if (unlikely(!g_session_cache)) {
        vnet_log_err("session cache create failed, error");
        goto session_cache_fail;
    }

    g_branch_cache = kmem_cache_create("virtual_network_session_branch_cache", sizeof(vnet_session_t),
        NFCT_INFOMASK + 1, SLAB_TYPESAFE_BY_RCU | SLAB_HWCACHE_ALIGN, NULL);
    if (unlikely(!g_branch_cache)) {
        vnet_log_err("session branch cache create failed, error");
        goto branch_cache_fail;
    }
    return SUCCESS;

branch_cache_fail:
    kmem_cache_destroy(g_session_cache);
    g_session_cache = NULL;
session_cache_fail:
    kvfree(g_branch_htable);
    g_branch_htable = NULL;
branch_htable_fail:
    kvfree(g_session_htable);
    g_session_htable = NULL;
    return ERROR;
}

static void vnet_uninit_session_data(void)
{
    if (likely(g_session_htable != NULL)) {
        kvfree(g_session_htable);
        g_session_htable = NULL;
    }

    if (likely(g_branch_htable != NULL)) {
        kvfree(g_branch_htable);
        g_branch_htable = NULL;
    }

    if (likely(g_session_cache != NULL)) {
        kmem_cache_destroy(g_session_cache);
        g_session_cache = NULL;
        vnet_log_info("session cache free");
    }

    if (likely(g_branch_cache != NULL)) {
        kmem_cache_destroy(g_branch_cache);
        g_branch_cache = NULL;
        vnet_log_info("session branch cache free");
    }
}

static void vnet_print_session_statistics_tx_rx(vnet_session_t *ss)
{
    vnet_log_info("uid:%u protocol %hhu packet tx %llu packet rx %llu"
        " traffic tx %llu traffic rx %llu"
        " packet_tx_primary %llu packet_tx_secondary %llu  packet_tx_wifi %llu"
        " traffic_tx_primary %llu traffic_tx_secondary %llu  traffic_tx_wifi %llu"
        " packet_rx_primary %llu packet_rx_secondary %llu packet_rx_wifi"
        " %llu traffic_rx_primary %llu traffic_rx_secondary %llu traffic_rx_wifi %llu",
        ss->uid, ss->master.tuple.dst.protonum, ss->statistics.packet_tx, ss->statistics.packet_rx,
        ss->statistics.traffic_tx, ss->statistics.traffic_rx, ss->statistics.packet_tx_primary,
        ss->statistics.packet_tx_secondary, ss->statistics.packet_tx_wifi, ss->statistics.traffic_tx_primary,
        ss->statistics.traffic_tx_secondary, ss->statistics.traffic_tx_wifi, ss->statistics.packet_rx_primary,
        ss->statistics.packet_rx_secondary, ss->statistics.packet_rx_wifi, ss->statistics.traffic_rx_primary,
        ss->statistics.traffic_rx_secondary, ss->statistics.traffic_rx_wifi);

    return;
}

static void vnet_print_session_statistics_rx_interval_each(vnet_session_t *ss, u32 *rx_interval, int len)
{
    if (len < NUM_41)
        return;
    vnet_log_info("rx interval uid:%u protocol %hhu"
        " %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u"
        " %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u",
        ss->uid, ss->master.tuple.dst.protonum, *(rx_interval + NUM_0), *(rx_interval + NUM_1), *(rx_interval + NUM_2),
        *(rx_interval + NUM_3), *(rx_interval + NUM_4), *(rx_interval + NUM_5), *(rx_interval + NUM_6),
        *(rx_interval + NUM_7), *(rx_interval + NUM_8), *(rx_interval + NUM_9), *(rx_interval + NUM_10),
        *(rx_interval + NUM_11), *(rx_interval + NUM_12), *(rx_interval + NUM_13), *(rx_interval + NUM_14),
        *(rx_interval + NUM_15), *(rx_interval + NUM_16), *(rx_interval + NUM_17), *(rx_interval + NUM_18),
        *(rx_interval + NUM_19), *(rx_interval + NUM_20), *(rx_interval + NUM_21), *(rx_interval + NUM_22),
        *(rx_interval + NUM_23), *(rx_interval + NUM_24), *(rx_interval + NUM_25), *(rx_interval + NUM_26),
        *(rx_interval + NUM_27), *(rx_interval + NUM_28), *(rx_interval + NUM_29), *(rx_interval + NUM_30),
        *(rx_interval + NUM_31), *(rx_interval + NUM_32), *(rx_interval + NUM_33), *(rx_interval + NUM_34),
        *(rx_interval + NUM_35), *(rx_interval + NUM_36), *(rx_interval + NUM_37), *(rx_interval + NUM_38),
        *(rx_interval + NUM_39), *(rx_interval + NUM_40));
    return;
}

static void vnet_print_session_statistics_rx_interval(vnet_session_t *ss)
{
    vnet_print_session_statistics_rx_interval_each(ss, ss->statistics.rx_interval, VNET_RX_INTERVAL_NUM);
    vnet_print_session_statistics_rx_interval_each(ss, ss->statistics.rx_interval_primary, VNET_RX_INTERVAL_NUM);
    vnet_print_session_statistics_rx_interval_each(ss, ss->statistics.rx_interval_secondary, VNET_RX_INTERVAL_NUM);
    vnet_print_session_statistics_rx_interval_each(ss, ss->statistics.rx_interval_wifi, VNET_RX_INTERVAL_NUM);
    return;
}

static void vnet_session_statistics_rx_interval_reset(vnet_session_t *ss)
{
    (void)memset_s(ss->statistics.rx_interval, sizeof(ss->statistics.rx_interval), 0x00,
        sizeof(ss->statistics.rx_interval));
    (void)memset_s(ss->statistics.rx_interval_primary, sizeof(ss->statistics.rx_interval_primary), 0x00,
        sizeof(ss->statistics.rx_interval_primary));
    (void)memset_s(ss->statistics.rx_interval_secondary, sizeof(ss->statistics.rx_interval_secondary), 0x00,
        sizeof(ss->statistics.rx_interval_secondary));
    (void)memset_s(ss->statistics.rx_interval_wifi, sizeof(ss->statistics.rx_interval_wifi), 0x00,
        sizeof(ss->statistics.rx_interval_wifi));
    return;
}

static void vnet_print_session_statistics(vnet_session_t *ss)
{
    read_lock_bh(&ss->lock);
    if (ss->statistics.packet_tx > SESSION_PRINT_PACKET_TX_THRES ||
        ss->statistics.packet_rx > SESSION_PRINT_PACKET_RX_THRES) {
        vnet_print_session_statistics_tx_rx(ss);
        vnet_print_session_statistics_rx_interval(ss);
    }

    vnet_session_statistics_rx_interval_reset(ss);
    read_unlock_bh(&ss->lock);
    return;
}

static void vnet_print_all_session_statistics(void)
{
    unsigned int bucket;
    vnet_log_info("session count is %d", atomic_read(&g_session_htable_count));
    for (bucket = 0; bucket < g_session_htable_size; bucket++) {
        vnet_branch_t *master = NULL;
        struct hlist_nulls_node *n = NULL;
        rwlock_t *lock = &g_session_htable_locks[bucket % VNET_SESSION_TBL_SIZE];

        read_lock_bh(lock);
        hlist_nulls_for_each_entry_rcu(master, n, &g_session_htable[bucket], hnnode)
        {
            vnet_print_session_statistics(master->ss);
        }
        read_unlock_bh(lock);
        cond_resched();
    }
    return;
}

static void vnet_statistics_print_work_handle(struct work_struct *work)
{
    vnet_print_all_session_statistics();
    vnet_print_all_uid_session_statistics();
}

static enum hrtimer_restart vnet_statistics_print_timer_func(struct hrtimer *timer)
{
    if (atomic_read(&g_session_htable_count) > 0) {
        schedule_work(&g_statistics_print_work);
    }

    hrtimer_forward_now(timer, ms_to_ktime(VNET_STATISTICS_PRINT_MS));
    return HRTIMER_RESTART;
}

static void vnet_insert_session_branch(vnet_branch_t *branch)
{
    rwlock_t *lock = &g_branch_htable_locks[branch->hash % VNET_SESSION_TBL_SIZE];
    write_lock_bh(lock);
    hlist_nulls_add_head_rcu(&branch->hnnode, &g_branch_htable[branch->hash]);
    write_unlock_bh(lock);
    return;
}

static void vnet_remove_session_branch(vnet_branch_t *branch)
{
    rwlock_t *lock = &g_branch_htable_locks[branch->hash % VNET_SESSION_TBL_SIZE];
    write_lock_bh(lock);
    hlist_nulls_del_rcu(&branch->hnnode);
    write_unlock_bh(lock);
    return;
}

static vnet_branch_t *vnet_find_session_branch(struct hlist_nulls_head *htable, int role_id,
    const struct nf_conntrack_tuple *tuple)
{
    vnet_branch_t *branch = NULL;
    struct hlist_nulls_node *n = NULL;

    hlist_nulls_for_each_entry_rcu(branch, n, htable, hnnode)
    {
        if ((role_id == branch->role_id) && nf_ct_tuple_equal(tuple, &branch->tuple)) {
            return branch;
        }
    }
    return NULL;
}

static vnet_branch_t *vnet_alloc_session_branch(vnet_session_t *ss, int role_id, const struct nf_conntrack_tuple *tuple,
    unsigned int hash)
{
    vnet_branch_t *branch = NULL;

    branch = kmem_cache_alloc(g_session_cache, GFP_ATOMIC);
    if (unlikely(branch == NULL)) {
        return NULL;
    }

    (void)memset_s(branch, sizeof(vnet_branch_t), 0, sizeof(vnet_branch_t));
    branch->tuple = *tuple;
    branch->hash = hash;
    branch->ss = ss;
    branch->role_id = role_id;
    vnet_log_info("alloc session branch uid:%u " VNET_TUPLE_PFORMAT " role_id:%d", ss->uid, VNET_TUPLE_PRINT(tuple),
        role_id);
    return branch;
}

static void vnet_free_session_branch(vnet_branch_t *branch)
{
    u32 uid;
    int role_id;
    struct nf_conntrack_tuple tuple;

    if (unlikely(branch == NULL)) {
        return;
    }

    uid = branch->ss->uid;
    role_id = branch->role_id;
    tuple = branch->tuple;
    if (unlikely(g_branch_cache == NULL)) {
        vnet_log_warn("fail to free session branch uid:%u role_id:%d after branch cache freed.", uid, role_id);
        return;
    }

    kmem_cache_free(g_branch_cache, branch);
    vnet_log_info("free session branch uid:%u " VNET_TUPLE_PFORMAT " role_id:%d", uid, VNET_TUPLE_PRINT(&tuple),
        role_id);
    return;
}

static void vnet_session_free_branches(vnet_session_t *ss)
{
    vnet_branch_t *branch = NULL;
    struct hlist_nulls_node *n = NULL;

    write_lock_bh(&ss->lock);
    hlist_nulls_for_each_entry_safe(branch, n, &ss->branches, ss_hnnode)
    {
        hlist_nulls_del_rcu(&branch->ss_hnnode);
        vnet_free_session_branch(branch);
    }
    write_unlock_bh(&ss->lock);

    return;
}

static void vnet_session_remove_branches(vnet_session_t *ss)
{
    vnet_branch_t *branch = NULL;
    struct hlist_nulls_node *n = NULL;

    write_lock_bh(&ss->lock);
    hlist_nulls_for_each_entry_safe(branch, n, &ss->branches, ss_hnnode)
    {
        vnet_log_info("remove session branch uid:%u " VNET_TUPLE_PFORMAT " role_id:%d hash %u", ss->uid,
            VNET_TUPLE_PRINT(&branch->tuple), branch->role_id, branch->hash);
        vnet_remove_session_branch(branch);
    }
    write_unlock_bh(&ss->lock);
    return;
}

static vnet_session_t *vnet_alloc_session(u32 uid, const struct nf_conntrack_tuple *tuple, unsigned int hash)
{
    vnet_session_t *ss;
    int session_count;

    ss = kmem_cache_alloc(g_session_cache, GFP_ATOMIC);
    if (unlikely(ss == NULL)) {
        return NULL;
    }

    (void)memset_s(ss, sizeof(vnet_session_t), 0, sizeof(vnet_session_t));
    ss->master.tuple = *tuple;
    ss->master.hash = hash;
    ss->master.ss = ss;
    ss->master.role_id = -1;

    kref_init(&ss->ref);
    rwlock_init(&ss->lock);
    ss->uid = uid;
    INIT_HLIST_NULLS_HEAD(&ss->branches, VNET_SESSION_PCPU_LISTS_NULLS_VAL);

    session_count = atomic_inc_return(&g_all_session_count);
    vnet_log_info("alloc session uid:%u " VNET_TUPLE_PFORMAT " session_count %d ", ss->uid, VNET_TUPLE_PRINT(tuple),
        session_count);
    return ss;
}

static void vnet_free_session(vnet_session_t *ss)
{
    u32 uid;
    int session_count;
    struct nf_conntrack_tuple tuple;

    if (unlikely(ss == NULL)) {
        return;
    }

    // A freed object has reference use == 0, that's the golden rule for SLAB_TYPESAFE_BY_RCU
    WARN_ON(kref_read(&ss->ref) != 0);

    vnet_session_free_branches(ss);

    if (likely(ss->uid_ss != NULL)) {
        vnet_uid_session_put(ss->uid_ss);
        ss->uid_ss = NULL;
    }

    uid = ss->uid;
    tuple = ss->master.tuple;
    if (likely(g_session_cache != NULL)) {
        kmem_cache_free(g_session_cache, ss);
    } else {
        vnet_log_warn("fail to free session uid:%u after session cache freed.", uid);
    }
    session_count = atomic_dec_return(&g_all_session_count);

    vnet_log_info("free session uid:%u " VNET_TUPLE_PFORMAT " session_count %d ", uid, VNET_TUPLE_PRINT(&tuple),
        session_count);
    return;
}

static vnet_session_t *vnet_insert_session(vnet_session_t *ss)
{
    vnet_branch_t *branch = NULL;
    rwlock_t *lock = &g_session_htable_locks[ss->master.hash % VNET_SESSION_TBL_SIZE];

    write_lock_bh(lock);
    // to avoid insert session with same tuple at the same time in different process
    branch = vnet_find_session_branch(&g_session_htable[ss->master.hash], -1, &ss->master.tuple);
    if (branch != NULL) {
        write_unlock_bh(lock);
        return branch->ss;
    }

    hlist_nulls_add_head_rcu(&ss->master.hnnode, &g_session_htable[ss->master.hash]);
    write_unlock_bh(lock);

    atomic_inc(&g_session_htable_count);
    return ss;
}

static void vnet_remove_session(vnet_session_t *ss)
{
    rwlock_t *lock = &g_session_htable_locks[ss->master.hash % VNET_SESSION_TBL_SIZE];
    write_lock_bh(lock);
    hlist_nulls_del_rcu(&ss->master.hnnode);
    write_unlock_bh(lock);

    atomic_dec(&g_session_htable_count);
    return;
}

/*
 * Need Existence check by is_session_existed or session_find_get before called
 * The return value(vnet_session_t *) must be vnet_session_put after use
 */
static vnet_session_t *vnet_create_session(u32 uid, const struct nf_conntrack_tuple *tuple, u32 net_hash)
{
    unsigned int hash;
    vnet_session_t *ss;
    vnet_session_t *ss_insert;
    vnet_uid_session_t *uid_ss;

    if (unlikely(atomic_read(&g_session_htable_count) > VNET_SESSION_COUNT_MAX)) {
        return NULL;
    }

    hash = vnet_cal_tuple_hash(tuple, net_hash);
    ss = vnet_alloc_session(uid, tuple, hash);
    if (!ss) {
        vnet_log_err("vnet_session alloc failed");
        return NULL;
    }

    uid_ss = vnet_search_uid_session(uid);
    if (uid_ss == NULL) {
        uid_ss = vnet_insert_uid_session(uid);
    }
    if (unlikely(uid_ss == NULL)) {
        vnet_log_err("uid %u " VNET_TUPLE_PFORMAT "hash %u uid_ss null", uid, VNET_TUPLE_PRINT(tuple), hash);
        vnet_session_put(ss);
        return NULL;
    }
    vnet_uid_session_get(uid_ss);
    ss->uid_ss = uid_ss;

    ss_insert = vnet_insert_session(ss);
    if (ss != ss_insert) {
        vnet_log_info("uid %u " VNET_TUPLE_PFORMAT "hash %u already exit", uid, VNET_TUPLE_PRINT(tuple), hash);
        vnet_session_put(ss);
        return ss_insert;
    }

    vnet_log_info("create session uid:%d " VNET_TUPLE_PFORMAT " hash %u htable_count %d ", ss->uid,
        VNET_TUPLE_PRINT(tuple), ss->master.hash, atomic_read(&g_session_htable_count));
    return ss;
}

static void vnet_delete_session(vnet_session_t *ss)
{
    vnet_log_info("delete session uid:%u " VNET_TUPLE_PFORMAT " hash %u htable_count %d ", ss->uid,
        VNET_TUPLE_PRINT(&ss->master.tuple), ss->master.hash, atomic_read(&g_session_htable_count));

    if (!hlist_nulls_unhashed(&ss->master.hnnode)) {
        vnet_remove_session(ss);
    }

    vnet_session_remove_branches(ss);
    vnet_session_put(ss);
    return;
}

static void vnet_clear_all_session(void)
{
    unsigned int bucket;

    vnet_log_info("clear all session, htable_count %d, session_count %d", atomic_read(&g_session_htable_count),
        atomic_read(&g_all_session_count));

    mutex_lock(&g_clear_session_mutex);
    for (bucket = 0; bucket < g_session_htable_size; bucket++) {
        vnet_branch_t *master = NULL;
        struct hlist_nulls_node *n = NULL;
        rwlock_t *lock = &g_session_htable_locks[bucket % VNET_SESSION_TBL_SIZE];

        write_lock_bh(lock);
        hlist_nulls_for_each_entry_safe(master, n, &g_session_htable[bucket], hnnode)
        {
            hlist_nulls_del_init_rcu(&master->hnnode);
            atomic_dec(&g_session_htable_count);
            vnet_delete_session(master->ss);
        }
        write_unlock_bh(lock);
        cond_resched();
    }
    mutex_unlock(&g_clear_session_mutex);
    return;
}

static vnet_session_t *vnet_create_session_and_get(u32 uid, const struct nf_conntrack_tuple *tuple, u32 net_hash)
{
    vnet_session_t *ss = NULL;

    ss = vnet_create_session(uid, tuple, net_hash);
    if (unlikely(ss == NULL)) {
        return NULL;
    }

    vnet_session_get(ss);
    return ss;
}

static vnet_session_t *vnet_get_session_by_master_tuple(const struct nf_conntrack_tuple *tuple, u32 net_hash)
{
    vnet_branch_t *master = NULL;
    unsigned int bucket = vnet_cal_tuple_hash(tuple, net_hash);

    rcu_read_lock();
    master = vnet_find_session_branch(&g_session_htable[bucket], -1, tuple);
    rcu_read_unlock();
    if (master) {
        vnet_session_get(master->ss);
        return master->ss;
    }
    return NULL;
}

static vnet_session_t *vnet_get_session_by_branch_tuple(int role_id, const struct nf_conntrack_tuple *tuple,
    u32 net_hash, __be16 pkt_id)
{
    vnet_branch_t *branch = NULL;
    unsigned int bucket = vnet_cal_tuple_hash(tuple, net_hash);

    rcu_read_lock();
    branch = vnet_find_session_branch(&g_branch_htable[bucket], role_id, tuple);
    rcu_read_unlock();

    if (branch) {
        vnet_session_get(branch->ss);

        write_lock_bh(&branch->ss->lock);
        branch->pkt_id = pkt_id;
        write_unlock_bh(&branch->ss->lock);
        return branch->ss;
    }
    return NULL;
}

static bool vnet_session_branch_match_tx_ip_frag(vnet_branch_t *branch, struct sk_buff *skb)
{
    struct iphdr *iph = ip_hdr(skb);

    if ((branch->tuple.dst.protonum != iph->protocol) || (branch->tuple.src.u3.ip != iph->saddr) ||
        (branch->tuple.dst.u3.ip != iph->daddr)) {
        return false;
    }

    return (branch->pkt_id == iph->id);
}

static bool vnet_session_branch_match_rx_ip_frag(vnet_branch_t *branch, struct sk_buff *skb)
{
    struct iphdr *iph = ip_hdr(skb);

    if ((branch->tuple.dst.protonum != iph->protocol) || (branch->tuple.src.u3.ip != iph->daddr) ||
        (branch->tuple.dst.u3.ip != iph->saddr)) {
        return false;
    }

    return (branch->pkt_id == iph->id);
}

static vnet_session_t *vnet_get_session_by_tx_ip_frag(struct sk_buff *skb)
{
    unsigned int bucket;
    bool find = false;
    vnet_session_t *ss = NULL;

    for (bucket = 0; bucket < g_session_htable_size && !find; bucket++) {
        vnet_branch_t *master = NULL;
        struct hlist_nulls_node *n = NULL;
        rcu_read_lock();
        hlist_nulls_for_each_entry_rcu(master, n, &g_session_htable[bucket], hnnode)
        {
            if (vnet_session_branch_match_tx_ip_frag(master, skb)) {
                vnet_session_get(master->ss);
                ss = master->ss;
                find = true;
                break;
            }
        }
        rcu_read_unlock();
    }

    return ss;
}

/*
 * find and Incremental reference count on a session.
 * The return value(vnet_session_t *) must be vnet_session_put after use
 */
vnet_session_t *vnet_get_session_for_tx(struct sk_buff *skb)
{
    vnet_session_t *ss = NULL;
    struct iphdr *iph = NULL;
    struct net *net = NULL;
    struct nf_conntrack_tuple tuple;

    if (unlikely(skb == NULL || skb->dev == NULL)) {
        return NULL;
    }

    iph = ip_hdr(skb);
    if ((iph->frag_off & htons(IP_DF)) == 0 && ((iph->frag_off & htons(IP_OFFSET)) != 0)) {
        vnet_log_debug("saddr:%u daddr:%u id:%hu", iph->saddr, iph->daddr, iph->id);
        ss = vnet_get_session_by_tx_ip_frag(skb);
        if (ss == NULL) {
            vnet_log_warn("fragment packet tx not find session, error");
            return NULL;
        }
    } else {
        net = dev_net(skb->dev);
        if (unlikely(net == NULL)) {
            return NULL;
        }

        if (!vnet_get_tuple_of_tx_skb(&tuple, skb)) {
            return NULL;
        }

        ss = vnet_get_session_by_master_tuple(&tuple, net_hash_mix(net));
        if (ss == NULL) {
            ss = vnet_create_session_and_get(skb->sk->sk_uid.val, &tuple, net_hash_mix(net));
            if (unlikely(ss == NULL)) {
                return NULL;
            }
        }
        write_lock_bh(&ss->lock);
        ss->master.pkt_id = iph->id;
        write_unlock_bh(&ss->lock);
    }

    return ss;
}

static vnet_session_t *vnet_get_session_by_rx_ip_frag(struct sk_buff *skb, int role_id)
{
    unsigned int bucket;
    bool find = false;
    vnet_session_t *ss = NULL;

    for (bucket = 0; bucket < g_session_htable_size && !find; bucket++) {
        vnet_branch_t *branch = NULL;
        struct hlist_nulls_node *n = NULL;
        rcu_read_lock();
        hlist_nulls_for_each_entry_rcu(branch, n, &g_branch_htable[bucket], hnnode)
        {
            if ((role_id == branch->role_id) && vnet_session_branch_match_rx_ip_frag(branch, skb)) {
                vnet_session_get(branch->ss);
                ss = branch->ss;
                find = true;
                break;
            }
        }
        rcu_read_unlock();
    }

    return ss;
}

/*
 * find and Incremental reference count on a session
 * The return value(vnet_session_t *) must be vnet_session_put after use
 */
vnet_session_t *vnet_get_session_for_rx(struct sk_buff *skb, int role_id)
{
    vnet_session_t *ss = NULL;
    struct iphdr *iph = NULL;
    struct net *net = NULL;
    struct nf_conntrack_tuple tuple;

    if (unlikely(skb == NULL || skb->dev == NULL)) {
        return NULL;
    }

    iph = ip_hdr(skb);
    if (((iph->frag_off & htons(IP_DF)) == 0) && ((iph->frag_off & htons(IP_OFFSET)) != 0)) {
        vnet_log_debug("saddr:%u daddr:%u id:%hu", iph->saddr, iph->daddr, iph->id);
        ss = vnet_get_session_by_rx_ip_frag(skb, role_id);
    } else {
        net = dev_net(skb->dev);
        if (unlikely(net == NULL)) {
            return NULL;
        }

        if (!vnet_get_tuple_of_rx_skb(&tuple, skb)) {
            return NULL;
        }

        ss = vnet_get_session_by_branch_tuple(role_id, &tuple, net_hash_mix(net), iph->id);
    }

    return ss;
}

static bool vnet_session_has_branch(vnet_session_t *ss, int role_id, const struct nf_conntrack_tuple *tuple)
{
    bool find = false;
    vnet_branch_t *branch = NULL;
    struct hlist_nulls_node *n = NULL;

    hlist_nulls_for_each_entry_rcu(branch, n, &ss->branches, ss_hnnode)
    {
        if (role_id == branch->role_id && nf_ct_tuple_equal(tuple, &branch->tuple)) {
            find = true;
            break;
        }
    }
    return find;
}

bool vnet_session_maybe_add_new_branch(vnet_session_t *ss, const struct sk_buff *skb, int role_id)
{
    struct iphdr *iph;
    unsigned int hash;
    struct nf_conntrack_tuple tuple;
    vnet_branch_t *branch;

    iph = ip_hdr(skb);
    if ((iph->frag_off & htons(IP_DF)) == 0 && ((iph->frag_off & htons(IP_OFFSET)) != 0)) {
        return true;
    }

    if (!vnet_get_tuple_of_tx_skb(&tuple, skb)) {
        return false;
    }

    rcu_read_lock();
    if (vnet_session_has_branch(ss, role_id, &tuple)) {
        rcu_read_unlock();
        return true;
    }
    rcu_read_unlock();

    hash = vnet_cal_tuple_hash(&tuple, net_hash_mix(dev_net(skb->dev)));
    branch = vnet_alloc_session_branch(ss, role_id, &tuple, hash);
    if (branch == NULL) {
        vnet_log_warn("fail to alloc session branch uid:%u " VNET_TUPLE_PFORMAT " role_id:%d", ss->uid,
            VNET_TUPLE_PRINT(&tuple), role_id);
        return false;
    }

    write_lock_bh(&ss->lock);
    // check again
    if (vnet_session_has_branch(ss, role_id, &tuple)) {
        write_unlock_bh(&ss->lock);
        vnet_free_session_branch(branch);
        return true;
    }
    hlist_nulls_add_tail_rcu(&branch->ss_hnnode, &ss->branches);
    write_unlock_bh(&ss->lock);

    vnet_insert_session_branch(branch);
    vnet_log_info("insert session branch uid:%u " VNET_TUPLE_PFORMAT " role_id:%d hash %u", ss->uid,
        VNET_TUPLE_PRINT(&tuple), role_id, hash);

    return true;
}

void vnet_session_update_tx(vnet_session_t *ss, const struct sk_buff *skb)
{
    if (unlikely(!ss || !skb)) {
        return;
    }

    write_lock_bh(&ss->lock);
    ss->statistics.packet_tx++;
    ss->statistics.traffic_tx += skb->len;
    ss->statistics.timestamp_tx = ktime_get_real_ns();
    write_unlock_bh(&ss->lock);
    return;
}

void vnet_session_update_rx(vnet_session_t *ss, const struct sk_buff *skb)
{
    u64 timestamp;

    if (unlikely(!ss || !skb)) {
        return;
    }

    timestamp = ktime_get_real_ns();
    write_lock_bh(&ss->lock);
    ss->statistics.packet_rx++;
    ss->statistics.traffic_rx += skb->len;
    if (ss->statistics.timestamp_rx != 0) {
        u64 interval = (timestamp - ss->statistics.timestamp_rx) / (VNET_RX_INTERVAL_STEP * NSEC_PER_MSEC);
        if (interval >= (VNET_RX_INTERVAL_NUM - 1))
            ss->statistics.rx_interval[VNET_RX_INTERVAL_NUM - 1]++;
        else
            ss->statistics.rx_interval[interval]++;
    }
    ss->statistics.timestamp_rx = timestamp;
    write_unlock_bh(&ss->lock);
    return;
}

void vnet_session_update_path_tx(vnet_session_t *ss, const struct sk_buff *skb, vnet_device_role_id_t role_id)
{
    if (unlikely(!ss || !skb)) {
        return;
    }

    write_lock_bh(&ss->lock);
    switch (role_id) {
        case VNET_ROLE_PRIMARY_MODEM:
            ss->statistics.packet_tx_wifi++;
            ss->statistics.traffic_tx += skb->len;
            break;
        case VNET_ROLE_SECONDARY_MODEM:
            ss->statistics.packet_tx_primary++;
            ss->statistics.traffic_tx_primary += skb->len;
            break;
        case VNET_ROLE_PRIMARY_WIFI:
            ss->statistics.packet_tx_secondary++;
            ss->statistics.traffic_tx_secondary += skb->len;
            break;
        default:
            break;
    }
    write_unlock_bh(&ss->lock);
    return;
}

void vnet_session_update_path_rx(vnet_session_t *ss, const struct sk_buff *skb, vnet_device_role_id_t role_id)
{
    u64 timestamp;
    u64 interval;

    if (unlikely(!ss || !skb)) {
        return;
    }

    timestamp = ktime_get_real_ns();
    write_lock_bh(&ss->lock);
    switch (role_id) {
        case VNET_ROLE_PRIMARY_MODEM:
            ss->statistics.packet_rx_primary++;
            ss->statistics.traffic_rx_primary += skb->len;
            interval = (timestamp - ss->statistics.timestamp_rx_primary) / (VNET_RX_INTERVAL_STEP * NSEC_PER_MSEC);
            if (interval >= (VNET_RX_INTERVAL_NUM - 1)) {
                ss->statistics.rx_interval_primary[VNET_RX_INTERVAL_NUM - 1]++;
            } else {
                ss->statistics.rx_interval_primary[interval]++;
            }
            ss->statistics.timestamp_rx_primary = timestamp;
            break;
        case VNET_ROLE_SECONDARY_MODEM:
            ss->statistics.packet_rx_secondary++;
            ss->statistics.traffic_rx_secondary += skb->len;
            interval = (timestamp - ss->statistics.timestamp_rx_secondary) / (VNET_RX_INTERVAL_STEP * NSEC_PER_MSEC);
            if (interval >= (VNET_RX_INTERVAL_NUM - 1)) {
                ss->statistics.rx_interval_secondary[VNET_RX_INTERVAL_NUM - 1]++;
            } else {
                ss->statistics.rx_interval_secondary[interval]++;
            }
            ss->statistics.timestamp_rx_secondary = timestamp;
            break;
        case VNET_ROLE_PRIMARY_WIFI:
            ss->statistics.packet_rx_wifi++;
            ss->statistics.traffic_rx_wifi += skb->len;
            interval = (timestamp - ss->statistics.timestamp_rx_wifi) / (VNET_RX_INTERVAL_STEP * NSEC_PER_MSEC);
            if (interval >= (VNET_RX_INTERVAL_NUM - 1)) {
                ss->statistics.rx_interval_wifi[VNET_RX_INTERVAL_NUM - 1]++;
            } else {
                ss->statistics.rx_interval_wifi[interval]++;
            }
            ss->statistics.timestamp_rx_wifi = timestamp;
            break;
        default:
            break;
    }

    write_unlock_bh(&ss->lock);
    return;
}

void vnet_session_mgr_start_statistics_print(void)
{
    if (!hrtimer_active(&g_statistics_print_timer)) {
        hrtimer_start(&g_statistics_print_timer, ms_to_ktime(VNET_STATISTICS_PRINT_MS), HRTIMER_MODE_REL);
    }
    return;
}

void vnet_session_mgr_stop_statistics_print(void)
{
    hrtimer_cancel(&g_statistics_print_timer);
    cancel_work_sync(&(g_statistics_print_work));
    return;
}

// Incremental reference count on a vnet_session_t
void vnet_session_get(vnet_session_t *ss)
{
    if (likely(ss)) {
        kref_get(&ss->ref);
    }
    return;
}

static void vnet_session_release(struct kref *kref)
{
    vnet_session_t *ss = container_of(kref, vnet_session_t, ref);
    vnet_free_session(ss);
}

// decrement reference count on a vnet_session_t
void vnet_session_put(vnet_session_t *ss)
{
    if (likely(ss)) {
        kref_put(&ss->ref, vnet_session_release);
    }
    return;
}

bool vnet_has_sessions(void)
{
    return atomic_read(&g_session_htable_count) != 0;
}

int vnet_session_mgr_init(void)
{
    vnet_log_info("virtual network session module init");

    if (vnet_init_uid_session_data() == ERROR) {
        vnet_log_err("uid_session data init failed");
        return ERROR;
    }

    if (vnet_init_session_data() == ERROR) {
        vnet_log_err("session data alloc failed, error");
        vnet_uninit_uid_session_data();
        return ERROR;
    }

    hrtimer_init(&g_statistics_print_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    g_statistics_print_timer.function = vnet_statistics_print_timer_func;
    INIT_WORK(&g_statistics_print_work, vnet_statistics_print_work_handle);
    return SUCCESS;
}

void vnet_session_mgr_exit(void)
{
    vnet_log_info("cloud network session module exit");

    vnet_session_mgr_stop_statistics_print();
    vnet_clear_all_session();
    vnet_clear_all_uid_session();
    vnet_uninit_session_data();
    vnet_uninit_uid_session_data();
    return;
}

void vnet_session_mgr_reset(void)
{
    vnet_log_info("cloud network session module exit");
    vnet_session_mgr_stop_statistics_print();
    vnet_clear_all_session();
    vnet_clear_all_uid_session();
    vnet_session_mgr_start_statistics_print();
    return;
}
