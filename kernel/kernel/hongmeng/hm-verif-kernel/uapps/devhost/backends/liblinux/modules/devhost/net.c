/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Feb 18 15:55:48 2019
 */
#include <linux/fs.h>
#include <linux/if.h>
#include <linux/can.h>
#include <linux/init.h>
#include <linux/ioctl.h>
#include <linux/genhd.h>
#include <linux/if_arp.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ethtool.h>
#include <linux/highmem.h>
#include <linux/if_vlan.h>
#include <linux/version.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/seq_file_net.h>
#include <devhost/net.h>
#include <devhost/memory.h>

#include "devhost.h"
#include "linux_device.h"

#ifndef ETH_RESERVED
/* Reserved space between skb->data and skb->head */
#define ETH_RESERVED	32
#endif

/* driver_ioctl related structure */
struct net_actv_subcall {
	int request;
	int (*callback)(struct device_info *info, unsigned long args, int request);
};

static struct net_device *devinfo2netdev(struct device_info *info)
{
	struct linux_device_object *obj = NULL;

	obj = container_of(info, struct linux_device_object, info);

	return container_of(obj->dev, struct net_device, dev);
}

/* for can & ppp device */
static void  __fill_skb_protocol(const struct net_device *dev, struct sk_buff *skb)
{
	if (dev->type == ARPHRD_CAN) {
		skb->protocol = (skb->len == CAN_MTU) ? htons(ETH_P_CAN) : htons(ETH_P_CANFD);
	} else if (dev->type == ARPHRD_PPP) {
		skb->protocol = htons(ETH_P_IP);
	}
}

static int native_net_tx(unsigned long flags,
			 unsigned int payload_len,
			 unsigned int size,
			 unsigned int offset,
			 struct device_info *info)
{
	int err;
	void *payload_addr = NULL;
	struct sk_buff *skb = NULL;
	struct net_device *pnet_dev = devinfo2netdev(info);

	if (payload_len > size) {
		pr_err("the payload_len is too large\n");
		return -EINVAL;
	}

	skb = netdev_alloc_skb(pnet_dev, payload_len);
	if (unlikely(skb == NULL)) {
		pr_err("alloc skb failed, size=%u\n", payload_len);
		return -ENOMEM;
	}

	payload_addr = (void *)(pool_offset2va(offset) + ETH_RESERVED);
	skb_put(skb, payload_len);
	err = skb_store_bits(skb, 0, payload_addr, payload_len);
	if (err) {
		pr_err("store data to skb failed\n");
		kfree_skb(skb);
		return -EFAULT;
	}
	__fill_skb_protocol(pnet_dev, skb);

	/*
	 * Do one more bh disable to prevent NET_RX softirq action from running
	 * after skb sending is complete in dev_queue_xmit, which will cause dead lock
	 * because hm-network does not support net tx and rx in same context currently.
	 */
	local_bh_disable();
	err = dev_queue_xmit(skb);
	_local_bh_enable();
	if (err < 0) {
		kfree_skb(skb);
		pr_err("dev_queue_xmit failed, err=%d\n", err);
		return err;
	}
	liblinux_pal_free_dma_pool(offset);

	return 0;
}

#if LINUX_VERSION_CODE > KERNEL_VERSION(5,10,0)
/* ldk 5.10+ */
#define call_kernel_dev_ioctl(ret, pnet_dev, cmd, ifr, need_copyout)	\
	ret = dev_ioctl(&init_net, cmd, ifr, need_copyout);
#else
#define call_kernel_dev_ioctl(ret, pnet_dev, cmd, ifr, need_copyout)	\
	if ((pnet_dev)->dev_ioctl_handler == NULL) {			\
		ret = -EOPNOTSUPP;					\
	} else {							\
		ret = (pnet_dev)->dev_ioctl_handler(&init_net, cmd, ifr, need_copyout);	\
	}
#endif

#define call_user_dev_ioctl(ret, pnet_dev, cmd, arg)	\
	if ((pnet_dev)->dev_ioctl_handler == NULL) {	\
		ret = -EOPNOTSUPP;			\
	} else {					\
		ret = (pnet_dev)->dev_ioctl_handler(&init_net, cmd, (void __user *)(uintptr_t)(arg));	\
	}

static int native_net_actv_subcall_dev_ioctl(struct device_info *info,
					     unsigned long arg, int request)
{
	struct net_device *pnet_dev = devinfo2netdev((void *)info);
	int cmd = request;
	int ret;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,19,0)
	struct ifreq ifr;
	bool need_copyout = true;
