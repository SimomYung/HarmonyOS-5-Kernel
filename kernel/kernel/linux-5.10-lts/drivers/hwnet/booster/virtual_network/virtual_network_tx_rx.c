/*
 * virtual_network_tx_rx.c
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

#include "virtual_network_tx_rx.h"
#include "virtual_network.h"
#include "virtual_network_utils.h"

#include <linux/netdevice.h>
#include <linux/device.h>
#include <linux/rbtree.h>
#include <linux/rwlock.h>
#include <linux/kref.h>
#include <net/ip.h>
#include <net/ipv6.h>
#include <net/udp.h>
#include <net/icmp.h>
#include <net/checksum.h>
#include <net/route.h>

typedef struct vnet_tx_rx_policy {
    struct kref ref;
    struct rb_node node;
    vnet_tx_rx_policy_cfg_t cfg;
} vnet_tx_rx_policy_t;

static __cacheline_aligned_in_smp rwlock_t g_tx_rx_policies_lock;
static __read_mostly struct rb_root g_tx_rx_policies = RB_ROOT;
static __read_mostly vnet_tx_rx_policy_t *g_default_policy = NULL;

static void vnet_tx_rx_policy_release(struct kref *kref)
{
    vnet_tx_rx_policy_t *policy = container_of(kref, vnet_tx_rx_policy_t, ref);
    kfree(policy);
}

static void vnet_tx_rx_policy_get(vnet_tx_rx_policy_t *policy)
{
    if (likely(policy)) {
        kref_get(&policy->ref);
    }
    return;
}

static void vnet_tx_rx_policy_put(vnet_tx_rx_policy_t *policy)
{
    if (likely(policy)) {
        kref_put(&policy->ref, vnet_tx_rx_policy_release);
    }
    return;
}

// should be called with write_lock_bh or read_lock_bh
static vnet_tx_rx_policy_t *vnet_search_tx_rx_policy(int policy_id)
{
    struct rb_node *node = g_tx_rx_policies.rb_node;
    while (node != NULL) {
        vnet_tx_rx_policy_t *policy = rb_entry(node, vnet_tx_rx_policy_t, node);
        if (policy_id < policy->cfg.policy_id) {
            node = node->rb_left;
        } else if (policy_id > policy->cfg.policy_id) {
            node = node->rb_right;
        } else {
            return policy;
        }
    }
    return NULL;
}

int vnet_register_tx_rx_policy(const vnet_tx_rx_policy_cfg_t *policy_cfg)
{
    struct rb_node **insert = NULL;
    struct rb_node *parent = NULL;
    vnet_tx_rx_policy_t *policy = NULL;

    if (unlikely((policy_cfg == NULL) || (policy_cfg->policy_id <= 0) || (policy_cfg->tx_match_func == NULL) ||
        (policy_cfg->tx_func == NULL) || (policy_cfg->rx_match_func == NULL) || (policy_cfg->rx_func == NULL))) {
        return ERROR;
    }

    policy = kzalloc(sizeof(vnet_tx_rx_policy_t), GFP_ATOMIC);
    if (unlikely(policy == NULL)) {
        vnet_log_err("alloc vnet_tx_rx_policy_t fail");
        return ERROR;
    }
    kref_init(&policy->ref);
    policy->cfg = *policy_cfg;

    write_lock_bh(&g_tx_rx_policies_lock);
    insert = &g_tx_rx_policies.rb_node;
    while (*insert != NULL) {
        vnet_tx_rx_policy_t *cur = rb_entry(*insert, vnet_tx_rx_policy_t, node);
        parent = *insert;
        if (policy_cfg->policy_id < cur->cfg.policy_id) {
            insert = &((*insert)->rb_left);
        } else if (policy_cfg->policy_id > cur->cfg.policy_id) {
            insert = &((*insert)->rb_right);
        } else {
            write_unlock_bh(&g_tx_rx_policies_lock);
            kfree(policy);
            vnet_log_warn("tx_rx policy %d has already register", policy_cfg->policy_id);
            return SUCCESS;
        }
    }

    rb_link_node(&policy->node, parent, insert);
    rb_insert_color(&policy->node, &g_tx_rx_policies);
    write_unlock_bh(&g_tx_rx_policies_lock);
    vnet_log_info("register tx_rx policy %d", policy_cfg->policy_id);
    return SUCCESS;
}

void vnet_unregister_tx_rx_policy(const vnet_tx_rx_policy_cfg_t *policy_cfg)
{
    vnet_tx_rx_policy_t *policy = NULL;

    if (unlikely(policy_cfg == NULL || policy_cfg->policy_id <= 0)) {
        return;
    }

    write_lock_bh(&g_tx_rx_policies_lock);
    policy = vnet_search_tx_rx_policy(policy_cfg->policy_id);
    if (policy != NULL) {
        vnet_log_info("unregister tx_rx policy %d", policy_cfg->policy_id);
        rb_erase(&policy->node, &g_tx_rx_policies);
        vnet_tx_rx_policy_put(policy);
    }
    write_unlock_bh(&g_tx_rx_policies_lock);
}

static void vnet_clear_tx_rx_policy(void)
{
    vnet_tx_rx_policy_t *policy = NULL;
    struct rb_node *node = NULL;

    write_lock_bh(&g_tx_rx_policies_lock);
    while (true) {
        node = rb_first(&g_tx_rx_policies);
        if (node == NULL) {
            break;
        }
        policy = rb_entry(node, vnet_tx_rx_policy_t, node);
        vnet_log_info("clear tx_rx policy %d", policy->cfg.policy_id);
        rb_erase(&policy->node, &g_tx_rx_policies);
        vnet_tx_rx_policy_put(policy);
    }
    g_tx_rx_policies = RB_ROOT;
    write_unlock_bh(&g_tx_rx_policies_lock);
    return;
}

static vnet_tx_rx_policy_t *vnet_get_tx_rx_policy(int policy_id)
{
    vnet_tx_rx_policy_t *policy = NULL;
    read_lock_bh(&g_tx_rx_policies_lock);
    policy = vnet_search_tx_rx_policy(policy_id);
    if (policy) {
        vnet_tx_rx_policy_get(policy);
    }
    read_unlock_bh(&g_tx_rx_policies_lock);
    return policy;
}

vnet_tx_rx_mode_t vnet_default_policy_match_func(struct sk_buff *skb)
{
    return VNET_TX_RX_MODE_NAT;
}

int vnet_default_policy_tx_func(struct sk_buff *skb, vnet_session_t *ss)
{
    return ERROR;
}

int vnet_default_policy_rx_func(struct sk_buff *skb, vnet_session_t *ss)
{
    return ERROR;
}

static int vnet_tx_rx_init_default_policy(void)
{
    vnet_tx_rx_policy_t *policy = kzalloc(sizeof(vnet_tx_rx_policy_t), GFP_ATOMIC);
    if (unlikely(policy == NULL)) {
        vnet_log_err("alloc default policy fail");
        return ERROR;
    }

    kref_init(&policy->ref);
    policy->cfg.policy_id = 0;
    policy->cfg.tx_match_func = vnet_default_policy_match_func;
    policy->cfg.tx_func = vnet_default_policy_tx_func;
    policy->cfg.rx_match_func = vnet_default_policy_match_func;
    policy->cfg.rx_func = vnet_default_policy_rx_func;
    g_default_policy = policy;
    return SUCCESS;
}

static void vnet_tx_rx_uninit_default_policy(void)
{
    if (likely(g_default_policy)) {
        vnet_tx_rx_policy_put(g_default_policy);
        g_default_policy = NULL;
    }
    return;
}

static inline bool vnet_l4proto_match(u8 l4proto)
{
    switch (l4proto) {
        case IPPROTO_TCP:
        case IPPROTO_UDP:
        case IPPROTO_ICMP:
            return true;
        default:
            return false;
    }
}

static bool vnet_ipv4_protocol_match(struct sk_buff *skb)
{
    struct iphdr *iph = NULL;
    if (!pskb_may_pull(skb, sizeof(struct iphdr))) {
        vnet_log_debug("pskb_may_pull failed");
        return false;
    }

    iph = ip_hdr(skb);
    if (iph->version != 4) {
        return false;
    }

    return vnet_l4proto_match(iph->protocol);
}

static bool vnet_ipv6_protocol_match(struct sk_buff *skb)
{
    return false;
}

static bool vnet_protocol_match(struct sk_buff *skb)
{
    switch(skb->protocol) {
        case htons(ETH_P_IP):
            return vnet_ipv4_protocol_match(skb);
        case htons(ETH_P_IPV6):
            return vnet_ipv6_protocol_match(skb);
        default:
            return false;
    }
}

static int vnet_skb_set_last_mac(struct sk_buff *skb, vnet_device_role_id_t role_id)
{
    struct iphdr *iph = ip_hdr(skb);
    int ihl = iph->ihl;
    const char *dest_eth_addr = vnet_get_device_mac(role_id);
    if (unlikely(dest_eth_addr == NULL)) {
        vnet_log_err("get net device mac failed, error");
        return ERROR;
    }

    skb->mac_len = ETH_HLEN;
    eth_header(skb, skb->dev, ETH_P_IP, dest_eth_addr, NULL, 0);
    skb_reset_mac_header(skb);
    skb_set_network_header(skb, ETH_HLEN);
    skb_set_transport_header(skb, ETH_HLEN + ihl * 4);
    return SUCCESS;
}

static int vnet_skb_set_mac(struct sk_buff *skb, vnet_device_role_id_t role_id)
{
    struct rtable *rt;
    struct neighbour *n;
    struct iphdr *iph;
    int ihl;

    iph = ip_hdr(skb);
    ihl = iph->ihl;
    rt = ip_route_output(dev_net(skb->dev), iph->daddr, 0, RT_TOS(0), skb->dev->ifindex);
    if ((IS_ERR(rt)) || (rt == NULL) || (rt->rt_gw4 == 0)) {
        vnet_log_warn("ip route output failed, error");
        return vnet_skb_set_last_mac(skb, role_id);
    }

    n = __ipv4_neigh_lookup(skb->dev, rt->rt_gw4);
    ip_rt_put(rt);
    if (IS_ERR(n)) {
        vnet_log_warn("ipv4 neigh lookup failed, error");
        return vnet_skb_set_last_mac(skb, role_id);
    }

    if (n->ha[0] != 0xe0 && n->ha[0] != 0xe8) {
        skb->mac_len = ETH_HLEN;
        eth_header(skb, skb->dev, ETH_P_IP, n->ha, NULL, 0);
        skb_reset_mac_header(skb);
        skb_set_network_header(skb, ETH_HLEN);
        skb_set_transport_header(skb, ETH_HLEN + ihl * 4);

        vnet_set_device_mac(role_id, n->ha);
        neigh_release(n);
    } else {
        neigh_release(n);
        vnet_log_warn("neighbour param is invalid, error");
        return vnet_skb_set_last_mac(skb, role_id);
    }

    return SUCCESS;
}

static int vnet_process_tcp(struct sk_buff *skb, __be32 addr, __be32 new_addr)
{
    struct iphdr *iph;
    struct tcphdr *tcph;
    int ihl;

    iph = ip_hdr(skb);
    ihl = iph->ihl * 4;

    if (!pskb_may_pull(skb, ihl + sizeof(struct tcphdr))) {
        vnet_log_warn("pskb_may_pull fail");
        return ERROR;
    }

    tcph = (struct tcphdr *)(((char *)iph) + ihl);
    inet_proto_csum_replace4(&tcph->check, skb, addr, new_addr, true);
    return SUCCESS;
}

static int vnet_process_udp(struct sk_buff *skb, __be32 addr, __be32 new_addr)
{
    struct iphdr *iph;
    struct udphdr *udph;
    int ihl;

    iph = ip_hdr(skb);
    ihl = iph->ihl * 4;

    if (!pskb_may_pull(skb, ihl + sizeof(struct udphdr))) {
        vnet_log_warn("pskb_may_pull fail");
        return ERROR;
    }

    udph = (struct udphdr *)(((char *)iph) + ihl);
    if (udph->check != 0 || skb->ip_summed == CHECKSUM_PARTIAL) {
        inet_proto_csum_replace4(&udph->check, skb, addr, new_addr, true);
        if (udph->check == 0) {
            udph->check = CSUM_MANGLED_0;
        }
    }
    return SUCCESS;
}

static int vnet_process_icmp_tx(struct sk_buff *skb, __be32 saddr, __be32 new_saddr)
{
    struct iphdr *iph;
    struct icmphdr *icmph;
    int ihl;

    iph = ip_hdr(skb);
    ihl = iph->ihl * 4;

    if (!pskb_may_pull(skb, ihl + sizeof(struct icmphdr))) {
        vnet_log_warn("pskb_may_pull fail");
        return ERROR;
    }

    icmph = (struct icmphdr *)(((char *)iph) + ihl);
    if (!icmp_is_err(icmph->type)) {
        vnet_log_warn("err icmph->type:%hhu", icmph->type);
        return SUCCESS;
    }

    if (!pskb_may_pull(skb, ihl + sizeof(struct icmphdr) + sizeof(struct iphdr))) {
        vnet_log_warn("pskb_may_pull fail");
        return ERROR;
    }

    iph = (void *)(icmph + 1);
    saddr = iph->saddr;
    iph->saddr = new_saddr;
    /* Fix up the inner checksums. */
    inet_proto_csum_replace4(&icmph->checksum, skb, saddr, new_saddr, false);
    return SUCCESS;
}

