/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Dlist
 * Author: Huawei OS Kernel Lab
 * Create: Sun Jul 8 00:56:34 2018
 */
/* The functions in this file are FORMALLY VERIFIED. */

#ifndef KLIBS_DLIST_H
#define KLIBS_DLIST_H

#include <hmkernel/types.h>
#include <hmkernel/compiler.h>
#include <hmasm/panic_trap_lib.h>
#include <lib/utils.h>

/* The dlist node structure
 * representing the head node and the body nodes of the dlist
 */
struct dlist_node {
	struct dlist_node *prev;
	struct dlist_node *next;
};

#define DLIST_HEAD(name) struct dlist_node name = { &(name), &(name) }
#define DLIST_HEAD_WRITABLE(name) struct dlist_node name __rw_after_init = { &(name), &(name) }
#define DLIST_HEAD_INIT(name) { &(name), &(name) }

#ifdef DLIST_DEBUG_ENABLE
extern void __cold __dlist_bug(const char *fmt, ...);
#define __dlist_dbg_check(condition, fmt, ...)		\
do {							\
	if (condition) {				\
		arch_panic_trap_preproc();              \
		__dlist_bug(fmt, ##__VA_ARGS__);	\
	}						\
} while (__false)

static void
__dlist_insert_valid(struct dlist_node *prev, struct dlist_node *next)
{
	__dlist_dbg_check(next->prev != prev,
			  "dlist_insert corruption: node(%px)->prev should be %px, but was %px\n",
			  next, prev, next->prev);
}

static void
__dlist_delete_valid(struct dlist_node *node)
{
	struct dlist_node *prev = node->prev;
	struct dlist_node *next = node->next;

	__dlist_dbg_check(prev->next != node,
			  "dlist_delete corruption, prev was %px, prev->next should be %px, but was %px\n",
			  prev, node, prev->next);

	__dlist_dbg_check(next->prev != node,
			  "dlist_delete corruption, next was %px, next->prev should be %px, but was %px\n",
			  next, node, next->prev);
}
#else
static inline
void __dlist_insert_valid(struct dlist_node *prev, struct dlist_node *next)
{
	/*
	 * UNUSED macro is defined by different header files
	 * in kernel mode and user mode, we cannot directly used UNUSED
	 */
	(void)prev;
	(void)next;
}

static inline
void __dlist_delete_valid(struct dlist_node *node)
{
	/*
	 * UNUSED macro is defined by different header files
	 * in kernel mode and user mode, we cannot directly used UNUSED
	 */
	(void)node;
}
#endif

/* Initialize the empty dlist
 * PRE: a dlist_node struct for the head node, with unspecified field values
 * POST: the field set to point to the head node itself, thus initialized to be an empty dlist
 */
static inline
void dlist_init(struct dlist_node *head)
{
	head->prev = head;
	head->next = head;
	return;
}

/* Check if the dlist is empty
 * PRE: head points to the head node of a well formed dlist
 * POST: return __true if the dlist is empty, return __false if it is not
 */
static inline
__bool dlist_empty(const struct dlist_node *head)
{
	/* dlist is well formed, so only needs check the next ptr here */
	return head->next == head ? __true : __false;
}

/* Get the first node of a dlist
 * PRE: head points to the head node of a well formed dlist
 * POST: return the pointer to the first node of the dlist if it's not empty, or to the head node if it's empty
 */
static inline
struct dlist_node *dlist_get_first(struct dlist_node *head)
{
	return head->next;
}

/* Get the last node of a dlist
 * PRE: head points to the head node of a well formed dlist
 * POST: return the pointer to the last node of the dlist if it's not empty, or to the head node if it's empty
 */
static inline
struct dlist_node *dlist_get_last(struct dlist_node *head)
{
	return head->prev;
}

/* Insert after a given position of the dlist
 * PRE: pos points to a node(can be the head node) in a well formed dlist,
 *	node points to a node to be inserted(not in the dlist)
 * POST: node has been inserted into the dlist after pos, the new dlist is well formed
 */
static inline
void dlist_insert(struct dlist_node *pos, struct dlist_node *node)
{
	struct dlist_node *tmp;

	tmp = pos->next;

	/* check data corruption */
	__dlist_insert_valid(pos, tmp);

	tmp->prev = node;
	node->prev = pos;
	node->next = pos->next;
	pos->next = node;
	return;
}

/* Insert a new node at head of a dlist
 * PRE: head points to the head node of a well formed dlist, node points to the node to be inserted(not in the dlist)
 * POST: the new node has been inserted to the head of the dlist, the new dlist is well formed
 */
static inline
void dlist_insert_head(struct dlist_node *head, struct dlist_node *node)
{
	dlist_insert(head, node);
	return;
}

/* Insert a new node at tail of a dlist
 * PRE: head points to the head node of a well formed dlist, node points to the node to be inserted(not in the dlist)
 * POST: the new node has been inserted to the tail of the dlist, the new dlist is well formed
 */
static inline
void dlist_insert_tail(struct dlist_node *head, struct dlist_node *node)
{
	struct dlist_node *tmp;

	tmp = dlist_get_last(head);
	dlist_insert(tmp, node);
	return;
}

/* Delete a node from a dlist
 * PRE: node points to a node in a well formed dlist
 * POST: node has been taken out of the dlist, the remaining dlist is still well formed
 */
static inline
void dlist_delete(struct dlist_node *node)
{
	struct dlist_node *tmp;

	/* check data corruption */
	__dlist_delete_valid(node);

	tmp = node->prev;
	tmp->next = node->next;
	tmp = node->next;
	tmp->prev = node->prev;
	dlist_init(node);
	return;
}

/* Replace an old node in the dlist with a new node
 * PRE: old node points to a node in the dlist, new node points a node not in the dlist, dlist well formed
 * POST: the new node has been inserted into the dlist, the old node has been taken out, the dlist is still well formed
 */
static inline
void dlist_replace(struct dlist_node *old_node, struct dlist_node *new_node)
{
	struct dlist_node *tmp;

	new_node->prev = old_node->prev;
	new_node->next = old_node->next;

	tmp = old_node->prev;
	tmp->next = new_node;

	tmp = old_node->next;
	tmp->prev = new_node;

	dlist_init(old_node);
}

/* Get the prev node of a dlist node or a dlist head
 * PRE: node points to a dlist head or a dlist node of a well formed dlist
 * POST: return the pointer to the prev node of the dlist node or the dlist head
 */
static inline
struct dlist_node *dlist_get_prev(struct dlist_node *node)
{
	return node->prev;
}

static inline
__bool dlist_has_single_entry(const struct dlist_node *head)
{
	return ((!dlist_empty(head)) && (head->next == head->prev));
}

/* Get the next node of a dlist node or a dlist head
 * PRE: node points to a dlist head or a dlist node of a well formed dlist
 * POST: return the pointer to the next node of the dlist node or the dlist head
 */
static inline
struct dlist_node *dlist_get_next(struct dlist_node *node)
{
	return node->next;
}

static inline
void dlist_merge(struct dlist_node *l1_node, struct dlist_node *l2_head)
{
	struct dlist_node *l1_next;
	struct dlist_node *l2_first;
	struct dlist_node *l2_last;

	l1_next = l1_node->next;
	l2_first = dlist_get_first(l2_head);
	l2_last = dlist_get_last(l2_head);

	l1_node->next = l2_first;
	l2_first->prev = l1_node;
	l2_last->next = l1_next;
	l1_next->prev = l2_last;

	dlist_init(l2_head);
}

static inline
void dlist_replace_merge(struct dlist_node *l1_node, struct dlist_node *l2_head)
{
	struct dlist_node *l1_prev;
	struct dlist_node *l1_next;
	struct dlist_node *l2_first;
	struct dlist_node *l2_last;

	__dlist_delete_valid(l1_node);

	l1_prev = l1_node->prev;
	l1_next = l1_node->next;
	l2_first = dlist_get_first(l2_head);
	l2_last = dlist_get_last(l2_head);

	l1_prev->next = l2_first;
	l2_first->prev = l1_prev;
	l2_last->next = l1_next;
	l1_next->prev = l2_last;

	dlist_init(l1_node);
	dlist_init(l2_head);
}

/* get the address of the containing struct */
#define dlist_entry(ptr, type, member) \
	container_of(ptr, type, member)

/* dlist_first_entry */
#define dlist_first_entry(ptr, type, member) \
	dlist_entry((ptr)->next, type, member)

/* dlist_last_entry */
#define dlist_last_entry(ptr, type, member) \
	dlist_entry((ptr)->prev, type, member)

/**
 * dlist_first_entry_or_null - get the first entry from a list
 * if the dlist is empty, it returns NULL.
 */
#define dlist_first_entry_or_null(ptr, type, member) ({ \
	struct dlist_node *head_ = (ptr); \
	struct dlist_node *pos_ = dlist_get_first(head_); \
	pos_ != head_ ? dlist_entry(pos_, type, member) : NULL; \
})

