/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: interface for libdopra
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 29 09:56:30 2021
 */

#include <sys/ioctl.h>
#include <net/if_arp.h>
#include <netinet/if_ether.h>
#include <libhwsecurec/securec.h>
#include <arpa/inet.h>
#include <visp_arp.h>

#include "ux_netdev.h"
#include "hm_neighbour.h"
#include "ux_visp_arp.h"
#include "ux_libvisp.h"

#define RT_RECORD_BUFSIZE	128

static int visp_ioctl_del_arp(struct arpreq *req)
{
	return visp_del_arp(((struct sockaddr_in *)&req->arp_pa)->sin_addr.s_addr,
			    DELETE_STATIC);
}

static int visp_ioctl_get_arp(struct arpreq *req)
{
	return visp_get_arp(((struct sockaddr_in *)&req->arp_pa)->sin_addr.s_addr,
			    req->arp_ha.sa_data, ETH_ALEN);
}

static int visp_ioctl_set_arp(struct arpreq *req)
{
	return visp_set_static_arp(((struct sockaddr_in *)&req->arp_pa)->sin_addr.s_addr,
				   req->arp_ha.sa_data);
}

int visp_arp_ioctl(int cmd, struct arpreq *req)
{
	int ret;

	switch (cmd) {
	case SIOCDARP:
		ret = visp_ioctl_del_arp(req);
		break;
	case SIOCGARP:
		ret = visp_ioctl_get_arp(req);
		break;
	case SIOCSARP:
		ret = visp_ioctl_set_arp(req);
		break;
	default:
		ret = E_HM_INVAL;
		break;
	}

	return ret;
}

#define SIZE_IP4BUF 16
#define SIZE_MACBUF 18
#define SIZE_IFNAME 48
#define DEFAULT_ARP_HW_TYPE 1
#define DEFAULT_ARP_FLAGS 2

struct dump_arp_args {
	char *msg;
	size_t max_len;

	unsigned int offset;
};

static int dump_arp_iterator(struct visp_arp_entry *entry, void *args)
{
	struct dump_arp_args *dump_args = args;
	char ip4buf[SIZE_IP4BUF] = {0};
	char macbuf[SIZE_MACBUF] = {0};
	char ifname[SIZE_IFNAME] = {0};
	struct in_addr ip_addr = {entry->ipaddr};
	unsigned char *p = entry->mac;
	int ret;

	if (inet_ntop(AF_INET, &ip_addr, ip4buf, sizeof(ip4buf)) == NULL) {
		return E_HM_INVAL;
	}

	ret = snprintf_s(macbuf, SIZE_MACBUF, SIZE_MACBUF,
			 "%02x:%02x:%02x:%02x:%02x:%02x",
			 p[0], p[1], p[2], p[3], p[4], p[5]); /* 0,1,2,3,4,5 are indexes of mac address */
	if (ret <= 0) {
		return E_HM_INVAL;
	}

	visp_get_ifname_by_index(entry->if_index, ifname, sizeof(ifname));

	ret = snprintf_s(dump_args->msg + dump_args->offset, dump_args->max_len - dump_args->offset,
			 RT_RECORD_BUFSIZE,
			 "%-16s 0x%-10x0x%-10x%s     *        %s\n",
			 ip4buf, DEFAULT_ARP_HW_TYPE, DEFAULT_ARP_FLAGS,
			 macbuf, ifname);
	if (ret <= 0) {
		return E_HM_INVAL;
	}

	dump_args->offset += ret;
	if (dump_args->offset >= dump_args->max_len) {
		return 1;    /* stop iteration */
	}

	return 0;
}

int visp_dump_arp_table(char **msg, int *msg_len, int type)
{
	struct dump_arp_args args = {0};
	int total;
	int ret;

	if (msg == NULL || msg_len == NULL) {
		return E_HM_INVAL;
	}

	total = visp_get_arp_entry_count(GET_ALL);
	if (total <= 0) {
		total = 0;
	}

	args.max_len = (total + 1) * RT_RECORD_BUFSIZE;
	args.msg = malloc(args.max_len);
	if (args.msg == NULL) {
		return E_HM_NOMEM;
	}

	ret = snprintf_s(args.msg, args.max_len, RT_RECORD_BUFSIZE,
			"IP address       HW type     Flags       HW address            Mask     Device\n");
	if (ret < 0) {
		free(args.msg);
		return E_HM_INVAL;
	}

	args.offset += ret;

	if (total != 0) {
		ret = visp_for_each_arp_entry(dump_arp_iterator, &args);
		if (ret < 0) {
			free(args.msg);
			return ret;
		}
	}

	*msg = args.msg;
	*msg_len = args.offset;
	return 0;
}

struct get_neigh_info_args {
	nd_entry_t *nbuf;
	unsigned int count;

	unsigned int index;
};

static int get_neigh_info_iterator(struct visp_arp_entry *entry, void *args)
{
	struct get_neigh_info_args *neigh_args = args;
	nd_entry_t *nbuf = neigh_args->nbuf;
	int i = neigh_args->index;

	if (neigh_args->index >= neigh_args->count) {
		return 1;
	}

	nbuf[i].dst = entry->ipaddr;
	if (entry->flags == 2) {        /* entry->flags: 2(RESOLVED); 3(RESOLVING) */
		nbuf[i].state = 2;
	} else if (entry->flags == 3) { /* entry->flags: 2(RESOLVED); 3(RESOLVING) */
		nbuf[i].state = 1;
	} else {
		nbuf[i].state = 0;      /* Invalidate arp entry */
	}

	NOFAIL(memcpy_s(nbuf[i].lladdr, sizeof(nbuf[i].lladdr), entry->mac, sizeof(nbuf[i].lladdr)));
	visp_get_ifname_by_index(entry->if_index, nbuf[i].ifname, sizeof(nbuf[i].ifname));
	neigh_args->index ++;
	return 0;
}

int visp_get_neigh_info_nl(void **buf, size_t *len)
{
	struct get_neigh_info_args args;
	int ret;

	if (buf == NULL || len == NULL) {
		return E_HM_INVAL;
	}

	*len = 0;
	*buf = NULL;

	args.count = visp_get_arp_entry_count(GET_ALL);
	if (args.count == 0) {
		return 0;
	}

	args.nbuf = calloc(1, sizeof(nd_entry_t) * args.count);
	if (args.nbuf == NULL) {
		return E_HM_NOMEM;
	}

	args.index = 0;
	ret = visp_for_each_arp_entry(get_neigh_info_iterator, &args);
	if (ret < 0) {
		free(args.nbuf);
		return ret;
	}

	*len = sizeof(nd_entry_t) * args.index;
	*buf = args.nbuf;
	return 0;
}

void visp_del_neigh_info_nl(uint32_t dst, char *ifname, uint16_t state)
{
	UNUSED(ifname, state);
	visp_del_arp(dst, DELETE_STATIC);
}
