/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 */

#include <linux/sched.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0)
#include <linux/sockptr.h>
#endif
#include "smartcom/smartcom_schedule_driver.h"
#include "../smartcom_netlink.h"
#include "../smartcom_utils.h"
#include "schedule_drive_by_tuple.h"
#include <asm/uaccess.h>
#include <linux/fdtable.h>
#include <linux/in.h>
#include <linux/inet.h>
#include <linux/inetdevice.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/spinlock.h>
#include <linux/string.h>
#include <linux/version.h>
#include <net/inet_hashtables.h>
#include <net/inet6_hashtables.h>
#include <net/pkt_sched.h>
#include <net/sch_generic.h>
#include <net/sock.h>
#include <net/tcp.h>
#include <net/udp.h>

#include "securec.h"

#undef HWLOG_TAG
#define HWLOG_TAG smartcom_schedule_driver
HWLOG_REGIST();

struct smartcom_schedule_driver_acc_app_info_tuple_list g_current_tuple_list[SMARTCOM_MAX_ACC_APP_UID];
struct smartcom_schedule_driver_speed_ctrl_info g_speedctrl_info;
uid_t g_fastsyn_uid;
bool g_schedule_drive_hook_init = false;

struct smartcom_schedule_driver_netem_skb_cb {
	psched_time_t time_to_send;
	ktime_t tstamp_save;
};

static unsigned int schedule_driver_ipv4_hook_out(void *priv, struct sk_buff *skb,
	const struct nf_hook_state *state);

static unsigned int schedule_driver_ipv6_hook_out(void *priv, struct sk_buff *skb,
	const struct nf_hook_state *state);

static const struct nf_hook_ops schedule_driver_nfhooks[] = {
	{
		.hook = schedule_driver_ipv4_hook_out,
		.pf = PF_INET,
		.hooknum = NF_INET_LOCAL_OUT,
		.priority = NF_IP_PRI_FILTER - 1,
	},
	{
		.hook = schedule_driver_ipv6_hook_out,
		.pf = PF_INET,
		.hooknum = NF_INET_LOCAL_OUT,
		.priority = NF_IP_PRI_FILTER - 1,
	},
};

static inline bool invalid_app_uid(uid_t uid)
{
	/* if uid less than 200000, it is not an ohos apk */
	return (uid < UID_APP);
}


static inline bool invalid_speedctrl_size(uint32_t grade)
{
	/* the speed control grade bigger than 10000 */
	return (grade < SMARTCOM_SPEED_CTRL_BASE_WIN_SIZE);
}
static inline struct smartcom_schedule_driver_netem_skb_cb *smartcom_schedule_driver_netem_skb_cb(
	const struct sk_buff *skb)
{
	/* we assume we can use skb next/prev/tstamp as storage for rb_node */
	qdisc_cb_private_validate(skb, sizeof(struct smartcom_schedule_driver_netem_skb_cb));
	return (struct smartcom_schedule_driver_netem_skb_cb *)qdisc_skb_cb(skb)->data;
}

static void schedule_driver_register_nfhook(void)
{
	int ret;
	local_irq_disable();
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 1)
	ret = nf_register_net_hooks(&init_net, schedule_driver_nfhooks, ARRAY_SIZE(schedule_driver_nfhooks));
#else
	ret = nf_register_hooks(schedule_driver_nfhooks, ARRAY_SIZE(schedule_driver_nfhooks));
#endif
	local_irq_enable();
	if (ret == 0)
		smartcom_logi("schedule_driver register nf hooks successfully");
	else
		smartcom_loge("schedule_driver register nf hooks failed, ret:%d", ret);
}

static void schedule_driver_unregister_nfhook(void)
{
	local_irq_disable();
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 14, 1)
	nf_unregister_net_hooks(&init_net, schedule_driver_nfhooks, ARRAY_SIZE(schedule_driver_nfhooks));
#else
	nf_unregister_hooks(schedule_driver_nfhooks, ARRAY_SIZE(schedule_driver_nfhooks));
#endif
	local_irq_enable();
	smartcom_logi("schedule driver unregister nf hooks successfully");
}

static unsigned int schedule_driver_ipv4_hook_out(void *priv, struct sk_buff *skb,
	const struct nf_hook_state *state)
{
	smartcom_schedule_driver_set_acc_state_tuple(skb);
	return NF_ACCEPT;
}

