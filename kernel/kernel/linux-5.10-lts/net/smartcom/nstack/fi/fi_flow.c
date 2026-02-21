/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 */

#include "fi_flow.h"
#include <linux/list.h>
#include <linux/string.h>
#include <linux/timer.h>
#include <linux/types.h>
#include <net/inet6_hashtables.h>
#include <net/inet_sock.h>
#include <net/ip.h>
#include <net/tcp.h>
#include "fi.h"
#include "fi_utils.h"

static struct fi_flow g_fi_ipv4_flow;
#if IS_ENABLED(CONFIG_IPV6)
static struct fi_flow g_fi_ipv6_flow;
#endif

struct sk_buff *fi_get_netlink_skb(int type, int length, char **data);
void fi_enqueue_netlink_skb(struct sk_buff *pskb);

uint32_t fi_ipv4_flow_node_num(void)
{
	return atomic_read(&g_fi_ipv4_flow.node_num);
}

void fi_ipv4_flow_lock(uint32_t lock_index)
{
	spin_lock_bh(&(g_fi_ipv4_flow.flow_lock[lock_index & FI_FLOW_TABLE_MASK]));
}

void fi_ipv4_flow_unlock(uint32_t lock_index)
{
	spin_unlock_bh(&(g_fi_ipv4_flow.flow_lock[lock_index & FI_FLOW_TABLE_MASK]));
}

uint32_t fi_ipv4_flow_hash(struct fi_pkt_parse *pkt_info, int str_len, bool exchange)
{
	int result;
	static u32 fi_ipv4_flow_secret __read_mostly;
	__be32 addr1 = pkt_info->msg.flow_msg.ipv4_sip;
	__u16 port1 = pkt_info->msg.flow_msg.sport;
	__be32 addr2 = pkt_info->msg.flow_msg.ipv4_dip;
	__be16 port2 = pkt_info->msg.flow_msg.dport;
	char *flow = pkt_info->msg.flow_msg.flow;

	net_get_random_once(&fi_ipv4_flow_secret, sizeof(fi_ipv4_flow_secret));
	if (exchange) {
		if (flow != NULL) {
			result = sprintf_s(flow, str_len, "%u%u%u%u", addr2, port2, addr1, port1);
			if (result == -1) {
				fi_loge("sprintf_s flow err, ret %d", result);
				flow = NULL;
			}
		}
		return __inet_ehashfn(addr2, port2, addr1, port1, fi_ipv4_flow_secret) & FI_FLOW_TABLE_MASK;
	}
	if (flow != NULL) {
		result = sprintf_s(flow, str_len, "%u%u%u%u", addr1, port1, addr2, port2);
		if (result == -1) {
			fi_loge("sprintf_s flow err, ret %d", result);
			flow = NULL;
		}
	}
	return __inet_ehashfn(addr1, port1, addr2, port2, fi_ipv4_flow_secret) & FI_FLOW_TABLE_MASK;
}

inline struct fi_flow_head *fi_ipv4_flow_header(uint32_t index)
{
	return &(g_fi_ipv4_flow.flow_table[index & FI_FLOW_TABLE_MASK]);
}

bool fi_ipv4_flow_same(struct fi_flow_node *node, __be32 src_addr, __u16 src_port, __be32 dst_addr, __be16 dst_port)
{
	return (((node->ipv4_saddr == src_addr)
			&& (node->ipv4_daddr == dst_addr)
			&& (node->sport == src_port)
			&& (node->dport == dst_port))
			|| ((node->ipv4_saddr == dst_addr)
			&& (node->ipv4_daddr == src_addr)
			&& (node->sport == dst_port)
			&& (node->dport == src_port)));
}

struct fi_flow_node *fi_ipv4_flow_find(const struct fi_flow_head *head,
			__be32 src_addr, __u16 src_port, __be32 dst_addr, __be16 dst_port)
{
	struct fi_flow_node *node = NULL;

	if (head != NULL) {
		list_for_each_entry(node, &(head->list), list) {
			if (fi_ipv4_flow_same(node, src_addr, src_port, dst_addr, dst_port)) {
				node->updatetime = jiffies_to_msecs(jiffies);
				return node;
			}
		}
	}
	return NULL;
}

static int fi_flow_ctx_init(struct fi_flow_node *new_node, fi_flow_cb *flow_cb)
{
	int i;
	int ret = memset_s(&new_node->flow_ctx, sizeof(struct fi_flow_ctx), 0, sizeof(struct fi_flow_ctx));
	if (ret) {
		fi_loge("memset_s failed]ret=%d", ret);
		return ret;
	}
	new_node->flow_ctx.last_up_timestamp = jiffies;
	new_node->flow_ctx.flow_stat.up_down_diff_time = -1;
	if (flow_cb != NULL) {
		for (i = 0; i < FI_DIR_BOTH; i++) {
			if (flow_cb->flow_identify_cfg->periodic[i].report_type == FI_RPT_TIME_CTRL) {
				int length = flow_cb->flow_identify_cfg->periodic[i].roi_stop -
					flow_cb->flow_identify_cfg->periodic[i].roi_start;
				if (length <= 0) {
					fi_logi("time interval config err]len=%d", length);
					flow_cb->flow_identify_cfg->periodic[i].report_type = FI_RPT_OFF;
					return -1;
				}
				new_node->flow_ctx.periodic_ctrl[i].ts_start = jiffies
					+ msecs_to_jiffies(flow_cb->flow_identify_cfg->periodic[i].roi_start);
				new_node->flow_ctx.periodic_ctrl[i].ts_end = new_node->flow_ctx.periodic_ctrl[i].ts_start
					+ msecs_to_jiffies(length);
			}
			if (flow_cb->flow_identify_cfg->pkt[i].report_type == FI_RPT_TIME_CTRL) {
				int length = flow_cb->flow_identify_cfg->pkt[i].roi_stop - flow_cb->flow_identify_cfg->pkt[i].roi_start;
				if (length <= 0) {
					fi_logi("time interval config err]len=%d", length);
					flow_cb->flow_identify_cfg->pkt[i].report_type = FI_RPT_OFF;
					return -1;
				}
				new_node->flow_ctx.pkt_ctrl[i].ts_start = jiffies
					+ msecs_to_jiffies(flow_cb->flow_identify_cfg->pkt[i].roi_start);
				new_node->flow_ctx.pkt_ctrl[i].ts_end = new_node->flow_ctx.pkt_ctrl[i].ts_start
					+ msecs_to_jiffies(length);
			}
		}
	}
	return ret;
}

struct fi_flow_node *fi_ipv4_flow_add(struct fi_flow_head *head, int32_t uid,
	struct fi_pkt_parse *pkt_info, struct sock *sk)
{
	__be32 saddr = pkt_info->msg.flow_msg.ipv4_sip;
	__u16 sport = pkt_info->msg.flow_msg.sport;
	__be32 daddr = pkt_info->msg.flow_msg.ipv4_dip;
	__be16 dport = pkt_info->msg.flow_msg.dport;
	fi_flow_cb flow_cb = pkt_info->flow_cb;