/* get the address of the next containing struct on the dlist */
#define dlist_next_entry(pos, type, member) \
	dlist_entry((pos)->member.next, type, member)

/* get the address of the previous containing struct on the dlist */
#define dlist_prev_entry(pos, type, member) \
	dlist_entry((pos)->member.prev, type, member)

/* dlist for each node entry */
#define dlist_for_each(pos, head) \
	for ((pos) = (head)->next; \
	     (const struct dlist_node *)(pos) != (const struct dlist_node *)(head); \
	     (pos) = (pos)->next)

#define dlist_for_each_safe(pos, n, head) \
	for ((pos) = (head)->next, (n) = (pos)->next; \
	     (const struct dlist_node *)(pos) != (const struct dlist_node *)(head); \
	     (pos) = (n), (n) = (pos)->next)

/* dlist for each struct entry */
#define dlist_for_each_entry(pos, head, type, member) \
	for ((pos) = dlist_first_entry(head, type, member); \
	     (const struct dlist_node *)&((pos)->member) != (const struct dlist_node *)(head); \
	     (pos) = dlist_next_entry(pos, type, member))

#define dlist_for_each_entry_safe(pos, n, head, type, member) 	\
	for ((pos) = dlist_first_entry(head, type, member),	\
	     (n) = dlist_next_entry(pos, type, member);	\
	     ((const struct dlist_node *)&(pos)->member != (const struct dlist_node *)(head)); \
	     (pos) = (n), (n) = dlist_next_entry(n, type, member))