static unsigned int schedule_driver_ipv6_hook_out(void *priv, struct sk_buff *skb,
	const struct nf_hook_state *state)
{
	smartcom_schedule_driver_set_acc_state_tuple(skb);
	return NF_ACCEPT;
}

void smartcom_schedule_driver_init(void)
{
	uint8_t index;
	uint8_t index_tuple;

	for (index = 0; index < SMARTCOM_MAX_ACC_APP_UID; index++) {
		for (index_tuple = 0; index_tuple < SMARTCOM_MAX_ACC_APP_TUPLE; index_tuple++) {
			g_current_tuple_list[index].flow_msg_list[index_tuple].flow_msg.uid = TUPLE_EMPTY_APP;
			g_current_tuple_list[index].flow_msg_list[index_tuple].age = 0;
		}
		g_current_tuple_list[index].uid = TUPLE_EMPTY_APP;
	}
	g_speedctrl_info.uid = UID_INVALID_APP;
	g_speedctrl_info.size = 0;
	spin_lock_init(&g_speedctrl_info.stlocker);

	g_fastsyn_uid = UID_INVALID_APP;
}

bool smartcom_schedule_driver_is_accuid(uid_t uid)
{
	uint8_t index;

	for (index = 0; index < SMARTCOM_MAX_ACC_APP_UID; index++) {
		if (uid == g_current_tuple_list[index].uid) {
			smartcom_logi("find acc uid [%d]", uid);
			return true;
		}
	}
	smartcom_logi("not find acc uid [%d]", uid);
	return false;
}

bool smartcom_schedule_driver_hook_ul_stub(struct sock *pstsock)
{
	uid_t sock_uid;

	if (pstsock == NULL) {
		smartcom_logd("param invalid");
		return false;
	}

	/**
	 * if uid equals current acc uid, accelerate it,else stop it
	 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 10)
	sock_uid = sock_i_uid(pstsock).val;
#else
	sock_uid = sock_i_uid(pstsock);
#endif

	return smartcom_schedule_driver_is_accuid(sock_uid);
}

int smartcom_schedule_driver_clear(void)
{
	uint8_t index;
	uint8_t index_tuple;
	for (index = 0; index < SMARTCOM_MAX_ACC_APP_UID; index++) {
		for (index_tuple = 0; index_tuple < SMARTCOM_MAX_ACC_APP_TUPLE; index_tuple++) {
			g_current_tuple_list[index].flow_msg_list[index_tuple].flow_msg.uid = TUPLE_EMPTY_APP;
			g_current_tuple_list[index].flow_msg_list[index_tuple].age = 0;
		}
		g_current_tuple_list[index].uid = TUPLE_EMPTY_APP;
	}
	smartcom_schedule_driver_set_speedctrl(&g_speedctrl_info, UID_INVALID_APP, 0);
	g_fastsyn_uid = UID_INVALID_APP;
	if (g_schedule_drive_hook_init) {
		schedule_driver_unregister_nfhook();
		g_schedule_drive_hook_init = false;
	}
	return 0;
}

/*
 * if the application is send packet, limit the other background  application
 * send pakcet rate according adjust the send wind
 */
void smartcom_schedule_driver_speedctrl_winsize(struct sock *pstsock, uint32_t *pstsize)
{
	uid_t sock_uid;
	uid_t uid;
	uint32_t size;

	if (pstsock == NULL) {
		smartcom_logd("param invalid\n");
		return;
	}

	if (pstsize == NULL) {
		smartcom_logd("window size invalid\n");
		return;
	}

	uid = g_speedctrl_info.uid;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 10)
	sock_uid = sock_i_uid(pstsock).val;
#else
	sock_uid = sock_i_uid(pstsock);
#endif
	if (invalid_app_uid(sock_uid))
		return;

	spin_lock_bh(&(g_speedctrl_info.stlocker));
	uid = g_speedctrl_info.uid;
	size = g_speedctrl_info.size;
	spin_unlock_bh(&(g_speedctrl_info.stlocker));
	/* check uid */
	if (sock_uid == uid)
		return;

	if (size)
		*pstsize = (g_speedctrl_info.size < *pstsize) ? g_speedctrl_info.size : *pstsize;
}