	struct fi_flow_node *new_node = NULL;
	int ret;
	if (head != NULL) {
		if (atomic_read(&g_fi_ipv4_flow.node_num) > FI_FLOW_NODE_LIMIT) {
			fi_loge("IPV4 flow node out of limited %d", FI_FLOW_NODE_LIMIT);
			return NULL;
		}
		new_node = (struct fi_flow_node *)kzalloc(sizeof(struct fi_flow_node), GFP_ATOMIC);
		if (new_node == NULL) {
			fi_loge("failed to malloc new IPV4 node for uid:%d", uid);
			return NULL;
		}
		ret = memset_s(new_node, sizeof(struct fi_flow_node), 0, sizeof(struct fi_flow_node));
		if (ret) {
			kfree(new_node);
			fi_loge("memset_s failed ret=%d", ret);
			return NULL;
		}
		new_node->ipv4_saddr = saddr;
		new_node->ipv4_daddr = daddr;
		new_node->sport = sport;
		new_node->dport = dport;
		new_node->uid = uid;
		new_node->updatetime = jiffies_to_msecs(jiffies);
		new_node->ip_flow_print_log_time = jiffies_to_msecs(jiffies) - FI_FLOW_PRINT_LOG_PERIOD;
		new_node->flow_qos_print_log_time = jiffies_to_msecs(jiffies) - FI_FLOW_PRINT_LOG_PERIOD;
		new_node->flow_cb = flow_cb;
		if (fi_flow_ctx_init(new_node, &flow_cb)) {
			fi_loge("fi_flow_ctx_init failed");
			kfree(new_node);
			return NULL;
		}
		if (sk != NULL && (sk->sk_protocol == IPPROTO_TCP || sk->sk_protocol == IPPROTO_UDP)) {
			new_node->sk = sk;
			sock_hold(sk);
		}
		list_add(&(new_node->list), &(head->list));
		atomic_inc(&g_fi_ipv4_flow.node_num);
		fi_logd("add a new IPV4 flow, uid:%d", uid);
	}
	return new_node;
}

struct fi_flow_node *fi_ipv4_flow_get(struct fi_flow_head *head, int32_t uid,
	struct fi_pkt_parse *pkt_info, bool add_flow, struct sock *sk)
{
	__be32 saddr = pkt_info->msg.flow_msg.ipv4_sip;
	__u16 sport = pkt_info->msg.flow_msg.sport;
	__be32 daddr = pkt_info->msg.flow_msg.ipv4_dip;
	__be16 dport = pkt_info->msg.flow_msg.dport;
	bool *newflow = &(pkt_info->msg.flow_msg.new_flow);

	struct fi_flow_node *node = NULL;

	if (head != NULL && newflow != NULL) {
		node = fi_ipv4_flow_find(head, saddr, sport, daddr, dport);
		if (node != NULL) {
			if (add_flow && sk && ((node->flow_ctx.flow_msg.dev_if != sk->sk_bound_dev_if) ||
				(node->sk && sk != node->sk))) {
				fi_logi("dev_if change rebuild flow, uid:%d port[%hu -- %hu] dev_if[%d -- %d]",
					uid, sport, dport, node->flow_ctx.flow_msg.dev_if, sk->sk_bound_dev_if);
				fi_ipv4_flow_clear_node(node);
				return NULL;
			} else {
				return node;
			}
		}
		if (add_flow) {
			if (sk && sk->sk_state >= TCP_FIN_WAIT1 && sk->sk_protocol == IPPROTO_TCP) {
				fi_logd("Ignore flow add:sk_state=%d", sk->sk_state);
				*newflow = false;
				return NULL;
			}
			*newflow = true;
			return fi_ipv4_flow_add(head, uid, pkt_info, sk);
		}
		*newflow = false;
	}
	return NULL;
}

void fi_ipv4_flow_clear_node(struct fi_flow_node *flow_node)
{
	list_del(&(flow_node->list));
	if (flow_node->sk != NULL) {
		sock_put(flow_node->sk);
		flow_node->sk = NULL;
	}
	kfree(flow_node);
	flow_node = NULL;
	atomic_dec(&g_fi_ipv4_flow.node_num);
}

void fi_ipv4_flow_age(void)
{
	uint32_t cur_time;
	struct fi_flow_head *flow_head = NULL;
	struct fi_flow_node *node = NULL;
	struct fi_flow_node *tmp_node = NULL;
	int i;

	cur_time = jiffies_to_msecs(jiffies);

	for (i = 0; i < FI_FLOW_TABLE_SIZE; i++) {
		flow_head = g_fi_ipv4_flow.flow_table + i;
		if (list_empty(&(flow_head->list))) {
			continue;
		}
		fi_ipv4_flow_lock(i);
		list_for_each_entry_safe(node, tmp_node, &(flow_head->list), list) {
			if (node->sk && ((node->sk->sk_protocol == IPPROTO_TCP && node->sk->sk_state >= TCP_FIN_WAIT1) ||
				node->flow_ctx.flow_msg.dev_if != node->sk->sk_bound_dev_if ||
				node->sk->sk_err == ECONNABORTED || sock_flag(node->sk, SOCK_DEAD))) {
				fi_logi("age free flow, node->uid=%d [%hu--%hu] uid=%d [%hu--%hu] protocol=%d sk_state=%d flag=%d",
					node->uid, node->sport, node->dport, node->flow_ctx.flow_msg.uid,
					node->flow_ctx.flow_msg.sport, node->flow_ctx.flow_msg.dport,
					node->sk->sk_protocol, node->sk->sk_state, sock_flag(node->sk, SOCK_DEAD));
				fi_ipv4_flow_clear_node(node);
			} else if (cur_time - node->updatetime > FI_FLOW_AGING_TIME) {
				fi_ipv4_flow_clear_node(node);
			}
		}
		fi_ipv4_flow_unlock(i);
	}
	return;
}

void fi_ipv4_flow_clear(void)
{
	struct fi_flow_head *flow_head = NULL;
	struct fi_flow_node *node = NULL;
	struct fi_flow_node *tmp_node = NULL;
	int i;

	for (i = 0; i < FI_FLOW_TABLE_SIZE; i++) {
		flow_head = g_fi_ipv4_flow.flow_table + i;
		if (list_empty(&(flow_head->list))) {
			continue;
		}
		fi_ipv4_flow_lock(i);
		list_for_each_entry_safe(node, tmp_node, &(flow_head->list), list) {
			fi_ipv4_flow_clear_node(node);
		}
		fi_ipv4_flow_unlock(i);
	}
	return;
}

void fi_ipv4_flow_clear_by_uid(int32_t uid)
{
	struct fi_flow_head *flow_head = NULL;
	struct fi_flow_node *node = NULL;
	struct fi_flow_node *tmp_node = NULL;
	int i;

	for (i = 0; i < FI_FLOW_TABLE_SIZE; i++) {
		flow_head = g_fi_ipv4_flow.flow_table + i;
		if (list_empty(&(flow_head->list))) {
			continue;
		}
		fi_ipv4_flow_lock(i);
		list_for_each_entry_safe(node, tmp_node, &(flow_head->list), list) {
			if (node->uid == uid)
				fi_ipv4_flow_clear_node(node);
		}
		fi_ipv4_flow_unlock(i);
	}
	return;
}

void fi_ipv4_flow_init(void)
{
	int index, result;

	result = memset_s(&g_fi_ipv4_flow, sizeof(g_fi_ipv4_flow), 0, sizeof(g_fi_ipv4_flow));
	if (result) {
		fi_loge("memset_s failed ret=%d", result);
		return;
	}
	atomic_set(&(g_fi_ipv4_flow.node_num), 0);
	for (index = 0; index < FI_FLOW_TABLE_SIZE; index++) {
		INIT_LIST_HEAD(&(g_fi_ipv4_flow.flow_table[index].list));
		spin_lock_init(&(g_fi_ipv4_flow.flow_lock[index]));
	}
	return;
}

#if IS_ENABLED(CONFIG_IPV6)
uint32_t fi_ipv6_flow_node_num(void)
{
	return atomic_read(&g_fi_ipv6_flow.node_num);
}

