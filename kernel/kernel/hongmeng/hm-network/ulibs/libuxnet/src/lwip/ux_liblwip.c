/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: interface for liblwip
 * Author: Huawei OS Kernel Lab
 * Create: Sat Oct 12 10:49:11 2019
 */

#include <errno.h>
#include <net/route.h>

#include <lwip/sys.h>
#include <lwip/init.h>
#include <lwip/tcpip.h>
#include <lwip/procfs_if.h>
#include <lwip/etharp.h>
#include <lwip/stats.h>
#include <lwip/ip.h>
#include <lwip/tcp.h>
#include <lwip/udp.h>
#include <lwip/icmp.h>
#include <lwip/raw.h>
#include <lwip/api.h>
#include <lwip/debug.h>
#include <lwip/route.h>
#include <lwip/security.h>
#include <lwip/prot/ip4.h>

#include <libhmlog/hmlog.h>
#include <libhmsrv_sys/hm_timer.h>
#include <libstrict/strict.h>

#include "hm_netlink.h"
#include "hm_neighbour.h"
#include "ux_liblwip.h"
#include "ux_phy_mem.h"
#include "ux_procfs.h"
#include "ux_netlink_route.h"
#include "ux_netdev.h"
#include "ux_utils.h"
#include "security/ux_security.h"
#include "netfilter/ux_netfilter.h"


#define RX_ZERO_COPY_PBUF_NUM	20480
#define RT_RECORD_BUFSIZE	128
#define RT_PRINT_FORMAT		"%s\t%08x\t%08x\t%04X\t%d\t%d\t%d\t%08x\t%hu\t%d\t%d\n"
#define NETSTAT_PROTO_TYPES	3
#define NETSTAT_TCP_HEAD	"sl local_address rem_address st tx_queue " \
				"rx_queue tr tm->when retrnsmt uid timeout " \
				"inode\n"
#define NETSTAT_UDP_HEAD	"sl local_address rem_address st tx_queue " \
				"rx_queue tr tm->when retrnsmt uid timeout " \
				"inode ref pointer drops\n"
#define NETSTAT_RAW_HEAD	"sl local_address rem_address st tx_queue " \
				"rx_queue tr tm->when retrnsmt uid timeout " \
				"inode ref pointer drops\n"
#define NETSTAT_TCP_INFO_FORMAT	"0: %08X:%04X %08X:%04X %02X %08X:%08X 0:0 0 0 0 0\n"
#define NETSTAT_UDP_INFO_FORMAT	"0: %08X:%04X %08X:%04X %02X %08X:%08X 0:0 0 0 0 0 0 0 0\n"
#define NETSTAT_RAW_INFO_FORMAT	"0: %08X:%04X %08X:%04X %02X %08X:%08X 0:0 0 0 0 0 0 0 0\n"

#define PROTO_INDEX_TCP 0
#define PROTO_INDEX_UDP 1
#define PROTO_INDEX_RAW 2

typedef struct my_custom_pbuf {
	struct pbuf_custom p;
	unsigned int cell_offset;
	void *cell_unit_ptr;
	unsigned long free_size;
	hmshm_mgr_t *shmmgr;
} my_custom_pbuf_t;

struct procfs_index_mapping {
	enum procfs_net index;
	enum lwip_procfs_cfg item;
};

LWIP_MEMPOOL_DECLARE(RX_POOL, RX_ZERO_COPY_PBUF_NUM, (sizeof(struct my_custom_pbuf)), "rx zero copy pool");

extern int route_cnt;
extern struct netif_route *route_list;

static char *netstat_head_info[NETSTAT_PROTO_TYPES] = {
	NETSTAT_TCP_HEAD,
	NETSTAT_UDP_HEAD,
	NETSTAT_RAW_HEAD
};
static char *netstat_format[NETSTAT_PROTO_TYPES] = {
	NETSTAT_TCP_INFO_FORMAT,
	NETSTAT_UDP_INFO_FORMAT,
	NETSTAT_RAW_INFO_FORMAT
};

