/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#include "smartcom/smartcom_interface.h"

#include <linux/netdevice.h>
#include <linux/inetdevice.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/in.h>
#include <net/inet_sock.h>
#include "smartcom_utils.h"

#ifdef CONFIG_HUAWEI_SMARTCOM_MPFLOW
#include "mpflow/smartcom_mpflow.h"
#endif

#undef HWLOG_TAG
#define HWLOG_TAG smartcom
HWLOG_REGIST();

static bool smartcom_bind2device_done(char *ifname, struct sock *sk)
{
	struct net_device *dev = NULL;
	struct inet_sock *inet = NULL;
	struct in_device *in_dev = NULL;

	dev = dev_get_by_name(sock_net(sk), ifname);
	if (!dev) {
		smartcom_loge("not found netdevice %s", ifname);
		return false;
	}

	in_dev = in_dev_get(dev);
	if (!in_dev) {
		smartcom_loge("get inet addr in_dev is null dev: %s", dev->name);
		dev_put(dev);
		return false;
	}

	if (in_dev->ifa_list == NULL) {
		smartcom_loge("get ifa_list in_dev is null dev: %s", dev->name);
		in_dev_put(in_dev);
		dev_put(dev);
		return false;
	}

	in_dev_put(in_dev);
	sk->sk_bound_dev_if = dev->ifindex;
	dev_put(dev);
	if (sk->sk_protocol == IPPROTO_UDP) {
		sk_dst_reset(sk);
		inet = inet_sk(sk);
		if (inet->inet_saddr)
			inet->inet_saddr = 0;
	}
	return true;
}

void smartcom_bind2device(struct sock *sk, struct sockaddr *uaddr)
{
	char ifname[IFNAMSIZ] = {0};
	uid_t uid;
	int err;

	if (!sk || !uaddr)
		return;

	uid = sock_i_uid(sk).val;

#ifdef CONFIG_HUAWEI_SMARTCOM_MPFLOW
	err = smartcom_mpflow_get_best_device(uid, sk, uaddr, ifname, IFNAMSIZ - 1);
	if (err != MPFLOW_OK)
		return;
	if (!smartcom_bind2device_done(ifname, sk))
		return;

	struct sockaddr_in *usin = (struct sockaddr_in *)uaddr;
	if (usin->sin_family == AF_INET) {
		smartcom_logi("Bind Completed. uid: %u, ifname: %s, DstIP:"IPV4_FMT", DstPort: %u ",
			uid, ifname, ipv4_info(usin->sin_addr.s_addr), ntohs(usin->sin_port));
	} else if (usin->sin_family == AF_INET6) {
		struct sockaddr_in6 *usin6 = (struct sockaddr_in6 *)uaddr;
		smartcom_logi("Bind Completed. uid: %u, ifname: %s, DstIP:"IPV6_FMT", DstPort: %u ",
			uid, ifname, ipv6_info(usin6->sin6_addr), ntohs(usin6->sin6_port));
	}
#endif
}
EXPORT_SYMBOL(smartcom_bind2device);

void smartcom_tcp_fallback(struct sock *sk, int reason, int state)
{
#ifdef CONFIG_HUAWEI_SMARTCOM_MPFLOW
	smartcom_mpflow_tcp_fallback(sk, reason, state);
#endif
}
EXPORT_SYMBOL(smartcom_tcp_fallback);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("smartcom module driver");