void fi_ipv6_flow_lock(uint32_t lock_index)
{
	spin_lock_bh(&(g_fi_ipv6_flow.flow_lock[lock_index & FI_FLOW_TABLE_MASK]));
}

void fi_ipv6_flow_unlock(uint32_t lock_index)
{
	spin_unlock_bh(&(g_fi_ipv6_flow.flow_lock[lock_index & FI_FLOW_TABLE_MASK]));
}

uint32_t fi_ipv6_flow_hash(struct fi_pkt_parse *pkt_info, int str_len, bool exchange)
{
	int result;
	static u32 fi_ipv6_flow_secret __read_mostly;
	static u32 fi_ipv6_flow_d_secret __read_mostly;
	u32 hash1;
	u32 hash2;
	struct in6_addr *addr1 = (struct in6_addr *)&(pkt_info->msg.flow_msg.ipv6_sip);
	__u16 port1 = pkt_info->msg.flow_msg.sport;
	struct in6_addr *addr2 = (struct in6_addr *)&(pkt_info->msg.flow_msg.ipv6_dip);
	__be16 port2 = pkt_info->msg.flow_msg.dport;
	char *flow = pkt_info->msg.flow_msg.flow;

	net_get_random_once(&fi_ipv6_flow_secret, sizeof(fi_ipv6_flow_secret));
	net_get_random_once(&fi_ipv6_flow_d_secret, sizeof(fi_ipv6_flow_d_secret));
	if (exchange) {
		hash2 = (__force u32)addr2->s6_addr32[3];
		hash1 = __ipv6_addr_jhash(addr1, fi_ipv6_flow_d_secret);
		if (flow != NULL) {
			result = sprintf_s(flow, str_len, "%u%u%u%u", (__force u32)addr2->s6_addr32[3],
				port2, (__force u32)addr1->s6_addr32[3], port1);
			if (result == -1) {
				fi_loge("sprintf_s flow err, ret %d", result);
				flow = NULL;
			}
		}
		return __inet6_ehashfn(hash2, port2, hash1, port1, fi_ipv6_flow_secret) & FI_FLOW_TABLE_MASK;
	}
	hash1 = (__force u32)addr1->s6_addr32[3];
	hash2 = __ipv6_addr_jhash(addr2, fi_ipv6_flow_d_secret);
	if (flow != NULL) {
		result = sprintf_s(flow, str_len, "%u%u%u%u", (__force u32)addr1->s6_addr32[3],
			port1, (__force u32)addr2->s6_addr32[3], port2);
		if (result == -1) {
			fi_loge("sprintf_s flow err, ret %d", result);
			flow = NULL;
		}
	}
	return __inet6_ehashfn(hash1, port1, hash2, port2, fi_ipv6_flow_secret) & FI_FLOW_TABLE_MASK;
}

inline struct fi_flow_head *fi_ipv6_flow_header(uint32_t table_index)
{
	return &(g_fi_ipv6_flow.flow_table[table_index & FI_FLOW_TABLE_MASK]);
}

bool fi_ipv6_flow_same(struct fi_flow_node *flow_node,
						const struct in6_addr *saddr, __u16 sport,
						const struct in6_addr *daddr, __be16 dport)
{
	return ((ipv6_addr_equal((const struct in6_addr *)(&(flow_node->ipv6_saddr)), saddr) &&
			ipv6_addr_equal((const struct in6_addr *)(&(flow_node->ipv6_daddr)), daddr) &&
			flow_node->sport == sport && flow_node->dport == dport) ||
			(ipv6_addr_equal((const struct in6_addr *)(&(flow_node->ipv6_saddr)), daddr) &&
			ipv6_addr_equal((const struct in6_addr *)(&(flow_node->ipv6_daddr)), saddr) &&
			flow_node->sport == dport && flow_node->dport == sport));
}

struct fi_flow_node *fi_ipv6_flow_find(const struct fi_flow_head *head,
			const struct in6_addr *saddr, __u16 sport,
			const struct in6_addr *daddr, __be16 dport)
{
	struct fi_flow_node *flow_node = NULL;

	if (saddr != NULL && daddr != NULL && head != NULL) {
		list_for_each_entry(flow_node, &(head->list), list) {
			if (fi_ipv6_flow_same(flow_node, saddr, sport, daddr, dport)) {
				flow_node->updatetime = jiffies_to_msecs(jiffies);
				return flow_node;
			}
		}
	}
	return NULL;
}

struct fi_flow_node *fi_ipv6_flow_add(struct fi_flow_head *head, int32_t uid,
	struct fi_pkt_parse *pkt_info, struct sock *sk)
{
	struct fi_flow_node *new_node = NULL;
	int ret;
	struct in6_addr *saddr = (struct in6_addr *)&(pkt_info->msg.flow_msg.ipv6_sip);
	__u16 sport = pkt_info->msg.flow_msg.sport;
	struct in6_addr *daddr = (struct in6_addr *)&(pkt_info->msg.flow_msg.ipv6_dip);
	__be16 dport = pkt_info->msg.flow_msg.dport;
	fi_flow_cb flow_cb = pkt_info->flow_cb;

	if (saddr != NULL && daddr != NULL && head != NULL) {
		if (atomic_read(&g_fi_ipv6_flow.node_num) > FI_FLOW_NODE_LIMIT) {
			fi_loge("IPV6 flow node out of limited %d", FI_FLOW_NODE_LIMIT);
			return NULL;
		}
		new_node = (struct fi_flow_node *)kzalloc(sizeof(struct fi_flow_node), GFP_ATOMIC);
		if (new_node == NULL) {
			fi_loge("failed to malloc new IPV6 node for uid %d", uid);
			return NULL;
		}
		ret = memset_s(new_node, sizeof(struct fi_flow_node), 0, sizeof(struct fi_flow_node));
		if (ret) {
			kfree(new_node);
			fi_loge("memset_s failed ret=%d", ret);
			return NULL;
		}

		new_node->ipv6_saddr = *saddr;
		new_node->ipv6_daddr = *daddr;
		new_node->sport = sport;
		new_node->dport = dport;
		new_node->uid = uid;
		new_node->updatetime = jiffies_to_msecs(jiffies);
		new_node->ip_flow_print_log_time = jiffies_to_msecs(jiffies) - FI_FLOW_PRINT_LOG_PERIOD;
		new_node->flow_qos_print_log_time = jiffies_to_msecs(jiffies) - FI_FLOW_PRINT_LOG_PERIOD;
		new_node->flow_cb = flow_cb;
		if (fi_flow_ctx_init(new_node, &flow_cb)) {
			fi_loge("fi_flow_ctx_init failed");
			kfree(new_node);
			return NULL;
		}

		if (sk != NULL && (sk->sk_protocol == IPPROTO_TCP || sk->sk_protocol == IPPROTO_UDP)) {
			new_node->sk = sk;
			sock_hold(sk);
		}
		list_add(&(new_node->list), &(head->list));
		atomic_inc(&g_fi_ipv6_flow.node_num);
	}
	return new_node;
}

struct fi_flow_node *fi_ipv6_flow_get(struct fi_flow_head *head, int32_t uid,
	struct fi_pkt_parse *pkt_info, bool add_flow, struct sock *sk)
{
	struct in6_addr *saddr = (struct in6_addr *)&(pkt_info->msg.flow_msg.ipv6_sip);
	__u16 sport = pkt_info->msg.flow_msg.sport;
	struct in6_addr *daddr = (struct in6_addr *)&(pkt_info->msg.flow_msg.ipv6_dip);
	__be16 dport = pkt_info->msg.flow_msg.dport;
	bool *newflow = &(pkt_info->msg.flow_msg.new_flow);

