// SPDX-License-Identifier: GPL-2.0
#include <linux/net.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/tcp.h>
#include <linux/compat.h>
#include <linux/netdevice.h>
#include <linux/thread_info.h>
#include <linux/uaccess.h>
#include <net/sock.h>
#include <net/inet_connection_sock.h>
#include <net/udp.h>
#include <net/tcp.h>
#include <net/rtnetlink.h>
#include <liblinux/pal.h>

#include <trace/hooks/liblinux.h>

struct socket * __override sockfd_lookup(int fd, int *err)
{
	struct file *file;
	struct socket *sock;

	file = liblinux_pal_fget_ex(NET_FD, fd, NULL);
	if (!file) {
		*err = -EBADF;
		return NULL;
	}

	sock = sock_from_file(file, err);
	if (!sock)
		fput(file);
	return sock;
}

/* for synchronizing events to the epoll/poll during status change */
static void hook_inet_stream_connect(void *__data, struct socket *sock, int flags)
{
	if ((inet_sk_state_load(sock->sk) != TCP_CLOSE) && (flags & O_NONBLOCK))
		sk_event_sync_to_hmkobj(sock->sk, sock->ops->poll(NULL, sock, NULL));
}
INIT_VENDOR_HOOK(ldk_vh_inet_stream_connect, hook_inet_stream_connect);

static void hook_inet_listen(void *__data, struct socket *sock)
{
	if (inet_sk_state_load(sock->sk) != TCP_CLOSE)
		sk_event_sync_to_hmkobj(sock->sk, sock->ops->poll(NULL, sock, NULL));
}
INIT_VENDOR_HOOK(ldk_vh_inet_listen, hook_inet_listen);

static void hook_inet_accept(void *__data, struct socket *sock)
{
	if (reqsk_queue_empty(&inet_csk(sock->sk)->icsk_accept_queue))
		sk_event_sync_to_hmkobj(sock->sk, DEFAULT_RECHECK);
}
INIT_VENDOR_HOOK(ldk_vh_inet_accept, hook_inet_accept);

static void hook_tcp_sendmsg(void *__data, struct sock *sk)
{
	if (!(sk->sk_shutdown & SEND_SHUTDOWN) && !__sk_stream_is_writeable(sk, 1))
		sk_event_sync_to_hmkobj(sk, DEFAULT_RECHECK);
}
INIT_VENDOR_HOOK(ldk_vh_tcp_sendmsg, hook_tcp_sendmsg);

static void hook_tcp_recvmsg(void *__data, struct sock *sk)
{
	if (skb_queue_empty(&sk->sk_receive_queue))
		sk_event_sync_to_hmkobj(sk, DEFAULT_RECHECK);
}
INIT_VENDOR_HOOK(ldk_vh_tcp_recvmsg, hook_tcp_recvmsg);

static void hook_udp_sendmsg(void *__data, struct sock *sk)
{
	if (!sock_writeable(sk))
		sk_event_sync_to_hmkobj(sk, DEFAULT_RECHECK);
}
INIT_VENDOR_HOOK(ldk_vh_udp_sendmsg, hook_udp_sendmsg);

static void hook_udp_recvmsg(void *__data, struct sock *sk)
{
	if (skb_queue_empty_lockless(&sk->sk_receive_queue) && skb_queue_empty_lockless(&udp_sk(sk)->reader_queue))
		sk_event_sync_to_hmkobj(sk, DEFAULT_RECHECK);
}
INIT_VENDOR_HOOK(ldk_vh_udp_recvmsg, hook_udp_recvmsg);

static void hook_retransmit_fail(void *__data, struct sock *sk,
					struct sk_buff *skb, char *extra_info, int err)
{
	struct inet_sock *inet = inet_sk(sk);
	unsigned int saddr, daddr;
	struct in6_addr saddrv6 = { 0 };
	struct in6_addr daddrv6 = { 0 };
	unsigned int seq = TCP_SKB_CB(skb)->seq;
	unsigned int rto = (sk_fullsock(sk) ? jiffies_to_msecs(inet_csk(sk)->icsk_rto) : 0);
	void *func = __builtin_return_address(1);
	void *caller = __builtin_return_address(2);

	if (sk->sk_protocol != IPPROTO_TCP) {
		return;
	}

	__hook_skb_drop_reason(sk, skb, SKB_TCP_RETRANSMIT_FAIL, func, caller);
	if (sk->sk_family == AF_INET6) {
		saddrv6 = sk->sk_v6_rcv_saddr;
		daddrv6 = sk->sk_v6_daddr;
		net_info_ratelimited("retransmit failed! pkt info: seq=%u src-port=%u dst-port=%u "
				     "saddrv6=*:*:*:*:%x%x daddrv6=*:*:*:*:%x%x rto=%u(ms) flag=0x%x err=%d reason:[%s]\n",
				     seq, ntohs(inet->inet_sport), ntohs(inet->inet_dport),
				     saddrv6.s6_addr[14], saddrv6.s6_addr[15],  daddrv6.s6_addr[14], daddrv6.s6_addr[15],
				     rto, TCP_SKB_CB(skb)->tcp_flags, err, extra_info);
	} else {
		saddr = ntohl(inet->inet_saddr);
		daddr = ntohl(inet->inet_daddr);
		net_info_ratelimited("retransmit failed! pkt info: seq=%u src-port=%u dst-port=%u "
				     "saddr=*.*.%u.%u daddr=*.*.%u.%u rto=%u(ms) flag=0x%x err=%d reason:[%s]\n",
				     seq, ntohs(inet->inet_sport), ntohs(inet->inet_dport),
				     (saddr & 0xff00) >> 8, saddr & 0xff, (daddr & 0xff00) >> 8, daddr & 0xff,
				     rto, TCP_SKB_CB(skb)->tcp_flags, err, extra_info);
	}
}
INIT_VENDOR_HOOK(ldk_vh_retransmit_fail, hook_retransmit_fail);

