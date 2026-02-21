/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#include <linux/init.h>
#include <linux/ip.h>
#include <linux/module.h>
#include <linux/types.h>
#include <net/sock.h>
#include <net/ip.h>
#include <linux/skbuff.h>
#include <linux/tcp.h>
#include <net/tcp.h>
#include <linux/udp.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/inetdevice.h>
#include <net/net_namespace.h>
#include <linux/proc_fs.h>
#include <linux/cred.h>
#include <linux/seq_file.h>
#include <linux/init.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_nat.h>
#include <uapi/linux/netfilter/nf_nat.h>
#include <net/inet_hashtables.h>

#include "securec.h"

#define NTI_PROC_NAME "nat_trav_iface" // proc/net/nat_trav_iface
#define MAX_INTERFACE_CHAR_NUM 64
#define NAT_SUPPORT_INFO_ARG_NUM 7
#define NAT_SUPPORT_INFO_FMT "%s %d %d %u %u %u %u"
#define NAT_SUPPORT_INFOS_LEN 2

extern struct sock *udp4_lib_lookup(struct net *net, __be32 saddr, __be16 sport, __be32 daddr, __be16 dport, int dif);

static struct net *g_anco_container_net;

static spinlock_t g_anco_container_net_lock;

static struct proc_dir_entry *g_nat_trav_proc_entry = NULL;

// nti stand for nat traversal interface
static unsigned int ipv4_nat_traversal(void *priv, struct sk_buff *skb, const struct nf_hook_state *state);
static unsigned int ipv6_nat_traversal(void *priv, struct sk_buff *skb, const struct nf_hook_state *state);
static bool try_correct_hl_v4(struct net *net, struct sk_buff *skb);
static bool try_correct_hl_v6(struct net *net, struct sk_buff *skb);
static bool is_nonhost_veth_dev(struct net_device *dev);
static bool allow_ipv4_addr_change(__be32 *ipv4_addr);
static bool allow_in6_addr_change(struct in6_addr *sin6_addr);

struct nat_support_info {
	char host_iface_name[IFNAMSIZ];
	bool enable;
	pid_t rgm_pid;
	__be32 anco_ip_addr;
	__be32 anco_ip_netmask;
	__be32 host_ip_addr;
	__be32 host_ip_netmask;
};
static seqlock_t g_nat_support_infos_lock;
static struct nat_support_info g_nat_support_infos[NAT_SUPPORT_INFOS_LEN];

// requires g_nat_support_infos_lock before invocation
static struct nat_support_info* get_nat_support_info(const char *iface_name);
static struct nat_support_info* get_target_nat_support_info(const char *iface_name, const struct iphdr *iph);

static const struct nf_hook_ops ipv4_nat_traversal_ops[] = {
	{
		.hook		= ipv4_nat_traversal,
		.pf			= NFPROTO_IPV4,
		.hooknum	= NF_INET_PRE_ROUTING,
		.priority	= NF_IP_PRI_CONNTRACK_CONFIRM,
	},
};

static const struct nf_hook_ops ipv6_nat_traversal_ops[] = {
	{
		.hook = ipv6_nat_traversal,
		.pf = NFPROTO_IPV6,
		.hooknum = NF_INET_PRE_ROUTING,
		.priority = NF_IP_PRI_CONNTRACK_CONFIRM,
	},
};

bool allow_current_uid_operation()
{
	return current_uid().val != 0;
}

void correct_v4_saddr(struct net *net, __be32 *saddr)
{
	struct net_device *host_dev = NULL;
	struct net_device *virtual_dev = NULL;
	struct in_device *virtual_in_dev = NULL;
	struct in_ifaddr *virtual_ifa = NULL;

	if (!net || net == &init_net)
		return;

	if (!allow_ipv4_addr_change(saddr))
		return;

	host_dev = __ip_dev_find(&init_net, *saddr, true);
	if (!host_dev)
		return;

	virtual_dev = dev_get_by_name(net, host_dev->name);
	if (virtual_dev) {
		virtual_in_dev = in_dev_get(virtual_dev);
		if (!is_nonhost_veth_dev(virtual_dev) || !virtual_in_dev) {
			dev_put(virtual_dev);
			dev_put(host_dev);
			return;
		}

		if (virtual_in_dev->ifa_list == NULL) {
			in_dev_put(virtual_in_dev);
			dev_put(virtual_dev);
			dev_put(host_dev);
			return;
		}

		rcu_read_lock();
		in_dev_for_each_ifa_rcu(virtual_ifa, virtual_in_dev) {
			if (virtual_ifa->ifa_scope == 0 && (virtual_ifa->ifa_flags & IFA_F_PERMANENT)) {
				pr_info("correct_v4_saddr change saddr from %d..%d to anco addr\n",
					((unsigned char *)saddr)[0], ((unsigned char *)saddr)[3]);
				*saddr = virtual_ifa->ifa_address;
				break;
			}
		}
		rcu_read_unlock();

		in_dev_put(virtual_in_dev);
		dev_put(virtual_dev);
	}
	dev_put(host_dev);
}

