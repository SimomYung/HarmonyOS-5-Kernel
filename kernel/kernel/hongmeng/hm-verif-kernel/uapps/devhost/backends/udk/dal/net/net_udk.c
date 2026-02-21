/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Net device operations in udk
 * Author: Huawei OS Kernel Lab
 * Create: Wed Sep 25 15:55:48 2019
 */
#include <inttypes.h>
#include <devhost/net.h>
#include <devhost/memory.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <udk/log.h>
#include <udk/sync.h>
#include <udk/device.h>
#include <udk/mm.h>
#include <udk/module.h>
#include <udk/api.h>
#include <hongmeng/errno.h>
#include <hongmeng/types.h>
#include <libhmsync/raw_mutex.h>
#include <libstrict/strict.h>
#include <libhwsecurec/securec.h>
#include <libsysif/devmgr/api.h>
#include <udk/net/ethtool.h>

#include "net.h"
#include "pbuf_priv.h"

raw_static_assert(sizeof(((struct ifreq *)0)->ifr_name) == sizeof(((struct ifreq32 *)0)->ifr_name),
		  ifreq32_ifr_name_size_is_wrong);

/* driver_ioctl related structure */
struct net_actv_subcall {
	int request;
	int (*callback)(struct device_info *info, unsigned long args);
};

static struct udk_net_device *devinfo2netdev(const struct device_info *info)
{
	struct udk_device *udkdev = udk_device_of(info);

	if (udkdev != NULL) {
		return container_of(udkdev, struct udk_net_device, udk_dev);
	}
	return NULL;
}

static int native_net_tx(unsigned long flags,
			 unsigned int payload_len,
			 unsigned int size,
			 unsigned int shm_offset,
			 struct device_info *info)
{
	uintptr_t addr;
	struct pkt_buf *pbuf = NULL;
	struct udk_net_device *pnet_dev = devinfo2netdev(info);

	UNUSED(flags);
	addr = pool_offset2va(shm_offset);
	pbuf = udk_alloc_pbuf(pnet_dev, (void *)(uintptr_t)addr);
	if (pbuf == NULL) {
		udk_error("udk_build_pbuf failed. require size=%u\n", size);
		return E_HM_NOMEM;
	}

	udk_pbuf_update_len(pbuf, payload_len);
	udk_pbuf_reserve_eth(pbuf);

	return udk_netif_tx(pbuf);
}

static int native_net_show_status(struct device_info *info, struct netdev_status *p)
{
	struct udk_net_device *pnet_dev = devinfo2netdev(info);

	if (pnet_dev == NULL || pnet_dev->netdev_ops == NULL ||
	    pnet_dev->netdev_ops->show_net_status  == NULL) {
		  return 0;
	}

	p->rx_in         = pnet_dev->dev_stats.rx_in;
	p->tx_in         = pnet_dev->dev_stats.tx_in;
	p->drv_alloc     = pnet_dev->dev_stats.drv_alloc;
	p->drv_free      = pnet_dev->dev_stats.drv_free;
	p->devhost_alloc = pnet_dev->dev_stats.devhost_alloc;
	p->devhost_free  = pnet_dev->dev_stats.devhost_free;
	p->full_drop     = pnet_dev->dev_stats.full_drop;
	p->deact_drop    = pnet_dev->dev_stats.deact_drop;

	udk_debug("tx_in=%llu drv_free=%llu devhost_free=%llu full_drop=%llu deact_drop=%llu\n",
		  p->tx_in, p->drv_free, p->devhost_free,
		  p->full_drop, p->deact_drop);
	udk_debug("rx_in=%llu drv_alloc=%llu devhost_alloc=%llu\n",
		  p->rx_in, p->drv_alloc, p->devhost_alloc);

	pnet_dev->netdev_ops->show_net_status(pnet_dev);

	return 0;
}

static inline struct udk_module *udk_netdev_owner_of(const struct udk_net_device *udk_netdev)
{
	if (udk_netdev == NULL || udk_netdev->netdev_ops == NULL ||
	    udk_netdev->netdev_ops->owner == NULL) {
		    return NULL;
	}

	return (struct udk_module *)(*udk_netdev->netdev_ops->owner);
}

