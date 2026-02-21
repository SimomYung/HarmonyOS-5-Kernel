/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: interface for libdopra
 * Author: Huawei OS Kernel Lab
 * Create: Thu Apr 29 09:56:30 2021
 */

#include "hm_netlink.h"

#include "ux_visp_arp.h"
#include "ux_visp_route.h"
#include "ux_libvisp.h"

#include <visp_route.h>

#define RT_RECORD_BUFSIZE	128
#define RT_PRINT_FORMAT		"%s\t%08x\t%08x\t%04X\t%d\t%d\t%d\t%08x\t%hu\t%d\t%d\n"

struct get_route_info_args {
	char *msg;
	unsigned int max_size;

	unsigned int offset;
};

#define MAX_NETMASK_LEN	32U

static unsigned int masklen_2_mask(unsigned int len)
{
	unsigned int mask = 0;
	unsigned int i;

	if (len >= MAX_NETMASK_LEN) {
		return 0xFFFFFFFF;
	}

	for (i = 1; i <= len; ++i) {
		mask |= (1U << (MAX_NETMASK_LEN - i));
	}

	return mask;
}

static int get_route_info_iterator(struct visp_route_entry *entry, void *arg)
{
	struct get_route_info_args *info = arg;
	char ifname[IFNAMSIZ];
	unsigned short flags = entry->flags;
	in_addr_t gw = 0;
	int ret;

	if (flags & RTF_GATEWAY) {
		gw = entry->gw;
	}

	visp_get_ifname_by_index(entry->if_index, ifname, sizeof(ifname));
	ret = snprintf_s(info->msg + info->offset, info->max_size - info->offset,
			 RT_RECORD_BUFSIZE, RT_PRINT_FORMAT, ifname,
			 entry->dst, gw, flags, 0, 0, 0,
			 htonl(masklen_2_mask(entry->dst_len)), 0 /* mtu */, 0, 0);
	if (ret < 0) {
		return E_HM_INVAL;
	}

	if (info->offset > info->max_size - RT_RECORD_BUFSIZE) {
		return 1;
	}

	info->offset += ret;
	return 0;
}

int visp_get_route_info_procfs(char **msg, int *msg_len, int type)
{
	UNUSED(type);
	struct get_route_info_args args;
	int route_cnt = 0;
	int ret;

	if (msg == NULL || msg_len == NULL) {
		return E_HM_INVAL;
	}

	route_cnt = visp_get_route_entry_count();
	if (route_cnt < 0) {
		route_cnt = 0;
	}

	/* 2 additional: 1 for default gateway, 1 for route header message */
	args.max_size = ((unsigned int)route_cnt + 2)  * RT_RECORD_BUFSIZE;
	args.msg = (char *)malloc(args.max_size);
	if (args.msg == NULL) {
		return E_HM_NOMEM;
	}

	ret = snprintf_s(args.msg, args.max_size, RT_RECORD_BUFSIZE,
			"Iface\tDestination\tGateway\tFlags\tRefCnt\tUse\tMetric\tMask\tMTU\tWindow\tIRTT\n");
	if (ret < 0) {
		free(args.msg);
		return E_HM_INVAL;
	}

	args.offset = ret;
	ret = visp_for_each_route_entry(get_route_info_iterator, &args);
	if (ret < 0) {
		free(args.msg);
		return ret;
	}

	*msg = args.msg;
	*msg_len = (int)args.offset;
	return 0;
}

struct get_route_info_nl_args {
	rt_entry_t *entries;
	int max_count;

	int index;
};

static int get_route_info_nl_iter(struct visp_route_entry *entry, void *arg)
{
	struct get_route_info_nl_args *info = arg;
	rt_entry_t *rt = NULL;

	if (info->index >= info->max_count) {
		return 1;
	}

	rt = &(info->entries[info->index]);

	rt->dst = entry->dst;
	rt->dst_len = entry->dst_len;
	rt->flags = entry->flags;
	rt->gw = entry->gw;
	visp_get_ifname_by_index(entry->if_index, rt->oifname, sizeof(rt->oifname));

	info->index++;
	return 0;
}

int visp_get_route_info_nl(void **buf, size_t *len)
{
	struct get_route_info_nl_args args;
	int ret = 0;

	if (buf == NULL || len == NULL) {
		return E_HM_INVAL;
	}

	args.max_count = visp_get_route_entry_count();
	if (args.max_count < 0) {
		return E_HM_POSIX_FAULT;
	}

	args.entries = calloc(1, sizeof(rt_entry_t) * (args.max_count + 1));
	if (args.entries == NULL) {
		return E_HM_NOMEM;
	}

	args.index = 0;
	ret = visp_for_each_route_entry(get_route_info_nl_iter, &args);
	if (ret < 0) {
		free(args.entries);
		return ret;
	}

	*len = (unsigned int)(args.index * sizeof(rt_entry_t));
	*buf = args.entries;
	return ret;
}