/*
 * when tcp need retrans sync packet, call this fucntion to
 * adjust the interval for the application
 */
void smartcom_schedule_driver_fastsyn(struct sock *pstsock)
{
	uid_t sock_uid;
	struct inet_connection_sock *icsk = NULL;

	if (pstsock == NULL) {
		smartcom_logd("param invalid");
		return;
	}
	if (pstsock->sk_state != TCP_SYN_SENT)
		return;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 1, 10)
	sock_uid = sock_i_uid(pstsock).val;
#else
	sock_uid = sock_i_uid(pstsock);
#endif
	if (invalid_app_uid(sock_uid))
		return;
	if (sock_uid != g_fastsyn_uid)
		return;

	icsk = inet_csk(pstsock);
	if (icsk->icsk_retransmits <= FAST_SYN_COUNT)
		icsk->icsk_rto = TCP_TIMEOUT_INIT;
}

/*
 * message proc , process every message for schedule_driver module
 */
void smartcom_schedule_driver_evt_proc(int32_t event, const void *data, uint16_t len)
{
	switch (event) {
	case NETLINK_SMARTCOM_DK_START_ACC:
		smartcom_logd("smartcom netlink receive acc start");
			smartcom_schedule_driver_start_acc_tuple(data, len);
		break;
	case NETLINK_SMARTCOM_DK_STOP_ACC:
		smartcom_logd("smartcom netlink receive acc stop");
			smartcom_schedule_driver_stop_acc_tuple(data, len);
		break;
	case NETLINK_SMARTCOM_DK_CLEAR:
		smartcom_logd("smartcom netlink receive clear info");
		smartcom_schedule_driver_clear();
		break;
	default:
		smartcom_logi("unsupport packet, the type is %d.\n", event);
		break;
	}
}

void smartcom_schedule_driver_set_speedctrl(struct smartcom_schedule_driver_speed_ctrl_info *speedctrl_info,
	uid_t uid_value, uint32_t size_value)
{
		spin_lock_bh(&(speedctrl_info->stlocker));
		speedctrl_info->uid = uid_value;
		speedctrl_info->size = size_value;
		spin_unlock_bh(&(speedctrl_info->stlocker));
}

bool tuple_is_equal(struct flow_msg_tuple *left, struct flow_msg_tuple *right)
{
	if (left == NULL || right == NULL)
		return false;
	if ((left->l3proto != right->l3proto) || (left->proto != right->proto))
		return false;
	if ((left->sport != right->sport) || (left->dport != right->dport))
		return false;
	if (left->uid != right->uid)
		return false;

	if (left->l3proto == ETH_P_IP)
		return (left->ipv4_sip == right->ipv4_sip) && (left->ipv4_dip == right->ipv4_dip);
	if (left->l3proto == ETH_P_IPV6)
#if IS_ENABLED(CONFIG_IPV6)
		return ipv6_addr_equal(&(left->ipv6_sip), &(right->ipv6_sip)) &&
			ipv6_addr_equal(&(left->ipv6_dip), &(right->ipv6_dip));
#endif
	return false;
}

bool tuple_copy(struct flow_msg_tuple *src, struct flow_msg_tuple *dst)
{
	if (dst == NULL || src == NULL)
		return false;
	dst->uid = src->uid;
	dst->sport = src->sport;
	dst->dport = src->dport;
	dst->proto = src->proto;

	dst->l3proto = src->l3proto;
	if (src->l3proto == ETH_P_IP) {
		dst->ipv4_sip = src->ipv4_sip;
		dst->ipv4_dip = src->ipv4_dip;
	} else if (src->l3proto == ETH_P_IPV6) {
#if IS_ENABLED(CONFIG_IPV6)
		memcpy_s(&(dst->ipv6_sip), sizeof(dst->ipv6_sip), &(src->ipv6_sip), sizeof(dst->ipv6_sip));
		memcpy_s(&(dst->ipv6_sip), sizeof(dst->ipv6_sip), &(src->ipv6_sip), sizeof(dst->ipv6_sip));
#endif
	} else {
		return false;
	}
	return true;
}

