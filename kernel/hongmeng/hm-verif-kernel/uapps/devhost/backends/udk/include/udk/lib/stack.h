/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
 * Description: Stack implemention for udk
 * Author: Huawei OS Kernel Lab
 * Create: Wed Jul 14 14:40:49 2021
 */

#ifndef __UDK_LIB_STACK_H__
#define __UDK_LIB_STACK_H__

#include <udk/types.h>

struct udk_stack;

struct udk_stack *udk_stack_create(void);
void udk_stack_destroy(struct udk_stack *stack);
bool udk_stack_empty(const struct udk_stack *stack);
int udk_stack_push(struct udk_stack *stack, void *data);
void *udk_stack_pop(const struct udk_stack *stack);
void *udk_stack_top(const struct udk_stack *stack);

#define udk_stack_for_each_entry_pop(top, stack) \
	for ((top) = udk_stack_pop(stack); \
	     (top) != NULL; \
	     (top) = udk_stack_pop(stack))

#endif /* __UDK_LIB_STACK_H__ */