static int vnet_process_icmp_rx(struct sk_buff *skb, __be32 daddr, __be32 new_daddr)
{
    struct iphdr *iph;
    struct icmphdr *icmph;
    int ihl;

    iph = ip_hdr(skb);
    ihl = iph->ihl * 4;

    if (!pskb_may_pull(skb, ihl + sizeof(struct icmphdr))) {
        vnet_log_warn("pskb_may_pull fail");
        return ERROR;
    }

    icmph = (struct icmphdr *)(((char *)iph) + ihl);
    if (!icmp_is_err(icmph->type)) {
        vnet_log_warn("err icmph->type:%hhu", icmph->type);
        return SUCCESS;
    }

    if (!pskb_may_pull(skb, ihl + sizeof(struct icmphdr) + sizeof(struct iphdr))) {
        vnet_log_warn("pskb_may_pull fail");
        return ERROR;
    }

    iph = (void *)(icmph + 1);
    daddr = iph->daddr;
    iph->daddr = new_daddr;
    /* Fix up the inner checksums. */
    inet_proto_csum_replace4(&icmph->checksum, skb, daddr, new_daddr, false);
    return SUCCESS;
}

int vnet_skb_config_daddr(struct sk_buff *skb, __be32 new_daddr)
{
    struct iphdr *iph = ip_hdr(skb);
    __be32 daddr = iph->daddr;

    iph->daddr = new_daddr;
    csum_replace4(&iph->check, daddr, new_daddr);
    skb_reset_network_header(skb);
    skb_set_transport_header(skb, ip_hdr(skb)->ihl * 4);

    if ((iph->frag_off & htons(IP_DF)) == 0 && ((iph->frag_off & htons(IP_OFFSET)) != 0)) {
        return SUCCESS;
    }

    switch (iph->protocol) {
        case IPPROTO_TCP:
            return vnet_process_tcp(skb, daddr, new_daddr);

        case IPPROTO_UDP:
            return vnet_process_udp(skb, daddr, new_daddr);

        case IPPROTO_ICMP:
            return vnet_process_icmp_rx(skb, daddr, new_daddr);

        default:
            return ERROR;
    }
}

