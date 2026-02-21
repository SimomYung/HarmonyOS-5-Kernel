/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Define interface of udk_stack
 * Author: Huawei OS Kernel Lab
 * Create: Mon Sep 06 11:00:22 2021
 */

#include <udk/log.h>
#include <udk/types.h>
#include <udk/iolib.h>
#include <udk/errno.h>
#include <udk/securec.h>
#include <udk/lib/dlist.h>
#include <udk/lib/stack.h>

struct udk_stack {
	/* top pointer to the head of stack */
	struct dlist_node top;
};

struct udk_stack_node {
	void *data;
	struct dlist_node node;
};

struct udk_stack *udk_stack_create(void)
{
	struct udk_stack *stack = NULL;

	stack = udk_malloc(sizeof(*stack));
	if (stack == NULL) {
		udk_error("alloc stack failed\n");
		return NULL;
	}
	mem_zero_s(*stack);
	dlist_init(&stack->top);

	return stack;
}

void udk_stack_destroy(struct udk_stack *stack)
{
	struct udk_stack_node *pos = NULL;
	struct udk_stack_node *n = NULL;

	if (stack == NULL) {
		return;
	}
	dlist_for_each_entry_safe(pos, n, &stack->top,
				  struct udk_stack_node, node) {
		dlist_delete(&pos->node);
		udk_free(pos);
	}
	udk_free(stack);
}

bool udk_stack_empty(const struct udk_stack *stack)
{
	if (stack == NULL) {
		return true;
	}

	return dlist_empty(&stack->top);
}

int udk_stack_push(struct udk_stack *stack, void *data)
{
	struct udk_stack_node *node = NULL;

	if (stack == NULL) {
		return -EINVAL;
	}
	node = udk_malloc(sizeof(*node));
	if (node == NULL) {
		udk_error("udk_stack_node malloc failed\n");
		return -ENOMEM;
	}
	node->data = data;
	dlist_insert_head(&stack->top, &node->node);

	return 0;
}

void *udk_stack_top(const struct udk_stack *stack)
{
	struct udk_stack_node *p = NULL;

	if (stack == NULL || udk_stack_empty(stack)) {
		return NULL;
	}
	p = dlist_first_entry(&stack->top, struct udk_stack_node, node);

	return p->data;
}

void *udk_stack_pop(const struct udk_stack *stack)
{
	struct udk_stack_node *p = NULL;
	void *data = NULL;

	if (stack == NULL || udk_stack_empty(stack)) {
		return NULL;
	}
	p = dlist_first_entry(&stack->top, struct udk_stack_node, node);
	data = p->data;
	dlist_delete(&p->node);
	udk_free(p);

	return data;
}