	struct fi_flow_node *node = NULL;

	if (saddr != NULL && daddr != NULL && head != NULL && newflow != NULL) {
		node = fi_ipv6_flow_find(head, saddr, sport, daddr, dport);
		if (node != NULL) {
			if (add_flow && sk && ((node->flow_ctx.flow_msg.dev_if != sk->sk_bound_dev_if) ||
				(node->sk && sk != node->sk))) {
				fi_logi("dev_if change rebuild flow, uid:%d port[%hu -- %hu] dev_if[%d -- %d]",
					uid, sport, dport, node->flow_ctx.flow_msg.dev_if, sk->sk_bound_dev_if);
				fi_ipv6_flow_clear_node(node);
				return NULL;
			} else {
				return node;
			}
		}
		if (add_flow) {
			if (sk && sk->sk_state >= TCP_FIN_WAIT1 && sk->sk_protocol == IPPROTO_TCP) {
				fi_logd("Ignore flow add:sk_state=%d", sk->sk_state);
				*newflow = false;
				return NULL;
			}
			*newflow = true;
			return fi_ipv6_flow_add(head, uid, pkt_info, sk);
		}
		*newflow = false;
	}
	return NULL;
}

void fi_ipv6_flow_clear_node(struct fi_flow_node *flow_node)
{
	list_del(&(flow_node->list));
	if (flow_node->sk != NULL) {
		sock_put(flow_node->sk);
		flow_node->sk = NULL;
	}
	kfree(flow_node);
	flow_node = NULL;
	atomic_dec(&g_fi_ipv6_flow.node_num);
}

void fi_ipv6_flow_age(void)
{
	uint32_t cur_time;
	struct fi_flow_head *flow_head = NULL;
	struct fi_flow_node *node = NULL;
	struct fi_flow_node *tmp_node = NULL;
	int i;

	cur_time = jiffies_to_msecs(jiffies);

	for (i = 0; i < FI_FLOW_TABLE_SIZE; i++) {
		flow_head = g_fi_ipv6_flow.flow_table + i;
		if (list_empty(&(flow_head->list))) {
			continue;
		}
		fi_ipv6_flow_lock(i);
		list_for_each_entry_safe(node, tmp_node, &(flow_head->list), list) {
			if (node->sk && ((node->sk->sk_protocol == IPPROTO_TCP && node->sk->sk_state >= TCP_FIN_WAIT1) ||
				node->flow_ctx.flow_msg.dev_if != node->sk->sk_bound_dev_if ||
				node->sk->sk_err == ECONNABORTED || sock_flag(node->sk, SOCK_DEAD))) {
				fi_logi("age free flow, node->uid=%d [%hu--%hu] uid=%d [%hu--%hu] protocol=%d sk_state=%d flag=%d",
					node->uid, node->sport, node->dport, node->flow_ctx.flow_msg.uid,
					node->flow_ctx.flow_msg.sport, node->flow_ctx.flow_msg.dport,
					node->sk->sk_protocol, node->sk->sk_state, sock_flag(node->sk, SOCK_DEAD));
				fi_ipv6_flow_clear_node(node);
			} else if (cur_time - node->updatetime > FI_FLOW_AGING_TIME) {
				fi_ipv6_flow_clear_node(node);
			}
		}
		fi_ipv6_flow_unlock(i);
	}
	return;
}

void fi_ipv6_flow_clear(void)
{
	struct fi_flow_head *flow_head = NULL;
	struct fi_flow_node *node = NULL;
	struct fi_flow_node *tmp_node = NULL;
	int i;

	for (i = 0; i < FI_FLOW_TABLE_SIZE; i++) {
		flow_head = g_fi_ipv6_flow.flow_table + i;
		if (list_empty(&(flow_head->list))) {
			continue;
		}
		fi_ipv6_flow_lock(i);
		list_for_each_entry_safe(node, tmp_node, &(flow_head->list), list) {
			fi_ipv6_flow_clear_node(node);
		}
		fi_ipv6_flow_unlock(i);
	}
	return;
}

void fi_ipv6_flow_clear_by_uid(int32_t uid)
{
	struct fi_flow_head *flow_head = NULL;
	struct fi_flow_node *node = NULL;
	struct fi_flow_node *tmp_node = NULL;
	int i;

	for (i = 0; i < FI_FLOW_TABLE_SIZE; i++) {
		flow_head = g_fi_ipv6_flow.flow_table + i;
		if (list_empty(&(flow_head->list))) {
			continue;
		}
		fi_ipv6_flow_lock(i);
		list_for_each_entry_safe(node, tmp_node, &(flow_head->list), list) {
			if (node->uid == uid)
				fi_ipv6_flow_clear_node(node);
		}
		fi_ipv6_flow_unlock(i);
	}
	return;
}

void fi_ipv6_flow_init(void)
{
	int index, result;

	result = memset_s(&g_fi_ipv6_flow, sizeof(g_fi_ipv6_flow), 0, sizeof(g_fi_ipv6_flow));
	if (result) {
		fi_loge("memset_s failed ret=%d", result);
		return;
	}

	atomic_set(&(g_fi_ipv6_flow.node_num), 0);
	for (index = 0; index < FI_FLOW_TABLE_SIZE; index++) {
		INIT_LIST_HEAD(&(g_fi_ipv6_flow.flow_table[index].list));
		spin_lock_init(&(g_fi_ipv6_flow.flow_lock[index]));
	}
	return;
}
#endif

bool fi_payload_len_filter_check(__be16 payload_len,
	uint32_t filter_size_start, uint32_t filter_size_stop)
{
	if (payload_len < filter_size_start || payload_len > filter_size_stop) {
		fi_logd("payloadlen not match[%u,%u], payloadlen=%hu", filter_size_start, filter_size_stop, payload_len);
		return false;
	}
	return true;
}

bool fi_payload_str_filter_check(struct fi_pkt_parse *pkt_info, fi_rpt_cfg *rpt_cfg)
{
	int index;
	bool is_udp_multi_frag = (pkt_info->msg.flow_msg.l4proto == IPPROTO_UDP && pkt_info->is_multi_frag);
	__be16 payloadlen = is_udp_multi_frag ?
		pkt_info->frag_payload_len : (__be16)pkt_info->parse_len;
	char *payload = (is_udp_multi_frag ? pkt_info->frag_payload : pkt_info->payload);

	/* If no filter set, default return match */
	if (!rpt_cfg->filter_str_len[0])
		return true;

	if (payload == NULL)
		return false;

	for (index = 0; index < FI_FLOW_FILTER_STR_MAX; index++) {
		if (!rpt_cfg->filter_str_len[index])
			break;
		if (rpt_cfg->filter_str_offset + rpt_cfg->filter_str_len[index] > payloadlen)
			continue;
		if (memcmp(rpt_cfg->filter_str[index],
				payload + rpt_cfg->filter_str_offset,
				rpt_cfg->filter_str_len[index]) == 0)
			return true;
	}
	return false;
}

void fi_flow_get_filesize(struct fi_flow_node *node)
{
	uint32_t download_bytes;
	struct tcp_sock *tp = tcp_sk(node->sk);
	if (unlikely(node->file_size.last_rcv_bytes == 0))
		return;

	if (tp->bytes_received < node->file_size.last_rcv_bytes)
		return;

	download_bytes = tp->bytes_received - node->file_size.last_rcv_bytes;
	if (download_bytes > node->file_size.size)
		node->file_size.size = download_bytes;
}