int vnet_skb_config_saddr(struct sk_buff *skb, __be32 new_saddr)
{
    struct iphdr *iph = ip_hdr(skb);
    __be32 saddr = iph->saddr;

    iph->saddr = new_saddr;
    csum_replace4(&iph->check, saddr, new_saddr);

    if ((iph->frag_off & htons(IP_DF)) == 0 && ((iph->frag_off & htons(IP_OFFSET)) != 0)) {
        return SUCCESS;
    }

    switch (iph->protocol) {
        case IPPROTO_TCP:
            return vnet_process_tcp(skb, saddr, new_saddr);

        case IPPROTO_UDP:
            return vnet_process_udp(skb, saddr, new_saddr);

        case IPPROTO_ICMP:
            return vnet_process_icmp_tx(skb, saddr, new_saddr);

        default:
            return ERROR;
    }
}

int vnet_skb_xmit(struct sk_buff *skb, vnet_session_t *ss, vnet_device_role_id_t role_id)
{
    __be32 new_saddr;

    if (unlikely(skb == NULL || ss == NULL)) {
        return ERROR;
    }

    skb->dev = vnet_get_device(role_id);
    new_saddr = vnet_select_addr(role_id);
    if (new_saddr == 0) {
        vnet_log_warn("can't select addr for role_id:%d", role_id);
        return ERROR;
    }

    if (vnet_skb_config_saddr(skb, new_saddr) != SUCCESS) {
        return ERROR;
    }

    if (!vnet_session_maybe_add_new_branch(ss, skb, role_id)) {
        vnet_log_warn("fail to add session branch uid:%u role_id:%d", ss->uid, role_id);
        return ERROR;
    }
    vnet_session_update_path_tx(ss, skb, role_id);

    if (vnet_skb_set_mac(skb, role_id) != SUCCESS) {
        return ERROR;
    }

    vnet_log_debug("uid:%u role_id:%d mac_len %hu mac_header:%hu network_header:%hu transport_header:%hu", ss->uid,
        role_id, skb->mac_len, skb->mac_header, skb->network_header, skb->transport_header);
    if (dev_queue_xmit_accel(skb, NULL) != NET_XMIT_SUCCESS) {
        vnet_log_warn("send skb loss, uid:%u role_id:%d", ss->uid, role_id);
    }

    return SUCCESS;
}