static bool check_net_valid(const struct net *net)
{
	struct net *ns;
	bool ret = false;

	if (net) {
		rcu_read_lock();
		for_each_net_rcu(ns) {
			if (ns == net) {
				ret = true;
				break;
			}
		}
		rcu_read_unlock();
	}
	return ret;
}

static struct sock *sk_lookup_slow_v4(struct net *net, struct sk_buff *skb, __be32 daddr, const struct net_device *dev)
{
	const struct iphdr *iph = ip_hdr(skb);
	bool is_udp = (iph->protocol == IPPROTO_UDP);
	struct udphdr *hp;
	struct tcphdr tcph;
	__be32 saddr;
	__be32 dst_addr;
	__be16 sport;
	__be16 dport;
	int doff = 0;
	struct sock *sk = NULL;

	if (!is_udp && (iph->protocol != IPPROTO_TCP))
		return NULL;

	hp = skb_header_pointer(skb, ip_hdrlen(skb), is_udp ? sizeof(*hp) : sizeof(tcph), &tcph);
	if (hp == NULL)
		return NULL;

	saddr = iph->saddr;
	dst_addr = daddr ? daddr : iph->daddr;
	sport = hp->source;
	dport = hp->dest;
	doff = is_udp ? ip_hdrlen(skb) + sizeof(*hp) : ip_hdrlen(skb) + __tcp_hdrlen((struct tcphdr*)hp);

	rcu_read_lock();
	if (is_udp)
		sk = udp4_lib_lookup(net, saddr, sport, dst_addr, dport, dev->ifindex);
	else
		sk = inet_lookup(net, &tcp_hashinfo, skb, doff, saddr, sport, dst_addr, dport, dev->ifindex);
	rcu_read_unlock();

	return sk;
}

static struct nat_support_info* get_nat_support_info(const char *iface_name)
{
	int i;
	int target = -1;

	for (i = 0; i < NAT_SUPPORT_INFOS_LEN; i++) {
		if (strcmp(g_nat_support_infos[i].host_iface_name, iface_name) == 0) {
			target = i;
			break;
		}

		if ((target == -1) && !g_nat_support_infos[i].enable)
			target = i;
	}

	return (target == -1) ? NULL : &g_nat_support_infos[target];
}

static struct nat_support_info* get_target_nat_support_info(const char *iface_name, const struct iphdr *iph)
{
	__be32 iph_prefix = 0;
	__be32 host_prefix = 0;
	struct nat_support_info *nat_support_info_ptr = get_nat_support_info(iface_name);

	if ((nat_support_info_ptr == NULL) || !nat_support_info_ptr->enable)
		return NULL;

	iph_prefix = (ntohl(iph->saddr) & nat_support_info_ptr->host_ip_netmask);
	host_prefix = (nat_support_info_ptr->host_ip_addr & nat_support_info_ptr->host_ip_netmask);
	return (iph_prefix == host_prefix) ? nat_support_info_ptr : NULL;
}

// requires g_nat_support_infos_lock before invocation
static void update_anco_container_net(struct net *anco_net, bool enable)
{
	int i;

	if (!IS_ERR(anco_net)) {
		if (net_eq(anco_net, g_anco_container_net)) {
			put_net(anco_net);
		} else {
			spin_lock(&g_anco_container_net_lock);
			if (check_net_valid(g_anco_container_net))
				put_net(g_anco_container_net);
			g_anco_container_net = anco_net;
			spin_unlock(&g_anco_container_net_lock);
		}
	}

	if (!enable && check_net_valid(g_anco_container_net)) {
		for (i = 0; i < NAT_SUPPORT_INFOS_LEN; i++) {
			if (g_nat_support_infos[i].enable)
				return;
		}
		spin_lock(&g_anco_container_net_lock);
		put_net(g_anco_container_net);
		g_anco_container_net = NULL;
		spin_unlock(&g_anco_container_net_lock);
	}
}

