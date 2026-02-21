/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2019-2021. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Mon Dec 30 09:49:09 2019
 */

#include <net/if_arp.h>
#include <net/if.h>

#include "hm_can.h"
#include "hm_ifaddrs.h"

#include "ux_netlog.h"
#include "ux_demux.h"
#include "ux_pktbuf.h"
#include "ux_phy_mem.h"
#include "ux_can_netdev.h"
#include "ux_can_raw.h"
#include "ux_can_filter.h"

int ux_can_netdev_xmit(ux_netdev_t *dev, const ux_pktbuf_t *pkt, int flags)
{
	return ux_netdev_xmit(dev, (void *)pkt, (int)pkt->len, flags);
}

static int ux_can_netdev_data_recv(ux_netdev_t *dev, const void *data, size_t len)
{
	ux_pktbuf_t *pkt = NULL;

	pkt = ux_pktbuf_alloc(len);
	if (pkt == NULL) {
		return E_HM_NOMEM;
	}

	(void)ux_pktbuf_push(pkt, (unsigned int)len);
	NOFAIL(memcpy_s(pkt->data, pkt->len, data, len));

	ux_can_filters_dispatch(dev, pkt);

	ux_pktbuf_free(pkt);

	return 0;
}

static int ux_can_netdev_recv_zero_copy(ux_netdev_t *dev,
		void *data, size_t len, size_t offset)
{
	ux_can_netdev_t *candev = ux_netdev_priv(dev);
	ux_can_raw_sock_t *rsk = NULL;
	ux_buf_t *ux_buf = NULL;
	ux_copy_data_args_t args;

	raw_mutex_lock(&candev->raw_sock_list_lock);
	dlist_for_each_entry(rsk, &candev->raw_sock_list, ux_can_raw_sock_t,
				raw_sock_list_node) {
		/* alloc ux_buf and queued to rsk recv queue and signal wait events */
		ux_buf = (ux_buf_t *)calloc(1, sizeof(ux_buf_t) + len);
		if (ux_buf == NULL) {
			raw_mutex_unlock(&candev->raw_sock_list_lock);
			/* free share mem from devhost */
			phy_mem_free(dev->shmmgr, data, offset);
			return E_HM_NOMEM;
		}

		args.dst = (void *)ux_buf->data;
		args.dstlen = len;
		args.src = (const void *)data;
		args.count = len;
		args.offset = 0;
		args.flags = 1;
		if (ux_netdev_copy_data(dev, &args) != (int)len) {
			free(ux_buf);
			ux_buf = NULL;
			continue;
		}

		ux_buf->data_len = len;
		ux_buf_queue_lock(&rsk->recv_queue);
		ux_buf_queue_insert_tail(&rsk->recv_queue, ux_buf);
		ux_buf_queue_unlock(&rsk->recv_queue);

		ux_sock_poll_common(&rsk->sk, EPOLLIN);
		if (raw_sem_post(&rsk->recvsem) != 0) {
			hm_info("raw_sem_post failed\n");
		}
	}
	raw_mutex_unlock(&candev->raw_sock_list_lock);

	/* free share mem from devhost */
	phy_mem_free(dev->shmmgr, data, offset);

	return (int)len;
}

static int ux_can_netdev_data_copy(ux_netdev_t *dev, const void *from, void *to,
				   size_t len, size_t offset)
{
	UNUSED(dev);
	const ux_pktbuf_t *pkt = (ux_pktbuf_t *)from;

	if (len <= offset) {
		return 0;
	}

	NOFAIL(memcpy_s(to, len, (char *)pkt->data + offset, pkt->len - offset));

	return (int)(pkt->len - offset);
}

static void ux_can_netdev_data_free(ux_netdev_t *dev, const void *data)
{
	UNUSED(dev);
	ux_pktbuf_free((ux_pktbuf_t *)data);
}

