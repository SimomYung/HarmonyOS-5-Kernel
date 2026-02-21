/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: interface for network lwip netdev
 * Author: Huawei OS Kernel Lab
 * Create: Sun Apr 28 13:34:09 2019
 */

#include <errno.h>
#include <net/if_arp.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/if_ether.h>

#include <libhwsecurec/securec.h>
#include <libhmsrv_io/net.h>
#include <libhmlog/hmlog.h>
#include <libstrict/strict.h>

#include <lwip/opt.h>
#include <lwip/sys.h>
#include <lwip/timeouts.h>
#include <lwip/debug.h>
#include <lwip/stats.h>
#include <lwip/snmp.h>
#include <lwip/init.h>
#include <lwip/tcpip.h>
#include <lwip/netif.h>
#include <lwip/netifapi.h>
#include <lwip/api.h>
#include <lwip/etharp.h>
#include <lwip/ethip6.h>
#include <lwip/netifapi.h>
#include <lwip/tcp.h>
#include <lwip/udp.h>
#include <lwip/route.h>
#include <lwip/err.h>

#include "hm_ifaddrs.h"
#include "ux_utils.h"
#include "ux_netdev.h"
#include "ux_demux.h"
#include "ux_liblwip.h"
#include "ux_vlan.h"

#define IFNAME(netif)       (netif)->name
#define IF_GWADDR(netif)    (netif)->gw.u_addr.ip4.addr
#define IF_MASKADDR(netif)  (netif)->netmask.u_addr.ip4.addr
#define IF_IPADDR(netif)    (netif)->ip_addr.u_addr.ip4.addr

#define IP_STR_LEN_MAX		20
#define MTU_MAX			65535
#define SPEED_INIT_VALUE	100000000

#define to_num(c) ((int)(c) - (0x30))

static u32_t set_default_netmask(const struct sockaddr_in *sin)
{
	int num = 0;
	size_t ip_len;
	char result_mask[IP_STR_LEN_MAX] = {0};
	char ip[IP_STR_LEN_MAX] = {0};
	const char *res = NULL;
	struct in_addr addr = {0};

	res = inet_ntop(AF_INET, (void *)&sin->sin_addr, ip, INET_ADDRSTRLEN);
	if (res == NULL) {
		net_error(HMNET_INET, "inet_ntop failed!!!\n");
		return INADDR_BROADCAST;
	}

	ip_len = strlen(ip);
	for (size_t i = 0; i < ip_len; i++) {
		if (ip[i] != '.') {
			num = num * 10;
			num += (to_num(ip[i]));
		} else {
			break;
		}
	}

	if (num >= 0 && num <= 127) {
		NOFAIL(strcpy_s(result_mask, IP_STR_LEN_MAX, "255.0.0.0"));
	}

	if (num >= 128 && num <= 191) {
		NOFAIL(strcpy_s(result_mask, IP_STR_LEN_MAX, "255.255.0.0"));
	}

	if (num >= 192 && num <= 223) {
		NOFAIL(strcpy_s(result_mask, IP_STR_LEN_MAX, "255.255.255.0"));
	}

	if (inet_aton(result_mask, &addr) == 0) {
		/* Actually, never com here all the time */
		net_error(HMNET_INET, "inet_aton error, result_mask: %s\n", result_mask);
		return INADDR_LOOPBACK;
	}

	return addr.s_addr;
}

static int lwip_netif_get_mac(ux_netdev_t *dev,
		unsigned char *buf, size_t size)
{
	struct netif *netif = ux_netdev_priv(dev);

	if (size != sizeof(netif->hwaddr))
		return E_HM_INVAL;

	NOFAIL(memcpy_s((void *)buf, size, (void *)netif->hwaddr, sizeof(netif->hwaddr)));

	return 0;
}

static int lwip_netif_set_mac(ux_netdev_t *dev,
		const unsigned char *buf, size_t size)
{
	struct netif *netif = ux_netdev_priv(dev);
	int ret;

	if (size != sizeof(netif->hwaddr)) {
		return E_HM_INVAL;
	}

	ret = memcpy_s((char *)netif->hwaddr, sizeof(netif->hwaddr), (char *)buf, size);
	if (ret != EOK) {
		return E_HM_INVAL;
	}

	netif->hwaddr_len = ETH_HWADDR_LEN;

	return 0;
}