static bool is_target_pkt(struct net* net, struct sk_buff *skb, const struct net_device *dev, __be32 anco_ip_addr)
{
	struct iphdr *iph = ip_hdr(skb);
	struct udphdr *udph = NULL;
	struct tcphdr *tcph = NULL;
	uint16_t sport = 0;
	uint16_t dport = 0;
	struct sock *sk = NULL;
	struct net_device *anco_dev = NULL;

	if ((iph->protocol != IPPROTO_UDP) && (iph->protocol != IPPROTO_TCP))
		return false;

	if (iph->protocol == IPPROTO_UDP) {
		udph = udp_hdr(skb);
		if ((skb->pkt_type != PACKET_HOST) || unlikely(udph == NULL))
			return false;
		sport = ntohs(udph->source);
		dport = ntohs(udph->dest);
	} else {
		tcph = tcp_hdr(skb);
		if (unlikely(tcph == NULL) || (tcph->syn != 1) || (tcph->ack != 0))
			return false;
		sport = ntohs(tcph->source);
		dport = ntohs(tcph->dest);
	}

	sk = sk_lookup_slow_v4(net, skb, iph->daddr, dev);
	if (sk) {
		sock_gen_put(sk);
		return false; // skb is listened by host sk
	}

	anco_dev = dev_get_by_name(g_anco_container_net, dev->name);
	if (anco_dev == NULL)
		return false;

	sk = sk_lookup_slow_v4(g_anco_container_net, skb, anco_ip_addr, anco_dev);
	dev_put(anco_dev);
	if (sk == NULL)
		return false;

	sock_gen_put(sk);
	pr_info("is_target_pkt got anco listener. sport:%u, dport:%u, ptl:%u, pkt_type:%d, skb_iif:%d, dev:%s(%d)\n",
		sport, dport, iph->protocol, skb->pkt_type, skb->skb_iif, dev->name, dev->ifindex);
	return true;
}

static void update_nat_info(__be32 daddr, struct nf_conn *ct)
{
	struct nf_nat_range2 range = {0};

	range.flags = NF_NAT_RANGE_MAP_IPS;
	range.min_addr.ip = daddr;
	range.max_addr.ip = daddr;
	range.min_proto.all = 0;
	range.max_proto.all = 0xFFFF;
	ct->status &= (~IPS_DST_NAT_DONE);  // need redo nat
	nf_nat_setup_info(ct, &range, NF_NAT_MANIP_DST);
}

static void check_anco_recv(struct net* net, struct sk_buff *skb, const struct net_device *dev)
{
	struct iphdr *iph = ip_hdr(skb);
	struct nf_conn *ct;
	enum ip_conntrack_info ctinfo;
	unsigned int seq;
	struct nat_support_info *nat_support_info_ptr = NULL;
	__be32 anco_ip_addr = 0;

	if (unlikely(skb->sk) || unlikely(iph == NULL))
		return; // alredy found at host. for host DNS response, may not found sk

	do {
		seq = read_seqbegin(&g_nat_support_infos_lock);
		nat_support_info_ptr = get_target_nat_support_info(dev->name, iph);
		if (nat_support_info_ptr)
			anco_ip_addr = htonl(nat_support_info_ptr->anco_ip_addr);
	} while (read_seqretry(&g_nat_support_infos_lock, seq));
	if (anco_ip_addr == 0)
		return;

	ct = nf_ct_get(skb, &ctinfo);
	if ((ct == NULL) || (ctinfo == IP_CT_ESTABLISHED_REPLY) || (ctinfo == IP_CT_ESTABLISHED))
		return;

	spin_lock(&g_anco_container_net_lock);
	if (!check_net_valid(g_anco_container_net) || net_eq(net, g_anco_container_net) || !net_eq(net, &init_net)) {
		spin_unlock(&g_anco_container_net_lock);
		return;
	}

	if (!is_target_pkt(net, skb, dev, anco_ip_addr)) {
		spin_unlock(&g_anco_container_net_lock);
		return;
	}
	spin_unlock(&g_anco_container_net_lock);

	if (skb_ensure_writable(skb, sizeof(*iph)) || (iph->daddr == anco_ip_addr))
		return;

	pr_info("check_anco_recv, update daddr to anco addr\n");
	iph = ip_hdr(skb); // refresh iph as skb_ensure_writable() may modify it
	csum_replace4(&iph->check, iph->daddr, anco_ip_addr); // csum re-calculate
	iph->daddr = anco_ip_addr; // change skb ip addr to anco dev addr
	update_nat_info(iph->daddr, ct);
}