static void fi_flow_get_sk_info(struct fi_flow_node *node)
{
	if (node->sk != NULL) {
		if ((node->flow_ctx.flow_msg.dev_if != node->sk->sk_bound_dev_if) ||
		node->sk->sk_err == ECONNABORTED || sock_flag(node->sk, SOCK_DEAD)) {
			fi_logi("sock reset, port[%hu--%hu] l4proto[%d] sk_state[%d] dev_if[%d--%d] sk_err[%d] sk[%pK] flag[%d]",
				node->flow_ctx.flow_msg.sport, node->flow_ctx.flow_msg.dport,
				node->flow_ctx.flow_msg.l4proto, node->sk->sk_state,
				node->flow_ctx.flow_msg.dev_if, node->sk->sk_bound_dev_if,
				node->sk->sk_err, node->sk, sock_flag(node->sk, SOCK_DEAD));
			node->flow_ctx.flow_stat.sock_state = FLOW_RESET;
		} else {
			node->flow_ctx.flow_stat.sock_state = node->sk->sk_state;
		}

		if (node->sk->sk_protocol == IPPROTO_TCP) {
			const struct tcp_sock *tp = tcp_sk(node->sk);
			unsigned int in_flight = tcp_packets_in_flight(tp);
			if (tp->snd_cwnd > in_flight)
				node->flow_ctx.flow_stat.cwnd_left = tp->snd_cwnd - in_flight;
			else
				node->flow_ctx.flow_stat.cwnd_left = 0;
			node->flow_ctx.flow_stat.total_retrans = tp->total_retrans - node->flow_ctx.last_total_retrans;
			node->flow_ctx.last_total_retrans = tp->total_retrans;
		}
		node->flow_ctx.flow_msg.sk_mark = node->sk->sk_mark & 0xFFFF;
	} else {
		node->flow_ctx.flow_stat.sock_state = 0;
		node->flow_ctx.flow_msg.sk_mark = 0;
	}
}

static void fi_flow_raw_stat_update(struct fi_flow_node *node, const struct fi_pkt_parse *pkt_info,
	enum fi_dir dir)
{
	struct fi_flow_stat *flow_stat = &node->flow_ctx.flow_stat;
	bool is_udp_multi_frag = (pkt_info->msg.flow_msg.l4proto == IPPROTO_UDP && pkt_info->is_multi_frag);
	if (!pkt_info->flow_cb.flow_identify_cfg->basic_rpt_en)
		return;
	if (dir == FI_DIR_UP) {
		flow_stat->raw_up_pkt_num += (is_udp_multi_frag ? pkt_info->raw_frag_num : 1);
		flow_stat->raw_up_pkt_byte += pkt_info->msg.payload_len;
		flow_stat->up_byte += pkt_info->msg.total_len;
	} else {
		flow_stat->raw_down_pkt_num += (is_udp_multi_frag ? pkt_info->raw_frag_num : 1);
		flow_stat->raw_down_pkt_byte += pkt_info->msg.payload_len;
		flow_stat->down_byte += pkt_info->msg.total_len;
	}
}

static void fi_flow_stat_start(struct fi_flow_node *node, enum fi_dir dir)
{
	node->flow_ctx.stat_stop |= (dir == FI_DIR_UP) ? FI_FLOW_PERIODIC_REPORT_NEED_UP : FI_FLOW_PERIODIC_REPORT_NEED_DOWN;
}

static void fi_flow_stat_stop(struct fi_flow_node *node, enum fi_dir dir)
{
	node->flow_ctx.stat_stop |= (dir == FI_DIR_UP) ? FI_FLOW_PERIODIC_REPORT_STOP_UP :
		FI_FLOW_PERIODIC_REPORT_STOP_DOWN;

	node->flow_ctx.stat_stop &= (dir == FI_DIR_UP) ?
		~FI_FLOW_PERIODIC_REPORT_NEED_UP : ~FI_FLOW_PERIODIC_REPORT_NEED_DOWN;
}

static void fi_flow_stat_update(struct fi_flow_node *node, struct fi_pkt_parse *pkt_info,
	enum fi_dir dir)
{
	struct fi_flow_stat *flow_stat = &node->flow_ctx.flow_stat;
	bool is_udp_multi_frag = (pkt_info->msg.flow_msg.l4proto == IPPROTO_UDP && pkt_info->is_multi_frag);

	fi_flow_stat_start(node, dir);
	if (dir == FI_DIR_UP) {
		flow_stat->up_pkt_num += (is_udp_multi_frag ? pkt_info->filter_frag_num : 1);
		flow_stat->up_pkt_byte += pkt_info->msg.payload_len;
	} else {
		flow_stat->down_pkt_num += (is_udp_multi_frag ? pkt_info->filter_frag_num : 1);
		flow_stat->down_pkt_byte += pkt_info->msg.payload_len;
	}
}

static void fi_flow_stat_echo_delay(struct fi_flow_node *node, struct fi_pkt_parse *pkt_info,
	enum fi_dir dir)
{
	struct fi_flow_stat *flow_stat = &node->flow_ctx.flow_stat;
	fi_flow_identify_cfg *flow_cfg = pkt_info->flow_cb.flow_identify_cfg;
	if (!flow_cfg->periodic[dir].opt_enable[FI_ECHO_DELAY] && !flow_cfg->pkt[dir].opt_enable[FI_ECHO_DELAY])
		return;

	if (dir == FI_DIR_UP) {
		node->flow_ctx.last_up_timestamp = jiffies;
	} else {
		if (node->flow_ctx.last_up_timestamp) {
			flow_stat->up_down_diff_time = jiffies_to_msecs(jiffies - node->flow_ctx.last_up_timestamp);
			pkt_info->msg.up_down_diff_time = flow_stat->up_down_diff_time;
			node->flow_ctx.last_up_timestamp = 0;
		} else {
			pkt_info->msg.up_down_diff_time = -1;
		}
	}
}

static void fi_flow_stat_payload(struct fi_flow_node *node, struct fi_pkt_parse *pkt_info, enum fi_dir dir)
{
	int result;
	uint32_t stat_report_len;
	bool is_udp_multi_frag = (pkt_info->msg.flow_msg.l4proto == IPPROTO_UDP && pkt_info->is_multi_frag);
	uint32_t parse_len = is_udp_multi_frag ?
		(uint32_t)pkt_info->frag_payload_len : pkt_info->parse_len;
	char *payload = is_udp_multi_frag ?
		pkt_info->frag_payload : pkt_info->payload;
	struct fi_flow_stat *flow_stat = &node->flow_ctx.flow_stat;
	fi_flow_identify_cfg *flow_cfg = pkt_info->flow_cb.flow_identify_cfg;

	if (!flow_cfg->periodic[dir].opt_enable[FI_PAYLOAD_SEGMENT_PARSE] ||
		payload == NULL || !parse_len)
		return;
	if (flow_cfg->periodic[dir].seg_begin >= parse_len)
		return;

	stat_report_len = flow_cfg->periodic[dir].seg_end - flow_cfg->periodic[dir].seg_begin + 1;
	if (flow_cfg->periodic[dir].seg_end >= parse_len)
		stat_report_len = min(parse_len - flow_cfg->periodic[dir].seg_begin, (uint32_t)FI_PAYLOAD_SEG_LEN);

	result = memcpy_s(flow_stat->payload_seg[dir], sizeof(flow_stat->payload_seg[dir]),
			payload + flow_cfg->periodic[dir].seg_begin, stat_report_len);
	if (result)
		fi_loge("memcpy_s failed dir:%d, ret=%d, stat_report_len=%u. begin=%u, end=%u",
			dir, result, stat_report_len, flow_cfg->periodic[dir].seg_begin, flow_cfg->periodic[dir].seg_end);
	fi_logd("get period payload_seg is_udp_multi_frag:%d, parse_len:%u, stat_report_len:%u. begin=%u end=%u",
		is_udp_multi_frag, parse_len, stat_report_len,
		flow_cfg->periodic[dir].seg_begin, flow_cfg->periodic[dir].seg_end);
}

