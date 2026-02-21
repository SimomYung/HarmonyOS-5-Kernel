/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: The adaptation of netlink between devhost and liblinux
 * Author: Huawei OS Kernel Lab
 * Create: Thu Jul 23 07:45:22 2020
 */
#include "devhost.h"

#include <net/sock.h>
#include <linux/slab.h>
#include <linux/skbuff.h>
#include <linux/securec.h>
#include <linux/netlink.h>

#include <devhost/netlink.h>

#define MAX_MSG_SIZE	(65535U)
/* the interface to process netlink message */
int devhost_linux_netlink_recv(struct netlink_rcv_msg *args)
{
	void *skb_data = NULL;
	struct sk_buff *skb = NULL;
	struct socket *sock = NULL;
	struct sockaddr_nl sk_addr = { AF_NETLINK, 0, args->portid, 0 };
	int err;

	if ((args->msg_size) > MAX_MSG_SIZE) {
		printk(KERN_WARNING "message is to larger!\n");
		return -EMSGSIZE;
	}
	skb = alloc_skb(args->msg_size, GFP_KERNEL);
	if (skb == NULL) {
		return -ENOMEM;
	}
	skb_data = skb_put(skb, args->msg_size);
	err = copy_from_user(skb_data, args->msg, args->msg_size);
	if (err != 0) {
		kfree_skb(skb);
		return -EFAULT;
	}

	NETLINK_CB(skb).portid = args->portid;
	skb->protocol = (__be16)args->protocol;

	err = sock_create(PF_NETLINK, SOCK_DGRAM, args->protocol, &sock);
	if (err) {
		kfree_skb(skb);
		return err;
	}
	if (sock->ops == NULL || sock->ops->bind == NULL) {
		kfree_skb(skb);
		sock_release(sock);
		return -EFAULT;
	}
	err = sock->ops->bind(sock, (struct sockaddr *)&sk_addr, sizeof(sk_addr));
	if (err) {
		kfree_skb(skb);
		sock_release(sock);
		return err;
	}
	err = netlink_unicast(sock->sk, skb, 0, 0); /* to kernel */
	if ((unsigned long)err != args->msg_size) {
		printk(KERN_WARNING "netlink unicast error, err is %d\n", err);
		err = -EAGAIN;
	}
	if (sock->sk->sk_err) {
		err = sock_error(sock->sk);
	}

	sock_release(sock);
	return err;
}
EXPORT_SYMBOL(devhost_linux_netlink_recv);