static uint8_t smartcom_schedule_driver_found_avaiable_accindex_tuple(struct flow_msg_tuple *flow_msg,
	uint8_t uid_index)
{
	uint8_t tuple_index;
	uint8_t idle_index = SMARTCOM_MAX_ACC_APP_TUPLE;
	uint8_t old_index = SMARTCOM_MAX_ACC_APP_TUPLE;
	uint16_t old_age = 0;

	for (tuple_index = 0; tuple_index < SMARTCOM_MAX_ACC_APP_TUPLE; tuple_index++) {
		if (tuple_is_equal(flow_msg, &(g_current_tuple_list[uid_index].flow_msg_list[tuple_index].flow_msg))) {
			return (uid_index * SMARTCOM_MAX_ACC_APP_TUPLE + tuple_index);
		} else if ((idle_index == SMARTCOM_MAX_ACC_APP_TUPLE) &&
			(g_current_tuple_list[uid_index].flow_msg_list[tuple_index].tuple == TUPLE_EMPTY_APP)) {
			idle_index = tuple_index; /* found first idle tuple */
			break;
		} else {
			g_current_tuple_list[uid_index].age++; /* to get the oldest tuple */
			if (g_current_tuple_list[uid_index].age > old_age) {
				old_age = g_current_tuple_list[uid_index].age;
				old_index = uid_index;
			}
		}
	}
	/* remove the too old acc tuple */
	if (old_age > SMARTCOM_UID_ACC_AGE_MAX) {
		g_current_tuple_list[uid_index].flow_msg_list[tuple_index].age = 0;
		g_current_tuple_list[uid_index].flow_msg_list[tuple_index].tuple  = TUPLE_EMPTY_APP;
		memset_s(&(g_current_tuple_list[uid_index].flow_msg_list[tuple_index].flow_msg),
			sizeof(struct smartcom_schedule_driver_acc_app_info_tuple), 0,
			sizeof(struct smartcom_schedule_driver_acc_app_info_tuple));
	}

	/* if it is new tuple, and has idle position , add it */
	if (idle_index < SMARTCOM_MAX_ACC_APP_TUPLE)
		return uid_index * SMARTCOM_MAX_ACC_APP_TUPLE + idle_index;
	/* if it is new uid, and acc list if full , replace the oldest */
	if (old_index < SMARTCOM_MAX_ACC_APP_TUPLE)
		return uid_index * SMARTCOM_MAX_ACC_APP_TUPLE + old_index;
	return (SMARTCOM_MAX_ACC_APP_TUPLE * SMARTCOM_MAX_ACC_APP_UID);
}

static uint8_t smartcom_schedule_driver_found_avaiable_accindex_uid(struct flow_msg_tuple *flow_msg)
{
	uint8_t uid_index;
	uint8_t idle_index = SMARTCOM_MAX_ACC_APP_UID;
	uint8_t old_index = SMARTCOM_MAX_ACC_APP_UID;
	uint16_t old_age = 0;

	/* check whether has the same tuple, and record the first idle position and the oldest position */
	for (uid_index = 0; uid_index < SMARTCOM_MAX_ACC_APP_UID; uid_index++) {
		if (g_current_tuple_list[uid_index].uid == flow_msg->uid) { /* found aim uid */
			return smartcom_schedule_driver_found_avaiable_accindex_tuple(flow_msg, uid_index);
		} else if ((idle_index == SMARTCOM_MAX_ACC_APP_UID)
			&& (g_current_tuple_list[uid_index].uid == TUPLE_EMPTY_APP)) { /* found first idle uid */
			idle_index = uid_index;
			break;
		} else { /* to get the oldest uid */
			g_current_tuple_list[uid_index].age++;
			if (g_current_tuple_list[uid_index].age > old_age) {
				old_age = g_current_tuple_list[uid_index].age;
				old_index = uid_index;
			}
		}
	}

	/* remove the too old acc uid */
	if (old_age > SMARTCOM_UID_ACC_AGE_MAX) {
		smartcom_logd("old_index:%d added too long, remove it", old_index);
		g_current_tuple_list[old_index].age = 0;
		g_current_tuple_list[old_index].uid  = TUPLE_EMPTY_APP;
		memset_s(&(g_current_tuple_list[old_index].flow_msg_list),
			SMARTCOM_MAX_ACC_APP_UID * sizeof(struct smartcom_schedule_driver_acc_app_info_tuple),
			0, SMARTCOM_MAX_ACC_APP_UID * sizeof(struct smartcom_schedule_driver_acc_app_info_tuple));
	}
	smartcom_logd("idle_index=%u, old_index=%u, old_age=%u", idle_index, old_index, old_age);

	/* if it is new uid, and has idle position , add it */
	if (idle_index < SMARTCOM_MAX_ACC_APP_UID)
		return idle_index * SMARTCOM_MAX_ACC_APP_TUPLE;
	/* if it is new uid, and acc list if full , replace the oldest */
	if (old_index < SMARTCOM_MAX_ACC_APP_UID)
		return old_index * SMARTCOM_MAX_ACC_APP_TUPLE;
	return (SMARTCOM_MAX_ACC_APP_TUPLE * SMARTCOM_MAX_ACC_APP_UID);
}

