/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 20 20:17:25 2021
 */

#ifndef _NSS_NETGROUP_H
#define _NSS_NETGROUP_H

#define		_PATH_NETGROUP		"/etc/netgroup"
#define		ENTRY_STR_LEN		256

typedef enum net_group_type {
	TRIPLE,
	GROUP
} net_group_type;

typedef struct net_group_entry {
	net_group_type type;
	union {
		struct {
			char host[ENTRY_STR_LEN];
			char name[ENTRY_STR_LEN];
			char domain[ENTRY_STR_LEN];
		} triple;
		char group[ENTRY_STR_LEN];
	};

	struct net_group_entry *next;
} net_group_entry;

typedef struct net_group {
	struct net_group *next;
	struct net_group_entry *entry;
	struct net_group_entry *last;
	char name[0];
} net_group;

typedef struct net_groups {
	struct net_group *group;
	struct net_group *last;
} net_groups;

struct net_groups *net_group_parse_file(void);

#endif
