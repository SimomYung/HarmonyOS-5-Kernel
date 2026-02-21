/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023. All rights reserved.
 * Description: Define the head of ubdqueue - uapi
 * Author: Huawei OS Kernel Lab
 * Create: Tue Apr 25 2023
 */

#ifndef MAPI_UAPI_HMKERNEL_CTRLMEM_UBDQUEUE_H
#define MAPI_UAPI_HMKERNEL_CTRLMEM_UBDQUEUE_H

#include <hmkernel/errno.h>

/*
 * Give userspace a way to inject its own atomic type.
 * See comments in kernel/include/mapi/uapi/hmkernel/ctrlmem/kobjs/vregion.h
 */
#ifndef __uatomic32
# ifndef __KERNEL__
struct __ctrlmem_ubdq_uatomic32_s {
	__u32 donttouch;
};
typedef struct __ctrlmem_ubdq_uatomic32_s __ctrlmem_ubdq_uatomic32_t;
#  define __uatomic32 __ctrlmem_ubdq_uatomic32_t
# else
#  include <lib/atomic.h>
#  define __uatomic32 vatomic32_t
# endif
#endif

#ifndef __uatomicptr
# ifndef __KERNEL__
struct __ctrlmem_ubdq_uatomicptr_s {
	__uptr_t donttouch;
};
typedef struct __ctrlmem_ubdq_uatomicptr_s __ctrlmem_ubdq_uatomicptr_t;
#  define __uatomicptr __ctrlmem_ubdq_uatomicptr_t
# else
#  include <lib/atomic.h>
#  define __uatomicptr vatomicptr_t
# endif
#endif

#ifdef CONFIG_CTRLMEM_PAGEFAULT_COCALL
/*
 * The unbounded queue node structure
 * Store the va of previous and next node
 */
struct __ctrlmem_ubdq_node_s {
	struct __ctrlmem_ubdq_node_s *prev;
	struct __ctrlmem_ubdq_node_s *next;
};

struct __ctrlmem_ubdq_s {
	struct __ctrlmem_ubdq_node_s head;      /* the head ptr of ubdqueue */
	unsigned int count;                     /* the real num of ubdqueue nodes */
};
#else
/*
 * The unbounded queue node structure
 * Store the va of next node
 */
struct __ctrlmem_ubdq_node_s {
	struct __ctrlmem_ubdq_node_s *next;
};

struct __ctrlmem_ubdq_s {
	__uatomicptr head;            /* the head ptr of ubdqueue */
	__uatomic32 count;            /* only a hint, not the real num */
};
#endif

#endif
