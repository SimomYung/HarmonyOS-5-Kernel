/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: interface for network visp netdev
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 29 09:56:30 2021
 */

#include <errno.h>
#include <net/if_arp.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <netinet/if_ether.h>

#include <libhwsecurec/securec.h>
#include <libhmsrv_io/net.h>
#include <libhmlog/hmlog.h>
#include <libstrict/strict.h>

#include <visp_route.h>
#include <visp_netdev.h>

#include "ux_phy_mem.h"
#include "hm_ifaddrs.h"
#include "ux_utils.h"
#include "ux_netdev.h"
#include "ux_demux.h"
#include "ux_libvisp.h"
#include "ux_compat.h"

#define MTU_MAX		65535

static int visp_netif_get_mac(ux_netdev_t *dev,
			      unsigned char *buf, size_t size)
{
	struct netif *netif = ux_netdev_priv(dev);

	if (size < ETHER_ADDR_LEN) {
		return E_HM_INVAL;
	}

	return visp_get_mac(netif->ifindex, buf, size);
}

static int visp_netif_set_mac(ux_netdev_t *dev,
			      const unsigned char *buf, size_t size)
{
	struct netif *netif = ux_netdev_priv(dev);

	if (size < ETHER_ADDR_LEN) {
		return E_HM_INVAL;
	}

	return visp_set_mac(netif->ifindex, buf, size);
}

struct netif_flag_map {
	unsigned short flag;
	int (*set)(unsigned int ifindex);
	int (*unset)(unsigned int ifindex);
};

static struct netif_flag_map netif_flag_maps[] = {
	{IFF_ALLMULTI,      NULL,               NULL},
	{IFF_MULTICAST,     NULL,               NULL},
	{IFF_SLAVE,         NULL,               NULL},
	{IFF_MASTER,        NULL,               NULL},
	{IFF_UP,            visp_set_netif_up,  visp_set_netif_down},
};

static int visp_netif_set_flag(struct netif *netif, unsigned short flags, struct netif_flag_map *map)
{
	int ret = 0;

	if ((netif->flags & map->flag) != (flags & map->flag)) {
		if (flags & map->flag) {
			if (map->set == NULL || (ret = map->set(netif->ifindex)) == 0)
				netif->flags |= map->flag;
		} else {
			if (map->unset == NULL || (ret = map->unset(netif->ifindex)) == 0)
				netif->flags &= ~map->flag;
		}
	}

	return ret;
}

static int visp_set_if_flags(ux_netdev_t *dev, short int flags)
{
	int ret = 0;
	struct netif *netif = ux_netdev_priv(dev);
	unsigned short uflags = (unsigned short)flags;

	for (unsigned int i = 0; i < ARRAY_SIZE(netif_flag_maps); i++) {
		if ((ret = visp_netif_set_flag(netif, uflags, &netif_flag_maps[i])) != 0) {
			break;
		}
	}

	return ret;
}

static int visp_netif_set_flags(ux_netdev_t *dev, short int flags, const char *name)
{
	int ret;
	struct netif *netif = ux_netdev_priv(dev);
	char *colon = strchr(name, ':');
	if (!colon) {
		ret = visp_set_if_flags(dev, flags);
	} else {
		/* only the link status can be set for a sub-interface. */
		ret = visp_set_alias_if_updown(netif->ifindex, colon + 1,
					       (flags & IFF_UP) ? 1 : 0);
	}

	return ret;
}

static int visp_netif_set_ipaddr(ux_netdev_t *dev, struct sockaddr *addr,
				 const char *name, unsigned int ipmask, bool netlink)
{
	int ret;
	struct netif *netif = ux_netdev_priv(dev);
	char *colon = strchr(name, ':');
	if (!colon) {
		ret = visp_set_ip(netif->ifindex, (struct sockaddr_in*)addr, ipmask, netlink);
	} else {
		ret = visp_set_alias_if_addr(netif->ifindex, colon + 1, (struct sockaddr_in*)addr);
	}

	return ret;
}

