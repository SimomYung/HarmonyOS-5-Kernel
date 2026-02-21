/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LIBLINUX_NET_H__
#define __LIBLINUX_NET_H__

#include_next <linux/net.h>

#define TCP_EVENT_LIST_LEN 20
#define UDP_EVENT_LIST_LEN 4
#define SK_EVENT_INFO_MAX_LEN 1024

#define	EVENT_KERN_SND  6
#define	EVENT_KERN_RCV  6
#define	EVENT_USER_SYSCALL  4
#define	EVENT_TRANS_ERR 4

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
	SKB_TCP_RETRANSMIT_FAIL,
	SKB_TCP_RCV_NOBUF,
	SKB_TCP_RCV_ZEROWIN,
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

enum TCP_EVENT_FLAG {
	TCP_EVENT_START = 0x20,
	TCP_SEND,
	TCP_RCV,
	TCP_EVENT_SYSCALL,
	TCP_WRITE,
	TCP_READ,
	TCP_DELAY_EVENT,
	TCP_RETRANS,
	TCP_PROBE,
	TCP_PROBE0,
	TCP_TLP,
	TCP_REO,
	TCP_CWND_DEC,
	TCP_EVENT_MAX,
};

struct sk_event {
	u8 pos;
	u64 time;
	union {
		struct {
			u16 flag;
			u16 len;
			u32 seq;
			u32 ack_seq;
		};
		struct {
			u64 caller;
			u16 errno;
		};
	};
};

struct sk_event_list {
	struct rcu_head	rcu;
	atomic_t ridx;
	atomic_t widx;
	atomic_t uidx;
	atomic_t idx;
	u8 len;
	struct sk_event list[];
};

struct sk_event_info {
	u64 time;
	u32 seq;
	u32 peer_seq;
};

struct sock;
void pr_netlog(const char *fmt, ...);
void pr_hisysevent(const char *fmt, ...);
#ifdef CONFIG_HW_PACKET_FILTER_BYPASS
bool sk_is_fg(const struct sock *sk);
#else
static inline bool sk_is_fg(const struct sock *sk)
{
	return true;
}
#endif
void __hook_skb_drop_reason(const struct sock *sk, struct sk_buff *skb,
				int reason, void *func, void *caller);
#endif /* __LIBLINUX_NET_H__ */
