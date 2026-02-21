/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Author: Huawei Dresden Research Center
 */
#ifndef VGDUMP_LIST_H
#define VGDUMP_LIST_H

/*******************************************************************************
 * @brief A linked-list for registered threads of GDump
 *
 * The list supports parallel insertions, but allows only one remove operation
 * at a time. Hence, threads running in parallel must call `gdump_list_rem`
 * in a mutually exclusive way (e.g. inside the CS of a lock). `gdump_list_add`,
 * on the other hand can be invoked by parallel threads safely. A remove
 * operation can run safely in parallel with insert operations.
 *
 ******************************************************************************/
#include <vsync/common/assert.h>
#include <vsync/common/dbg.h>
#include <vsync/atomic.h>
#include <vsync/common/verify.h>

typedef struct gdump_list_node_s {
	vatomicptr_t next;
} gdump_list_node_t;

typedef struct gdump_list_s {
	gdump_list_node_t head;
} gdump_list_t;

typedef struct gdump_list_iter_s {
	gdump_list_node_t *curr;
} gdump_list_iter_t;

#define GDUMP_LIST_INIT()                             \
	{                                             \
		.head = {.next = VATOMIC_INIT(NULL) } \
	}

static inline gdump_list_node_t *gdump_list_get_head(gdump_list_t *list);

/**
 * Initializes the iterator to start from the head of the list.
 *
 * @param iter address of gdump_list_iter_t object.
 * @param lst address of gdump_list_t object, to start the iterator on its
 * heads.
 */
static inline void gdump_list_iter_init(gdump_list_iter_t *iter,
					gdump_list_t *lst)
{
	ASSERT(iter);
	ASSERT(lst);
	iter->curr = vatomicptr_read(&lst->head.next);
}
/**
 * Initializes the iterator to start from the given node.
 *
 * @param iter address of gdump_list_iter_t object.
 * @param node address of gdump_list_node_t object, to start the iterator on.
 */
static inline void gdump_list_iter_init_from(gdump_list_iter_t *iter,
					     gdump_list_node_t *node)
{
	ASSERT(iter);
	iter->curr = node;
}
/**
 * Returns the current node and advances the iterator.
 *
 * @param iter address of gdump_list_iter_t object.
 * @return gdump_list_node_t* address of the current node object.
 * @return NULL there are no more nodes.
 */
static inline gdump_list_node_t *gdump_list_iter_next(gdump_list_iter_t *iter)
{
	gdump_list_node_t *curr = iter->curr;
	if (curr) {
		iter->curr = vatomicptr_read(&curr->next);
	}
	return curr;
}
/**
 * Initializes the given list.
 *
 * @param list address of gdump_list_t object.
 *
 * @note call before threads' creation.
 */
static inline void gdump_list_init(gdump_list_t *list)
{
	ASSERT(list);
	vatomicptr_init(&list->head.next, NULL);
}
/**
 * Inserts `node` into `list`.
 *
 * @param list address of gdump_list_t object.
 * @param node address of gdump_list_node_t object.
 *
 * @note Call outside the CS. It is a lock-free safe API.
 */
static inline void gdump_list_add(gdump_list_t *list, gdump_list_node_t *node)
{
	gdump_list_node_t *next = NULL;
	gdump_list_node_t *head = NULL;

	head = vatomicptr_read(&list->head.next);
	do {
		next = head;
		vatomicptr_write(&node->next, next);
		head = vatomicptr_cmpxchg(&list->head.next, next, node);
		verification_assume(head == next);
	} while (head != next);
}
/**
 * Removes `node` from `list`.
 *
 * @param list address of gdump_list_t object.
 * @param node address of gdump_list_node_t object.
 *
 * @note Must be called within a lock CS.
 */
static inline void gdump_list_rem(gdump_list_t *list, gdump_list_node_t *node)
{
	gdump_list_node_t *pred = NULL;
	gdump_list_node_t *curr = NULL;
	gdump_list_node_t *succ = NULL;
	vbool_t disconnected = false;

	ASSERT(list);
	ASSERT(node);

	while (!disconnected) {
		pred = &list->head;
		curr = vatomicptr_read(&pred->next);

		/* look for node */
		while (curr && curr != node) {
			pred = curr;
			curr = vatomicptr_read(&curr->next);
		}
		if (curr) {
			ASSERT(curr == node);
			succ = vatomicptr_read(&curr->next);
			disconnected = vatomicptr_cmpxchg(&pred->next, curr,
							  succ) == curr;
			verification_assume(disconnected);
		} else {
			disconnected = true;
		}
	}
}
/**
 * Returns the actual head of the list.
 *
 * @param list address of gdump_list_t object.
 * @return gdump_list_node_t* address of the head object. That is not the
 * sentinel.
 */
static inline gdump_list_node_t *gdump_list_get_head(gdump_list_t *list)
{
	return vatomicptr_read(&list->head.next);
}
/**
 * Returns the successor of the given node.
 *
 * @param node address of gdump_list_node_t object.
 * @return gdump_list_node_t* address of the successor object of `node`.
 */
static inline gdump_list_node_t *gdump_list_get_next(gdump_list_node_t *node)
{
	return vatomicptr_read(&node->next);
}
/**
 * Returns the count of nodes reachable from `head`.
 *
 * @param head address of gdump_list_node_t object.
 * @return vsize_t count of nodes.
 *
 * @note Must be called within a lock CS.
 */
static inline vsize_t gdump_list_count(gdump_list_node_t *count_from)
{
	gdump_list_iter_t iter;
	vsize_t count = 0;

	gdump_list_iter_init_from(&iter, count_from);

	while (gdump_list_iter_next(&iter)) {
		count++;
	}
	return count;
}
/**
 * Returns the length of `list`.
 *
 * @param list address of gdump_list_t object.
 * @return vsize_t the length of `list`.
 *
 * @note Must be called within a lock CS.
 */
static inline vsize_t gdump_list_get_length(gdump_list_t *list)
{
	return gdump_list_count(gdump_list_get_head(list));
}
#endif