static struct procfs_index_mapping type_mapping[] = {
	{PROCFS_ALL_ACCEPT_REDIRECTS, LWIP_CFG_ALL_ACCEPT_REDIRECTS},
	{PROCFS_DEFAULT_ACCEPT_REDIRECTS, LWIP_CFG_DEFAULT_ACCEPT_REDIRECTS},
	{PROCFS_ALL_SECURE_REDIRECTS, LWIP_CFG_ALL_SECURE_REDIRECTS},
	{PROCFS_DEFAULT_SECURE_REDIRECTS, LWIP_CFG_DEFAULT_SECURE_REDIRECTS},
	{PROCFS_ALL_LOG_MARTIANS, LWIP_CFG_ALL_LOG_MARTIANS},
	{PROCFS_DEFAULT_LOG_MARTIANS, LWIP_CFG_DEFAULT_LOG_MARTIANS},
	{PROCFS_TCP_SYNCOOKIES, LWIP_CFG_TCP_SYNCOOKIES},
	{PROCFS_TCP_FIN_TIMEOUT, LWIP_CFG_TCP_FIN_TIMEOUT},
	{PROCFS_ALL_ACCEPT_SOURCE_ROUTE, LWIP_CFG_ALL_ACCEPT_SOURCE_ROUTE},
	{PROCFS_DEFAULT_ACCEPT_SOURCE_ROUTE, LWIP_CFG_DEFAULT_ACCEPT_SOURCE_ROUTE},
	{PROCFS_TCP_SYNBACKLOG, LWIP_CFG_TCP_SYNBACKLOG},
	{PROCFS_ICMP_ECHO_IGNORE_ALL, LWIP_CFG_ICMP_ECHO_IGNORE_ALL},
	{PROCFS_TCP_TIMESTAMPS, LWIP_CFG_TCP_TIMESTAMPS},
	{PROCFS_ICMP_ECHO_IGNORE_BROADCASTS, LWIP_CFG_ICMP_ECHO_IGNORE_BROADCASTS},
	{PROCFS_ICMP_IGNORE_BOGUS_ERROR_RESPONSES, LWIP_CFG_ICMP_IGNORE_BOGUS_ERROR_RESPONSES},
	{PROCFS_DEFAULT_ARP_PROXY, LWIP_CFG_DEFAULT_ARP_PROXY},
	{PROCFS_ALL_ARP_PROXY, LWIP_CFG_ALL_ARP_PROXY},
	{PROCFS_IP_FORWARD, LWIP_CFG_IP_FORWARD},
	{PROCFS_ALL_RP_FILTER, LWIP_CFG_ALL_RP_FILTER},
	{PROCFS_DEFAULT_RP_FILTER, LWIP_CFG_DEFAULT_RP_FILTER},
	{PROCFS_ALL_SEND_REDIRECTS, LWIP_CFG_ALL_SEND_REDIRECTS},
	{PROCFS_DEFAULT_SEND_REDIRECTS, LWIP_CFG_DEFAULT_SEND_REDIRECTS},
};

static void liblwip_init_rxpool(void)
{
	LWIP_MEMPOOL_INIT(RX_POOL);
}

static void lwip_set_seed(void)
{
	struct timespec ts = {0};
	int ret = 0;

	ret = hm_clock_gettime(CLOCK_MONOTONIC, &ts);
	if (ret != E_HM_OK) {
		net_debug(HMNET_CORE, "clock get time error\n");
		return;
	}

	srandom((unsigned int)ts.tv_nsec + (unsigned int)ts.tv_sec * NSEC_PER_SEC);
}

static void local_init(void *arg)
{
	sys_sem_t *init_sem = (sys_sem_t*)arg;

	lwip_set_seed();

	sys_sem_signal(init_sem);
}

static int lwip_stack_init(void)
{
	sys_sem_t init_sem;
	err_t err;

	err = sys_sem_new(&init_sem, 0);
	if (err != (err_t)ERR_OK) {
		net_error(HMNET_INET, "failed to create init_sem\n");
		return E_HM_NOMEM;
	}

	tcpip_init(local_init, &init_sem);

	/* we have to wait for initialization to finish before
	 * calling update_adapter()!
	 */
	if (sys_sem_wait(&init_sem) != 0) {
		/* Actually, we won't come here all the time */
		sys_sem_free(&init_sem);
		net_info(HMNET_INET, "lwip stack init failed\n");
		return -1;
	}
	sys_sem_free(&init_sem);
	net_info(HMNET_INET, "lwip stack init done\n");

	return 0;
}

