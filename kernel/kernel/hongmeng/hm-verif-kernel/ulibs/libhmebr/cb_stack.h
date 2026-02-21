/*
 * Copyright (C) Huawei Technologies Co., Ltd. 2020. All rights reserved.
 * Description: Declarations for ebr lock-free stack
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 29 14:05:30 2020
 */
#ifndef ULIBS_LIBHMEBR_CB_STACK_H
#define ULIBS_LIBHMEBR_CB_STACK_H

#include <hmkernel/barrier.h>
#include <vsync/atomic.h>
#include <libhmebr/ebr.h>

static inline struct cb_stack_entry *cb_stack_first(struct cb_stack *stack)
{
	return stack->head;
}

static inline struct cb_stack_entry *cb_stack_next(struct cb_stack_entry *entry)
{
	return (entry == NULL) ? NULL : entry->next;
}

#define cb_stack_for_each_continue_safe(entry, n, start)	\
	for ((entry) = (start), (n) = cb_stack_next(entry);	\
	     ((entry) != NULL);					\
	     (entry) = (n), (n) = cb_stack_next(entry))

static inline void cb_stack_init(struct cb_stack *stack)
{
	__write_once(stack->head, NULL);
}

static inline void cb_stack_push(struct cb_stack *stack, struct cb_stack_entry *entry)
{
	struct cb_stack_entry *next_entry = NULL;

	do {
		next_entry = __read_once(stack->head);
		entry->next = next_entry;
		smp_wmb();
	} while (vatomicptr_cmpxchg_rlx((vatomicptr_t *)(uintptr_t)&stack->head,
				       next_entry, entry) != next_entry);
}

static inline struct cb_stack_entry *cb_stack_pop_all(struct cb_stack *stack)
{
	return (void *)vatomicptr_xchg_rlx((vatomicptr_t *)(uintptr_t)&stack->head, NULL);
}
#endif
