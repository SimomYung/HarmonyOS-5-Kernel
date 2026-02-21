#include <linux/net.h>
#include <net/inet_sock.h>
#include <net/sock.h>
#include <linux/skbuff.h>
#include <net/tcp.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/bpf.h>
#include <linux/tcp.h>
#include <linux/minmax.h>
#include <linux/netdevice.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter_ipv6/ip6_tables.h>
#include <trace/hooks/liblinux.h>
#include <securec.h>
#ifdef CONFIG_HW_PACKET_FILTER_BYPASS
#include <hwnet/booster/hw_packet_filter_bypass.h>
#endif

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
	FN(SKB_TCP_RETRANSMIT_FAIL, TCP_RETRANSMIT_FAIL)
	FN(SKB_TCP_RCV_NOBUF, TCP_RCV_NOBUF)
	FN(SKB_TCP_RCV_ZEROWIN, TCP_RCV_ZEROWIN)
	FN(SKB_NETFILTER_DROP, NETFILTER_DROP)
	FN(SKB_IP_NOROUTES, IP_NOROUTES)
	FN(SKB_NEIGH_FAILED, NEIGH_FAILED)
	FN(SKB_NEIGH_QUEUEFULL, NEIGH_QUEUEFULL)
	FN(SKB_INVALID_MAC, INVALID_MAC)
	FN(SKB_QDISC_STOP, QDISC_STOP)
	FN(SKB_CPU_BACKLOG_FULL, CPU_BACKLOG_FULL)
};

static DEFINE_RATELIMIT_STATE(netlog_ratelimit_state, 1 * HZ, 10);
static DEFINE_RATELIMIT_STATE(netfilter_ratelimit_state, 1 * HZ, 10);
static DEFINE_RATELIMIT_STATE(route_ratelimit_state, 5 * HZ, 3);
static DEFINE_RATELIMIT_STATE(bg_ratelimit_state, 10 * HZ, 3);