static unsigned int ipv4_nat_traversal(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
	if (likely(skb) && likely(skb->dev) && likely(state) && likely(state->net)) {
		try_correct_hl_v4(state->net, skb);
		check_anco_recv(state->net, skb, skb->dev);
	}
	return NF_ACCEPT;
}

static unsigned int ipv6_nat_traversal(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
	if (likely(skb) && likely(skb->dev) && likely(state) && likely(state->net))
		try_correct_hl_v6(state->net, skb);
	return NF_ACCEPT;
}

static bool correct_hl_v4(struct sk_buff *skb)
{
	struct iphdr *iph; /* Our header */

	if (skb_ensure_writable(skb, sizeof(*iph))) {
		return false;
	}

	iph = ip_hdr(skb);
	csum_replace2(&iph->check, htons(iph->ttl << 8), htons((iph->ttl + 1) << 8));
	iph->ttl++;

	return true;
}

static bool try_correct_hl_v4(struct net *net, struct sk_buff *skb)
{
	struct net* ns;
	struct net_device *dev;
	struct iphdr *iph = ip_hdr(skb);
	bool has_nh = false;

	if (iph->ttl > 1 || !net_eq(net, &init_net))
		return false;

	rcu_read_lock();
	for_each_net_rcu(ns) {
		if (net_eq(ns, net))
			continue;
		dev = __ip_dev_find(ns, iph->daddr, true); // find if dst addr belong to other namespace
		if (dev && dev->rtnl_link_ops && strcmp(dev->rtnl_link_ops->kind, "veth") == 0) { // just care veth type
			has_nh = true;
			break;
		}
	}
	if (dev)
		dev_put(dev);
	rcu_read_unlock();

	if (!has_nh)
		return false;

	return correct_hl_v4(skb);
}

static bool correct_hl_v6(struct sk_buff *skb)
{
	struct ipv6hdr *ip6h; /* Our header */

	if (skb_ensure_writable(skb, sizeof(*ip6h))) {
		return false;
	}

	ip6h = ipv6_hdr(skb);
	ip6h->hop_limit++;

	return true;
}

static bool try_correct_hl_v6(struct net *net, struct sk_buff *skb)
{
	struct net* ns;
	struct net_device *dev;
	struct ipv6hdr *ip6h = ipv6_hdr(skb);
	bool has_nh = false;

	if (ip6h->hop_limit > 1 || !net_eq(net, &init_net))
		return false;

	rcu_read_lock();
	for_each_net_rcu(ns) {
		if (net_eq(ns, net))
			continue;
		dev = ipv6_dev_find(ns, &ip6h->daddr, NULL); // find if dst addr belong to other namespace
		if (dev && dev->rtnl_link_ops && strcmp(dev->rtnl_link_ops->kind, "veth") == 0) { // just care veth type
			has_nh = true;
			break;
		}
	}
	rcu_read_unlock();

	if (!has_nh)
		return false;

	return correct_hl_v6(skb);
}

static bool is_nonhost_veth_dev(struct net_device *dev)
{
	if (!dev || dev_net(dev) == &init_net)
		return false;

	if (dev->type == ARPHRD_LOOPBACK)
		return false;

	return dev->rtnl_link_ops && strcmp("veth", dev->rtnl_link_ops->kind) == 0;
}

static bool allow_ipv4_addr_change(__be32 *ipv4_addr)
{
	if (!ipv4_addr || *ipv4_addr == 0)
		return false;

	if (ipv4_is_loopback(*ipv4_addr) || ipv4_is_multicast(*ipv4_addr)
		|| ipv4_is_local_multicast(*ipv4_addr) || ipv4_is_lbcast(*ipv4_addr))
		return false;
	return true;
}

