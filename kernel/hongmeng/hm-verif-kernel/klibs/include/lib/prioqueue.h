/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description: prioqueue function-like macro
 * Author: Huawei OS Kernel Lab
 * Create: Fri Nov 9 14:37:05 2018
 */
#ifndef KLIBS_PRIOQUEUE_H
#define KLIBS_PRIOQUEUE_H

#include <lib/dlist.h>
#include <hmkernel/compiler.h>
#include <hmkernel/bitops/bitops.h>
#include <hmkernel/rq.h>

#define __DECLARE_PRIOQUEUE_NAME(name, len)	\
struct name##_prioqueue {			\
	struct dlist_node queues[len];		\
	DEFINE_BITMAP(bitmap, len);		\
};

#define __DECLARE_PRIOQUEUE_OPS_INIT(name, len)						\
static void __used name##_prioqueue_init(struct name##_prioqueue *map) 			\
{											\
	unsigned int i;									\
											\
	i = 0U;										\
	while (i < (len)) {								\
		struct dlist_node *tmp;							\
		tmp = &((map->queues)[i]);						\
		dlist_init(tmp);							\
		i = i + 1U;								\
	}										\
	bitmap_clear_all(map->bitmap, len);						\
	return;										\
}

#define __DECLARE_PRIOQUEUE_OPS_ENQUEUE(name)						\
static void __used									\
name##_prioqueue_enqueue(struct name##_prioqueue *map, unsigned int prio,		\
			 struct dlist_node *link)					\
{											\
	dlist_insert_head(&((map->queues)[prio]), link);				\
	__set_bit(map->bitmap, prio);							\
	return;										\
}

#define __DECLARE_PRIOQUEUE_OPS_APPEND(name)						\
static void __used									\
name##_prioqueue_append(struct name##_prioqueue *map, unsigned int prio,		\
			struct dlist_node *link)					\
{											\
	dlist_insert_tail(&((map->queues)[prio]), link);				\
	__set_bit(map->bitmap, prio);							\
	return;										\
}

#define __DECLARE_PRIOQUEUE_OPS_DEQUEUE(name)						\
static void __used									\
name##_prioqueue_dequeue(struct name##_prioqueue *map, unsigned int prio,		\
			 struct dlist_node *link)					\
{											\
	bool is_empty = true;								\
											\
	dlist_delete(link);								\
	is_empty = dlist_empty(&((map->queues)[prio]));					\
	if (is_empty) {									\
		__clear_bit(map->bitmap, prio);						\
	}										\
	return;										\
}

/*
 * As prioqueue_get_highest can't get a node whose prio is less than
 * HM_RUNQUEUE_IDLE, this interface is not very general, so introduce
 * an interface that can get a highest prio node whose prio >= the assigned
 * lowest prio.
 *
 * get the highest prio node whose prio is >= lowest_prio
 */
#define __DECLARE_PRIOQUEUE_OPS_GET_HIGHEST_GE(name, len)				\
static inline struct dlist_node * __used						\
__##name##_prioqueue_get_highest_ge(struct name##_prioqueue *map,			\
				    unsigned int *p_prio,				\
				    unsigned int lowest_prio)				\
{											\
	struct dlist_node *ret = NULL;							\
	unsigned int highest_prio;							\
											\
	highest_prio = bitmap_find_first_one_msb(map->bitmap, len);			\
	if ((highest_prio < len) && (highest_prio >= lowest_prio)) {			\
		ret = dlist_get_first(&(map->queues)[highest_prio]);			\
		if (p_prio != NULL) {							\
			*p_prio = highest_prio;						\
		}									\
	}										\
	return ret;									\
}

/*
 * highest_prio < 0 means error, less than PRIO of IDLE means
 * idle in block state. Report NULL in both cases.
 */
#define __DECLARE_PRIOQUEUE_OPS_GET_HIGHEST(name)					\
static struct dlist_node * __used							\
name##_prioqueue_get_highest(struct name##_prioqueue *map, unsigned int *p_prio)	\
{											\
	return __##name##_prioqueue_get_highest_ge(map, p_prio, HM_RUNQUEUE_IDLE);	\
}

#define __DECLARE_PRIOQUEUE_OPS_GET_NEXT_HIGHEST(name, len)				\
static struct dlist_node * __used							\
name##_prioqueue_get_next_highest(struct name##_prioqueue *map, unsigned int prio,	\
				  struct dlist_node *node, unsigned int *p_prio)	\
{											\
	struct dlist_node *ret = NULL;							\
											\
	ret = dlist_get_next(node);							\
	if (ret == &(map->queues[prio])) {						\
		unsigned int highest_prio;						\
		highest_prio = bitmap_find_next_one_msb(map->bitmap, len, prio);	\
		if (highest_prio >= len) {						\
			ret = NULL;							\
		} else {								\
			ret = dlist_get_first(&(map->queues)[highest_prio]);		\
											\
			if (p_prio != NULL) {						\
				*p_prio = highest_prio;					\
			}								\
		}									\
	} else {									\
		if (p_prio != NULL) {							\
			*p_prio = prio;							\
		}									\
	}										\
											\
	return ret;									\
}

#define DECLARE_PRIOQUEUE_STURCT(name, len)			\
	__DECLARE_PRIOQUEUE_NAME(name, len)

#define DECLARE_PRIOQUEUE_OPS(name, len) \
	__DECLARE_PRIOQUEUE_OPS_INIT(name, len)			\
	__DECLARE_PRIOQUEUE_OPS_ENQUEUE(name)			\
	__DECLARE_PRIOQUEUE_OPS_APPEND(name)			\
	__DECLARE_PRIOQUEUE_OPS_DEQUEUE(name)			\
	__DECLARE_PRIOQUEUE_OPS_GET_HIGHEST_GE(name, len)	\
	__DECLARE_PRIOQUEUE_OPS_GET_HIGHEST(name)		\
	__DECLARE_PRIOQUEUE_OPS_GET_NEXT_HIGHEST(name, len)

#define DECLARE_PRIOQUEUE(name, len)				\
	DECLARE_PRIOQUEUE_STURCT(name, len)			\
	DECLARE_PRIOQUEUE_OPS(name, len)

#endif
