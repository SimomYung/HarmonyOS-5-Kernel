/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Filter table impl code
 * Author: Huawei OS Kernel Lab
 * Create: Fri Jan 29 14:22:56 2021
 */
#include <sys/socket.h>
#include "ux_socket.h"
#include "ux_iptables.h"
#include "ux_netfilter.h"
#include "ux_filter_table.h"

#define ENTRIES_NUM 3
#define HOOK_NUM 5
#define ERROR_ENTRY "ERROR"
#define FILTER_TABLE_NAME "filter"

#define FILTER_VALID_HOOKS ((1 << UX_NF_LOCAL_IN) | \
			    (1 << UX_NF_FORWARD) | \
			    (1 << UX_NF_LOCAL_OUT))

int ux_ipt_hook(unsigned int hook,
		struct ux_pktbuf *pkt,
		struct ux_netdev *indev,
		struct ux_netdev *outdev)
{
	int ret;
	struct ux_ipt_netdev netdev;

	netdev.indev = indev;
	netdev.outdev = outdev;

	ret = ux_ipt_do_table(hook, pkt, netdev, "filter");

	return ret;
}

static struct ux_nf_hook_ops ipt_ops[] = {
	{
		.hook		= ux_ipt_hook,
		.pf		= AF_INET,
		.hooknum	= UX_NF_LOCAL_IN,
		.priority	= 0,
	},
	{
		.hook		= ux_ipt_hook,
		.pf		= AF_INET,
		.hooknum	= UX_NF_FORWARD,
		.priority	= 0,
	},
	{
		.hook		= ux_ipt_hook,
		.pf		= AF_INET,
		.hooknum	= UX_NF_LOCAL_OUT,
		.priority	= 0,
	},
};

static void init_hook_offset(unsigned int *entry)
{
	int buf[HOOK_NUM] = {
		0xffffffff,
		0,
		sizeof(struct ux_ipt_standard),
		sizeof(struct ux_ipt_standard) + sizeof(struct ux_ipt_standard),
		0xffffffff
	};

	for (int i = 0; i < HOOK_NUM; i++) {
		entry[i] = buf[i];
	}

	return;
}

static int init_filter(struct ux_ipt_table *filter_table)
{
	struct ux_ipt_standard *entry_start;
	struct ux_ipt_error *entry_term;
	if (strcpy_s(filter_table->name,
		     sizeof(filter_table->name),
		     FILTER_TABLE_NAME)) {
		return E_HM_POSIX_FAULT;
	}
	filter_table->valid_hooks = FILTER_VALID_HOOKS;
	filter_table->num_entries = ENTRIES_NUM + 1;
	filter_table->size = sizeof(struct ux_ipt_standard) *
				    ENTRIES_NUM +
				    sizeof(struct ux_ipt_error);
	init_hook_offset(&filter_table->hook_entry[0]);
	init_hook_offset(&filter_table->underflow[0]);

	entry_start = (struct ux_ipt_standard*)malloc(
		       sizeof(struct ux_ipt_standard) *
		       ENTRIES_NUM + sizeof(struct ux_ipt_error));
	if (entry_start == NULL) {
		return E_HM_NOMEM;
	}

	for (int i = 0; i < ENTRIES_NUM; i++) {
		entry_start[i].entry.target_offset = sizeof(struct ux_ipt_entry);
		entry_start[i].entry.next_offset = sizeof(struct ux_ipt_standard);
		entry_start[i].target.target.u.user.target_size = ux_ipt_align(sizeof(struct ux_ipt_standard_target));
		entry_start[i].target.verdict = -UX_NF_ACCEPT - 1;
	}

	entry_term = (struct ux_ipt_error *)((char *)entry_start +
					     sizeof(struct ux_ipt_standard) *
					     ENTRIES_NUM);
	entry_term->entry.target_offset = sizeof(struct ux_ipt_entry);
	entry_term->entry.next_offset = sizeof(struct ux_ipt_error);
	entry_term->target.target.u.user.target_size = ux_ipt_align(sizeof(struct ux_ipt_error_target));
	if (strcpy_s(entry_term->target.target.u.user.name,
		     sizeof(entry_term->target.target.u.user.name),
		     ERROR_ENTRY)) {
		free(entry_start);
		return E_HM_POSIX_FAULT;
	}
	if (strcpy_s(entry_term->target.errorname,
		     sizeof(entry_term->target.errorname),
		     ERROR_ENTRY)) {
		free(entry_start);
		return E_HM_POSIX_FAULT;
	}

	filter_table->entries = entry_start;
	return 0;
}

int init_filter_table(void)
{
	struct ux_ipt_table *filter_table;
	int ret = 0;

	filter_table = (struct ux_ipt_table *)malloc(sizeof(struct ux_ipt_table));
	if (filter_table == NULL) {
		return E_HM_NOMEM;
	}

	ret = init_filter(filter_table);
	if (ret) {
		free(filter_table);
		return ret;
	}
	ipt_add_table(&filter_table->list);
	for (int i = 0; i < ENTRIES_NUM; i++) {
		ret = ux_nf_register_hook(&ipt_ops[i]);
		if (ret < 0) {
			return ret;
		}
	}

	return 0;
}