static int lwip_dump_default_gw(char **msg, unsigned int total, unsigned int *poffset)
{
	ip4_addr_t *gw = NULL;
	unsigned short int flags = RTF_UP;
	int ret = 0;

	if (netif_default && netif_is_up(netif_default) &&
	    ip_2_ip4(&netif_default->gw)->addr != 0) {
		flags |= RTF_GATEWAY;

		gw = ip_2_ip4(&netif_default->gw);
		ret = snprintf_s((char *)*msg + *poffset, total - *poffset, RT_RECORD_BUFSIZE,
				RT_PRINT_FORMAT,
				netif_default->name, (unsigned int)0,
				gw->addr, flags,
				0, 0, 0, (unsigned int)0,
				netif_default->mtu, 0, 0);
		if (ret < 0) {
			return E_HM_INVAL;
		}
		*poffset += (unsigned int)ret;
	}

	return ret;
}

static int lwip_dump_route_entry(char **msg, unsigned int total, unsigned int *poffset)
{
	struct netif_route *route = NULL;
	struct netif *netif = NULL;
	unsigned short int flags;
	int ret = 0;

	if (route_list == NULL) {
		return ret;
	}

	for (route = route_list->next; route != route_list; route = route->next) {
		netif = netif_get_by_index((u8_t)route->ifindex);
		if (netif == NULL || (int)netif_is_up(netif) == 0) {
			continue;
		}

		flags = RTF_UP;
		if (route->netmask == 0xffffffffUL) {
			flags |= RTF_HOST;
		}

		if (route->gateway != 0) {
			flags |= RTF_GATEWAY;
		}

		ret = snprintf_s((char *)*msg + *poffset, total - *poffset,
				 RT_RECORD_BUFSIZE, RT_PRINT_FORMAT, netif->name,
				 route->subnet, route->gateway, flags, 0, 0, 0,
				 route->netmask, netif->mtu, 0, 0);
		if (ret < 0) {
			return E_HM_INVAL;
		}

		*poffset += (unsigned int)ret;
		if (*poffset > total - RT_RECORD_BUFSIZE) {
			break;
		}
	}

	return ret;
}

static int lwip_get_route_info(char **msg, int *mas_len, int type)
{
	UNUSED(type);
	unsigned int offset = 0;
	unsigned int total;
	int ret;
	char *p = NULL;

	if (msg == NULL || mas_len == NULL) {
		return E_HM_INVAL;
	}

	LOCK_TCPIP_CORE();
	/* 2 additional: 1 for default gateway, 1 for route header message */
	total = ((unsigned int)route_cnt + 2)  * RT_RECORD_BUFSIZE;
	p = (char *)malloc(total);
	if (p == NULL) {
		UNLOCK_TCPIP_CORE();
		return E_HM_NOMEM;
	}
	*msg = p;

	ret = snprintf_s((char *)*msg, total, RT_RECORD_BUFSIZE,
			"Iface\tDestination\tGateway\tFlags\tRefCnt\tUse\tMetric\tMask\tMTU\tWindow\tIRTT\n");
	if (ret < 0) {
		ret = E_HM_INVAL;
		goto err;
	}
	offset += (unsigned int)ret;
	ret = lwip_dump_default_gw(msg, total, &offset);
	if (ret < 0) {
		goto err;
	}
	ret = lwip_dump_route_entry(msg, total, &offset);
	if (ret < 0) {
		goto err;
	}
	UNLOCK_TCPIP_CORE();
	*mas_len = (int)offset;

	return 0;
err:
	UNLOCK_TCPIP_CORE();
	free(p);
	return ret;
}

static void lwip_get_route_from_netif(struct netif *netif, rt_entry_t *rt, int dt)
{
	ip4_addr_t *gw = NULL;
	ip4_addr_t *ip = NULL;
	ip4_addr_t *netmask = NULL;

	NOFAIL(memset_s(rt, sizeof(rt_entry_t), 0, sizeof(rt_entry_t)));
	NOFAIL(memcpy_s(rt->oifname, sizeof(rt->oifname), netif->name,
		       sizeof(netif->name)));

	if (dt == 1) {
		gw = ip_2_ip4(&netif->gw);
		rt->gw = gw->addr;
	} else {
		ip = ip_2_ip4(&netif->ip_addr);
		netmask = ip_2_ip4(&netif->netmask);
		rt->dst = (ip->addr & netmask->addr);
		rt->dst_len = mask_to_prelen(netmask->addr);
		rt->presrc = ip->addr;
	}
}

