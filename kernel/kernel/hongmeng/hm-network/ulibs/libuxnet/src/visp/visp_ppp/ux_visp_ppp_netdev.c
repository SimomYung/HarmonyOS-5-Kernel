/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Interface for network visp ppp netdev
 * Author: Huawei OS Kernel Lab
 * Create: Sun Apr 25 16:10:41 2021
 */

#include <errno.h>
#include <net/if_arp.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <net/ethernet.h>

#include <libhwsecurec/securec.h>
#include <libhmsrv_io/net.h>
#include <libhmlog/hmlog.h>
#include <libstrict/strict.h>

#include <visp_route.h>
#include <visp_netdev.h>

#include "ux_visp.h"
#include "hm_ifaddrs.h"
#include "ux_utils.h"
#include "ux_netdev.h"
#include "ux_demux.h"
#include "ux_visp_ppp_netdev.h"

struct iter_args {
	uint32_t dst_addr;

	const ux_netdev_t *dev;
	char **ifname;
};

static int iter_addr_callback(struct visp_if_addr *addr, void *arg)
{
	struct iter_args *args = (struct iter_args *)arg;

	if (ip_addr_match(args->dst_addr, addr->ip, addr->mask)) {
		*(args->ifname) = (char*)(args->dev->name);
		return 1;
	}

	return 0;
}

static int iter_device_callback(const ux_netdev_t *dev, void *data)
{
	struct iter_args *args = (struct iter_args *)data;
	struct netif *nif = ux_netdev_priv(dev);

	args->dev = dev;
	return visp_for_each_addr(nif->ifindex, iter_addr_callback, args);
}

static int visp_ppp_netif_find_ip_reachable(char **ifname, const u32_t ip_addr)
{
	struct iter_args args = {ip_addr, NULL, ifname};

	return ux_netdev_for_each_device(iter_device_callback, &args);
}

static int visp_ppp_netif_set_default(ux_netdev_t *dev, int action,
				  struct sockaddr *gw)
{
	if (action == 1) {
		return visp_del_default_route(((struct sockaddr_in *)gw)->sin_addr.s_addr);
	} else {
		return visp_set_default_route(((struct sockaddr_in *)gw)->sin_addr.s_addr);
	}
}

static int visp_ppp_set_route_entry(int action,  struct rtentry *rt)
{
	int ret = E_HM_OK;
	ux_netdev_t *dev = NULL;
	struct netif *nif = NULL;

	dev = ux_netdev_get_by_name((char *)(rt->rt_dev));
	if (dev == NULL) {
		return E_HM_NODEV;
	}

	nif = ux_netdev_priv(dev);

	if (action == 1) {
		ret = visp_del_route_entry(((struct sockaddr_in *)(&rt->rt_dst))->sin_addr.s_addr,
					    ((struct sockaddr_in *)(&rt->rt_genmask))->sin_addr.s_addr,
					    ((struct sockaddr_in *)(&rt->rt_gateway))->sin_addr.s_addr,
					    nif->ifindex);
	} else {
		ret = visp_add_route_entry(((struct sockaddr_in *)(&rt->rt_dst))->sin_addr.s_addr,
					    ((struct sockaddr_in *)(&rt->rt_genmask))->sin_addr.s_addr,
					    ((struct sockaddr_in *)(&rt->rt_gateway))->sin_addr.s_addr,
					    nif->ifindex);
	}

	ux_netdev_put(dev);

	return ret;
}

static unsigned int visp_ppp_netif_xmit(uintptr_t phy_link, void *mbuf)
{
	ux_netdev_t *dev = (void *)phy_link;
	int ret = ux_netdev_xmit(dev, mbuf, visp_mbuf_length(mbuf), 0);
	visp_destroy_mbuf(mbuf);
	return ret;
}

static unsigned int visp_ppp_netif_ioctl(uintptr_t phy_link, unsigned int cmd, char *data)
{
	net_warn(HMNET_INET, "!!! ioctl cmd:0x%x is not implemented yet\n", cmd);
	return 0;
}

static int visp_ppp_phy_init(void *dev, const unsigned int *ifindex,
			     const unsigned int *parg3, unsigned int arg4)
{
	ux_netdev_t *netdev = visp_get_arg_from_devlink(dev);
	struct netif *netif = ux_netdev_priv(netdev);

	UNUSED(parg3, arg4);

	if (dev == NULL || ifindex == NULL) {
		return E_HM_INVAL;
	}

	netif->ifindex = *ifindex;
	netif->flags |= NETIF_FLAG_POINTOPOINT;

	visp_set_netif_extra(netif->ifindex, netdev);
	visp_set_netif_xmit_hook(netif->ifindex, visp_ppp_netif_xmit);
	visp_set_netif_ioctl_hook(netif->ifindex, visp_ppp_netif_ioctl);

	return 0;
}

static int visp_ppp_device_init(ux_netdev_t *dev)
{
	struct netif *netif = ux_netdev_priv(dev);

	dev->priv_ops = visp_get_netdev_priv_ops();
	netif->netdev = visp_create_netdev(NETDEV_PPP, visp_ppp_phy_init,
					   &netif->ifindex, dev);
	if (netif->netdev == NULL) {
		return E_HM_RESFAULT;
	}

	dev->flags |= NETIF_FLAG_POINTOPOINT;

	return 0;
}

static void visp_ppp_device_uninit(const ux_netdev_t *dev)
{
	struct netif *netif = ux_netdev_priv(dev);
	int ret;

	ret = visp_delete_netdev(netif->ifindex, netif->netdev);
	if (ret != 0) {
		net_error(HMNET_INET, "visp ppp delete netif failed ret = %d\n", ret);
	}
}

static ux_packet_demux_t visp_ppp_demux = {
	.head = DLIST_HEAD_INIT(visp_ppp_demux.head),
	.dev = NULL,
	.type = ARPHRD_PPP,
	.init = visp_ppp_device_init,
	.uninit = visp_ppp_device_uninit,
	.func = NULL,
	.find_ip_reachable = visp_ppp_netif_find_ip_reachable,
	.set_default = visp_ppp_netif_set_default,
	.set_route_entry = visp_ppp_set_route_entry,
};

int visp_ppp_netdev_init(void)
{
	int ret;

	ret = ux_register_packet_demux(&visp_ppp_demux);
	if (ret != 0) {
		net_error(HMNET_PPP, "visp register ppp demux failed\n");
	}

	return ret;
}

void visp_ppp_netdev_uninit(void)
{
	ux_unregister_packet_demux(&visp_ppp_demux);
}