int vnet_skb_deliver(struct sk_buff *skb, vnet_session_t *ss)
{
    struct net_device *tun_dev = NULL;

    if (unlikely(skb == NULL || ss == NULL)) {
        return ERROR;
    }

    vnet_session_update_rx(ss, skb);

    tun_dev = vnet_get_tun_dev();
    if (tun_dev == NULL) {
        vnet_log_err("rx cannot find tun dev");
        return ERROR;
    }

    skb->dev = tun_dev;
    skb->skb_iif = tun_dev->ifindex;
    vnet_log_debug("uid:%u role_id:%d mac_len %hu mac_header %hu network_header %hu transport_header %hu", ss->uid, -1,
        skb->mac_len, skb->mac_header, skb->network_header, skb->transport_header);
    netif_rx_any_context(skb);
    return SUCCESS;
}

static int vnet_nat_tx(struct sk_buff *skb, vnet_session_t *ss)
{
    vnet_device_role_id_t role_id = vnet_get_default_device_role_id();
    if (role_id == VNET_ROLE_ERR) {
        return ERROR;
    }

    return vnet_skb_xmit(skb, ss, role_id);
}

static int vnet_nat_rx(struct sk_buff *skb, vnet_session_t *ss)
{
    if (vnet_skb_config_daddr(skb, ss->master.tuple.src.u3.ip) == ERROR) {
        return ERROR;
    }

    return vnet_skb_deliver(skb, ss);
}