static int lwip_get_route_info_nl(void **buf, size_t *len)
{
	struct netif *netif = NULL;
	rt_entry_t rt;
	void *nbuf = NULL;
	int ret = 0;
	unsigned cnt = 0;
	size_t total_len;
	size_t copy_len = 0;

	if (buf == NULL || len == NULL) {
		return E_HM_INVAL;
	}

	LOCK_TCPIP_CORE();
	NETIF_FOREACH(netif) {
		cnt++;
	}
	total_len = (size_t)(sizeof(rt_entry_t) * (cnt + 1));
	nbuf = calloc(1, total_len);
	if (nbuf == NULL) {
		UNLOCK_TCPIP_CORE();
		return E_HM_NOMEM;
	}

	if (netif_default && netif_is_up(netif_default)) {
		lwip_get_route_from_netif(netif_default, &rt, 1);
		if (rt.gw != 0) {
			NOFAIL(memcpy_s(nbuf, total_len, &rt, sizeof(rt)));
			copy_len += sizeof(rt);
		}
	}

	NETIF_FOREACH(netif) {
		if (netif_is_up(netif) && strcmp(netif->name, "lo")) {
			lwip_get_route_from_netif(netif, &rt, 0);
			if (rt.gw != 0 || rt.dst != 0) {
				void *tmpbuf = (void *)((char *)nbuf + copy_len);
				NOFAIL(memcpy_s(tmpbuf, total_len - copy_len, &rt, sizeof(rt)));
				copy_len += sizeof(rt);
			}
		}
	}
	UNLOCK_TCPIP_CORE();
	*len = (unsigned int)copy_len;
	*buf = nbuf;
	return ret;
}

static int lwip_get_neigh_info_nl(void **buf, size_t *len)
{
	void *nbuf = NULL;
	size_t total_len;

	if (buf == NULL || len == NULL) {
		return E_HM_INVAL;
	}

	total_len = sizeof(nd_entry_t) * ARP_TABLE_SIZE;
	nbuf = calloc(1, total_len);
	if (nbuf == NULL) {
		return E_HM_NOMEM;
	}
	*len = (unsigned int)arp_print_all_entry_nl(nbuf, total_len);
	*buf = nbuf;
	return 0;
}

static void lwip_del_neigh_info_nl(uint32_t dst, char *ifname, uint16_t state)
{
	LOCK_TCPIP_CORE();
	arp_del_arp_nl(dst, ifname, state);
	UNLOCK_TCPIP_CORE();
}

static int get_all_pcb_nums(int proto)
{
	switch (proto) {
	case NETCONN_TCP:
		return get_all_tcp_pcb_nums();
	case NETCONN_UDP:
		return get_all_udp_pcb_nums();
	case NETCONN_RAW:
		return get_all_raw_pcb_nums();
	default:
		return 0;
	}
}

static int get_proto_index(int proto)
{
	/* 0, 1, 2 represent index of tcp, udp, raw */
	switch (proto) {
	case NETCONN_TCP:
		return PROTO_INDEX_TCP;
	case NETCONN_UDP:
		return PROTO_INDEX_UDP;
	case NETCONN_RAW:
		return PROTO_INDEX_RAW;
	default:
		return -1;
	}
}

static int format_netstat_info(char *buf, const struct netstat_info *info,
			       int total_len, int offset)
{
	int proto;
	int ret;

	proto = get_proto_index(info->protocol);
	/* 2 represents index of NETCONN_RAW */
	if (proto < 0 || proto > 2) {
		return E_HM_INVAL;
	}

	if (offset + NETSTAT_ITEM_LEN >= total_len) {
		return E_HM_MSGSIZE;
	}

	ret = sprintf_s(buf + offset,
			(size_t)(unsigned int)(total_len - offset),
			netstat_format[proto], info->laddr, info->lport,
			info->raddr, info->rport, info->state,
			info->send_queue, info->recv_queue);
	if (ret < 0) {
		ret = E_HM_INVAL;
	}

	return ret;
}

static int lwip_get_netstat_info(char **msg, int *msg_len, const int proto)
{
	int pcb_nums, total_len, ret, index;
	char *info = NULL;
	char *head_info = NULL;

	index = get_proto_index(proto);
	/* 2 represents index of NETCONN_RAW */
	if (index < 0 || index > 2) {
		return E_HM_INVAL;
	}

	head_info = netstat_head_info[index];

	LOCK_TCPIP_CORE();
	pcb_nums = get_all_pcb_nums(proto);
	total_len = (int)strlen(head_info) + pcb_nums * NETSTAT_ITEM_LEN + 1;

	info = calloc(1, (size_t)(unsigned int)total_len);
	if (info == NULL) {
		UNLOCK_TCPIP_CORE();
		return E_HM_NOMEM;
	}

	ret = sprintf_s(info, (size_t)(unsigned int)total_len, "%s", head_info);
	if (ret < 0) {
		ret = E_HM_MSGSIZE;
		goto err;
	}

	ret = lwip_iter_socket(proto, info, total_len, (int)strlen(head_info),
			       format_netstat_info);
	if (ret < 0) {
		goto err;
	}

	UNLOCK_TCPIP_CORE();

	*msg_len = (int)strlen(info);
	*msg = info;
	return 0;

err:
	UNLOCK_TCPIP_CORE();
	free(info);
	info = NULL;
	return ret;
}