static int visp_netif_del_ipaddr(ux_netdev_t *dev, struct sockaddr *addr, unsigned int ipmask)
{
	int ret;
	struct netif *netif = ux_netdev_priv(dev);

	ret = visp_del_ip(netif->ifindex, (struct sockaddr_in*)addr, ipmask);

	return ret;
}

static int visp_netif_set_netmask(ux_netdev_t *dev, struct sockaddr *mask,
				  const char *name)
{
	int ret;
	struct netif *netif = ux_netdev_priv(dev);
	char *colon = strchr(name, ':');
	if (!colon) {
		ret = visp_set_mask(netif->ifindex, (struct sockaddr_in*)mask);
	} else {
		ret = visp_set_alias_if_mask(netif->ifindex, colon + 1, (struct sockaddr_in*)mask);
	}

	return ret;
}

static int visp_netif_set_dstaddr(ux_netdev_t *dev, struct sockaddr *addr)
{
	net_warn(HMNET_INET, "!!! do not support set dst addr\n");
	return 0;
}

static void visp_netif_get_flags(ux_netdev_t *dev, short int *flags, const char *name)
{
	UNUSED(name);
	struct netif *netif = ux_netdev_priv(dev);
	*flags = (short int)(netif->flags & 0xFFFF);
}

static int visp_netif_get_mtu(ux_netdev_t *dev, int *mtu)
{
	struct netif *netif = ux_netdev_priv(dev);
	int ret;

	ret = visp_get_mtu(netif->ifindex);
	if (ret < 0) {
		return ret;
	}

	*mtu = ret;
	return 0;
}

static void visp_netif_get_ipaddr(ux_netdev_t *dev, struct sockaddr_in *sockaddr, const char *name)
{
	struct netif *netif = ux_netdev_priv(dev);
	char *colon = strchr(name, ':');
	if (!colon) {
		visp_get_ip(netif->ifindex, sockaddr);
	} else {
		visp_get_alias_if_addr(netif->ifindex, colon + 1, sockaddr);
	}
}

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

static int visp_netif_find_ip_reachable(char **ifname, const u32_t ip_addr)
{
	struct iter_args args = {ip_addr, NULL, ifname};

	return ux_netdev_for_each_device(iter_device_callback, &args);
}

static void visp_netif_get_netmask(ux_netdev_t *dev,
				   struct sockaddr_in *sockaddr, const char *name)
{
	struct netif *netif = ux_netdev_priv(dev);
	char *colon = strchr(name, ':');
	if (!colon) {
		visp_get_mask(netif->ifindex, sockaddr);
	} else {
		visp_get_alias_if_mask(netif->ifindex, colon + 1, sockaddr);
	}
}

static void visp_netif_get_dstaddr(ux_netdev_t *dev,
				   struct sockaddr_in *sockaddr)
{
	sockaddr->sin_addr.s_addr = 0;
	sockaddr->sin_family = AF_INET;
}

static int visp_netif_set_mtu(ux_netdev_t *dev, int mtu)
{
	struct netif *netif = ux_netdev_priv(dev);

	return visp_set_mtu(netif->ifindex, mtu);
}

static int visp_netif_set_rxhwcsum(ux_netdev_t *dev, int enable)
{
	net_warn(HMNET_INET, "!!! set_rxhwcsum do not support now\n");
	return 0;
}

static int visp_netif_set_default(ux_netdev_t *dev, int action,
				  struct sockaddr *gw)
{
	if (action == 1) {
		return visp_del_default_route(((struct sockaddr_in *)gw)->sin_addr.s_addr);
	} else {
		return visp_set_default_route(((struct sockaddr_in *)gw)->sin_addr.s_addr);
	}
}

static int visp_set_route_entry(int action,  struct rtentry *rt)
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

static int visp_netif_packet_rcv(ux_netdev_t *dev, const void *data, size_t len)
{
	struct netif *netif = ux_netdev_priv(dev);

	return visp_netif_recv_data(netif->ifindex, (void*)data, len);
}

static int visp_netif_packet_rcv_zero_copy(ux_netdev_t *dev,
					   void *data, size_t len, size_t offset)
{
	struct netif *netif = ux_netdev_priv(dev);
	int ret;

	ret = visp_netif_recv_data(netif->ifindex, data, len);
	if (ret == 0) {
		phy_mem_free(dev->shmmgr, data, offset);
	}

	return ret;
}

