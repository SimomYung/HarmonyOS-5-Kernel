/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Memory Control
 * Author: Huawei OS Kernel Lab
 * Create: Fri Aug 14 11:58:14 2020
 */
#ifndef SYSMGR_MEM_CONTROL_H
#define SYSMGR_MEM_CONTROL_H

#include <libhwsecurec/securec.h>
#include <lib/dlist.h>

#define EVENT_CONTROL_MAX_SIZE	20

struct event {
	int efd;
	int ofd;
	__u32 cnode_idx;
	struct dlist_node dnode;
};

struct memcg_s;

int mcgroup_register_event(char *buf, size_t size);
void mcgroup_unregister_event(uint32_t cnode_idx);
struct dlist_node *memctrl_event_list_of(void);

#endif