static int lwip_procfs_get_netstat(char **msgp, int *msg_len, int type)
{
	UNUSED(type);
	char *msg = NULL;
	size_t msglen = PROCFS_MSG_MAX_LEN;
	size_t offset = 0;
	int ret = 0;
	int i = 0;

	msg = calloc(1, msglen);
	if (msg == NULL) {
		return E_HM_NOMEM;
	}

	ret = sprintf_s(msg, msglen, "%s", "TcpExt:");
	if (ret < 0) {
		ret = E_HM_MSGSIZE;
		goto err;
	}
	offset += (size_t)(unsigned int)ret;
	for (i = 0; snmp_tcp_list[i].name != NULL; i++) {
		ret = sprintf_s(msg + offset, msglen - offset, " %s",
				snmp_tcp_list[i].name);
		if (ret < 0) {
			ret = E_HM_MSGSIZE;
			goto err;
		}
		offset += (size_t)(unsigned int)ret;
	}
	ret = sprintf_s(msg + offset, msglen - offset, "%s", "\nTcpExt:");
	if (ret < 0) {
		ret = E_HM_MSGSIZE;
		goto err;
	}
	offset += (size_t)(unsigned int)ret;
	for (i = 0; snmp_tcp_list[i].name != NULL; i++) {
		ret = sprintf_s(msg + offset, msglen - offset, " %u",
				*(snmp_tcp_list[i].entry));
		if (ret < 0) {
			ret = E_HM_MSGSIZE;
			goto err;
		}
		offset += (size_t)(unsigned int)ret;
	}

	*msg_len = (int)offset;
	*msgp = msg;
	return 0;
err:
	free(msg);
	return ret;
}

static int __arp_entry_num_inc(struct etharp_entry *entry, void *data)
{
	UNUSED(entry);
	int *count = (int *)data;
	*count += 1;
	return 0;
}

struct arp_dump_args {
	char *buf;
	unsigned int total;
	unsigned int *offset;
};

#define SIZE_IP4BUF 16
#define SIZE_MACBUF 18
#define DEFAULT_ARP_HW_TYPE 1
#define DEFAULT_ARP_FLAGS 2
static int __arp_entry_dump(struct etharp_entry *entry, void *data)
{
	int ret;
	char ip4buf[SIZE_IP4BUF] = {0};
	char macbuf[SIZE_MACBUF] = {0};
	unsigned char *p;
	unsigned int *offset;
	struct arp_dump_args *args = (struct arp_dump_args *)data;

	/* skip pending entry */
	if (entry->state < (u8_t)ETHARP_STATE_STABLE) {
		return 0;
	}

	/*
	 * Format ip address to string,
	 * p[0], p[1], p[2], p[3] represents the four segments of the ip address.
	 */
	p = (unsigned char *)&entry->ipaddr;
	ret = snprintf_s(ip4buf, SIZE_IP4BUF, SIZE_IP4BUF - 1, "%u.%u.%u.%u", p[0], p[1], p[2], p[3]);
	if (ret < 0) {
		return E_HM_INVAL;
	}

	/*
	 * Format mac address to string,
	 * p[0], p[1], p[2], p[3], p[4], p[5] represents the six segments of
	 * the mac address.
	 */
	p = (unsigned char *)&entry->ethaddr;
	ret = snprintf_s(macbuf, SIZE_MACBUF, SIZE_MACBUF - 1, "%02x:%02x:%02x:%02x:%02x:%02x",
			 p[0], p[1], p[2], p[3], p[4], p[5]);
	if (ret < 0) {
		return E_HM_INVAL;
	}

	offset = args->offset;
	ret = snprintf_s(args->buf + *offset, args->total - *offset, RT_RECORD_BUFSIZE,
			 "%-16s 0x%-10x0x%-10x%s     *        %s\n",
			 ip4buf, DEFAULT_ARP_HW_TYPE, DEFAULT_ARP_FLAGS,
			 macbuf, entry->netif ? entry->netif->name : "");
	if (ret < 0) {
		return E_HM_INVAL;
	}
	*offset += (unsigned int)ret;

	return 0;
}