#endif
	if (!pnet_dev) {
		return -ENODEV;
	}
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,19,0)
	if (copy_from_user(&ifr, (void __user *)(uintptr_t)arg,
			   sizeof(struct ifreq))) {
		return -EFAULT;
	}

	call_kernel_dev_ioctl(ret, pnet_dev, cmd, &ifr, &need_copyout)
	if (!ret && need_copyout) {
		if (copy_to_user((void __user *)(uintptr_t)arg, &ifr,
				 sizeof(struct ifreq))) {
			return -EFAULT;
		}
	}
#else
	call_user_dev_ioctl(ret, pnet_dev, cmd, arg)
#endif

	return ret;
}

static struct net_actv_subcall net_actv_subcall_list[] = {
	{SIOCETHTOOL, native_net_actv_subcall_dev_ioctl},
	{SIOCSIFHWADDR, native_net_actv_subcall_dev_ioctl},
	{SIOCGIFHWADDR, native_net_actv_subcall_dev_ioctl},
	{SIOCSIFFLAGS, native_net_actv_subcall_dev_ioctl},
	{SIOCSIFMTU, native_net_actv_subcall_dev_ioctl},
	{SIOCADDMULTI, native_net_actv_subcall_dev_ioctl},
	{SIOCDELMULTI, native_net_actv_subcall_dev_ioctl},
};

static int native_net_ioctl(struct device_info *info, int request, unsigned long args)
{
	struct net_actv_subcall *subcall = NULL;
	int ret = -EINVAL;
	unsigned int i = 0;

	for (i = 0; i < ARRAY_SIZE(net_actv_subcall_list); i++) {
		subcall = &net_actv_subcall_list[i];
		if (subcall->request == request) {
			ret = subcall->callback(info, args, request);
			break;
		}
	}

	return ret;
}

static struct devhost_net_ops netops __read_mostly = {
	.net_tx = native_net_tx,
	.driver_ioctl = native_net_ioctl,
	.show_status = NULL,
};

static int netdev_event(struct notifier_block *this, unsigned long event, void *ptr)
{
	int ret;
	struct net_device *dev = netdev_notifier_info_to_dev(ptr);
	struct linux_device_object *obj = NULL;

	switch (event) {
	case NETDEV_REGISTER:
		/* NETDEV_REGISTER must be after device_add */
		obj = linux_device_object_of(&dev->dev);
		if (obj == NULL) {
			return -EINVAL;
		}
		devhost_device_capable_net(&obj->info, &netops);
		/* for liblinux, netdev must have a pre-allocated ifindex */
		BUG_ON(dev->ifindex <= 0);

		/*
		 * Ignore net device gro feature because if too many skbs are merged,
		 * the share memory between devhost and hmnetd maybe insufficient for
		 * store it.
		 */
		dev->features &= ~NETIF_F_GRO;
		ret = devhost_register_netdev(&obj->info, dev->name,
					      dev->features, &dev->ifindex);
		if (ret < 0) {
			pr_err("netdev_event: register netdev:%s failed\n", dev->name);
			return notifier_from_errno(ret);
		}
		break;
	case NETDEV_UNREGISTER:
		devhost_unregister_netdev(dev->ifindex);
		break;
	case NETDEV_POST_INIT:
		pr_warn_once("unhandled event :NETDEV_POST_INIT\n");
		break;
	case NETDEV_UP:
		pr_warn_once("unhandled event :NETDEV_UP\n");
		break;
	case NETDEV_DOWN:
		pr_warn_once("unhandled event :NETDEV_DOWN\n");
		break;
	case NETDEV_FEAT_CHANGE:
		dev->features &= ~NETIF_F_GRO;
		devhost_netdev_update_features(dev->ifindex, dev->features);
		break;
	default:
		pr_warn_once("unhandled event:%lu\n", event);
		break;
	}

	return NOTIFY_DONE;
}

static struct notifier_block net_dev_notifier = {
	.notifier_call	= netdev_event,
};

static int ldk_net_stack_ready(void)
{
	int ret;
	struct socket *sock = NULL;

	/* If sock is created successfully, the ldk stack is ready */
	ret = sock_create(AF_INET, SOCK_STREAM, IPPROTO_IP, &sock);
	if (sock != NULL) {
		sock_release(sock);
	}

	return ret;
}

int devhost_net_init(void)
{
	int ret;

	/* LDK net stack exist, skip net device init */
	if (ldk_net_stack_ready() == 0) {
		return 0;
	}

	ret = register_netdevice_notifier(&net_dev_notifier);
	if (ret < 0) {
		pr_err("devhost_net_init: register netdev notifier failed, ret=%d\n", ret);
		return ret;
	}
	ret = devhost_net_stack_init();
	if (ret < 0) {
		pr_err("dh net stack init failed\n");
	}

	return ret;
}