static vnet_tx_rx_policy_t *vnet_get_match_policy(u32 uid)
{
    vnet_tx_rx_policy_t *policy;

    // get tx policy id by uid
    int policy_id = vnet_get_policy_for_uid(uid);
    if (policy_id < 0) {
        // uid isn't allowed
        return NULL;
    }

    if (policy_id == 0) {
        vnet_tx_rx_policy_get(g_default_policy);
        return g_default_policy;
    }

    policy = vnet_get_tx_rx_policy(policy_id);
    if (policy == NULL) {
        vnet_log_debug("policy %d not registered for uid %u, use default policy.", policy_id, uid);
        vnet_tx_rx_policy_get(g_default_policy);
        return g_default_policy;
    }

    return policy;
}

static int vnet_tx(struct sk_buff *skb, struct net_device *dev)
{
    vnet_session_t *ss;
    vnet_tx_rx_policy_t *policy;
    vnet_tx_rx_mode_t tx_mode;

    if (unlikely((skb == NULL) || (skb->sk == NULL) || (skb->dev == NULL))) {
        return ERROR;
    }

    if (!vnet_is_available() || !vnet_protocol_match(skb) || !vnet_tun_dev_skb_match(skb)) {
        return ERROR;
    }

    policy = vnet_get_match_policy(skb->sk->sk_uid.val);
    if (policy == NULL) {
        dev_kfree_skb_any(skb);
        return SUCCESS;
    }

    tx_mode = policy->cfg.tx_match_func(skb);
    if ((tx_mode != VNET_TX_RX_MODE_POLICY) && (tx_mode != VNET_TX_RX_MODE_NAT)) {
        vnet_log_warn("uid:%u tx_mode err", skb->sk->sk_uid.val);
        vnet_tx_rx_policy_put(policy);
        dev_kfree_skb_any(skb);
        return SUCCESS;
    }

    ss = vnet_get_session_for_tx(skb);
    if (ss == NULL) {
        vnet_log_warn("uid:%u get session failed", skb->sk->sk_uid.val);
        vnet_tx_rx_policy_put(policy);
        dev_kfree_skb_any(skb);
        return SUCCESS;
    }

    vnet_session_update_tx(ss, skb);
    if (tx_mode == VNET_TX_RX_MODE_POLICY) {
        if (policy->cfg.tx_func(skb, ss) != SUCCESS) {
            dev_kfree_skb_any(skb);
        }
    } else {
        if (vnet_nat_tx(skb, ss) != SUCCESS) {
            dev_kfree_skb_any(skb);
        }
    }
    vnet_session_put(ss);
    vnet_tx_rx_policy_put(policy);
    return SUCCESS;
}