static int lwip_procfs_dump_arptable(char **msg, int *msg_len, int type)
{
	UNUSED(type);
	unsigned int offset = 0;
	unsigned int total;
	int ret;
	int num = 0;
	char *p = NULL;
	struct arp_dump_args dump_args;

	if (msg == NULL || msg_len == NULL) {
		return E_HM_INVAL;
	}

	LOCK_TCPIP_CORE();
	(void)etharp_foreach_entry(__arp_entry_num_inc, &num);
	/* 1 extra for header line */
	total = (unsigned int)((num + 1) * RT_RECORD_BUFSIZE);
	p = (char *)malloc(total);
	if (p == NULL) {
		UNLOCK_TCPIP_CORE();
		return E_HM_NOMEM;
	}
	*msg = p;

	ret = snprintf_s((char *)*msg, total, RT_RECORD_BUFSIZE,
			"IP address       HW type     Flags       HW address            Mask     Device\n");
	if (ret < 0) {
		ret = E_HM_INVAL;
		goto err;
	}
	offset += (unsigned int)ret;
	dump_args.buf = *msg;
	dump_args.total = total;
	dump_args.offset = &offset;
	ret = etharp_foreach_entry(__arp_entry_dump, &dump_args);
	if (ret < 0) {
		goto err;
	}
	UNLOCK_TCPIP_CORE();
	*msg_len = (int)offset;

	return 0;
err:
	UNLOCK_TCPIP_CORE();
	free(p);
	return ret;
}

static int lwip_get_netstat(char **msg, int *msg_len, int type)
{
	switch (type) {
	case PROCFS_NETSTAT_TCP:
		return lwip_get_netstat_info(msg, msg_len, (int)NETCONN_TCP);
	case PROCFS_NETSTAT_UDP:
		return lwip_get_netstat_info(msg, msg_len, (int)NETCONN_UDP);
	case PROCFS_NETSTAT_RAW:
		return lwip_get_netstat_info(msg, msg_len, (int)NETCONN_RAW);
	case PROCFS_GET_ROUTE_INFO:
		return lwip_get_route_info(msg, msg_len, type);
	case PROCFS_GET_NETSTAT:
		return lwip_procfs_get_netstat(msg, msg_len, type);
	case PROCFS_DUMP_ARPTABLE:
		return lwip_procfs_dump_arptable(msg, msg_len, type);
	default:
		break;
	}

	return E_HM_OPNOTSUPP;
}

static void lwip_release_netstat_mem(char *buf, int type)
{
	UNUSED(type);
	if (buf != NULL) {
		free(buf);
	}
}

static int lwip_lookup_procfs_item(int type)
{
	int i;

	for (i = 0; i < (int)ARRAY_SIZE(type_mapping); i++) {
		if ((int)type_mapping[i].index == type) {
			return (int)type_mapping[i].item;
		}
	}

	return -1;
}

static int lwip_procfs_set_configure(int val, int type)
{
	int item;

	item = lwip_lookup_procfs_item(type);
	if (item == -1) {
		return E_HM_OPNOTSUPP;
	}

	return lwip_set_configure(val, item);
}

static int lwip_procfs_get_configure(int *val, int type)
{
	int item;

	item = lwip_lookup_procfs_item(type);
	if (item == -1) {
		return E_HM_OPNOTSUPP;
	}

	return lwip_get_configure(val, item);
}

static void lwip_procfs_init(void)
{
	ux_procfs_ops_t *ops = NULL;

	ops = get_procfs_ops();
	if (ops == NULL) {
		return;
	}

	ops->get_port_range = lwip_get_port_range;
	ops->set_port_range = lwip_set_port_range;
	ops->get_netstat = lwip_get_netstat;
	ops->release_netstat = lwip_release_netstat_mem;
	ops->get_configure = lwip_procfs_get_configure;
	ops->set_configure = lwip_procfs_set_configure;
}

static void lwip_netlink_init(void)
{
	ux_netlink_ops_t ops = {
		.get_neigh_info = lwip_get_neigh_info_nl,
		.release_route_info = NULL,
		.get_route_info = lwip_get_route_info_nl,
		.del_neigh_info = lwip_del_neigh_info_nl,
		.release_neigh_info = NULL,
	};

	ux_netlink_register_ops(&ops);
}