static void hook_tcp_connect_rcv(void *__data, struct sock *sk, const struct tcphdr *th)
{
	struct inet_sock *inet = inet_sk(sk);

	if (sk->sk_state != TCP_ESTABLISHED || sock_flag(sk, SOCK_DEAD))
		pr_info("connect src-port %u, dst-port %u recv fin %u, syn %u, rst %u, psh %u, ack %u\n",
			inet ? ntohs(inet->inet_sport) : 0, inet ? ntohs(inet->inet_dport) : 0,
			th->fin, th->syn, th->rst, th->psh, th->ack);
}
INIT_VENDOR_HOOK(ldk_vh_tcp_connect_rcv, hook_tcp_connect_rcv);

#ifdef CONFIG_NETLINK_ROUTE_NLMSG_READPRIV
static void hook_sysctl_core_net_init(void *__data, struct net *net, struct ctl_table *tbl,
				      struct user_namespace *init_user_ns)
{
	tbl[1].data = &net->core.sysctl_getlink_disable;
}
INIT_VENDOR_HOOK(ldk_vh_sysctl_core_net_init, hook_sysctl_core_net_init);

static void hook_net_defaults_init_net(void *__data, struct net *net)
{
	struct net *current_net = NULL;

	trace_ldk_vh_devhost_net_socket_get_netns(&current_net);
	if (current_net && net != current_net) {
		net->core.sysctl_getlink_disable = current_net->core.sysctl_getlink_disable;
	} else {
		net->core.sysctl_getlink_disable = 0;
	}

	if (current_net)
		put_net(current_net);
}
INIT_VENDOR_HOOK(ldk_vh_net_defaults_init_net, hook_net_defaults_init_net);
#endif

static long sock_do_ioctl(struct net *net, struct socket *sock,
			  unsigned int cmd, unsigned long arg)
{
	int err;
	void __user *argp = (void __user *)arg;

	err = sock->ops->ioctl(sock, cmd, arg);
	/*
	 * If this ioctl is unknown try to hand it down
	 * to the NIC driver.
	 */
	if (err != -ENOIOCTLCMD)
		return err;

	if (cmd == SIOCGIFCONF) {
		struct ifconf ifc;
		if (copy_from_user(&ifc, argp, sizeof(struct ifconf)))
			return -EFAULT;
		rtnl_lock();
		err = dev_ifconf(net, &ifc, sizeof(struct ifreq));
		rtnl_unlock();
		if (!err && copy_to_user(argp, &ifc, sizeof(struct ifconf)))
			err = -EFAULT;
	} else if (is_socket_ioctl_cmd(cmd)) {
		struct ifreq ifr;
		bool need_copyout;
		if (copy_from_user(&ifr, argp, sizeof(struct ifreq)))
			return -EFAULT;
		err = dev_ioctl(net, cmd, &ifr, &need_copyout);
		if (!err && need_copyout)
			if (copy_to_user(argp, &ifr, sizeof(struct ifreq)))
				return -EFAULT;
	} else {
		err = -ENOTTY;
	}
	return err;
}

static void hook_compat_ifreq_ioctl(void *__data, struct net *net, struct socket *sock,
		    unsigned int cmd, struct compat_ifreq __user *uifr32, int *ret)
{
	struct ifreq ifr = {0};
	mm_segment_t fs;
	int err;

	if (copy_from_user(&ifr, uifr32, sizeof(*uifr32))) {
		*ret = -EFAULT;
		return;
	}

	fs = get_fs();
	set_fs(KERNEL_DS);
	err = sock_do_ioctl(net, sock, cmd, (unsigned long)&ifr);
	set_fs(fs);

	if (!err) {
		switch (cmd) {
		case SIOCGIFFLAGS:
		case SIOCGIFMETRIC:
		case SIOCGIFMTU:
		case SIOCGIFMEM:
		case SIOCGIFHWADDR:
		case SIOCGIFINDEX:
		case SIOCGIFADDR:
		case SIOCGIFBRDADDR:
		case SIOCGIFDSTADDR:
		case SIOCGIFNETMASK:
		case SIOCGIFPFLAGS:
		case SIOCGIFTXQLEN:
		case SIOCGMIIPHY:
		case SIOCGMIIREG:
		case SIOCGIFNAME:
			if (copy_to_user(uifr32, &ifr, sizeof(*uifr32)))
				err = -EFAULT;
			break;
		}
	}

	*ret = err;
}
INIT_VENDOR_HOOK(ldk_rvh_compat_ifreq_ioctl, hook_compat_ifreq_ioctl);
