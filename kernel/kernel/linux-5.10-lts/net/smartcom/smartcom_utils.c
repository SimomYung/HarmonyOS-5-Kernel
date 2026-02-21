/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 */

#include "smartcom_utils.h"

#include <linux/module.h>
#include <linux/types.h>
#include <linux/socket.h>
#include <linux/in.h>
#include <linux/in6.h>
#include <linux/netdevice.h>
#include <net/sock.h>
#include <net/tcp.h>
#include <net/udp.h>

#undef HWLOG_TAG
#define HWLOG_TAG smartcom_utils
HWLOG_REGIST();
MODULE_LICENSE("GPL");

bool smartcom_check_ip_is_private(struct sockaddr *addr)
{
	struct sockaddr_in *usin_tmp = (struct sockaddr_in *)addr;

	if (usin_tmp->sin_family == AF_INET) {
		return (ipv4_is_linklocal_169(usin_tmp->sin_addr.s_addr) ||
			ipv4_is_private_10(usin_tmp->sin_addr.s_addr) ||
			ipv4_is_private_172(usin_tmp->sin_addr.s_addr) ||
			ipv4_is_private_192(usin_tmp->sin_addr.s_addr));
	} else if (usin_tmp->sin_family == AF_INET6) {
		struct sockaddr_in6 *usin6_tmp = (struct sockaddr_in6 *)addr;
		int addr_type = ipv6_addr_type(&usin6_tmp->sin6_addr);
		if (addr_type & IPV6_ADDR_MAPPED) {
			__be32 s_addr = usin6_tmp->sin6_addr.s6_addr32[SMARTCOM_CLAT_IPV4_INDEX];
			return (ipv4_is_linklocal_169(s_addr) ||
				ipv4_is_private_10(s_addr) ||
				ipv4_is_private_172(s_addr) ||
				ipv4_is_private_192(s_addr));
		}
	}

	return false;
}

bool smartcom_check_ip_addrss(struct sockaddr *addr)
{
	struct sockaddr_in *usin = (struct sockaddr_in *)addr;

	if (usin->sin_family == AF_INET) {
		return !ipv4_is_loopback(usin->sin_addr.s_addr) && !ipv4_is_multicast(usin->sin_addr.s_addr) &&
				!ipv4_is_zeronet(usin->sin_addr.s_addr) && !ipv4_is_lbcast(usin->sin_addr.s_addr);
	} else if (usin->sin_family == AF_INET6) {
		struct sockaddr_in6 *usin6_tmp = (struct sockaddr_in6 *)addr;
		return !ipv6_addr_loopback(&usin6_tmp->sin6_addr) && !ipv6_addr_is_multicast(&usin6_tmp->sin6_addr);
	}

	return true;
}

void smartcom_reset_tcp_skerror(struct sock *sk, int32_t err_num)
{
	smartcom_logd("reset sk %pK sport is %u, state[%u]", sk, sk->sk_num, sk->sk_state);
	if (sk->sk_state == TCP_ESTABLISHED || sk->sk_state == TCP_SYN_SENT) {
		local_bh_disable();
		bh_lock_sock(sk);

		if (!sock_flag(sk, SOCK_DEAD)) {
			sk->sk_err = err_num;
			/* This barrier is coupled with smp_rmb() in tcp_poll() */
			smp_wmb();
			sk->sk_error_report(sk);
			if (tcp_need_reset(sk->sk_state))
				tcp_send_active_reset(sk, GFP_ATOMIC);
			tcp_done(sk);
		}

		bh_unlock_sock(sk);
		local_bh_enable();
	}
}

void smartcom_reset_udp_skerror(struct sock *sk, int32_t err_num)
{
	smartcom_logd("reset sk %pK sport is %u, state[%u]", sk, sk->sk_num, sk->sk_state);
	local_bh_disable();
	bh_lock_sock(sk);

	if (!sock_flag(sk, SOCK_DEAD)) {
		sk->sk_err = err_num;
		smp_wmb();
		sk->sk_error_report(sk);
	}

	bh_unlock_sock(sk);
	local_bh_enable();
}

void smartcom_enable_selected_path(struct sock *sk, const char *selected_path_iface)
{
	struct net_device *net_dev = dev_get_by_name(sock_net(sk), selected_path_iface);
	struct inet_sock *inet = NULL;
	if (net_dev) {
		unsigned int flags = dev_get_flags(net_dev);
		if (flags & IFF_RUNNING) {
			sk->sk_bound_dev_if = net_dev->ifindex;
			sk_dst_reset(sk);
			inet = inet_sk(sk);
			if (inet->inet_saddr)
				inet->inet_saddr = 0;
		}
		dev_put(net_dev);
	}
}