static int lwip_netif_set_flags(ux_netdev_t *dev, short int flags, const char *name)
{
	UNUSED(name);
	struct netif *netif = ux_netdev_priv(dev);
	unsigned short uflags = (unsigned short)flags;

	/* to set netdev rx_mode flags */
	if ((uflags & IFF_PROMISC) != 0) {
		netif->flags |= IFF_PROMISC;
	} else {
		netif->flags &= ~IFF_PROMISC;
	}

	if ((uflags & IFF_ALLMULTI) != 0) {
		netif->flags |= IFF_ALLMULTI;
	} else {
		netif->flags &= ~IFF_ALLMULTI;
	}

	if ((uflags & IFF_MULTICAST) != 0) {
		netif->flags |= IFF_MULTICAST;
		(void)igmp_start(netif);
	} else {
		netif->flags &= ~IFF_MULTICAST;
		(void)igmp_stop(netif);
	}

	if ((uflags & IFF_SLAVE) != 0) {
		netif->flags |= IFF_SLAVE;
	} else {
		netif->flags &= ~IFF_SLAVE;
	}

	if ((uflags & IFF_MASTER) != 0) {
		netif->flags |= IFF_MASTER;
	} else {
		netif->flags &= ~IFF_MASTER;
	}

	/* to set netdev updown in stack */
	if ((netif->flags & IFF_UP) != (uflags & IFF_UP)) {
		LOCK_TCPIP_CORE();
		if ((uflags & IFF_UP) != 0U) {
			netif_set_up(netif);
			/*
			 *  for some netdev (such as bcmdhd wifi), the mac changed after ifconfig up;
			 * here sync the mac to stack
			*/
			__ux_sync_macaddr_to_stack(dev);
		} else {
			netif_set_down(netif);
		}
		UNLOCK_TCPIP_CORE();
	}

	return 0;
}

static int lwip_netif_set_ipaddr(ux_netdev_t *dev, struct sockaddr *addr,
				 const char *name, unsigned int ipmask, bool netlink)
{
	UNUSED(ipmask);
	UNUSED(name);
	struct netif *netif = ux_netdev_priv(dev);
	struct sockaddr_in *sin = (struct sockaddr_in *)addr;

	if (netlink) {
		return E_HM_OPNOTSUPP;
	}

	ip4_addr_t ip4_old_ipaddr = {0};
	ip4_addr_t ip4_ipaddr = {0};
	ip4_addr_t ip4_netmask = {0};
	ip4_addr_t ip4_gw = {0};
	int ret = 0;

	ip4_ipaddr.addr = sin->sin_addr.s_addr;
	ip4_gw.addr = IF_GWADDR(netif);
	ip4_old_ipaddr.addr = IF_IPADDR(netif);
	if (sin->sin_addr.s_addr == htonl(INADDR_ANY)) {
		ip4_netmask.addr = 0;
		ip4_gw.addr = 0;
	} else if (ip4_old_ipaddr.addr == ip4_ipaddr.addr) {
		ip4_netmask.addr = IF_MASKADDR(netif);
	} else {
		ip4_netmask.addr = set_default_netmask(sin);
		if (ip4_netmask.addr == IPADDR_LOOPBACK) {
			net_error(HMNET_INET, "set_default_netmask failed!!!\n");
			ret = -1;
			goto error;
		}
	}

	ret = netifapi_netif_set_addr(netif, &ip4_ipaddr,
				      &ip4_netmask, &ip4_gw);
	if (ret != 0) {
		net_error(HMNET_INET, "netifapi_netif_set_addr failed, ret: %d\n", ret);
	}

error:
	return ret;
}

static int lwip_netif_set_netmask(ux_netdev_t *dev, struct sockaddr *addr, const char *name)
{
	UNUSED(name);
	struct netif *netif = ux_netdev_priv(dev);
	struct sockaddr_in *sin = (struct sockaddr_in *)addr;
	int ret = E_HM_INVAL;

	ip4_addr_t ip4_ipaddr = {0};
	ip4_addr_t ip4_netmask = {0};
	ip4_addr_t ip4_gw = {0};
	unsigned int netmask_addr;

	ip4_netmask.addr = sin->sin_addr.s_addr;
	ip4_gw.addr = IF_GWADDR(netif);
	ip4_ipaddr.addr = IF_IPADDR(netif);
	netmask_addr = htonl(ip4_netmask.addr);

	/* step: check netmask value is continouns 1, like 1111... */
	netmask_addr = ~netmask_addr + 1;
	if ((netmask_addr & (netmask_addr - 1)) != 0) {
		net_error(HMNET_INET, "ifconfig: bad network address\n");
		return ret;
	}

	return (int)netifapi_netif_set_addr(netif, &ip4_ipaddr, &ip4_netmask, &ip4_gw);
}