/*
 * start the special application use high priority queue, by check the 5-tuples
 */
int smartcom_schedule_driver_start_acc_tuple(const void *pdata, uint16_t len)
{
	struct flow_msg_tuple *flow_msg;
	uint8_t index;
	uint8_t uid_index;
	uint8_t tuple_index;

	/* input param check */
	if (pdata == NULL) {
		smartcom_loge("data is null");
		return -EINVAL;
	}

	if (!g_schedule_drive_hook_init) {
		schedule_driver_register_nfhook();
		g_schedule_drive_hook_init = true;
	}

	flow_msg = (struct flow_msg_tuple *)pdata;

	index = smartcom_schedule_driver_found_avaiable_accindex_uid(flow_msg);
	/* if it is new uid, and has idle position , add it to global data structure */
	if (index >= (SMARTCOM_MAX_ACC_APP_UID - 1) * (SMARTCOM_MAX_ACC_APP_TUPLE - 1))
		return 0;
	uid_index = index / SMARTCOM_MAX_ACC_APP_UID;
	tuple_index = index % SMARTCOM_MAX_ACC_APP_UID;
	spin_lock_bh(&(g_current_tuple_list[uid_index].flow_msg_list[tuple_index].tuple_lock));
	g_current_tuple_list[uid_index].uid = flow_msg->uid;
	g_current_tuple_list[uid_index].age = 0;
	g_current_tuple_list[uid_index].flow_msg_list[tuple_index].age = 0;
	g_current_tuple_list[uid_index].flow_msg_list[tuple_index].tuple = TUPLE_FILL_APP;
	if (!tuple_copy(flow_msg, &(g_current_tuple_list[uid_index].flow_msg_list[tuple_index].flow_msg))) {
		spin_unlock_bh(&(g_current_tuple_list[uid_index].flow_msg_list[tuple_index].tuple_lock));
		return -EINVAL;
	}
	struct flow_msg_tuple *flow_msg_elem = &(g_current_tuple_list[uid_index].flow_msg_list[tuple_index].flow_msg);
	smartcom_logd("start tuple acc : sport:[%u], dport:[%u], index:[%u][%u]",
		flow_msg_elem->sport, flow_msg_elem->dport, uid_index, tuple_index);
	spin_unlock_bh(&(g_current_tuple_list[uid_index].flow_msg_list[tuple_index].tuple_lock));
	return 0;
}

static bool smartcom_schedule_driver_is_acctuple(struct flow_msg_tuple *flow_msg)
{
	uint8_t index;
	uint8_t index_tuple;

	smartcom_logd("catch flow_msg uid:[%d], proto: [%u], sip[%x], dip:[%x], dport[%u], sport[%u]",
		flow_msg->uid, flow_msg->proto, flow_msg->ipv4_sip,
		flow_msg->ipv4_dip, flow_msg->dport, flow_msg->sport);

	for (index = 0; index < SMARTCOM_MAX_ACC_APP_UID; index++) {
		if (g_current_tuple_list[index].uid != flow_msg->uid)
			break;
		spin_lock_bh(&(g_current_tuple_list[index].list_lock));
		for (index_tuple = 0; index_tuple < SMARTCOM_MAX_ACC_APP_TUPLE; index_tuple++) {
			if (tuple_is_equal(flow_msg, &(g_current_tuple_list[index].flow_msg_list[index_tuple].flow_msg))) {
				spin_unlock_bh(&(g_current_tuple_list[index].list_lock));
				smartcom_logd("do acc tuple ");
				return true;
			}
		}
		spin_unlock_bh(&(g_current_tuple_list[index].list_lock));
	}
	smartcom_logd("not acc tuple ");
	return false;
}