static int native_net_actv_subcall_set_flags(struct device_info *info, unsigned long args)
{
	struct udk_net_device *pnet_dev = devinfo2netdev(info);
	struct ifreq ifr;
	unsigned int change = 0;
	int ret = 0;

	mem_zero_s(ifr);
	BUG_ON(pnet_dev == NULL);

	if (udk_copy_from_user(&ifr, (void *)(uintptr_t)args,
			       sizeof(struct ifreq)) != 0) {
		return -EFAULT;
	}

	udk_mutex_lock(&pnet_dev->flags_lock);
	change = pnet_dev->flags ^ (unsigned int)ifr.ifr_flags;
	if ((pnet_dev->flags & IFF_UP) != ((unsigned short)ifr.ifr_flags & IFF_UP)) {
		if (((unsigned short)ifr.ifr_flags & IFF_UP) != 0) {
			ret = udk_module_open(udk_netdev_owner_of(pnet_dev));
			if (ret != 0) {
				goto restore;
			}
			ret = udk_device_opener_inc(&pnet_dev->udk_dev);
			if (ret != 0) {
				udk_module_close(udk_netdev_owner_of(pnet_dev));
				goto restore;
			}
			ret = udk_netdev_open(pnet_dev);
			if (ret != 0) {
				udk_device_opener_dec(&pnet_dev->udk_dev);
				udk_module_close(udk_netdev_owner_of(pnet_dev));
				goto restore;
			}
		} else {
			ret = udk_netdev_close(pnet_dev);
			if (ret != 0) {
				goto restore;
			}
			udk_device_opener_dec(&pnet_dev->udk_dev);
			udk_module_close(udk_netdev_owner_of(pnet_dev));
		}
	}

restore:
	if (ret == 0) {
		pnet_dev->flags = (unsigned int)(int)ifr.ifr_flags;
		if ((change & IFF_PROMISC) != 0 || (change & IFF_ALLMULTI) != 0) {
			if (pnet_dev->netdev_ops != NULL && pnet_dev->netdev_ops->change_rx_flags != NULL) {
				pnet_dev->netdev_ops->change_rx_flags(pnet_dev, (int)change);
			}
		}
	}
	udk_mutex_unlock(&pnet_dev->flags_lock);

	return ret;
}

static int native_net_actv_subcall_add_mc_list(struct device_info *info, unsigned long args)
{
	struct udk_net_device *pnet_dev = devinfo2netdev(info);
	struct ifreq ifr;

	BUG_ON(pnet_dev == NULL);

	if (pnet_dev == NULL || pnet_dev->netdev_ops == NULL ||
	    pnet_dev->netdev_ops->set_multicast_list == NULL) {
		return -ENOSYS;
	}

	mem_zero_s(ifr);
	if (udk_copy_from_user(&ifr, ptr_to_void(args),
			       sizeof(struct ifreq)) != 0) {
		return -EFAULT;
	}

	return pnet_dev->netdev_ops->set_multicast_list(pnet_dev, ifr.ifr_hwaddr.sa_data, true);
}

static int native_net_actv_subcall_del_mc_list(struct device_info *info, unsigned long args)
{
	struct udk_net_device *pnet_dev = devinfo2netdev(info);
	struct ifreq ifr;

	BUG_ON(pnet_dev == NULL);

	if (pnet_dev == NULL || pnet_dev->netdev_ops == NULL ||
	    pnet_dev->netdev_ops->set_multicast_list == NULL) {
		return -ENOSYS;
	}

	mem_zero_s(ifr);
	if (udk_copy_from_user(&ifr, (void *)(uintptr_t)args,
			       sizeof(struct ifreq)) != 0) {
		return -EFAULT;
	}

	return pnet_dev->netdev_ops->set_multicast_list(pnet_dev, ifr.ifr_hwaddr.sa_data, false);
}

static int native_net_actv_subcall_set_mac(struct device_info *info, unsigned long args)
{
	struct udk_net_device *pnet_dev = devinfo2netdev(info);
	struct sockaddr sockaddr_net = {0};
	struct ifreq ifr;
	int ret = 0;

	if (pnet_dev == NULL || pnet_dev->netdev_ops == NULL ||
	    pnet_dev->netdev_ops->set_mac_address == NULL) {
		return -ENOSYS;
	}

	mem_zero_s(ifr);
	if (udk_copy_from_user(&ifr, (void *)(uintptr_t)args,
			       sizeof(struct ifreq)) != 0) {
		return -EFAULT;
	}

	NOFAIL(memcpy_s(sockaddr_net.sa_data, sizeof(sockaddr_net.sa_data),
			ifr.ifr_hwaddr.sa_data, sizeof(sockaddr_net.sa_data)));

	udk_mutex_lock(&pnet_dev->dev_lock);
	ret = pnet_dev->netdev_ops->set_mac_address(pnet_dev, &sockaddr_net);
	udk_mutex_unlock(&pnet_dev->dev_lock);

	return ret;
}

static int native_net_actv_subcall_get_mac(struct device_info *info, unsigned long args)
{
	struct udk_net_device *pnet_dev = devinfo2netdev(info);
	struct ifreq ifr;
	int ret;

	if (pnet_dev == NULL) {
		return -ENOSYS;
	}

	mem_zero_s(ifr);
	udk_mutex_lock(&pnet_dev->dev_lock);
	NOFAIL(memcpy_s(ifr.ifr_hwaddr.sa_data, sizeof(ifr.ifr_hwaddr.sa_data),
			pnet_dev->mac_addr, sizeof(pnet_dev->mac_addr)));
	udk_mutex_unlock(&pnet_dev->dev_lock);

	ret = udk_copy_to_user(ulong_to_ptr(args, void), &ifr, sizeof(struct ifreq));

	return ret;
}