static int lwip_netif_set_dstaddr(ux_netdev_t *dev, struct sockaddr *addr)
{
	struct netif *netif = ux_netdev_priv(dev);
	struct sockaddr_in *sin = (struct sockaddr_in *)addr;

	ip4_addr_t ip4_ipaddr = {0};
	ip4_addr_t ip4_netmask = {0};
	ip4_addr_t ip4_gw = {0};

	ip4_gw.addr = sin->sin_addr.s_addr;
	ip4_ipaddr.addr = IF_IPADDR(netif);
	ip4_netmask.addr = IF_MASKADDR(netif);

	return (int)netifapi_netif_set_addr(netif, &ip4_ipaddr, &ip4_netmask, &ip4_gw);
}

static void lwip_netif_get_flags(ux_netdev_t *dev, short int *flags, const char *name)
{
	UNUSED(name);
	struct netif *netif = ux_netdev_priv(dev);

	*flags = (short int)(netif->flags & 0xffff);
}

static int lwip_netif_get_mtu(ux_netdev_t *dev, int *mtu)
{
	struct netif *netif = ux_netdev_priv(dev);
	*mtu = netif->mtu;

	return 0;
}

static void lwip_netif_get_ipaddr(ux_netdev_t *dev, struct sockaddr_in *sockaddr, const char *name)
{
	UNUSED(name);
	struct netif *netif = ux_netdev_priv(dev);

	sockaddr->sin_addr.s_addr = IF_IPADDR(netif);
	sockaddr->sin_family = AF_INET;
}

static int lwip_netif_find_ip_reachable(char **ifname, const u32_t ip_addr)
{
	struct netif *netif = NULL;
	ip4_addr_t ip4_gw = { .addr = ip_addr };

	LOCK_TCPIP_CORE();
	NETIF_FOREACH(netif) {
		/* is the netif up, does it have a link and a valid address? */
		if (netif_is_up(netif) && netif_is_link_up(netif) &&
		    !ip4_addr_isany_val(*netif_ip4_addr(netif)) &&
		    /* network mask matches? */
		    ip4_addr_netcmp(&ip4_gw, netif_ip4_addr(netif), netif_ip4_netmask(netif))) {
			/* If we specify the device name, be reachable only if we matched the right netif */
			if (*ifname != NULL && strcmp(netif->name, *ifname) == 0) {
				UNLOCK_TCPIP_CORE();
				return 1;
			}
			/* Or, any device is OK. */
			if (*ifname == NULL) {
				*ifname = netif->name;
				UNLOCK_TCPIP_CORE();
				/* return netif on which to forward IP packet */
				return 1;
			}
		}
	}
	UNLOCK_TCPIP_CORE();
	return 0;
}

static void lwip_netif_get_netmask(ux_netdev_t *dev, struct sockaddr_in *sockaddr, const char *name)
{
	UNUSED(name);
	struct netif *netif = ux_netdev_priv(dev);

	sockaddr->sin_addr.s_addr = IF_MASKADDR(netif);
	sockaddr->sin_family = AF_INET;
}

static void lwip_netif_get_dstaddr(ux_netdev_t *dev,
				  struct sockaddr_in *sockaddr)
{
	struct netif *netif = ux_netdev_priv(dev);

	sockaddr->sin_addr.s_addr = IF_GWADDR(netif);
	sockaddr->sin_family = AF_INET;
}

static int lwip_netif_set_mtu(ux_netdev_t *dev, int mtu)
{
	struct netif *netif = ux_netdev_priv(dev);

	netif->mtu = (unsigned short)(unsigned int)mtu;

	return 0;
}

static int lwip_netif_set_rxhwcsum(ux_netdev_t *dev, int enable)
{
	struct netif *netif = ux_netdev_priv(dev);
	uint32_t chksumflags = (uint32_t)netif->chksum_flags;
	uint32_t rxhwsum_mask = (NETIF_CHECKSUM_CHECK_IP |
				NETIF_CHECKSUM_CHECK_UDP |
				NETIF_CHECKSUM_CHECK_TCP);

	if (enable != 0) {
		chksumflags &= (~rxhwsum_mask);
	} else {
		chksumflags |= rxhwsum_mask;
	}

	NETIF_SET_CHECKSUM_CTRL(netif, (uint16_t)chksumflags);

	return 0;
}

static int lwip_netif_set_default(ux_netdev_t *dev, int action,
				  struct sockaddr *gw)
{
	struct netif *netif = ux_netdev_priv(dev);
	struct sockaddr_in *sin_gw = (struct sockaddr_in *)gw;
	ip4_addr_t ip4_gw;

