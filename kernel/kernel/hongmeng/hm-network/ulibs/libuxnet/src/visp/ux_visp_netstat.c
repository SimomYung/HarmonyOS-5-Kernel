/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: interface for visp netstats
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 29 09:56:30 2021
 */

#include "ux_libvisp.h"
#include "ux_procfs.h"
#include "ux_visp_netstat.h"
#include "ux_visp_route.h"
#include "ux_visp_arp.h"

#include <visp_procfs.h>

#define NETSTAT_ITEM_LEN	150
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

enum {
	PROTO_INDEX_TCP,
	PROTO_INDEX_UDP,
	PROTO_INDEX_RAW,
	NETSTAT_PROTO_TYPES
};

struct dump_netstat_info {
	char *msg;
	size_t total_len;
	unsigned int offset;
	int type;
};

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

static int format_netstat_info(struct netstat_info *info,
			       void *dump_args, bool *is_break)
{
	int ret;
	struct dump_netstat_info *args = dump_args;

	if (args->offset + NETSTAT_ITEM_LEN >= args->total_len) {
		*is_break = true;
		return 0;
	}

	ret = sprintf_s(args->msg + args->offset,
			(size_t)(unsigned int)(args->total_len - args->offset),
			netstat_format[args->type], info->laddr, info->lport,
			info->raddr, info->rport, info->state,
			info->send_queue, info->recv_queue);
	if (ret >= 0) {
		args->offset += ret;
	} else {
		ret = E_HM_INVAL;
	}

	return ret;
}

static int visp_get_netstat_entry_count(int type)
{
	int ret;

	if (type == PROTO_INDEX_TCP) {
		ret = visp_get_tcp_entry_count();
	} else if (type == PROTO_INDEX_UDP) {
		ret = visp_get_udp_entry_count();
	} else {
		ret = E_HM_NOSYS;
	}

	return ret;
}

static int visp_for_each_stat_entry(int (*cb)(struct netstat_info *, void *, bool *), struct dump_netstat_info *args)
{
	int ret;

	if (args->type == PROTO_INDEX_TCP) {
		ret = visp_for_each_tcp_stat_entry(cb, args);
	} else if (args->type == PROTO_INDEX_UDP) {
		ret = visp_for_each_udp_stat_entry(cb, args);
	} else {
		ret = E_HM_NOSYS;
	}

	return ret;
}

static int visp_get_netstat_info(char **msg, int *msg_len, int type)
{
	int ret;
	char *head_info = NULL;
	int offset;
	int count = 0;
	struct dump_netstat_info args = {0};

	/* now netstat type support tcp and udp */
	if (msg == NULL || msg_len == NULL ||
	    type < PROTO_INDEX_TCP || type >= NETSTAT_PROTO_TYPES) {
		return E_HM_INVAL;
	}

	head_info = netstat_head_info[type];
	count = visp_get_netstat_entry_count(type);
	if (count <= 0) {
		count = 0;
	}

	args.total_len = (int)strlen(head_info) + count * NETSTAT_ITEM_LEN + 1;
	args.type = type;
	args.msg = calloc(1, args.total_len);
	if (args.msg == NULL) {
		return E_HM_NOMEM;
	}

	ret = sprintf_s(args.msg, (size_t)(unsigned int)args.total_len, "%s", head_info);
	if (ret < 0) {
		free(args.msg);
		return E_HM_MSGSIZE;
	}

	offset = (int)strlen(head_info);
	args.offset += offset;

	if (count != 0) {
		ret = visp_for_each_stat_entry(format_netstat_info, &args);
		if (ret < 0) {
			free(args.msg);
			return ret;
		}
	}

	*msg_len = (int)strlen(args.msg);
	*msg = args.msg;
	return 0;
}

static int visp_procfs_get_net_statistics(char **msgp, int *msg_len, int type)
{
	struct net_statistics stat;
	char *msg = NULL;
	size_t msglen = PROCFS_MSG_MAX_LEN;
	size_t offset = 0;
	int ret;

	UNUSED(type);

	ret = visp_get_net_statistics(&stat);
	if (ret != 0) {
		return ret;
	}

	msg = calloc(1, msglen);
	if (msg == NULL) {
		return E_HM_NOMEM;
	}

	ret = sprintf_s(msg, msglen, "%s", "IpExt: InNoRoutes InTruncatedPkts InMcastPkts OutMcastPkts "
				     "InBcastPkts OutBcastPkts InOctets OutOctets InMcastOctets OutMcastOctets "
				     "InBcastOctets OutBcastOctets InCsumErrors InNoECTPkts InECT1Pkts InECT0Pkts "
				     "InCEPkts ReasmOverlaps\n");
	if (ret <= 0) {
		free(msg);
		return E_HM_FAULT;
	}

	offset += (size_t)(unsigned int)ret;

	ret = sprintf_s(msg + offset, msglen - offset, "%u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u %u\n",
			stat.noroute,
			0,
			stat.mcasts,
			stat.mcastsout,
			stat.bcasts,
			stat.bcastsout,
			0,
			0,
			stat.mcastbytes,
			stat.mcastbytesout,
			stat.bcastbytes,
			stat.bcastbytesout,
			stat.badsum,
			0,
			0,
			0,
			0,
			0);
	if (ret <= 0) {
		free(msg);
		return E_HM_FAULT;
	}

	offset += ret;

	*msg_len = (int)offset;
	*msgp = msg;
	return 0;
}

/* PROCFS_NETSTAT_RAW is not support by visp */
int visp_procfs_get_netstat(char **msg, int *msg_len, int type)
{
	switch (type) {
	case PROCFS_NETSTAT_TCP:
		return visp_get_netstat_info(msg, msg_len, PROTO_INDEX_TCP);
	case PROCFS_NETSTAT_UDP:
		return visp_get_netstat_info(msg, msg_len, PROTO_INDEX_UDP);
	case PROCFS_GET_NETSTAT:
		return visp_procfs_get_net_statistics(msg, msg_len, type);
	case PROCFS_GET_ROUTE_INFO:
		return visp_get_route_info_procfs(msg, msg_len, type);
	case PROCFS_DUMP_ARPTABLE:
		return visp_dump_arp_table(msg, msg_len, type);
	default:
		break;
	}

	return E_HM_OPNOTSUPP;
}

void visp_procfs_release_netstat(char *buf, int type)
{
	UNUSED(type);
	if (buf != NULL) {
		free(buf);
	}
}