static bool allow_in6_addr_change(struct in6_addr *sin6_addr)
{
	if (!sin6_addr)
		return false;
	int addr_type = ipv6_addr_type(sin6_addr);
	if (addr_type == IPV6_ADDR_ANY || addr_type == IPV6_ADDR_MULTICAST ||
		addr_type == IPV6_ADDR_LOOPBACK || addr_type == IPV6_ADDR_LINKLOCAL)
		return false;

	return __ipv6_addr_type(sin6_addr) == (IPV6_ADDR_UNICAST | (IPV6_ADDR_SCOPE_GLOBAL << 16));
}

__be32 get_host_ipv4_dev_addr_by_name(char *dev_name)
{
	struct net_device *host_dev = NULL;
	struct in_device *in_dev = NULL;
	struct in_ifaddr *ifa = NULL;
	__be32 ret_addr = 0;

	host_dev = dev_get_by_name(&init_net, dev_name);
	if (!host_dev)
		return 0;

	in_dev = in_dev_get(host_dev);
	if (!in_dev) {
		dev_put(host_dev);
		return 0;
	}

	if (in_dev->ifa_list == NULL) {
		in_dev_put(in_dev);
		dev_put(host_dev);
		return 0;
	}

	rcu_read_lock();
	in_dev_for_each_ifa_rcu(ifa, in_dev) {
		if (ifa->ifa_scope == 0 && (ifa->ifa_flags & IFA_F_PERMANENT)) {
			ret_addr = ifa->ifa_local != 0 ? ifa->ifa_local : ifa->ifa_address;
			break;
		}
	}
	rcu_read_unlock();

	in_dev_put(in_dev);
	dev_put(host_dev);
	return ret_addr;
}

__be32 get_uniform_v4_addr(struct net_device *dev)
{
	if (!is_nonhost_veth_dev(dev))
		return 0;

	return get_host_ipv4_dev_addr_by_name(dev->name);
}

bool try_change_to_uniform_in_ifaddr(struct net_device *dev, struct in_ifaddr *unif_ifa)
{
	struct net_device *host_dev = NULL;
	struct in_device *in_dev = NULL;
	struct in_ifaddr *temp_ifa = NULL;
	bool ret = false;

	if (!is_nonhost_veth_dev(dev))
		return false;

	host_dev = dev_get_by_name(&init_net, dev->name);
	if (!host_dev)
		return false;

	in_dev = in_dev_get(host_dev);
	if (!in_dev) {
		dev_put(host_dev);
		return false;
	}

	if (in_dev->ifa_list == NULL) {
		in_dev_put(in_dev);
		dev_put(host_dev);
		return false;
	}

	rcu_read_lock();
	in_dev_for_each_ifa_rcu(temp_ifa, in_dev) {
		if (temp_ifa->ifa_scope == 0 && (temp_ifa->ifa_flags & IFA_F_PERMANENT)) {
			unif_ifa->ifa_local = temp_ifa->ifa_local ? temp_ifa->ifa_local : temp_ifa->ifa_address;
			unif_ifa->ifa_address = temp_ifa->ifa_address;
			unif_ifa->ifa_prefixlen = temp_ifa->ifa_prefixlen;
			unif_ifa->ifa_mask = temp_ifa->ifa_mask;
			unif_ifa->ifa_flags = temp_ifa->ifa_flags;
			unif_ifa->ifa_scope = temp_ifa->ifa_scope;
			ret = true;
			break;
		}
	}
	rcu_read_unlock();

	in_dev_put(in_dev);
	dev_put(host_dev);
	return ret;
}

bool can_change_to_uniform_in_ifa(const struct in_ifaddr *ifa)
{
	if (!allow_current_uid_operation())
		return false;

	if (!ifa || !ifa->ifa_dev || !ifa->ifa_address)
		return false;

	return (ifa->ifa_flags & IFA_F_PERMANENT) && (ifa->ifa_scope == 0);
}

bool can_change_to_uniform_in6_ifa(const struct inet6_ifaddr *ifa)
{
	if (!allow_current_uid_operation())
		return false;

	if (!ifa || !ifa->idev || ipv6_addr_any(&ifa->addr))
		return false;
	return (ifa->flags & IFA_F_PERMANENT) && (ifa->scope == 0);
}

