/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Description: pac interfaces for double linked list
 * Author: Huawei OS Kernel Lab
 * Create: Oct 31 2023
 */
#ifndef KLIBS_DLIST_SEC_H
#define KLIBS_DLIST_SEC_H

#include <lib/dlist_ext.h>

#ifdef PAC_DFI_STRUCT_DLIST
struct dlist_sec_node {
	__attribute__((pac_protected_ptr)) struct dlist_sec_node *prev;
	__attribute__((pac_protected_ptr)) struct dlist_sec_node *next;
};

typedef struct dlist_sec_node dlist_sec_node_t;
#else
typedef struct dlist_node dlist_sec_node_t;
#endif

#define DLIST_SEC_HEAD(name) dlist_sec_node_t name = { &(name), &(name) }
#define DLIST_SEC_HEAD_INIT(name) { &(name), &(name) }

#ifdef DLIST_DEBUG_ENABLE
static void
__dlist_sec_insert_valid(dlist_sec_node_t *prev, dlist_sec_node_t *next)
{
	__dlist_dbg_check(next->prev != prev,
			  "dlist_insert corruption: node(%p)->prev should be %p, but was %p\n",
			  next, prev, next->prev);
}

static void
__dlist_sec_delete_valid(dlist_sec_node_t *node)
{
	dlist_sec_node_t *prev = node->prev;
	dlist_sec_node_t *next = node->next;

	__dlist_dbg_check(prev->next != node,
			  "dlist_delete corruption, prev was %p, prev->next should be %p, but was %p\n",
			  prev, node, prev->next);

	__dlist_dbg_check(next->prev != node,
			  "dlist_delete corruption, next was %p, next->prev should be %p, but was %p\n",
			  next, node, next->prev);
}
#else
static inline
void __dlist_sec_insert_valid(dlist_sec_node_t *prev, dlist_sec_node_t *next)
{
	(void)prev;
	(void)next;
}

static inline
void __dlist_sec_delete_valid(dlist_sec_node_t *node)
{
	(void)node;
}
#endif

static inline
void dlist_sec_init(dlist_sec_node_t *head)
{
	head->prev = head;
	head->next = head;
	return;
}

static inline
__bool dlist_sec_empty(const dlist_sec_node_t *head)
{
	/* dlist is well formed, so only needs check the next ptr here */
	return head->next == head ? __true : __false;
}

static inline
dlist_sec_node_t *dlist_sec_get_first(dlist_sec_node_t *head)
{
	return head->next;
}

static inline
dlist_sec_node_t *dlist_sec_get_last(dlist_sec_node_t *head)
{
	return head->prev;
}

static inline
void dlist_sec_insert(dlist_sec_node_t *pos, dlist_sec_node_t *node)
{
	dlist_sec_node_t *tmp;

	tmp = pos->next;

	/* check data corruption */
	__dlist_sec_insert_valid(pos, tmp);

	tmp->prev = node;
	node->prev = pos;
	node->next = pos->next;
	pos->next = node;
	return;
}

static inline
void dlist_sec_insert_head(dlist_sec_node_t *head, dlist_sec_node_t *node)
{
	dlist_sec_insert(head, node);
	return;
}

static inline
void dlist_sec_insert_tail(dlist_sec_node_t *head, dlist_sec_node_t *node)
{
	dlist_sec_node_t *tmp;

	tmp = dlist_sec_get_last(head);
	dlist_sec_insert(tmp, node);
	return;
}

static inline
void dlist_sec_delete(dlist_sec_node_t *node)
{
	dlist_sec_node_t *tmp;

	/* check data corruption */
	__dlist_sec_delete_valid(node);

	tmp = node->prev;
	tmp->next = node->next;
	tmp = node->next;
	tmp->prev = node->prev;
	dlist_sec_init(node);
	return;
}

static inline
void dlist_sec_replace(dlist_sec_node_t *old_node, dlist_sec_node_t *new_node)
{
	dlist_sec_node_t *tmp;

	new_node->prev = old_node->prev;
	new_node->next = old_node->next;

	tmp = old_node->prev;
	tmp->next = new_node;

	tmp = old_node->next;
	tmp->prev = new_node;

	dlist_sec_init(old_node);
}