static void lwip_security_init(void)
{
	ux_sec_rsvport_init(check_in_reserved_ports, add_port_range_to_port_list);
}

#ifdef CONFIG_UXNET_NETFILTER
static void ux_nf_send_unreach(struct pbuf *pbuf, enum icmp_dur_type t)
{
	struct ip_hdr *iphdr;
	struct icmp_echo_hdr *icmphdr;

	if (pbuf->len < sizeof(struct ip_hdr)) {
		return;
	}

	/*
	 * Note:
	 *	If the packet is already an unreachable ICMP packet,
	 *	we do not need to send an unreachable ICMP packet again.
	 */
	iphdr = (struct ip_hdr *)pbuf->payload;
	if (IPH_PROTO(iphdr) == IPPROTO_ICMP) {
		if (pbuf->len < sizeof(struct icmp_echo_hdr) + IPH_HL_BYTES(iphdr)) {
			net_debug(HMNET_CORE, "ux_nf_send_unreach: pbuf->len is too small\n");
			return;
		} else {
			icmphdr = (struct icmp_echo_hdr *)((char *)pbuf->payload + IPH_HL_BYTES(iphdr));
			if (icmphdr->type == ICMP_DUR) {
				return;
			}
		}
	}

	icmp_dest_unreach(pbuf, t);

	return;
}

static int ux_nf_rej(struct pbuf *pbuf, int nf_ret)
{
	int ret = E_HM_INVAL;

	switch (nf_ret) {
	case UX_NF_ICMP_NET_REJ:
		ux_nf_send_unreach(pbuf, ICMP_DUR_NET);
		ret = E_HM_OK;
		break;
	case UX_NF_ICMP_HOST_REJ:
		ux_nf_send_unreach(pbuf, ICMP_DUR_HOST);
		ret = E_HM_OK;
		break;
	case UX_NF_ICMP_PROT_REJ:
		ux_nf_send_unreach(pbuf, ICMP_DUR_PROTO);
		ret = E_HM_OK;
		break;
	case UX_NF_ICMP_PORT_REJ:
		ux_nf_send_unreach(pbuf, ICMP_DUR_PORT);
		ret = E_HM_OK;
		break;
	case UX_NF_ICMP_NET_PRO_REJ:
		ux_nf_send_unreach(pbuf, ICMP_DUR_NET_ANO);
		ret = E_HM_OK;
		break;
	case UX_NF_ICMP_HOST_PRO_REJ:
		ux_nf_send_unreach(pbuf, ICMP_DUR_HOST_ANO);
		ret = E_HM_OK;
		break;
	case UX_NF_ICMP_ADMIN_PRO_REJ:
		ux_nf_send_unreach(pbuf, ICMP_DUR_PKT_FILTERED);
		ret = E_HM_OK;
		break;
	default:
		break;
	}

	return ret;
}

static bool ux_nf_is_reject(int result)
{
	bool ret = false;
	if (result == UX_NF_ICMP_NET_REJ ||
	    result == UX_NF_ICMP_HOST_REJ ||
	    result == UX_NF_ICMP_PROT_REJ ||
	    result == UX_NF_ICMP_PORT_REJ ||
	    result == UX_NF_ICMP_NET_PRO_REJ ||
	    result == UX_NF_ICMP_HOST_PRO_REJ ||
	    result == UX_NF_ICMP_ADMIN_PRO_REJ) {
		ret = true;
	}
	return ret;
}
#endif

static int liblwip_nf_hook(int protocol, unsigned int hook,
			   struct pbuf *pbuf, const struct netif *ifin, const struct netif *ifout)
{
	int ret = 0;
	unsigned int copied = 0;
	ux_pktbuf_t *pktbuf = NULL;

	pktbuf = ux_pktbuf_alloc(NF_MAX_PKTBUF);
	if (pktbuf == NULL) {
		goto out;
	}

	copied = pbuf_copy_partial(pbuf, pktbuf->payload, NF_MAX_PKTBUF, 0);
	if (copied == 0) {
		ux_pktbuf_free(pktbuf);
		goto out;
	}
	pktbuf->len = copied;