static int vnet_rx(struct sk_buff *skb, struct net_device *dev)
{
    vnet_session_t *ss;
    vnet_tx_rx_policy_t *policy;
    vnet_tx_rx_mode_t rx_mode;
    vnet_device_role_id_t role_id;

    if (unlikely(skb == NULL || skb->dev == NULL)) {
        return GRO_NORMAL;
    }

    if (!vnet_is_available() || vnet_net_device_is_tun_dev(skb->dev) || !vnet_protocol_match(skb)) {
        return GRO_NORMAL;
    }

    role_id = vnet_get_role_id(skb->dev);
    ss = vnet_get_session_for_rx(skb, role_id);
    if (ss == NULL) {
        return GRO_NORMAL;
    }

    vnet_session_update_path_rx(ss, skb, role_id);
    policy = vnet_get_match_policy(ss->uid);
    if (policy == NULL) {
        vnet_log_warn("uid:%u no policy", ss->uid);
        dev_kfree_skb_any(skb);
        vnet_session_put(ss);
        return GRO_DROP;
    }

    rx_mode = policy->cfg.rx_match_func(skb);
    if ((rx_mode != VNET_TX_RX_MODE_POLICY) && (rx_mode != VNET_TX_RX_MODE_NAT)) {
        vnet_log_warn("uid:%u policy_id:%d rx_mode err", ss->uid, policy->cfg.policy_id);
        dev_kfree_skb_any(skb);
        vnet_tx_rx_policy_put(policy);
        vnet_session_put(ss);
        return GRO_DROP;
    }

    if (rx_mode == VNET_TX_RX_MODE_POLICY) {
        if (policy->cfg.rx_func(skb, ss) != SUCCESS) {
            dev_kfree_skb_any(skb);
        }
    } else {
        if (vnet_nat_rx(skb, ss) != SUCCESS) {
            dev_kfree_skb_any(skb);
        }
    }

    vnet_tx_rx_policy_put(policy);
    vnet_session_put(ss);
    return GRO_DROP;
}

static int vnet_register_data_process_hook(void)
{
#ifdef CONFIG_HW_CLOUD_NETWORK
    struct hw_cloud_network_ops ops;
    ops.rx_handle = vnet_rx;
    ops.tx_handle = vnet_tx;
    return cloud_network_op_register(&ops);
#else
    return 0;
#endif
}

static void vnet_unregister_data_process_hook(void)
{
#ifdef CONFIG_HW_CLOUD_NETWORK
    cloud_network_op_unregister();
#endif
    return;
}

int vnet_tx_rx_init(void)
{
    vnet_log_info("virtual network tx rx module init");
    rwlock_init(&g_tx_rx_policies_lock);
    if (vnet_tx_rx_init_default_policy() != SUCCESS) {
        return ERROR;
    }

    return vnet_register_data_process_hook();
}

void vnet_tx_rx_exit(void)
{
    vnet_unregister_data_process_hook();
    vnet_tx_rx_uninit_default_policy();
    vnet_clear_tx_rx_policy();
}