static int visp_netif_copy_data(ux_netdev_t *dev,
				const void *from, void *to, size_t len, size_t offset)
{
	if (dev == NULL) {
		return -1;
	}

	return visp_mbuf_copy_data(from, to, len, offset);
}

static int visp_netif_dump_stats(ux_netdev_t *dev,
		void *buf, size_t len)
{
	net_warn(HMNET_INET, "!!! dump stats do not support now\n");
	return 0;
}

static int visp_netif_get_stats(ux_netdev_t *dev, int file_bytes, char *result)
{
	struct netif *netif = ux_netdev_priv(dev);
	struct visp_netif_stat stat;
	int ret;

	mem_zero_s(stat);

	ret = visp_get_netif_stat(netif->ifindex, &stat);
	if (ret != 0) {
		return ret;
	}

	ret = snprintf_s(result, (unsigned int)file_bytes, (unsigned int)file_bytes - 1,
			 "%6s: %7u %7u %4u %4u %4u %5u %10u %9u "
			 "%8u %7u %4u %4u %4u %5u %7u %10u\n",
			 dev->name, stat.ibytes, stat.ipackets,
			 stat.ierror,
			 stat.idrop_packets,
			 0U,
			 0U,
			 0U, stat.mcasts,
			 stat.obytes, stat.opackets,
			 0U, stat.odrop_packets,
			 0U, 0U,
			 0U,
			 0U);
	if (ret < 0) {
		net_warn(HMNET_INET, "print lwip netif stats failed\n");
		return E_HM_INVAL;
	}

	return 0;
}

static int visp_netif_get_main_ifconf(ux_netdev_t *dev, void *ifr_dst, void **next_dst)
{
	struct ifreq ifr = {0};
	int ret = strcpy_s(ifr.ifr_ifrn.ifrn_name, IFNAMSIZ, dev->name);
	if (ret != 0) {
		net_warn(HMNET_INET, "strcpy failed, name is %s\n", dev->name);
		return E_HM_POSIX_FAULT;
	}

	visp_netif_get_ipaddr(dev, (struct sockaddr_in *)(&(ifr.ifr_addr)), dev->name);
	return ux_netdev_ifr_copy_to_user(ifr_dst, &ifr, next_dst);
}

static int copy_ifconf(void *dst, struct visp_ifaddr *ifaddr, void **next_dst)
{
	struct ifreq ifr = {0};
	int ret = strcpy_s(ifr.ifr_ifrn.ifrn_name, IFNAMSIZ, ifaddr->name);
	if (ret != 0) {
		net_warn(HMNET_INET, "strcpy failed, name is %s\n", ifaddr->name);
		return E_HM_POSIX_FAULT;
	}

	struct sockaddr_in *addr = (struct sockaddr_in *)(&(ifr.ifr_addr));
	addr->sin_addr.s_addr = htonl(ifaddr->ipaddr);
	addr->sin_family = AF_INET;

	return ux_netdev_ifr_copy_to_user(dst, &ifr, next_dst);
}

static int visp_netif_get_alias_ifconf(ux_netdev_t *dev, void *ifr_dst, int ifr_len, int *size)
{
	int copy_cnt = ifr_len;
	if (!ifr_dst) {
		copy_cnt = 0;
	}

	struct visp_main_if mainif = {0};
	struct netif *netif = ux_netdev_priv(dev);
	mainif.ifindex = netif->ifindex;
	int ret = strcpy_s(mainif.name, IFNAMSIZ, dev->name);
	if (ret != 0) {
		net_warn(HMNET_INET, "strcpy failed, name is %s\n", dev->name);
		return E_HM_POSIX_FAULT;
	}

	return visp_for_each_alias_addr(&mainif, copy_cnt, size, ifr_dst, copy_ifconf);
}