#define netlog_ratelimited(is_fg, fmt, ...)                     \
do {								\
	if (is_fg && __ratelimit(&netlog_ratelimit_state)) {	\
		pr_netlog(fmt, ##__VA_ARGS__);			\
		pr_hisysevent(fmt, ##__VA_ARGS__);		\
	} else if (!is_fg && __ratelimit(&bg_ratelimit_state)) 	\
		pr_netlog(fmt, ##__VA_ARGS__);			\
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

static inline struct sock *get_full_sk(const struct sock *sk_ori,
				       struct sk_buff *skb)
{
	struct sock *sk;

	if (sk_ori)
		sk = sk_to_full_sk((struct sock *)sk_ori);
	else
		sk = skb ? sk_to_full_sk(skb->sk) : NULL;

	if (sk && sk_fullsock(sk))
		return sk;

	return NULL;
}

static inline struct sk_event_list *sk_get_event_list(const struct sock *sk)
{
	return (sk && sk_fullsock(sk)) ? rcu_dereference(sk->events) : NULL;
}

static struct sk_event_list *sk_event_init(int val, gfp_t gfp)
{
	struct sk_event_list *events;

	events = kzalloc(sizeof(*events) + val * sizeof(struct sk_event), gfp);
	if (!events)
		return NULL;
	events->len = val;

	return events;
}

void sk_event_free(struct sock *sk)
{
	struct sk_event_list *events = sk_get_event_list(sk);
	if (!events)
		return;
	rcu_assign_pointer(sk->events, NULL);
	kfree_rcu(events, rcu);
}

static inline void tcp_fill_event_info(struct sk_event *event, u32 seq,
					u32 ack, u16 flag, u32 len)
{
	event->seq = seq;
	event->ack_seq = ack;
	event->len = len;
	event->flag = flag;
}

static int get_event_idx(int pos, int protocol, struct sk_event_list *events)
{
	int offset, idx = 0, qlen = events->len;

	if (protocol != IPPROTO_TCP)
		return atomic_inc_return(&events->idx) % qlen;

	switch (pos) {
	case TCP_SEND:
		return atomic_inc_return(&events->widx) % EVENT_KERN_SND;
	case TCP_RCV:
		return atomic_inc_return(&events->ridx) % EVENT_KERN_RCV + EVENT_KERN_SND;
	case TCP_WRITE:
	case TCP_READ:
		offset = EVENT_KERN_SND + EVENT_KERN_RCV;
		return atomic_inc_return(&events->uidx) % EVENT_USER_SYSCALL + offset;
	default:
		offset = EVENT_KERN_SND + EVENT_KERN_RCV + EVENT_USER_SYSCALL;
		return atomic_inc_return(&events->idx) % EVENT_TRANS_ERR + offset;
	}
}

static void sk_error_event_push(const struct sock *sk_ori, struct sk_buff *skb,
				int pos, void *caller, int val)
{
	struct sock *sk = get_full_sk(sk_ori, skb);
	struct sk_event_list *events = sk_get_event_list(sk);
	struct sk_event *event;
	u64 time;
	int idx;

	if (!events)
		return;

	time = div_u64(ktime_get_real_ns(), NSEC_PER_MSEC);
	idx = get_event_idx(pos, sk->sk_protocol, events);
	event = &events->list[idx];
	event->pos = pos;
	event->time = time;
	if (pos > TCP_EVENT_START) {
		struct tcp_sock *tp = tcp_sk(sk);
		u16 tcp_flag = skb ? TCP_SKB_CB(skb)->tcp_flags : 0;

		if (pos == TCP_RCV || pos == TCP_SEND) {
			struct tcphdr *th = (struct tcphdr *)skb->data;
			tcp_fill_event_info(event, ntohl(th->seq),
					    ntohl(th->ack_seq),
					    tcp_flag_byte(th), skb->len);
		} else if (pos == TCP_WRITE || pos == TCP_READ)
			tcp_fill_event_info(event, tp->write_seq,
					    tp->copied_seq, tcp_flag, val);
		else
			tcp_fill_event_info(event, tp->snd_nxt, tp->rcv_nxt,
					    tcp_flag, tp->snd_nxt - tp->snd_una);
	} else {
		event->caller = caller;
		event->errno = val;
	}
}

static inline bool is_fg(const struct sock *sk_ori, struct sk_buff *skb)
{
	const struct sock *sk = get_full_sk(sk_ori, skb);

	return sk_is_fg(sk);
}

static void backtrace_show(int reason, void *func, void *caller)
{
	netlog_ratelimited(false, "%s, %ps->%ps\n", drop_reasons[reason], func, caller);
}

static void transport_info_show(const struct sock *sk, int reason, void *func, void *caller)
{
	struct inet_sock *inet;
	bool fg;

	if (!sk || !sk_fullsock(sk))
		return;

	inet = inet_sk(sk);
	fg = is_fg(sk, NULL);
	if (sk->sk_family == AF_INET6) {
		netlog_ratelimited(fg, "%s %ps->%ps,err:%d,state:%u mark:%x,sp %u dp %u "
				   "src6=*:%02x%02x\n",
				   drop_reasons[reason], func, caller, sk->sk_err, sk->sk_state,
				   sk->sk_mark, ntohs(inet->inet_sport), ntohs(inet->inet_dport),
				   sk->sk_v6_rcv_saddr.s6_addr[14], sk->sk_v6_rcv_saddr.s6_addr[15]);
	} else {
		netlog_ratelimited(fg, "%s %ps->%ps,err:%d,state:%u mark:%x,sp %u dp %u src=%u.*.*.%u\n",
				    drop_reasons[reason], func, caller, sk->sk_err, sk->sk_state,
				    sk->sk_mark, ntohs(inet->inet_sport), ntohs(inet->inet_dport),
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

void __hook_skb_drop_reason(const struct sock *sk, struct sk_buff *skb,
			    int reason, void *func, void *caller)
{
	int err = 0;

	if (sk == NULL && skb == NULL)
		return;

	if (sk && sk_fullsock(sk))
		err = sk->sk_err;

	if ((reason > SKB_COMMON_START) && (reason < SKB_COMMON_END)) {
		void *upper_caller = __builtin_return_address(4);

		sk_error_event_push(sk, skb, reason, upper_caller, err);
		transport_info_show(sk, reason, caller, upper_caller);
		return;
	}

	sk_error_event_push(sk, skb, reason, caller, err);
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

void hook_skb_drop_reason(void *__data, const struct sock *sk,
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

static bool nf_bypass_skb(const struct nf_hook_state *state, struct sk_buff *skb)
{
#ifdef CONFIG_HW_PACKET_FILTER_BYPASS
	int hook = hw_translate_hook_num(state->pf, state->hook);

	if (hw_bypass_skb(state->pf, hook, NULL, skb, state->in, state->out, DROP))
		return true;
#endif
	return false;
}

static void hook_trace_iptable_drop(void *__data, const struct nf_hook_state *state,
			    const char *tablename, const struct xt_table_info *private,
			    const struct ipt_entry *e, struct sk_buff *skb)
{
	const void *root;
	const char *hookname, *chainname;
	const struct ipt_entry *iter;
	unsigned int hook = state->hook;

	if (hook >= NF_INET_NUMHOOKS || nf_bypass_skb(state, skb))
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

static void hook_trace_iptable6_drop(void *__data, const struct nf_hook_state *state,
			    const char *tablename, const struct xt_table_info *private,
			    const struct ip6t_entry *e, struct sk_buff *skb)
{
	const void *root;
	const char *hookname, *chainname;
	const struct ip6t_entry *iter;
	unsigned int hook = state->hook;

	if (hook >= NF_INET_NUMHOOKS || nf_bypass_skb(state, skb))
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
			   "src=%u.*.*.%u dst=%u.*.*.%u\n",
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

	/* tcp connect and udp syscall will return errno directly*/
	if (sk->sk_state != TCP_ESTABLISHED)
		return;

	sk_error_event_push(sk, NULL, SKB_IP_NOROUTES, caller, err);
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
		sk_error_event_push(NULL, (struct sk_buff *)ctx, reason, caller, 0);
		ip_info_show(NULL, (struct sk_buff *)ctx, reason, func, caller);
		break;
	case BPF_DROP_SOCK:
		sk_error_event_push((struct sock *)ctx, NULL, reason, caller, 0);
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

static void hook_sk_event_set(void *__data, struct sock *sk, int val, int *ret)
{
	struct sk_event_list *events = sk_get_event_list(sk);

	if (!sk || !sk_fullsock(sk)) {
		*ret = -EINVAL;
		return;
	}

	if (sk->sk_protocol == IPPROTO_TCP)
		val = TCP_EVENT_LIST_LEN;
	else if (sk->sk_protocol == IPPROTO_UDP)
		val = UDP_EVENT_LIST_LEN;
	else {
		*ret = -EPROTONOSUPPORT;
		return;
	}

	if (!events) {
		events = sk_event_init(val, GFP_KERNEL);
		if (!events) {
			*ret = -ENOMEM;
			return;
		}
		rcu_assign_pointer(sk->events, events);
	}

	*ret = 0;
}
INIT_VENDOR_HOOK(ldk_rvh_sk_event_set, hook_sk_event_set);

static void hook_sk_event_free(void *__data, struct sock *sk)
{
	sk_event_free(sk);
}
INIT_VENDOR_HOOK(ldk_rvh_sk_event_free, hook_sk_event_free);

static void hook_tcp_event_update(void *__data, struct sock *sk,
				  struct sk_buff *skb, int flag, int val)
{
	sk_error_event_push(sk, skb, flag, NULL, val);
}
INIT_VENDOR_HOOK(ldk_rvh_tcp_event_update, hook_tcp_event_update);

static int tcp_info_to_str(struct sock *sk, char *log, int len)
{
	struct tcp_sock *tp = tcp_sk(sk);
	int rxq, txq, retrans, state = inet_sk_state_load(sk);

	rxq = max_t(int, READ_ONCE(tp->rcv_nxt) - READ_ONCE(tp->copied_seq), 0);
	txq = READ_ONCE(tp->write_seq) - tp->snd_una;
	retrans = inet_csk(sk)->icsk_retransmits;
	return snprintf_s(log, len, len - 1, "%x %x %x %x %x %x\n",
			  state, txq, rxq, retrans, tp->snd_wnd, tp->rcv_wnd);
}

static void sk_event_start_info(struct sk_event_list *events, int pos,
				struct sk_event_info *info)
{
	u32 seq = U32_MAX, pseq = U32_MAX, idx = 0;
	struct sk_event *event;

	while (idx < events->len) {
		event = &events->list[idx++];
		if (!event->pos)
			continue;
		info->time = min(info->time ?: U64_MAX, event->time);
		/* get tcp event start seq and ack num of send and rcv */
		if (event->pos > TCP_EVENT_START) {
			if (event->pos == TCP_RCV) {
				seq = min(seq, event->ack_seq ?: seq);
				pseq = min(pseq, event->seq);
			} else {
				seq = min(seq, event->seq);
				pseq = min(pseq, event->ack_seq ?: pseq);
			}
		}
	}
	info->seq = seq;
	info->peer_seq = pseq;
}

static int sk_event_to_str(struct sock *sk, char *log, int len)
{
	struct sk_event_list *events = sk_get_event_list(sk);
	struct sk_event_info info = {};
	struct sk_event *event;
	int idx = 0, pos = 0, wlen;
	u32 seq, ack;

	if (!events)
		return -EFAULT;

	if (sk->sk_protocol == IPPROTO_TCP) {
		pos = tcp_info_to_str(sk, log, len);
		if (pos <= 0)
			return -EFBIG;
	}

	sk_event_start_info(events, pos, &info);
	wlen = snprintf_s(log + pos, len - pos, len - pos -1,
			"%lx %x %x\n", info.time, info.seq, info.peer_seq);
	if (wlen <= 0)
		return -EFBIG;

	pos += wlen;
	while (idx < events->len) {
		event = &events->list[idx++];
		if (event->pos == 0)
			continue;

		/* Calculate the relative value of seq and ack */
		if (event->pos > TCP_EVENT_START) {
			if (event->pos == TCP_RCV) {
				seq = event->seq - info.peer_seq;
				ack = event->ack_seq ? event->ack_seq - info.seq : 0;
			} else {
				seq = event->seq - info.seq;
				ack = event->ack_seq ? event->ack_seq - info.peer_seq : 0;
			}
			wlen = snprintf_s(log + pos, len - pos, len - pos - 1,
					"%lx %x %x %x %x %x\n", event->time - info.time ,
					event->pos, event->flag, seq, ack, event->len);
		} else if (event->pos > 0 && event->pos < TCP_EVENT_START)
			wlen = snprintf_s(log + pos, len - pos, len - pos - 1,
					"%lx %x %x %ps\n", event->time - info.time,
					event->pos, event->errno, event->caller);
		if (wlen <= 0)
			return -EFBIG;
		pos += wlen;
	}
	return pos;
}

static void hook_sk_event_read(void *__data, struct sock *sk, char __user *optval,
				int __user *optlen, int *ret)
{
	int len, err = 0;
	char *log;

	if (get_user(len, optlen)) {
		*ret = -EFAULT;
		return;
	}
	if (len <= 0) {
		*ret = -EINVAL;
		return;
	}
	if (len > SK_EVENT_INFO_MAX_LEN)
		len = SK_EVENT_INFO_MAX_LEN;

	log = kzalloc(len, GFP_KERNEL);
	if (!log) {
		*ret = -ENOMEM;
		return;
	}

	rcu_read_lock();
	err = sk_event_to_str(sk, log, len);
	if (err < 0) {
		rcu_read_unlock();
		goto free;
	}
	rcu_read_unlock();

	if (put_user(len, optlen)) {
		err = -EFAULT;
		goto free;
	}
	err = copy_to_user(optval, log, len);
	if (err) {
		err = -EFAULT;
		goto free;
	}

free:
	kfree(log);
	*ret = err;
}
INIT_VENDOR_HOOK(ldk_rvh_sk_event_read, hook_sk_event_read);