static inline
dlist_sec_node_t *dlist_sec_get_prev(dlist_sec_node_t *node)
{
	return node->prev;
}

static inline
dlist_sec_node_t *dlist_sec_get_next(dlist_sec_node_t *node)
{
	return node->next;
}

#define dlist_sec_entry(ptr, type, member) \
	container_of(ptr, type, member)

#define dlist_sec_first_entry(ptr, type, member) \
	dlist_sec_entry((type *)((ptr)->next), type, member)

#define dlist_sec_last_entry(ptr, type, member) \
	dlist_sec_entry((type *)((ptr)->prev), type, member)

#define dlist_sec_next_entry(pos, type, member) \
	dlist_sec_entry((type *)((pos)->member.next), type, member)

#define dlist_sec_prev_entry(pos, type, member) \
	dlist_sec_entry((type *)((pos)->member.prev), type, member)

#define dlist_sec_for_each(pos, head) \
	for ((pos) = (head)->next; \
		 (const dlist_sec_node_t *)(pos) != (const dlist_sec_node_t *)(head); \
		 (pos) = (pos)->next)

#define dlist_sec_for_each_safe(pos, n, head) \
	for ((pos) = (head)->next, (n) = (pos)->next; \
		 (const dlist_sec_node_t *)(pos) != (const dlist_sec_node_t *)(head); \
		 (pos) = (n), (n) = (pos)->next)

#define dlist_sec_for_each_entry(pos, head, type, member) \
	for ((pos) = dlist_sec_first_entry(head, type, member); \
		 (const dlist_sec_node_t *)&((pos)->member) != (const dlist_sec_node_t *)(head); \
		 (pos) = dlist_sec_next_entry(pos, type, member))

#define dlist_sec_for_each_entry_safe(pos, n, head, type, member)   \
	for ((pos) = dlist_sec_first_entry(head, type, member),     \
		 (n) = dlist_sec_next_entry(pos, type, member); \
		 ((const dlist_sec_node_t *)&(pos)->member != (const dlist_sec_node_t *)(head)); \
		 (pos) = (n), (n) = dlist_sec_next_entry(n, type, member))

#define dlist_sec_for_each_entry_reverse(pos, head, type, member) \
	for ((pos) = dlist_sec_last_entry(head, type, member); \
		 (const dlist_sec_node_t *)&((pos)->member) != (const dlist_sec_node_t *)(head); \
		 (pos) = dlist_sec_prev_entry(pos, type, member))

#define dlist_sec_for_each_entry_continue(pos, head, type, member) \
	for ((pos) = dlist_sec_next_entry(pos, type, member); \
		 (const dlist_sec_node_t *)&((pos)->member) != (const dlist_sec_node_t *)(head); \
		 (pos) = dlist_sec_next_entry(pos, type, member))

#define dlist_sec_for_each_entry_reverse_continue(pos, head, type, member) \
	for ((pos) = dlist_sec_prev_entry(pos, type, member); \
		 (const dlist_sec_node_t *)&((pos)->member) != (const dlist_sec_node_t *)(head); \
		 (pos) = dlist_sec_prev_entry(pos, type, member))

#define dlist_sec_del_entry(node, type, member) \
	dlist_sec_delete(&(node)->member)

#define dlist_sec_replace_entry(old_node, new_node, type, member) \
	dlist_sec_replace(&(old_node)->member, &(new_node)->member)

static inline
void dlist_sec_transfer(dlist_sec_node_t *node, dlist_sec_node_t *head)
{
	dlist_sec_delete(node);
	dlist_sec_insert_head(head, node);
}

static inline
void dlist_sec_transfer_tail(dlist_sec_node_t *node, dlist_sec_node_t *head)
{
	dlist_sec_delete(node);
	dlist_sec_insert_tail(head, node);
}

static inline
dlist_sec_node_t *dlist_sec_pop_first(dlist_sec_node_t *head)
{
	dlist_sec_node_t *_node;
	dlist_sec_node_t *p_ret = NULL;

	_node = dlist_sec_get_first(head);
	if (head != _node) {
		dlist_sec_delete(_node);
		p_ret = _node;
	}

	return p_ret;
}

#endif
