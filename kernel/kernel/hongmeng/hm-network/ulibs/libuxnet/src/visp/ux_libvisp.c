/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: interface for libdopra
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 29 09:56:30 2021
 */

#include <errno.h>
#include <net/route.h>

#include <libhmlog/hmlog.h>
#include <libstrict/strict.h>

#include <ux_netdev.h>
#include <ux_netlink_route.h>
#include <security/ux_security.h>

#include <visp_stack.h>
#include <visp_procfs.h>
#include <visp_security.h>

#include "ux_procfs.h"
#include "ux_visp_netstat.h"
#include "ux_visp_route.h"
#include "ux_visp_arp.h"
#include "ux_libvisp.h"

static unsigned int ux_packet_hook(void *mbuf)
{
	struct netif *loop_device = NULL;
	ux_netdev_t *dev = NULL;
	int len;

	if (mbuf == NULL) {
		return 0;
	}

	dev = visp_get_netdev_from_mbuf(mbuf);
	if (dev == NULL) {
		return 0;    /* always return 0, non-zero means do not continue process the packet */
	}

	len = visp_mbuf_length(mbuf);
	loop_device = ux_netdev_priv(dev);
	/**
	 * We only catch packet belong to loop interface
	 */
	if ((loop_device->flags & NETIF_FLAG_LOOPBACK) == 0) {
		return 0;
	}

	ux_dev_queue_nit_comm(mbuf, loop_device, len, UXDEV_PKT_BUF_NOLINK);
	return 0;
}

static int visp_get_configure(int *val, int type)
{
	switch(type) {
	case PROCFS_ICMP_ECHO_IGNORE_BROADCASTS:
		return visp_get_icmp_ignore_broadcast(val);
	case PROCFS_ALL_RP_FILTER:
	case PROCFS_DEFAULT_RP_FILTER:
		*val = (int)visp_get_rp_filter_mode();
		return 0;
	default:
		break;
	}

	return E_HM_OPNOTSUPP;
}

static int visp_set_configure(const int val, int type)
{
	switch(type) {
	case PROCFS_ICMP_ECHO_IGNORE_BROADCASTS:
		return visp_set_icmp_ignore_broadcast((unsigned int)val);
	case PROCFS_ALL_RP_FILTER:
	case PROCFS_DEFAULT_RP_FILTER:
		return visp_set_rp_filter_mode((enum rp_filter_mode)val);
	default:
		break;
	}

	return E_HM_OPNOTSUPP;
}

static void visp_procfs_init(void)
{
	ux_procfs_ops_t *ops = NULL;

	ops = get_procfs_ops();
	if (ops == NULL) {
		return;
	}

	ops->get_port_range = visp_get_port_range;
	ops->set_port_range = visp_set_port_range;
	ops->get_netstat = visp_procfs_get_netstat;
	ops->release_netstat = visp_procfs_release_netstat;
	ops->get_configure = visp_get_configure;
	ops->set_configure = visp_set_configure;
}

static void visp_netlink_init(void)
{
	ux_netlink_ops_t ops = {
		.get_neigh_info = visp_get_neigh_info_nl,
		.release_neigh_info = NULL,
		.get_route_info = visp_get_route_info_nl,
		.del_neigh_info = visp_del_neigh_info_nl,
		.release_route_info = NULL,
	};

	ux_netlink_register_ops(&ops);
}

static void visp_security_init(void)
{
	ux_sec_rsvport_init(check_in_reserved_ports, add_port_range_to_port_list);
}

static int visp_copy_from_user(void *dst, void *src, size_t size)
{
	return hm_actv_read_caller_vm(dst, src, size);
}

static int visp_copy_to_user(void *dst, void *src, size_t size)
{
	return hm_actv_write_caller_vm(dst, src, size);
}

int af_inet_init(void)
{
	int ret;

	ret = visp_stack_init();
	if (ret) {
		net_error(HMNET_INET, "visp stack init failed\n");
		return ret;
	}

	ret = visp_netdev_init();
	if (ret) {
		net_error(HMNET_INET, "visp netdev init failed\n");
		return ret;
	}

	ret = visp_socket_init();
	if (ret) {
		net_error(HMNET_INET, "visp socket init failed\n");
		return ret;
	}

	ret = visp_reg_packet_hook(ux_packet_hook);
	if (ret != 0) {
		net_error(HMNET_INET, "visp reg packet hook failed\n");
		return ret;
	}

	ret = visp_reg_event_notify_func(visp_event_notify_hook);
	if (ret != 0) {
		net_error(HMNET_INET, "visp reg event notify func failed\n");
		return ret;
	}

	ret = visp_reg_copy_functions(visp_copy_from_user, visp_copy_to_user);
	if (ret != 0) {
		net_error(HMNET_INET, "visp reg copy functions failed\n");
		return ret;
	}

	visp_reg_get_ifname_hook(visp_get_ifname_by_index);

	visp_procfs_init();
	visp_netlink_init();
	visp_security_init();
	ux_register_tcpip_set_log_level_hook(visp_set_log_level);

	return 0;
}

struct getifname_cb {
	unsigned int visp_ifindex;

	char *name;
	size_t max_len;
};

static int iter_callback(const ux_netdev_t *dev, void *arg)
{
	struct getifname_cb *cb = (struct getifname_cb*)arg;
	struct netif *netif = ux_netdev_priv(dev);

	if (netif->ifindex == cb->visp_ifindex) {
		int ret = strcpy_s(cb->name, cb->max_len, dev->name);
		if (ret != EOK) {
			return E_HM_POSIX_FAULT;
		}
		return 1;
	}

	return 0;
}

int visp_get_ifname_by_index(unsigned int ifindex, char *name, size_t max_len)
{
	struct getifname_cb cb = {ifindex, name, max_len};

	return ux_netdev_for_each_device(iter_callback, &cb);
}