void fi_flow_statistics_time_ctrl(struct fi_flow_node *flow, struct fi_pkt_parse *pktinfo,
	enum fi_dir dir)
{
	if ((jiffies >= flow->flow_ctx.periodic_ctrl[dir].ts_start)
		&& (jiffies <= flow->flow_ctx.periodic_ctrl[dir].ts_end)) {
		fi_flow_stat_update(flow, pktinfo, dir);
	} else if (jiffies > flow->flow_ctx.periodic_ctrl[dir].ts_end) {
		fi_flow_stat_stop(flow, dir);
		fi_logd("flow:%s dir:%d reach time ctrl, add PERIODIC_REPORT_STOP flag", pktinfo->msg.flow_msg.flow, dir);
	}
}

void fi_flow_statistics_num_ctrl(struct fi_flow_node *flow, struct fi_pkt_parse *pktinfo,
	enum fi_dir dir)
{
	int num;
	fi_rpt_cfg *periodic_cfg = &pktinfo->flow_cb.flow_identify_cfg->periodic[dir];

	flow->flow_ctx.periodic_ctrl[dir].pkt_num++; // GSO may have more than one pkt!
	num = flow->flow_ctx.periodic_ctrl[dir].pkt_num;
	if ((num >= periodic_cfg->roi_start) && (num <= periodic_cfg->roi_stop)) {
		fi_flow_stat_update(flow, pktinfo, dir);
	} else if (num > periodic_cfg->roi_stop) {
		fi_flow_stat_stop(flow, dir);
		fi_logd("flow:%s dir:%d reach num ctrl, add PERIODIC_REPORT_STOP flag", pktinfo->msg.flow_msg.flow, dir);
	}
}

void fi_flow_statistics(struct fi_flow_node *node, struct fi_pkt_parse *pkt_info,
	enum fi_dir dir)
{
	int ret;
	bool filter_pass = false;
	fi_rpt_cfg *periodic_cfg = NULL;

	fi_flow_stat_echo_delay(node, pkt_info, dir);

	fi_flow_raw_stat_update(node, pkt_info, dir);

	if (node->flow_ctx.stat_stop & ((dir == FI_DIR_UP) ? FI_FLOW_PERIODIC_REPORT_STOP_UP :
		FI_FLOW_PERIODIC_REPORT_STOP_DOWN))
		return;

	if (pkt_info->msg.flow_msg.new_flow) {
		ret = memcpy_s(&node->flow_ctx.flow_msg, sizeof(struct fi_flow_msg),
			&pkt_info->msg.flow_msg, sizeof(struct fi_flow_msg));
		if (ret) {
			fi_loge("memcpy_s failed ret=%d", ret);
			return;
		}
	}

	spin_lock_bh(&(pkt_info->flow_cb.app_info->lock));

	periodic_cfg = &pkt_info->flow_cb.flow_identify_cfg->periodic[dir];
	if (fi_payload_len_filter_check(pkt_info->msg.payload_len,
			periodic_cfg->filter_pkt_size_start, periodic_cfg->filter_pkt_size_stop)
		&& fi_payload_str_filter_check(pkt_info, periodic_cfg)) {
		filter_pass = true;
		fi_flow_stat_payload(node, pkt_info, dir);
	}

	switch (periodic_cfg->report_type) {
	case FI_RPT_ALWAYS:
		if (filter_pass)
			fi_flow_stat_update(node, pkt_info, dir);
		break;
	case FI_RPT_TIME_CTRL:
		if (filter_pass)
			fi_flow_statistics_time_ctrl(node, pkt_info, dir);
		break;
	case FI_RPT_NUM_CTRL:
		if (filter_pass)
			fi_flow_statistics_num_ctrl(node, pkt_info, dir);
		break;
	default:
		node->flow_ctx.stat_stop |= ((dir == FI_DIR_UP) ? FI_FLOW_PERIODIC_REPORT_STOP_UP :
			FI_FLOW_PERIODIC_REPORT_STOP_DOWN);
		break;
	}
	spin_unlock_bh(&(pkt_info->flow_cb.app_info->lock));
	return;
}

static void fi_flow_stat_log(const struct fi_flow_node *node)
{
	if (node->flow_ctx.flow_msg.l3proto == ETH_P_IP) {
		fi_logi("dev: %s, sk_mark:%u, src_ip: "IPV4_FMT", src_port: %hu, dst_ip: "IPV4_FMT", dst_port: %hu, "
			"l4_proto: %u, sk_state: %u, up_pkt_num: %d, down_pkt_num: %d, up_byte: %d, down_byte: %d, "
			"raw_up_pkt_byte: %d, raw_down_pkt_byte: %d, rtt: %u, total_retrans: %u, sk: %pK",
			node->flow_ctx.flow_msg.dev, node->flow_ctx.flow_msg.sk_mark,
			ipv4_info(node->flow_ctx.flow_msg.ipv4_sip), node->flow_ctx.flow_msg.sport,
			ipv4_info(node->flow_ctx.flow_msg.ipv4_dip), node->flow_ctx.flow_msg.dport,
			node->flow_ctx.flow_msg.l4proto, node->flow_ctx.flow_stat.sock_state,
			node->flow_ctx.flow_stat.up_pkt_num, node->flow_ctx.flow_stat.down_pkt_num,
			node->flow_ctx.flow_stat.up_byte, node->flow_ctx.flow_stat.down_byte,
			node->flow_ctx.flow_stat.raw_up_pkt_byte, node->flow_ctx.flow_stat.raw_down_pkt_byte,
			node->flow_ctx.flow_stat.rtt, node->flow_ctx.flow_stat.total_retrans, node->sk);
#if IS_ENABLED(CONFIG_IPV6)
	} else {
		fi_logi("dev: %s, sk_mark:%u, src_ip: "IPV6_FMT", src_port: %hu, dst_ip: "IPV6_FMT", dst_port: %hu, "
			"l4_proto: %u, sk_state: %u, up_pkt_num: %d, down_pkt_num: %d, up_byte: %d, down_byte: %d, "
			"raw_up_pkt_byte: %d, raw_down_pkt_byte: %d, rtt: %u, total_retrans: %u, sk: %pK",
			node->flow_ctx.flow_msg.dev, node->flow_ctx.flow_msg.sk_mark,
			ipv6_info(node->flow_ctx.flow_msg.ipv6_sip), node->flow_ctx.flow_msg.sport,
			ipv6_info(node->flow_ctx.flow_msg.ipv6_dip), node->flow_ctx.flow_msg.dport,
			node->flow_ctx.flow_msg.l4proto, node->flow_ctx.flow_stat.sock_state,
			node->flow_ctx.flow_stat.up_pkt_num, node->flow_ctx.flow_stat.down_pkt_num,
			node->flow_ctx.flow_stat.up_byte, node->flow_ctx.flow_stat.down_byte,
			node->flow_ctx.flow_stat.raw_up_pkt_byte, node->flow_ctx.flow_stat.raw_down_pkt_byte,
			node->flow_ctx.flow_stat.rtt, node->flow_ctx.flow_stat.total_retrans, node->sk);
#endif
	}
}