/*
 * These two functions, only for ethtool.
 * Ifreq contains a union variable, so it cannot be written as a common conversion function.
 * Currently, Only the ethtool activation call request is forwarded by hmnetd, need to be processed for ILP32.
 */
static int udk_copy_ethtool_ifreq_from_user(unsigned long arg, struct ifreq *ifr)
{
	struct ifreq32 ifr32;
	int ret;

	mem_zero_s(ifr32);
	if (udk_is_compat_client()) {
		ret = udk_copy_from_user(&ifr32, ulong_to_ptr(arg, void), sizeof(struct ifreq32));
		if (ret < 0) {
			return ret;
		}

		/* ethtool only care ifr_name and ifr_data */
		NOFAIL(memcpy_s(ifr->ifr_name, sizeof(ifr->ifr_name),
				ifr32.ifr_name, sizeof(ifr32.ifr_name)));
		ifr->ifr_data = (void *)(uintptr_t)ifr32.ifr_data;

		return ret;
	}

	return udk_copy_from_user(ifr, ulong_to_ptr(arg, void), sizeof(struct ifreq));
}

static int udk_copy_ethtool_ifreq_to_user(unsigned long arg, const struct ifreq *ifr)
{
	struct ifreq32 ifr32;

	mem_zero_s(ifr32);
	if (udk_is_compat_client()) {
		/* ethtool only care ifr_name and ifr_data */
		NOFAIL(memcpy_s(ifr32.ifr_name, sizeof(ifr32.ifr_name),
				ifr->ifr_name, sizeof(ifr->ifr_name)));
		ifr32.ifr_data = (unsigned int)(uintptr_t)ifr->ifr_data;

		return udk_copy_to_user(ulong_to_ptr(arg, void), &ifr32, sizeof(struct ifreq32));
	}

	return udk_copy_to_user(ulong_to_ptr(arg, void), ifr, sizeof(struct ifreq));
}

static int native_net_actv_subcall_ethtool_ioctl(struct device_info *info,
						 unsigned long arg)
{
	struct udk_net_device *pnet_dev = devinfo2netdev(info);
	struct ifreq ifr;
	char *col = NULL;
	int ret;

	if (pnet_dev == NULL) {
		return -ENODEV;
	}

	mem_zero_s(ifr);
	ret = udk_copy_ethtool_ifreq_from_user(arg, &ifr);
	if (ret < 0) {
		return ret;
	}

	ifr.ifr_name[IFNAMSIZ - 1] = 0;
	col = strchr(ifr.ifr_name, ':');
	if (col != NULL) {
		*col = '\0';
	}

	udk_mutex_lock(&pnet_dev->dev_lock);
	ret = udk_net_call_ethtool(pnet_dev, &ifr);
	udk_mutex_unlock(&pnet_dev->dev_lock);
	if (ret == 0) {
		if (col != NULL) {
			*col = ':';
		}

		ret = udk_copy_ethtool_ifreq_to_user(arg, &ifr);
	}

	return ret;
}

static int native_net_actv_subcall_set_mtu(struct device_info *info, unsigned long arg)
{
	/*
	* The udk framework does not support the MTU setting yet.
	* Add the specific implementation if necessary.
	*/
	UNUSED(info, arg);
	return 0;
}

static struct net_actv_subcall net_actv_subcall_list[] = {
	{SIOCSIFHWADDR, native_net_actv_subcall_set_mac},
	{SIOCGIFHWADDR, native_net_actv_subcall_get_mac},
	{SIOCSIFFLAGS, native_net_actv_subcall_set_flags},
	{SIOCSIFMTU, native_net_actv_subcall_set_mtu},
	{SIOCETHTOOL, native_net_actv_subcall_ethtool_ioctl},
	{SIOCADDMULTI, native_net_actv_subcall_add_mc_list},
	{SIOCDELMULTI, native_net_actv_subcall_del_mc_list},
};

static int native_net_ioctl(struct device_info *info, int request, unsigned long args)
{
	struct net_actv_subcall *subcall = NULL;
	int ret = -EINVAL;
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(net_actv_subcall_list); i++) {
		subcall = &net_actv_subcall_list[i];
		if (subcall->request == request) {
			ret = subcall->callback(info, args);
			break;
		}
	}

	return ret;
}

struct devhost_net_ops udk_net_ops __read_mostly = {
	.net_tx = native_net_tx,
	.show_status = native_net_show_status,
	.driver_ioctl = native_net_ioctl,
};
