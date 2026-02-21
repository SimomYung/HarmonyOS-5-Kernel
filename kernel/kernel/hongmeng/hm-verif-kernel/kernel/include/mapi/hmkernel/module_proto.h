/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2019-2020. All rights reserved.
 * Description: Declaration of kernel module
 * Author: Huawei OS Kernel Lab
 * Create: Thu Oct 10 09:56:05 2019
 */
#ifndef MODULE_PROTO_H
#define MODULE_PROTO_H

#include <lib/dlist.h>

#include <hmkernel/attributes/hmattributes.h>

#define RBTREE_NODE_RESERVED_ULONG_NR	3u

struct timer_protodata_s {
	union {
#ifdef CONFIG_MODULE_TIMER_RBTREE
		unsigned long rbtree_node[RBTREE_NODE_RESERVED_ULONG_NR] __kit_shape_rbtree_node(timer);
#endif
		struct dlist_node list;
	} u;
};

#endif
