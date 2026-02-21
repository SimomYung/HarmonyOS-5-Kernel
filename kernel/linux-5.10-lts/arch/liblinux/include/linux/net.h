/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBLINUX_NET_H__
#define __LIBLINUX_NET_H__

#include_next <linux/net.h>

enum skb_drop_reason {
	SKB_NOT_SPECIFIC = 0,
	/* protocol not specified */
	SKB_COMMON_START,
	SKB_ERROR_REPORT,
	BPF_DROP_SOCK,
	BPF_DROP_SKB,
	SKB_COMMON_END,
	/* tcp drop reason */
	SKB_TCP_START,
	SKB_TCP_INVALID_CSUM,
	SKB_TCP_SEND_RST,
	SKB_TCP_RCV_RST,
	SKB_TCP_PROBE_FAIL,
	SKB_TCP_END,
	/* ip drop reason */
	SKB_IP_START,
	SKB_NETFILTER_DROP,
	SKB_IP_NOROUTES,
	SKB_NEIGH_FAILED,
	SKB_NEIGH_QUEUEFULL,
	SKB_INVALID_MAC,
	SKB_QDISC_STOP,
	SKB_IP_END,
	SKB_MAC_START,
	SKB_CPU_BACKLOG_FULL,
	SKB_MAC_END,
	SKB_DROP_REASON_MAX,
};

struct sock;
void pr_netlog(const char *fmt, ...);
void pr_hisysevent(const char *fmt, ...);
#ifdef CONFIG_HW_PACKET_FILTER_BYPASS
bool sk_is_fg(const struct sock *sk);
#else
static inline bool sk_is_fg(const struct sock *sk)
{
	return false;
}
#endif

#endif /* __LIBLINUX_NET_H__ */
