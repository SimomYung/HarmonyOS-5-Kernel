/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Extra interfaces for double linked list
 * Author: Huawei OS Kernel Lab
 * Create: Wed Aug 1 19:25:44 2018
 */
#ifndef KLIBS_DLIST_EXT_H
#define KLIBS_DLIST_EXT_H

#include <lib/dlist.h>

/* Transfer a node from one list to another
 * PRE: node is in list A, head is the head of list B
 * POST: remove node from list A, insert node into list B at head
 */
static inline
void dlist_transfer(struct dlist_node *node, struct dlist_node *head)
{
	dlist_delete(node);
	dlist_insert_head(head, node);
}

/* Transfer a node from one list to another
 * PRE: node is in list A, head is the head of list B
 * POST: remove node from list A, insert node into list B at tail
 */
static inline
void dlist_transfer_tail(struct dlist_node *node, struct dlist_node *head)
{
	dlist_delete(node);
	dlist_insert_tail(head, node);
}

/* Pop the head node of the list
 * PRE: head != NULL
 * POST: remove the first node of the list and return (NULL if empty)
 */
static inline
struct dlist_node *
dlist_pop_first(struct dlist_node *head)
{
	struct dlist_node *_node;
	struct dlist_node *p_ret = NULL;

	_node = dlist_get_first(head);

	if (head != _node) {
		dlist_delete(_node);
		p_ret = _node;
	}

	return p_ret;
}

#endif