static int ux_can_netif_get_mac(ux_netdev_t *dev, unsigned char *mac, size_t len)
{
	UNUSED(dev);
	char zero[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	safe_copy((char *)mac, zero, len);

	return 0;
}

static int ux_can_netif_getifaddrs(ux_netdev_t *dev, char *buf, size_t size, int *cnt)
{
	struct ifaddrs_storage_internal *ifp = NULL;
	size_t ifaddrs_len = sizeof(struct ifaddrs_storage_internal);

	if (size < ifaddrs_len) {
		return 0;
	}

	*cnt = 1;
	ifp = (struct ifaddrs_storage_internal *)(void *)buf;
	mem_zero_s(*ifp);
	NOFAIL(memcpy_s(ifp->name, sizeof(ifp->name), dev->name, IFNAMSIZ));

	return (int)ifaddrs_len;
}

static int ux_can_netif_get_mtu(ux_netdev_t *dev, int *mtu)
{
	*mtu = dev->_mtu;

	return 0;
}

static void ux_can_netif_get_flags(ux_netdev_t *dev, short int *flags, const char *name)
{
	UNUSED(name);
	*flags = (short int)dev->flags;
}

static void ux_can_netif_get_ipaddr(ux_netdev_t *dev, struct sockaddr_in *sockaddr, const char *name)
{
	UNUSED(dev, name);
	sockaddr->sin_family = AF_CAN;
}

static void ux_can_netif_get_netmask(ux_netdev_t *dev, struct sockaddr_in *sockaddr, const char *name)
{
	UNUSED(dev, name);
	sockaddr->sin_family = AF_CAN;
}

static void ux_can_netif_get_dstaddr(ux_netdev_t *dev, struct sockaddr_in *sockaddr)
{
	UNUSED(dev);
	sockaddr->sin_family = AF_CAN;
}

static int ux_can_netif_get_stats(ux_netdev_t *dev, int file_bytes, char *result)
{
	int err;

	err = snprintf_s(result, (size_t)(unsigned int)file_bytes, (size_t)(unsigned int)file_bytes - 1,
			 "%6s: %7u %7u %4u %4u %4u %5u %10u %9u "
			 "%8u %7u %4u %4u %4u %5u %7u %10u\n",
			  dev->name, 0, 0, 0, 0, 0, 0, 0, 0,
			  0, 0, 0, 0, 0, 0, 0, 0);
	if (err <= 0) {
		return E_HM_INVAL;
	}

	return 0;
}

static int ux_can_netif_get_ifconf(ux_netdev_t *dev, void *ifr_dst, int ifr_len, int *size)
{
	UNUSED(ifr_len);

	*size = 1;
	if (ifr_dst == NULL) {
		return 0;
	}

	struct ifreq ifr = {0};
	int ret = strncpy_s(ifr.ifr_ifrn.ifrn_name, IF_NAMESIZE,
			dev->name, IF_NAMESIZE - 1);
	if (ret != 0) {
		return -1;
	}

	struct sockaddr_can *caddr = (struct sockaddr_can *)&ifr.ifr_ifru.ifru_addr;
	caddr->can_family = AF_CAN;
	return ux_netdev_ifr_copy_to_user(ifr_dst, &ifr, &ifr_dst);
}

/* notify protocol */
static int ux_can_netif_set_mac(ux_netdev_t *dev, const unsigned char *mac, size_t len)
{
	UNUSED(dev, mac, len);
	printf("%s\n", __func__);
	return 0;
}

static int ux_can_netif_set_ipaddr(ux_netdev_t *dev, struct sockaddr *addr,
				   const char *name, unsigned int ipmask, bool netlink)
{
	UNUSED(dev, addr, name);
	return 0;
}

static int ux_can_netif_set_netmask(ux_netdev_t *dev, struct sockaddr *addr, const char *name)
{
	UNUSED(dev, addr, name);
	return 0;
}

static int ux_can_netif_set_dstaddr(ux_netdev_t *dev, struct sockaddr *addr)
{
	UNUSED(dev, addr);
	return 0;
}

static int ux_can_netif_set_mtu(ux_netdev_t *dev, int mtu)
{
	if (mtu != (int)CANFD_MTU && mtu != (int)CAN_MTU) {
		return E_HM_INVAL;
	}

	dev->_mtu = mtu;

	return 0;
}

static int ux_can_netif_set_flags(ux_netdev_t *dev, short int flags, const char *name)
{
	UNUSED(name);
	if (flags & IFF_UP) {
		dev->flags |= IFF_UP | IFF_RUNNING;
	} else {
		dev->flags &= ~(IFF_UP | IFF_RUNNING);
	}

	return 0;
}

static int ux_can_netif_set_default(ux_netdev_t *dev, int action,
			     struct sockaddr *gw)
{
	UNUSED(dev, action, gw);
	return 0;
}

static ux_netdev_ops_priv_t can_netdev_priv_ops = {
	.dump_stat = NULL,
	.get_mac = ux_can_netif_get_mac,
	.getifaddrs = ux_can_netif_getifaddrs,
	.get_mtu = ux_can_netif_get_mtu,
	.get_flags = ux_can_netif_get_flags,
	.get_ipaddr = ux_can_netif_get_ipaddr,
	.get_netmask = ux_can_netif_get_netmask,
	.get_dstaddr = ux_can_netif_get_dstaddr,
	.get_stats = ux_can_netif_get_stats,
	.get_ifconf = ux_can_netif_get_ifconf,

	.set_mac = ux_can_netif_set_mac,
	.set_ipaddr = ux_can_netif_set_ipaddr,
	.del_ipaddr = NULL,
	.set_netmask  = ux_can_netif_set_netmask,
	.set_dstaddr = ux_can_netif_set_dstaddr,
	.set_mtu = ux_can_netif_set_mtu,
	.set_flags = ux_can_netif_set_flags,
	.set_rxhwcsum = NULL,

	.recv = ux_can_netdev_data_recv,
	.recv_zero_copy = ux_can_netdev_recv_zero_copy,
	.copy = ux_can_netdev_data_copy,
	.free = ux_can_netdev_data_free,

	.add_header = NULL,
	.remove_header = NULL,
	.get_packet_hdr_len = NULL,
	.set_packet_hdr = NULL,
};

static int ux_can_device_init(ux_netdev_t *dev)
{
	ux_can_netdev_t *candev = ux_netdev_priv(dev);

	dev->priv_ops = &can_netdev_priv_ops;
	dev->type = ARPHRD_CAN;
	dev->flags = IFF_NOARP;
	dev->_mtu = (int)CANFD_MTU;

	dlist_init(&candev->raw_sock_list);
	raw_mutex_init(&candev->raw_sock_list_lock);

	return 0;
}

static ux_packet_demux_t can_demux = {
	.head = DLIST_HEAD_INIT(can_demux.head),
	.type = ARPHRD_CAN,
	.dev = NULL,

	.init = ux_can_device_init,
	.uninit = NULL,
	.func = NULL,

	.find_ip_reachable = NULL,
	.set_route_entry = NULL,
	.set_default = ux_can_netif_set_default,
};

int ux_can_netdev_init(void)
{
	int ret;

	ret = ux_register_packet_demux(&can_demux);
	if (ret) {
		net_error(HMNET_CAN, "can device register demux failed\n");
		return ret;
	}

	return 0;
}

void ux_can_netdev_exit(void)
{
	ux_unregister_packet_demux(&can_demux);
}
