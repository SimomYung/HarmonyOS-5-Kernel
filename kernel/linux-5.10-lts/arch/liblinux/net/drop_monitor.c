#include <linux/net.h>
#include <net/inet_sock.h>
#include <net/sock.h>
#include <linux/skbuff.h>
#include <net/tcp.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/bpf.h>
#include <linux/netdevice.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter_ipv6/ip6_tables.h>
#include <trace/hooks/liblinux.h>

#define IP4_HMASK(addr) ((addr & 0xff000000) >> 24)
#define IP4_NMASK(addr) (addr & 0xff)

#undef FN
#define FN(a, b) [a] = #b,

static const char * const drop_reasons[] = {
	FN(SKB_NOT_SPECIFIC, NOT_SPECIFIC)
	FN(BPF_DROP_SOCK, BPF_DROP_SOCK)
	FN(BPF_DROP_SKB, BPF_DROP_SKB)
	FN(SKB_ERROR_REPORT, SKB_ERROR_REPORT)
	FN(SKB_TCP_INVALID_CSUM, TCP_INVALID_CSUM)
	FN(SKB_TCP_SEND_RST, TCP_SEND_RST)
	FN(SKB_TCP_RCV_RST, TCP_RCV_RST)
	FN(SKB_TCP_PROBE_FAIL, TCP_PROBE_FAIL)
	FN(SKB_NETFILTER_DROP, NETFILTER_DROP)
	FN(SKB_IP_NOROUTES, IP_NOROUTES)
	FN(SKB_NEIGH_FAILED, NEIGH_FAILED)
	FN(SKB_NEIGH_QUEUEFULL, NEIGH_QUEUEFULL)
	FN(SKB_INVALID_MAC, INVALID_MAC)
	FN(SKB_QDISC_STOP, QDISC_STOP)
	FN(SKB_CPU_BACKLOG_FULL, CPU_BACKLOG_FULL)
};

static DEFINE_RATELIMIT_STATE(netlog_ratelimit_state, 1 * HZ, 3);
static DEFINE_RATELIMIT_STATE(netfilter_ratelimit_state, 1 * HZ, 10);
static DEFINE_RATELIMIT_STATE(route_ratelimit_state, 5 * HZ, 3);