	pktbuf->ctrk = pbuf->pct;
	ret = ux_nf_hook(protocol, hook, pktbuf,
			ifin == NULL ? NULL : ux_netdev_from_priv(ifin),
			ifout == NULL ? NULL : ux_netdev_from_priv(ifout));
	pbuf->pct = pktbuf->ctrk;
#ifdef CONFIG_UXNET_NETFILTER
	if (ret == UX_NF_ACCEPT) {
		ret = 1;
	} else if (ret == UX_NF_DROP) {
		ret = E_HM_PERM;
	} else if (ux_nf_is_reject(ret)) {
		if (ux_nf_rej(pbuf, ret) == E_HM_OK) {
			ret = E_HM_PERM;
		}
	} else {
		/* do nothing */
	}
#endif

	ux_pktbuf_free(pktbuf);

out:
	return ret;
}

int af_inet_init(void)
{
	int ret;

	ret = lwip_stack_init();
	if (ret != E_HM_OK) {
		net_error(HMNET_INET, "lwip stack init failed\n");
		return ret;
	}

	ret = lwip_netdev_init();
	if (ret != E_HM_OK) {
		net_error(HMNET_INET, "lwip stack init failed\n");
		return ret;
	}

	ret = lwip_socket_init();
	if (ret != E_HM_OK) {
		net_error(HMNET_INET, "lwip stack init failed\n");
		return ret;
	}

	liblwip_init_rxpool();

	lwip_procfs_init();
	lwip_netlink_init();
	lwip_security_init();
	lwip_register_nf_hook(liblwip_nf_hook);
	lwip_register_queue_nit_hook(ux_dev_queue_nit_comm);
	ux_register_tcpip_set_log_level_hook(liblwip_set_log_level);

	return 0;
}

void my_pbuf_free_custom(struct pbuf* p)
{
	my_custom_pbuf_t *my_pbuf = (my_custom_pbuf_t*)(void *)p;

	phy_mem_free(my_pbuf->shmmgr, my_pbuf->cell_unit_ptr, my_pbuf->cell_offset);
	LWIP_MEMPOOL_FREE(RX_POOL, my_pbuf);
}

struct pbuf *my_pbuf_alloc(void *data, size_t len, unsigned int cell_offset,
			   unsigned long free_size, hmshm_mgr_t *shmmgr)
{
	struct pbuf *pbuf = NULL;
	my_custom_pbuf_t* my_pbuf  = (my_custom_pbuf_t*)LWIP_MEMPOOL_ALLOC(RX_POOL);
	if (my_pbuf == NULL) {
		net_error(HMNET_INET, "RX POOL alloc fail\n");
		return NULL;
	}

	my_pbuf->p.custom_free_function = my_pbuf_free_custom;
	my_pbuf->cell_unit_ptr = data;
	my_pbuf->cell_offset = cell_offset;
	my_pbuf->free_size = free_size;
	my_pbuf->shmmgr = shmmgr;

	pbuf = pbuf_alloced_custom(PBUF_RAW,
			(unsigned short)len,
			PBUF_REF,
			&my_pbuf->p,
			(void *)data,
			(unsigned short)len);
	if (pbuf == NULL) {
		net_error(HMNET_INET, "pbuf_alloced_custom fail\n");
		my_pbuf_free_custom((struct pbuf *)my_pbuf);
	}

	return pbuf;
}

void rx_zero_poll_stat(void)
{
	net_info(HMNET_INET, "\nMEM %s\n\t", "RX_ZERO_POOL");
	net_info(HMNET_INET, "avail: %"U32_F"\n\t", (u32_t)memp_RX_POOL.stats->avail);
	net_info(HMNET_INET, "used: %"U32_F"\n\t", (u32_t)memp_RX_POOL.stats->used);
	net_info(HMNET_INET, "max: %"U32_F"\n\t", (u32_t)memp_RX_POOL.stats->max);
	net_info(HMNET_INET, "err: %"U32_F"\n", (u32_t)memp_RX_POOL.stats->err);
}

static uint32_t hmlog_level_to_lwip_level(int level)
{
	switch (level) {
	case HMLOG_DEBUG:
		return LWIP_DBG_LEVEL_ALL;
	case HMLOG_INFO:
		return LWIP_DBG_LEVEL_ALL;
	case HMLOG_WARN:
		return LWIP_DBG_LEVEL_WARNING;
	case HMLOG_ERROR:
		return LWIP_DBG_LEVEL_SERIOUS;
	case HMLOG_PANIC:
		return LWIP_DBG_LEVEL_SEVERE;
	default:
		return LWIP_DBG_LEVEL_WARNING;
	}
}

void liblwip_set_log_level(int level)
{
	net_info(HMNET_CORE, "set inet log level to %d\n", level);
	g_lwip_log_level = hmlog_level_to_lwip_level(level);
}