static int visp_netif_get_ifconf(ux_netdev_t *dev, void *ifr_dst, int ifr_len, int *size)
{
	if (ifr_dst != NULL) {
		int ret = visp_netif_get_main_ifconf(dev, ifr_dst, &ifr_dst);
		if (ret != 0) {
			return ret;
		}
	}

	*size = 1;

	int cnt;
	int ret = visp_netif_get_alias_ifconf(dev, ifr_dst, ifr_len - 1, &cnt);
	if ( ret != 0) {
		return ret;
	}

	*size += cnt;
	return 0;
}

static int visp_netif_get_main_ifaddrs(ux_netdev_t *dev, char *buf, size_t size)
{
	struct netif *netif = ux_netdev_priv(dev);
	struct ifaddrs_storage_internal *ifp = NULL;
	struct sockaddr_in my_addr = {0};
	size_t ifaddrs_len = sizeof(struct ifaddrs_storage_internal);
	uint32_t ipaddr = 0;
	uint32_t ipmask = 0;
	int ret;

	if (size < ifaddrs_len) {
		return 0;
	}

	ifp = (struct ifaddrs_storage_internal *)(void *)buf;
	mem_zero_s(*ifp);
	NOFAIL(memcpy_s(ifp->name, sizeof(ifp->name), dev->name, IFNAMSIZ));

	ifp->ifa_flags = netif->flags;

	my_addr.sin_family = AF_INET;

	ret = visp_get_ip_mask(netif->ifindex, &ipaddr, &ipmask);
	if (ret != 0) {
		net_warn(HMNET_INET, "visp get main ip addr failed ret = %d\n", ret);
	}

	my_addr.sin_addr.s_addr = ipaddr;
	NOFAIL(memcpy_s(&ifp->ifa_addr, sizeof(ifp->ifa_addr), &my_addr, sizeof(my_addr)));

	my_addr.sin_addr.s_addr = ipmask;
	NOFAIL(memcpy_s(&ifp->ifa_netmask, sizeof(ifp->ifa_netmask), &my_addr, sizeof(my_addr)));

	my_addr.sin_addr.s_addr = 0;
	NOFAIL(memcpy_s(&ifp->ifa_ifu.ifu_dstaddr, sizeof(ifp->ifa_ifu.ifu_dstaddr),
			&my_addr, sizeof(my_addr)));

	ifp->stats.packets_received = 0;
	ifp->stats.packets_transmitted = 0;
	ifp->stats.total_bytes_received = 0;
	ifp->stats.total_bytes_transmitted = 0;
	ifp->stats.errors_received = 0;
	ifp->stats.errors_transmit = 0;
	ifp->stats.dropped_recv = 0;
	ifp->stats.dropped_send = 0;
	ifp->stats.multicast = 0;

	return (int)ifaddrs_len;
}

static int copy_ifaddrs(void *dst, struct visp_ifaddr *ifaddr, void **next_dst)
{
	struct ifaddrs_storage_internal *ifp = (struct ifaddrs_storage_internal *)dst;
	mem_zero_s(*ifp);
	NOFAIL(memcpy_s(ifp->name, sizeof(ifp->name), ifaddr->name, IFNAMSIZ));

	struct sockaddr_in *addr;
	addr = (struct sockaddr_in *)(&ifp->ifa_addr);
	addr->sin_addr.s_addr = htonl(ifaddr->ipaddr);
	addr->sin_family = AF_INET;

	addr = (struct sockaddr_in *)(&ifp->ifa_netmask);
	addr->sin_addr.s_addr = htonl(ifaddr->netmask);
	addr->sin_family = AF_INET;

	addr = (struct sockaddr_in *)(&ifp->ifa_ifu.ifu_dstaddr);
	addr->sin_addr.s_addr = 0;
	addr->sin_family = AF_INET;

	ifp->ifa_flags = ifaddr->flags;

	*next_dst = ifp + 1;
	return 0;
}

static int visp_netif_get_alias_ifaddrs(ux_netdev_t *dev, char *buf, size_t size, int *cnt)
{
	struct visp_main_if mainif = {0};
	struct netif *netif = ux_netdev_priv(dev);
	mainif.ifindex = netif->ifindex;
	mainif.flags = netif->flags;
	int ret = strcpy_s(mainif.name, IFNAMSIZ, dev->name);
	if (ret != 0) {
		net_warn(HMNET_INET, "strcpy failed, name is %s\n", dev->name);
		*cnt = 0;
		return 0;
	}

	size_t ifaddrs_len = sizeof(struct ifaddrs_storage_internal);
	int copy_cnt = size / ifaddrs_len;
	(void)visp_for_each_alias_addr(&mainif, copy_cnt, cnt, buf, copy_ifaddrs);
	return (*cnt) * ifaddrs_len;
}