void try_change_to_uniform_ipv4_addr(__be32 *ipv4_addr, struct sock *sk)
{
	struct dst_entry *dst = NULL;
	struct net_device *virtual_dev = NULL;
	__be32 addr = 0;
	if (sock_net(sk) == &init_net)
		return;

	if (!allow_ipv4_addr_change(ipv4_addr))
		return;

	if (!allow_current_uid_operation())
		return;

	rcu_read_lock();
	dst = __sk_dst_get(sk);
	if (dst && false) // unapplicable now
		addr = get_uniform_v4_addr(dst->dev);
	else {
		virtual_dev = __ip_dev_find(sock_net(sk), *ipv4_addr, false);
		if (virtual_dev)
			addr = get_uniform_v4_addr(virtual_dev);
	}
	rcu_read_unlock();

	if (addr)
		*ipv4_addr = addr;
}

void try_change_to_uniform_sockaddr_in(struct sockaddr_in *sin, struct sock *sk)
{
	try_change_to_uniform_ipv4_addr(&sin->sin_addr.s_addr, sk);
}

bool try_change_to_uniform_in6_ifaddr(struct net_device *dev, struct inet6_ifaddr *in6_ifaddr)
{
	struct net_device *host_dev = NULL;
	struct inet6_dev *idev = NULL;
	struct inet6_ifaddr *ifa6 = NULL;
	bool ret = false;

	if (!in6_ifaddr)
		return false;

	if (!is_nonhost_veth_dev(dev))
		return false;

	host_dev = dev_get_by_name(&init_net, dev->name);
	if (!host_dev)
		return false;

	idev = in6_dev_get(host_dev);
	if (!idev) {
		dev_put(host_dev);
		return false;
	}

	read_lock_bh(&idev->lock);
	list_for_each_entry(ifa6, &idev->addr_list, if_list)
	{
		if (ifa6->scope == 0 &&
			((ifa6->flags & IFA_F_PERMANENT) || (ifa6->flags & IFA_F_TEMPORARY))) {
			in6_ifaddr->addr = ifa6->addr;
			in6_ifaddr->prefix_len = ifa6->prefix_len;
			ret = true;
			break;
		}
	}
	read_unlock_bh(&idev->lock);
	in6_dev_put(idev);
	dev_put(host_dev);
	return ret;
}

// can deal both v6 mapped addr and v6 standalone addr
void try_change_to_uniform_sockaddr_in6(struct sockaddr_in6 *sin, struct sock *sk)
{
	struct net_device *virtual_dev = NULL;
	struct dst_entry *dst = NULL;
	struct inet6_ifaddr in6_ifaddr = {0};
	__be32 ipv4Addr = 0;
	bool need_correct_v6 = false;

	if (!allow_current_uid_operation())
		return;

	if (sock_net(sk) == &init_net)
		return;

	if (!sin || ipv6_addr_any(&sin->sin6_addr))
		return;

	if (ipv6_addr_type(&sin->sin6_addr) & IPV6_ADDR_MAPPED) {
		if (!allow_ipv4_addr_change(&sin->sin6_addr.s6_addr32[3]))
			return;
		virtual_dev = __ip_dev_find(sock_net(sk), sin->sin6_addr.s6_addr32[3], true);
		if (virtual_dev) {
			if (is_nonhost_veth_dev(virtual_dev))
				ipv4Addr = get_host_ipv4_dev_addr_by_name(virtual_dev->name);
			dev_put(virtual_dev);
		}
		if (ipv4Addr)
			sin->sin6_addr.s6_addr32[3] = ipv4Addr;
	} else {
		if (!allow_in6_addr_change(&sin->sin6_addr))
			return;
		rcu_read_lock();
		dst = __sk_dst_get(sk);
		if (dst && false) { // unapplicable now
			need_correct_v6 = try_change_to_uniform_in6_ifaddr(dst->dev, &in6_ifaddr);
		} else {
			virtual_dev = ipv6_dev_find(sock_net(sk), &sin->sin6_addr, NULL);
			if (virtual_dev)
				need_correct_v6 = try_change_to_uniform_in6_ifaddr(virtual_dev, &in6_ifaddr);
		}
		rcu_read_unlock();
		if (need_correct_v6)
			sin->sin6_addr = in6_ifaddr.addr;
	}
}