	if (action == 1) {
		LOCK_TCPIP_CORE();
		if (netif_default == NULL || !netif_is_up(netif_default) ||
		    sin_gw->sin_addr.s_addr != ip_2_ip4(&netif_default->gw)->addr) {
			net_debug(HMNET_INET, "Gateway is not exist\n");
			UNLOCK_TCPIP_CORE();
			return E_HM_SRCH;
		}
		netif_set_default(NULL);
		UNLOCK_TCPIP_CORE();
	} else {
		LOCK_TCPIP_CORE();
		netif_set_default(netif);
		if (sin_gw->sin_addr.s_addr != 0U) {
			ip4_gw.addr = sin_gw->sin_addr.s_addr;
			netif_set_gw(netif, &ip4_gw);
		}
		UNLOCK_TCPIP_CORE();
	}

	return 0;
}

static int lwip_set_route_entry(int action,  struct rtentry *rt)
{
	int ret;
	struct sockaddr_in *dst_ip = (struct sockaddr_in *)(&rt->rt_dst);
	struct sockaddr_in *netmask = (struct sockaddr_in *)(&rt->rt_genmask);
	struct sockaddr_in *gw = (struct sockaddr_in *)(&rt->rt_gateway);

	if (action == 1) {
		LOCK_TCPIP_CORE();
		ret = del_route(rt->rt_dev, (unsigned int)dst_ip->sin_addr.s_addr,
				(unsigned int)netmask->sin_addr.s_addr,
				(unsigned int)gw->sin_addr.s_addr);
		UNLOCK_TCPIP_CORE();
		if (ret < 0) {
			net_debug(HMNET_INET, "route: del route failed!, ret=%d\n", ret);
			return posix2hmerrno(-ret);
		}
	} else {
		if (!rt->rt_dev || *(rt->rt_dev) == '\0') {
			net_debug(HMNET_INET, "route: can't get device information!\n");
			return E_HM_NODEV;
		}
		LOCK_TCPIP_CORE();
		ret = add_route(rt->rt_dev, (unsigned int)dst_ip->sin_addr.s_addr,
				(unsigned int)netmask->sin_addr.s_addr,
				(unsigned int)gw->sin_addr.s_addr);
		UNLOCK_TCPIP_CORE();
		if (ret < 0) {
			net_debug(HMNET_INET, "route: add route failed, ret=%d\n", ret);
			return posix2hmerrno(-ret);
		}
	}

	return 0;
}

static int lwip_netif_packet_rcv(ux_netdev_t *dev,
				 const void *data, size_t len)
{
	struct netif *netif = ux_netdev_priv(dev);
	struct pbuf *skb = NULL;
	int ret = 0;

	if (len > (size_t)netif->mtu + ETH_HLEN) {
		net_warn(HMNET_INET, "Packet too big, len: %zu\n", len);
		return -1;
	}

	LINK_STATS_INC(link.recv);

	skb = pbuf_alloc(PBUF_RAW, (u16_t)len, PBUF_POOL);
	if (skb == NULL) {
		goto out;
	}

	skb->len = (u16_t)len;
	skb->tot_len = (u16_t)len;

	ret = memcpy_s(skb->payload, (size_t)skb->len, data, len);
	if (ret != 0) {
		(void)pbuf_free(skb);
		goto out;
	}

	MIB2_STATS_NETIF_ADD(netif, ifinoctets, skb->tot_len);
	if ((((u8_t *)skb->payload)[0] & 1U) != 0U) {
		/* broadcast or multicast packet */
		MIB2_STATS_NETIF_INC(netif, ifinnucastpkts);
	} else {
		/* unicast packet */
		MIB2_STATS_NETIF_INC(netif, ifinucastpkts);
	}

	LOCK_TCPIP_CORE();
	netif->input(skb, netif);
	UNLOCK_TCPIP_CORE();

	return 0;
out:
	LINK_STATS_INC(link.memerr);
	LINK_STATS_INC(link.drop);
	MIB2_STATS_NETIF_INC(netif, ifindiscards);
	return -1;
}