static int visp_netif_getifaddrs(ux_netdev_t *dev, char *buf, size_t size, int *cnt)
{
	int main_len = visp_netif_get_main_ifaddrs(dev, buf, size);
	int left_len = size - main_len;
	if (main_len == 0 || left_len < main_len) {
		*cnt = 0;
		return main_len;
	}

	int alias_cnt;
	int alias_len = visp_netif_get_alias_ifaddrs(dev, buf + main_len, left_len, &alias_cnt);
	*cnt = alias_cnt + 1;
	return main_len + alias_len;
}

static size_t visp_get_packet_hdr_len(ux_netdev_t *dev, const void *data, size_t len, int flags)
{
	UNUSED(dev, data, len);

	if (flags == UXDEV_PKT_BUF_NOLINK) {
		return (size_t)ETH_HLEN;
	}

	return (size_t)0;
}

static int visp_set_packet_hdr(ux_netdev_t *dev, void *hdr,
			       const void *data, size_t len, int flags)
{
	struct ethhdr *ehdr = (struct ethhdr*)hdr;

	UNUSED(dev, data, len, flags);

	NOFAIL(memset_s(ehdr, sizeof(struct ethhdr), 0x00, sizeof(struct ethhdr)));
	ehdr->h_proto = htons(ETH_P_IP);
	return E_HM_OK;
}

ux_netdev_ops_priv_t visp_netdev_priv_ops = {
	.dump_stat = visp_netif_dump_stats,
	.get_mac = visp_netif_get_mac,
	.getifaddrs = visp_netif_getifaddrs,
	.get_mtu = visp_netif_get_mtu,
	.get_flags = visp_netif_get_flags,
	.get_ipaddr = visp_netif_get_ipaddr,
	.get_netmask = visp_netif_get_netmask,
	.get_dstaddr = visp_netif_get_dstaddr,
	.get_stats = visp_netif_get_stats,
	.get_ifconf = visp_netif_get_ifconf,

	.set_mac = visp_netif_set_mac,
	.set_ipaddr = visp_netif_set_ipaddr,
	.del_ipaddr = visp_netif_del_ipaddr,
	.set_netmask  = visp_netif_set_netmask,
	.set_dstaddr = visp_netif_set_dstaddr,
	.set_flags = visp_netif_set_flags,
	.set_mtu = visp_netif_set_mtu,
	.set_rxhwcsum = visp_netif_set_rxhwcsum,

	.recv = visp_netif_packet_rcv,
	.recv_zero_copy = visp_netif_packet_rcv_zero_copy,
	.copy = visp_netif_copy_data,
	.free = NULL,

	.get_packet_hdr_len = visp_get_packet_hdr_len,
	.set_packet_hdr = visp_set_packet_hdr,
};

ux_netdev_ops_priv_t* visp_get_netdev_priv_ops(void)
{
	return &visp_netdev_priv_ops;
}

static int visp_netdev_lo_init(ux_netdev_t *dev)
{
	struct netif *netif = ux_netdev_priv(dev);
	int ret;

	ret = visp_get_loopif_index(&netif->ifindex);
	if (ret != 0) {
		net_error(HMNET_INET, "visp get loopback index failed ret = %d\n", ret);
		return ret;
	}

	netif->flags |= (NETIF_FLAG_LOOPBACK | NETIF_FLAG_UP);

	visp_set_netif_extra(netif->ifindex, dev);
	visp_set_mtu(netif->ifindex, MTU_MAX);

	return 0;
}

static ux_netdev_ops_t visp_lo_ops = {
	.ndo_init = visp_netdev_lo_init,
	.ndo_xmit = NULL,
	.ndo_set_mtu = NULL,
	.ndo_set_mac = NULL,
	.ndo_ioctl = NULL,
	.ndo_dump_stats = NULL,
	.ndo_get_master = NULL,
	.ndo_dellink = NULL,
	.ndo_add_slave = NULL,
	.ndo_del_slave = NULL,
};