void correct_v6_saddr(struct net *net, struct in6_addr *sin6_addr)
{
	struct net_device *host_dev = NULL;
	struct net_device *virtual_dev = NULL;
	struct inet6_dev *virtual_idev = NULL;
	struct inet6_ifaddr *virtual_ifa6 = NULL;

	if (net == &init_net)
		return;

	if (!allow_in6_addr_change(sin6_addr))
		return;

	rcu_read_lock();
	host_dev = ipv6_dev_find(&init_net, sin6_addr, NULL);
	if (!host_dev) {
		rcu_read_unlock();
		return;
	}

	if (host_dev->type == ARPHRD_LOOPBACK) {
		rcu_read_unlock();
		return;
	}

	virtual_dev = dev_get_by_name(net, host_dev->name);
	if (virtual_dev) {
		virtual_idev = in6_dev_get(virtual_dev);
		if (!is_nonhost_veth_dev(virtual_dev) || !virtual_idev) {
			dev_put(virtual_dev);
			rcu_read_unlock();
			return;
		}
		read_lock_bh(&virtual_idev->lock);
		list_for_each_entry(virtual_ifa6, &virtual_idev->addr_list, if_list)
		{
			if (virtual_ifa6->scope == 0 &&
				((virtual_ifa6->flags & IFA_F_PERMANENT) || (virtual_ifa6->flags & IFA_F_TEMPORARY))) {
				pr_info("correct_v6_saddr change sin6_addr from %#x..%#x to anco addr\n",
					ntohl(sin6_addr->s6_addr32[0]), ntohl(sin6_addr->s6_addr32[3]));
				*sin6_addr = virtual_ifa6->addr;
				break;
			}
		}
		read_unlock_bh(&virtual_idev->lock);
		in6_dev_put(virtual_idev);
		dev_put(virtual_dev);
	}
	rcu_read_unlock();
}

static int nti_proc_show(struct seq_file *file, void *buf)
{
	unsigned int seq;
	struct nat_support_info *nat_support_info_ptr = NULL;
	int i;

	do {
		seq = read_seqbegin(&g_nat_support_infos_lock);
		for (i = 0; i < NAT_SUPPORT_INFOS_LEN; i++) {
			nat_support_info_ptr = &g_nat_support_infos[i];
			seq_printf(file, NAT_SUPPORT_INFO_FMT "\n", nat_support_info_ptr->host_iface_name,
				nat_support_info_ptr->enable, nat_support_info_ptr->rgm_pid,
				nat_support_info_ptr->anco_ip_addr, nat_support_info_ptr->anco_ip_netmask,
				nat_support_info_ptr->host_ip_addr, nat_support_info_ptr->host_ip_netmask);
		}
	} while (read_seqretry(&g_nat_support_infos_lock, seq));
	return 0;
}

static int nti_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, nti_proc_show, NULL);
}

static ssize_t nti_proc_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	int ret;
	char nat_support_info_str[MAX_INTERFACE_CHAR_NUM] = "";
	struct nat_support_info nat_support_info_obj;
	struct nat_support_info *nat_support_info_ptr;
	struct net *anco_net;

	pr_info("enter nti_proc_write. count:%lu\n", (unsigned long)count);
	if ((count >= MAX_INTERFACE_CHAR_NUM) || copy_from_user(nat_support_info_str, buf, count))
		return -EINVAL;

	nat_support_info_str[count] = '\0';
	ret = sscanf_s(nat_support_info_str, NAT_SUPPORT_INFO_FMT, nat_support_info_obj.host_iface_name, IFNAMSIZ,
		&nat_support_info_obj.enable, &nat_support_info_obj.rgm_pid, &nat_support_info_obj.anco_ip_addr,
		&nat_support_info_obj.anco_ip_netmask, &nat_support_info_obj.host_ip_addr,
		&nat_support_info_obj.host_ip_netmask);
	if (ret != NAT_SUPPORT_INFO_ARG_NUM)
		return -EINVAL;

	anco_net = get_net_ns_by_pid(nat_support_info_obj.rgm_pid);
	if (nat_support_info_obj.enable &&
		((nat_support_info_obj.anco_ip_addr == 0) || (nat_support_info_obj.anco_ip_netmask == 0) ||
		(nat_support_info_obj.host_ip_addr == 0) || (nat_support_info_obj.host_ip_netmask == 0) || IS_ERR(anco_net)))
		nat_support_info_obj.enable = false; // rgm_pid or some addr is invalid
	pr_info("nti_proc_write, enable: %d, host_iface_name: %s, rgm_pid: %d\n", nat_support_info_obj.enable,
		nat_support_info_obj.host_iface_name, nat_support_info_obj.rgm_pid);
	write_seqlock(&g_nat_support_infos_lock);
	nat_support_info_ptr = get_nat_support_info(nat_support_info_obj.host_iface_name);
	if (nat_support_info_ptr == NULL) {
		pr_err("nti_proc_write, exceed nat_support_info len\n");
		if (!IS_ERR(anco_net))
			put_net(anco_net);
		write_sequnlock(&g_nat_support_infos_lock);
		return -EINVAL;
	}

	*nat_support_info_ptr = nat_support_info_obj;
	update_anco_container_net(anco_net, nat_support_info_ptr->enable);
	write_sequnlock(&g_nat_support_infos_lock);
	return count;
}