static int lwip_netif_packet_rcv_zero_copy(ux_netdev_t *dev,
		void *data, size_t len, size_t offset)
{
	struct netif *netif = ux_netdev_priv(dev);
	ux_netdev_t *indev = NULL;
	struct pbuf *skb = NULL;

	if (len > (size_t)netif->mtu + ETH_HLEN) {
		net_warn(HMNET_INET, "Packet too big, len: %zu\n", len);
		return -1;
	}

	LINK_STATS_INC(link.recv);

	if ((dev->priv_flags & IFF_802_1Q_VLAN) != 0) {
		ux_netdev_vlan_t *vlan = NULL;
		vlan = ux_netdev_priv_vlan(dev);
		indev = vlan->realdev;
	} else {
		indev = dev;
	}

	skb = my_pbuf_alloc(data, len, (unsigned int)offset, 0, indev->shmmgr);
	if (skb == NULL) {
		net_error(HMNET_INET, "pbuf custom alloc fail.\n");
		goto out;
	}

	LOCK_TCPIP_CORE();
	MIB2_STATS_NETIF_ADD(netif, ifinoctets, skb->tot_len);
	if ((((u8_t *)skb->payload)[0] & 1U) != 0U) {
		/* broadcast or multicast packet */
		MIB2_STATS_NETIF_INC(netif, ifinnucastpkts);
	} else {
		/* unicast packeti */
		MIB2_STATS_NETIF_INC(netif, ifinucastpkts);
	}

	netif->input(skb, netif);
	UNLOCK_TCPIP_CORE();
	return 0;
out:
	LINK_STATS_INC(link.memerr);
	LINK_STATS_INC(link.drop);
	MIB2_STATS_NETIF_INC(netif, ifindiscards);
	return -1;
}

static int lwip_netif_copy_data(ux_netdev_t *dev, const void *from,
				void *to, size_t len, size_t offset)
{
	struct netif *netif = NULL;

	if (dev == NULL) {
		return -1;
	}

	netif = ux_netdev_priv(dev);
	if (len > (size_t)netif->mtu + ETH_HLEN) {
		net_warn(HMNET_INET, "Packet too big, len: %zu\n", len);
		return -1;
	}

	return pbuf_copy_partial(from, to, (unsigned short)len, (unsigned short)offset);
}

static int lwip_netif_dump_stats(ux_netdev_t *dev,
				  void *buf, size_t len)
{
	struct netif *netif = ux_netdev_priv(dev);
	int ret;

	ret = snprintf_s(buf, len, len - 1,
		"%s : rx_pkt:%u, tx_pkt:%u, total_bytes_received:%u, total_bytes_transmitted:%u, "
		"rx_error:%u, tx_error:%u, rx_drop:%u, tx_drop:%u, mcast:%u.\n",
		netif->name,
		netif->mib2_counters.ifinucastpkts + netif->mib2_counters.ifinnucastpkts,
		netif->mib2_counters.ifoutucastpkts + netif->mib2_counters.ifoutnucastpkts,
		netif->mib2_counters.ifinoctets, netif->mib2_counters.ifoutoctets,
		netif->mib2_counters.ifinerrors, netif->mib2_counters.ifouterrors,
		netif->mib2_counters.ifindiscards, netif->mib2_counters.ifoutdiscards,
		netif->mib2_counters.ifinnucastpkts);
	if (ret < 0) {
		return -1;
	}
	return 0;
}

static int lwip_netif_get_stats(ux_netdev_t *dev, int file_bytes, char *result)
{
	int ret;
	struct netif *netif = ux_netdev_priv(dev);
	struct stats_mib2_netif_ctrs *stats = &netif->mib2_counters;

	ret = snprintf_s(result, (unsigned int)file_bytes, (unsigned int)file_bytes - 1,
			 "%6s: %7llu %7llu %4llu %4llu %4llu %5llu %10llu %9llu "
			 "%8llu %7llu %4llu %4llu %4llu %5llu %7llu %10llu\n",
			 netif->name, stats->ifinoctets, stats->ifinucastpkts + stats->ifinnucastpkts,
			 stats->ifinerrors,
			 stats->ifindiscards + stats->ifinerrors,
			 0ULL,
			 0ULL,
			 0ULL, stats->ifinnucastpkts,
			 stats->ifoutoctets, stats->ifoutucastpkts + stats->ifoutnucastpkts,
			 stats->ifouterrors, stats->ifoutdiscards,
			 0ULL, 0ULL,
			 0ULL,
			 0ULL);
	if (ret < 0) {
		return -1;
	}
	return 0;
}

static int lwip_netif_get_ifconf(ux_netdev_t *dev, void *ifr_dst, int ifr_len, int *size)
{
	UNUSED(ifr_len);

	*size = 1;
	if (ifr_dst == NULL) {
		return 0;
	}

	struct ifreq ifr = {0};
	struct netif *netif = ux_netdev_priv(dev);
	int ret = strcpy_s(ifr.ifr_ifrn.ifrn_name, IFNAMSIZ, netif->name);
	if (ret != 0) {
		return -1;
	}

	lwip_netif_get_ipaddr(dev, (struct sockaddr_in *)(&(ifr.ifr_addr)), netif->name);
	return ux_netdev_ifr_copy_to_user(ifr_dst, &ifr, &ifr_dst);
}

