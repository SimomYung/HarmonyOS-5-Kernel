/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Implementaion of ebr-safe dlist
 * Author: Huawei OS Kernel Lab
 * Create: Mon Agu 24 14:05:30 2020
 */

#ifndef ULIBS_LIBHMEBR_EBR_DLIST_H
#define ULIBS_LIBHMEBR_EBR_DLIST_H

#include <lib/dlist.h>
#include <libhmebr/ebr.h>

static inline void ebr_dlist_init(struct dlist_node *head)
{
	__write_once(head->prev, head);
	__write_once(head->next, head);
}

static inline int ebr_dlist_empty(const struct dlist_node *head)
{
	return ebr_dereference(head->next) == head ? 1 : 0;
}

static inline struct dlist_node *ebr_dlist_get_first(struct dlist_node *head)
{
	return ebr_dereference(head->next);
}

static inline struct dlist_node *ebr_dlist_get_last(struct dlist_node *head)
{
	return ebr_dereference(head->prev);
}

static inline
void ebr_dlist_insert(struct dlist_node *pos, struct dlist_node *node)
{
	struct dlist_node *tmp = NULL;

	tmp = pos->next;
	node->prev = pos;
	node->next = pos->next;
	ebr_assign_pointer(pos->next, node);
	tmp->prev = node;
}

static inline
void ebr_dlist_insert_head(struct dlist_node *head, struct dlist_node *node)
{
	ebr_dlist_insert(head, node);
}

static inline
void ebr_dlist_insert_tail(struct dlist_node *head, struct dlist_node *node)
{
	struct dlist_node *tmp = NULL;

	tmp = ebr_dlist_get_last(head);
	ebr_dlist_insert(tmp, node);
}

static inline void ebr_dlist_delete(struct dlist_node *node)
{
	node->next->prev = node->prev;
	__write_once(node->prev->next, node->next);
}

static inline
void ebr_dlist_replace(struct dlist_node *old_node, struct dlist_node *new_node)
{
	struct dlist_node *tmp = NULL;

	new_node->prev = old_node->prev;
	new_node->next = old_node->next;
	tmp = old_node->prev;
	ebr_assign_pointer(tmp->next, new_node);
	tmp = old_node->next;
	tmp->prev = new_node;
}

static inline struct dlist_node *ebr_dlist_get_next(struct dlist_node *node)
{
	return ebr_dereference(node->next);
}

/* get the address of the containing struct */
#define ebr_dlist_entry(ptr, type, member) \
	container_of(ebr_dereference(ptr), type, member)

/* ebr_dlist_first_entry */
#define ebr_dlist_first_entry(ptr, type, member) \
	ebr_dlist_entry((ptr)->next, type, member)

/* ebr_dlist_last_entry */
#define ebr_dlist_last_entry(ptr, type, member) \
	ebr_dlist_entry((ptr)->prev, type, member)

/* get the address of the next containing struct on the dlist */
#define ebr_dlist_next_entry(pos, type, member) \
	ebr_dlist_entry((pos)->member.next, type, member)

/* get the address of the previous containing struct on the dlist */
#define ebr_dlist_prev_entry(pos, type, member) \
	ebr_dlist_entry((pos)->member.prev, type, member)

/*
 * This function is used to replace the combination usage of ebr_dlist_empty() and
 * ebr_dlist_first_entry(). For example:
 * if (ebr_dlist_empty(mylist)) {
 * 	ebr_dlist_first_entry(mylist, struct foo, list_member);
 * }
 * After check the ebr_dlist_empty, the list may be empty when do the ebr_dlist_first_entry,
 * in this case, ebr_dlist_first_entry may return unintended struct pointer.
 *
 * ebr_dlist_first_entry_or_null - get the first element from a list
 * @ptr: the list head to take the element from.
 * Note that it returns NULL if the list is empty.
 */
#define ebr_dlist_first_entry_or_null(ptr, type, member) 			\
({ 										\
	struct dlist_node *_next = ebr_dereference((ptr)->next); 		\
	likely(_next != (ptr)) ? ebr_dlist_entry(_next, type, member) : NULL;	\
})

/*
 * ebr_dlist_next_entry_or_null - get the next element from the ptr in a list
 * @head: the head for the list.
 * @ptr:  the list head to take the next element from.
 * Note that it returns NULL if the ptr is at the end of the list.
 */
#define ebr_dlist_next_entry_or_null(head, ptr, type, member) 			\
({ 										\
	struct dlist_node *_next = ebr_dereference((ptr)->next); 		\
	likely(_next != (head)) ? ebr_dlist_entry(_next, type, member) : NULL;	\
})

/* ebr_dlist for each node entry */
#define ebr_dlist_for_each(pos, head)					\
	for ((pos) = ebr_dereference((head)->next); (pos) != (head);	\
	     (pos) = ebr_dereference((pos)->next))

/* ebr_dlist for each struct entry */
#define ebr_dlist_for_each_entry(pos, head, type, member)	\
	for ((pos) = ebr_dlist_first_entry(head, type, member);	\
	     &((pos)->member) != (head);			\
	     (pos) = ebr_dlist_next_entry(pos, type, member))

#define ebr_dlist_for_each_entry_safe(pos, n, head, type, member)	\
	for ((pos) = ebr_dlist_first_entry(head, type, member),		\
	     (n) = ebr_dlist_next_entry(pos, type, member);		\
	     (&(pos)->member != (head));				\
	     (pos) = (n), (n) = ebr_dlist_next_entry(n, type, member))

#define ebr_dlist_for_each_entry_continue(pos, head, type, member)	\
	for ((pos) = ebr_dlist_next_entry(pos, type, member);		\
	     &((pos)->member) != (head); (pos) = ebr_dlist_next_entry(pos, type, member))

/* delete a struct entry */
#define ebr_dlist_del_entry(node, type, member) \
	ebr_dlist_delete(&(node)->member)

/* replace a struct entry */
#define ebr_dlist_replace_entry(old_node, new_node, type, member) \
	ebr_dlist_replace(&(old_node)->member, &(new_node)->member)

#endif