static const struct proc_ops nti_proc_fops = {
	.proc_open = nti_proc_open,
	.proc_read = seq_read,
	.proc_write = nti_proc_write,
	.proc_release = single_release,
};

static int __init ipv4_nat_traversal_init(void)
{
	int ret;
	pr_info("ipv4_nat_traversal_init module\n");

	seqlock_init(&g_nat_support_infos_lock);
	g_nat_trav_proc_entry = proc_create(NTI_PROC_NAME, 0644, init_net.proc_net, &nti_proc_fops);
	if (g_nat_trav_proc_entry == NULL) {
		pr_err("ipv4_nat_traversal_init, create nat_trav proc entry failed!\n");
		return -ENOMEM;
	}

	ret = nf_register_net_hooks(&init_net, ipv4_nat_traversal_ops, ARRAY_SIZE(ipv4_nat_traversal_ops));
	if (ret) {
		pr_err("ipv4_nat_traversal_init, nf_register_net_hooks failed!\n");
		if (g_nat_trav_proc_entry)
			remove_proc_entry(NTI_PROC_NAME, init_net.proc_net);
		return ret;
	}

	write_seqlock(&g_nat_support_infos_lock);
	(void)memset_s(g_nat_support_infos, sizeof(g_nat_support_infos), 0, sizeof(g_nat_support_infos));
	write_sequnlock(&g_nat_support_infos_lock);
	spin_lock_init(&g_anco_container_net_lock);
	return 0;
}

static void __exit ipv4_nat_traversal_exit(void)
{
	pr_info("ipv4_nat_traversal_exit module\n");
	if (g_nat_trav_proc_entry)
		remove_proc_entry(NTI_PROC_NAME, init_net.proc_net);
	g_nat_trav_proc_entry = NULL;
	nf_unregister_net_hooks(&init_net, ipv4_nat_traversal_ops, ARRAY_SIZE(ipv4_nat_traversal_ops));
	write_seqlock(&g_nat_support_infos_lock);
	(void)memset_s(g_nat_support_infos, sizeof(g_nat_support_infos), 0, sizeof(g_nat_support_infos));
	write_sequnlock(&g_nat_support_infos_lock);
}

static int ipv6_nat_traversal_init(void)
{
	int ret;
	pr_info("ipv6_nat_traversal_init module\n");

	ret = nf_register_net_hooks(&init_net, ipv6_nat_traversal_ops, ARRAY_SIZE(ipv6_nat_traversal_ops));
	if (ret) {
		pr_err("ipv6_nat_traversal_init, nf_register_net_hooks failed!\n");
		return ret;
	}

	return 0;
}

static void ipv6_nat_traversal_exit(void)
{
	pr_info("ipv6_nat_traversal_exit module\n");
	nf_unregister_net_hooks(&init_net, ipv6_nat_traversal_ops, ARRAY_SIZE(ipv6_nat_traversal_ops));
}

static int __init nat_traversal_init(void)
{
	int ret = ipv4_nat_traversal_init();
	if (ret != 0)
		return ret;
	ret = ipv6_nat_traversal_init();
	return ret;
}

static void __exit nat_traversal_exit(void)
{
	ipv4_nat_traversal_exit();
	ipv6_nat_traversal_exit();
}

bool anco_bypass_mtu(const struct net_device *dev, const struct net_device *rcv_dev)
{
	if (!dev || !rcv_dev ||
		!rcv_dev->rtnl_link_ops || !rcv_dev->rtnl_link_ops->kind)
		return false;

	return !net_eq(dev_net(dev), dev_net(rcv_dev)) && strcmp(rcv_dev->rtnl_link_ops->kind, "veth") == 0;
}

module_init(nat_traversal_init);
module_exit(nat_traversal_exit);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("nat traversal module driver");