static void fi_flow_stat_log_limited(struct fi_flow_node *flow_node)
{
	uint32_t cur_time = jiffies_to_msecs(jiffies);
	if (cur_time - flow_node->ip_flow_print_log_time >= FI_FLOW_PRINT_LOG_PERIOD) {
		fi_flow_stat_log(flow_node);
		flow_node->ip_flow_print_log_time = cur_time;
	}
}

bool fi_pkt_report_check(struct fi_flow_node *flow, struct fi_pkt_parse *pkt_info, enum fi_dir dir)
{
	bool enable = false;
	bool filter_pass = false;
	fi_rpt_cfg *pkt_cfg = NULL;

	if (flow->flow_ctx.stat_stop & ((dir == FI_DIR_UP) ? FI_FLOW_PKT_REPORT_STOP_UP :
		FI_FLOW_PKT_REPORT_STOP_DOWN))
		return enable;

	spin_lock_bh(&(pkt_info->flow_cb.app_info->lock));
	pkt_cfg = &pkt_info->flow_cb.flow_identify_cfg->pkt[dir];
	if (fi_payload_len_filter_check(pkt_info->msg.payload_len,
			pkt_cfg->filter_pkt_size_start, pkt_cfg->filter_pkt_size_stop)
		&& fi_payload_str_filter_check(pkt_info, pkt_cfg))
		filter_pass = true;

	switch (pkt_cfg->report_type) {
	case FI_RPT_ALWAYS:
		enable = true;
		break;
	case FI_RPT_TIME_CTRL:
		if ((jiffies >= flow->flow_ctx.pkt_ctrl[dir].ts_start)
			&& (jiffies <= flow->flow_ctx.pkt_ctrl[dir].ts_end)) {
			enable = true;
		} else if (jiffies > flow->flow_ctx.pkt_ctrl[dir].ts_end) {
			flow->flow_ctx.stat_stop |= ((dir == FI_DIR_UP) ? FI_FLOW_PKT_REPORT_STOP_UP :
				FI_FLOW_PKT_REPORT_STOP_DOWN);
			fi_logd("flow:%s dir:%d reach time ctrl, add PKT_REPORT_STOP flag", pkt_info->msg.flow_msg.flow, dir);
		}
		break;
	case FI_RPT_NUM_CTRL:
		if (filter_pass) {
			flow->flow_ctx.pkt_ctrl[dir].pkt_num++; // GSO may have more than one pkt!
			if ((flow->flow_ctx.pkt_ctrl[dir].pkt_num >= pkt_cfg->roi_start)
				&& (flow->flow_ctx.pkt_ctrl[dir].pkt_num <= pkt_cfg->roi_stop)) {
				enable = true;
			} else if (flow->flow_ctx.pkt_ctrl[dir].pkt_num > pkt_cfg->roi_stop) {
				flow->flow_ctx.stat_stop |= ((dir == FI_DIR_UP) ? FI_FLOW_PKT_REPORT_STOP_UP :
					FI_FLOW_PKT_REPORT_STOP_DOWN);
				fi_logd("flow:%s dir:%d reach num ctrl, add PKT_REPORT_STOP flag", pkt_info->msg.flow_msg.flow, dir);
			}
		}
		break;
	default:
		flow->flow_ctx.stat_stop |= ((dir == FI_DIR_UP) ? FI_FLOW_PKT_REPORT_STOP_UP :
				FI_FLOW_PKT_REPORT_STOP_DOWN);
		break;
	}
	spin_unlock_bh(&(pkt_info->flow_cb.app_info->lock));
	return (enable && filter_pass);
}

static bool fi_flow_get_stats(int num, const struct fi_flow *flow,
				struct sk_buff **pskb, struct report_local_var *local_var, char **data)
{
	struct fi_flow_node *flow_node = NULL;
	struct fi_flow_node *tmp_node = NULL;
	list_for_each_entry_safe(flow_node, tmp_node, &(flow->flow_table[num].list), list) {
		if ((flow_node->flow_ctx.stat_stop & FI_FLOW_PERIODIC_REPORT_NEED) ||
				flow_node->flow_cb.flow_identify_cfg->basic_rpt_en) {
			if (flow_node->flow_cb.flow_identify_cfg->basic_rpt_en)
				fi_flow_get_sk_info(flow_node);
			if (flow_node->flow_ctx.key_info_check_rst == FI_RPT_BLOCK &&
				flow_node->flow_ctx.flow_stat.sock_state < FLOW_FINISH)
				continue;
			if (local_var->offset >= local_var->size) {
				/* enqueue pre skb and alloc a new skb */
				fi_enqueue_netlink_skb(*pskb);
				local_var->total_left -= local_var->size;
				if (local_var->total_left <= 0) {
					fi_logi("exceed total size = %u", local_var->size);
					return false;
				}
				local_var->size = FI_MAX_ALIGN_SIZE;
				*pskb = fi_get_netlink_skb(FI_PERIODIC_MSG_RPT, local_var->size, data);
				if (*pskb == NULL) {
					fi_loge("fail to get netlink skb");
					return false;
				}
				local_var->offset = 0;
			}
			flow_node->flow_ctx.flow_stat.rtt = flow_node->qos.rtt;
			local_var->ret = memcpy_s(*data + local_var->offset, local_var->size - local_var->offset,
				&flow_node->flow_ctx.flow_stat, sizeof(struct fi_periodic_rpt_msg));
			if (local_var->ret) {
				fi_loge("memcpy_s failed ret=%d,size=%u,offset=%u",
					local_var->ret, local_var->size, local_var->offset);
				kfree_skb(*pskb);
				return false;
			}
			fi_flow_stat_log_limited(flow_node);
			local_var->ret = memset_s(&flow_node->flow_ctx.flow_stat,
				sizeof(struct fi_flow_stat), 0, sizeof(struct fi_flow_stat));
			if (local_var->ret)
				fi_loge("memset_s failed ret=%d", local_var->ret);
			flow_node->flow_ctx.flow_stat.up_down_diff_time = -1;
			local_var->offset += sizeof(struct fi_periodic_rpt_msg);
		}
	}
	return true;
}

void _fi_flow_stat_update_report(const struct fi_flow *flow, void (*flow_lock)(uint32_t),
	void (*flow_unlock)(uint32_t))
{
	int index;
	struct report_local_var local_var = {0};
	char *data = NULL;
	struct sk_buff *pskb = NULL;

	/*lint -e574*/
	local_var.total_left = atomic_read(&flow->node_num) * sizeof(struct fi_periodic_rpt_msg);
	if (!local_var.total_left) {
		fi_logd("no data need to do flow stat periodic report");
		return;
	}

	local_var.total_left = (local_var.total_left > FI_NETLINK_REPORT_MAX_NUM * FI_MAX_ALIGN_SIZE) ?
		(FI_NETLINK_REPORT_MAX_NUM * FI_MAX_ALIGN_SIZE) : local_var.total_left;
	local_var.size = (local_var.total_left > FI_MAX_ALIGN_SIZE) ? FI_MAX_ALIGN_SIZE : local_var.total_left;
	pskb = fi_get_netlink_skb(FI_PERIODIC_MSG_RPT, local_var.size, &data);
	if (pskb == NULL) {
		fi_loge("fail to malloc memory for data");
		return;
	}
	/*lint +e574*/

	for (index = 0; index < FI_FLOW_TABLE_SIZE; index++) {
		if (list_empty(&(flow->flow_table[index].list))) {
			continue;
		}

		(*flow_lock)(index);
		if (!fi_flow_get_stats(index, flow, &pskb, &local_var, &data)) {
			(*flow_unlock)(index);
			return;
		}
		(*flow_unlock)(index);
	}
	if (pskb != NULL) {
		if (local_var.offset == 0) {
			kfree_skb(pskb);
			return;
		} else if (local_var.offset < local_var.size) {
			struct nlmsghdr *nlh = (struct nlmsghdr *)(data - NLMSG_HDRLEN);
			nlh->nlmsg_len = local_var.offset + NLMSG_HDRLEN;
			fi_logd("need to adjust netlink len size=%u,offset=%u", local_var.size, local_var.offset);
		}
		fi_enqueue_netlink_skb(pskb);
	}
}