static int lwip_netif_getifaddrs(ux_netdev_t *dev, char *buf, size_t size, int *cnt)
{
	struct netif *netif = ux_netdev_priv(dev);
	struct ifaddrs_storage_internal *ifp = NULL;
	struct sockaddr_in my_addr = {0};
	size_t ifaddrs_len = sizeof(struct ifaddrs_storage_internal);

	if (size < ifaddrs_len) {
		*cnt = 0;
		return 0;
	}

	ifp = (struct ifaddrs_storage_internal *)(void *)buf;
	mem_zero_s(*ifp);
	NOFAIL(memcpy_s(ifp->name, sizeof(ifp->name), IFNAME(netif), IFNAMSIZ));
	ifp->ifa_flags = netif->flags;
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = IF_IPADDR(netif);
	NOFAIL(memcpy_s(&ifp->ifa_addr, sizeof(ifp->ifa_addr), &my_addr,
			sizeof(my_addr)));
	my_addr.sin_addr.s_addr = IF_MASKADDR(netif);
	NOFAIL(memcpy_s(&ifp->ifa_netmask, sizeof(ifp->ifa_netmask), &my_addr,
			sizeof(my_addr)));
	my_addr.sin_addr.s_addr = IF_GWADDR(netif);
	NOFAIL(memcpy_s(&ifp->ifa_ifu.ifu_dstaddr, sizeof(ifp->ifa_ifu.ifu_dstaddr),
			&my_addr, sizeof(my_addr)));
	ifp->stats.packets_received = (__u32)(netif->mib2_counters.ifinucastpkts +
				      netif->mib2_counters.ifinucastpkts);
	ifp->stats.packets_transmitted = (__u32)(netif->mib2_counters.ifoutucastpkts +
					netif->mib2_counters.ifoutnucastpkts);
	ifp->stats.total_bytes_received = (__u32)netif->mib2_counters.ifinoctets;
	ifp->stats.total_bytes_transmitted = (__u32)netif->mib2_counters.ifoutoctets;
	ifp->stats.errors_received = (__u32)netif->mib2_counters.ifinerrors;
	ifp->stats.errors_transmit = (__u32)netif->mib2_counters.ifouterrors;
	ifp->stats.dropped_recv = (__u32)netif->mib2_counters.ifindiscards;
	ifp->stats.dropped_send = (__u32)netif->mib2_counters.ifoutdiscards;
	ifp->stats.multicast = (__u32)netif->mib2_counters.ifinnucastpkts;

	*cnt = 1;
	return (int)ifaddrs_len;
}

static int lwip_netif_add_header(ux_netdev_t *dev, void *pkt, size_t len, const void *header, size_t header_len)
{
	UNUSED(dev, len);
	int ret;

	if (header == NULL) {
		return E_HM_INVAL;
	}

	ret = (int)pbuf_add_header(pkt, header_len);
	if (ret != 0) {
		return E_HM_INVAL;
	}

	ret = pbuf_take(pkt, header, (uint16_t)header_len);
	if (ret < 0) {
		ret =  err_to_errno((int8_t)ret);
		return posix2hmerrno(ret);
	}

	return 0;
}

static int lwip_netif_remove_header(ux_netdev_t *dev, void *pkt, size_t len, void *saved_header, size_t header_len)
{
	UNUSED(dev, len);
	int ret;

	if (saved_header != NULL) {
		ret = (int)pbuf_copy_partial(pkt, saved_header, (uint16_t)header_len, 0);
		if (ret < 0) {
			return E_HM_INVAL;
		}
	}

	ret = (int)pbuf_remove_header(pkt, header_len);
	if (ret != 0) {
		return E_HM_INVAL;
	}

	return 0;
}

static size_t lwip_get_packet_hdr_len(ux_netdev_t *dev, const void *data, size_t len, int flags)
{
	UNUSED(dev, data, len);

	if (flags == UXDEV_PKT_BUF_NOLINK) {
		return (size_t)ETH_HLEN;
	}

	return (size_t)0;
}

static int lwip_set_packet_hdr(ux_netdev_t *dev, void *hdr,
			       const void *data, size_t len, int flags)
{
	struct ethhdr *ehdr = (struct ethhdr*)hdr;

	UNUSED(dev, data, len, flags);

	NOFAIL(memset_s(ehdr, sizeof(struct ethhdr), 0x00, sizeof(struct ethhdr)));
	ehdr->h_proto = htons(ETH_P_IP);
	return 0;
}