static int visp_loopback_init(void)
{
	ux_netdev_t *dev = NULL;
	int ret;

	dev = ux_netdev_alloc("lo", sizeof(struct netif));
	if (dev == NULL) {
		return E_HM_NOMEM;
	}

	dev->ops = &visp_lo_ops;
	dev->priv_ops = &visp_netdev_priv_ops;
	dev->type = ARPHRD_LOOPBACK;

	ux_rtnl_lock();
	ret = ux_netdev_register(dev);
	ux_rtnl_unlock();
	if (ret) {
		ux_netdev_free(dev);
		return ret;
	}

	return 0;
}

static unsigned int visp_netif_xmit(uintptr_t phy_link, void *mbuf)
{
	ux_netdev_t *dev = (void *)phy_link;
	int ret = ux_netdev_xmit(dev, mbuf, visp_mbuf_length(mbuf), 0);
	visp_destroy_mbuf(mbuf);
	return ret;
}

static unsigned int visp_netif_ioctl(uintptr_t phy_link, unsigned int cmd, char *data)
{
	net_warn(HMNET_INET, "!!! ioctl cmd:0x%x not implements\n", cmd);
	return 0;
}

static int visp_phy_init(void *dev, const unsigned int *ifindex,
			 const unsigned int *parg3, unsigned int arg4)
{
	ux_netdev_t *netdev = visp_get_arg_from_devlink(dev);
	struct netif *netif = ux_netdev_priv(netdev);

	UNUSED(parg3, arg4);

	if (dev == NULL || ifindex == NULL) {
		return E_HM_INVAL;
	}

	netif->ifindex = *ifindex;
	netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP |
			NETIF_FLAG_ETHERNET | NETIF_FLAG_IGMP | NETIF_FLAG_MLD6;

	visp_set_netif_extra(netif->ifindex, netdev);
	visp_set_netif_xmit_hook(netif->ifindex, visp_netif_xmit);
	visp_set_netif_ioctl_hook(netif->ifindex, visp_netif_ioctl);

	return 0;
}

static int visp_device_init(ux_netdev_t *dev)
{
	struct netif *netif = ux_netdev_priv(dev);
	unsigned char mac[6];

	dev->priv_ops = &visp_netdev_priv_ops;
	netif->netdev = visp_create_netdev(NETDEV_ETHERNET, visp_phy_init,
					   &netif->ifindex, dev);
	if (netif->netdev == NULL) {
		return E_HM_RESFAULT;
	}

	dev->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP |
		      NETIF_FLAG_ETHERNET | NETIF_FLAG_IGMP | NETIF_FLAG_MLD6;

	ux_eth_random_addr(mac, sizeof(mac));
	visp_set_mac(netif->ifindex, mac, sizeof(mac));

	return 0;
}

static void visp_device_uninit(const ux_netdev_t *dev)
{
	struct netif *netif = ux_netdev_priv(dev);
	int ret;

	ret = visp_delete_netdev(netif->ifindex, netif->netdev);
	if (ret != 0) {
		net_error(HMNET_INET, "visp delete netif failed ret = %d\n", ret);
	}
}

static ux_packet_demux_t visp_demux = {
	.head = DLIST_HEAD_INIT(visp_demux.head),
	.dev = NULL,
	.type = ARPHRD_ETHER,
	.init = visp_device_init,
	.uninit = visp_device_uninit,
	.func = NULL,
	.find_ip_reachable = visp_netif_find_ip_reachable,
	.set_default = visp_netif_set_default,
	.set_route_entry = visp_set_route_entry,
};

int visp_netdev_init(void)
{
	int ret;

	ret = ux_register_packet_demux(&visp_demux);
	if (ret) {
		net_error(HMNET_INET, "visp register demux failed\n");
		return ret;
	}

	ret = visp_loopback_init();
	if (ret) {
		net_error(HMNET_INET, "visp netdev lo init failed\n");
		return ret;
	}

	return 0;
}