static bool smartcom_schedule_driver_hook_ul_stub_tuple(struct sk_buff *skb)
{
	struct flow_msg_tuple flow_msg;
	struct iphdr *iph = NULL;
	struct udphdr *udph = NULL;
	struct tcphdr *tcph = NULL;

	if (skb == NULL) {
		smartcom_logd("param invalid");
		return false;
	}

	/**
	 * if 5-tuple equals current acc 5-tuple, accelerate it, else stop it
	 */
	iph = ip_hdr(skb);
	if (iph == NULL)
		return false;
	flow_msg.ipv4_sip = iph->saddr;
	flow_msg.ipv4_dip = iph->daddr;
	flow_msg.uid = sock_i_uid(skb->sk).val;
	if (iph->protocol == IPPROTO_UDP) {
		udph = udp_hdr(skb);
		flow_msg.proto = IPPROTO_UDP;
		flow_msg.sport = ntohs(udph->source);
		flow_msg.dport = ntohs(udph->dest);
	} else if (iph->protocol == IPPROTO_TCP) {
		tcph = tcp_hdr(skb);
		flow_msg.proto = IPPROTO_TCP;
		flow_msg.sport = ntohs(tcph->source);
		flow_msg.dport = ntohs(tcph->dest);
	} else {
		smartcom_logd("not tcp nor udp tuple");
		return false;
	}

	return smartcom_schedule_driver_is_acctuple(&flow_msg);
}

void smartcom_schedule_driver_set_acc_state_tuple(struct sk_buff *skb)
{
	if (skb == NULL) {
		smartcom_logd("skb is null");
		return;
	}
	struct sock * sk = skb_to_full_sk(skb);
	if (sk == NULL) {
		smartcom_logd("sk is null");
		return;
	}
	if (invalid_app_uid(sock_i_uid(sk).val))
		return;
	if (smartcom_schedule_driver_hook_ul_stub(sk)
		&& smartcom_schedule_driver_hook_ul_stub_tuple(skb)) {
		skb->mark = SMARTCOM_SCHEDULE_DRIVER_ACC_HIGH;
		sk->sk_mark = SMARTCOM_SCHEDULE_DRIVER_ACC_HIGH;
		smartcom_logd("do attach the acc tag: [%d] ", skb->mark);
	}
}

/*
 * stop the special application use high priority queue
 */
int smartcom_schedule_driver_stop_acc_tuple(const void *pdata, uint16_t len)
{
	struct flow_msg_tuple *flow_msg;
	uint8_t index;
	uint8_t index_tuple;

	/* input param check */
	if (pdata == NULL) {
		smartcom_loge("data is null");
		return -EINVAL;
	}

	flow_msg = (struct flow_msg_tuple *)pdata;

	/* remove specify uid */
	for (index = 0; index < SMARTCOM_MAX_ACC_APP_UID; index++) {
		if (g_current_tuple_list[index].uid != flow_msg->uid)
			break;
		spin_lock_bh(&(g_current_tuple_list[index].list_lock));
		for (index_tuple = 0; index_tuple < SMARTCOM_MAX_ACC_APP_TUPLE; index_tuple++) {
			if (!tuple_is_equal(flow_msg, &(g_current_tuple_list[index].flow_msg_list[index_tuple].flow_msg)))
				continue;
			g_current_tuple_list[index].flow_msg_list[index_tuple].age = 0;
			memset_s(&(g_current_tuple_list[index].flow_msg_list[index_tuple].flow_msg),
				sizeof(struct flow_msg_tuple), 0, sizeof(struct flow_msg_tuple));
			g_current_tuple_list[index].flow_msg_list[index_tuple].flow_msg.uid  = TUPLE_EMPTY_APP;
			smartcom_logd("stop the acc index :%u index_tuple: %u", index, index_tuple);
			break;
		}
		spin_unlock_bh(&(g_current_tuple_list[index].list_lock));
	}

	return 0;
}

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("schedule_driver module driver");