static ux_netdev_ops_priv_t lwip_netdev_priv_ops = {
	.dump_stat = lwip_netif_dump_stats,
	.get_mac = lwip_netif_get_mac,
	.getifaddrs = lwip_netif_getifaddrs,
	.get_mtu = lwip_netif_get_mtu,
	.get_flags = lwip_netif_get_flags,
	.get_ipaddr = lwip_netif_get_ipaddr,
	.get_netmask = lwip_netif_get_netmask,
	.get_dstaddr = lwip_netif_get_dstaddr,
	.get_stats = lwip_netif_get_stats,
	.get_ifconf = lwip_netif_get_ifconf,

	.set_mac = lwip_netif_set_mac,
	.set_ipaddr = lwip_netif_set_ipaddr,
	.del_ipaddr = NULL,
	.set_netmask  = lwip_netif_set_netmask,
	.set_dstaddr = lwip_netif_set_dstaddr,
	.set_flags = lwip_netif_set_flags,
	.set_mtu = lwip_netif_set_mtu,
	.set_rxhwcsum = lwip_netif_set_rxhwcsum,

	.recv = lwip_netif_packet_rcv,
	.recv_zero_copy = lwip_netif_packet_rcv_zero_copy,
	.copy = lwip_netif_copy_data,
	.add_header = lwip_netif_add_header,
	.remove_header = lwip_netif_remove_header,
	.free = NULL,

	.get_packet_hdr_len = lwip_get_packet_hdr_len,
	.set_packet_hdr = lwip_set_packet_hdr,
};

static int lwip_netdev_lo_init(ux_netdev_t *dev)
{
	struct netif *netif = ux_netdev_priv(dev);

	loop_netif = netif;

	LOCK_TCPIP_CORE();
	netif_init();
	loop_netif->mtu = MTU_MAX;
	UNLOCK_TCPIP_CORE();

	return 0;
}