#define dlist_for_each_entry_reverse(pos, head, type, member) \
	for ((pos) = dlist_last_entry(head, type, member); \
	     (const struct dlist_node *)&((pos)->member) != (const struct dlist_node *)(head); \
	     (pos) = dlist_prev_entry(pos, type, member))

#define dlist_for_each_entry_reverse_safe(pos, n, head, type, member) \
	for ((pos) = dlist_last_entry(head, type, member), \
	     (n) = dlist_prev_entry(pos, type, member); \
	     (const struct dlist_node *)&((pos)->member) != (const struct dlist_node *)(head); \
	     (pos) = (n), (n) = dlist_prev_entry(n, type, member))

#define dlist_for_each_entry_from(pos, head, type, member) \
	for (; \
	     (const struct dlist_node *)&((pos)->member) != (const struct dlist_node *)(head); \
	     (pos) = dlist_next_entry(pos, type, member))

#define dlist_for_each_entry_from_safe(pos, n, head, type, member) \
	for ((n) = dlist_next_entry(pos, type, member); \
	     (const struct dlist_node *)&((pos)->member) != (const struct dlist_node *)(head); \
	     (pos) = (n), (n) = dlist_next_entry(n, type, member))

#define dlist_for_each_entry_continue(pos, head, type, member) \
	for ((pos) = dlist_next_entry(pos, type, member); \
	     (const struct dlist_node *)&((pos)->member) != (const struct dlist_node *)(head); \
	     (pos) = dlist_next_entry(pos, type, member))

#define dlist_for_each_entry_continue_safe(pos, n, head, type, member) \
	for ((pos) = dlist_next_entry(pos, type, member), \
	     (n) = dlist_next_entry(pos, type, member); \
	     (const struct dlist_node *)&((pos)->member) != (const struct dlist_node *)(head); \
	     (pos) = (n), (n) = dlist_next_entry(n, type, member))

#define dlist_for_each_entry_reverse_continue(pos, head, type, member) \
	for ((pos) = dlist_prev_entry(pos, type, member); \
	     (const struct dlist_node *)&((pos)->member) != (const struct dlist_node *)(head); \
	     (pos) = dlist_prev_entry(pos, type, member))

#define dlist_for_each_entry_reverse_continue_safe(pos, n, head, type, member) \
	for ((pos) = dlist_prev_entry(pos, type, member), \
	     (n) = dlist_prev_entry(pos, type, member); \
	     (const struct dlist_node *)&((pos)->member) != (const struct dlist_node *)(head); \
	     (pos) = (n), (n) = dlist_prev_entry(n, type, member))

/* delete a struct entry */
#define dlist_del_entry(node, type, member) \
	dlist_delete(&(node)->member)

/* replace a struct entry */
#define dlist_replace_entry(old_node, new_node, type, member) \
	dlist_replace(&(old_node)->member, &(new_node)->member)

#endif
