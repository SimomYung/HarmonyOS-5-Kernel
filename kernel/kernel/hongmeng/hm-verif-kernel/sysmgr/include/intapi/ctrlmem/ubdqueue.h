/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Introduce unbounded queue
 * Author: Huawei OS Kernel Lab
 * Create: Thu May 08 11:31:05 2023
 */
#ifndef INTAPI_CTRLMEM_UBDQUEUE_H
#define INTAPI_CTRLMEM_UBDQUEUE_H

#include <hongmeng/compiler.h>
#include <hmkernel/ctrlmem/ubdqueue.h>
#ifndef CONFIG_CTRLMEM_PAGEFAULT_COCALL
#include <vsync/atomic.h>
#endif

extern void ctrlmem_ubdq_enqueue(struct __ctrlmem_ubdq_s *pool, struct __ctrlmem_ubdq_node_s *node);

extern struct __ctrlmem_ubdq_node_s *ctrlmem_ubdq_dequeue(struct __ctrlmem_ubdq_s *pool);

extern void ctrlmem_ubdq_init(struct __ctrlmem_ubdq_s *pool);

extern void ctrlmem_ubdq_node_delete_unsafe(struct __ctrlmem_ubdq_s *pool, struct __ctrlmem_ubdq_node_s *node);

#ifdef CONFIG_CTRLMEM_PAGEFAULT_COCALL
static inline unsigned int ctrlmem_ubdq_count_of(struct __ctrlmem_ubdq_s *pool)
{
	return pool->count;
}
#else /* !CONFIG_CTRLMEM_PAGEFAULT_COCALL */
static inline unsigned int ctrlmem_ubdq_count_of(struct __ctrlmem_ubdq_s *pool)
{
	return vatomic32_read_rlx(&pool->count);
}
#endif /* CONFIG_CTRLMEM_PAGEFAULT_COCALL */

#endif