void fi_flow_stat_update_report(void)
{
	_fi_flow_stat_update_report(&g_fi_ipv4_flow,
		fi_ipv4_flow_lock, fi_ipv4_flow_unlock);
#if IS_ENABLED(CONFIG_IPV6)
	_fi_flow_stat_update_report(&g_fi_ipv6_flow,
		fi_ipv6_flow_lock, fi_ipv6_flow_unlock);
#endif
}

void fi_flow_qos_log(struct fi_flow_node *node)
{
	if (node->flow_ctx.flow_msg.l3proto == ETH_P_IP) {
		fi_logi("src_ip: "IPV4_FMT", src_port: %hu, dst_ip: "IPV4_FMT", dst_port: %hu, "
			"uid: %u, dev %s, rtt: %u, bw: %u, rcv_bytes: %u, file_size %u tcp_up_pkt_num %d",
			ipv4_info(node->flow_ctx.flow_msg.ipv4_sip), node->flow_ctx.flow_msg.sport,
			ipv4_info(node->flow_ctx.flow_msg.ipv4_dip), node->flow_ctx.flow_msg.dport,
			node->uid, node->flow_ctx.flow_msg.dev, node->qos.rtt, node->qos.bw_est,
			node->qos.rcv_bytes, node->file_size.size, node->flow_ctx.up_pkt_num);
#if IS_ENABLED(CONFIG_IPV6)
	} else {
		fi_logi("src_ip: "IPV6_FMT", src_port: %hu, dst_ip: "IPV6_FMT", dst_port: %hu, "
			"uid: %u, dev %s, rtt: %u, bw: %u, rcv_bytes: %u, file_size %u tcp_up_pkt_num %d",
			ipv6_info(node->flow_ctx.flow_msg.ipv6_sip), node->flow_ctx.flow_msg.sport,
			ipv6_info(node->flow_ctx.flow_msg.ipv6_dip), node->flow_ctx.flow_msg.dport,
			node->uid, node->flow_ctx.flow_msg.dev, node->qos.rtt, node->qos.bw_est,
			node->qos.rcv_bytes, node->file_size.size, node->flow_ctx.up_pkt_num);
#endif
	}
}

void fi_flow_qos_log_limited(struct fi_flow_node * node)
{
	uint32_t curtime = jiffies_to_msecs(jiffies);
	if (curtime - node->flow_qos_print_log_time >= FI_FLOW_PRINT_LOG_PERIOD) {
		fi_flow_qos_log(node);
		node->flow_qos_print_log_time = curtime;
	}
}

int fi_flow_qos_msg_update(const struct fi_flow_head *head, struct sk_buff **pskb,
	char **data, uint32_t *offset, uint32_t *size)
{
	int ret;
	struct fi_flow_node *node = NULL;
	struct fi_flow_node *tmp = NULL;
	int buff_num = 0;
	struct fi_qos_rpt_msg flow_qos_msg;
	list_for_each_entry_safe(node, tmp, &(head->list), list) {
		if (fi_flow_qos_rpt_enable(node)) {
			if (node->sk == NULL || node->sk->sk_protocol != IPPROTO_TCP
			|| (node->qos.rcv_bytes == 0 && node->sk->sk_state != TCP_SYN_SENT)
			|| (node->flow_ctx.up_pkt_num == 0 && node->sk->sk_state == TCP_SYN_SENT))
				continue;
			fi_rtt_update(node);
			if (node->qos.rtt == 0)
				continue;
			fi_bw_calc(node);
			fi_flow_get_filesize(node);
			if (node->flow_ctx.key_info_check_rst == FI_RPT_BLOCK)
				continue;
			fi_flow_qos_log_limited(node);
			if (*offset >= *size) {
				/* enqueue pre skb and alloc a new skb */
				fi_enqueue_netlink_skb(*pskb);
				buff_num += 1;
				if (buff_num >= FI_NETLINK_REPORT_MAX_NUM) {
					fi_logi("exceed max report num %u", FI_NETLINK_REPORT_MAX_NUM);
					return -1;
				}
				*pskb = fi_get_netlink_skb(FI_QOS_MSG_RPT, *size, data);
				if (*pskb == NULL)
					return -1;
				*offset = 0;
			}
			flow_qos_msg.flow_msg = node->flow_ctx.flow_msg;
			flow_qos_msg.qos = node->qos;
			flow_qos_msg.size = node->file_size.size;
			node->qos.rcv_bytes = 0;
			node->qos.bw_est = 0;
			node->flow_ctx.up_pkt_num = 0;
			ret = memcpy_s(*data + *offset, *size - *offset, &flow_qos_msg, sizeof(struct fi_qos_rpt_msg));
			if (ret) {
				fi_loge("memcpy_s failed ret=%d,size=%u,offset=%u", ret, *size, *offset);
				kfree_skb(*pskb);
				return -1;
			}
			*offset += sizeof(struct fi_qos_rpt_msg);
		}
	}
	return 0;
}

void _fi_flow_qos_update_report(const struct fi_flow *flow, void (*flow_lock)(uint32_t),
	void (*flow_unlock)(uint32_t))
{
	int index;
	uint32_t offset = 0;
	uint32_t size = (FI_NETLINK_BUF_MAX_SIZE / sizeof(struct fi_qos_rpt_msg)) * sizeof(struct fi_qos_rpt_msg);
	char *data = NULL;
	struct sk_buff *pskb = NULL;
	if (!atomic_read(&flow->node_num)) {
		fi_logd("no data need to do qos periodic report");
		return;
	}
	pskb = fi_get_netlink_skb(FI_QOS_MSG_RPT, size, &data);
	if (pskb == NULL) {
		fi_loge("fail to malloc memory for data");
		return;
	}

	for (index = 0; index < FI_FLOW_TABLE_SIZE; index++) {
		const struct fi_flow_head *head = flow->flow_table + index;
		if (list_empty(&(head->list))) {
			continue;
		}
		(*flow_lock)(index);
		if (fi_flow_qos_msg_update(head, &pskb, &data, &offset, &size) != 0) {
			(*flow_unlock)(index);
			return;
		}
		(*flow_unlock)(index);
	}
	if (offset == 0) {
		kfree_skb(pskb);
		return;
	}
	if (pskb != NULL) {
		if (offset < size) {
			struct nlmsghdr *nlh = (struct nlmsghdr *)(data - NLMSG_HDRLEN);
			nlh->nlmsg_len = offset + NLMSG_HDRLEN;
			fi_logd("need to adjust netlink len size=%u,offset=%u", size, offset);
		}
		fi_enqueue_netlink_skb(pskb);
	}
	return;
}

void fi_flow_qos_update_report(void)
{
	_fi_flow_qos_update_report(&g_fi_ipv4_flow,
		fi_ipv4_flow_lock, fi_ipv4_flow_unlock);
#if IS_ENABLED(CONFIG_IPV6)
	_fi_flow_qos_update_report(&g_fi_ipv6_flow,
		fi_ipv6_flow_lock, fi_ipv6_flow_unlock);
#endif
}