static ux_netdev_ops_t lwip_lo_ops = {
	.ndo_init = lwip_netdev_lo_init,
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

static int lwip_loopback_init(void)
{
	ux_netdev_t *dev = NULL;
	int ret;

	dev = ux_netdev_alloc("lo", sizeof(struct netif));
	if (dev == NULL)
		return E_HM_NOMEM;

	dev->ops = &lwip_lo_ops;
	dev->priv_ops = &lwip_netdev_priv_ops;
	dev->type = ARPHRD_LOOPBACK;

	ux_rtnl_lock();
	ret = ux_netdev_register(dev);
	ux_rtnl_unlock();
	if (ret != E_HM_OK) {
		ux_netdev_free(dev);
		return ret;
	}

	return 0;
}

static err_t lwip_netif_xmit(struct netif *netif, const struct pbuf *p)
{
	ux_netdev_t *dev = ux_netdev_from_priv(netif);

	LINK_STATS_INC(link.xmit);

	MIB2_STATS_NETIF_ADD(netif, ifoutoctets, p->tot_len);
	if ((((u8_t *)p->payload)[0] & 1U) != 0U) {
		/* broadcast or multicast packet */
		MIB2_STATS_NETIF_INC(netif, ifoutnucastpkts);
	} else {
		/* unicast packeti */
		MIB2_STATS_NETIF_INC(netif, ifoutucastpkts);
	}

	if (netif_is_up(netif) == 0) {
		MIB2_STATS_NETIF_INC(netif, ifoutdiscards);
		return ERR_OK;
	}

	return (err_t)ux_netdev_xmit(dev, p, p->tot_len, 0);
}

static err_t lwip_netif_init(struct netif *netif)
{
	netif->linkoutput = lwip_netif_xmit;
	netif->output = etharp_output;
	netif->output_ip6 = ethip6_output;
	netif->mtu = ETHERNET_MTU;
	netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP |
		       NETIF_FLAG_ETHERNET | NETIF_FLAG_IGMP | NETIF_FLAG_MLD6;
	MIB2_INIT_NETIF(netif, snmp_ifType_ethernet_csmacd, SPEED_INIT_VALUE);
	netif_set_igmp_mac_filter(netif, netif_set_dev_mc);

	return ERR_OK;
}

static void netif_status_callback(struct netif *netif)
{
	net_info(HMNET_INET, "%s status changed %s\n", netif->name,
		ip4addr_ntoa(netif_ip4_addr(netif)));
}

static int lwip_device_init(ux_netdev_t *dev)
{
	struct netif *netif = ux_netdev_priv(dev);
	ip4_addr_t ipaddr = {0};
	ip4_addr_t ipmask = {0};
	ip4_addr_t ipgw = {0};

	IP4_ADDR(&ipaddr, 0, 0, 0, 0);
	IP4_ADDR(&ipmask, 255, 255, 255, 0);
	IP4_ADDR(&ipgw, 0, 0, 0, 0);

	dev->priv_ops = &lwip_netdev_priv_ops;
	if (strcpy_s(netif->name, NETIF_NAMESIZE, dev->name) != 0) {
		net_error(HMNET_INET, "strcpy_s error, dev->name: %s\n", dev->name);
		return -1;
	}

	LOCK_TCPIP_CORE();
	ux_eth_random_addr(netif->hwaddr, sizeof(netif->hwaddr));
	netif->hwaddr_len = ETH_HWADDR_LEN;

	netif->ip6_autoconfig_enabled = 0;
	/* init dev flags */
	dev->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP |
		      NETIF_FLAG_ETHERNET | NETIF_FLAG_IGMP | NETIF_FLAG_MLD6;

	if (netif_add(netif, &ipaddr, &ipmask, &ipgw, NULL,
		      lwip_netif_init, netif_input) == NULL) {
		net_error(HMNET_INET, "netif add failed, name: %s\n", netif->name);
		UNLOCK_TCPIP_CORE();
		return -1;
	}

	netif_create_ip6_linklocal_address(netif, 1);
	netif_set_status_callback(netif, netif_status_callback);
	netif_set_down(netif);
	netif_set_link_up(netif);
	UNLOCK_TCPIP_CORE();

	return 0;
}

static void lwip_device_uninit(const ux_netdev_t *dev)
{
	struct netif *netif = ux_netdev_priv(dev);

	LOCK_TCPIP_CORE();
	/* devhost had delete the netif, so need not let devhost del mac filter */
	netif_set_igmp_mac_filter(netif, NULL);
	netif_remove(netif);
	UNLOCK_TCPIP_CORE();
}

static ux_packet_demux_t lwip_demux = {
	.head = DLIST_HEAD_INIT(lwip_demux.head),
	.dev = NULL,
	.type = ARPHRD_ETHER,
	.init = lwip_device_init,
	.uninit = lwip_device_uninit,
	.func = NULL,
	.find_ip_reachable = lwip_netif_find_ip_reachable,
	.set_default = lwip_netif_set_default,
	.set_route_entry = lwip_set_route_entry,
};

/*
 * Function: get netdevice multicast mac from group ip
 * groupaddr: unsigned int virable to restore the group ip
 * mac: virable to restore mac
 *
 * How calculate the multicast mac address here?
 * For ipv4 multicast mac, first three byte is `01 00 5e`, after that,
 * we left 24 bit for mac address, the 25 bit is `0`. For last 23 bit of the
 * mac address, use the last 23 bit of the group ip.
 */
static void ether_get_mc_mac(unsigned int group_addr, char *mac, unsigned int size)
{
	UNUSED(size);
	mac[0] = (char)0x01;
	mac[1] = (char)0x00;
	mac[2] = (char)0x5e;
	mac[3] = (char)((group_addr >> 16) & 0x7f);
	mac[4] = (char)((group_addr >> 8) & 0xff);
	mac[5] = (char)(group_addr & 0xff);
}

err_t netif_set_dev_mc(struct netif *netif, const ip4_addr_t *group, enum netif_mac_filter_action op)
{
	ux_netdev_t *dev = ux_netdev_from_priv(netif);
	unsigned int addr = htonl(group->addr);
	struct ifreq ifr = {0};
	int opt = SIOCDELMULTI;

	if (op == NETIF_ADD_MAC_FILTER) {
		opt = SIOCADDMULTI;
	}

	if (dev->devhost == NULL || dev->ctx.dh_cnode_idx == 0) {
		/* internal netdev, need not do hmsrv_io_net_ioctl */
		return 0;
	}

	NOFAIL(strncpy_s(ifr.ifr_name, IFNAMSIZ, dev->name,
			 sizeof(dev->name) - 1));
	ether_get_mc_mac(addr, ifr.ifr_hwaddr.sa_data, sizeof(ifr.ifr_hwaddr.sa_data));

	return (err_t)hmsrv_io_net_ioctl(&dev->ctx, opt, (unsigned long)&ifr, false);
}

int lwip_netdev_init(void)
{
	int ret;

	ret = ux_register_packet_demux(&lwip_demux);
	if (ret != 0) {
		net_error(HMNET_INET, "lwip register demux failed\n");
		return ret;
	}

	ret = lwip_loopback_init();
	if (ret != 0) {
		net_error(HMNET_INET, "lwip netdev lo init failed\n");
		return ret;
	}

	return 0;
}
