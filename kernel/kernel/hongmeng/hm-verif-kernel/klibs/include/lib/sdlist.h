/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: The sdlist structure and basic functions
 * Author: Huawei OS Kernel Lab
 * Create: Sun Jul 8 00:56:34 2018
 */
#ifndef KLIBS_SDLIST_H
#define KLIBS_SDLIST_H

#include <hmkernel/types.h>
#include <lib/utils.h>
#include <lib/dlist.h>

/* The sdlist node structure
 * representing the head node and the body nodes of the dlist
 */
struct sdlist_node {
	int key;
	struct dlist_node dnode;
};

#define SDLIST_HEAD(name) struct sdlist_node name = { .key = -1, .dnode = DLIST_HEAD_INIT((name).dnode) }

static inline
void sdlist_init(struct sdlist_node *head)
{
	head->key = -1;
	dlist_init(&head->dnode);
	return;
}

static inline void sdlist_node_init(struct sdlist_node *node, int key)
{
	node->key = key;
	dlist_init(&node->dnode);
}

static inline
__bool sdlist_empty(struct sdlist_node *head)
{
	return dlist_empty(&head->dnode);
}

static inline
struct sdlist_node *sdlist_get_first(struct sdlist_node *head)
{
	return container_of(dlist_get_first(&head->dnode), struct sdlist_node, dnode);
}

static inline
struct sdlist_node *sdlist_get_last(struct sdlist_node *head)
{
	return container_of(dlist_get_last(&head->dnode), struct sdlist_node, dnode);
}

static inline void sdlist_node_setkey(struct sdlist_node *node, int key)
{
	node->key = key;
}

static inline
struct sdlist_node *sdlist_get_prev(struct sdlist_node *node)
{
	return container_of(node->dnode.prev, struct sdlist_node, dnode);
}

static inline
struct sdlist_node *sdlist_get_next(struct sdlist_node *node)
{
	return container_of(node->dnode.next, struct sdlist_node, dnode);
}

static inline
void sdlist_insert(struct sdlist_node *head, struct sdlist_node *node)
{
	struct sdlist_node *tmp = head;
	do {
		tmp = sdlist_get_next(tmp);
	} while ((tmp->key <= node->key) && (tmp != head));

	/* (1) tmp == head: all node's key <= node->key, insert to the tail;
	 * (2) tmp != head: tmp->key > node->key, insert just before the tmp */
	dlist_insert(tmp->dnode.prev, &(node->dnode));
}

static inline
void sdlist_delete(struct sdlist_node *node)
{
	dlist_delete(&node->dnode);
	return;
}

static inline
struct sdlist_node *sdlist_get_node(struct sdlist_node *head, int key)
{
	struct sdlist_node *tmp = head;
	do {
		tmp = sdlist_get_next(tmp);
	} while ((tmp->key != key) && (tmp != head));

	return (tmp == head) ?  NULL : tmp;
}

static inline
struct sdlist_node *sdlist_pop_node(struct sdlist_node *head, int key)
{
	struct sdlist_node *tmp = NULL;

	tmp = sdlist_get_node(head, key);
	if (tmp != NULL) {
		sdlist_delete(tmp);
	}

	return tmp;
}
#endif /* !SDLIST_H */
