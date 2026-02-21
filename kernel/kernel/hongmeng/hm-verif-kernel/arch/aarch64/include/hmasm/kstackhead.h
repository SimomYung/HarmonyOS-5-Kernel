/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024. All rights reserved.
 * Description: define kstack head, which is the start kernel address of current
 *              kernel call, in case when kernel exceptions are allowed to recursive.
 * Author: Huawei OS Kernel Lab
 * Create: Sun Sep 22 21:33:51 2024
 */

#ifndef A64_HMASM_KSTACKHEAD_H
#define A64_HMASM_KSTACKHEAD_H

#ifndef __ASSEMBLY__

#include <hmasm/registers.h>

/*
 * Each time we trap in el1_xxx we save previous stack head
 * here to support recursive el1 exception, By this way we
 * can use unified kernel stack.
 *
 * Use the word 'kstack_head' because the concepts of stack-top
 * and stack-bottom have some misunderstanding. See 'stack_area_check()'.
 * stack_head is a new concept which is defined by us.
 */
struct arch_kstackhead_s {
	const struct arch_kstackhead_s *prev;
	__u64 padding;
	struct arch_regs regs;
};

#endif

#define KSTACKHEAD_OFFSET_PREV	0
#define KSTACKHEAD_OFFSET_REGS	16

#endif
