/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: Slist
 * Author: Huawei OS Kernel Lab
 * Create: Sun Jul 8 00:56:34 2018
 */
/* The functions in this file are FORMALLY VERIFIED. */

#ifndef KLIBS_SLIST_H
#define KLIBS_SLIST_H

#include <hmkernel/types.h>

/* node struct for singly linked circular list with head node */
struct slist_head {
	struct slist_head *next;
};

/* macro for slist init */
#define SLIST_HEAD_INIT(name) { .next = &(name) }
#define SLIST_HEAD(name) struct slist_head name = SLIST_HEAD_INIT(name)

/* initialize the slist
 * PRE: a slist_head struct
 * POST: the next pointer of the head node points to it self
 * */
static inline void
INIT_SLIST_HEAD(struct slist_head *list)
{
	list->next = list;
}

/* check if the slist is empty
 * PRE: h points to a well-formed slist
 * POST: return __true if the slist is empty, __false if not
 */
static inline __bool
slist_empty(const struct slist_head *h)
{
	return h->next == h ? __true : __false;
}

/* insert a new node into the slist, after position pointed by head
 * PRE: head points to the node before the inserting pos, new points to the new node not in the slist
 * POST: the new node has been inserted into the slist after pos pointed by head, the new slist is well-formed
 */
static inline void
slist_add(struct slist_head *another, struct slist_head *head)
{
	another->next = head->next;
	head->next = another;
}

/* delete a node from sdlist
 * PRE: node points to a node in a slist to be deleted, prev points to head node before the node to be delete
 * POST: node has been deleted, the remaining slist is well formed,
 *	 we got the memory space of the delete node as a separate struct
 */
static inline void
slist_del(struct slist_head *prev, struct slist_head *node)
{
	prev->next = node->next;
}

/* macros for slist traversal */
#define slist_entry(ptr, type, member) \
	container_of(ptr, type, member)

#define slist_first_entry(ptr, type, member) \
	slist_entry((ptr)->next, type, member)

#define slist_next_entry(pos, type, member) \
	slist_entry((pos)->member.next, type, member)

#define slist_for_each(pos, head) \
	for (pos = (head)->next; pos != head; pos = pos->next)

#define slist_for_each_entry(pos, head, type, member) \
	for (pos = slist_first_entry(head, type, member); \
	     &((pos)->member) != head; pos = slist_next_entry(pos, type, member))

#define slist_for_each_entry_with_prev(pos, prev, head, type, member)\
	for (pos = slist_first_entry(head, type, member),	\
	     prev = slist_entry(head, type, member);         \
	     &((pos)->member) != head; 			\
	     prev = pos,					\
	     pos = slist_next_entry(pos, type, member))

#define slist_for_each_entry_safe(pos, prev, np, head, type, member)\
	for (pos = slist_first_entry(head, type, member),	\
	     prev = slist_entry(head, type, member),		\
	     np = slist_entry((pos)->member.next, type, member);\
	     &((pos)->member) != head; 			\
	     prev = (slist_entry((prev)->member.next, type, member) == pos ? pos : prev), \
	     pos = np, np = slist_next_entry(np, type, member))

#define slist_del_entry(prev, node, type, member) \
	slist_del(&(prev)->member, &(node)->member)

/* struct for a singly linked circular list with head node and tail pointer */
struct tlist_head {
	struct slist_head head;
	struct slist_head *tail;
};

/* check if a node is the tail node of a slist
 * PRE: p points to a node in the slist pointed by head
 * POST: return __true if p points to the last node, return __false otherwise
 */
static inline __bool
slist_is_tail(struct slist_head *p, struct slist_head *head)
{
	return p->next == head ? __true : __false;
}

/* macro for tlist init */
#define TLIST_HEAD_INIT(name) { .head = SLIST_HEAD_INIT(name.head), .tail = &name.head }
#define TLIST_HEAD(name) struct tlist_head name = TLIST_HEAD_INIT(name)

/* initialize the tlist
 * PRE: list points to a tlist_head struct
 * POST: the next and the tail field of the tlist head both points to itself
 */
static inline void
INIT_TLIST_HEAD(struct tlist_head *list)
{
	INIT_SLIST_HEAD(&list->head);
	list->tail = &list->head;
}

/* check if the tlist is empty
 * PRE: h points to the head node of the tlist, the tlist is well-formed
 * POST: return __true if the tlist is empty, otherwise return __false
 */
static inline __bool
tlist_empty(const struct tlist_head *h)
{
	__bool ret;

	ret = slist_empty(&h->head);
	return ret;
}

/* add a new node at the first position of the tlist
 * PRE: head points to a well-formed tlist, new points to a slist node not in the tlist
 * POST: the new node has been inserted into the tlist, at the first position
 */
static inline void
tlist_add(struct slist_head *another, struct tlist_head *head)
{
	__bool ret;

	slist_add(another, &head->head);
	ret = slist_is_tail(another, &head->head);
	if (ret) {
		head->tail = another;
	}
}

/* add a new node at the last pos of the tlist
 * PRE: head points to a well-formed tlist, new points to a slist node not in the tlist
 * POST: the new node has been inserted into the tlist, at the last position
 */
static inline void
tlist_add_tail(struct slist_head *another, struct tlist_head *head)
{
	slist_add(another, head->tail);
	head->tail = another;
}

/* delete the node pointed by node from the tlist
 * PRE: head points to a well-formed tlist, node points to the node in this list to be deleted,
 *	prev points to the node before it
 * POST: the node has been delete from the tlist, memory represented as a separated structure,
 *	 the tlist is still well-formed
 */
static inline void
tlist_del(struct slist_head *prev, struct slist_head *node,
	  struct tlist_head *head)
{
	__bool ret;

	ret = slist_is_tail(node, &head->head);
	if (ret) {
		head->tail = prev;
	}
	prev->next = node->next;
}

/* macro for tlist traversal */
#define __tlist_slist_head(h) (&((h)->head))
#define tlist_first_entry(ptr, type, member) \
	slist_first_entry(__tlist_slist_head(ptr), type, member)

#define tlist_for_each(pos, head) \
	slist_for_each(pos, __tlist_slist_head(head))

#define tlist_for_each_entry(pos, head, type, member) \
	slist_for_each_entry(pos, __tlist_slist_head(head), type, member) \

#define tlist_for_each_entry_safe(pos, prev, np, head, type, member)\
	slist_for_each_entry_safe(pos, prev, np, __tlist_slist_head(head), type, member)\

#define tlist_del_entry(prev, node, head, type, member) \
	tlist_del(&(prev)->member, &(node)->member, head)

#define tlist_last_entry(ptr, type, member) \
	slist_entry((ptr)->tail, type, member)
#endif