#define netlog_ratelimited(is_fg, fmt, ...)                     \
do {								\
	if (__ratelimit(&netlog_ratelimit_state)) {		\
		pr_netlog(fmt, ##__VA_ARGS__);			\
		if (is_fg)					\
			pr_hisysevent(fmt, ##__VA_ARGS__);	\
	}							\
} while (0)

#define netfilterlog_ratelimited(is_fg, fmt, ...)         	\
do {								\
	if (__ratelimit(&netfilter_ratelimit_state)) {		\
		pr_netlog(fmt, ##__VA_ARGS__);			\
		if (is_fg)					\
			pr_hisysevent(fmt, ##__VA_ARGS__);	\
	}							\
} while (0)

#define routelog_ratelimited(is_fg, fmt, ...)                   \
do {								\
	if (__ratelimit(&route_ratelimit_state)) {		\
		pr_netlog(fmt, ##__VA_ARGS__);			\
		if (is_fg)                                      \
			pr_hisysevent(fmt, ##__VA_ARGS__);      \
	}							\
} while (0)


static bool is_fg(const struct sock *sk_ori, struct sk_buff *skb)
{
	const struct sock *sk;

	if (sk_ori)
		sk = sk_to_full_sk((struct sock *)sk_ori);
	else
		sk = skb ? sk_to_full_sk(skb->sk) : NULL;

	return sk_is_fg(sk);
}

static void backtrace_show(int reason, void *func, void *caller)
{
	netlog_ratelimited(false, "%s, %ps->%ps\n", drop_reasons[reason], func, caller);
}

static void transport_info_show(const struct sock *sk, int reason, void *func, void *caller)
{
	unsigned int mark = sk ? sk->sk_mark : 0;
	int err = sk ? sk->sk_err : 0;
	struct inet_sock *inet;

	if (!sk || !sk_fullsock(sk))
		return;

	inet = inet_sk(sk);
	if (sk->sk_family == AF_INET6) {
		netlog_ratelimited(is_fg(sk, NULL), "%s %ps->%ps, err:%d, mark:%x, sport %u dport %u "
				   "src6=*:%02x%02x\n",
				   drop_reasons[reason], func, caller, err, mark,
				   ntohs(inet->inet_sport), ntohs(inet->inet_dport),
				   sk->sk_v6_rcv_saddr.s6_addr[14], sk->sk_v6_rcv_saddr.s6_addr[15]);
	} else {
		netlog_ratelimited(is_fg(sk, NULL), "%s %ps->%ps, err:%d, mark:%x, sport %u dport %u src=%u.*.*.%u\n",
				    drop_reasons[reason], func, caller, err, mark,
				    ntohs(inet->inet_sport), ntohs(inet->inet_dport),
				    IP4_NMASK(inet->inet_saddr), IP4_HMASK(inet->inet_saddr));
	}
}

static void tcp_info_show(const struct sock *sk, struct sk_buff *skb, int reason,
			  void *func, void *caller)
{
	unsigned int seq = skb ? TCP_SKB_CB(skb)->seq : 0;
	struct inet_sock *inet;

	/* no_tcp_socket receive packet will send rst and call trace,
	 * Logs do not need to be printed in this scenario.
	 */
	if (!sk || (sk->sk_state == TCP_CLOSE))
		return;

	if (!sk_fullsock(sk)) {
		if (sk->sk_state == TCP_NEW_SYN_RECV) {
			struct request_sock *req = inet_reqsk(sk);
			transport_info_show(req->rsk_listener, reason, func, caller);
		}
		return;
	}

	inet = inet_sk(sk);
	netlog_ratelimited(is_fg(sk, skb), "%s %ps->%ps, seq %u, sk_state:%u, "
			   "mark:%x, sport %u dport %u\n",
			   drop_reasons[reason], func, caller, seq,
			   sk->sk_state, sk->sk_mark,
			   ntohs(inet->inet_sport), ntohs(inet->inet_dport));
}

static void ip6_hdr_show(const struct sock *sk, struct sk_buff *skb, int reason,
			 void *func, void *caller)
{
	char *devname = skb->dev ? skb->dev->name : "";
	struct ipv6hdr *ip6 = ipv6_hdr(skb);
	u16 sport = 0, dport = 0;

	if (ip6->nexthdr == IPPROTO_TCP) {
		struct tcphdr *tcph = tcp_hdr(skb);

		sport = tcph->source;
		dport = tcph->dest;
	} else if (ip6->nexthdr == IPPROTO_UDP) {
		struct udphdr *uh = udp_hdr(skb);

		sport = uh->source;
		dport = uh->dest;
	}

	netlog_ratelimited(is_fg(sk, skb), "%s %ps->%ps dev:%s sp=%u dp=%u src6=*:%02x%02x dst6=*:%02x%02x\n",
			   drop_reasons[reason], func, caller, devname,
			   ntohs(sport), ntohs(dport),
			   ip6->saddr.s6_addr[14], ip6->saddr.s6_addr[15],
			   ip6->daddr.s6_addr[14], ip6->daddr.s6_addr[15]);
}

static void ip_hdr_show(const struct sock *sk, struct sk_buff *skb, int reason,
			void *func, void *caller)
{
	char *devname = skb->dev ? skb->dev->name : "";
	struct iphdr *ip = ip_hdr(skb);
	u16 sport = 0, dport = 0;

	if (ip->protocol == IPPROTO_TCP) {
		struct tcphdr *tcph = tcp_hdr(skb);

		sport = tcph->source;
		dport = tcph->dest;
	} else if (ip->protocol == IPPROTO_UDP) {
		struct udphdr *uh = udp_hdr(skb);

		sport = uh->source;
		dport = uh->dest;
	}

	netlog_ratelimited(is_fg(sk, skb), "%s %ps->%ps dev:%s sp=%u dp=%u "
			    "src=%u.*.*.%u dst=%u.*.*.%u\n",
			    drop_reasons[reason], func, caller, devname,
			    ntohs(sport), ntohs(dport),
			    IP4_NMASK(ip->saddr), IP4_HMASK(ip->saddr),
			    IP4_NMASK(ip->daddr), IP4_HMASK(ip->daddr));
}

static void arp_hdr_show(const struct sock *sk, struct sk_buff *skb, int reason,
			 void *func, void *caller)
{
	struct arphdr *arp = arp_hdr(skb);
	struct net_device *dev = skb->dev;
	unsigned char *arp_ptr;
	__be32 sip;

	if (arp->ar_pro != htons(ETH_P_IP) || !dev) {
		backtrace_show(reason, func, caller);
		return;
	}

	arp_ptr = (unsigned char *)(arp + 1);
	arp_ptr += dev->addr_len;
	memcpy(&sip, arp_ptr, 4);
	netlog_ratelimited(is_fg(sk, skb), "%s %ps->%ps dev:%s type:%u src=%u.*.*.%u\n",
			   drop_reasons[reason], func, caller, dev->name, arp->ar_op,
			   IP4_NMASK(sip), IP4_HMASK(sip));
}

static void ip_info_show(const struct sock *sk, struct sk_buff *skb,
			 int reason, void *func, void *caller)
{
	if (skb == NULL) {
		backtrace_show(reason, func, caller);
		return;
	}

	if (skb->protocol == htons(ETH_P_IP))
		ip_hdr_show(sk, skb, reason, func, caller);
	else if (skb->protocol == htons(ETH_P_IPV6))
		ip6_hdr_show(sk, skb, reason, func, caller);
	else if (skb->protocol == htons(ETH_P_ARP))
		arp_hdr_show(sk, skb, reason, func, caller);
}

static void mac_info_show(const struct sock *sk, struct sk_buff *skb,
		     int reason, void *func, void *caller)
{
	char *devname = skb->dev ? skb->dev->name : "";

	netlog_ratelimited(is_fg(sk, skb), "%s %ps->%ps dev:%s, proto:%x\n",
			   drop_reasons[reason], func, caller, devname, skb->protocol);
}

static void __hook_skb_drop_reason(const struct sock *sk, struct sk_buff *skb,
			    int reason, void *func, void *caller)
{
	if (sk == NULL && skb == NULL)
		return;

	if ((reason > SKB_COMMON_START) && (reason < SKB_COMMON_END)) {
		void *upper_caller = __builtin_return_address(4);

		transport_info_show(sk, reason, caller, upper_caller);
		return;
	}

	if ((reason > SKB_TCP_START) && (reason < SKB_TCP_END)) {
		tcp_info_show(sk, skb, reason, func, caller);
		return;
	}

	if ((reason > SKB_IP_START) && (reason < SKB_IP_END)) {
		ip_info_show(sk, skb, reason, func, caller);
		return;
	}

	if ((reason > SKB_MAC_START) && (reason < SKB_MAC_END)) {
		mac_info_show(sk, skb, reason, func, caller);
		return;
	}
}

static void hook_skb_drop_reason(void *__data, const struct sock *sk,
					struct sk_buff *skb, int reason)
{
	void *func = __builtin_return_address(1);
	void *caller = __builtin_return_address(2);
	__hook_skb_drop_reason(sk, skb, reason, func, caller);
}
INIT_VENDOR_HOOK(ldk_rvh_skb_drop_reason, hook_skb_drop_reason);

static void hook_netfilter_drop_reason(void *__data, const struct sock *sk,
					struct sk_buff *skb, void *hookfn)
{
	void *upper_caller = __builtin_return_address(3);
	__hook_skb_drop_reason(sk, skb, SKB_NETFILTER_DROP, hookfn, upper_caller);
}
INIT_VENDOR_HOOK(ldk_rvh_netfilter_drop_reason, hook_netfilter_drop_reason);

static const char *const chainnames[] = {
	[NF_INET_PRE_ROUTING] = "PREROUTING",
	[NF_INET_LOCAL_IN] = "INPUT",
	[NF_INET_FORWARD] = "FORWARD",
	[NF_INET_LOCAL_OUT] = "OUTPUT",
	[NF_INET_POST_ROUTING] = "POSTROUTING",
};

static inline struct ipt_entry *
get_ipt_entry(const void *base, unsigned int offset)
{
	return (struct ipt_entry *)(base + offset);
}

static inline struct ip6t_entry *
get_ip6t_entry(const void *base, unsigned int offset)
{
	return (struct ip6t_entry *)(base + offset);
}

static inline struct xt_entry_target *
ipt_get_target_c(const struct ipt_entry *e)
{
	return ipt_get_target((struct ipt_entry *)e);
}

static inline struct xt_entry_target *
ip6t_get_target_c(const struct ip6t_entry *e)
{
	return ip6t_get_target((struct ip6t_entry *)e);
}

static void hook_trace_iptable_drop(void *__data, unsigned int hook,
			    const char *tablename, const struct xt_table_info *private,
			    const struct ipt_entry *e, struct sk_buff *skb)
{
	const void *root;
	const char *hookname, *chainname;
	const struct ipt_entry *iter;

	if (hook >= NF_INET_NUMHOOKS)
		return;

	root = get_ipt_entry(private->entries, private->hook_entry[hook]);

	hookname = chainname = chainnames[hook];

	xt_entry_foreach(iter, root, private->size - private->hook_entry[hook]) {
		const struct xt_standard_target *t = (void *)ipt_get_target_c(iter);
		if (strcmp(t->target.u.kernel.target->name, XT_ERROR_TARGET) == 0) {
			chainname = t->target.data;
		} else if (iter == e)
			break;
	}

	netfilterlog_ratelimited(is_fg(NULL, skb), "NF_DROP: iptable:%s hook:%s chain:%s\n",
			    tablename, hookname, chainname);
}
INIT_VENDOR_HOOK(ldk_rvh_trace_iptable_drop, hook_trace_iptable_drop);

static void hook_trace_iptable6_drop(void *__data, unsigned int hook,
			    const char *tablename, const struct xt_table_info *private,
			    const struct ip6t_entry *e, struct sk_buff *skb)
{
	const void *root;
	const char *hookname, *chainname;
	const struct ip6t_entry *iter;

	if (hook >= NF_INET_NUMHOOKS)
		return;

	root = get_ip6t_entry(private->entries, private->hook_entry[hook]);

	hookname = chainname = chainnames[hook];

	xt_entry_foreach(iter, root, private->size - private->hook_entry[hook]) {
		const struct xt_standard_target *t = (void *)ip6t_get_target_c(iter);
		if (strcmp(t->target.u.kernel.target->name, XT_ERROR_TARGET) == 0) {
			chainname = t->target.data;
		} else if (iter == e)
			break;
	}

	netfilterlog_ratelimited(is_fg(NULL, skb), "NF_DROP: ip6table:%s hook:%s chain:%s\n",
			    tablename, hookname, chainname);
}
INIT_VENDOR_HOOK(ldk_rvh_trace_iptable6_drop, hook_trace_iptable6_drop);

static void flowi4_info_show(struct flowi4 *fl4, void *func, void *caller, int err, bool fg)
{
	routelog_ratelimited(fg, "%s err:%d %ps->%ps, mark:%u, oif:%u, iif:%u,"
			   "src=*.*.%u.%u dst=*.*.%u.%u\n",
			   drop_reasons[SKB_IP_NOROUTES], err, func, caller,
			   fl4->flowi4_mark, fl4->flowi4_oif, fl4->flowi4_iif,
			   IP4_NMASK(fl4->saddr), IP4_HMASK(fl4->saddr),
			   IP4_NMASK(fl4->daddr), IP4_HMASK(fl4->daddr));
}

static void flowi6_info_show(struct flowi6 *fl6, void *func, void *caller, int err, bool fg)
{
	routelog_ratelimited(fg, "%s err:%d %ps->%ps, mark:%u, oif:%u, iif:%u,"
			   "src6=*:%02x%02x dst6=*:%02x%02x\n",
			   drop_reasons[SKB_IP_NOROUTES], err, func, caller,
			   fl6->flowi6_mark, fl6->flowi6_oif, fl6->flowi6_iif,
			   fl6->saddr.s6_addr[14], fl6->saddr.s6_addr[15],
			   fl6->daddr.s6_addr[14], fl6->daddr.s6_addr[15]);
}

static void hook_route_drop_reason(void *__data, const struct sock *sk,
				   void *flow, int err)
{
	void *func = __builtin_return_address(1);
	void *caller = __builtin_return_address(2);
	bool fg;

	if (flow == NULL || sk == NULL)
		return;

	fg = is_fg(sk, NULL);
	switch (sk->sk_family) {
	case AF_INET:
		flowi4_info_show((struct flowi4 *)flow, func, caller, err, fg);
		break;
	case AF_INET6:
		flowi6_info_show((struct flowi6 *)flow, func, caller, err, fg);
		break;
	default:
		break;
	}
}
INIT_VENDOR_HOOK(ldk_rvh_route_drop_reason, hook_route_drop_reason);

static void hook_bpf_drop_reason(void *__data, void *ctx, int reason)
{
	void *func = __builtin_return_address(1);
	void *caller = __builtin_return_address(2);

	switch (reason) {
	case BPF_DROP_SKB:
		ip_info_show(NULL, (struct sk_buff *)ctx, reason, func, caller);
		break;
	case BPF_DROP_SOCK:
		transport_info_show((struct sock *)ctx, reason, func, caller);
		break;
	default:
		backtrace_show(reason, func, caller);
		break;
	}
}
INIT_VENDOR_HOOK(ldk_rvh_bpf_drop_reason, hook_bpf_drop_reason);

static void hook_netdev_queue_debug(void *__data, void *ctx, struct netdev_queue *q, int reason)
{
	void *func = NULL;
	void *caller = NULL;

	if (q == NULL || (!netif_xmit_frozen_or_stopped(q))) {
		return;
	}

	/* print logs 10 seconds later */
	if (q->stop_time != 0 && time_after(jiffies, q->stop_time + 10 * HZ)) {
		func = __builtin_return_address(1);
		caller = __builtin_return_address(2);

		__hook_skb_drop_reason(NULL, (struct sk_buff *)ctx, reason, func, caller);
	}
}
INIT_VENDOR_HOOK(ldk_rvh_netdev_queue_debug, hook_netdev_queue_debug);
